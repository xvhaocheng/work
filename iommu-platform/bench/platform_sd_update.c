/*
 * platform-sd-update - replace the 32 MiB boot partition payload.
 *
 * Safety properties enforced by this tool:
 *   - the target must be a plain /dev/mmcblk<number>p1 block device with no
 *     symlink anywhere in its path, and the process must run as root;
 *   - the partition must report exactly 32 MiB through BLKGETSIZE64 and the
 *     matching sysfs attributes (partition 1, start 2048, size 65536); the
 *     target, parent and image are opened with O_NOFOLLOW, both block devices
 *     must report a 512 byte logical sector size, and the sysfs dev attributes
 *     must match the opened device numbers and the real sysfs parent;
 *   - the GPT in the parent disk is validated through a read-only descriptor:
 *     signature, revision, header size, header CRC32, entry array CRC32, the
 *     boot partition type GUID, its unique GUID, the first and last LBA, the
 *     usable LBA range and backup header position, the overflow checked entry
 *     array bounds, and that no other partition overlaps the boot partition;
 *   - a mounted target or a second updater holding the parent lock is refused;
 *   - the image is loaded into a calloc'ed 32 MiB buffer before any writable
 *     descriptor exists, so a changing or short source file can never leak
 *     stale bytes into the partition; the remaining bytes are written as zero;
 *   - only the partition descriptor is ever opened for writing, always at
 *     offset 0 for exactly 32 MiB; the parent descriptor stays read-only;
 *   - write and fsync errors return non-zero without printing success, and the
 *     tool never reboots the board.
 */
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef BLKGETSIZE64
#define BLKGETSIZE64 _IOR(0x12, 114, size_t)
#endif
#ifndef BLKSSZGET
#define BLKSSZGET _IO(0x12, 104)
#endif

#define PATH_BUF 4096
#define NAME_BUF 256

#define SECTOR_SIZE 512ULL
#define PARTITION_START_LBA 2048ULL
#define PARTITION_SECTORS 65536ULL
#define PARTITION_SIZE (PARTITION_SECTORS * SECTOR_SIZE)
#define GPT_HEADER_BYTES 512
#define GPT_MAX_ENTRIES 256u
#define GPT_MAX_ENTRY_BYTES (1024u * 1024u)

static const char *const PROGRAM = "platform-sd-update";
static const char *const BOOT_TYPE_GUID = "7412F7D5-A156-4B13-81DC-867174929325";
static const char *const BOOT_UNIQUE_GUID = "46298C61-BAC2-44C9-A8FF-8FA61CF2BEE0";

static void report(const char *format, ...)
{
    va_list arguments;
    fprintf(stderr, "%s: error: ", PROGRAM);
    va_start(arguments, format);
    vfprintf(stderr, format, arguments);
    va_end(arguments);
    fputc('\n', stderr);
}

static void usage(FILE *stream)
{
    fprintf(stream,
            "usage: %s --device /dev/mmcblk0p1 --image FILE [--dry-run]\n"
            "\n"
            "Validates the GPT boot partition layout and rewrites the whole 32 MiB\n"
            "partition payload. The parent disk is only ever opened read-only.\n",
            PROGRAM);
#ifdef PLATFORM_SD_UPDATE_TEST
    fprintf(stream,
            "\n"
            "test-only options (absent from production builds):\n"
            "  --test-disk FILE   treat FILE as a whole-disk image whose boot\n"
            "                     partition starts at LBA %llu\n"
            "  --test-fail-fsync  force the final fsync to fail\n",
            (unsigned long long)PARTITION_START_LBA);
#endif
}

static int pread_full(int descriptor, void *buffer, size_t length, off_t offset)
{
    uint8_t *cursor = buffer;
    size_t done = 0;
    while (done < length) {
        ssize_t got = pread(descriptor, cursor + done, length - done, offset + (off_t)done);
        if (got < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        if (got == 0) {
            errno = EIO;
            return -1;
        }
        done += (size_t)got;
    }
    return 0;
}

static int pwrite_full(int descriptor, const void *buffer, size_t length, off_t offset)
{
    const uint8_t *cursor = buffer;
    size_t done = 0;
    while (done < length) {
        ssize_t put = pwrite(descriptor, cursor + done, length - done, offset + (off_t)done);
        if (put < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        if (put == 0) {
            errno = EIO;
            return -1;
        }
        done += (size_t)put;
    }
    return 0;
}

static int mul_u64_checked(uint64_t left, uint64_t right, uint64_t *result)
{
    if (left != 0 && right > UINT64_MAX / left) {
        return -1;
    }
    *result = left * right;
    return 0;
}

static int add_u64_checked(uint64_t left, uint64_t right, uint64_t *result)
{
    if (right > UINT64_MAX - left) {
        return -1;
    }
    *result = left + right;
    return 0;
}

static uint32_t crc32_table[256];
static int crc32_ready = 0;

static void crc32_build_table(void)
{
    uint32_t index;
    if (crc32_ready) {
        return;
    }
    for (index = 0; index < 256u; index++) {
        uint32_t value = index;
        int bit;
        for (bit = 0; bit < 8; bit++) {
            value = (value & 1u) ? (0xEDB88320u ^ (value >> 1)) : (value >> 1);
        }
        crc32_table[index] = value;
    }
    crc32_ready = 1;
}

static uint32_t crc32_bytes(const void *data, size_t length)
{
    const uint8_t *cursor = data;
    uint32_t crc = 0xFFFFFFFFu;
    size_t index;
    crc32_build_table();
    for (index = 0; index < length; index++) {
        crc = crc32_table[(crc ^ cursor[index]) & 0xFFu] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFu;
}

static uint32_t read_le32(const uint8_t *data)
{
    return (uint32_t)data[0] | ((uint32_t)data[1] << 8) |
           ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
}

static uint64_t read_le64(const uint8_t *data)
{
    return (uint64_t)read_le32(data) | ((uint64_t)read_le32(data + 4) << 32);
}

static int hex_value(int character)
{
    if (character >= '0' && character <= '9') {
        return character - '0';
    }
    if (character >= 'a' && character <= 'f') {
        return character - 'a' + 10;
    }
    if (character >= 'A' && character <= 'F') {
        return character - 'A' + 10;
    }
    return -1;
}

static int parse_guid(const char *text, uint8_t guid[16])
{
    static const size_t dash_positions[4] = {8, 13, 18, 23};
    uint8_t canonical[16];
    size_t index;
    size_t filled = 0;
    if (strlen(text) != 36) {
        return -1;
    }
    for (index = 0; index < 4; index++) {
        if (text[dash_positions[index]] != '-') {
            return -1;
        }
    }
    for (index = 0; index < 36; index++) {
        int high;
        int low;
        if (text[index] == '-') {
            continue;
        }
        high = hex_value((unsigned char)text[index]);
        index++;
        low = hex_value((unsigned char)text[index]);
        if (high < 0 || low < 0 || filled >= 16) {
            return -1;
        }
        canonical[filled++] = (uint8_t)((high << 4) | low);
    }
    if (filled != 16) {
        return -1;
    }
    guid[0] = canonical[3];
    guid[1] = canonical[2];
    guid[2] = canonical[1];
    guid[3] = canonical[0];
    guid[4] = canonical[5];
    guid[5] = canonical[4];
    guid[6] = canonical[7];
    guid[7] = canonical[6];
    for (index = 8; index < 16; index++) {
        guid[index] = canonical[index];
    }
    return 0;
}

static void format_guid(const uint8_t guid[16], char text[37])
{
    snprintf(text, 37,
             "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
             guid[3], guid[2], guid[1], guid[0], guid[5], guid[4], guid[7], guid[6],
             guid[8], guid[9], guid[10], guid[11], guid[12], guid[13], guid[14], guid[15]);
}

static int split_dir_base(const char *path, char *dir, size_t dir_size, char *base, size_t base_size)
{
    const char *slash = strrchr(path, '/');
    const char *leaf = slash ? slash + 1 : path;
    size_t leaf_length = strlen(leaf);
    if (leaf_length == 0 || leaf_length + 1 > base_size) {
        return -1;
    }
    memcpy(base, leaf, leaf_length + 1);
    if (slash) {
        size_t dir_length = (size_t)(slash - path);
        if (dir_length == 0) {
            dir_length = 1;
        }
        if (dir_length + 1 > dir_size) {
            return -1;
        }
        memcpy(dir, path, dir_length);
        dir[dir_length] = '\0';
    } else {
        if (dir_size < 2) {
            return -1;
        }
        dir[0] = '.';
        dir[1] = '\0';
    }
    return 0;
}

static void join_path(char *out, size_t out_size, const char *dir, const char *base)
{
    if (strcmp(dir, "/") == 0) {
        snprintf(out, out_size, "/%s", base);
    } else {
        snprintf(out, out_size, "%s/%s", dir, base);
    }
}

static int path_has_symlink(const char *path)
{
    char buffer[PATH_BUF];
    size_t length = strlen(path);
    size_t index;
    struct stat status;
    if (length == 0 || length >= sizeof(buffer)) {
        return -1;
    }
    memcpy(buffer, path, length + 1);
    for (index = 1; index <= length; index++) {
        char saved;
        int result;
        if (index < length && buffer[index] != '/') {
            continue;
        }
        saved = buffer[index];
        buffer[index] = '\0';
        result = lstat(buffer, &status);
        buffer[index] = saved;
        if (result == 0 && S_ISLNK(status.st_mode)) {
            return 1;
        }
    }
    return 0;
}

static int parse_boot_partition_name(const char *base, char *parent, size_t parent_size)
{
    const char *cursor = base;
    const char *digits;
    size_t prefix;
    if (strncmp(cursor, "mmcblk", 6) != 0) {
        return -1;
    }
    cursor += 6;
    digits = cursor;
    while (*cursor >= '0' && *cursor <= '9') {
        cursor++;
    }
    if (cursor == digits) {
        return -1;
    }
    if (cursor[0] != 'p' || cursor[1] != '1' || cursor[2] != '\0') {
        return -1;
    }
    prefix = (size_t)(cursor - base);
    if (prefix + 1 > parent_size) {
        return -1;
    }
    memcpy(parent, base, prefix);
    parent[prefix] = '\0';
    return 0;
}

static int read_sysfs_value(const char *path, char *value, size_t value_size)
{
    int descriptor = open(path, O_RDONLY | O_CLOEXEC);
    ssize_t got;
    int saved;
    if (descriptor < 0) {
        return -1;
    }
    got = read(descriptor, value, value_size - 1);
    saved = errno;
    close(descriptor);
    if (got < 0) {
        errno = saved;
        return -1;
    }
    value[got] = '\0';
    while (got > 0 && (value[got - 1] == '\n' || value[got - 1] == '\r' ||
                       value[got - 1] == ' ' || value[got - 1] == '\t')) {
        value[--got] = '\0';
    }
    return 0;
}

static int block_logical_sector_size(int descriptor, const char *description, uint64_t *size)
{
    int sector = 0;
    if (ioctl(descriptor, BLKSSZGET, &sector) != 0) {
        report("BLKSSZGET failed on %s: %s", description, strerror(errno));
        return -1;
    }
    if (sector <= 0) {
        report("%s reports an invalid logical sector size %d", description, sector);
        return -1;
    }
    *size = (uint64_t)sector;
    return 0;
}

static int sysfs_says(const char *name, const char *attribute, const char *expected)
{
    char path[PATH_BUF];
    char value[128];
    snprintf(path, sizeof(path), "/sys/class/block/%s/%s", name, attribute);
    if (read_sysfs_value(path, value, sizeof(value)) != 0) {
        report("cannot read %s: %s", path, strerror(errno));
        return -1;
    }
    if (strcmp(value, expected) != 0) {
        report("sysfs reports %s %s, expected %s", path, value, expected);
        return -1;
    }
    return 0;
}

static int sysfs_device_matches(const char *name, dev_t device)
{
    char path[PATH_BUF];
    char value[128];
    unsigned int field_major = 0;
    unsigned int field_minor = 0;
    snprintf(path, sizeof(path), "/sys/class/block/%s/dev", name);
    if (read_sysfs_value(path, value, sizeof(value)) != 0) {
        report("cannot read %s: %s", path, strerror(errno));
        return -1;
    }
    if (sscanf(value, "%u:%u", &field_major, &field_minor) != 2) {
        report("sysfs reports %s %s, expected a MAJ:MIN pair", path, value);
        return -1;
    }
    if (field_major != (unsigned int)major(device) ||
        field_minor != (unsigned int)minor(device)) {
        report("sysfs %s reports %u:%u, expected %u:%u", path, field_major, field_minor,
               (unsigned int)major(device), (unsigned int)minor(device));
        return -1;
    }
    return 0;
}

static int sysfs_real_parent(const char *name, char *parent, size_t parent_size)
{
    char path[PATH_BUF];
    char resolved[PATH_BUF];
    ssize_t length;
    char *slash;
    snprintf(path, sizeof(path), "/sys/class/block/%s", name);
    length = readlink(path, resolved, sizeof(resolved) - 1);
    if (length < 0) {
        report("cannot resolve %s: %s", path, strerror(errno));
        return -1;
    }
    resolved[length] = '\0';
    slash = strrchr(resolved, '/');
    if (slash == NULL || slash == resolved) {
        report("unexpected sysfs layout for %s", name);
        return -1;
    }
    *slash = '\0';
    slash = strrchr(resolved, '/');
    if (slash == NULL || slash[1] == '\0') {
        report("unexpected sysfs layout for %s", name);
        return -1;
    }
    if (strlen(slash + 1) + 1 > parent_size) {
        report("sysfs parent name for %s is too long", name);
        return -1;
    }
    snprintf(parent, parent_size, "%s", slash + 1);
    return 0;
}

static int verify_sysfs_layout(const char *partition_name, const char *parent_name,
                               dev_t partition_device, dev_t parent_device)
{
    char expected_start[32];
    char expected_size[32];
    char path[PATH_BUF];
    char value[128];
    char resolved_parent[NAME_BUF];
    unsigned long long parent_sectors;
    snprintf(expected_start, sizeof(expected_start), "%llu", (unsigned long long)PARTITION_START_LBA);
    snprintf(expected_size, sizeof(expected_size), "%llu", (unsigned long long)PARTITION_SECTORS);
    if (sysfs_device_matches(partition_name, partition_device) != 0) {
        return -1;
    }
    if (sysfs_device_matches(parent_name, parent_device) != 0) {
        return -1;
    }
    if (sysfs_real_parent(partition_name, resolved_parent, sizeof(resolved_parent)) != 0) {
        return -1;
    }
    if (strcmp(resolved_parent, parent_name) != 0) {
        report("sysfs reports %s under parent %s, expected %s",
               partition_name, resolved_parent, parent_name);
        return -1;
    }
    if (sysfs_says(partition_name, "partition", "1") != 0) {
        return -1;
    }
    if (sysfs_says(partition_name, "start", expected_start) != 0) {
        return -1;
    }
    if (sysfs_says(partition_name, "size", expected_size) != 0) {
        return -1;
    }
    snprintf(path, sizeof(path), "/sys/class/block/%s/size", parent_name);
    if (read_sysfs_value(path, value, sizeof(value)) != 0) {
        report("cannot read %s: %s", path, strerror(errno));
        return -1;
    }
    errno = 0;
    parent_sectors = strtoull(value, NULL, 10);
    if (errno != 0 || parent_sectors < PARTITION_START_LBA + PARTITION_SECTORS) {
        report("sysfs reports parent %s size %s sectors, too small for the boot partition",
               parent_name, value);
        return -1;
    }
    return 0;
}

static int device_is_mounted(dev_t device, const char *path)
{
    FILE *stream = fopen("/proc/self/mountinfo", "r");
    char *line = NULL;
    size_t capacity = 0;
    unsigned int want_major = (unsigned int)major(device);
    unsigned int want_minor = (unsigned int)minor(device);
    int mounted = 0;
    if (stream == NULL) {
        return -1;
    }
    while (getline(&line, &capacity, stream) != -1) {
        char *token[64];
        int count = 0;
        char *cursor = line;
        int index;
        unsigned int field_major = 0;
        unsigned int field_minor = 0;
        while (count < 64) {
            while (*cursor == ' ') {
                cursor++;
            }
            if (*cursor == '\0' || *cursor == '\n') {
                break;
            }
            token[count++] = cursor;
            while (*cursor != '\0' && *cursor != ' ' && *cursor != '\n') {
                cursor++;
            }
            if (*cursor != '\0') {
                *cursor = '\0';
                cursor++;
            }
        }
        if (count < 6) {
            continue;
        }
        if (sscanf(token[2], "%u:%u", &field_major, &field_minor) == 2 &&
            field_major == want_major && field_minor == want_minor) {
            mounted = 1;
            break;
        }
        for (index = 4; index < count; index++) {
            if (strcmp(token[index], path) == 0) {
                mounted = 1;
                break;
            }
        }
        if (mounted) {
            break;
        }
    }
    free(line);
    fclose(stream);
    return mounted;
}

struct boot_partition {
    uint64_t start_lba;
    uint64_t sectors;
};

static int validate_gpt(int parent, uint64_t parent_capacity, struct boot_partition *boot)
{
    uint8_t header[GPT_HEADER_BYTES];
    uint8_t expected_type[16];
    uint8_t expected_unique[16];
    uint8_t *entries = NULL;
    uint32_t revision;
    uint32_t header_size;
    uint32_t header_crc;
    uint64_t current_lba;
    uint64_t backup_lba;
    uint64_t first_usable;
    uint64_t last_usable;
    uint64_t entries_lba;
    uint32_t entry_count;
    uint32_t entry_size;
    uint32_t entries_crc;
    uint64_t entries_bytes;
    uint64_t entries_offset;
    uint64_t entries_end;
    uint64_t parent_sectors;
    uint32_t computed_crc;
    const uint8_t *entry;
    uint64_t first_lba;
    uint64_t last_lba;
    uint8_t scratch[GPT_HEADER_BYTES];
    uint32_t index;

    if (parse_guid(BOOT_TYPE_GUID, expected_type) != 0 ||
        parse_guid(BOOT_UNIQUE_GUID, expected_unique) != 0) {
        report("internal boot GUID constants are malformed");
        return -1;
    }
    if (pread_full(parent, header, sizeof(header), (off_t)SECTOR_SIZE) != 0) {
        report("cannot read the GPT header: %s", strerror(errno));
        return -1;
    }
    if (memcmp(header, "EFI PART", 8) != 0) {
        report("the parent device does not contain a GPT header");
        return -1;
    }
    revision = read_le32(header + 8);
    header_size = read_le32(header + 12);
    header_crc = read_le32(header + 16);
    current_lba = read_le64(header + 24);
    backup_lba = read_le64(header + 32);
    first_usable = read_le64(header + 40);
    last_usable = read_le64(header + 48);
    entries_lba = read_le64(header + 72);
    entry_count = read_le32(header + 80);
    entry_size = read_le32(header + 84);
    entries_crc = read_le32(header + 88);

    if (revision != 0x00010000u) {
        report("unsupported GPT revision 0x%08x", revision);
        return -1;
    }
    if (header_size < 92u || header_size > GPT_HEADER_BYTES) {
        report("invalid GPT header size %u", header_size);
        return -1;
    }
    memcpy(scratch, header, header_size);
    memset(scratch + 16, 0, 4);
    computed_crc = crc32_bytes(scratch, header_size);
    if (computed_crc != header_crc) {
        report("GPT header CRC mismatch (stored 0x%08x, computed 0x%08x)",
               header_crc, computed_crc);
        return -1;
    }
    if (current_lba != 1u) {
        report("the GPT header is not the primary header at LBA 1");
        return -1;
    }
    parent_sectors = parent_capacity / SECTOR_SIZE;
    if (first_usable < 2u || first_usable > PARTITION_START_LBA ||
        last_usable < first_usable || last_usable >= parent_sectors) {
        report("the GPT usable LBA range %llu..%llu is inconsistent with the parent device",
               (unsigned long long)first_usable, (unsigned long long)last_usable);
        return -1;
    }
    if (parent_sectors == 0u || backup_lba != parent_sectors - 1) {
        report("the GPT backup header is not at the last sector of the parent device");
        return -1;
    }
    if (entry_count == 0u || entry_count > GPT_MAX_ENTRIES) {
        report("unsupported GPT partition entry count %u", entry_count);
        return -1;
    }
    if (entry_size < 128u || entry_size > 4096u ||
        (uint64_t)entry_count * entry_size > GPT_MAX_ENTRY_BYTES) {
        report("unsupported GPT partition entry size %u", entry_size);
        return -1;
    }
    if (mul_u64_checked((uint64_t)entry_count, (uint64_t)entry_size, &entries_bytes) != 0 ||
        mul_u64_checked(entries_lba, SECTOR_SIZE, &entries_offset) != 0 ||
        add_u64_checked(entries_offset, entries_bytes, &entries_end) != 0) {
        report("the GPT partition entry array range overflows");
        return -1;
    }
    if (entries_lba < 2u || entries_end > parent_capacity ||
        entries_end > first_usable * SECTOR_SIZE) {
        report("the GPT partition entry array lies outside the primary partition table area");
        return -1;
    }
    if ((entries_bytes + SECTOR_SIZE - 1) / SECTOR_SIZE >= backup_lba ||
        last_usable >= backup_lba - (entries_bytes + SECTOR_SIZE - 1) / SECTOR_SIZE) {
        report("the GPT usable range overlaps the backup partition table");
        return -1;
    }
    entries = malloc((size_t)entries_bytes);
    if (entries == NULL) {
        report("cannot allocate %llu bytes for the GPT entry array",
               (unsigned long long)entries_bytes);
        return -1;
    }
    if (pread_full(parent, entries, (size_t)entries_bytes, (off_t)entries_offset) != 0) {
        report("cannot read the GPT entry array: %s", strerror(errno));
        free(entries);
        return -1;
    }
    computed_crc = crc32_bytes(entries, (size_t)entries_bytes);
    if (computed_crc != entries_crc) {
        report("GPT entry array CRC mismatch (stored 0x%08x, computed 0x%08x)",
               entries_crc, computed_crc);
        free(entries);
        return -1;
    }
    entry = entries;
    if (memcmp(entry, expected_type, 16) != 0) {
        char found[37];
        format_guid(entry, found);
        report("boot partition type GUID mismatch: expected %s, found %s",
               BOOT_TYPE_GUID, found);
        free(entries);
        return -1;
    }
    if (memcmp(entry + 16, expected_unique, 16) != 0) {
        char found[37];
        format_guid(entry + 16, found);
        report("boot partition unique GUID mismatch: expected %s, found %s",
               BOOT_UNIQUE_GUID, found);
        free(entries);
        return -1;
    }
    first_lba = read_le64(entry + 32);
    last_lba = read_le64(entry + 40);
    if (first_lba != PARTITION_START_LBA || last_lba < first_lba ||
        last_lba - first_lba + 1 != PARTITION_SECTORS) {
        report("boot partition geometry mismatch: start %llu, last %llu, expected %llu..%llu",
               (unsigned long long)first_lba, (unsigned long long)last_lba,
               (unsigned long long)PARTITION_START_LBA,
               (unsigned long long)(PARTITION_START_LBA + PARTITION_SECTORS - 1));
        free(entries);
        return -1;
    }
    if (first_lba < first_usable || last_lba > last_usable ||
        (last_lba + 1) * SECTOR_SIZE > parent_capacity) {
        report("the boot partition extends past the GPT usable range or parent capacity");
        free(entries);
        return -1;
    }
    for (index = 1; index < entry_count; index++) {
        static const uint8_t empty_guid[16] = {0};
        const uint8_t *item = entries + (size_t)index * entry_size;
        uint64_t item_first;
        uint64_t item_last;
        if (memcmp(item, empty_guid, 16) == 0) {
            continue;
        }
        item_first = read_le64(item + 32);
        item_last = read_le64(item + 40);
        if (item_first > item_last ||
            item_first < first_usable || item_last > last_usable) {
            report("GPT partition %u has an invalid LBA range %llu..%llu",
                   index + 1u, (unsigned long long)item_first,
                   (unsigned long long)item_last);
            free(entries);
            return -1;
        }
        if (item_first <= PARTITION_START_LBA + PARTITION_SECTORS - 1 &&
            item_last >= PARTITION_START_LBA) {
            report("GPT partition %u overlaps the boot partition", index + 1u);
            free(entries);
            return -1;
        }
    }
    free(entries);
    boot->start_lba = first_lba;
    boot->sectors = last_lba - first_lba + 1;
    return 0;
}

static int load_image(const char *path, uint8_t *buffer, size_t buffer_size, size_t *image_size)
{
    struct stat status;
    int descriptor = open(path, O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
    if (descriptor < 0) {
        if (errno == ELOOP) {
            report("image %s must not be a symbolic link", path);
        } else {
            report("cannot open image %s: %s", path, strerror(errno));
        }
        return -1;
    }
    if (fstat(descriptor, &status) != 0) {
        report("cannot stat image %s: %s", path, strerror(errno));
        close(descriptor);
        return -1;
    }
    if (!S_ISREG(status.st_mode)) {
        report("image %s is not a regular file", path);
        close(descriptor);
        return -1;
    }
    if (status.st_size <= 0) {
        report("image %s is empty", path);
        close(descriptor);
        return -1;
    }
    if ((uint64_t)status.st_size > (uint64_t)buffer_size) {
        report("image %s is %lld bytes, larger than the %llu byte boot partition",
               path, (long long)status.st_size, (unsigned long long)buffer_size);
        close(descriptor);
        return -1;
    }
    if (pread_full(descriptor, buffer, (size_t)status.st_size, 0) != 0) {
        report("cannot read image %s: %s", path, strerror(errno));
        close(descriptor);
        return -1;
    }
    close(descriptor);
    *image_size = (size_t)status.st_size;
    return 0;
}

int main(int argc, char **argv)
{
    const char *device = NULL;
    const char *image_path = NULL;
#ifdef PLATFORM_SD_UPDATE_TEST
    const char *test_disk = NULL;
    int test_fail_fsync = 0;
#endif
    const char *target;
    const char *parent_description;
    int dry_run = 0;
    int simulated = 0;
    int result = 1;
    int target_fd = -1;
    int parent_fd = -1;
    int write_fd = -1;
    uint8_t *image = NULL;
    size_t image_size = 0;
    uint64_t write_offset = 0;
    uint64_t parent_capacity = 0;
    dev_t target_device = 0;
    struct stat target_stat;
    struct stat target_fstat;
    struct stat parent_stat;
    struct stat write_stat;
    struct boot_partition boot;
    char dir[PATH_BUF];
    char base[NAME_BUF];
    char parent_base[NAME_BUF];
    char parent_path[PATH_BUF];
    int index;

    memset(&target_stat, 0, sizeof(target_stat));
    memset(&target_fstat, 0, sizeof(target_fstat));
    memset(&parent_stat, 0, sizeof(parent_stat));
    memset(&write_stat, 0, sizeof(write_stat));
    memset(&boot, 0, sizeof(boot));

    for (index = 1; index < argc; index++) {
        const char *argument = argv[index];
        if (strcmp(argument, "--help") == 0 || strcmp(argument, "-h") == 0) {
            usage(stdout);
            return 0;
        } else if (strcmp(argument, "--device") == 0) {
            if (++index >= argc) {
                report("--device needs a path");
                usage(stderr);
                return 2;
            }
            device = argv[index];
        } else if (strcmp(argument, "--image") == 0) {
            if (++index >= argc) {
                report("--image needs a path");
                usage(stderr);
                return 2;
            }
            image_path = argv[index];
        } else if (strcmp(argument, "--dry-run") == 0) {
            dry_run = 1;
#ifdef PLATFORM_SD_UPDATE_TEST
        } else if (strcmp(argument, "--test-disk") == 0) {
            if (++index >= argc) {
                report("--test-disk needs a path");
                usage(stderr);
                return 2;
            }
            test_disk = argv[index];
        } else if (strcmp(argument, "--test-fail-fsync") == 0) {
            test_fail_fsync = 1;
#endif
        } else {
            report("unknown option: %s", argument);
            usage(stderr);
            return 2;
        }
    }

    if (image_path == NULL) {
        report("--image is required");
        usage(stderr);
        return 2;
    }

    target = device;
#ifdef PLATFORM_SD_UPDATE_TEST
    if (test_disk != NULL) {
        simulated = 1;
        target = test_disk;
    }
#endif
    if (target == NULL) {
        report("--device is required");
        usage(stderr);
        return 2;
    }

    if (split_dir_base(target, dir, sizeof(dir), base, sizeof(base)) != 0) {
        report("path is too long or empty: %s", target);
        goto cleanup;
    }

    if (!simulated) {
        int linked = path_has_symlink(target);
        if (linked < 0) {
            report("cannot inspect path %s", target);
            goto cleanup;
        }
        if (linked > 0) {
            report("refusing symlinked target: %s", target);
            goto cleanup;
        }
        if (stat(target, &target_stat) != 0) {
            report("cannot stat %s: %s", target, strerror(errno));
            goto cleanup;
        }
        if (!S_ISBLK(target_stat.st_mode)) {
            report("target %s is not a block device", target);
            goto cleanup;
        }
        if (strcmp(dir, "/dev") != 0) {
            report("target %s must live under /dev as /dev/mmcblk<number>p1", target);
            goto cleanup;
        }
        if (parse_boot_partition_name(base, parent_base, sizeof(parent_base)) != 0) {
            report("target basename %s is not mmcblk<number>p1", base);
            goto cleanup;
        }
        if (geteuid() != 0) {
            report("refusing to write the boot partition without root privileges");
            goto cleanup;
        }
        join_path(parent_path, sizeof(parent_path), dir, parent_base);
        parent_description = parent_path;
    } else {
        parent_description = target;
    }

    target_fd = open(target, O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
    if (target_fd < 0) {
        report("cannot open %s read-only: %s", target, strerror(errno));
        goto cleanup;
    }
    if (fstat(target_fd, &target_fstat) != 0) {
        report("cannot stat %s: %s", target, strerror(errno));
        goto cleanup;
    }
    if (simulated) {
        if (!S_ISREG(target_fstat.st_mode)) {
            report("test disk %s is not a regular file", target);
            goto cleanup;
        }
        if ((uint64_t)target_fstat.st_size < PARTITION_START_LBA * SECTOR_SIZE + PARTITION_SIZE) {
            report("test disk %s is %lld bytes; at least %llu bytes are required",
                   target, (long long)target_fstat.st_size,
                   (unsigned long long)(PARTITION_START_LBA * SECTOR_SIZE + PARTITION_SIZE));
            goto cleanup;
        }
        parent_capacity = (uint64_t)target_fstat.st_size;
        write_offset = PARTITION_START_LBA * SECTOR_SIZE;
    } else {
        uint64_t size = 0;
        if (!S_ISBLK(target_fstat.st_mode) || target_fstat.st_rdev != target_stat.st_rdev) {
            report("device %s changed while opening", target);
            goto cleanup;
        }
        target_device = target_fstat.st_rdev;
        if (ioctl(target_fd, BLKGETSIZE64, &size) != 0) {
            report("BLKGETSIZE64 failed on %s: %s", target, strerror(errno));
            goto cleanup;
        }
        if (size != PARTITION_SIZE) {
            report("partition %s reports %llu bytes, expected exactly %llu",
                   target, (unsigned long long)size, (unsigned long long)PARTITION_SIZE);
            goto cleanup;
        }
        {
            uint64_t logical_sector = 0;
            if (block_logical_sector_size(target_fd, target, &logical_sector) != 0) {
                goto cleanup;
            }
            if (logical_sector != SECTOR_SIZE) {
                report("partition %s reports a %llu byte logical sector, expected %llu",
                       target, (unsigned long long)logical_sector,
                       (unsigned long long)SECTOR_SIZE);
                goto cleanup;
            }
        }
        {
            int mounted = device_is_mounted(target_device, target);
            if (mounted < 0) {
                report("cannot read /proc/self/mountinfo: %s", strerror(errno));
                goto cleanup;
            }
            if (mounted) {
                report("target %s is mounted; unmount it before updating", target);
                goto cleanup;
            }
        }
        write_offset = 0;
    }

    parent_fd = open(parent_description, O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
    if (parent_fd < 0) {
        report("cannot open %s read-only: %s", parent_description, strerror(errno));
        goto cleanup;
    }
    if (fstat(parent_fd, &parent_stat) != 0) {
        report("cannot stat %s: %s", parent_description, strerror(errno));
        goto cleanup;
    }
    if (simulated) {
        if (parent_stat.st_dev != target_fstat.st_dev ||
            parent_stat.st_ino != target_fstat.st_ino) {
            report("test disk %s changed while opening", target);
            goto cleanup;
        }
    } else {
        uint64_t size = 0;
        if (!S_ISBLK(parent_stat.st_mode)) {
            report("parent %s is not a block device", parent_description);
            goto cleanup;
        }
        if (major(parent_stat.st_rdev) != major(target_device) ||
            parent_stat.st_rdev == target_device) {
            report("parent %s does not match partition %s", parent_description, target);
            goto cleanup;
        }
        if (ioctl(parent_fd, BLKGETSIZE64, &size) != 0) {
            report("BLKGETSIZE64 failed on %s: %s", parent_description, strerror(errno));
            goto cleanup;
        }
        parent_capacity = size;
        {
            uint64_t logical_sector = 0;
            if (block_logical_sector_size(parent_fd, parent_description, &logical_sector) != 0) {
                goto cleanup;
            }
            if (logical_sector != SECTOR_SIZE) {
                report("parent %s reports a %llu byte logical sector, expected %llu",
                       parent_description, (unsigned long long)logical_sector,
                       (unsigned long long)SECTOR_SIZE);
                goto cleanup;
            }
        }
    }

    if (flock(parent_fd, LOCK_EX | LOCK_NB) != 0) {
        report("cannot lock %s exclusively: %s", parent_description, strerror(errno));
        goto cleanup;
    }

    if (!simulated &&
        verify_sysfs_layout(base, parent_base, target_device, parent_stat.st_rdev) != 0) {
        goto cleanup;
    }

    if (validate_gpt(parent_fd, parent_capacity, &boot) != 0) {
        goto cleanup;
    }
    if (boot.start_lba != PARTITION_START_LBA || boot.sectors != PARTITION_SECTORS) {
        report("the boot partition does not match the mmcblk<number>p1 layout");
        goto cleanup;
    }

    image = calloc(1, (size_t)PARTITION_SIZE);
    if (image == NULL) {
        report("cannot allocate %llu bytes for the image buffer",
               (unsigned long long)PARTITION_SIZE);
        goto cleanup;
    }
    if (load_image(image_path, image, (size_t)PARTITION_SIZE, &image_size) != 0) {
        goto cleanup;
    }

    if (dry_run) {
        printf("DRY-RUN: %s is a valid %llu byte boot partition; would write %zu image bytes "
               "and %llu zero padding bytes\n",
               target, (unsigned long long)PARTITION_SIZE, image_size,
               (unsigned long long)(PARTITION_SIZE - image_size));
        result = 0;
        goto cleanup;
    }

    write_fd = open(target, O_RDWR | O_CLOEXEC | O_NOFOLLOW);
    if (write_fd < 0) {
        report("cannot open %s for writing: %s", target, strerror(errno));
        goto cleanup;
    }
    if (fstat(write_fd, &write_stat) != 0) {
        report("cannot stat %s: %s", target, strerror(errno));
        goto cleanup;
    }
    if (simulated) {
        if (!S_ISREG(write_stat.st_mode) || write_stat.st_dev != target_fstat.st_dev ||
            write_stat.st_ino != target_fstat.st_ino) {
            report("test disk %s changed before the write", target);
            goto cleanup;
        }
        if ((uint64_t)write_stat.st_size < write_offset + PARTITION_SIZE) {
            report("test disk %s shrank before the write", target);
            goto cleanup;
        }
    } else {
        uint64_t size = 0;
        if (!S_ISBLK(write_stat.st_mode) || write_stat.st_rdev != target_device) {
            report("device %s changed before the write", target);
            goto cleanup;
        }
        if (ioctl(write_fd, BLKGETSIZE64, &size) != 0 || size != PARTITION_SIZE) {
            report("partition %s no longer reports exactly %llu bytes",
                   target, (unsigned long long)PARTITION_SIZE);
            goto cleanup;
        }
    }

    if (!simulated) {
        int mounted = device_is_mounted(target_device, target);
        if (mounted < 0) {
            report("cannot read /proc/self/mountinfo: %s", strerror(errno));
            goto cleanup;
        }
        if (mounted) {
            report("target %s became mounted; refusing to write", target);
            goto cleanup;
        }
    }
    if (pwrite_full(write_fd, image, (size_t)PARTITION_SIZE, (off_t)write_offset) != 0) {
        report("writing %s failed: %s", target, strerror(errno));
        goto cleanup;
    }

    {
        int sync_result;
#ifdef PLATFORM_SD_UPDATE_TEST
        if (test_fail_fsync) {
            errno = EIO;
            sync_result = -1;
        } else {
            sync_result = fsync(write_fd);
        }
#else
        sync_result = fsync(write_fd);
#endif
        if (sync_result != 0) {
            report("fsync of %s failed: %s", target, strerror(errno));
            goto cleanup;
        }
    }

    if (close(write_fd) != 0) {
        report("close of %s failed: %s", target, strerror(errno));
        write_fd = -1;
        goto cleanup;
    }
    write_fd = -1;

    printf("PASS: wrote %llu bytes to %s (%zu image bytes, %llu zero padding bytes)\n",
           (unsigned long long)PARTITION_SIZE, target, image_size,
           (unsigned long long)(PARTITION_SIZE - image_size));
    result = 0;

cleanup:
    if (write_fd >= 0) {
        close(write_fd);
    }
    if (parent_fd >= 0) {
        close(parent_fd);
    }
    if (target_fd >= 0) {
        close(target_fd);
    }
    free(image);
    return result;
}

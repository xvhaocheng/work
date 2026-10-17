#define _GNU_SOURCE
#define _LARGEFILE64_SOURCE
/* Independent configuration-readonly PN helper; no MODE/GAP write API. */
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PN_BASE UINT64_C(0x50000000)
#define PN_CAP UINT64_C(0x49444d41504e0001)
#define GAP_CAP UINT64_C(0x49444d4147415001)
#define PN_FIRST 192U
#define PN_WORDS 8U
#define PN_HISTORY_SUPPORTED (UINT64_C(1) << 63)
#define OBS_CONTROL 0x60U
#define OBS_INDEX 0x70U
#define OBS_DATA 0x78U
#define V_GAP 0xb0U
#define V_GAP_CAP 0xb8U

enum pn_operation { PN_INVALID, PN_CAPABILITIES, PN_READ };

/* No general write callback: the sole store selects an index in 192..199. */
struct pn_io {
    void *context;
    int (*load64)(void *context, unsigned int offset, uint64_t *value);
    int (*select_index)(void *context, unsigned int index);
};

struct pn_result {
    enum pn_operation operation;
    int error;
    const char *stage;
    uint64_t raw[PN_WORDS], gap, gap_cap, before, after;
    unsigned int raw_valid, scalar_valid, index_write_attempts;
};

enum { HAVE_GAP = 1U, HAVE_GAP_CAP = 2U, HAVE_BEFORE = 4U, HAVE_AFTER = 8U };

static int pn_fail(struct pn_result *r, int error, const char *stage)
{
    if (!r->error) {
        r->error = error > 0 ? error : EIO;
        r->stage = stage;
    }
    return r->error;
}

static enum pn_operation pn_arguments(int argc, char **argv)
{
    if (argc != 2)
        return PN_INVALID;
    if (!strcmp(argv[1], "capabilities"))
        return PN_CAPABILITIES;
    if (!strcmp(argv[1], "read"))
        return PN_READ;
    return PN_INVALID;
}

static int pn_snapshot(struct pn_io *io, enum pn_operation operation,
                       struct pn_result *r)
{
    unsigned int i;
    uint64_t selected;
    int error;

    memset(r, 0, sizeof(*r));
    r->operation = operation;
    r->stage = "complete";
    if (operation != PN_READ && operation != PN_CAPABILITIES)
        return pn_fail(r, EINVAL, "arguments");
    if (!io || !io->load64 || !io->select_index)
        return pn_fail(r, EINVAL, "interface");
    error = io->load64(io->context, OBS_CONTROL, &r->before);
    if (error)
        return pn_fail(r, error, "observer_before");
    r->scalar_valid |= HAVE_BEFORE;
    if (r->before & 3U)
        return pn_fail(r, EBUSY, "observer_before");
    for (i = 0; i < PN_WORDS; i++) {
        r->index_write_attempts++;
        error = io->select_index(io->context, PN_FIRST + i);
        if (error)
            return pn_fail(r, error, "index_write");
        error = io->load64(io->context, OBS_INDEX, &selected);
        if (error)
            return pn_fail(r, error, "index_readback");
        if (selected != PN_FIRST + i)
            return pn_fail(r, EIO, "index_readback");
        error = io->load64(io->context, OBS_DATA, &r->raw[i]);
        if (error)
            return pn_fail(r, error, "bank_read");
        r->raw_valid |= 1U << i;
    }
    error = io->load64(io->context, V_GAP, &r->gap);
    if (error)
        return pn_fail(r, error, "gap_read");
    r->scalar_valid |= HAVE_GAP;
    error = io->load64(io->context, V_GAP_CAP, &r->gap_cap);
    if (error)
        return pn_fail(r, error, "gap_cap_read");
    r->scalar_valid |= HAVE_GAP_CAP;
    error = io->load64(io->context, OBS_CONTROL, &r->after);
    if (error)
        return pn_fail(r, error, "observer_after");
    r->scalar_valid |= HAVE_AFTER;
    if (r->after & 3U)
        return pn_fail(r, EBUSY, "observer_after");
    if (r->raw[0] != PN_CAP)
        return pn_fail(r, ENODEV, "pn_capability");
    if (r->gap_cap != GAP_CAP)
        return pn_fail(r, ENODEV, "gap_capability");
    if (r->raw[1] > 3U && r->raw[1] != 6U)
        return pn_fail(r, EPROTO, "mode");
    if (r->raw[1] == 6U && !(r->raw[2] & PN_HISTORY_SUPPORTED))
        return pn_fail(r, EPROTO, "history_support");
    if (r->gap >> 32)
        return pn_fail(r, EPROTO, "gap_high_bits");
    return 0;
}

static void pn_value(FILE *out, bool valid, uint64_t value)
{
    if (valid)
        fprintf(out, "%" PRIu64, value);
    else
        fputs("null", out);
}

static void pn_bool(FILE *out, bool valid, bool value)
{
    fputs(valid ? (value ? "true" : "false") : "null", out);
}

static void pn_json(FILE *out, const struct pn_result *r)
{
    unsigned int i;
    const char *operation = r->operation == PN_READ ? "read" :
        r->operation == PN_CAPABILITIES ? "capabilities" : "invalid";

    fprintf(out, "{\"schema\":\"idma.pn_control.v1\",\"success\":%s,"
            "\"errno\":%d,\"operation\":\"%s\",\"read_only\":true,"
            "\"stage\":\"%s\",\"mmio_base\":%" PRIu64 ",\"word_bits\":64,",
            r->error ? "false" : "true", r->error, operation, r->stage, PN_BASE);
#ifdef PN_CONTROL_HOST_TEST
    fputs("\"fixture_only\":true,\"synthetic\":true,\"source_board_cases\":0,", out);
#endif
    fputs("\"cap\":", out);
    pn_value(out, r->raw_valid & 1U, r->raw[0]);
    fputs(",\"mode\":", out);
    pn_value(out, r->raw_valid & 2U, r->raw[1] & 3U);
    fputs(",\"mode_word\":", out);
    pn_value(out, r->raw_valid & 2U, r->raw[1]);
    fputs(",\"history_shared\":", out);
    pn_bool(out, r->raw_valid & 2U, r->raw[1] & 4U);
    fputs(",\"history_control_supported\":", out);
    pn_bool(out, r->raw_valid & 4U, r->raw[2] & PN_HISTORY_SUPPORTED);
    fputs(",\"partition_history\":", out);
    if (!(r->raw_valid & 2U))
        fputs("null", out);
    else if (r->raw[1] == 0U || r->raw[1] == 1U)
        fputs("\"not_partitioned\"", out);
    else if (r->raw[1] == 2U || r->raw[1] == 3U)
        fputs("\"isolated\"", out);
    else if (r->raw[1] == 6U)
        fputs("\"shared\"", out);
    else
        fputs("null", out);
    fputs(",\"v_gap_cycles\":", out);
    pn_value(out, r->scalar_valid & HAVE_GAP, r->gap);
    fputs(",\"gap_cap\":", out);
    pn_value(out, r->scalar_valid & HAVE_GAP_CAP, r->gap_cap);
    fputs(",\"observer_before\":", out);
    pn_value(out, r->scalar_valid & HAVE_BEFORE, r->before);
    fputs(",\"observer_after\":", out);
    pn_value(out, r->scalar_valid & HAVE_AFTER, r->after);
    fprintf(out, ",\"index_selector_write_attempts\":%u,\"raw\":{",
            r->index_write_attempts);
    for (i = 0; i < PN_WORDS; i++) {
        fprintf(out, "%s\"%u\":", i ? "," : "", PN_FIRST + i);
        pn_value(out, r->raw_valid & (1U << i), r->raw[i]);
    }
    fputs("}}", out);
}

#ifndef PN_CONTROL_HOST_TEST
#if !defined(__riscv) || __riscv_xlen != 64
#error "Production pn-control requires RV64; host tests must use PN_CONTROL_HOST_TEST"
#endif
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

struct pn_mmio { volatile unsigned char *mapping; };
static sigjmp_buf pn_fault;
static volatile sig_atomic_t pn_access_active;

static void pn_fault_handler(int signal_number)
{
    if (pn_access_active)
        siglongjmp(pn_fault, 1);
    _exit(128 + signal_number);
}

static inline void pn_fence(void)
{
    __asm__ volatile ("fence iorw, iorw" ::: "memory");
}

/* These two noinline functions are the complete production MMIO access surface. */
static __attribute__((noinline)) int pn_mmio_load64(void *context,
                                      unsigned int offset, uint64_t *value)
{
    struct pn_mmio *mmio = context;
    volatile uint64_t *address;

    if (offset != OBS_CONTROL && offset != OBS_INDEX && offset != OBS_DATA &&
        offset != V_GAP && offset != V_GAP_CAP)
        return EINVAL;
    address = (volatile uint64_t *)(mmio->mapping + offset);
    if (sigsetjmp(pn_fault, 1)) {
        pn_access_active = 0;
        return EIO;
    }
    pn_access_active = 1;
    pn_fence();
    *value = *address;
    pn_fence();
    pn_access_active = 0;
    return 0;
}

static __attribute__((noinline)) int pn_mmio_select_index(void *context,
                                                        unsigned int index)
{
    struct pn_mmio *mmio = context;
    volatile uint64_t *address =
        (volatile uint64_t *)(mmio->mapping + OBS_INDEX);

    if (index < PN_FIRST || index >= PN_FIRST + PN_WORDS)
        return EINVAL;
    if (sigsetjmp(pn_fault, 1)) {
        pn_access_active = 0;
        return EIO;
    }
    pn_access_active = 1;
    pn_fence();
    *address = (uint64_t)index;
    pn_fence();
    pn_access_active = 0;
    return 0;
}

int main(int argc, char **argv)
{
    struct pn_result result = { .operation = PN_INVALID, .stage = "complete" };
    struct pn_mmio mmio;
    struct pn_io io = { &mmio, pn_mmio_load64, pn_mmio_select_index };
    struct sigaction action = { .sa_handler = pn_fault_handler }, old_bus, old_segv;
    bool bus_installed = false, segv_installed = false;
    long page_bytes;
    int fd = -1;

    result.operation = pn_arguments(argc, argv);
    if (result.operation == PN_INVALID) {
        pn_fail(&result, EINVAL, "arguments");
        goto output;
    }
    page_bytes = sysconf(_SC_PAGESIZE);
    if (page_bytes < 4096 || PN_BASE % (uint64_t)page_bytes) {
        pn_fail(&result, EINVAL, "page_size");
        goto output;
    }
    fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC);
    if (fd < 0) {
        pn_fail(&result, errno, "open_devmem");
        goto output;
    }
    mmio.mapping = mmap64(NULL, (size_t)page_bytes, PROT_READ | PROT_WRITE,
                          MAP_SHARED, fd, (off64_t)PN_BASE);
    if (mmio.mapping == MAP_FAILED) {
        pn_fail(&result, errno, "mmap64");
        goto close_fd;
    }
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGBUS, &action, &old_bus) < 0) {
        pn_fail(&result, errno, "sigaction_bus");
        goto unmap;
    }
    bus_installed = true;
    if (sigaction(SIGSEGV, &action, &old_segv) < 0) {
        pn_fail(&result, errno, "sigaction_segv");
        goto restore;
    }
    segv_installed = true;
    pn_snapshot(&io, result.operation, &result);
restore:
    if (segv_installed && sigaction(SIGSEGV, &old_segv, NULL) < 0)
        pn_fail(&result, errno, "restore_sigsegv");
    if (bus_installed && sigaction(SIGBUS, &old_bus, NULL) < 0)
        pn_fail(&result, errno, "restore_sigbus");
unmap:
    if (munmap((void *)mmio.mapping, (size_t)page_bytes) < 0)
        pn_fail(&result, errno, "munmap");
close_fd:
    if (close(fd) < 0)
        pn_fail(&result, errno, "close_devmem");
output:
    pn_json(stdout, &result);
    fputc('\n', stdout);
    if (fflush(stdout) || ferror(stdout))
        return 1;
    return result.error ? 1 : 0;
}
#endif

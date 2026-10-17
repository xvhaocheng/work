#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE 1
#endif
/* Bounded PN configuration writer. No observer/DMA command or arbitrary store. */
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PN_BASE UINT64_C(0x50000000)
#define PN_CAP UINT64_C(0x49444d41504e0001)
#define PN_GAP_CAP UINT64_C(0x49444d4147415001)
#define PN_HISTORY (UINT64_C(1) << 63)
#define PN_REJECT UINT64_C(2)
#define PN_STATUS_ALLOWED (PN_HISTORY | UINT64_C(0x3f))
#define PN_REQUIRED UINT64_C(0x11)
#define PN_BUSY_FAULT UINT64_C(0x2c)
#define PN_OBS 0x60U
#define PN_INDEX 0x70U
#define PN_DATA 0x78U
#define PN_GAP 0xb0U
#define PN_GAPCAP 0xb8U

enum pn_operation { PN_INVALID, PN_SET_MODE, PN_SET_GAP };
struct pn_request { enum pn_operation operation; uint64_t value; bool valid; };
struct pn_io {
    void *context;
    int (*load64)(void *, unsigned int, uint64_t *);
    int (*select_index)(void *, unsigned int);
    int (*write_mode)(void *, uint64_t);
    int (*write_gap)(void *, uint64_t);
};
enum { PN_HAVE_OBS=1U, PN_HAVE_CAP=2U, PN_HAVE_MODE=4U,
       PN_HAVE_STATUS=8U, PN_HAVE_GAP=16U, PN_HAVE_GAPCAP=32U };
struct pn_snapshot {
    uint64_t observer, cap, mode, status, gap, gap_cap;
    unsigned int present;
};
struct pn_result {
    struct pn_request request;
    struct pn_snapshot before, after;
    int error;
    const char *stage;
    unsigned int index_attempts, functional_attempts;
    bool readback_verified;
};

static bool pn_mode_allowed(uint64_t v)
{
    return v == 0 || v == 1 || v == 2 || v == 3 || v == 6;
}
static bool pn_gap_allowed(uint64_t v)
{
    return v == 0 || v == 1000 || v == 4000;
}
static bool pn_index_allowed(unsigned int v)
{
    return v == 192 || v == 193 || v == 194;
}
static int pn_fail(struct pn_result *r, int error, const char *stage)
{
    if (!r->error) {
        r->error = error > 0 ? error : EIO;
        r->stage = stage;
    }
    return r->error;
}
static struct pn_request pn_arguments(int argc, char **argv)
{
    struct pn_request r = { PN_INVALID, 0, false };
    static const char *const modes[] = { "0", "1", "2", "3", "6" };
    static const uint64_t mode_values[] = { 0, 1, 2, 3, 6 };
    static const char *const gaps[] = { "0", "1000", "4000" };
    static const uint64_t gap_values[] = { 0, 1000, 4000 };
    unsigned int i;

    if (argc != 3)
        return r;
    if (!strcmp(argv[1], "set-mode")) {
        r.operation = PN_SET_MODE;
        for (i = 0; i < sizeof(modes)/sizeof(modes[0]); i++)
            if (!strcmp(argv[2], modes[i])) {
                r.value = mode_values[i];
                r.valid = true;
                return r;
            }
    } else if (!strcmp(argv[1], "set-gap")) {
        r.operation = PN_SET_GAP;
        for (i = 0; i < sizeof(gaps)/sizeof(gaps[0]); i++)
            if (!strcmp(argv[2], gaps[i])) {
                r.value = gap_values[i];
                r.valid = true;
                return r;
            }
    }
    return r;
}
static int pn_select(struct pn_io *io, unsigned int index,
                     struct pn_result *r, const char *stage)
{
    uint64_t selected;
    int error;

    if (!pn_index_allowed(index))
        return pn_fail(r, EINVAL, "index_whitelist");
    r->index_attempts++;
    error = io->select_index(io->context, index);
    if (error)
        return pn_fail(r, error, stage);
    error = io->load64(io->context, PN_INDEX, &selected);
    if (error)
        return pn_fail(r, error, stage);
    if (selected != index)
        return pn_fail(r, EIO, stage);
    return 0;
}
static int pn_read_snapshot(struct pn_io *io, struct pn_snapshot *s,
                            struct pn_result *r, bool after)
{
    uint64_t *values[] = { &s->cap, &s->mode, &s->status };
    const unsigned int flags[] = { PN_HAVE_CAP, PN_HAVE_MODE, PN_HAVE_STATUS };
    const char *const stage_pre[] = { "before_cap", "before_mode", "before_status" };
    const char *const stage_post[] = { "after_cap", "after_mode", "after_status" };
    const char *const *stages = after ? stage_post : stage_pre;
    int error;
    unsigned int i;

    error = io->load64(io->context, PN_OBS, &s->observer);
    if (error)
        return pn_fail(r, error, after ? "after_observer" : "before_observer");
    s->present |= PN_HAVE_OBS;
    if (s->observer & 3U)
        return pn_fail(r, EBUSY, after ? "after_observer" : "before_observer");
    for (i = 0; i < 3; i++) {
        if (pn_select(io, 192U+i, r, stages[i]))
            return r->error;
        error = io->load64(io->context, PN_DATA, values[i]);
        if (error)
            return pn_fail(r, error, stages[i]);
        s->present |= flags[i];
    }
    error = io->load64(io->context, PN_GAP, &s->gap);
    if (error)
        return pn_fail(r, error, after ? "after_gap" : "before_gap");
    s->present |= PN_HAVE_GAP;
    error = io->load64(io->context, PN_GAPCAP, &s->gap_cap);
    if (error)
        return pn_fail(r, error, after ? "after_gap_cap" : "before_gap_cap");
    s->present |= PN_HAVE_GAPCAP;
    return 0;
}
static int pn_validate_snapshot(const struct pn_snapshot *s,
                                struct pn_result *r, bool after)
{
    if (s->cap != PN_CAP)
        return pn_fail(r, ENODEV, after ? "after_capability" : "before_capability");
    if (s->gap_cap != PN_GAP_CAP)
        return pn_fail(r, ENODEV, after ? "after_gap_capability" : "before_gap_capability");
    if (!pn_mode_allowed(s->mode))
        return pn_fail(r, EPROTO, after ? "after_mode_word" : "before_mode_word");
    if (s->status & ~PN_STATUS_ALLOWED)
        return pn_fail(r, EPROTO, after ? "after_reserved_status" : "before_reserved_status");
    if (s->mode == 6 && !(s->status & PN_HISTORY))
        return pn_fail(r, EPROTO, after ? "after_history_support" : "before_history_support");
    if ((s->status & PN_REQUIRED) != PN_REQUIRED || s->status & PN_BUSY_FAULT)
        return pn_fail(r, EBUSY, after ? "after_eligibility" : "before_eligibility");
    if (s->gap >> 32)
        return pn_fail(r, EPROTO, after ? "after_gap_high_bits" : "before_gap_high_bits");
    return 0;
}
static int pn_configure(struct pn_io *io, struct pn_request request,
                         struct pn_result *r)
{
    int error;

    memset(r, 0, sizeof(*r));
    r->request = request;
    r->stage = "complete";
    /* Repeat validation at the core API boundary, before any callback. */
    if (!request.valid ||
        (request.operation == PN_SET_MODE && !pn_mode_allowed(request.value)) ||
        (request.operation == PN_SET_GAP && !pn_gap_allowed(request.value)) ||
        (request.operation != PN_SET_MODE && request.operation != PN_SET_GAP))
        return pn_fail(r, EINVAL, "arguments");
    if (!io || !io->load64 || !io->select_index || !io->write_mode || !io->write_gap)
        return pn_fail(r, EINVAL, "interface");
    if (pn_read_snapshot(io, &r->before, r, false) ||
        pn_validate_snapshot(&r->before, r, false))
        return r->error;
    if (request.operation == PN_SET_MODE) {
        if (request.value == 6 && !(r->before.status & PN_HISTORY))
            return pn_fail(r, EOPNOTSUPP, "requested_history_support");
        if (pn_select(io, 193, r, "mode_index_readback"))
            return r->error;
        r->functional_attempts++;
        error = io->write_mode(io->context, request.value);
    } else {
        r->functional_attempts++;
        error = io->write_gap(io->context, request.value);
    }
    if (error)
        return pn_fail(r, error, request.operation == PN_SET_MODE ? "mode_write" : "gap_write");
    if (pn_read_snapshot(io, &r->after, r, true) ||
        pn_validate_snapshot(&r->after, r, true))
        return r->error;
    if (r->before.cap != r->after.cap || r->before.gap_cap != r->after.gap_cap ||
        ((r->before.status ^ r->after.status) & PN_HISTORY))
        return pn_fail(r, EPROTO, "capabilities_changed");
    if (request.operation == PN_SET_MODE) {
        if (r->after.mode != request.value)
            return pn_fail(r, EIO, "mode_readback");
        if (r->after.status & PN_REJECT)
            return pn_fail(r, EACCES, "mode_rejected");
        if (r->after.gap != r->before.gap)
            return pn_fail(r, EIO, "gap_changed_during_mode");
    } else {
        if (r->after.gap != request.value)
            return pn_fail(r, EIO, "gap_readback");
        if (r->after.mode != r->before.mode)
            return pn_fail(r, EIO, "mode_changed_during_gap");
        if ((r->after.status ^ r->before.status) & PN_REJECT)
            return pn_fail(r, EIO, "mode_reject_changed_during_gap");
    }
    r->readback_verified = true;
    return 0;
}
static void pn_value(FILE *out, bool valid, uint64_t value)
{
    if (valid)
        fprintf(out, "%" PRIu64, value);
    else
        fputs("null", out);
}
static void pn_json_snapshot(FILE *out, const struct pn_snapshot *s)
{
    const char *const names[] = { "observer_word", "cap_word", "mode_word",
                                "status_word", "gap_word", "gap_cap_word" };
    const uint64_t values[] = { s->observer, s->cap, s->mode, s->status, s->gap, s->gap_cap };
    unsigned int i;
    fputs("{\"present\":{", out);
    for (i = 0; i < 6; i++)
        fprintf(out, "%s\"%s\":%s", i ? "," : "", names[i],
                s->present & (1U << i) ? "true" : "false");
    fputs("}", out);
    for (i = 0; i < 6; i++) {
        fprintf(out, ",\"%s\":", names[i]);
        pn_value(out, s->present & (1U << i), values[i]);
    }
    fputc('}', out);
}
static void pn_json(FILE *out, const struct pn_result *r)
{
    const char *operation = r->request.operation == PN_SET_MODE ? "set-mode" :
                            r->request.operation == PN_SET_GAP ? "set-gap" : "invalid";
    fprintf(out, "{\"schema\":\"idma.pn_config.v1\",\"operation\":\"%s\","
            "\"success\":%s,\"errno\":%d,\"stage\":\"%s\","
            "\"read_only\":false,\"mmio_base\":%" PRIu64 ",\"word_bits\":64,",
            operation, !r->error && r->readback_verified ? "true" : "false",
            r->error, r->stage, PN_BASE);
#ifdef PN_CONFIG_HOST_TEST
    fputs("\"fixture_only\":true,\"synthetic\":true,\"evidence_level\":\"synthetic_fixture\",", out);
#else
    fputs("\"fixture_only\":false,\"synthetic\":false,\"evidence_level\":\"configuration_mmio\",", out);
#endif
    fputs("\"source_board_cases\":0,\"requested_value\":", out);
    pn_value(out, r->request.valid, r->request.value);
    fputs(",\"before\":", out);pn_json_snapshot(out, &r->before);
    fputs(",\"after\":", out);pn_json_snapshot(out, &r->after);
    fprintf(out, ",\"index_selector_write_attempts\":%u,"
            "\"functional_write_attempts\":%u,\"readback_verified\":%s}",
            r->index_attempts, r->functional_attempts,
            r->readback_verified ? "true" : "false");
}

#if !defined(PN_CONFIG_HOST_TEST) || defined(PN_CONFIG_ACCESSOR_TEST)
#if !defined(PN_CONFIG_HOST_TEST) && (!defined(__riscv) || __riscv_xlen != 64)
#error "Production pn-config requires RV64; host fixtures use PN_CONFIG_HOST_TEST"
#endif
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

struct pn_mmio {
    volatile unsigned char *mapping;
    unsigned int selected;
    bool selection_pending, selection_verified, functional_attempted;
};
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
#ifdef PN_CONFIG_HOST_TEST
    /* Only host fixture compilation differs; RV64 production emits full fences. */
    __asm__ volatile ("" ::: "memory");
#else
    __asm__ volatile ("fence iorw, iorw" ::: "memory");
#endif
}
static __attribute__((noinline)) int pn_mmio_load64(void *context,
                                        unsigned int offset, uint64_t *value)
{
    struct pn_mmio *m = context;
    volatile uint64_t *address;
    uint64_t raw;
    if (!m || !m->mapping || !value)
        return EINVAL;
    if (offset != PN_OBS && offset != PN_INDEX && offset != PN_DATA &&
        offset != PN_GAP && offset != PN_GAPCAP)
        return EINVAL;
    if (offset == PN_DATA && (!m->selection_verified || !pn_index_allowed(m->selected)))
        return EACCES;
    address = (volatile uint64_t *)(m->mapping + offset);
    if (sigsetjmp(pn_fault, 1)) {
        pn_access_active = 0;
        m->selection_verified = false;
        return EIO;
    }
    pn_access_active = 1;
    pn_fence();raw = *address;pn_fence();
    pn_access_active = 0;
    *value = raw;
    if (offset == PN_INDEX)
        m->selection_verified = m->selection_pending && raw == m->selected;
    return 0;
}
static __attribute__((noinline)) int pn_mmio_select_index(void *context,
                                                        unsigned int index)
{
    struct pn_mmio *m = context;
    volatile uint64_t *address;
    if (!m || !m->mapping || !pn_index_allowed(index))
        return EINVAL;
    m->selection_pending = m->selection_verified = false;
    address = (volatile uint64_t *)(m->mapping + PN_INDEX);
    if (sigsetjmp(pn_fault, 1)) {
        pn_access_active = 0;
        return EIO;
    }
    pn_access_active = 1;
    pn_fence();*address = (uint64_t)index;pn_fence();
    pn_access_active = 0;
    m->selected = index;
    m->selection_pending = true;
    return 0;
}
static __attribute__((noinline)) int pn_mmio_write_mode(void *context, uint64_t value)
{
    struct pn_mmio *m = context;
    volatile uint64_t *address;
    if (!m || !m->mapping || !pn_mode_allowed(value))
        return EINVAL;
    if (m->functional_attempted || !m->selection_verified || m->selected != 193)
        return EACCES;
    m->functional_attempted = true;
    m->selection_verified = false;
    address = (volatile uint64_t *)(m->mapping + PN_DATA);
    if (sigsetjmp(pn_fault, 1)) {
        pn_access_active = 0;
        return EIO;
    }
    pn_access_active = 1;
    pn_fence();*address = value;pn_fence();
    pn_access_active = 0;
    return 0;
}
static __attribute__((noinline)) int pn_mmio_write_gap(void *context, uint64_t value)
{
    struct pn_mmio *m = context;
    volatile uint64_t *address;
    if (!m || !m->mapping || !pn_gap_allowed(value))
        return EINVAL;
    if (m->functional_attempted)
        return EACCES;
    m->functional_attempted = true;
    address = (volatile uint64_t *)(m->mapping + PN_GAP);
    if (sigsetjmp(pn_fault, 1)) {
        pn_access_active = 0;
        return EIO;
    }
    pn_access_active = 1;
    pn_fence();*address = value;pn_fence();
    pn_access_active = 0;
    return 0;
}
#if !defined(PN_CONFIG_HOST_TEST) || defined(PN_CONFIG_ENTRY_TEST)
int main(int argc, char **argv)
{
    struct pn_result result = { .stage = "complete" };
    struct pn_mmio mmio = {0};
    struct pn_io io = { &mmio, pn_mmio_load64, pn_mmio_select_index,
                       pn_mmio_write_mode, pn_mmio_write_gap };
    struct sigaction action = { .sa_handler = pn_fault_handler }, old_bus, old_segv;
    bool bus_installed=false, segv_installed=false;
    long page_bytes=0;
    int fd=-1;

    result.request = pn_arguments(argc, argv);
    if (!result.request.valid) {
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
    pn_configure(&io, result.request, &result);
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
    pn_json(stdout, &result);fputc('\n', stdout);
    if (fflush(stdout) || ferror(stdout))
        return 1;
    return result.error || !result.readback_verified ? 1 : 0;
}
#endif
#endif

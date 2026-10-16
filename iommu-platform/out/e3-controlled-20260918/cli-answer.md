## Summary

Proposed bounded implementation for the independent 64 B controlled E3 CLI, fixed board runner, and build/rootfs/profile integration.

No execution was performed. The host tests assume `out/e3-controlled-20260918/driver-0.patch` has already been applied to `linux/include/uapi/linux/idma.h`; otherwise the new CLI/stub do not compile because `IDMA_E3_CONTROLLED_WORDS`, `struct idma_e3_controlled`, `IDMA_EXPERIMENT_CAP_E3_CONTROLLED`, and `IDMA_IOC_RUN_E3_CONTROLLED` are absent.

---

## New file: `bench/idma_e3_controlled.c`

```c
// SPDX-License-Identifier: GPL-2.0
/* Finite controlled E3 CLI: fixed 64B, one ioctl owns both rounds and mapping lifetime. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <linux/idma.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define E3_OBSERVER_MAGIC_V6 0x49444d4145330006ULL
#define E3_BANK_V2           0x49444d4145330002ULL
#define E3_CTRL_STAGE_DONE   8U

#define E3_PREP_COLD 0U
#define E3_PREP_HOT  1U
#define E3_PAIR_COMPLEMENTARY 0U
#define E3_PAIR_ZERO          1U

#define E3_FIXED_VICTIM_PAGES    1U
#define E3_FIXED_AGGRESSOR_PAGES 1U
#define E3_FIXED_CHUNK_BYTES     64U
#define E3_FIXED_TRANSFERS       1ULL
#define E3_FIXED_WARMUP          0ULL
#define E3_DEFAULT_TIMEOUT_MS    5000U

/*
 * Raw launcher record layout.
 * These offsets/magic are not part of the provided UAPI patch; they must match
 * the separately applied driver-0.patch launcher record.
 */
#define E3_LAUNCH_W_MAGIC         0U
#define E3_LAUNCH_W_ERROR         1U
#define E3_LAUNCH_W_FLAGS         2U
#define E3_LAUNCH_W_INACTIVE_MASK 3U
#define E3_LAUNCH_W_ISSUE         4U
#define E3_LAUNCH_W_DONE          5U
#define E3_LAUNCH_W_CANCEL        6U
#define E3_LAUNCH_W_VICTIM_ID     7U
#define E3_LAUNCH_W_AGGRESSOR_ID  8U
#define E3_LAUNCH_MAGIC           0x4c41554e43483130ULL /* must match driver */
#define E3_LAUNCH_FROZEN          (1ULL << 2)
#define E3_LAUNCH_ABORTED         (1ULL << 0)

static int strict_number(const char *s, unsigned max, unsigned *out)
{
    unsigned long long n = 0;

    if (!s || !*s)
        return -1;
    for (; *s; s++) {
        if (*s < '0' || *s > '9')
            return -1;
        n = n * 10 + (unsigned long long)(*s - '0');
        if (n > max)
            return -1;
    }
    *out = (unsigned)n;
    return 0;
}

static void print_stats(const struct idma_experiment_stats *s)
{
    printf("{\"submitted\":%llu,\"completed\":%llu,\"completed_bytes\":%llu,"
           "\"active_cycles\":%llu,\"iotlb_miss\":%llu,\"s1_ptw\":%llu,\"s2_ptw\":%llu,"
           "\"refill_gap_ns_sum\":%llu,\"refill_gap_ns_max\":%llu,\"refill_gap_samples\":%llu,"
           "\"in_flight_at_start\":%u,\"in_flight_at_end\":%u,\"data_errors\":%u,"
           "\"quarantined\":%u}",
           (unsigned long long)s->submitted, (unsigned long long)s->completed,
           (unsigned long long)s->completed_bytes, (unsigned long long)s->active_cycles,
           (unsigned long long)s->iotlb_miss, (unsigned long long)s->s1_ptw,
           (unsigned long long)s->s2_ptw, (unsigned long long)s->refill_gap_ns_sum,
           (unsigned long long)s->refill_gap_ns_max,
           (unsigned long long)s->refill_gap_samples,
           s->in_flight_at_start, s->in_flight_at_end,
           s->data_errors, s->quarantined);
}

static int stats_zero(const struct idma_experiment_stats *s)
{
    return !s->submitted && !s->completed && !s->completed_bytes &&
           !s->active_cycles && !s->iotlb_miss && !s->s1_ptw && !s->s2_ptw &&
           !s->refill_gap_ns_sum && !s->refill_gap_ns_max &&
           !s->refill_gap_samples && !s->in_flight_at_start &&
           !s->in_flight_at_end && !s->data_errors && !s->quarantined;
}

static int lane_exact_1(const struct idma_experiment_stats *s)
{
    return s->submitted == 1 && s->completed == 1 &&
           s->completed_bytes == 64 && !s->data_errors &&
           !s->quarantined && !s->s2_ptw && !s->in_flight_at_end;
}

static int probe_exact_1(const struct idma_experiment_stats *s)
{
    return s->submitted == 1 && s->completed == 1 &&
           s->completed_bytes == 64 && !s->iotlb_miss &&
           !s->s1_ptw && !s->s2_ptw && !s->data_errors &&
           !s->quarantined && !s->in_flight_at_end;
}

static int launcher_ok(const struct idma_e3_controlled *c, unsigned r)
{
    const __u64 *w = c->controlled[r].launcher;
    unsigned expected_mask =
        (c->pair.request.aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR) ? 1U : 3U;

    if (w[E3_LAUNCH_W_MAGIC] != E3_LAUNCH_MAGIC)
        return 0;
    if (w[E3_LAUNCH_W_ERROR] != 0)
        return 0;
    if (!(w[E3_LAUNCH_W_FLAGS] & E3_LAUNCH_FROZEN))
        return 0;
    if (w[E3_LAUNCH_W_FLAGS] & E3_LAUNCH_ABORTED)
        return 0;
    if (w[E3_LAUNCH_W_INACTIVE_MASK] != expected_mask)
        return 0;
    if (!w[E3_LAUNCH_W_ISSUE] || w[E3_LAUNCH_W_ISSUE] != w[E3_LAUNCH_W_DONE])
        return 0;
    if (w[E3_LAUNCH_W_CANCEL] != 0)
        return 0;
    if (!w[E3_LAUNCH_W_VICTIM_ID] || !w[E3_LAUNCH_W_AGGRESSOR_ID])
        return 0;
    if (w[E3_LAUNCH_W_VICTIM_ID] != w[E3_LAUNCH_W_AGGRESSOR_ID])
        return 0;
    return 1;
}

static int controlled_round_ok(const struct idma_e3_controlled *c, unsigned r)
{
    const struct idma_experiment *v = &c->pair.result[r];
    const struct idma_e3_controlled_round *ctl = &c->controlled[r];
    unsigned expected_delay =
        c->pair_mode == E3_PAIR_ZERO ? 0U :
        (r ? 128U - c->pair.first_delay : c->pair.first_delay);
    unsigned expected_a_prime = c->prep_mode == E3_PREP_HOT ? 1U : 0U;

    if (!c->pair.sequence[r])
        return 0;
    if (v->status || !v->measurement_started || !v->measurement_complete ||
        !v->hpm_configured)
        return 0;
    if (!ctl->prep_complete || ctl->flush_status != 0 ||
        ctl->stage != E3_CTRL_STAGE_DONE)
        return 0;
    if (ctl->observer_magic != E3_OBSERVER_MAGIC_V6)
        return 0;
    if (c->pair.observation[r][0] != E3_BANK_V2 ||
        c->pair.observation[r][1] != 0)
        return 0;
    if (c->pair.observation[r][3] != expected_delay)
        return 0;
    if (!ctl->v_hw_cycles)
        return 0;

    if (!lane_exact_1(&v->victim))
        return 0;
    if (c->pair.request.aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR) {
        if (!stats_zero(&v->aggressor))
            return 0;
    } else {
        if (!lane_exact_1(&v->aggressor))
            return 0;
    }

    if (!lane_exact_1(&ctl->prime[0]))
        return 0; /* V prime */
    if (expected_a_prime) {
        if (!lane_exact_1(&ctl->prime[1]))
            return 0; /* hot A prime */
    } else {
        if (!stats_zero(&ctl->prime[1]))
            return 0; /* cold A prime */
    }

    if (!probe_exact_1(&ctl->probe))
        return 0;
    if (!launcher_ok(c, r))
        return 0;
    return 1;
}

static void print_round(const struct idma_e3_controlled *c, unsigned r)
{
    const struct idma_experiment *v = &c->pair.result[r];
    const struct idma_e3_controlled_round *ctl = &c->controlled[r];
    unsigned delay =
        c->pair_mode == E3_PAIR_ZERO ? 0U :
        (r ? 128U - c->pair.first_delay : c->pair.first_delay);
    unsigned i;

    printf("{\"delay\":%u,\"sequence\":%llu,\"status\":%d,\"hpm_configured\":%u,"
           "\"measurement_started\":%u,\"measurement_complete\":%u,"
           "\"victim_elapsed_ns\":%llu,\"snapshot_before_ns\":%llu,"
           "\"snapshot_after_ns\":%llu,\"drain_ns\":%llu,\"victim\":",
           delay, (unsigned long long)c->pair.sequence[r], v->status,
           v->hpm_configured, v->measurement_started, v->measurement_complete,
           (unsigned long long)v->victim_elapsed_ns,
           (unsigned long long)v->snapshot_before_ns,
           (unsigned long long)v->snapshot_after_ns,
           (unsigned long long)v->drain_ns);
    print_stats(&v->victim);
    printf(",\"aggressor\":");
    print_stats(&v->aggressor);
    printf(",\"e3_words\":[");
    for (i = 0; i < IDMA_E3_WORDS; i++)
        printf("%s%llu", i ? "," : "",
               (unsigned long long)c->pair.observation[r][i]);
    printf("],\"controlled\":{\"flush_status\":%d,\"prep_complete\":%u,"
           "\"stage\":%u,\"prime\":[",
           ctl->flush_status, ctl->prep_complete, ctl->stage);
    print_stats(&ctl->prime[0]);
    printf(",");
    print_stats(&ctl->prime[1]);
    printf("],\"probe\":");
    print_stats(&ctl->probe);
    printf(",\"launcher\":[");
    for (i = 0; i < IDMA_E3_CONTROLLED_WORDS; i++)
        printf("%s%llu", i ? "," : "",
               (unsigned long long)ctl->launcher[i]);
    printf("],\"observer_magic\":%llu,\"v_hw_cycles\":%llu}}",
           (unsigned long long)ctl->observer_magic,
           (unsigned long long)ctl->v_hw_cycles);
}

static void print_json(const struct idma_e3_controlled *c, int err, int attempted,
                       int success, const char *stage, unsigned aggressor,
                       unsigned prep, unsigned pair, unsigned first_delay,
                       unsigned timeout)
{
    unsigned i;

    printf("{\"schema\":\"idma.e3_controlled.v1\",\"success\":%s,"
           "\"stage\":\"%s\",\"errno\":%d,\"run_attempted\":%s,"
           "\"status\":%d,\"rounds_attempted\":%u,\"rounds_completed\":%u,"
           "\"scope\":{\"hpm\":\"GO_to_both_drained\","
           "\"observer\":\"V_window\"},"
           "\"input\":{\"victim_id\":0,\"aggressor_id\":%u,"
           "\"victim_pages\":1,\"aggressor_pages\":1,\"chunk_bytes\":64,"
           "\"transfers\":1,\"warmup\":0,\"timeout_ms\":%u,"
           "\"first_delay\":%u,\"prep_mode\":%u,\"prep\":\"%s\","
           "\"pair_mode\":%u,\"pair\":\"%s\",\"flags\":%u},\"mapping\":[",
           success ? "true" : "false", stage, err,
           attempted ? "true" : "false", c->pair.status,
           c->pair.rounds_attempted, c->pair.rounds_completed,
           aggressor, timeout, first_delay, prep,
           prep == E3_PREP_HOT ? "hot" : "cold",
           pair, pair == E3_PAIR_ZERO ? "zero" : "complementary",
           c->pair.request.flags);
    for (i = 0; i < 8; i++)
        printf("%s%llu", i ? "," : "",
               (unsigned long long)c->pair.mapping[i]);
    printf("],\"rounds\":[");
    for (i = 0; i < 2; i++) {
        if (i)
            putchar(',');
        print_round(c, i);
    }
    puts("]}");
}

static void usage(void)
{
    puts("Usage: idma_e3_controlled [--aggressor none|1] [--prep cold|hot]\n"
         "       [--pair complementary|zero] [--first-delay 0|128]\n"
         "       [--timeout-ms 1..60000]\n"
         "Fixed /dev/idma0, V/A pages 1, chunk 64 bytes, transfers 1, warmup 0.\n"
         "Zero pair requires first-delay 0. Solo (--aggressor none) is cold only.\n"
         "One controlled ioctl; two rounds; no retry. Schema idma.e3_controlled.v1.");
}

int main(int argc, char **argv)
{
    struct idma_e3_controlled c = {0};
    struct idma_experiment_caps caps = {0};
    unsigned aggressor = 1U, prep = E3_PREP_COLD;
    unsigned pair = E3_PAIR_COMPLEMENTARY;
    unsigned first_delay = 0U, timeout = E3_DEFAULT_TIMEOUT_MS;
    int fd = -1, rc = -1, err = 0, attempted = 0, success = 0;
    const char *stage = "arguments";
    unsigned seen = 0;

    if (argc == 2 && !strcmp(argv[1], "--help")) {
        usage();
        return 0;
    }

    for (int i = 1; i < argc; i += 2) {
        const char *names[] = {
            "--aggressor", "--prep", "--pair", "--first-delay", "--timeout-ms"
        };
        unsigned k, n;

        for (k = 0; k < 5; k++)
            if (!strcmp(argv[i], names[k]))
                break;
        if (k == 5 || i + 1 == argc || (seen & (1U << k))) {
            err = EINVAL;
            goto output;
        }
        seen |= 1U << k;

        if (k == 0) {
            if (!strcmp(argv[i + 1], "none"))
                n = IDMA_EXPERIMENT_NO_AGGRESSOR;
            else if (!strcmp(argv[i + 1], "1"))
                n = 1U;
            else {
                err = EINVAL;
                goto output;
            }
        } else if (k == 1) {
            if (!strcmp(argv[i + 1], "cold"))
                n = E3_PREP_COLD;
            else if (!strcmp(argv[i + 1], "hot"))
                n = E3_PREP_HOT;
            else {
                err = EINVAL;
                goto output;
            }
        } else if (k == 2) {
            if (!strcmp(argv[i + 1], "complementary"))
                n = E3_PAIR_COMPLEMENTARY;
            else if (!strcmp(argv[i + 1], "zero"))
                n = E3_PAIR_ZERO;
            else {
                err = EINVAL;
                goto output;
            }
        } else {
            unsigned max = (k == 3) ? 128U : 60000U;
            if (strict_number(argv[i + 1], max, &n)) {
                err = EINVAL;
                goto output;
            }
        }

        switch (k) {
        case 0: aggressor = n; break;
        case 1: prep = n; break;
        case 2: pair = n; break;
        case 3: first_delay = n; break;
        case 4: timeout = n; break;
        }
    }

    if ((aggressor != 1U && aggressor != IDMA_EXPERIMENT_NO_AGGRESSOR) ||
        prep > E3_PREP_HOT || pair > E3_PAIR_ZERO ||
        (first_delay != 0U && first_delay != 128U) ||
        (pair == E3_PAIR_ZERO && first_delay != 0U) ||
        (aggressor == IDMA_EXPERIMENT_NO_AGGRESSOR && prep != E3_PREP_COLD) ||
        timeout < 1U || timeout > 60000U) {
        err = EINVAL;
        goto output;
    }

    c.pair.abi_version = 1U;
    c.pair.struct_size = sizeof(c.pair);
    c.pair.first_delay = first_delay;
    c.pair.request = (struct idma_experiment) {
        .abi_version = IDMA_EXPERIMENT_ABI,
        .struct_size = sizeof(struct idma_experiment),
        .aggressor_id = aggressor,
        .victim_pages = E3_FIXED_VICTIM_PAGES,
        .aggressor_pages = E3_FIXED_AGGRESSOR_PAGES,
        .chunk_bytes = E3_FIXED_CHUNK_BYTES,
        .run_timeout_ms = timeout,
        .victim_transfers = E3_FIXED_TRANSFERS,
        .warmup_transfers = E3_FIXED_WARMUP,
    };
    c.prep_mode = prep;
    c.pair_mode = pair;

    stage = "open";
    fd = open("/dev/idma0", O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        err = errno;
        goto output;
    }

    stage = "capabilities";
    rc = ioctl(fd, IDMA_IOC_EXPERIMENT_CAPS, &caps);
    if (rc) {
        err = rc < 0 ? errno : EPROTO;
        goto output;
    }
    if (caps.abi_version != IDMA_EXPERIMENT_ABI ||
        caps.struct_size != sizeof(caps) ||
        !(caps.capabilities & IDMA_EXPERIMENT_CAP_E3_CONTROLLED) ||
        caps.device_id != 0 || caps.did != 10 || caps.page_bytes != 4096) {
        err = EOPNOTSUPP;
        goto output;
    }

    stage = "run";
    attempted = 1;
    rc = ioctl(fd, IDMA_IOC_RUN_E3_CONTROLLED, &c);
    if (rc)
        err = rc < 0 ? errno : EPROTO;
    success = !rc && !c.pair.status &&
              c.pair.rounds_attempted == 2 && c.pair.rounds_completed == 2;
    for (unsigned r = 0; r < 2 && success; r++) {
        if (!controlled_round_ok(&c, r))
            success = 0;
    }
    if (!success && !err)
        err = EPROTO;

output:
    if (fd >= 0 && close(fd) && !err) {
        err = errno;
        success = 0;
        stage = "close";
    }
    print_json(&c, err, attempted, success, stage,
               aggressor, prep, pair, first_delay, timeout);
    if (err)
        fprintf(stderr, "idma_e3_controlled: %s: %s\n",
                stage, strerror(err));
    return success ? 0 : 1;
}
```

---

## New file: `tests/e3_controlled_cli_stub.c`

```c
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <linux/idma.h>

#define E3_OBSERVER_MAGIC_V6 0x49444d4145330006ULL
#define E3_BANK_V2           0x49444d4145330002ULL
#define E3_LAUNCH_MAGIC      0x4c41554e43483130ULL
#define E3_PREP_COLD         0U
#define E3_PREP_HOT          1U
#define E3_PAIR_COMPLEMENTARY 0U
#define E3_PAIR_ZERO          1U

static int run_calls;

int __wrap_open(const char *path, int flags, ...)
{
    (void)flags;
    if (getenv("E3_EXPECT_NO_OPEN"))
        assert(0 && "open on invalid input");
    assert(!strcmp(path, "/dev/idma0"));
    return 7;
}

int __wrap_close(int fd)
{
    const char *mode = getenv("E3_MOCK");

    assert(fd == 7);
    if (mode && !strcmp(mode, "closefail")) {
        errno = EIO;
        return -1;
    }
    return 0;
}

int __wrap_ioctl(int fd, unsigned long op, ...)
{
    const char *mode = getenv("E3_MOCK");
    va_list ap;
    void *arg;

    if (getenv("E3_EXPECT_NO_IOCTL"))
        assert(0 && "ioctl on invalid input");
    assert(fd == 7);

    va_start(ap, op);
    arg = va_arg(ap, void *);
    va_end(ap);

    if (op == IDMA_IOC_EXPERIMENT_CAPS) {
        struct idma_experiment_caps *c = arg;

        *c = (struct idma_experiment_caps) {
            .abi_version = IDMA_EXPERIMENT_ABI,
            .struct_size = sizeof(*c),
            .capabilities = IDMA_EXPERIMENT_CAP_E3_CONTROLLED,
            .device_id = 0,
            .did = 10,
            .page_bytes = 4096,
        };
        if (mode && !strcmp(mode, "old"))
            c->capabilities = 0;
        if (mode && !strcmp(mode, "wrongcap"))
            c->capabilities = IDMA_EXPERIMENT_CAP_E3_QUEUE_PAIR;
        if (mode && !strcmp(mode, "wrongdid"))
            c->did = 11;
        if (mode && !strcmp(mode, "wrongpage"))
            c->page_bytes = 8192;
        if (mode && !strcmp(mode, "wrongdevice"))
            c->device_id = 1;
        if (mode && !strcmp(mode, "wrongabi"))
            c->abi_version = 2;
        if (mode && !strcmp(mode, "wrongsize"))
            c->struct_size = sizeof(*c) - 1;
        return 0;
    }

    assert(op == IDMA_IOC_RUN_E3_CONTROLLED);
    assert(++run_calls == 1);

    if (mode && !strcmp(mode, "interrupt")) {
        errno = EINTR;
        return -1;
    }
    if (mode && !strcmp(mode, "runfail")) {
        errno = EIO;
        return -1;
    }

    struct idma_e3_controlled *c = arg;

    c->pair.rounds_attempted = 2;
    c->pair.rounds_completed = 2;

    for (int r = 0; r < 2; r++) {
        unsigned a_present =
            c->pair.request.aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;
        struct idma_e3_controlled_round *ctl = &c->controlled[r];
        unsigned a_prime;

        c->pair.sequence[r] = (unsigned long long)(r + 1);
        c->pair.result[r] = c->pair.request;
        c->pair.result[r].measurement_started = 1;
        c->pair.result[r].measurement_complete = 1;
        c->pair.result[r].hpm_configured = 1;

        c->pair.result[r].victim.submitted = 1;
        c->pair.result[r].victim.completed = 1;
        c->pair.result[r].victim.completed_bytes = 64;

        c->pair.result[r].aggressor.submitted = a_present;
        c->pair.result[r].aggressor.completed = a_present;
        c->pair.result[r].aggressor.completed_bytes = a_present ? 64 : 0;

        c->pair.observation[r][0] = E3_BANK_V2;
        c->pair.observation[r][1] = 0;
        c->pair.observation[r][2] = 0; /* no trigger coverage in host model */
        c->pair.observation[r][3] =
            (c->pair_mode == E3_PAIR_ZERO) ? 0 :
            (r ? 128 - c->pair.first_delay : c->pair.first_delay);

        ctl->flush_status = 0;
        ctl->prep_complete = 1;
        ctl->stage = 8;

        ctl->prime[0].submitted = 1;
        ctl->prime[0].completed = 1;
        ctl->prime[0].completed_bytes = 64;

        a_prime = c->prep_mode == E3_PREP_HOT ? 1 : 0;
        ctl->prime[1].submitted = a_prime;
        ctl->prime[1].completed = a_prime;
        ctl->prime[1].completed_bytes = a_prime ? 64 : 0;

        ctl->probe.submitted = 1;
        ctl->probe.completed = 1;
        ctl->probe.completed_bytes = 64;

        ctl->observer_magic = E3_OBSERVER_MAGIC_V6;
        ctl->v_hw_cycles = 1000 + r;

        ctl->launcher[0] = E3_LAUNCH_MAGIC;
        ctl->launcher[1] = 0;
        ctl->launcher[2] = (1ULL << 2); /* FROZEN */
        ctl->launcher[3] = a_present ? 3 : 1;
        ctl->launcher[4] = 1;
        ctl->launcher[5] = 1;
        ctl->launcher[6] = 0;
        ctl->launcher[7] = 10;
        ctl->launcher[8] = 10;
    }

    if (mode) {
        if (!strcmp(mode, "partial")) {
            c->pair.rounds_completed = 1;
        } else if (!strcmp(mode, "bad_magic")) {
            c->controlled[1].observer_magic = 0x49444d4145330001ULL;
        } else if (!strcmp(mode, "bad_bank")) {
            c->pair.observation[1][0] = 0x49444d4145330001ULL;
        } else if (!strcmp(mode, "bad_error")) {
            c->pair.observation[1][1] = 1;
        } else if (!strcmp(mode, "bad_d")) {
            c->pair.observation[1][3] = 99;
        } else if (!strcmp(mode, "bad_stage")) {
            c->controlled[1].stage = 7;
        } else if (!strcmp(mode, "bad_prep")) {
            c->controlled[1].prep_complete = 0;
        } else if (!strcmp(mode, "bad_flush")) {
            c->controlled[1].flush_status = -EIO;
        } else if (!strcmp(mode, "bad_vcycles")) {
            c->controlled[1].v_hw_cycles = 0;
        } else if (!strcmp(mode, "bad_prime")) {
            c->controlled[1].prime[1].submitted = 2;
        } else if (!strcmp(mode, "bad_probe")) {
            c->controlled[1].probe.iotlb_miss = 1;
        } else if (!strcmp(mode, "bad_launcher_magic")) {
            c->controlled[1].launcher[0] = 0;
        } else if (!strcmp(mode, "bad_launcher_id")) {
            c->controlled[1].launcher[7] = 0;
        } else if (!strcmp(mode, "bad_launcher_mask")) {
            c->controlled[1].launcher[3] = 2;
        } else if (!strcmp(mode, "bad_launcher_issue")) {
            c->controlled[1].launcher[4] = 2;
        } else if (!strcmp(mode, "bad_launcher_cancel")) {
            c->controlled[1].launcher[6] = 1;
        } else if (!strcmp(mode, "bad_round_status")) {
            c->pair.result[1].status = -EIO;
        } else if (!strcmp(mode, "bad_measurement")) {
            c->pair.result[1].measurement_complete = 0;
        } else if (!strcmp(mode, "bad_hpm")) {
            c->pair.result[1].hpm_configured = 0;
        } else if (!strcmp(mode, "bad_victim")) {
            c->pair.result[1].victim.completed = 0;
        } else if (!strcmp(mode, "bad_aggressor")) {
            c->pair.result[1].aggressor.data_errors = 1;
        } else if (!strcmp(mode, "bad_sequence")) {
            c->pair.sequence[1] = 0;
        } else if (!strcmp(mode, "bad_pair_status")) {
            c->pair.status = -EIO;
        } else if (!strcmp(mode, "nocoverage")) {
            c->pair.observation[1][2] = 0; /* explicit no-trigger coverage */
        }
    }

    return 0;
}
```

---

## New file: `tests/test_e3_controlled_cli.py`

```python
#!/usr/bin/env python3
import json, os, subprocess, tempfile
from pathlib import Path

root = Path(__file__).resolve().parents[1]

with tempfile.TemporaryDirectory() as td:
    t = Path(td)
    (t / 'linux').mkdir()
    (t / 'linux/idma.h').write_bytes(
        (root / 'linux/include/uapi/linux/idma.h').read_bytes()
    )
    exe = t / 'e3_controlled'

    subprocess.run([
        'cc', '-std=gnu11', '-O2', '-Wall', '-Wextra', '-Werror',
        '-I', str(t),
        '-Wl,--wrap=open', '-Wl,--wrap=ioctl', '-Wl,--wrap=close',
        str(root / 'bench/idma_e3_controlled.c'),
        str(root / 'tests/e3_controlled_cli_stub.c'),
        '-o', str(exe),
    ], check=True)

    count = 0

    def run(args, ok, mode='', expect_no_open=False):
        global count
        env = {**os.environ, 'E3_MOCK': mode}
        if expect_no_open:
            env['E3_EXPECT_NO_OPEN'] = '1'
            env['E3_EXPECT_NO_IOCTL'] = '1'

        p = subprocess.run([str(exe), *args], env=env,
                           capture_output=True, text=True, timeout=3)
        if not p.stdout.strip():
            raise AssertionError((args, p.returncode, p.stdout, p.stderr))

        d = json.loads(p.stdout)
        assert (p.returncode == 0) == ok, (args, p.stdout, p.stderr)
        assert d['schema'] == 'idma.e3_controlled.v1'
        assert d['success'] == ok
        assert len(d['rounds']) == 2
        assert all(len(r['e3_words']) == 256 for r in d['rounds'])
        assert all(len(r['controlled']['launcher']) == 32 for r in d['rounds'])
        assert d['scope']['hpm'] == 'GO_to_both_drained'
        assert d['scope']['observer'] == 'V_window'
        count += 1
        return d

    p = subprocess.run([str(exe), '--help'], capture_output=True, text=True)
    assert p.returncode == 0 and 'Usage:' in p.stdout

    # Positive all modes/delay.
    for aggressor in ('none', '1'):
        for prep in ('cold', 'hot'):
            if aggressor == 'none' and prep != 'cold':
                continue
            for pair in ('complementary', 'zero'):
                for delay in ('0', '128'):
                    if pair == 'zero' and delay != '0':
                        continue
                    args = ['--aggressor', aggressor, '--prep', prep,
                            '--pair', pair, '--first-delay', delay]
                    d = run(args, True)
                    expected = [int(delay),
                                0 if pair == 'zero' else 128 - int(delay)]
                    assert [r['delay'] for r in d['rounds']] == expected, \
                        (args, d['rounds'])
                    assert all(
                        r['controlled']['observer_magic'] == 0x49444d4145330006
                        for r in d['rounds']
                    )
                    assert all(
                        r['e3_words'][0] == 0x49444d4145330002
                        for r in d['rounds']
                    )
                    # No-trigger coverage is retained as executed data,
                    # not required for success.
                    assert all(r['e3_words'][2] == 0 for r in d['rounds'])

    run([], True, 'nocoverage')

    # Negative input must not open or issue ioctls.
    negative = [
        ['--bad'],
        ['--prep'],
        ['--prep', 'cold', '--prep', 'hot'],
        ['--first-delay', '-1'],
        ['--first-delay', '1'],
        ['--first-delay', '129'],
        ['--timeout-ms', '0'],
        ['--timeout-ms', '60001'],
        ['--timeout-ms', '-1'],
        ['--timeout-ms', ' 1'],
        ['--timeout-ms', '1 '],
        ['--timeout-ms', '1x'],
        ['--timeout-ms', '9999999999999999999'],
        ['--timeout-ms', '0x10'],
        ['--aggressor', '0'],
        ['--aggressor', 'none', '--prep', 'hot'],
        ['--pair', 'zero', '--first-delay', '128'],
        ['--pair', 'bad'],
        ['--prep', 'warm'],
        ['--help', 'x'],
    ]
    for args in negative:
        d = run(args, False, expect_no_open=True)
        assert d['run_attempted'] is False, args
        assert d['stage'] == 'arguments', args

    # Old/wrong capabilities return without run.
    for mode in ('old', 'wrongcap', 'wrongdid', 'wrongpage',
                 'wrongdevice', 'wrongabi', 'wrongsize'):
        d = run([], False, mode)
        assert d['run_attempted'] is False, mode
        assert d['errno'] in (95, 22, 5), (mode, d['errno'])

    # EINTR is not retried.
    d = run([], False, 'interrupt')
    assert d['run_attempted'] is True
    assert d['errno'] == 4  # EINTR

    # Partial failed round raw data is preserved.
    d = run([], False, 'partial')
    assert d['run_attempted'] is True
    assert d['rounds_attempted'] == 2 and d['rounds_completed'] == 1
    assert len(d['rounds'][1]['e3_words']) == 256

    # Metadata failures produce EPROTO/EIO.
    metadata_modes = [
        'bad_magic', 'bad_bank', 'bad_error', 'bad_d', 'bad_stage',
        'bad_prep', 'bad_flush', 'bad_vcycles', 'bad_prime', 'bad_probe',
        'bad_launcher_magic', 'bad_launcher_id', 'bad_launcher_mask',
        'bad_launcher_issue', 'bad_launcher_cancel', 'bad_round_status',
        'bad_measurement', 'bad_hpm', 'bad_victim', 'bad_aggressor',
        'bad_sequence', 'bad_pair_status',
    ]
    for mode in metadata_modes:
        d = run([], False, mode)
        assert d['run_attempted'] is True, mode
        assert d['errno'] in (71, 5), (mode, d['errno'])

    # Close failure is reported after a successful run.
    d = run([], False, 'closefail')
    assert d['run_attempted'] is True
    assert d['stage'] == 'close'
    assert d['errno'] == 5  # EIO

    print(f'PASS E3 controlled CLI {count} cases (host ioctl model only)')
```

---

## New file: `tests/board-e3-controlled.sh`

```sh
#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
# Fixed finite controlled E3 board runner.
#
# Usage: platform-e3-controlled short|main|all [OUTPUT_DIR]
#   short: three short pairs
#   main:  eight formal pairs. Run only after short passed on user/host.
#   all:   short, then main only if short passed.
#
# OUTPUT_DIR is created new; it must not already exist.
# Default OUTPUT_DIR is /tmp/e3-controlled-$$.
#
# No dmesg clear, no network, no SD burn, no ioctl retry, no hidden failure.
set -u

PATH=/sbin:/bin:/usr/sbin:/usr/bin
export PATH

usage() {
    cat <<'EOF'
Usage: platform-e3-controlled short|main|all [OUTPUT_DIR]
  short  3 short pairs
  main   8 formal pairs (only after short passed on user/host)
  all    short, then main if short passed
OUTPUT_DIR must not exist. Default: /tmp/e3-controlled-$$.
Matrix:
  short 1: --aggressor none --prep cold --pair complementary --first-delay 0
  short 2: --aggressor 1    --prep hot  --pair complementary --first-delay 0
  short 3: --aggressor 1    --prep cold --pair zero          --first-delay 0
  main 1:  --aggressor 1    --prep cold --pair zero          --first-delay 0
  main 2:  --aggressor 1    --prep cold --pair complementary --first-delay 0
  main 3:  --aggressor 1    --prep hot  --pair complementary --first-delay 0
  main 4:  --aggressor 1    --prep cold --pair complementary --first-delay 128
  main 5:  --aggressor 1    --prep cold --pair zero          --first-delay 0
  main 6:  --aggressor 1    --prep cold --pair complementary --first-delay 0
  main 7:  --aggressor 1    --prep hot  --pair complementary --first-delay 128
  main 8:  --aggressor 1    --prep cold --pair complementary --first-delay 128
EOF
}

fatal() {
    echo "E3CTL: fatal: $*" >&2
    exit 2
}

[ $# -ge 1 ] && [ $# -le 2 ] || { usage >&2; exit 2; }
mode=$1
case "$mode" in
    short|main|all) ;;
    *) usage >&2; exit 2 ;;
esac

if [ $# -eq 2 ]; then
    OUT=$2
else
    OUT=/tmp/e3-controlled-$$
fi

if [ -e "$OUT" ]; then
    fatal "output directory already exists: $OUT"
fi
mkdir "$OUT" || fatal "mkdir failed: $OUT"

echo "mode=$mode" > "$OUT/input.txt"
echo "output=$OUT" >> "$OUT/input.txt"

cat > "$OUT/matrix.txt" <<'EOF'
short:
1 --aggressor none --prep cold --pair complementary --first-delay 0
2 --aggressor 1 --prep hot --pair complementary --first-delay 0
3 --aggressor 1 --prep cold --pair zero --first-delay 0
main:
1 --aggressor 1 --prep cold --pair zero --first-delay 0
2 --aggressor 1 --prep cold --pair complementary --first-delay 0
3 --aggressor 1 --prep hot --pair complementary --first-delay 0
4 --aggressor 1 --prep cold --pair complementary --first-delay 128
5 --aggressor 1 --prep cold --pair zero --first-delay 0
6 --aggressor 1 --prep cold --pair complementary --first-delay 0
7 --aggressor 1 --prep hot --pair complementary --first-delay 128
8 --aggressor 1 --prep cold --pair complementary --first-delay 128
EOF

save_dmesg() {
    dmesg > "$OUT/dmesg.raw" 2>/dev/null || true
    awk '
        /^\[[[:space:]]*[0-9]+\.[0-9]+\]/ {
            key=$0
            sub(/^\[[[:space:]]*[0-9]+\.[0-9]+\][[:space:]]*/, "", key)
            if (seen[key]++) next
        }
        { print }
    ' "$OUT/dmesg.raw" > "$OUT/dmesg.txt"
    echo "E3CTL: dmesg=$OUT/dmesg.txt" >&2
    cat "$OUT/dmesg.txt" >&2
}

save_taint() {
    taint=$(cat /proc/sys/kernel/tainted 2>/dev/null || echo unknown)
    echo "$taint" > "$OUT/taint.txt"
    echo "E3CTL: taint=$taint" >&2
}

cleanup() {
    rc=$?
    trap - EXIT HUP INT TERM
    save_dmesg
    save_taint
    echo "E3CTL: final_exit=$rc" >&2
    exit "$rc"
}
trap cleanup EXIT HUP INT TERM

run_case() {
    phase=$1
    index=$2
    shift 2

    stdout_file="$OUT/$phase-$index.json"
    stderr_file="$OUT/$phase-$index.err"
    before_taint=$(cat /proc/sys/kernel/tainted 2>/dev/null || echo unknown)

    echo "E3CTL: phase=$phase case=$index begin taint=$before_taint args=$*" >&2
    idma_e3_controlled "$@" > "$stdout_file" 2> "$stderr_file"
    rc=$?
    after_taint=$(cat /proc/sys/kernel/tainted 2>/dev/null || echo unknown)
    echo "E3CTL: phase=$phase case=$index exit=$rc taint=$after_taint" >&2

    cat "$stdout_file"
    if [ -s "$stderr_file" ]; then
        cat "$stderr_file" >&2
    fi

    if [ "$rc" -ne 0 ]; then
        echo "E3CTL: phase=$phase case=$index failed rc=$rc" >&2
        return "$rc"
    fi
    return 0
}

run_short() {
    run_case short 1 --aggressor none --prep cold --pair complementary --first-delay 0 || return $?
    run_case short 2 --aggressor 1 --prep hot --pair complementary --first-delay 0 || return $?
    run_case short 3 --aggressor 1 --prep cold --pair zero --first-delay 0 || return $?
    return 0
}

run_main() {
    run_case main 1 --aggressor 1 --prep cold --pair zero --first-delay 0 || return $?
    run_case main 2 --aggressor 1 --prep cold --pair complementary --first-delay 0 || return $?
    run_case main 3 --aggressor 1 --prep hot --pair complementary --first-delay 0 || return $?
    run_case main 4 --aggressor 1 --prep cold --pair complementary --first-delay 128 || return $?
    run_case main 5 --aggressor 1 --prep cold --pair zero --first-delay 0 || return $?
    run_case main 6 --aggressor 1 --prep cold --pair complementary --first-delay 0 || return $?
    run_case main 7 --aggressor 1 --prep hot --pair complementary --first-delay 128 || return $?
    run_case main 8 --aggressor 1 --prep cold --pair complementary --first-delay 128 || return $?
    return 0
}

case "$mode" in
    short)
        run_short || exit $?
        ;;
    main)
        echo "E3CTL: warning: main requires that short already passed on user/host" >&2
        run_main || exit $?
        ;;
    all)
        run_short || exit $?
        echo "E3CTL: short passed; starting main" >&2
        run_main || exit $?
        ;;
esac

echo "E3CTL: complete ok mode=$mode output=$OUT"
exit 0
```

---

## New file: `configs/migration-4dma-e3-controlled.json`

```json
{
  "description": "DS50/50, finite controlled E3: fixed 64B single-transfer pairs, short 3 + formal 8, no hardware parameter changes.",
  "board": "genesys2",
  "num_dma": 4,
  "device_ids": [10, 11, 12, 13],
  "iotlb_entries": 16,
  "ds_delay_input": 50,
  "ds_delay_output": 50,
  "cpu_hz": 50000000,
  "timebase_hz": 25000000
}
```

---

## Diff: `scripts/platform.py`

```diff
@@ -84,6 +84,8 @@ def bench():
     run([CC+'gcc', '-O2', '-static', '-Wall', '-Wextra',
          '-I', uapi, ROOT/'bench/idma_e3_pair.c', '-o', dest/'idma_e3_pair'])
+    run([CC+'gcc', '-O2', '-static', '-Wall', '-Wextra',
+         '-I', uapi, ROOT/'bench/idma_e3_controlled.c', '-o', dest/'idma_e3_controlled'])
     run([CC+'gcc', '-O2', '-static', '-Wall', '-Wextra',
          ROOT/'bench/platform_timeout.c', '-o', dest/'platform-timeout'])
@@ -247,7 +249,7 @@ def main():
     global BUILD_CONTEXT
     parser=argparse.ArgumentParser()
     parser.add_argument('action',choices=['doctor','check','bench','rootfs','kernel','firmware','software','hpm-test','fpga','bundle'])
-    parser.add_argument('--profile',default='migration-4dma',choices=['migration-4dma','baseline-4dma','migration-4dma-scfix','migration-4dma-observe','migration-4dma-batch','baseline-4dma-batch','migration-4dma-path','migration-4dma-e3','migration-4dma-e3-queue'])
+    parser.add_argument('--profile',default='migration-4dma',choices=['migration-4dma','baseline-4dma','migration-4dma-scfix','migration-4dma-observe','migration-4dma-batch','baseline-4dma-batch','migration-4dma-path','migration-4dma-e3','migration-4dma-e3-queue','migration-4dma-e3-controlled'])
     parser.add_argument('--jobs',type=int,default=12)
     args=parser.parse_args()
```

---

## Diff: `scripts/rootfs.py`

```diff
@@ -8,6 +8,8 @@
     'idma_experiment': 'out/bench/idma_experiment',
     'platform-smoke': 'tests/board-smoke.sh',
     'platform-timeout': 'out/bench/platform-timeout',
+    'idma_e3_controlled': 'out/bench/idma_e3_controlled',
+    'platform-e3-controlled': 'tests/board-e3-controlled.sh',
 }
 
 def read_newc(data):
```

---

## Assumptions / unresolved dependency

1. **Launcher raw-word layout is not in the supplied UAPI patch.**  
   `E3_LAUNCH_W_*`, `E3_LAUNCH_MAGIC`, `E3_LAUNCH_FROZEN`, and `E3_LAUNCH_ABORTED` in `bench/idma_e3_controlled.c` are centralized assumptions. They must be reconciled with the separately applied `driver-0.patch` launcher record. The host stub mirrors the same assumptions.

2. **`prep_mode` / `pair_mode` numeric values.**  
   I used:
   - `prep_mode`: `0 = cold`, `1 = hot`
   - `pair_mode`: `0 = complementary`, `1 = zero`
   
   If the driver uses different enum values, change the `E3_PREP_*` / `E3_PAIR_*` macros in the CLI and stub.

3. **UAPI patch must be applied before compiling/running tests.**  
   The new CLI, stub, and tests depend on `IDMA_E3_CONTROLLED_WORDS`, `struct idma_e3_controlled`, `IDMA_EXPERIMENT_CAP_E3_CONTROLLED`, and `IDMA_IOC_RUN_E3_CONTROLLED` from `driver-0.patch`. The test script copies the current UAPI header; it does not apply the patch.

4. **No hardware run was performed.**  
   All host-test statements are proposals; actual results must be established by the parent after applying the UAPI patch and running the test/build commands.

5. **Runner calls `idma_e3_controlled` from PATH.**  
   `rootfs.py` installs the CLI as `/usr/bin/idma_e3_controlled` and the runner as `/usr/bin/platform-e3-controlled`.

---

## Suggested acceptance checks

1. Apply `out/e3-controlled-20260918/driver-0.patch` to `linux/include/uapi/linux/idma.h`.
2. Host CLI model:
   ```sh
   python3 tests/test_e3_controlled_cli.py
   ```
   Expected: `PASS E3 controlled CLI ... cases (host ioctl model only)`.
3. Build integration:
   ```sh
   python3 scripts/platform.py bench
   python3 scripts/rootfs.py
   ```
   Expected: `out/bench/idma_e3_controlled` exists; `out/rootfs.cpio` contains `/usr/bin/idma_e3_controlled` and `/usr/bin/platform-e3-controlled`.
4. Profile acceptance:
   ```sh
   python3 scripts/platform.py --profile migration-4dma-e3-controlled check
   ```
   Expected: pinned component check passes; new config matches existing four-DMA wiring and clock constraints.
5. Board runner syntax check:
   ```sh
   sh -n tests/board-e3-controlled.sh
   ```
   Expected: no syntax errors under POSIX shell / BusyBox `ash`.
6. On hardware, start with:
   ```sh
   platform-e3-controlled short /tmp/e3-short-$$
   ```
   Only after `short` passes on user/host, run:
   ```sh
   platform-e3-controlled main /tmp/e3-main-$$
   ```
   or use `all` for the automatic short gate.
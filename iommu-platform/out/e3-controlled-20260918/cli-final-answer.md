## Findings and interpretation

I traced the reviewer's list against the three sources that define the record (`dma_controlled_launcher.sv` read indices, `linux/include/uapi/linux/idma.h`, and the CLI decode table in `bench/idma_e3_controlled.c`). Every requested tightening maps to a real readback field:

- `launcher_ok` (`bench/idma_e3_controlled.c:137-191`) currently only tests `FROZEN` set / `RUNNING|ABORTED` clear, masks the cancel word, and only *inequality* for counts. The reviewer wants the exact terminal record: `status == 4`, `word18 seen_held == word15 issued == word16 done == word19 expected ∈ {1,3}`, `word24/26 == 1`, `word25/27 ∈ {1,0}`, `word28 has_a == with_a`, `word29/30/31 == 0`, `word17 == 0` (unmasked), V `word11 == word13 != 0`, A `word12 == word14 != 0` (or both 0), select `word20/21/22/23` per A-present, and `word22 == mapping[4]`.
- `victim_did`/`aggressor_did` live in `struct idma_experiment.result[]` (`idma.h:67-68`), which is `pair.result[r]`; they are currently neither validated nor printed.
- The stub already fabricates the whole `struct idma_e3_controlled`, so the new required fields (seen/has_a/select_iova/result DIDs) can be populated for both A and solo, and each new check gets a dedicated mutant that would have *passed* the old code.
- `tests/test_rootfs.py` fails because `scripts/rootfs.py:8-15` already lists `idma_e3_controlled` and `platform-e3-controlled`, but the test fixtures omit both → the supplied `out/e3-controlled-20260918/rootfs-test.log` `FileNotFoundError`.

I did **not** touch UAPI, the driver, RTL, the matrix, or `scripts/rootfs.py`.

---

## Complete changed files

### 1) `bench/idma_e3_controlled.c`

```c
// SPDX-License-Identifier: GPL-2.0
/* Finite controlled E3 CLI: fixed 64 B, one ioctl owns both rounds and mapping lifetime. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <linux/idma.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

/* Observer front-end revision and E3 bank revision (idma.c). */
#define E3_OBSERVER_MAGIC_V6 0x49444d414f425306ULL
#define E3_BANK_V2           0x49444d4145330002ULL
/* Separate DMA0 launcher register-block magic (idma.c / dma_controlled_launcher.sv). */
#define E3_LAUNCH_MAGIC      0x49444d414c4e4301ULL
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
/* Fixed device DID advertised by IDMA_IOC_EXPERIMENT_CAPS on this platform. */
#define E3_FIXED_VICTIM_DID      10U

/*
 * Raw launcher record word indices, authoritative from
 * cva6/corev_apu/fpga/src/dma_controlled_launcher.sv (c0 index -> c8 data)
 * as read by idma_launcher_read_words() in linux/drivers/misc/idma.c.
 * index 0 magic, 1 error, 2 status_word, 3 ticks, 4..6 go/select stamps,
 * 7/8 issue ts V/A, 9/10 done ts V/A, 11/12 issue_id V/A, 13/14 done_id V/A,
 * 15 issued_mask, 16 done_mask, 17 cancelled_mask, 18 seen_held, 19 expected_mask,
 * 20 selected_id, 21 selected_did, 22 selected_iova, 23 selected_write,
 * 24/25 issue_count V/A, 26/27 done_count V/A, 28 has_a, 29 held, 30 active, 31 zero.
 */
#define E3_LW_MAGIC        0U
#define E3_LW_ERROR        1U
#define E3_LW_STATUS       2U
#define E3_LW_ISSUE_ID_V   11U
#define E3_LW_ISSUE_ID_A   12U
#define E3_LW_DONE_ID_V    13U
#define E3_LW_DONE_ID_A    14U
#define E3_LW_ISSUED_MASK  15U
#define E3_LW_DONE_MASK    16U
#define E3_LW_CANCEL_MASK  17U
#define E3_LW_SEEN         18U
#define E3_LW_EXPECTED     19U
#define E3_LW_SELECT_ID    20U
#define E3_LW_SELECT_DID   21U
#define E3_LW_SELECT_IOVA  22U
#define E3_LW_SELECT_WRITE 23U
#define E3_LW_ISSUE_CNT_V  24U
#define E3_LW_ISSUE_CNT_A  25U
#define E3_LW_DONE_CNT_V   26U
#define E3_LW_DONE_CNT_A   27U
#define E3_LW_HAS_A        28U
#define E3_LW_HELD         29U
#define E3_LW_ACTIVE       30U
#define E3_LW_ZERO         31U

/* status_word bit assignments (dma_controlled_launcher.sv).  A successful
 * terminal record has exactly FROZEN set; RUNNING/ABORTED and the sticky
 * error field status[63:32] must all be clear. */
#define E3_LAUNCH_STATUS_RUNNING (1ULL << 1)
#define E3_LAUNCH_STATUS_FROZEN  (1ULL << 2)
#define E3_LAUNCH_STATUS_ABORTED (1ULL << 3)

/* Reserved A-selection descriptor identity, mirrored from the launcher RTL
 * localparams A_SEL_ID / A_DID (dma_controlled_launcher.sv).  These are the
 * hardware reservation values, not CLI inputs. */
#define E3_LW_A_SELECT_ID  4U
#define E3_LW_A_SELECT_DID 11U

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
           "\"active_cycles\":%llu,\"iotlb_miss\":%llu,\"s1_ptw\":%llu,"
           "\"s2_ptw\":%llu,\"refill_gap_ns_sum\":%llu,\"refill_gap_ns_max\":%llu,"
           "\"refill_gap_samples\":%llu,\"in_flight_at_start\":%u,"
           "\"in_flight_at_end\":%u,\"data_errors\":%u,\"quarantined\":%u}",
           (unsigned long long)s->submitted,
           (unsigned long long)s->completed,
           (unsigned long long)s->completed_bytes,
           (unsigned long long)s->active_cycles,
           (unsigned long long)s->iotlb_miss,
           (unsigned long long)s->s1_ptw,
           (unsigned long long)s->s2_ptw,
           (unsigned long long)s->refill_gap_ns_sum,
           (unsigned long long)s->refill_gap_ns_max,
           (unsigned long long)s->refill_gap_samples,
           s->in_flight_at_start,
           s->in_flight_at_end,
           s->data_errors,
           s->quarantined);
}

static int stats_zero(const struct idma_experiment_stats *s)
{
    return !s->submitted && !s->completed && !s->completed_bytes &&
           !s->active_cycles && !s->iotlb_miss && !s->s1_ptw && !s->s2_ptw &&
           !s->refill_gap_ns_sum && !s->refill_gap_ns_max &&
           !s->refill_gap_samples && !s->in_flight_at_start &&
           !s->in_flight_at_end && !s->data_errors && !s->quarantined;
}

/* One 64 B transfer, no errors, no S2 walk, drained. */
static int lane_exact_1(const struct idma_experiment_stats *s)
{
    return s->submitted == 1 && s->completed == 1 &&
           s->completed_bytes == 64 && !s->data_errors &&
           !s->quarantined && !s->s2_ptw && !s->in_flight_at_end;
}

/* V probe must be a real completion against a warm (hot) IOTLB: the preceding
 * V prime already fetched the translation, so a fresh miss here would be
 * unexplained. */
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
    unsigned with_a = c->pair.request.aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;
    __u64 expected = with_a ? 3ULL : 1ULL;
    __u64 a_expected = with_a ? 1ULL : 0ULL;

    if (w[E3_LW_MAGIC] != E3_LAUNCH_MAGIC)
        return 0;
    if (w[E3_LW_ERROR] != 0)
        return 0;
    /* Terminal status must be exactly FROZEN: no RUNNING/ABORTED and no
     * sticky error bits in status[63:32]. */
    if (w[E3_LW_STATUS] != E3_LAUNCH_STATUS_FROZEN)
        return 0;
    /* Raw handshake readbacks: held/active released, reserved word zero. */
    if (w[E3_LW_HELD] != 0 || w[E3_LW_ACTIVE] != 0 || w[E3_LW_ZERO] != 0)
        return 0;
    if (w[E3_LW_EXPECTED] != expected ||
        w[E3_LW_SEEN] != expected ||
        w[E3_LW_ISSUED_MASK] != expected ||
        w[E3_LW_DONE_MASK] != expected)
        return 0;
    if (w[E3_LW_CANCEL_MASK] != 0)     /* unmasked: any cancel is a fault */
        return 0;

    if (!w[E3_LW_ISSUE_ID_V] || w[E3_LW_ISSUE_ID_V] != w[E3_LW_DONE_ID_V])
        return 0;
    if (w[E3_LW_ISSUE_CNT_V] != 1 || w[E3_LW_DONE_CNT_V] != 1)
        return 0;

    if (with_a) {
        if (!w[E3_LW_ISSUE_ID_A] || w[E3_LW_ISSUE_ID_A] != w[E3_LW_DONE_ID_A])
            return 0;
    } else if (w[E3_LW_ISSUE_ID_A] || w[E3_LW_DONE_ID_A]) {
        return 0;
    }
    if (w[E3_LW_ISSUE_CNT_A] != a_expected ||
        w[E3_LW_DONE_CNT_A] != a_expected)
        return 0;
    if (w[E3_LW_HAS_A] != a_expected)
        return 0;

    if (with_a) {
        if (w[E3_LW_SELECT_ID] != E3_LW_A_SELECT_ID ||
            w[E3_LW_SELECT_DID] != E3_LW_A_SELECT_DID ||
            w[E3_LW_SELECT_IOVA] != c->pair.mapping[4] ||
            w[E3_LW_SELECT_WRITE] != 0)
            return 0;
    } else if (w[E3_LW_SELECT_ID] || w[E3_LW_SELECT_DID] ||
               w[E3_LW_SELECT_IOVA] || w[E3_LW_SELECT_WRITE]) {
        return 0;
    }
    return 1;
}

static int controlled_round_ok(const struct idma_e3_controlled *c, unsigned r)
{
    const struct idma_experiment *v = &c->pair.result[r];
    const struct idma_e3_controlled_round *ctl = &c->controlled[r];
    unsigned expected_delay =
        c->pair_mode == E3_PAIR_ZERO ? 0U :
        (r ? 128U - c->pair.first_delay : c->pair.first_delay);
    unsigned expect_a_prime = c->prep_mode == E3_PREP_HOT ? 1U : 0U;
    unsigned with_a = c->pair.request.aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;

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

    /* Reserved DIDs are validated from the result, not from the JSON inputs. */
    if (v->victim_did != E3_FIXED_VICTIM_DID)
        return 0;
    if (with_a) {
        if (v->aggressor_did != E3_LW_A_SELECT_DID)
            return 0;
    } else if (v->aggressor_did != IDMA_EXPERIMENT_NO_AGGRESSOR) {
        return 0;
    }

    if (!lane_exact_1(&v->victim))
        return 0;
    if (with_a) {
        if (!lane_exact_1(&v->aggressor))
            return 0;
    } else if (!stats_zero(&v->aggressor)) {
        return 0;
    }

    if (!lane_exact_1(&ctl->prime[0]))         /* V prime */
        return 0;
    if (expect_a_prime) {
        if (!lane_exact_1(&ctl->prime[1]))     /* hot A prime */
            return 0;
    } else if (!stats_zero(&ctl->prime[1])) {  /* cold A prime */
        return 0;
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
           "\"victim_did\":%u,\"aggressor_did\":%u,"
           "\"victim_elapsed_ns\":%llu,\"snapshot_before_ns\":%llu,"
           "\"snapshot_after_ns\":%llu,\"drain_ns\":%llu,\"victim\":",
           delay, (unsigned long long)c->pair.sequence[r], v->status,
           v->hpm_configured, v->measurement_started, v->measurement_complete,
           v->victim_did, v->aggressor_did,
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
    printf("],\"observer_magic\":%llu,\"v_hw_cycles\":%llu}",
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
    int i;

    if (argc == 2 && !strcmp(argv[1], "--help")) {
        usage();
        return 0;
    }

    for (i = 1; i < argc; i += 2) {
        static const char * const names[] = {
            "--aggressor", "--prep", "--pair", "--first-delay", "--timeout-ms"
        };
        unsigned k, n = 0;

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
        default: timeout = n; break;
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
        caps.device_id != 0 || caps.did != E3_FIXED_VICTIM_DID ||
        caps.page_bytes != 4096) {
        err = EOPNOTSUPP;
        goto output;
    }

    stage = "run";
    attempted = 1;
    rc = ioctl(fd, IDMA_IOC_RUN_E3_CONTROLLED, &c);   /* never retried, incl. EINTR */
    if (rc)
        err = rc < 0 ? errno : EPROTO;
    success = !rc && !c.pair.status &&
              c.pair.rounds_attempted == 2 && c.pair.rounds_completed == 2;
    for (unsigned r = 0; r < 2 && success; r++)
        if (!controlled_round_ok(&c, r))
            success = 0;
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
    /* A truncated report is a failure even when the run itself succeeded.
     * No ioctl is retried; the diagnostic goes to stderr only. */
    if (fflush(stdout) == EOF || ferror(stdout)) {
        fprintf(stderr, "idma_e3_controlled: stdout: write failed\n");
        success = 0;
    }
    return success ? 0 : 1;
}
```

---

### 2) `tests/e3_controlled_cli_stub.c`

```c
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <linux/idma.h>

/* Must match bench/idma_e3_controlled.c and the real RTL / driver constants. */
#define E3_OBSERVER_MAGIC_V6 0x49444d414f425306ULL
#define E3_BANK_V2           0x49444d4145330002ULL
#define E3_LAUNCH_MAGIC      0x49444d414c4e4301ULL

#define E3_PREP_COLD 0U
#define E3_PREP_HOT  1U
#define E3_PAIR_COMPLEMENTARY 0U
#define E3_PAIR_ZERO          1U

/* Launcher word indices; identical to the CLI and dma_controlled_launcher.sv. */
#define LW_MAGIC        0U
#define LW_ERROR        1U
#define LW_STATUS       2U
#define LW_ISSUE_ID_V   11U
#define LW_ISSUE_ID_A   12U
#define LW_DONE_ID_V    13U
#define LW_DONE_ID_A    14U
#define LW_ISSUED_MASK  15U
#define LW_DONE_MASK    16U
#define LW_CANCEL_MASK  17U
#define LW_SEEN         18U
#define LW_EXPECTED     19U
#define LW_SELECT_ID    20U
#define LW_SELECT_DID   21U
#define LW_SELECT_IOVA  22U
#define LW_SELECT_WRITE 23U
#define LW_ISSUE_CNT_V  24U
#define LW_ISSUE_CNT_A  25U
#define LW_DONE_CNT_V   26U
#define LW_DONE_CNT_A   27U
#define LW_HAS_A        28U
#define LW_HELD         29U
#define LW_ACTIVE       30U
#define LW_ZERO         31U

static int run_calls;

int __wrap_open(const char *path, int flags, ...)
{
    const char *mode = getenv("E3_MOCK");

    (void)flags;
    if (getenv("E3_EXPECT_NO_OPEN"))
        assert(0 && "open on invalid input");
    assert(!strcmp(path, "/dev/idma0"));
    if (mode && !strcmp(mode, "openfail")) {
        errno = EIO;
        return -1;
    }
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
        struct idma_experiment_caps *caps = arg;

        *caps = (struct idma_experiment_caps) {
            .abi_version = IDMA_EXPERIMENT_ABI,
            .struct_size = sizeof(*caps),
            .capabilities = IDMA_EXPERIMENT_CAP_E3_CONTROLLED,
            .device_id = 0,
            .did = 10,
            .page_bytes = 4096,
        };
        if (mode && !strcmp(mode, "old"))
            caps->capabilities = 0;
        if (mode && !strcmp(mode, "wrongcap"))
            caps->capabilities = IDMA_EXPERIMENT_CAP_E3_QUEUE_PAIR;
        if (mode && !strcmp(mode, "wrongdid"))
            caps->did = 11;
        if (mode && !strcmp(mode, "wrongpage"))
            caps->page_bytes = 8192;
        if (mode && !strcmp(mode, "wrongdevice"))
            caps->device_id = 1;
        if (mode && !strcmp(mode, "wrongabi"))
            caps->abi_version = 2;
        if (mode && !strcmp(mode, "wrongsize"))
            caps->struct_size = sizeof(*caps) - 1;
        return 0;
    }

    assert(op == IDMA_IOC_RUN_E3_CONTROLLED);
    assert(++run_calls == 1);            /* no retry under any condition */

    if (mode && !strcmp(mode, "interrupt")) {
        errno = EINTR;
        return -1;
    }
    if (mode && !strcmp(mode, "runfail")) {
        errno = EIO;
        return -1;
    }

    {
        struct idma_e3_controlled *c = arg;
        unsigned a_present =
            c->pair.request.aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;
        unsigned hot = c->prep_mode == E3_PREP_HOT;
        unsigned expected = a_present ? 3U : 1U;
        int r;

        c->pair.rounds_attempted = 2;
        c->pair.rounds_completed = 2;
        /* One live mapping: A source IOVA is the value the select tap captures. */
        c->pair.mapping[4] = 0x4000U;

        for (r = 0; r < 2; r++) {
            struct idma_e3_controlled_round *ctl = &c->controlled[r];
            __u64 *lw = ctl->launcher;
            unsigned a_prime = hot ? 1U : 0U;
            /* Arbitrary nonzero NEXT_ID values, per-lane issue == done. */
            __u64 v_id = r ? 7ULL : 4ULL;
            __u64 a_id = hot ? (r ? 5ULL : 3ULL) : (r ? 3ULL : 2ULL);

            c->pair.sequence[r] = (__u64)(r + 1);
            c->pair.result[r] = c->pair.request;
            c->pair.result[r].measurement_started = 1;
            c->pair.result[r].measurement_complete = 1;
            c->pair.result[r].hpm_configured = 1;
            c->pair.result[r].victim_did = 10U;
            c->pair.result[r].aggressor_did =
                a_present ? 11U : IDMA_EXPERIMENT_NO_AGGRESSOR;
            c->pair.result[r].victim_elapsed_ns = 1000 + (__u64)r;
            c->pair.result[r].snapshot_before_ns = 10 + (__u64)r;
            c->pair.result[r].snapshot_after_ns = 20 + (__u64)r;
            c->pair.result[r].drain_ns = 30 + (__u64)r;

            c->pair.result[r].victim.submitted = 1;
            c->pair.result[r].victim.completed = 1;
            c->pair.result[r].victim.completed_bytes = 64;

            c->pair.result[r].aggressor.submitted = a_present ? 1 : 0;
            c->pair.result[r].aggressor.completed = a_present ? 1 : 0;
            c->pair.result[r].aggressor.completed_bytes = a_present ? 64 : 0;

            c->pair.observation[r][0] = E3_BANK_V2;
            c->pair.observation[r][1] = 0;
            c->pair.observation[r][2] = 0;  /* no trigger coverage: still executed-valid */
            c->pair.observation[r][3] =
                (c->pair_mode == E3_PAIR_ZERO) ? 0 :
                (r ? 128 - c->pair.first_delay : c->pair.first_delay);

            ctl->flush_status = 0;
            ctl->prep_complete = 1;
            ctl->stage = 8;

            ctl->prime[0].submitted = 1;
            ctl->prime[0].completed = 1;
            ctl->prime[0].completed_bytes = 64;
            ctl->prime[1].submitted = a_prime;
            ctl->prime[1].completed = a_prime;
            ctl->prime[1].completed_bytes = a_prime ? 64 : 0;

            ctl->probe.submitted = 1;
            ctl->probe.completed = 1;
            ctl->probe.completed_bytes = 64;

            ctl->observer_magic = E3_OBSERVER_MAGIC_V6;
            ctl->v_hw_cycles = 1000 + (__u64)r;

            memset(lw, 0, sizeof(ctl->launcher));
            lw[LW_MAGIC] = E3_LAUNCH_MAGIC;
            lw[LW_ERROR] = 0;
            lw[LW_STATUS] = (1ULL << 2); /* FROZEN, not RUNNING, not ABORTED */
            lw[LW_ISSUE_ID_V] = v_id;
            lw[LW_DONE_ID_V] = v_id;
            lw[LW_ISSUED_MASK] = expected;
            lw[LW_DONE_MASK] = expected;
            lw[LW_CANCEL_MASK] = 0;
            lw[LW_SEEN] = expected;
            lw[LW_EXPECTED] = expected;
            lw[LW_HAS_A] = a_present ? 1U : 0U;
            lw[LW_ISSUE_CNT_V] = 1;
            lw[LW_DONE_CNT_V] = 1;
            if (a_present) {
                lw[LW_ISSUE_ID_A] = a_id;
                lw[LW_DONE_ID_A] = a_id;
                lw[LW_SELECT_ID] = 4;
                lw[LW_SELECT_DID] = 11;
                lw[LW_SELECT_IOVA] = c->pair.mapping[4];
                lw[LW_SELECT_WRITE] = 0;
                lw[LW_ISSUE_CNT_A] = 1;
                lw[LW_DONE_CNT_A] = 1;
            }
        }

        if (mode) {
            struct idma_e3_controlled_round *c1 = &c->controlled[1];
            __u64 *l1 = c1->launcher;

            if (!strcmp(mode, "partial")) {
                c->pair.rounds_completed = 1;
            } else if (!strcmp(mode, "nocoverage")) {
                c->pair.observation[1][2] = 0;
            } else if (!strcmp(mode, "bad_magic")) {
                c1->observer_magic = 0;
            } else if (!strcmp(mode, "bad_bank")) {
                c->pair.observation[1][0] = 0;
            } else if (!strcmp(mode, "bad_error")) {
                c->pair.observation[1][1] = 1;
            } else if (!strcmp(mode, "bad_d")) {
                c->pair.observation[1][3] = 99;
            } else if (!strcmp(mode, "bad_stage")) {
                c1->stage = 7;
            } else if (!strcmp(mode, "bad_prep")) {
                c1->prep_complete = 0;
            } else if (!strcmp(mode, "bad_flush")) {
                c1->flush_status = -EIO;
            } else if (!strcmp(mode, "bad_vcycles")) {
                c1->v_hw_cycles = 0;
            } else if (!strcmp(mode, "bad_prime")) {
                c1->prime[1].submitted = 2;
            } else if (!strcmp(mode, "bad_probe")) {
                c1->probe.iotlb_miss = 1;
            } else if (!strcmp(mode, "bad_launcher_magic")) {
                l1[LW_MAGIC] = 0;
            } else if (!strcmp(mode, "bad_launcher_error")) {
                l1[LW_ERROR] = 1;
            } else if (!strcmp(mode, "bad_launcher_frozen")) {
                l1[LW_STATUS] = 0;
            } else if (!strcmp(mode, "bad_launcher_aborted")) {
                l1[LW_STATUS] = (1ULL << 2) | (1ULL << 3);
            } else if (!strcmp(mode, "bad_launcher_status")) {
                l1[LW_STATUS] = (1ULL << 2) | (1ULL << 32); /* sticky error kept */
            } else if (!strcmp(mode, "bad_launcher_active")) {
                l1[LW_ACTIVE] = 1;
            } else if (!strcmp(mode, "bad_launcher_held")) {
                l1[LW_HELD] = 1;
            } else if (!strcmp(mode, "bad_launcher_zero")) {
                l1[LW_ZERO] = 1;
            } else if (!strcmp(mode, "bad_launcher_id")) {
                l1[LW_ISSUE_ID_V] = 0;
            } else if (!strcmp(mode, "bad_launcher_id_mismatch")) {
                l1[LW_DONE_ID_V] = 1234;
            } else if (!strcmp(mode, "bad_launcher_mask")) {
                l1[LW_ISSUED_MASK] = 2;
            } else if (!strcmp(mode, "bad_launcher_seen")) {
                l1[LW_SEEN] = 0;
            } else if (!strcmp(mode, "bad_launcher_expected")) {
                l1[LW_EXPECTED] = 2;
            } else if (!strcmp(mode, "bad_launcher_count")) {
                l1[LW_DONE_CNT_V] = 2;
            } else if (!strcmp(mode, "bad_launcher_vcount")) {
                l1[LW_ISSUE_CNT_V] = 2;
                l1[LW_DONE_CNT_V] = 2;
            } else if (!strcmp(mode, "bad_launcher_acount")) {
                l1[LW_ISSUE_CNT_A] = 2;
                l1[LW_DONE_CNT_A] = 2;
            } else if (!strcmp(mode, "bad_launcher_has_a")) {
                l1[LW_HAS_A] = a_present ? 0U : 1U;
            } else if (!strcmp(mode, "bad_launcher_cancel")) {
                l1[LW_CANCEL_MASK] = 1;
            } else if (!strcmp(mode, "bad_launcher_cancel_high")) {
                l1[LW_CANCEL_MASK] = (1ULL << 10); /* outside the old 0x3 mask */
            } else if (!strcmp(mode, "bad_launcher_select_iova")) {
                l1[LW_SELECT_IOVA] = l1[LW_SELECT_IOVA] + 1;
            } else if (!strcmp(mode, "bad_launcher_a_id_solo")) {
                l1[LW_ISSUE_ID_A] = 9;   /* solo: A ids must read back zero */
                l1[LW_DONE_ID_A] = 9;
            } else if (!strcmp(mode, "bad_launcher_select_iova_solo")) {
                l1[LW_SELECT_IOVA] = 0x4000U; /* solo: select words must be zero */
            } else if (!strcmp(mode, "bad_victim_did")) {
                c->pair.result[1].victim_did = 11;
            } else if (!strcmp(mode, "bad_aggressor_did")) {
                c->pair.result[1].aggressor_did = 12;
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
            }
        }
    }

    return 0;
}
```

---

### 3) `tests/test_e3_controlled_cli.py`

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

    # Positive all modes/delays.
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
                        r['controlled']['observer_magic'] == 0x49444d414f425306
                        for r in d['rounds']
                    )
                    assert all(
                        r['e3_words'][0] == 0x49444d4145330002
                        for r in d['rounds']
                    )
                    # No-trigger coverage is retained as executed data, never required.
                    assert all(r['e3_words'][2] == 0 for r in d['rounds'])
                    # Reserved DIDs are read from the result, not the JSON inputs.
                    assert all(r['victim_did'] == 10 for r in d['rounds'])
                    expected_did = 0xffffffff if aggressor == 'none' else 11
                    assert all(r['aggressor_did'] == expected_did
                               for r in d['rounds']), args

    run([], True, 'nocoverage')

    # Negative input must not open or issue ioctls.  Rounds in this path are
    # rendered from a zeroed struct (delay pair [0, 128]); that is acceptable
    # because errno is set, run_attempted is false, and no run happened.
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

    # Open failure is reported at the open stage with no ioctl.
    d = run([], False, 'openfail')
    assert d['run_attempted'] is False
    assert d['stage'] == 'open'
    assert d['errno'] == 5  # EIO

    # EINTR is not retried.
    d = run([], False, 'interrupt')
    assert d['run_attempted'] is True
    assert d['errno'] == 4  # EINTR

    # Partial failed round raw data is preserved.
    d = run([], False, 'partial')
    assert d['run_attempted'] is True
    assert d['rounds_attempted'] == 2 and d['rounds_completed'] == 1
    assert len(d['rounds'][1]['e3_words']) == 256
    assert len(d['rounds'][1]['controlled']['launcher']) == 32

    # Metadata failures produce EPROTO/EIO.
    metadata_modes = [
        'bad_magic', 'bad_bank', 'bad_error', 'bad_d', 'bad_stage',
        'bad_prep', 'bad_flush', 'bad_vcycles', 'bad_prime', 'bad_probe',
        'bad_launcher_magic', 'bad_launcher_error', 'bad_launcher_frozen',
        'bad_launcher_aborted', 'bad_launcher_active', 'bad_launcher_id',
        'bad_launcher_id_mismatch', 'bad_launcher_mask',
        'bad_launcher_expected', 'bad_launcher_count', 'bad_launcher_cancel',
        'bad_round_status', 'bad_measurement', 'bad_hpm', 'bad_victim',
        'bad_aggressor', 'bad_sequence', 'bad_pair_status',
        # Stricter launcher word checks: each mutant passed the old lax test.
        'bad_launcher_status', 'bad_launcher_seen', 'bad_launcher_held',
        'bad_launcher_zero', 'bad_launcher_cancel_high', 'bad_launcher_vcount',
        'bad_launcher_acount', 'bad_launcher_has_a', 'bad_launcher_select_iova',
        'bad_victim_did', 'bad_aggressor_did',
    ]
    for mode in metadata_modes:
        d = run([], False, mode)
        assert d['run_attempted'] is True, mode
        assert d['errno'] in (71, 5), (mode, d['errno'])

    # Solo requires every A id / select word to read back zero.
    solo_modes = [
        'bad_launcher_a_id_solo', 'bad_launcher_select_iova_solo',
    ]
    for mode in solo_modes:
        d = run(['--aggressor', 'none'], False, mode)
        assert d['run_attempted'] is True, mode
        assert d['errno'] in (71, 5), (mode, d['errno'])

    # Close failure is reported after a successful run.
    d = run([], False, 'closefail')
    assert d['run_attempted'] is True
    assert d['stage'] == 'close'
    assert d['errno'] == 5  # EIO

    # A failed stdout write must exit nonzero with a stderr diagnostic and must
    # not retry the run ioctl (host-only /dev/full proof).
    full = Path('/dev/full')
    if full.exists():
        with open(full, 'wb') as sink:
            p = subprocess.run([str(exe)], stdout=sink,
                               stderr=subprocess.PIPE, timeout=3)
        assert p.returncode != 0, (p.returncode, p.stderr)
        assert b'idma_e3_controlled' in p.stderr, p.stderr
        count += 1

    print(f'PASS E3 controlled CLI {count} cases (host ioctl model only)')
```

---

### 4) `tests/board-e3-controlled.sh`

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
# Every case requires a readable decimal taint of exactly 0 before and after
# the run; any nonzero/unknown readback stops the run and is preserved.
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

: > "$OUT/input.txt" || fatal "input write failed: $OUT/input.txt"
{
    echo "mode=$mode"
    echo "output=$OUT"
} >> "$OUT/input.txt" || fatal "input write failed: $OUT/input.txt"

if ! cat > "$OUT/matrix.txt" <<'EOF'
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
then
    fatal "matrix write failed: $OUT/matrix.txt"
fi

read_taint() {
    if value=$(cat /proc/sys/kernel/tainted 2>/dev/null); then
        case "$value" in
            ''|*[!0-9]*) echo unknown ;;
            *) echo "$value" ;;
        esac
    else
        echo unknown
    fi
}

save_dmesg() {
    # Preserve the raw ring buffer verbatim; any de-duplication is done offline.
    dmesg > "$OUT/dmesg.raw" 2> "$OUT/dmesg.err"
    dmesg_rc=$?
    if [ "$dmesg_rc" -eq 0 ]; then
        return 0
    fi
    echo "dmesg collection failed rc=$dmesg_rc" > "$OUT/dmesg.error"
    return "$dmesg_rc"
}

save_taint() {
    if value=$(cat /proc/sys/kernel/tainted 2>/dev/null); then
        echo "$value" > "$OUT/taint.txt" || return 1
    else
        echo unknown > "$OUT/taint.txt" || return 1
    fi
    return 0
}

collection_failed=0

cleanup() {
    rc=$?
    trap - EXIT HUP INT TERM
    save_dmesg || collection_failed=1
    save_taint || collection_failed=1
    if [ "$rc" -eq 0 ] && [ "$collection_failed" -ne 0 ]; then
        rc=4
    fi
    echo "E3CTL: final_exit=$rc collection_failed=$collection_failed" >&2
    if [ "$rc" -eq 0 ]; then
        echo "E3CTL: complete ok mode=$mode output=$OUT"
    fi
    exit "$rc"
}

trap cleanup EXIT
trap 'exit 129' HUP
trap 'exit 130' INT
trap 'exit 143' TERM

run_case() {
    phase=$1
    index=$2
    shift 2

    stdout_file="$OUT/$phase-$index.json"
    stderr_file="$OUT/$phase-$index.err"

    before_taint=$(read_taint)
    if [ "$before_taint" != "0" ]; then
        echo "E3CTL: phase=$phase case=$index before_taint=$before_taint refusing to run" >&2
        return 3
    fi

    echo "E3CTL: phase=$phase case=$index begin taint=$before_taint args=$*" >&2
    idma_e3_controlled "$@" > "$stdout_file" 2> "$stderr_file"
    rc=$?
    after_taint=$(read_taint)
    echo "E3CTL: phase=$phase case=$index exit=$rc taint=$after_taint" >&2

    cat "$stdout_file" || {
        echo "E3CTL: phase=$phase case=$index stdout cat failed" >&2
        return 4
    }
    if [ -s "$stderr_file" ]; then
        cat "$stderr_file" >&2 || {
            echo "E3CTL: phase=$phase case=$index stderr cat failed" >&2
            return 4
        }
    fi

    if [ "$after_taint" != "0" ]; then
        echo "E3CTL: phase=$phase case=$index after_taint=$after_taint stopping" >&2
        return 3
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

exit 0
```

---

### 5) `tests/test_rootfs.py`

```python
#!/usr/bin/env python3
"""Host-only packaging regression; does not build or run FPGA software."""
import contextlib
import importlib.util
import io
from pathlib import Path
import stat
import tempfile
import unittest


SCRIPT = Path(__file__).resolve().parents[1] / 'scripts/rootfs.py'
SPEC = importlib.util.spec_from_file_location('platform_rootfs', SCRIPT)
rootfs = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(rootfs)


class RootfsPackagingTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        for directory in ('assets', 'out/bench', 'tests'):
            (self.root / directory).mkdir(parents=True)
        (self.root / 'out/bench/idma_experiment').write_bytes(b'new experiment binary')
        (self.root / 'out/bench/idma_e3_pair').write_bytes(b'new paired experiment binary')
        (self.root / 'out/bench/idma_e3_controlled').write_bytes(b'new controlled experiment binary')
        (self.root / 'tests/board-smoke.sh').write_bytes(b'#!/bin/sh\nexit 0\n')
        (self.root / 'tests/board-e3-controlled.sh').write_bytes(b'#!/bin/sh\nexit 0\n')
        (self.root / 'out/bench/platform-timeout').write_bytes(b'new timeout binary')
        # These old and unrelated artifacts must remain on disk but never be packed.
        for name in sorted(rootfs.LEGACY_NAMES | {'unexpected-program'}):
            (self.root / 'out/bench' / name).write_bytes(b'historical output')
        self.device = [1, stat.S_IFCHR | 0o600, 0, 0, 1, 0, 0, 0, 0, 240, 0, 0, 0]
        self.file = [2, stat.S_IFREG | 0o644, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0]

    def base(self, extra=()):
        data = bytearray()
        for name, fields, body in [
            ('dev/idma0', self.device, b''),
            ('etc/banner', self.file, b'preserve this file'),
            *extra,
        ]:
            rootfs.write_entry(data, name, fields, body)
        rootfs.write_entry(data, 'TRAILER!!!', [0] * 13, b'')
        (self.root / 'assets/rootfs-base.cpio').write_bytes(data)

    def pack(self):
        with contextlib.redirect_stdout(io.StringIO()):
            rootfs.repack(self.root)
        return list(rootfs.read_newc((self.root / 'out/rootfs.cpio').read_bytes()))

    def test_replaces_all_managed_copies_and_ignores_stale_outputs(self):
        names = list(rootfs.LEGACY_NAMES) + [
            'usr/bin/idma_experiment', './usr/bin/idma_experiment',
            'idma_experiment', 'usr/bin/platform-smoke', 'usr/bin/platform-smoke',
            'platform-smoke', 'usr/bin/idma_pair_bench', 'usr/bin/platform-timeout',
            './usr/bin/platform-timeout', 'platform-timeout',
            'usr/bin/idma_e3_controlled', './usr/bin/idma_e3_controlled',
            'idma_e3_controlled', 'usr/bin/platform-e3-controlled',
            './usr/bin/platform-e3-controlled', 'platform-e3-controlled',
        ]
        self.base([(name, self.file, b'old copy') for name in names])
        entries = self.pack()
        packed = {name: (fields, body) for name, fields, body in entries}
        self.assertEqual(len(entries), len(packed))
        self.assertEqual(set(packed), {
            'dev/idma0', 'etc/banner', 'usr/bin/idma_experiment', 'usr/bin/platform-smoke',
            'usr/bin/platform-timeout', 'usr/bin/idma_e3_pair',
            'usr/bin/idma_e3_controlled', 'usr/bin/platform-e3-controlled',
        })
        for name, source in rootfs.EXECUTABLES.items():
            fields, body = packed['usr/bin/' + name]
            self.assertEqual(fields[1], stat.S_IFREG | 0o755)
            self.assertEqual(body, (self.root / source).read_bytes())
        device_fields, device_body = packed['dev/idma0']
        self.assertEqual(device_fields[1:11], self.device[1:11])
        self.assertEqual(device_body, b'')
        self.assertEqual(packed['etc/banner'][1], b'preserve this file')
        self.assertEqual((self.root / 'out/bench/idma_once').read_bytes(), b'historical output')

    def test_missing_current_binary_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'out/bench/idma_experiment').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_missing_controlled_binary_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'out/bench/idma_e3_controlled').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_missing_controlled_runner_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'tests/board-e3-controlled.sh').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_missing_watchdog_preserves_existing_output(self):
        self.base()
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        (self.root / 'out/bench/platform-timeout').unlink()
        with self.assertRaises(FileNotFoundError):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_unrelated_duplicate_rejected_before_output_replaced(self):
        self.base([('./etc/banner', self.file, b'ambiguous duplicate')])
        output = self.root / 'out/rootfs.cpio'
        output.write_bytes(b'previous rootfs')
        with self.assertRaisesRegex(ValueError, 'Duplicate rootfs entry'):
            self.pack()
        self.assertEqual(output.read_bytes(), b'previous rootfs')

    def test_repacking_current_image_does_not_duplicate_entrypoints(self):
        self.base()
        self.pack()
        data = (self.root / 'out/rootfs.cpio').read_bytes()
        (self.root / 'assets/rootfs-base.cpio').write_bytes(data)
        entries = self.pack()
        for name in rootfs.EXECUTABLES:
            self.assertEqual(sum(Path(path).name == name for path, _, _ in entries), 1)


if __name__ == '__main__':
    unittest.main()
```

---

### 6) `tests/test_board_e3_controlled.py` (new, bounded)

```python
#!/usr/bin/env python3
"""Bounded host-side tests for tests/board-e3-controlled.sh.

The production runner is exercised through a TEST-ONLY transformed copy whose
command search path and taint path point at isolated mocks.  No board
hardware, no real /proc/sys/kernel/tainted read, and no production override
are introduced.
"""
import os
import signal
import subprocess
import tempfile
import time
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RUNNER = ROOT / 'tests' / 'board-e3-controlled.sh'

EXPECTED_MATRIX = """short:
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
"""


class BoardRunnerTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.dir = Path(self.temp.name)
        self.bindir = self.dir / 'bin'
        self.bindir.mkdir()
        self.taint = self.dir / 'tainted'
        self.taint.write_text('0\n')
        self.count = self.dir / 'count'
        self.runner = self.dir / 'runner.sh'

        text = RUNNER.read_text()
        # Fail loudly if the runner drifts and the TEST-only rewrites miss.
        self.assertIn('PATH=/sbin:/bin:/usr/sbin:/usr/bin\n', text)
        self.assertIn('/proc/sys/kernel/tainted', text)
        text = text.replace(
            'PATH=/sbin:/bin:/usr/sbin:/usr/bin\n',
            'PATH="%s:/sbin:/bin:/usr/sbin:/usr/bin"\n' % self.bindir)
        text = text.replace('/proc/sys/kernel/tainted', '"$E3_TEST_TAINT"')
        self.runner.write_text(text)
        os.chmod(self.runner, 0o755)

        self._mock('dmesg',
                   '#!/bin/sh\n'
                   'echo "mock raw dmesg"\n'
                   'exit "${E3_TEST_DMESG_RC:-0}"\n')
        self._mock('idma_e3_controlled',
                   '#!/bin/sh\n'
                   'n=0\n'
                   '[ -f "$E3_TEST_COUNT" ] && n=$(cat "$E3_TEST_COUNT")\n'
                   'n=$((n + 1))\n'
                   'echo "$n" > "$E3_TEST_COUNT"\n'
                   'echo "{\\"mock_case\\":$n}"\n'
                   'if [ -n "${E3_TEST_SLEEP:-}" ]; then sleep "$E3_TEST_SLEEP"; fi\n'
                   'if [ -n "${E3_TEST_CLI_FAIL_AT:-}" ] && [ "$n" = "$E3_TEST_CLI_FAIL_AT" ]; then\n'
                   '    exit 1\n'
                   'fi\n'
                   'if [ -n "${E3_TEST_TAINT_AT:-}" ] && [ "$n" = "$E3_TEST_TAINT_AT" ]; then\n'
                   '    echo "${E3_TEST_TAINT_VALUE:-1}" > "$E3_TEST_TAINT"\n'
                   'fi\n'
                   'exit 0\n')

    def _mock(self, name, body):
        path = self.bindir / name
        path.write_text(body)
        os.chmod(path, 0o755)

    def _env(self, **extra):
        env = dict(os.environ)
        env['E3_TEST_TAINT'] = str(self.taint)
        env['E3_TEST_COUNT'] = str(self.count)
        for key, value in extra.items():
            env[key] = str(value)
        return env

    def _out(self, name):
        return self.dir / name

    def _run(self, *args, **extra):
        return subprocess.run(['sh', str(self.runner), *args],
                              env=self._env(**extra), capture_output=True,
                              text=True, timeout=20)

    def test_success_short(self):
        out = self._out('out-short')
        p = self._run('short', str(out))
        self.assertEqual(p.returncode, 0, p.stderr)
        self.assertIn('complete ok', p.stderr)
        self.assertEqual((out / 'matrix.txt').read_text(), EXPECTED_MATRIX)
        for index in (1, 2, 3):
            self.assertTrue((out / ('short-%d.json' % index)).exists())
        self.assertTrue((out / 'dmesg.raw').exists())
        self.assertEqual((out / 'taint.txt').read_text(), '0\n')

    def test_success_main_matrix(self):
        out = self._out('out-main')
        p = self._run('main', str(out))
        self.assertEqual(p.returncode, 0, p.stderr)
        for index in range(1, 9):
            self.assertTrue((out / ('main-%d.json' % index)).exists())

    def test_cli_error_stops(self):
        out = self._out('out-cli')
        p = self._run('short', str(out), E3_TEST_CLI_FAIL_AT=2)
        self.assertNotEqual(p.returncode, 0, p.stderr)
        self.assertTrue((out / 'short-2.json').exists())
        self.assertFalse((out / 'short-3.json').exists())
        self.assertEqual(self.count.read_text().strip(), '2')

    def test_taint_change_stops(self):
        out = self._out('out-taint')
        p = self._run('short', str(out), E3_TEST_TAINT_AT=1)
        self.assertEqual(p.returncode, 3, p.stderr)
        self.assertTrue((out / 'short-1.json').exists())
        self.assertFalse((out / 'short-2.json').exists())

    def test_unknown_taint_refuses_before_case(self):
        self.taint.write_text('unknown\n')
        out = self._out('out-unknown')
        p = self._run('short', str(out))
        self.assertEqual(p.returncode, 3, p.stderr)
        self.assertFalse((out / 'short-1.json').exists())
        self.assertFalse(self.count.exists())

    def test_capture_failure_is_nonzero(self):
        out = self._out('out-capture')
        p = self._run('short', str(out), E3_TEST_DMESG_RC=1)
        self.assertNotEqual(p.returncode, 0, p.stderr)
        self.assertTrue((out / 'dmesg.error').exists())

    def test_output_dir_must_not_exist(self):
        out = self._out('out-exists')
        out.mkdir()
        p = self._run('short', str(out))
        self.assertEqual(p.returncode, 2, p.stderr)

    def test_interrupt_is_nonzero(self):
        out = self._out('out-int')
        proc = subprocess.Popen(
            ['sh', str(self.runner), 'short', str(out)],
            env=self._env(E3_TEST_SLEEP=30),
            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
            start_new_session=True)
        try:
            time.sleep(0.5)
            try:
                os.killpg(os.getpgid(proc.pid), signal.SIGINT)
            except ProcessLookupError:
                pass
            proc.wait(timeout=15)
        finally:
            if proc.poll() is None:
                proc.kill()
                proc.wait()
        self.assertNotEqual(proc.returncode, 0)


if __name__ == '__main__':
    unittest.main()
```

---

## Assumptions (explicit)

1. **`mapping[4]` is the aggressor source IOVA.** Header comment `idma.h:103` states `mapping` order is V src/dst IOVA/PA then A src/dst IOVA/PA, so `mapping[4]` is A src IOVA. The stub writes one live value (`0x4000`) for the whole ioctl and mirrors it into launcher word 22 for both rounds, matching the driver's single-mapping lifetime.
2. **`has_a`/`seen_held` semantics.** Launcher word 28 is `has_a_q` (set from `CMD_HAS_A`, which mirrors aggressor presence) and word 18 is `seen_held_q`; on a clean FROZEN completion both equal the expected lane mask (1 or 3). I compare exact equality, not `>=`.
3. **`aggressor_did` value.** I validate against `E3_LW_A_SELECT_DID` (11), i.e. the same reservation DID the E3 select tap captures, and `IDMA_EXPERIMENT_NO_AGGRESSOR` for solo. The value is read from `pair.result[r]`; the CLI does not inject it.
4. **"Do not hardcode reservation IDs."** Interpreted as: word 20/21 are compared against the existing named RTL-mirror constants `E3_LW_A_SELECT_ID`/`E3_LW_A_SELECT_DID`, word 22 against the live `mapping[4]` (never a literal), and the JSON emits `victim_did`/`aggressor_did` from the result. No new literal reservation IDs were introduced.
5. **`victim_did` constant.** `E3_FIXED_VICTIM_DID` (10) replaces the literal `10` in the caps check and is reused for the round check; the capabilities stage still gates it.
6. **`probe_exact_1` unchanged, comment corrected.** The probe runs after the V prime, so it is a warm (hot) IOTLB completion; the mask of checks is unchanged.
7. **Exit codes.** Runner: `2` usage/fatal, `3` taint stop, `4` collection/display failure, otherwise the propagated case rc; signals map to `129/130/143`. These do not collide with the CLI's `0/1`.
8. **`/dev/full`.** The output-failure check is skipped if `/dev/full` is absent.
9. **Runner INT test.** Uses `start_new_session` + `killpg` and only asserts a nonzero exit (the reviewer's "INT nonzero" bar), since signal-to-shell timing can be delivered either to the shell trap or the foreground child.
10. **No production env override.** The `E3_TEST_*` variables only exist in the transformed test copy; `scripts/board-e3-controlled.sh`/`tests/board-e3-controlled.sh` remain unchanged in behavior.
11. `scripts/rootfs.py`, `linux/include/uapi/linux/idma.h`, the driver, RTL, and the 3+8 matrix are untouched.

## Unresolved / for Codex to confirm

- Whether `seen_held` can ever legitimately exceed the expected mask on hardware (glitch on a non-expected lane) — if so, the exact-equality check in `launcher_ok` may need `>= expected` on word 18 only.
- Whether `aggressor_did` is guaranteed to equal the A select DID in the driver for the controlled path; the stub is written to that assumption and the CLI now enforces it.

## Suggested acceptance checks (not executed here)

- `tests/test_e3_controlled_cli.py` — expect `PASS E3 controlled CLI ...` (builds CLI + stub with `-Werror`; exercises all new metadata/solo mutants, open-failure, and `/dev/full`).
- `tests/test_board_e3_controlled.py` — expect 8 passing bounded runner tests (success short + main matrix, CLI stop, taint stop, unknown-taint refusal, capture failure, outdir refusal, INT nonzero).
- `tests/test_rootfs.py` — expect 7 passing tests (adds the two new binaries' fixtures plus missing-controlled-binary/runner preservation tests).
- Manual runner sanity on a board only: `tests/board-e3-controlled.sh short /tmp/e3ctl-short` must refuse an existing dir (rc 2), abort nonzero on any taint readback ≠ 0, preserve `dmesg.raw` verbatim, and emit `E3CTL: complete ok` only when every capture succeeded.
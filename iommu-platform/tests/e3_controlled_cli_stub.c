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

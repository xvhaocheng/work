// SPDX-License-Identifier: GPL-2.0
/* One ioctl owns both rounds and their mapping lifetime. Never retry a run. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <linux/idma.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

/* Observer bank versions in raw e3_words[0]; v2 adds waiting-queue gates. */
#define E3_BANK_V1 0x49444d4145330001ULL
#define E3_BANK_V2 0x49444d4145330002ULL

static int number(const char *s, unsigned *out)
{
    unsigned n = 0;
    if (!*s) return -1;
    for (; *s; s++) {
        if (*s < '0' || *s > '9' || n > 60000) return -1;
        n = n * 10 + *s - '0';
    }
    if (n > 60000) return -1;
    *out = n;
    return 0;
}
static void stats(const struct idma_experiment_stats *s)
{
    printf("{\"submitted\":%llu,\"completed\":%llu,\"completed_bytes\":%llu,"
           "\"active_cycles\":%llu,\"iotlb_miss\":%llu,\"s1_ptw\":%llu,\"s2_ptw\":%llu,"
           "\"data_errors\":%u,\"quarantined\":%u}",
           s->submitted, s->completed, s->completed_bytes, s->active_cycles,
           s->iotlb_miss, s->s1_ptw, s->s2_ptw, s->data_errors, s->quarantined);
}
int main(int argc, char **argv)
{
    struct idma_e3_pair p = {.abi_version = 1, .struct_size = sizeof(p)};
    struct idma_e3_diag d = {0};
    unsigned diagnostic = 0;
    struct idma_experiment *e = &p.request;
    struct idma_experiment_caps c = {0};
    int fd = -1, rc = -1, err = 0, attempted = 0, success = 0;
    const char *stage = "arguments";
    unsigned seen = 0;
    *e = (struct idma_experiment) {.abi_version = IDMA_EXPERIMENT_ABI,
        .struct_size = sizeof(*e), .aggressor_id = 1, .victim_pages = 1,
        .aggressor_pages = 8, .chunk_bytes = 4096, .run_timeout_ms = 60000,
        .victim_transfers = 4096, .warmup_transfers = 256};
    if (argc == 2 && !strcmp(argv[1], "--help")) {
        puts("Usage: idma_e3_pair [--aggressor none|1] [--aggressor-pages 1|8]\n"
             "  [--transfers 1..4096] [--warmup 0..4096] [--timeout-ms 1..60000]\n"
             "  [--first-delay 0|128] [--diagnostic 0|1|2]\n"
             "Diagnostic 1 uses D0/D0 and software warmup/start records; first-delay must be 0.\n"
             "Diagnostic 2 uses D0/D128 with the same diagnostic fields; first-delay may be 0 or 128.\n"
             "Fixed victim /dev/idma0. Two rounds in one mapping lifetime.\n"
             "One JSON record; retain failures and no-trigger pairs. No automatic retry.");
        return 0;
    }
    for (int i = 1; i < argc; i += 2) {
        const char *names[] = {"--aggressor", "--aggressor-pages", "--transfers",
                              "--warmup", "--timeout-ms", "--first-delay", "--diagnostic"};
        unsigned k, n;
        for (k = 0; k < 7; k++) if (!strcmp(argv[i], names[k])) break;
        if (k == 7 || i + 1 == argc || (seen & (1U << k))) {err = EINVAL; goto output;}
        seen |= 1U << k;
        if (k == 0 && !strcmp(argv[i+1], "none")) n = IDMA_EXPERIMENT_NO_AGGRESSOR;
        else if (number(argv[i+1], &n)) {err = EINVAL; goto output;}
        switch (k) {
        case 0: e->aggressor_id = n; break;
        case 1: e->aggressor_pages = n; break;
        case 2: e->victim_transfers = n; break;
        case 3: e->warmup_transfers = n; break;
        case 4: e->run_timeout_ms = n; break;
        case 5: p.first_delay = n; break;
        case 6: diagnostic = n; break;
        }
    }
    if ((e->aggressor_id != 1 && e->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR) ||
        (e->aggressor_pages != 1 && e->aggressor_pages != 8) ||
        !e->victim_transfers || e->victim_transfers > 4096 || e->warmup_transfers > 4096 ||
        !e->run_timeout_ms || diagnostic > 2 || (diagnostic == 1 && p.first_delay) ||
        (p.first_delay != 0 && p.first_delay != 128)) {err = EINVAL; goto output;}
    stage = "open";
    fd = open("/dev/idma0", O_RDWR | O_CLOEXEC);
    if (fd < 0) {err = errno; goto output;}
    stage = "capabilities";
    rc = ioctl(fd, IDMA_IOC_EXPERIMENT_CAPS, &c);
    if (rc) {err = rc < 0 ? errno : EPROTO; goto output;}
    if (c.abi_version != IDMA_EXPERIMENT_ABI || c.struct_size != sizeof(c) ||
        !(c.capabilities & (diagnostic == 0 ? IDMA_EXPERIMENT_CAP_E3_PAIR :
                            diagnostic == 1 ? IDMA_EXPERIMENT_CAP_E3_DIAG :
                                              IDMA_EXPERIMENT_CAP_E3_QUEUE_PAIR)) ||
        c.device_id || c.did != 10 || c.page_bytes != 4096) {
        err = EOPNOTSUPP; goto output;
    }
    stage = "run"; attempted = 1;
    if (diagnostic) {
        d.pair = p;
        rc = ioctl(fd, diagnostic == 2 ? IDMA_IOC_RUN_E3_QUEUE_PAIR : IDMA_IOC_RUN_E3_DIAG, &d);
        p = d.pair;
    } else {
        rc = ioctl(fd, IDMA_IOC_RUN_E3_PAIR, &p);
    }
    if (rc) err = rc < 0 ? errno : EPROTO;
    success = !rc && !p.status && p.rounds_attempted == 2 && p.rounds_completed == 2;
    for (unsigned r = 0; r < 2; r++) {
        struct idma_experiment *v = &p.result[r];
        int bank_ok = diagnostic == 2 ? p.observation[r][0] == E3_BANK_V2 :
            (p.observation[r][0] == E3_BANK_V1 || p.observation[r][0] == E3_BANK_V2);
        success = success && !v->status && v->measurement_started && v->measurement_complete &&
            v->hpm_configured && !v->victim.data_errors && !v->aggressor.data_errors &&
            !v->victim.quarantined && !v->aggressor.quarantined &&
            v->victim.submitted == e->victim_transfers && v->victim.completed == e->victim_transfers &&
            v->victim.completed_bytes == e->victim_transfers * 4096 && p.sequence[r] &&
            bank_ok && !p.observation[r][1] &&
            p.observation[r][3] == (r && diagnostic != 1 ? 128 - p.first_delay : p.first_delay);
    }
    if (diagnostic && success) {
        for (unsigned r = 0; r < 2; r++) {
            const struct idma_e3_diag_round *g = &d.diagnostic[r];
            unsigned lanes = e->aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR ? 1 : 2;
            success = success && g->warm_complete && g->warm_begin_ns &&
                g->warm_end_ns >= g->warm_begin_ns && g->warm_drained_ns >= g->warm_end_ns &&
                g->arm_begin_ns >= g->warm_drained_ns && g->arm_end_ns >= g->arm_begin_ns &&
                g->lane[0].warm_submitted == e->warmup_transfers &&
                g->lane[0].warm_completed == e->warmup_transfers;
            for (unsigned n = 0; n < lanes; n++) {
                const struct idma_e3_diag_lane *l = &g->lane[n];
                success = success && l->warm_completed <= l->warm_submitted &&
                    l->warm_submitted - l->warm_completed == l->warm_drained &&
                    l->warm_in_flight == l->warm_drained &&
                    l->launch_begin_ns >= g->arm_end_ns && l->launch_end_ns >= l->launch_begin_ns;
            }
            if (lanes == 2)
                success = success && g->lane[0].launch_begin_ns >= g->lane[1].launch_end_ns;
        }
    }
    if (!success && !err) err = EIO;
output:
    if (fd >= 0 && close(fd) && !err) {err = errno; success = 0; stage = "close";}
    printf("{\"schema\":\"%s\",\"success\":%s,\"stage\":\"%s\","
           "\"errno\":%d,\"run_attempted\":%s,\"status\":%d,\"rounds_attempted\":%u,"
           "\"rounds_completed\":%u,\"input\":{\"victim_id\":0,\"aggressor_id\":%u,"
           "\"aggressor_pages\":%u,\"transfers\":%llu,\"warmup\":%llu,\"timeout_ms\":%u,"
           "\"first_delay\":%u},\"mapping\":[",
           diagnostic == 0 ? "idma.e3_pair.v1" :
           diagnostic == 1 ? "idma.e3_diag.v1" : "idma.e3_queue_pair.v1",
           success ? "true" : "false", stage, err, attempted ? "true" : "false", p.status,
           p.rounds_attempted, p.rounds_completed, e->aggressor_id, e->aggressor_pages,
           e->victim_transfers, e->warmup_transfers, e->run_timeout_ms, p.first_delay);
    for (unsigned i = 0; i < 8; i++) printf("%s%llu", i ? "," : "", p.mapping[i]);
    printf("],\"rounds\":[");
    for (unsigned r = 0; r < 2; r++) {
        const struct idma_experiment *v = &p.result[r];
        printf("%s{\"delay\":%u,\"sequence\":%llu,\"status\":%d,\"hpm_configured\":%u,"
               "\"measurement_started\":%u,\"measurement_complete\":%u,\"victim_elapsed_ns\":%llu,"
               "\"snapshot_before_ns\":%llu,\"snapshot_after_ns\":%llu,\"drain_ns\":%llu,\"victim\":",
               r ? "," : "", r && diagnostic != 1 ? 128 - p.first_delay : p.first_delay, p.sequence[r], v->status,
               v->hpm_configured, v->measurement_started, v->measurement_complete, v->victim_elapsed_ns,
               v->snapshot_before_ns, v->snapshot_after_ns, v->drain_ns);
        stats(&v->victim); printf(",\"aggressor\":"); stats(&v->aggressor);
        printf(",\"e3_words\":[");
        for (unsigned i = 0; i < IDMA_E3_WORDS; i++) printf("%s%llu", i ? "," : "", p.observation[r][i]);
        printf("]");
        if (diagnostic) {
            const struct idma_e3_diag_round *g = &d.diagnostic[r];
            printf(",\"diagnostic\":{\"warm_complete\":%u,\"warm_begin_ns\":%llu,"
                   "\"warm_end_ns\":%llu,\"warm_drained_ns\":%llu,"
                   "\"arm_begin_ns\":%llu,\"arm_end_ns\":%llu,\"lane\":[",
                   g->warm_complete, g->warm_begin_ns, g->warm_end_ns, g->warm_drained_ns,
                   g->arm_begin_ns, g->arm_end_ns);
            for (unsigned n = 0; n < 2; n++) {
                const struct idma_e3_diag_lane *l = &g->lane[n];
                printf("%s{\"warm_submitted\":%llu,\"warm_completed\":%llu,"
                       "\"warm_drained\":%llu,\"warm_in_flight\":%u,"
                       "\"warm_last_offset\":%llu,\"warm_next_offset\":%llu,"
                       "\"launch_begin_ns\":%llu,\"launch_end_ns\":%llu,\"warm_hpm\":",
                       n ? "," : "", l->warm_submitted, l->warm_completed, l->warm_drained,
                       l->warm_in_flight, l->warm_last_offset, l->warm_next_offset,
                       l->launch_begin_ns, l->launch_end_ns);
                printf("{\"active_cycles\":%llu,\"iotlb_miss\":%llu,\"s1_ptw\":%llu,\"s2_ptw\":%llu}}",
                       l->warm_hpm.active_cycles, l->warm_hpm.iotlb_miss,
                       l->warm_hpm.s1_ptw, l->warm_hpm.s2_ptw);
            }
            printf("]}");
        }
        printf("}");
    }
    puts("]}");
    if (err) fprintf(stderr, "idma_e3_pair: %s: %s\n", stage, strerror(err));
    return success ? 0 : 1;
}

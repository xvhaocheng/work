#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <linux/idma.h>
static int runs;
int __wrap_open(const char *path, int flags, ...) { (void)flags; assert(!strcmp(path,"/dev/idma0")); return 7; }
int __wrap_close(int fd) { assert(fd==7 && runs<=1); return 0; }
int __wrap_ioctl(int fd, unsigned long op, ...) {
    const char *mode=getenv("E3_MOCK");
    va_list ap;void *arg;
    assert(fd==7);va_start(ap,op);arg=va_arg(ap,void*);va_end(ap);
    if(op==IDMA_IOC_EXPERIMENT_CAPS) {
        struct idma_experiment_caps *c=arg;
        *c=(struct idma_experiment_caps){.abi_version=1,.struct_size=sizeof(*c),
            .capabilities=IDMA_EXPERIMENT_CAP_E3_PAIR|IDMA_EXPERIMENT_CAP_E3_DIAG|
                          IDMA_EXPERIMENT_CAP_E3_QUEUE_PAIR,.did=10,.page_bytes=4096};
        if(mode && !strcmp(mode,"old"))c->capabilities=0;
        if(mode && !strcmp(mode,"pair_only"))c->capabilities=IDMA_EXPERIMENT_CAP_E3_PAIR;
        return 0;
    }
    assert((op==IDMA_IOC_RUN_E3_PAIR || op==IDMA_IOC_RUN_E3_DIAG ||
            op==IDMA_IOC_RUN_E3_QUEUE_PAIR) && ++runs==1);
    struct idma_e3_pair *p=arg;
    unsigned long long magic=op==IDMA_IOC_RUN_E3_QUEUE_PAIR?0x49444d4145330002ULL:0x49444d4145330001ULL;
    if(mode && !strcmp(mode,"bank_v1"))magic=0x49444d4145330001ULL;
    if(mode && !strcmp(mode,"bank_v2"))magic=0x49444d4145330002ULL;
    if(mode && !strcmp(mode,"interrupt")){p->status=-EINTR;p->rounds_attempted=1;errno=EINTR;return -1;}
    p->rounds_attempted=p->rounds_completed=2;
    for(int r=0;r<2;r++){
        p->sequence[r]=r+1;p->result[r]=p->request;
        p->result[r].measurement_started=p->result[r].measurement_complete=p->result[r].hpm_configured=1;
        p->result[r].victim.submitted=p->result[r].victim.completed=p->request.victim_transfers;
        p->result[r].victim.completed_bytes=p->request.victim_transfers*4096;
        p->observation[r][0]=magic;
        p->observation[r][3]=r && op!=IDMA_IOC_RUN_E3_DIAG?128-p->first_delay:p->first_delay;
    }
    if(op!=IDMA_IOC_RUN_E3_PAIR) {
        struct idma_e3_diag *d=arg;
        if(op==IDMA_IOC_RUN_E3_DIAG) assert(!p->first_delay);
        for(int r=0;r<2;r++) {
            struct idma_e3_diag_round *g=&d->diagnostic[r];
            *g=(struct idma_e3_diag_round){.warm_complete=1,.warm_begin_ns=1,.warm_end_ns=2,
                .warm_drained_ns=3,.arm_begin_ns=4,.arm_end_ns=5};
            for(int n=0;n<(p->request.aggressor_id==IDMA_EXPERIMENT_NO_AGGRESSOR?1:2);n++)
                g->lane[n]=(struct idma_e3_diag_lane){.warm_submitted=p->request.warmup_transfers,
                    .warm_completed=p->request.warmup_transfers,.launch_begin_ns=n?6:8,.launch_end_ns=n?7:9};
        }
        if(mode && !strcmp(mode,"diag_counts")) d->diagnostic[1].lane[1].warm_completed++;
        if(mode && !strcmp(mode,"diag_time")) d->diagnostic[1].lane[0].launch_begin_ns=1;
        if(mode && !strcmp(mode,"diag_incomplete")) d->diagnostic[1].warm_complete=0;
        if(mode && !strcmp(mode,"diag_delay")) p->observation[1][3]=128;
    }
    if(mode && !strcmp(mode,"bad"))p->result[1].victim.data_errors=1;
    return 0;
}

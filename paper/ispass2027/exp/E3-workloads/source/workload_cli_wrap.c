/* Actual CLI main, modeled syscalls. No hardware access. */
#include <assert.h>
#include <errno.h>
#include <linux/idma_workload.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
static unsigned runs;
static int mode(const char *s){const char *p=getenv("WORKLOAD_FAKE_MODE");return p&&!strcmp(p,s);}
int __wrap_open(const char *p,int flags,...){(void)flags;
    if(mode("openfail")){errno=EIO;return -1;}assert(!strcmp(p,"/dev/idma0"));return 17;}
int __wrap_close(int fd){assert(fd==17);
    if(mode("closefail")){errno=EIO;return -1;}return 0;}
static uint64_t period(const struct idma_workload_geometry *g){
    if(g->geometry==2)return (uint64_t)g->matrix_rows*g->columns;
    return g->geometry==1?(uint64_t)g->ring_pages*(4096/g->row_bytes):(uint64_t)g->ring_pages*4096/g->row_bytes;
}
static uint64_t at(const struct idma_workload_geometry *g,uint64_t i){
    i%=period(g);
    if(g->geometry==1)return i/(4096/g->row_bytes)*4096+i%(4096/g->row_bytes)*g->row_bytes;
    if(g->geometry==2){uint64_t group=i/((uint64_t)g->columns*g->rows_per_operation),column=(i/g->rows_per_operation)%g->columns;
        return (group*g->rows_per_operation+i%g->rows_per_operation)*g->row_pitch_bytes+column*g->row_bytes;}
    return i*g->row_bytes;
}
static void coverage(const struct idma_workload_geometry *g,uint64_t n,uint64_t w,struct idma_workload_coverage *c){
    unsigned char *bits=calloc((size_t)g->ring_pages*4096,1);assert(bits);
    uint64_t ring=period(g);
    for(unsigned x=0;x<2;x++){uint64_t count=x?w:n;if(count>ring)count=ring;for(uint64_t k=0;k<count;k++){
        uint64_t start=at(g,k);for(unsigned j=0;j<g->row_bytes;j++)bits[start+j]|=1U<<x;}}
    for(unsigned p=0;p<g->ring_pages;p++){unsigned m=0,z=0;for(unsigned j=0;j<4096;j++){
        unsigned b=bits[p*4096+j];c->measured_distinct_bytes+=!!(b&1);c->verified_union_bytes+=!!b;m|=b&1;z|=b&2;}
        c->measured_distinct_pages+=!!m;c->warmup_distinct_pages+=!!z;}free(bits);
}
int __wrap_ioctl(int fd,unsigned long cmd,...){
    assert(fd==17);va_list ap;va_start(ap,cmd);void *arg=va_arg(ap,void*);va_end(ap);
    if(cmd==IDMA_IOC_CAPS_WORKLOAD){
        if(mode("oldkernel")){errno=ENOTTY;return -1;}
        struct idma_workload_caps c={.abi_version=5,.struct_size=64,.capabilities=5,.did=10,.max_pages=256,
            .max_rows_per_op=64,.max_operation_bytes=65536,.batch_magic=IDMA_WORKLOAD_BATCH_MAGIC,
            .observer_magic=IDMA_WORKLOAD_OBSERVER_MAGIC,.geometry_mask=7,.row_length_mask=127,.path_queue_depth=64,.max_run_timeout_ms=60000};
        if(mode("oldbit"))c.batch_magic--;
    if(mode("oldobs"))c.observer_magic--;
    if(mode("badhpm"))c.capabilities&=~1;
        if(mode("baddepth"))c.path_queue_depth=4;
    if(mode("badlimit"))c.max_pages=4096;
        if(mode("capabi"))c.abi_version=4;
    if(mode("capsize"))c.struct_size--;
        memcpy(arg,&c,sizeof(c));return 0;
    }
    assert(cmd==IDMA_IOC_RUN_WORKLOAD&&!runs++);
    struct idma_workload_experiment *p=arg;
    assert(!p->flags&&!p->reserved0&&!p->reserved1&&!p->reserved[0]&&!p->reserved[1]&&p->supply==1);
    p->victim_did=10;p->aggressor_did=p->aggressor_id==IDMA_EXPERIMENT_NO_AGGRESSOR?IDMA_EXPERIMENT_NO_AGGRESSOR:11;
    p->hpm_configured=p->measurement_started=p->measurement_complete=1;p->observer_sequence=17;p->victim_elapsed_ns=1000;
    p->victim.submitted=p->victim.completed=p->victim_operations*p->victim_geometry.rows_per_operation;
    p->victim.completed_bytes=p->victim.completed*p->victim_geometry.row_bytes;
    p->victim_warmup_descriptors=p->victim_warmup_operations*p->victim_geometry.rows_per_operation;
    p->victim_warmup_bytes=p->victim_warmup_descriptors*p->victim_geometry.row_bytes;
    coverage(&p->victim_geometry,p->victim.completed,p->victim_warmup_descriptors,&p->victim_coverage);
    if(p->aggressor_id!=IDMA_EXPERIMENT_NO_AGGRESSOR){
        p->aggressor.submitted=p->aggressor.completed=32;p->aggressor.completed_bytes=32ULL*p->aggressor_geometry.row_bytes;
        p->aggressor.in_flight_at_end=1;p->aggressor_warmup_descriptors=17;p->aggressor_warmup_bytes=17ULL*p->aggressor_geometry.row_bytes;
        coverage(&p->aggressor_geometry,32,17,&p->aggressor_coverage);
    }
    if(mode("badbytes"))p->victim.completed_bytes++;
    if(mode("badrows"))p->victim.completed++;
    if(mode("bada_bytes"))p->aggressor.completed_bytes++;
    if(mode("bada_rows"))p->aggressor.submitted++;
    if(mode("warmbytes"))p->victim_warmup_bytes++;
    if(mode("warmrows"))p->victim_warmup_descriptors++;
    if(mode("awarmbytes"))p->aggressor_warmup_bytes++;
    if(mode("coveragebytes"))p->victim_coverage.measured_distinct_bytes++;
    if(mode("coveragepages"))p->victim_coverage.measured_distinct_pages++;
    if(mode("warmuppages"))p->victim_coverage.warmup_distinct_pages++;
    if(mode("union"))p->victim_coverage.verified_union_bytes--;
    if(mode("covreserved"))p->aggressor_coverage.reserved=1;
    if(mode("quarantine"))p->aggressor.quarantined=1;
    if(mode("dataerror"))p->victim.data_errors=1;
    if(mode("changedinput"))p->victim_geometry.row_bytes=1024;
    if(mode("seqzero"))p->observer_sequence=0;
    if(mode("inflight"))p->victim.in_flight_at_end=1;
    if(mode("badabi"))p->abi_version=4;
    if(mode("badstatus"))p->status=-EIO;
    if(mode("EINTR")){p->status=-EINTR;errno=EINTR;return -1;}
    return 0;
}

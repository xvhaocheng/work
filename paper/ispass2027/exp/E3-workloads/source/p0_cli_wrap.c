/* Host-only link wrappers: no device open/ioctl occurs. */
#include <errno.h>
#include <linux/idma_p0.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
static unsigned run_calls;
static int mode(const char *s){const char *m=getenv("P0_FAKE_MODE");return m&&!strcmp(m,s);}
int __wrap_open(const char *path,int flags,...){(void)flags;return strcmp(path,"/dev/idma0")?-1:17;}
int __wrap_close(int fd){return fd==17?0:-1;}
int __wrap_ioctl(int fd,unsigned long request,...){
    if(fd!=17)abort();
    va_list ap;va_start(ap,request);void *arg=va_arg(ap,void*);va_end(ap);
    if(request==IDMA_IOC_CAPS_P0){
        if(mode("oldkernel")){errno=ENOTTY;return -1;}
        struct idma_p0_caps c={.abi_version=2,.struct_size=64,.capabilities=5,.device_id=0,.did=10,
            .max_pages=4096,.page_bytes=4096,.max_run_timeout_ms=300000,.batch_magic=IDMA_P0_BATCH_MAGIC,
            .victim_length_mask=7,.aggressor_length_mask=4,.geometry_mask=1};
        if(mode("oldbit"))c.batch_magic--;
        if(mode("capreserved"))c.reserved[0]=1;
        memcpy(arg,&c,sizeof(c));return 0;
    }
    if(request!=IDMA_IOC_RUN_BATCH_P0||run_calls++)abort();
    struct idma_p0_experiment *p=arg;
    p->status=0;p->victim_did=10;p->aggressor_did=p->aggressor_id==IDMA_EXPERIMENT_NO_AGGRESSOR?IDMA_EXPERIMENT_NO_AGGRESSOR:11;
    p->hpm_configured=p->measurement_started=p->measurement_complete=1;p->victim_elapsed_ns=1000;
    p->victim.submitted=p->victim.completed=4096;p->victim.completed_bytes=4096ULL*p->victim_chunk_bytes;
    if(p->aggressor_id!=IDMA_EXPERIMENT_NO_AGGRESSOR){
        p->aggressor.submitted=p->aggressor.completed=32;p->aggressor.completed_bytes=32*4096;
    }
    if(mode("badbytes"))p->victim.completed_bytes++;
    if(mode("undrained"))p->aggressor.submitted++;
    if(mode("badabi"))p->abi_version=1;
    if(mode("dataerror"))p->victim.data_errors=1;
    if(mode("quarantine"))p->aggressor.quarantined=1;
    if(mode("changedinput"))p->victim_chunk_bytes=1024;
    if(mode("EINTR")){p->status=-EINTR;errno=EINTR;return -1;}
    return 0;
}

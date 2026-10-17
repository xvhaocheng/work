/* Host fixture only: real extracted driver functions, mocked frozen MMIO. */
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/linux/idma.h"
#include "include/linux/idma_multi.h"
typedef unsigned long long u64;
typedef unsigned int u32;
#define __iomem
#define __user
#define noinline __attribute__((noinline))
#define READ_ONCE(value) (value)
#define U32_MAX UINT32_MAX
#define PAGE_SIZE 4096U
#define SZ_4K 4096U
struct idma_device { unsigned id, did; void *reg; struct { void *parent; } miscdev; };
static unsigned char registers[256];
static struct idma_device device={.id=0,.did=10,.reg=registers};
static u64 magic, control, index_value, values[35], paths[16][8], histogram[16][32], hist_info;
static unsigned hist_reads, info_reads, log_lines, hist_lines, invalid_batch, assertions;
static char log_buffer[128*1024];
static size_t log_used;
static struct idma_device *idma_get_instance(unsigned id) { return id==0 ? &device : NULL; }
static bool idma_batch_supported(struct idma_device *d,bool unused) { (void)d;(void)unused;return true; }
static bool riscv_iommu_hpm_supported(void *unused) { (void)unused;return true; }
static int copy_to_user(void *to,const void *from,size_t n) { memcpy(to,from,n);return 0; }
static int scnprintf(char *buffer,size_t size,const char *fmt,...) {
    va_list ap;va_start(ap,fmt);int result=vsnprintf(buffer,size,fmt,ap);va_end(ap);
    return size ? (result<(int)size ? result : (int)size-1) : 0;
}
static void host_log(const char *fmt,...) {
    char text[1024];va_list ap;va_start(ap,fmt);int n=vsnprintf(text,sizeof(text),fmt,ap);va_end(ap);
    assert(n>=0 && n<(int)sizeof(text));assert(log_used+(size_t)n<sizeof(log_buffer));
    memcpy(log_buffer+log_used,text,n);log_used+=n;log_buffer[log_used]=0;++log_lines;
    if(strstr(text,"idma_path_hist "))++hist_lines;
    if(strstr(text,"part=time valid=0"))++invalid_batch;
}
#define dev_info(device_pointer,...) host_log(__VA_ARGS__)
static u64 readq(const void *address) {
    uintptr_t offset=(uintptr_t)address-(uintptr_t)registers;
    if(offset==0x58)return magic;
    if(offset==0x60)return control;
    assert(offset==0x78);
    if(index_value<35)return values[index_value];
    if(index_value>=64 && index_value<192)return paths[(index_value-64)/8][(index_value-64)%8];
    if(index_value==192){++info_reads;return hist_info;}
    if(index_value>=256 && index_value<768){++hist_reads;return histogram[(index_value-256)/32][(index_value-256)%32];}
    abort();
}
static void writeq(u64 value,void *address) {
    uintptr_t offset=(uintptr_t)address-(uintptr_t)registers;
    if(offset==0x70){index_value=value;return;}
    if(offset==0x60){if(value==4)control=0;else if(value==2)control=4;else control=1;return;}
    assert(offset==0x68);
}
/* BEGIN_ACTUAL_DRIVER_FUNCTIONS */
/* END_ACTUAL_DRIVER_FUNCTIONS */
static void fixture(u64 version) {
    magic=version;control=4;index_value=0;device.id=0;device.did=10;idma_path_observe=true;
    hist_reads=info_reads=log_lines=hist_lines=invalid_batch=0;log_used=0;log_buffer[0]=0;
    memset(values,0,sizeof(values));memset(paths,0,sizeof(paths));memset(histogram,0,sizeof(histogram));
    values[0]=values[1]=values[11]=10;values[7]=values[8]=values[9]=1;
    values[14]=values[15]=values[16]=values[17]=2;
    for(unsigned lane=0;lane<2;lane++)for(unsigned stage=0;stage<4;stage++) {
        u64 *p=paths[lane*4+stage];p[0]=p[1]=2;p[3]=p[7]=4;p[4]=2;
        histogram[lane*4+stage][2]=2;
    }
    hist_info=0x20202001ULL;
}
static void check(bool condition,const char *label) {
    ++assertions;if(!condition){fprintf(stderr,"FAIL check=%u %s\n",assertions,label);abort();}
    printf("CHECK %u %s\n",assertions,label);
}
static struct idma_observation observation(bool batch) {
    struct idma_observation o={.batch=batch};
    return o;
}
static int finish(struct idma_observation *o,int error) {
    struct idma_experiment e={.victim_transfers=1,.victim_did=10,.aggressor_did=IDMA_EXPERIMENT_NO_AGGRESSOR};
    o->control=&device;o->armed=true;control=4;return idma_observation_finish(o,&e,error);
}
int main(void) {
    puts("FIXTURE_ONLY: actual driver functions with mocked frozen MMIO; no board measurements");
    for(unsigned version=1;version<=4;version++) {
        fixture(IDMA_OBS_MAGIC+version-1);struct idma_observation o=observation(true);
        int rc=idma_observation_prepare(&o,true);
        check(rc==(version>=3?0:-EOPNOTSUPP),"batch requires path-capable v3 or v4");
        if(version>=3) {
            check(o.path && o.hist==(version==4),"prepare records actual histogram capability");
            check(finish(&o,0)==0,"valid frozen batch remains valid");
            check(hist_reads==(version==4?512:0) && info_reads==(version==4?1:0),"bounded one-pass MMIO reads; v3 reads no histogram");
            check(hist_lines==(version==4?16:0),"all 16 histogram lines or legacy no lines");
            if(version==4) { puts("BEGIN_VALID_V4_FIXTURE_LOG");fputs(log_buffer,stdout);puts("END_VALID_V4_FIXTURE_LOG"); }
        }
        struct idma_multi_caps caps={};control=4;
        check(!idma_caps_multi(&device,&caps),"multi capability query succeeds");
        check(!!(caps.capabilities&IDMA_MULTI_CAP_SELECTED_PATH)==(version>=3),"multi path capability accepts v3 and v4 consistently");
    }
    fixture(IDMA_OBS_MAGIC_V2);idma_path_observe=false;struct idma_observation o=observation(true);
    check(!idma_observation_prepare(&o,true) && !o.path && !o.hist,"old v2 batch without path remains supported");
    fixture(IDMA_OBS_MAGIC);o=observation(false);
    check(!idma_observation_prepare(&o,true) && !o.path && !o.hist,"old v1 nonbatch remains supported");
    fixture(0);o=observation(true);check(idma_observation_prepare(&o,true)==-EOPNOTSUPP,"unknown magic rejects");
    fixture(IDMA_OBS_MAGIC_V4);device.did=11;o=observation(true);
    check(idma_observation_prepare(&o,true)==-EOPNOTSUPP,"wrong control DID rejects");
    fixture(IDMA_OBS_MAGIC_V4);control=1;o=observation(true);
    check(idma_observation_prepare(&o,true)==-EBUSY,"active observation cannot be acquired");
    fixture(IDMA_OBS_MAGIC_V4);o=observation(true);check(!idma_observation_prepare(&o,false)&&!o.control,"disabled prepare does no MMIO ownership");
    for(unsigned fault=0;fault<7;fault++) {
        fixture(IDMA_OBS_MAGIC_V4);o=observation(true);check(!idma_observation_prepare(&o,true),"prepare negative finish fixture");
        switch(fault) {
        case 0:histogram[15][3]=1;break; /* absent A must have zero full/bins */
        case 1:histogram[3][2]=1;break; /* missing count */
        case 2:hist_info|=1ULL<<32;break; /* overflow */
        case 3:hist_info^=3;break; /* schema mismatch */
        case 4:hist_info|=1ULL<<63;break; /* reserved field */
        case 5:histogram[0][0]=~0ULL;break; /* retain out-of-range original value */
        case 6:break; /* pre-existing run error invalidates every observation */
        }
        check(finish(&o,fault==6?-EILSEQ:0)==-EIO,"invalid histogram or run returns EIO");
        check(invalid_batch==1 && hist_lines==16 && hist_reads==512,"overall batch validity rejects partial or corrupt histogram");
        if(fault==5)check(strstr(log_buffer,"18446744073709551615")!=NULL,"invalid 64-bit MMIO value preserved in raw log");
    }
    printf("PASS actual driver source: %u host fixture assertions\n",assertions);
    return 0;
}

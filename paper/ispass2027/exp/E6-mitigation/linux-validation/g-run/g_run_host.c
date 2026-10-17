/* Production driver functions are inserted verbatim by verify_pages_driver.py.
 * The host model replaces allocation/mapping, MMIO, HPM, observer, time and locks.
 * It tests driver control/data/accounting, not kernel/RTL/IOMMU/cache correctness.
 */
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/idma_p0.h>
#include <linux/idma_multi.h>
#include <linux/idma_pages.h>
#include <linux/idma_workload.h>
#include <linux/idma_g.h>
#include <stdarg.h>
#include <limits.h>
typedef __u64 u64; typedef uint32_t u32; typedef uint8_t u8;
typedef uintptr_t dma_addr_t; typedef uintptr_t phys_addr_t;
struct device {int index;}; struct page {int dummy;};
struct miscdevice {struct device *parent;};
struct iommu_domain {int type,index;unsigned long pgsize_bitmap;};
struct riscv_iommu_hpm_session {int dummy;};
struct riscv_iommu_hpm_sample {u64 iotlb_miss[2],s1_ptw[2],s2_ptw[2];};
struct riscv_iommu_hpm_multi_sample {u64 cycles,iotlb_miss[4],s1_ptw[4],s2_ptw[4];};
#define noinline __attribute__((noinline))
#define __iomem
#define __user
#define READ_ONCE(x) (*(volatile __typeof__(x) *)&(x))
#define WRITE_ONCE(x,v) ((x)=(v))
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define BIT(n) (1U << (n))
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#define PAGE_SIZE 4096U
#define U32_MAX UINT32_MAX
#define SZ_4K 4096U
#define PAGE_SHIFT 12
#define SZ_2M 2097152U
#define IS_ALIGNED(x,a) (!((x)&((a)-1)))
#define ALIGN_DOWN(x,a) ((x)&~((__typeof__(x))(a)-1))
#define IOMMU_READ 1
#define IOMMU_WRITE 2
#define CONFIG_RISCV_IOMMU 1
#define CONFIG_64BIT 1
#define IS_REACHABLE(x) (x)
#define IS_ENABLED(x) (x)
#define NSEC_PER_USEC 1000ULL
#define NSEC_PER_MSEC 1000000ULL
#define DMA_ATTR_FORCE_CONTIGUOUS 0
#define GFP_KERNEL 0
#define IOMMU_DOMAIN_IDENTITY 1
#define current NULL
#define IS_ERR_OR_NULL(x) (!(x))
#define check_mul_overflow(a,b,c) __builtin_mul_overflow((a),(b),(c))
#define offset_in_page(x) ((uintptr_t)(x)&4095)
#define readq_poll_timeout(addr,val,condition,delay,timeout) ({int _ret=-ETIMEDOUT; \
    for(int _i=0;_i<8;_i++){(val)=readq(addr);if(condition){_ret=0;break;}} _ret;})
#define BITS_PER_LONG (sizeof(unsigned long)*CHAR_BIT)
#define BITS_TO_LONGS(n) (((n)+BITS_PER_LONG-1)/BITS_PER_LONG)
#define DECLARE_BITMAP(name,n) unsigned long name[BITS_TO_LONGS(n)]
#define IDMA_WORKLOAD_GRANULE 64U
#define IDMA_WORKLOAD_BITMAP_BITS (IDMA_WORKLOAD_MAX_PAGES * 4096U / IDMA_WORKLOAD_GRANULE)
/* TYPES */
static int idma_devices_lock,idma_experiment_lock;
static struct idma_device *idma_devices[IDMA_MAX_DEVS];
static bool idma_observe, idma_path_observe=true;
static u64 idma_observation_sequence;
static struct device devs[4]={{0},{1},{2},{3}};
static struct iommu_domain domains[4]={{0,0,0},{0,1,0},{0,2,0},{0,3,0}};
static struct idma_device idmas[4];
struct model {u64 regs[32],next_id,last_len,completed;bool pending;unsigned pending_polls;};
static struct model hw[4];
#include "g_state.h"
struct mapping {unsigned long iova;size_t size;void *ptr;bool active;};
static struct mapping maps[4096];static size_t map_pgsize[4096];static unsigned map_count,allocs,frees,stop_writes,flushes,log_errors;
static unsigned lock_depth,observer_calls;
static unsigned cpu_delay,batch_delay,poll_phase,resched_phase[2];
static bool model_multi_active;
static u64 nonmulti_yield_hash=1469598103934665603ULL,nonmulti_yields;
static u64 clock_ns;
static int zero_id=-1,drain_failure=-1,verify_failure=-1;
static size_t verify_offset;
static bool verify_source;
static int stop_unquiet=-1;
static bool stop_seen[4];
static bool signal_flag;
static int map_failure=-1,batch_zero_id=-1,signal_on_batch=-1,premature=-1,exhausted=-1;
static int hpm_failure,observe_failure,arm_failure,context_failure,from_failure,to_failure;
static bool path_disabled;
static unsigned contexts,context_frees,hpm_nr;
static u64 hist_info=0x0040000020202002ULL;
static int alloc_failure=-1,pa_unaligned=-1,pa_noncontiguous=-1,map_call_failure=-1;
static int short_unmap=-1,query_fault_at=-1,query_fault_kind,find_failure;
static bool failed_map_residual,short_clears;
static unsigned map_calls,unmap_calls,query_calls,alloc_attempts;
static size_t allocation_sizes[256];

static u64 trace_hash=1469598103934665603ULL,trace_count;
static void trace(unsigned lane,unsigned off,u64 value,unsigned write){
    assert(trace_count<50000000);
    u64 values[]={lane,off,value,write};
    for(unsigned i=0;i<4;i++){trace_hash^=values[i];trace_hash*=1099511628211ULL;}trace_count++;
}
static void *allocations[256];static unsigned nallocations;
static void *kzalloc(size_t n,int flags){(void)flags;if(context_failure)return NULL;contexts++;return calloc(1,n);}
static void kfree(void *p){if(p){context_frees++;for(unsigned i=0;i<nallocations;i++)if(allocations[i]==p)allocations[i]=NULL;free(p);}}

static void mutex_lock(int *m){(void)m;lock_depth++;}
static int mutex_lock_interruptible(int *m){mutex_lock(m);return 0;}
static void mutex_unlock(int *m){(void)m;assert(lock_depth);lock_depth--;}
static void dev_err(struct device *d,const char *fmt,...){(void)d;(void)fmt;log_errors++;}
static void dev_crit(struct device *d,const char *fmt,...){(void)d;(void)fmt;log_errors++;}
static void g_refresh(void);
static u64 ktime_get_ns(void){clock_ns+=1000;g_refresh();return clock_ns;}
static bool signal_pending(void *p){(void)p;return signal_flag;}
static void cond_resched(void){
    resched_phase[poll_phase]++;
    if(!model_multi_active){u64 data[]={trace_count,clock_ns,poll_phase};
        for(unsigned i=0;i<3;i++){nonmulti_yield_hash^=data[i];nonmulti_yield_hash*=1099511628211ULL;}nonmulti_yields++;}
}
static void cpu_relax(void){}
static void *memchr_inv(const void *p,int c,size_t n){for(size_t i=0;i<n;i++)if(((u8*)p)[i]!=(u8)c)return (u8*)p+i;return NULL;}
static int copy_from_user(void *d,const void *s,size_t n){if(from_failure)return 1;memcpy(d,s,n);return 0;}
static int copy_to_user(void *d,const void *s,size_t n){if(to_failure)return 1;memcpy(d,s,n);return 0;}
static struct idma_device *idma_get_instance(u32 n){return n<4?idma_devices[n]:NULL;}
static void idma_put_instance(struct idma_device *d){(void)d;}
static struct iommu_domain *iommu_get_domain_for_dev(struct device *d){return &domains[d->index];}
static void *dma_alloc_attrs(struct device *d,size_t n,dma_addr_t *a,int g,int f){
    (void)d;(void)g;(void)f;if((int)alloc_attempts++==alloc_failure)return NULL;
    void *p=aligned_alloc(n==SZ_2M?SZ_2M:4096,n);assert(p);*a=(uintptr_t)p;allocs++;
    assert(nallocations<256);allocation_sizes[nallocations]=n;allocations[nallocations++]=p;return p;}
static void dma_free_attrs(struct device *d,size_t n,void *p,dma_addr_t a,int f){
    (void)n;(void)a;(void)f;assert(!idmas[d->index].quarantined);frees++;for(unsigned i=0;i<nallocations;i++)if(allocations[i]==p)allocations[i]=NULL;free(p);}
static bool is_vmalloc_addr(void *p){(void)p;return false;}
static struct page *vmalloc_to_page(void *p){return (struct page*)p;}
static struct page *virt_to_page(void *p){return (struct page*)p;}
static phys_addr_t page_to_phys(struct page *p){for(unsigned i=0;i<nallocations;i++)if(allocations[i]&&(uintptr_t)p>=(uintptr_t)allocations[i]&&
    (uintptr_t)p<(uintptr_t)allocations[i]+allocation_sizes[i]){
    if((int)i==pa_unaligned)return (uintptr_t)p+4096;
    if((int)i==pa_noncontiguous&&(uintptr_t)p==(uintptr_t)allocations[i]+4096)return (uintptr_t)p+4096;
    }return (uintptr_t)p;}
static int idma_find_iova(struct iommu_domain *d,size_t n,unsigned long *s,unsigned long *t){
    if(find_failure)return find_failure;
    *s=0x1000000UL+d->index*0x1000000UL;*t=*s+max(n,0x100000UL);return 0;}
static int idma_iommu_map_range(struct iommu_domain *d,unsigned long iova,phys_addr_t pa,size_t n,size_t pg){
    (void)pg;if(d->index==map_failure)return -ENOMEM;assert(map_count<4096);maps[map_count++]=(struct mapping){iova,n,(void*)pa,true};return 0;}
static void idma_iommu_unmap_range(struct iommu_domain *d,unsigned long iova,size_t n,size_t pg){
    (void)d;(void)n;(void)pg;for(unsigned i=0;i<map_count;i++)if(maps[i].iova==iova&&maps[i].active){maps[i].active=false;return;}assert(0);}
static int iommu_map(struct iommu_domain *d,unsigned long iova,phys_addr_t pa,size_t n,int prot,int flags){
    (void)d;(void)flags;assert(prot==(IOMMU_READ|IOMMU_WRITE));
    bool fail=(int)map_calls++==map_call_failure;
    if(!fail||failed_map_residual){assert(map_count<4096);map_pgsize[map_count]=n;
        maps[map_count++]=(struct mapping){iova,n,(void*)pa,true};}
    return fail?-ENOMEM:0;
}
static size_t iommu_unmap(struct iommu_domain *d,unsigned long iova,size_t n){
    (void)d;bool short_read=(int)unmap_calls++==short_unmap;
    for(unsigned i=0;i<map_count;i++)if(maps[i].active&&maps[i].iova==iova){
        assert(maps[i].size==n);if(!short_read||short_clears)maps[i].active=false;
        return short_read?n/2:n;
    }return 0;
}
static int riscv_iommu_query_leaf(struct iommu_domain *d,unsigned long iova,struct riscv_iommu_leaf_info *info){
    (void)d;memset(info,0,sizeof(*info));bool fault=(int)query_calls++==query_fault_at;
    if(fault&&query_fault_kind==1)return -EOPNOTSUPP;
    if(fault&&query_fault_kind==5)return -ENOENT;
    for(unsigned i=0;i<map_count;i++)if(maps[i].active&&iova>=maps[i].iova&&iova<maps[i].iova+maps[i].size){
        info->phys=(uintptr_t)maps[i].ptr+iova-maps[i].iova;
        info->leaf_shift=map_pgsize[i]==SZ_2M?21:12;info->leaf_iova_base=maps[i].iova;
        info->raw_pte=(((uintptr_t)maps[i].ptr>>12)<<10)|7;
        if(fault){if(query_fault_kind==2)info->leaf_shift=30;
            if(query_fault_kind==3)info->phys+=4096;
            if(query_fault_kind==4)info->raw_pte&=~4ULL;}
        return 0;
    }return -ENOENT;
}
static void *resolve(unsigned long iova,size_t n){
    for(unsigned i=0;i<map_count;i++) {
        if(maps[i].active&&iova>=maps[i].iova&&iova+n<=maps[i].iova+maps[i].size)
            return (u8*)maps[i].ptr+iova-maps[i].iova;
    }
    assert(0);return NULL;
}
static phys_addr_t iommu_iova_to_phys(struct iommu_domain *d,unsigned long iova){(void)d;return (uintptr_t)resolve(iova,1);}
static void local_flush_icache_all(void){flushes++;if(flushes==2&&verify_failure>=0)
    ((u8*)resolve(hw[verify_failure].regs[(verify_source?IDMA_SRC_ADDR_REG_OFFSET:IDMA_DST_ADDR_REG_OFFSET)/8],verify_offset+1))[verify_offset]^=1;}
/* Independent geometry model: never calls a production offset/bitmap helper. */
static u64 model_offset(const struct model *h,u64 i,u64 len,u64 span){
    unsigned geom=h->regs[0xb0/8];
    if(geom==2){u64 rows=h->regs[0xb8/8],cols=h->regs[0xd0/8];
        u64 k=i%(h->regs[0xc8/8]*cols),group=k/(rows*cols);
        return (group*rows+k%rows)*h->regs[0xc0/8]+((k/rows)%cols)*len;}
    if(geom==1){u64 slots=4096/len;i%=span/4096*slots;return i/slots*4096+i%slots*len;}
    return i*len%span;
}
static void copy_tasks(int lane,u64 count){struct model *h=&hw[lane];u64 len=h->regs[IDMA_NUM_BYTES_REG_OFFSET/8];
    assert(count<=262144);
    u64 span=h->regs[IDMA_BATCH_PAGES_OFFSET/8]*4096,off=0;
    for(u64 i=0;i<count;i++){memcpy(resolve(h->regs[1]+off,len),resolve(h->regs[0]+off,len),len);
        if(h->regs[IDMA_BATCH_CONTROL_OFFSET/8]&1)off=model_offset(h,i+1,len,span);}
    h->completed=count;h->last_len=len;
}
static struct model *model_at(void *p,int *lane,unsigned *off){for(int i=0;i<4;i++)
    if((u8*)p>=(u8*)hw[i].regs&&(u8*)p<(u8*)hw[i].regs+sizeof(hw[i].regs)){
        *lane=i;*off=(unsigned)((u8*)p-(u8*)hw[i].regs);return &hw[i];}assert(0);return NULL;}
#include "g_model.inc"
static bool riscv_iommu_hpm_supported(struct device *d){(void)d;return true;}
static int riscv_iommu_hpm_acquire(struct device *v,struct device *a,struct riscv_iommu_hpm_session **p){
    (void)v;(void)a;static struct riscv_iommu_hpm_session s;*p=hpm_failure?NULL:&s;return hpm_failure;}
static void riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *p){(void)p;}
static void riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *p,struct riscv_iommu_hpm_sample *s){(void)p;memset(s,0,sizeof(*s));g_hpm_reads++;if(g_hpm_reads==2){g_hpm_after_event=++g_event;g_hpm_A_pending=hw[1].pending;}}
static int riscv_iommu_hpm_acquire_multi(struct device *const d[],unsigned nr,struct riscv_iommu_hpm_session **p){
    hpm_nr=nr;for(unsigned i=0;i<nr;i++)assert(d[i]->index==(int)i);
    static struct riscv_iommu_hpm_session s;*p=hpm_failure?NULL:&s;return hpm_failure;
}
static void riscv_iommu_hpm_read_multi(struct riscv_iommu_hpm_session *p,struct riscv_iommu_hpm_multi_sample *s){assert(p);memset(s,0,sizeof(*s));}
static bool is_power_of_2(u32 n){return n&&!(n&(n-1));}
static void bitmap_zero(unsigned long *p,size_t n){memset(p,0,BITS_TO_LONGS(n)*sizeof(*p));}
static void bitmap_set(unsigned long *p,size_t start,size_t n){while(n--){p[start/BITS_PER_LONG]|=1UL<<(start%BITS_PER_LONG);start++;}}
static bool test_bit(size_t n,const unsigned long *p){return !!(p[n/BITS_PER_LONG]&(1UL<<(n%BITS_PER_LONG)));}
static unsigned hweight_long(unsigned long n){return __builtin_popcountl(n);}
static unsigned bitmap_weight(const unsigned long *p,unsigned n){unsigned total=0;for(unsigned i=0;i<n;i++)total+=test_bit(i,p);return total;}
#include "idma-workload.h"
#include "idma-g.h"
#define __force
#define __io_br() do {} while(0)
#define __io_ar(v) do {(void)(v);} while(0)
#define le64_to_cpu(v) (v)
#define get_kernel_nofault(v,p) g_nofault(&(v),(const void *)(p))
#define BIT_ULL(n) (1ULL << (n))
#define scnprintf snprintf
/* FUNCTIONS */

static long run_multi_instrumented(struct idma_device *v,struct idma_multi_experiment *p){
    model_multi_active=true;long ret=idma_run_multi(v,p);model_multi_active=false;return ret;
}
#define idma_run_multi run_multi_instrumented
static void reset(u64 magic){
    /* Test-only model reboot reclaims intentionally retained DMA buffers. */
    for(unsigned i=0;i<nallocations;i++)free(allocations[i]);
    nallocations=0;
    alloc_failure=pa_unaligned=pa_noncontiguous=map_call_failure=short_unmap=query_fault_at=-1;
    query_fault_kind=find_failure=0;failed_map_residual=short_clears=false;
    map_calls=unmap_calls=query_calls=alloc_attempts=0;
    memset(map_pgsize,0,sizeof(map_pgsize));
    map_failure=batch_zero_id=signal_on_batch=premature=exhausted=-1;
    hpm_failure=observe_failure=arm_failure=context_failure=from_failure=to_failure=0;
    hist_info=0x0040000020202002ULL;cpu_delay=batch_delay=poll_phase=0;memset(resched_phase,0,sizeof(resched_phase));
    path_disabled=false;contexts=context_frees=hpm_nr=0;

    memset(hw,0,sizeof(hw));memset(idmas,0,sizeof(idmas));memset(idma_devices,0,sizeof(idma_devices));
    memset(maps,0,sizeof(maps));map_count=allocs=frees=stop_writes=flushes=log_errors=lock_depth=observer_calls=0;
    zero_id=drain_failure=verify_failure=-1;clock_ns=0;signal_flag=false;
    verify_offset=0;verify_source=false;stop_unquiet=-1;
    memset(stop_seen,0,sizeof(stop_seen));
    for(int i=0;i<4;i++){domains[i].pgsize_bitmap=SZ_4K|SZ_2M;idmas[i].id=i;idmas[i].did=10+i;idmas[i].reg=hw[i].regs;
        idmas[i].miscdev.parent=&devs[i];idma_devices[i]=&idmas[i];hw[i].regs[0x80/8]=magic;hw[i].regs[IDMA_OBS_MAGIC_OFFSET/8]=IDMA_OBS_MAGIC_V3;hw[i].next_id=10;}
}

#include "g_cases.inc"

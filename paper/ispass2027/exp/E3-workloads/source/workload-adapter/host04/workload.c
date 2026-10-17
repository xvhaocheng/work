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
#include <limits.h>
typedef __u64 u64; typedef uint32_t u32; typedef uint8_t u8;
typedef uintptr_t dma_addr_t; typedef uintptr_t phys_addr_t;
struct device {int index;}; struct page {int dummy;};
struct miscdevice {struct device *parent;};
struct iommu_domain {int type,index;unsigned long pgsize_bitmap;};
struct riscv_iommu_hpm_session {int dummy;};
struct riscv_iommu_hpm_sample {u64 iotlb_miss[2],s1_ptw[2],s2_ptw[2];};
struct riscv_iommu_hpm_multi_sample {u64 cycles,iotlb_miss[4],s1_ptw[4],s2_ptw[4];};
#define __iomem
#define __user
#define READ_ONCE(x) (*(volatile __typeof__(x) *)&(x))
#define WRITE_ONCE(x,v) ((x)=(v))
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define BIT(n) (1U << (n))
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#define PAGE_SIZE 4096U
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
#define IDMA_SRC_ADDR_REG_OFFSET 0x00
#define IDMA_DST_ADDR_REG_OFFSET 0x08
#define IDMA_NUM_BYTES_REG_OFFSET 0x10
#define IDMA_CONF_REG_OFFSET 0x18
#define IDMA_STATUS_REG_OFFSET 0x20
#define IDMA_NEXT_ID_REG_OFFSET 0x28
#define IDMA_DONE_REG_OFFSET 0x30
#define IDMA_ACTIVE_CYCLES_REG_OFFSET 0x50
#define IDMA_OBS_MAGIC_OFFSET 0x58
#define IDMA_OBS_CONTROL_OFFSET 0x60
#define IDMA_OBS_TARGET_OFFSET 0x68
#define IDMA_OBS_INDEX_OFFSET 0x70
#define IDMA_OBS_DATA_OFFSET 0x78
#define IDMA_OBS_MAGIC 0x49444d414f425301ULL
#define IDMA_OBS_MAGIC_V2 0x49444d414f425302ULL
#define IDMA_OBS_MAGIC_V3 0x49444d414f425303ULL
#define IDMA_OBS_MAGIC_V4 0x49444d414f425304ULL
#define IDMA_OBS_MAGIC_V5 0x49444d414f425305ULL
#define IDMA_OBS_VALUES 34
#define IDMA_BATCH_MAGIC_OFFSET 0x80
#define IDMA_BATCH_CONTROL_OFFSET 0x88
#define IDMA_BATCH_TARGET_OFFSET 0x90
#define IDMA_BATCH_PAGES_OFFSET 0x98
#define IDMA_BATCH_COMPLETED_OFFSET 0xa0
#define IDMA_BATCH_ISSUED_OFFSET 0xa8
#define IDMA_BATCH_GEOMETRY_OFFSET 0xb0
#define IDMA_BATCH_ROWS_OFFSET 0xb8
#define IDMA_BATCH_PITCH_OFFSET 0xc0
#define IDMA_BATCH_MATRIX_ROWS_OFFSET 0xc8
#define IDMA_BATCH_COLUMNS_OFFSET 0xd0
#define IDMA_BATCH_MAGIC 0x49444d4142544301ULL
#define IDMA_XFER_TIMEOUT_US 2000000
#define IDMA_MAX_DEVS 8
#define IDMA_MAX_PAGES 4096U
#define IDMA_MAX_RUN_TIMEOUT_MS 300000U
struct idma_device {
	struct miscdevice miscdev;
	char name[16];
	int id;
	u32 did;
	void __iomem *reg;
	bool quarantined;
};
struct idma_stream {
	struct idma_device *idma;
	struct device *dev;
	struct iommu_domain *domain;
	void *src, *dst;
	dma_addr_t src_dma, dst_dma;
	unsigned long src_iova, dst_iova;
	size_t size, offset, next_offset, touched_end;
	u32 chunk;
	u64 transfer_id, deadline_ns, last_complete_ns;
	bool mapped_src, mapped_dst, in_flight, retain;
	bool batch_configured, batch_started;
	struct idma_experiment_stats *stats;
	/* NULL for all older ABIs; allocated with the private ABI5 request context. */
	struct idma_workload_stream *workload;
};
struct idma_observation {
	struct idma_device *control;
	u64 sequence;
	bool armed, batch, path, hist, hist_v5;
};
struct idma_multi_context {
	struct idma_stream streams[IDMA_MULTI_MAX_DEVICES];
	struct idma_device *devices[IDMA_MULTI_MAX_DEVICES];
	struct idma_stream *aggressors[IDMA_MULTI_MAX_DEVICES - 1];
	struct idma_observation observation;
	struct riscv_iommu_hpm_multi_sample before, after;
	u64 active_before[IDMA_MULTI_MAX_DEVICES], active_after[IDMA_MULTI_MAX_DEVICES];
};
struct idma_pages_context {
	struct idma_pages_experiment request;
	bool iova_assigned[2];
};
struct idma_workload_stream {
	const struct idma_workload_geometry *geometry;
	u64 warmup_issued, warmup_completed;
	bool data_verified;
	DECLARE_BITMAP(warmup, IDMA_WORKLOAD_BITMAP_BITS);
	DECLARE_BITMAP(measured, IDMA_WORKLOAD_BITMAP_BITS);
};
struct idma_workload_context {
	struct idma_workload_experiment request;
	struct idma_workload_stream streams[2];
};
struct riscv_iommu_leaf_info {
	u64 raw_pte;
	phys_addr_t phys;
	unsigned long leaf_iova_base;
	u32 leaf_shift;
};

static int idma_devices_lock,idma_experiment_lock;
static struct idma_device *idma_devices[IDMA_MAX_DEVS];
static bool idma_observe;
static struct device devs[4]={{0},{1},{2},{3}};
static struct iommu_domain domains[4]={{0,0,0},{0,1,0},{0,2,0},{0,3,0}};
static struct idma_device idmas[4];
struct model {u64 regs[32],next_id,last_len,completed;bool pending;};
static struct model hw[4];
struct mapping {unsigned long iova;size_t size;void *ptr;bool active;};
static struct mapping maps[4096];static size_t map_pgsize[4096];static unsigned map_count,allocs,frees,stop_writes,flushes,log_errors;
static unsigned lock_depth,observer_calls;
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
static u64 ktime_get_ns(void){return clock_ns+=1000;}
static bool signal_pending(void *p){(void)p;return signal_flag;}
static void cond_resched(void){}static void cpu_relax(void){}
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
static void writeq(u64 value,void *p){int lane;unsigned off;struct model *h=model_at(p,&lane,&off);
    trace(lane,off,value,1);
    if(off==IDMA_BATCH_CONTROL_OFFSET){
        if(value==0){h->regs[off/8]=0;h->regs[0xa0/8]=h->regs[0xa8/8]=0;
            h->regs[0xb0/8]=h->regs[0xc0/8]=h->regs[0xc8/8]=h->regs[0xd0/8]=0;h->regs[0xb8/8]=1;}
        else if(value==1)h->regs[off/8]=1;
        else if(value==2){stop_writes++;stop_seen[lane]=true;if(h->pending&&drain_failure!=lane){
            h->regs[0x30/8]=h->next_id;h->pending=false;h->regs[off/8]=5;
            h->regs[0xa0/8]=h->regs[0xa8/8]=h->completed;}}
    }else h->regs[off/8]=value;
}
static u64 readq(void *p){int lane;unsigned off;struct model *h=model_at(p,&lane,&off);
    trace(lane,off,h->regs[off/8],0);
    if(off==IDMA_BATCH_CONTROL_OFFSET&&h->pending&&premature==lane)return 1;
    if(off==IDMA_BATCH_CONTROL_OFFSET&&stop_unquiet==lane&&stop_seen[lane]&&!h->pending&&h->completed)return 3;
    if(off==IDMA_BATCH_ISSUED_OFFSET&&h->pending&&exhausted==lane)return IDMA_EXPERIMENT_MAX_TRANSFERS;
    if(off==IDMA_OBS_DATA_OFFSET&&h->regs[IDMA_OBS_INDEX_OFFSET/8]==192)return hist_info;
    if(off==IDMA_NEXT_ID_REG_OFFSET){h->next_id++;bool batch=h->regs[0x88/8]&1;
        u64 count=batch?(lane>0?32:h->regs[0x90/8]):1;
        copy_tasks(lane,count);
        if(batch&&lane>0){h->pending=true;h->regs[0x88/8]=3;}
        else{h->regs[0x30/8]=h->next_id;if(batch){h->regs[0x88/8]=9;h->regs[0xa0/8]=h->regs[0xa8/8]=count;}}
        if(batch&&signal_on_batch==lane)signal_flag=true;
        return (zero_id==lane||(batch&&batch_zero_id==lane))?0:h->next_id;
    }return h->regs[off/8];
}
static bool riscv_iommu_hpm_supported(struct device *d){(void)d;return true;}
static int riscv_iommu_hpm_acquire(struct device *v,struct device *a,struct riscv_iommu_hpm_session **p){
    (void)v;(void)a;static struct riscv_iommu_hpm_session s;*p=hpm_failure?NULL:&s;return hpm_failure;}
static void riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *p){(void)p;}
static void riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *p,struct riscv_iommu_hpm_sample *s){(void)p;memset(s,0,sizeof(*s));}
#ifdef IDMA_OBS_MAGIC_V4
static bool idma_observer_has_path(u64 magic);
#endif
static int idma_observation_prepare(struct idma_observation *o,bool enabled){(void)enabled;observer_calls++;
    o->path=!path_disabled;
#ifdef IDMA_OBS_MAGIC_V4
    o->path=o->path&&idma_observer_has_path(hw[0].regs[IDMA_OBS_MAGIC_OFFSET/8]);
    o->hist=o->path&&hw[0].regs[IDMA_OBS_MAGIC_OFFSET/8]==IDMA_OBS_MAGIC_V4;
#endif
    o->hist_v5=o->path&&hw[0].regs[IDMA_OBS_MAGIC_OFFSET/8]==IDMA_OBS_MAGIC_V5;
    o->hist=o->hist||o->hist_v5;o->control=&idmas[0];
    o->sequence=1;return observe_failure;}
static int idma_observation_arm(struct idma_observation *o,struct idma_device *v,struct idma_device *a,u64 n){(void)n;o->armed=true;assert(v->id==0&&(!a||a->id==1));return arm_failure;}
static int idma_observation_finish(struct idma_observation *o,const struct idma_experiment *e,int error){(void)o;(void)e;(void)error;return 0;}
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
static struct page *idma_buffer_page(void *ptr)
{
	return is_vmalloc_addr(ptr) ? vmalloc_to_page(ptr) : virt_to_page(ptr);
}

static u64 idma_pattern(struct idma_stream *s, size_t word)
{
	return (0xa53c691df027b84eULL ^
		((u64)s->idma->id * 0x0101010101010101ULL) ^
		((u64)word * 0x9e3779b97f4a7c15ULL));
}

static void idma_stream_cleanup(struct idma_stream *s)
{
	if (!s->dev || s->retain)
		return;
	if (s->mapped_dst)
		idma_iommu_unmap_range(s->domain, s->dst_iova, s->size, PAGE_SIZE);
	if (s->mapped_src)
		idma_iommu_unmap_range(s->domain, s->src_iova, s->size, PAGE_SIZE);
	if (s->dst)
		dma_free_attrs(s->dev, s->size, s->dst, s->dst_dma,
			       DMA_ATTR_FORCE_CONTIGUOUS);
	if (s->src)
		dma_free_attrs(s->dev, s->size, s->src, s->src_dma,
			       DMA_ATTR_FORCE_CONTIGUOUS);
}

static int idma_stream_allocate(struct idma_stream *s, struct idma_device *idma,
			       u32 pages, u32 chunk,
			       struct idma_experiment_stats *stats)
{
	size_t i;

	s->idma = idma;
	s->dev = idma->miscdev.parent;
	s->stats = stats;
	s->domain = iommu_get_domain_for_dev(s->dev);
	if (!s->domain || s->domain->type == IOMMU_DOMAIN_IDENTITY)
		return -EOPNOTSUPP;
	s->size = (size_t)pages * PAGE_SIZE;
	s->chunk = chunk;
	s->src = dma_alloc_attrs(s->dev, s->size, &s->src_dma, GFP_KERNEL,
				 DMA_ATTR_FORCE_CONTIGUOUS);
	if (!s->src)
		return -ENOMEM;
	s->dst = dma_alloc_attrs(s->dev, s->size, &s->dst_dma, GFP_KERNEL,
				 DMA_ATTR_FORCE_CONTIGUOUS);
	if (!s->dst)
		return -ENOMEM;
	for (i = 0; i < s->size / sizeof(u64); i++) {
		((u64 *)s->src)[i] = idma_pattern(s, i);
		((u64 *)s->dst)[i] = ~idma_pattern(s, i);
	}
	return 0;
}

static int idma_stream_prepare(struct idma_stream *s, struct idma_device *idma,
			       u32 pages, u32 chunk,
			       struct idma_experiment_stats *stats)
{
	struct page *src_page, *dst_page;
	int ret;

	ret = idma_stream_allocate(s, idma, pages, chunk, stats);
	if (ret)
		return ret;
	src_page = idma_buffer_page(s->src);
	dst_page = idma_buffer_page(s->dst);
	if (!src_page || !dst_page)
		return -EFAULT;
	ret = idma_find_iova(s->domain, s->size, &s->src_iova, &s->dst_iova);
	if (ret)
		return ret;
	ret = idma_iommu_map_range(s->domain, s->src_iova,
				  page_to_phys(src_page), s->size, PAGE_SIZE);
	if (ret)
		return ret;
	s->mapped_src = true;
	ret = idma_iommu_map_range(s->domain, s->dst_iova,
				  page_to_phys(dst_page), s->size, PAGE_SIZE);
	if (ret)
		return ret;
	s->mapped_dst = true;
	return 0;
}

static void idma_quarantine(struct idma_stream *s)
{
	s->retain = true;
	WRITE_ONCE(s->idma->quarantined, true);
	s->stats->quarantined = 1;
	dev_crit(s->dev, "experiment DMA not safely drained; buffers retained until reboot\n");
}

static int idma_pages_verify_buffer(struct idma_stream *s,
				    struct idma_pages_buffer *buffer,
				    size_t pgsize, bool after)
{
	struct riscv_iommu_leaf_info info;
	u32 *verified = after ? &buffer->verified_after_4k_pages :
				&buffer->verified_before_4k_pages;
	unsigned int expected_shift = pgsize == SZ_2M ? 21 : PAGE_SHIFT;
	size_t off;
	int ret;

	for (off = 0; off < s->size; off += PAGE_SIZE) {
		ret = riscv_iommu_query_leaf(s->domain, buffer->iova + off, &info);
		if (ret)
			goto out;
		if (!off) {
			if (after)
				buffer->after_first_pte = info.raw_pte;
			else {
				buffer->before_first_pte = info.raw_pte;
				buffer->first_leaf_iova_base = info.leaf_iova_base;
			}
		}
		if (after)
			buffer->after_last_pte = info.raw_pte;
		else {
			buffer->before_last_pte = info.raw_pte;
			buffer->last_leaf_iova_base = info.leaf_iova_base;
		}
		if (!buffer->leaf_shift_min || info.leaf_shift < buffer->leaf_shift_min)
			buffer->leaf_shift_min = info.leaf_shift;
		buffer->leaf_shift_max = max(buffer->leaf_shift_max, info.leaf_shift);
		/* The explicit COPY mapping requires RISC-V V/R/W (bits 0..2). */
		if (info.leaf_shift != expected_shift || (info.raw_pte & 7) != 7 ||
		    info.phys != buffer->phys + off ||
		    info.leaf_iova_base != ALIGN_DOWN(buffer->iova + off, pgsize)) {
			ret = -EIO;
			goto out;
		}
		(*verified)++;
	}
	/* Check the final byte's superpage offset as well as each 4K base. */
	ret = riscv_iommu_query_leaf(s->domain, buffer->iova + s->size - 1, &info);
	if (!ret && (info.phys != buffer->phys + s->size - 1 ||
	    info.leaf_shift != expected_shift || (info.raw_pte & 7) != 7 ||
	    info.leaf_iova_base != ALIGN_DOWN(buffer->iova + s->size - 1, pgsize)))
		ret = -EIO;
out:
	if (after)
		buffer->after_error = ret;
	else
		buffer->before_error = ret;
	return ret;
}

static int idma_pages_contiguous(void *ptr, size_t bytes, phys_addr_t *phys)
{
	struct page *page;
	size_t off;

	if (!IS_ALIGNED((unsigned long)ptr, PAGE_SIZE))
		return -EINVAL;
	page = idma_buffer_page(ptr);
	if (!page)
		return -EFAULT;
	*phys = page_to_phys(page);
	if (!IS_ALIGNED(*phys, SZ_2M))
		return -EINVAL;
	for (off = 0; off < bytes; off += PAGE_SIZE) {
		page = idma_buffer_page(ptr + off);
		if (!page || page_to_phys(page) != *phys + off)
			return -EFAULT;
	}
	return 0;
}

static int idma_pages_map_buffer(struct idma_stream *s,
				 struct idma_pages_buffer *buffer, size_t pgsize)
{
	size_t off;
	int ret;

	for (off = 0; off < s->size; off += pgsize) {
		ret = iommu_map(s->domain, buffer->iova + off, buffer->phys + off,
				pgsize, IOMMU_READ | IOMMU_WRITE, GFP_KERNEL);
		if (ret)
			return ret;
		buffer->mapped_bytes += pgsize;
	}
	return 0;
}

static int idma_pages_prepare(struct idma_stream *s, struct idma_device *idma,
			      struct idma_experiment_stats *stats,
			      struct idma_pages_context *context, unsigned int lane)
{
	struct idma_pages_experiment *p = &context->request;
	struct idma_pages_buffer *buffers = &p->buffers[2 * lane];
	struct riscv_iommu_leaf_info info;
	phys_addr_t phys;
	unsigned int i;
	int ret;

	for (i = 0; i < 2; i++) {
		buffers[i].bytes = IDMA_PAGES_BUFFER_BYTES;
		buffers[i].requested_page_bytes = p->mapping_page_bytes;
	}
	ret = idma_stream_allocate(s, idma, IDMA_PAGES_BUFFER_BYTES / PAGE_SIZE,
				   SZ_4K, stats);
	if (ret)
		return ret;
	for (i = 0; i < 2; i++) {
		phys = 0;
		ret = idma_pages_contiguous(i ? s->dst : s->src, s->size, &phys);
		buffers[i].phys = phys;
		if (ret)
			return ret;
	}
	ret = idma_find_iova(s->domain, s->size, &s->src_iova, &s->dst_iova);
	if (ret)
		return ret;
	/* Reject unsupported query/domain contexts before creating a mapping. */
	ret = riscv_iommu_query_leaf(s->domain, s->src_iova, &info);
	if (ret != -ENOENT)
		return ret ? ret : -EBUSY;
	ret = riscv_iommu_query_leaf(s->domain, s->dst_iova, &info);
	if (ret != -ENOENT)
		return ret ? ret : -EBUSY;
	context->iova_assigned[lane] = true;
	buffers[0].iova = s->src_iova;
	buffers[1].iova = s->dst_iova;
	for (i = 0; i < 2; i++) {
		if (!IS_ALIGNED(buffers[i].iova, SZ_2M))
			return -EINVAL;
		ret = idma_pages_map_buffer(s, &buffers[i], p->mapping_page_bytes);
		if (ret)
			return ret;
		ret = idma_pages_verify_buffer(s, &buffers[i], p->mapping_page_bytes, false);
		if (ret)
			return ret;
		p->verified_before_mask |= BIT(2 * lane + i);
	}
	p->prepared_lane_mask |= BIT(lane);
	return 0;
}

static int idma_pages_cleanup(struct idma_stream *s,
			      struct idma_pages_context *context, unsigned int lane)
{
	struct idma_pages_experiment *p = &context->request;
	struct idma_pages_buffer *buffers = &p->buffers[2 * lane];
	struct riscv_iommu_leaf_info info;
	void **pointers[2] = { &s->src, &s->dst };
	dma_addr_t dma[2] = { s->src_dma, s->dst_dma };
	unsigned int i;
	size_t off, unmapped;
	int ret = 0, next;

	if (!s->dev)
		return 0;
	if (s->in_flight || s->retain) {
		if (!s->retain)
			idma_quarantine(s);
		p->retained_lane_mask |= BIT(lane);
		return -EIO;
	}
	p->quiet_lane_mask |= BIT(lane);
	if (context->iova_assigned[lane]) {
		for (i = 0; i < 2; i++) {
			struct idma_pages_buffer *b = &buffers[i];
			int error = 0;

			for (off = 0; off < b->mapped_bytes; off += p->mapping_page_bytes) {
				unmapped = iommu_unmap(s->domain, b->iova + off,
						       p->mapping_page_bytes);
				if (unmapped <= p->mapping_page_bytes)
					b->unmapped_bytes += unmapped;
				if (unmapped != p->mapping_page_bytes)
					error = -EIO;
			}
			b->empty_after_cleanup = 1;
			for (off = 0; off < s->size; off += PAGE_SIZE) {
				next = riscv_iommu_query_leaf(s->domain, b->iova + off, &info);
				if (next != -ENOENT) {
					b->empty_after_cleanup = 0;
					if (!error)
						error = next ? next : -EIO;
				}
			}
			b->unmap_error = error;
			if (!error)
				p->unmapped_buffer_mask |= BIT(2 * lane + i);
			else if (!ret)
				ret = error;
		}
	}
	if (ret) {
		/* A dangling mapping keeps both original lane allocations alive. */
		idma_quarantine(s);
		p->retained_lane_mask |= BIT(lane);
		return ret;
	}
	for (i = 0; i < 2; i++) {
		if (*pointers[i]) {
			dma_free_attrs(s->dev, s->size, *pointers[i], dma[i],
				       DMA_ATTR_FORCE_CONTIGUOUS);
			*pointers[i] = NULL;
			p->freed_buffer_mask |= BIT(2 * lane + i);
		}
	}
	return 0;
}

static int idma_submit(struct idma_stream *s, bool measured)
{
	u64 now;

	s->offset = s->next_offset;
	writeq(s->src_iova + s->offset, s->idma->reg + IDMA_SRC_ADDR_REG_OFFSET);
	writeq(s->dst_iova + s->offset, s->idma->reg + IDMA_DST_ADDR_REG_OFFSET);
	writeq(s->chunk, s->idma->reg + IDMA_NUM_BYTES_REG_OFFSET);
	writeq(0, s->idma->reg + IDMA_CONF_REG_OFFSET);
	s->transfer_id = readq(s->idma->reg + IDMA_NEXT_ID_REG_OFFSET);
	if (!s->transfer_id) {
		/* Without a valid ID we cannot prove that buffers are no longer used. */
		idma_quarantine(s);
		return -EIO;
	}
	/* Include configuration writes and the NEXT_ID trigger in refill cost. */
	now = ktime_get_ns();
	if (s->last_complete_ns && measured) {
		u64 gap = now - s->last_complete_ns;

		s->stats->refill_gap_ns_sum += gap;
		s->stats->refill_gap_ns_max = max(s->stats->refill_gap_ns_max, gap);
		s->stats->refill_gap_samples++;
	}
	s->last_complete_ns = 0;
	s->deadline_ns = now + (u64)IDMA_XFER_TIMEOUT_US * NSEC_PER_USEC;
	s->in_flight = true;
	if (s->workload && !s->batch_configured) {
		s->workload->warmup_issued++;
		s->next_offset = idma_workload_offset(s->workload->geometry,
						     s->workload->warmup_issued);
	} else {
		s->next_offset += s->chunk;
		if (s->next_offset == s->size)
			s->next_offset = 0;
	}
	if (measured)
		s->stats->submitted++;
	return 0;
}

static int idma_poll(struct idma_stream *s, bool measured)
{
	u64 done;

	if (!s->in_flight)
		return 0;
	done = readq(s->idma->reg + IDMA_DONE_REG_OFFSET);
	if (done != s->transfer_id)
		return ktime_get_ns() > s->deadline_ns ? -ETIMEDOUT : 0;
	s->last_complete_ns = ktime_get_ns();
	s->in_flight = false;
	s->touched_end = max(s->touched_end, s->offset + s->chunk);
	if (s->workload && !s->batch_configured)
		s->workload->warmup_completed++;
	if (measured) {
		s->stats->completed++;
		s->stats->completed_bytes += s->chunk;
	}
	return 1;
}

static int idma_run_window_many(struct idma_stream *v,
				struct idma_stream *const aggressors[],
				unsigned int nr_aggressors, u64 count,
				u64 deadline, bool measured, u64 *end_ns)
{
	u64 completed = 0;
	u32 spins = 0;
	unsigned int i;
	int ret;

	if (!count)
		return 0;
	if (ktime_get_ns() >= deadline) {
		ret = -ETIMEDOUT;
		goto out;
	}
	if (signal_pending(current)) {
		ret = -EINTR;
		goto out;
	}
	ret = idma_submit(v, measured);
	if (ret)
		goto out;
	for (;;) {
		if (ktime_get_ns() >= deadline) {
			ret = -ETIMEDOUT;
			break;
		}
		if (signal_pending(current)) {
			ret = -EINTR;
			break;
		}
		ret = idma_poll(v, measured);
		if (ret < 0)
			break;
		if (ret == 1) {
			if (++completed == count) {
				/* Capture V's end before polling/refilling A or taking snapshots. */
				*end_ns = v->last_complete_ns;
				return 0;
			}
			ret = idma_submit(v, measured);
			if (ret)
				break;
		}
		for (i = 0; i < nr_aggressors; i++) {
			struct idma_stream *a = aggressors[i];

			ret = idma_poll(a, measured);
			if (ret < 0)
				break;
			if (ret == 1) {
				ret = idma_submit(a, measured);
				if (ret)
					break;
			}
		}
		if (ret < 0)
			break;
		if (!(++spins & 1023))
			cond_resched();
		cpu_relax();
	}
 out:
	*end_ns = ktime_get_ns();
	return ret;
}

static int idma_run_window(struct idma_stream *v, struct idma_stream *a,
			   u64 count, u64 deadline, bool measured, u64 *end_ns)
{
	struct idma_stream *aggressors[] = { a };

	return idma_run_window_many(v, aggressors, a ? 1 : 0, count,
				   deadline, measured, end_ns);
}

static int idma_drain(struct idma_stream *s)
{
	u64 done;
	int ret;

	if (!s->in_flight)
		return s->retain ? -EIO : 0;
	ret = readq_poll_timeout(s->idma->reg + IDMA_DONE_REG_OFFSET, done,
				 done == s->transfer_id, 0, IDMA_XFER_TIMEOUT_US);
	if (ret) {
		idma_quarantine(s);
		return ret;
	}
	s->in_flight = false;
	s->touched_end = max(s->touched_end, s->offset + s->chunk);
	if (s->workload && !s->batch_configured)
		s->workload->warmup_completed++;
	return 0;
}

static int idma_verify(struct idma_stream *s)
{
	size_t i;

	if (s->retain)
		return -EIO;
	/* Verify untouched bytes as well, and source integrity independently. */
	for (i = 0; i < s->size; i++) {
		u64 pattern = idma_pattern(s, i / sizeof(u64));
		u8 value = ((u8 *)&pattern)[i % sizeof(u64)];
		bool touched = s->workload ?
			(test_bit(i / IDMA_WORKLOAD_GRANULE, s->workload->warmup) ||
			 test_bit(i / IDMA_WORKLOAD_GRANULE, s->workload->measured)) :
			i < s->touched_end;
		u8 expected = touched ? value : (u8)~value;
		u8 src = READ_ONCE(((u8 *)s->src)[i]);
		u8 dst = READ_ONCE(((u8 *)s->dst)[i]);

		if (src != value || dst != expected) {
			phys_addr_t src_pa, dst_pa, src_pte_pa, dst_pte_pa;
			u8 src_after, dst_after;

			/*
			 * On this CVA6, FENCE.I also invalidates the WT data cache.
			 * Probe only after failure and safe drain; preserve the failure.
			 * This is not a generic DMA cache-coherency fix.
			 */
			local_flush_icache_all();
			src_after = READ_ONCE(((u8 *)s->src)[i]);
			dst_after = READ_ONCE(((u8 *)s->dst)[i]);

			src_pa = page_to_phys(idma_buffer_page(s->src + i)) +
				 offset_in_page(s->src + i);
			dst_pa = page_to_phys(idma_buffer_page(s->dst + i)) +
				 offset_in_page(s->dst + i);
			src_pte_pa = iommu_iova_to_phys(s->domain, s->src_iova + i);
			dst_pte_pa = iommu_iova_to_phys(s->domain, s->dst_iova + i);

			s->stats->data_errors++;
			dev_err(s->dev,
				"experiment data mismatch: id=%d did=%u offset=%zu touched_end=%zu size=%zu chunk=%u src=%02x src_after_fence_i=%02x expected_src=%02x dst=%02x dst_after_fence_i=%02x expected_dst=%02x src_iova=%#lx dst_iova=%#lx src_pa=%pa dst_pa=%pa src_pte_pa=%pa dst_pte_pa=%pa\n",
				s->idma->id, s->idma->did, i, s->touched_end,
				s->size, s->chunk, src, src_after, value,
				dst, dst_after, expected,
				s->src_iova + i, s->dst_iova + i, &src_pa,
				&dst_pa, &src_pte_pa, &dst_pte_pa);
			return -EILSEQ;
		}
	}
	if (s->workload)
		s->workload->data_verified = true;
	return 0;
}

static bool idma_batch_supported(struct idma_device *idma, bool p0)
{
	u64 magic = readq(idma->reg + IDMA_BATCH_MAGIC_OFFSET);

	return magic == IDMA_WORKLOAD_BATCH_MAGIC || magic == IDMA_P0_BATCH_MAGIC ||
	       (!p0 && magic == IDMA_BATCH_MAGIC);
}

static int idma_batch_configure(struct idma_stream *s, u64 count, bool p0)
{
	void __iomem *base = s->idma->reg;

	if (!idma_batch_supported(s->idma, p0))
		return -EOPNOTSUPP;
	if (s->workload && readq(base + IDMA_BATCH_MAGIC_OFFSET) != IDMA_WORKLOAD_BATCH_MAGIC)
		return -EOPNOTSUPP;
	if (readq(base + IDMA_BATCH_CONTROL_OFFSET) & 2)
		return -EBUSY;
	writeq(0, base + IDMA_BATCH_CONTROL_OFFSET);
	if (s->workload) {
		const struct idma_workload_geometry *g = s->workload->geometry;

		writeq(g->geometry, base + IDMA_BATCH_GEOMETRY_OFFSET);
		writeq(g->rows_per_operation, base + IDMA_BATCH_ROWS_OFFSET);
		writeq(g->row_pitch_bytes, base + IDMA_BATCH_PITCH_OFFSET);
		writeq(g->matrix_rows, base + IDMA_BATCH_MATRIX_ROWS_OFFSET);
		writeq(g->columns, base + IDMA_BATCH_COLUMNS_OFFSET);
	}
	writeq(count, base + IDMA_BATCH_TARGET_OFFSET);
	writeq(s->size / PAGE_SIZE, base + IDMA_BATCH_PAGES_OFFSET);
	writeq(1, base + IDMA_BATCH_CONTROL_OFFSET);
	s->batch_configured = true;
	s->next_offset = 0;
	return readq(base + IDMA_BATCH_CONTROL_OFFSET) == 1 ? 0 : -EIO;
}

static void idma_batch_stop(struct idma_stream *s)
{
	if (s && s->batch_configured && s->batch_started)
		writeq(2, s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
}

static int idma_batch_collect(struct idma_stream *s)
{
	u64 status, issued, completed, bytes;

	if (!s || !s->batch_configured)
		return 0;
	if (s->retain)
		return -EIO;
	status = readq(s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
	if (status & 2) {
		idma_quarantine(s);
		return -EIO;
	}
	if (s->batch_started) {
		issued = readq(s->idma->reg + IDMA_BATCH_ISSUED_OFFSET);
		completed = readq(s->idma->reg + IDMA_BATCH_COMPLETED_OFFSET);
		s->stats->submitted = issued;
		s->stats->completed = completed;
		if (issued != completed || issued > IDMA_EXPERIMENT_MAX_TRANSFERS)
			return -EIO;
		if (check_mul_overflow(completed, (u64)s->chunk, &bytes))
			return -EOVERFLOW;
		s->stats->completed_bytes = bytes;
		if (s->workload)
			idma_workload_prefix_bitmap(s->workload->measured,
						    s->workload->geometry, completed);
		else
			/* Old ABIs retain their contiguous-prefix validation contract. */
			s->touched_end = max(s->touched_end, (size_t)min(bytes, (u64)s->size));
	}
	return status >> 32 ? -EIO : 0;
}

static void idma_batch_disable(struct idma_stream *s)
{
	if (s && s->batch_configured && !s->retain)
		writeq(0, s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
}

static int idma_batch_window_many(struct idma_stream *v,
				  struct idma_stream *const aggressors[],
				  unsigned int nr_aggressors, u64 deadline,
				  u64 *end_ns, struct idma_multi_experiment *multi)
{
	u32 spins = 0;
	unsigned int i;
	int ret;

	if (ktime_get_ns() >= deadline) {
		ret = -ETIMEDOUT;
		goto out;
	}
	if (signal_pending(current)) {
		ret = -EINTR;
		goto out;
	}
	for (i = 0; i < nr_aggressors; i++) {
		struct idma_stream *a = aggressors[i];

		if (multi) {
			if (ktime_get_ns() >= deadline) {
				ret = -ETIMEDOUT;
				goto out;
			}
			if (signal_pending(current)) {
				ret = -EINTR;
				goto out;
			}
			multi->start_attempted_mask |= BIT(i + 1);
			multi->start_ns[i + 1] = ktime_get_ns();
		}
		a->batch_started = true;
		ret = idma_submit(a, false);
		if (ret)
			goto out;
	}
	if (multi) {
		if (ktime_get_ns() >= deadline) {
			ret = -ETIMEDOUT;
			goto out;
		}
		if (signal_pending(current)) {
			ret = -EINTR;
			goto out;
		}
		multi->start_attempted_mask |= BIT(0);
		multi->start_ns[0] = ktime_get_ns();
	}
	v->batch_started = true;
	ret = idma_submit(v, false);
	if (ret)
		goto out;
	v->deadline_ns = deadline;
	for (;;) {
		if (ktime_get_ns() >= deadline) {
			ret = -ETIMEDOUT;
			break;
		}
		if (signal_pending(current)) {
			ret = -EINTR;
			break;
		}
		ret = idma_poll(v, false);
		if (ret < 0)
			break;
		if (ret == 1) {
			*end_ns = v->last_complete_ns;
			return 0;
		}
		if (!(++spins & 1023))
			cond_resched();
		cpu_relax();
	}
 out:
	*end_ns = ktime_get_ns();
	return ret;
}

static int idma_batch_window(struct idma_stream *v, struct idma_stream *a,
			     u64 deadline, u64 *end_ns)
{
	struct idma_stream *aggressors[] = { a };

	return idma_batch_window_many(v, aggressors, a ? 1 : 0,
				     deadline, end_ns, NULL);
}

static int idma_multi_stop(struct idma_stream streams[], unsigned int nr,
			   struct idma_multi_experiment *multi, bool check_running)
{
	unsigned int i;
	int ret = 0;

	/* Snapshot every live A before issuing any STOP; V's HW window is frozen. */
	if (check_running) {
		for (i = 1; i < nr; i++) {
			struct idma_stream *s = &streams[i];
			u64 status, issued;

			if (!s->batch_started || s->retain) {
				ret = -EIO;
				continue;
			}
			status = readq(s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
			issued = readq(s->idma->reg + IDMA_BATCH_ISSUED_OFFSET);
			multi->pre_stop_status[i] = status;
			multi->pre_stop_issued[i] = issued;
			multi->stop_status_valid_mask |= BIT(i);
			if (!(status & 2) || status >> 32 ||
			    issued >= IDMA_EXPERIMENT_MAX_TRANSFERS)
				ret = -EIO;
		}
	}
	for (i = 0; i < nr; i++) {
		struct idma_stream *s = &streams[i];

		if (!s->batch_configured || !s->batch_started || (multi->stopped_mask & BIT(i)))
			continue;
		multi->stop_ns[i] = ktime_get_ns();
		idma_batch_stop(s); /* STOP never cancels AXI or releases buffers. */
		multi->stopped_mask |= BIT(i);
	}
	return ret;
}

static int idma_multi_drain_collect(struct idma_stream streams[], unsigned int nr,
				    struct idma_multi_experiment *multi)
{
	u64 stamp = ktime_get_ns();
	unsigned int i;
	int ret = 0, next;

	for (i = 0; i < nr; i++) {
		struct idma_stream *s = &streams[i];

		if (!s->dev)
			continue;
		next = idma_drain(s); /* Existing timeout -> quarantine + retain. */
		multi->drain_end_ns[i] = ktime_get_ns();
		if (!next && !s->retain)
			multi->drained_mask |= BIT(i);
		else if (!ret)
			ret = next;
	}
	multi->drain_ns = ktime_get_ns() - stamp;
	for (i = 0; i < nr; i++) {
		struct idma_stream *s = &streams[i];

		next = idma_batch_collect(s); /* Existing error/issued=completed checks. */
		if (!next && s->batch_started &&
		    s->stats->completed_bytes != s->stats->completed * 4096ULL)
			next = -EIO;
		if (next && !ret)
			ret = next;
	}
	return ret;
}

static void idma_snapshot(struct riscv_iommu_hpm_session *hpm,
			  struct idma_stream *v, struct idma_stream *a,
			  struct riscv_iommu_hpm_sample *sample, u64 active[2])
{
	riscv_iommu_hpm_read(hpm, sample);
	active[0] = readq(v->idma->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET);
	active[1] = a ? readq(a->idma->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET) : 0;
}

static void idma_snapshot_delta(struct idma_experiment_stats *stats, int n,
			       const struct riscv_iommu_hpm_sample *before,
			       const struct riscv_iommu_hpm_sample *after,
			       u64 active_before, u64 active_after)
{
	stats->active_cycles = active_after - active_before;
	stats->iotlb_miss = after->iotlb_miss[n] - before->iotlb_miss[n];
	stats->s1_ptw = after->s1_ptw[n] - before->s1_ptw[n];
	stats->s2_ptw = after->s2_ptw[n] - before->s2_ptw[n];
}

static int idma_validate(const struct idma_experiment *e)
{
	if (PAGE_SIZE != SZ_4K)
		return -EOPNOTSUPP;
	if (e->abi_version != IDMA_EXPERIMENT_ABI || e->struct_size != sizeof(*e))
		return -EPROTO;
	if (e->flags || memchr_inv(e->reserved, 0, sizeof(e->reserved)))
		return -EINVAL;
	if (!e->victim_pages || e->victim_pages > IDMA_MAX_PAGES ||
	    !e->aggressor_pages || e->aggressor_pages > IDMA_MAX_PAGES ||
	    !e->chunk_bytes || e->chunk_bytes > PAGE_SIZE ||
	    PAGE_SIZE % e->chunk_bytes || !e->victim_transfers ||
	    e->victim_transfers > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    e->warmup_transfers > IDMA_EXPERIMENT_MAX_TRANSFERS ||
	    !e->run_timeout_ms || e->run_timeout_ms > IDMA_MAX_RUN_TIMEOUT_MS)
		return -EINVAL;
	return 0;
}

static bool idma_any_quarantined(void)
{
	bool quarantined = false;
	int i;

	mutex_lock(&idma_devices_lock);
	for (i = 0; i < IDMA_MAX_DEVS; i++)
		if (!IS_ERR_OR_NULL(idma_devices[i]) && idma_devices[i]->quarantined)
			quarantined = true;
	mutex_unlock(&idma_devices_lock);
	return quarantined;
}

static bool idma_observer_has_path(u64 magic)
{
	return magic == IDMA_OBS_MAGIC_V3 || magic == IDMA_OBS_MAGIC_V4 || magic == IDMA_OBS_MAGIC_V5;
}

static long idma_run_experiment_mapped(struct idma_device *victim,
		struct idma_experiment *request, bool batch, u32 aggressor_chunk, bool p0,
		struct idma_pages_context *pages, struct idma_workload_context *workload)
{
	struct idma_experiment e = *request;
	struct idma_stream streams[2] = {};
	struct idma_observation observation = { .batch = batch };
	struct idma_stream *v = &streams[0], *a = NULL;
	struct idma_device *aggressor = NULL;
	struct riscv_iommu_hpm_session *hpm = NULL;
	struct riscv_iommu_hpm_sample before, after;
	u64 active_before[2], active_after[2];
	u64 start, end = 0, deadline, stamp;
	int ret, drain_ret, verify_ret, observation_ret;
	unsigned int i, lane;

	memset(&e.status, 0, sizeof(e) - offsetof(struct idma_experiment, status));
	e.victim_did = victim->did;
	e.aggressor_did = IDMA_EXPERIMENT_NO_AGGRESSOR;
	/* ABI5 was bounded and expanded by its adapter before entering this core. */
	ret = workload ? (pages || !batch || p0 ? -EINVAL : 0) : idma_validate(&e);
	if (!ret && batch && ((!workload && !p0 && e.chunk_bytes != PAGE_SIZE) ||
			      e.victim_transfers > IDMA_BATCH_MAX_TRANSFERS))
		ret = -EINVAL;
	if (ret)
		goto out_copy;
	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		goto out_copy;
	if (idma_any_quarantined()) {
		ret = -EIO;
		goto out_unlock;
	}
	if (e.aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR) {
		aggressor = idma_get_instance(e.aggressor_id);
		if (!aggressor) {
			ret = -ENODEV;
			goto out_unlock;
		}
		if (aggressor == victim || aggressor->did == victim->did) {
			ret = -EINVAL;
			goto out_put;
		}
		e.aggressor_did = aggressor->did;
		a = &streams[1];
		if ((pages || workload) && (aggressor->id != 1 || aggressor->did != 11)) {
			ret = -EOPNOTSUPP;
			goto out_put;
		}
	}
	ret = idma_observation_prepare(&observation, batch || READ_ONCE(idma_observe));
	if (ret)
		goto out_put;
	if (pages) {
		if (!observation.path) {
			ret = -EOPNOTSUPP;
			goto out_put;
		}
		pages->request.observer_sequence = observation.sequence;
	}
	if (workload) {
		/* The new bounded geometry requires both the BTC3 producer and the
		 * OBS5/Depth64 observer. Check after clearing, under the same lock.
		 */
		if (!observation.hist_v5 || !observation.path ||
		    readq(victim->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_WORKLOAD_BATCH_MAGIC ||
		    (aggressor && readq(aggressor->reg + IDMA_BATCH_MAGIC_OFFSET) !=
		     IDMA_WORKLOAD_BATCH_MAGIC)) {
			ret = -EOPNOTSUPP;
			goto out_put;
		}
		writeq(192, observation.control->reg + IDMA_OBS_INDEX_OFFSET);
		if (readq(observation.control->reg + IDMA_OBS_DATA_OFFSET) !=
		    0x0040000020202002ULL) {
			ret = -EOPNOTSUPP;
			goto out_put;
		}
		workload->request.observer_sequence = observation.sequence;
		v->workload = &workload->streams[0];
		if (a)
			a->workload = &workload->streams[1];
	}
	if (batch && (!idma_batch_supported(victim, p0) ||
	    (aggressor && !idma_batch_supported(aggressor, p0)))) {
		ret = -EOPNOTSUPP;
		goto out_put;
	}
	if (batch && ((readq(victim->reg + IDMA_BATCH_CONTROL_OFFSET) & 2) ||
	    (aggressor && (readq(aggressor->reg + IDMA_BATCH_CONTROL_OFFSET) & 2)))) {
		ret = -EBUSY;
		goto out_put;
	}
	ret = pages ? idma_pages_prepare(v, victim, &e.victim, pages, 0) :
		idma_stream_prepare(v, victim, e.victim_pages, e.chunk_bytes, &e.victim);
	if (ret)
		goto out_cleanup;
	if (a) {
		ret = pages ? idma_pages_prepare(a, aggressor, &e.aggressor, pages, 1) :
			idma_stream_prepare(a, aggressor, e.aggressor_pages,
					      aggressor_chunk, &e.aggressor);
		if (ret)
			goto out_cleanup;
	}
	ret = riscv_iommu_hpm_acquire(v->dev, a ? a->dev : NULL, &hpm);
	if (ret)
		goto out_cleanup;
	e.hpm_configured = 1;
	/*
	 * Platform workaround: this CVA6 has no DMA snoop of the WT data cache.
	 * Its FENCE.I drains CPU writes and invalidates D-cache as well as I-cache.
	 * Hand initialized buffers to DMA before warmup; never flush per transfer.
	 */
	local_flush_icache_all();
	deadline = ktime_get_ns() + (u64)e.run_timeout_ms * NSEC_PER_MSEC;
	if (a && (!batch || e.warmup_transfers)) {
		ret = idma_submit(a, false);
		if (ret)
			goto out_drain;
	}
	ret = idma_run_window(v, a, e.warmup_transfers, deadline, false, &end);
	if (ret)
		goto out_drain;
	/* Every measured V task starts at the same address after warmup. */
	v->next_offset = 0;
	v->last_complete_ns = 0;
	if (a)
		a->last_complete_ns = 0;
	if (batch) {
		/* Software warmup is drained before enabling the autonomous mode. */
		if (a) {
			ret = idma_drain(a);
			if (ret)
				goto out_drain;
		}
		ret = idma_batch_configure(v, e.victim_transfers, p0);
		if (!ret && a)
			ret = idma_batch_configure(a, IDMA_EXPERIMENT_MAX_TRANSFERS, p0);
		if (ret)
			goto out_drain;
	}
	e.victim.in_flight_at_start = v->in_flight;
	e.aggressor.in_flight_at_start = a && a->in_flight;
	ret = idma_observation_arm(&observation, victim, aggressor, e.victim_transfers);
	if (ret)
		goto out_drain;
	stamp = ktime_get_ns();
	idma_snapshot(hpm, v, a, &before, active_before);
	start = ktime_get_ns();
	e.snapshot_before_ns = start - stamp;
	e.measurement_started = 1;
	ret = batch ? idma_batch_window(v, a, deadline, &end) :
		idma_run_window(v, a, e.victim_transfers, deadline, true, &end);
	e.victim_elapsed_ns = end - start;
	e.measurement_complete = !ret;
	e.victim.in_flight_at_end = v->in_flight;
	e.aggressor.in_flight_at_end = a && a->in_flight;
	if ((pages || workload) && a && a->batch_started) {
		u64 control = readq(a->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
		u64 issued = readq(a->idma->reg + IDMA_BATCH_ISSUED_OFFSET);

		if (pages) {
			pages->request.pre_stop_A_control = control;
			pages->request.pre_stop_A_issued = issued;
			pages->request.A_status_valid = 1;
		}
		if (!ret && (!(control & 2) || control >> 32 ||
		    issued >= IDMA_EXPERIMENT_MAX_TRANSFERS))
			ret = -EIO;
	}
	/* Stop autonomous A after V observation; hardware window is already frozen. */
	if (batch) {
		idma_batch_stop(a);
		if (ret)
			idma_batch_stop(v);
	}
	/* No more software A refills after V's final observed completion. */
	idma_snapshot(hpm, v, a, &after, active_after);
	e.snapshot_after_ns = ktime_get_ns() - end;
	idma_snapshot_delta(&e.victim, 0, &before, &after,
			    active_before[0], active_after[0]);
	if (a)
		idma_snapshot_delta(&e.aggressor, 1, &before, &after,
				    active_before[1], active_after[1]);
 out_drain:
	if (batch) {
		idma_batch_stop(v);
		idma_batch_stop(a);
	}
	stamp = ktime_get_ns();
	drain_ret = idma_drain(v);
	if (drain_ret && !ret)
		ret = drain_ret;
	if (a) {
		drain_ret = idma_drain(a);
		if (drain_ret && !ret)
			ret = drain_ret;
	}
	e.drain_ns = ktime_get_ns() - stamp;
	if (batch) {
		drain_ret = idma_batch_collect(v);
		if (drain_ret && !ret)
			ret = drain_ret;
		drain_ret = idma_batch_collect(a);
		if (drain_ret && !ret)
			ret = drain_ret;
		if (!ret && e.victim.completed != e.victim_transfers)
			ret = -EIO;
	}
	riscv_iommu_hpm_release(hpm);
	if (workload)
		for (lane = 0; lane < (a ? 2 : 1); lane++) {
			struct idma_workload_stream *w = &workload->streams[lane];

			idma_workload_prefix_bitmap(w->warmup, w->geometry, w->warmup_completed);
		}
	if (!ret) {
		/* Both lanes are drained: reacquire CPU visibility before any check. */
		local_flush_icache_all();
		verify_ret = idma_verify(v);
		if (verify_ret)
			ret = verify_ret;
		if (a) {
			verify_ret = idma_verify(a);
			if (verify_ret && !ret)
				ret = verify_ret;
		}
	}
 out_cleanup:
	if (workload)
		for (lane = 0; lane < (a ? 2 : 1); lane++) {
			struct idma_workload_stream *w = &workload->streams[lane];
			struct idma_workload_experiment *p = &workload->request;
			struct idma_workload_coverage *coverage = lane ?
				&p->aggressor_coverage : &p->victim_coverage;
			u64 bytes;

			coverage->measured_distinct_bytes =
				(u64)bitmap_weight(w->measured, IDMA_WORKLOAD_BITMAP_BITS) *
				IDMA_WORKLOAD_GRANULE;
			coverage->measured_distinct_pages =
				idma_workload_distinct_pages(w->measured, w->geometry);
			coverage->warmup_distinct_pages =
				idma_workload_distinct_pages(w->warmup, w->geometry);
			if (w->data_verified)
				coverage->verified_union_bytes =
					idma_workload_union_bytes(w->warmup, w->measured);
			if (check_mul_overflow(w->warmup_completed, (u64)w->geometry->row_bytes, &bytes)) {
				bytes = 0;
				if (!ret)
					ret = -EOVERFLOW;
			}
			if (lane) {
				p->aggressor_warmup_descriptors = w->warmup_completed;
				p->aggressor_warmup_bytes = bytes;
			} else {
				p->victim_warmup_descriptors = w->warmup_completed;
				p->victim_warmup_bytes = bytes;
			}
		}
	if (pages && !v->in_flight && !v->retain &&
	    (!a || (!a->in_flight && !a->retain))) {
		for (lane = 0; lane < (a ? 2 : 1); lane++) {
			struct idma_stream *s = &streams[lane];

			if (!s->dev || !s->size || s->retain || s->in_flight)
				continue;
			for (i = 0; i < 2; i++) {
				struct idma_pages_buffer *b = &pages->request.buffers[2 * lane + i];

				if (b->mapped_bytes != s->size)
					continue;
				verify_ret = idma_pages_verify_buffer(s, b,
						pages->request.mapping_page_bytes, true);
				if (!verify_ret)
					pages->request.verified_after_mask |= BIT(2 * lane + i);
				else if (!ret)
					ret = verify_ret;
			}
		}
	}
	observation_ret = idma_observation_finish(&observation, &e, ret);
	if (observation_ret && !ret)
		ret = observation_ret;
	if (batch) {
		idma_batch_disable(v);
		idma_batch_disable(a);
	}
	if (pages) {
		if (a) {
			verify_ret = idma_pages_cleanup(a, pages, 1);
			if (verify_ret && !ret)
				ret = verify_ret;
		}
		verify_ret = idma_pages_cleanup(v, pages, 0);
		if (verify_ret && !ret)
			ret = verify_ret;
	} else {
		if (a)
			idma_stream_cleanup(a);
		idma_stream_cleanup(v);
	}
 out_put:
	idma_put_instance(observation.control);
	idma_put_instance(aggressor);
 out_unlock:
	mutex_unlock(&idma_experiment_lock);
 out_copy:
	e.status = ret;
	*request = e;
	return ret;
}

static long idma_run_experiment_core(struct idma_device *victim,
		struct idma_experiment *request, bool batch, u32 aggressor_chunk, bool p0)
{
	return idma_run_experiment_mapped(victim, request, batch, aggressor_chunk, p0, NULL, NULL);
}

static long idma_run_experiment_mode(struct idma_device *victim,
			       struct idma_experiment __user *uarg, bool batch)
{
	struct idma_experiment e;
	long ret;

	if (copy_from_user(&e, uarg, sizeof(e)))
		return -EFAULT;
	ret = idma_run_experiment_core(victim, &e, batch, e.chunk_bytes, false);
	if (copy_to_user(uarg, &e, sizeof(e)))
		return -EFAULT;
	return ret;
}

static int idma_validate_p0(struct idma_device *victim,
			    const struct idma_p0_experiment *e)
{
	if (e->abi_version != IDMA_P0_ABI || e->struct_size != sizeof(*e))
		return -EPROTO;
	if (PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10)
		return -EOPNOTSUPP;
	if (e->flags || memchr_inv(e->reserved, 0, sizeof(e->reserved)))
		return -EINVAL;
	/* Deliberately bounded to the approved P0 matrix, rather than a general ABI. */
	if (e->geometry != IDMA_P0_GEOMETRY_PACKED_RING || e->victim_pages != 1 ||
	    (e->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR && e->aggressor_id != 1) ||
	    (e->aggressor_pages != 1 && e->aggressor_pages != 8) ||
	    (e->aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR && e->aggressor_pages != 1) ||
	    (e->victim_chunk_bytes != 64 && e->victim_chunk_bytes != 1024 &&
	     e->victim_chunk_bytes != 4096) || e->aggressor_chunk_bytes != 4096 ||
	    e->victim_transfers != 4096 || e->warmup_transfers != 256 ||
	    !e->run_timeout_ms || e->run_timeout_ms > IDMA_MAX_RUN_TIMEOUT_MS)
		return -EINVAL;
	/* Sizes are <=8 pages, V bytes <=2^24, and even A's target*len <=2^44.
	 * Per-stream collection nevertheless checks multiplication before use.
	 */
	return 0;
}

static long idma_run_p0(struct idma_device *victim,
			struct idma_p0_experiment __user *uarg)
{
	struct idma_p0_experiment p;
	struct idma_experiment e = {};
	long ret;

	if (copy_from_user(&p, uarg, sizeof(p)))
		return -EFAULT;
	memset(&p.status, 0, sizeof(p) - offsetof(struct idma_p0_experiment, status));
	p.victim_did = victim->did;
	p.aggressor_did = IDMA_EXPERIMENT_NO_AGGRESSOR;
	ret = idma_validate_p0(victim, &p);
	if (ret)
		goto out;
	/* Translate inputs to the unchanged internal ABI1-shaped request. */
	e.abi_version = IDMA_EXPERIMENT_ABI;
	e.struct_size = sizeof(e);
	e.aggressor_id = p.aggressor_id;
	e.victim_pages = p.victim_pages;
	e.aggressor_pages = p.aggressor_pages;
	e.chunk_bytes = p.victim_chunk_bytes;
	e.run_timeout_ms = p.run_timeout_ms;
	e.victim_transfers = p.victim_transfers;
	e.warmup_transfers = p.warmup_transfers;
	ret = idma_run_experiment_core(victim, &e, true, p.aggressor_chunk_bytes, true);
	/* Named copies avoid relying on equal struct sizes to alias ABI layouts. */
	p.victim_did = e.victim_did;
	p.aggressor_did = e.aggressor_did;
	p.hpm_configured = e.hpm_configured;
	p.measurement_started = e.measurement_started;
	p.measurement_complete = e.measurement_complete;
	p.victim_elapsed_ns = e.victim_elapsed_ns;
	p.snapshot_before_ns = e.snapshot_before_ns;
	p.snapshot_after_ns = e.snapshot_after_ns;
	p.drain_ns = e.drain_ns;
	p.victim = e.victim;
	p.aggressor = e.aggressor;
 out:
	p.status = ret;
	if (copy_to_user(uarg, &p, sizeof(p)))
		return -EFAULT;
	return ret;
}

static long idma_caps_p0(struct idma_device *victim, void __user *uarg)
{
	struct idma_p0_caps caps = {
		.abi_version = IDMA_P0_ABI,
		.struct_size = sizeof(caps),
		.capabilities = IDMA_EXPERIMENT_CAP_HW_BATCH,
		.device_id = victim->id,
		.did = victim->did,
		.max_pages = IDMA_MAX_PAGES,
		.page_bytes = PAGE_SIZE,
		.max_run_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS,
		.batch_magic = IDMA_P0_BATCH_MAGIC,
		.victim_length_mask = IDMA_P0_LENGTH_64 | IDMA_P0_LENGTH_1024 | IDMA_P0_LENGTH_4096,
		.aggressor_length_mask = IDMA_P0_LENGTH_4096,
		.geometry_mask = IDMA_P0_GEOMETRY_MASK_PACKED_RING,
	};

	if (victim->id != 0 || victim->did != 10 || !idma_batch_supported(victim, true))
		return -EOPNOTSUPP;
	if (riscv_iommu_hpm_supported(victim->miscdev.parent))
		caps.capabilities |= IDMA_EXPERIMENT_CAP_DID_HPM;
	return copy_to_user(uarg, &caps, sizeof(caps)) ? -EFAULT : 0;
}

static int idma_multi_validate(struct idma_device *victim,
			       const struct idma_multi_experiment *multi)
{
	u32 pages, same_total;
	unsigned int i;

	if (PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10)
		return -EOPNOTSUPP;
	if (multi->abi_version != IDMA_MULTI_ABI || multi->struct_size != sizeof(*multi) ||
	    multi->flags || multi->reserved0 || multi->nr_devices < 2 ||
	    multi->nr_devices > IDMA_MULTI_MAX_DEVICES || multi->path_aggressor_index != 1 ||
	    multi->chunk_bytes != SZ_4K || multi->victim_transfers != 4096 ||
	    multi->warmup_transfers != 256 || !multi->run_timeout_ms ||
	    multi->run_timeout_ms > IDMA_MAX_RUN_TIMEOUT_MS || multi->pages[0] != 1)
		return -EINVAL;
	for (i = 0; i < ARRAY_SIZE(multi->reserved); i++)
		if (multi->reserved[i])
			return -EINVAL;
	pages = multi->pages[1];
	same_total = 6 / (multi->nr_devices - 1);
	if (pages != 1 && pages != 8 && pages != same_total)
		return -EINVAL;
	for (i = 0; i < IDMA_MULTI_MAX_DEVICES; i++) {
		if (i < multi->nr_devices) {
			if (multi->device_id[i] != i || (i && multi->pages[i] != pages))
				return -EINVAL;
		} else if (multi->device_id[i] || multi->pages[i]) {
			return -EINVAL;
		}
	}
	return 0;
}

static void idma_multi_snapshot(struct riscv_iommu_hpm_session *hpm,
				struct idma_multi_context *context,
				struct riscv_iommu_hpm_multi_sample *sample,
				u64 active[], unsigned int nr)
{
	unsigned int i;

	riscv_iommu_hpm_read_multi(hpm, sample);
	for (i = 0; i < nr; i++)
		active[i] = readq(context->devices[i]->reg + IDMA_ACTIVE_CYCLES_REG_OFFSET);
}

static long idma_run_multi(struct idma_device *victim,
			   struct idma_multi_experiment __user *uarg)
{
	struct idma_multi_experiment multi;
	struct idma_multi_context *context;
	struct riscv_iommu_hpm_session *hpm = NULL;
	struct idma_experiment observed = {};
	u64 start, end = 0, stamp, deadline;
	unsigned int i, nr;
	int ret, next;

	if (copy_from_user(&multi, uarg, sizeof(multi)))
		return -EFAULT;
	memset(&multi.status, 0, sizeof(multi) - offsetof(struct idma_multi_experiment, status));
	ret = idma_multi_validate(victim, &multi);
	if (ret)
		goto out_copy;
	nr = multi.nr_devices;
	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context) {
		ret = -ENOMEM;
		goto out_copy;
	}
	context->observation.batch = true;
	context->devices[0] = victim;
	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		goto out_free;
	if (idma_any_quarantined()) {
		ret = -EIO;
		goto out_put;
	}
	for (i = 0; i < nr; i++) {
		struct idma_device *device;

		if (i)
			context->devices[i] = idma_get_instance(multi.device_id[i]);
		device = context->devices[i];
		if (!device) {
			ret = -ENODEV;
			goto out_put;
		}
		multi.did[i] = device->did;
		if (device->did != 10U + i || !idma_batch_supported(device, false)) {
			ret = -EOPNOTSUPP;
			goto out_put;
		}
		if (readq(device->reg + IDMA_BATCH_CONTROL_OFFSET) & 2) {
			ret = -EBUSY;
			goto out_put;
		}
		if (i)
			context->aggressors[i - 1] = &context->streams[i];
	}
	ret = idma_observation_prepare(&context->observation, true);
	if (ret)
		goto out_put;
	if (!context->observation.path) {
		ret = -EOPNOTSUPP;
		goto out_put;
	}
	multi.observer_sequence = context->observation.sequence;
	for (i = 0; i < nr; i++) {
		ret = idma_stream_prepare(&context->streams[i], context->devices[i],
					  multi.pages[i], SZ_4K, &multi.stats[i]);
		if (ret)
			goto out_cleanup;
		multi.prepared_mask |= BIT(i);
	}
	ret = riscv_iommu_hpm_acquire_multi((struct device *const []) {
		context->streams[0].dev, context->streams[1].dev,
		context->streams[2].dev, context->streams[3].dev }, nr, &hpm);
	if (ret)
		goto out_cleanup;
	multi.hpm_configured = 1;
	local_flush_icache_all();
	deadline = ktime_get_ns() + (u64)multi.run_timeout_ms * NSEC_PER_MSEC;
	for (i = 1; i < nr; i++) {
		ret = idma_submit(&context->streams[i], false);
		if (ret)
			goto out_drain;
	}
	ret = idma_run_window_many(&context->streams[0], context->aggressors, nr - 1,
				   multi.warmup_transfers, deadline, false, &end);
	if (ret)
		goto out_drain;
	for (i = 0; i < nr; i++) {
		ret = idma_drain(&context->streams[i]);
		if (ret)
			goto out_drain;
		context->streams[i].last_complete_ns = 0;
	}
	context->streams[0].next_offset = 0;
	for (i = 0; i < nr; i++) {
		u64 target = i ? IDMA_EXPERIMENT_MAX_TRANSFERS : multi.victim_transfers;

		ret = idma_batch_configure(&context->streams[i], target, false);
		if (ret)
			goto out_drain;
	}
	for (i = 0; i < nr; i++)
		multi.stats[i].in_flight_at_start = context->streams[i].in_flight;
	ret = idma_observation_arm(&context->observation, victim, context->devices[1],
				   multi.victim_transfers);
	if (ret)
		goto out_drain;
	stamp = ktime_get_ns();
	idma_multi_snapshot(hpm, context, &context->before, context->active_before, nr);
	start = ktime_get_ns();
	multi.snapshot_before_ns = start - stamp;
	multi.measurement_started = 1;
	ret = idma_batch_window_many(&context->streams[0], context->aggressors, nr - 1,
				     deadline, &end, &multi);
	multi.victim_elapsed_ns = end - start;
	multi.measurement_complete = !ret;
	for (i = 0; i < nr; i++)
		multi.stats[i].in_flight_at_end = context->streams[i].in_flight;
	next = idma_multi_stop(context->streams, nr, &multi, !ret);
	if (next && !ret)
		ret = next;
	idma_multi_snapshot(hpm, context, &context->after, context->active_after, nr);
	multi.snapshot_after_ns = ktime_get_ns() - end;
	for (i = 0; i < nr; i++) {
		multi.stats[i].active_cycles = context->active_after[i] - context->active_before[i];
		multi.stats[i].iotlb_miss = context->after.iotlb_miss[i] -
					  context->before.iotlb_miss[i];
		multi.stats[i].s1_ptw = context->after.s1_ptw[i] - context->before.s1_ptw[i];
		multi.stats[i].s2_ptw = context->after.s2_ptw[i] - context->before.s2_ptw[i];
	}
out_drain:
	/* Cover every started lane even after a partial warmup/start or V error. */
	idma_multi_stop(context->streams, nr, &multi, false);
	next = idma_multi_drain_collect(context->streams, nr, &multi);
	if (next && !ret)
		ret = next;
	if (!ret && multi.stats[0].completed != multi.victim_transfers)
		ret = -EIO;
	riscv_iommu_hpm_release(hpm);
	hpm = NULL;
	if (!ret) {
		local_flush_icache_all();
		for (i = 0; i < nr; i++) {
			next = idma_verify(&context->streams[i]);
			if (!next)
				multi.verified_mask |= BIT(i);
			else if (!ret)
				ret = next;
		}
	}
out_cleanup:
	riscv_iommu_hpm_release(hpm);
	observed.victim_transfers = multi.victim_transfers;
	observed.victim_did = multi.did[0];
	observed.aggressor_did = multi.did[1];
	next = idma_observation_finish(&context->observation, &observed, ret);
	if (next && !ret)
		ret = next;
	for (i = nr; i > 0; i--) {
		idma_batch_disable(&context->streams[i - 1]);
		idma_stream_cleanup(&context->streams[i - 1]);
	}
out_put:
	idma_put_instance(context->observation.control);
	for (i = 1; i < nr; i++)
		idma_put_instance(context->devices[i]);
	mutex_unlock(&idma_experiment_lock);
out_free:
	kfree(context);
out_copy:
	multi.status = ret;
	if (copy_to_user(uarg, &multi, sizeof(multi)))
		return -EFAULT;
	return ret;
}

static long idma_caps_multi(struct idma_device *victim, void __user *uarg)
{
	struct idma_multi_caps caps = {
		.abi_version = IDMA_MULTI_ABI,
		.struct_size = sizeof(caps),
		.device_id = victim->id,
		.did = victim->did,
		.max_devices = IDMA_MULTI_MAX_DEVICES,
		.max_pages = IDMA_MULTI_MAX_PAGES,
		.page_bytes = PAGE_SIZE,
		.max_run_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS,
	};

	if (victim->id != 0 || victim->did != 10 || PAGE_SIZE != SZ_4K)
		return -EOPNOTSUPP;
	if (idma_batch_supported(victim, false))
		caps.capabilities |= IDMA_MULTI_CAP_HW_BATCH;
	if (riscv_iommu_hpm_supported(victim->miscdev.parent))
		caps.capabilities |= IDMA_MULTI_CAP_ALL_DID_HPM;
	if (idma_observer_has_path(readq(victim->reg + IDMA_OBS_MAGIC_OFFSET)))
		caps.capabilities |= IDMA_MULTI_CAP_SELECTED_PATH;
	return copy_to_user(uarg, &caps, sizeof(caps)) ? -EFAULT : 0;
}

static int idma_pages_validate(struct idma_device *victim,
			       const struct idma_pages_experiment *p)
{
	if (p->abi_version != IDMA_PAGES_ABI || p->struct_size != sizeof(*p))
		return -EPROTO;
	if (!IS_REACHABLE(CONFIG_RISCV_IOMMU) || !IS_ENABLED(CONFIG_64BIT))
		return -EOPNOTSUPP;
	if (PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10)
		return -EOPNOTSUPP;
	if (p->flags || p->reserved0 || memchr_inv(p->reserved, 0, sizeof(p->reserved)) ||
	    (p->mapping_page_bytes != SZ_4K && p->mapping_page_bytes != SZ_2M) ||
	    p->buffer_bytes != IDMA_PAGES_BUFFER_BYTES || p->chunk_bytes != SZ_4K ||
	    p->victim_transfers != 4096 || p->warmup_transfers != 256 ||
	    !p->run_timeout_ms || p->run_timeout_ms > IDMA_MAX_RUN_TIMEOUT_MS)
		return -EINVAL;
	return 0;
}

static long idma_run_pages(struct idma_device *victim,
			  struct idma_pages_experiment __user *uarg)
{
	struct idma_pages_context *context;
	struct idma_pages_experiment *p;
	struct idma_experiment *e;
	long ret;

	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context)
		return -ENOMEM;
	p = &context->request;
	if (copy_from_user(p, uarg, sizeof(*p))) {
		ret = -EFAULT;
		goto out_free;
	}
	memset(&p->status, 0, sizeof(*p) - offsetof(struct idma_pages_experiment, status));
	ret = idma_pages_validate(victim, p);
	if (ret)
		goto out_copy;
	e = &p->experiment;
	e->abi_version = IDMA_EXPERIMENT_ABI;
	e->struct_size = sizeof(*e);
	e->aggressor_id = 1;
	e->victim_pages = IDMA_PAGES_BUFFER_BYTES / PAGE_SIZE;
	e->aggressor_pages = IDMA_PAGES_BUFFER_BYTES / PAGE_SIZE;
	e->chunk_bytes = p->chunk_bytes;
	e->run_timeout_ms = p->run_timeout_ms;
	e->victim_transfers = p->victim_transfers;
	e->warmup_transfers = p->warmup_transfers;
	ret = idma_run_experiment_mapped(victim, e, true, SZ_4K, false, context, NULL);
out_copy:
	p->status = ret;
	if (copy_to_user(uarg, p, sizeof(*p)))
		ret = -EFAULT;
out_free:
	kfree(context);
	return ret;
}

static long idma_caps_pages(struct idma_device *victim, void __user *uarg)
{
	struct iommu_domain *domain = iommu_get_domain_for_dev(victim->miscdev.parent);
	struct idma_pages_caps caps = {
		.abi_version = IDMA_PAGES_ABI,
		.struct_size = sizeof(caps),
		.device_id = victim->id,
		.did = victim->did,
		.kernel_page_bytes = PAGE_SIZE,
		.buffer_bytes = IDMA_PAGES_BUFFER_BYTES,
		.chunk_bytes = SZ_4K,
		.max_run_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS,
	};

	if (PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10)
		return -EOPNOTSUPP;
	if (idma_batch_supported(victim, false))
		caps.capabilities |= IDMA_PAGES_CAP_BATCH;
	if (riscv_iommu_hpm_supported(victim->miscdev.parent))
		caps.capabilities |= IDMA_PAGES_CAP_HPM;
	if (idma_observer_has_path(readq(victim->reg + IDMA_OBS_MAGIC_OFFSET)))
		caps.capabilities |= IDMA_PAGES_CAP_PATH;
	if (IS_REACHABLE(CONFIG_RISCV_IOMMU) && IS_ENABLED(CONFIG_64BIT))
		caps.capabilities |= IDMA_PAGES_CAP_LEAF_API;
	if (domain) {
		if (domain->pgsize_bitmap & SZ_4K)
			caps.mapping_page_mask |= IDMA_PAGES_MAP_4K;
		if (domain->pgsize_bitmap & SZ_2M)
			caps.mapping_page_mask |= IDMA_PAGES_MAP_2M;
	}
	return copy_to_user(uarg, &caps, sizeof(caps)) ? -EFAULT : 0;
}

static int idma_workload_validate(struct idma_device *victim,
		const struct idma_workload_experiment *p, u64 *count, u64 *warmup)
{
	int ret;

	if (p->abi_version != IDMA_WORKLOAD_ABI || p->struct_size != sizeof(*p))
		return -EPROTO;
	if (!IS_REACHABLE(CONFIG_RISCV_IOMMU) || !IS_ENABLED(CONFIG_64BIT) ||
	    PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10)
		return -EOPNOTSUPP;
	if (p->flags || p->reserved0 || p->reserved1 ||
	    memchr_inv(p->reserved, 0, sizeof(p->reserved)) ||
	    p->supply != IDMA_WORKLOAD_SUPPLY_HW_BATCH ||
	    (p->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR && p->aggressor_id != 1) ||
	    !p->run_timeout_ms || p->run_timeout_ms > IDMA_WORKLOAD_MAX_TIMEOUT_MS)
		return -EINVAL;
	ret = idma_workload_geometry_validate(&p->victim_geometry);
	if (ret)
		return ret;
	if (p->aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR) {
		if (memchr_inv(&p->aggressor_geometry, 0, sizeof(p->aggressor_geometry)))
			return -EINVAL;
	} else {
		ret = idma_workload_geometry_validate(&p->aggressor_geometry);
		if (ret)
			return ret;
	}
	if (!p->victim_operations ||
	    check_mul_overflow(p->victim_operations,
			       (u64)p->victim_geometry.rows_per_operation, count) ||
	    check_mul_overflow(p->victim_warmup_operations,
			       (u64)p->victim_geometry.rows_per_operation, warmup) ||
	    *count > IDMA_WORKLOAD_MAX_DESCRIPTORS ||
	    *warmup > IDMA_WORKLOAD_MAX_WARMUP_DESCRIPTORS)
		return -EINVAL;
	return 0;
}

static long idma_run_workload(struct idma_device *victim,
			      struct idma_workload_experiment __user *uarg)
{
	struct idma_workload_context *context;
	struct idma_workload_experiment *p;
	struct idma_experiment e = {};
	u64 count, warmup;
	long ret;

	/* Both prefix bitmaps live on the heap, outside the kernel stack and the
	 * timed path. No user pointer survives the initial request copy.
	 */
	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context)
		return -ENOMEM;
	p = &context->request;
	if (copy_from_user(p, uarg, sizeof(*p))) {
		ret = -EFAULT;
		goto out_free;
	}
	memset(&p->status, 0, sizeof(*p) - offsetof(struct idma_workload_experiment, status));
	p->victim_did = victim->did;
	p->aggressor_did = IDMA_EXPERIMENT_NO_AGGRESSOR;
	ret = idma_workload_validate(victim, p, &count, &warmup);
	if (ret)
		goto out_copy;
	context->streams[0].geometry = &p->victim_geometry;
	context->streams[1].geometry = &p->aggressor_geometry;
	e.abi_version = IDMA_EXPERIMENT_ABI;
	e.struct_size = sizeof(e);
	e.aggressor_id = p->aggressor_id;
	e.victim_pages = p->victim_geometry.ring_pages;
	e.aggressor_pages = p->aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR ? 1 :
		p->aggressor_geometry.ring_pages;
	e.chunk_bytes = p->victim_geometry.row_bytes;
	e.run_timeout_ms = p->run_timeout_ms;
	e.victim_transfers = count;
	e.warmup_transfers = warmup;
	ret = idma_run_experiment_mapped(victim, &e, true,
		p->aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR ? SZ_4K :
		p->aggressor_geometry.row_bytes, false, NULL, context);
	/* Keep ABI layouts independent; completion counts are actual row COPYs. */
	p->victim_did = e.victim_did;
	p->aggressor_did = e.aggressor_did;
	p->hpm_configured = e.hpm_configured;
	p->measurement_started = e.measurement_started;
	p->measurement_complete = e.measurement_complete;
	p->victim_elapsed_ns = e.victim_elapsed_ns;
	p->snapshot_before_ns = e.snapshot_before_ns;
	p->snapshot_after_ns = e.snapshot_after_ns;
	p->drain_ns = e.drain_ns;
	p->victim = e.victim;
	p->aggressor = e.aggressor;
out_copy:
	p->status = ret;
	if (copy_to_user(uarg, p, sizeof(*p)))
		ret = -EFAULT;
out_free:
	kfree(context);
	return ret;
}

static long idma_caps_workload(struct idma_device *victim, void __user *uarg)
{
	struct idma_workload_caps caps = {
		.abi_version = IDMA_WORKLOAD_ABI,
		.struct_size = sizeof(caps),
		.capabilities = IDMA_EXPERIMENT_CAP_HW_BATCH,
		.device_id = victim->id,
		.did = victim->did,
		.max_pages = IDMA_WORKLOAD_MAX_PAGES,
		.max_rows_per_op = IDMA_WORKLOAD_MAX_ROWS,
		.max_operation_bytes = IDMA_WORKLOAD_MAX_OPERATION_BYTES,
		.batch_magic = IDMA_WORKLOAD_BATCH_MAGIC,
		.observer_magic = IDMA_WORKLOAD_OBSERVER_MAGIC,
		.geometry_mask = BIT(IDMA_WORKLOAD_LINEAR_RING) |
			BIT(IDMA_WORKLOAD_PAGE_PACKED_RING) | BIT(IDMA_WORKLOAD_TILE_ROWS),
		.row_length_mask = IDMA_WORKLOAD_LENGTH_64 | IDMA_WORKLOAD_LENGTH_256 |
			IDMA_WORKLOAD_LENGTH_1024 | IDMA_WORKLOAD_LENGTH_1536 |
			IDMA_WORKLOAD_LENGTH_4096 | IDMA_WORKLOAD_LENGTH_16384 |
			IDMA_WORKLOAD_LENGTH_65536,
		.path_queue_depth = IDMA_WORKLOAD_PATH_DEPTH,
		.max_run_timeout_ms = IDMA_WORKLOAD_MAX_TIMEOUT_MS,
	};

	/* CAPS is read-only. RUN checks exact INFO2/depth after locked reset. */
	if (!IS_REACHABLE(CONFIG_RISCV_IOMMU) || !IS_ENABLED(CONFIG_64BIT) ||
	    PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10 ||
	    readq(victim->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_WORKLOAD_BATCH_MAGIC ||
	    readq(victim->reg + IDMA_OBS_MAGIC_OFFSET) != IDMA_WORKLOAD_OBSERVER_MAGIC)
		return -EOPNOTSUPP;
	if (riscv_iommu_hpm_supported(victim->miscdev.parent))
		caps.capabilities |= IDMA_EXPERIMENT_CAP_DID_HPM;
	return copy_to_user(uarg, &caps, sizeof(caps)) ? -EFAULT : 0;
}
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
    hist_info=0x0040000020202002ULL;
    path_disabled=false;contexts=context_frees=hpm_nr=0;

    memset(hw,0,sizeof(hw));memset(idmas,0,sizeof(idmas));memset(idma_devices,0,sizeof(idma_devices));
    memset(maps,0,sizeof(maps));map_count=allocs=frees=stop_writes=flushes=log_errors=lock_depth=observer_calls=0;
    zero_id=drain_failure=verify_failure=-1;clock_ns=0;signal_flag=false;
    verify_offset=0;verify_source=false;stop_unquiet=-1;
    memset(stop_seen,0,sizeof(stop_seen));
    for(int i=0;i<4;i++){domains[i].pgsize_bitmap=SZ_4K|SZ_2M;idmas[i].id=i;idmas[i].did=10+i;idmas[i].reg=hw[i].regs;
        idmas[i].miscdev.parent=&devs[i];idma_devices[i]=&idmas[i];hw[i].regs[0x80/8]=magic;hw[i].regs[IDMA_OBS_MAGIC_OFFSET/8]=IDMA_OBS_MAGIC_V3;hw[i].next_id=10;}
}
static struct idma_p0_experiment request(u32 len,u32 a){return (struct idma_p0_experiment){
    .abi_version=2,.struct_size=sizeof(struct idma_p0_experiment),.aggressor_id=a?1:IDMA_EXPERIMENT_NO_AGGRESSOR,
    .victim_pages=1,.aggressor_pages=a?a:1,.victim_chunk_bytes=len,.aggressor_chunk_bytes=4096,
    .run_timeout_ms=60000,.geometry=1,.victim_transfers=4096,.warmup_transfers=256};}
static unsigned legacy_cases(void){unsigned checks=0;
    for(u32 len=64;len<=4096;len=len==64?1024:len==1024?4096:8192)for(unsigned a=0;a<3;a++){
        fprintf(stderr,"legacy case len=%u a=%u\n",len,a);
        reset(IDMA_P0_BATCH_MAGIC);struct idma_p0_experiment p=request(len,a==2?8:a);
        assert(idma_run_p0(&idmas[0],&p)==0&&p.status==0&&p.measurement_complete);
        assert(p.victim.completed==4096&&p.victim.completed_bytes==4096ULL*len&&hw[0].last_len==len);
        if(a)assert(p.aggressor.completed_bytes==p.aggressor.completed*4096&&hw[1].last_len==4096&&stop_writes>=1);
        assert(allocs==frees&&!lock_depth&&!p.victim.data_errors&&!p.aggressor.data_errors);checks++;
    }
    for(unsigned m=1;m<=2;m++)for(unsigned batch=0;batch<=1;batch++)for(unsigned a=0;a<=1;a++){
        reset(m==1?IDMA_BATCH_MAGIC:IDMA_P0_BATCH_MAGIC);
        struct idma_experiment e={.abi_version=1,.struct_size=sizeof(e),.aggressor_id=a?1:IDMA_EXPERIMENT_NO_AGGRESSOR,
            .victim_pages=1,.aggressor_pages=8,.chunk_bytes=4096,.run_timeout_ms=60000,.victim_transfers=4096,.warmup_transfers=256};
        assert(idma_run_experiment_mode(&idmas[0],&e,batch)==0&&e.victim.completed_bytes==16777216);
        assert(allocs==frees&&!lock_depth);checks++;
    }
    reset(IDMA_BATCH_MAGIC);struct idma_p0_experiment p=request(64,8);
    assert(idma_run_p0(&idmas[0],&p)==-EOPNOTSUPP&&allocs==0&&!p.measurement_started);checks++;
    for(unsigned invalid=0;invalid<12;invalid++){
        reset(IDMA_P0_BATCH_MAGIC);p=request(64,8);
        switch(invalid){case 0:p.reserved[5]=1;break;case 1:p.flags=1;break;case 2:p.abi_version=1;break;
        case 3:p.struct_size--;break;case 4:p.victim_pages=2;break;case 5:p.aggressor_pages=2;break;
        case 6:p.victim_chunk_bytes=256;break;case 7:p.aggressor_chunk_bytes=64;break;case 8:p.geometry=0;break;
        case 9:p.victim_transfers=~0ULL;break;case 10:p.warmup_transfers=0;break;case 11:p.run_timeout_ms=300001;break;}
        assert(idma_run_p0(&idmas[0],&p)<0&&allocs==0&&!p.measurement_started);checks++;
    }
    reset(IDMA_P0_BATCH_MAGIC);struct idma_p0_caps caps;
    assert(idma_caps_p0(&idmas[0],&caps)==0&&caps.abi_version==2&&caps.batch_magic==IDMA_P0_BATCH_MAGIC);checks++;
    reset(IDMA_BATCH_MAGIC);assert(idma_caps_p0(&idmas[0],&caps)==-EOPNOTSUPP);checks++;
    for(int lane=0;lane<2;lane++){
        reset(IDMA_P0_BATCH_MAGIC);zero_id=lane;p=request(64,8);
        assert(idma_run_p0(&idmas[0],&p)==-EIO&&idmas[lane].quarantined&&allocs>frees);
        assert((lane?p.aggressor.quarantined:p.victim.quarantined)==1&&!lock_depth);checks++;
    }
    reset(IDMA_P0_BATCH_MAGIC);drain_failure=1;p=request(1024,8);
    assert(idma_run_p0(&idmas[0],&p)==-ETIMEDOUT&&p.aggressor.quarantined&&allocs>frees&&!lock_depth);checks++;
    reset(IDMA_P0_BATCH_MAGIC);verify_failure=0;p=request(64,8);
    assert(idma_run_p0(&idmas[0],&p)==-EILSEQ&&p.victim.data_errors&&allocs==frees&&!p.victim.quarantined);checks++;
    // Actual prepare/collect/verify helpers: measured prefix must not erase warmup.
    reset(IDMA_P0_BATCH_MAGIC);struct idma_stream s={};struct idma_experiment_stats stats={};
    assert(idma_stream_prepare(&s,&idmas[0],1,64,&stats)==0);
    memcpy(s.dst,s.src,256);s.touched_end=256;s.batch_configured=s.batch_started=true;
    hw[0].regs[0xa0/8]=hw[0].regs[0xa8/8]=1;hw[0].regs[0x88/8]=9;
    assert(idma_batch_collect(&s)==0&&stats.completed_bytes==64&&s.touched_end==256&&idma_verify(&s)==0);checks++;
    ((u8*)s.dst)[1024]^=1;assert(idma_verify(&s)==-EILSEQ&&stats.data_errors==1);checks++;
    idma_stream_cleanup(&s);assert(allocs==frees);
    printf("PASS %u legacy/P0 actual-function scenarios; LEGACY_TRACE=%llu:%llu\n",checks,(unsigned long long)trace_hash,(unsigned long long)trace_count);return checks;
}

#ifndef TEST_LEGACY_ONLY
static struct idma_multi_experiment multi_request(unsigned nr,unsigned pages)
{
    struct idma_multi_experiment m={.abi_version=3,.struct_size=sizeof(m),
        .nr_devices=nr,.path_aggressor_index=1,.chunk_bytes=4096,.run_timeout_ms=60000,
        .victim_transfers=4096,.warmup_transfers=256};
    for(unsigned i=0;i<nr;i++){m.device_id[i]=i;m.pages[i]=i?pages:1;}
    return m;
}
static void complete_release(bool retained)
{
    assert(contexts==context_frees&&!lock_depth);
    if(retained)assert(allocs>frees);else assert(allocs==frees);
}
static unsigned multi_cases(void)
{
    unsigned checks=0;
    struct idma_multi_experiment m;
    for(unsigned magic=1;magic<=2;magic++)for(unsigned nr=2;nr<=4;nr++)for(unsigned c=0;c<3;c++){
        reset(magic==1?IDMA_BATCH_MAGIC:IDMA_P0_BATCH_MAGIC);
        unsigned pages=c==0?1:c==1?8:6/(nr-1),mask=(1U<<nr)-1;
        m=multi_request(nr,pages);
        assert(idma_run_multi(&idmas[0],&m)==0&&m.status==0&&m.measurement_complete);
        assert(m.prepared_mask==mask&&m.start_attempted_mask==mask&&m.stopped_mask==mask);
        assert(m.drained_mask==mask&&m.verified_mask==mask&&m.stop_status_valid_mask==(mask&~1U));
        assert(hpm_nr==nr&&m.observer_sequence==1);
        assert(m.stats[0].completed==4096&&m.stats[0].completed_bytes==16777216);
        assert(m.stats[0].in_flight_at_start==0&&m.stats[0].in_flight_at_end==0);
        for(unsigned i=1;i<nr;i++){
            assert(m.stats[i].submitted==m.stats[i].completed&&m.stats[i].completed_bytes==m.stats[i].completed*4096);
            assert(m.stats[i].in_flight_at_start==0&&m.stats[i].in_flight_at_end==1);
            assert(m.start_ns[i]<m.start_ns[0]&&(m.pre_stop_status[i]&2));
            assert(m.did[i]==10+i&&!m.stats[i].quarantined&&!m.stats[i].data_errors);
        }
        complete_release(false);checks++;
    }
    printf("PASS 18 multi matrix/magic cases, all masks, actual end in-flight states\n");
    for(unsigned bad=0;bad<14;bad++){
        reset(IDMA_BATCH_MAGIC);m=multi_request(2,8);
        switch(bad){case 0:m.abi_version=1;break;case 1:m.struct_size--;break;
        case 2:m.flags=1;break;case 3:m.reserved0=1;break;case 4:m.reserved[7]=1;break;
        case 5:m.nr_devices=1;break;case 6:m.nr_devices=5;break;case 7:m.device_id[1]=0;break;
        case 8:m.device_id[3]=3;break;case 9:m.pages[3]=1;break;case 10:m.pages[1]=2;break;
        case 11:m.victim_transfers=8;break;case 12:m.warmup_transfers=0;break;
        case 13:m.path_aggressor_index=2;break;}
        assert(idma_run_multi(&idmas[0],&m)==-EINVAL&&!m.measurement_started&&!allocs&&!contexts);checks++;
    }
    for(int stage=0;stage<6;stage++){
        reset(IDMA_BATCH_MAGIC);m=multi_request(4,8);
        switch(stage){case 0:context_failure=1;break;case 1:map_failure=2;break;
        case 2:hpm_failure=-EBUSY;break;case 3:observe_failure=-EOPNOTSUPP;break;
        case 4:arm_failure=-EIO;break;case 5:path_disabled=true;break;}
        assert(idma_run_multi(&idmas[0],&m)<0&&!m.measurement_started);
        assert(!m.start_attempted_mask);complete_release(false);checks++;
    }
    for(int lane=0;lane<4;lane++){
        reset(IDMA_BATCH_MAGIC);m=multi_request(4,8);batch_zero_id=lane;
        assert(idma_run_multi(&idmas[0],&m)==-EIO&&m.status==-EIO&&m.stats[lane].quarantined);
        assert(!(m.drained_mask&BIT(lane))&&!(m.verified_mask&BIT(lane)));
        if(lane>0)assert(!(m.start_attempted_mask&1));
        complete_release(true);checks++;
    }
    for(int lane=1;lane<4;lane++){
        reset(IDMA_BATCH_MAGIC);m=multi_request(4,8);drain_failure=lane;
        assert(idma_run_multi(&idmas[0],&m)==-ETIMEDOUT&&m.status==-ETIMEDOUT);
        assert(m.stats[lane].quarantined&&!(m.drained_mask&BIT(lane))&&!m.verified_mask);
        assert(m.stopped_mask==15);complete_release(true);checks++;
    }
    for(int lane=0;lane<4;lane++){
        reset(IDMA_BATCH_MAGIC);m=multi_request(4,8);verify_failure=lane;
        assert(idma_run_multi(&idmas[0],&m)==-EILSEQ&&m.stats[lane].data_errors);
        assert(m.verified_mask==(15U&~BIT(lane))&&m.drained_mask==15);
        complete_release(false);checks++;
    }
    for(int stage=0;stage<2;stage++){
        reset(IDMA_BATCH_MAGIC);m=multi_request(4,8);
        if(stage)exhausted=2;else premature=2;
        assert(idma_run_multi(&idmas[0],&m)==-EIO&&m.measurement_complete&&!m.verified_mask);
        assert(m.stop_status_valid_mask==14&&m.drained_mask==15&&m.stopped_mask==15);
        complete_release(false);checks++;
    }
    reset(IDMA_BATCH_MAGIC);m=multi_request(4,8);signal_on_batch=2;drain_failure=1;
    assert(idma_run_multi(&idmas[0],&m)==-EINTR&&m.status==-EINTR);
    assert(m.start_attempted_mask==(BIT(1)|BIT(2))&&!(m.start_attempted_mask&1));
    assert(m.stopped_mask==(BIT(1)|BIT(2))&&m.stats[1].quarantined);
    assert(!(m.drained_mask&BIT(1))&&m.stats[1].in_flight_at_end==1);
    complete_release(true);checks++;
    reset(IDMA_BATCH_MAGIC);m=multi_request(4,8);idma_devices[2]=NULL;
    assert(idma_run_multi(&idmas[0],&m)==-ENODEV&&!allocs);complete_release(false);checks++;
    reset(IDMA_BATCH_MAGIC);m=multi_request(2,8);idmas[3].quarantined=true;
    assert(idma_run_multi(&idmas[0],&m)==-EIO&&!allocs);complete_release(false);checks++;
    reset(IDMA_BATCH_MAGIC);struct idma_multi_caps caps;
    assert(idma_caps_multi(&idmas[0],&caps)==0&&caps.abi_version==3&&caps.capabilities==7&&caps.max_devices==4);
    assert(idma_caps_multi(&idmas[1],&caps)==-EOPNOTSUPP);checks++;
    reset(IDMA_BATCH_MAGIC);m=multi_request(4,8);from_failure=1;
    assert(idma_run_multi(&idmas[0],&m)==-EFAULT&&!allocs&&!contexts);checks++;
    reset(IDMA_BATCH_MAGIC);m=multi_request(4,8);to_failure=1;
    assert(idma_run_multi(&idmas[0],&m)==-EFAULT);complete_release(false);checks++;
    printf("PASS %u multi finite actual-function scenarios; stubs do not establish RTL/cache/HPM events\n",checks);
    reset(IDMA_BATCH_MAGIC);
    return checks;
}
#endif

#ifdef TEST_PAGES
_Static_assert(sizeof(struct idma_pages_caps)==64,"caps ABI4");
_Static_assert(sizeof(struct idma_pages_buffer)==128,"buffer ABI4");
_Static_assert(sizeof(struct idma_pages_experiment)==984,"request ABI4");
_Static_assert(offsetof(struct idma_pages_experiment,status)==80,"status ABI4");
_Static_assert(offsetof(struct idma_pages_experiment,experiment)==144,"experiment ABI4");
_Static_assert(offsetof(struct idma_pages_experiment,buffers)==472,"buffers ABI4");
static struct idma_pages_experiment pages_request(unsigned pg){return (struct idma_pages_experiment){
    .abi_version=4,.struct_size=sizeof(struct idma_pages_experiment),.mapping_page_bytes=pg,
    .buffer_bytes=SZ_2M,.chunk_bytes=SZ_4K,.run_timeout_ms=60000,.victim_transfers=4096,.warmup_transfers=256};}
static void pages_released(void){assert(contexts==context_frees&&!lock_depth&&allocs==frees);
    for(unsigned i=0;i<map_count;i++)assert(!maps[i].active);}
static unsigned pages_cases(void){unsigned checks=0;struct idma_pages_experiment p;
    for(unsigned obs=3;obs<=
#ifdef IDMA_OBS_MAGIC_V4
        4
#else
        3
#endif
        ;obs++)for(unsigned magic=1;magic<=2;magic++)for(unsigned pg=SZ_4K;pg<=SZ_2M;pg*=512){
        reset(magic==1?IDMA_BATCH_MAGIC:IDMA_P0_BATCH_MAGIC);
#ifdef IDMA_OBS_MAGIC_V4
        if(obs==4)hw[0].regs[IDMA_OBS_MAGIC_OFFSET/8]=IDMA_OBS_MAGIC_V4;
#endif
        p=pages_request(pg);
        assert(idma_run_pages(&idmas[0],&p)==0&&p.status==0);
        assert(p.prepared_lane_mask==3&&p.verified_before_mask==15&&p.verified_after_mask==15);
        assert(p.quiet_lane_mask==3&&p.unmapped_buffer_mask==15&&p.freed_buffer_mask==15&&!p.retained_lane_mask);
        assert(p.experiment.measurement_started&&p.experiment.measurement_complete&&p.observer_sequence==1);
        assert(p.A_status_valid&&(p.pre_stop_A_control&2)&&p.pre_stop_A_issued<IDMA_EXPERIMENT_MAX_TRANSFERS);
        assert(p.experiment.victim.completed==4096&&p.experiment.victim.completed_bytes==16777216);
        assert(p.experiment.aggressor.completed_bytes==p.experiment.aggressor.completed*4096&&p.experiment.aggressor.completed);
        assert(!p.experiment.victim.in_flight_at_end&&p.experiment.aggressor.in_flight_at_end);
        for(unsigned i=0;i<4;i++){struct idma_pages_buffer *b=&p.buffers[i];
            assert(b->bytes==SZ_2M&&b->mapped_bytes==SZ_2M&&b->unmapped_bytes==SZ_2M&&b->requested_page_bytes==pg);
            assert(!(b->phys%(SZ_2M))&&!(b->iova%(SZ_2M))&&b->verified_before_4k_pages==512&&b->verified_after_4k_pages==512);
            assert(b->leaf_shift_min==(pg==SZ_2M?21:12)&&b->leaf_shift_max==b->leaf_shift_min);
            assert((b->before_first_pte&7)==7&&(b->after_last_pte&7)==7&&b->empty_after_cleanup);
            assert(!b->before_error&&!b->after_error&&!b->unmap_error);
        }
        assert(allocs==4&&alloc_attempts==4&&query_calls==6156);pages_released();checks++;
    }
    for(unsigned bad=0;bad<15;bad++){
        reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_4K);
        switch(bad){case 0:p.abi_version=3;break;case 1:p.struct_size--;break;case 2:p.flags=1;break;
        case 3:p.reserved0=1;break;case 4:p.reserved[7]=1;break;case 5:p.mapping_page_bytes=8192;break;
        case 6:p.buffer_bytes=4096;break;case 7:p.chunk_bytes=64;break;case 8:p.victim_transfers=512;break;
        case 9:p.warmup_transfers=0;break;case 10:p.run_timeout_ms=0;break;case 11:p.run_timeout_ms=300001;break;
        case 12:idmas[0].did=99;break;case 13:idmas[0].id=1;break;case 14:idmas[1].did=12;break;}
        assert(idma_run_pages(&idmas[0],&p)<0&&!allocs&&!p.experiment.measurement_started);
        pages_released();checks++;
    }
    for(int fault=0;fault<4;fault++){
        reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);alloc_failure=fault;
        assert(idma_run_pages(&idmas[0],&p)==-ENOMEM&&!p.experiment.measurement_started);
        pages_released();checks++;
    }
    for(int kind=0;kind<2;kind++)for(int buffer=0;buffer<4;buffer++){
        reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);
        if(kind)pa_noncontiguous=buffer;else pa_unaligned=buffer;
        assert(idma_run_pages(&idmas[0],&p)==(kind?-EFAULT:-EINVAL)&&!p.experiment.measurement_started);
        pages_released();checks++;
    }
    const int failures4k[]={0,1,511,512,513,1023,1024,1535,2047};
    for(unsigned n=0;n<ARRAY_SIZE(failures4k)+4;n++){
        reset(IDMA_BATCH_MAGIC);p=pages_request(n<ARRAY_SIZE(failures4k)?SZ_4K:SZ_2M);
        map_call_failure=n<ARRAY_SIZE(failures4k)?failures4k[n]:(int)(n-ARRAY_SIZE(failures4k));
        assert(idma_run_pages(&idmas[0],&p)==-ENOMEM&&!p.experiment.measurement_started);
        assert(!hw[0].next_id||hw[0].next_id==10);pages_released();checks++;
    }
    reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_4K);map_call_failure=1;failed_map_residual=true;
    assert(idma_run_pages(&idmas[0],&p)==-ENOMEM&&p.retained_lane_mask==1&&idmas[0].quarantined);
    assert(!p.freed_buffer_mask&&p.buffers[0].unmap_error==-EIO&&!p.buffers[0].empty_after_cleanup);
    complete_release(true);checks++;
    for(int kind=1;kind<=5;kind++){
        reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);query_fault_kind=kind;query_fault_at=kind==1?0:2;
        assert(idma_run_pages(&idmas[0],&p)<0&&!p.experiment.measurement_started);
        assert(!map_calls||kind!=1);pages_released();checks++;
    }
    for(int kind=1;kind<=5;kind++){
        reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);query_fault_kind=kind;query_fault_at=2056;
        assert(idma_run_pages(&idmas[0],&p)<0&&p.experiment.measurement_complete);
        assert(p.buffers[0].after_error<0&&p.verified_after_mask==14);pages_released();checks++;
    }
    for(int stage=0;stage<6;stage++){
        reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);
        switch(stage){case 0:context_failure=1;break;case 1:hpm_failure=-EBUSY;break;
        case 2:observe_failure=-EOPNOTSUPP;break;case 3:arm_failure=-EIO;break;
        case 4:path_disabled=true;break;case 5:find_failure=-ENOSPC;break;}
        assert(idma_run_pages(&idmas[0],&p)<0&&!p.experiment.measurement_started);pages_released();checks++;
    }
    for(int kind=0;kind<2;kind++)for(int lane=0;lane<2;lane++){
        reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);if(kind)batch_zero_id=lane;else zero_id=lane;
        assert(idma_run_pages(&idmas[0],&p)==-EIO&&p.retained_lane_mask==BIT(lane));
        assert(!p.verified_after_mask&&!(p.freed_buffer_mask&(3U<<(2*lane))));
        assert(idmas[lane].quarantined);complete_release(true);checks++;
    }
    reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);drain_failure=1;
    assert(idma_run_pages(&idmas[0],&p)==-ETIMEDOUT&&p.retained_lane_mask==2&&!p.verified_after_mask);
    assert(p.freed_buffer_mask==3&&stop_writes>=2);complete_release(true);checks++;
    for(int lane=0;lane<2;lane++){
        reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);verify_failure=lane;
        assert(idma_run_pages(&idmas[0],&p)==-EILSEQ&&p.verified_after_mask==15);
        assert(lane?p.experiment.aggressor.data_errors:p.experiment.victim.data_errors);pages_released();checks++;
    }
    for(int kind=0;kind<2;kind++){
        reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);if(kind)exhausted=1;else premature=1;
        assert(idma_run_pages(&idmas[0],&p)==-EIO&&p.A_status_valid&&p.verified_after_mask==15);
        pages_released();checks++;
    }
    for(int buffer=0;buffer<4;buffer++)for(int clears=0;clears<2;clears++){
        reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);
        /* A is cleaned before V; each buffer is one 2MiB unmap. */
        short_unmap=(buffer+2)%4;short_clears=clears;
        assert(idma_run_pages(&idmas[0],&p)==-EIO&&p.retained_lane_mask==BIT(buffer/2));
        assert(p.buffers[buffer].unmap_error==-EIO&&p.buffers[buffer].empty_after_cleanup==(unsigned)clears);
        assert(!(p.unmapped_buffer_mask&BIT(buffer))&&!(p.freed_buffer_mask&(3U<<(2*(buffer/2)))));
        assert(unmap_calls==4&&p.freed_buffer_mask==(15U&~(3U<<(2*(buffer/2)))));
        complete_release(true);checks++;
    }
    for(int buffer=0;buffer<4;buffer++){
        reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_4K);short_unmap=((buffer+2)%4)*512+511;
        assert(idma_run_pages(&idmas[0],&p)==-EIO&&p.retained_lane_mask==BIT(buffer/2));
        assert(unmap_calls==2048&&p.buffers[buffer].unmap_error==-EIO);
        assert(p.buffers[buffer].unmapped_bytes==SZ_2M-2048&&!p.buffers[buffer].empty_after_cleanup);
        complete_release(true);checks++;
    }
    reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);query_fault_at=2056+512;query_fault_kind=3;
    assert(idma_run_pages(&idmas[0],&p)==-EIO&&p.buffers[0].verified_after_4k_pages==512);
    assert(p.buffers[0].after_error==-EIO&&p.verified_after_mask==14);pages_released();checks++;
    reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);verify_failure=0;query_fault_at=4108;query_fault_kind=1;
    assert(idma_run_pages(&idmas[0],&p)==-EILSEQ&&p.buffers[2].unmap_error==-EOPNOTSUPP);
    assert(p.retained_lane_mask==2);complete_release(true);checks++;
    reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);signal_on_batch=1;short_unmap=0;
    assert(idma_run_pages(&idmas[0],&p)==-EINTR&&p.status==-EINTR&&p.retained_lane_mask==2);
    complete_release(true);checks++;
    reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);query_fault_at=4108;query_fault_kind=1;
    assert(idma_run_pages(&idmas[0],&p)==-EOPNOTSUPP&&p.retained_lane_mask==2);
    assert(p.buffers[2].unmap_error==-EOPNOTSUPP&&!p.buffers[2].empty_after_cleanup);
    complete_release(true);checks++;
    reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);from_failure=1;
    assert(idma_run_pages(&idmas[0],&p)==-EFAULT&&!allocs);pages_released();checks++;
    reset(IDMA_BATCH_MAGIC);p=pages_request(SZ_2M);to_failure=1;
    assert(idma_run_pages(&idmas[0],&p)==-EFAULT);pages_released();checks++;
    reset(IDMA_BATCH_MAGIC);struct idma_pages_caps caps;
    assert(idma_caps_pages(&idmas[0],&caps)==0&&caps.abi_version==4&&caps.capabilities==15&&caps.mapping_page_mask==3);
    assert(idma_caps_pages(&idmas[1],&caps)==-EOPNOTSUPP);checks++;
#ifdef IDMA_OBS_MAGIC_V4
    for(unsigned version=1;version<=4;version++){
        reset(IDMA_BATCH_MAGIC);hw[0].regs[IDMA_OBS_MAGIC_OFFSET/8]=IDMA_OBS_MAGIC+version-1;
        assert(idma_caps_pages(&idmas[0],&caps)==0);
        assert(!!(caps.capabilities&IDMA_PAGES_CAP_PATH)==(version>=3));
        if(version<3){p=pages_request(SZ_2M);assert(idma_run_pages(&idmas[0],&p)==-EOPNOTSUPP&&!allocs);}
        pages_released();checks++;
    }
#endif
    printf("PASS %u E4 finite lifecycle/ABI/mapping/proof/retention actual-function scenarios; no hardware claim\n",checks);
    reset(IDMA_BATCH_MAGIC);return checks;
}
#endif


#ifdef TEST_WORKLOAD
static struct idma_workload_geometry geometry(unsigned shape,unsigned size)
{
    struct idma_workload_geometry g={.geometry=shape,.row_bytes=size,.rows_per_operation=1,.ring_pages=8};
    if(shape==0)g.ring_pages=size>4096?size/4096:1;
    if(shape==1&&size>4096){g.geometry=0;g.ring_pages=size>32768?size/4096:8;}
    if(shape==2){g.ring_pages=256;g.row_pitch_bytes=16384;g.matrix_rows=64;
        g.row_bytes=size>1024?1024:size;g.rows_per_operation=size/g.row_bytes;g.columns=16384/g.row_bytes;}
    return g;
}
static u64 expected_ring(const struct idma_workload_geometry *g)
{
    if(g->geometry==2)return (u64)g->matrix_rows*g->columns;
    if(g->geometry==1)return g->ring_pages*(4096/g->row_bytes);
    return (u64)g->ring_pages*4096/g->row_bytes;
}
static u64 expected_offset(const struct idma_workload_geometry *g,u64 n)
{
    struct model h={};h.regs[0xb0/8]=g->geometry;h.regs[0xb8/8]=g->rows_per_operation;
    h.regs[0xc0/8]=g->row_pitch_bytes;h.regs[0xc8/8]=g->matrix_rows;h.regs[0xd0/8]=g->columns;
    return model_offset(&h,n,g->row_bytes,(u64)g->ring_pages*4096);
}
static void expected_coverage(const struct idma_workload_geometry *g,u64 n,u64 w,
                             const struct idma_workload_coverage *actual,bool verified)
{
    /* Independent byte visitation, rather than the driver's 64B bitmap helper. */
    unsigned char *bits=calloc((size_t)g->ring_pages*4096,1);assert(bits);
    u64 ring=expected_ring(g),measured=0,united=0;unsigned mp=0,wp=0;
    for(u64 i=0;i<(n<ring?n:ring);i++){
        u64 off=expected_offset(g,i);for(unsigned j=0;j<g->row_bytes;j++)bits[off+j]|=1;
    }
    for(u64 i=0;i<(w<ring?w:ring);i++){
        u64 off=expected_offset(g,i);for(unsigned j=0;j<g->row_bytes;j++)bits[off+j]|=2;
    }
    for(unsigned p=0;p<g->ring_pages;p++){bool m=false,z=false;
        for(unsigned j=0;j<4096;j++){unsigned b=bits[p*4096+j];measured+=!!(b&1);united+=!!b;m|=b&1;z|=b&2;}
        mp+=m;wp+=z;
    }
    assert(actual->measured_distinct_bytes==measured&&actual->measured_distinct_pages==mp);
    assert(actual->warmup_distinct_pages==wp&&actual->verified_union_bytes==(verified?united:0));free(bits);
}
static void workload_reset(void){reset(IDMA_WORKLOAD_BATCH_MAGIC);hw[0].regs[IDMA_OBS_MAGIC_OFFSET/8]=IDMA_OBS_MAGIC_V5;}
static struct idma_workload_experiment workload_request(unsigned vs,unsigned vl,unsigned as,unsigned al,bool solo)
{
    struct idma_workload_experiment p={.abi_version=5,.struct_size=sizeof(p),.run_timeout_ms=60000,
        .supply=1,.aggressor_id=solo?IDMA_EXPERIMENT_NO_AGGRESSOR:1,.victim_geometry=geometry(vs,vl)};
    if(!solo)p.aggressor_geometry=geometry(as,al);
    p.victim_operations=expected_ring(&p.victim_geometry)/p.victim_geometry.rows_per_operation+1;
    p.victim_warmup_operations=expected_ring(&p.victim_geometry)/p.victim_geometry.rows_per_operation;
    return p;
}
static void workload_success(struct idma_workload_experiment *p)
{
    int ret=idma_run_workload(&idmas[0],p);
    if(ret)fprintf(stderr,"workload failed Vg%u/%u/%u Ag%u/%u/%u ret=%d\n",p->victim_geometry.geometry,
        p->victim_geometry.row_bytes,p->victim_geometry.rows_per_operation,p->aggressor_geometry.geometry,
        p->aggressor_geometry.row_bytes,p->aggressor_geometry.rows_per_operation,ret);
    assert(ret==0&&p->status==0&&p->measurement_complete&&p->hpm_configured&&p->observer_sequence==1);
    assert(p->victim.completed==p->victim_operations*p->victim_geometry.rows_per_operation);
    assert(p->victim.completed_bytes==p->victim.completed*p->victim_geometry.row_bytes);
    assert(p->victim_warmup_descriptors==p->victim_warmup_operations*p->victim_geometry.rows_per_operation);
    assert(p->victim_warmup_bytes==p->victim_warmup_descriptors*p->victim_geometry.row_bytes);
    assert(hw[0].last_len==p->victim_geometry.row_bytes&&!p->victim.data_errors);
    expected_coverage(&p->victim_geometry,p->victim.completed,p->victim_warmup_descriptors,&p->victim_coverage,true);
    if(p->aggressor_id!=IDMA_EXPERIMENT_NO_AGGRESSOR){
        assert(p->aggressor.completed==32&&p->aggressor.submitted==32&&stop_writes>=1);
        assert(p->aggressor.completed_bytes==32ULL*p->aggressor_geometry.row_bytes);
        assert(hw[1].last_len==p->aggressor_geometry.row_bytes&&!p->aggressor.data_errors);
        assert(p->aggressor_warmup_bytes==p->aggressor_warmup_descriptors*p->aggressor_geometry.row_bytes);
        expected_coverage(&p->aggressor_geometry,32,p->aggressor_warmup_descriptors,&p->aggressor_coverage,true);
    }else{struct idma_workload_coverage zero={};assert(!memcmp(&p->aggressor_coverage,&zero,sizeof(zero)));}
    complete_release(false);
}
static unsigned workload_cases(void)
{
    unsigned checks=0;const unsigned sizes[]={64,256,1024,4096,16384,65536};
    struct idma_workload_experiment p;
    for(unsigned vs=0;vs<3;vs++)for(unsigned vl=0;vl<6;vl++){
        workload_reset();p=workload_request(vs,sizes[vl],0,4096,true);workload_success(&p);checks++;
        for(unsigned as=0;as<3;as++)for(unsigned al=0;al<6;al++){
            workload_reset();p=workload_request(vs,sizes[vl],as,sizes[al],false);workload_success(&p);checks++;
        }
    }
    for(unsigned a=0;a<2;a++){
        workload_reset();p=workload_request(1,1536,1,1536,a==0);workload_success(&p);checks++;
    }
    /* A stops after 32 actual rows of a 64-row tile: never round up to a tile. */
    workload_reset();p=workload_request(0,64,2,65536,false);p.victim_warmup_operations=0;
    workload_success(&p);assert(p.aggressor_coverage.measured_distinct_bytes==32768&&
        p.aggressor_coverage.measured_distinct_pages==32&&p.aggressor_warmup_descriptors==0);checks++;
    /* Warmup may cover a larger, noncontiguous prefix than measurement. */
    for(unsigned shape=1;shape<=2;shape++){
        workload_reset();p=workload_request(shape,shape==1?1536:65536,0,4096,true);
        p.victim_operations=1;p.victim_warmup_operations=4;workload_success(&p);checks++;
    }
    for(unsigned bad=0;bad<27;bad++){
        workload_reset();p=workload_request(0,64,0,4096,false);
        switch(bad){case 0:p.abi_version--;break;case 1:p.struct_size--;break;case 2:p.flags=1;break;
        case 3:p.reserved0=1;break;case 4:p.reserved1=1;break;case 5:p.reserved[1]=1;break;
        case 6:p.supply=0;break;case 7:p.aggressor_id=2;break;case 8:p.run_timeout_ms=0;break;
        case 9:p.run_timeout_ms=60001;break;case 10:p.victim_geometry.row_bytes=0;break;
        case 11:p.aggressor_geometry.row_bytes=1536;break;case 12:p.victim_geometry.ring_pages=3;break;
        case 13:p.victim_geometry.reserved[3]=1;break;case 14:p.aggressor_geometry.reserved0=1;break;
        case 15:p.victim_operations=0;break;case 16:p.victim_operations=262145;break;
        case 17:p.victim_warmup_operations=16385;break;case 18:p.victim_geometry=geometry(2,65536);p.victim_operations=~0ULL;break;
        case 19:p.victim_geometry=geometry(2,65536);p.victim_warmup_operations=~0ULL;break;
        case 20:p.aggressor_id=IDMA_EXPERIMENT_NO_AGGRESSOR;break;case 21:p.victim_geometry.geometry=3;break;
        case 22:idmas[0].id=1;break;case 23:idmas[0].did=12;break;case 24:idmas[1].did=12;break;
        case 25:p.victim_geometry=geometry(2,16384);p.victim_geometry.row_pitch_bytes=8192;break;
        case 26:p.aggressor_geometry=geometry(1,1536);p.aggressor_geometry.row_bytes=2048;break;}
        assert(idma_run_workload(&idmas[0],&p)<0&&!p.measurement_started&&!allocs);complete_release(false);checks++;
    }
    for(unsigned bad=0;bad<12;bad++){
        workload_reset();p=workload_request(0,64,0,4096,false);
        switch(bad){case 0:hw[0].regs[0x80/8]=IDMA_BATCH_MAGIC;break;case 1:hw[1].regs[0x80/8]=IDMA_P0_BATCH_MAGIC;break;
        case 2:hw[0].regs[IDMA_OBS_MAGIC_OFFSET/8]=IDMA_OBS_MAGIC_V4;break;case 3:path_disabled=true;break;
        case 4:hist_info^=1;break;case 5:hist_info^=1ULL<<48;break;case 6:hist_info|=1ULL<<56;break;
        case 7:hist_info|=1ULL<<32;break;case 8:observe_failure=-EIO;break;
        case 9:hw[1].regs[0x88/8]=3;break;case 10:idma_devices[1]=NULL;break;case 11:idmas[3].quarantined=true;break;}
        assert(idma_run_workload(&idmas[0],&p)<0&&!allocs&&!p.measurement_started);complete_release(false);checks++;
    }
    for(unsigned fault=0;fault<12;fault++){
        workload_reset();p=workload_request(1,1536,2,65536,false);
        switch(fault){case 0:alloc_failure=1;break;case 1:map_failure=1;break;case 2:hpm_failure=-EBUSY;break;
        case 3:arm_failure=-EIO;break;case 4:zero_id=0;break;case 5:batch_zero_id=1;break;
        case 6:drain_failure=1;break;case 7:verify_failure=0;break;case 8:verify_failure=1;break;
        case 9:premature=1;break;case 10:exhausted=1;break;case 11:signal_on_batch=1;break;}
        assert(idma_run_workload(&idmas[0],&p)<0);complete_release(fault>=4&&fault<=6);
        if(fault==6)assert(p.aggressor.quarantined&&!p.aggressor_coverage.verified_union_bytes);
        if(fault==7)assert(p.victim.data_errors&&!p.victim_coverage.verified_union_bytes);
        if(fault==8)assert(p.aggressor.data_errors&&!p.aggressor_coverage.verified_union_bytes);
        checks++;
    }
    /* Data verification must include padding, tile gaps and untouched source. */
    for(unsigned fault=0;fault<4;fault++){
        workload_reset();p=workload_request(1,1536,2,65536,false);
        p.victim_operations=1;p.victim_warmup_operations=0;
        verify_failure=fault&1;verify_source=fault>=2;
        verify_offset=(fault&1)?1024:3072;
        assert(idma_run_workload(&idmas[0],&p)==-EILSEQ);
        assert((fault&1)?p.aggressor.data_errors:p.victim.data_errors);
        assert((fault&1)?!p.aggressor_coverage.verified_union_bytes:!p.victim_coverage.verified_union_bytes);
        complete_release(false);checks++;
    }
    /* DONE alone does not permit freeing a lane whose batch control is active. */
    workload_reset();p=workload_request(0,65536,2,65536,false);stop_unquiet=1;
    assert(idma_run_workload(&idmas[0],&p)==-EIO&&p.aggressor.quarantined);
    assert(!p.aggressor_coverage.verified_union_bytes);complete_release(true);checks++;
    workload_reset();struct idma_workload_caps caps;assert(idma_caps_workload(&idmas[0],&caps)==0&&
        caps.abi_version==5&&caps.struct_size==64&&caps.batch_magic==IDMA_WORKLOAD_BATCH_MAGIC&&
        caps.observer_magic==IDMA_OBS_MAGIC_V5&&caps.path_queue_depth==64&&caps.row_length_mask==127);checks++;
    workload_reset();hw[0].regs[0x80/8]=IDMA_P0_BATCH_MAGIC;
    assert(idma_caps_workload(&idmas[0],&caps)==-EOPNOTSUPP);checks++;
    for(unsigned fault=0;fault<3;fault++){
        workload_reset();p=workload_request(0,64,0,4096,false);
        if(fault==0)context_failure=1;else if(fault==1)from_failure=1;else to_failure=1;
        assert(idma_run_workload(&idmas[0],&p)<0);complete_release(false);checks++;
    }
    /* BTC3 retains all old production entry points, independently of ABI5. */
    for(unsigned len=64;len<=4096;len=len==64?1024:len==1024?4096:8192){
        workload_reset();struct idma_p0_experiment old=request(len,8);
        assert(idma_run_p0(&idmas[0],&old)==0&&old.victim.completed_bytes==4096ULL*len);
        complete_release(false);checks++;
    }
    for(unsigned batch=0;batch<2;batch++){
        workload_reset();struct idma_experiment old={.abi_version=1,.struct_size=sizeof(old),.aggressor_id=1,
            .victim_pages=1,.aggressor_pages=8,.chunk_bytes=4096,.run_timeout_ms=60000,
            .victim_transfers=4096,.warmup_transfers=256};
        assert(idma_run_experiment_mode(&idmas[0],&old,batch)==0&&old.victim.completed_bytes==16777216);
        complete_release(false);checks++;
    }
    workload_reset();struct idma_multi_experiment multi=multi_request(4,8);
    assert(idma_run_multi(&idmas[0],&multi)==0&&multi.verified_mask==15);complete_release(false);checks++;
    for(unsigned pg=SZ_4K;pg<=SZ_2M;pg*=512){
        workload_reset();struct idma_pages_experiment old=pages_request(pg);
        assert(idma_run_pages(&idmas[0],&old)==0&&old.verified_after_mask==15);pages_released();checks++;
    }
    printf("PASS %u ABI5 actual lifecycle scenarios; independent byte coverage and partial tile rows verified\n",checks);
    reset(IDMA_BATCH_MAGIC);return checks;
}
#endif

int main(void)
{
    setvbuf(stdout,NULL,_IOLBF,0);
    legacy_cases();
#ifndef TEST_LEGACY_ONLY
    multi_cases();
#ifdef TEST_PAGES
    pages_cases();
    printf("ALL_OLD_TRACE=%llu:%llu\n",(unsigned long long)trace_hash,(unsigned long long)trace_count);
#ifdef TEST_WORKLOAD
    workload_cases();
#endif
#endif
#endif
    return 0;
}

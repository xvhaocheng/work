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
/* TYPES */
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
static bool signal_flag;
static int map_failure=-1,batch_zero_id=-1,signal_on_batch=-1,premature=-1,exhausted=-1;
static int hpm_failure,observe_failure,arm_failure,context_failure,from_failure,to_failure;
static bool path_disabled;
static unsigned contexts,context_frees,hpm_nr;
static int alloc_failure=-1,pa_unaligned=-1,pa_noncontiguous=-1,map_call_failure=-1;
static int short_unmap=-1,query_fault_at=-1,query_fault_kind,find_failure;
static bool failed_map_residual,short_clears;
static unsigned map_calls,unmap_calls,query_calls,alloc_attempts;
static size_t allocation_sizes[256];

static u64 trace_hash=1469598103934665603ULL,trace_count;
static void trace(unsigned lane,unsigned off,u64 value,unsigned write){
    assert(trace_count<2000000);
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
    ((u8*)resolve(hw[verify_failure].regs[IDMA_DST_ADDR_REG_OFFSET/8],1))[0]^=1;}
static void copy_tasks(int lane,u64 count){struct model *h=&hw[lane];u64 len=h->regs[IDMA_NUM_BYTES_REG_OFFSET/8];
    assert(count<=4096);
    u64 span=h->regs[IDMA_BATCH_PAGES_OFFSET/8]*4096,off=0;
    for(u64 i=0;i<count;i++){memcpy(resolve(h->regs[1]+off,len),resolve(h->regs[0]+off,len),len);
        if(h->regs[IDMA_BATCH_CONTROL_OFFSET/8]&1)off=(off+len)%span;}
    h->completed=count;h->last_len=len;
}
static struct model *model_at(void *p,int *lane,unsigned *off){for(int i=0;i<4;i++)
    if((u8*)p>=(u8*)hw[i].regs&&(u8*)p<(u8*)hw[i].regs+sizeof(hw[i].regs)){
        *lane=i;*off=(unsigned)((u8*)p-(u8*)hw[i].regs);return &hw[i];}assert(0);return NULL;}
static void writeq(u64 value,void *p){int lane;unsigned off;struct model *h=model_at(p,&lane,&off);
    trace(lane,off,value,1);
    if(off==IDMA_BATCH_CONTROL_OFFSET){
        if(value==0){h->regs[off/8]=0;h->regs[0xa0/8]=h->regs[0xa8/8]=0;}
        else if(value==1)h->regs[off/8]=1;
        else if(value==2){stop_writes++;if(h->pending&&drain_failure!=lane){
            h->regs[0x30/8]=h->next_id;h->pending=false;h->regs[off/8]=5;
            h->regs[0xa0/8]=h->regs[0xa8/8]=h->completed;}}
    }else h->regs[off/8]=value;
}
static u64 readq(void *p){int lane;unsigned off;struct model *h=model_at(p,&lane,&off);
    trace(lane,off,h->regs[off/8],0);
    if(off==IDMA_BATCH_CONTROL_OFFSET&&h->pending&&premature==lane)return 1;
    if(off==IDMA_BATCH_ISSUED_OFFSET&&h->pending&&exhausted==lane)return IDMA_EXPERIMENT_MAX_TRANSFERS;
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
    o->sequence=1;return observe_failure;}
static int idma_observation_arm(struct idma_observation *o,struct idma_device *v,struct idma_device *a,u64 n){(void)n;o->armed=true;assert(v->id==0&&(!a||a->id==1));return arm_failure;}
static int idma_observation_finish(struct idma_observation *o,const struct idma_experiment *e,int error){(void)o;(void)e;(void)error;return 0;}
static int riscv_iommu_hpm_acquire_multi(struct device *const d[],unsigned nr,struct riscv_iommu_hpm_session **p){
    hpm_nr=nr;for(unsigned i=0;i<nr;i++)assert(d[i]->index==(int)i);
    static struct riscv_iommu_hpm_session s;*p=hpm_failure?NULL:&s;return hpm_failure;
}
static void riscv_iommu_hpm_read_multi(struct riscv_iommu_hpm_session *p,struct riscv_iommu_hpm_multi_sample *s){assert(p);memset(s,0,sizeof(*s));}
/* FUNCTIONS */
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
    path_disabled=false;contexts=context_frees=hpm_nr=0;

    memset(hw,0,sizeof(hw));memset(idmas,0,sizeof(idmas));memset(idma_devices,0,sizeof(idma_devices));
    memset(maps,0,sizeof(maps));map_count=allocs=frees=stop_writes=flushes=log_errors=lock_depth=observer_calls=0;
    zero_id=drain_failure=verify_failure=-1;clock_ns=0;signal_flag=false;
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


int main(void)
{
    legacy_cases();
#ifndef TEST_LEGACY_ONLY
    multi_cases();
#ifdef TEST_PAGES
    pages_cases();
#endif
#endif
    return 0;
}

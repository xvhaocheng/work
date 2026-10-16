/* Host boundary harness: includes the REAL driver FSM, NOT hardware evidence. */
#define _GNU_SOURCE
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uapi/linux/idma_sva.h>

typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned long phys_addr_t;
typedef int spinlock_t;
#define __user
#define __iomem
#define __noreturn __attribute__((noreturn))
#define module_param_named(...)
#define MODULE_PARM_DESC(...)
#define WRITE_ONCE(value, next) ((value) = (next))
#define READ_ONCE(value) (value)
#define PAGE_SIZE 4096UL
#define PAGE_MASK (~(PAGE_SIZE - 1UL))
#define HZ 100
#define U32_MAX UINT32_MAX
#define GFP_KERNEL 0
#define FOLL_WRITE 1
#define FOLL_LONGTERM 2
#define VM_SHARED 1
#define VM_SPECIAL 2
#define VM_HUGETLB 4
#define VM_READ 8
#define VM_WRITE 16
#define SATP_MODE_39 8
#define TASK_SIZE (1UL << 47)
#define CAP_SYS_RAWIO 1
#define IOMMU_READ 1
#define IOMMU_WRITE 2
#define NSEC_PER_MSEC 1000000ULL
#define _PAGE_READ 2
#define _PAGE_USER 16
#define _PAGE_ACCESSED 64
#define _PAGE_DIRTY 128
#define ERESTARTSYS 512
#define IS_ALIGNED(value, align) (!((value) & ((align) - 1)))
#define check_add_overflow(a, b, result) __builtin_add_overflow(a, b, result)
#define ERR_PTR(error) ((void *)(intptr_t)(error))
#define IS_ERR(value) ((uintptr_t)(value) > (uintptr_t)-4096)
#define PTR_ERR(value) ((int)(intptr_t)(value))
#define offset_in_page(value) ((value) & (PAGE_SIZE - 1))
#define IDMA_SRC_ADDR_REG_OFFSET 0
#define IDMA_DST_ADDR_REG_OFFSET 8
#define IDMA_NUM_BYTES_REG_OFFSET 16
#define IDMA_CONF_REG_OFFSET 24
#define IDMA_STATUS_REG_OFFSET 32
#define IDMA_NEXT_ID_REG_OFFSET 40
#define IDMA_DONE_REG_OFFSET 48
#define IDMA_BATCH_CONTROL_OFFSET 0x88
#define IDMA_LAUNCHER_MAGIC_OFFSET 0xb0
#define IDMA_LAUNCHER_CONTROL_OFFSET 0xb8
#define IDMA_LAUNCHER_MAGIC 0x49444d414c4e4301ULL
#define IDMA_LAUNCHER_STATUS_PREP 1ULL
#define IDMA_LAUNCHER_STATUS_RUNNING 2ULL
#define IDMA_LAUNCHER_HELD_MASK (15ULL << 8)
#define IDMA_LAUNCHER_ACTIVE_MASK (15ULL << 12)
#define IDMA_LAUNCHER_ERROR_MASK (0xffffffffULL << 32)
#define IDMA_MAX_RUN_TIMEOUT_MS 300000U

struct mm_struct { int mm_users; };
static int atomic_read(int *value) { return *value; }
struct task_struct { struct mm_struct *mm; };
struct iommu_domain { int kind; };
struct device { int index; struct iommu_domain *domain; };
struct idma_device { unsigned int id, did; bool quarantined; void *reg; struct { struct device *parent; } miscdev; };
struct page { unsigned int index; unsigned long pfn; };
struct vm_area_struct { unsigned long vm_start, vm_end, vm_flags; };
struct riscv_iommu_research_certificate { u64 epoch; };
typedef struct { u64 value; } pgd_t;
typedef pgd_t p4d_t;
typedef pgd_t pud_t;
typedef pgd_t pmd_t;
typedef pgd_t pte_t;

static struct {
 const char *scenario;
 unsigned char *arena;
 struct page pages[8];
 pte_t ptes[8];
 pgd_t table;
 struct mm_struct mm;
 struct task_struct task;
 struct vm_area_struct vma;
 struct device devices[4];
 struct idma_device lanes[4];
 struct iommu_domain defaults[4], blocking, task_domain;
 u64 regs[4][64], next_id[4], last_done[4];
 int devrefs[4], mmrefs, pins, mapped, claimed[2], attached[2];
 int cpus_lock, mmap_lock, mutex_lock, domain_alive, domain_mmref, session_alive;
 int pin_calls, map_calls, claim_calls, attach_calls, barriers, unpins, releases;
 int next_calls, done_calls, active_lane, active_reads, completions, cache_calls;
 int returned, retained, next_after_timeout;
 u64 clock_ns;
 char events[256][96];
 unsigned int event_count;
 jmp_buf retained_jump;
} H;
static struct task_struct *current = &H.task;
static int idma_experiment_lock;
static int satp_mode = SATP_MODE_39;

static bool is_case(const char *name) { return !strcmp(H.scenario, name); }
static void event(const char *format, ...) {
 va_list ap; assert(H.event_count < 256); va_start(ap, format);
 vsnprintf(H.events[H.event_count++], 96, format, ap); va_end(ap);
}
static void *memchr_inv(const void *memory, int c, size_t n) {
 const unsigned char *p = memory; for (size_t i = 0; i < n; i++) if (p[i] != c) return (void *)(p+i); return NULL;
}
static int copy_from_user(void *to, const void *from, size_t size) { memcpy(to, from, size); return 0; }
static int copy_to_user(void *to, const void *from, size_t size) {
 assert(H.active_lane == -1 && !H.pins && !H.mapped && !H.domain_alive && !H.mmap_lock && !H.mutex_lock);
 event("RETURN_COPY"); memcpy(to, from, size); H.returned = 1; return 0;
}
static bool capable(int capability) { (void)capability; return true; }
static int get_nr_threads(struct task_struct *task) { (void)task; return 1; }
static int num_online_cpus(void) { return 1; }
static bool of_machine_is_compatible(const char *machine) { (void)machine; return true; }
static bool riscv_iommu_research_supported(struct device *a, struct device *b) { return a && b; }
static void *kzalloc(size_t size, int flags) { (void)flags; H.session_alive++; return calloc(1, size); }
static void kfree(void *ptr) { assert(H.session_alive == 1); H.session_alive--; free(ptr); }
static struct idma_device *idma_get_instance(unsigned int index) {
 if (index >= 4) return NULL; H.devrefs[index]++; return &H.lanes[index];
}
static void idma_put_instance(struct idma_device *d) { if (d) { assert(H.devrefs[d->id] > 0); H.devrefs[d->id]--; } }
static bool idma_any_quarantined(void) { return false; }
static void mmget(struct mm_struct *mm) { assert(mm == &H.mm); H.mmrefs++; event("MM_GET"); }
static void mmput(struct mm_struct *mm) { assert(mm == &H.mm && H.mmrefs > 0); H.mmrefs--; event("MM_PUT"); }
static void cpus_read_lock(void) { H.cpus_lock++; event("CPUS_LOCK"); }
static void cpus_read_unlock(void) { assert(H.cpus_lock > 0); H.cpus_lock--; event("CPUS_UNLOCK"); }
static int mutex_lock_interruptible(int *lock) { (void)lock; assert(!H.mutex_lock); H.mutex_lock=1; event("TASK_LOCK"); return 0; }
static void mutex_unlock(int *lock) { (void)lock; assert(H.mutex_lock && H.active_lane == -1); H.mutex_lock=0; event("TASK_UNLOCK"); }
static void mmap_read_lock(struct mm_struct *mm) { assert(mm == &H.mm && H.mmrefs && H.mutex_lock); H.mmap_lock++; event("MMAP_LOCK"); }
static void mmap_read_unlock(struct mm_struct *mm) { assert(mm == &H.mm && H.mmap_lock && !H.pins); H.mmap_lock--; event("MMAP_UNLOCK"); }
static struct vm_area_struct *find_vma(struct mm_struct *mm, unsigned long addr) { (void)mm; (void)addr; return &H.vma; }
static bool vma_is_anonymous(struct vm_area_struct *vma) { (void)vma; return true; }
static long pin_user_pages(unsigned long base, unsigned long count, unsigned int flags, struct page **out) {
 assert(H.mmap_lock && H.mmrefs && base == (uintptr_t)H.arena && count == 8 && flags == (FOLL_WRITE|FOLL_LONGTERM));
 H.pin_calls++; H.pins = is_case("partial-pin") ? 3 : 8;
 for (int i=0; i<H.pins; i++) out[i]=&H.pages[i];
 event("PIN_%d", H.pins); return H.pins;
}
static void unpin_user_pages_dirty_lock(struct page **pages, unsigned long count, bool dirty) {
 (void)pages; assert(dirty && H.pins == (int)count && !H.domain_alive && !H.mapped && H.active_lane == -1 && H.mmap_lock);
 assert(!H.claimed[0] && !H.claimed[1] && !H.attached[0] && !H.attached[1]);
 H.pins=0; H.unpins++; event("UNPIN");
}
static pgd_t *pgd_offset(struct mm_struct *mm, unsigned long addr) { (void)mm; (void)addr; return &H.table; }
static p4d_t *p4d_offset(pgd_t *pgd, unsigned long addr) { (void)addr; return pgd; }
static pud_t *pud_offset(p4d_t *p4d, unsigned long addr) { (void)addr; return p4d; }
static pmd_t *pmd_offset(pud_t *pud, unsigned long addr) { (void)addr; return pud; }
#define pgd_none(v) (!(v).value)
#define pgd_bad(v) (false)
#define p4d_none(v) (!(v).value)
#define p4d_bad(v) (false)
#define pud_none(v) (!(v).value)
#define pud_bad(v) (false)
#define pud_leaf(v) (false)
#define pmd_none(v) (!(v).value)
#define pmd_bad(v) (false)
#define pmd_leaf(v) (false)
static pte_t *pte_offset_map_lock(struct mm_struct *mm, pmd_t *pmd, unsigned long addr, spinlock_t **lock) {
 static spinlock_t unused; (void)mm; (void)pmd; *lock=&unused; return &H.ptes[(addr-(uintptr_t)H.arena)/4096];
}
static pte_t ptep_get(pte_t *pte) { return *pte; }
#define pgd_val(pgd) ((pgd).value)
#define pmd_val(pmd) ((pmd).value)
#define pte_val(pte) ((pte).value)
#define pte_present(pte) ((pte).value & 1)
#define pte_user(pte) ((pte).value & 16)
#define pte_write(pte) ((pte).value & 4)
#define pte_young(pte) ((pte).value & 64)
#define pte_dirty(pte) ((pte).value & 128)
#define pte_pfn(pte) ((pte).value >> 10)
static unsigned long page_to_pfn(struct page *page) { return page->pfn; }
static phys_addr_t page_to_phys(struct page *page) { return page->pfn * PAGE_SIZE; }
static void pte_unmap_unlock(pte_t *pte, spinlock_t *lock) { (void)pte; (void)lock; }
static void *kmap_local_page(struct page *page) { return H.arena + page->index*PAGE_SIZE; }
static void kunmap_local(void *ptr) { (void)ptr; }
static void mb(void) { }
static void wmb(void) { }
static void local_flush_icache_all(void) { assert(H.active_lane == -1); H.cache_calls++; event("CACHE"); }
static u64 ktime_get_ns(void) { u64 before = H.clock_ns; H.clock_ns += 2000000; return before; }
static void usleep_range(unsigned int a, unsigned int b) { (void)a; (void)b; }
static void dev_err(struct device *dev, const char *format, ...) { (void)dev; (void)format; event("DEADLINE"); }
static void dev_crit(struct device *dev, const char *format, ...) {
 va_list ap; char reason[96]; (void)dev; va_start(ap,format); vsnprintf(reason,sizeof(reason),format,ap); va_end(ap);
 event("RETAIN:%.85s",reason);
}
static void schedule_timeout_uninterruptible(long ticks) {
 (void)ticks; assert(H.pins && H.mmrefs && H.mmap_lock && H.mutex_lock && H.session_alive);
 H.retained=1; longjmp(H.retained_jump,1);
}
static int reg_lane(const void *address, unsigned int *offset) {
 for (int i=0; i<4; i++) if ((uintptr_t)address >= (uintptr_t)H.regs[i] && (uintptr_t)address < (uintptr_t)(H.regs[i]+64)) {
  *offset=(unsigned int)((uintptr_t)address-(uintptr_t)H.regs[i]); return i;
 }
 assert(!"unknown MMIO address"); return -1;
}
static void writeq(u64 value, void *address) {
 unsigned int offset; int lane=reg_lane(address,&offset);
 assert(H.active_lane == -1); H.regs[lane][offset/8]=value;
}
static u64 readq(const void *address) {
 unsigned int offset; int lane=reg_lane(address,&offset);
 if (offset == IDMA_LAUNCHER_MAGIC_OFFSET) return IDMA_LAUNCHER_MAGIC;
 if (offset == IDMA_LAUNCHER_CONTROL_OFFSET || offset == IDMA_STATUS_REG_OFFSET) return 0;
 if (offset == IDMA_NEXT_ID_REG_OFFSET) {
  assert(H.active_lane == -1 && H.domain_alive && H.pins == 8 && H.barriers == 1 && H.mmap_lock && H.mutex_lock);
  assert(H.attached[0] && H.attached[1]);
  const int expected_lanes[]={0,1,1,0}; assert(H.next_calls < 4 && lane == expected_lanes[H.next_calls]);
  H.active_lane=lane; H.active_reads=0; H.next_calls++; event("NEXT_%d",lane);
  H.next_id[lane]++;
  if (is_case("next-zero")) return 0;
  return H.next_id[lane];
 }
 if (offset == IDMA_DONE_REG_OFFSET) {
  H.done_calls++; assert(H.active_lane == lane); H.active_reads++;
  if (H.active_reads < (is_case("timeout") ? 3 : 1)) return H.last_done[lane];
  u64 src=H.regs[lane][0], dst=H.regs[lane][1], n=H.regs[lane][2];
  u64 base=src-(H.next_calls==1?32:H.next_calls==2?4128:H.next_calls==3?8224:12320);
  assert(n>0 && n<=1011 && src>=base && dst>=base && dst-base+n<=7*4096);
  memcpy(H.arena+(dst-base),H.arena+(src-base),n);
  H.last_done[lane]=H.next_id[lane]; H.active_lane=-1; H.completions++; event("DONE_%d",lane);
  return H.last_done[lane];
 }
 return H.regs[lane][offset/8];
}
static struct iommu_domain *riscv_iommu_research_domain_alloc(struct device *dev, struct mm_struct *mm) {
 (void)dev; assert(H.pins==8 && H.mmap_lock && !H.domain_alive); H.domain_alive=1;
 if (mm) { H.domain_mmref=1; mmget(mm); }
 event("DOMAIN_ALLOC"); return &H.task_domain;
}
static int iommu_map(struct iommu_domain *domain, unsigned long iova, phys_addr_t pa, size_t n, int prot, int flags) {
 (void)iova; (void)pa; (void)prot; (void)flags;
 assert(domain==&H.task_domain && H.domain_alive && n==4096 && !H.attached[0] && !H.attached[1]);
 if (++H.map_calls==3 && is_case("map")) return -ENOMEM;
 H.mapped++; event("MAP_%d",H.mapped); return 0;
}
static struct iommu_domain *iommu_get_domain_for_dev(struct device *dev) { return dev->domain; }
static int iommu_device_claim_dma_owner(struct device *dev, void *owner) {
 (void)owner; assert(dev->index<2 && !H.claimed[dev->index]); H.claim_calls++;
 if (H.claim_calls==2 && is_case("claim")) return -EBUSY;
 H.claimed[dev->index]=1; dev->domain=&H.blocking; event("CLAIM_%d",dev->index); return 0;
}
static int iommu_attach_device(struct iommu_domain *domain, struct device *dev) {
 assert(domain==&H.task_domain && H.claimed[0] && H.claimed[1]); H.attach_calls++;
 if (H.attach_calls==2 && is_case("attach")) return -EIO;
 H.attached[dev->index]=1; dev->domain=domain; event("ATTACH_%d",dev->index); return 0;
}
static void iommu_detach_device(struct iommu_domain *domain, struct device *dev) {
 assert(domain==&H.task_domain && H.active_lane==-1 && H.attached[dev->index]);
 H.attached[dev->index]=0; dev->domain=&H.blocking; event("DETACH_%d",dev->index);
}
static void iommu_device_release_dma_owner(struct device *dev) {
 assert(H.claimed[dev->index] && H.active_lane==-1 && !H.attached[dev->index]);
 H.claimed[dev->index]=0; dev->domain=is_case("restore") && dev->index==1 ? &H.blocking : &H.defaults[dev->index];
 event("OWNER_RELEASE_%d",dev->index);
}
static int riscv_iommu_research_barrier(struct device *a, struct device *b, void (*sync)(void*),void *arg,struct riscv_iommu_research_certificate *cert) {
 (void)a; (void)b; (void)cert; assert(H.active_lane==-1 && H.pins==8 && H.mmap_lock); H.barriers++;
 event("BARRIER_%d",H.barriers); sync(arg);
 if ((is_case("initial-barrier") && H.barriers==1) || (is_case("restore-barrier") && H.barriers==2)) return -EIO;
 return 0;
}
static size_t iommu_unmap(struct iommu_domain *domain, unsigned long iova, size_t n) {
 (void)iova; assert(domain==&H.task_domain && H.active_lane==-1 && !H.claimed[0] && !H.claimed[1] && H.mapped>0);
 if (is_case("unmap")) return 0;
 H.mapped--; event("UNMAP_%d",H.mapped); return n;
}
static int riscv_iommu_research_domain_release(struct iommu_domain *domain) {
 assert(domain==&H.task_domain && H.domain_alive && !H.mapped && !H.attached[0] && !H.attached[1]);
 H.releases++; event("DOMAIN_RELEASE"); if (is_case("release")) return -EBUSY;
 H.domain_alive=0; if (H.domain_mmref) { H.domain_mmref=0; mmput(&H.mm); } return 0;
}

/* Only the kernel/MMIO/IOMMU boundary above is stubbed. No copied driver FSM. */
#include "drivers/misc/idma_sva_task.h"

static void initialize(const char *scenario) {
 memset(&H,0,sizeof(H)); H.scenario=scenario; H.active_lane=-1;
 H.arena=aligned_alloc(4096,32768); assert(H.arena); memset(H.arena,0xa5,32768);
 const int src[]={32,4128,8224,12320}, len[]={60,65,503,1011};
 for(int s=0;s<4;s++) for(int i=0;i<len[s];i++) H.arena[src[s]+i]=(unsigned char)(s*17+i*73);
 for(int i=0;i<8;i++) { H.pages[i].index=i; H.pages[i].pfn=0x80000+i; H.ptes[i].value=(H.pages[i].pfn<<10)|0xd7; }
 for(int i=0;i<4;i++) { H.devices[i].index=i; H.devices[i].domain=&H.defaults[i]; H.lanes[i].id=i; H.lanes[i].did=10+i; H.lanes[i].reg=H.regs[i]; H.lanes[i].miscdev.parent=&H.devices[i]; }
 H.table.value=1; H.mm.mm_users=1; H.task.mm=&H.mm; H.vma.vm_start=(uintptr_t)H.arena; H.vma.vm_end=(uintptr_t)H.arena+32768; H.vma.vm_flags=VM_READ|VM_WRITE;
 idma_sva_enable=true;
}
static void check_released(void) {
 assert(H.returned && !H.retained && H.active_lane==-1 && !H.pins && !H.mapped && !H.mmrefs && !H.domain_alive && !H.session_alive);
 assert(!H.cpus_lock && !H.mmap_lock && !H.mutex_lock && !H.claimed[0] && !H.claimed[1] && !H.attached[0] && !H.attached[1]);
 for(int i=0;i<4;i++) assert(!H.devrefs[i] && H.devices[i].domain==&H.defaults[i]);
}
static int run_case(const char *scenario,unsigned int mode) {
 initialize(scenario);
 struct idma_sva_run r={.abi_version=1,.struct_size=sizeof(r),.address_mode=mode,.peer_index=1,.timeout_ms=1,.arena_user=(uintptr_t)H.arena,.arena_bytes=32768};
 long result=0;
 if (!setjmp(H.retained_jump)) result=idma_sva_run_ioctl(&H.lanes[0],&r);
 bool retained=!strcmp(scenario,"initial-barrier") || !strcmp(scenario,"restore-barrier") || !strcmp(scenario,"next-zero") || !strcmp(scenario,"restore") || !strcmp(scenario,"unmap") || !strcmp(scenario,"release");
 if (retained) {
  assert(H.retained && !H.returned && H.pins==8 && H.mmrefs && H.mmap_lock && H.mutex_lock && H.session_alive);
  assert(H.lanes[0].quarantined && H.lanes[1].quarantined && H.unpins==0);
 } else {
  check_released(); assert(H.unpins==1);
  if (!strcmp(scenario,"success")) {
   assert(result==0 && r.status==0 && r.result_flags==IDMA_SVA_RESULT_SAFE && r.submitted==4 && r.completed==4 && H.completions==4);
   assert(H.barriers==2 && H.next_calls==4 && H.pin_calls==1);
   assert(r.pinned_pages==8 && r.mapped_pages==(mode==3?0:7));
  } else if (!strcmp(scenario,"timeout")) {
   assert(result==-ETIMEDOUT && r.status==-ETIMEDOUT && r.submitted==1 && r.completed==1 && H.next_calls==1 && H.completions==1 && H.barriers==2);
  } else {
   assert(result<0 && r.status==result && H.next_calls==0 && H.completions==0);
   if (!strcmp(scenario,"partial-pin")) assert(result==-EFAULT && H.map_calls==0 && H.barriers==0);
   if (!strcmp(scenario,"map")) assert(result==-ENOMEM && H.map_calls==3 && H.claim_calls==0);
   if (!strcmp(scenario,"claim")) assert(result==-EBUSY && H.claim_calls==2 && H.attach_calls==0 && H.barriers==1);
   if (!strcmp(scenario,"attach")) assert(result==-EIO && H.attach_calls==2 && H.barriers==1);
  }
 }
 printf("{\"scenario\":\"%s\",\"mode\":%u,\"result\":\"PASS\",\"hardware_evidence\":false,\"retained\":%s,\"returned\":%s,\"next\":%d,\"done\":%d,\"events\":[",scenario,mode,retained?"true":"false",H.returned?"true":"false",H.next_calls,H.completions);
 for(unsigned int i=0;i<H.event_count;i++) printf("%s\"%s\"",i?",":"",H.events[i]);
 puts("]}");
 if (!retained) free(H.arena);
 return 0;
}
int main(int argc,char **argv) {
 if(argc!=3) return 2; return run_case(argv[1],(unsigned int)atoi(argv[2]));
}

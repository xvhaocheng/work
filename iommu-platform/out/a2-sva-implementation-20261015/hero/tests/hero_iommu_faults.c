/* SPDX-License-Identifier: Apache-2.0
 * Link the actual production hero_iommu.c against explicit kernel/hardware
 * fakes. These tests prove software failure paths, never physical completion.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/errno.h>
#include "hero_iommu.h"

static struct device dev;
static struct iommu_domain domain;
static int owner_a, owner_b;
static struct page pages[32];
static struct {
    int pins, pin_calls, unpins, dirty, refs, allocs;
    int maps, unmaps, quiesces, resumes, syncs, cache_syncs, held;
    int pin_result, map_fail_at, short_unmap_at, sync_fail_at, quiesce_error, cache_error;
    unsigned long active[64]; unsigned count;
} w;

void *kzalloc(size_t size, int flags) { w.allocs++; return calloc(1,size); }
void *kcalloc(size_t n, size_t size, int flags) { w.allocs++; return calloc(n,size); }
void kfree(void *p) { if(p) { w.allocs--; free(p); } }
void __module_get(void *m) { w.refs++; }
void module_put(void *m) { assert(w.refs > 0); w.refs--; }
long pin_user_pages_fast(unsigned long addr, int nr, unsigned flags, struct page **out)
{
    int got = w.pin_result == -999 ? nr : w.pin_result;
    assert((flags & (FOLL_WRITE|FOLL_LONGTERM)) == (FOLL_WRITE|FOLL_LONGTERM));
    w.pin_calls++;
    if (got < 0) return got;
    assert(got <= nr && nr <= 32);
    for (int i=0;i<got;i++) { pages[i].phys=0x100000+i*PAGE_SIZE; out[i]=&pages[i]; }
    w.pins += got; return got;
}
void unpin_user_pages(struct page **p, unsigned long nr)
{ assert(w.pins >= (int)nr); w.pins-=nr; w.unpins+=nr; }
void unpin_user_pages_dirty_lock(struct page **p, unsigned long nr, bool dirty)
{ assert(dirty); w.dirty+=nr; unpin_user_pages(p,nr); }
int iommu_map(struct iommu_domain *d, unsigned long addr, u64 phys, size_t len, int prot, int flags)
{
    assert(w.held && len==PAGE_SIZE && prot==(IOMMU_READ|IOMMU_WRITE));
    w.maps++;
    if (w.maps==w.map_fail_at) return -ENOMEM;
    assert(w.count<64);
    for(unsigned i=0;i<w.count;i++) assert(w.active[i]!=addr);
    w.active[w.count++]=addr; return 0;
}
size_t iommu_unmap(struct iommu_domain *d, unsigned long addr, size_t len)
{
    assert(w.held && len==PAGE_SIZE); w.unmaps++;
    if(w.unmaps==w.short_unmap_at) return 0;
    for(unsigned i=0;i<w.count;i++) if(w.active[i]==addr) {
        w.active[i]=w.active[--w.count]; return PAGE_SIZE;
    }
    assert(!"unmapping unknown page"); return 0;
}
#ifndef TEST_DEFAULT_GATES
int hero_platform_quiesce(struct device *d)
{ w.quiesces++; if(w.quiesce_error) return w.quiesce_error; w.held=1; return 0; }
void hero_platform_resume(struct device *d)
{ assert(w.held); w.held=0; w.resumes++; }
int hero_platform_invalidate_complete(struct device *d, struct iommu_domain *domain)
{ assert(w.held); w.syncs++; return w.syncs==w.sync_fail_at ? -EIO : 0; }
int hero_platform_cache_sync(struct device *d, struct page **p, unsigned long nr, bool for_cpu)
{ assert(w.held); w.cache_syncs++; return w.cache_error; }
#endif

static void fresh(struct list_head *regions)
{
    assert(!w.pins && !w.refs && !w.allocs && !w.count);
    memset(&w,0,sizeof(w)); w.pin_result=-999; INIT_LIST_HEAD(regions);
}
static int add(struct list_head *regions, void *owner, u64 addr, unsigned nr)
{ return hero_iommu_region_add(&dev,&domain,regions,owner,0x1000000+addr-HERO_IOVA_BASE,addr,nr*PAGE_SIZE); }
static int remove_region(struct list_head *regions, void *owner, u64 addr, unsigned nr)
{ return hero_iommu_region_remove(&dev,&domain,regions,owner,0x1000000+addr-HERO_IOVA_BASE,addr,nr*PAGE_SIZE); }
static void clean(struct list_head *regions)
{ assert(list_empty(regions) && !w.pins && !w.refs && !w.allocs && !w.count); }
static void result(const char *name) { printf("PASS %s\n",name); }
int main(void)
{
    LIST_HEAD(regions);
#ifdef TEST_DEFAULT_GATES
    fresh(&regions);
    assert(add(&regions,&owner_a,HERO_IOVA_BASE,3)==-EOPNOTSUPP);
    assert(w.pin_calls==0 && w.maps==0); clean(&regions);
    result("actual-default-gate-refuses-before-pin");
#else
    fresh(&regions);
    assert(add(&regions,&owner_a,HERO_IOVA_BASE,3)==0);
    assert(w.pins==3 && w.count==6 && w.refs==1 && w.syncs==1);
    assert(remove_region(&regions,&owner_b,HERO_IOVA_BASE,3)==-ENOENT);
    assert(add(&regions,&owner_b,HERO_IOVA_BASE,3)==-EEXIST);
    assert(hero_iommu_cache_sync(&dev,&regions,&owner_a,0x1000000,HERO_IOVA_BASE,3*PAGE_SIZE,false)==0);
    assert(hero_iommu_cache_sync(&dev,&regions,&owner_a,0x1000000,HERO_IOVA_BASE,3*PAGE_SIZE,true)==0);
    assert(remove_region(&regions,&owner_a,HERO_IOVA_BASE,3)==0);
    assert(w.dirty==3 && w.unmaps==6 && w.cache_syncs==2); clean(&regions);
    result("normal-two-alias-owner-collision-cache-unpin");

    fresh(&regions); w.pin_result=1;
    assert(add(&regions,&owner_a,HERO_IOVA_BASE,3)==-EFAULT);
    assert(w.unpins==1 && w.maps==0); clean(&regions);
    result("partial-pin-rolls-back-exact-pinned-count");

    fresh(&regions); w.pin_result=-ENOMEM;
    assert(add(&regions,&owner_a,HERO_IOVA_BASE,3)==-ENOMEM);
    assert(w.unpins==0 && w.maps==0); clean(&regions);
    result("negative-pin-error-no-unpin");

    fresh(&regions); w.map_fail_at=4;
    assert(add(&regions,&owner_a,HERO_IOVA_BASE,3)==-ENOMEM);
    assert(w.unmaps==3 && w.dirty==3 && w.syncs==1); clean(&regions);
    result("partial-map-both-aliases-rollback-before-unpin");

    fresh(&regions); assert(add(&regions,&owner_a,HERO_IOVA_BASE,3)==0);
    w.short_unmap_at=1;
    assert(remove_region(&regions,&owner_a,HERO_IOVA_BASE,3)==-EIO);
    assert(w.pins==3 && w.refs==1 && w.count==6 && w.held && !list_empty(&regions));
    assert(add(&regions,&owner_b,HERO_IOVA_BASE+8*PAGE_SIZE,1)==-EIO);
    w.short_unmap_at=0;
    assert(remove_region(&regions,&owner_a,HERO_IOVA_BASE,3)==0); clean(&regions);
    result("short-unmap-quarantines-then-exact-retry");

    fresh(&regions); assert(add(&regions,&owner_a,HERO_IOVA_BASE,3)==0);
    w.sync_fail_at=2;
    assert(remove_region(&regions,&owner_a,HERO_IOVA_BASE,3)==-EIO);
    assert(w.count==0 && w.pins==3 && w.refs==1 && w.held && w.unpins==0);
    assert(remove_region(&regions,&owner_a,HERO_IOVA_BASE,3)==0); clean(&regions);
    result("full-unmap-is-not-completion-barrier-failure-retains-pins");

    fresh(&regions); w.map_fail_at=4; w.sync_fail_at=1;
    assert(add(&regions,&owner_a,HERO_IOVA_BASE,3)==-EIO);
    assert(w.count==0 && w.pins==3 && w.refs==1 && w.held);
    assert(remove_region(&regions,&owner_a,HERO_IOVA_BASE,3)==0); clean(&regions);
    result("rollback-invalidation-failure-retains-owner-and-pins");

    fresh(&regions); assert(add(&regions,&owner_a,HERO_IOVA_BASE,3)==0);
    w.quiesce_error=-EOPNOTSUPP;
    assert(remove_region(&regions,&owner_a,HERO_IOVA_BASE,3)==-EIO);
    assert(w.pins==3 && w.count==6 && w.refs==1);
    w.quiesce_error=0;
    assert(remove_region(&regions,&owner_a,HERO_IOVA_BASE,3)==0); clean(&regions);
    result("quiesce-failure-retains-maps-and-pins");

    fresh(&regions); assert(add(&regions,&owner_a,HERO_IOVA_BASE,3)==0);
    w.cache_error=-EIO;
    assert(hero_iommu_cache_sync(&dev,&regions,&owner_a,0x1000000,HERO_IOVA_BASE,3*PAGE_SIZE,false)==-EIO);
    assert(w.pins==3 && w.count==6 && w.held);
    assert(hero_iommu_cache_sync(&dev,&regions,&owner_a,0x1000000,HERO_IOVA_BASE,3*PAGE_SIZE,true)==-EIO);
    assert(remove_region(&regions,&owner_a,HERO_IOVA_BASE,3)==0); clean(&regions);
    result("cache-failure-quarantines-blocks-new-ownership");

    fresh(&regions); assert(add(&regions,&owner_a,HERO_IOVA_BASE,3)==0);
    assert(add(&regions,&owner_b,HERO_IOVA_BASE+8*PAGE_SIZE,2)==0);
    assert(hero_iommu_release_owner(&dev,&domain,&regions,&owner_a)==0);
    assert(w.pins==2 && w.count==4 && w.refs==1 && !list_empty(&regions));
    assert(hero_iommu_release_owner(&dev,&domain,&regions,&owner_b)==0); clean(&regions);
    result("file-release-only-removes-own-buffers");

    fresh(&regions);
    assert(add(&regions,&owner_a,HERO_IOVA_BASE+1,3)==-EINVAL);
    assert(add(&regions,&owner_a,HERO_IOVA_BASE,0)==-EINVAL);
    assert(add(&regions,&owner_a,HERO_IOVA_BASE+HERO_IOVA_SIZE-PAGE_SIZE,3)==-EINVAL);
    assert(!w.pin_calls && !w.quiesces); clean(&regions);
    result("invalid-alignment-size-range-rejected-before-pin");
#endif
    return 0;
}

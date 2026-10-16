// SPDX-License-Identifier: GPL-2.0 OR Apache-2.0
#include <linux/errno.h>
#include <linux/limits.h>
#include <linux/module.h>
#include <linux/overflow.h>
#include <linux/slab.h>
#include "hero_iommu.h"

static struct hero_iommu_region *find_region(struct list_head *regions,
        const void *owner, u64 user_addr, u64 iova, u64 length)
{
    struct hero_iommu_region *r;
    list_for_each_entry(r, regions, list)
        if (r->owner == owner && r->user_addr == user_addr &&
            r->iova == iova && r->length == length)
            return r;
    return NULL;
}

static bool has_quarantine(struct list_head *regions)
{
    struct hero_iommu_region *r;
    list_for_each_entry(r, regions, list)
        if (r->quarantined) return true;
    return false;
}

/* The caller holds every publisher stopped. Preserve exact progress on short
 * unmaps so neither a retry nor another owner can recycle an alias too early. */
static int revoke(struct device *dev, struct iommu_domain *domain,
                  struct hero_iommu_region *r)
{
    size_t done;
    while (r->bypass_mapped) {
        u64 addr = r->iova + (r->bypass_mapped - 1) * PAGE_SIZE;
        done = iommu_unmap(domain, addr + HERO_BYPASS_OFFSET, PAGE_SIZE);
        if (done != PAGE_SIZE)
            goto quarantine;
        r->bypass_mapped--;
    }
    while (r->base_mapped) {
        u64 addr = r->iova + (r->base_mapped - 1) * PAGE_SIZE;
        done = iommu_unmap(domain, addr, PAGE_SIZE);
        if (done != PAGE_SIZE)
            goto quarantine;
        r->base_mapped--;
    }
    /* This checked completion hook is mandatory even after a full-size unmap. */
    if (hero_platform_invalidate_complete(dev, domain))
        goto quarantine;
    unpin_user_pages_dirty_lock(r->pages, r->nr_pages, true);
    list_del(&r->list);
    kfree(r->pages);
    kfree(r);
    module_put(THIS_MODULE);
    return 0;
quarantine:
    r->quarantined = true;
    dev_err(dev, "HERO revoke incomplete: retaining pinned pages and stopped publishers\n");
    return -EIO;
}

int hero_iommu_region_add(struct device *dev, struct iommu_domain *domain,
        struct list_head *regions, const void *owner,
        u64 user_addr, u64 iova, u64 length)
{
    struct hero_iommu_region *r, *other;
    u64 end, user_end;
    long pinned;
    unsigned long i;
    int ret;

    if (!domain)
        return -ENODEV;
    if (!length || ((user_addr | iova | length) & (PAGE_SIZE - 1)) ||
        user_addr > ULONG_MAX || check_add_overflow(user_addr, length, &user_end) ||
        user_end > ULONG_MAX || check_add_overflow(iova, length, &end) ||
        iova < HERO_IOVA_BASE || end > HERO_IOVA_BASE + HERO_IOVA_SIZE ||
        (length >> PAGE_SHIFT) > INT_MAX)
        return -EINVAL;
    list_for_each_entry(other, regions, list) {
        if (other->quarantined)
            return -EIO;
        if (iova < other->iova + other->length && other->iova < end)
            return -EEXIST;
    }
    /* Capability is checked BEFORE pinning or installing a PTE. */
    ret = hero_platform_quiesce(dev);
    if (ret)
        return ret;
    r = kzalloc(sizeof(*r), GFP_KERNEL);
    if (!r) { ret = -ENOMEM; goto resume; }
    r->nr_pages = length >> PAGE_SHIFT;
    r->pages = kcalloc(r->nr_pages, sizeof(*r->pages), GFP_KERNEL);
    if (!r->pages) { ret = -ENOMEM; kfree(r); goto resume; }
    pinned = pin_user_pages_fast(user_addr, r->nr_pages,
                                FOLL_WRITE | FOLL_LONGTERM, r->pages);
    if (pinned < 0 || (unsigned long)pinned != r->nr_pages) {
        if (pinned > 0)
            unpin_user_pages(r->pages, pinned);
        ret = pinned < 0 ? pinned : -EFAULT;
        kfree(r->pages); kfree(r); goto resume;
    }
    r->owner = owner;
    r->user_addr = user_addr;
    r->iova = iova;
    r->length = length;
    /* Keep a module reference for each mapping, including quarantined ones. */
    __module_get(THIS_MODULE);
    list_add_tail(&r->list, regions);
    for (i = 0; i < r->nr_pages; i++) {
        ret = iommu_map(domain, iova + i * PAGE_SIZE,
                        page_to_phys(r->pages[i]), PAGE_SIZE,
                        IOMMU_READ | IOMMU_WRITE, GFP_KERNEL);
        if (ret) goto rollback;
        r->base_mapped++;
        ret = iommu_map(domain, iova + i * PAGE_SIZE + HERO_BYPASS_OFFSET,
                        page_to_phys(r->pages[i]) + HERO_BYPASS_OFFSET,
                        PAGE_SIZE, IOMMU_READ | IOMMU_WRITE, GFP_KERNEL);
        if (ret) goto rollback;
        r->bypass_mapped++;
    }
    ret = hero_platform_invalidate_complete(dev, domain);
    if (ret) goto rollback;
    hero_platform_resume(dev);
    return 0;
rollback:
    /* Do not expose an incomplete IOVA. No unpin until BOTH aliases are
     * removed and their invalidation completion has actually been received. */
    if (revoke(dev, domain, r))
        return -EIO; /* publishers remain stopped; record remains owned */
resume:
    hero_platform_resume(dev);
    return ret;
}

int hero_iommu_region_remove(struct device *dev, struct iommu_domain *domain,
        struct list_head *regions, const void *owner,
        u64 user_addr, u64 iova, u64 length)
{
    struct hero_iommu_region *r = find_region(regions, owner, user_addr, iova, length);
    int ret;
    if (!r) return -ENOENT;
    ret = hero_platform_quiesce(dev);
    if (ret) {
        r->quarantined = true;
        return -EIO; /* PTEs and pins remain; refuse new submissions/maps. */
    }
    ret = revoke(dev, domain, r);
    if (!ret && !has_quarantine(regions)) hero_platform_resume(dev);
    return ret;
}

int hero_iommu_release_owner(struct device *dev, struct iommu_domain *domain,
        struct list_head *regions, const void *owner)
{
    struct hero_iommu_region *r, *next;
    int ret = 0;
    list_for_each_entry_safe(r, next, regions, list) {
        if (r->owner != owner) continue;
        ret = hero_iommu_region_remove(dev, domain, regions, owner,
                                      r->user_addr, r->iova, r->length);
        if (ret) return ret;
    }
    return 0;
}

int hero_iommu_cache_sync(struct device *dev, struct list_head *regions,
        const void *owner, u64 user_addr, u64 iova, u64 length, bool for_cpu)
{
    struct hero_iommu_region *r = find_region(regions, owner, user_addr, iova, length);
    int ret;
    if (!r) return -ENOENT;
    if (has_quarantine(regions)) return -EIO;
    ret = hero_platform_quiesce(dev);
    if (ret) return ret;
    ret = hero_platform_cache_sync(dev, r->pages, r->nr_pages, for_cpu);
    if (!ret) hero_platform_resume(dev);
    /* Cache failure also leaves publishers stopped and blocks new maps. */
    else r->quarantined = true;
    return ret;
}

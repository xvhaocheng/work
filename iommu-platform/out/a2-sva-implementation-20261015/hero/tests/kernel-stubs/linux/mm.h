#pragma once
#include "types.h"
#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#define FOLL_WRITE 1
#define FOLL_LONGTERM 2
struct page { u64 phys; };
long pin_user_pages_fast(unsigned long addr, int count, unsigned int flags, struct page **pages);
void unpin_user_pages(struct page **pages, unsigned long nr);
void unpin_user_pages_dirty_lock(struct page **pages, unsigned long nr, bool dirty);
static inline u64 page_to_phys(struct page *p) { return p->phys; }

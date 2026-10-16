/* Host link wrappers: no device is opened and no real ioctl is called. */
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "../source/linux/include/uapi/linux/idma.h"

int __wrap_open(const char *path, int flags, ...)
{
    (void)flags;
    assert(strcmp(path, "/synthetic/idma0") == 0);
    return dup(STDOUT_FILENO);
}

int __wrap_ioctl(int fd, unsigned long request, ...)
{
    va_list ap;
    struct idma_finite_run *r;
    struct idma_finite_batch *b;
    struct idma_finite_leaf *l;
    (void)fd;
    assert(request == IDMA_IOC_RUN_FINITE);
    va_start(ap, request);
    r = va_arg(ap, struct idma_finite_run *);
    va_end(ap);
    r->status = -EIO;
    r->release_status = -EINPROGRESS;
    r->stage = 3;
    r->quarantined = 1;
    r->batch_count = 1;
    r->leaf_count = 1;
    r->allocation_kind = 1;
    r->extent_count = 4;
    r->backing_bytes = IDMA_FINITE_BACKING_BYTES;
    r->reference_bytes = 2*IDMA_FINITE_POOL_BYTES;
    r->swapout_tasks = 9;
    r->swapout_bytes = 36864;
    for (unsigned i = 0; i < 4; i++) {
        r->stages[i].reached = 1;
        r->stages[i].status = i == 3 ? -EIO : 0;
        r->stages[i].begin.cpu_cycles = 100+i*100;
        r->stages[i].begin.time_ticks = 200+i*100;
        r->stages[i].begin.nominal_ns = 300+i*100;
        r->stages[i].end = r->stages[i].begin;
    }
    b = (void *)(uintptr_t)r->batches_user;
    b[0].bytes = 36864;
    b[0].first_id = 37;
    b[0].observed_done = 36;
    b[0].issued = 9;
    b[0].completed = 8;
    b[0].status = -ETIMEDOUT;
    l = (void *)(uintptr_t)r->leaves_user;
    l[0].iova = 0x40000000;
    l[0].pa = 0x84000000;
    l[0].bytes = 2097152;
    l[0].pte = (0x84000000ULL >> 12 << 10)|0xc7;
    l[0].table_count = 2;
    l[0].table_pa[0] = 0x90000000;
    l[0].table_pa[1] = 0x90001000;
    r->hpm_before[0][0] = 123;
    r->hpm_after[0][0] = 456;
    r->observer[0][7] = 9;
    r->observer_status[0] = 4;
    r->composition[0][0] = IDMA_COMPOSITION_MAGIC;
    ((unsigned char *)(uintptr_t)r->pool_user[0])[0] = 0x5a;
    errno = EIO;
    return -1;
}

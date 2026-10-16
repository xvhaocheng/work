/* SPDX-License-Identifier: Apache-2.0
 * A CPU-only fault-injection backend for the real repaired host main.
 * This does not execute the kernel driver, target runtime, DMA, or hardware.
 */
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libhero/hero_api.h>

static const char *mode;
static unsigned maps, targets, syncs, unmaps;
static void *buffers[2], *original[2];
static unsigned lengths[2];
static int device_owned[2];

static void report(void)
{
    fprintf(stderr, "CPU harness maps=%u targets=%u syncs=%u unmaps=%u\n",
            maps, targets, syncs, unmaps);
    free(original[0]); free(original[1]);
}
void init_omp_target(void)
{
    mode = getenv("A2_TEST_MODE");
    if (!mode) mode = "normal";
    atexit(report);
}
uintptr_t hero_iommu_map_virt(HeroDev *dev, unsigned length, void *addr)
{
    unsigned id = maps++;
    if (id >= 2) abort();
    if ((!strcmp(mode, "map-fail") && id == 1) ||
        !strcmp(mode, "unsupported")) {
        errno = EOPNOTSUPP; return 0;
    }
    buffers[id] = addr; lengths[id] = length;
    return (uintptr_t)addr;
}
static unsigned lookup(void *addr, uintptr_t iova, unsigned length)
{
    for (unsigned i = 0; i < 2; i++)
        if (buffers[i] == addr && iova == (uintptr_t)addr && lengths[i] == length)
            return i;
    abort();
}
int hero_iommu_sync_for_device(HeroDev *dev, unsigned length, void *addr, uintptr_t iova)
{
    unsigned id = lookup(addr, iova, length);
    syncs++;
    if (device_owned[id]) abort();
    if (!strcmp(mode, "cache-fail")) { errno = EIO; return -1; }
    device_owned[id] = 1; return 0;
}
int hero_iommu_sync_for_cpu(HeroDev *dev, unsigned length, void *addr, uintptr_t iova)
{
    unsigned id = lookup(addr, iova, length);
    syncs++;
    if (!device_owned[id]) abort();
    device_owned[id] = 0; return 0;
}
int hero_iommu_unmap_virt(HeroDev *dev, unsigned length, void *addr, uintptr_t iova)
{
    unsigned id = lookup(addr, iova, length);
    unmaps++;
    if (!strcmp(mode, "unmap-fail") && id == 1) { errno = EIO; return -1; }
    buffers[id] = NULL; return 0;
}
int axpy(uint64_t x_alias, uint64_t y_alias, float alpha, uint32_t n)
{
    float *x = (float *)(uintptr_t)(x_alias - UINT64_C(0x200000000));
    float *y = (float *)(uintptr_t)(y_alias - UINT64_C(0x200000000));
    if (x != buffers[0] || y != buffers[1] || !device_owned[0] || !device_owned[1])
        abort();
    targets++;
    if (targets == 1) {
        for (unsigned i = 0; i < 2; i++) {
            original[i] = malloc(n * sizeof(float));
            if (!original[i]) abort();
            memcpy(original[i], buffers[i], n * sizeof(float));
        }
    } else if (memcmp(original[0], x, n * sizeof(float)) ||
               memcmp(original[1], y, n * sizeof(float))) {
        fprintf(stderr, "hot inputs were not restored\n");
        return -1;
    }
    for (unsigned i = 0; i < n; i++) y[i] = fmaf(alpha, x[i], y[i]);
    if (!strcmp(mode, "corrupt")) y[n - 1] += 1.0f;
    return 0;
}

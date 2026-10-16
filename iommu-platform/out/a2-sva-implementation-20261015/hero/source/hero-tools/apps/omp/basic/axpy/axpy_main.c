// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Cyril Koenig   <cykoenig@iis.ee.ethz.ch>
// Enrico Zelioli <ezelioli@iis.ee.ethz.ch>

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <libhero/hero_api.h>

#define TILE_FLOATS 512U
#define PAGE_BYTES 4096U
#define BYPASS_OFFSET UINT64_C(0x200000000)

int axpy(uint64_t x_iova, uint64_t y_iova, float alpha, uint32_t n);

/* Actual buffers never alias either reference. Read every result element before
 * a restore, and compare both cold and hot outputs against one immutable oracle. */
static int consume(const float *actual_x, const float *actual_y,
                   const float *reference_x, const float *expected, unsigned n,
                   const char *label)
{
    for (unsigned i = 0; i < n; i++) {
        if (actual_x[i] != reference_x[i] ||
            memcmp(&actual_y[i], &expected[i], sizeof(float))) {
            fprintf(stderr, "%s mismatch at %u: got %a expected %a\n",
                    label, i, actual_y[i], expected[i]);
            return -1;
        }
    }
    printf("%s consumer verified %u FP32 elements\n", label, n);
    return 0;
}

int main(int argc, char **argv)
{
    unsigned n = 32768U;
    float alpha = 1.0f;
    char *end;
    long parsed;
    int status = EXIT_FAILURE, x_mapped = 0, y_mapped = 0;
    int x_attempted = 0, y_attempted = 0;
    float *x_ref = NULL, *y_ref = NULL, *expected = NULL;
    float *x_dma = NULL, *y_dma = NULL;
    uintptr_t x_iova = 0, y_iova = 0;
    unsigned bytes = 0;

    if (argc > 1) {
        errno = 0; parsed = strtol(argv[1], &end, 10);
        if (errno || *end || parsed <= 0 || (unsigned long)parsed > UINT_MAX / sizeof(float))
            goto usage;
        n = (unsigned)parsed;
    }
    if (argc > 2) {
        errno = 0; alpha = strtof(argv[2], &end);
        if (errno || *end || !isfinite(alpha)) goto usage;
    }
    /* This repair deliberately binds the enabled-IOVA workload. The legacy
     * physical allocator/cache contract needs its own accepted control. */
    if (n % TILE_FLOATS || (argc > 3 && strcmp(argv[3], "1")) ||
        (argc > 4 && strcmp(argv[4], "0")) || argc > 5)
        goto usage;
    bytes = n * sizeof(float);
    if (bytes > UINT_MAX - PAGE_BYTES + 1U) goto usage;
    bytes = (bytes + PAGE_BYTES - 1U) & ~(PAGE_BYTES - 1U);
    x_ref = aligned_alloc(PAGE_BYTES, bytes);
    y_ref = aligned_alloc(PAGE_BYTES, bytes);
    expected = aligned_alloc(PAGE_BYTES, bytes);
    x_dma = aligned_alloc(PAGE_BYTES, bytes);
    y_dma = aligned_alloc(PAGE_BYTES, bytes);
    if (!x_ref || !y_ref || !expected || !x_dma || !y_dma) goto cleanup;
    /* Fully initialize the pinned extent, including any trailing page bytes. */
    memset(x_dma, 0, bytes); memset(y_dma, 0, bytes);
    for (unsigned i = 0; i < n; i++) {
        x_ref[i] = (float)((i * 17U + 3U) % 113U) * 0.25f;
        y_ref[i] = (float)((i * 11U + 7U) % 127U) * 0.25f;
        expected[i] = fmaf(alpha, x_ref[i], y_ref[i]);
        if (!isfinite(expected[i])) goto cleanup;
    }
    init_omp_target();
    /* Domain mode is owned by the kernel; never rewrite global DDTP from
     * /dev/mem while other mappings or requesters may be active. */
    x_attempted = 1;
    x_iova = hero_iommu_map_virt(NULL, bytes, x_dma);
    if (!x_iova) { perror("map x (requires accepted platform barriers)"); goto cleanup; }
    x_mapped = 1;
    y_attempted = 1;
    y_iova = hero_iommu_map_virt(NULL, bytes, y_dma);
    if (!y_iova) { perror("map y"); goto cleanup; }
    y_mapped = 1;

    for (unsigned run = 0; run < 2; run++) {
        const char *label = run ? "hot" : "cold";
        /* Cold and hot begin with identical, independently preserved inputs. */
        memcpy(x_dma, x_ref, n * sizeof(float));
        memcpy(y_dma, y_ref, n * sizeof(float));
        if (hero_iommu_sync_for_device(NULL, bytes, x_dma, x_iova) ||
            hero_iommu_sync_for_device(NULL, bytes, y_dma, y_iova)) {
            perror("cache handoff to device"); goto cleanup;
        }
        /* Synchronous target return now includes final output DMA wait before
         * the target dispatcher is allowed to publish DEVICE_DONE. */
        if (axpy(x_iova + BYPASS_OFFSET, y_iova + BYPASS_OFFSET, alpha, n))
            goto cleanup;
        if (hero_iommu_sync_for_cpu(NULL, bytes, x_dma, x_iova) ||
            hero_iommu_sync_for_cpu(NULL, bytes, y_dma, y_iova)) {
            perror("cache handoff to CPU"); goto cleanup;
        }
        if (consume(x_dma, y_dma, x_ref, expected, n, label)) goto cleanup;
    }
    status = EXIT_SUCCESS;
cleanup:
    if (y_mapped) {
        if (hero_iommu_unmap_virt(NULL, bytes, y_dma, y_iova)) {
            perror("unmap y: kernel retains pins until safe revocation");
            status = EXIT_FAILURE;
        } else y_mapped = 0;
    }
    if (x_mapped) {
        if (hero_iommu_unmap_virt(NULL, bytes, x_dma, x_iova)) {
            perror("unmap x: kernel retains pins until safe revocation");
            status = EXIT_FAILURE;
        } else x_mapped = 0;
    }
    /* free() is permitted only after successful unmap/invalidation/unpin.
     * On failure the process may exit, but physical pages remain pinned. */
    /* A failed ioctl may have quarantined a partially installed map. Without
     * a returned handle, retain that allocation until process exit as well. */
    if (!x_attempted || (x_iova && !x_mapped)) free(x_dma);
    if (!y_attempted || (y_iova && !y_mapped)) free(y_dma);
    free(x_ref); free(y_ref); free(expected);
    return status;
usage:
    fprintf(stderr, "usage: %s [n divisible by 512] [finite alpha] [1] [0]\n", argv[0]);
    return EXIT_FAILURE;
}

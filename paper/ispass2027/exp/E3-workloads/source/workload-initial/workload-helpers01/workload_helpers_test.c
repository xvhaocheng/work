/* Exact private production helpers, with host stand-ins for kernel bitops. */
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <linux/idma_workload.h>
typedef uint32_t u32;
typedef uint64_t u64;
#define BITS_PER_LONG (sizeof(unsigned long) * CHAR_BIT)
#define BITS_TO_LONGS(n) (((n) + BITS_PER_LONG - 1) / BITS_PER_LONG)
#define min(a,b) ((a) < (b) ? (a) : (b))
#define check_mul_overflow(a,b,p) __builtin_mul_overflow(a,b,p)
static bool is_power_of_2(u32 n) { return n && !(n & (n - 1)); }
static const void *memchr_inv(const void *v, int c, size_t n)
{
	const unsigned char *p = v;
	while (n--) { if (*p != (unsigned char)c) return p; p++; }
	return NULL;
}
static void bitmap_zero(unsigned long *p, size_t n)
{
	memset(p, 0, BITS_TO_LONGS(n) * sizeof(*p));
}
static void bitmap_set(unsigned long *p, size_t start, size_t n)
{
	while (n--) { p[start / BITS_PER_LONG] |= 1UL << (start % BITS_PER_LONG); start++; }
}
static unsigned int hweight_long(unsigned long n) { return __builtin_popcountl(n); }
static unsigned int bitmap_weight(const unsigned long *p, unsigned int n)
{
	unsigned int total = 0;
	for (unsigned int i = 0; i < n; i++) total += !!(p[i / BITS_PER_LONG] & (1UL << (i % BITS_PER_LONG)));
	return total;
}
#include "idma-workload.h"
_Static_assert(sizeof(unsigned long) == 8, "host test follows RV64 bit word width");
_Static_assert(sizeof(struct idma_workload_caps) == 64, "caps");
_Static_assert(sizeof(struct idma_workload_geometry) == 64, "geometry");
_Static_assert(sizeof(struct idma_workload_experiment) == 544, "run");

int main(void)
{
	unsigned int id;
	struct idma_workload_geometry g = {};
	unsigned long measured[BITS_TO_LONGS(IDMA_WORKLOAD_BITMAP_BITS)];
	unsigned long warmup[BITS_TO_LONGS(IDMA_WORKLOAD_BITMAP_BITS)];
	unsigned long long n, w, reserved;
	while (scanf("%u %u %u %u %u %u %u %u %u %llu %llu %llu", &id, &g.geometry,
		&g.row_bytes, &g.rows_per_operation, &g.row_pitch_bytes, &g.ring_pages,
		&g.matrix_rows, &g.columns, &g.reserved0, &reserved, &n, &w) == 12) {
		g.reserved[3] = reserved;
		int ret = idma_workload_geometry_validate(&g);
		printf("{\"index\":%u,\"validation\":%d", id, ret);
		if (!ret) {
			u64 ring = idma_workload_ring_descriptors(&g);
			u64 rows = g.rows_per_operation;
			u64 group = rows * (g.columns ? g.columns : 1);
			u64 indices[] = {0, 1, 2, 3, rows-1, rows, rows+1, group-1, group,
				ring-1, ring, ring+1, 2*ring-1, 2*ring, UINT64_MAX};
			idma_workload_prefix_bitmap(measured, &g, n);
			idma_workload_prefix_bitmap(warmup, &g, w);
			printf(",\"ring\":%llu,\"measured_bytes\":%llu,\"warmup_bytes\":%llu,\"union_bytes\":%llu,\"measured_pages\":%u,\"warmup_pages\":%u",
				(unsigned long long)ring,
				(unsigned long long)bitmap_weight(measured, IDMA_WORKLOAD_BITMAP_BITS)*64,
				(unsigned long long)bitmap_weight(warmup, IDMA_WORKLOAD_BITMAP_BITS)*64,
				(unsigned long long)idma_workload_union_bytes(warmup, measured),
				idma_workload_distinct_pages(measured, &g), idma_workload_distinct_pages(warmup, &g));
			printf(",\"offset_checks\":[");
			for (unsigned int k=0;k<sizeof(indices)/sizeof(indices[0]);k++)
				printf("%s[%llu,%zu]", k?",":"", (unsigned long long)indices[k], idma_workload_offset(&g, indices[k]));
			printf("],\"measured_bitmap\":\"");
			for (unsigned int k=0;k<BITS_TO_LONGS(IDMA_WORKLOAD_BITMAP_BITS);k++) printf("%016lx", measured[k]);
			printf("\",\"warmup_bitmap\":\"");
			for (unsigned int k=0;k<BITS_TO_LONGS(IDMA_WORKLOAD_BITMAP_BITS);k++) printf("%016lx", warmup[k]);
			printf("\"");
		}
		puts("}");
	}
	return 0;
}

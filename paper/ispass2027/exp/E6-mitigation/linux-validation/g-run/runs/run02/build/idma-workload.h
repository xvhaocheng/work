/* SPDX-License-Identifier: GPL-2.0 */
/* Private bounded geometry helpers. The caller includes bitmap/kernel/overflow
 * and the ABI5 UAPI first. No MMIO, allocation, cache or DMA operation occurs.
 */
#ifndef IDMA_WORKLOAD_INTERNAL_H
#define IDMA_WORKLOAD_INTERNAL_H

#define IDMA_WORKLOAD_GRANULE 64U
#define IDMA_WORKLOAD_BITMAP_BITS (IDMA_WORKLOAD_MAX_PAGES * 4096U / IDMA_WORKLOAD_GRANULE)

static inline bool idma_workload_power_length(u32 bytes)
{
	return bytes == 64 || bytes == 256 || bytes == 1024 || bytes == 4096 ||
	       bytes == 16384 || bytes == 65536;
}

static inline int idma_workload_geometry_validate(const struct idma_workload_geometry *g)
{
	u64 span = (u64)g->ring_pages * 4096, operation;

	if (!g->ring_pages || g->ring_pages > IDMA_WORKLOAD_MAX_PAGES ||
	    !is_power_of_2(g->ring_pages) || !g->row_bytes ||
	    !g->rows_per_operation || g->rows_per_operation > IDMA_WORKLOAD_MAX_ROWS ||
	    g->reserved0 || memchr_inv(g->reserved, 0, sizeof(g->reserved)) ||
	    check_mul_overflow((u64)g->row_bytes, (u64)g->rows_per_operation, &operation) ||
	    operation > IDMA_WORKLOAD_MAX_OPERATION_BYTES)
		return -EINVAL;
	switch (g->geometry) {
	case IDMA_WORKLOAD_LINEAR_RING:
		if (!idma_workload_power_length(g->row_bytes) || g->rows_per_operation != 1 ||
		    g->row_pitch_bytes || g->matrix_rows || g->columns ||
		    g->row_bytes > span || span % g->row_bytes)
			return -EINVAL;
		break;
	case IDMA_WORKLOAD_PAGE_PACKED_RING:
		if ((g->row_bytes != 64 && g->row_bytes != 256 && g->row_bytes != 1024 &&
		     g->row_bytes != 1536 && g->row_bytes != 4096) ||
		    g->rows_per_operation != 1 || g->row_pitch_bytes || g->matrix_rows || g->columns)
			return -EINVAL;
		break;
	case IDMA_WORKLOAD_TILE_ROWS:
		if (g->ring_pages != 256 || g->row_pitch_bytes != 16384 || g->matrix_rows != 64 ||
		    !((g->row_bytes == 64 && g->rows_per_operation == 1 && g->columns == 256) ||
		      (g->row_bytes == 256 && g->rows_per_operation == 1 && g->columns == 64) ||
		      (g->row_bytes == 1024 && g->columns == 16 &&
		       (g->rows_per_operation == 1 || g->rows_per_operation == 4 ||
			g->rows_per_operation == 16 || g->rows_per_operation == 64))))
			return -EINVAL;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

/* All helpers below require a successfully validated geometry. Counts may be
 * arbitrarily large; first reduce to one finite ring before any multiplication.
 */
static inline u64 idma_workload_ring_descriptors(const struct idma_workload_geometry *g)
{
	if (g->geometry == IDMA_WORKLOAD_TILE_ROWS)
		return (u64)g->matrix_rows * g->columns;
	if (g->geometry == IDMA_WORKLOAD_PAGE_PACKED_RING)
		return (u64)g->ring_pages * (4096U / g->row_bytes);
	return (u64)g->ring_pages * 4096 / g->row_bytes;
}

static inline size_t idma_workload_offset(const struct idma_workload_geometry *g, u64 descriptor)
{
	u64 index = descriptor % idma_workload_ring_descriptors(g);

	if (g->geometry == IDMA_WORKLOAD_TILE_ROWS) {
		u64 op = index / g->rows_per_operation;
		u64 row = index % g->rows_per_operation;

		return ((op / g->columns) * g->rows_per_operation + row) * g->row_pitch_bytes +
		       (op % g->columns) * g->row_bytes;
	}
	if (g->geometry == IDMA_WORKLOAD_PAGE_PACKED_RING) {
		u32 slots = 4096U / g->row_bytes;

		return (index / slots) * 4096 + (index % slots) * g->row_bytes;
	}
	return index * g->row_bytes;
}

static inline void idma_workload_prefix_bitmap(unsigned long *bits,
		const struct idma_workload_geometry *g, u64 completed)
{
	u64 i, count = min(completed, idma_workload_ring_descriptors(g));

	bitmap_zero(bits, IDMA_WORKLOAD_BITMAP_BITS);
	for (i = 0; i < count; i++)
		bitmap_set(bits, idma_workload_offset(g, i) / IDMA_WORKLOAD_GRANULE,
			   g->row_bytes / IDMA_WORKLOAD_GRANULE);
}

static inline u32 idma_workload_distinct_pages(const unsigned long *bits,
		const struct idma_workload_geometry *g)
{
	u32 page, pages = 0;

	for (page = 0; page < g->ring_pages; page++)
		if (bitmap_weight(bits + page * (64U / BITS_PER_LONG), 64))
			pages++;
	return pages;
}

static inline u64 idma_workload_union_bytes(const unsigned long *warmup,
		const unsigned long *measured)
{
	u32 i;
	u64 granules = 0;

	for (i = 0; i < BITS_TO_LONGS(IDMA_WORKLOAD_BITMAP_BITS); i++)
		granules += hweight_long(warmup[i] | measured[i]);
	return granules * IDMA_WORKLOAD_GRANULE;
}

#endif

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _IDMA_G_HELPERS_H
#define _IDMA_G_HELPERS_H

#include <linux/errno.h>
#include <linux/string.h>
#include <linux/types.h>
#include <uapi/linux/idma_g.h>

/* Pure helpers, shared by the future RUN adapter and host tests. */
static inline int idma_g_validate_input(const struct idma_g_experiment *p)
{
	bool has_a = p->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;

	if (p->abi_version != IDMA_G_ABI || p->struct_size != sizeof(*p))
		return -EPROTO;
	if (p->flags || memchr_inv(p->reserved, 0, sizeof(p->reserved)) ||
	    p->victim_pages != 1 || p->chunk_bytes != IDMA_G_CHUNK_BYTES ||
	    p->victim_transfers != IDMA_G_TRANSFERS ||
	    p->warmup_transfers != IDMA_G_WARMUP_TRANSFERS ||
	    p->victim_gap_cycles || !p->run_timeout_ms ||
	    p->run_timeout_ms > IDMA_G_MAX_TIMEOUT_MS)
		return -EINVAL;
	if (!has_a)
		return p->aggressor_pages || p->aggressor_transfers ||
			p->aggressor_gap_cycles ? -EINVAL : 0;
	if (p->aggressor_id != 1 || p->aggressor_pages != 8 ||
	    p->aggressor_transfers != IDMA_G_TRANSFERS ||
	    (p->aggressor_gap_cycles != 0 && p->aggressor_gap_cycles != 1000 &&
	     p->aggressor_gap_cycles != 2000))
		return -EINVAL;
	return 0;
}

/* Validate frozen hardware words without modifying them or substituting input
 * values. The caller must additionally enforce old OBS4/PATH/hist validity,
 * real DMA counts, both lanes' quiet state, data verification, and run errno.
 */
static inline u32 idma_g_validate_snapshot(const u64 w[IDMA_G_BANK_WORDS],
					 bool has_a, u32 gap, u64 old_v_cycles)
{
	u32 bad = 0;
	unsigned int i;
	static const unsigned int solo_zero[] = {
		5, 7, 11, 12, 13, 15, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 29, 30
	};

	if (w[0] != IDMA_G_BANK_MAGIC)
		bad |= IDMA_G_BAD_CAP;
	if (w[1] || w[31])
		bad |= IDMA_G_BAD_RESERVED;
	if (w[2] != (has_a ? 0x3c : 0x0c))
		bad |= IDMA_G_BAD_FLAGS;
	if (w[3])
		bad |= IDMA_G_HARDWARE_ERROR;
	if (w[4] != IDMA_G_TRANSFERS || w[6] ||
	    w[5] != (has_a ? IDMA_G_TRANSFERS : 0) ||
	    w[7] != (has_a ? gap : 0) ||
	    (gap != 0 && gap != 1000 && gap != 2000) || (!has_a && gap))
		bad |= IDMA_G_BAD_CONFIG;
	if (w[8] != IDMA_G_TRANSFERS || w[9] != IDMA_G_TRANSFERS ||
	    (has_a && (w[11] != IDMA_G_TRANSFERS || w[12] != IDMA_G_TRANSFERS)))
		bad |= IDMA_G_BAD_BUDGET;
	if (!w[10] || w[28] < w[27] || w[28] - w[27] != w[10] ||
	    w[10] != w[16] || w[16] != old_v_cycles || w[14] ||
	    (has_a && (!w[13] || w[30] < w[29] || w[30] - w[29] != w[13] ||
		       w[15] > w[13] || w[15] != (u64)(IDMA_G_TRANSFERS - 1) * gap)))
		bad |= IDMA_G_BAD_TIME;
	/* Bounds precede sums/subtractions, so corrupt 64-bit MMIO cannot wrap
	 * a conservation check into a plausible value.
	 */
	if (w[17] > IDMA_G_TRANSFERS || w[18] > IDMA_G_TRANSFERS ||
	    w[19] > 1 || w[20] > 1 || w[21] > 1 || w[22] > 1 ||
	    w[19] + w[17] != w[18] + w[20])
		bad |= IDMA_G_BAD_WINDOW;
	if (w[23] > IDMA_G_TRANSFERS || w[24] > IDMA_G_TRANSFERS ||
	    (!!w[23] != !!w[19]) || (!!w[24] != !!w[20]) ||
	    (w[22] && !w[19]) ||
	    (w[21] != (u64)(!!w[19] && !w[22] && !!w[18])))
		bad |= IDMA_G_BAD_IDS;
	if (!w[18]) {
		if (w[25] || w[26])
			bad |= IDMA_G_BAD_IDS;
	} else if (!w[25] || w[25] > IDMA_G_TRANSFERS ||
		   w[26] < w[25] || w[26] > IDMA_G_TRANSFERS ||
		   w[26] - w[25] + 1 != w[18] ||
		   (w[19] && w[25] != w[23]) ||
		   (w[20] && w[24] != w[26] + 1)) {
		bad |= IDMA_G_BAD_IDS;
	}
	if (!has_a)
		for (i = 0; i < sizeof(solo_zero) / sizeof(solo_zero[0]); i++)
			if (w[solo_zero[i]])
				bad |= IDMA_G_BAD_SOLO;
	return bad;
}

#endif

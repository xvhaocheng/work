/*
 * Host model of the controlled-launch contract consumed by
 * linux/drivers/misc/idma.c (IDMA_IOC_RUN_E3_CONTROLLED).
 *
 * Production helpers are inserted verbatim by tests/test_idma_controlled.py at
 * the PRODUCTION_TYPES / PRODUCTION_FUNCTIONS markers.  Everything else is a
 * host stub (kernel MMIO/ktime/HPM/userspace copies) plus a model of the two
 * hardware blocks the driver actually talks to:
 *
 *   cva6/corev_apu/fpga/src/dma_controlled_launcher.sv
 *   cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv
 *
 * Software control flow only: not FPGA, IOTLB, cache or HPM acceptance data.
 *
 * Model rules taken from that RTL:
 *   - ABORT never fabricates completion.  Issued lanes finish naturally; held
 *     lanes are cancelled and their reserved ID retired with no copy; a lane
 *     offered to a backend that never accepts (COUNT with ready=0) cannot be
 *     cancelled at all and blocks terminal forever.
 *   - ABORT observed in PREP moves the launcher to RUNNING so the drain and the
 *     terminal condition are evaluated; a registered GO pulse still pending at
 *     that point is masked (launch_go_o is gated by abort_eff).
 *   - status_word = error[63:32] | active[15:12] | held[11:8] |
 *     FROZEN[2] | RUNNING[1] | PREP[0] | ABORTED[3]; active includes HOLD.
 *   - launcher c8 bank is the 32-word dma_controlled_launcher record.
 *   - IDs come from idma_transfer_id_gen: monotonic, no reuse, per frontend,
 *     first value 2, advancing on priming and formal jobs; a cancelled job
 *     retires its reserved ID into the DONE register without a completion.
 *   - The A port is released when the select tap observes the A read, before
 *     A completes.
 */
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/idma.h>

typedef __u64 u64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef uintptr_t dma_addr_t;
typedef uintptr_t phys_addr_t;

struct device { int index; };
struct page { int unused; };
struct miscdevice { struct device *parent; };
struct mutex { int unused; };
struct iommu_domain { int unused; };

#define BIT_ULL(n) (1ULL << (n))
#define BIT(n) (1U << (n))
#define __iomem
#define __user
#define WRITE_ONCE(dst, src) ((dst) = (src))
#define READ_ONCE(src) (*(volatile __typeof__(src) *)&(src))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define NSEC_PER_USEC 1000ULL
#define NSEC_PER_MSEC 1000000ULL
#define PAGE_SIZE 4096U
#define offset_in_page(ptr) ((uintptr_t)(ptr) & (PAGE_SIZE - 1))
#define SZ_4K 4096U
#define DMA_ATTR_FORCE_CONTIGUOUS 0
#define current NULL
#define GFP_KERNEL 0
#define kzalloc(n, flags) calloc(1, (n))
#define kfree(ptr) free(ptr)

/* PRODUCTION_TYPES */

struct riscv_iommu_hpm_session { int unused; };
struct riscv_iommu_hpm_sample { u64 cycles, iotlb_miss[2], s1_ptw[2], s2_ptw[2]; };

/* --------------------------------------------------------------------- */
/* Model of dma_controlled_launcher.sv / idma_reg64_frontend.sv          */
/* --------------------------------------------------------------------- */

#define FE_PORTS 2
#define SIM_STEP_NS 1000ULL
#define ID_GEN_FIRST 2ULL
#define OBS_INDEX0_VALUE 0xfeedULL

enum fe_state { FE_IDLE, FE_HOLD, FE_COUNT, FE_WAIT };
enum launcher_state { L_OFF, L_PREP, L_RUNNING, L_FROZEN };

/* Sticky launcher error bits, exactly as in dma_controlled_launcher.sv. */
#define LERR_BAD_STROBE   0
#define LERR_BAD_CMD      1
#define LERR_BAD_PREP     2
#define LERR_BAD_GO       3
#define LERR_ABORT        4
#define LERR_FRONTEND     5
#define LERR_UNEXP_ISSUE  6
#define LERR_UNEXP_DONE   7
#define LERR_DONE_NO_ISSUE 8
#define LERR_ISSUE_ID     9
#define LERR_DONE_ID      10
#define LERR_EXTRA_ISSUE  11
#define LERR_EXTRA_DONE   12
#define LERR_TICK_OVF     13
#define LERR_BAD_INDEX    14
#define LERR_BAD_C8       15
#define LERR_BAD_B0       16
#define LERR_CANCEL_RACE  17

/* One frontend port: idma_reg64_frontend state machine + its backend. */
struct fe_port {
	enum fe_state state;
	bool ctrl;			/* controlled job reserved (launch_ctrl_q) */
	bool busy;			/* backend transfer in flight */
	bool stalling;			/* backend never accepts */
	u64 id_q;			/* reserved/active transfer id */
	u64 src, dst, nbytes;		/* latched at offer time */
	u64 due;			/* backend completion time (ns) */
	bool issue_ev, done_ev;		/* this cycle obs_issue/obs_done */
	u64 issue_id_ev, done_id_ev;
	u64 last_done_id;		/* dma_hw2reg.done */
	u64 issue_count, done_count;
	u64 id_gen;			/* idma_transfer_id_gen */
	u64 next_id_reads;
	u64 done_reads;
	bool done_seen_by_sw;		/* driver read the controlled completion */
};

struct fake_dma {
	u64 regs[64];
	u64 latency;
	bool populated;			/* HPM: translation present since flush */
	u64 miss;
	struct fe_port fe;
};

static struct fake_dma hw[FE_PORTS];
static struct device devices[2] = {{0}, {1}};
static struct idma_device idmas[2];
static struct mutex idma_experiment_lock;
static struct riscv_iommu_hpm_session session;
static struct iommu_domain dummy_domain;

static u64 sim_now = 1000;
static unsigned allocations, frees, unmaps, mmio_writes;
static unsigned hpm_acquires, hpm_releases, hpm_reads;
static unsigned obs_arms, obs_finishes, obs_mmio, batch_writes;
static bool hpm_last_read_after_terminal;
static u64 idma_observation_sequence;
static bool controlled_v7;
static bool idma_path_observe = true;

/* Registered src/dst IOVA ownership per lane, so unmap can only release a
 * range this harness actually mapped and whose lane is idle. */
static uintptr_t lane_iova[FE_PORTS][2];

/* Injected faults (none by default). */
static bool ctl_stall_v;		/* V backend never completes */
static bool ctl_stall_a;		/* A backend never completes */
static bool ctl_no_ready_a;		/* A offered to backend, never accepted */
static bool ctl_drop_v_done;		/* launcher never sees V completion */
static bool ctl_late_v_done;		/* V completes during launcher status read */
static bool ctl_go_error;		/* GO readback reports a sticky launcher error */
static bool ctl_zero_id_v, ctl_zero_id_a;
static bool ctl_bad_id, ctl_bad_count, ctl_extra_mask;

/* Launcher state (dma_controlled_launcher.sv). */
static enum launcher_state l_state;
static u32 l_error;
static u64 l_ticks;
static u8 l_expected_mask, l_hold_mask, l_issued_mask, l_done_mask;
static u8 l_cancelled_mask, l_seen_held;
static bool l_aborted, l_has_a;
static u8 l_index;
static u64 l_issue_id[2], l_done_id[2];
static bool l_issue_id_valid[2], l_done_id_valid[2];
static u64 l_issue_count[2], l_done_count[2];
static u64 l_issue_ts[2], l_done_ts[2];
static bool l_issue_ts_valid[2], l_done_ts_valid[2];
static u64 l_go_a_ts, l_a_select_ts, l_go_v_ts;
static bool l_go_a_ts_valid, l_a_select_ts_valid, l_go_v_ts_valid;
static u64 l_selected_id, l_selected_did, l_selected_iova;
static bool l_selected_write;
static bool l_go_pending[2];
static bool l_a_select_seen, l_v_go_sent;
static unsigned l_prep_writes, l_ctrl_writes, l_go_count, l_abort_count;
static unsigned l_cancel_pulses, l_freezes;
static bool l_froze_before_v_done_read;
static unsigned v_late_done_hits;

/* Late-V race fault: armed by the test, pending only after a controlled V
 * DONE read that does not yet match, fired once on the launcher status read. */
static bool v_late_pending;
static u64 v_controlled_id;		/* formal V reserved transfer id */
static u64 v_controlled_done_id;	/* formal V id actually retired by a copy */

static unsigned launcher_held_bits(void)
{
	unsigned m = 0;
	int p;

	for (p = 0; p < FE_PORTS; p++)
		if (hw[p].fe.state == FE_HOLD)
			m |= 1u << p;
	return m;
}

static unsigned launcher_active_bits(void)
{
	unsigned m = 0;
	int p;

	for (p = 0; p < FE_PORTS; p++)
		if (hw[p].fe.state == FE_HOLD || hw[p].fe.state == FE_COUNT ||
		    hw[p].fe.state == FE_WAIT)
			m |= 1u << p;
	return m;
}

/* status_word of dma_controlled_launcher.sv. */
static u64 launcher_status(void)
{
	u64 s = 0;

	s |= (u64)launcher_held_bits() << IDMA_LAUNCHER_HELD_SHIFT;
	s |= (u64)launcher_active_bits() << IDMA_LAUNCHER_ACTIVE_SHIFT;
	if (l_aborted)
		s |= IDMA_LAUNCHER_STATUS_ABORTED;
	if (l_state == L_FROZEN)
		s |= IDMA_LAUNCHER_STATUS_FROZEN;
	if (l_state == L_RUNNING)
		s |= IDMA_LAUNCHER_STATUS_RUNNING;
	if (l_state == L_PREP)
		s |= IDMA_LAUNCHER_STATUS_PREP;
	s |= (u64)l_error << IDMA_LAUNCHER_ERROR_SHIFT;
	return s;
}

static void launcher_reset(void)
{
	int p;

	l_state = L_OFF;
	l_error = 0;
	l_ticks = 0;
	l_expected_mask = l_hold_mask = l_issued_mask = l_done_mask = 0;
	l_cancelled_mask = l_seen_held = 0;
	l_aborted = false;
	l_has_a = false;
	l_index = 0;
	l_go_a_ts = l_a_select_ts = l_go_v_ts = 0;
	l_go_a_ts_valid = l_a_select_ts_valid = l_go_v_ts_valid = false;
	l_selected_id = l_selected_did = l_selected_iova = 0;
	l_selected_write = false;
	l_go_pending[0] = l_go_pending[1] = false;
	l_a_select_seen = false;
	l_v_go_sent = false;
	for (p = 0; p < FE_PORTS; p++) {
		l_issue_id[p] = l_done_id[p] = 0;
		l_issue_id_valid[p] = l_done_id_valid[p] = false;
		l_issue_count[p] = l_done_count[p] = 0;
		l_issue_ts[p] = l_done_ts[p] = 0;
		l_issue_ts_valid[p] = l_done_ts_valid[p] = false;
	}
}

/* One clock of the launcher + frontend + backend. */
static void hw_step(void)
{
	unsigned held, active, exp, iss, canc, seen;
	int p;

	for (p = 0; p < FE_PORTS; p++) {
		hw[p].fe.issue_ev = false;
		hw[p].fe.done_ev = false;
	}

	/* 1. Backend completions.  Held reservations are untouched here. */
	for (p = 0; p < FE_PORTS; p++) {
		struct fe_port *fe = &hw[p].fe;

		if (fe->state != FE_WAIT || !fe->busy || sim_now < fe->due)
			continue;
		if (fe->nbytes)
			memcpy((void *)(uintptr_t)fe->dst,
			       (void *)(uintptr_t)fe->src, (size_t)fe->nbytes);
		/* Remember an actual controlled V completion before clearing. */
		if (p == 0 && fe->ctrl)
			v_controlled_done_id = fe->id_q;
		fe->busy = false;
		fe->state = FE_IDLE;
		fe->ctrl = false;
		fe->last_done_id = fe->id_q;
		fe->done_id_ev = fe->id_q;
		fe->id_q = 0;
		fe->done_count++;
		if (!(ctl_drop_v_done && p == 0))
			fe->done_ev = true;
	}

	/* 2. Registered launch_go_o pulse: HOLD -> COUNT.  The RTL gates
	 *    launch_go_o with abort_eff, so a pulse still pending when the
	 *    launcher is aborted is dropped and the lane stays held (and is
	 *    then cancelled by the drain). */
	for (p = 0; p < FE_PORTS; p++) {
		if (!l_go_pending[p])
			continue;
		if (!l_aborted && hw[p].fe.state == FE_HOLD)
			hw[p].fe.state = FE_COUNT;
		l_go_pending[p] = false;
	}

	/* 3. Frontend offers the transfer to the backend (COUNT -> WAIT).  The
	 *    cold IOTLB miss is attributed here, where the backend actually
	 *    accepts, not on a NEXT_ID read that only reserves an ID. */
	for (p = 0; p < FE_PORTS; p++) {
		struct fe_port *fe = &hw[p].fe;

		if (fe->state != FE_COUNT || fe->stalling)
			continue;
		fe->src = hw[p].regs[IDMA_SRC_ADDR_REG_OFFSET / 8];
		fe->dst = hw[p].regs[IDMA_DST_ADDR_REG_OFFSET / 8];
		fe->nbytes = hw[p].regs[IDMA_NUM_BYTES_REG_OFFSET / 8];
		fe->busy = true;
		if ((p == 0 && ctl_stall_v) || (p == 1 && ctl_stall_a) ||
		    (ctl_late_v_done && p == 0 && fe->ctrl))
			fe->due = UINT64_MAX;
		else
			fe->due = sim_now + hw[p].latency;
		if (!hw[p].populated) {
			hw[p].miss++;
			hw[p].populated = true;
		}
		fe->issue_id_ev = fe->id_q;
		fe->issue_ev = true;
		fe->issue_count++;
		fe->state = FE_WAIT;
	}

	held = launcher_held_bits();
	active = launcher_active_bits();

	if (l_state == L_PREP || l_state == L_RUNNING) {
		l_seen_held |= (u8)held;
		l_ticks++;
	}

	/* 4. Abort drain: cancel held, never-issued lanes.  The ID is retired
	 *    into the frontend DONE register with no obs_issue/obs_done and
	 *    therefore no copy. */
	if ((l_state == L_PREP || l_state == L_RUNNING) && l_aborted) {
		for (p = 0; p < FE_PORTS; p++) {
			unsigned bit = 1u << p;
			u64 retired;

			if (!(l_expected_mask & bit) || !(l_hold_mask & bit) ||
			    !(held & bit) || (l_issued_mask & bit) ||
			    (l_cancelled_mask & bit))
				continue;
			retired = hw[p].fe.id_q;
			hw[p].fe.state = FE_IDLE;
			hw[p].fe.ctrl = false;
			hw[p].fe.id_q = 0;
			hw[p].fe.last_done_id = retired;
			l_cancelled_mask |= (u8)bit;
			l_hold_mask &= (u8)~bit;
			l_cancel_pulses++;
		}
	}

	/* 5. Actual backend events seen by the launcher. */
	if (l_state == L_PREP || l_state == L_RUNNING) {
		for (p = 0; p < FE_PORTS; p++) {
			unsigned bit = 1u << p;
			bool iss = hw[p].fe.issue_ev;
			bool don = hw[p].fe.done_ev;

			if (iss && !(l_expected_mask & bit))
				l_error |= 1u << LERR_UNEXP_ISSUE;
			if (don && !(l_expected_mask & bit))
				l_error |= 1u << LERR_UNEXP_DONE;

			if (iss && (l_expected_mask & bit)) {
				if (l_state == L_PREP)
					l_error |= 1u << LERR_UNEXP_ISSUE;
				if (l_issued_mask & bit)
					l_error |= 1u << LERR_EXTRA_ISSUE;
				if (!hw[p].fe.issue_id_ev)
					l_error |= 1u << LERR_ISSUE_ID;
				if (!l_issue_id_valid[p]) {
					l_issue_id[p] = hw[p].fe.issue_id_ev;
					l_issue_id_valid[p] = true;
				}
				l_issued_mask |= (u8)bit;
				l_issue_count[p]++;
				if (!l_issue_ts_valid[p]) {
					l_issue_ts[p] = l_ticks;
					l_issue_ts_valid[p] = true;
				}
			}
			if (don && (l_expected_mask & bit)) {
				if (l_state == L_PREP)
					l_error |= 1u << LERR_UNEXP_DONE;
				if (!(l_issued_mask & bit) && !iss)
					l_error |= 1u << LERR_DONE_NO_ISSUE;
				if (l_done_mask & bit)
					l_error |= 1u << LERR_EXTRA_DONE;
				if (l_issue_id_valid[p] &&
				    hw[p].fe.done_id_ev != l_issue_id[p])
					l_error |= 1u << LERR_DONE_ID;
				l_done_mask |= (u8)bit;
				l_done_count[p]++;
				if (!l_done_id_valid[p]) {
					l_done_id[p] = hw[p].fe.done_id_ev;
					l_done_id_valid[p] = true;
				}
				if (!l_done_ts_valid[p]) {
					l_done_ts[p] = l_ticks;
					l_done_ts_valid[p] = true;
				}
			}
		}
	}

	/* 6. A select tap releases V before A completes. */
	if (!l_aborted && l_state == L_RUNNING && l_has_a && !l_a_select_seen &&
	    !l_v_go_sent &&
	    (hw[1].fe.issue_ev || (l_issued_mask & 2))) {
		l_a_select_seen = true;
		l_selected_id = IDMA_CONTROLLED_A_SELECT_ID;
		l_selected_did = IDMA_CONTROLLED_A_SELECT_DID;
		l_selected_iova = hw[1].fe.src;
		l_selected_write = false;
		if (!l_a_select_ts_valid) {
			l_a_select_ts = l_ticks;
			l_a_select_ts_valid = true;
		}
		l_go_pending[0] = true;
		l_v_go_sent = true;
		if (!l_go_v_ts_valid) {
			l_go_v_ts = l_ticks + 1;
			l_go_v_ts_valid = true;
		}
	}

	/* 7. Finish conditions, only after the drain. */
	exp = l_expected_mask;
	iss = l_issued_mask;
	canc = l_cancelled_mask;
	seen = l_seen_held;
	if (l_state == L_RUNNING && !l_aborted &&
	    iss == exp && l_done_mask == exp &&
	    !(held & exp) && !(active & exp)) {
		l_state = L_FROZEN;
		l_hold_mask = 0;
		l_freezes++;
		if (!hw[0].fe.done_seen_by_sw)
			l_froze_before_v_done_read = true;
	}
	if (l_state == L_RUNNING && l_aborted &&
	    !(held & exp) && !(active & exp) &&
	    !(iss & ~l_done_mask) && !(exp & ~iss & ~canc & seen)) {
		l_state = L_FROZEN;
		l_hold_mask = 0;
		l_freezes++;
		if (!hw[0].fe.done_seen_by_sw)
			l_froze_before_v_done_read = true;
	}
}

static void advance(u64 delta)
{
	while (delta >= SIM_STEP_NS) {
		sim_now += SIM_STEP_NS;
		hw_step();
		delta -= SIM_STEP_NS;
	}
}

/* Launcher command register (b8). */
static void launcher_command(u64 value)
{
	unsigned held = launcher_held_bits();
	unsigned active = launcher_active_bits();
	int p;

	l_ctrl_writes++;

	if ((value & ~IDMA_LAUNCHER_PREP_HAS_A) == IDMA_LAUNCHER_PREP) {
		if ((l_state == L_OFF || l_state == L_FROZEN) && !held && !active) {
			l_state = L_PREP;
			l_aborted = false;
			l_error = 0;
			l_has_a = (value & IDMA_LAUNCHER_PREP_HAS_A) != 0;
			l_expected_mask = (u8)(1 | (l_has_a ? 2 : 0));
			l_hold_mask = l_expected_mask;
			l_issued_mask = l_done_mask = 0;
			l_cancelled_mask = l_seen_held = 0;
			l_ticks = 0;
			l_go_a_ts = l_a_select_ts = l_go_v_ts = 0;
			l_go_a_ts_valid = l_a_select_ts_valid = false;
			l_go_v_ts_valid = false;
			l_selected_id = l_selected_did = l_selected_iova = 0;
			l_selected_write = false;
			l_go_pending[0] = l_go_pending[1] = false;
			l_a_select_seen = false;
			l_v_go_sent = false;
			l_prep_writes++;
			/* A new PREP owns a fresh job: no stale race fault or
			 * stale controlled-V identity survives it. */
			v_late_pending = false;
			v_controlled_id = 0;
			v_controlled_done_id = 0;
			for (p = 0; p < FE_PORTS; p++) {
				l_issue_id[p] = l_done_id[p] = 0;
				l_issue_id_valid[p] = l_done_id_valid[p] = false;
				l_issue_count[p] = l_done_count[p] = 0;
				l_issue_ts[p] = l_done_ts[p] = 0;
				l_issue_ts_valid[p] = l_done_ts_valid[p] = false;
				hw[p].fe.done_seen_by_sw = false;
			}
		} else {
			l_error |= 1u << LERR_BAD_PREP;
		}
	} else if (value == IDMA_LAUNCHER_GO) {
		if (l_state == L_PREP && !l_error &&
		    (held & l_expected_mask) == l_expected_mask) {
			l_state = L_RUNNING;
			if (l_has_a) {
				l_go_pending[1] = true;
				if (!l_go_a_ts_valid) {
					l_go_a_ts = l_ticks + 1;
					l_go_a_ts_valid = true;
				}
			} else {
				l_go_pending[0] = true;
				if (!l_go_v_ts_valid) {
					l_go_v_ts = l_ticks + 1;
					l_go_v_ts_valid = true;
				}
			}
			l_go_count++;
			if (ctl_go_error)
				l_error |= 1u << LERR_BAD_GO;
		} else {
			l_error |= 1u << LERR_BAD_GO;
		}
	} else if (value == IDMA_LAUNCHER_ABORT) {
		l_abort_count++;
		if (l_state == L_OFF) {
			l_state = L_FROZEN;
			l_aborted = true;
			l_hold_mask = 0;
		} else if (l_state == L_PREP || l_state == L_RUNNING) {
			/* RTL: ABORT from PREP/RUNNING sets aborted_q and moves
			 * state to RUNNING (dma_controlled_launcher.sv). */
			l_aborted = true;
			l_state = L_RUNNING;
		} else {
			l_error |= 1u << LERR_ABORT;
		}
	} else if (value == IDMA_LAUNCHER_CLEAR) {
		if (l_state == L_OFF || l_state == L_FROZEN) {
			launcher_reset();
		} else {
			l_error |= 1u << LERR_BAD_CMD;
		}
	} else {
		l_error |= 1u << LERR_BAD_CMD;
	}
}

/* c8 bank of dma_controlled_launcher.sv. */
static u64 launcher_word(unsigned int idx)
{
	switch (idx) {
	case 0:
		return IDMA_LAUNCHER_MAGIC;
	case 1:
		return (u64)l_error;
	case 2:
		return launcher_status();
	case 3:
		return l_ticks;
	case 4:
		return l_go_a_ts_valid ? l_go_a_ts : 0;
	case 5:
		return l_a_select_ts_valid ? l_a_select_ts : 0;
	case 6:
		return l_go_v_ts_valid ? l_go_v_ts : 0;
	case 7:
		return l_issue_ts_valid[0] ? l_issue_ts[0] : 0;
	case 8:
		return l_issue_ts_valid[1] ? l_issue_ts[1] : 0;
	case 9:
		return l_done_ts_valid[0] ? l_done_ts[0] : 0;
	case 10:
		return l_done_ts_valid[1] ? l_done_ts[1] : 0;
	case 11:
		return l_issue_id_valid[0] ? l_issue_id[0] : 0;
	case 12:
		return l_issue_id_valid[1] ? l_issue_id[1] : 0;
	case 13:
		if (ctl_bad_id)
			return l_done_id[0] + 0x1000;
		return l_done_id_valid[0] ? l_done_id[0] : 0;
	case 14:
		return l_done_id_valid[1] ? l_done_id[1] : 0;
	case 15:
		return ctl_extra_mask ? (u64)(l_issued_mask | 4) :
					(u64)l_issued_mask;
	case 16:
		return ctl_extra_mask ? (u64)(l_done_mask | 4) :
					(u64)l_done_mask;
	case 17:
		return (u64)l_cancelled_mask;
	case 18:
		return ctl_extra_mask ? (u64)(l_seen_held | 4) :
					(u64)l_seen_held;
	case 19:
		return (u64)l_expected_mask;
	case 20:
		return l_selected_id;
	case 21:
		return l_selected_did;
	case 22:
		return l_selected_iova;
	case 23:
		return l_selected_write ? 1 : 0;
	case 24:
		return l_issue_count[0];
	case 25:
		return l_issue_count[1];
	case 26:
		return ctl_bad_count ? l_done_count[0] + 1 : l_done_count[0];
	case 27:
		return l_done_count[1];
	case 28:
		return l_has_a ? 1 : 0;
	case 29:
		return (u64)launcher_held_bits();
	case 30:
		return (u64)launcher_active_bits();
	default:
		return 0;
	}
}

/* --------------------------------------------------------------------- */
/* Host stubs                                                            */
/* --------------------------------------------------------------------- */

static struct page *idma_buffer_page(void *ptr)
{
	return (struct page *)((uintptr_t)ptr & ~(uintptr_t)(PAGE_SIZE - 1));
}

static phys_addr_t page_to_phys(struct page *page)
{
	return (uintptr_t)page;
}

static phys_addr_t iommu_iova_to_phys(struct iommu_domain *domain,
				      unsigned long iova)
{
	(void)domain;
	return iova + PAGE_SIZE;
}

static struct fake_dma *mmio(void *address, size_t *offset)
{
	int i;

	for (i = 0; i < FE_PORTS; i++) {
		uintptr_t p = (uintptr_t)address;
		uintptr_t base = (uintptr_t)hw[i].regs;

		if (p >= base && p < base + sizeof(hw[i].regs)) {
			*offset = p - base;
			return &hw[i];
		}
	}
	assert(!"invalid MMIO address");
	return NULL;
}

static u64 obs_data(u64 index)
{
	return index == 0 ? OBS_INDEX0_VALUE : 0x1000ULL + index;
}

static void writeq(u64 value, void *address)
{
	size_t offset;
	struct fake_dma *h = mmio(address, &offset);

	mmio_writes++;

	if (offset >= IDMA_LAUNCHER_MAGIC_OFFSET) {
		if (offset == IDMA_LAUNCHER_CONTROL_OFFSET) {
			launcher_command(value);
		} else if (offset == IDMA_LAUNCHER_INDEX_OFFSET) {
			if (value < 32)
				l_index = (u8)value;
			else
				l_error |= 1u << LERR_BAD_INDEX;
		} else if (offset == IDMA_LAUNCHER_MAGIC_OFFSET) {
			l_error |= 1u << LERR_BAD_B0;
		}
		return;
	}
	if (offset >= IDMA_BATCH_MAGIC_OFFSET) {
		batch_writes++;
		h->regs[offset / 8] = value;
		return;
	}
	if (offset >= IDMA_OBS_MAGIC_OFFSET) {
		obs_mmio++;
		if (offset == IDMA_OBS_CONTROL_OFFSET) {
			if (value == 4) {
				h->regs[offset / 8] = 0;	/* clear */
			} else if (value == 2) {
				h->regs[offset / 8] = 4;	/* freeze */
			} else {
				assert(h->regs[IDMA_OBS_TARGET_OFFSET / 8] == 1);
				assert((value & BIT_ULL(29)) != 0);
				h->regs[offset / 8] = 1;	/* armed */
				obs_arms++;
			}
		} else {
			h->regs[offset / 8] = value;
		}
		return;
	}
	h->regs[offset / 8] = value;
}

static u64 readq(void *address)
{
	size_t offset;
	struct fake_dma *h = mmio(address, &offset);
	int p = (h == &hw[1]) ? 1 : 0;

	advance(SIM_STEP_NS);

	if (offset >= IDMA_LAUNCHER_MAGIC_OFFSET) {
		if (offset == IDMA_LAUNCHER_MAGIC_OFFSET)
			return IDMA_LAUNCHER_MAGIC;
		if (offset == IDMA_LAUNCHER_CONTROL_OFFSET) {
			/* Fault: the controlled V completes exactly between the
			 * driver's controlled V DONE poll and this status read.
			 * Fire once, then disarm. */
			if (v_late_pending && hw[0].fe.state == FE_WAIT &&
			    hw[0].fe.busy) {
				hw[0].fe.due = sim_now;
				hw_step();
				v_late_pending = false;
				ctl_late_v_done = false;
				v_late_done_hits++;
			}
			return launcher_status();
		}
		if (offset == IDMA_LAUNCHER_INDEX_OFFSET)
			return l_index;
		if (offset == IDMA_LAUNCHER_DATA_OFFSET)
			return l_index < 32 ? launcher_word(l_index) : 0;
		return 0;
	}
	if (offset >= IDMA_BATCH_MAGIC_OFFSET) {
		if (offset == IDMA_BATCH_MAGIC_OFFSET)
			return IDMA_BATCH_MAGIC;
		return h->regs[offset / 8];
	}
	if (offset >= IDMA_OBS_MAGIC_OFFSET) {
		obs_mmio++;
		if (offset == IDMA_OBS_MAGIC_OFFSET)
			return controlled_v7 ? IDMA_OBS_MAGIC_V7 : IDMA_OBS_MAGIC_V6;
		if (offset == IDMA_OBS_DATA_OFFSET)
			return obs_data(h->regs[IDMA_OBS_INDEX_OFFSET / 8]);
		return h->regs[offset / 8];
	}

	if (offset == IDMA_NEXT_ID_REG_OFFSET) {
		bool hold = ((l_hold_mask >> p) & 1) &&
			    (l_state == L_PREP || l_state == L_RUNNING);
		u64 id;

		h->fe.next_id_reads++;
		if (hold && h->fe.state == FE_IDLE) {
			/* HOLD: reserve an ID; no backend traffic until GO. */
			id = h->fe.id_gen++;
			if ((ctl_zero_id_v && p == 0) || (ctl_zero_id_a && p == 1))
				id = 0;
			h->fe.id_q = id;
			h->fe.ctrl = true;
			h->fe.state = FE_HOLD;
			if (p == 0)
				v_controlled_id = id;
			return id;
		}
		if (h->fe.state == FE_IDLE &&
		    h->regs[IDMA_NUM_BYTES_REG_OFFSET / 8] != 0) {
			/* Ordinary register-file issue (priming). */
			id = h->fe.id_gen++;
			h->fe.id_q = id;
			h->fe.ctrl = false;
			h->fe.state = FE_COUNT;
			return id;
		}
		return 0;
	}
	if (offset == IDMA_DONE_REG_OFFSET) {
		h->fe.done_reads++;
		/* Arm the race fault only for a controlled V that is still in
		 * WAIT and whose DONE read does not yet match its reservation. */
		if (p == 0 && ctl_late_v_done && h->fe.ctrl &&
		    h->fe.state == FE_WAIT &&
		    h->fe.last_done_id != v_controlled_id)
			v_late_pending = true;
		/* Only an actually retired controlled V completion counts; an
		 * old priming completion must not be mistaken for it. */
		if (p == 0 && h->fe.last_done_id &&
		    h->fe.last_done_id == v_controlled_done_id)
			h->fe.done_seen_by_sw = true;
		return h->fe.last_done_id;
	}
	if (offset == IDMA_ACTIVE_CYCLES_REG_OFFSET)
		return sim_now;
	return h->regs[offset / 8];
}

/*
 * Bounded, kernel-shaped replacement for readq_poll_timeout().  Re-evaluates
 * the condition on every read so idma_drain()/cleanup behave as on the target.
 */
#define readq_poll_timeout(addr, val, cond, delay_us, timeout_us)		\
	({								\
		u64 __poll_start = sim_now;				\
		int __poll_ret = 0;					\
		(void)(delay_us);					\
		for (;;) {						\
			(val) = readq(addr);				\
			if (cond)					\
				break;					\
			if (sim_now - __poll_start >=			\
			    (u64)(timeout_us) * NSEC_PER_USEC) {		\
				__poll_ret = -ETIMEDOUT;		\
				break;					\
			}						\
		}							\
		__poll_ret;						\
	})

static u64 ktime_get_ns(void)
{
	advance(SIM_STEP_NS);
	return sim_now;
}

static bool signal_pending(void *task)
{
	(void)task;
	return false;
}

static void cpu_relax(void)
{
	advance(SIM_STEP_NS);
}

static void local_flush_icache_all(void)
{
	assert(!hw[0].fe.busy && !hw[1].fe.busy);
}

static void dev_err(struct device *dev, const char *fmt, ...)
{
	(void)dev;
	(void)fmt;
}

static void dev_crit(struct device *dev, const char *fmt, ...)
{
	(void)dev;
	(void)fmt;
}

static int copy_from_user(void *dst, const void *src, size_t n)
{
	memcpy(dst, src, n);
	return 0;
}

static int copy_to_user(void *dst, const void *src, size_t n)
{
	memcpy(dst, src, n);
	return 0;
}

static int mutex_lock_interruptible(struct mutex *m)
{
	(void)m;
	return 0;
}

static void mutex_unlock(struct mutex *m)
{
	(void)m;
}

static const void *memchr_inv(const void *ptr, int c, size_t n)
{
	const unsigned char *p = ptr;

	while (n--) {
		if (*p != (unsigned char)c)
			return p;
		p++;
	}
	return NULL;
}

static bool idma_any_quarantined(void)
{
	return idmas[0].quarantined || idmas[1].quarantined;
}

static struct idma_device *idma_get_instance(u32 id)
{
	return id < 2 ? &idmas[id] : NULL;
}

static void idma_put_instance(struct idma_device *d)
{
	(void)d;
}

static void idma_iommu_unmap_range(struct iommu_domain *domain,
				   unsigned long iova, size_t size,
				   size_t page)
{
	int i, owner = -1;

	(void)domain;
	(void)size;
	(void)page;
	for (i = 0; i < FE_PORTS; i++) {
		if (lane_iova[i][0] == (uintptr_t)iova ||
		    lane_iova[i][1] == (uintptr_t)iova) {
			owner = i;
			break;
		}
	}
	/* Never unmap a range this harness did not map, and never release a
	 * lane that is not provably idle with no backend copy in flight. */
	assert(owner >= 0);
	assert(hw[owner].fe.state == FE_IDLE && !hw[owner].fe.busy);
	unmaps++;
}

static void dma_free_attrs(struct device *d, size_t n, void *ptr,
			   dma_addr_t dma, unsigned attrs)
{
	(void)n;
	(void)dma;
	(void)attrs;
	/* Storage handed to a backend that may still touch it is never freed. */
	assert(hw[d->index].fe.state == FE_IDLE && !hw[d->index].fe.busy);
	frees++;
	free(ptr);
}

/* Declared early: the stream stub needs the production pattern function. */
static u64 idma_pattern(struct idma_stream *s, size_t word);

static int idma_stream_prepare(struct idma_stream *s, struct idma_device *idma,
			       u32 pages, u32 chunk,
			       struct idma_experiment_stats *stats)
{
	size_t i;
	size_t size = (size_t)pages * PAGE_SIZE;

	memset(s, 0, sizeof(*s));
	s->idma = idma;
	s->dev = idma->miscdev.parent;
	s->stats = stats;
	s->domain = &dummy_domain;
	s->size = size;
	s->chunk = chunk;
	/* Page/64B aligned fixture, matching the PAGE_SIZE mapping granularity. */
	s->src = aligned_alloc(PAGE_SIZE, size);
	s->dst = aligned_alloc(PAGE_SIZE, size);
	assert(s->src && s->dst);
	allocations += 2;
	s->src_iova = (uintptr_t)s->src;
	s->dst_iova = (uintptr_t)s->dst;
	s->mapped_src = s->mapped_dst = true;
	lane_iova[idma->id][0] = s->src_iova;
	lane_iova[idma->id][1] = s->dst_iova;
	for (i = 0; i < size / sizeof(u64); i++) {
		((u64 *)s->src)[i] = idma_pattern(s, i);
		((u64 *)s->dst)[i] = ~idma_pattern(s, i);
	}
	return 0;
}

static int riscv_iommu_hpm_acquire(struct device *v, struct device *a,
				   struct riscv_iommu_hpm_session **s)
{
	(void)v;
	(void)a;
	hpm_acquires++;
	*s = &session;
	return 0;
}

static void riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *s,
				 struct riscv_iommu_hpm_sample *sample)
{
	int i;

	(void)s;
	memset(sample, 0, sizeof(*sample));
	sample->cycles = sim_now;
	for (i = 0; i < 2; i++) {
		sample->iotlb_miss[i] = hw[i].miss;
		sample->s1_ptw[i] = hw[i].miss * 3;
		sample->s2_ptw[i] = 0;
	}
	hpm_reads++;
	/* The after-drain snapshot must reflect the current launcher terminal
	 * state, not a freeze count accumulated across rounds. */
	hpm_last_read_after_terminal = (l_state == L_FROZEN);
	advance(SIM_STEP_NS);
}

static void riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *s)
{
	assert(s == &session);
	hpm_releases++;
}

static int riscv_iommu_hpm_flush_s1(struct riscv_iommu_hpm_session *s)
{
	assert(s == &session);
	assert(!hw[0].fe.busy && !hw[1].fe.busy);
	hw[0].populated = false;
	hw[1].populated = false;
	return 0;
}

static int idma_observation_finish(struct idma_observation *o,
				   const struct idma_experiment *e, int run_error)
{
	(void)e;
	if (!o->control || !o->armed)
		return 0;
	obs_finishes++;
	writeq(2, o->control->reg + IDMA_OBS_CONTROL_OFFSET);
	if (run_error)
		return -EIO;
	/*
	 * Host mock of the observer finisher.  It cannot validate the frozen
	 * record, but a "successful" round must at least have observed the
	 * controlled V completion through the DONE register; otherwise the
	 * success would be fabricated.
	 */
	if (!hw[0].fe.done_seen_by_sw)
		return -EIO;
	return 0;
}

static int idma_e3_finish(struct idma_observation *o,
			  const struct idma_experiment *e, u64 *words)
{
	(void)e;
	if (!o->armed)
		return -EIO;
	/*
	 * Host mock: returns a zeroed E3 bank.  This only proves the driver
	 * reached the finisher; it is not evidence that a real observer
	 * emitted a valid frozen record.
	 */
	memset(words, 0, IDMA_E3_WORDS * sizeof(u64));
	return 0;
}

/* PRODUCTION_FUNCTIONS */

static struct idma_experiment controlled_input(void)
{
	struct idma_experiment e = {
		.abi_version = IDMA_EXPERIMENT_ABI,
		.struct_size = sizeof(e),
		.aggressor_id = 1,
		.victim_pages = 1,
		.aggressor_pages = 1,
		.chunk_bytes = 64,
		.run_timeout_ms = 100,
		.victim_transfers = 1,
	};
	int i;

	for (i = 0; i < 2; i++) {
		idmas[i].id = i;
		idmas[i].did = 10 + i;
		idmas[i].reg = hw[i].regs;
		idmas[i].miscdev.parent = &devices[i];
		idmas[i].quarantined = false;
		hw[i].regs[IDMA_BATCH_MAGIC_OFFSET / 8] = IDMA_BATCH_MAGIC;
		hw[i].regs[IDMA_LAUNCHER_MAGIC_OFFSET / 8] = IDMA_LAUNCHER_MAGIC;
		hw[i].fe.state = FE_IDLE;
		hw[i].fe.id_gen = ID_GEN_FIRST;
		hw[i].latency = 20000;
	}
	hw[0].regs[IDMA_OBS_MAGIC_OFFSET / 8] = IDMA_OBS_MAGIC_V6;
	launcher_reset();
	return e;
}

static void pass(const char *name)
{
	printf("PASS %s\n", name);
}

int main(int argc, char **argv)
{
	struct idma_e3_controlled c;
	struct idma_experiment e;
	const char *name;
	long ret;

	assert(argc == 2);
	name = argv[1];
	e = controlled_input();
	memset(&c, 0, sizeof(c));
	c.pair.abi_version = 1;
	c.pair.struct_size = sizeof(c.pair);
	c.pair.request = e;

	if (!strcmp(name, "invalid_before_mmio")) {
		c.pair.abi_version = 0;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -EINVAL);
		assert(c.pair.status == -EINVAL);
		assert(c.pair.rounds_attempted == 0);
		assert(!allocations && !mmio_writes && !l_ctrl_writes && !obs_mmio);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "cold_solo_ok")) {
		c.pair.request.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
		c.prep_mode = 0;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == 0);
		assert(c.pair.status == 0);
		assert(c.pair.rounds_attempted == 2);
		assert(c.pair.rounds_completed == 2);
		assert(c.pair.result[0].status == 0);
		assert(c.pair.result[1].status == 0);
		/* No A lane exists: no A MMIO at all. */
		assert(hw[1].fe.next_id_reads == 0);
		assert(hw[1].miss == 0);
		/* Three V tasks per round: prime, probe, formal. */
		assert(hw[0].fe.next_id_reads == 6);
		assert(c.controlled[0].prime[0].iotlb_miss == 1);
		assert(c.controlled[0].prime[0].submitted == 1);
		assert(c.controlled[0].prime[0].completed == 1);
		assert(c.controlled[0].prime[1].iotlb_miss == 0);
		assert(c.controlled[0].prime[1].submitted == 0);
		assert(c.controlled[0].probe.iotlb_miss == 0);
		assert(c.controlled[0].probe.s1_ptw == 0);
		assert(c.controlled[0].probe.completed == 1);
		assert(c.controlled[0].v_hw_cycles == OBS_INDEX0_VALUE);
		/* Launcher record, round 1: V formal reserved ID 4. */
		assert(c.controlled[0].launcher[0] == IDMA_LAUNCHER_MAGIC);
		assert(c.controlled[0].launcher[11] == 4);
		assert(c.controlled[0].launcher[13] == 4);
		assert(c.controlled[0].launcher[15] == 1);
		assert(c.controlled[0].launcher[16] == 1);
		assert(c.controlled[0].launcher[17] == 0);
		assert(c.controlled[0].launcher[18] == 1);
		assert(c.controlled[0].launcher[19] == 1);
		assert(c.controlled[0].launcher[24] == 1);
		assert(c.controlled[0].launcher[26] == 1);
		assert(c.controlled[0].launcher[28] == 0);
		assert(c.controlled[0].launcher[29] == 0);
		assert(c.controlled[0].launcher[30] == 0);
		assert(c.pair.result[0].victim.submitted == 1);
		assert(c.pair.result[0].victim.completed == 1);
		assert(c.pair.result[0].victim.completed_bytes == 64);
		assert(c.pair.result[0].victim.data_errors == 0);
		assert(c.pair.result[0].measurement_started == 1);
		assert(c.pair.result[0].measurement_complete == 1);
		assert(c.pair.result[0].victim_elapsed_ns > 0);
		assert(allocations == 2 && frees == 2 && unmaps == 2);
		assert(!idmas[0].quarantined && !idmas[1].quarantined);
		assert(hpm_acquires == 2 && hpm_releases == 2);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "v7_controlled_ok")) {
		c.pair.request.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
		c.prep_mode = 0;
		controlled_v7 = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == 0);
		assert(c.pair.status == 0);
		assert(c.pair.rounds_attempted == 2);
		assert(c.pair.rounds_completed == 2);
		assert(allocations == 2 && frees == 2 && unmaps == 2);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "hot_pair_lane")) {
		c.prep_mode = 1;
		c.pair_mode = 1;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == 0);
		/* V: prime + probe + formal per round. A: prime + formal. */
		assert(hw[0].fe.next_id_reads == 6);
		assert(hw[1].fe.next_id_reads == 4);
		/* Frontend issue counters are TOTAL, including priming. */
		assert(hw[0].fe.issue_count == 6);
		assert(hw[1].fe.issue_count == 4);
		/* HPM lanes are per device. */
		assert(c.controlled[0].prime[1].iotlb_miss == 1);
		assert(c.controlled[0].prime[1].completed == 1);
		assert(c.controlled[0].prime[0].iotlb_miss == 1);
		assert(c.controlled[0].probe.iotlb_miss == 0);
		assert(c.controlled[1].prime[1].iotlb_miss == 1);
		assert(c.controlled[1].prime[0].iotlb_miss == 1);
		assert(c.controlled[1].probe.iotlb_miss == 0);
		/* A released by the select tap, never by A completion. */
		assert(c.controlled[0].launcher[28] == 1);
		assert(c.controlled[0].launcher[20] == IDMA_CONTROLLED_A_SELECT_ID);
		assert(c.controlled[0].launcher[21] == IDMA_CONTROLLED_A_SELECT_DID);
		assert(c.controlled[0].launcher[22] == c.pair.mapping[4]);
		assert(c.controlled[0].launcher[5] < c.controlled[0].launcher[6]);
		assert(c.controlled[0].launcher[8] <= c.controlled[0].launcher[5]);
		assert(c.controlled[0].launcher[10] >= c.controlled[0].launcher[8]);
		/* IDs advance across rounds, per frontend, never reused. */
		assert(c.controlled[0].launcher[11] == 4);
		assert(c.controlled[1].launcher[11] == 7);
		assert(c.controlled[0].launcher[12] == 3);
		assert(c.controlled[1].launcher[12] == 5);
		assert(c.pair.result[0].victim.submitted == 1);
		assert(c.pair.result[0].aggressor.submitted == 1);
		assert(c.pair.result[0].aggressor.completed == 1);
		assert(c.pair.result[1].measurement_complete == 1);
		assert(allocations == 4 && frees == 4 && unmaps == 4);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "submit_counts")) {
		/* Cold pair: A is only ever submitted by the formal phase. */
		c.prep_mode = 0;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == 0);
		assert(hw[0].fe.next_id_reads == 6);
		assert(hw[1].fe.next_id_reads == 2);
		/* Frontend issue counter is TOTAL, including the two V primes. */
		assert(hw[0].fe.issue_count == 6);
		assert(hw[1].fe.issue_count == 2);
		assert(c.controlled[0].prime[1].submitted == 0);
		assert(c.controlled[1].prime[1].submitted == 0);
		assert(c.pair.result[0].aggressor.submitted == 1);
		assert(c.pair.result[1].aggressor.submitted == 1);
		assert(c.controlled[1].launcher[12] == c.controlled[0].launcher[12] + 1);
		assert(c.controlled[1].launcher[11] == c.controlled[0].launcher[11] + 3);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "two_round_reset")) {
		c.pair.first_delay = 128;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == 0);
		assert(c.pair.rounds_attempted == 2);
		assert(c.pair.rounds_completed == 2);
		assert(c.pair.result[1].hpm_configured == 1);
		assert(c.pair.result[1].measurement_complete == 1);
		assert(c.controlled[0].flush_status == 0);
		assert(c.controlled[1].flush_status == 0);
		assert(c.controlled[1].stage == IDMA_CONTROLLED_STAGE_COMPLETE);
		/* Round 2 must not inherit round 1 launcher state.  The default
		 * request keeps the A lane, so both lanes issue and complete. */
		assert(c.controlled[1].launcher[15] == 3);
		assert(c.controlled[1].launcher[16] == 3);
		assert(c.controlled[1].launcher[17] == 0);
		assert(c.controlled[1].launcher[18] == 3);
		assert(c.controlled[1].launcher[11] == c.controlled[0].launcher[11] + 3);
		assert(c.controlled[1].launcher[12] == c.controlled[0].launcher[12] + 1);
		assert(c.controlled[1].prime[0].iotlb_miss == 1);
		assert(c.controlled[1].probe.iotlb_miss == 0);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "v_done_late_observer")) {
		/* V completes between the driver's V DONE poll and the launcher
		 * status read: the launcher may be FROZEN before the software
		 * has seen V's completion, and the driver must keep polling
		 * until it observes it. */
		c.pair.request.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
		ctl_late_v_done = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == 0);
		assert(v_late_done_hits == 1);
		assert(l_froze_before_v_done_read);
		assert(hw[0].fe.done_reads >= 2);
		assert(c.pair.result[0].status == 0);
		assert(c.pair.result[0].measurement_complete == 1);
		assert(c.pair.result[0].measurement_started == 1);
		assert(c.pair.result[0].victim.data_errors == 0);
		assert(c.pair.result[0].victim_elapsed_ns > 0);
		assert(c.pair.result[0].victim.quarantined == 0);
		assert(c.controlled[0].launcher[9] != 0);
		assert(c.controlled[0].launcher[16] == 1);
		assert(allocations == 2 && frees == 2 && unmaps == 2);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "no_terminal_quarantine")) {
		/* Real backend completion the launcher never observes: no
		 * fabricated terminal state, bounded timeout, raw record kept. */
		c.pair.request.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
		ctl_drop_v_done = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -ETIMEDOUT);
		assert(c.pair.status == -ETIMEDOUT);
		assert(c.pair.result[0].status == -ETIMEDOUT);
		assert(!c.pair.result[0].measurement_complete);
		assert(idmas[0].quarantined);
		assert(allocations == 2 && frees == 0 && unmaps == 0);
		assert(l_abort_count == 1);
		assert(hpm_acquires == 1 && hpm_releases == 1);
		assert(c.controlled[0].launcher[0] == IDMA_LAUNCHER_MAGIC);
		assert(c.controlled[0].launcher[15] == 1);
		assert(c.controlled[0].launcher[16] == 0);
		assert(c.controlled[0].launcher[3] != 0);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "first_error_preserved")) {
		/* Cleanup would also fail account here; the first error must win. */
		c.pair.request.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
		ctl_drop_v_done = true;
		ctl_bad_id = true;
		ctl_bad_count = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -ETIMEDOUT);
		assert(c.pair.status == -ETIMEDOUT);
		assert(c.pair.result[0].status == -ETIMEDOUT);
		assert(idmas[0].quarantined);
		assert(allocations == 2 && frees == 0 && unmaps == 0);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "a_never_accepts_quarantine")) {
		/* A offered to a backend that never accepts (COUNT, ready=0):
		 * COUNT cannot be cancelled, so the abort drain never reaches
		 * terminal and both lanes are quarantined. */
		ctl_no_ready_a = true;
		hw[1].fe.stalling = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -ETIMEDOUT);
		assert(idmas[0].quarantined && idmas[1].quarantined);
		assert(allocations == 4 && frees == 0 && unmaps == 0);
		assert(c.controlled[0].launcher[0] == IDMA_LAUNCHER_MAGIC);
		assert(c.controlled[0].launcher[15] == 0);	/* nothing issued */
		assert(c.controlled[0].launcher[16] == 0);
		assert(c.controlled[0].launcher[17] == 1);	/* V cancelled */
		assert(c.controlled[0].launcher[18] == 3);	/* both seen held */
		/* COUNT stays offered, never accepted, never cancelled. */
		assert(hw[1].fe.state == FE_COUNT);
		assert(!hw[1].fe.busy);
		assert(hw[1].fe.issue_count == 0);
		/* The two V priming completions copied; the cancelled formal V
		 * did not. */
		assert(hw[0].fe.done_count == 2);
		assert(l_cancel_pulses == 1);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "a_never_completes_quarantine")) {
		/* A issued but never completes: A's buffers must never be
		 * unmapped, and the drain must not fake A's completion. */
		ctl_stall_a = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -ETIMEDOUT);
		assert(idmas[0].quarantined && idmas[1].quarantined);
		assert(allocations == 4 && frees == 0 && unmaps == 0);
		assert(hw[1].fe.busy);
		assert(c.controlled[0].launcher[0] == IDMA_LAUNCHER_MAGIC);
		assert(c.controlled[0].launcher[15] == 3);
		assert(c.controlled[0].launcher[16] == 1);
		assert(c.controlled[0].launcher[17] == 0);
		assert(c.controlled[0].launcher[22] == c.pair.mapping[4]);
		assert(c.controlled[0].launcher[3] != 0);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "prep_quarantine_visible")) {
		/* Primer failure quarantines V and is visible on its own lane
		 * statistics; the launcher is never touched. */
		c.pair.request.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
		ctl_stall_v = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -ETIMEDOUT);
		assert(c.controlled[0].prime[0].submitted == 1);
		assert(c.controlled[0].prime[0].completed == 0);
		assert(c.controlled[0].prime[0].quarantined == 1);
		assert(c.controlled[0].probe.submitted == 0);
		assert(c.controlled[0].stage == IDMA_CONTROLLED_STAGE_VPRIME);
		assert(c.controlled[0].flush_status == 0);
		assert(l_ctrl_writes == 0);
		assert(l_go_count == 0);
		assert(idmas[0].quarantined);
		assert(allocations == 2 && frees == 0 && unmaps == 0);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "partial_prepare_zero_a")) {
		ctl_zero_id_a = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -EIO);
		assert(idmas[0].quarantined && idmas[1].quarantined);
		assert(l_go_count == 0);
		assert(allocations == 4 && frees == 0 && unmaps == 0);
		assert(c.controlled[0].launcher[17] == 3);	/* both cancelled */
		assert(c.controlled[0].launcher[16] == 0);	/* nothing done */
		pass(name);
		return 0;
	}
	if (!strcmp(name, "zero_id_v")) {
		c.pair.request.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
		ctl_zero_id_v = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -EIO);
		assert(idmas[0].quarantined);
		assert(allocations == 2 && frees == 0 && unmaps == 0);
		assert(c.controlled[0].launcher[15] == 0);
		assert(c.controlled[0].launcher[17] == 1);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "bad_count_quarantine") ||
	    !strcmp(name, "bad_id_quarantine") ||
	    !strcmp(name, "bad_mask_quarantine")) {
		c.prep_mode = 1;
		c.pair_mode = 1;
		ctl_bad_count = !strcmp(name, "bad_count_quarantine");
		ctl_bad_id = !strcmp(name, "bad_id_quarantine");
		ctl_extra_mask = !strcmp(name, "bad_mask_quarantine");
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -EIO);
		assert(c.pair.status == -EIO);
		assert(idmas[0].quarantined && idmas[1].quarantined);
		assert(allocations == 4 && frees == 0 && unmaps == 0);
		assert(c.controlled[0].launcher[0] == IDMA_LAUNCHER_MAGIC);
		assert(c.controlled[0].launcher[17] == 0);
		assert(!c.pair.result[0].measurement_complete);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "go_error_hpm_after_drain")) {
		/* GO readback error aborts the round, but both lanes are still
		 * drained and the after-HPM snapshot is taken after that drain.
		 * The GO pulse was already registered when the error is read,
		 * so both lanes naturally issue before the ABORT: issued and
		 * done masks are 3 (V and A), cancelled is 0. */
		ctl_go_error = true;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -EIO);
		assert(c.pair.result[0].status == -EIO);
		assert(!c.pair.result[0].measurement_complete);
		assert(c.pair.result[0].drain_ns > 0);
		assert(c.pair.result[0].snapshot_after_ns > 0);
		assert(hpm_last_read_after_terminal);
		assert(hpm_acquires == 1 && hpm_releases == 1);
		assert(!idmas[0].quarantined && !idmas[1].quarantined);
		assert(allocations == 4 && frees == 4 && unmaps == 4);
		assert(c.controlled[0].launcher[15] == 3);	/* V + A issued */
		assert(c.controlled[0].launcher[16] == 3);	/* both completed */
		assert(c.controlled[0].launcher[17] == 0);	/* nothing cancelled */
		pass(name);
		return 0;
	}
	if (!strcmp(name, "preflight_busy_reject")) {
		/* Preflight rejects an active backend before any write. */
		hw[1].regs[IDMA_STATUS_REG_OFFSET / 8] = 1;
		ret = idma_run_e3_controlled(&idmas[0], &c);
		assert(ret == -EBUSY);
		assert(c.pair.status == -EBUSY);
		assert(!mmio_writes);
		assert(!batch_writes);
		assert(!l_ctrl_writes);
		assert(!allocations);
		assert(!l_go_count && !l_abort_count);
		assert(!c.pair.rounds_attempted);
		pass(name);
		return 0;
	}
	if (!strcmp(name, "partial_held_abort")) {
		/* Direct launcher-level contract: PREP reserves V, then ABORT
		 * arrives before GO.  The single held reservation must be
		 * cancelled and its ID retired into the frontend DONE register,
		 * with no obs_issue/obs_done and no data copied. */
		u64 id;

		launcher_command(IDMA_LAUNCHER_PREP);
		assert(l_state == L_PREP);
		id = readq(idmas[0].reg + IDMA_NEXT_ID_REG_OFFSET);
		assert(id == ID_GEN_FIRST);
		assert(hw[0].fe.state == FE_HOLD);
		assert(hw[0].fe.ctrl);
		assert(hw[0].fe.issue_count == 0);
		assert(hw[0].fe.done_count == 0);

		launcher_command(IDMA_LAUNCHER_ABORT);
		/* RTL: ABORT observed in PREP escalates the state to RUNNING. */
		assert(l_state == L_RUNNING);
		assert(l_aborted);

		advance(2 * SIM_STEP_NS);
		assert(l_state == L_FROZEN);
		assert(l_cancel_pulses == 1);
		assert(l_issued_mask == 0);
		assert(l_done_mask == 0);
		assert(l_cancelled_mask == 1);
		assert(l_seen_held == 1);
		assert(hw[0].fe.state == FE_IDLE);
		assert(!hw[0].fe.ctrl);
		assert(!hw[0].fe.busy);
		assert(hw[0].fe.issue_count == 0);	/* never reached the backend */
		assert(hw[0].fe.done_count == 0);	/* no copy */
		assert(hw[0].fe.last_done_id == id);	/* ID retired, no data */
		assert(!idmas[0].quarantined);
		pass(name);
		return 0;
	}

	assert(!"unknown test");
	return 1;
}

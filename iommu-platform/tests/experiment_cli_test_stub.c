// SPDX-License-Identifier: GPL-2.0
/* Host-only ioctl responses. No DMA, timing, or HPM behavior is simulated. */
#include <errno.h>
#include <linux/idma.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

static int mode(const char *name)
{
	const char *selected = getenv("IDMA_TEST_MODE");

	return selected && !strcmp(selected, name);
}

static void record(const char *name)
{
	const char *path = getenv("IDMA_TEST_LOG");
	FILE *f = path ? fopen(path, "a") : NULL;

	if (!f)
		abort();
	fprintf(f, "%s\n", name);
	fclose(f);
}

static void zero_corun_window(struct idma_p4m *p)
{
	unsigned int i;

	for (i = 2; i <= 12; i++)
		p->window[i] = 0;
}

static void fill_p4m(struct idma_p4m *p)
{
	int has_a = p->experiment.aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;
	unsigned int i;

	memset(p->observation, 0, sizeof(p->observation));
	memset(p->window, 0, sizeof(p->window));
	p->sequence = 1;
	p->observer_status = 4;
	p->window[0] = 0x49444d4150344d01ULL;
	p->window[1] = 1000;
	p->window[13] = p->experiment.victim_transfers;
	p->window[14] = p->experiment.victim_transfers;
	if (has_a) {
		p->window[2] = 10;      /* A issues inside the window */
		p->window[3] = 10;      /* A completions inside the window */
		p->window[4] = 1;       /* opening in-flight stock */
		p->window[5] = 1;       /* closing in-flight stock */
		p->window[6] = 1;       /* started before opening, completed inside */
		p->window[7] = 0;       /* no issue on the opening edge */
		p->window[8] = 5;       /* opening in-flight ID */
		p->window[9] = 15;      /* closing in-flight ID == last done + 1 */
		p->window[10] = 5;      /* first done ID == opening in-flight ID */
		p->window[11] = 14;     /* last done ID == first + done - 1 */
		p->window[12] = 0;
		p->window[15] = 0x7ULL; /* hasA | closed | strict pre-opening cohort */
	} else {
		p->window[15] = 0x2ULL; /* closed, no A lane */
	}
	for (i = 0; i < IDMA_P4M_OBS_WORDS; i++)
		p->observation[i] = 0x1000 + i;
	p->observation[0] = p->window[1];
	p->observation[7] = p->experiment.victim_transfers;
	p->observation[8] = p->experiment.victim_transfers;
	if (mode("p4m_bank_magic")) p->window[0]++;
	if (mode("p4m_bank_closed")) p->window[15] &= ~0x2ULL;
	if (mode("p4m_cycles")) p->window[1] = 0;
	if (mode("p4m_cycles_mismatch")) p->observation[0]++;
	if (mode("p4m_conservation")) p->window[3]++;
	if (mode("p4m_flags")) p->window[15] |= 0x8ULL;
	if (mode("p4m_ids")) p->window[8] = 0;
	if (mode("p4m_vcount")) p->window[13]--;
	if (mode("p4m_legacy_vcount")) p->observation[7]++;
	if (mode("p4m_status")) p->experiment.status = -EIO;
	if (mode("p4m_observer_status")) p->observer_status = 7;
	if (mode("p4m_sequence")) p->sequence = 0;
	if (mode("p4m_solo_nonzero") && !has_a) p->window[2] = 1;
	if (mode("p4m_nohasA") && has_a) p->window[15] &= ~0x1ULL;
	if (mode("p4m_strict") && has_a) p->window[15] &= ~0x4ULL;
	if (mode("p4m_carry") && has_a) p->window[6] = 0;
	if (mode("p4m_contiguous") && has_a) p->window[11]++;
	if (mode("p4m_endid") && has_a) p->window[9] = 16;
	if (mode("p4m_odd_cycles") && has_a) {
		p->window[1] = 3001;
		p->observation[0] = 3001;
	}
	/* Boundary fixtures: records with no completions inside the window. */
	if (mode("p4m_newtask_pending") && has_a) {
		zero_corun_window(p);
		p->window[2] = 1;       /* a single issue inside the window */
		p->window[5] = 1;       /* the new task is still in flight at close */
		p->window[9] = 7;       /* closing in-flight ID is unrelated to any open */
		p->window[15] = 0x3ULL; /* hasA | closed, no strict cohort */
	}
	if (mode("p4m_zero_progress_carry") && has_a) {
		zero_corun_window(p);
		p->window[4] = 1;       /* opening in-flight carried into the window */
		p->window[5] = 1;       /* the same task is still in flight at close */
		p->window[8] = 5;       /* opening in-flight ID */
		p->window[9] = 5;       /* closing ID matches the opening ID */
		p->window[15] = 0x7ULL; /* hasA | closed | strict cohort */
	}
	if (mode("p4m_zero_progress_drained") && has_a) {
		zero_corun_window(p);
		p->window[15] = 0x3ULL; /* hasA | closed, nothing in flight */
	}
	if (mode("p4m_nodone_first_only") && has_a) {
		zero_corun_window(p);
		p->window[10] = 5;      /* first_done set while done == 0 */
		p->window[15] = 0x3ULL;
	}
	if (mode("p4m_nodone_last_only") && has_a) {
		zero_corun_window(p);
		p->window[11] = 5;      /* last_done set while done == 0 */
		p->window[15] = 0x3ULL;
	}
	if (mode("p4m_modular_overflow") && has_a) {
		zero_corun_window(p);
		p->window[2] = ~0ULL;   /* conservation would only pass via u64 wrap */
		p->window[4] = 1;       /* opening in-flight stock */
		p->window[8] = 5;       /* opening in-flight ID */
		p->window[15] = 0x7ULL; /* hasA | closed | strict cohort */
	}
}

int ioctl(int fd, unsigned long request, ...)
{
	va_list ap;
	void *arg;
	unsigned int i;

	(void)fd;
	va_start(ap, request);
	arg = va_arg(ap, void *);
	va_end(ap);
	if (request == IDMA_IOC_EXPERIMENT_CAPS) {
		struct idma_experiment_caps *c = arg;

		record("caps");
		if (mode("caps_enotty")) {
			errno = ENOTTY;
			return -1;
		}
		memset(c, 0, sizeof(*c));
		c->abi_version = IDMA_EXPERIMENT_ABI;
		c->struct_size = sizeof(*c);
		c->capabilities = IDMA_EXPERIMENT_CAP_DID_HPM | IDMA_EXPERIMENT_CAP_SW_REFILL |
			IDMA_EXPERIMENT_CAP_HW_BATCH | IDMA_EXPERIMENT_CAP_P4M;
		c->device_id = 0;
		c->did = 10;
		c->max_pages = 4096;
		c->page_bytes = 4096;
		c->max_run_timeout_ms = 300000;
		if (mode("caps_abi")) c->abi_version++;
		if (mode("caps_size")) c->struct_size--;
		if (mode("caps_reserved")) c->reserved[2] = 1;
		if (mode("caps_page")) c->page_bytes = 8192;
		if (mode("caps_limit")) c->max_pages = 0;
		if (mode("caps_no_hpm")) c->capabilities &= ~IDMA_EXPERIMENT_CAP_DID_HPM;
		if (mode("caps_no_batch")) c->capabilities &= ~IDMA_EXPERIMENT_CAP_HW_BATCH;
		if (mode("caps_no_p4m")) c->capabilities &= ~IDMA_EXPERIMENT_CAP_P4M;
		if (mode("caps_no_refill")) c->capabilities &= ~IDMA_EXPERIMENT_CAP_SW_REFILL;
		return mode("caps_positive") ? 1 : 0;
	}
	if (request == IDMA_IOC_RUN_EXPERIMENT || request == IDMA_IOC_RUN_BATCH ||
	    request == IDMA_IOC_RUN_P4M) {
		struct idma_experiment *r;
		struct idma_p4m *p4m = NULL;

		if (request == IDMA_IOC_RUN_P4M) {
			p4m = arg;
			r = &p4m->experiment;
			record("p4m");
		} else {
			r = arg;
			record(request == IDMA_IOC_RUN_BATCH ? "batch" : "run");
		}
		if (r->abi_version != IDMA_EXPERIMENT_ABI || r->struct_size != sizeof(*r) || r->flags)
			abort();
		for (i = 0; i < sizeof(r->reserved) / sizeof(r->reserved[0]); i++)
			if (r->reserved[i]) abort();
		if (p4m) {
			for (i = 0; i < sizeof(p4m->reserved) / sizeof(p4m->reserved[0]); i++)
				if (p4m->reserved[i]) abort();
		}
		if (mode("run_enotty") || mode("run_eintr")) {
			errno = mode("run_eintr") ? EINTR : ENOTTY;
			return -1;
		}
		r->hpm_configured = 1;
		r->measurement_started = 1;
		r->measurement_complete = 1;
		r->victim_did = 10;
		r->aggressor_did = 11;
		r->victim_elapsed_ns = 1234567;
		r->snapshot_before_ns = 250;
		r->snapshot_after_ns = 300;
		r->drain_ns = 400;
		r->victim.submitted = r->victim_transfers;
		r->victim.completed = r->victim_transfers;
		r->victim.completed_bytes = r->victim_transfers * r->chunk_bytes;
		r->victim.active_cycles = 123;
		r->victim.iotlb_miss = 3;
		r->victim.s1_ptw = 4;
		r->victim.s2_ptw = 5;
		r->victim.refill_gap_ns_sum = 700;
		r->victim.refill_gap_ns_max = 70;
		r->victim.refill_gap_samples = 10;
		if (r->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR) {
			r->aggressor.submitted = 11;
			r->aggressor.completed = 11;
			r->aggressor.completed_bytes = 11 * r->chunk_bytes;
			r->aggressor.active_cycles = 456;
			r->aggressor.iotlb_miss = 13;
			r->aggressor.s1_ptw = 14;
			r->aggressor.s2_ptw = 15;
			r->aggressor.refill_gap_ns_sum = 900;
			r->aggressor.refill_gap_ns_max = 90;
			r->aggressor.refill_gap_samples = 10;
			r->aggressor.in_flight_at_start = 1;
			r->aggressor.in_flight_at_end = 1;
		}
		if (mode("run_data")) r->victim.data_errors = 1;
		if (mode("run_aggressor_data")) r->aggressor.data_errors = 1;
		if (mode("run_quarantine")) r->victim.quarantined = 1;
		if (mode("run_aggressor_quarantine")) r->aggressor.quarantined = 1;
		if (mode("run_status")) r->status = -EIO;
		if (mode("run_hpm")) r->hpm_configured = 0;
		if (mode("run_abi")) r->abi_version++;
		if (mode("run_size")) r->struct_size++;
		if (mode("run_count")) r->victim.completed--;
		if (mode("run_submitted")) r->victim.submitted++;
		if (mode("run_bytes")) r->victim.completed_bytes++;
		if (mode("run_did")) r->victim_did++;
		if (mode("run_elapsed")) r->victim_elapsed_ns = 0;
		if (mode("run_started")) r->measurement_started = 0;
		if (mode("run_incomplete")) r->measurement_complete = 0;
		if (mode("run_timeout")) {
			r->status = -ETIMEDOUT;
			r->measurement_complete = 0;
			r->victim.completed = 7;
			r->victim.completed_bytes = 7 * r->chunk_bytes;
			errno = ETIMEDOUT;
			return -1;
		}
		if (mode("run_warmup_timeout")) {
			r->status = -ETIMEDOUT;
			r->measurement_started = 0;
			r->measurement_complete = 0;
			errno = ETIMEDOUT;
			return -1;
		}
		if (p4m)
			fill_p4m(p4m);
		return mode("run_positive") ? 1 : 0;
	}
	abort();
}

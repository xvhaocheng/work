// SPDX-License-Identifier: GPL-2.0
/* Host-only syscalls: this executable cannot open a real DMA device. */
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/idma_multi.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int opened, caps_calls, run_calls, closed;

static int is(const char *value)
{
	const char *mode = getenv("E2_CLI_CASE");
	return mode && !strcmp(mode, value);
}

static void report(void)
{
	fprintf(stderr, "MOCK_SYSCALLS open=%u caps=%u run=%u close=%u\n",
		opened, caps_calls, run_calls, closed);
}

__attribute__((constructor)) static void setup(void)
{
	atexit(report);
}

int __wrap_open(const char *path, int flags, ...)
{
	assert(!strcmp(path, "/dev/idma0"));
	assert(flags == (O_RDWR | O_CLOEXEC));
	opened++;
	if (is("open_error")) { errno = ENOENT; return -1; }
	return 42;
}

int __wrap_close(int fd)
{
	assert(fd == 42);
	closed++;
	return 0;
}

int __wrap_ioctl(int fd, unsigned long command, ...)
{
	va_list args;
	void *arg;
	assert(fd == 42);
	va_start(args, command);
	arg = va_arg(args, void *);
	va_end(args);
	if (command == IDMA_IOC_CAPS_MULTI) {
		struct idma_multi_caps *c = arg;
		caps_calls++;
		if (is("caps_error")) { errno = ENOTTY; return -1; }
		*c = (struct idma_multi_caps) {
			.abi_version = 3, .struct_size = sizeof(*c), .capabilities = 7,
			.did = 10, .max_devices = 4, .max_pages = 8, .page_bytes = 4096,
			.max_run_timeout_ms = 300000,
		};
		if (is("caps_bad_abi")) c->abi_version = 2;
		if (is("caps_bad_reserved")) c->reserved[6] = 1;
		if (is("caps_no_path")) c->capabilities &= ~4U;
		return 0;
	}
	assert(command == IDMA_IOC_RUN_MULTI_BATCH);
	run_calls++;
	struct idma_multi_experiment *m = arg;
	unsigned int mask = (1U << m->nr_devices) - 1;
	assert(m->abi_version == 3 && m->struct_size == sizeof(*m));
	assert(m->flags == 0 && m->path_aggressor_index == 1 && m->chunk_bytes == 4096);
	assert(m->victim_transfers == 4096 && m->warmup_transfers == 256);
	assert(m->nr_devices >= 2 && m->nr_devices <= 4);
	for (unsigned int i = 0; i < m->nr_devices; i++) {
		assert(m->device_id[i] == i && (i || m->pages[i] == 1));
		m->did[i] = 10 + i;
		m->stats[i].submitted = m->stats[i].completed = i ? 4200 + i : 4096;
		m->stats[i].completed_bytes = m->stats[i].completed * 4096;
		m->stats[i].in_flight_at_end = i != 0;
		m->stats[i].iotlb_miss = i + 100;
		m->stats[i].s1_ptw = i + 200;
		m->stats[i].s2_ptw = i + 300;
		m->start_ns[i] = i ? 100 + i : 200;
		m->stop_ns[i] = 1000 + i;
		m->drain_end_ns[i] = 2000 + i;
		if (i) { m->pre_stop_status[i] = 3; m->pre_stop_issued[i] = 4200 + i; }
	}
	m->hpm_configured = m->measurement_started = m->measurement_complete = 1;
	m->prepared_mask = m->start_attempted_mask = m->stopped_mask = mask;
	m->drained_mask = m->verified_mask = mask;
	m->stop_status_valid_mask = mask & ~1U;
	m->observer_sequence = 17;
	m->victim_elapsed_ns = 700;
	if (is("run_error")) {
		m->status = -EIO; m->stats[1].quarantined = 1; m->drained_mask &= ~2U;
		errno = EIO; return -1;
	}
	if (is("bad_nr")) m->nr_devices = 99;
	if (is("bad_bytes")) m->stats[1].completed_bytes--;
	if (is("bad_mask")) m->drained_mask &= ~2U;
	if (is("bad_active")) m->pre_stop_status[1] = 1;
	if (is("bad_target")) m->pre_stop_issued[1] = IDMA_EXPERIMENT_MAX_TRANSFERS;
	if (is("bad_error")) m->pre_stop_status[1] |= 1ULL << 32;
	if (is("bad_end")) m->stats[1].in_flight_at_end = 0;
	if (is("bad_start")) m->stats[0].in_flight_at_start = 1;
	if (is("bad_launch_order")) m->start_ns[1] = m->start_ns[0];
	if (is("bad_did")) m->did[1] = 99;
	if (is("bad_data")) m->stats[1].data_errors = 1;
	if (is("bad_quarantine")) m->stats[1].quarantined = 1;
	if (is("bad_sequence")) m->observer_sequence = 0;
	return 0;
}

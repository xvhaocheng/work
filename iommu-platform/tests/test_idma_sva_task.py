#!/usr/bin/env python3
"""Independent file/lifecycle tests. Host adapter and mock ioctls are NOT DMA evidence."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
LENGTHS = (60, 568, 1011)

# Test-only syscall interposition exercises hard-to-trigger userspace API errors.
# It is never linked to the production executable and is enabled only by this suite.
SHIM = r'''
#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/idma_sva.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <unistd.h>
static int scenario(const char *value) {
 const char *s = getenv("IDMA_SVA_TEST_SCENARIO"); return s && !strcmp(s, value);
}
static const void *locked_arena, *last_arena;
static void lifecycle_event(const char *text) {
 const char *path = getenv("IDMA_SVA_TEST_LIFECYCLE_LOG");
 if (!path) return;
 int fd = syscall(SYS_openat, AT_FDCWD, path, O_WRONLY | O_CREAT | O_APPEND, 0600);
 if (fd < 0) abort();
 if (syscall(SYS_write, fd, text, strlen(text)) != (ssize_t)strlen(text)) abort();
 syscall(SYS_close, fd);
}
int mlock(const void *addr, size_t bytes) {
 if (!getenv("IDMA_SVA_TEST_IOCTL")) {
  int (*real_mlock)(const void *, size_t) = dlsym(RTLD_NEXT, "mlock");
  return real_mlock(addr, bytes);
 }
 if (bytes != 32768 || (uintptr_t)addr % 4096 || locked_arena) abort();
 lifecycle_event("MLOCK\n"); last_arena = addr;
 if (scenario("mlock-error")) { errno = ENOMEM; return -1; }
 locked_arena = addr; return 0;
}
int munlock(const void *addr, size_t bytes) {
 if (!getenv("IDMA_SVA_TEST_IOCTL")) {
  int (*real_munlock)(const void *, size_t) = dlsym(RTLD_NEXT, "munlock");
  return real_munlock(addr, bytes);
 }
 if (bytes != 32768 || locked_arena != addr) abort();
 lifecycle_event("MUNLOCK\n");
 if (scenario("munlock-error")) { errno = EIO; return -1; }
 locked_arena = NULL; return 0;
}
int munmap(void *addr, size_t bytes) {
 int (*real_munmap)(void *, size_t) = dlsym(RTLD_NEXT, "munmap");
 if (getenv("IDMA_SVA_TEST_IOCTL") && addr == last_arena) {
  if (bytes != 32768 || (locked_arena && !scenario("munlock-error"))) abort();
  lifecycle_event("MUNMAP\n"); locked_arena = NULL; last_arena = NULL;
 }
 return real_munmap(addr, bytes);
}
int ioctl(int fd, unsigned long op, ...) {
 va_list ap; void *arg;
 va_start(ap, op); arg = va_arg(ap, void *); va_end(ap);
 if (!getenv("IDMA_SVA_TEST_IOCTL")) {
  int (*real_ioctl)(int, unsigned long, ...) = dlsym(RTLD_NEXT, "ioctl");
  return real_ioctl(fd, op, arg);
 }
 if (op == IDMA_IOC_SVA_CAPS) {
  struct idma_sva_caps *c = arg;
  if (scenario("caps-errno")) { errno = ENOTTY; return -1; }
  c->capabilities = 15; c->address_modes = 7; c->primary_did = 10; c->peer_did = 11;
  c->page_bytes = 4096; c->arena_pages = 8; c->data_pages = 7;
  c->descriptors = 4; c->packets = 3; c->max_timeout_ms = 10000;
  if (scenario("caps-mode")) c->address_modes = 0;
  if (scenario("caps-did")) c->peer_did = 12;
  if (scenario("caps-abi")) c->abi_version++;
  if (scenario("caps-profile")) c->capabilities &= ~IDMA_SVA_CAP_SINGLE_HART_FENCE_I;
  return 0;
 }
 if (op == IDMA_IOC_SVA_RUN) {
  static unsigned int runs;
  struct idma_sva_run *r = arg;
  unsigned char *a = (void *)(uintptr_t)r->arena_user;
  if ((r->address_mode == 3 && locked_arena != a) ||
      (r->address_mode != 3 && locked_arena)) abort();
  lifecycle_event("RUN\n");
  const unsigned int src[] = {32, 4128, 8224, 12320};
  const unsigned int dst[] = {16396, 20492, 20557, 24588};
  const unsigned int len[] = {60, 65, 503, 1011};
  unsigned int i, j;
  uint64_t base = r->address_mode == 1 ? 0x100000 : r->arena_user;
  if (r->abi_version != 1 || r->struct_size != sizeof(*r) || r->arena_bytes != 32768 ||
      r->flags || r->peer_index != 1 || r->arena_user % 4096) { errno = EINVAL; return -1; }
  /* Each repetition must begin with freshly poisoned destination/source guards. */
  for (i = 0; i < 4; i++) for (j = 0; j < len[i]; j++)
   if (a[dst[i] + j] != 0xa5) { errno = EUCLEAN; return -1; }
  runs++;
  r->status = 0; r->restore_status = 0; r->result_flags = IDMA_SVA_RESULT_SAFE;
  r->primary_did = 10; r->peer_did = 11; r->submitted = 4; r->completed = 4;
  r->pinned_pages = 8; r->mapped_pages = r->address_mode == 3 ? 0 : 7;
  r->domain_pt_pages = r->address_mode == 3 ? 0 : 3;
  if (scenario("run-errno")) { r->status = -ETIMEDOUT; errno = ETIMEDOUT; return -1; }
  for (i = 0; i < 4; i++) {
   memcpy(a + dst[i], a + src[i], len[i]);
   r->transfer_id[i] = runs * 10 + i + 1;
   r->device_src[i] = base + src[i]; r->device_dst[i] = base + dst[i];
  }
  if (scenario("run-status")) r->status = -EIO;
  if (scenario("restore-status")) r->restore_status = -EIO;
  if (scenario("ownership")) r->result_flags &= ~IDMA_SVA_RESULT_CPU_OWNED;
  if (scenario("pins")) r->result_flags &= ~IDMA_SVA_RESULT_PINS_RELEASED;
  if (scenario("partial-done")) r->completed = 3;
  if (scenario("resources")) r->pinned_pages = 7;
  if (scenario("address")) r->device_dst[2]++;
  if (scenario("receipt")) r->transfer_id[1] = 0;
  if (scenario("payload")) a[20492 + 65] ^= 1;
  if (scenario("header")) a[16384] ^= 1;
  if (scenario("source")) a[32] ^= 1;
  if (scenario("source-guard")) a[16] ^= 1;
  if (scenario("destination-guard")) a[16396 + 60] ^= 1;
  if (scenario("page-tail")) a[24576 + 4095] ^= 1;
  if (scenario("metadata")) a[28672 + 9] ^= 1;
  return 0;
 }
 errno = ENOTTY; return -1;
}
ssize_t writev(int fd, const struct iovec *iov, int count) {
 static unsigned int calls;
 ssize_t (*real_writev)(int, const struct iovec *, int) = dlsym(RTLD_NEXT, "writev");
 const char *io = getenv("IDMA_SVA_TEST_IO");
 if (locked_arena) abort();
 lifecycle_event("CONSUMER\n");
 calls++;
 if (io && !strcmp(io, "partial")) {
  struct iovec limited[3]; int n = 0; size_t left = 7;
  if (calls % 4 == 1) { errno = EINTR; return -1; }
  while (n < count && left) {
   limited[n] = iov[n]; if (limited[n].iov_len > left) limited[n].iov_len = left;
   left -= limited[n].iov_len; n++;
  }
  return real_writev(fd, limited, n);
 }
 if (io && !strcmp(io, "zero")) return 0;
 if (io && (!strcmp(io, "error") || (!strcmp(io, "partial-error") && calls > 1))) {
  errno = ENOSPC; return -1;
 }
 return real_writev(fd, iov, count);
}
ssize_t read(int fd, void *buffer, size_t count) {
 static unsigned int calls;
 ssize_t (*real_read)(int, void *, size_t) = dlsym(RTLD_NEXT, "read");
 const char *io = getenv("IDMA_SVA_TEST_IO");
 if (io && !strcmp(io, "read-partial")) {
  if (++calls % 4 == 1) { errno = EINTR; return -1; }
  if (count > 7) count = 7;
 }
 return real_read(fd, buffer, count);
}
int close(int fd) {
 int (*real_close)(int) = dlsym(RTLD_NEXT, "close");
 const char *io = getenv("IDMA_SVA_TEST_IO");
 int flags = fcntl(fd, F_GETFL);
 int result = real_close(fd);
 if (io && !strcmp(io, "close-error") && flags >= 0 && (flags & O_ACCMODE) == O_WRONLY) {
  errno = EIO; return -1;
 }
 return result;
}
'''


class TaskTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.build = tempfile.TemporaryDirectory(prefix="idma-sva-task-build-")
        cls.builddir = Path(cls.build.name)
        include = cls.builddir / "include/linux"
        include.mkdir(parents=True)
        shutil.copy(ROOT / "linux/include/uapi/linux/idma_sva.h", include)
        cls.binary = cls.builddir / "idma_sva_task"
        common = ["gcc", "-O2", "-Wall", "-Wextra", "-Werror", "-I", str(include.parent)]
        subprocess.run(common + [str(ROOT / "bench/idma_sva_task.c"), "-o", str(cls.binary)], check=True)
        source = cls.builddir / "shim.c"
        source.write_text(SHIM)
        cls.shim = cls.builddir / "shim.so"
        subprocess.run(common + ["-shared", "-fPIC", str(source), "-ldl", "-o", str(cls.shim)], check=True)

    @classmethod
    def tearDownClass(cls):
        cls.build.cleanup()

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="idma-sva-task-test-")
        self.addCleanup(self.temp.cleanup)
        self.work = Path(self.temp.name)
        self.source = self.work / "input.bin"
        # Deliberately differs from the materialized fixture, including byte 0xa5.
        self.data = b"".join(struct.pack("<QI", p + 1, n) +
                            bytes(((j * 73 + p * 51 + 165) ^ (j >> 3)) & 255 for j in range(n))
                            for p, n in enumerate(LENGTHS))
        self.source.write_bytes(self.data)

    def run_task(self, mode="pinned-mm", *, host=True, scenario=None, io=None,
                 outputs=None, repeat=1, extra=(), source=None, lifecycle_log=None):
        outputs = outputs or [self.work / "output.bin"]
        args = [str(self.binary), "--input", str(source or self.source), "--address-mode", mode,
                "--repeat", str(repeat), "--device", "/dev/null"]
        for output in outputs:
            args += ["--output", str(output)]
        if host:
            args += ["--backend", "host-model"]
        args += list(extra)
        env = os.environ.copy()
        if scenario is not None or io:
            env["LD_PRELOAD"] = str(self.shim)
        if scenario is not None:
            env["IDMA_SVA_TEST_IOCTL"] = "1"
            env["IDMA_SVA_TEST_SCENARIO"] = scenario
        if io:
            env["IDMA_SVA_TEST_IO"] = io
        if lifecycle_log:
            env["IDMA_SVA_TEST_LIFECYCLE_LOG"] = str(lifecycle_log)
        result = subprocess.run(args, env=env, text=True, capture_output=True)
        artifact_root = os.environ.get("IDMA_SVA_TEST_ARTIFACT_DIR")
        if artifact_root:
            self._artifact_calls = getattr(self, "_artifact_calls", 0) + 1
            artifact = Path(artifact_root) / self._testMethodName / str(self._artifact_calls)
            artifact.mkdir(parents=True, exist_ok=True)
            (artifact / "stdout.jsonl").write_text(result.stdout)
            (artifact / "stderr.log").write_text(result.stderr)
            (artifact / "invocation.json").write_text(json.dumps({"argv": args, "returncode": result.returncode,
                "test_only_ioctl_mock": scenario is not None, "hardware_evidence": False}, indent=2) + "\n")
            if lifecycle_log and Path(lifecycle_log).exists():
                shutil.copyfile(lifecycle_log, artifact / "lifecycle.log")
            for index, output in enumerate(outputs):
                if Path(output).is_file():
                    shutil.copyfile(output, artifact / f"consumer-{index}.bin")
        return result, outputs

    def test_materialized_real_file_all_explicit_host_modes(self):
        source = ROOT / "out/a2-native-sva-binding-20261015/design/three-frames.bin"
        expected = source.read_bytes()
        self.assertEqual(hashlib.sha256(expected).hexdigest(),
                         "914ec5d1fe933aca831497dcdd4a8599ede1e22053303cf9453832b5c3f0f12f")
        for mode in ("iova", "sameva-shadow", "pinned-mm"):
            with self.subTest(mode=mode):
                completed, outputs = self.run_task(mode, source=source)
                self.assertEqual(completed.returncode, 0, completed.stderr)
                self.assertEqual(outputs[0].read_bytes(), expected)
                record = json.loads(completed.stdout)
                self.assertEqual(record["backend"], "host-model-NO-DMA")
                self.assertEqual(record["dma_completed"], 0)
                self.assertEqual(record["timing"], "UNKNOWN")
                trace = record["model_trace"]
                self.assertEqual(len(trace), 33)
                self.assertEqual(trace[:7], ["d0:" + e for e in
                                 ("SOURCE_READY", "NEXT_ID", "COUNT_ACCEPT", "R", "W", "B", "DONE")])
                self.assertEqual(trace[-5:], ["CACHE_ACQUIRE", "CONTEXT_RESTORED", "UNPIN",
                                             "INDEPENDENT_VERIFY", "FILE_CONSUMER"])

    def test_arbitrary_payload_and_cpu_controls(self):
        for mode in ("pinned-mm", "cpu-copy", "source-writev"):
            with self.subTest(mode=mode):
                completed, outputs = self.run_task(mode, host=mode == "pinned-mm")
                self.assertEqual(completed.returncode, 0, completed.stderr)
                self.assertEqual(outputs[0].read_bytes(), self.data)
                record = json.loads(completed.stdout)
                self.assertEqual(record["dma_submitted"], 0)
                self.assertEqual(record["pinned_pages"], 0)
                if mode == "source-writev":
                    self.assertEqual(record["arena_pages"], 0)

    def test_production_never_silently_falls_back(self):
        completed, outputs = self.run_task(host=False)
        self.assertNotEqual(completed.returncode, 0)
        self.assertIn("CAPS", completed.stderr)
        self.assertFalse(outputs[0].exists())
        self.assertNotIn("PASS", completed.stdout)

    def test_fixed_frames_reject_bad_length_cookie_truncation_and_trailing_data(self):
        variants = (self.data[:-1], self.data + b"x", self.data[:72],
                    b"\x02" + self.data[1:], self.data[:8] + struct.pack("<I", 61) + self.data[12:])
        for data in variants:
            with self.subTest(length=len(data)):
                self.source.write_bytes(data)
                completed, outputs = self.run_task()
                self.assertNotEqual(completed.returncode, 0)
                self.assertFalse(outputs[0].exists())

    def test_repeat_reinitializes_and_delivers_independent_files(self):
        outputs = [self.work / f"run-{i}.bin" for i in range(3)]
        # A mock RUN also checks every destination payload is reset before publication.
        completed, _ = self.run_task(host=False, scenario="success", outputs=outputs, repeat=3)
        self.assertEqual(completed.returncode, 0, completed.stderr)
        records = [json.loads(line) for line in completed.stdout.splitlines()]
        self.assertEqual([r["iteration"] for r in records], [1, 2, 3])
        for output in outputs:
            self.assertEqual(output.read_bytes(), self.data)

    def test_partial_and_interrupted_input_and_consumer_io(self):
        for io in ("partial", "read-partial"):
            for mode in ("pinned-mm", "source-writev"):
                with self.subTest(io=io, mode=mode):
                    completed, outputs = self.run_task(mode, host=mode == "pinned-mm", io=io)
                    self.assertEqual(completed.returncode, 0, completed.stderr)
                    self.assertEqual(outputs[0].read_bytes(), self.data)

    def test_existing_output_truncated_to_exact_consumer_bytes(self):
        output = self.work / "output.bin"
        output.write_bytes(b"old" * 4000)
        completed, _ = self.run_task(outputs=[output])
        self.assertEqual(completed.returncode, 0, completed.stderr)
        self.assertEqual(output.read_bytes(), self.data)

    def test_early_or_partial_consumer_failures_never_report_success(self):
        for io in ("error", "zero", "partial-error", "close-error"):
            with self.subTest(io=io):
                completed, outputs = self.run_task(io=io)
                self.assertNotEqual(completed.returncode, 0)
                self.assertIn("file consumer", completed.stderr)
                self.assertNotIn("PASS", completed.stdout)
                if io == "partial-error":
                    self.assertEqual(outputs[0].read_bytes(), self.data[:72])

    def test_caps_run_restore_owner_resource_and_receipt_failures(self):
        for scenario in ("caps-errno", "caps-mode", "caps-did", "caps-abi", "caps-profile",
                         "run-errno", "run-status", "restore-status", "ownership", "pins",
                         "partial-done", "resources", "address", "receipt"):
            with self.subTest(scenario=scenario):
                completed, outputs = self.run_task(host=False, scenario=scenario)
                self.assertNotEqual(completed.returncode, 0)
                self.assertFalse(outputs[0].exists())
                self.assertNotIn("PASS", completed.stdout)

    def test_independent_payload_header_source_guard_tail_and_metadata_detection(self):
        for scenario in ("payload", "header", "source", "source-guard", "destination-guard",
                         "page-tail", "metadata"):
            with self.subTest(scenario=scenario):
                completed, outputs = self.run_task(host=False, scenario=scenario)
                self.assertNotEqual(completed.returncode, 0)
                self.assertIn("arena mismatch", completed.stderr)
                self.assertFalse(outputs[0].exists())

    def test_input_output_alias_and_repeat_path_alias_rejected_without_truncation(self):
        alias = self.work / "hardlink.bin"
        os.link(self.source, alias)
        for outputs, repeat in (([self.source], 1), ([alias], 1),
                                ([self.work / "same", self.work / "same"], 2)):
            with self.subTest(outputs=outputs):
                completed, _ = self.run_task(outputs=outputs, repeat=repeat)
                self.assertNotEqual(completed.returncode, 0)
                self.assertEqual(self.source.read_bytes(), self.data)

    def test_new_output_alias_cannot_truncate_prior_delivery(self):
        output = self.work / "new-output.bin"
        alias = str(self.work) + "/./new-output.bin"
        completed, _ = self.run_task(outputs=[output, alias], repeat=2)
        self.assertNotEqual(completed.returncode, 0)
        self.assertEqual(output.read_bytes(), self.data)
        self.assertEqual(len(completed.stdout.splitlines()), 1)

    def test_native_borrowed_locks_each_same_arena_and_unlocks_before_consumer(self):
        log = self.work / "lifecycle.log"
        outputs = [self.work / f"locked-{i}.bin" for i in range(2)]
        completed, _ = self.run_task(host=False, scenario="success", outputs=outputs,
                                     repeat=2, lifecycle_log=log)
        self.assertEqual(completed.returncode, 0, completed.stderr)
        self.assertEqual(log.read_text().splitlines(),
                         ["MLOCK", "RUN", "MUNLOCK", "CONSUMER", "CONSUMER", "CONSUMER", "MUNMAP"] * 2)
        for output in outputs:
            self.assertEqual(output.read_bytes(), self.data)
        for record in completed.stdout.splitlines():
            self.assertEqual(json.loads(record)["locked_pages_peak"], 8)

    def test_lock_unlock_and_safely_returned_run_failures_never_consume(self):
        for scenario in ("mlock-error", "munlock-error", "run-errno"):
            with self.subTest(scenario=scenario):
                log = self.work / (scenario + ".log")
                completed, outputs = self.run_task(host=False, scenario=scenario, lifecycle_log=log)
                self.assertNotEqual(completed.returncode, 0)
                self.assertFalse(outputs[0].exists())
                self.assertNotIn("PASS", completed.stdout)
                events = log.read_text().splitlines()
                self.assertNotIn("CONSUMER", events)
                if scenario == "mlock-error":
                    self.assertEqual(events, ["MLOCK", "MUNMAP"])
                    self.assertIn("arena mlock", completed.stderr)
                else:
                    self.assertEqual(events, ["MLOCK", "RUN", "MUNLOCK", "MUNMAP"])
                    self.assertIn("arena munlock" if scenario == "munlock-error" else "DMA RUN", completed.stderr)

    def test_ordinary_cpu_and_host_paths_do_not_pay_borrowed_lock_cost(self):
        for mode, host in (("iova", False), ("sameva-shadow", False), ("pinned-mm", True),
                           ("cpu-copy", False), ("source-writev", False)):
            with self.subTest(mode=mode, host=host):
                log = self.work / (mode + ".log")
                completed, outputs = self.run_task(mode, host=host, scenario="mlock-error", lifecycle_log=log)
                self.assertEqual(completed.returncode, 0, completed.stderr)
                self.assertEqual(outputs[0].read_bytes(), self.data)
                self.assertNotIn("MLOCK", log.read_text())
                self.assertNotIn("MUNLOCK", log.read_text())
                self.assertEqual(json.loads(completed.stdout)["locked_pages_peak"], 0)

    def test_all_native_address_mode_contracts_with_test_only_mock(self):
        for mode in ("iova", "sameva-shadow", "pinned-mm"):
            with self.subTest(mode=mode):
                completed, outputs = self.run_task(mode, host=False, scenario="success")
                self.assertEqual(completed.returncode, 0, completed.stderr)
                self.assertEqual(outputs[0].read_bytes(), self.data)
                self.assertEqual(json.loads(completed.stdout)["dma_completed"], 4)


if __name__ == "__main__":
    unittest.main(verbosity=2)

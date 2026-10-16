#!/bin/sh
# Run only after root has accepted this private instrumented BIT and timing.
# Original benchmark is executed exactly once with the frozen request. The
# readback companion runs after its ioctl/drain, without another DMA job.
set -u
if [ "$#" -ne 4 ]; then
    echo "usage: $0 /path/idma_composition /path/mig_capture_read NEW-DIR LOADED-BIT-SHA256" >&2
    exit 2
fi
bench=$1
reader=$2
dest=$3
bit_sha=$4
case "$bit_sha" in *[!0-9a-fA-F]*|'') echo "invalid BIT SHA256" >&2; exit 2;; esac
if [ "${#bit_sha}" -ne 64 ] || [ ! -x "$bench" ] || [ ! -x "$reader" ]; then
    echo "need executable tools and exact 64-hex loaded BIT SHA256" >&2; exit 2
fi
# Existing destination is a hard stop; this launcher never retries a workload.
mkdir "$dest" || exit 2
printf '%s\n' "$bit_sha" > "$dest/loaded-bit.sha256"
uname -a > "$dest/uname.before.txt"
uname -r > "$dest/kernel.release.txt"
cat /proc/sys/kernel/random/boot_id > "$dest/boot.before.txt"
cat /proc/sys/kernel/tainted > "$dest/tainted.before.txt"
sha256sum "$bench" "$reader" > "$dest/tools.sha256"
"$bench" --abi > "$dest/original-tool-abi.json" 2> "$dest/original-tool-abi.stderr"
abi_rc=$?
"$reader" --probe > "$dest/private-probe.json" 2> "$dest/private-probe.stderr"
probe_rc=$?
if [ "$abi_rc" -ne 0 ] || [ "$probe_rc" -ne 0 ]; then
    printf 'abi_exit=%s\nprobe_exit=%s\nworkload_attempted=0\n' "$abi_rc" "$probe_rc" > "$dest/launcher.receipt"
    exit 2
fi
date -u > "$dest/started.utc.txt"
printf '%s\n' "$bench --device /dev/idma0 --mode 3 --a-pages 8 --v-gap 0 --transfers 64 --warmup 256 --timeout-ms 15000" > "$dest/command.txt"
"$bench" --device /dev/idma0 --mode 3 --a-pages 8 --v-gap 0 --transfers 64 --warmup 256 --timeout-ms 15000 > "$dest/composition.stdout.json" 2> "$dest/composition.stderr"
bench_rc=$?
"$reader" --dump "$dest/mig-capture.bin" > "$dest/readback.receipt.json" 2> "$dest/readback.stderr"
read_rc=$?
cat /proc/sys/kernel/random/boot_id > "$dest/boot.after.txt"
cat /proc/sys/kernel/tainted > "$dest/tainted.after.txt"
date -u > "$dest/finished.utc.txt"
printf 'abi_exit=%s\nprobe_exit=%s\nworkload_attempted=1\nworkload_exit=%s\nreadback_exit=%s\n' "$abi_rc" "$probe_rc" "$bench_rc" "$read_rc" > "$dest/launcher.receipt"
sha256sum "$dest/composition.stdout.json" > "$dest/raw.sha256"
if [ -f "$dest/mig-capture.bin" ]; then sha256sum "$dest/mig-capture.bin" >> "$dest/raw.sha256"; fi
if [ "$bench_rc" -ne 0 ] || [ "$read_rc" -ne 0 ]; then exit 1; fi
exit 0

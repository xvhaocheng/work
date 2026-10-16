#!/bin/sh
# Run ON THE FPGA Linux console. This does not program the FPGA or overwrite media.
set -eu
dump_kernel_log() {
    result=$?
    trap - 0
    echo 'KERNEL LOG: last 60 lines (also collected after a failed command)'
    dmesg | tail -60 || :
    exit "$result"
}
trap dump_kernel_log 0
echo 'BEGIN platform smoke'
uname -a
cat /proc/device-tree/cpus/timebase-frequency | od -An -tx1
echo 'Calibration candidates only: warmup=0 does not itself establish an IOTLB cold state.'
for n in 0 1 2 3; do
    test -c "/dev/idma$n"
    for warmup in 0 32; do
        echo "SOLO DMA$n warmup=$warmup: compare per-DID events and verified completion counts"
        platform-timeout 120 idma_experiment --device "/dev/idma$n" \
            --aggressor none --victim-pages 1 --chunk 4096 --transfers 8 \
            --warmup "$warmup" --timeout-ms 10000
    done
    echo "SOLO DMA$n: copy four distinct pages once; verify four untouched pages too"
    platform-timeout 120 idma_experiment --device "/dev/idma$n" \
        --aggressor none --victim-pages 8 --chunk 4096 --transfers 4 \
        --warmup 0 --timeout-ms 10000
done
for victim in 0 1; do
    aggressor=$((1 - victim))
    echo "CORUN victim=DMA$victim aggressor=DMA$aggressor: role/DID exchange candidate"
    platform-timeout 120 idma_experiment --device "/dev/idma$victim" \
        --aggressor "$aggressor" --victim-pages 1 --aggressor-pages 8 \
        --chunk 4096 --transfers 64 --warmup 32 --timeout-ms 10000
done
echo 'END platform smoke: commands completed; calibration remains pending human review.'
echo 'Save all output and inspect status/data errors, DIDs, HPM, refill gaps, window work and kernel log.'
echo 'Counters need not have fixed miss values. Software submissions do not prove concurrent AXI pressure.'

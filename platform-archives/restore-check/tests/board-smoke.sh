#!/bin/sh
# Run ON THE FPGA Linux console. This does not program the FPGA or overwrite media.
set -eu
echo 'BEGIN platform smoke'
uname -a
cat /proc/device-tree/cpus/timebase-frequency | od -An -tx1
for n in 0 1 2 3; do
    test -c "/dev/idma$n"
    echo "SOLO DMA$n"
    /bin/busybox timeout 120 idma_pair_bench --single --victim-dev "/dev/idma$n" \
        --aggressor-id none --victim-size 2m --victim-iters 2 \
        --victim-mode 4k --chunk 4k --verify
done
echo 'Large single-descriptor transfer, DMA0'
/bin/busybox timeout 120 idma_once 2M
for schedule in lockstep independent; do
    echo "PAIR $schedule"
    /bin/busybox timeout 120 idma_pair_bench --single --victim-dev /dev/idma0 --aggressor-id 1 \
        --victim-size 2m --aggressor-size 2m --victim-iters 2 --aggressor-iters 2 \
        --victim-mode 4k --aggressor-mode 4k --chunk 4k --schedule "$schedule" --verify
done
echo 'END platform smoke: inspect CSV errors/return codes and kernel log before accepting'
dmesg | tail -60

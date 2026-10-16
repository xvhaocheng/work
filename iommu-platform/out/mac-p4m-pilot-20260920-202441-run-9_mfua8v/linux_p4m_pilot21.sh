#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
# P4M pilot-21 board runner (exactly 21 trials: 7 conditions x 3 repetitions).
#
# Usage: linux_p4m_pilot21.sh OUTPUT_DIR
#
# Frozen conditions (extra idma_experiment arguments):
#   S        --p4m --aggressor none --a-gap-cycles 0
#   A1g0     --p4m --aggressor 1 --aggressor-pages 1 --a-gap-cycles 0
#   A8g0     --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0
#   A8g500   --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 500
#   A8g1000  --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 1000
#   A8g2000  --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 2000
#   B8       --batch --aggressor 1 --aggressor-pages 8  (no --p4m, no gap)
#
# Frozen for every trial:
#   idma_experiment --device /dev/idma0 --victim-pages 1 --chunk 4096
#     --transfers 4096 --warmup 256 --timeout-ms 60000
#   wrapped in: platform-timeout 120
#
# Frozen interleaved order (3 blocks of 7 = 21 trials):
#   rep1: S A1g0 B8 A8g0 A8g500 A8g1000 A8g2000
#   rep2: S A8g2000 A8g1000 A8g500 A8g0 B8 A1g0
#   rep3: S A8g1000 A8g500 A8g2000 B8 A8g0 A1g0
#
# OUTPUT_DIR is required and must not already exist.
#
# Engineering run only: no retries, no extra experiments, the first failure
# stops the run. No dmesg clear, no network, no SD burn, no hidden failure.
# Every trial requires a readable decimal taint of exactly 0 before and after
# the run; any nonzero/unknown readback stops the run and is preserved.
# The whole final dmesg and the final taint are always printed, even when the
# run fails. Control records ("P4M:" lines, exit records, dmesg, taint) go to
# stderr; the captured per-trial stdout (JSON) is printed to stdout, matching
# platform-p4m. Capture both streams on the UART.
#
# Exit codes:
#   0            clean complete (21/21 trials)
#   1..3         first trial failure (command rc, 3 taint gate)
#   4            capture/output failure after an otherwise clean run
#   127          required command missing
#   129/130/143  HUP/INT/TERM signal
#   2            usage or fatal setup error
set -u

PATH=/sbin:/bin:/usr/sbin:/usr/bin
export PATH

KERNEL_EXPECT=6.5.0-rc2-00088-g9f8da9e7a6a0
TOTAL_TRIALS=21
PLATFORM_TIMEOUT=120
VICTIM_DEVICE=/dev/idma0
COMMON_ARGS="--device $VICTIM_DEVICE --victim-pages 1 --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000"

BLOCK1="S A1g0 B8 A8g0 A8g500 A8g1000 A8g2000"
BLOCK2="S A8g2000 A8g1000 A8g500 A8g0 B8 A1g0"
BLOCK3="S A8g1000 A8g500 A8g2000 B8 A8g0 A1g0"

usage() {
    cat <<'EOF'
Usage: linux_p4m_pilot21.sh OUTPUT_DIR
  Exactly 21 P4M pilot trials: 7 conditions x 3 repetitions, frozen order.
    S  A1g0  A8g0  A8g500  A8g1000  A8g2000  B8 (ordinary --batch)
  Every trial: --device /dev/idma0 --victim-pages 1 --chunk 4096
    --transfers 4096 --warmup 256 --timeout-ms 60000,
    wrapped in platform-timeout 120.
  OUTPUT_DIR is required and must not already exist.
  First failure stops the run: no retries, no extra experiments.
EOF
}

fatal() {
    echo "P4M: fatal: $*" >&2
    exit 2
}

[ $# -eq 1 ] || { usage >&2; exit 2; }
OUT=$1
[ -n "$OUT" ] || { usage >&2; exit 2; }

if [ -e "$OUT" ]; then
    fatal "output directory already exists: $OUT"
fi
mkdir "$OUT" || fatal "mkdir failed: $OUT"

read_taint() {
    if value=$(cat /proc/sys/kernel/tainted 2>/dev/null); then
        case "$value" in
            ''|*[!0-9]*) echo unknown ;;
            *) echo "$value" ;;
        esac
    else
        echo unknown
    fi
}

save_dmesg() {
    dmesg > "$OUT/dmesg.raw" 2> "$OUT/dmesg.err"
    dmesg_rc=$?
    if [ "$dmesg_rc" -eq 0 ]; then
        emit_rc=0
        echo "P4M: dmesg.raw begin" >&2 || emit_rc=1
        cat "$OUT/dmesg.raw" >&2 || emit_rc=1
        echo "P4M: dmesg.raw end" >&2 || emit_rc=1
        if [ "$emit_rc" -ne 0 ]; then
            echo "P4M: dmesg.raw cat failed" >&2
            return 4
        fi
        return 0
    fi
    echo "dmesg collection failed rc=$dmesg_rc" > "$OUT/dmesg.error"
    echo "P4M: dmesg collection failed rc=$dmesg_rc" >&2
    echo "P4M: dmesg.raw begin (partial)" >&2
    cat "$OUT/dmesg.raw" >&2 2>/dev/null || echo "P4M: dmesg.raw cat failed" >&2
    echo "P4M: dmesg.raw end (partial)" >&2
    if [ -s "$OUT/dmesg.err" ]; then
        echo "P4M: dmesg.err begin" >&2
        cat "$OUT/dmesg.err" >&2 2>/dev/null || echo "P4M: dmesg.err cat failed" >&2
        echo "P4M: dmesg.err end" >&2
    fi
    return "$dmesg_rc"
}

save_taint() {
    final_taint=$(read_taint)
    if ! echo "$final_taint" > "$OUT/taint.txt"; then
        echo "P4M: taint write failed" >&2
        return 1
    fi
    echo "P4M: final_taint=$final_taint" >&2
    if [ "$final_taint" != "0" ]; then
        echo "P4M: final taint not clean (value=$final_taint)" >&2
        return 1
    fi
    return 0
}

case_args() {
    case "$1" in
        S)       echo "--p4m --aggressor none --a-gap-cycles 0" ;;
        A1g0)    echo "--p4m --aggressor 1 --aggressor-pages 1 --a-gap-cycles 0" ;;
        A8g0)    echo "--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0" ;;
        A8g500)  echo "--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 500" ;;
        A8g1000) echo "--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 1000" ;;
        A8g2000) echo "--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 2000" ;;
        B8)      echo "--batch --aggressor 1 --aggressor-pages 8" ;;
        *)       return 1 ;;
    esac
    return 0
}

run_trial() {
    rt_id=$1
    rt_cond=$2
    rt_rep=$3
    rt_seq=$4

    if ! rt_extra=$(case_args "$rt_cond"); then
        echo "P4M: internal error unknown condition=$rt_cond" >&2
        return 2
    fi

    # Recorded argv and the executed argv are built from the same tokens.
    # No token contains whitespace, glob characters or quotes; default IFS is
    # in effect, so the unquoted expansions below are exactly this argv.
    rt_argv="platform-timeout $PLATFORM_TIMEOUT idma_experiment $COMMON_ARGS $rt_extra"

    rt_out="$OUT/$rt_id.json"
    rt_err="$OUT/$rt_id.err"

    rt_before=$(read_taint)
    if [ "$rt_before" != "0" ]; then
        echo "P4M: trial_gate id=$rt_id condition=$rt_cond rep=$rt_rep seq=$rt_seq before_taint=$rt_before refusing to run" >&2
        echo "P4M: trial_end id=$rt_id condition=$rt_cond rep=$rt_rep exit=3" >&2
        return 3
    fi

    echo "P4M: trial_begin id=$rt_id condition=$rt_cond rep=$rt_rep seq=$rt_seq total=$TOTAL_TRIALS taint_before=$rt_before argv=$rt_argv" >&2

    if ! printf 'seq=%s id=%s condition=%s rep=%s argv=%s\n' \
            "$rt_seq" "$rt_id" "$rt_cond" "$rt_rep" "$rt_argv" >> "$OUT/argv.txt"; then
        echo "P4M: trial_argv_record_failed id=$rt_id" >&2
        echo "P4M: trial_end id=$rt_id condition=$rt_cond rep=$rt_rep exit=4" >&2
        return 4
    fi

    # shellcheck disable=SC2086
    platform-timeout "$PLATFORM_TIMEOUT" idma_experiment $COMMON_ARGS $rt_extra \
        > "$rt_out" 2> "$rt_err"
    rt_rc=$?
    rt_after=$(read_taint)

    rt_cat_failed=0
    echo "P4M: trial_stdout_begin id=$rt_id condition=$rt_cond rep=$rt_rep" >&2
    cat "$rt_out" || {
        echo "P4M: trial_stdout_cat_failed id=$rt_id" >&2
        rt_cat_failed=1
    }
    echo "P4M: trial_stdout_end id=$rt_id condition=$rt_cond rep=$rt_rep" >&2

    echo "P4M: trial_stderr_begin id=$rt_id condition=$rt_cond rep=$rt_rep" >&2
    if [ -s "$rt_err" ]; then
        cat "$rt_err" >&2 || {
            echo "P4M: trial_stderr_cat_failed id=$rt_id" >&2
            rt_cat_failed=1
        }
    fi
    echo "P4M: trial_stderr_end id=$rt_id condition=$rt_cond rep=$rt_rep" >&2

    # Emit the ring after every trial so early evidence survives ring wrap.
    save_dmesg || rt_cat_failed=1

    echo "P4M: trial_exit id=$rt_id condition=$rt_cond rep=$rt_rep seq=$rt_seq exit=$rt_rc taint_after=$rt_after" >&2

    if [ "$rt_rc" -ne 0 ]; then
        echo "P4M: trial_failed id=$rt_id condition=$rt_cond rep=$rt_rep exit=$rt_rc" >&2
        echo "P4M: trial_end id=$rt_id condition=$rt_cond rep=$rt_rep exit=$rt_rc" >&2
        return "$rt_rc"
    fi
    if [ "$rt_cat_failed" -ne 0 ]; then
        echo "P4M: trial_end id=$rt_id condition=$rt_cond rep=$rt_rep exit=4" >&2
        return 4
    fi
    if [ "$rt_after" != "0" ]; then
        echo "P4M: trial_taint_stop id=$rt_id condition=$rt_cond rep=$rt_rep after_taint=$rt_after" >&2
        echo "P4M: trial_end id=$rt_id condition=$rt_cond rep=$rt_rep exit=3" >&2
        return 3
    fi

    echo "P4M: trial_end id=$rt_id condition=$rt_cond rep=$rt_rep exit=0" >&2
    return 0
}

collection_failed=0
trials_done=0

cleanup() {
    rc=$?
    trap - EXIT HUP INT TERM
    save_dmesg || collection_failed=1
    save_taint || collection_failed=1
    if [ "$collection_failed" -ne 0 ] && [ "$rc" -eq 0 ]; then
        rc=4
    fi
    echo "P4M: final_exit=$rc collection_failed=$collection_failed trials_done=$trials_done total=$TOTAL_TRIALS" >&2
    if [ "$rc" -eq 0 ]; then
        if ! echo "P4M: complete ok mode=pilot21 trials=$trials_done output=$OUT"; then
            echo "P4M: complete marker write failed" >&2
            rc=4
        fi
    fi
    exit "$rc"
}

trap cleanup EXIT
trap 'exit 129' HUP
trap 'exit 130' INT
trap 'exit 143' TERM

if ! {
    echo "mode=pilot21"
    echo "output=$OUT"
    echo "kernel_expected=$KERNEL_EXPECT"
    echo "trials=$TOTAL_TRIALS platform_timeout=$PLATFORM_TIMEOUT"
} > "$OUT/input.txt"; then
    fatal "input write failed: $OUT/input.txt"
fi

if ! {
    echo "P4M pilot-21 frozen matrix"
    echo "target_kernel=$KERNEL_EXPECT"
    echo "trials=$TOTAL_TRIALS repetitions=3 conditions=7"
    echo "victim_device=$VICTIM_DEVICE aggressor_device=1 (fixed role V=DMA0, A=DMA1)"
    echo "common_argv=idma_experiment $COMMON_ARGS"
    echo "wrapper=platform-timeout $PLATFORM_TIMEOUT"
    echo "condition_argv:"
    echo "  S       $(case_args S)"
    echo "  A1g0    $(case_args A1g0)"
    echo "  A8g0    $(case_args A8g0)"
    echo "  A8g500  $(case_args A8g500)"
    echo "  A8g1000 $(case_args A8g1000)"
    echo "  A8g2000 $(case_args A8g2000)"
    echo "  B8      $(case_args B8)"
    echo "order_rep1=$BLOCK1"
    echo "order_rep2=$BLOCK2"
    echo "order_rep3=$BLOCK3"
} > "$OUT/matrix.txt"; then
    fatal "matrix write failed: $OUT/matrix.txt"
fi

: > "$OUT/argv.txt" || fatal "argv write failed: $OUT/argv.txt"

kernel_actual=$(uname -r)
echo "P4M: uname=$(uname -a)" >&2
echo "P4M: kernel_expected=$KERNEL_EXPECT kernel_actual=$kernel_actual" >&2
if [ "$kernel_actual" != "$KERNEL_EXPECT" ]; then
    fatal "wrong kernel running: expected $KERNEL_EXPECT got $kernel_actual"
fi

if ! command -v platform-timeout >/dev/null 2>&1; then
    echo "P4M: missing command: platform-timeout" >&2
    exit 127
fi
if ! command -v idma_experiment >/dev/null 2>&1; then
    echo "P4M: missing command: idma_experiment" >&2
    exit 127
fi

rep=1
tseq=0
while [ "$rep" -le 3 ]; do
    case "$rep" in
        1) block=$BLOCK1 ;;
        2) block=$BLOCK2 ;;
        3) block=$BLOCK3 ;;
        *) fatal "internal error rep=$rep" ;;
    esac
    for cond in $block; do
        tseq=$((tseq + 1))
        tid=$(printf 'p4m21-t%02d-%s-r%s' "$tseq" "$cond" "$rep") ||
            fatal "trial id build failed seq=$tseq"
        run_trial "$tid" "$cond" "$rep" "$tseq" || exit $?
        trials_done=$tseq
    done
    rep=$((rep + 1))
done

exit 0

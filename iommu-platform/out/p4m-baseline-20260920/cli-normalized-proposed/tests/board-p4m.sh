#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
# P4M short board runner.
#
# Usage: platform-p4m short [OUTPUT_DIR]
#
# Short acceptance only: four trials, each 64 transfers, warmup 8, 4 KiB.
#   1 Solo        --aggressor none  --a-gap-cycles 0
#   2 A1 gap0     --aggressor 1     --aggressor-pages 1 --a-gap-cycles 0
#   3 A8 gap0     --aggressor 1     --aggressor-pages 8 --a-gap-cycles 0
#   4 A8 gap1000  --aggressor 1     --aggressor-pages 8 --a-gap-cycles 1000
#
# This is an engineering check, not a formal gap sweep. It does not run the
# later full pilot (4096 transfers, 256 warmup) automatically.
#
# OUTPUT_DIR is created new; it must not already exist.
# Default OUTPUT_DIR is /tmp/p4m-$$.
#
# No dmesg clear, no network, no SD burn, no ioctl retry, no hidden failure.
# Every case requires a readable decimal taint of exactly 0 before and after
# the run; any nonzero/unknown readback stops the run and is preserved.
#
# Exit codes:
#   0            clean complete
#   1..3         first CLI/taint failure (CLI rc, 3 taint gate)
#   4            capture/output failure after an otherwise clean run
#   127          required command missing
#   129/130/143  HUP/INT/TERM signal
#   2            usage or fatal setup error
set -u

PATH=/sbin:/bin:/usr/sbin:/usr/bin
export PATH

usage() {
    cat <<'EOF'
Usage: platform-p4m short [OUTPUT_DIR]
  short  4 short P4M trials:
         1 Solo gap0
         2 A1 gap0
         3 A8 gap0
         4 A8 gap1000 engineering check
OUTPUT_DIR must not exist. Default: /tmp/p4m-$$.
Each trial uses 64 transfers, warmup 8, 4096-byte chunk.
No formal grid and no automatic 4096/256 pilot.
EOF
}

fatal() {
    echo "P4M: fatal: $*" >&2
    exit 2
}

[ $# -ge 1 ] && [ $# -le 2 ] || { usage >&2; exit 2; }
mode=$1
case "$mode" in
    short) ;;
    *) usage >&2; exit 2 ;;
esac

if [ $# -eq 2 ]; then
    OUT=$2
else
    OUT=/tmp/p4m-$$
fi

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

collection_failed=0

cleanup() {
    rc=$?
    trap - EXIT HUP INT TERM
    save_dmesg || collection_failed=1
    save_taint || collection_failed=1
    if [ "$collection_failed" -ne 0 ] && [ "$rc" -eq 0 ]; then
        rc=4
    fi
    echo "P4M: final_exit=$rc collection_failed=$collection_failed" >&2
    if [ "$rc" -eq 0 ]; then
        if ! echo "P4M: complete ok mode=$mode output=$OUT"; then
            echo "P4M: complete marker write failed" >&2
            rc=4
        fi
    fi
    exit "$rc"
}

run_case() {
    name=$1
    shift

    stdout_file="$OUT/$name.json"
    stderr_file="$OUT/$name.err"

    before_taint=$(read_taint)
    if [ "$before_taint" != "0" ]; then
        echo "P4M: case=$name before_taint=$before_taint refusing to run" >&2
        return 3
    fi

    echo "P4M: case=$name begin taint=$before_taint args=$*" >&2
    platform-timeout 120 idma_experiment "$@" > "$stdout_file" 2> "$stderr_file"
    rc=$?
    after_taint=$(read_taint)
    echo "P4M: case=$name exit=$rc taint=$after_taint" >&2

    cat_failed=0
    cat "$stdout_file" || {
        echo "P4M: case=$name stdout cat failed" >&2
        cat_failed=1
    }
    if [ -s "$stderr_file" ]; then
        cat "$stderr_file" >&2 || {
            echo "P4M: case=$name stderr cat failed" >&2
            cat_failed=1
        }
    fi

    if [ "$rc" -ne 0 ]; then
        echo "P4M: case=$name failed rc=$rc" >&2
        return "$rc"
    fi
    if [ "$cat_failed" -ne 0 ]; then
        return 4
    fi
    if [ "$after_taint" != "0" ]; then
        echo "P4M: case=$name after_taint=$after_taint stopping" >&2
        return 3
    fi
    return 0
}

trap cleanup EXIT
trap 'exit 129' HUP
trap 'exit 130' INT
trap 'exit 143' TERM

: > "$OUT/input.txt" || fatal "input write failed: $OUT/input.txt"
{
    echo "mode=$mode"
    echo "output=$OUT"
} >> "$OUT/input.txt" || fatal "input write failed: $OUT/input.txt"

if ! cat > "$OUT/matrix.txt" <<'EOF'
short:
1 Solo        --aggressor none  --a-gap-cycles 0
2 A1 gap0     --aggressor 1     --aggressor-pages 1 --a-gap-cycles 0
3 A8 gap0     --aggressor 1     --aggressor-pages 8 --a-gap-cycles 0
4 A8 gap1000  --aggressor 1     --aggressor-pages 8 --a-gap-cycles 1000
EOF
then
    fatal "matrix write failed: $OUT/matrix.txt"
fi

if ! command -v platform-timeout >/dev/null 2>&1; then
    echo "P4M: missing command: platform-timeout" >&2
    exit 127
fi
if ! command -v idma_experiment >/dev/null 2>&1; then
    echo "P4M: missing command: idma_experiment" >&2
    exit 127
fi

run_case solo --p4m --aggressor none --a-gap-cycles 0 \
    --victim-pages 1 --chunk 4096 --transfers 64 --warmup 8 --timeout-ms 10000 || exit $?
run_case a1-gap0 --p4m --aggressor 1 --aggressor-pages 1 --a-gap-cycles 0 \
    --victim-pages 1 --chunk 4096 --transfers 64 --warmup 8 --timeout-ms 10000 || exit $?
run_case a8-gap0 --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0 \
    --victim-pages 1 --chunk 4096 --transfers 64 --warmup 8 --timeout-ms 10000 || exit $?
run_case a8-gap1000 --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 1000 \
    --victim-pages 1 --chunk 4096 --transfers 64 --warmup 8 --timeout-ms 10000 || exit $?

exit 0

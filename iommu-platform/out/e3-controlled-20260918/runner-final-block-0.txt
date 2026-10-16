#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
# Fixed finite controlled E3 board runner.
#
# Usage: platform-e3-controlled short|main|all [OUTPUT_DIR]
#   short: three short pairs
#   main:  eight formal pairs. Run only after short passed on user/host.
#   all:   short, then main only if short passed.
#
# OUTPUT_DIR is created new; it must not already exist.
# Default OUTPUT_DIR is /tmp/e3-controlled-$$.
#
# No dmesg clear, no network, no SD burn, no ioctl retry, no hidden failure.
# Every case requires a readable decimal taint of exactly 0 before and after
# the run; any nonzero/unknown readback stops the run and is preserved.
#
# Raw dmesg and the final taint value are always emitted to stderr (UART)
# between explicit E3CTL begin/end markers, so the host keeps the raw capture
# even when the collection itself fails.  Nothing is filtered or de-duplicated
# here; de-duplication happens offline.  Exit codes:
#   0            clean complete
#   1..3         first CLI/taint failure (CLI rc, 3 taint gate)
#   4            capture/output failure after an otherwise clean run
#   129/130/143  HUP/INT/TERM signal
#   2            usage or fatal setup error
set -u

PATH=/sbin:/bin:/usr/sbin:/usr/bin
export PATH

usage() {
    cat <<'EOF'
Usage: platform-e3-controlled short|main|all [OUTPUT_DIR]
  short  3 short pairs
  main   8 formal pairs (only after short passed on user/host)
  all    short, then main if short passed
OUTPUT_DIR must not exist. Default: /tmp/e3-controlled-$$.
Matrix:
  short 1: --aggressor none --prep cold --pair complementary --first-delay 0
  short 2: --aggressor 1    --prep hot  --pair complementary --first-delay 0
  short 3: --aggressor 1    --prep cold --pair zero          --first-delay 0
  main 1:  --aggressor 1    --prep cold --pair zero          --first-delay 0
  main 2:  --aggressor 1    --prep cold --pair complementary --first-delay 0
  main 3:  --aggressor 1    --prep hot  --pair complementary --first-delay 0
  main 4:  --aggressor 1    --prep cold --pair complementary --first-delay 128
  main 5:  --aggressor 1    --prep cold --pair zero          --first-delay 0
  main 6:  --aggressor 1    --prep cold --pair complementary --first-delay 0
  main 7:  --aggressor 1    --prep hot  --pair complementary --first-delay 128
  main 8:  --aggressor 1    --prep cold --pair complementary --first-delay 128
EOF
}

fatal() {
    echo "E3CTL: fatal: $*" >&2
    exit 2
}

[ $# -ge 1 ] && [ $# -le 2 ] || { usage >&2; exit 2; }
mode=$1
case "$mode" in
    short|main|all) ;;
    *) usage >&2; exit 2 ;;
esac

if [ $# -eq 2 ]; then
    OUT=$2
else
    OUT=/tmp/e3-controlled-$$
fi

if [ -e "$OUT" ]; then
    fatal "output directory already exists: $OUT"
fi
mkdir "$OUT" || fatal "mkdir failed: $OUT"

# --- capture helpers -------------------------------------------------------

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

# Always store the raw ring buffer, then always echo it back to stderr (UART)
# between explicit markers.  A failed collection keeps its partial raw file
# and additionally echoes the captured stderr and the error file.
save_dmesg() {
    dmesg > "$OUT/dmesg.raw" 2> "$OUT/dmesg.err"
    dmesg_rc=$?

    if [ "$dmesg_rc" -eq 0 ]; then
        emit_rc=0
        echo "E3CTL: dmesg.raw begin" >&2 || emit_rc=1
        cat "$OUT/dmesg.raw" >&2 || emit_rc=1
        echo "E3CTL: dmesg.raw end" >&2 || emit_rc=1
        if [ "$emit_rc" -ne 0 ]; then
            echo "E3CTL: dmesg.raw cat failed" >&2
            return 4
        fi
        return 0
    fi

    echo "dmesg collection failed rc=$dmesg_rc" > "$OUT/dmesg.error"
    echo "E3CTL: dmesg collection failed rc=$dmesg_rc" >&2
    echo "E3CTL: dmesg.raw begin (partial)" >&2
    cat "$OUT/dmesg.raw" >&2 2>/dev/null || echo "E3CTL: dmesg.raw cat failed" >&2
    echo "E3CTL: dmesg.raw end (partial)" >&2
    if [ -s "$OUT/dmesg.err" ]; then
        echo "E3CTL: dmesg.err begin" >&2
        cat "$OUT/dmesg.err" >&2 2>/dev/null || echo "E3CTL: dmesg.err cat failed" >&2
        echo "E3CTL: dmesg.err end" >&2
    fi
    echo "E3CTL: dmesg.error begin" >&2
    cat "$OUT/dmesg.error" >&2 2>/dev/null || echo "E3CTL: dmesg.error cat failed" >&2
    echo "E3CTL: dmesg.error end" >&2
    return "$dmesg_rc"
}

# Read the final taint through the same reader, persist it verbatim and always
# emit it.  unknown/nonzero is a failure so a dirty final value can never leave
# the run marked "complete ok", even when the last case observed a clean 0.
save_taint() {
    final_taint=$(read_taint)
    if ! echo "$final_taint" > "$OUT/taint.txt"; then
        echo "E3CTL: taint write failed" >&2
        return 1
    fi
    echo "E3CTL: final_taint=$final_taint" >&2
    if [ "$final_taint" != "0" ]; then
        echo "E3CTL: final taint not clean (value=$final_taint)" >&2
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
    # An earlier failure (CLI rc, taint gate, signal) always wins; only an
    # otherwise clean run is upgraded to 4 for capture/output trouble.
    if [ "$collection_failed" -ne 0 ] && [ "$rc" -eq 0 ]; then
        rc=4
    fi
    echo "E3CTL: final_exit=$rc collection_failed=$collection_failed" >&2
    if [ "$rc" -eq 0 ]; then
        if ! echo "E3CTL: complete ok mode=$mode output=$OUT"; then
            echo "E3CTL: complete marker write failed" >&2
            rc=4
        fi
    fi
    exit "$rc"
}

run_case() {
    phase=$1
    index=$2
    shift 2

    stdout_file="$OUT/$phase-$index.json"
    stderr_file="$OUT/$phase-$index.err"

    before_taint=$(read_taint)
    if [ "$before_taint" != "0" ]; then
        echo "E3CTL: phase=$phase case=$index before_taint=$before_taint refusing to run" >&2
        return 3
    fi

    echo "E3CTL: phase=$phase case=$index begin taint=$before_taint args=$*" >&2
    idma_e3_controlled "$@" > "$stdout_file" 2> "$stderr_file"
    rc=$?
    after_taint=$(read_taint)
    echo "E3CTL: phase=$phase case=$index exit=$rc taint=$after_taint" >&2

    cat_failed=0
    cat "$stdout_file" || {
        echo "E3CTL: phase=$phase case=$index stdout cat failed" >&2
        cat_failed=1
    }
    if [ -s "$stderr_file" ]; then
        cat "$stderr_file" >&2 || {
            echo "E3CTL: phase=$phase case=$index stderr cat failed" >&2
            cat_failed=1
        }
    fi

    # The CLI's own failure is the first error and keeps its status.
    if [ "$rc" -ne 0 ]; then
        echo "E3CTL: phase=$phase case=$index failed rc=$rc" >&2
        return "$rc"
    fi
    if [ "$cat_failed" -ne 0 ]; then
        return 4
    fi
    if [ "$after_taint" != "0" ]; then
        echo "E3CTL: phase=$phase case=$index after_taint=$after_taint stopping" >&2
        return 3
    fi
    return 0
}

run_short() {
    run_case short 1 --aggressor none --prep cold --pair complementary --first-delay 0 || return $?
    run_case short 2 --aggressor 1 --prep hot --pair complementary --first-delay 0 || return $?
    run_case short 3 --aggressor 1 --prep cold --pair zero --first-delay 0 || return $?
    return 0
}

run_main() {
    run_case main 1 --aggressor 1 --prep cold --pair zero --first-delay 0 || return $?
    run_case main 2 --aggressor 1 --prep cold --pair complementary --first-delay 0 || return $?
    run_case main 3 --aggressor 1 --prep hot --pair complementary --first-delay 0 || return $?
    run_case main 4 --aggressor 1 --prep cold --pair complementary --first-delay 128 || return $?
    run_case main 5 --aggressor 1 --prep cold --pair zero --first-delay 0 || return $?
    run_case main 6 --aggressor 1 --prep cold --pair complementary --first-delay 0 || return $?
    run_case main 7 --aggressor 1 --prep hot --pair complementary --first-delay 128 || return $?
    run_case main 8 --aggressor 1 --prep cold --pair complementary --first-delay 128 || return $?
    return 0
}

# --- traps armed as soon as the fresh output directory exists ---------------
# (functions above are defined first so the handlers always exist)

trap cleanup EXIT
trap 'exit 129' HUP
trap 'exit 130' INT
trap 'exit 143' TERM

# --- run inputs and the frozen matrix (failures still capture logs) ---------

: > "$OUT/input.txt" || fatal "input write failed: $OUT/input.txt"
{
    echo "mode=$mode"
    echo "output=$OUT"
} >> "$OUT/input.txt" || fatal "input write failed: $OUT/input.txt"

if ! cat > "$OUT/matrix.txt" <<'EOF'
short:
1 --aggressor none --prep cold --pair complementary --first-delay 0
2 --aggressor 1 --prep hot --pair complementary --first-delay 0
3 --aggressor 1 --prep cold --pair zero --first-delay 0
main:
1 --aggressor 1 --prep cold --pair zero --first-delay 0
2 --aggressor 1 --prep cold --pair complementary --first-delay 0
3 --aggressor 1 --prep hot --pair complementary --first-delay 0
4 --aggressor 1 --prep cold --pair complementary --first-delay 128
5 --aggressor 1 --prep cold --pair zero --first-delay 0
6 --aggressor 1 --prep cold --pair complementary --first-delay 0
7 --aggressor 1 --prep hot --pair complementary --first-delay 128
8 --aggressor 1 --prep cold --pair complementary --first-delay 128
EOF
then
    fatal "matrix write failed: $OUT/matrix.txt"
fi

case "$mode" in
    short)
        run_short || exit $?
        ;;
    main)
        echo "E3CTL: warning: main requires that short already passed on user/host" >&2
        run_main || exit $?
        ;;
    all)
        run_short || exit $?
        echo "E3CTL: short passed; starting main" >&2
        run_main || exit $?
        ;;
esac

exit 0

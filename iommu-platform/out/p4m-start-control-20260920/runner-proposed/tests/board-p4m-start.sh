#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
# P4M software batch-start control board runner (short block + comparison block).
#
# Usage: platform-p4m-start MODE [OUTPUT_DIR]
#
# Protocol and field semantics: P4M_START_CONTROL.md.
# No FPGA bitstream and no SD/MMC media is written. The only benchmark this
# runner invokes is the frozen idma_experiment case list below.
#
# Modes (MODE is required; there is no implicit mode):
#   short    4 trials, 64 transfers, warmup 8, 4096-byte chunk:
#               s1 Solo        --p4m --aggressor none --a-gap-cycles 0
#               s2 A1 gap0     --p4m --aggressor 1 --aggressor-pages 1 --a-gap-cycles 0
#               s3 A8 gap0     --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0
#               s4 A8 gap1000  --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 1000
#   compare  8 trials, 4096 transfers, warmup 256, 4096-byte chunk, frozen order
#            B8 P8g0 P8g0 B8 B8 P8g0 P8g0 B8, where
#              B8   = --batch --aggressor 1 --aggressor-pages 8 (ordinary batch,
#                     no P4M bank; the startup record is still mandatory)
#              P8g0 = --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0
#   all     short block first, then the comparison block only when every short
#           trial passed: 12 idma_experiment runs in total. Recommended, but it
#           must be requested explicitly.
#
# Every trial runs as
#   platform-timeout 120 idma_experiment --device /dev/idma0 --victim-pages 1
#     --chunk 4096 --transfers <64|4096> --warmup <8|256> --timeout-ms 60000
#     <condition args>
# with victim device idma0 (DID 10, configured aggressor DID 11). There is no
# automatic retry, no extra trial and no hidden failure: the first failure stops
# the run and all evidence is kept.
#
# Pre-DMA gates, before the first idma_experiment call:
#   * required commands present: platform-timeout idma_experiment dmesg uname awk
#   * uname -r does not match KERNEL_OLD_MATCH (the known old baseline revision;
#     the new revision hash is deliberately not hardcoded)
#   * path_observe reads Y when the plan contains a --p4m case
#   * taint reads exactly 0
#
# Per-trial gates: taint exactly 0 before and after the CLI call, and
# path_observe Y for --p4m cases. Ordinary --batch forces the observation path,
# so B8 trials record the value but are not required to read Y.
#
# Evidence kept for every trial even on failure (nothing is deleted):
#   <case>.json           CLI stdout, raw
#   <case>.err            CLI stderr, raw
#   <case>.dmesg.before   dmesg snapshot taken before the CLI call
#   <case>.dmesg.raw      dmesg snapshot taken after the CLI call
#   <case>.startup        the single new verified idma_batch_start record
#   argv.txt input.txt matrix.txt params.txt path_observe.txt
#   dmesg.raw dmesg.err taint.txt (final capture written by cleanup)
#
# Per-trial verification, fail closed (any mismatch stops the run with exit 5):
#   * exactly one NEW line containing "idma_batch_start " appears between the
#     before and after snapshots; a missing, stale or reused record is refused
#   * that record has the mandated prefix "idma_batch_start version=1 seq=" and
#     every contract field, method=irq_pair, attempted=1, v_triggered=1,
#     v_did=10, error=0, integer fields, ordered timestamps, positive irq_span_ns
#   * solo trials must report a_did=4294967295 and zero A fields; aggressor
#     trials must report a_did=11, has_a=1, a_triggered=1 and
#     a_begin_ns <= a_end_ns <= v_begin_ns <= v_end_ns with
#     a_to_v_ns = v_begin_ns - a_begin_ns
#   * when the CLI JSON reports a top-level "sequence" it must equal the seq of
#     the new dmesg record, so a reused sequence cannot pass
#   * the CLI JSON must report success=true, batch_polled=1 and the exact V
#     issued/done counts for the trial; --p4m trials additionally require
#     time_valid=1 and status=0x4. B8 trials have no P4M bank, so the P4M window
#     fields are not required there.
#   * opening and progress ids are recorded (the raw JSON is emitted) but never
#     compared with expected values: a different opening id and zero A progress
#     are not failures.
#
# Exit codes:
#   0            clean complete
#   1..3         first CLI failure (CLI rc); 3 also for a taint/path_observe gate
#   4            capture/collection/output failure
#   5            per-trial verification failure (startup record or CLI JSON)
#   127          required command missing
#   129/130/143  HUP/INT/TERM signal
#   2            usage or fatal setup error
set -u

PATH=/sbin:/bin:/usr/sbin:/usr/bin
export PATH

TAINT_PATH=/proc/sys/kernel/tainted
PATH_OBSERVE_PATH=/sys/module/idma/parameters/path_observe

# Known old baseline revision. Anything whose uname -r contains this is refused
# before any DMA because it predates the batch-start record.
KERNEL_OLD_MATCH=00088

VICTIM_DEVICE=/dev/idma0
V_DID=10
A_DID=11
NO_AGGRESSOR=4294967295
CHUNK=4096
DRIVER_TIMEOUT_MS=60000
WATCHDOG=120

SHORT_TRANSFERS=64
SHORT_WARMUP=8
LONG_TRANSFERS=4096
LONG_WARMUP=256

usage() {
    cat <<'EOF'
Usage: platform-p4m-start MODE [OUTPUT_DIR]
  MODE is required; there is no implicit mode.
    short    4 P4M trials, 64 transfers, warmup 8, 4096-byte chunk:
               1 Solo        --p4m --aggressor none --a-gap-cycles 0
               2 A1 gap0     --p4m --aggressor 1 --aggressor-pages 1 --a-gap-cycles 0
               3 A8 gap0     --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0
               4 A8 gap1000  --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 1000
    compare  8 trials, 4096 transfers, warmup 256, 4096-byte chunk, frozen order
             B8 P8g0 P8g0 B8 B8 P8g0 P8g0 B8
               B8   --batch --aggressor 1 --aggressor-pages 8 (no P4M bank)
               P8g0 --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0
    all      short block then the comparison block, but only when every short
             trial passed: 12 idma_experiment runs total. Recommended; request
             it explicitly.
  OUTPUT_DIR must not already exist. Default: /tmp/p4m-start-$$.
  Every trial is wrapped in "platform-timeout 120" and uses --device /dev/idma0
  --victim-pages 1 --chunk 4096 --transfers <64|4096> --warmup <8|256>
  --timeout-ms 60000.
  No retry, no other benchmark, no FPGA or SD write; the first failure stops.
  Per-trial evidence is kept under OUTPUT_DIR even on failure.
EOF
}

fatal() {
    echo "P4M: fatal: $*" >&2
    exit 2
}

[ $# -ge 1 ] && [ $# -le 2 ] || { usage >&2; exit 2; }
mode=$1
case "$mode" in
    short)   TOTAL_CASES=4;  plan_has_p4m=1 ;;
    compare) TOTAL_CASES=8;  plan_has_p4m=1 ;;
    all)     TOTAL_CASES=12; plan_has_p4m=1 ;;
    *) usage >&2; exit 2 ;;
esac

if [ $# -eq 2 ]; then
    OUT=$2
else
    OUT=/tmp/p4m-start-$$
fi
[ -n "$OUT" ] || { usage >&2; exit 2; }
if [ -e "$OUT" ]; then
    fatal "output directory already exists: $OUT"
fi
mkdir "$OUT" || fatal "mkdir failed: $OUT"

read_taint() {
    if value=$(cat "$TAINT_PATH" 2>/dev/null); then
        case "$value" in
            ''|*[!0-9]*) echo unknown ;;
            *) echo "$value" ;;
        esac
    else
        echo unknown
    fi
}

read_path_observe() {
    if value=$(cat "$PATH_OBSERVE_PATH" 2>/dev/null); then
        case "$value" in
            Y|y) echo Y ;;
            N|n) echo N ;;
            *) echo unknown ;;
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

# condition -> "<kind> <mode> <extra args>"
# kind: solo | pair (pair = an aggressor is configured)
# mode: p4m | batch (batch = ordinary --batch, no P4M bank)
condition_spec() {
    case "$1" in
        S)       echo "solo p4m --p4m --aggressor none --a-gap-cycles 0" ;;
        P1g0)    echo "pair p4m --p4m --aggressor 1 --aggressor-pages 1 --a-gap-cycles 0" ;;
        P8g0)    echo "pair p4m --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0" ;;
        P8g1000) echo "pair p4m --p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 1000" ;;
        B8)      echo "pair batch --batch --aggressor 1 --aggressor-pages 8" ;;
        *)       return 1 ;;
    esac
    return 0
}

# Strip one surrounding pair of double quotes from a JSON string scalar.
unquote() {
    uq_value=$1
    case "$uq_value" in
        \"*\") uq_value=${uq_value#\"}; uq_value=${uq_value%\"} ;;
    esac
    printf '%s\n' "$uq_value"
}

# Print the decimal integer form of a scalar, accepting 0x-prefixed hex.
# Prints MISSING when the scalar is not an integer. Used for the narrow CLI
# JSON checks only; ids are compared as exact strings by the runner.
int_of() {
    awk -v s="$1" '
    function hex2dec(h,   i, n, c, d, v) {
        n = length(h); v = 0
        for (i = 1; i <= n; i++) {
            c = substr(h, i, 1)
            if (c >= "0" && c <= "9") d = c + 0
            else if (c >= "a" && c <= "f") d = index("abcdef", c) + 9
            else if (c >= "A" && c <= "F") d = index("ABCDEF", c) + 9
            else return "MISSING"
            v = v * 16 + d
        }
        return v
    }
    BEGIN {
        if (s ~ /^-?[0-9]+$/) { printf "%d\n", s + 0; exit 0 }
        if (s ~ /^0x[0-9a-fA-F]+$/) { printf "%d\n", hex2dec(substr(s, 3)); exit 0 }
        if (s ~ /^-0x[0-9a-fA-F]+$/) { printf "-%d\n", hex2dec(substr(s, 4)); exit 0 }
        print "MISSING"
    }
    '
}

# Narrow top-level scalar reader for the frozen CLI JSON. This is deliberately
# NOT a general JSON parser: it recognizes only "key": <scalar> pairs at object
# depth 1 and prints the raw scalar text (quotes kept for strings). MISSING is
# printed when the key is absent or the value is an object/array.
json_scalar() {
    awk -v key="$2" '
    BEGIN { want = "\"" key "\"" }
    { text = text " " $0 }
    END {
        n = length(text)
        depth = 0; instr = 0; esc = 0
        for (i = 1; i <= n; i++) {
            c = substr(text, i, 1)
            if (instr) {
                if (esc) { esc = 0; continue }
                if (c == "\\") { esc = 1; continue }
                if (c != "\"") continue
                instr = 0
                if (depth != 1) continue
                if (substr(text, start, i - start + 1) != want) continue
                j = i + 1
                while (j <= n && substr(text, j, 1) == " ") j++
                if (j > n || substr(text, j, 1) != ":") continue
                j++
                while (j <= n && substr(text, j, 1) == " ") j++
                if (j > n) continue
                if (substr(text, j, 1) == "\"") {
                    v = "\""; j++
                    while (j <= n) {
                        ch = substr(text, j, 1)
                        if (ch == "\\") { v = v ch substr(text, j + 1, 1); j += 2; continue }
                        v = v ch
                        if (ch == "\"") break
                        j++
                    }
                    print v
                    exit 0
                }
                v = ""
                while (j <= n) {
                    ch = substr(text, j, 1)
                    if (ch == "," || ch == "}" || ch == "]" || ch == " " || ch == "\t") break
                    v = v ch
                    j++
                }
                print v
                exit 0
            }
            if (c == "\"") { instr = 1; start = i; continue }
            if (c == "{" || c == "[") { depth++; continue }
            if (c == "}" || c == "]") { depth--; continue }
        }
        print "MISSING"
    }
    ' "$1"
}

# Count the batch-start records in a dmesg snapshot (any version, so that a
# wrong-version record is still counted as "the new line" and then fails field
# validation instead of being reported as missing).
startup_count() {
    awk '/idma_batch_start / { n++ } END { print n + 0 }' "$1"
}

# Require exactly one NEW batch-start record between the before and after
# snapshots and extract it. Fail closed on a stale, reused or missing record.
locate_new_startup() {
    lns_id=$1
    lns_before=$2
    lns_after=$3
    lns_dest=$4

    lns_n_before=$(startup_count "$lns_before") || lns_n_before="bad"
    lns_n_after=$(startup_count "$lns_after") || lns_n_after="bad"
    case "$lns_n_before" in
        ''|*[!0-9]*)
            echo "P4M: case=$lns_id verify_failed=startup_count_before_unreadable file=$lns_before" >&2
            return 1 ;;
    esac
    case "$lns_n_after" in
        ''|*[!0-9]*)
            echo "P4M: case=$lns_id verify_failed=startup_count_after_unreadable file=$lns_after" >&2
            return 1 ;;
    esac

    lns_want=$((lns_n_before + 1))
    if [ "$lns_n_after" != "$lns_want" ]; then
        echo "P4M: case=$lns_id verify_failed=startup_line_count before=$lns_n_before after=$lns_n_after want=$lns_want" >&2
        return 1
    fi
    if ! awk '/idma_batch_start / { last = $0 } END { if (last == "") exit 1; print last }' \
            "$lns_after" > "$lns_dest"; then
        echo "P4M: case=$lns_id verify_failed=startup_line_extract file=$lns_after" >&2
        return 1
    fi
    return 0
}

# Validate the new batch-start record against the frozen log contract.
# Prints the canonical field summary on success; prints verify_failed lines and
# returns 1 on any mismatch. Unknown extra fields are accepted (ignored);
# optional a_id/v_id, when present, must be nonzero integers.
verify_startup() {
    vs_id=$1
    vs_file=$2
    vs_kind=$3
    vs_json_seq=$4

    vs_out=$(awk -v id="$vs_id" -v kind="$vs_kind" -v json_seq="$vs_json_seq" '
    function fail(m) { printf "P4M: case=%s verify_failed=%s\n", id, m; bad = 1 }
    function isint(s) { return s ~ /^-?[0-9]+$/ }
    BEGIN {
        nreq = split("version seq method v_did a_did attempted has_a a_triggered v_triggered a_begin_ns a_end_ns v_begin_ns v_end_ns irq_span_ns a_to_v_ns error", req, " ")
        for (i = 1; i <= nreq; i++) need[req[i]] = 1
        opt["a_id"] = 1
        opt["v_id"] = 1
    }
    {
        if ($0 !~ /idma_batch_start version=1 seq=/) fail("startup_prefix_not_found")
        n = split($0, tok, "[ \t]+")
        for (i = 1; i <= n; i++) {
            t = tok[i]
            if (t !~ /^[A-Za-z_][A-Za-z0-9_]*=/) continue
            p = index(t, "=")
            val[substr(t, 1, p - 1)] = substr(t, p + 1)
        }
        lines++
    }
    END {
        if (lines == 0) { fail("startup_line_empty"); exit 5 }
        for (k in need) {
            if (!(k in val)) { fail("field_missing_" k); continue }
            if (k == "method") {
                if (val[k] != "irq_pair") fail("method_not_irq_pair")
                continue
            }
            if (!isint(val[k])) fail("field_not_integer_" k)
        }
        if (bad) exit 5
        if (val["version"] + 0 != 1) fail("version_not_1")
        if (val["attempted"] + 0 != 1) fail("attempted_not_1")
        if (val["error"] + 0 != 0) fail("error_not_0")
        if (val["v_triggered"] + 0 != 1) fail("v_triggered_not_1")
        if (val["v_did"] != "10") fail("v_did_not_10")
        if (val["seq"] + 0 <= 0) fail("seq_not_positive")
        if (val["v_begin_ns"] + 0 <= 0) fail("v_begin_ns_not_positive")
        if (val["v_end_ns"] + 0 < val["v_begin_ns"] + 0) fail("v_time_order")
        if (val["irq_span_ns"] + 0 <= 0) fail("irq_span_ns_not_positive")
        if (kind == "solo") {
            if (val["a_did"] != "4294967295") fail("solo_a_did_not_no_aggressor")
            if (val["has_a"] + 0 != 0) fail("solo_has_a_not_0")
            if (val["a_triggered"] + 0 != 0) fail("solo_a_triggered_not_0")
            if (val["a_begin_ns"] + 0 != 0) fail("solo_a_begin_ns_not_0")
            if (val["a_end_ns"] + 0 != 0) fail("solo_a_end_ns_not_0")
            if (val["a_to_v_ns"] + 0 != 0) fail("solo_a_to_v_ns_not_0")
        } else {
            if (val["a_did"] != "11") fail("pair_a_did_not_11")
            if (val["has_a"] + 0 != 1) fail("pair_has_a_not_1")
            if (val["a_triggered"] + 0 != 1) fail("pair_a_triggered_not_1")
            if (val["a_begin_ns"] + 0 <= 0) fail("pair_a_begin_ns_not_positive")
            if (val["a_end_ns"] + 0 < val["a_begin_ns"] + 0) fail("pair_a_time_order")
            if (val["v_begin_ns"] + 0 < val["a_end_ns"] + 0) fail("pair_a_end_after_v_begin")
            if (val["a_to_v_ns"] + 0 != val["v_begin_ns"] + 0 - val["a_begin_ns"] + 0) fail("pair_a_to_v_ns_mismatch")
        }
        if (json_seq != "" && isint(json_seq) && val["seq"] + 0 != json_seq + 0) fail("seq_json_mismatch")
        for (k in opt) {
            if (!(k in val)) continue
            if (!isint(val[k]) || val[k] + 0 == 0) fail("optional_" k "_zero_or_not_integer")
        }
        if (bad) exit 5
        printf "version=%s seq=%s method=%s v_did=%s a_did=%s attempted=%s has_a=%s a_triggered=%s v_triggered=%s a_begin_ns=%s a_end_ns=%s v_begin_ns=%s v_end_ns=%s irq_span_ns=%s a_to_v_ns=%s error=%s\n", val["version"], val["seq"], val["method"], val["v_did"], val["a_did"], val["attempted"], val["has_a"], val["a_triggered"], val["v_triggered"], val["a_begin_ns"], val["a_end_ns"], val["v_begin_ns"], val["v_end_ns"], val["irq_span_ns"], val["a_to_v_ns"], val["error"]
    }
    ' "$vs_file")

    vs_rc=$?
    if [ "$vs_rc" -ne 0 ] || [ -z "$vs_out" ]; then
        if [ -z "$vs_out" ]; then
            echo "P4M: case=$vs_id verify_failed=startup_record_unreadable file=$vs_file" >&2
        else
            printf '%s\n' "$vs_out" >&2
        fi
        return 1
    fi
    printf '%s\n' "$vs_out"
    return 0
}

# Narrow CLI JSON checks. Key names mirror the frozen CLI output; adjust only
# this function if the CLI names differ. Opening/aggressor ids and progress are
# never compared with expected values here.
check_case_json() {
    cj_id=$1
    cj_file=$2
    cj_mode=$3
    cj_transfers=$4

    if [ ! -s "$cj_file" ]; then
        echo "P4M: case=$cj_id verify_failed=cli_json_missing_or_empty file=$cj_file" >&2
        return 1
    fi

    cj_v=$(unquote "$(json_scalar "$cj_file" success)")
    if [ "$cj_v" != "true" ]; then
        echo "P4M: case=$cj_id verify_failed=json_success_not_true got=$cj_v" >&2
        return 1
    fi

    cj_v=$(int_of "$(unquote "$(json_scalar "$cj_file" batch_polled)")")
    if [ "$cj_v" != "1" ]; then
        echo "P4M: case=$cj_id verify_failed=json_batch_polled_not_1 got=$cj_v" >&2
        return 1
    fi

    cj_v=$(int_of "$(unquote "$(json_scalar "$cj_file" v_issued)")")
    if [ "$cj_v" != "$cj_transfers" ]; then
        echo "P4M: case=$cj_id verify_failed=json_v_issued_mismatch got=$cj_v want=$cj_transfers" >&2
        return 1
    fi

    cj_v=$(int_of "$(unquote "$(json_scalar "$cj_file" v_done)")")
    if [ "$cj_v" != "$cj_transfers" ]; then
        echo "P4M: case=$cj_id verify_failed=json_v_done_mismatch got=$cj_v want=$cj_transfers" >&2
        return 1
    fi

    if [ "$cj_mode" = "p4m" ]; then
        cj_v=$(int_of "$(unquote "$(json_scalar "$cj_file" time_valid)")")
        if [ "$cj_v" != "1" ]; then
            echo "P4M: case=$cj_id verify_failed=json_time_valid_not_1 got=$cj_v" >&2
            return 1
        fi
        cj_v=$(int_of "$(unquote "$(json_scalar "$cj_file" status)")")
        if [ "$cj_v" != "4" ]; then
            echo "P4M: case=$cj_id verify_failed=json_status_not_4 got=$cj_v" >&2
            return 1
        fi
    fi
    return 0
}

run_case() {
    rc_id=$1
    rc_cond=$2
    rc_transfers=$3
    rc_warmup=$4

    if ! rc_spec=$(condition_spec "$rc_cond"); then
        echo "P4M: internal error unknown condition=$rc_cond" >&2
        return 2
    fi
    rc_kind=${rc_spec%% *}
    rc_rest=${rc_spec#* }
    rc_mode=${rc_rest%% *}
    rc_extra=${rc_rest#* }

    rc_json="$OUT/$rc_id.json"
    rc_err="$OUT/$rc_id.err"
    rc_before="$OUT/$rc_id.dmesg.before"
    rc_after="$OUT/$rc_id.dmesg.raw"
    rc_startup="$OUT/$rc_id.startup"

    rc_common="--device $VICTIM_DEVICE --victim-pages 1 --chunk $CHUNK --transfers $rc_transfers --warmup $rc_warmup --timeout-ms $DRIVER_TIMEOUT_MS"
    rc_argv="platform-timeout $WATCHDOG idma_experiment $rc_common $rc_extra"

    rc_before_taint=$(read_taint)
    rc_observe=$(read_path_observe)
    if ! printf '%s %s\n' "$rc_id" "$rc_observe" >> "$OUT/path_observe.txt"; then
        echo "P4M: case=$rc_id path_observe_record_failed" >&2
        return 4
    fi

    if [ "$rc_before_taint" != "0" ]; then
        echo "P4M: case=$rc_id gate=taint_before value=$rc_before_taint refusing to run" >&2
        return 3
    fi
    if [ "$rc_mode" = "p4m" ] && [ "$rc_observe" != "Y" ]; then
        echo "P4M: case=$rc_id gate=path_observe value=$rc_observe refusing to run (--p4m case requires Y)" >&2
        return 3
    fi

    if ! dmesg > "$rc_before" 2> "$OUT/$rc_id.dmesg.before.err"; then
        echo "P4M: case=$rc_id collection=dmesg_before_failed" >&2
        return 4
    fi

    if ! printf 'id=%s condition=%s kind=%s mode=%s transfers=%s warmup=%s argv=%s\n' \
            "$rc_id" "$rc_cond" "$rc_kind" "$rc_mode" "$rc_transfers" "$rc_warmup" "$rc_argv" \
            >> "$OUT/argv.txt"; then
        echo "P4M: case=$rc_id argv_record_failed" >&2
        return 4
    fi

    echo "P4M: case_begin id=$rc_id condition=$rc_cond kind=$rc_kind mode=$rc_mode taint_before=$rc_before_taint path_observe=$rc_observe total=$TOTAL_CASES argv=$rc_argv" >&2

    # shellcheck disable=SC2086
    platform-timeout "$WATCHDOG" idma_experiment $rc_common $rc_extra > "$rc_json" 2> "$rc_err"
    rc_rc=$?
    rc_after_taint=$(read_taint)

    rc_cat_failed=0
    echo "P4M: case_stdout_begin id=$rc_id" >&2
    cat "$rc_json" || { echo "P4M: case_stdout_cat_failed id=$rc_id" >&2; rc_cat_failed=1; }
    echo "P4M: case_stdout_end id=$rc_id" >&2

    echo "P4M: case_stderr_begin id=$rc_id" >&2
    if [ -s "$rc_err" ]; then
        cat "$rc_err" >&2 || { echo "P4M: case_stderr_cat_failed id=$rc_id" >&2; rc_cat_failed=1; }
    fi
    echo "P4M: case_stderr_end id=$rc_id" >&2

    if ! dmesg > "$rc_after" 2> "$OUT/$rc_id.dmesg.err"; then
        echo "P4M: case=$rc_id collection=dmesg_after_failed" >&2
        return 4
    fi
    echo "P4M: dmesg.raw_begin id=$rc_id" >&2
    cat "$rc_after" >&2 || { echo "P4M: dmesg.raw_cat_failed id=$rc_id" >&2; rc_cat_failed=1; }
    echo "P4M: dmesg.raw_end id=$rc_id" >&2

    echo "P4M: case_exit id=$rc_id exit=$rc_rc taint_after=$rc_after_taint" >&2

    if [ "$rc_rc" -ne 0 ]; then
        echo "P4M: case_failed id=$rc_id exit=$rc_rc" >&2
        return "$rc_rc"
    fi
    if [ "$rc_cat_failed" -ne 0 ]; then
        echo "P4M: case=$rc_id collection=emit_failed" >&2
        return 4
    fi
    if [ "$rc_after_taint" != "0" ]; then
        echo "P4M: case=$rc_id gate=taint_after value=$rc_after_taint stopping" >&2
        return 3
    fi

    if ! locate_new_startup "$rc_id" "$rc_before" "$rc_after" "$rc_startup"; then
        return 5
    fi

    if ! check_case_json "$rc_id" "$rc_json" "$rc_mode" "$rc_transfers"; then
        return 5
    fi

    rc_json_seq=$(int_of "$(unquote "$(json_scalar "$rc_json" sequence)")")
    if ! rc_sfields=$(verify_startup "$rc_id" "$rc_startup" "$rc_kind" "$rc_json_seq"); then
        return 5
    fi
    echo "P4M: case_startup id=$rc_id $rc_sfields" >&2

    cases_done=$((cases_done + 1))
    echo "P4M: case_end id=$rc_id exit=0 cases_done=$cases_done total=$TOTAL_CASES" >&2
    return 0
}

run_short_block() {
    run_case "start-s1-solo" S "$SHORT_TRANSFERS" "$SHORT_WARMUP" || return $?
    run_case "start-s2-a1-gap0" P1g0 "$SHORT_TRANSFERS" "$SHORT_WARMUP" || return $?
    run_case "start-s3-a8-gap0" P8g0 "$SHORT_TRANSFERS" "$SHORT_WARMUP" || return $?
    run_case "start-s4-a8-gap1000" P8g1000 "$SHORT_TRANSFERS" "$SHORT_WARMUP" || return $?
    return 0
}

run_compare_block() {
    run_case "start-c1-b8" B8 "$LONG_TRANSFERS" "$LONG_WARMUP" || return $?
    run_case "start-c2-p8g0" P8g0 "$LONG_TRANSFERS" "$LONG_WARMUP" || return $?
    run_case "start-c3-p8g0" P8g0 "$LONG_TRANSFERS" "$LONG_WARMUP" || return $?
    run_case "start-c4-b8" B8 "$LONG_TRANSFERS" "$LONG_WARMUP" || return $?
    run_case "start-c5-b8" B8 "$LONG_TRANSFERS" "$LONG_WARMUP" || return $?
    run_case "start-c6-p8g0" P8g0 "$LONG_TRANSFERS" "$LONG_WARMUP" || return $?
    run_case "start-c7-p8g0" P8g0 "$LONG_TRANSFERS" "$LONG_WARMUP" || return $?
    run_case "start-c8-b8" B8 "$LONG_TRANSFERS" "$LONG_WARMUP" || return $?
    return 0
}

collection_failed=0
cases_done=0

trap cleanup EXIT
trap 'exit 129' HUP
trap 'exit 130' INT
trap 'exit 143' TERM

cleanup() {
    rc=$?
    trap - EXIT HUP INT TERM
    save_dmesg || collection_failed=1
    save_taint || collection_failed=1
    if [ "$collection_failed" -ne 0 ] && [ "$rc" -eq 0 ]; then
        rc=4
    fi
    echo "P4M: final_exit=$rc collection_failed=$collection_failed mode=$mode cases_done=$cases_done total=$TOTAL_CASES output=$OUT" >&2
    if [ "$rc" -eq 0 ] && [ "$cases_done" -eq "$TOTAL_CASES" ]; then
        if ! echo "P4M: complete ok mode=$mode cases=$cases_done total=$TOTAL_CASES output=$OUT"; then
            echo "P4M: complete marker write failed" >&2
            rc=4
        fi
    fi
    exit "$rc"
}

if ! : > "$OUT/argv.txt"; then
    fatal "argv write failed: $OUT/argv.txt"
fi
if ! : > "$OUT/path_observe.txt"; then
    fatal "path_observe record write failed: $OUT/path_observe.txt"
fi

if ! {
    echo "mode=$mode"
    echo "output=$OUT"
    echo "kernel_old_match=$KERNEL_OLD_MATCH"
    echo "kernel_expect=newer than the old baseline; the new hash is not hardcoded"
    echo "cases_total=$TOTAL_CASES"
    echo "watchdog_seconds=$WATCHDOG driver_timeout_ms=$DRIVER_TIMEOUT_MS"
    echo "short=4x transfers=$SHORT_TRANSFERS warmup=$SHORT_WARMUP chunk=$CHUNK"
    echo "compare=8x transfers=$LONG_TRANSFERS warmup=$LONG_WARMUP chunk=$CHUNK"
    echo "victim_device=$VICTIM_DEVICE victim_did=$V_DID aggressor_did=$A_DID no_aggressor=$NO_AGGRESSOR"
} > "$OUT/input.txt"; then
    fatal "input write failed: $OUT/input.txt"
fi

if ! {
    echo "P4M software batch-start frozen plan ($mode)"
    echo "order_short=S P1g0 P8g0 P8g1000"
    echo "order_compare=B8 P8g0 P8g0 B8 B8 P8g0 P8g0 B8"
    echo "condition_specs (kind mode argv):"
    for cond in S P1g0 P8g0 P8g1000 B8; do
        echo "  $cond $(condition_spec "$cond")"
    done
    echo "compare_block_runs_only_when_short_block_passed=1"
} > "$OUT/matrix.txt"; then
    fatal "matrix write failed: $OUT/matrix.txt"
fi

kernel_release=$(uname -r)
kernel_all=$(uname -a)
observe=$(read_path_observe)
taint_baseline=$(read_taint)

if ! {
    echo "runner=platform-p4m-start"
    echo "mode=$mode"
    echo "output=$OUT"
    echo "date_utc=$(date -u +%Y-%m-%dT%H:%M:%SZ 2>/dev/null || echo unknown)"
    echo "time_format=UTC ISO-8601 seconds (date -u +%Y-%m-%dT%H:%M:%SZ)"
    echo "case_time_source=dmesg default kernel timestamp; no per-case wall clock is stored"
    echo "uname=$kernel_all"
    echo "kernel_release=$kernel_release"
    echo "kernel_old_match=$KERNEL_OLD_MATCH"
    echo "taint_baseline=$taint_baseline"
    echo "taint_path=$TAINT_PATH"
    echo "path_observe=$observe"
    echo "path_observe_path=$PATH_OBSERVE_PATH"
    echo "path=$PATH"
    echo "watchdog_seconds=$WATCHDOG driver_timeout_ms=$DRIVER_TIMEOUT_MS"
    echo "chunk=$CHUNK victim_device=$VICTIM_DEVICE victim_did=$V_DID aggressor_did=$A_DID"
    echo "short_transfers=$SHORT_TRANSFERS short_warmup=$SHORT_WARMUP"
    echo "compare_transfers=$LONG_TRANSFERS compare_warmup=$LONG_WARMUP"
    echo "cases_total=$TOTAL_CASES"
} > "$OUT/params.txt"; then
    fatal "params write failed: $OUT/params.txt"
fi

for cmd in platform-timeout idma_experiment dmesg uname awk; do
    if ! command -v "$cmd" >/dev/null 2>&1; then
        echo "P4M: missing command: $cmd" >&2
        exit 127
    fi
done

case "$kernel_release" in
    *"$KERNEL_OLD_MATCH"*)
        fatal "old baseline kernel running ($kernel_release matches $KERNEL_OLD_MATCH); refusing before any DMA"
        ;;
esac

printf 'preflight %s\n' "$observe" >> "$OUT/path_observe.txt"
if [ "$plan_has_p4m" = "1" ] && [ "$observe" != "Y" ]; then
    echo "P4M: path_observe gate failed value=$observe ($PATH_OBSERVE_PATH); --p4m cases require Y before any DMA" >&2
    exit 3
fi
if [ "$taint_baseline" != "0" ]; then
    echo "P4M: taint gate failed value=$taint_baseline; refusing before any DMA" >&2
    exit 3
fi

case "$mode" in
    short)
        run_short_block || exit $?
        ;;
    compare)
        run_compare_block || exit $?
        ;;
    all)
        run_short_block || exit $?
        echo "P4M: short_block complete; starting comparison block" >&2
        run_compare_block || exit $?
        ;;
esac

exit 0

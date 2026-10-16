#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
#
# Passive research board runner: frozen E2 entry-lifecycle and natural-cost
# case list driven through the frozen idma_research CLI.
#
# Usage: board-research.sh MODE [OUTPUT_DIR]
#   MODE is required and has no implicit default:
#     short          run the 6 frozen short-stage cases (ordinals 1..6)
#     all            run all 84 frozen cases (6 short + 60 main E2 + 18 main cost)
#     cost-followup  run the fixed 11-case cost follow-up segment selected from
#                    the same frozen 84-row plan: the two frozen short-stage
#                    cost cases (short-cost-solo, short-cost-a8g0) plus the
#                    main-stage cost cases whose condition is Solo/A8g0/
#                    A8g2000 (three blocks each), each with its original id,
#                    ordinal and per-block order.  This is an independent
#                    software collection segment: it does not complete the
#                    frozen 84-case run, and its new Solo data must not be
#                    merged with the earlier non-Solo data as one slowdown
#                    comparison.
#
# Rules:
#   * The case list comes from one frozen plan file (RESEARCH_PLAN, default
#     /etc/platform-research-plan.tsv; when RESEARCH_PLAN is unset and the
#     default is unreadable, a sibling research-plan.tsv next to this script is
#     used and the chosen source is logged). The plan bytes that are validated
#     and executed are copied once into OUTPUT_DIR/plan.copy.tsv and every later
#     step (header check, data rows, per-case argv) reads that copy, so editing
#     the original after the copy cannot change the run. plan.sha256 records the
#     sha256 of the copy, never of the mutable original; when no sha256 tool is
#     available that is recorded honestly instead of leaving the file absent.
#   * Every selected case runs at most once in frozen order. There is no retry,
#     no rerun and no automatic resume: the first failure stops the run, keeps
#     every artifact already written and exits non-zero.
#   * A case passes only when the CLI exits 0, its captured stdout artifact is
#     non-empty and the kernel taint value was exactly 0 before and after the
#     call. This runner never parses, counts or greps the CLI output: hardware
#     schema, bit and completion-count checks and driver errno consistency are
#     the CLI's own contract and the CLI must exit non-zero when they fail; that
#     status is propagated as-is. The runner-side contract is intentionally
#     bounded to "CLI exit 0 + non-empty stdout artifact + taint 0".
#   * The console stays small: one compact result line per successful case and a
#     bounded tail of the case stderr on failure. The complete stdout, stderr
#     and dmesg files always stay in OUTPUT_DIR so the existing board
#     return-file archival collects them unchanged.
#   * dmesg is collected raw before and after every case and stored as-is. It is
#     evidence only: this runner makes no zero-warning claim about it.
#
# Environment overrides (unset on the board; the host tests use them to inject
# stubs):
#   RESEARCH_PLAN   plan TSV path       (default /etc/platform-research-plan.tsv)
#   RESEARCH_CLI    CLI command/path    (default idma_research)
#   RESEARCH_TAINT  taint file          (default /proc/sys/kernel/tainted)
#   RESEARCH_DMESG  dmesg command/path  (default dmesg)
#   RESEARCH_UNAME  uname command/path  (default uname)
#   RESEARCH_WRAP   optional prefix executed ahead of the CLI (default empty)
#
# Evidence kept in OUTPUT_DIR (nothing is deleted on failure):
#   plan.copy.tsv plan.source.txt plan.sha256 plan.errors
#   plan.data.tsv plan.selected.tsv params.txt dmesg.note.txt
#   uname.before.txt uname.after.txt taint.txt argv.tsv cases.tsv
#   dmesg.raw dmesg.err
#   <case>.json <case>.err <case>.exit <case>.cmd
#   <case>.dmesg.before <case>.dmesg.before.err
#   <case>.dmesg.after  <case>.dmesg.after.err
#   STOPPED.txt on any stop (status, reason, cases_done, no-resume notice)
#
# Exit codes:
#   0            every selected case ran and passed
#   1..125       exit status of the first failing CLI case, propagated as-is
#   2            usage, plan/setup error, or refused output directory
#   3            taint gate: taint was not exactly 0 before or after a case
#   4            capture/collection/output failure
#   5            CLI exit 0 but its stdout capture is missing or empty
#   127          a required command is missing
#   129/130/143  HUP/INT/TERM
set -f
set -u

TAB=$(printf '\t')
PLAN_DEFAULT=/etc/platform-research-plan.tsv
EXPECTED_HEADER="id${TAB}ordinal${TAB}stage${TAB}mode${TAB}condition${TAB}apages${TAB}acount${TAB}gap${TAB}vcount${TAB}warmup${TAB}window${TAB}timeout"
PLAN_ROWS=84
SHORT_ROWS=6
ALL_ROWS=84
FOLLOWUP_ROWS=11
FULLSTATS_ROWS=21
STDERR_TAIL_BYTES=2048

usage() {
    cat <<'EOF'
Usage: board-research.sh MODE [OUTPUT_DIR]
  MODE is required; there is no implicit default.
    short          run the 6 frozen short-stage cases (ordinals 1..6)
    all            run all 84 frozen cases (6 short + 60 main E2 + 18 main cost)
    cost-followup  run the fixed 11-case cost follow-up segment selected from
                   the same frozen 84-row plan (2 frozen short cost cases +
                   Solo/A8g0/A8g2000 x 3 main cost cases).
    cost-fullstats run all 21 frozen cost cases (3 short + 18 main) with
                   --require-fullstats appended to every CLI invocation.
  OUTPUT_DIR must not already exist; this script creates it.
  Default OUTPUT_DIR: /tmp/research-$$.
  Plan file: RESEARCH_PLAN or /etc/platform-research-plan.tsv.
  The plan is copied to OUTPUT_DIR/plan.copy.tsv and only that copy is used.
  A case passes when the CLI exits 0, its stdout artifact is non-empty and the
  taint gate is exactly 0 before and after; the runner never parses the output.
  The first failing case stops the run and its CLI exit status is propagated.
  There is no retry, no rerun and no automatic resume.
EOF
}

fatal() {
    echo "RESEARCH: fatal: $*" >&2
    exit 2
}

# stop CODE REASON...: record the stop, keep every artifact, exit non-zero.
stop() {
    stop_code=$1
    shift
    echo "RESEARCH: stop exit=$stop_code reason=$*" >&2
    {
        echo "status=stopped"
        echo "exit=$stop_code"
        echo "mode=$MODE"
        echo "cases_done=$cases_done"
        echo "cases_total=$SEL_N"
        echo "reason=$*"
        echo "resume_supported=0"
        echo "rerun_supported=0"
        echo "output=$OUT"
    } > "$OUT/STOPPED.txt" 2>/dev/null || true
    exit "$stop_code"
}

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

cleanup() {
    rc=$?
    trap - EXIT HUP INT TERM
    "$DMESG_CMD" > "$OUT/dmesg.raw" 2> "$OUT/dmesg.err" || collection_failed=1
    read_taint > "$OUT/taint.txt" 2> /dev/null || collection_failed=1
    "$UNAME_CMD" -a > "$OUT/uname.after.txt" 2> "$OUT/uname.after.err" || collection_failed=1
    if [ "$collection_failed" -ne 0 ] && [ "$rc" -eq 0 ]; then
        rc=4
    fi
    if [ "$rc" -ne 0 ] && [ ! -e "$OUT/STOPPED.txt" ]; then
        {
            echo "status=stopped"
            echo "exit=$rc"
            echo "mode=$MODE"
            echo "cases_done=$cases_done"
            echo "cases_total=$SEL_N"
            echo "reason=early_exit_or_signal"
            echo "resume_supported=0"
            echo "rerun_supported=0"
            echo "output=$OUT"
        } > "$OUT/STOPPED.txt" 2>/dev/null || true
    fi
    echo "RESEARCH: final_exit=$rc mode=$MODE cases_done=$cases_done total=$SEL_N collection_failed=$collection_failed out=$OUT" >&2
    if [ "$rc" -eq 0 ] && [ "$cases_done" -eq "$SEL_N" ]; then
        echo "RESEARCH: complete mode=$MODE cases=$cases_done total=$SEL_N out=$OUT" || rc=4
    fi
    exit "$rc"
}

# report_bounded_stderr ID: echo a bounded tail of one case's stderr and name
# the full artifacts that stay on disk. The full files are never trimmed: the
# board return-file archival keeps collecting them unchanged.
report_bounded_stderr() {
    report_id=$1
    echo "RESEARCH: case_stderr_tail_begin id=$report_id bytes=$STDERR_TAIL_BYTES" >&2
    if [ -s "$OUT/$report_id.err" ]; then
        tail -c "$STDERR_TAIL_BYTES" "$OUT/$report_id.err" >&2 || true
    else
        echo "(stderr empty)" >&2
    fi
    echo "RESEARCH: case_stderr_tail_end id=$report_id" >&2
    echo "RESEARCH: case_files_kept id=$report_id stdout=$OUT/$report_id.json stderr=$OUT/$report_id.err exit_file=$OUT/$report_id.exit" >&2
}

# Validate the frozen plan data rows (the header is checked separately). Any
# deviation prints a PLAN: line and fails; the runner then refuses to run.
# Mode counts are checked per stage: short E2/cost 3/3, main E2/cost 60/18 and
# the all-stage mode totals 63/21. counts.e2 and counts.cost in the JSON
# manifest are main-stage counts; the all-stage totals are recorded there as
# all_e2/all_cost so the two cannot be confused.
plan_check() {
    awk -F "$TAB" '
    BEGIN {
        nbad = 0
        n_short = 0
        n_short_e2 = 0
        n_short_cost = 0
        n_main_e2 = 0
        n_main_cost = 0
        ne2 = split("w0 w1 w4 w7 w8 w16", e2o, " ")
        for (i = 1; i <= ne2; i++) e2idx[e2o[i]] = i
        nco = split("solo a1g0 a8g0 a16g0 a8g1000 a8g2000", co, " ")
        for (i = 1; i <= nco; i++) cidx[co[i]] = i
        short_id[1] = "short-e2-w0"
        short_id[2] = "short-e2-w1"
        short_id[3] = "short-e2-w16"
        short_id[4] = "short-cost-solo"
        short_id[5] = "short-cost-a8g0"
        short_id[6] = "short-cost-a16g0"
        short_mode[1] = "e2"
        short_mode[2] = "e2"
        short_mode[3] = "e2"
        short_mode[4] = "cost"
        short_mode[5] = "cost"
        short_mode[6] = "cost"
        short_cond[1] = "w0"
        short_cond[2] = "w1"
        short_cond[3] = "w16"
        short_cond[4] = "solo"
        short_cond[5] = "a8g0"
        short_cond[6] = "a16g0"
    }
    function bad(m) { printf "PLAN: line=%d id=%s: %s\n", NR, (id == "" ? "-" : id), m; nbad++ }
    function cost_apages(c) {
        if (c == "solo") return 0
        if (c == "a1g0") return 1
        if (c == "a16g0") return 16
        if (c == "a8g0" || c == "a8g1000" || c == "a8g2000") return 8
        return -1
    }
    function cost_gap(c) {
        if (c == "a8g1000") return 1000
        if (c == "a8g2000") return 2000
        if (c == "solo" || c == "a1g0" || c == "a8g0" || c == "a16g0") return 0
        return -1
    }
    {
        id = $1
        if (NF != 12) { bad("field_count=" NF); next }
        if (id !~ /^[a-z0-9][a-z0-9-]*$/) { bad("id_charset"); next }
        if (id ~ /(^|-)no(-|$)/) { bad("id_has_forbidden_prefix"); next }
        if (id in seen) { bad("duplicate_id"); next }
        seen[id] = 1
        numeric_ok = ($2 ~ /^[0-9]+$/ && $6 ~ /^[0-9]+$/ && $7 ~ /^[0-9]+$/ && $8 ~ /^[0-9]+$/ && $9 ~ /^[0-9]+$/ && $10 ~ /^[0-9]+$/ && $11 ~ /^[0-9]+$/ && $12 ~ /^[0-9]+$/)
        if (!numeric_ok) { bad("non_numeric_field"); next }
        stage = $3
        mode = $4
        cond = $5
        if (stage != "short" && stage != "main") { bad("stage=" stage); next }
        if (mode != "e2" && mode != "cost") { bad("mode=" mode); next }
        o = $2 + 0
        apages = $6 + 0
        acount = $7 + 0
        gap = $8 + 0
        vcount = $9 + 0
        warmup = $10 + 0
        window = $11 + 0
        timeout = $12 + 0
        if (o != NR) bad("ordinal_not_sequential want=" NR " got=" o)
        if (o < 1 || o > 84) bad("ordinal_out_of_range=" o)
        if (o <= 6) {
            if (stage != "short") bad("stage_not_short")
            if (id != short_id[o]) bad("short_id want=" short_id[o])
            if (mode != short_mode[o]) bad("short_mode want=" short_mode[o])
            if (cond != short_cond[o]) bad("short_condition want=" short_cond[o])
        } else if (o <= 66) {
            if (mode != "e2") bad("mode_not_e2")
            if (stage != "main") bad("stage_not_main")
            j = o - 7
            blk = int(j / 6) + 1
            want = e2o[((blk - 1) + (j % 6)) % 6 + 1]
            if (cond != want) bad("e2_rotation want=" want " got=" cond)
            if (id != sprintf("e2-b%02d-%s", blk, cond)) bad("e2_id_mismatch")
        } else if (o <= 84) {
            if (mode != "cost") bad("mode_not_cost")
            if (stage != "main") bad("stage_not_main")
            j = o - 67
            blk = int(j / 6) + 1
            want = co[((blk - 1) + (j % 6)) % 6 + 1]
            if (cond != want) bad("cost_rotation want=" want " got=" cond)
            if (id != sprintf("cost-b%02d-%s", blk, cond)) bad("cost_id_mismatch")
        }
        if (mode == "e2") {
            if (!(cond in e2idx)) { bad("unknown_e2_condition=" cond); next }
            want = substr(cond, 2) + 0
            if (apages != want) bad("e2_apages want=" want " got=" apages)
            want = (apages == 0) ? 0 : 112
            if (acount != want) bad("e2_acount want=" want " got=" acount)
            if (gap != 0) bad("e2_gap_not_0 got=" gap)
            if (vcount != 1) bad("e2_vcount_not_1 got=" vcount)
            if (warmup != 2) bad("e2_warmup_not_2 got=" warmup)
            if (window != 5000000) bad("e2_window_not_5000000 got=" window)
            if (timeout != 2000) bad("e2_timeout_not_2000 got=" timeout)
        } else {
            if (!(cond in cidx)) { bad("unknown_cost_condition=" cond); next }
            want = cost_apages(cond)
            if (apages != want) bad("cost_apages want=" want " got=" apages)
            want = cost_gap(cond)
            if (gap != want) bad("cost_gap want=" want " got=" gap)
            if (acount != 0) bad("cost_acount_not_applicable want=0 got=" acount)
            if (window != 0) bad("cost_window_not_0 got=" window)
            if (timeout != 10000) bad("cost_timeout_not_10000 got=" timeout)
            if (stage == "short") {
                if (vcount != 64) bad("cost_short_transfers_not_64 got=" vcount)
                if (warmup != 8) bad("cost_short_warmup_not_8 got=" warmup)
            } else {
                if (vcount != 4096) bad("cost_main_transfers_not_4096 got=" vcount)
                if (warmup != 256) bad("cost_main_warmup_not_256 got=" warmup)
            }
        }
        if (stage == "short") {
            n_short++
            if (mode == "e2") n_short_e2++
            else n_short_cost++
        } else if (mode == "e2") {
            n_main_e2++
        } else {
            n_main_cost++
        }
    }
    END {
        if (NR != 84) { printf "PLAN: row_count want=84 got=%d\n", NR; nbad++ }
        if (n_short != 6) { printf "PLAN: short_count want=6 got=%d\n", n_short; nbad++ }
        if (n_short_e2 != 3) { printf "PLAN: short_e2_count want=3 got=%d\n", n_short_e2; nbad++ }
        if (n_short_cost != 3) { printf "PLAN: short_cost_count want=3 got=%d\n", n_short_cost; nbad++ }
        if (n_main_e2 != 60) { printf "PLAN: main_e2_count want=60 got=%d\n", n_main_e2; nbad++ }
        if (n_main_cost != 18) { printf "PLAN: main_cost_count want=18 got=%d\n", n_main_cost; nbad++ }
        if (n_short_e2 + n_main_e2 != 63) { printf "PLAN: all_e2_count want=63 got=%d\n", n_short_e2 + n_main_e2; nbad++ }
        if (n_short_cost + n_main_cost != 21) { printf "PLAN: all_cost_count want=21 got=%d\n", n_short_cost + n_main_cost; nbad++ }
        if (nbad > 0) exit 1
        exit 0
    }
    ' "$1"
}

[ $# -ge 1 ] && [ $# -le 2 ] || { usage >&2; exit 2; }
MODE=$1
case "$MODE" in
    short)
        SEL_N=$SHORT_ROWS
        MODE_NOTE="frozen short stage, ordinals 1..6 of the frozen 84-case plan"
        ;;
    all)
        SEL_N=$ALL_ROWS
        MODE_NOTE="all 84 frozen cases (6 short + 60 main E2 + 18 main cost)"
        ;;
    cost-followup)
        SEL_N=$FOLLOWUP_ROWS
        MODE_NOTE="independent cost-followup segment: 2 frozen short cost + Solo/A8g0/A8g2000 x3 main cost; not a completion of the frozen 84-case plan"
        ;;
    cost-fullstats)
        SEL_N=$FULLSTATS_ROWS
        MODE_NOTE="independent cost-fullstats segment: all 21 frozen cost cases, every CLI argv gets --require-fullstats"
        ;;
    *)
        usage >&2
        exit 2
        ;;
esac
if [ $# -eq 2 ]; then
    OUT=$2
else
    OUT=/tmp/research-$$
fi
[ -n "$OUT" ] || { usage >&2; exit 2; }
if [ -e "$OUT" ]; then
    fatal "output directory already exists: $OUT"
fi
mkdir "$OUT" || fatal "mkdir failed: $OUT"

CLI=${RESEARCH_CLI:-idma_research}
TAINT_PATH=${RESEARCH_TAINT:-/proc/sys/kernel/tainted}
DMESG_CMD=${RESEARCH_DMESG:-dmesg}
UNAME_CMD=${RESEARCH_UNAME:-uname}
CLI_WRAP=${RESEARCH_WRAP:-}

PLAN_SOURCE=environment
PLAN=${RESEARCH_PLAN:-}
if [ -z "$PLAN" ]; then
    PLAN_SOURCE=default
    PLAN=$PLAN_DEFAULT
    if [ ! -r "$PLAN" ]; then
        PLAN_SOURCE=sibling
        PLAN="$(dirname "$0")/research-plan.tsv"
    fi
fi

cases_done=0
collection_failed=0
HASH_TOOL=none
PLAN_SHA=unavailable

trap cleanup EXIT
trap 'exit 129' HUP
trap 'exit 130' INT
trap 'exit 143' TERM

for cmd in awk sed cat mkdir dirname tail wc; do
    if ! command -v "$cmd" > /dev/null 2>&1; then
        echo "RESEARCH: missing command: $cmd" >&2
        exit 127
    fi
done
for cmd in "$CLI" "$DMESG_CMD" "$UNAME_CMD"; do
    if ! command -v "$cmd" > /dev/null 2>&1; then
        echo "RESEARCH: missing command: $cmd" >&2
        exit 127
    fi
done

if [ ! -r "$PLAN" ]; then
    fatal "plan not readable: $PLAN (source=$PLAN_SOURCE)"
fi

# Freeze the plan bytes used by this run: everything below reads the copy, so
# the original file may change afterwards without affecting the run, and the
# recorded sha256 describes exactly the bytes that were validated.
if ! cat "$PLAN" > "$OUT/plan.copy.tsv"; then
    fatal "plan copy failed: $PLAN"
fi

plan_header=$(sed -n '1p' "$OUT/plan.copy.tsv")
if [ "$plan_header" != "$EXPECTED_HEADER" ]; then
    echo "RESEARCH: fatal: unexpected plan header in $PLAN" >&2
    printf 'RESEARCH: got:  %s\n' "$plan_header" >&2
    printf 'RESEARCH: want: %s\n' "$EXPECTED_HEADER" >&2
    exit 2
fi

# Optional hash tooling, recorded honestly when absent. The hash is taken over
# the frozen copy, never over the original that may still change on the board.
if command -v sha256sum > /dev/null 2>&1; then
    HASH_TOOL=sha256sum
    PLAN_SHA=$(sha256sum "$OUT/plan.copy.tsv" 2>/dev/null | awk '{print $1}')
elif command -v shasum > /dev/null 2>&1; then
    HASH_TOOL=shasum
    PLAN_SHA=$(shasum -a 256 "$OUT/plan.copy.tsv" 2>/dev/null | awk '{print $1}')
fi
case "$PLAN_SHA" in
    ''|unknown)
        HASH_TOOL=none
        PLAN_SHA=unavailable
        ;;
esac
{
    echo "plan_copy=plan.copy.tsv"
    echo "plan_source_path=$PLAN"
    echo "hash_tool=$HASH_TOOL"
    echo "plan_sha256=$PLAN_SHA"
} > "$OUT/plan.sha256" || fatal "plan.sha256 write failed"

if ! sed -n '2,$p' "$OUT/plan.copy.tsv" > "$OUT/plan.data.tsv"; then
    fatal "plan read failed: $PLAN"
fi
plan_rows=$(awk 'END { print NR + 0 }' "$OUT/plan.data.tsv")
if [ "$plan_rows" != "$PLAN_ROWS" ]; then
    fatal "plan row count got=$plan_rows want=$PLAN_ROWS"
fi

if ! plan_check "$OUT/plan.data.tsv" > "$OUT/plan.errors" 2>&1; then
    if [ -s "$OUT/plan.errors" ]; then
        cat "$OUT/plan.errors" >&2
    fi
    fatal "plan validation failed: $PLAN (details in $OUT/plan.errors)"
fi

{
    echo "plan_source=$PLAN_SOURCE"
    echo "plan_path=$PLAN"
    echo "plan_copy=$OUT/plan.copy.tsv"
    echo "plan_rows=$plan_rows"
    echo "mode=$MODE"
    echo "cases_total=$SEL_N"
    echo "hash_tool=$HASH_TOOL"
    echo "plan_sha256=$PLAN_SHA"
} > "$OUT/plan.source.txt" || fatal "plan.source write failed"

# Select the rows this mode executes: short/all keep the historical prefix and
# the whole plan, cost-followup is a fixed filter over the same validated
# 84-row plan (no operator edit on the board), so the selected ids, ordinals
# and per-block order are exactly the frozen ones and other conditions drop out.
# cost-fullstats is a pure filter that keeps every cost row ($4 == "cost"):
# the 3 short-stage cost cases plus the 18 main-stage cost cases, in frozen
# order, exactly the 21 rows the full-aggregation segment expects.
case "$MODE" in
    cost-followup)
        if ! awk -F "$TAB" '
            ($3 == "short" && $4 == "cost" && ($1 == "short-cost-solo" || $1 == "short-cost-a8g0")) ||
            ($3 == "main" && $4 == "cost" && ($5 == "solo" || $5 == "a8g0" || $5 == "a8g2000"))
        ' "$OUT/plan.data.tsv" > "$OUT/plan.selected.tsv"; then
            fatal "plan selection failed"
        fi
        ;;
    cost-fullstats)
        if ! awk -F "$TAB" '$4 == "cost"' "$OUT/plan.data.tsv" > "$OUT/plan.selected.tsv"; then
            fatal "plan selection failed"
        fi
        ;;
    *)
        if ! sed -n "1,${SEL_N}p" "$OUT/plan.data.tsv" > "$OUT/plan.selected.tsv"; then
            fatal "plan selection failed"
        fi
        ;;
esac
selected_rows=$(awk 'END { print NR + 0 }' "$OUT/plan.selected.tsv")
if [ "$selected_rows" != "$SEL_N" ]; then
    fatal "selected row count got=$selected_rows want=$SEL_N"
fi

if ! "$UNAME_CMD" -a > "$OUT/uname.before.txt" 2> "$OUT/uname.before.err"; then
    fatal "uname -a failed"
fi
kernel_all=$(cat "$OUT/uname.before.txt")

{
    echo "runner=board-research.sh"
    echo "mode=$MODE"
    echo "mode_note=$MODE_NOTE"
    echo "output=$OUT"
    echo "plan_source=$PLAN_SOURCE"
    echo "plan_path=$PLAN"
    echo "plan_copy=plan.copy.tsv"
    echo "plan_rows=$plan_rows"
    echo "cases_total=$SEL_N"
    echo "cli=$CLI"
    echo "cli_wrap=$CLI_WRAP"
    echo "taint_path=$TAINT_PATH"
    echo "dmesg_cmd=$DMESG_CMD"
    echo "uname_cmd=$UNAME_CMD"
    echo "uname=$kernel_all"
    echo "date_utc=$(date -u +%Y-%m-%dT%H:%M:%SZ 2>/dev/null || echo unknown)"
    echo "hash_tool=$HASH_TOOL"
    echo "plan_sha256=$PLAN_SHA"
    echo "plan_sha256_source=plan.copy.tsv"
    echo "dmesg_role=raw_collection_only; no zero-warning claim is made or checked"
    echo "case_checks=cli_exit_zero,nonempty_stdout_file,taint_zero_before_and_after"
    echo "case_output_console=compact_result_only; bounded_stderr_tail_on_failure; full_files_kept_in_output_dir"
    echo "case_failure_stderr_tail_bytes=$STDERR_TAIL_BYTES"
    echo "runner_never=parses_json,greps_json_strings,claims_json_semantics,retries,resumes,runs_a_case_twice"
} > "$OUT/params.txt" || fatal "params write failed"

printf '%s\n' "The dmesg files in this directory are raw, as-collected kernel ring-buffer text. They are kept as evidence only. This runner does not filter, count or judge them and makes no claim that the run was warning-free or that any case was free of kernel messages." > "$OUT/dmesg.note.txt" || fatal "dmesg note write failed"

: > "$OUT/argv.tsv" || fatal "argv record create failed"
printf 'id\tordinal\tstage\tmode\tcondition\tcli_exit\ttaint_before\ttaint_after\n' > "$OUT/cases.tsv" || fatal "cases record create failed"

echo "RESEARCH: plan_ok source=$PLAN_SOURCE rows=$plan_rows mode=$MODE cases=$SEL_N hash_tool=$HASH_TOOL out=$OUT" >&2

n=0
while [ "$n" -lt "$SEL_N" ]; do
    n=$((n + 1))
    set -- $(sed -n "${n}p" "$OUT/plan.selected.tsv")
    c_id=${1:-}
    c_ordinal=${2:-}
    c_stage=${3:-}
    c_mode=${4:-}
    c_cond=${5:-}
    c_apages=${6:-}
    c_acount=${7:-}
    c_gap=${8:-}
    c_vcount=${9:-}
    c_warmup=${10:-}
    c_window=${11:-}
    c_timeout=${12:-}

    if [ -z "$c_id" ] || [ -z "$c_timeout" ]; then
        stop 2 "selected_line_unreadable index=$n"
    fi

    case "$c_mode" in
        e2)
            case_args="--e2 --a-pages $c_apages --a-transfers $c_acount --window-cycles $c_window --warmup $c_warmup --timeout-ms $c_timeout"
            ;;
        cost)
            # Cost mode has no A-transfer count: the A (P4M) batch runs until V
            # finishes and is then stopped/drained, so no --a-transfers is passed.
            case_args="--cost --a-pages $c_apages --a-gap-cycles $c_gap --transfers $c_vcount --warmup $c_warmup --timeout-ms $c_timeout"
            # cost-fullstats requires the full-aggregation raw fields; the flag
            # is appended to the real invocation, so the recorded .cmd file and
            # the argv.tsv row match it exactly.
            if [ "$MODE" = "cost-fullstats" ]; then
                case_args="$case_args --require-fullstats"
            fi
            ;;
        *)
            stop 2 "selected_line_bad_mode id=$c_id mode=$c_mode"
            ;;
    esac

    c_before_taint=$(read_taint)
    if [ "$c_before_taint" != "0" ]; then
        printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$c_id" "$c_ordinal" "$c_stage" "$c_mode" "$c_cond" "not_run" "$c_before_taint" "-" >> "$OUT/cases.tsv" || true
        stop 3 "taint_before id=$c_id value=$c_before_taint"
    fi

    if ! "$DMESG_CMD" > "$OUT/$c_id.dmesg.before" 2> "$OUT/$c_id.dmesg.before.err"; then
        stop 4 "dmesg_before_collection id=$c_id"
    fi

    printf '%s %s\n' "$CLI" "$case_args" > "$OUT/$c_id.cmd" || stop 4 "cmd_record_failed id=$c_id"
    printf '%s\t%s\n' "$c_id" "$case_args" >> "$OUT/argv.tsv" || stop 4 "argv_record_failed id=$c_id"

    echo "RESEARCH: case_begin id=$c_id ordinal=$c_ordinal stage=$c_stage mode=$c_mode condition=$c_cond cases_done=$cases_done total=$SEL_N argv=$case_args" >&2

    if [ -n "$CLI_WRAP" ]; then
        # shellcheck disable=SC2086
        $CLI_WRAP "$CLI" $case_args > "$OUT/$c_id.json" 2> "$OUT/$c_id.err"
    else
        # shellcheck disable=SC2086
        "$CLI" $case_args > "$OUT/$c_id.json" 2> "$OUT/$c_id.err"
    fi
    c_rc=$?

    c_after_taint=$(read_taint)
    printf '%s\n' "$c_rc" > "$OUT/$c_id.exit" || stop 4 "exit_record_failed id=$c_id"

    if ! "$DMESG_CMD" > "$OUT/$c_id.dmesg.after" 2> "$OUT/$c_id.dmesg.after.err"; then
        stop 4 "dmesg_after_collection id=$c_id"
    fi

    printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$c_id" "$c_ordinal" "$c_stage" "$c_mode" "$c_cond" "$c_rc" "$c_before_taint" "$c_after_taint" >> "$OUT/cases.tsv" || stop 4 "cases_record_failed id=$c_id"

    # The full stdout/stderr files stay on disk for the board return-file
    # archival; the console only ever sees a bounded summary.
    if [ "$c_rc" -ne 0 ]; then
        echo "RESEARCH: case_failed id=$c_id cli_exit=$c_rc reason=cli_exit_nonzero" >&2
        report_bounded_stderr "$c_id"
        stop "$c_rc" "cli_exit_nonzero id=$c_id"
    fi
    if [ "$c_after_taint" != "0" ]; then
        echo "RESEARCH: case_failed id=$c_id cli_exit=0 reason=taint_after value=$c_after_taint" >&2
        report_bounded_stderr "$c_id"
        stop 3 "taint_after id=$c_id value=$c_after_taint"
    fi
    if [ ! -s "$OUT/$c_id.json" ]; then
        echo "RESEARCH: case_failed id=$c_id cli_exit=0 reason=stdout_missing_or_empty" >&2
        report_bounded_stderr "$c_id"
        stop 5 "cli_stdout_missing_or_empty id=$c_id"
    fi

    cases_done=$((cases_done + 1))
    echo "RESEARCH: case_end id=$c_id cli_exit=0 cases_done=$cases_done total=$SEL_N stdout_bytes=$(wc -c < "$OUT/$c_id.json" | awk '{print $1 + 0}') json_file=$OUT/$c_id.json" >&2
done

exit 0

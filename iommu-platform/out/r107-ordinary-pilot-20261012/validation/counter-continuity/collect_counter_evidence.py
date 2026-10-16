#!/usr/bin/env python3
"""Read only named frozen sources and existing ELF; write only this seat's evidence."""
import datetime
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time

OUT = Path(__file__).resolve().parent
P = OUT.parents[1]
C = P.parent / "composition-isolated-20261007"
F = C / "fpga-src/cva6"
EXPECTED_BIT = "f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca"
FILES = [
    (C / "fpga-src/build-inputs.json", [(1, 15)]),
    (C / "route-opt/result.json", [(1, 30)]),
    (C / "route-opt/optimize.tcl", [(5, 34)]),
    (C / "route-opt/reports/timing_summary_routed.rpt", [(153, 177)]),
    (C / "fpga-build.log", [(9865, 9865)]),
    (F / "corev_apu/fpga/scripts/add_sources.tcl", [(1, 5)]),
    (F / "corev_apu/fpga/scripts/run.tcl", [(19, 70)]),
    (F / "corev_apu/fpga/scripts/prologue.tcl", [(18, 28)]),
    (F / "corev_apu/fpga/src/genesysii.svh", [(14, 27)]),
    (F / "vendor/pulp-platform/common_cells/include/common_cells/registers.svh", []),
    (F / "core/include/ariane_pkg.sv", [(255, 267)]),
    (F / "core/include/riscv_pkg.sv", [(552, 559)]),
    (F / "core/csr_regfile.sv", [(428, 435), (501, 503), (525, 539),
        (680, 688), (1024, 1024), (1058, 1062), (1137, 1138),
        (1684, 1695), (1803, 1807), (1817, 1831), (1880, 1887), (1947, 1954)]),
    (F / "core/ariane.sv", [(62, 80)]),
    (F / "core/cva6.sv", [(608, 623), (670, 678)]),
    (F / "corev_apu/fpga/src/ariane_xilinx.sv", [(259, 268), (712, 761),
        (1092, 1099), (1128, 1137)]),
    (F / "corev_apu/clint/clint.sv", [(37, 39), (87, 129),
        (198, 219), (239, 294)]),
    (F / "corev_apu/fpga/xilinx/xlnx_clk_gen/tcl/run.tcl", [(10, 26)]),
    (F / "corev_apu/fpga/xilinx/xlnx_clk_gen/xlnx_clk_gen.gen/sources_1/ip/xlnx_clk_gen/xlnx_clk_gen_clk_wiz.v",
        [(71, 83), (128, 153), (170, 195), (212, 214)]),
    (P / "build/artifact-manifest.json", [(1, 13), (58, 83)]),
    (P / "source/linux/drivers/misc/idma_finite.h", [(34, 57), (247, 256),
        (326, 368), (401, 406), (423, 459), (500, 509), (547, 563), (678, 703)]),
    (P / "source/linux/drivers/misc/idma.c", [(1022, 1026), (1054, 1057)]),
    (P / "source/linux/drivers/iommu/riscv/iommu-hpm.c", [(38, 42), (55, 68), (196, 218)]),
    (P / "source/linux/drivers/perf/riscv_pmu_sbi.c", [(477, 493), (914, 948)]),
    (P / "build/linux/.config", [(250, 250), (402, 406), (420, 420), (4093, 4095)]),
    (P / "source/opensbi/platform/fpga/ariane/platform.c", [(29, 37), (51, 62), (162, 173)]),
    (P / "source/opensbi/lib/sbi/sbi_hart.c", [(54, 69), (109, 137), (480, 508)]),
    (P / "source/opensbi/lib/sbi/sbi_trap.c", [(243, 247)]),
    (P / "source/opensbi/lib/sbi/sbi_illegal_insn.c", [(35, 80)]),
    (P / "source/opensbi/lib/sbi/sbi_emulate_csr.c", [(64, 79)]),
    (P / "source/opensbi/lib/sbi/sbi_timer.c", [(47, 50), (84, 88), (144, 175)]),
    (P / "source/opensbi/lib/utils/timer/aclint_mtimer.c", [(24, 31), (54, 81),
        (91, 110), (115, 142), (173, 203), (232, 235)]),
    (P / "source/opensbi/lib/sbi/sbi_pmu.c", [(279, 320), (337, 384), (675, 690)]),
    (P / "source/opensbi/lib/sbi/sbi_ecall_pmu.c", [(56, 79)]),
    (P / "source/lloader/rv64.S", [(1, 27)]),
    (P / "host/calibrate_finite_timer.py", [(111, 173), (176, 184)]),
    (P / "validation/timer-bounds-analysis.md", [(72, 82)]),
]


def digest(path):
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def write_json(name, data):
    (OUT / name).write_text(json.dumps(data, indent=2, ensure_ascii=False) + "\n")


start = time.monotonic_ns()
records, extracts = [], []
for path, ranges in FILES:
    lines = path.read_text(errors="replace").splitlines()
    row = {"path": str(path), "sha256": digest(path), "bytes": path.stat().st_size,
           "line_count": len(lines), "ranges": ranges}
    records.append(row)
    extracts.append(f"FILE {path}\nSHA256 {row['sha256']}\n")
    for first, last in ranges:
        extracts.append(f"LINES {first}-{last}\n")
        extracts.extend(f"{index}: {lines[index-1]}\n"
                        for index in range(first, min(last, len(lines)) + 1))
    extracts.append("\n")
(OUT / "source-extracts.txt").write_text("".join(extracts))

add_sources = (F / "corev_apu/fpga/scripts/add_sources.tcl").read_text()
compile_paths = list(dict.fromkeys(re.findall(r"/[^\s{}]+\.(?:sv|v|svh|vh)(?=[\s{}]|$)", add_sources)))
macro_definitions = []
searched = compile_paths + [str(F / "corev_apu/fpga/src/genesysii.svh"),
    str(F / "vendor/pulp-platform/common_cells/include/common_cells/registers.svh")]
for path in searched:
    for line, text in enumerate(Path(path).read_text(errors="replace").splitlines(), 1):
        if re.match(r"\s*`define\s+(SPIKE_TANDEM|DROMAJO)(?:\s|$)", text):
            macro_definitions.append({"path": path, "line": line, "text": text})
csr_lines = (F / "core/csr_regfile.sv").read_text().splitlines()
negative_search = {
    "CSR_TIME_or_MCOUNTINHIBIT_in_frozen_csr_regfile": [
        {"line": i, "text": line} for i, line in enumerate(csr_lines, 1)
        if re.search(r"\b(?:CSR_TIME|CSR_TIMEH|CSR_MCOUNTINHIBIT)\b", line)],
    "SPIKE_TANDEM_or_DROMAJO_define_in_compilation_files_and_explicit_global_headers": macro_definitions,
    "actual_compilation_file_count": len(compile_paths),
    "compiler_source_paths": compile_paths,
    "all_indirect_include_files_scanned": False,
    "limitation": "Finite search of actual compilation list and explicit global headers; no claim of arbitrary indirect include closure."
}
write_json("source-evidence.json", {"schema": "r111-counter-source-evidence-v1", "files": records,
    "negative_searches": negative_search})

elf = P / "build/opensbi/platform/fpga/ariane/firmware/fw_payload.elf"
commands = []


def capture(args):
    begin = time.monotonic_ns()
    result = subprocess.run(args, text=True, capture_output=True, check=True)
    commands.append({"argv": args, "returncode": result.returncode,
                     "dell_monotonic_elapsed_ns": time.monotonic_ns() - begin,
                     "stderr": result.stderr})
    return result.stdout


symbols = capture(["riscv64-linux-gnu-nm", str(elf)])
names = ("sbi_emulate_csr_read", "sbi_illegal_insn_handler", "sbi_timer_value",
    "get_platform_ticks", "mtimer_time_rd64", "mtimer_value", "pmu_ctr_start_hw", "sbi_ecall_pmu_handler")
selected_symbols = "\n".join(line for line in symbols.splitlines() if line.split()[-1] in names) + "\n"
(OUT / "firmware-symbols.txt").write_text(selected_symbols)
disasm_names = ("sbi_timer_value", "get_platform_ticks", "mtimer_time_rd64", "mtimer_value",
                "pmu_ctr_start_hw", "sbi_emulate_csr_read")
disasm = [capture(["riscv64-linux-gnu-objdump", "-d", f"--disassemble={name}", str(elf)])
          for name in disasm_names]
(OUT / "firmware-disassembly.txt").write_text("\n".join(disasm))

bit = C / "route-opt/ariane_xilinx.bit"
bit_sha = digest(bit)
assert bit_sha == EXPECTED_BIT
assert not negative_search["CSR_TIME_or_MCOUNTINHIBIT_in_frozen_csr_regfile"]
assert not macro_definitions
manifest = json.loads((P / "build/artifact-manifest.json").read_text())
elf_sha = digest(elf)
elf_record = next(a for a in manifest["artifacts"] if a["path"] == str(elf))
assert elf_sha == elf_record["sha256"]

result = {
    "schema": "r111-counter-continuity-review-v1",
    "actor": "Codex gpt-6.1-sol ultra",
    "observed_at_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
    "scope": "Frozen Oct7 f9 counter/clock and new stage111 firmware source review only",
    "report": "counter-continuity-report.txt",
    "source_evidence": "source-evidence.json",
    "original_lines": "source-extracts.txt",
    "selected_bit": {"path": str(bit), "sha256": bit_sha, "matches_assigned_bit": True},
    "new_firmware_elf": {"path": str(elf), "sha256": elf_sha, "matches_artifact_manifest": True},
    "provenance": {
        "historical_staged_build_directory_located": True,
        "actual_source_list_located": True,
        "actual_generated_clock_IP_located": True,
        "exact_base_counter_files_prebuild_digests_available": False,
        "limitation": "Retained Oct7 snapshot, overlay manifest, compiler trace and routed result bind the inspected paths to this build; hashes newly calculated here are not claimed to be original Oct7 per-file hashes."
    },
    "source_conclusions": {
        "main_clock": "Fixed MMCM multiply5/divide1/divide20; DRP, dynamic phase and powerdown tied off; BUFG output; design nominal 50MHz is not an independent measured physical rate.",
        "cpu_cycle": "64-bit cycle_q advances once per main clock outside debug; no WFI/commit/privilege gate in increment; M-mode MCYCLE writes override; common reset clears.",
        "time": "Unsupported native TIME is emulated by OpenSBI; host S-mode gets raw 64-bit CLINT mtime via MMIO, incremented at rtc/main-clock-div2 rising edges; no debug/WFI gate; MMIO mtime writes override and common reset clears.",
        "cpu_time_are_independent_oscillators": False,
        "mcountinhibit_native_implemented_in_inspected_CSR": False,
        "real_cpu_counter_reprogramming_path": "Linux perf SBI SET_INIT_VALUE -> SBI PMU start -> csr_write_num(MCYCLE+cidx); no-MCOUNTINHIBIT skips inhibit handling but still writes initial value.",
        "ordinary_counter_reset_path": "Finite ordinary only reads CYCLE/TIME; DMA observer reset and IOMMU HPM inhibit/save/restore operate separate MMIO counters, not MCYCLE/MTIME.",
        "two_windows": "stage3 swapout and stage6 restore bracket phase() including instrumentation/control/polling/capture; CPU and TIME sample points are distinct; not pure DMA hardware delay."
    },
    "stage_conversion": {
        "allowed": "Conditional conversion of each counter's own raw stamp delta using accepted same-boot Dell calibration bounds.",
        "formula_seconds": "[max(0,D-1)/f_high, (D+1)/f_low]",
        "shared_conditions": ["Actual same new boot/hart and assigned f9 BIT identity accepted by root",
            "No reset/reconfiguration/clock stop or counter write during calibration through the reported stamp pair",
            "Effective rate remains within applied Dell-relative bounds during reported stamp pair; analog drift not established by static source"],
        "cpu_extra_conditions": ["No debug_mode pause", "No CPU perf/PMU mcycle initial-value write"],
        "time_read_semantics": "Actual sample lies in trap's CLINT MMIO read; trap and bus latency not bounded here; full CAPS command still brackets that event.",
        "preferred_wording": "Instrumented ordinary phase elapsed time relative to Dell CLOCK_MONOTONIC, conditional on fixed-divider continuity and rate stability; raw CPU cycle/TIME/nominal ns retained separately.",
        "not_allowed": ["Exact point Hz from intended 50MHz/25MHz or CPU:TIME ratio",
            "Unconditional stage physical duration from three sampled averages",
            "Atomic simultaneous CPU/TIME endpoint assumption", "SI accuracy without independent Dell reference error bound",
            "Pure DMA/translation delay from phase() stamps"]
    },
    "acceptance": {
        "static_counter_and_clock_source_review_complete": True,
        "actual_calibration_accepted_by_this_seat": False,
        "actual_frequency_bounds": None,
        "actual_DMA_receipts_inspected": False,
        "physical_stability_verified": False,
        "run_counter_continuity_verified": False,
        "root_retains_board_execution_and_final_acceptance": True,
        "missing": ["actual new boot/BIT execution identity", "real CAPS event-time bounds",
            "actual RUN no-reset/no-counter-write/no-debug/no-clock-stop evidence", "independent analog rate-drift bound"]
    },
    "operations": {"UART": False, "JTAG": False, "SD": False, "network": False,
        "compile": False, "test": False, "model_or_old_replay": False, "CLI_validation": False,
        "shared_document_or_source_mutation": False, "extra_goal": False,
        "DeepSeek": False, "Claude": False, "writes_only_this_directory": True},
    "read_only_ELF_commands": commands,
    "collector_dell_monotonic_elapsed_ns": time.monotonic_ns() - start
}
write_json("counter-continuity.json", result)
write_json("artifact-hashes.json", {p.name: digest(p) for p in sorted(OUT.iterdir())
    if p.is_file() and p.name != "artifact-hashes.json"})
print(json.dumps({"directory": str(OUT), "bit_match": True, "firmware_ELF_manifest_match": True,
    "source_files": len(records), "actual_samples": 0, "board_actions": 0, "report": result["report"]}))

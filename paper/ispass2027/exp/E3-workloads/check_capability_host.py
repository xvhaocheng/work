#!/usr/bin/env python3
"""Exercise frozen CLI validators and a 2D geometry example; never access hardware."""

import argparse
import datetime
import json
from pathlib import Path
import subprocess
import tempfile


HARNESS = r'''
#undef main
int main(int argc, char **argv) {
 struct options o = {.device="/never-opened", .request={
  .abi_version=IDMA_EXPERIMENT_ABI, .struct_size=sizeof(struct idma_experiment),
  .aggressor_id=IDMA_EXPERIMENT_NO_AGGRESSOR, .victim_pages=1, .aggressor_pages=8,
  .chunk_bytes=4096, .run_timeout_ms=60000, .victim_transfers=4096,
  .warmup_transfers=256}};
 struct idma_experiment_caps c={.abi_version=IDMA_EXPERIMENT_ABI,
  .struct_size=sizeof(c), .capabilities=IDMA_EXPERIMENT_CAP_DID_HPM|
  IDMA_EXPERIMENT_CAP_SW_REFILL|IDMA_EXPERIMENT_CAP_HW_BATCH,
  .device_id=0, .did=10, .max_pages=4096, .page_bytes=4096,
  .max_run_timeout_ms=300000};
 char err[256]=""; const char *stage="arguments";
 int r=parse(argc,argv,&o,err,sizeof(err));
 if (!r) { stage="capabilities"; r=validate_caps(&o,&c,err,sizeof(err)); }
 if (!r) stage="accepted_host_checks";
 printf("{\"stage\":"); json_string(stage);
 printf(",\"error\":"); json_string(err);
 printf(",\"run_attempted\":false}\n");
 return r ? 2 : 0;
}
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--workspace", type=Path, default=Path(__file__).resolve().parents[4])
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    root = args.workspace.resolve()
    software = root / "iommu-platform/out/ispass2027-20261017-software01"
    source = software / "inputs/bench/idma_experiment.c"
    if not source.is_file() or not (software / "uapi/linux/idma.h").is_file():
        parser.error("frozen pathv3 CLI/UAPI missing; restore the manifest inputs first")

    cases = []
    for size in (64, 128, 256, 512, 1024, 4096):
        cases.append((f"sw-{size}", ["--chunk", str(size)], "accepted_host_checks"))
    for size in (1536, 1500, 16384, 65536):
        cases.append((f"sw-{size}", ["--chunk", str(size)], "capabilities"))
    for size in (64, 1536, 16384, 65536):
        cases.append((f"batch-{size}", ["--batch", "--chunk", str(size)], "arguments"))
    cases.extend([
        ("batch-4096", ["--batch", "--chunk", "4096"], "accepted_host_checks"),
        ("max-pages", ["--batch", "--victim-pages", "4096", "--aggressor-pages", "4096"], "accepted_host_checks"),
        ("too-many-pages", ["--victim-pages", "4097"], "capabilities"),
        ("independent-v-size", ["--victim-chunk", "64"], "arguments"),
        ("2d-row", ["--row-bytes", "1024"], "arguments"),
    ])
    # Only the wrapper main executes. Renaming the original main retains the
    # original static parse()/validate_caps() without running open()/ioctl().
    harness = "#define main original_idma_main\n#include " + json.dumps(str(source)) + "\n" + HARNESS
    flags = ["-O2", "-Wall", "-Wextra", "-Werror"]
    with tempfile.TemporaryDirectory(prefix="e3-host-caps-") as temporary:
        binary = str(Path(temporary) / "check")
        compile_command = ["gcc", *flags, "-I", str(software / "uapi"), "-x", "c", "-", "-o", binary]
        compilation = subprocess.run(compile_command, input=harness, text=True, capture_output=True, timeout=60)
        if compilation.returncode:
            raise RuntimeError(f"host compiler failed ({compilation.returncode}): {compilation.stderr}")
        records = []
        for name, arguments, expected in cases:
            run = subprocess.run([binary, *arguments], text=True, capture_output=True, timeout=5)
            observed = json.loads(run.stdout)
            passed = (observed["stage"] == expected and observed["run_attempted"] is False
                      and run.returncode == (0 if expected == "accepted_host_checks" else 2))
            records.append({"case": name, "argv": arguments, "expected_stage": expected,
                            "observed": observed, "exit_code": run.returncode, "passed": passed})

    rows, columns, element_bytes, pitch, tile_rows, tile_columns = 8, 256, 4, 4096, 2, 128
    length = tile_columns * element_bytes
    offsets = [(ty * tile_rows + row) * pitch + tx * length
               for ty in range(rows // tile_rows)
               for tx in range(columns // tile_columns)
               for row in range(tile_rows)]
    intervals = sorted((offset, offset + length) for offset in offsets)
    geometry_passed = (offsets[:4] == [0, 4096, 512, 4608]
                       and len(offsets) == len(set(offsets)) == 16
                       and all(first[1] <= second[0] for first, second in zip(intervals, intervals[1:]))
                       and sum(end - start for start, end in intervals) == rows * columns * element_bytes == 8192
                       and max(end for _, end in intervals) == 29696)
    geometry = {"kind": "desired_2d_synthesis_not_supported_by_current_uapi", "passed": geometry_passed,
                "rows": rows, "columns": columns, "element_bytes": element_bytes,
                "src_pitch_bytes": pitch, "dst_pitch_bytes": pitch,
                "tile_rows": tile_rows, "tile_columns": tile_columns,
                "iteration_order": "tile_y, tile_x, row_in_tile",
                "descriptor_count": len(offsets), "descriptor_bytes": length,
                "src_offsets": offsets, "dst_offsets": offsets,
                "copied_bytes": 8192, "address_span_bytes": 29696, "allocated_pages_each": 8}
    passed = all(record["passed"] for record in records) and geometry_passed
    report = {"schema": "e3.host-capability.v1", "recorded_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
              "kind": "original parse()/validate_caps() using synthetic pathv3 capabilities",
              "passed": passed, "host_only": True, "kernel_driver_executed": False,
              "hardware_access": False, "board_result": False,
              "source": str(source.relative_to(root)), "uapi_include": str((software / "uapi").relative_to(root)),
              "source_manifest": "paper/ispass2027/board/manifests/software-build.json",
              "compiler": subprocess.check_output(["gcc", "-dumpfullversion"], text=True, timeout=5).strip(),
              "compile_flags": flags, "compiler_stderr": compilation.stderr,
              "synthetic_capabilities": {"abi": 1, "page_bytes": 4096, "max_pages": 4096,
                                         "max_run_timeout_ms": 300000, "did_hpm": True,
                                         "sw_refill": True, "hw_batch": True},
              "cli_checks_count": len(records), "cases": records, "geometry": geometry}
    output = json.dumps(report, indent=2, ensure_ascii=False) + "\n"
    if args.output:
        with args.output.open("x", encoding="utf-8") as handle:
            handle.write(output)
        print(json.dumps({"output": str(args.output), "host_only": True,
                          "cli_checks_count": len(records), "geometry_passed": geometry_passed, "passed": passed}))
    else:
        print(output, end="")
    return 0 if passed else 1


if __name__ == "__main__":
    raise SystemExit(main())

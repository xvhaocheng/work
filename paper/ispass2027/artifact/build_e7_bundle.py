#!/usr/bin/env python3
"""Export the accepted E7 inputs/data and frozen RTL into a portable source bundle.

Does not prepare or modify E7 inputs, invoke Git, run simulations, or use FPGA.
The generated build.py only compiles; it never executes the resulting model.
"""
from __future__ import annotations

import argparse
from collections import Counter
import csv
import gzip
import hashlib
import io
import json
from pathlib import Path
import tarfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
E7 = Path("paper/ispass2027/exp/E7-capacity-sim")
FROZEN = Path("iommu-platform/out/invalidation-interference-20261017/frozen")
CAPACITIES = (16, 32, 64)

DRIVER = r'''#!/usr/bin/env python3
"""Verify this source bundle and compile E7; never execute simulation models."""
from __future__ import annotations
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import platform
import re
import shutil
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parent

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def save(path, data):
    path.write_text(json.dumps(data, indent=2, sort_keys=True) + '\n')

def verify():
    manifest = json.loads((ROOT / 'bundle-manifest.json').read_text())
    for relative, entry in manifest['files'].items():
        path = ROOT / relative
        if not path.is_file() or sha(path) != entry['sha256']:
            raise ValueError('Bundle content mismatch: ' + relative)
    closure = json.loads((ROOT / 'sources/source-manifest.json').read_text())
    for entry in closure['files']:
        if sha(ROOT / 'sources' / entry['logical_path']) != entry['sha256']:
            raise ValueError('Frozen source mismatch: ' + entry['logical_path'])
    print('VERIFIED: %d payload files; 114 frozen source/include/license files; no simulator run.' % len(manifest['files']), flush=True)
    return manifest

def version(command):
    result = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise RuntimeError('Unable to query tool version: ' + command[0])
    return result.stdout.splitlines()[0]

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--verify-only', action='store_true')
    parser.add_argument('--attempt', help='New directory name; existing attempts are never reused')
    parser.add_argument('--capacities', type=int, nargs='+', choices=(16,32,64), default=[16,32,64])
    parser.add_argument('--jobs', type=int, default=6)
    parser.add_argument('--timeout', type=int, default=900)
    args = parser.parse_args()
    manifest = verify()
    if args.verify_only:
        return 0
    if not args.attempt or not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]*', args.attempt):
        parser.error('--attempt must be a new simple directory name')
    if args.jobs < 1 or args.timeout < 1 or len(set(args.capacities)) != len(args.capacities):
        parser.error('jobs/timeout must be positive and capacities unique')
    for tool in ('verilator','g++','make'):
        if not shutil.which(tool):
            raise RuntimeError('Missing required tool: ' + tool)
    tools = dict(verilator=version(['verilator','--version']), cpp=version(['g++','--version']),
                 make=version(['make','--version']), python=platform.python_version())
    attempt = ROOT / 'build' / args.attempt
    attempt.mkdir(parents=True, exist_ok=False)
    all_results = []
    overall = dict(schema='e7-portable-compile-attempt-v1', identity='new_compile_only',
                   attempt=args.attempt, source_identity_sha256=manifest['source_identity_sha256'],
                   package_manifest_sha256=sha(ROOT/'bundle-manifest.json'),
                   tools=tools, started_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),
                   model_executed=False, fpga_used=False, capacities=all_results)
    save(attempt/'summary.json',overall)
    for capacity in args.capacities:
        out = attempt / str(capacity)
        out.mkdir()
        history = json.loads((ROOT / ('historical/builds/%d.json' % capacity)).read_text())
        template = history['portable_command_template']
        command = []
        for token in template:
            if token == '@JOBS@': token = str(args.jobs)
            elif token == '@OBJDIR@': token = str(out/'obj')
            elif token.startswith('-Isources/'): token = '-I' + str(ROOT/token[2:])
            elif token.startswith(('sources/','inputs/')): token = str(ROOT/token)
            command.append(token)
        record = dict(schema='e7-portable-compile-v1', identity='new_compile_only', capacity=capacity,
                      state='running', source_identity_sha256=manifest['source_identity_sha256'],
                      package_manifest_sha256=sha(ROOT/'bundle-manifest.json'),
                      historical_build_identity_sha256=history['original_manifest_sha256'],
                      driver_sha256=sha(Path(__file__)), tools=tools,
                      portable_command_template=template, build_jobs=args.jobs,
                      started_unix=time.time(), model_executed=False, fpga_used=False)
        save(out/'compile-manifest.json',record)
        print('COMPILE: capacity=%d, attempt=%s' % (capacity,args.attempt),flush=True)
        try:
            with (out/'build.log').open('w') as log:
                result = subprocess.run(command,cwd=ROOT,stdout=log,stderr=subprocess.STDOUT,timeout=args.timeout)
            record['exit_code'] = result.returncode
            record['state'] = 'passed' if result.returncode == 0 else 'failed'
        except subprocess.TimeoutExpired:
            record.update(state='timed_out',exit_code=124)
        except OSError as error:
            record.update(state='failed',exit_code=127,error=str(error))
        record['ended_unix'] = time.time()
        executable = out/'obj/Vinvalidation_dma_top'
        if record['state'] == 'passed':
            if not executable.is_file():
                record.update(state='failed',exit_code=1,error='Compiler returned success without executable')
            else:
                record['executable_sha256'] = sha(executable)
        record['log_sha256'] = sha(out/'build.log')
        save(out/'compile-manifest.json',record)
        all_results.append(record)
        overall['state'] = 'running' if record['state']=='passed' else 'failed'
        save(attempt/'summary.json',overall)
        print('RESULT: capacity=%d state=%s exit=%d' % (capacity,record['state'],record['exit_code']),flush=True)
        if record['state'] != 'passed':
            return record['exit_code'] or 1
    overall.update(state='passed',ended_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
    save(attempt/'summary.json',overall)
    verify()
    print('COMPLETED: compile only; executables were not run.',flush=True)
    return 0

if __name__ == '__main__':
    sys.exit(main())
'''

README = """# Portable E7 capacity compilation source bundle

This is a derivative export of accepted E7 inputs and 12 historical result cells.
It is not an anonymous rewrite of the historical manifests, a new experiment,
an FPGA image, or a claim that compilation repeats the recorded measurements.

Requirements: Python 3.8+, Verilator, a C++ compiler (g++), and make. The accepted
historical compiler was Verilator 4.038. Other versions are unverified until a
new compilation succeeds; RTL, source order and elaboration flags stay fixed.
No Git checkout, network access, production repository or historical absolute
path is needed after extracting this package. Paths resolve from build.py.

```sh
python3 build.py --verify-only
python3 build.py --attempt compile01 --capacities 16 32 64 --jobs 6
```

Use a new attempt name each time. build/ contains local objects, executables,
logs and new compile identities; they are excluded from the source archive.
The compiler is invoked with the original assertion/output-split/top-module,
IOTLB SV/C++ parameters and ordered source list. Only the relocated file paths,
new object directory and requested build parallelism differ. build.py never
runs the resulting executable, even after a successful build.

sources/ contains exactly the 114 manifest-listed source/include/license files.
inputs/ contains the actual E7 top, C++ workload, package stub and overlay patch;
these are copied byte-for-byte, not regenerated by prepare.py.

results.csv, analysis-import.original.json and runs/*/*/{summary.json,jobs.csv}
are unchanged accepted historical paper data. The receipt's run-manifest hashes
refer to original manifests, whose path-bearing bytes are intentionally absent.
historical/ contains explicitly labeled neutral identity exports: original
manifest SHA256, source refs/hashes, model/compile parameters, selected output
hashes and historical executable hashes. Those exports have their own hashes
and must never be passed off as the original manifests or used to overwrite them.
bundle-manifest.json maps every shipped payload to its hash and origin.

The model uses behavioral DDR (16 cycles to the head first beat, 8 B per beat),
ordinary MMIO supply and one active COPY per device; CPU/Linux and FPGA are not
executed. Preserve this simulation evidence level when using the data.

Upstream licenses and copyright notices remain in sources/ and source headers.
Source comments may identify upstream authors; this source archive is not an
anonymity certificate. No publisher PDFs, credentials, trace/log payloads,
generated C++ build products or old executables are included.
"""


def digest(data):
    return hashlib.sha256(data).hexdigest()


def encoded(value):
    return (json.dumps(value, indent=2, sort_keys=True) + "\n").encode()


def export(root, frozen, output):
    e7 = root / E7
    manifest_path = frozen / "source-manifest.json"
    closure_bytes = manifest_path.read_bytes()
    closure = json.loads(closure_bytes)
    receipt_bytes = (e7 / "analysis-import.json").read_bytes()
    receipt = json.loads(receipt_bytes)
    if not (receipt["status"] == "passed" and receipt["complete"] and len(receipt["cells"]) == 12):
        raise ValueError("E7 receipt is incomplete")
    rows = list(csv.DictReader(io.StringIO((e7 / "results.csv").read_text())))
    cells = {(int(row["capacity"]), row["condition"]): row for row in rows}
    expected = {(n, c) for n in CAPACITIES for c in ("Solo", "A1", "A8", "A64")}
    if len(rows) != 12 or set(cells) != expected:
        raise ValueError("Expected the accepted twelve E7 cells")
    if {(c["capacity"], c["condition"]) for c in receipt["cells"]} != expected:
        raise ValueError("Acceptance receipt cell set differs from the results")
    if digest((e7 / "results.csv").read_bytes()) != receipt["results_sha256"]:
        raise ValueError("Results do not match accepted receipt")
    output.mkdir(parents=True, exist_ok=False)
    files = {}
    consumed = {}

    def write(relative, data, *, origin, original_sha=None):
        path = output / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)
        files[relative] = dict(sha256=digest(data), bytes=len(data), origin=origin)
        if original_sha:
            files[relative]["original_manifest_sha256"] = original_sha

    def read(path):
        data = path.read_bytes()
        consumed[path] = digest(data)
        return data

    def copy(path, relative, expected_sha=None):
        data = read(path)
        if expected_sha and digest(data) != expected_sha:
            raise ValueError("Input hash mismatch: " + str(path))
        write(relative, data, origin="unchanged_historical_input")

    copy(manifest_path, "sources/source-manifest.json", digest(closure_bytes))
    for entry in closure["files"]:
        logical = entry["logical_path"]
        if Path(logical).is_absolute() or ".." in Path(logical).parts:
            raise ValueError("Unsafe frozen source path")
        copy(frozen / logical, "sources/" + logical, entry["sha256"])
    for directory in closure["include_roots"]:
        (output / "sources" / directory).mkdir(parents=True, exist_ok=True)
    for name in ("ariane_pkg_stub.sv", "invalidation_dma_top.sv", "capacity_dma.cpp"):
        copy(e7 / "inputs" / name, "inputs/" + name)
    copy(e7 / "test-overlay.patch", "inputs/test-overlay.patch")
    copy(e7 / "analysis-import.json", "analysis-import.original.json", digest(receipt_bytes))
    copy(e7 / "results.csv", "results.csv", receipt["results_sha256"])

    def neutral_path(path):
        if "/frozen/" in path:
            return "sources/" + path.split("/frozen/", 1)[1]
        if "/inputs/" in path:
            return "inputs/" + path.rsplit("/", 1)[1]
        raise ValueError("Unrecognized historical source path: " + path)

    def neutral_command(argv):
        result, next_role = [], None
        for token in argv:
            if next_role:
                result.append("@JOBS@" if next_role == "jobs" else "@OBJDIR@")
                next_role = None
            elif token in ("-j", "--Mdir"):
                result.append(token)
                next_role = "jobs" if token == "-j" else "obj"
            elif token.startswith("-I/"):
                result.append("-I" + neutral_path(token[2:]))
            elif token.startswith("/"):
                result.append(neutral_path(token))
            else:
                result.append(token)
        return result

    histories = {}
    for capacity in CAPACITIES:
        attempt = "calibration01" if capacity == 16 else "matrix01"
        original = e7 / "manifests" / attempt / f"build-{capacity}.json"
        data = read(original)
        build = json.loads(data)
        if build["state"] != "passed" or build["exit_code"] != 0 or build["capacity"] != capacity:
            raise ValueError("Historical build is not accepted")
        inputs = [{"path": neutral_path(item["path"]), "sha256": item["sha256"]} for item in build["sources"]]
        wanted = (["inputs/ariane_pkg_stub.sv"] + ["sources/" + p for p in closure["source_order"]]
                  + ["inputs/invalidation_dma_top.sv", "inputs/capacity_dma.cpp"])
        if [item["path"] for item in inputs] != wanted:
            raise ValueError("Historical source order mismatch")
        for item in inputs:
            if files[item["path"]]["sha256"] != item["sha256"]:
                raise ValueError("E7 input differs from actual historical build: " + item["path"])
        history = dict(schema="e7-historical-build-neutral-export-v1", identity="derivative_not_original_manifest",
                       original_manifest_sha256=digest(data), original_manifest_locator=f"manifests/{attempt}/build-{capacity}.json",
                       capacity=capacity, state=build["state"], exit_code=build["exit_code"],
                       tool_version=build["tool_version"], baseline_refs=build["baseline_refs"], model=build["model"],
                       original_executable_sha256=build["executable"]["sha256"],
                       original_driver_sha256=build["driver_sha256"], original_prepare_sha256=build["prepare_sha256"],
                       sources=inputs, portable_command_template=neutral_command(build["argv"]))
        write(f"historical/builds/{capacity}.json", encoded(history), origin="neutral_historical_identity_export", original_sha=digest(data))
        histories[capacity] = history

    for cell in receipt["cells"]:
        capacity, condition = cell["capacity"], cell["condition"]
        summary_relative = cells[capacity, condition]["source"]
        if Path(summary_relative).is_absolute() or ".." in Path(summary_relative).parts:
            raise ValueError("Unsafe result source path")
        directory = Path(summary_relative).parent
        for filename, key in (("summary.json", "summary_sha256"), ("jobs.csv", "jobs_sha256")):
            copy(e7 / directory / filename, str(directory / filename), cell[key])
        original = read(e7 / directory / "run-manifest.json")
        if digest(original) != cell["run_manifest_sha256"]:
            raise ValueError("Historical run-manifest hash mismatch")
        run = json.loads(original)
        if run["state"] != "passed" or run["exit_code"] != 0:
            raise ValueError("Run not accepted")
        if run["executable_sha256"] != histories[capacity]["original_executable_sha256"]:
            raise ValueError("Historical executable/build mismatch")
        history = dict(schema="e7-historical-run-neutral-export-v1", identity="derivative_not_original_manifest",
                       original_manifest_sha256=digest(original), original_manifest_locator=str(directory / "run-manifest.json"),
                       state=run["state"], exit_code=run["exit_code"], base_id=run["base_id"],
                       capacity=capacity, condition=condition, original_executable_sha256=run["executable_sha256"],
                       historical_build_export=f"historical/builds/{capacity}.json",
                       workload=dict(v_jobs=int(cells[capacity, condition]["v_jobs"]), a_pages=0 if condition == "Solo" else int(condition[1:])),
                       included_outputs={name: run["outputs"][name] for name in ("summary.json", "jobs.csv")},
                       omitted_outputs=sorted(name for name in run["outputs"] if name not in ("summary.json", "jobs.csv")))
        write(f"historical/runs/c{capacity}-{condition}.json", encoded(history), origin="neutral_historical_identity_export", original_sha=digest(original))

    write("build.py", DRIVER.encode(), origin="new_portable_compile_driver")
    write("README.md", README.encode(), origin="new_package_documentation")
    write(".gitignore", b"/build/\n/local/\n__pycache__/\n", origin="new_local_build_exclusions")
    refs = {key: closure[key] for key in ("cva6", "iommu", "register_interface", "axi_slice")}
    source_identity = digest(encoded({"refs": refs, "sources": {p: d["sha256"] for p, d in files.items() if p.startswith(("sources/", "inputs/"))}}))
    package = dict(schema="e7-portable-source-bundle-v1", identity="new_derivative_export", base_id=receipt["base_id"],
                   source_identity_sha256=source_identity, baseline_refs=refs,
                   original_source_manifest_sha256=digest(closure_bytes), original_acceptance_receipt_sha256=digest(receipt_bytes),
                   original_experiment_identities_unchanged=True, simulation_executed_by_exporter=False, fpga_used=False,
                   frozen_counts=dict(Counter(e["role"] for e in closure["files"])),
                   frozen_bytes=sum(files["sources/"+e["logical_path"]]["bytes"] for e in closure["files"]),
                   result_cells=12, files=files)
    (output / "bundle-manifest.json").write_bytes(encoded(package))
    for path, expected_sha in consumed.items():
        if digest(path.read_bytes()) != expected_sha:
            raise ValueError("Input changed during export: " + str(path))
    for path in output.rglob("*"):
        if path.is_file() and (b"/home/" in path.read_bytes() or b"work-root-b" in path.read_bytes()):
            raise ValueError("Private development path in package: " + str(path))
    return package


def archive(output, archive_path):
    archive_path.parent.mkdir(parents=True, exist_ok=True)
    if archive_path.exists():
        raise FileExistsError(archive_path)
    paths = sorted(p for p in output.rglob("*") if p.relative_to(output).parts[0] not in ("build", "local", "__pycache__"))
    with archive_path.open("xb") as stream:
        with gzip.GzipFile(fileobj=stream, filename="", mode="wb", mtime=0) as compressed:
            with tarfile.open(fileobj=compressed, mode="w") as tar:
                for path in paths:
                    info = tar.gettarinfo(str(path), arcname=str(Path(output.name) / path.relative_to(output)))
                    info.uid = info.gid = info.mtime = 0
                    info.uname = info.gname = ""
                    info.mode = 0o755 if path.is_dir() else 0o644
                    if path.is_file():
                        with path.open("rb") as data:
                            tar.addfile(info, data)
                    elif path.is_dir():
                        tar.addfile(info)
                    else:
                        raise ValueError("Nonportable package entry: " + str(path))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--frozen", type=Path, help="Existing frozen closure; default is under --root")
    parser.add_argument("--output", type=Path, required=True, help="New, nonexistent source-bundle directory")
    args = parser.parse_args()
    root, output = args.root.resolve(), args.output.resolve()
    package = export(root, (args.frozen or root / FROZEN).resolve(), output)
    target = output / "local" / (output.name + ".tar.gz")
    archive(output, target)
    print(json.dumps(dict(output=str(output), archive=str(target), archive_sha256=digest(target.read_bytes()),
                          frozen_counts=package["frozen_counts"], frozen_bytes=package["frozen_bytes"],
                          payload_files=len(package["files"]), source_identity_sha256=package["source_identity_sha256"]), indent=2))


if __name__ == "__main__":
    main()

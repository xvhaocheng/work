#!/usr/bin/env python3
"""Bounded validation of the actual production observer, with preserved inputs."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time

HERE = Path(__file__).resolve().parent
BASE = "8aeb9ac9b3aea82a975cc789cb47633fb29bf5c8"
RTL = "corev_apu/fpga/src/"
NAMES = ("dma_path_observer.sv", "dma_observer.sv")
FRONTEND = "corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv"


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def save(path, value):
    path.write_text(json.dumps(value, indent=2) + "\n")


def run(args):
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=False)
    source = args.cva6.resolve()
    manifest = {"schema": "ispass2027-e5-rtl-validation-v1", "status": "running",
                "base_cva6": BASE, "candidate_root": str(source),
                "start": time.time(), "commands": [], "sources": {},
                "scope": "Finite production observer RTL; no FPGA, driver, or board execution",
                "limits": {"build_timeout_s": 300, "simulation_timeout_s": 30}}
    manifest_path = out / "manifest.json"

    def command(argv, log, timeout):
        entry = {"argv": list(map(str, argv)), "cwd": str(out), "log": str(log.relative_to(out)),
                 "timeout_s": timeout, "start": time.time()}
        manifest["commands"].append(entry)
        save(manifest_path, manifest)
        with log.open("w") as stream:
            try:
                result = subprocess.run(entry["argv"], cwd=out, stdout=stream,
                                        stderr=subprocess.STDOUT, timeout=timeout)
            except subprocess.TimeoutExpired:
                entry.update(end=time.time(), status="timeout")
                save(manifest_path, manifest)
                raise
        entry.update(end=time.time(), returncode=result.returncode)
        save(manifest_path, manifest)
        if result.returncode:
            raise RuntimeError(f"command failed: {log}")

    def build(top, sources, test, label, parameters=()):
        obj = out / ("obj-" + label)
        command(["verilator", "--cc", "--exe", "--build", "-j", "4", "-Wno-fatal",
                 "--top-module", top, "--Mdir", obj, *parameters, *sources, test],
                out / (label + "-build.log"), 300)
        command([obj / ("V" + top)], out / (label + "-run.log"), 30)

    try:
        for sub in ("src/base", "src/candidate", "src/reference", "src/tests"):
            (out / sub).mkdir(parents=True)
        for name in NAMES:
            original = subprocess.check_output(["git", "-C", source, "show", BASE + ":" + RTL + name])
            (out / "src/base" / name).write_bytes(original)
            shutil.copyfile(source / RTL / name, out / "src/candidate" / name)
            reference = original.decode().replace("dma_path_stage", "dma_path_stage_reference")
            reference = reference.replace("dma_path_observer", "dma_path_observer_reference")
            reference = reference.replace("dma_observer", "dma_observer_reference")
            (out / "src/reference" / name).write_text(reference)
        for name in ("stage_top.sv", "stage_test.cpp", "path_v3_test.cpp", "run_rtl.py"):
            shutil.copyfile(HERE / name, out / "src/tests" / name)
        # The accepted P0 frontend is an explicit byte identity check, not a synthetic replacement.
        expected = subprocess.check_output(["git", "-C", source, "show", BASE + ":" + FRONTEND])
        if (source / FRONTEND).read_bytes() != expected:
            raise ValueError("P0 frontend changed relative to the accepted base")
        manifest["p0_frontend_unchanged"] = {"path": FRONTEND,
                "sha256": hashlib.sha256(expected).hexdigest(), "base": BASE}
        manifest["candidate_head"] = subprocess.check_output(["git", "-C", source, "rev-parse", "HEAD"], text=True).strip()
        (out / "candidate.patch").write_bytes(subprocess.check_output(["git", "-C", source, "diff", "--binary", BASE]))
        # Both real modules see the identical input nets; every low and high clock evaluation compares outputs.
        candidate_text = (out / "src/candidate/dma_observer.sv").read_text()
        ports = candidate_text.split(") (", 1)[1].split(");", 1)[0]
        compare = "module e5_observer_compare #(parameter NumPorts=4, AxiIdWidth=4) (" + ports + ",\noutput logic legacy_equal_o);\n"
        compare += "logic [63:0] reference_data;\n"
        compare += "dma_observer #(.PathEnable(1),.HistEnable(0)) candidate(.*);\n"
        compare += "dma_observer_reference #(.PathEnable(1)) reference_dut(.cfg_rdata_o(reference_data),.*);\n"
        compare += "assign legacy_equal_o = cfg_rdata_o == reference_data;\nendmodule\n"
        (out / "src/tests/e5_observer_compare.sv").write_text(compare)
        original_test = (HERE / "path_v3_test.cpp").read_text()
        equivalence = original_test.replace("Vdma_observer", "Ve5_observer_compare")
        old_tick = "void tick() { d.clk_i=0; d.eval(); d.clk_i=1; d.eval(); }"
        new_tick = """unsigned eq_cycles=0;
    void tick() {
        d.clk_i=0; d.eval(); assert(d.legacy_equal_o);
        d.clk_i=1; d.eval(); assert(d.legacy_equal_o);
        std::printf("EQ cycle=%u addr=%u data=%llu equal=%u\\n",eq_cycles++,unsigned(d.cfg_addr_i),
                    (unsigned long long)d.cfg_rdata_o,unsigned(d.legacy_equal_o));
    }"""
        if old_tick not in equivalence:
            raise ValueError("unrecognized original integration clock interface")
        equivalence = equivalence.replace(old_tick, new_tick)
        (out / "src/tests/equivalence.cpp").write_text(equivalence)
        # Reuse all original integration scenarios with histogram conservation checked at every valid freeze.
        histogram_test = original_test.replace("0x49444d414f425303ULL", "0x49444d414f425304ULL")
        marker = "    void arm(unsigned v=0,unsigned a=1,bool pair=false) {"
        helper = """    uint64_t hist(unsigned lane,unsigned stage,unsigned bin) {
        write(0x70,256+lane*128+stage*32+bin); return reg(0x78);
    }
    void check_hist() {
        write(0x70,192); assert(reg(0x78)==0x20202001ULL);
        for(unsigned lane=0;lane<4;lane++) for(unsigned stage=0;stage<4;stage++) {
            uint64_t total=0; for(unsigned bin=0;bin<32;bin++) total+=hist(lane,stage,bin);
            assert(total==get(lane,stage,1));
            std::printf("HIST lane=%u stage=%u full=%llu total=%llu\\n",lane,stage,
                        (unsigned long long)get(lane,stage,1),(unsigned long long)total);
        }
    }
"""
        if marker not in histogram_test:
            raise ValueError("unrecognized original integration arm interface")
        histogram_test = histogram_test.replace(marker, helper + marker)
        histogram_test = histogram_test.replace("d.poll_i=0;\n    }", "d.poll_i=0;\n        if(reg(0x60)==4) check_hist();\n    }")
        marker = "assert(b.get(ch,s,7)==duration[s]);"
        histogram_test = histogram_test.replace(marker, marker + "\n                unsigned bin=0, age=duration[s]; while(age){++bin;age>>=1;}\n                for(unsigned k=0;k<32;k++) assert(b.hist(ch,s,k)==(k==bin?1:0));")
        marker = '    std::printf("PASS path observer: %u integration scenarios\\n",cases);'
        extra = """    {
        Bench b; b.write(0x70,767); assert(b.reg(0x70)==767 && b.reg(0x78)==0);
        b.write(0x70,768); assert(b.reg(0x60)&(1ULL<<32));
        b.write(0x60,4); b.write(0x70,193); assert(b.reg(0x60)&(1ULL<<32)); cases++;
    }
"""
        if marker not in histogram_test:
            raise ValueError("unrecognized integration final summary")
        histogram_test = histogram_test.replace(marker, extra + marker)
        (out / "src/tests/histogram.cpp").write_text(histogram_test)
        for path in sorted((out / "src").rglob("*")):
            if path.is_file(): manifest["sources"][str(path.relative_to(out))] = sha(path)
        manifest["sources"]["candidate.patch"] = sha(out / "candidate.patch")
        save(manifest_path, manifest)
        candidate = [out / "src/candidate" / n for n in NAMES]
        reference = [out / "src/reference" / n for n in NAMES]
        build("e5_observer_compare", [*candidate, *reference, out / "src/tests/e5_observer_compare.sv"],
              out / "src/tests/equivalence.cpp", "equivalence")
        build("dma_observer", candidate, out / "src/tests/histogram.cpp", "histogram", ("-GPathEnable=1", "-GHistEnable=1"))
        build("stage_top", [candidate[0], out / "src/tests/stage_top.sv"], out / "src/tests/stage_test.cpp", "stage")
        manifest.update(status="passed", end=time.time())
        save(manifest_path, manifest)
        save(out / "summary.json", {"status": "passed", "scope": manifest["scope"],
             "base_cva6": BASE, "manifest": "manifest.json", "p0_frontend_unchanged": manifest["p0_frontend_unchanged"],
             "checks": ["HistEnable0 cycle equality with actual accepted base RTL",
                        "HistEnable1 actual observer integration and MMIO; full/bin conservation",
                        "Actual stage 32-bit bin boundaries, full FIFO replacement, carry membership, clear, invalidation",
                        "Actual 3-bit histogram saturation, overflow mask PATH invalidation; 8-bit timer wrap"],
             "logs": ["equivalence-run.log", "histogram-run.log", "stage-run.log"],
             "not_verified": ["Linux integration", "full Vivado implementation", "board measurements"]})
    except Exception as error:
        manifest.update(status="failed", end=time.time(), error=str(error))
        save(manifest_path, manifest)
        raise


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cva6", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    run(parser.parse_args())

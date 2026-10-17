#!/usr/bin/env python3
"""Run original module suites in this output tree, with explicit V2 policy adaptation."""
from pathlib import Path
import subprocess,hashlib,json,difflib,time,argparse,concurrent.futures
ROOT=Path(__file__).resolve().parents[3]
OUT=Path(__file__).resolve().parent.parent
IOMMU=ROOT/'cva6/corev_apu/riscv-iommu'
REFSHA='137f5e5fb1f2cc65a1346359226b16247c3f8445'
REL='rtl/translation_logic/rv_iommu_iotlb.sv'
ref=subprocess.check_output(['git','-C',str(IOMMU),'show',REFSHA+':'+REL],text=True).replace('module rv_iommu_iotlb #(','module rv_iommu_iotlb_reference #(')
# Independent timestamp-based reference, in place of old PLRU. It uses original
# 137f lookup/content/flush code, not V2's rank algorithm or current RTL body.
lru_ref=ref
lru_ref=lru_ref.replace('    // sequential process','''
    // Independent exact-LRU reference: monotonically increasing 64-bit stamps.
    longint unsigned stamp_q[RVIOMMUCfg.NumIotlbEntries];
    longint unsigned stamp_n[RVIOMMUCfg.NumIotlbEntries];
    longint unsigned counter_q, counter_n;
    always_comb begin : timestamp_history
        counter_n = counter_q;
        for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) stamp_n[i] = stamp_q[i];
        for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
            if (lu_hit[i]) begin
                counter_n += 1;
                stamp_n[i] = counter_n;
            end
        end
        if (update_i.update && !inval_i.inval_vma && !inval_i.inval_gvma) begin
            for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
                if (replace_en[i]) begin
                    counter_n += 1;
                    stamp_n[i] = counter_n;
                end
            end
        end
    end
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            counter_q <= 0;
            for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) stamp_q[i] <= 0;
        end else begin
            counter_q <= counter_n;
            for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) stamp_q[i] <= stamp_n[i];
        end
    end
    assign reference_victim_o = replace_en;

    // sequential process''')
start=lru_ref.index('        for (int unsigned i = 0; i < RVIOMMUCfg.NumIotlbEntries; i += 1)')
end=lru_ref.index('\n    end\n',start)
lru_ref=lru_ref[:start]+'''        begin
            int chosen;
            longint unsigned oldest;
            bit found_invalid;
            chosen = -1; oldest = '1; found_invalid = 0;
            replace_en = '0;
            for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
                if (!tags_q[i].valid && !found_invalid) begin
                    chosen = i; found_invalid = 1;
                end
            end
            if (!found_invalid) begin
                for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
                    if (chosen < 0 || stamp_q[i] < oldest) begin
                        chosen = i; oldest = stamp_q[i];
                    end
                end
            end
            if (chosen >= 0) replace_en[chosen] = 1'b1;
        end'''+lru_ref[end:]
lru_ref=lru_ref.replace('    // Lookup\n','    output logic [RVIOMMUCfg.NumIotlbEntries-1:0] reference_victim_o,\n\n    // Lookup\n',1)
def patch_tb(suite, text):
    if suite=='research':
        text=text.replace('victim_from_plru(top->tap_plru) != lowest_bit16(top->tap_replace_mask)', 'top->expected_lru_victim != top->tap_replace_mask')
        text=text.replace('replace_mask disagrees with the PLRU tree read-out','replace_mask disagrees with independent timestamp LRU reference')
        text=text.replace('replace_mask disagrees with the PLRU read-out','replace_mask disagrees with independent timestamp LRU reference')
    else:
        text=text.replace('    if (top->a_replace_mask != top->b_replace_mask || top->a_plru != top->b_plru) {','    if (top->a_replace_mask != top->expected_lru_victim) fail("victim disagrees with independent timestamp LRU reference (" + where + ")");\n    if (top->a_replace_mask != top->b_replace_mask || top->a_plru != top->b_plru) {')
        text=text.replace('if (top->a_lu_hit_o) fail("tm: the tag replaced by the second fill still hits");','if (!top->a_lu_hit_o) fail("tm: invalid-first LRU failed to preserve the first tag while empty ways remain");')
        text=text.replace('// Updates, lookup misses and invalidations leave the PLRU tree untouched', '// V2 uses invalid-first allocation and fill-MRU recency. The retained legacy\n    // PLRU observation is passive. Updates, misses and invalidations leave that tree untouched')
        text=text.replace('// (checked above and below), so the second fill re-used the victim way\n    // chosen for the first fill and replaced t: the tag that survives in the\n    // IOTLB is t2.', '// (checked above and below). LRU fills two distinct invalid ways, retaining\n    // both t and t2; the second fill does not evict the first tag.')
    return text

def prepare(v,suite):
    run=OUT/'regression'/f'v{v}'/suite
    run.mkdir(parents=True,exist_ok=True)
    rt=run/'rv_iommu_iotlb_reference.sv';rt.write_text(lru_ref if v==2 else ref)
    top_src=ROOT/'tests'/f'{suite}_iotlb_top.sv'
    top_text=top_src.read_text()
    if v==2:
        top_text=top_text.replace('    // 16 IOTLB entries','    // 16 IOTLB entries',1)
        # Add a scalar expected-victim port and wire the independent reference.
        marker='    input  logic                              clk_i,' if suite=='research' else '    input  logic                          clk_i,'
        assert marker in top_text
        top_text=top_text.replace(marker,'    output logic [15:0] expected_lru_victim,\n'+marker,1)
        marker='    ) i_reference ('
        assert marker in top_text
        top_text=top_text.replace(marker,marker+'\n        .reference_victim_o(expected_lru_victim),',1)
    top=run/top_src.name;top.write_text(top_text)
    tb_src=ROOT/'tests'/f'{suite}_iotlb_test.cpp'
    tb_text=tb_src.read_text(); tb=run/tb_src.name
    tb.write_text(patch_tb(suite,tb_text) if v==2 else tb_text)
    for name,before,after in [('top',top_src.read_text(),top_text),('testbench',tb_text,tb.read_text()),('reference',ref,rt.read_text())]:
        (OUT/'diffs'/f'v{v}-{suite}-{name}.diff').write_text(''.join(difflib.unified_diff(before.splitlines(True),after.splitlines(True),fromfile='existing/'+name,tofile=f'v{v}/'+name)))
    return run,rt,top,tb

def one(v,suite):
    run,rt,top,tb=prepare(v,suite)
    variant=OUT/'variants'/f'v{v}'/'overlay'/'cva6/corev_apu/riscv-iommu'/REL
    sources=[IOMMU/'packages/rv_iommu_reg_pkg.sv',IOMMU/'packages/rv_iommu_cfg_pkg.sv',IOMMU/'packages/rv_iommu_pkg.sv',variant,rt,top,tb]
    cmd=['verilator','--cc','--exe','--build','-j','2','-Wall','-Wno-fatal','--top-module',f'{suite}_iotlb_top','--Mdir',str(run/'obj')]+[str(p) for p in sources]
    rec={'variant':v,'suite':suite,'cwd':str(ROOT),'build_command':cmd,'inputs':[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in sources],'started':time.time()}
    with (run/'build.log').open('w') as f:
        p=subprocess.run(cmd,cwd=ROOT,stdout=f,stderr=subprocess.STDOUT,timeout=600)
    rec['build_exit_code']=p.returncode
    if p.returncode==0:
        exe=run/'obj'/f'V{suite}_iotlb_top';rec['run_command']=[str(exe)]
        with (run/'stdout.log').open('w') as f,(run/'stderr.log').open('w') as err:
            q=subprocess.run([str(exe)],cwd=ROOT,stdout=f,stderr=err,timeout=120)
        rec['run_exit_code']=q.returncode
        rec['summary_lines']=[s for s in (run/'stdout.log').read_text().splitlines() if s.startswith(('PASS','FAIL'))][-8:]
    rec['finished']=time.time();(run/'receipt.json').write_text(json.dumps(rec,indent=2)+'\n')
    return rec
if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('--variants',nargs='+',type=int,default=[0,1,2]);args=ap.parse_args()
    with concurrent.futures.ThreadPoolExecutor(max_workers=3) as ex:
        futures=[ex.submit(one,v,s) for v in args.variants for s in ['research','composition']]
        results=[f.result() for f in futures]
    (OUT/'regression'/'results.json').write_text(json.dumps(results,indent=2)+'\n')
    for r in results:print(r['variant'],r['suite'],r['build_exit_code'],r.get('run_exit_code'),r.get('summary_lines',[]),flush=True)
    raise SystemExit(0 if all(r['build_exit_code']==0 and r.get('run_exit_code')==0 for r in results) else 1)

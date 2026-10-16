#!/usr/bin/env python3
"""Create the private, fixed ACV-2x2 source adapters; never edit originals."""
from pathlib import Path
import difflib, hashlib, json
P=Path(__file__).resolve().parent
ROOT=P.parents[3]
assert (ROOT/'tests/nonblock_actual_dma_test.cpp').is_file(),ROOT
cpp_source=ROOT/'tests/nonblock_actual_dma_test.cpp'
mux_source=ROOT/'cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv'
s=cpp_source.read_text()
def replace_once(old,new):
 global s
 assert s.count(old)==1,(old[:100],s.count(old))
 s=s.replace(old,new)
replace_once('if (!(transfers == 64 || transfers == 1024 || transfers == 4096)) {','if (transfers != 2) {')
replace_once('if (!(transfers == 64 && a_target == 64ULL && v_gap == 0 &&\n              (a_pages == 1 || a_pages == 8) && warmup == 256 && ptw_lat == 31)) {','if (!(transfers == 2 && a_target == 2ULL && v_gap == 0 &&\n              a_pages == 2 && warmup == 0 && ptw_lat == 31 &&\n              mode == "n" && !cpu_enable && !ds_priority &&\n              !read_latency_overlap && write_tail_pause == 0 &&\n              !v_cold_dst && !owner_only && launch_order == "av" &&\n              start_delay == 0 && v_start_delay == 0 && a_start_delay == 0)) {')
replace_once('CHECK(full_rd_bytes[c] == 262144ULL,','CHECK(full_rd_bytes[c] == 8192ULL,')
replace_once('CHECK(full_wr_bytes[c] == 262144ULL,','CHECK(full_wr_bytes[c] == 8192ULL,')
s=s.replace('+transfers must be 64|1024|4096', '+transfers must be 2 for ACV-2x2')
s=s.replace('full-batch read payload bytes != 262144','ACV-2x2 read payload bytes != 8192').replace('full-batch write payload bytes != 262144','ACV-2x2 write payload bytes != 8192')
start=s.index('        // AXI allows R interleaving across different IDs. Promote only the')
end=s.index('        MemRead& h = mem_q.front();',start)
s=s[:start]+'        CHECK(!ds_priority, "ACV-2x2 forbids accepted-return priority");\n'+s[end:]
# Include both lanes' actual register configuration in the common ready epoch.
replace_once('        // ---- 2. configure BOTH batches before any trigger --------------','        if (full_batch) full_measure_begin();\n\n        // ---- 2. configure BOTH batches before any trigger --------------')
replace_once('        (void)enable;\n        full_measure_begin();','        (void)enable;\n        // ACV-2x2: common ready was captured before configuring either batch.')
s=s.replace('"actual-dma-full-batch-v1\\",','"research-acv-2x2-v1\\",')
# A private entry only accepts the explicit full-batch contract.
needle='    // The wrapper forwards --mode into the run-time nonblock_enable_i and'
assert s.count(needle)==1
s=s.replace(needle,'    if (!full_batch) {\n        std::fprintf(stderr, "ACV-2x2 requires +full-batch=1\\n");\n        return 2;\n    }\n\n'+needle)
assert 'std::rotate(mem_q' not in s
(P/'acv_test.cpp').write_text(s)
m=mux_source.read_text()
a=m.index('    rr_arb_tree #(',m.index('    // AR Channel\n'))
b=m.index('    spill_register #(',a)
old=m[a:b]
assert "i_ar_arbiter" in old and "i_aw_arbiter" not in old
assert "mst_ar_chan_t" in old and "mst_aw_chan_t" not in old
new='''    // Research-only adapter: the test's system mux has four inputs and local ID4.
    // The board uses a different vendor copy; no board equivalence is claimed.
    if (NoSlvPorts == 4 && SlvAxiIDWidth == 4) begin : gen_research_ar
      research_ar_selector #(.DataType(mst_ar_chan_t)) i_ar_arbiter (
        .clk_i(clk_i), .rst_ni(rst_ni),
        .req_i(slv_ar_valids), .gnt_o(slv_ar_readies), .data_i(slv_ar_chans),
        .gnt_i(ar_ready), .req_o(ar_valid), .data_o(mst_ar_chan), .idx_o()
      );
    end else begin : gen_original_ar
'''+old+'''    end

'''
m=m[:a]+new+m[b:]
(P/'axi_mux_private.sv').write_text(m)
files=[('driver',cpp_source,P/'acv_test.cpp'),('ar-selector-adapter',mux_source,P/'axi_mux_private.sv')]
manifest=[]
for kind,original,private in files:
 diff=''.join(difflib.unified_diff(original.read_text().splitlines(True),private.read_text().splitlines(True),fromfile=str(original),tofile=str(private)))
 (P/(kind+'.patch')).write_text(diff)
 manifest.append({'kind':kind,'original':str(original),'original_sha256':hashlib.sha256(original.read_bytes()).hexdigest(),'private':str(private),'private_sha256':hashlib.sha256(private.read_bytes()).hexdigest()})
(P/'adapter-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
print(json.dumps({'private_adapters':len(manifest),'accepted_return_reorder_removed':True,'common_ready':'before_both_batch_configurations'}))

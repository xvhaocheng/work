#!/usr/bin/env python3
"""Create isolated ablation RTL from the exact frozen discovery source."""
from pathlib import Path
import hashlib,json,difflib,subprocess
ROOT=Path(__file__).resolve().parents[3]
OUT=Path(__file__).resolve().parent.parent
REL=Path('cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv')
base=(ROOT/REL).read_text()
sha=hashlib.sha256(base.encode()).hexdigest()
expected='2868d6de26c8d98d2a750a0d78f6ce96cb896b9253b82b070a3e3772f37e9d26'
assert sha==expected,(sha,expected)
(OUT/'variants'/'discovery-rv_iommu_iotlb.sv').write_text(base)
compact_decl='''
    // V1: independent, near-balanced binary trees over the two fixed partitions.
    // Bits 0..12 are preorder internal nodes of 14 leaves (physical ways 2..15);
    // bit 13 is the two-leaf protected tree (physical ways 0..1).
    logic [13:0] compact_tree_q, compact_tree_n;
    logic [RVIOMMUCfg.NumIotlbEntries-1:0] legacy_replace_en;
    function automatic int compact_victim(input logic [13:0] tree, input bit protected_part);
        int lo, count, node, left_count;
        if (protected_part) return tree[13] ? 1 : 0;
        lo = 0; count = 14; node = 0;
        for (int level = 0; level < 4; level++) begin
            if (count > 1) begin
                left_count = count / 2;
                if (tree[node]) begin
                    lo += left_count;
                    node += left_count;
                    count -= left_count;
                end else begin
                    node += 1;
                    count = left_count;
                end
            end
        end
        return lo + 2;
    endfunction
    always_comb begin : compact_history
        compact_tree_n = compact_tree_q;
        if (partition_local_active) begin
            for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
                if (lu_hit[i] && (!TouchEnable || touch_i)) begin
                    if (i < 2) compact_tree_n[13] = (i == 0);
                    else begin
                        int lo, count, node, left_count;
                        lo = 0; count = 14; node = 0;
                        for (int level = 0; level < 4; level++) begin
                            if (count > 1) begin
                                left_count = count / 2;
                                if ((i - 2) < (lo + left_count)) begin
                                    compact_tree_n[node] = 1'b1;
                                    node += 1;
                                    count = left_count;
                                end else begin
                                    compact_tree_n[node] = 1'b0;
                                    lo += left_count;
                                    node += left_count;
                                    count -= left_count;
                                end
                            end
                        end
                    end
                end
            end
        end
        if (PolicyEnable && policy_clear_i) compact_tree_n = '0;
    end
    always_comb begin : compact_selection
        replace_en = legacy_replace_en;
        if (partition_local_active) begin
            replace_en = '0;
            replace_en[compact_victim(compact_tree_q, policy_mask_i == ProtectedWays)] = 1'b1;
        end
    end
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) compact_tree_q <= '0;
        else compact_tree_q <= compact_tree_n;
    end
'''
lru_decl='''
    // V2: total recency order, rank 0 = oldest and N-1 = newest.
    // Empty eligible ways have priority; lowest physical index breaks cold ties.
    localparam int LruRankW = $clog2(RVIOMMUCfg.NumIotlbEntries);
    logic [RVIOMMUCfg.NumIotlbEntries-1:0][LruRankW-1:0] lru_rank_q, lru_rank_n;
    logic [RVIOMMUCfg.NumIotlbEntries-1:0] legacy_replace_en;
    always_comb begin : exact_lru_selection
        int victim, oldest;
        bit found_invalid;
        victim = -1; oldest = RVIOMMUCfg.NumIotlbEntries; found_invalid = 0;
        replace_en = '0;
        for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
            if (allocation_mask[i] && !tags_q[i].valid && !found_invalid) begin
                victim = i; found_invalid = 1;
            end
        end
        if (!found_invalid) begin
            for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
                if (allocation_mask[i] && int'(lru_rank_q[i]) < oldest) begin
                    victim = i; oldest = int'(lru_rank_q[i]);
                end
            end
        end
        if (victim >= 0) replace_en[victim] = 1'b1;
    end
    always_comb begin : exact_lru_history
        lru_rank_n = lru_rank_q;
        // Same explicit touch permission as V0/V1; process duplicate matches in
        // ascending way order, the same deterministic order as the original RTL.
        for (int touched = 0; touched < RVIOMMUCfg.NumIotlbEntries; touched++) begin
            if (lu_hit[touched] && (!TouchEnable || touch_i)) begin
                for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
                    if (i != touched && lru_rank_n[i] > lru_rank_n[touched])
                        lru_rank_n[i] = lru_rank_n[i] - 1'b1;
                end
                lru_rank_n[touched] = LruRankW'(RVIOMMUCfg.NumIotlbEntries-1);
            end
        end
        // A committed fill becomes MRU after any same-edge old-entry hit.
        if (update_i.update && !inval_i.inval_vma && !inval_i.inval_gvma &&
            !(PolicyEnable && policy_clear_i)) begin
            for (int filled = 0; filled < RVIOMMUCfg.NumIotlbEntries; filled++) begin
                if (replace_en[filled]) begin
                    for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
                        if (i != filled && lru_rank_n[i] > lru_rank_n[filled])
                            lru_rank_n[i] = lru_rank_n[i] - 1'b1;
                    end
                    lru_rank_n[filled] = LruRankW'(RVIOMMUCfg.NumIotlbEntries-1);
                end
            end
        end
        if (PolicyEnable && policy_clear_i)
            for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) lru_rank_n[i] = LruRankW'(i);
    end
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni)
            for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) lru_rank_q[i] <= LruRankW'(i);
        else lru_rank_q <= lru_rank_n;
    end
'''
trace='''
`ifndef SYNTHESIS
    // CSV prefix,variant,local_cycle,instance,pscid,gscid,request_vpn,victim,
    // evicted_vpn,evicted_valid,valid_bitmap,eligible_mask,legacy_plru,kind,
    // active_tree,lru_ranks. Values are pre-edge; cycle 0 is first reset release.
    longint unsigned ablation_cycle_q;
    logic [RVIOMMUCfg.NumIotlbEntries-1:0] ablation_valid;
    always_comb begin
        for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++)
            ablation_valid[i] = tags_q[i].valid;
    end
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) ablation_cycle_q <= 0;
        else begin
            ablation_cycle_q <= ablation_cycle_q + 1;
            if (update_i.update && !inval_i.inval_vma && !inval_i.inval_gvma &&
                !(PolicyEnable && policy_clear_i)) begin
                for (int i = 0; i < RVIOMMUCfg.NumIotlbEntries; i++) begin
                    if (replace_en[i])
                        $display("IOTLB_VICTIM,VVAR,%0d,%m,%0d,%0d,%h,%0d,%h,%0d,%h,%h,%h,%s,%h,%h",
                            ablation_cycle_q, update_i.pscid, update_i.gscid,
                            update_i.vpn, i, {tags_q[i].vpn2,tags_q[i].vpn1,tags_q[i].vpn0},
                            tags_q[i].valid, ablation_valid, allocation_mask, plru_tree_q,
                            tags_q[i].valid ? "valid_evict" : "invalid_fill", ACTIVE, RANKS);
                end
            end
        end
    end
`endif
'''
for v in range(3):
    src=base
    if v:
        src=src.replace('replace_en[i] = en && allocation_mask[i];','legacy_replace_en[i] = en && allocation_mask[i];')
        src=src.replace('    // Only read the actual pre-edge and next state.',(compact_decl if v==1 else lru_decl)+'\n    // Only read the actual pre-edge and next state.')
    src=src.replace('\nendmodule',trace.replace('VVAR',str(v)).replace('ACTIVE',"(partition_local_active ? 15'(compact_tree_q) : plru_tree_q)" if v==1 else 'plru_tree_q').replace('RANKS', 'lru_rank_q' if v==2 else "64'b0")+'\nendmodule')
    target=OUT/'variants'/f'v{v}'/'overlay'/REL
    target.parent.mkdir(parents=True,exist_ok=True); target.write_text(src)
    (OUT/'diffs'/f'v{v}-vs-discovery.diff').write_text(''.join(difflib.unified_diff(base.splitlines(True),src.splitlines(True),fromfile='discovery/'+str(REL),tofile=f'v{v}/'+str(REL))))
(OUT/'variants'/'source-identity.json').write_text(json.dumps({'discovery_rtl_sha256':sha,'original_manifest':'../problem-discovery-run-20261017/build01/build-manifest.json','component_head':subprocess.check_output(['git','-C',str(ROOT/'cva6/corev_apu/riscv-iommu'),'rev-parse','HEAD'],text=True).strip(),'variants':{f'v{v}':{'path':str((OUT/'variants'/f'v{v}'/'overlay'/REL).relative_to(OUT)),'sha256':hashlib.sha256((OUT/'variants'/f'v{v}'/'overlay'/REL).read_bytes()).hexdigest()} for v in range(3)}},indent=2)+'\n')
print('Generated V0/V1/V2; discovery hash verified:',sha)

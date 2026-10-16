// Codex §50. Actual raw-parent ownership and direct per-parent logical returns.
// Narrow immutable shared normal-memory source, same-bank single 2KiB parent.
// Independent successful READ recipes required. WRITE remains in actual TC.
// Mode 0: closed long read; 1: open long read; 2: strong q-read control.
module shared_pretl_read #(
    parameter int unsigned ParentDepth = 4, Q = 2,
    parameter type ar_t = logic, parameter type r_t = logic
) (
    input logic clk_i, rst_ni, pause_i, cutover_i,
    input logic [1:0] mode_i,
    input logic [15:0] read_quantum_i,
    input logic keep_q_i,
    input logic [1:0][8:0] raw_written_i,
    input logic [1:0][8:0] write_recipe_end_i,
    input logic [1:0][8:0] promised_write_end_i,
    input logic a_write_quiet_i,
    output logic detached_o, a_hold_o, v_hold_o,
    output logic cutover_seen_o,
    output logic [63:0] cutover_vcut_bytes_o, cutover_oldend_bytes_o,
    input logic [1:0][ParentDepth-1:0] cache_valid_i, cache_failed_i,
    input logic [1:0][ParentDepth-1:0][55:0] cache_pa_i,
    output logic child_direct_o,
    output logic [1:0] flow_ready_o,
    input logic s_ar_valid_i, output logic s_ar_ready_o, input ar_t s_ar_i,
    output logic m_ar_valid_o, input logic m_ar_ready_i, output ar_t m_ar_o,
    input logic m_r_valid_i, output logic m_r_ready_o, input r_t m_r_i,
    output logic s_r_valid_o, input logic s_r_ready_i, output r_t s_r_o,
    input logic [1:0] job_fire_i, job_done_i, job_normal_i,
    input logic [1:0][63:0] job_src_i, job_dst_i, job_length_i,
    input logic v_aw_offer_i, v_aw_owner_i,
    input logic [63:0] v_aw_addr_i,
    input logic [7:0] v_aw_len_i,
    input logic [2:0] v_aw_size_i,
    output logic [31:0] parent_count_o, child_count_o, merged_count_o, parent_last_count_o,
    output logic [1:0][63:0] returned_bytes_o, coverage_o,
    output logic a_skid_o,
    output logic [31:0] a_child_count_o,
    output logic [63:0] a_child_bytes_o, detach_cut_bytes_o
);
    logic [8:0] shared_beats, solo_beats [2], certified_end_q [2];
    always_comb begin
        shared_beats = (mode_i == 0 || mode_i == 2) ? 9'(read_quantum_i) : 9'(256-returned_q[0]);
        if (shared_beats > 9'(256-returned_q[0])) shared_beats = 9'(256-returned_q[0]);
        if (mode_i == 0 || mode_i == 2) begin
            if (certified_end_q[0] <= returned_q[0] || certified_end_q[1] <= returned_q[0]) shared_beats = 0;
            else begin
                if (shared_beats > certified_end_q[0]-returned_q[0]) shared_beats = certified_end_q[0]-returned_q[0];
                if (shared_beats > certified_end_q[1]-returned_q[0]) shared_beats = certified_end_q[1]-returned_q[0];
            end
        end
        for (int f=0; f<2; f++) begin
            solo_beats[f] = 9'(read_quantum_i);
            if (solo_beats[f] > 9'(256-returned_q[f])) solo_beats[f] = 9'(256-returned_q[f]);
            if (certified_end_q[f] <= returned_q[f]) solo_beats[f] = 0;
            else if (solo_beats[f] > certified_end_q[f]-returned_q[f]) solo_beats[f] = certified_end_q[f]-returned_q[f];
        end
    end
    ar_t parent_q [2][ParentDepth];
    logic [31:0] pa_q [2], done_q [2];
    logic [63:0] src_q [2], dst_q [2], length_q [2];
    logic [1:0] active_q;
    logic [8:0] returned_q [2];
    logic offered_q, busy_q, child_owner_q;
    ar_t child_q;
    logic [8:0] child_offset_q, child_beats_q, child_got_q;
    logic [1:0] child_members_q;
    logic detached_q, detach_pending_q;
    // §57: close W-data prefix, sink irrevocable old physical R, then repair.
    // No payload added. The two logical parent cursors remain independent.
    logic [1:0] cutover_phase_q;
    logic [8:0] v_cut_q, old_end_q;
    logic [8:0] closure_target;
    logic start_cutover;
    logic skid_q;
    r_t skid_data_q;
    logic [1:0] skid_pending_q;
    logic [1:0] required;
    logic [8:0] beat_index;
    logic eligible_r, return_owner;
    logic held_ar_q, held_r_q;
    ar_t saved_ar_q;
    r_t saved_r_q;
    assign child_direct_o = 1'b1;
    assign detached_o = detached_q;
    // A fill R is serial behind the old V physical stream. Its NEW long W
    // must also stay behind that stream, even if resume arrives during old B.
    // Already offered q W remains irrevocable in the independent q writer.
    assign a_hold_o = pause_i || (detach_pending_q && !detached_q) ||
                      ((detached_q || mode_i==3) && done_q[0] == 0 && cutover_phase_q != 3);
    assign v_hold_o = start_cutover || cutover_phase_q == 1 || cutover_phase_q == 2 ||
                      (cutover_phase_q == 3 && returned_q[1] < v_cut_q);
    assign cutover_seen_o = cutover_phase_q >= 2;
    assign cutover_vcut_bytes_o = 64'(v_cut_q)*8;
    assign cutover_oldend_bytes_o = 64'(old_end_q)*8;
    always_comb begin
        closure_target = returned_q[0] > promised_write_end_i[0] ?
                         returned_q[0] : promised_write_end_i[0];
        // V's 256-beat raw WRITE starts at zero and uses q2 children.
        closure_target = (closure_target+9'd1) & 9'h1fe;
    end
    assign start_cutover = cutover_i && cutover_phase_q == 0 && (detached_q || mode_i == 3) && busy_q &&
                          !skid_q && pa_q[0] == 1 && pa_q[1] == 1 && done_q[0] == 0 && done_q[1] == 0 &&
                          cache_valid_i[0][0] && cache_valid_i[1][0] &&
                          certified_end_q[0] == 256 && certified_end_q[1] == 256 &&
                          closure_target < 256 && closure_target <= child_offset_q+child_beats_q;
    assign a_skid_o = skid_q && skid_pending_q[1];
    for (genvar f=0; f<2; f++) assign returned_bytes_o[f] = 64'(returned_q[f])*8;
    always_comb begin
        flow_ready_o = active_q & {pa_q[1]==0,pa_q[0]==0};
        s_ar_ready_o = flow_ready_o[s_ar_i.id[2]];
        m_ar_valid_o = offered_q; m_ar_o = child_q;
        beat_index = child_offset_q + child_got_q;
        required = child_members_q;
        if (detached_q && child_members_q == 2'b11) required[1] = 0;
        if (cutover_phase_q == 2) required = 0;
        if (cutover_phase_q == 3 && beat_index >= v_cut_q) required = 2'b11;
        eligible_r = busy_q && !skid_q && required != 0;
        for (int f=0; f<2; f++) begin
            if (required[f] && beat_index >= certified_end_q[f]) eligible_r = 0;
            if (required[f] && coverage_o[f] < (64'(beat_index)+1)*8) begin
                // Narrow B3/q2 aligned prototype: borrow at most two existing
                // private payload slots while active. Frozen skid reserves one
                // return per member; after pause only committed W debt passes.
                // Last logical R waits for W offer, preserving the V-tail AW
                // fence before any A fill AW can enter the common W ordering.
                if (returned_q[f] < raw_written_i[f] ||
                    returned_q[f]-raw_written_i[f] >= 2 || beat_index == 255 ||
                    beat_index+1 == certified_end_q[f] ||
                    (f == 1 && (pause_i || detach_pending_q || (detached_q && cutover_phase_q != 3))))
                    eligible_r = 0;
            end
        end
        if (cutover_phase_q == 1 && beat_index >= v_cut_q) eligible_r = 0;
        if (start_cutover) eligible_r = 0;
        // Sink real old responses including a held R, independently of DMA
        // FIFO availability. Never issue the repair AR until true RLAST.
        if (cutover_phase_q == 2 && busy_q && !skid_q) eligible_r = 1;
        m_r_ready_o = eligible_r;
        return_owner = !skid_pending_q[0];
        s_r_valid_o = skid_q;
        s_r_o = skid_data_q;
        s_r_o.id = parent_q[return_owner][0].id;
        s_r_o.last = returned_q[return_owner] == 9'(parent_q[return_owner][0].len);
    end
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            active_q <= 0; offered_q <= 0; busy_q <= 0; child_owner_q <= 0;
            child_q <= '0; child_offset_q <= 0; child_beats_q <= 0; child_got_q <= 0;
            child_members_q <= 0; detached_q <= 0; detach_pending_q <= 0;
            cutover_phase_q <= 0; v_cut_q <= 0; old_end_q <= 0;
            skid_q <= 0; skid_data_q <= '0; skid_pending_q <= 0;
            coverage_o <= '0; parent_count_o <= 0; child_count_o <= 0;
            merged_count_o <= 0; parent_last_count_o <= 0;
            a_child_count_o <= 0; a_child_bytes_o <= 0; detach_cut_bytes_o <= 0;
            held_ar_q <= 0; held_r_q <= 0; saved_ar_q <= '0; saved_r_q <= '0;
            for (int f=0; f<2; f++) begin
                pa_q[f] <= 0; done_q[f] <= 0; returned_q[f] <= 0;
                certified_end_q[f] <= 0;
                src_q[f] <= 0; dst_q[f] <= 0; length_q[f] <= 0;
                for (int n=0; n<ParentDepth; n++) parent_q[f][n] <= '0;
            end
        end else begin
            if (held_ar_q && (!m_ar_valid_o || m_ar_o != saved_ar_q)) $fatal(1,"SR50 held AR changed");
            if (held_r_q && (!s_r_valid_o || s_r_o != saved_r_q)) $fatal(1,"SR50 held logical R changed");
            held_ar_q <= m_ar_valid_o && !m_ar_ready_i; saved_ar_q <= m_ar_o;
            held_r_q <= s_r_valid_o && !s_r_ready_i; saved_r_q <= s_r_o;
            for (int f=0; f<2; f++) begin
                if (pause_i) $fatal(1,"SR54 successful-frontier fixture excludes external pause");
                if (cutover_i && !keep_q_i) $fatal(1,"SR57 cutover requires bounded q2 WRITE");
                if (read_quantum_i == 0 || read_quantum_i > 256) $fatal(1,"SR51 invalid read quantum");
                if (cache_valid_i[f][1] && write_recipe_end_i[f] > certified_end_q[f])
                    certified_end_q[f] <= write_recipe_end_i[f];
                if (job_fire_i[f]) begin
                    if (active_q[f] || !job_normal_i[f] || job_length_i[f] != 2048 ||
                        job_src_i[f][11:0] != 0 ||
                        (f==0 && job_dst_i[f][11:0] != 0) ||
                        (f==1 && job_dst_i[f][11:0] != 0 &&
                         (job_dst_i[f][11:0] <= 2048 || job_dst_i[f][2:0] != 0)))
                        $fatal(1,"SR54 prototype requires one aligned-source 2KiB normal job");
                    active_q[f] <= 1; src_q[f] <= job_src_i[f];
                    dst_q[f] <= job_dst_i[f]; length_q[f] <= job_length_i[f];
                end
                if (job_done_i[f]) begin
                    if (done_q[f] != 1 || returned_q[f] != 256) $fatal(1,"SR50 premature job done");
                    active_q[f] <= 0;
                end
                if (cache_failed_i[f][0]) $fatal(1,"SR50 READ recipe failed; no shared data admitted");
            end
            if (s_ar_valid_i && s_ar_ready_o) begin
                int f;
                f = int'(s_ar_i.id[2]);
                if (s_ar_i.addr != src_q[f] || s_ar_i.len != 255 || s_ar_i.size != 3 ||
                    s_ar_i.burst != 1 || s_ar_i.lock || s_ar_i.cache != 2 || s_ar_i.mmu_atst ||
                    s_ar_i.id != (f == 0 ? 4'd0 : 4'd4))
                    $fatal(1,"SR50 raw AR outside prototype contract owner=%0d id=%0d addr=%h src=%h len=%0d size=%0d burst=%0d lock=%0d cache=%0d atst=%0d", f,s_ar_i.id,s_ar_i.addr,src_q[f],s_ar_i.len,s_ar_i.size,s_ar_i.burst,s_ar_i.lock,s_ar_i.cache,s_ar_i.mmu_atst);
                parent_q[f][0] <= s_ar_i; pa_q[f] <= 1; parent_count_o <= parent_count_o+1;
            end
            if (v_aw_offer_i) begin
                if (v_aw_size_i != 3 || v_aw_addr_i < dst_q[v_aw_owner_i]) $fatal(1,"SR50 invalid W coverage");
                coverage_o[v_aw_owner_i] <= v_aw_addr_i-dst_q[v_aw_owner_i]+(64'(v_aw_len_i)+1)*8;
            end
            if ((mode_i == 0 || mode_i == 1) && !detached_q && certified_end_q[1] > 0 &&
                certified_end_q[1] < 256 && returned_q[1] >= certified_end_q[1]) detach_pending_q <= 1;
            // Future R participation closes once every first-VALID A-W
            // payload beat is already accepted by the real A DMA. Its old
            // registered AW/W/B obligations remain live in the q writer.
            if (detach_pending_q && !a_skid_o && returned_q[1] >= promised_write_end_i[1]) begin
                // Root §52: a release pin arriving before obligation closure
                // may retain the existing cohort if no delivered suffix gap
                // and no frozen skid exists. No first-VALID R/AR is altered.
                if (cache_valid_i[1][1] && write_recipe_end_i[1] > certified_end_q[1] &&
                    !skid_q && returned_q[0] == returned_q[1]) begin
                    detach_pending_q <= 0;
                end else begin
                    detached_q <= 1; detach_pending_q <= 0;
                    detach_cut_bytes_o <= 64'(returned_q[1])*8;
                end
            end
            if (start_cutover) begin
                // Mode3 started V independently, so it has never passed the
                // initial shared-cohort PA/normal-attribute check. Each READ
                // recipe is still independent; only equal immutable payload
                // may supply the joint logical tail after W debt closure.
                if (cache_pa_i[0][0] != cache_pa_i[1][0] ||
                    parent_q[0][0].cache != parent_q[1][0].cache ||
                    parent_q[0][0].prot != parent_q[1][0].prot ||
                    parent_q[0][0].qos != parent_q[1][0].qos ||
                    parent_q[0][0].region != parent_q[1][0].region ||
                    parent_q[0][0].user != parent_q[1][0].user)
                    $fatal(1,"SR59 late shared recipes or normal-memory attributes differ");
                if (mode_i == 3) begin
                    if (returned_q[1] != 0) $fatal(1,"SR59 independent V bootstrap expects untouched A cursor");
                    detached_q <= 1;
                    detach_cut_bytes_o <= 64'(returned_q[1])*8;
                end
                cutover_phase_q <= 1;
                v_cut_q <= closure_target;
                old_end_q <= child_offset_q+child_beats_q;
            end
            if (cutover_phase_q == 1 && !skid_q && returned_q[0] == v_cut_q &&
                raw_written_i[0] == v_cut_q && returned_q[0] >= promised_write_end_i[0]) begin
                cutover_phase_q <= 2;
            end
            if (!offered_q && !busy_q && !skid_q) begin
                if (cutover_phase_q == 2) begin
                    if (cache_valid_i[0][0] && cache_valid_i[1][0] &&
                        certified_end_q[0] == 256 && certified_end_q[1] == 256 &&
                        returned_q[1] <= v_cut_q && returned_q[0] == v_cut_q) begin
                        child_q <= parent_q[1][0];
                        child_q.addr <= 64'(cache_pa_i[1][0])+64'(returned_q[1])*8;
                        child_q.len <= 8'(255-returned_q[1]);
                        child_offset_q <= returned_q[1]; child_beats_q <= 9'(256-returned_q[1]);
                        child_members_q <= 2'b10; child_owner_q <= 1; offered_q <= 1;
                        child_count_o <= child_count_o+1; a_child_count_o <= a_child_count_o+1;
                        a_child_bytes_o <= a_child_bytes_o+64'(256-returned_q[1])*8;
                        cutover_phase_q <= 3;
                    end
                end else if (cutover_phase_q == 0) begin
                if (mode_i != 3 && !detached_q && pa_q[0] == 1 && pa_q[1] == 1 && done_q[0] == 0 && done_q[1] == 0 &&
                    cache_valid_i[0][0] && cache_valid_i[1][0] &&
                    cache_valid_i[0][1] && cache_valid_i[1][1] && shared_beats != 0 &&
                    certified_end_q[0] > returned_q[0] && certified_end_q[1] > returned_q[1] &&
                    (mode_i != 1 || certified_end_q[0] >= returned_q[0]+shared_beats)) begin
                    if (cache_pa_i[0][0] != cache_pa_i[1][0] || returned_q[0] != returned_q[1] ||
                        parent_q[0][0].cache != parent_q[1][0].cache ||
                        parent_q[0][0].prot != parent_q[1][0].prot ||
                        parent_q[0][0].qos != parent_q[1][0].qos ||
                        parent_q[0][0].region != parent_q[1][0].region ||
                        parent_q[0][0].user != parent_q[1][0].user)
                        $fatal(1,"SR50 shared recipes or normal-memory attributes differ");
                    child_q <= parent_q[0][0];
                    child_q.addr <= 64'(cache_pa_i[0][0])+64'(returned_q[0])*8;
                    child_q.len <= 8'(shared_beats-1);
                    child_offset_q <= returned_q[0];
                    child_beats_q <= shared_beats;
                    child_members_q <= 2'b11; child_owner_q <= 0; offered_q <= 1;
                    child_count_o <= child_count_o+1; merged_count_o <= merged_count_o+1;
                    a_child_count_o <= a_child_count_o+1;
                    a_child_bytes_o <= a_child_bytes_o+64'(shared_beats)*8;
                end else if (detached_q || mode_i == 3) begin
                    // Serial parent ownership: A fill only after the V physical child and skid retire.
                    for (int f=1; f>=0; f--) begin
                        if (pa_q[f] == 1 && done_q[f] == 0 && cache_valid_i[f][0] &&
                            (f == 0 || done_q[0] == 1) && solo_beats[f] != 0) begin
                            child_q <= parent_q[f][0];
                            child_q.addr <= 64'(cache_pa_i[f][0])+64'(returned_q[f])*8;
                            child_q.len <= 8'(solo_beats[f]-1);
                            child_offset_q <= returned_q[f]; child_beats_q <= solo_beats[f];
                            child_members_q <= f == 0 ? 2'b01 : 2'b10;
                            child_owner_q <= 1'(f); offered_q <= 1; child_count_o <= child_count_o+1;
                            if (f == 1) begin
                                a_child_count_o <= a_child_count_o+1;
                                a_child_bytes_o <= a_child_bytes_o+64'(solo_beats[f])*8;
                            end
                        end
                    end
                end
                end
            end
            if (m_ar_valid_o && m_ar_ready_i) begin
                offered_q <= 0; busy_q <= 1; child_got_q <= 0;
            end
            if (m_r_valid_i && m_r_ready_o) begin
                if (m_r_i.id != child_q.id) $fatal(1,"SR50 physical response ID mismatch");
                if (m_r_i.last != (child_got_q+1 == child_beats_q)) $fatal(1,"SR50 physical RLAST mismatch");
                if (cutover_phase_q >= 2 && m_r_i.resp != 0) $fatal(1,"SR57 sink/repair excludes physical READ errors");
                for (int f=0; f<2; f++)
                    if (required[f] && returned_q[f] != beat_index) $fatal(1,"SR50 duplicate or skipped logical beat");
                if (cutover_phase_q != 2) begin
                    skid_q <= 1; skid_data_q <= m_r_i; skid_pending_q <= required;
                end
                child_got_q <= child_got_q+1;
                if (m_r_i.last) busy_q <= 0;
            end
            if (s_r_valid_o && s_r_ready_i) begin
                returned_q[return_owner] <= returned_q[return_owner]+1;
                skid_pending_q[return_owner] <= 0;
                if (skid_pending_q == (return_owner ? 2'b10 : 2'b01)) skid_q <= 0;
                if (s_r_o.last) begin done_q[return_owner] <= 1; parent_last_count_o <= parent_last_count_o+1; end
            end
        end
    end
    final $display("SR50 mode=%0d parents=%0d children=%0d merged=%0d logical_lasts=%0d returned_v=%0d returned_a=%0d detached=%0d cut_bytes=%0d offered=%0d busy=%0d skid=%0d",mode_i,parent_count_o,child_count_o,merged_count_o,parent_last_count_o,returned_q[0],returned_q[1],detached_q,detach_cut_bytes_o,offered_q,busy_q,skid_q);
endmodule

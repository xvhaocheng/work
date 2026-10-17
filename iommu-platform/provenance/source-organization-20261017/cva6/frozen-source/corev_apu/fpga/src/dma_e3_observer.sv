// SPDX-License-Identifier: Solderpad-0.51
// Fixed-role E3: DMA0/DID10 victim, DMA1/DID11 aggressor. Passive identity
// scoreboard plus two bounded request records around one PTW perturbation.
module dma_e3_observer (
    input logic clk_i, rst_ni, arm_i, clear_i, active_i, open_i, close_i, abort_i,
    input logic delay128_i,
    input rv_iommu::e3_ctl_t ctl_i,
    input rv_iommu::e3_tl_t tl_i,
    input logic [1:0] tr_fire_i, out_valid_i, out_ready_i, return_i,
    input logic [3:0] tr_id_i[2], out_id_i[2], return_id_i[2],
    input logic [63:0] tr_address_i[2],
    input logic wlast_i,
    input logic [7:0] index_i,
    output logic [63:0] data_o,
    output logic error_o
);
    typedef struct packed { logic [31:0] seq, time_stamp; logic [63:0] address; } entry_t;
    entry_t queue_mem[4][4], selected;
    logic [1:0] head_q[4], tail_q[4];
    logic [2:0] count_q[4];
    logic [31:0] seq_q[4], comp_q[4], returned_q[4];
    logic [3:0] push, pop;
    logic [3:0][4:0] events;
    logic [1:0] selected_lane, current_lane_q;
    logic [1:0] out_hold_q;
    logic [31:0] time_q;
    logic run_q, bad_q, delay_q;
    logic [63:0] current_q[32], shot_q[2][32];
    logic [63:0] stage_data[4][4][8], write_data[32];
    logic [15:0] stage_error;
    logic write_error;
    logic [31:0] triggers_q, inserted_q, hits_q[4], misses_q[4];
    logic [31:0] capture_q, available_q, consume_q;
    logic [7:0] extra_q;
    logic [1:0] level_q;
    logic [63:0] pte_q;
    logic [2:0] state_at_capture_q, waiting_q;
    logic available_seen_q, consume_seen_q;
    logic head_v, capture_head_v_q;
    logic [5:0] gate_event;
    logic [31:0] gate_q[6];

    assign head_v = ctl_i.head_valid && ctl_i.head_did == 24'd10 && ctl_i.head_id == 0;
    // Count all candidate opportunities, including after the one-shot is used.
    always_comb begin
        gate_event = 0;
        gate_event[0] = active_i && tl_i.ptw_r_fire;
        gate_event[1] = gate_event[0] && tl_i.a_s1_miss;
        gate_event[2] = gate_event[1] && ctl_i.victim_waiting != 0;
        gate_event[3] = gate_event[1] && ctl_i.victim_waiting == 0;
        gate_event[4] = gate_event[2] && !head_v;
        gate_event[5] = active_i && tl_i.a_s1_miss && ctl_i.victim_waiting != 0;
    end
    assign selected_lane = {ctl_i.select_id == 8'd4, ctl_i.select_write};
    always_comb begin
        selected = queue_mem[selected_lane][head_q[selected_lane]];
        if (count_q[selected_lane] == 0) begin
            selected.seq = seq_q[selected_lane] + 1;
            selected.time_stamp = time_q;
            selected.address = tr_address_i[selected_lane[0]];
        end
        for (int lane=0; lane<4; lane++) begin
            push[lane] = tr_fire_i[lane%2] && tr_id_i[lane%2] == (lane<2 ? 0 : 4);
            pop[lane] = ctl_i.select_valid && selected_lane == 2'(lane);
            events[lane][0] = push[lane];
            events[lane][1] = pop[lane];
            events[lane][2] = ctl_i.request_fire && current_lane_q == 2'(lane);
            events[lane][3] = ctl_i.response_fire && current_lane_q == 2'(lane);
            events[lane][4] = out_valid_i[lane%2] && !out_hold_q[lane%2] && out_id_i[lane%2] == (lane<2 ? 0 : 4);
        end
        data_o = 0;
        if (index_i >= 96 && index_i < 224)
            data_o = stage_data[(index_i-96)/32][((index_i-96)%32)/8][index_i%8];
        else if (index_i >= 224) data_o = write_data[index_i-224];
        else if (index_i >= 32) data_o = shot_q[(index_i-32)/32][index_i%32];
        else case(index_i)
            0: data_o = 64'h49444d4145330002;
            1: data_o = {59'd0,ctl_i.queue_error,tl_i.error,write_error,|stage_error,bad_q};
            2: data_o = 64'(triggers_q);
            3: data_o = delay_q ? 128 : 0;
            4: data_o = 64'(capture_q);
            5: data_o = 64'(available_q);
            6: data_o = 64'(consume_q);
            7: data_o = 64'(extra_q);
            8: data_o = 64'(level_q);
            9: data_o = pte_q;
            10: data_o = shot_q[1][0];
            11: data_o = shot_q[0][0];
            12: data_o = 64'(inserted_q);
            13: data_o = {62'd0,consume_seen_q,available_seen_q};
            14: data_o = 64'(state_at_capture_q);
            15: data_o = 64'(waiting_q);
            16: data_o = 64'(time_q);
            17,18,19,20: data_o = 64'(hits_q[index_i-17]);
            21,22,23,24: data_o = 64'(misses_q[index_i-21]);
            25,26,27,28,29,30: data_o = 64'(gate_q[index_i-25]);
            31: data_o = 64'(capture_head_v_q);
            default: ;
        endcase
    end
    assign error_o = bad_q || |stage_error || write_error || tl_i.error || ctl_i.queue_error;
    for(genvar lane=0;lane<4;lane++) begin : gen_lane
        for(genvar stage=0;stage<4;stage++) begin : gen_stage
            dma_path_stage i_stage (
                .clk_i,.rst_ni,.clear_i(clear_i || arm_i),.run_i(run_q),
                .active_i,.open_i,.close_i,.enter_i(events[lane][stage]),
                .leave_i(events[lane][stage+1]),.time_i(time_q),
                .error_o(stage_error[lane*4+stage]),.data_o(stage_data[lane][stage])
            );
        end
    end
    dma_e3_write_observer i_writes (
        .clk_i,.rst_ni,.clear_i(clear_i || arm_i),.run_i(run_q),.active_i,.open_i,.close_i,
        .time_i(time_q),.aw_i(out_valid_i[1] && out_ready_i[1]),.aw_id_i(out_id_i[1]),
        .wlast_i,.b_i(return_i[1]),.b_id_i(return_id_i[1]),
        .data_o(write_data),.error_o(write_error)
    );
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            run_q<=0; bad_q<=0; time_q<=0; out_hold_q<=0; current_lane_q<=0; delay_q<=0;
            triggers_q<=0; inserted_q<=0; capture_q<=0; available_q<=0; consume_q<=0;
            available_seen_q<=0; consume_seen_q<=0; extra_q<=0; level_q<=0; pte_q<=0;
            state_at_capture_q<=0; waiting_q<=0; capture_head_v_q<=0;
            for(int k=0;k<6;k++) gate_q[k]<=0;
            for(int lane=0;lane<4;lane++) begin
                head_q[lane]<=0; tail_q[lane]<=0; count_q[lane]<=0; seq_q[lane]<=0;
                comp_q[lane]<=0; returned_q[lane]<=0; hits_q[lane]<=0; misses_q[lane]<=0;
            end
            for(int k=0;k<32;k++) begin current_q[k]<=0; shot_q[0][k]<=0;shot_q[1][k]<=0;end
        end else if(clear_i || arm_i) begin
            run_q<=arm_i; bad_q<=arm_i && (ctl_i.queue_error || ctl_i.victim_waiting != 0); time_q<=0; out_hold_q<=0; current_lane_q<=0; delay_q<=delay128_i;
            triggers_q<=0; inserted_q<=0; capture_q<=0; available_q<=0; consume_q<=0;
            available_seen_q<=0; consume_seen_q<=0; extra_q<=0; level_q<=0; pte_q<=0;
            state_at_capture_q<=0; waiting_q<=0; capture_head_v_q<=0;
            for(int k=0;k<6;k++) gate_q[k]<=0;
            for(int lane=0;lane<4;lane++) begin
                head_q[lane]<=0; tail_q[lane]<=0; count_q[lane]<=0; seq_q[lane]<=0;
                comp_q[lane]<=0; returned_q[lane]<=0; hits_q[lane]<=0; misses_q[lane]<=0;
            end
            for(int k=0;k<32;k++) begin current_q[k]<=0; shot_q[0][k]<=0;shot_q[1][k]<=0;end
        end else if(run_q) begin
            time_q<=time_q+1;
            if(&time_q) bad_q<=1;
            if(close_i || abort_i) run_q<=0;
            if(ctl_i.queue_error) bad_q<=1;
            for(int k=0;k<6;k++) if(gate_event[k]) begin
                gate_q[k]<=gate_q[k]+1;
                if(&gate_q[k]) bad_q<=1;
            end
            for(int ch=0;ch<2;ch++) begin
                out_hold_q[ch]<=out_valid_i[ch] && !out_ready_i[ch];
                if ((tr_fire_i[ch] && tr_id_i[ch]!=0 && tr_id_i[ch]!=4) ||
                    (out_valid_i[ch] && out_id_i[ch]!=0 && out_id_i[ch]!=4) ||
                    (return_i[ch] && return_id_i[ch]!=0 && return_id_i[ch]!=4)) bad_q<=1;
            end
            for(int lane=0;lane<4;lane++) begin
                if(push[lane]) begin
                    queue_mem[lane][tail_q[lane]] <= '{seq_q[lane]+1,time_q,tr_address_i[lane%2]};
                    tail_q[lane]<=tail_q[lane]+1'b1; seq_q[lane]<=seq_q[lane]+1;
                    if((&seq_q[lane]) || (count_q[lane]==4 && !pop[lane])) bad_q<=1;
                end
                if(pop[lane]) begin
                    head_q[lane]<=head_q[lane]+1'b1;
                    if(count_q[lane]==0 && !push[lane]) bad_q<=1;
                end
                case({push[lane],pop[lane]})
                    2'b10:count_q[lane]<=count_q[lane]+1'b1;
                    2'b01:count_q[lane]<=count_q[lane]-1'b1;
                    default:;
                endcase
                if(events[lane][4]) begin
                    comp_q[lane]<=comp_q[lane]+1;
                    if(&comp_q[lane]) bad_q<=1;
                    for(int shot=0;shot<2;shot++) if(triggers_q!=0 &&
                        shot_q[shot][1] == (lane<2 ? 0 : 4) && shot_q[shot][13] == 64'(lane%2) &&
                        shot_q[shot][0] == 64'(comp_q[lane]+1)) begin
                        shot_q[shot][10]<=64'(time_q); shot_q[shot][4][5]<=1;
                    end
                end
                if(return_i[lane%2] && return_id_i[lane%2] == (lane<2 ? 0 : 4)) begin
                    returned_q[lane]<=returned_q[lane]+1;
                    if(&returned_q[lane]) bad_q<=1;
                    for(int shot=0;shot<2;shot++) if(triggers_q!=0 &&
                        shot_q[shot][1] == (lane<2 ? 0 : 4) && shot_q[shot][13] == 64'(lane%2) &&
                        shot_q[shot][0] == 64'(returned_q[lane]+1)) begin
                        shot_q[shot][11]<=64'(time_q); shot_q[shot][4][6]<=1;
                    end
                end
            end
            if(ctl_i.select_valid) begin
                current_lane_q<=selected_lane;
                for(int k=0;k<32;k++) current_q[k]<=0;
                current_q[0]<=64'(selected.seq);current_q[1]<=64'(ctl_i.select_id);
                current_q[2]<=64'(ctl_i.select_did);current_q[3]<=ctl_i.select_iova;
                current_q[4]<=3;current_q[5]<=64'(selected.time_stamp);current_q[6]<=64'(time_q);
                current_q[12]<=2;current_q[13]<=64'(ctl_i.select_write);
                if((ctl_i.select_id!=0 && ctl_i.select_id!=4) ||
                    ctl_i.select_did != (selected_lane<2 ? 24'd10 : 24'd11) ||
                    ctl_i.select_iova != selected.address) bad_q<=1;
                if(triggers_q!=0 && !shot_q[1][4][1]) begin
                    if(selected_lane==0 && selected.seq==32'(shot_q[1][0])) begin
                        shot_q[1][6]<=64'(time_q);shot_q[1][4][1]<=1;
                    end else begin
                        inserted_q<=inserted_q+1;
                        if(&inserted_q) bad_q<=1;
                        for(int slot=0;slot<2;slot++) if(inserted_q==32'(slot)) begin
                            shot_q[0][14+slot*8]<=64'(selected.seq);
                            shot_q[0][15+slot*8]<=64'(ctl_i.select_id);
                            shot_q[0][16+slot*8]<=64'(ctl_i.select_did);
                            shot_q[0][17+slot*8]<=ctl_i.select_iova;
                            shot_q[0][18+slot*8]<=64'(ctl_i.select_write);
                            shot_q[0][19+slot*8]<=64'(selected.time_stamp);
                            shot_q[0][20+slot*8]<=64'(time_q);
                            shot_q[0][21+slot*8]<=2;
                            shot_q[0][31]<=64'(slot+1);
                        end
                        if(inserted_q>=2) shot_q[0][30]<=1; // Trace truncated; total count remains available.
                    end
                end
            end
            if(ctl_i.request_fire) begin
                current_q[7]<=64'(time_q);current_q[4][2]<=1;
                for(int shot=0;shot<2;shot++) if(triggers_q!=0 && current_q[0]==shot_q[shot][0] &&
                    current_q[1]==shot_q[shot][1] && current_q[13]==shot_q[shot][13]) begin
                    shot_q[shot][7]<=64'(time_q);shot_q[shot][4][2]<=1;
                end
            end
            if(tl_i.lookup) begin
                current_q[8]<=64'(time_q);current_q[4][3]<=1;current_q[12]<=64'(tl_i.hit);
                if(!tl_i.normal || tl_i.did!=32'(current_q[2]) || current_q[4][3]) bad_q<=1;
                if(active_i) begin
                    if(tl_i.hit) begin hits_q[current_lane_q]<=hits_q[current_lane_q]+1;if(&hits_q[current_lane_q])bad_q<=1;end
                    else begin misses_q[current_lane_q]<=misses_q[current_lane_q]+1;if(&misses_q[current_lane_q])bad_q<=1;end
                end
                for(int shot=0;shot<2;shot++) if(triggers_q!=0 && current_q[0]==shot_q[shot][0] &&
                    current_q[1]==shot_q[shot][1] && current_q[13]==shot_q[shot][13]) begin
                    shot_q[shot][8]<=64'(time_q);shot_q[shot][4][3]<=1;shot_q[shot][12]<=64'(tl_i.hit);
                end
            end
            if(tl_i.lookup && triggers_q!=0) begin
                for(int slot=0;slot<2;slot++) if(shot_q[0][14+slot*8]!=0 &&
                    shot_q[0][14+slot*8]==current_q[0] && shot_q[0][15+slot*8]==current_q[1] &&
                    shot_q[0][16+slot*8]==current_q[2] && shot_q[0][18+slot*8]==current_q[13])
                    shot_q[0][21+slot*8]<=64'(tl_i.hit);
            end
            if(ctl_i.response_fire) begin
                current_q[9]<=64'(time_q);current_q[4][4]<=1;
                if(ctl_i.response_error) bad_q<=1;
                for(int shot=0;shot<2;shot++) if(triggers_q!=0 && current_q[0]==shot_q[shot][0] &&
                    current_q[1]==shot_q[shot][1] && current_q[13]==shot_q[shot][13]) begin
                    shot_q[shot][9]<=64'(time_q);shot_q[shot][4][4]<=1;
                end
            end
            if(tl_i.capture) begin
                triggers_q<=triggers_q+1; capture_q<=time_q; level_q<=tl_i.level;pte_q<=tl_i.pte_address;
                state_at_capture_q<=ctl_i.state;waiting_q<=count_q[0];capture_head_v_q<=head_v;
                for(int k=0;k<32;k++) shot_q[0][k]<=current_q[k];
                shot_q[1][0]<=64'(queue_mem[0][head_q[0]].seq);
                shot_q[1][1]<=0;shot_q[1][2]<=10;shot_q[1][3]<=queue_mem[0][head_q[0]].address;
                shot_q[1][4]<=1;shot_q[1][5]<=64'(queue_mem[0][head_q[0]].time_stamp);
                shot_q[1][12]<=2;shot_q[1][13]<=0;
                // Snapshot counters before this edge; not a full IOTLB state dump.
                for(int lane=0;lane<4;lane++) begin
                    shot_q[1][14+lane]<=64'(hits_q[lane]);
                    shot_q[1][18+lane]<=64'(misses_q[lane]);
                    shot_q[1][22+lane]<=64'(seq_q[lane]);
                    shot_q[1][26+lane]<=64'(comp_q[lane]);
                end
                shot_q[1][30]<={31'd0,ctl_i.head_valid,ctl_i.head_did,ctl_i.head_id};
                shot_q[1][31]<=ctl_i.head_iova;
                if(triggers_q!=0 || !active_i || current_lane_q<2 || current_q[12]!=0 ||
                    !tl_i.ptw_r_fire || !tl_i.a_s1_miss || ctl_i.queue_error || count_q[0]==0 ||
                    ctl_i.victim_waiting!=32'(count_q[0])) bad_q<=1;
            end
            if(tl_i.available) begin available_q<=time_q;available_seen_q<=1;extra_q<=tl_i.extra;end
            if(tl_i.consume) begin consume_q<=time_q;consume_seen_q<=1;extra_q<=tl_i.extra;end
        end
    end
endmodule

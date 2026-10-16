// PRIVATE passive readback mailbox. No AXI ready/valid outputs.
// Bundled index/data stay stable until the synchronized toggle is acknowledged.
module mig_capture_cdc (
  input logic src_clk_i, src_rst_ni,
  input logic [12:0] index_i,
  input logic invalidate_i, arm_i, open_i, close_i,
  output logic [63:0] data_o, status_o,
  input logic dst_clk_i, dst_rst_ni,
  output logic arm_o, window_o,
  output logic rd_req_o,
  output logic [11:0] rd_index_o,
  input logic rd_valid_i,
  input logic [63:0] rd_data_i
);
  logic request_q, response_q, pending_q, valid_q;
  logic [11:0] index_hold_q, cached_index_q;
  logic [63:0] result_hold_q, cached_data_q;
  logic arm_toggle_q, window_q;
  (* ASYNC_REG = "TRUE" *) logic req_meta_q, req_sync_q;
  (* ASYNC_REG = "TRUE" *) logic rsp_meta_q, rsp_sync_q;
  (* ASYNC_REG = "TRUE" *) logic arm_meta_q, arm_sync_q;
  (* ASYNC_REG = "TRUE" *) logic win_meta_q, win_sync_q;
  logic req_seen_q, arm_seen_q, waiting_q;
  wire query = index_i >= 13'h1000 && index_i < 13'h1e00;
  assign data_o = valid_q && cached_index_q == index_i[11:0] ? cached_data_q : '0;
  always_comb begin
    status_o = '0;
    status_o[0] = valid_q;
    status_o[1] = pending_q;
    status_o[27:16] = cached_index_q;
    status_o[63:32] = 32'h4d434301;
  end
  always_ff @(posedge src_clk_i or negedge src_rst_ni) begin
    if (!src_rst_ni) begin
      request_q <= 0; pending_q <= 0; valid_q <= 0;
      index_hold_q <= 0; cached_index_q <= 0; cached_data_q <= 0;
      rsp_meta_q <= 0; rsp_sync_q <= 0;
      arm_toggle_q <= 0; window_q <= 0;
    end else begin
      rsp_meta_q <= response_q;
      rsp_sync_q <= rsp_meta_q;
      if (arm_i) begin arm_toggle_q <= !arm_toggle_q; window_q <= 0; end
      if (open_i) window_q <= 1;
      if (close_i) window_q <= 0;
      if (invalidate_i) valid_q <= 0;
      if (pending_q && rsp_sync_q == request_q) begin
        cached_data_q <= result_hold_q;
        cached_index_q <= index_hold_q;
        valid_q <= 1;
        pending_q <= 0;
      end
      if (query && !pending_q && (!valid_q || cached_index_q != index_i[11:0])) begin
        index_hold_q <= index_i[11:0];
        request_q <= !request_q;
        pending_q <= 1;
        valid_q <= 0;
      end
    end
  end
  always_ff @(posedge dst_clk_i or negedge dst_rst_ni) begin
    if (!dst_rst_ni) begin
      req_meta_q <= 0; req_sync_q <= 0; req_seen_q <= 0;
      response_q <= 0; waiting_q <= 0; result_hold_q <= 0;
      arm_meta_q <= 0; arm_sync_q <= 0; arm_seen_q <= 0;
      win_meta_q <= 0; win_sync_q <= 0;
      arm_o <= 0; window_o <= 0; rd_req_o <= 0; rd_index_o <= 0;
    end else begin
      req_meta_q <= request_q; req_sync_q <= req_meta_q;
      arm_meta_q <= arm_toggle_q; arm_sync_q <= arm_meta_q;
      arm_seen_q <= arm_sync_q; arm_o <= arm_sync_q != arm_seen_q;
      win_meta_q <= window_q; win_sync_q <= win_meta_q; window_o <= win_sync_q;
      rd_req_o <= 0;
      if (req_sync_q != req_seen_q && !waiting_q) begin
        rd_index_o <= index_hold_q;
        rd_req_o <= 1;
        waiting_q <= 1;
        req_seen_q <= req_sync_q;
      end
      if (waiting_q && rd_valid_i) begin
        result_hold_q <= rd_data_i;
        response_q <= req_seen_q;
        waiting_q <= 0;
      end
    end
  end
endmodule

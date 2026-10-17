// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Finite two-bank, S1/Sv39/4KiB hit path. Misses and unsupported requests are
// left unaccepted and subsequently use the normal translation/fault path.
// The DDTC and IOTLB ports below are time-shared with the original walker.
module rv_iommu_nonblock_hit
  import rv_iommu::*;
#(
  parameter int unsigned PAddrWidth = 56,
  parameter bit HasMsiContext = 0,
  parameter type dc_t = logic,
  parameter type req_t = logic,
  parameter type resp_t = logic,
  parameter type ddtp_t = logic
)(
  input logic clk_i, rst_ni,
  input logic enable_i, window_i,
  input req_t candidate_i,
  input logic candidate_valid_i, normal_fire_i,
  input logic invalidate_i,
  input ddtp_t ddtp_i,
  output logic intercept_o, accept_o, busy_o,
  output logic ddtc_lookup_o, ddtc_touch_o,
  output device_id_t ddtc_did_o,
  input logic ddtc_hit_i,
  input dc_t ddtc_dc_i,
  output logic iotlb_lookup_o, iotlb_touch_o,
  output logic [GPPNW39-1:0] iotlb_vpn_o,
  output pscid_t iotlb_pscid_o,
  input logic iotlb_hit_i,
  input iotlb_content_t iotlb_content_i,
  output resp_t response_o,
  output logic response_valid_o,
  input logic response_ready_i
);
  typedef enum logic [2:0] {IDLE, DC, LEAF, OUT, REJECT} state_t;
  state_t state_q;
  req_t req_q;
  dc_t dc_q;
  ddtp_t ddtp_q;
  resp_t resp_q;
  logic start, dc_ok, leaf_ok, msi_safe, msi_leaf_safe;
  logic probe_stable;
  logic is_write, is_read, canonical;
  localparam int unsigned PPNW = PAddrWidth - 12;

  assign start = state_q == IDLE && enable_i && window_i &&
                 candidate_valid_i && !invalidate_i;
  assign busy_o = state_q != IDLE && state_q != REJECT;
  assign intercept_o = start || busy_o;
  assign ddtc_lookup_o = state_q == DC || state_q == LEAF;
  assign ddtc_did_o = req_q.did;
  assign iotlb_lookup_o = state_q == LEAF;
  assign iotlb_vpn_o = req_q.iova[GPLEN39-1:12];
  assign iotlb_pscid_o = dc_q.ta.pscid;
  assign is_write = req_q.ttype == UNTRANSLATED_W;
  assign is_read = req_q.ttype == UNTRANSLATED_R;
  assign canonical = req_q.iova[63:39] == {25{req_q.iova[38]}};

  // The actual cached DC belongs to this request. No active-walker context
  // or saved inferred PSCID is used. All excluded cases fall through.
  if (HasMsiContext) begin : gen_msi_guard
    // Flat MSI can coexist with ordinary S1 copies. Exclude both the input
    // range and the cached S1 GPA range, using the normal TL/PTW match widths.
    // Reads are conservatively excluded too; MSI/reserved modes fall through.
    logic input_is_msi, leaf_is_msi;
    assign input_is_msi =
        (req_q.iova[VLEN39-1:12] & ~ddtc_dc_i.msi_addr_mask.mask[VPNW39-1:0]) ==
        (ddtc_dc_i.msi_addr_pattern.pattern[VPNW39-1:0] &
         ~ddtc_dc_i.msi_addr_mask.mask[VPNW39-1:0]);
    assign leaf_is_msi =
        (iotlb_content_i.content_1S.ppn[GPPNW39-1:0] &
         ~dc_q.msi_addr_mask.mask[GPPNW39-1:0]) ==
        (dc_q.msi_addr_pattern.pattern[GPPNW39-1:0] &
         ~dc_q.msi_addr_mask.mask[GPPNW39-1:0]);
    assign msi_safe = ddtc_dc_i.msiptp.mode == 0 ||
                     (ddtc_dc_i.msiptp.mode == 1 && !input_is_msi);
    assign msi_leaf_safe = dc_q.msiptp.mode == 0 ||
                          (dc_q.msiptp.mode == 1 && !leaf_is_msi);
  end else begin : gen_no_msi
    assign msi_safe = 1'b1;
    assign msi_leaf_safe = 1'b1;
  end
  assign dc_ok = msi_safe && ddtc_hit_i && ddtc_dc_i.tc.v &&
      !ddtc_dc_i.tc.pdtv && !ddtc_dc_i.tc.sxl &&
      !ddtc_dc_i.tc.t2gpa && !ddtc_dc_i.tc.en_ats &&
      !ddtc_dc_i.tc.en_pri && !req_q.pid_valid && !req_q.priv &&
      !req_q.is_debug && (is_write || is_read) && canonical &&
      ddtc_dc_i.fsc.mode == ModeSv39 &&
      ddtc_dc_i.iohgatp.mode == ModeBare &&
      ((ddtp_i.iommu_mode.q == 2 && req_q.did[23:6] == 0) ||
       (ddtp_i.iommu_mode.q == 3 && req_q.did[23:15] == 0) ||
       ddtp_i.iommu_mode.q == 4);
  assign leaf_ok = msi_leaf_safe && iotlb_hit_i && iotlb_content_i.content_1S.v &&
      iotlb_content_i.content_1S.u &&
      !iotlb_content_i.content_1S.is_1G &&
      !iotlb_content_i.content_1S.is_2M &&
      (is_write ? iotlb_content_i.content_1S.w :
                  iotlb_content_i.content_1S.r);
  assign accept_o = state_q == LEAF && enable_i &&
      candidate_valid_i && candidate_i == req_q &&
      !invalidate_i && ddtp_i == ddtp_q && dc_ok &&
      ddtc_dc_i == dc_q && leaf_ok;
  // Probes do not change replacement state; only an actually accepted hit
  // touches both real caches, exactly once.
  assign ddtc_touch_o = accept_o;
  assign iotlb_touch_o = accept_o;
  assign response_o = resp_q;
  assign response_valid_o = state_q == OUT;

  assign probe_stable = enable_i && candidate_valid_i && candidate_i == req_q &&
                        !invalidate_i && ddtp_i == ddtp_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      state_q <= IDLE;
      req_q <= '0;
      dc_q <= '0;
      ddtp_q <= '0;
      resp_q <= '0;
    end else begin
      case (state_q)
        IDLE: if (start) begin
          req_q <= candidate_i;
          ddtp_q <= ddtp_i;
          state_q <= DC;
        end
        DC: begin
          if (!enable_i || invalidate_i || ddtp_i != ddtp_q ||
              !dc_ok || !candidate_valid_i || candidate_i != req_q)
            state_q <= REJECT;
          else begin
            dc_q <= ddtc_dc_i;
            state_q <= LEAF;
          end
        end
        LEAF: begin
          if (accept_o) begin
            resp_q <= '0;
            resp_q.bank <= req_q.bank;
            resp_q.spaddr <= {iotlb_content_i.content_1S.ppn[PPNW-1:0],
                              req_q.iova[11:0]};
            resp_q.r <= iotlb_content_i.content_1S.r;
            resp_q.w <= iotlb_content_i.content_1S.w;
            resp_q.x <= iotlb_content_i.content_1S.x;
            resp_q.u <= iotlb_content_i.content_1S.u;
            resp_q.g <= iotlb_content_i.content_1S.g;
            state_q <= OUT;
          end else state_q <= REJECT;
        end
        OUT: if (response_ready_i) state_q <= IDLE;
        REJECT: if (normal_fire_i || !candidate_valid_i) state_q <= IDLE;
        default: state_q <= IDLE;
      endcase
    end
  end
endmodule

"""Codex §37: bounded original-parent credit experiment; out copies only."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = Path(__file__).resolve().parent

def replace_once(text, old, new):
    assert text.count(old) == 1, (old[:100], text.count(old))
    return text.replace(old, new, 1)

inner = (ROOT / 'tests/nonblock_actual_dma_top.sv').read_text()
inner = replace_once(inner, '    input  logic rst_ni,', '''    input  logic rst_ni,
    // §37: fixed before reset; total original AR accepted through parent RLAST.
    input logic [3:0] parent_window_i,
    output logic [7:0] parent_live_o, parent_highwater_o,''')
inner = replace_once(inner, '''    assign pool_m_rsp = {slv_resps[1], slv_resps[0]};
    assign slv_reqs[0] = pool_m_req[0];
    assign slv_reqs[1] = pool_m_req[1];''', '''    // Gate before arbitration so a flow without credit cannot hold the other.
    // One credit means one *original* source AR until its restored parent RLAST.
    // No data storage, and no changes to AW/W/B or to R backpressure.
    logic [1:0][3:0] parent_live_q, parent_highwater_q;
    logic [1:0] parent_accept, parent_retire;
    logic [1:0] ar_stalled_q;
    axi_tr2_ar_chan_t ar_stalled_chan_q [2]; // assertion observation only
    assign parent_live_o = parent_live_q;
    assign parent_highwater_o = parent_highwater_q;
    always_comb begin
        pool_m_rsp = {slv_resps[1], slv_resps[0]};
        for (int f = 0; f < 2; f++) begin
            slv_reqs[f] = pool_m_req[f];
            slv_reqs[f].ar_valid = pool_m_req[f].ar_valid &&
                                  parent_live_q[f] < parent_window_i;
            pool_m_rsp[f].ar_ready = slv_resps[f].ar_ready &&
                                    parent_live_q[f] < parent_window_i;
            parent_accept[f] = pool_m_req[f].ar_valid && pool_m_rsp[f].ar_ready;
            parent_retire[f] = pool_m_rsp[f].r_valid && pool_m_req[f].r_ready &&
                               pool_m_rsp[f].r.last;
        end
    end
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            parent_live_q <= 0; parent_highwater_q <= 0; ar_stalled_q <= 0;
            for (int f = 0; f < 2; f++) ar_stalled_chan_q[f] <= '0;
        end else begin
            if (!(parent_window_i == 1 || parent_window_i == 2 ||
                  parent_window_i == 4 || parent_window_i == 8))
                $fatal(1, "unsupported original-parent window");
            for (int f = 0; f < 2; f++) begin
                if (parent_live_q[f] > parent_window_i ||
                    (parent_retire[f] && parent_live_q[f] == 0))
                    $fatal(1, "original-parent credit overflow/underflow");
                if (ar_stalled_q[f] &&
                    (!slv_reqs[f].ar_valid || slv_reqs[f].ar != ar_stalled_chan_q[f]))
                    $fatal(1, "original-parent gate changed stalled AR");
                ar_stalled_q[f] <= slv_reqs[f].ar_valid && !slv_resps[f].ar_ready;
                ar_stalled_chan_q[f] <= slv_reqs[f].ar;
                case ({parent_accept[f], parent_retire[f]})
                    2'b10: begin
                        parent_live_q[f] <= parent_live_q[f] + 1;
                        if (parent_live_q[f] + 1 > parent_highwater_q[f])
                            parent_highwater_q[f] <= parent_live_q[f] + 1;
                    end
                    2'b01: parent_live_q[f] <= parent_live_q[f] - 1;
                    default: ;
                endcase
            end
        end
    end''')
(OUT / 'window_actual_dma_top.sv').write_text(inner)

outer = (ROOT / 'out/parent-capacity-20261008/system_top.sv').read_text()
outer = replace_once(outer, '    input  logic        rst_ni,', '''    input  logic        rst_ni,
    input logic [3:0] parent_window_i,
    output logic [7:0] parent_live_o, parent_highwater_o,''')
outer = replace_once(outer, '''    ) i_dma (
        .clk_i,
        .rst_ni,''', '''    ) i_dma (
        .clk_i,
        .rst_ni,
        .parent_window_i, .parent_live_o, .parent_highwater_o,''')
(OUT / 'window_system_top.sv').write_text(outer)

cpp = (ROOT / 'out/fragment-placement-20261008/placement_test.cpp').read_text()
cpp = replace_once(cpp, 'class Tb {', 'static unsigned task_parent_window = 8;\n\nclass Tb {')
cpp = replace_once(cpp, '    Tb() : top(new TopT) {}',
                   '    Tb() : top(new TopT) { top->parent_window_i = task_parent_window; }')
cpp = replace_once(cpp, '        nat_report_placement_json();', '''        nat_report_placement_json();
        CHECK(top->parent_window_i == task_parent_window, "parent window input mismatch");
        CHECK(top->parent_live_o == 0, "parent window did not drain");
        const unsigned h0 = top->parent_highwater_o & 15;
        const unsigned h1 = (top->parent_highwater_o >> 4) & 15;
        CHECK(h0 > 0 && h0 <= task_parent_window && h1 > 0 && h1 <= task_parent_window,
              "parent window highwater outside configured bound");
        std::printf("WINDOW-JSON {\\"schema\\":\\"original-parent-window-v1\\","
                    "\\"configured\\":%u,\\"actual\\":%u,\\"live_v\\":%u,\\"live_a\\":%u,"
                    "\\"highwater_v\\":%u,\\"highwater_a\\":%u}\\n",
                    task_parent_window, (unsigned)top->parent_window_i,
                    (unsigned)(top->parent_live_o & 15),
                    (unsigned)((top->parent_live_o >> 4) & 15), h0, h1);''')
cpp = replace_once(cpp, '        if (const char* v = val("mode"))      mode = v;', '''        if (const char* v = val("parent-window")) {
            const std::string setting(v);
            if (setting != "1" && setting != "2" && setting != "4" && setting != "8") {
                std::fprintf(stderr, "invalid +parent-window (expected 1|2|4|8)\\n");
                return 2;
            }
            task_parent_window = (unsigned)std::strtoul(v, nullptr, 10);
        }
        if (const char* v = val("mode"))      mode = v;''')
(OUT / 'window_test.cpp').write_text(cpp)

argv = json.loads((ROOT / 'out/parent-capacity-20261008/build-depth1-command.json').read_text())
mapping = {
    str(ROOT / 'tests/nonblock_actual_dma_top.sv'): str(OUT / 'window_actual_dma_top.sv'),
    str(ROOT / 'out/parent-capacity-20261008/system_top.sv'): str(OUT / 'window_system_top.sv'),
    str(ROOT / 'out/posttl-independent-20261008/natural_fragment_test.cpp'): str(OUT / 'window_test.cpp'),
    str(ROOT / 'out/parent-capacity-20261008/obj-depth1'): str(OUT / 'obj-window'),
}
argv = [mapping.get(arg, arg) for arg in argv]
(OUT / 'build-command.json').write_text(json.dumps(argv, indent=2) + '\n')
production = json.loads((ROOT / 'out/parent-translation-reuse-20261008/production-after.json').read_text())
current = {p: hashlib.sha256((ROOT / p).read_bytes()).hexdigest() for p in production}
assert production == current
(OUT / 'production-before.json').write_text(json.dumps(current, indent=2) + '\n')

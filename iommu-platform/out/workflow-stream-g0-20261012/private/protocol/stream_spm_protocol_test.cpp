// Private bounded slave-protocol test, authored by Codex, 2026-10-12.
#include "Vstream_spm_protocol_top.h"
#include "verilated.h"
#include <cstdint>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

static constexpr uint64_t Base = 0x100000000ULL;
static constexpr uint64_t Bytes = 98304;
static constexpr unsigned HoldCycles = 17;
using BPayload = std::tuple<unsigned, unsigned, unsigned>;
using RPayload = std::tuple<unsigned, uint64_t, unsigned, unsigned, unsigned>;

struct Sample {
    bool aw, w, ar, bv, br, rv, rr, busy;
    BPayload b;
    RPayload r;
};
struct Grant {
    uint64_t addr, data;
    bool we, error;
};
struct Token {
    bool valid = false, we = false, error = false;
    uint64_t addr = 0, data = 0;
};

class Bench {
public:
    Vstream_spm_protocol_top top;
    std::ofstream trace;
    uint64_t cycle = 0, aw = 0, w = 0, b = 0, ar = 0, r = 0;
    uint64_t grants = 0, responses = 0, errors = 0, error_b = 0, error_r = 0;
    uint64_t b_stalls = 0, r_stalls = 0;
    std::map<uint64_t, uint64_t> shadow;
    std::deque<Grant> planned;
    Token pending;
    bool b_held = false, r_held = false;
    BPayload held_b;
    RPayload held_r;

    explicit Bench(const std::string& trace_path) : trace(trace_path) {
        if (!trace) throw std::runtime_error("cannot open protocol trace");
        trace << "cycle,awfire,wfire,arfire,bvalid,bready,bid,bresp,buser,"
                 "rvalid,rready,rid,rdata,rresp,rlast,ruser,grant,owner,we,addr,"
                 "wdata,strb,response,rspowner,rspwe,rspaddr,rspdata,rsperr,busy\n";
        top.clk_i = 0; top.rst_ni = 0;
        top.aw_valid_i = top.w_valid_i = top.ar_valid_i = 0;
        top.b_ready_i = top.r_ready_i = 0;
        top.aw_id_i = top.aw_len_i = top.ar_id_i = top.ar_len_i = 0;
        top.aw_addr_i = top.ar_addr_i = top.w_data_i = 0;
        top.w_strb_i = top.w_last_i = 0;
        for (unsigned i = 0; i < 4; ++i) tick();
        top.rst_ni = 1;
        tick(); tick();
    }
    ~Bench() { top.final(); }

    void require(bool ok, const std::string& what) const {
        if (!ok) throw std::runtime_error("cycle " + std::to_string(cycle) + ": " + what);
    }
    Sample tick() {
        top.clk_i = 0;
        top.eval();
        Sample s{
            bool(top.aw_valid_i && top.aw_ready_o),
            bool(top.w_valid_i && top.w_ready_o),
            bool(top.ar_valid_i && top.ar_ready_o),
            bool(top.b_valid_o), bool(top.b_ready_i),
            bool(top.r_valid_o), bool(top.r_ready_i), bool(top.spm_busy_o),
            BPayload{top.b_id_o, top.b_resp_o, top.b_user_o},
            RPayload{top.r_id_o, top.r_data_o, top.r_resp_o, top.r_last_o, top.r_user_o}
        };
        // Sample before the edge, like the accepted common-clock harness.
        if (top.rst_ni) {
            require(!top.comp_busy_o && !top.comp_done_o && !top.mac_valid_o &&
                    top.comp_fault_o == 0 && top.comp_cmd_ready_o,
                    "disabled compute must remain idle, ready and fault-free");
            if (b_held) require(s.bv && s.b == held_b, "B VALID/payload withdrawn under stall");
            if (r_held) require(s.rv && s.r == held_r, "R VALID/payload withdrawn under stall");
            b_held = s.bv && !s.br;
            r_held = s.rv && !s.rr;
            held_b = s.b; held_r = s.r;
            if (b_held || r_held) require(s.busy, "busy dropped with parked response");
            b_stalls += b_held; r_stalls += r_held;
            aw += s.aw; w += s.w; ar += s.ar;
            b += s.bv && s.br; r += s.rv && s.rr;
            error_b += s.bv && s.br && std::get<1>(s.b) != 0;
            error_r += s.rv && s.rr && std::get<2>(s.r) != 0;

            // Every actual memory grant produces exactly one next-edge response,
            // including writes and errors. The independent shadow is updated
            // only on legal write grants, never through a backdoor DUT access.
            require(bool(top.mem_rsp_valid_o) == pending.valid, "response latency/count mismatch");
            if (pending.valid) {
                require(!top.mem_rsp_owner_o && bool(top.mem_rsp_we_o) == pending.we &&
                        bool(top.mem_rsp_err_o) == pending.error &&
                        top.mem_rsp_addr_o == pending.addr && top.mem_rsp_rdata_o == pending.data,
                        "memory response owner/kind/address/data/error mismatch");
                ++responses;
            }
            Token next;
            if (top.mem_grant_valid_o) {
                require(!top.mem_grant_owner_o, "unexpected compute memory grant");
                require(!planned.empty(), "unexpected extra memory grant");
                const Grant g = planned.front(); planned.pop_front();
                require(top.mem_grant_addr_o == g.addr && bool(top.mem_grant_we_o) == g.we,
                        "grant differs from requested AXI beat address/kind");
                if (g.we) require(top.mem_grant_wdata_o == g.data && top.mem_grant_strb_o == 0xff,
                                  "write grant data/strobes differ from submitted beat");
                next.valid = true; next.we = g.we; next.error = g.error; next.addr = g.addr;
                next.data = (g.we || g.error) ? 0 : shadow[g.addr];
                if (g.we && !g.error) shadow[g.addr] = g.data;
                ++grants; errors += g.error;
            }
            pending = next;
        }
        trace << cycle << ',' << s.aw << ',' << s.w << ',' << s.ar << ','
              << s.bv << ',' << s.br << ',' << std::get<0>(s.b) << ','
              << std::get<1>(s.b) << ',' << std::get<2>(s.b) << ','
              << s.rv << ',' << s.rr << ',' << std::get<0>(s.r) << ','
              << std::get<1>(s.r) << ',' << std::get<2>(s.r) << ','
              << std::get<3>(s.r) << ',' << std::get<4>(s.r) << ','
              << unsigned(top.mem_grant_valid_o) << ',' << unsigned(top.mem_grant_owner_o) << ','
              << unsigned(top.mem_grant_we_o) << ',' << top.mem_grant_addr_o << ','
              << top.mem_grant_wdata_o << ',' << unsigned(top.mem_grant_strb_o) << ','
              << unsigned(top.mem_rsp_valid_o) << ',' << unsigned(top.mem_rsp_owner_o) << ','
              << unsigned(top.mem_rsp_we_o) << ',' << top.mem_rsp_addr_o << ','
              << top.mem_rsp_rdata_o << ',' << unsigned(top.mem_rsp_err_o) << ',' << s.busy << '\n';
        top.clk_i = 1; top.eval();
        top.clk_i = 0; top.eval();
        ++cycle;
        return s;
    }
    static bool outside(uint64_t addr) { return addr < Base || addr > Base + Bytes - 8; }
    void quiet() {
        for (unsigned i = 0; i < 128; ++i) {
            Sample s = tick();
            if (!s.busy) {
                require(!s.bv && !s.rv && !pending.valid && planned.empty(),
                        "busy cleared before memory/AXI queues drained");
                return;
            }
        }
        require(false, "endpoint failed to drain");
    }
    void write(uint64_t addr, unsigned id, const std::vector<uint64_t>& data) {
        require(data.size() >= 1 && data.size() <= 2, "fixture write length");
        for (unsigned i = 0; i < data.size(); ++i)
            planned.push_back({addr + i * 8, data[i], true, outside(addr + i * 8)});
        top.aw_valid_i = 1; top.aw_addr_i = addr; top.aw_id_i = id;
        top.aw_len_i = data.size() - 1; top.b_ready_i = 0;
        unsigned sent = 0;
        bool aw_sent = false;
        for (unsigned timeout = 0; timeout < 128 && (!aw_sent || sent != data.size()); ++timeout) {
            top.w_valid_i = sent != data.size();
            if (sent != data.size()) {
                top.w_data_i = data[sent]; top.w_strb_i = 0xff;
                top.w_last_i = sent + 1 == data.size();
            }
            Sample s = tick();
            if (s.aw) { aw_sent = true; top.aw_valid_i = 0; }
            if (s.w) ++sent;
        }
        require(aw_sent && sent == data.size(), "write request timeout");
        top.w_valid_i = 0;
        Sample first{};
        bool found = false;
        for (unsigned i = 0; i < 128; ++i) {
            first = tick();
            if (first.bv) { found = true; break; }
        }
        require(found, "B response timeout");
        const unsigned resp = outside(addr) ? 2 : 0; // Bridge maps memory error to SLVERR.
        const BPayload expected{id, resp, 0};
        require(first.b == expected, "B ID/response/user mismatch");
        for (unsigned i = 0; i < HoldCycles; ++i) {
            Sample s = tick();
            require(s.bv && !s.br && s.b == expected, "B not held for 17 cycles");
        }
        top.b_ready_i = 1;
        Sample accepted = tick();
        require(accepted.bv && accepted.br && accepted.b == expected, "B handshake mismatch");
        top.b_ready_i = 0;
        quiet();
    }
    void read(uint64_t addr, unsigned id, const std::vector<uint64_t>& expected_data) {
        require(expected_data.size() >= 1 && expected_data.size() <= 2, "fixture read length");
        for (unsigned i = 0; i < expected_data.size(); ++i)
            planned.push_back({addr + i * 8, 0, false, outside(addr + i * 8)});
        top.ar_valid_i = 1; top.ar_addr_i = addr; top.ar_id_i = id;
        top.ar_len_i = expected_data.size() - 1; top.r_ready_i = 0;
        bool ar_sent = false;
        for (unsigned i = 0; i < 128; ++i) {
            Sample s = tick();
            if (s.ar) { ar_sent = true; top.ar_valid_i = 0; break; }
        }
        require(ar_sent, "read address timeout");
        Sample first{};
        bool found = false;
        for (unsigned i = 0; i < 128; ++i) {
            first = tick();
            if (first.rv) { found = true; break; }
        }
        require(found, "R response timeout");
        const RPayload expected_first{id, expected_data[0], outside(addr) ? 2U : 0U,
                                      unsigned(expected_data.size() == 1), 0};
        require(first.r == expected_first, "first R payload mismatch");
        for (unsigned i = 0; i < HoldCycles; ++i) {
            Sample s = tick();
            require(s.rv && !s.rr && s.r == expected_first, "R not held for 17 cycles");
        }
        top.r_ready_i = 1;
        unsigned received = 0;
        for (unsigned timeout = 0; timeout < 128 && received != expected_data.size(); ++timeout) {
            Sample s = tick();
            if (s.rv && s.rr) {
                const RPayload expected{id, expected_data[received],
                                        outside(addr + received * 8) ? 2U : 0U,
                                        unsigned(received + 1 == expected_data.size()), 0};
                require(s.r == expected, "R ID/data/response/last/user mismatch");
                ++received;
            }
        }
        require(received == expected_data.size(), "read data timeout");
        top.r_ready_i = 0;
        quiet();
    }
    void run() {
        const std::vector<uint64_t> pair{0x0123456789abcdefULL, 0xfedcba9876543210ULL};
        const uint64_t last = Base + Bytes - 8, guard = 0xd15ea5e5a55a1234ULL;
        write(Base, 9, pair);
        read(Base, 42, pair);
        write(last, 37, {guard});
        read(last, 17, {guard});
        write(Base + Bytes, 55, {0xbaadf00ddeadbeefULL});
        read(Base + Bytes, 61, {0});
        read(last, 23, {guard});
        read(Base, 31, pair);
        quiet();
        require(aw == 3 && w == 4 && b == 3 && ar == 5 && r == 7,
                "AXI transaction/beat conservation mismatch");
        require(grants == 11 && responses == 11 && errors == 2 && error_b == 1 && error_r == 1,
                "memory grant/response/error conservation mismatch");
        require(b_stalls >= HoldCycles && r_stalls >= HoldCycles,
                "requested response backpressure coverage absent");
        require(!top.spm_busy_o && !top.comp_busy_o && !pending.valid && planned.empty(),
                "final endpoint not empty");
    }
};

static std::string json_escape(const std::string& in) {
    std::string out;
    for (char c : in) {
        if (c == '"' || c == '\\') out += '\\';
        if (c == '\n') out += "\\n"; else out += c;
    }
    return out;
}
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    const std::string result = argc > 1 ? argv[1] :
        "/home/xvhaocheng/work/iommu-platform/out/workflow-stream-g0-20261012/private/protocol/result.json";
    const auto parent = std::filesystem::path(result).parent_path();
    if (!parent.empty()) std::filesystem::create_directories(parent);
    std::unique_ptr<Bench> bench;
    std::string error;
    try {
        bench = std::make_unique<Bench>(result + ".csv");
        bench->run();
    } catch (const std::exception& e) { error = e.what(); }
    std::ofstream out(result);
    if (!out) { std::cerr << "FAIL cannot open result\n"; return 1; }
    out << "{\n  \"schema\":\"stream-spm-protocol-result-v1\",\n  \"status\":\""
        << (error.empty() ? "PASS" : "FAIL") << "\",\n  \"error\":\"" << json_escape(error) << "\",\n"
        << "  \"trace\":\"" << json_escape(result + ".csv") << "\",\n"
        << "  \"hold_cycles_per_response\":" << HoldCycles;
    if (bench) {
        out << ",\n  \"cycles\":" << bench->cycle
            << ",\n  \"axi_aw\":" << bench->aw << ",\n  \"axi_w_beats\":" << bench->w
            << ",\n  \"axi_b\":" << bench->b << ",\n  \"axi_ar\":" << bench->ar
            << ",\n  \"axi_r_beats\":" << bench->r << ",\n  \"memory_grants\":" << bench->grants
            << ",\n  \"memory_responses\":" << bench->responses
            << ",\n  \"error_memory_requests\":" << bench->errors
            << ",\n  \"error_b\":" << bench->error_b << ",\n  \"error_r\":" << bench->error_r
            << ",\n  \"b_stalled_cycles\":" << bench->b_stalls
            << ",\n  \"r_stalled_cycles\":" << bench->r_stalls
            << ",\n  \"final_spm_busy\":" << unsigned(bench->top.spm_busy_o)
            << ",\n  \"final_comp_busy\":" << unsigned(bench->top.comp_busy_o);
        bench->trace.flush();
    }
    out << "\n}\n";
    std::cout << (error.empty() ? "PASS bounded SPM protocol" : "FAIL " + error) << '\n';
    return error.empty() ? 0 : 1;
}

# Findings — §34 请求头容量 / 源 AR↔parent 关联证据

以下全部来自你提供的仓库文件，只做接口/结构核证，不做 root-cause 或架构结论。行号对应对应文件。

## 1. 实际 iDMA `NumAxInFlight`

`tests/nonblock_actual_dma_top.sv`（非 `PostTL` 文件，是真实 DMA 实例化点）：

- L352-374 DMA0（V，`i_dma0`）：L358
  ```systemverilog
  .NumAxInFlight    ( 8               ),
  ```
- L411-433 DMA1（A，`i_dma1`）：L417
  ```systemverilog
  .NumAxInFlight    ( 8               ),
  ```
- 头注释 L17 记录 "JobFifoDepth 8, BufferDepth 512, NumAxInFlight 8, MemSysDepth 0"。

**结论证据**：两个真实 DMA 的 `NumAxInFlight` 均为硬编码 `8`，与顶层的 `NumOutstandingTrans` 参数（L44）不是同一根。`NumOutstandingTrans=8` 只转发给 `nonblock_banked_top`（L791）。

## 2. TC 容量拆半 / MaxTrans / MaxWTrans

`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv`：

- L162-164 每 bank 半拆：
  ```systemverilog
  NumOutstandingTrans : RVIOMMUCfg.NumOutstandingTrans / 2,
  WFifoDepth          : RVIOMMUCfg.WFifoDepth / 2,
  ```
- L291 输入 demux：`.MaxTrans (RVIOMMUCfg.NumOutstandingTrans * 2),`
- L434 输出 completion mux：`.MaxWTrans (RVIOMMUCfg.NumOutstandingTrans),`
- L149-154 参数校验：`NumOutstandingTrans` 偶数且 ≥2；`WFifoDepth` 偶数且 ≥2。

`tests/nonblock_actual_dma_top.sv`：

- L695-696 翻译请求 `i_dma_iommu_tr_mux`（真实 `axi_mux`，2 活口 + 2 空口）：
  ```systemverilog
  .NoSlvPorts    ( 4                  ),
  .MaxWTrans     ( 8                  )
  ```
- L233-235：`DmaAxiIdWidth=2`，`MuxAxiIdWidth=4`。因此 master AR/AW id = `{slv_port[1:0], dma_AxID[1:0]}`，`id[2]` 即 DMA/bank 选择位（fragment 用它做 owner）。

`rv_iommu_trans_ctl_wrap.sv`（per-bank TC）：

- L121-139 AW FIFO `DEPTH(RVIOMMUCfg.NumOutstandingTrans)`；L146-164 AR FIFO 同深度；L222-240 W FIFO `DEPTH(RVIOMMUCfg.WFifoDepth)`。
- L220：`w_ready = (w_cnt_q != '0) ? axi_comp_resp.w_ready : 1'b0;`（W 需已有 accepted AW）。
- L332/L346：完成侧 AW/AR 直穿原始 id：`axi_comp_req.aw.id = aw_data.id;` / `ar.id = ar_data.id;`
- L105：`in_flight_o = (state_q != IDLE);`，一次只推进一个 translation（L620-732 单一 FSM）。

**结论证据**：TC 侧并发上限来自 per-bank 半拆的 AW/AR FIFO 深度；`MaxWTrans` 是 completion/翻译请求两处 mux 各自参数。三者数值不同，不能互相代入。

## 3. Source R 返回次序

`out/posttl-independent-20261008/natural_fragment_test.cpp`：

- L239：`std::deque<MemRead> mem_q; // SINGLE ordered R-return queue`
- L687：`top->ddr_ar_ready = (mem_q.size() < 16) ? 1 : 0;`
- L711-731：队头服务 + 固定 latency：
  ```cpp
  MemRead& h = mem_q.front();
  ...
  top->ddr_r_valid = 1;
  top->ddr_r_id    = (uint8_t)h.id;   // 6-bit shared-DDR ID echoed verbatim
  ```
- L700-710 的 `ds_priority` 轮转**只在 `win_open` 时**发生（natural run 不打开该窗口）。

**结论证据**：natural 模式下 R 是单队列严格 head-of-line，ID 原样回显；不存在按 ID 重排的证据。

## 4. 源 AR → 当前 parent 的关联（可核到什么）

`out/posttl-independent-20261008/posttl_owner_fragment.sv`：

- L51-52 接受门槛：
  ```systemverilog
  s_ar_ready_o = active_q[s_ar_i.id[2]] &&
                 pa_q[s_ar_i.id[2]] - done_q[s_ar_i.id[2]] < D;
  ```
- L153-163 虚拟 AR 落库（按 `va_q` 索引）：
  ```systemverilog
  s = int'(v_ar_owner_i); n = int'(va_q[s][2:0]);
  ...
  virtual_offset_q[s][n] <= v_ar_addr_i-src_q[s];
  virtual_len_q[s][n]    <= v_ar_len_i;
  va_q[s] <= va_q[s]+1;
  ```
- L164-179 物理 AR 落库（按 `pa_q` 索引）+ 关联：
  ```systemverilog
  s = int'(s_ar_i.id[2]); n = int'(pa_q[s][2:0]); vn = int'(pa_q[s][2:0]);
  if (va_q[s] <= pa_q[s] || s_ar_i.id != (s == 0 ? 4'd0 : 4'd4) ||
      s_ar_i.len != virtual_len_q[s][vn] || ...)
      $fatal(1,"postTL unsupported AR or virtual/physical association");
  parent_q[s][n] <= s_ar_i;
  offset_q[s][n] <= virtual_offset_q[s][vn];
  returned_q[s][n] <= 0;
  pa_q[s] <= pa_q[s]+1;
  ```

**可核证据只有两处**：`id[2]` owner 位（L168）与长度等值 `virtual_len_q[s][vn]`（L169）。VA→PA 不可能做地址等值（L157 只做 IOVA 窗口界检查）。所以“第 k 个物理 AR 对应第 k 个虚拟 AR”是一个 **FIFO 索引假设**，只由 owner 位 + 长度匹配守护，**没有 token/ID 随行**。

**这对 ParentDepth<8 是要害**：索引统一用 `[2:0]`（mod 8）：
- L27 `localparam int D = 8;`
- L62 `done_q[s][2:0]`；L101-102 `done_q[child_owner_q][2:0]`
- L70-71 `(vr_q + 32'(j)) & 7` / `(pr_q + 32'(j)) & 7`（硬编码 &7）
- L155 `va_q[s][2:0]`；L166-167 `pa_q[s][2:0]`（两次）
- L183 `vw_q[2:0]`；L196 `pw_q[2:0]`；L203 `v_beats_q[vr_q[2:0]]`
- L217-219 `done_q[candidate_owner][2:0]`

`D` 同时被用于：parent 队列（L52/134/156）、虚拟 AR 关联（L155）、两侧 AW 环（L130/184/197）、capacity 扫描（L68）。要“虚拟 AR 关联与两侧 AW 环仍 Depth8、物理 parent 队列独立 ParentDepth”，必须把 parent 队列相关的 `[2:0]`（`pa_q/done_q` 索引、L166-167 的 `n`/`vn`、L217-219）与 AR 关联环（`va_q/virtual_*`）的索引**分别 modulo 到各自的深度**。这正是计划中的 POSTTL-STATE（当前物理输入 ARid / W 前缀 owner / parent 队列 pa/done）才能给出独立证据的地方——我在这里只标出证据缺口，不给判断。

## 5. 上层观测映射（脚本会用到）

`out/posttl-independent-20261008/system_top.sv`：

- L1274-1304 fragment 实例化；L1291-1292 虚拟 AR 取 `i_dma.tr_ar_*`；L1300 物理 AW 取 `u_comp_aw_*`；L1298 `v_w_fire_i = i_dma.tr_w_valid && i_dma.tr_w_ready`。
- L1428-1429：`dbg_frontier_grant = fragment_parents`；`dbg_frontier_wait = fragment_children`。
- L1004（test.cpp）打印 `POSTTL-STATS parents=%u children=%u policy=%u`，即上面的 `parent_count_o/child_count_o`。

`natural_fragment_test.cpp`：

- L858-861 natural run 头；L967-983 虚拟 AW/WLAST 记账（L969-980 明确把 W-before-AW 归为 **observer limitation，不是 DUT 判决**）；L949-959 每 DMA 物理几何；L1005 `nat_report_json`；L1010-1056 `NATURAL-JSON`；L985-1002 无 ordering/stall 声明。
- L1983-1993 `dump_stall` 里的 `NATURAL-QUEUE/RQ/WQ` 只在 watchdog 路径打印。

---

# Python 汇总脚本草案（proposal，不执行）

放到 `out/posttl-independent-20261008/` 目录。只读 `depth{1,2,4,8}-{v,a}-32768-{command.json,result.json,.log,.csv}`，只写 `summary.json` 和 `summary.md`。

```python
#!/usr/bin/env python3
"""Compact acceptance summariser for the depth-sweep natural runs.

Reads, in this directory:
    depth{1,2,4,8}-{v,a}-32768-command.json
    depth{1,2,4,8}-{v,a}-32768-result.json
    depth{1,2,4,8}-{v,a}-32768.log
    depth{1,2,4,8}-{v,a}-32768.csv
Writes summary.json + summary.md.  Does not execute the DUT, does not edit
source, does not rewrite any prior report.

Honest accounting: a run is PASS only if EVERY check below is green.  A
watchdog / SCENARIO-FAIL / non-zero exit is reported as NON-PASS, not as a
success.  Raw cause lines are copied verbatim and are never attributed.
"""
from __future__ import annotations

import csv
import json
import re
from collections import defaultdict
from pathlib import Path

HERE = Path(__file__).resolve().parent
DEPTHS = (1, 2, 4, 8)
SIDES = ("v", "a")

SOURCE_IOVA = {"v": 0x10000000, "a": 0x20000000}
SOURCE_PA = {"v": 0x80000000, "a": 0x90000000}
DEST_PA = {"v": 0x81000c00, "a": 0x90100c00}

EXPECT = {
    "var": 32, "vaw": 34,
    "r_beats": 4096, "w_beats": 4096,
    "parents": 32, "children": 64, "policy": 0,
    "parent_bytes": 2048, "parent_beats": 256, "child_beats": 128,
    "src_bytes": 32768,
}


# --------------------------------------------------------------------------- #
# loaders
# --------------------------------------------------------------------------- #
def load_json(p: Path):
    try:
        return json.loads(p.read_text())
    except FileNotFoundError:
        return {"__missing__": str(p)}
    except Exception as e:  # noqa: BLE001
        return {"__parse_error__": f"{p}: {e}"}


def load_text(p: Path) -> str:
    try:
        return p.read_text(errors="replace")
    except FileNotFoundError:
        return ""


# --------------------------------------------------------------------------- #
# parsers
# --------------------------------------------------------------------------- #
def parse_command(cmd):
    opts = {}
    for tok in cmd:
        if isinstance(tok, str) and tok.startswith("+"):
            k, _, v = tok[1:].partition("=")
            opts[k] = v
    return opts


_NJ = re.compile(r"NATURAL-JSON (\{.*\})")
_STATS = re.compile(r"POSTTL-STATS parents=(\d+) children=(\d+) policy=(\d+)")
_CHILD = re.compile(
    r"POSTTL-CHILD cycle=(\d+) owner=(\d+) parent=(\d+) "
    r"offset=(\d+) beats=(\d+) policy=(\d+)")


def scan_log(log: str):
    nj = None
    for m in _NJ.finditer(log):
        try:
            nj = json.loads(m.group(1))
        except json.JSONDecodeError:
            pass
    ms = list(_STATS.finditer(log))
    raw_pats = {
        "natural_queue": r"NATURAL-QUEUE .*",
        "natural_rq": r"NATURAL-RQ .*",
        "natural_wq": r"NATURAL-WQ .*",
        "stall": r".*STALL at .*",
        "scenario_fail": r".*SCENARIO-FAIL .*",
        "order_state": r"ORDER-STATE .*",
        "posttl_state": r"POSTTL-STATE .*",   # future SV line, may be absent
    }
    raw = {k: re.findall(p, log) for k, p in raw_pats.items()}
    return {
        "pass_marker": "nonblock_actual_dma_test: PASS" in log,
        "natural_json": nj,
        "posttl_stats": ({"parents": int(ms[-1].group(1)),
                          "children": int(ms[-1].group(2)),
                          "policy": int(ms[-1].group(3))} if ms else None),
        "posttl_children": [
            {"cycle": int(a), "owner": int(b), "parent": int(c),
             "offset": int(d), "beats": int(e), "policy": int(f)}
            for a, b, c, d, e, f in _CHILD.findall(log)],
        "raw": {k: v for k, v in raw.items() if v},
    }


def scan_csv(p: Path):
    if not p.exists():
        return {"present": False}
    rows = []
    with p.open(newline="") as f:
        for r in csv.DictReader(f):
            rows.append(r)

    var, vaw = [], []
    per_region = defaultdict(lambda: {"par": 0, "pw": 0, "paw": 0,
                                      "rfirst": 0, "rlast": 0})
    par_seq = defaultdict(list)
    first_par, first_pw, last_pw = {}, {}, {}
    for r in rows:
        ev, reg = r.get("event", ""), r.get("region", "")
        try:
            addr = int(r["addr"], 0)
            length = int(r["len"])
            cyc = int(r["cycle"])
        except (KeyError, ValueError):
            continue
        if ev == "var":
            var.append((reg, addr, length, cyc))
        elif ev == "vaw":
            vaw.append((reg, addr, length, cyc))
        elif reg and ev in per_region[reg]:
            per_region[reg][ev] += 1
        if ev == "par":
            par_seq[reg].append((cyc, addr, length))
            first_par.setdefault(reg, addr)
        if ev in ("pw", "pwlast"):
            first_pw.setdefault(reg, addr)
            last_pw[reg] = cyc
    return {
        "present": True,
        "var": var, "vaw": vaw,
        "per_region": {k: dict(v) for k, v in per_region.items()},
        "par_seq": {k: sorted(v) for k, v in par_seq.items()},
        "first_par": first_par, "first_pw": first_pw, "last_pw": last_pw,
    }


# --------------------------------------------------------------------------- #
# checks
# --------------------------------------------------------------------------- #
def check_run(side, cmd_json, res_json, logd, csvd):
    checks = []

    def add(name, ok, detail=""):
        checks.append({"check": name, "ok": bool(ok), "detail": str(detail)})

    # ---- command ----
    cmd = cmd_json.get("command")
    if not isinstance(cmd, list):
        add("command list present", False, "no command array")
        opts = {}
    else:
        opts = parse_command(cmd)
        add("command list present", True, " ".join(cmd))
    add("+mode=s", opts.get("mode") == "s", opts.get("mode", ""))
    add("+natural=1", opts.get("natural") == "1", opts.get("natural", ""))
    add("+natural-bytes=32768",
        opts.get("natural-bytes") == "32768", opts.get("natural-bytes", ""))
    add("+natural-first matches side",
        opts.get("natural-first") == side, opts.get("natural-first", ""))
    add("+read-admit=none", opts.get("read-admit") == "none",
        opts.get("read-admit", ""))
    add("+ptw-latency present", "ptw-latency" in opts,
        opts.get("ptw-latency", ""))

    # ---- result.json ----
    add("result JSON parseable",
        "__parse_error__" not in res_json and "__missing__" not in res_json,
        res_json.get("__parse_error__", res_json.get("__missing__", "")))
    ec = res_json.get("exit_code")
    add("exit_code == 0", ec == 0, f"exit_code={ec}")

    # ---- log PASS / NATURAL-JSON ----
    add("log PASS marker", logd["pass_marker"], "")
    nj = logd["natural_json"]
    add("NATURAL-JSON present", nj is not None, "")
    if isinstance(nj, dict):
        add("schema natural-write-order-v1",
            nj.get("schema") == "natural-write-order-v1", nj.get("schema"))
        add("completed == true", nj.get("completed") is True, nj.get("completed"))
        add("verified == true", nj.get("verified") is True, nj.get("verified"))
        add(f"var == {EXPECT['var']}", nj.get("var") == EXPECT["var"], nj.get("var"))
        add(f"vaw == {EXPECT['vaw']}", nj.get("vaw") == EXPECT["vaw"], nj.get("vaw"))
        for k in ("v_src_r_beats", "a_src_r_beats"):
            add(f"{k} == {EXPECT['r_beats']}",
                nj.get(k) == EXPECT["r_beats"], nj.get(k))
        for k in ("v_dst_w_beats", "a_dst_w_beats"):
            add(f"{k} == {EXPECT['w_beats']}",
                nj.get(k) == EXPECT["w_beats"], nj.get(k))
    else:
        for n in ("schema natural-write-order-v1", "completed == true",
                  "verified == true", f"var == {EXPECT['var']}",
                  f"vaw == {EXPECT['vaw']}",
                  f"v_src_r_beats == {EXPECT['r_beats']}",
                  f"a_src_r_beats == {EXPECT['r_beats']}",
                  f"v_dst_w_beats == {EXPECT['w_beats']}",
                  f"a_dst_w_beats == {EXPECT['w_beats']}"):
            add(n, False, "no natural json")

    # ---- POSTTL-STATS ----
    st = logd["posttl_stats"]
    add("POSTTL-STATS present", st is not None, "")
    if st:
        add(f"parents == {EXPECT['parents']}",
            st["parents"] == EXPECT["parents"], st["parents"])
        add(f"children == {EXPECT['children']}",
            st["children"] == EXPECT["children"], st["children"])
        add(f"policy == {EXPECT['policy']}",
            st["policy"] == EXPECT["policy"], st["policy"])
    else:
        for n in (f"parents == {EXPECT['parents']}",
                  f"children == {EXPECT['children']}",
                  f"policy == {EXPECT['policy']}"):
            add(n, False, "")

    # ---- POSTTL-CHILD structure ----
    kids = logd["posttl_children"]
    add("POSTTL-CHILD present", bool(kids), f"{len(kids)} lines")
    if kids:
        bad = [k for k in kids
               if k["parent"] != k["offset"] // EXPECT["parent_bytes"]]
        add("POSTTL-CHILD parent == offset//2048", not bad, f"bad={bad[:4]}")
        sums = defaultdict(int)
        for k in kids:
            sums[(k["owner"], k["parent"])] += k["beats"]
        bad_sums = {k: v for k, v in sums.items()
                    if v != EXPECT["parent_beats"]}
        add(f"children sum == {EXPECT['parent_beats']} beats/parent",
            not bad_sums, f"bad={dict(list(bad_sums.items())[:4])}")
        beats = sorted({k["beats"] for k in kids})
        add("fixed policy child beats == 128",
            beats == [EXPECT["child_beats"]], beats)
    else:
        for n in ("POSTTL-CHILD parent == offset//2048",
                  f"children sum == {EXPECT['parent_beats']} beats/parent",
                  "fixed policy child beats == 128"):
            add(n, False, "")

    # ---- CSV ----
    if not csvd.get("present"):
        add("csv present", False, "missing")
        return checks
    add("csv present", True, "")

    vsrc_v = [a for (reg, a, _l, _c) in csvd["var"] if reg == "vsrc"]
    asrc_v = [a for (reg, a, _l, _c) in csvd["var"] if reg == "asrc"]
    add("vsrc var first == 0x10000000",
        bool(vsrc_v) and min(vsrc_v) == SOURCE_IOVA["v"],
        hex(min(vsrc_v)) if vsrc_v else "")
    add("asrc var first == 0x20000000",
        bool(asrc_v) and min(asrc_v) == SOURCE_IOVA["a"],
        hex(min(asrc_v)) if asrc_v else "")

    add("vsrc par first == 0x80000000",
        csvd["first_par"].get("vsrc") == SOURCE_PA["v"],
        hex(csvd["first_par"].get("vsrc", 0)))
    add("asrc par first == 0x90000000",
        csvd["first_par"].get("asrc") == SOURCE_PA["a"],
        hex(csvd["first_par"].get("asrc", 0)))

    for src, base in (("vsrc", SOURCE_PA["v"]), ("asrc", SOURCE_PA["a"])):
        cur, covered, contiguous = base, 0, True
        for _cyc, addr, length in csvd["par_seq"].get(src, []):
            if addr != cur:
                contiguous = False
                break
            nbytes = (length + 1) * 8
            covered += nbytes
            cur += nbytes
        add(f"{src}: par contiguous cover of 32K",
            contiguous and covered == EXPECT["src_bytes"],
            f"covered={covered} contiguous={contiguous}")

    add("vdst first pw == 0x81000c00",
        csvd["first_pw"].get("vdst") == DEST_PA["v"],
        hex(csvd["first_pw"].get("vdst", 0)))
    add("adst first pw == 0x90100c00",
        csvd["first_pw"].get("adst") == DEST_PA["a"],
        hex(csvd["first_pw"].get("adst", 0)))
    for reg in ("vdst", "adst"):
        got = csvd["per_region"].get(reg, {}).get("pw", 0)
        add(f"{reg}: pw beats == {EXPECT['w_beats']}",
            got == EXPECT["w_beats"], got)

    add("last W cycle recorded", any(csvd["last_pw"].values()),
        json.dumps(csvd["last_pw"]))

    return checks


# --------------------------------------------------------------------------- #
# driver
# --------------------------------------------------------------------------- #
def run_all():
    runs = {}
    for depth in DEPTHS:
        for side in SIDES:
            name = f"depth{depth}-{side}-32768"
            cmd = load_json(HERE / f"{name}-command.json")
            res = load_json(HERE / f"{name}-result.json")
            logd = scan_log(load_text(HERE / f"{name}.log"))
            csvd = scan_csv(HERE / f"{name}.csv")
            checks = check_run(side, cmd, res, logd, csvd)
            runs[name] = {
                "depth": depth, "side": side,
                "pass": all(c["ok"] for c in checks),
                "n_checks": len(checks),
                "n_ok": sum(1 for c in checks if c["ok"]),
                "exit_code": res.get("exit_code"),
                "posttl_stats": logd["posttl_stats"],
                "checks": checks,
                "raw_cause_lines": logd["raw"],
            }
    return runs


def write_md(runs: dict, path: Path):
    lines = [
        "# depth-sweep natural acceptance",
        "",
        "Honest accounting: a run is PASS only when every check is green;",
        "watchdog / SCENARIO-FAIL / non-zero exit are reported as NON-PASS.",
        "",
        "| run | verdict | exit | parents | children | policy | failed checks |",
        "|---|---|---|---|---|---|---|",
    ]
    for name, r in runs.items():
        st = r["posttl_stats"] or {}
        fails = [c["check"] for c in r["checks"] if not c["ok"]]
        lines.append(
            f"| {name} | {'PASS' if r['pass'] else 'NON-PASS'} | "
            f"{r['exit_code']} | {st.get('parents', '-')} | "
            f"{st.get('children', '-')} | {st.get('policy', '-')} | "
            f"{'; '.join(fails) if fails else '-'} |")
    lines += [
        "",
        "## notes",
        "- Raw cause lines (NATURAL-QUEUE/RQ/WQ, STALL, ORDER-STATE, "
        "POSTTL-STATE) are copied verbatim from the logs into summary.json; "
        "no head-wait attribution is made.",
        "- Destination byte completeness is the C++ acceptance's "
        "responsibility (natural_fragment_test.cpp nat_verify_and_report); "
        "this script only checks the observable addresses / beat counts.",
        "- Per-flow queue shrinkage is NOT evidence of total state minimality "
        "or area; 8->2 is not by itself an innovation claim.",
    ]
    path.write_text("\n".join(lines) + "\n")


def main():
    runs = run_all()
    summary = {
        "schema": "posttl-depth-sweep-summary-v1",
        "producer": __file__,
        "directory": str(HERE),
        "runs": runs,
        "summary": {
            "total": len(runs),
            "passed": sum(1 for r in runs.values() if r["pass"]),
            "non_pass": sum(1 for r in runs.values() if not r["pass"]),
        },
        "not_evaluated": [
            "destination bytes (C++ nat_verify_and_report)",
            "AXI AR size/burst/lock/cache attributes (only SV checks them)",
            "DUT-internal parent-owner selection / AW order",
            "absolute board timing / area",
        ],
    }
    (HERE / "summary.json").write_text(json.dumps(summary, indent=2) + "\n")
    write_md(runs, HERE / "summary.md")
    print(json.dumps(summary["summary"]))


if __name__ == "__main__":
    main()
```

---

# Assumptions / unresolved / acceptance checks

**Assumptions**
- `result.json` 含顶层键 `exit_code`（与 `analysis.json` 一致）。
- 文件名严格为 `depth{N}-{v|a}-32768-{command.json,result.json,.log,.csv}`；若实际是 `-command.json` 与 `-result.json` 之外的拼法，需要改 `run_all()` 的两个路径模板。
- natural run 使用 `read-admit=none` → `adaptive_i=0` → child 上限 128 beats、`children==64`。若某 depth 运行改用 `read-admit=buffer`，`children`/beats 断言会变，脚本会如实报 NON-PASS 而不是失败。
- `POSTTL-CHILD` 行存在时其 `beats` 按“该 parent 下所有 child beats 之和 = 256”解释（对应 `analysis.json` 的 `children sum == 256 beats per parent`）。

**Unresolved / needs Codex**
- ParentDepth<8 时 `pa_q mod ParentDepth` 与 `va_q mod 8` 的索引一致性：RTL 侧如何把 L166-167 的 `n`/`vn` 拆分成不同 modulo，以及 L70-71 硬编码 `& 7` 如何随深度变更，属 Codex 的 RTL 修改决定。脚本只读输出。
- 未做 “从源 AR 到当前 parent” 的独立证明；只有 owner+length 两处守护。POSTTL-STATE（当前物理输入 ARid、W 前缀 owner、parent 队列 pa/done）是让这一点可独立核查的后续证据，脚本已把 `POSTTL-STATE` 原样保留在 `raw_cause_lines`。
- 单槽是否必死/双槽是否必通 **未假定**；脚本对任何非 PASS 只报 NON-PASS + 原始 watchdog 行。

**Acceptance checks（脚本自身）**
1. 每个 run 都有 `command/result/log/csv` 四件套；缺失 → 仅相关 check 红，不整体崩。
2. PASS 的 run 必满足：command `+mode=s,+natural=1,+natural-bytes=32768,+read-admit=none`、`exit_code==0`、log 有 PASS、`NATURAL-JSON.completed/verified==true`、`var==32 & vaw==34`、四个 R/W beats `==4096`、`POSTTL-STATS parents==32/children==64/policy==0`、POSTTL-CHILD `parent==offset//2048` 且每 parent child beats 和 256、CSV 源首地址 `0x80000000/0x90000000`、源 32K 连续覆盖、目标首地址 `0x81000c00/0x90100c00` 且各 4096 W beats、last W cycle 有记录。
3. `summary.md` 只列出失败 check 名，不产出任何 ordering/steady-state/area 结论。
4. 未授权执行：脚本仅 `Path.write_text` 写 `summary.json` / `summary.md`；不触碰源文件、不执行 DUT、不重写既有报告。

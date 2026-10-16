# 证据勘误与补充：Fixture A leaf 映射、nonblock_hit fast 路径来源（DeepSeek 证据整理）

范围与保留声明
- 本 worker 未执行命令、未写文件、未运行任何模型/DUT/测试；不声称任何测试通过。
- 第一 worker 原文 `out/translation-sharing-progress-20261008/deepseek-source-evidence.md` 整体保留；本勘误只取代下列明确点名的行/结论，其余行号与段落不变。
- 本文是提案，是否回写由 Codex root 核对后决定。

## E1（勘误，取代原文第 205 行“IOVA -> PA”行）

原文（保留，原样引用）：
> 205: | IOVA -> PA | V0 0x10000000 -> 0x80000000；V1 0x10001000 -> 0x80001000；Vcold 0x10002000 -> 0x80002000，`tests/composition_tl_test.cpp:358-361` | A page n: 0x20000000+n*0x1000 -> 0x90000000+n*0x1000，n=0..15，`tests/composition_tl_test.cpp:366-371` |

错误定位：该行把 Fixture A 索引 2 的 leaf 当成 Vcold。实际 `tests/composition_tl_test.cpp:360` 的 `0x50010` 是 **Vro（只读）**，`:361` 的 `0x50018` 才是 **Vcold**。

按测试文件写入值与 leaf 索引复核（PPN = value>>10；索引 = (leaf-0x50000)/8；IOVA = 0x10000000 + 索引*0x1000，索引/IOVA 为按 Sv39 推出的标注，非测试内显式常量）：

| leaf 地址 | 行 | 写入值 | 解出 PPN / 权限 | VPN0 索引 | 对应 IOVA | 语义 |
|---|---|---|---|---|---|---|
| 0x50000 | :358 | 0x00000000200000DF | 0x80000；R/W/X/U/A/D=1 | 0 | 0x10000000 | V0 → SPA 0x80000000 |
| 0x50008 | :359 | 0x00000000200004DF | 0x80001；全权限 | 1 | 0x10001000 | V1 → SPA 0x80001000 |
| 0x50010 | :360 | 0x00000000200000D3 | 0x80000；**r=1,w=0,x=0** | 2 | 0x10002000 | **Vro（只读）** → SPA 0x80000000 |
| 0x50018 | :361 | 0x00000000200008DF | 0x80002；全权限 | 3 | 0x10003000 | **Vcold** → SPA 0x80002000 |

注：0x50000 与 0x50010 解出同一 PPN（0x80000→SPA 0x80000000）但权限不同（0xDF 全权限 vs 0xD3 只读），因此“同 SPA”不能当作同 verdict；只读 leaf 能过读检查、不能过写检查。

修正后行（提案，供 root 采用）：
| IOVA -> PA | V0 0x10000000 -> 0x80000000（leaf 0x50000，0x200000DF）；V1 0x10001000 -> 0x80001000（0x50008，0x200004DF）；Vro 0x10002000 -> 0x80000000（0x50010，0x200000D3，r=1/w=0/x=0）；Vcold 0x10003000 -> 0x80002000（0x50018，0x200008DF），`tests/composition_tl_test.cpp:358-361` | A page n: 0x20000000+n*0x1000 -> 0x90000000+n*0x1000，n=0..15，`tests/composition_tl_test.cpp:366-371` |

原文第 207 行“关键 PTE 地址”行本身已经正确列入 `Vro 0x50010, Vcold 0x50018`，与本次修正一致；只需改第 205 行，以消除同一文档内部矛盾。

错误来源（避免再次混写）：Fixture B（`tests/nonblock_actual_dma_test.cpp:80-85,473-477`）的 leaf `0x50000/0x50008/0x50010` 分别是 V_SRC/V_DST/V_COLD_DST，其索引 2 的 SPA 恰好是 0x80002000。第一 worker 把 Fixture B 的“索引 2 = cold”语义带进了 Fixture A 的 IOVA→PA 行。

Fixture B **不得随之修改**：其正确构造是 src 0x50000（0x10000000→0x80000000）、dst 0x50008（0x10001000→0x80001000）、cold 0x50010（0x10002000→0x80002000），与原文第 221/223 行一致。两套 fixture 不可互相“对齐”。

## E2（未知项关闭，取代原文第 112 行与第 276 行相关句）

原文（保留）：
> 112: - 但 fastprobe 的 `n_iotlb_pscid` 由未提供的 `rv_iommu_nonblock_hit` 产生，`rv_iommu_tl_wrap.sv:325-327`；给定文件不能证明它如何从 candidate DID 的 DDTC/PC 推导 PSCID。这是关键未知。
> 276: - `rv_iommu_nonblock_hit.sv` 未提供：fastprobe 如何得到 `n_iotlb_pscid/n_iotlb_vpn`、如何使用 candidate DID/DDTC、以及 fast hit 的完整 gating 不能从给定文件完全证明。

状态更新：该文件本轮已提供（`cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv`，177 行）。以下为具体 file:line 证据。

DID / DC / PSCID / VPN 来源（“自有请求”，非借用活跃 walker 上下文）：
- `rv_iommu_nonblock_hit.sv:47-49`：`start`、`busy_o`、`intercept_o` 定义。
- `rv_iommu_nonblock_hit.sv:52`：`ddtc_did_o = req_q.did` —— DDTC 查找使用候选请求**自己的 DID**，不是当前 main owner 的 DID。
- `rv_iommu_nonblock_hit.sv:143-146`：仅 `start` 时 `req_q <= candidate_i`（IDLE 锁存候选请求）。
- `rv_iommu_nonblock_hit.sv:51`：`ddtc_lookup_o = state_q == DC || state_q == LEAF`。
- `rv_iommu_nonblock_hit.sv:153`：`dc_q <= ddtc_dc_i` —— DC 在 DC 状态捕获，来源即上述 DID 的 DDTC 命中内容。
- `rv_iommu_nonblock_hit.sv:55`：`iotlb_pscid_o = dc_q.ta.pscid`；`:54`：`iotlb_vpn_o = req_q.iova[GPLEN39-1:12]`。
- `rv_iommu_nonblock_hit.sv:60-61` 注释：“The actual cached DC belongs to this request. No active-walker context or saved inferred PSCID is used. All excluded cases fall through.”
- 结论：fast probe 的 IOTLB 查找键 = 候选请求自己 DID→DC 得到的 PSCID + 候选自己的 IOVA VPN。**不存在跨请求借用 PSCID 的结构**。该项由“关键未知”升级为“已由源码证明”。
- 仍需 wrapper 侧证据的部分（本 worker 未重核行号）：`gscid` 与 stage 位不是本模块输出，由 `rv_iommu_tl_wrap.sv:275-276` 侧强制（`n_iotlb_lu` 时 `en_1S=1/en_2S=0`，`gscid='0`）。

DC gating（`rv_iommu_nonblock_hit.sv:84-93`）：
- 必须 `ddtc_hit_i` 且 DC `tc.v=1`，且 `!pdtv、!sxl、!t2gpa、!en_ats、!en_pri`；`!pid_valid、!priv、!is_debug`；`is_write||is_read`；`canonical`（`:58` 检查 IOVA[63:39] 符号扩展）；`fsc.mode == ModeSv39`；`iohgatp.mode == ModeBare`；以及 IOMMU mode/DID 宽度检查（`:91-93`）。
- 与 wrapper 强制 `en_2S=0`、`gscid=0` 自洽：fast 路径本质是 stage1-only、G-stage Bare 的 4KiB 命中。
- `:149-151`：DC 状态任一稳定性/合法性条件不满足即转 REJECT，不产生 touch。

接受与 touch 条件（关键）：
- `rv_iommu_nonblock_hit.sv:100-103`：`accept_o = (state_q == LEAF) && enable_i && candidate_valid_i && (candidate_i == req_q) && !invalidate_i && (ddtp_i == ddtp_q) && dc_ok && (ddtc_dc_i == dc_q) && leaf_ok`。
  - 即候选请求、`ddtp`、以及**同 DID 的 DC 回读**都必须与捕获值逐位相同；DC 不稳定会取消接受。
- `rv_iommu_nonblock_hit.sv:94-99`：`leaf_ok` 要求 `iotlb_hit_i`、`content_1S.v`、`content_1S.u`、非 1G、非 2M，并按访问类型检查权限（写查 `w`、读查 `r`）。
- `rv_iommu_nonblock_hit.sv:106-107`：`ddtc_touch_o = accept_o; iotlb_touch_o = accept_o;`；`:104-105` 注释：“Probes do not change replacement state; only an actually accepted hit touches both real caches, exactly once.”
  - 因此 touch 是（a）仅在被接受的 hit 上、（b）恰好一次、（c）DDTC 与 IOTLB 由同一布尔驱动。
  - **该布尔不含任何 requester class/DID 信息**；IOTLB 侧看到的仍是 vpn/pscid（+ wrapper 的 stage/gscid/touch）。这是 root “main A 普通 alias hit 与 main A 忙时 fast V hit 在 IOTLB 输入边界不可区分”的直接源码支撑。
- `rv_iommu_nonblock_hit.sv:111-112`：`probe_stable` 不参与握手；`:114-132` 的 `diag_o` 为纯观测。
- `:159-167`：响应 SPA 用**被命中条目**的 `content_1S.ppn` + 本请求 `iova[11:0]`，权限也复制自该条目（`leaf_ok` 已按本请求访问类型检查）。

仍未解决 / 仍未核（保持未知，不得自推）：
- `rv_iommu_tl_wrap.sv:271-277,303-306,309-330,325-327` 的具体行号与 mux 细节，本 worker 未重核（沿用原引用）。
- `GPLEN39/GPPNW39/VPNW39` 与 `req_t/resp_t/dc_t/ddtp_t` 的字段定义（在 `rv_iommu_pkg.sv` 与 test wrapper）未由本文件给出。
- 生产/测试实例中 `HasMsiContext` 的实际取值未核；`:62-83` MSI guard 是否生效取决于该参数。
- `rv_iommu_msiptw.sv`、`rv_iommu_mrifc.sv`、`rv_iommu_ddtw.sv`、`rv_iommu_pdtw.sv` 仍未提供，不能对其内部结构下定论。
- 原文建议清单第 1 项（nonblock_hit）现已满足；第 2、3 项（msiptw、ddtw）仍开放。

## E3（不改动，仅确认一致）

- 原文第 207 行关键 PTE 地址行：与修正后 E1 一致，无需修改。
- 原文第 236 行“旧实验的 V/A 是分离 context、分离 root、分离 IOVA……不是 A/V 共享”：仍然成立。
- 原文第 151 行“fillmask/PLRU historical isolation 不拒绝共享命中”：与 `rv_iommu_nonblock_hit.sv` 无冲突（fast 的 touch 只是 accept 布尔）。
- `codex-requester-history-boundary.md:17` 中 P3=0x10003000 → leaf 0x50018、值 0x200008DF → SPA 0x80002000 与 E1 修正一致；原文第 205 行与此矛盾，属第一 worker 单点混写，不是两文档间实质分歧。

## 验收检查（供 root）

1. 修正后文档中不再出现“Vcold 0x10002000”组合；0x10002000 只与 Vro/只读关联。
2. Fixture B 行（src 0x50000 / dst 0x50008 / cold 0x50010）保持原样。
3. nonblock_hit 证据可 grep 到 `ddtc_did_o = req_q.did`、`iotlb_pscid_o = dc_q.ta.pscid`、`accept_o`、`ddtc_touch_o = accept_o`。
4. 未解决清单仍显式包含：tl_wrap 行号未复核、`HasMsiContext` 未核、msiptw/ddtw 未提供。


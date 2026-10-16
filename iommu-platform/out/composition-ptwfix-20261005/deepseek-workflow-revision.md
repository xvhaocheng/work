## 结论

只给出一份可应用 patch，相对于本次传入的两份实际文档（`MAC_FPGA_HANDOFF.md`、`iommu-platform/docs/composition-v1-mac-20261004.md`，均未应用过 DS 草稿）。未执行任何命令、未读取未提供文件、未包含任何凭据。

核心动作：
- `composition-v1-mac-20261004.md`：把顶部 `composition-trim + net3` 标题改为现行 `composition-ptwfix route-opt + net4trace + PN4096（全 172 进行中）`入口，旧 net3 段整体降为历史；旧命令/交付/cpn1 段落保留原样，不整理。
- `MAC_FPGA_HANDOFF.md`：标题后插入“现行入口索引”，下方 03:06 与 2026-09-15 保留。

---

## Unified diff：`iommu-platform/docs/composition-v1-mac-20261004.md`

```diff
diff --git a/iommu-platform/docs/composition-v1-mac-20261004.md b/iommu-platform/docs/composition-v1-mac-20261004.md
--- a/iommu-platform/docs/composition-v1-mac-20261004.md
+++ b/iommu-platform/docs/composition-v1-mac-20261004.md
@@ -1,7 +1,94 @@
 # composition-v1 交付与上板说明（2026-10-04）
 
-## 2026-10-05 最新纯网口（SSH）入口：composition-trim route-opt + net3 + 172
-
+## 2026-10-05 现行入口：composition-ptwfix route-opt + net4trace + PN4096（全 172 进行中）
+
+> 本节是当前操作入口；下方“composition-trim route-opt + net3 + 172”一节及其后的串口 SD 上板 / cpn1 示例均标为历史，只保留原始记录。全量 172 仍在运行，未完成、未全部通过；本节不写研究收益。
+
+### 当前硬件与状态
+
+- 现行 Mac BIT：`tools/composition-ptwfix-20261005/ariane_xilinx.bit`；Dell 验收对应 [out/composition-ptwfix-20261005/route-opt/ariane_xilinx.bit](../out/composition-ptwfix-20261005/route-opt/ariane_xilinx.bit)。不要用旧 composition-trim/net3 BIT 代替当前 BIT。
+- 验收记录 [acceptance.json](../out/composition-ptwfix-20261005/acceptance.json)：route-opt BIT 最终时序通过，WNS +0.031 ns、WHS +0.019 ns、WPWS +0.062 ns；276241/276241 线路布通、errors 0；native BIT DRC errors 0、组合环 / 锁存环 / 无时钟 0。完整板级 IO/CDC 签核仍未完成。
+- RTL 修复：沿用 DS 写通道来源对齐修复，并修正 PTW 合法叶分支（`PTW_legal_leaf_branching`）。针对性仿真 [targeted-test-acceptance.json](../out/composition-ptwfix-20261005/targeted-test-acceptance.json) 由旧失败转为 7/0 通过；旧 legal 4KiB 读、写被分别误报为 cause 13、cause 15。该范围只覆盖 PTW 接口与既有 TL 回归，不等于板级 PN 验收。
+- 已加载当前 BIT 并启动 Linux/SSH：boot_id `2695d848-8589-491e-bf7f-1de7af5ea9cb`；此前失败的 PN4096 实际 1/1 实板通过（V_completed 4096、A_completed 7244、V/A data errors 0、taint 0）。
+- 完整冻结 172 当前在 `results/composition-ptwfix-20261005-formal01` 运行，尚无完整结果；此处不写完成、不写全部通过、不写研究收益。旧正式记录保留：net3 一轮 52 通过 / 1 失败 / 119 未运行，未验收，不自动重跑。
+
+### 当前内核与网络
+
+- 现行内核：`6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty`。
+- Mac `192.168.77.1`，FPGA `192.168.77.2`，USB 网卡百兆，JTAG `1000000`。SD 常驻板卡，正常工作走以太网/SSH；UART 只作故障诊断，同一时间只能有一个采集者。
+
+### Mac 项目根目录
+
+```sh
+cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
+```
+
+### 日常：同步并重载当前 BIT（不带 `--payload` = 不写 SD）
+
+```sh
+tools/method88-venv/bin/python scripts/mac_update_reboot.py \
+  --run --transport ssh \
+  --credential-script scripts/run_research2_mac.py \
+  --host-ip 192.168.77.1 \
+  --board-host 192.168.77.2 \
+  --bit tools/composition-ptwfix-20261005/ariane_xilinx.bit \
+  --expected-kernel "6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty" \
+  --out "results/composition-ptwfix-$(date +%Y%m%d-%H%M%S)-reload" \
+  --boot-timeout 600 \
+  --jtag-hz 1000000
+```
+
+- 固定流程：正常工作走以太网、SD 留板卡；仅 RTL 改变时 sync + JTAG 重载指定 BIT、不写 SD；只有 Linux 改变时才加 `--payload <fw_payload.bin>`，网传后只写 32 MiB 第一原始分区并 sync，再用 JTAG 重载已验收 BIT，不覆盖整卡。
+- 不带 `--payload` 时工具仍先 sync 再 JTAG 重载；默认 dry-run，只有 `--run` 才执行；`--out` 必须每次新目录，不覆盖、不 resume。
+- 不加入常规 payload hash、写后回读、重复 build 或额外预跑；保留工具已有身份核对与退出码。
+
+### 异常卡死：先保存证据，再用已有明确 rescue 模式（故障例外，不是默认流程）
+
+先保存可取得的故障输出；UART 诊断保留且只能有一个采集者。确认既有 JTAG XVC 已正常退出后，直接加载同一已验收 BIT：
+
+```sh
+/opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000 \
+  tools/composition-ptwfix-20261005/ariane_xilinx.bit
+```
+
+这一步不写 SD；恢复后用 SSH 读取实际内核、boot_id 与网络状态。恢复成功不代表失败实验已通过；无法先 `sync` 只是本次授权例外，不能把免 sync 固化成正常流程。
+
+### 跑冻结 172（纯网口 SSH attach；当前在 formal01 进行中）
+
+```sh
+tools/method88-venv/bin/python tools/composition-v1-20261004/mac_composition.py \
+  --run --transport ssh \
+  --board-host 192.168.77.2 \
+  --credential-script scripts/run_research2_mac.py \
+  --ssh-helper scripts/mac_update_reboot.py \
+  --through-phase explore \
+  --expected-kernel "6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty" \
+  --attach-timeout 60 \
+  --out "results/composition-ptwfix-$(date +%Y%m%d-%H%M%S)"
+```
+
+- `--through-phase explore` 才跑冻结 172，一次执行已含前序阶段；`--out` 必须不存在。一次只执行一遍 172。
+- 失败即停：技术失败保留现场后停止并定位，不自动重跑；禁止自动改 172 计划或阈值、禁止用重试掩盖失败；科学阴性（fast==0、miss==0、无收益）不是技术失败。
+- 结束把原始目录（含失败现场）回传 Dell，释放 UART/JTAG 占用而 Linux 保持运行。
+- 正式 172 前关闭 trace（`echo N > /sys/module/idma/parameters/composition_trace`）；单 PN4096 只是前置诊断，不是 172 的一部分。
+- 凭据只通过现有 `scripts/run_research2_mac.py` helper；不读、不输出密码或密钥；不要把 `root@` 写进 `--board-host`。
+
+### 待填：全 172 结论（172 完成后替换此段）
+
+- 全量 172 运行中，目录 `results/composition-ptwfix-20261005-formal01`；待真实出现 `summary.json` 或 `STOPPED.txt` 后，在此填 planned/attempted/completed/failed 与结论。不得预填通过。
+- 不要把旧 net3 的 52/1/119 当作本次全量结果，也不要在 172 完成前宣称机制或性能收益。
+
+### 安全与失败边界
+
+- 不夸大板端完整 IO/CDC 签核，也不写成机制创新或性能收益。
+- 不做 payload hash、SD 回读、冗余测试或新框架；不做串口抢读。
+
+---
+
+## 2026-10-05 历史入口：composition-trim route-opt + net3 + 172
+
+> 以下为历史 net3 一轮，记录保留，不再作为当前入口。
+
 > 本节覆盖旧的串口上板入口；旧历史说明保留在下方。日常传输和操作走网口；当前开发实验保留 UART 记录现场，并保留 JTAG 恢复。net3 内核从原 SD 启动，本轮不写 SD。
 
 ### 当前硬件与网络
```

---

## Unified diff：`MAC_FPGA_HANDOFF.md`

```diff
diff --git a/MAC_FPGA_HANDOFF.md b/MAC_FPGA_HANDOFF.md
--- a/MAC_FPGA_HANDOFF.md
+++ b/MAC_FPGA_HANDOFF.md
@@ -1,4 +1,16 @@
 # Mac 本地任务交接：Genesys 2 首次上板
 
+## 现行入口索引（2026-10-05）
+
+> 当前操作以 [composition-v1-mac-20261004.md 顶部现行入口](iommu-platform/docs/composition-v1-mac-20261004.md) 为准；本节只做索引，下方 03:06 与 2026-09-15 内容保留为历史。
+
+- 当前 Mac BIT：`tools/composition-ptwfix-20261005/ariane_xilinx.bit`；Dell 验收对应 [out/composition-ptwfix-20261005/route-opt/ariane_xilinx.bit](iommu-platform/out/composition-ptwfix-20261005/route-opt/ariane_xilinx.bit)，验收记录 [acceptance.json](iommu-platform/out/composition-ptwfix-20261005/acceptance.json)。验收：WNS +0.031 ns、WHS +0.019 ns、WPWS +0.062 ns，276241/276241 线路布通，native BIT DRC errors 0；完整板级 IO/CDC 签核仍未完成。不要用旧 composition-trim/net3 BIT 代替。
+- 当前内核：`6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty`。Mac `192.168.77.1`，FPGA `192.168.77.2`，USB 网卡百兆，JTAG `1000000`；SD 常驻板卡，正常工作走以太网/SSH，UART 仅故障诊断且单进程拥有。
+- PN4096：此前失败，本次实际 1/1 实板通过，V/A data errors 0、taint 0；boot_id `2695d848-8589-491e-bf7f-1de7af5ea9cb`。单 PN4096 是前置诊断，不是 172 的一部分；正式 172 前关闭 `/sys/module/idma/parameters/composition_trace`。
+- 完整冻结 172：当前在 `results/composition-ptwfix-20261005-formal01` 运行，尚无完整结果，未完成/未全部通过；不得预填通过或宣称研究收益。旧 net3 记录 52 通过/1 失败/119 未运行保留，未验收，不自动重跑。
+- 固定流程：正常工作走以太网、SD 留板卡；仅 RTL 改变时 sync + JTAG 重载指定 BIT、不写 SD；只有 Linux 改变时才网传 `fw_payload` 后只写 32 MiB 第一原始分区并 sync，再 JTAG 重载已验收 BIT。异常卡死先保存证据，再用已有明确 rescue 模式；免 sync 只是本次例外，不固化为默认流程。
+- 一次只执行一遍 172；技术失败保留现场后停止并定位，不自动重跑、禁止自动改 172 计划或阈值、禁止用重试掩盖失败；结束回传原始目录并释放占用而 Linux 保持运行。
+- 不加入常规 hash/回读、重复 build、额外预跑或串口抢读；凭据只通过现有 helper，不读/不输出密码或密钥；`--board-host` 不放 `root@`。命令见 composition-v1-mac-20261004.md 顶部现行入口。
+
 ## 2026-10-05 03:06 JST：新的四模式BIT已完成构建验收
 
```

---

## 变更说明

- 仅新增现行入口并标注旧段历史；旧 net3 结果（52/1/119）、旧串口上板、旧 cpn1 示例、旧“尚未上板”状态全部原样保留，未重写。
- 事实来源：
  - BIT 路径/时序 `+0.031/+0.019/+0.062`、布通 `276241/276241`、`native_DRC_errors 0`：`iommu-platform/out/composition-ptwfix-20261005/acceptance.json` 行 9–22、行 27。
  - 修复项 `DS_write_source_alignment`、`PTW_legal_leaf_branching`：`acceptance.json` 行 3–6。
  - targeted 7/0 通过、旧 legal 4KiB 读/写误报 cause 13/15、范围“not board PN acceptance”：`targeted-test-acceptance.json` 行 10–13、行 19–23、行 32–56。
  - boot_id `2695d848-8589-491e-bf7f-1de7af5ea9cb`、PN4096 1/1 pass、旧 52/1/119、formal01：`execution-state.json` 行 7、行 9–15、行 60–79。
  - PN4096 数据错误 0/taint 0：`mac-records/results/composition-ptwfix-20261005-pn01/summary.json` 行 12–16。
  - 正式 172 前关闭 trace：`out/composition-ptwfix-20261005/mac-formal-prepared.py` 行 9、行 12。
  - 单 N4096 前置诊断：`mac-case-prepared.py` 行 8、行 15。
  - CLI 参数名核对：`scripts/mac_update_reboot.py` 行 902–929、行 966–1015、行 870–876（无 `--payload` 时不写卡、仍 sync 后 JTAG）；`scripts/mac_composition.py` 行 670–698、行 732–744。

## 未完成/待填

- **需在 172 完成后替换的最小结论段**：`composition-v1-mac-20261004.md` 新增的 `### 待填：全 172 结论（172 完成后替换此段）`（两行 bullet）；以及 `MAC_FPGA_HANDOFF.md` 新增索引里“完整冻结 172”那一条 bullet。当前只写“运行中/未完成/未全部通过”，不得预填通过。
- 未改源码、驱动、RTL、冻结 172 计划或阈值；未做 32 MiB 之外的任何写入说明；未加入 hash/回读/重复 build/额外预跑。
- 旧 net3 段的命令块（`tools/composition-trim-20261005/...`）保留在历史标题下，未逐个改写，符合“不顺便整理历史全文”。

## 假设与局限

- 我无法执行命令或读未提供文件；本 patch 基于所给快照，未实际 `git apply`。
- diff 前缀假定 git 仓库根同时含 `MAC_FPGA_HANDOFF.md` 与 `iommu-platform/`（与既有链接风格一致）。若实际仓库根不同，只调整 `a/`、`b/` 前缀，不改 hunk 内容。
- `--payload` 的 payload 文件名按现有交付约定写作占位 `<fw_payload.bin>`，未虚构 Mac 上的具体路径（传入内容未给出 ptwfix 的 payload 路径）。
- 未把 acceptance.json 的 `board_verified=false` 当成板级结论；板级状态以 execution-state 的加载/PN4096 与 boot_id 为准，并仍写明完整 IO/CDC 未签核。

## 建议验收检查

- `git apply --check`（或 `patch --dry-run`）通过，全 0 冲突。
- 正文两个命令的 `--board-host` 不含 `root@`；`--out` 均为新目录/时间戳。
- 172 仍写为“运行中/未完成/未全部通过”，未出现“通过/收益”。
- 命令参数名与 `scripts/mac_update_reboot.py`、`scripts/mac_composition.py` 定义一致。
- 旧 net3 的 52/1/119 与旧串口/cpn1 段除新增历史标注外字节未变。
- 文档内不出现任何密码、密钥或凭据内容（只有 helper 路径 `scripts/run_research2_mac.py`）。
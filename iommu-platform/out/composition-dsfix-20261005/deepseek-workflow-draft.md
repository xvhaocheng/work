## 导航摘要

- 仅给出两份文档的补丁草案，未执行、未改文件、未写完成结果。
- `iommu-platform/docs/composition-v1-mac-20261004.md`：顶部新增 `composition-dsfix + net4trace + 单 N4096 / 全 172（进行中）` 现行入口；旧 `composition-trim/net3` 标为历史；旧串口上板与 cpn1 172 示例标历史；旧“尚未上板”状态加历史快照标注。
- `MAC_FPGA_HANDOFF.md`：在标题后、旧 03:06 构建验收前插入简明现行入口索引。
- 全量 172 仍写作“运行中/待填”，旧 52 通过 / 1 失败 / 119 未运行保留。未改源码、驱动、RTL、冻结 172 计划或门槛。

## Unified diff：`iommu-platform/docs/composition-v1-mac-20261004.md`

```diff
diff --git a/iommu-platform/docs/composition-v1-mac-20261004.md b/iommu-platform/docs/composition-v1-mac-20261004.md
--- a/iommu-platform/docs/composition-v1-mac-20261004.md
+++ b/iommu-platform/docs/composition-v1-mac-20261004.md
@@ -1,5 +1,94 @@
 # composition-v1 交付与上板说明（2026-10-04）
 
-## 2026-10-05 最新纯网口（SSH）入口：composition-trim route-opt + net3 + 172
-
+## 2026-10-05 现行入口：composition-dsfix route-opt + net4trace + 单 N4096 / 全 172（全量进行中）
+
+> 本节是当前操作入口；下方“composition-trim route-opt + net3 + 172”及更早的串口/cpn1 内容均标为历史，仅保留原始记录。本轮不重写历史结果，不把全量 172 写成完成/通过，也不陈述研究收益。
+
+### 当前已验收产物与状态
+
+- 现行 Mac BIT：`tools/composition-dsfix-20261005/ariane_xilinx.bit`；Dell 对应验收产物：[out/composition-dsfix-20261005/route-opt/ariane_xilinx.bit](../out/composition-dsfix-20261005/route-opt/ariane_xilinx.bit)。不要用旧 composition-trim/net3 BIT 代替当前 BIT。
+- 验收记录：[out/composition-dsfix-20261005/acceptance.json](../out/composition-dsfix-20261005/acceptance.json)：route-opt BIT 最终时序/原生 DRC 通过，WNS +0.048 ns、WHS +0.040 ns、WPWS +0.062 ns；275498 线路布通 / errors 0；native DRC errors 0。完整板级 IO/CDC 签核仍未完成。
+- DS 写通道修复：AW/W 来源错配修正、owner FIFO 空满保护；四项针对性仿真已由旧失败转为新通过（见 [out/composition-dsfix-20261005/targeted-test-acceptance.json](../out/composition-dsfix-20261005/targeted-test-acceptance.json)；执行状态见 [execution-state.json](../out/composition-dsfix-20261005/execution-state.json)）。
+- Mac 已加载当前 BIT，Linux/SSH 启动通过：boot_id `7651a76f-6f96-4bd6-8e67-090391a54af1`；单 N4096 已 1/1 实板通过，data errors 0、taint 0。
+- 完整冻结 172 当前在新目录 `results/composition-dsfix-20261005-formal01` 运行；状态见 [execution-state.json](../out/composition-dsfix-20261005/execution-state.json) 的 `full172_new`。此处不写完成、不写全部通过、不写研究收益，待真实全量结果后填。
+- 旧正式记录保留：52 通过 / 1 失败 / 119 未运行（composition-trim route-opt + net3 历史一轮），未验收，不自动重跑。
+
+### 当前内核与网络
+
+- 现行内核：`6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty`。
+- Mac `192.168.77.1`，FPGA `192.168.77.2`，USB 网卡百兆，JTAG `1000000`。SD 常驻，日常走网口；UART 仅用于故障诊断，同一时间只能有一个采集者。
+
+### Mac 项目根目录
+
+```sh
+cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
+```
+
+### 只重载当前 BIT（不带 `--payload` = 不写 SD；正常先 sync）
+
+```sh
+tools/method88-venv/bin/python scripts/mac_update_reboot.py \
+  --run --transport ssh \
+  --credential-script scripts/run_research2_mac.py \
+  --host-ip 192.168.77.1 \
+  --board-host 192.168.77.2 \
+  --bit tools/composition-dsfix-20261005/ariane_xilinx.bit \
+  --expected-kernel "6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty" \
+  --out "results/composition-dsfix-20261005-$(date +%Y%m%d-%H%M%S)-reload" \
+  --boot-timeout 600 \
+  --jtag-hz 1000000
+```
+
+- 不带 `--payload`：只同步并 JTAG 重载指定 BIT，不写 SD；工具默认 dry-run，只有 `--run` 才执行。除显式 JTAG 重载外不追加重启，失败即停，不自动重跑。
+- `--out` 必须每次新目录，不能覆盖旧结果。
+- 若已有 JTAG XVC 占用端口，先正常退出释放；不要在未释放时直接加载。
+- 故障现场无法先 `sync` 时，直接 JTAG 重载只能是本次已授权例外；不把跳过 sync 固化为默认流程。
+
+### Linux 已失联时：直接 JTAG 恢复（故障例外，不是默认）
+
+先保存可取得的故障输出；UART 故障诊断保留且只能单一采集者。确认已有 XVC 已正常退出后，直接加载同一已验收 BIT：
+
+```sh
+/opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000 \
+  tools/composition-dsfix-20261005/ariane_xilinx.bit
+```
+
+这一步不写 SD；恢复后通过 SSH 读取实际内核、boot_id、网络状态。恢复成功不代表失败的 172 已通过。
+
+### 跑冻结 172（纯网口 SSH attach；当前全量在 formal01 进行中）
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
+  --out "results/composition-dsfix-20261005-$(date +%Y%m%d-%H%M%S)"
+```
+
+- `--through-phase explore` 才会跑冻结的 172；`--out` 必须不存在，禁止自动重跑/覆盖。
+- 单 N4096 是前置诊断，不是 172 的一部分；当前 172 与前置单 N 共享同一 boot（`7651a76f-6f96-4bd6-8e67-090391a54af1`），正式实验前需关闭 `/sys/module/idma/parameters/composition_trace`。
+- `mac_composition.py` 已支持 SSH transport，复用 `mac_research2.SerialConsole` 的 framer、`AttachRunner` 和冻结 172，不新增实验框架。
+- 凭据只通过现有 `scripts/run_research2_mac.py` helper；不要读/输出密码或密钥，不要把 `root@` 写进 `--board-host`。
+
+### 当前结果待填
+
+- 全量 172 运行中，新目录 `results/composition-dsfix-20261005-formal01`；待真实完成后在此填 planned/attempted/completed/failed、STOPPED.txt 或 summary.json 结论。
+- 不要把旧 net3 的 52/1/119 当作本次全量结果。
+
+### 安全与失败边界
+
+- 失败即停：技术失败保留现场后停止，不自动重跑；科学阴性不是技术失败。
+- 不做 payload hash、SD 回读、冗余测试或新框架；Linux 更新复用现有网传、32 MiB 第一原始分区写入、sync、JTAG 重载、校验新内核与 boot_id，不覆盖整卡。
+- 不夸大板端完整 IO/CDC 签核，也不写成机制创新或性能收益。
+
+---
+
+## 2026-10-05 历史入口：composition-trim route-opt + net3 + 172
+
+> 以下为历史 net3 一轮，记录保留，不再作为当前入口。
+
 > 本节覆盖旧的串口上板入口；旧历史说明保留在下方。日常传输和操作走网口；当前开发实验保留 UART 记录现场，并保留 JTAG 恢复。net3 内核从原 SD 启动，本轮不写 SD。
@@ -136,11 +225,13 @@
-## 2. 上板（Codex 通过 ssh 到 Mac 执行；SSH 目标是 Mac，不是板上的 Linux）
+## 2. 上板（历史串口流程；已由顶部网口入口取代；SSH 目标是 Mac，不是板上的 Linux）
+
+> 历史示例：下面描述旧的“先开串口采集，再加载 BIT”串口上板流程，保留原始记录。现行日常入口见顶部“只重载当前 BIT / 跑冻结 172”。
 
 1. SD 插入**断电的** Genesys 2，接好串口与 JTAG，再上电。
 2. 先做串口采集（记录启动输出）。
 3. 加载一次 BIT：
 
 ```sh
 /opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000 'tools/composition-v1-20261004/ariane_xilinx.bit'
 ```
 
 4. 等 Linux 起到 root shell；保存启动日志并关闭临时串口采集，再启动下方 runner，避免串口同时被占用。
@@ -150,3 +241,5 @@
-## 3. 运行全部 172（Codex 执行）
+## 3. 运行全部 172（历史 cpn1 attach 示例；现行入口见顶部，必须显式 --transport ssh、--board-host、--credential-script、--ssh-helper、--expected-kernel net4trace）
+
+> 历史示例：下面使用 cpn1 内核和默认 UART attach；现行全量入口见顶部。
 
 ```sh
@@ -181 +274 @@
-构建状态：Linux已完成；BIT待构建完成后更新。尚未上板。
+构建状态（历史快照）：Linux已完成；BIT待构建完成后更新。尚未上板。当前状态见顶部现行入口。
```

## Unified diff：`MAC_FPGA_HANDOFF.md`

```diff
diff --git a/MAC_FPGA_HANDOFF.md b/MAC_FPGA_HANDOFF.md
--- a/MAC_FPGA_HANDOFF.md
+++ b/MAC_FPGA_HANDOFF.md
@@ -1,3 +1,16 @@
 # Mac 本地任务交接：Genesys 2 首次上板
 
+## 现行入口索引（2026-10-05）
+
+> 当前操作以 [composition-v1-mac-20261004.md 顶部现行入口](iommu-platform/docs/composition-v1-mac-20261004.md) 为准；本节只做索引，下方 03:06 与 2026-09-15 内容保留为历史。
+
+- 当前 Mac BIT：`tools/composition-dsfix-20261005/ariane_xilinx.bit`；Dell 验收对应：[out/composition-dsfix-20261005/route-opt/ariane_xilinx.bit](iommu-platform/out/composition-dsfix-20261005/route-opt/ariane_xilinx.bit)，验收记录：[acceptance.json](iommu-platform/out/composition-dsfix-20261005/acceptance.json)。不要用旧 composition-trim/net3 BIT 代替。当前 BIT 验收：WNS +0.048 ns、WHS +0.040 ns、WPWS +0.062 ns，275498 线路布通，native DRC errors 0；完整板级 IO/CDC 签核仍未完成。
+- 当前内核：`6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty`。Mac `192.168.77.1`，FPGA `192.168.77.2`，USB 网卡百兆，JTAG `1000000`。
+- 单 N4096：已 1/1 实板通过，data errors 0、taint 0；boot_id `7651a76f-6f96-4bd6-8e67-090391a54af1`。这是前置诊断，不是 172 的一部分；当前 172 与前置单 N 共享同一 boot，正式实验前关闭 `/sys/module/idma/parameters/composition_trace`。
+- 完整冻结 172：当前在 `results/composition-dsfix-20261005-formal01` 运行；未完成/未全部通过，待真实结果后填写。旧 52 通过/1 失败/119 未运行保留。
+- 日常只重载 BIT（不写 SD）：`tools/method88-venv/bin/python scripts/mac_update_reboot.py --run --transport ssh --credential-script scripts/run_research2_mac.py --host-ip 192.168.77.1 --board-host 192.168.77.2 --bit tools/composition-dsfix-20261005/ariane_xilinx.bit --expected-kernel "6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty" --out "results/composition-dsfix-20261005-$(date +%Y%m%d-%H%M%S)-reload" --boot-timeout 600 --jtag-hz 1000000`。不带 `--payload` 只同步并 JTAG 重载，不写 SD；工具默认 dry-run。
+- 全量运行：`tools/method88-venv/bin/python tools/composition-v1-20261004/mac_composition.py --run --transport ssh --board-host 192.168.77.2 --credential-script scripts/run_research2_mac.py --ssh-helper scripts/mac_update_reboot.py --through-phase explore --expected-kernel "6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty" --attach-timeout 60 --out "results/composition-dsfix-20261005-$(date +%Y%m%d-%H%M%S)"`。`--out` 必须新目录，禁止自动重跑/覆盖。
+- 约定：不做全量哈希、SD 写后回读、冗余测试或新框架；Linux 更新复用现有网传/32 MiB 第一原始分区写入/sync/JTAG 重载/校验新内核与 boot_id，不覆盖整卡。凭据只通过现有 helper，不读/不输出密码或密钥；不要把 `root@` 放进 `--board-host`。
+- UART：故障诊断保留，同一时间只能有一个采集者；不得写成以后永久不需要 UART。JTAG 重载 helper 无 payload 只加载指定 BIT，正常先 sync；不能 sync 的故障现场 rescue 只是本次授权例外，不固化为默认跳过 sync。已有 XVC 必须先正常退出释放端口。
+
 ## 2026-10-05 03:06 JST：新的四模式BIT已完成构建验收
```

## 证据与行号

- 旧现行入口：`iommu-platform/docs/composition-v1-mac-20261004.md` 行 3–12 为旧 net3/trim 入口；行 52–64 为旧 172 命令；行 136–146 为旧串口上板；行 150–164 为旧 cpn1 attach 示例，其中行 158 是 cpn1 内核；行 181 为旧“尚未上板”状态。补丁只新增/标注，不删除原始结果。
- Mac 交接：`MAC_FPGA_HANDOFF.md` 行 1 标题、行 3 旧 03:06 构建验收、行 9 旧当前拷贝/上板入口、行 26 旧默认值、行 35 net3 状态、行 45 起 2026-09-15 历史。补丁在行 2 后插入索引，旧内容保留。
- 参数来源：`scripts/mac_composition.py` 行 674–697 定义 `--transport/--board-host/--credential-script/--ssh-helper/--out/--expected-kernel/--through-phase`；行 736–744 对 `--out` 已存在和 ssh 必填参数做硬拒绝；行 677–678 明确 `--board-host` 不扫描；行 694 `--through-phase` 默认 `short`。
- `scripts/mac_update_reboot.py` 行 898–930 定义 `--run/--transport/--bit/--expected-kernel/--out/--payload/--host-ip/--board-host/--credential-script/--jtag-hz/--boot-timeout`；行 974–987 做必填校验；行 870–876 显示无 `--payload` 时跳过下载写卡，但仍 sync 后 JTAG 重载；行 756–788 下载、sync、加载；行 790–820 等待新内核与 boot_id；行 241 显示 SSH argv 内部加 `root@`，因此 `--board-host` 不应再放 `root@`。
- 当前状态：`out/composition-dsfix-20261005/acceptance.json` 行 6–8 时序，行 14–19 布通/native DRC，行 23 完整 IO/CDC 未签核，行 24 BIT；`execution-state.json` 行 12–16 旧 52/1/119，行 54 Mac BIT，行 60 boot_id，行 62–69 单 N4096，行 71 全量 out，行 75–80 `full172_new.status=running` 且 completed/failed 为 null。
- 正式 172 前关闭 trace 的来源：`out/composition-dsfix-20261005/mac-formal-prepared.py` 行 9、行 12、行 15、行 26。

## 待最终 172 结果填入之处

- `composition-v1-mac-20261004.md` 新增 `### 当前结果待填`：待 `results/composition-dsfix-20261005-formal01/summary.json` 或 `STOPPED.txt` 真实出现后填 planned/attempted/completed/failed。
- `MAC_FPGA_HANDOFF.md` 新增 `完整冻结 172` 条目：当前只能保留“运行中/未完成/未全部通过”，不得预填通过。
- `out/composition-dsfix-20261005/execution-state.json` 的 `full172_new` 当前为 `running/null/null`；最终结果出来后只更新该处与实际 summary，不覆盖旧 52/1/119 历史。

## 假设与局限

- 我无法执行命令或修改文件；以上为基于所给快照的补丁草案。请用 `git apply --check` 或等效方式核对行号后应用。
- `targeted-test-acceptance.json` 的正文未提供，补丁只链接并引用已给事实，不展开具体仿真项。
- `MAC_FPGA_HANDOFF.md` 的链接按现有内容风格假定该文件位于 `iommu-platform` 上一级目录；若实际位置不同，只调整索引链接前缀。
- 补丁未改源码、驱动、RTL、冻结 172 计划或门槛；未读/输出密码或密钥；未把全量 172 写成完成/通过。
- 建议验收检查：`--board-host` 示例中不含 `root@`；全量 172 仍为运行中；旧历史结果区块除标注外未被重写；新命令参数与 `mac_composition.py` / `mac_update_reboot.py` 参数名一致。
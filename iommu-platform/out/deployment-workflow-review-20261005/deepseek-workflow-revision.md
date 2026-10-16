## (1) 已复核的入口地图

以下整节可直接放到 Dell 根 `MAC_FPGA_HANDOFF.md` 标题之后。未执行、未写卡、未切 BIT、未上板。

```markdown
## 2026-10-05 当前入口地图（只读复核版；旧正文保留为历史）

> 文件优先级：Dell 根 `MAC_FPGA_HANDOFF.md:3` 仍是 2026-09-15 旧正文；本节只补入口地图，旧正文不删。Mac 快照 `mac-snapshot/MAC_FPGA_HANDOFF.md:3` 已从 2026-10-04 N 原型草案审查起更新。Mac 最新 SD 交接优先 `mac-snapshot/MAC_HANDOFF.md`；Dell 侧优先 `CURRENT_STATUS.md`/`HANDOFF.md`。

### 0. 一句话链

Dell 编译/交付 → Mac 传输（旧 fetch 固定版本慎用；新软件与四模式 tools 按新路径）→ Mac 显式写 SD 第一分区 → 先开串口采集 → JTAG 一次易失加载（不加 `-f`）→ 关闭临时采集 → 仅用户授权实验才 attach runner → raw 完整结果回 Dell 并更新交接。以太网 cpn2-net1 更新是已实现软件的扩展路径，板端启用/验收尚未由本批证据确认。

### 1. Dell 编译/交付 → Mac 传输

- 分工：Mac 负责 JTAG 下载和串口；Dell 负责 FPGA/Linux 编译（`mac-snapshot/README.md:61-62`）。
- 旧固定 fetch 慎用：
  - `scripts/fpga.py fetch` 只从 Dell `ssh tar` 取固定 BIT 和参考资料，校验固定 BIT 大小/SHA，不取新软件（`mac-snapshot/scripts/fpga.py:121-171`, `:27-31`）。
  - `scripts/fetch_software.py` 只按固定清单下旧 software delivery（原回复 `deepseek-workflow-review.md:28`）；新 release 不要用它代替。
  - 新 `software-update-20261004` 的 payload 已在 Mac `artifacts/software/software-update-20261004/delivery/fw_payload.bin`，写卡脚本默认也读同一路径（`iommu-platform/out/sd-install-20261005/receipt.json:5`；`mac-snapshot/scripts/write_research_sd.py:15-17`）。
- 四模式 tools：若交付，走 `tools/composition-v1-20261004/`。`mac_composition.py` 要求与本目录 `mac_research2.py`/`composition_plan.py` 平铺，用 `tools/method88-venv/bin/python`；板端默认 `/usr/bin/idma_composition`、`/dev/idma0`；同一 boot、同一 BIT 内由 mode 0/1/2/3 运行 S/N/P/PN，全 172 用例（`iommu-platform/docs/composition-v1-mac-20261004.md:3`, `:5-21`）。加载命令为直接 `/opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000 'tools/composition-v1-20261004/ariane_xilinx.bit'`（同上 `:61-65`）。但只读存在性检查显示 `tools/composition-v1-20261004/ariane_xilinx.bit` 与 `kernel-release` 不存在（`mac-path-availability.json:10-17`）；只能说明这两个检查位置尚未部署、四模式此路径尚未交付 BIT/kernel-release，不能外推整个 Mac 无其他 BIT 或 Dell 构建失败。
- Mac cpn2 helper：只读检查 `scripts/mac_update_reboot.py` 与 `artifacts/software/software-update-20261004/delivery/mac_update_reboot.py` 均不存在（`mac-path-availability.json:2-9`）；Dell 侧现有 `iommu-platform/scripts/mac_update_reboot.py:1`，用户说明 Dell delivery 中该文件现有，可后续单独复制，不需重造脚本。

### 2. 当前显式写 SD 入口、默认值与盘号限制

- 本轮 release：`software-update-20261004` 只走 `scripts/write_research_sd.py`；必须显式 `--release software-update-20261004`，默认 `next-experiment-20261002` 会写旧镜像（`mac-snapshot/scripts/write_research_sd.py:12-14`）。
- 路径：默认 `artifacts/software/<release>/delivery/fw_payload.bin`；`research-two-bit-20261003` 例外为 `delivery/artifacts/fw_payload.bin`（`write_research_sd.py:15-17`）。本轮 receipt：payload `29,363,656` bytes、kernel `6.5.0-rc2-cpn2-net1-00093-gc0f5e13ea81f-dirty`、`disk:/dev/disk5`、`partition:/dev/rdisk5s1`、`exit_code 0`、`synced/ejected`、`board_boot_verified:false`（`receipt.json:2-20`）。
- `sd_card.py` 现状不能写该 release：`--release` choices 和 skip 白名单都不含 `software-update-20261004`（`mac-snapshot/scripts/sd_card.py:342`, `:152`）。
- 只写第一分区：`sd_card.py` 由 `--disk` 生成 `raw_disk = args.disk.replace("/dev/disk","/dev/rdisk")`，再打开 `raw_disk + "s1"`（`sd_card.py:268`, `:280-284`）；plan 固定 `partition2_written:false`（`:257`）。登记卡例是 `/dev/rdisk5s1`，不是 `/dev/rdisk5+`。`write_research_sd.py` 直接硬编码 `/dev/rdisk5s1`（`write_research_sd.py:30`）。
- 校验点：`sd_card.py:57-58` 只检查第一分区 boot `Content` 等于 `BOOT_TYPE`，不是第二分区内容类型。身份/UUID/容量/GPT CRC/备份在 `sd_card.py:40-113`；写入要求恰为 32 MiB 并在 `write_and_verify` 校验（`:135-146`）。`write_research_sd.py` 无 `--disk`，硬编码 `/dev/disk5`（`:11`）；每次必须先 `diskutil list` 确认整卡编号，不能沿用历史编号。
- 快慢行为：`write_research_sd.py` 用 `verify=False` 跳过回读，且无 payload SHA 分支，写后 `/bin/sync` 再 `diskutil eject`，状态固定 `written_not_readback_verified`（`write_research_sd.py:34-40`）；`sd_card.py` 默认完整 32 MiB 回读、重开设备再读、比较头尾保护区（`sd_card.py:135-146`, `:304-315`），慢但校验更强。当前速度偏好下，旧指南额外 hash/readback/先 plan/独立验收不作为本轮上板前置；脚本已有身份校验、退出码、首错停止保留，不删除。
- cpn2：`receipt.json:7` 的 kernel 是 `cpn2-net1`，这是软件更新，不要求新 cpn2 BIT；拟用既有兼容 BIT，或等待原四模式构建任务交付。仅凭本批文件存在性检查不能判定 Dell 构建状态。
- 最终 SD 状态：以 receipt 为准，已写/同步/eject/exit 0，但板端 cpn2 启动与 SD/网口仍无本次证据（`receipt.json:15-20`）。本轮审查不再次写卡或切 BIT。

### 3. 串口先采集 → JTAG 一次不写 flash → 关闭临时采集后再 attach

- 先串口采集：串口须先开启，再启动/复位板卡，才能捕获早期启动（`mac-snapshot/README.md:178`）；composition 流程也要求先做串口采集，再加载 BIT，等 root shell 后关闭临时串口采集再启动 runner（`iommu-platform/docs/composition-v1-mac-20261004.md:59-67`）。
- 串口参数/互斥：`fpga.py console` 固定 115200 8N1、无流控（`mac-snapshot/scripts/fpga.py:304-313`），用 `TIOCEXCL` 做内核级独占（`:331`），`Ctrl+]` 退出（`:361-366`）。`mac_update_reboot.py` 的唯一 token 是 shell 活跃/`sync` 确认标记，不是串口锁；本文件未见 `lsof` 或 `TIOCEXCL`（`iommu-platform/scripts/mac_update_reboot.py:8-12`, `:180-191`）。运行前释放其他 UART 占用即可，本次无需改它。
- JTAG 一次易失加载：`fpga.py program` 只构造 `openFPGALoader -b genesys2 [--freq] <bit>`，不加 `-f`（`fpga.py:252-255`）；不加 `-f` 表示不写 Flash。`program --profile` 默认历史 `migration-4dma`，未知 profile 运行时报错，必须显式选（`:185-243`, `:416-421`）。`--freq` 只允许 1000000/6000000，默认 1000000（`:419-420`）。实际依据：`README.md:191` 记录 6 MHz 首次 DONE 未就绪、1 MHz 重试恢复并启动；默认 1 MHz 有实测依据，不要为速度擅调 6 MHz 或自动重试。
- 新四模式 BIT 不经 `fpga.py`：直接 `openFPGALoader -b genesys2 --freq 1000000 <bit>`（`composition-v1-mac-20261004.md:61-65`）。
- 外置 JTAG 重启 helper：`mac_update_reboot.py` 默认 dry-run，无 `--run` 不开串口；`--run` 必须给 `--port/--bit/--expected-kernel/--out`，不复用自动端口/BIT（`mac_update_reboot.py:21-25`, `:333-381`）。它先确认 shell alive 并读 pre `uname -r`/`boot_id`，再只用 `openFPGALoader` SRAM 加载同一 BIT，等 shell 后要求 post `uname -r` 等于 expected 且 `boot_id` 改变；任何错误停止，不重试/不 resume（`:8-19`, `:199-207`, `:237-310`）。
- attach runner：关闭临时采集后再启动，避免串口同时占用（`composition-v1-mac-20261004.md:67`）；runner 只 attach，不写 BIT、不写 SD、不重启（`:88`）。`--through-phase` 默认 `short` 只 16；要全 172 必须 `explore`，不要先 `short` 再全量；`--out` 必须不存在；失败写 `STOPPED.txt` 停止，不自动重跑（`:82-86`）。仅用户授权实验才运行，不默认自动加 16/172 测试。

### 4. raw 完整结果回 Dell / 交接，以及以太网扩展

- raw 完整结果（含失败）回 Dell，不覆盖。composition 目的地为 `out/mac-composition-v1-20261004-01/`，跑完 Linux 保持运行，不关机/不重启/不重载 BIT/payload（`composition-v1-mac-20261004.md:90-98`）。其他轮次按现有交接惯例回传并更新 Dell 交接；`mac-snapshot/MAC_FPGA_HANDOFF.md:27`, `:33`, `:45`, `:49` 等记录了原始目录回 Dell 并生成回执的历史。
- 以太网后续闭环：`mac_update_reboot.py` docstring 明确用于 cpn2-net1 软件更新后的外置 JTAG 重启（`iommu-platform/scripts/mac_update_reboot.py:3-19`）；原审查引用的板端 `platform-sd-update`/`platform-net-up`/`board-fetch-update` 属于已实现软件路径（`deepseek-workflow-review.md:161-175`），但本批实文件与 receipt 没有板端 SD/网口启用证据（`receipt.json:20`）。因此列为“已实现软件、待板端启用/验收”的扩展路径，不作为本轮已通过能力。

### 5. 本批文件已证明的调用/快慢清单

| 入口 | 已证明调用/行为 | 快慢/默认 |
|---|---|---|
| `scripts/fpga.py check` | 本地工具 + 固定 BIT 检查，不访问 JTAG（`fpga.py:89-118`） | 离线；不证明板卡 |
| `scripts/fpga.py fetch` | `ssh tar` 取固定 BIT + 参考资料，`verify_bit` 校验固定 SHA/大小（`:121-171`） | 仅 BIT/资料，不取新软件 |
| `scripts/fpga.py program` | `openFPGALoader -b genesys2 [--freq] <bit>`，无 `-f`；默认 profile `migration-4dma`（`:252-255`, `:416-421`） | 默认 1 MHz；6 MHz 曾 DONE 未就绪，1 MHz 恢复（`README.md:191`） |
| `scripts/fpga.py console` | 115200 8N1 无流控，`TIOCEXCL` 独占，原始日志，`Ctrl+]` 退出（`:304-384`） | 先开采集再复位/启动 |
| `scripts/sd_card.py update` | 先验身份/GPT/备份，只打开 `raw_disk+"s1"`；默认完整回读；skip 仅白名单 release（`:40-113`, `:135-146`, `:149-153`, `:268`, `:282`, `:304-315`） | 默认慢且强校验；不 eject |
| `scripts/write_research_sd.py` | 硬编码 `/dev/disk5`、`/dev/rdisk5s1`；`verify=False`；sync + eject；状态 `written_not_readback_verified`（`:11-40`） | 比 `sd_card.py` 少回读/hash，更快 |
| `iommu-platform/scripts/mac_update_reboot.py` | 默认 dry-run；`--run` 需 `--port/--bit/--expected-kernel/--out`；token 确认 shell；SRAM 加载同一 BIT；post `uname -r`/`boot_id` 检查；错误即停（`:21-32`, `:199-207`, `:237-310`, `:333-381`） | dry-run 不碰串口/JTAG；不自选端口/BIT |
| `tools/composition-v1-20261004/mac_composition.py` | attach-only；只读 `lsof`；需 `--expected-kernel`；默认 `short` 16，`explore` 172；`--out` 不存在；失败写 `STOPPED.txt`（`composition-v1-mac-20261004.md:71-88`） | 默认只跑 16；全 172 更慢，需显式 `explore` |
```

## (2) 上一份回复应做的具体原文替换列表

按原回复 `iommu-platform/out/deployment-workflow-review-20261005/deepseek-workflow-review.md` 行号列出。未列出的内容不必重抄。

1. **`:28-29` 旧固定 fetch / 最新多版本 fetch**
   - 原文：旧固定 fetch 与最新多版本 fetch 的并列描述。
   - 替换/追加：  
     `旧固定 fetch 只用于固定 BIT/参考资料（fpga.py:121-171），不要用于新 release 软件；新 software-update-20261004 的 payload 已在 artifacts/software/software-update-20261004/delivery/fw_payload.bin（receipt.json:5；write_research_sd.py:15-17）。四模式 tools 若交付，走 tools/composition-v1-20261004/（composition-v1-mac-20261004.md:5-21）；只读存在性检查见 mac-path-availability.json:10-17。`

2. **`:42` 只写第一分区**
   - 原文：`**只写第一分区**：`/dev/rdisk5+` 打开分区字符设备，`:280-284`；plan 固定 `"partition2_written": False`，`:257`。`
   - 替换：  
     `**只写第一分区**：sd_card.py 由 --disk 生成 raw_disk = args.disk.replace("/dev/disk","/dev/rdisk")，再打开 raw_disk + "s1"（:268, :280-284）；登记卡例是 /dev/rdisk5s1，不是 /dev/rdisk5+。plan 固定 "partition2_written": False（:257）。write_research_sd.py 直接硬编码 /dev/rdisk5s1（:30）。`

3. **`:43` 分区校验对象**
   - 原文：`第二分区内容类型（:57-58）`。
   - 替换：`第一分区 boot Content 类型必须等于 BOOT_TYPE（:57-58）；不是第二分区内容类型。`

4. **`:81` BIT 字节数错字**
   - 原文：`14,443,722 B`。
   - 替换：`11,443,722 B（fpga.py:31 BIT_BYTES；MAC_FPGA_HANDOFF.md:63；mac-snapshot/README.md:204）。`

5. **`:84` 1 MHz 默认依据**
   - 原文：`--freq 只有 1000000, 6000000，默认 1000000（:419-420）。`
   - 替换：  
     `--freq 只有 1000000, 6000000，默认 1000000（:419-420）。依据：README.md:191 记录 6 MHz 首次 DONE 未就绪、1 MHz 重试恢复并启动；默认 1 MHz 有实测依据，不要为速度擅调 6 MHz 或自动重试。`

6. **`:130` mac_update_reboot 串口互斥**
   - 原文：`mac_update_reboot.py：**没有** lsof 检查，也没有 file lock；docstring 声称...但代码未实现独占。`
   - 替换：  
     `mac_update_reboot.py 的唯一 token 是 shell 活跃/sync 确认标记（mac_update_reboot.py:8-12, :180-191），不是串口独占锁；本文件未见 lsof 或 TIOCEXCL。运行前释放其他 UART 占用即可按现有流程做，无需本次修改。`

7. **`:159` 速度偏好结论**
   - 原文：当前速度偏好与“不应删除校验”的结论段。
   - 替换/追加：  
     `本轮只审查，不重写脚本、不执行上板/测试。按最新用户速度偏好，旧指南的额外 hash/readback/先 plan/独立验收不再列为本轮上板前置；脚本已有的身份校验、退出码、首错停止保留，不删除。`

8. **`:175` 最终 SD 状态**
   - 原文：`当前事实：receipt.json 显示 cpn2-net1 已写卡...但 board_boot_verified: false...`
   - 替换/追加：  
     `当前事实：receipt.json:1-21 显示 cpn2-net1 已写卡、synced、ejected、exit_code 0；board_boot_verified:false。板端 cpn2 启动与 SD/网口仍无本次证据。本轮审查不再次写卡或切 BIT。`

9. **`:191` cpn2 BIT**
   - 原文：`cpn2 是**软件任务**...**没有 cpn2 的 BIT 文件、SHA256 或是否已构建的源码事实。**`
   - 替换：  
     `cpn2 是软件更新，不要求新 cpn2 BIT；receipt.json:7 的 kernel 为 6.5.0-rc2-cpn2-net1-...。拟用既有兼容 BIT，或等待原四模式构建任务交付；仅凭本批文件存在性检查不能判定 Dell 构建状态。`

10. **`:192` composition BIT 状态**
    - 原文：`composition-v1 的 BIT 是否完成...无 routed DCP/BIT/MCS...需接手读取实时 state。`
    - 替换：  
      `mac-path-availability.json:10-17 只显示 tools/composition-v1-20261004/ariane_xilinx.bit 与 kernel-release 不存在；只能说明这两个检查位置尚未部署、四模式此路径尚未交付 BIT/kernel-release，不能外推整个 Mac 无其他 BIT 或 Dell 构建失败。`

11. **`:201` Dell/Mac 新旧写反**
    - 原文：`Dell 根 MAC_FPGA_HANDOFF.md ... 是当前版... mac-snapshot/MAC_FPGA_HANDOFF.md 是 2026-09-15 旧版...`
    - 替换：  
      `> 说明：Dell 根 MAC_FPGA_HANDOFF.md:3 仍是 2026-09-15 旧正文；本次在其后补当前入口地图，旧正文保留为历史。Mac 快照 mac-snapshot/MAC_FPGA_HANDOFF.md:3 已从 2026-10-04 N 原型草案审查起更新。Mac 最新 SD 交接仍优先 mac-snapshot/MAC_HANDOFF.md；Dell 侧优先 CURRENT_STATUS.md/HANDOFF.md。`

12. **`:210` payload 大小**
    - 原文：`30 MiB payload + 补零到 32 MiB`。
    - 替换：  
      `payload 29,363,656 bytes（receipt.json:6；composition-v1-mac-20261004.md:10），再补零到 32 MiB（write_research_sd.py:34；sd_card.py:135-146）。`

13. **`:222` “不要复用”条目**
    - 原文：`不要复用：mac-snapshot/MAC_FPGA_HANDOFF.md 为 2026-09-15 旧版；Dell 根 MAC_FPGA_HANDOFF.md 与 mac-snapshot/MAC_HANDOFF.md 才是当前交接。`
    - 替换：  
      `文件优先级：Dell 根 MAC_FPGA_HANDOFF.md:3 为 2026-09-15 旧正文，本次补入口地图并保留旧正文；mac-snapshot/MAC_FPGA_HANDOFF.md:3 已是 2026-10-04 起更新。Mac 最新 SD 交接优先 mac-snapshot/MAC_HANDOFF.md；Dell 侧优先 CURRENT_STATUS.md/HANDOFF.md。`
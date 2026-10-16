# Mac 本地任务交接：Genesys 2 首次上板

## 2026-10-06 23:38 JST 当前：裁剪 BIT 全172项板测通过

继续使用`tools/composition-clean-20261006/ariane_xilinx.bit`＋cpn3-clean，boot_id `05f32544-9e7d-4a2b-bc56-9b5dcabb8138`。Mac `results/composition-clean-20261006-formal01`一次172/172通过（16+12+144），exit0/failed0/data_errors0/taint0/恢复状态0，完整原始目录和UART日志已回传。[验收](iommu-platform/out/composition-clean-20261006-board/board-acceptance.json)。本轮没有重编/写卡/重载/额外预跑，Linux保持运行，UART已释放。无须重复执行；后续按用户新要求推进研究，不能将功能通过等同于性能或创新。下方“未重跑172”是之前软件更新阶段。

## 2026-10-06 23:03 JST 现行版本：裁剪 BIT + cpn3-clean

- Mac当前BIT：`tools/composition-clean-20261006/ariane_xilinx.bit`；当前helper同目录`mac_update_reboot.py`。Linux `6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty`；boot_id `05f32544-9e7d-4a2b-bc56-9b5dcabb8138`。新V9接口须配套本次Linux。
- 已完成Ethernet更新32MiB p1、同步、JTAG新BIT加载及SSH启动确认，SD常驻、Linux运行、UART已释放。[验收](iommu-platform/out/composition-clean-20261006/acceptance.json)。WNS+0.169ns，整板LUT156215。第一次旧系统`wc`软锁死的现场保留，恢复后更新成功；根因未确认。
- 本轮没有重跑172；下方旧ptwfix的172/172只属于旧BIT。旧版本命令保留为历史，后续操作使用[最新入口](iommu-platform/docs/composition-v1-mac-20261004.md)明确指定新BIT/内核/新结果目录，勿重复写卡或加载旧版本。

## 现行入口索引（2026-10-05）

> 当前操作以 [composition-v1-mac-20261004.md 顶部现行入口](iommu-platform/docs/composition-v1-mac-20261004.md) 为准；本节只做索引，下方 03:06 与 2026-09-15 内容保留为历史。

- 当前 Mac BIT：`tools/composition-ptwfix-20261005/ariane_xilinx.bit`；Dell 验收对应 [out/composition-ptwfix-20261005/route-opt/ariane_xilinx.bit](iommu-platform/out/composition-ptwfix-20261005/route-opt/ariane_xilinx.bit)，验收记录 [acceptance.json](iommu-platform/out/composition-ptwfix-20261005/acceptance.json)。验收：WNS +0.031 ns、WHS +0.019 ns、WPWS +0.062 ns，276241/276241 线路布通，native BIT DRC errors 0；完整板级 IO/CDC 签核仍未完成。不要用旧 composition-trim/net3 BIT 代替。
- 当前内核：`6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty`。Mac `192.168.77.1`，FPGA `192.168.77.2`，USB 网卡百兆，JTAG `1000000`；SD 常驻板卡，正常工作走以太网/SSH，UART 仅故障诊断且单进程拥有。
- PN4096：此前失败，本次实际 1/1 实板通过，V/A data errors 0、taint 0；boot_id `2695d848-8589-491e-bf7f-1de7af5ea9cb`。单次 PN4096 仅属本轮排错，不是 172 的一部分，也不作为每次 172 的常规额外预跑；进入正式 172 前关闭 `/sys/module/idma/parameters/composition_trace` 是当前内核要求。
- 完整冻结 172 已在 `results/composition-ptwfix-20261005-formal01` 完成：planned/attempted/completed 均为 172，failed=0，case problems 全空、taint/数据错误/恢复状态均为 0；[板测验收](iommu-platform/out/composition-ptwfix-20261005/board-acceptance.json)。原始记录已回传，UART/JTAG 占用释放，Linux 保持运行；本轮 JTAG 1 次、SD 写 0 次。旧 net3/DS-only 各 52/1/119 保留，不并入新结果；没有性能或创新收益结论。
- 固定流程：正常工作走以太网、SD 留板卡；仅 RTL 改变时，先确认并结束已有 XVC / 其他 JTAG 占用，再 sync + JTAG 重载指定 BIT、不写 SD；只有 Linux 改变时才网传 `fw_payload`（`--payload`），此时 `--expected-kernel` 必须换成该软件实际交付的 `uname -r` 版本，不沿用旧 net4trace，网传后只写 32 MiB 第一原始分区并 sync，再 JTAG 重载已验收 BIT。异常卡死先保存证据，再释放 JTAG 后手动恢复；此前已确认完全卡死的 DS 恢复曾使用免 sync 例外，本次 PTW 上板是正常 sync 模式，不能把免 sync 或手动 openFPGALoader 称为脚本现有 rescue CLI 开关，也不固化为默认流程。
- 一次只执行一遍 172；技术失败保留现场后停止并定位，不自动重跑、禁止自动改 172 计划或阈值、禁止用重试掩盖失败；结束回传原始目录并释放占用而 Linux 保持运行。
- 不加入常规 hash/回读、重复 build、额外预跑或串口抢读；凭据只通过现有 helper，不读/不输出密码或密钥；`--board-host` 不放 `root@`。命令见 composition-v1-mac-20261004.md 顶部现行入口。

## 2026-10-05 03:06 JST：新的四模式BIT已完成构建验收

Dell可用产物：`/home/xvhaocheng/work/iommu-platform/out/composition-trim-20261005/route-opt/ariane_xilinx.bit`（11443722字节），同目录MCS（31472748字节）。[验收记录](iommu-platform/out/composition-trim-20261005/acceptance.json) accepted=true：WNS+0.058ns、WHS+0.052ns、WPWS+0.062ns；三类失败端点0、全部273893线路布通/error0、组合/锁存环0、nativeBIT错误0。原stage的base BIT时序未通过，不得代替该优化产物。

本次优化沿用裁剪后的RTL/50MHz/约束/ABI与Linux/SD，不需要为这次修复重做SD。尚未上传Mac或进行板测；当前板端对话只做Linux/网络加载验收，不自动启动172项。加载/实验由现有板端对话按当前范围安排，沿用[现有Mac入口](iommu-platform/docs/composition-v1-mac-20261004.md)与[冻结规格](iommu-platform/docs/composition-v1-spec-20261004.md)，不新增解包、回读等流程。完整IO/CDC签核仍未完成。

## 2026-10-05 当前拷贝与上板入口（已核对现有源码）

Mac 工作区：`/Users/jojo/Desktop/在mac链接pfga(codex)`。Dell 开发/交付目录：`/home/xvhaocheng/work/iommu-platform`。Dell 当前状态优先读 [CURRENT_STATUS.md](CURRENT_STATUS.md)、[HANDOFF.md](HANDOFF.md)；Mac 最新安装状态读其 `MAC_HANDOFF.md`。本节是当前入口，下方 2026-09-15 首次上板说明保留为历史。

既有顺序：**Dell 编译/交付 → 拷到 Mac → 需要更新软件时写 SD → 先打开串口采集 → JTAG 加载一次指定 BIT → Linux 启动 → 释放临时串口后接入现有实验入口 → 原始结果回 Dell。** 仅执行用户要求的阶段，不默认追加实验。

| 阶段 | 已有入口及实际行为 |
|---|---|
| 拷贝交付 | 旧 `scripts/fpga.py fetch`、`fetch_software.py` 固定历史交付；按版本选择现有 `fetch_*.py` 或 SSH/SCP。当前新版软件已在 Mac `artifacts/software/software-update-20261004/delivery/`，四模式入口在 `tools/composition-v1-20261004/`。 |
| 写 SD | 当前 release 使用 `scripts/write_research_sd.py --release software-update-20261004`，复用 `sd_card.identity/write_and_verify`；只写 32 MiB 第一分区，补零、fsync/sync、安全推出，不做 payload 哈希/写后回读。本次已完成，勿重复写。 |
| JTAG 加载 | 旧 `scripts/fpga.py program --profile ...` 只支持其历史 profile。新四模式沿用指南直接调用 `/opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000 <指定 BIT>`；不加 `-f`，加载易失配置。 |
| 串口 | `scripts/fpga.py console <实际串口>`：115200、8N1、无流控，保存原始日志，`Ctrl+]` 退出；使用 `TIOCEXCL` 独占。先开启采集再加载 BIT，看到 root shell 后关闭临时采集，再交给 runner。 |
| 四模式实验 | `tools/composition-v1-20261004/mac_composition.py` 只 attach；复用同目录 `mac_research2.SerialConsole` 和 `composition_plan.py`，不写卡、不加载 BIT、不重启。显式给目标内核与新的输出目录；仅按用户授权运行实验。 |
| 结果回传 | 现有 `return_*.py`、`transfer_*.py` 和 SSH/SCP 可复用，按对应版本选择，完整保留原始记录及失败现场；回 Dell 独立 `out/` 目录，更新两端交接。实验结束默认保持 Linux 运行。 |

### 需要记住的实际默认值

- `write_research_sd.py` 默认 release 是 `next-experiment-20261002`；必须显式选新版。它硬编码 `/dev/disk5` 与 `/dev/rdisk5s1`，没有 `--disk` 参数；实际写卡前核对设备身份，盘号变化应停止，不绕过身份检查。[源码快照](iommu-platform/out/deployment-workflow-review-20261005/mac-snapshot/scripts/write_research_sd.py)
- `sd_card.py` 默认 `experiment-v1`，其 release 列表目前不含 `software-update-20261004`。它会备份/GPT 检查，默认回读；指定历史版本可用现有 skip 参数，但它本身不做 eject。旧 `run_sd_admin.py`/AppleScript 固定旧软件参数，不能直接当本次新版入口。[源码快照](iommu-platform/out/deployment-workflow-review-20261005/mac-snapshot/scripts/sd_card.py)
- `fpga.py program` 默认历史 `migration-4dma`，不是四模式新版；不得仅凭默认值加载。现有 1 MHz 有 Mac 6 MHz 首次未就绪、1 MHz 恢复的历史记录，不因追求速度擅调频率或自动重试。[源码快照](iommu-platform/out/deployment-workflow-review-20261005/mac-snapshot/scripts/fpga.py)
- `mac_composition.py` 默认只跑 short 16 项；全部 172 用例需 `--through-phase explore`，一次执行包含前序阶段，不先跑 short 再全量。旧指南内核 cpn1 应换成本次实际 `kernel-release`，当前新版为 cpn2-net1。旧 nonblock/method88 专用入口有固定内核/boot 身份，不能直接当新版通用入口。[四模式说明](iommu-platform/docs/composition-v1-mac-20261004.md)
- 串口独占方式并不统一：composition/nonblock 使用占用检查；新版 `mac_update_reboot.py` 的 token 用于 sync/shell 确认，未实现串口占用锁。运行前释放其他 UART 占用，不能把 token 当独占锁。
- 用户最新速度要求优先：不追加下载哈希、解包复验、写后回读、独立验收或重复测试；保留所选程序已有身份核对、退出码和错误停止。旧指南的额外清单不是自动追加的前置任务。

### 当前交付与以太网扩展

**2026-10-05 最新实际状态**：cpn2-net3纯网口更新入口已通过，boot_id 1a955cce-bcdd-4f0a-9012-6e0c7eda8322，Mac192.168.77.1与Linux192.168.77.2均自动/持久配置。现有mac_update_reboot.py新增SSH路径并复用run_research2_mac.py现有登录；从网传、32MiB p1写入同步、旧N v5 BIT JTAG重载到自动联网SSH验身份exit0，重载后无UART配置。日常可拔UART，JTAG保留；科学实验0，另一对话的新composition BIT未在本轮加载。整轮34.45分钟，速度未优化。[固定流程](iommu-platform/docs/software-update-20261004.md) · [原始结果](iommu-platform/out/ethernet-workflow-20261005/ethernet-workflow-20261005-03/result.json)。下方审查为早期快照。

新版 Linux `6.5.0-rc2-cpn2-net1-00093-gc0f5e13ea81f-dirty` 的 payload 为 29363656 字节，SD 已写入/同步/安全推出，exit0。[实际回执](iommu-platform/out/sd-install-20261005/receipt.json)。本次审查未写卡、加载 BIT、打开 UART、重启或测试，板端新版启动与 SD/网口仍未由本次证据确认。

只读核对 Mac 的两个位置 `scripts/mac_update_reboot.py`、`artifacts/software/software-update-20261004/delivery/mac_update_reboot.py` 均尚无文件；四模式 `tools/composition-v1-20261004/ariane_xilinx.bit` 与 `kernel-release` 也尚无文件。[存在性记录](iommu-platform/out/deployment-workflow-review-20261005/mac-path-availability.json)。这不判断 Dell 当前构建状态，也不说明 Mac 没有其他 BIT。

SD 常驻扩展已实现的软件入口为板端 `platform-net-up` → `platform-fetch-update` → `platform-sd-update`，写入同步后再用 [现有 Mac 重启工具](iommu-platform/scripts/mac_update_reboot.py) 重载指定同一 BIT；该工具可从 Dell 交付目录单独拷到 Mac。cpn2 是软件版本，不要求重新造 BIT。网口/板端 SD 可用性与完整闭环仍待实际启用；静态 IP 启动后需重新设置。

DeepSeek 梳理源码、提供流程稿并定向修订；Codex 直接复核写盘/JTAG/UART关键代码、核对 Mac 实际文件位置并保存入口索引。完整原始回复与源码快照保存在 `iommu-platform/out/deployment-workflow-review-20261005/`；原始回复保留，其中未经修订的错误以本节为准。

## 以下为 2026-09-15 首次上板历史说明

更新：2026-09-15。请先读本文件，再按需核查本地环境；不用重新全面扫描服务器工程。

## 1. 用户目标与工作方式

- 用户用 **MacBook M1 Pro** 工作，希望在身边只开这一台电脑，完成 FPGA 下载、串口操作和实验。
- 用户已有 Parallels Desktop，另有 Windows 电脑，但不希望同时使用两台本地电脑。
- 源码、Linux 和 FPGA 编译在实验室 **Linux x86_64 服务器**。服务器任务的工作目录是 `/home/xvhaocheng/work`，平台在 `/home/xvhaocheng/work/iommu-platform`。
- 本文件是交给 **Mac 本地任务**的上下文；首先确认实际执行环境是 macOS/Apple Silicon。不要把服务器的 `/home/xvhaocheng/...` 当成本地路径，也不要在服务器上安装 Mac 工具。
- 简洁中文，先说结论。只读取与当前步骤有关的文件；不要重复完整构建或扫描整个仓库。

## 2. 最新建议及证据边界

**优先试：服务器编译 + Mac 原生 openFPGALoader 下载 `.bit` + Mac 串口操作。** 本地无需先安装完整 Vivado 或虚拟机。

- openFPGALoader 的项目支持表明确列出 `genesys2` / `xc7k325tffg900`，内存加载和 Flash 写入均标 OK。
- Homebrew 提供 Apple Silicon 原生包。
- 这是工具支持依据，**还没有在用户的 Mac/板卡上测试**。不能提前声称 USB、下载、串口或 Linux 启动已通过。
- 如需 Vivado Hardware Manager / ILA 界面，再考虑 Parallels + Windows 11 ARM + Vivado Lab Edition。NUS 有 M1 Max/Parallels/Windows 11 运行 Vivado 并通过新版 Digilent Adept Runtime 连接开发板的记录；并非当前 Genesys 2 + Vivado 2023.1 的实测。
- Linux/Rosetta 也有社区方案，但 USB/XVC 有额外适配要求。不要把“AMD 未官方支持 ARM”解释为“ARM 绝不可能运行”。

## 3. 当前平台与启动方式

- 板卡：**Digilent Genesys 2，Kintex-7 XC7K325T**。
- 平台：RV64 CVA6 + 4 路 iDMA + IOMMU + Linux；CPU 50 MHz，timer 25 MHz，DID 10～13，IOTLB 16。
- `migration-4dma` BIT/MCS 已构建，软件构建和 HPM 定向仿真有通过记录；**尚未上板验收**，时钟/CDC/IO 约束审查待办保留。
- 现有 BIT 是 `migration-4dma`，DS 输入/输出延迟 50/50 cycles。`baseline-4dma` 是 0/0，未构建。不可混为同一性能基线。

区分两个步骤：

1. **配置 FPGA 电路**：JTAG 加载 `.bit`。首次只做易失配置加载，断电后需重新加载；不把 Flash 固化当作首次步骤。
2. **让 CVA6 启动 Linux**：目前沿用 SD 启动。Boot ROM 源码读取 SD 的 GPT 第一分区原始数据到 DDR 后启动，不是普通 FAT 文件拖拽流程。

当前软件交付目录不是已确认布局的 SD 整盘镜像。制卡布局、加载文件和生成 Boot ROM 的一致性仍需服务器端定向核对。不要凭通用教程直接格式化或写卡。

仓库存在 RISC-V Debug Module/OpenOCD/GDB 入口，但完整 Linux 无 SD 启动尚未验证。Vivado 下载 `.bit` 不等于把 Linux 镜像加载并启动。

## 4. 本地任务下一步

1. 确认当前是 Mac 本地环境；定向检查 macOS 版本、Homebrew、openFPGALoader 是否已有安装。无需扫描全盘。
2. 确认板卡供电、数据线与板载 USB-JTAG 接口连接。串口 USB 是单独的连接用途，按板卡标识核对，不能把 JTAG 口默认当 Linux 控制台。USB 留给实际使用的宿主机，避免虚拟机抢占。
3. 若尚未安装且用户要求开始配置，可在 Mac 使用 `brew install openfpgaloader`。
4. 按已安装版本的帮助检查 `genesys2` 支持，并先验证 JTAG 设备识别。遇到失败先查供电、接口、线缆、USB 占用及驱动，不立即重建 FPGA。
5. 从服务器取得下节 `.bit`，校验 SHA256 后下载到 FPGA。命令形式：

   ```sh
   openFPGALoader -b genesys2 /Mac上的实际路径/ariane_xilinx.bit
   ```

   不加 `-f`；此步骤不是写 Flash。
6. 识别实际串口设备并以 **115200、8N1、无流控**连接，保存完整启动日志。没有准备好 SD 时，能配置 FPGA 与能启动 Linux 应分别记录。
7. Linux 软件交付和 SD 镜像准备由服务器侧继续处理；软件到位后再做 smoke、E0 校准。当前唯一实验前端是 `idma_experiment`，不要使用已退役的旧 benchmark。

仍未知：macOS/Parallels 版本、Homebrew 状态、USB/串口枚举、SD 卡/读卡器是否到位、Mac 到服务器的 SSH 主机别名。能本地检查的先检查，只向用户询问确实缺失的信息。不要把历史 host ID 当成可用 SSH 地址。

## 5. 服务器产物与传输核对

现有硬件 BIT（服务器绝对路径，2026-09-15 已确认存在并计算哈希）：

```text
/home/xvhaocheng/work/iommu-platform/out/migration-4dma/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit
大小：11443722 bytes
SHA256：cd49d4aba0195d5176618fb7d03071154bcd5b347425b43f854945149cceed70
```

Mac 下载后用 `shasum -a 256 /实际路径/ariane_xilinx.bit` 核对。SSH 连接信息尚未记录；可复用用户已有连接配置，不虚构主机名，不把私钥或凭据写入交接。

服务器的软件交付目录：`/home/xvhaocheng/work/iommu-platform/out/experiment-v1/`。

**该软件交付尚未包含已提交的小 chunk 校验修复和新增 smoke。** 服务器侧需要重建并更新交付，不能把现有文件当最新版直接制作正式实验卡。此限制不妨碍先验证 Mac 的 FPGA 下载连接。

如需服务器侧证据，仅定向查看：

- `/home/xvhaocheng/work/HANDOFF.md`：总交接。
- `/home/xvhaocheng/work/iommu-platform/docs/board-host-setup.md`：方案比较与依据。
- `/home/xvhaocheng/work/iommu-platform/docs/validation.md`：构建、仿真与待验收边界。
- `/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/bootrom/src/main.c`、同目录 `gpt.c`：SD 启动代码。
- `/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/ariane.cfg`：RISC-V 调试入口，非已验证的无 SD Linux 运行器。

## 6. 已查资料

- [openFPGALoader 板卡支持表](https://trabucayre.github.io/openFPGALoader/compatibility/board.html)
- [Homebrew Apple Silicon 包](https://formulae.brew.sh/formula/openfpgaloader)
- [openFPGALoader 下载命令](https://trabucayre.github.io/openFPGALoader/guide/first-steps.html)
- [NUS：Parallels 上运行 Vivado 的实测说明](https://nus-cg3207.github.io/labs/getting_started/vivado_install_guide/)
- [Digilent 论坛：其他板卡的 Apple Silicon 实测](https://forum.digilent.com/topic/32526-report-on-vivado-working-on-arm-macos-to-program-basys-3/)
- [Linux/Rosetta 社区方案](https://github.com/ichi4096/vivado-on-silicon-mac)

完成本地实质工作后，在本地更新交接，记录工具版本、BIT 哈希、USB/JTAG/串口结果和日志位置；明确区分“下载成功”“Linux 启动”“smoke 通过”“HPM 校准通过”。

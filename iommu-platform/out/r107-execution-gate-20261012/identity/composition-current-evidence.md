# Composition-isolated 工件补核

结论：**可以有条件复用已优化 route-opt 的构建身份；不能复用原 manifest BIT 的时序结论来冒充优化版本。** 当前板端/live、普通R107 adapter及IO/CDC完整验收由root继续裁决；本席未操作设备。

| 工件 | SHA256 | 原始时序 | 判断 |
|---|---|---|---|
| 原 `fpga-src/.../ariane_xilinx.bit` | `f15f8a72c02da3f19e4ed5beafe3bae61407636b99505d9ace31bf0f9716715a` | WNS−0.219ns；setup1 endpoint失败 | 不能当时序通过版本 |
| 优化 `route-opt/ariane_xilinx.bit` | `f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca` | WNS+0.022ns；WHS+0.051ns；setup/hold/pulse失败0 | 可复用的候选构建身份 |

两个BIT均11,443,722B，所以文件大小不能区分。优化BIT与 `mac-package.tar` 中 `ariane_xilinx.bit` 精确同hash；`execution-state.json:39–72` 和 `board-acceptance.json:14–22` 指向同一次route-opt与历史交付。`optimize.tcl:140–157,206` 显示打开既有routed DCP、物理优化、写新报告后写BIT，未改RTL重新综合。本席没有重跑该流程。

冻结输入：`fpga-src/build-inputs.json` 与manifest的build_start相同；lock/profile哈希匹配。24项overlay中23项直接匹配，top仅builder将LegacyObserver1→0、Mode0→3，逆向归一后hash精确匹配。base IOMMU锁仍为238a39e；不是当前57eb562源码或stage106私有credit修复的构建身份。

静态模式/时钟：staged `ariane_xilinx.sv:172` 为build mode3；DMA缓冲 `ariane_peripherals_xilinx.sv:1269` 为512/512/3/3；IOTLB16、DS50/50。runtime S/N/P/PN为0/1/2/3，reset S0见 `dma_composition_control.sv:74–77,190`。ordinary S0关闭nonblock与partition flags，见peripherals `:1446–1448`；buildmode3不能当成当前runtimePN3。本席未读取当前mode寄存器。

优化原始报告：`route-opt/reports/timing_summary_routed.rpt:159` 支持+0.022/+0.051/+0.062ns；`:174` CPU50MHz/20ns；route_status `:8–10` 为263406/263406完全布线、0 errors。时序报告SHA256 `a9366b911444e6b7407e44b8aaffa391deea7d7e55673999874f60e796f547ac`。

既有约束风险与功能记录分列：优化报告 `:45–48` 仍有6条TIMING-4/6/7/27关键警告；独立check_timing `:22–27` 仍有21 input/23 output缺delay、3 partial input，涉及DDR、Ethernet、UART、SPI等；`board-acceptance.json:117` 明确不声称完整IO/CDC signoff。内部时序通过和仅0.022ns余量不能填掉这些缺口。另一方面历史 `board-acceptance.json:2–11` 记录22/22、failed0，formal boot `boot.json:2,38` 记录boot `d61e7517-2b15-4702-86f2-3f338ddd5345` 与cpn3-clean内核。这是已发生的有限功能记录，不能当作当前身份或完整signoff，也不因约束缺口而抹去。

当前本地Image/DTB与本轮此前核验及composition manifest `:29–30` 精确同hash（Image8c19da27…/24,131,072B；DTBaa9cdf4c…/3510B）。未补证dirty kernel源码/config的完整冻结；也未把历史boot当当前Dell正在运行此组合。

产物：`composition-current-evidence.json` 是完整数字、hash/mtime和overlay表；`composition-current-gate-result.json` 是条件与前提状态；`composition-current-raw-extracts.json/txt` 保留原始路径、行号、全文hash与关键原文；`composition-current-execution-ledger.json` 记录零build/test/设备操作。

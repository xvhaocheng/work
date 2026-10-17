# E6：角色互换的现有基线运行包

## Material Passport

- 类型：只读能力核查＋离线验证过的采集准备；2026-10-17。
- 状态：**准备完成，尚未上板**。本包仅由 Root 在用户安排的 E6 顺序执行。
- 源码：统一 baseline100 的 CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069`、IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`、Linux `45bf76b15e36cf608dff95013d91f8bedb7b1b8e`；实际镜像内核 `6.5.0-rc2-ispass2027-pathv3`。
- 接口：现有 `idma_experiment --batch`，无驱动、内核、RTL变更。复用 [baseline100 来源](../../board/packages/baseline100-20261017/source.json)及本次实际 load receipt。
- 模型：Codex-only；未调用 DeepSeek；A3 未访问 FPGA/JTAG/UART。

V=DMA1/DID11，A=DMA0/DID10。**本轮仅DS0**。Solo、A1、A8 各10次，总30点；10个区组内轮转三条件。V源/目的各1页，A源/目的各1或8页；4KiB COPY，4096次，预热256。无额外短测或独立重启；在 Root 安排的DS0阶段复用有效配置。采集器仅接受`--ds 0/0`，并将既有load receipt的`bit_sha256`与已知DS0 `e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6`比较，拒绝DS50误标；不重新hash硬件产物。

## 为什么可直接运行

以下为**实际冻结构建输入**，前缀分别为：

- `SW = /home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-software01/`
- `HW = /home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/`

`SW/src/linux/drivers/misc/idma.c:702–709` 把DMA0当观测寄存器控制入口；`718–734` 明确接受V/A任意0..3，校验DID=`10+id`，在ARM里动态编码角色。并未要求V=DMA0。`SW/inputs/bench/idma_experiment.c:25–33` 支持设备和干扰源参数；`417` 以实际打开设备的caps.did核对victim_did。

`HW/src/cva6/corev_apu/fpga/src/dma_observer.sv:255–266` 接受动态V/A端口；`dma_path_observer.sv:123–142` 以角色端口及mux后ID做事件匹配。`ariane_peripherals_xilinx.sv:924–946` 将所有端口事件接入observer，`1030` 仅让DMA0提供共享观测MMIO，不限制V。源文件范围/行号快照见 [source-evidence.json](source-evidence.json)。

## Root 运行命令

独立采集器不增加旧 `collect.py` 的 phase。先把本分支合入 Root 工作树，使 `../../board/uart.py` 可导入。仅打印命令可用 `--plan-only`，不打开UART。

```sh
python3 paper/ispass2027/exp/E6-mitigation/collect_role_swap.py \
  --output NEW_E6_RUN_DIR \
  --load-manifest CURRENT_VALID_LOAD_RECEIPT \
  --source-record paper/ispass2027/board/packages/baseline100-20261017/source.json \
  --expected-kernel 6.5.0-rc2-ispass2027-pathv3 \
  --ds 0/0
```

`NEW_E6_RUN_DIR`和`CURRENT_VALID_LOAD_RECEIPT`由 Root 的DS0阶段配置替换。复用上板skill的串口独占和既有receipt，不增加硬件hash/SD回读步骤。必须以普通Python运行；采集器明确拒绝Python `-O`，以保留原有断言检查。

保留CLI原JSON、完整UART、同sequence的3条batch和16条PATH。保留原检查：内核/boot/taint、CLI状态、双方data_errors/quarantined、submitted=completed、V4096次及字节数、观测状态、窗口H+gap、固定前端cadence、AXI守恒与重叠、全部PATH计数/边界守恒。新增角色、DID、A页数与条件核对。非零错误立即停止并保留输出；没有性能筛选或自动补采。若仅主机解析失败，`--recover-run OLD_RUN_DIR` 会先解析原始已完成前缀，只执行剩余计划点；无法完整解析或板端执行失败的命令会拒绝恢复，不以新样本替换。

每个区组的内置命令等价于：

```sh
platform-timeout 120 idma_experiment --batch --device /dev/idma1 \
  --aggressor 0 --aggressor-pages 8 --victim-pages 1 --chunk 4096 \
  --transfers 4096 --warmup 256 --timeout-ms 60000
```

Solo将aggressor改为none、A页数填1；A1保持aggressor0、A页数填1。采集器自行带上身份/水位/日志封装。

## 结果口径及验收

`records.json`保留原CLI双方完成进度、AXI窗口和PATH全字段；`review.json`仅对每试次硬件窗口、软件时间、miss、PATH `sum/full`及观察到的max给跨试次中位/最小/最大。`full=0`的mean/max均为null。A的CLI完成数含启动和末尾排空，不能当V窗口内完整A COPY数；A窗口AXI流量另从observer保留，不能混用。无请求分位数。

离线检查见 [host-validation.json](host-validation.json)：用真实原始baseline UART作为输入，显式构造合成角色/输入突变，仅验证解析协议；它们不是角色互换板测。检查包括正确角色可接受、原V0及错误DID/A页数/A身份拒绝、数据错误、排空失败、taint、缺PATH和过期sequence拒绝，以及30点计划。上板是否成功仍以Root新原始日志为准。

P/N/PN/P4M统一重测的必要变化另列 [change-request.md](change-request.md)，本包不构建这些版本。

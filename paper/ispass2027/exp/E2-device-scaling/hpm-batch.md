# E2 HPM第一批交付

## Material Passport

- 状态：Root直接审查接受；独立Linux提交完成，主checkout未切换；无FPGA/JTAG/UART操作。
- 基线：Linux `45bf76b15e36cf608dff95013d91f8bedb7b1b8e`。
- 新Linux commit：`968bec65dbf9fb0d85585574134be9597a6d24c6`，branch `ispass/a3-capabilities`，worktree `/home/xvhaocheng/work-a3-capabilities-linux`。
- 修改仅`drivers/iommu/riscv/iommu-hpm.c`和`include/linux/riscv_iommu_hpm.h`；旧idma.c和UAPI未改。
- Codex实现、直接审查及执行验证；未使用DeepSeek。

新增`riscv_iommu_hpm_acquire_multi()`/`read_multi()`、最多4设备的sample数组，按设备输入次序使用每DID三个event槽，最多0..11。共用session/helper动态记录owned计数；原pair sample[2]和ABI完整保留，旧Solo仍保存/验证6槽，仅启用3槽。multi的未用输出数组元素清零。

所有设备须DID唯一、位于同一IOMMU，调用者保持设备绑定且同task持有session。保存全owned selector/counter和inhibit后逐槽验证/清零/启用；任何中途失败都恢复全部保存槽，释放HPM锁；不触碰其他槽，不清cycle计数。read/release检查owner，错用mode/owner会告警且不执行MMIO；错误task不能替真正owner释放锁，所有者必须按合同调用release。HPM快照仍非原子，不等于V硬件window，也不证明板端event校准。

## 可直接审读和恢复

- [完整单提交patch](hpm-multi.patch)。
- [相对baseline最小增量bundle](linux-hpm-from-baseline.bundle)：仅含新提交，前提commit为45bf；`git bundle verify`通过。不向没有用户origin的上游推送。
- [源码/对象编译回执](hpm-kernel-validation.json)：实际RISC-V `iommu-hpm.o`和未改的`idma.o`编译退出0，源文件/config/object/log hash均记录。对象和build目录留本机且不进Git。
- [最终host回执](hpm-host-final/validation.json)、[真实函数测试原始日志](hpm-host-final/run.log)、[禁用配置头文件原始日志](hpm-host-final/disabled-run.log)。
- 首轮通过回执保存在`hpm-host/`；最终只做声明/行宽整理与禁用配置补验后以新目录保留最终回执，不覆盖首轮。

测试用生产C/header去除include后的原始函数体，不重写实现。ASan/UBSan下68场景迭代覆盖legacy pair/Solo、multi1..4、未用数组清零、未owned槽不写、重复DID/跨IOMMU/缺设备/忙/分配失败/无HPM，逐slot0..11的缺slot、DID过滤丢失、清零失败、冻结/启用失败及完整rollback；也覆盖输入顺序、错owner/mode、high-low-high翻转和NULL读/释放。额外编译运行CONFIG-disabled头文件stub，并核对旧sample大小56字节。host MMIO不模拟实际event语义，不声称板测或RTL通过。

```sh
python3 paper/ispass2027/exp/E2-device-scaling/check_hpm_host.py \
  --linux /home/xvhaocheng/work-a3-capabilities-linux \
  --output NEW_HOST_OUTPUT
```

最终源码的`checkpatch.pl --strict --file`为0 error/0 warning/0 check。实际对象编译命令及完整输出另见回执与`hpm-kernel-objects-final.log`；只编译对象，没有生成/加载新image或BIT。

下一批从A2已稳定P0接口与本HPM提交整合的Linux起步，再按 [E2方案](capability-and-plan.md)做multi ABI/driver。当前HPM批没有启动多A DMA，没有新增E2板测结果。

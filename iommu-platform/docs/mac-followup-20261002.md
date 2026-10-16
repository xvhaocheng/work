# Mac 交接稿：2026-10-02 FPGA 上板后续

## 一、范围与角色
本稿记录 Dell 构建/分析主机与 Mac 上板主机之间的当前交接状态，以及下一轮可执行的 Mac 动作和边界。Dell 负责源码、内核/驱动/加载器编译、离线分析和交付暂存；FPGA 板始终在 Mac 上，Dell 不接板，也不把板子移到 Dell 执行。Mac 负责 FPGA、SD 卡、UART 控制台和 JTAG。新版软件已在 Dell 编译和本机验证通过；尚未传到 Mac，也未执行新版板测。

## 二、当前状态：保留，不重做
- 现有 Linux 构建 00090 配合原始研究 BIT 的 84 例结果仍为 80 success、2 fail、2 unrun。原 80 个成功用例保留原样，不重跑、不编辑；全局 failed/incomplete 状态保持，不改成 complete。E2 不需要重跑。
- 已经执行过很多实验。当前要确认的是“此刻没有实验正在运行”，而不是说从未执行过实验。
- 保留当前原始结果、现有版本与哈希记录即可。为避免无谓操作，无需完整 dump SD 存档，无需重烧 BIT，也不引入新的批准流程。
- Dell 已修复 Solo 参数适配和离线分析，并完成软件编译；本轮暂不裁 Linux，暂不改 bootrom、SD 卡或 RTL。

## 三、冻结的 11 项新计划
新计划使用新软件版本，硬件 BIT 沿用。步骤冻结如下：
1. 先做 2 个短验收：Solo 与 A8g0。
2. 验收通过后，沿原主矩阵的相对顺序，选 Solo、A8g0、A8g2000 各 3 次。

以上共 2 + 9 = 11 项。每次搬运 4 KiB：主实验完成 4096 次、预热 256 次；短验收完成 64 次、预热 8 次。新模式为 platform-research cost-followup，路径 /tmp/research-followup-01。交付包已通过本机验证和编译，内核为 6.5.0-rc2-00091-g9ad05d00cca7；执行命令和校验值见 delivery/RUN_FIRST.txt 与 DELIVERY.json。执行纪律：任何错误都停止并保留现场，不自动重跑，不自动删数据，也不与旧 nonSolo 结果拼接 slowdown。

## 四、传输与 UART 预算
- 当前 payload 约 28 MiB，即 29363656 bytes。现有 bootrom 会读取首个 GPT 分区的全部 32 MiB；只缩小文件、保持分区不变，读取量不会减少。
- 在 115200、假定 8N1 下，传输该 payload 的理论下限约为 42.5 分钟。该 42.5 分钟对应的是传 28 MiB payload 的理论下限，不是读 32 MiB，也不是最坏值；实际窗口应按部署流程和额外开销安排。
- 缺少 receiver 的是 FPGA bootrom 一侧，不是 Mac 一侧。不要把该理论下限写成 Mac 侧最坏情况。

## 五、JTAG
JTAG 可以探测连接，但速度未经验证，经 JTAG 的可启动路径也未验证。建议安排一个独立维护窗口做后续验证，不要求用户再次审批。当前不演示完整 load command，因为没有测过。任何人都不应在正在运行的 Linux 上 halt、reset 或写内存来打断系统；这些操作只应放在独立维护窗口中。

## 六、Mac 当前可做
1. 确认当前没有实验在运行，并记录 USB 串口、JTAG 设备以及 OpenOCD 是否安装且可用。
2. 不 halt、不 reset、不写内存打断 Linux。完成第 1 步不能证明 JTAG 已经可以更快启动。
3. 下一次按原 SD 部署流程安装新 payload。已经正确运行的 BIT 无需重复配置；若断电导致配置丢失，按既有方式恢复同一 BIT。
4. 不必完整 dump SD 存档，也不必重烧 BIT；保留当前原始结果与现有版本/哈希记录即可。

## 七、Dell 交付路径与传递方式
Dell 交付使用绝对路径：
/home/xvhaocheng/work/iommu-platform/out/research-followup-20261002/delivery

本任务不能直接发送到 Mac 线程，因此请用户让 Mac 上板任务读取该路径。后续具体产物状态以最终 delivery/RUN_FIRST.txt 和 DELIVERY.json 为准；已完成本机验证和编译，尚未发送或上板；主机测试不能替代板卡验收。

## 八、本轮不做
不裁 Linux；不改 bootrom、SD 卡或 RTL；不启动完整 84 例矩阵；不做 PPT；不把旧 nonSolo 与新结果拼接 slowdown；不因本稿引入新的用户审批要求。

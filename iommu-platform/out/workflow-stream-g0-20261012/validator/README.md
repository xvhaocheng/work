# 独立离线验收

作者与执行者：Codex（本席 gpt-6.1-sol）。只读取已经完成的结果，绝不启动 DMA 模型，也不改原始证据。

首次对完整性案例执行：

```bash
python3 /home/xvhaocheng/work/iommu-platform/out/workflow-stream-g0-20261012/validator/validate_stream.py /home/xvhaocheng/work/iommu-platform/out/workflow-stream-g0-20261012/runs/integrity --self-test
```

完整应用采用同一入口，把目录换成对应 case，省略 `--self-test`。脚本默认写 `validator/<目录名>-acceptance.json`，每次实际调用追加 `validator/execution.json`。`--output`、`--ledger` 的目标必须仍在这个 validator 目录；验收失败退出 1。源码语法解析不启动模型。

验收直接使用冻结 `contract.json` 与原始 CSV/二进制，结果自报 `pass` 只是必须存在的运行时标记。独立检查包括：

- 65536 个小端 uint32 输出。完整案例从 A 与 B=2I 推导 C=2n；完整性案例用不同的稠密非对称 A、B 重算完整 256 项内积，检查第一条带前 32 列、其余零及未输出条带的 sentinel。
- 全部 A/B/C 任务的 ready、SUBMIT、ISSUE、DONE，实际 IOVA/物理范围、单引擎序列、固定选序、完整双方结束和后台尾部；同一个 V 完成窗口内后台完成的有效字节。
- 六个槽的代次与依赖：A 被本条带全部 tile 读完才覆盖，B 每 tile 重载且旧 tile 完成才覆盖，C 全条带完成才能输出，旧输出真实 DONE 才允许下一代计算。
- 每 tile 65536 MAC、131072 次读和 256 次写的预算。完整案例独立检查任务/事件/预算/输出，实际逐边执行另外依赖已审查驱动的运行时检查。
- 每个搬运的 AR/AW 范围恰好覆盖预期物理区间，不允许洞或重叠；AW→WLAST 全局 FIFO，B、AR→RLAST 各 ID FIFO；实际 W strobed 字节和 R beats、返回错误与真实任务 DONE 守恒。允许不同 R ID 的合法重排。
- 原始 TL_REQ/TL_RESP 的全程非负余额与最终守恒，LOOKUP、hit 和 DS AR→RLAST 延迟；最终 QUIET 的真实 3/0/0 观察，加上可见 AXI、TL 队列排空。额外隐藏状态排空、批次完成寄存器仍依赖驱动检查。
- 仅完整性案例：逐条 RAM grant/response 重建 96KiB 内存，核一次总 RW grant/clock、下一 edge 响应、暂停只禁止新 grant、DMA 写入的源数据、每一计算读的物理槽和 32 位半字、乘法与模累加、跨行 C32 strobes、最后写响应后才能完成。允许 tile DONE 时存在无关 DMA token。

`--self-test` 在内存中篡改输出值、删除一个 raw COPY_DONE、提前同槽覆盖或 C 输出、篡改 W 字节数；完整性案例另篡改一个 MAC 累加值与一个 RAM 返回值。每个负例必须拒绝，原始文件不改。负例不启动新实验。

验收边界：CSV 没有所有 AXI beat 数据及 VALID/READY 逐周期采样；逐 beat 数据、稳定性、源不变和后台最终内容由驱动实际检查。R/B stall 数取实际运行计数，并分别报告是否非零；没有 stall 时不能称为覆盖了该反压路径，有计数也不能称为 CSV 独立复算了稳定性。没有逐周期 request-valid，就不能从 grant 轨迹独立裁决公平性、未授权等待或端口争用成因。完整应用没有 RAM/MAC 全轨迹，不能把小型回放说成完整应用逐边回放。

已完成的小案例 `runs/integrity-capacity-fixed` 一次离线验收通过，六个内存篡改均拒绝，实际命令与 3.485503840 秒耗时见 `execution.json`。该案例只触及 A0/B0/B1/C0 的第 0 代；模型配置六槽不代表小案例已测试六槽长期复用。六槽跨代次安全必须由完整应用的真实任务和物理事务确认。其实际运行时反压计数：DDR R=208、B=0，SPM R=0、B=0；尚无 DDR B 或 SPM R/B 反压覆盖。

时刻是同一模型时钟的实际事件；不是 host_ns、原 HERO 八核软件、CVA6 指令或 FPGA 上板时间。数据/PTE/RAM 初始准备在 t0 前，配置、供给、命令与排空在 t0 后计入。初始化的真实 CPU 时间和分配成本没有建模，不能称为测得零成本。通过只表示这个固定模型的数据与可见执行合同可接受，不预设收益或主研究问题的阴性结论。

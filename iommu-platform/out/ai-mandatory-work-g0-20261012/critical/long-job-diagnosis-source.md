# stage106 长整张量任务：实际 RTL 定向诊断

本席直接读取冻结模型使用的 iDMA/IOMMU RTL；没有构建、运行或修改原 whole driver、Tb、RTL、任务和守卫。失败快照由 root 执行。逐文件原始/冻结 SHA256 及快照事实见 `long-job-diagnosis-source.json`，13 个关键源码与实际 diagnostic source-freeze 均逐字节相同。

**已证事实：** 原 4k-solo0 在同一停滞状态保持 payload RVALID=1/RREADY=0，后排有已经接受的 PTW R；DDR WREADY=1，但 WVALID=0。可确认全局 ordered-R 队头阻住了 PTW 返回，以及 W 排出没有继续。**W 被关闭的具体内部原因仍待一个内部计数器快照确认，不能只凭外部状态声称全部因果链已证。**

## 实际停滞与观测边界

root 实际执行的 `diagnostic/4k-solo0/run.log:1–14`：

- cycle 203152，channel0 最后真实进展3151，channel1未提供任何工作。
- held payload ID32，PA0x84001000，beat27/len255；下一 payload ID32 PA0x84001800；再后 PTW ID48 PA0x20008。
- TL accept13/response12，PTW active1，只有一个 bank in-flight；没有 translation response/error 返回，也没有硬件 DMA DONE。
- DDR 已接受7个 AW，首地址0x84a00000，仅3个 W beats（24B），后6个各0 beats；WVALID=0/WREADY=1。
- payload R已返回4312B，payload W已发出24B，净在途数据4288B。主 iDMA FIFO 是4096B；额外192B留在已有中间 FIFO、切片和数据路径。**4288B不是主 FIFO 的容量，不能据此把硬件容量改为4288B，也没有逐级测量这192B的所在位置。**

root 已直接核 retained Tb 全局16-entry R FIFO、append/head-only retire 和保持 VALID 的行为。本席不重复其内存路径审查。当前模型在跨 ID 间选全局顺序返回，而 AXI 的 ID 并不要求跨 ID 的全局完成顺序；已经 held 的 R 不能为了抢救 PTW 改 ID、撤回或换数据。不能只因全局 ordered 返回而断言它协议非法，也不能把这种模型停滞等同于服务器、GPU或FPGA板测结果。

## RREADY 与实际缓冲容量

1. `tests/nonblock_actual_dma_top.sv:48,231,352–374,411–433`：两真实 DMA 的 BufferDepth=512，数据总线64bit，NumAxInFlight=8，RAWCouplingAvail=0。
2. `cva6/corev_apu/iDMA/src/idma_buffer.sv:33–50`：8 个独立 byte FIFO，每个512个 byte。因此主私有 R→W 缓冲为512×8=4096B。
3. `idma_axi_transport_layer.sv:254–266`：masked input FIFO 全部 ready 才能接收 R；总 RREADY=`in_ready & r_dp_ready_i`。`343–360`：有全组数据且上游 WREADY 时才 pop FIFO；WVALID 取决于数据齐备。
4. `dma_core_wrap.sv:206–207,319`：实际1D frontend 的 `idma_rsp_ready=1`，backend 的 ErrorCap=NO_ERROR_HANDLING。`idma_backend.sv:531–532`：r_dp_rsp_ready、w_dp_rsp_ready直接取rsp_ready。当前非零普通 copy 不会因为软件尚未读取 DONE 而关掉 RREADY；controller不停轮询 DONE 也不会改变这一点。
5. `idma_backend.sv:837–857`：当前 RAWCouplingAvail=0 走 AW fall-through bypass，不能引用未实例化的 R→AW coupler 作为此次停滞原因。

## W 通路及一个具体 credit 边界

实际 W 通路为：iDMA byte FIFOs → DMA input mux 按 AW 记录的 W owner → bank input demux → bank W FIFO/translated-AW credit → error/completion demux → bank completion mux → system xbar/cuts → DDR。

- 实际编译的 AXI mux 是 `cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv`，不是另一份同名 vendor 文件。`317–333,352–380`：AW选择入 W owner FIFO；W owner直到 WLAST handshake才退。
- `axi_demux_simple.sv:175–177,232–253,429–435`：W routing保留owner，未完成前不能切到其他目标。`rv_iommu_banked_ctl.sv:246–268`已有跨bank read-admission保护；不能用旧注释直接断言此次 solo 是双bank互等。
- `rv_iommu_trans_ctl_wrap.sv:178–179,220,360`：credit在**内部 completion AW 被接受**时增加，在 WLAST排出时减少；W FIFO ready和completion WVALID只有 `w_cnt_q!=0` 才开放。这个计数不等于 DDR端观察到的AW个数。
- `rv_iommu_trans_ctl_wrap.sv:553–557`：内部 completion demux有 AW/W/AR/R spills；因此DDR7个AW不排除第8个内部completion-AW已被接受且留在spill/后续互连。
- `rv_iommu_banked_ctl.sv:163`把全局NumOutstandingTrans8分成每bank4；`rv_iommu_trans_ctl_wrap.sv:174,242–254`的w_cnt_q输出宽度是 `$clog2(4)+1=3` bit。
- 实际 `counter.sv:28–42`实例化delta_counter；`delta_counter.sv:28,54–58,67–72`保留4bit内部计数，但q_o只输出低3bit，overflow_o输出第3bit。若内部累计8个AW而没有WLAST，则内部counter_q=8、w_cnt_q=0、overflow=1。**W通路被当作没有credit关闭。**
- 这时IDLE中的AW优先条件 `rv_iommu_trans_ctl_wrap.sv:628`要求`!w_cnt_ovf`，于是会跳到允许的AR选择（646–668）。下一AR的翻译可能需要PTW；现有快照确有后排PTW。若快照counter=8成立，具体闭环就是：8-credit低位零 → W不排 → R缓冲/切片填满 → 全局R队头payload无法退 → 后排PTW无法返回 → 当前翻译及后续DMA无法完成。

上段是有精确RTL公式的**待实证假设**，不是已经读取了内部credit的结论。现有外部快照证明了ordered-R依赖和不排W，尚不能区分credit=8与另一个W owner/VALID依赖。

## 最小诊断与修正边界

无需新增RTL即可加一条 failure-only内部状态打印。现 Verilator `obj-diagnostic/Vnonblock_actual_dma_system_top.h:1669,1859` 已保留每bank真实4bit计数器字段；bank0字段：

`nonblock_actual_dma_system_top__DOT__i_dma__DOT__i_sys__DOT__gen_banked__DOT__i_banked__DOT__gen_bank__BRA__0__KET____DOT__i_bank__DOT__i_w_counter__DOT__i_counter__DOT__counter_q`

generated `Vnonblock_actual_dma_system_top__4.cpp:1948,2144` 的W gating明确取 `(counter_q & 7)!=0`。同一停滞点读取counter_q（及已有decr_w_cnt）即可检验上述8-credit假设。若不是8，应沿W通路仅补各级WVALID/WREADY和owner，定位首次断流，不能放大watchdog获得所谓成功。

不同动作的登记边界：

1. **只读 failure snapshot：** C++观测产物/构建输入改变，要独立冻结并保留原失败；RTL、输入任务、内存响应次序和时序不变。root已采用此方法。
2. **4KiB tensor-local普通DMA分段：** 主FIFO明确能容纳整段所有source数据，无需任何W排出即可接收全部该段R；本夹具source/destination均4KiB/8B对齐。每段至多两个2048B AXI bursts，也不需要8个AWcredit。6144B=4096+2048，两个4718592B各1152段，三个1536B原样，共每role2309描述符；两个有用源包仍各6完整tensor/9447936B、ready0。新driver仅用原NEXT/DONE接口、每物理channel一次一段、实际polledDONE后refill，single仍完整role0再role1。新task/register/commands/compiler/构建冻结及全部配置成本必须登记；旧whole试验保留为适配失败证据，不作为弱性能基线。
3. **RTL credit修正（如扩大真实输出计数/限制未完成AW数）：** 即使逻辑是明确bug，也会改变冻结RTL和可达性，需独立修订、critical接受及协议验收；本轮没有授权生产修改。
4. **改变DDR跨ID仲裁/队列或加buffer：** 改变模型时序、资源或普通强基线，需另登记，不能就地拿救活后的结果替换原模型。已经held的R仍必须保持合法。

4KiB是现有主buffer容量推导出的安全分段上限，不是已测最快分段。还须root构建和独立完整输出/事件验收；本席不声称分段模型已运行或全部case已通过。counter边界最终诊断若得到新状态，将只补本critical报告，不更改已交付分段文件。

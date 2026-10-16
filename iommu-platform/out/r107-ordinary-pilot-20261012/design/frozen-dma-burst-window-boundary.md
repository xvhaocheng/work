# f9fa冻结bit：4KiB task、AW数和普通批量可达性

本席只读取冻结源，不构建、不运行设备、不重跑模型。根提供并已接受的f9fa source入口为 `out/composition-isolated-20261007/fpga-src/cva6/corev_apu/`（下称F）；stage110稳定裁决把指定bit列为`out/composition-isolated-20261007/route-opt/ariane_xilinx.bit`、SHA256 `f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca`（out/r107-platform-identity-recovery-20261012/root-result-acceptance.json:27-28）。不拿currenttree或stage106模型代替这份冻结source。

## 已直接核查的参数调用链

- 总线64bit：F/fpga/src/ariane_xilinx.sv:159定义`AxiDataWidth=64`，:811-813传给ariane_peripherals；F/fpga/src/ariane_peripherals_xilinx.sv:1255-1258再传给DMA wrapper。F/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:306-316把相同DataWidth和HardwareLegalizer=1传到backend。
- backend没有覆盖Protocol，所以采用AXI默认值（F/iDMA/src/idma_backend.sv:13-14）；:328-330将Protocol/DataWidth传给legalizer。
- AXI最大burst为256beat（F/iDMA/src/idma_pkg.sv:107-111）。legalizer `StrbWidth=DataWidth/8=8`、`PageSize=min(256*8,4096)=2048`、`PageAddrWidth=11`（F/iDMA/src/idma_legalizer.sv:75-85）。默认不deburst的写页范围为2048B，数据长度按页剩余量依次截断（:170-178、254-267）。
- 全零CONF使用INCR、coupled AW/RW、无deburst（F/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:341-373）。因此双端4KiB对齐、正常4KiB task产生最多且正常恰好两条2048B AW。不能把24KiB logical run直接写成一个24KiB task：它会有12条AW，且超出512beat full-buffer论证。
- 实际JOB_FIFO_DEPTH=8，不是3（F/fpga/src/ariane_peripherals_xilinx.sv:1262）；BUFFER_DEPTH为DMA0/1=512、DMA2/3=3，NUM_AX_IN_FLIGHT=8（:1269-1270）。wrapper oned FIFO使用JobFifoDepth（F/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:186-202）。三beat数据buffer与8job FIFO是不同资源。

## 3job窗口的数学上限与实际可达性

如果一个额外接口合法保证全平台最多三个未完成4KiB task、无其他translated write、每个task恰有最多两条AW，则各bank的pending AW−WLAST不会超过6，达不到已知C=8别名条件。这是充分上限，不能把它说成当前hardware FIFO=3或板端已验。当前寄存器接口没有独立三job在途的可达路径，不能为了该数学界新增RTL或伪造该接口：

1. NEXT_ID请求只在IDLE分支处理（F/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:470-515）。普通非batch非zero请求设置issue和返回next_id；其MMIO响应ready直接等于FIFO ready_i（:494-509）。如果FIFO不可接收，它是backpressure，不是可读的“剩余credit数”；非batch分支不像batch分支那样在!ready_i时撤回issue（:510-513），故不能设计靠不断试读NEXT来探容量的控制。
2. COUNT仅发当前配置对应的一个request；真实FIFO握手后转WAIT（:519-530）。
3. WAIT必须等trans_complete_i后才退休；DEFAULT非batch回IDLE（:544-559）。busy时NEXT_ID的默认数据是0（:380-382），没有执行IDLE接受分支；不能把它当第二个合法job ID。配置写busy时也不是一个完整独立descriptor入队协议。
4. 因此普通寄存器外部可接受inflight window是1，这是冻结前端真实约束，不是本次driver人为single-child限制。FIFO物理深度8不改变这条FSM事实。

## 推荐保留硬件batch，规避W-credit条件

当前硬件batch允许num_bytes=4096、双端4KiB对齐、DEFAULT、非零bounded target和pages（同frontend:153-157）。它在COUNT/FIFO握手发一个task，WAIT等该task真实trans_complete；非最终task再回COUNT或合法GAP/ADMIT（:544-568），所以同一batch内部同样没有多个copy同时新发的路径。task issue/done统计来自这两个实际边界（:158-159），不是仅NEXT reservation；最终raw DONE只退休一个batch功能ID，物理任务数量另核batch issued/completed（:286-302）。

首pilot保留这一普通能力：每个合法连续24,576B run配置6个4KiB task，每个12,288B run配置3个4KiB task；旧hardware batch无须software三jobwindow。实际地址extent不连续时合法拆成更多batch，全部供给成本计入。logical runs固定96、physical tasks固定432，NEXT/MMIO batch启动次数另列，不能把三者混为一数。既有源码batch只能正向等距src/dst共加4KiB，不能合并7→5和5/6→6/7之间的不连续映射。

从全平台quiet/无旧write出发且仅DMA0产生translated DMA write，batch FSM使尚未完成task仅一个；该task最多两条AW，因此pending credit C≤2，已知C=8空/满故障不可由本有限合法路径触发。下一task等真实backend response后才发；F/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:206-207把idma_rsp_valid作为trans_complete_i并令response ready恒1。根仍须确认actual当前S0、正常copy/无MRIF、bit source参数和backend完成/quiet条件，才把这条源码界用于新pilot。

没有新的板端功能验收或独立AW/WLAST逐拍重放。已知W-credit私有修复并未进此bit；该有界路径规避故障触发条件，不宣称修复生产RTL、泛化并行DMA或任意更大task。旧4KiB分段solo证据仅作历史功能入口，不代替本冻结source推导。

# MQSim host完成边界：补查3份固定原件

2026-10-15。仍固定`51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16`；只补`src/host/PCIe_Root_Complex.cpp`、`src/host/IO_Flow_Base.cpp`、`src/sim/Engine.cpp`。原字节、hash、blob校验与确切阅读行段追加到`sources.json`。未build/run，未改原状态机。

**结论：有真正host CQE接收和read payload抵达入口，但原实现没有可直接使用的“所有host DMA和未来生成均已排空”谓词。** 不能以最后CQE计数、设备发送完成或`On_the_fly_requests=0`替代后者。

## 确切调用链与可挂只读记录的位置

| 层级 | 固定源码位置 | 实际语义 |
|---|---|---|
| 设备内部请求完成 | 已存`Host_Interface_NVMe.cpp:98–110` | 先移除waiting user request、`On_the_fly_requests--`，然后才发送read data，因此这个计数归零早于读payload抵达host |
| 设备发payload/CQE | 已存`Host_Interface_NVMe.cpp:140–141,347–375` | `Send_read_data`调用host write message；`Send_completion_queue_element`另发CQE。属于发送点，不能当接收点 |
| **host读payload抵达** | 新`PCIe_Root_Complex.cpp:9–14` | `Write_to_memory`的`address >= DATA_MEMORY_REGION`分支，原代码仅注释和no-op。是host endpoint接收入口，但不实际存数据，也不核数据正确性 |
| **host CQE抵达** | 新`PCIe_Root_Complex.cpp:14–20` | 同一`Write_to_memory`的NVMe低地址分支，根据CQE的SQ_ID定位flow，调用该flow的`NVMe_consume_io_request` |
| host完成账本 | 新`IO_Flow_Base.cpp:247–255,276–309` | 以CQE的Command_Identifier取出host request、从software pending移除、回收command ID并增加`STAT_serviced_request_count`等统计 |
| CQE回调仍可能补发 | 新`IO_Flow_Base.cpp:314–339` | 更新SQ head后，可能把`waiting_requests`搬入software/SQ并调用`Write_to_device`发新doorbell。**只在255行观察served变成generated，会漏掉后续动作和其他生成入口** |
| 建议最晚局部观察点 | 新`PCIe_Root_Complex.cpp:19`调用返回之后，结束`Write_to_memory`前 | 这里位于本次CQE完整consume调用之后；再汇总所有flow，并检查生成确已关闭及PCIe in-flight。该点比设备发送点和base计数点晚，但本身仍不是全局quiet证书 |

`IO_Flow_Base.cpp:457–464`提供generated/serviced getters；`405–424`显示请求还可在`waiting_requests`等待SQ/command ID。判所有有限flow完成时应同时检查每个flow的**预先确定终止条件确已满足且无后续生成**、accepted/generated与host接收serviced一致、software pending与waiting为空。generated==serviced在两个到达之间也可暂时成立，所以本身不足。实际用到的Synthetic/Trace派生生成入口及其停止条件不在这3件新增原件范围，当前未核，交root私有副本定向验。

## read payload与真正hostDMA quiet的限制

`PCIe_Root_Complex::Write_to_memory`没有message size/request identity参数；`IO_Flow_Base.cpp:385–399`又把所有PRP设为同一组dummy data addresses。因此不能只用此回调按真实buffer地址匹配每个request的payload字节，也不能声称验证了实际DMA内容。适合记录的是模拟器host endpoint的抵达事件；若要完整关联需在现有PCIe message envelope层保留type/size/归属，只增加观测不改变时序。

本次按3份原件上限**没有读取`PCIe_Link.cpp`和`PCIe_Switch.cpp`**，不能据此宣称payload一定先于CQE抵达、最后CQE后无链路队列/传输尾事件、或所有doorbell已送达。特别是CQE消费后是否有CQ head更新，base文件虽有`NVMe_update_and_submit_completion_queue_tail`（433–439行会发host→device消息），其真实调用入口本次未核，不能忽略。

因此，root在私有完整源副本里还必须定向核PCIe transport的FIFO/分片/传输完成事件与in-flight计数，以及实际finite flow回调。若这些不可见，则见证应精确命名“全部已生成命令的CQE已抵达host，并观察到read-data endpoint事件”，而不升级为“allhostquiet”。这些是验收条件，不是本席已经完成的动态检查。

## event engine会不会在GC残留时停止

`Engine.cpp:80–100`主循环只有两项退出条件：`_EventList->Count == 0`，或`stop`为true。事件按最小Fire_time执行，同时间节点中的事件链先执行完再移除节点。`103–106`的`Stop_simulation`只设`stop=true`。这里没有“全部host完成就自动停止”的判断；新增`IO_Flow_Base::NVMe_consume_io_request`（247–374）也没有调用它。

因此，只要GC/NAND未来完成事件仍在eventlist、且外部没有显式stop，engine会继续执行；**直接等Start_simulation返回才取状态，很可能已错过有限残留GC窗口**。应在上述host端边界回调中记录GC快照，并允许原engine继续自然排空。不能为了拿到GC>0而在设备发CQE时强行stop。

但eventlist为空也不等于所有内部队列空：如果存在没有后续唤醒事件的排队/锁住事务，engine会直接退出。判断正常完成需额外检查GC队列、chip活动、pending page movement和锁状态，不能仅引用engine返回。反过来，若某个未核的派生flow/其他对象调用`Stop_simulation`，它可以在eventlist仍有GC事件时截断执行。**本次没有完整Stop_simulation调用点覆盖，故“全局无其他提前停止路径”仍unknown**；root在实际私有构建源中做一次定向符号搜索即可裁决，无需广扫Linux/RTL。

可接受的下一步记录应同时给出：hook触发位置及事件时间、所有finite flow终止状态和请求计数、host endpoint payload/CQE接收证据、PCIe link/switch in-flight排空证据、GC/NAND活动快照、engine退出原因。若缺PCIe/生成结束证据，则只接受较窄host-CQE边界，不能补造allhostquiet。

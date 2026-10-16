# D完整准备私有桥接交付

仅实现和C++链接；本席未运行任何case，未运行微测试、旧包装脚本、完整RTL构建或设备任务。共享研究Codex-only。请求Sol/medium，实际model/effort运行元数据unknown。

- `preparation-harness.cpp`：当前原C++的私有副本；`private-source.patch`为完整差异。原warm256 COPY、V先poll/break、A反馈refill、A尾drain、cfg原timeout和各硬件guard未改。旧历史core为functional projection，不能代表当前production RTL身份。
- `ramulator-bridge.h/.cpp`：新增64B timing child provider；AR16、AW16，1024 line credits。ARREADY预留最坏33行、同时AW32行，W只在AW归属存在时READY。AW全64B块且8B INCR/fullstrobes，越域失败。
- B必须WLAST和每child实际WR+16成熟；child记录保留到WR+34；AR需child callback才返回原8B数据和ID/LAST。原mem_q顺序保留。WR按RoBaRaCoCh逆映射与唯一未issued accepted write child匹配，歧义失败。
- 所有DRAM请求FIFO，每tick至多一次send尝试；拒绝不移队首。不跨parent合并。source write_beat仍每W握手执行。CPUoff/AMOoff固定来源域；旧top未导出atop，不伪称动态atop检查。
- mode从reset前即P/PN固定；ordinary history在DRAM tick0（及row1的96）以真实read send/callback产生，到256严格整模型drain。history_read_discard明确其没有RTL R消费者。之后执行原RTL reset低8/高4；reset不清provider。
- 原warm A源/目的校验保留。早停分支额外校验V源/目的各512词，明确data_verified=true/full_task_executed=false。双方原configure后进入新增strictdrain，仅正常tick，extra cost单列，完成后return，未到run_full_batch t0或任何测量NEXT。

## 审计与时钟

`bridge-events.csv`含parent/child UID、AR/AW/W/R/B、R实际数据、cfg、hardware issue/done、child offer/accepted/callback、实际WR对应、数据末/恢复成熟及retire。普通DRAM issued/REFab_end/priority_send按tick取事件，含完整address_vector和原event_clock、accepted/request_type/channel（字段存在时）；callback记原Request.arrive/depart；不每tick dump完整YAML。child UID不复用，payload版本由(parent UID,地址,W握手数据)绑定。

preedge握手日志中的rtl_tick是本次被采样的Tb.g_cycle，dram_tick是tick之前已完成的provider时钟；该tick末内存先send再tick，now加1后DRAM事件的clock必须等于now。phase/cfg在tick后发生时rtl_tick是last-completed RTL tick，源码g_cycle已指向next tick；functional_queues另记next_rtl_tick，不能把两者当同一边沿。common_reset_start的rtl=-1（尚无RTL tick），dram=256。

完整phase state：cold、common_reset_start、原warmfinish before bytechecks、warm+A drain、双方configure后的pre-t0、strong final drain。state含controller五队列、refresh/cap/fullnode timing、future、bank状态、桥接AR/AW live/credit、高水位、lastWR/data-end/recovery/B。harness phase另记mem_q/caw_q/cb_q/bridge_B UID队列、heldR及实际dbg_in_flight/dbg_dma_job_quiet/CPUquiet/WRquiet/rst_ni。新增strong终点同时要求in_flight=0、DMAquiet=3、CPUquiet/WRquiet/rst_ni真；未达则继续正常tick并受200000 watchdog，不能冒充通过。

AW slot在B消耗后释放（aw_live<=16）；已B parent元数据仍保留到恢复成熟供host核账，所以parents map总长不声称<=16。恢复债务占用1024 child credit，不能因B/DONE提前销毁。该host审计元数据不是付费native接口实现；native成本unknown。初版按真实触行分配credit，AWREADY始终先保守确认32个可用位置；冻结实际AWlen255需要32个。

## 构建、绑定与运行草案

`attempt1-*`保留首次8.64s失败（不需要的verilated_threads.cpp缺宏）；`attempt2-*`为移除不需要runtime后8.19s成功。`attempt3-*`为root审查后修正时钟/READY最坏容量/retire日志的8.39s成功链接。最终按root必要证据接口要求补维护accepted/type/clock、callback arrive/depart、hardware quiet强终点及actual_R_data，再host链接8.29s成功；结果在`build-receipt.json`与`build.log`。没有重编旧archive或DRAM库，未写旧目录；使用原single-thread verilated.o。`prebuild-bindings.json`记录最终编译前source/contract/config/core/headers/library/runtime引用并检查冻结input hash，build结束重新比对。`delivery-bindings.json`另绑定runner等交付文件与runtime头，不能用请求model当实际证明。

root完成最终接受后可逐个执行下列草案；本席未执行。每case wall timeout180s，最大7900480 RTL tick，无自动retry，输出目录已存在即拒绝覆盖：

```sh
python3 /home/xvhaocheng/work/iommu-platform/out/d-dram-action-20261015/implementation/run-case-draft.py p_row0
python3 /home/xvhaocheng/work/iommu-platform/out/d-dram-action-20261015/implementation/run-case-draft.py p_row1
python3 /home/xvhaocheng/work/iommu-platform/out/d-dram-action-20261015/implementation/run-case-draft.py pn_row0
python3 /home/xvhaocheng/work/iommu-platform/out/d-dram-action-20261015/implementation/run-case-draft.py pn_row1
```

runner在运行前校验最终prebuild输入和binary hash，写argv/env/timeout和exit/duration receipt。源码全部未测；实际warm A次数、成功drain、是否超时/guard失败、histories最终state和P/PN差异都unknown。仅链接通过不证明桥接运行正确，不证明完整任务收益、状态观察价值或原生性能。

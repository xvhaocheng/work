# §43 根级写序可达性判别

2026-10-08。根 Codex 自己提出并核关键代码的命题；不是已完成构建/运行，不是跨模型通用定理。

问题：§42 的自然单job、固定正常映射、两真实DMA prepared-prefix原型，仅把SameBankDmasEnable=1改0，是否能出现不可撤virtual/physical AW/W顺序不同？不能强制owner/伪造未准备AW或用异常丢W造例子。

根命题：当前输入bank demux对不同bank的AW实行W完成串行化；当前source-child admission又要求每个被供给目的区间已有匹配且physical-accepted AW。合起来，从reset空payload出发，所有virtual WLAST均发生在对应physical AW已接受之后。bank内AW FIFO保序。每次进入新bank前，旧bank所有已offer AW的virtual WLAST必须完成，因此旧bank对应physical AW都已接受；新bank的任何physical AW不能越过它们。由bank group归纳，virtual first-offer与physical accepted AW全局owner/seq次序兼容。AW spill和first-push早于handshake会延后可见点，但不改变组内顺序。

不能把本命题误写成“两个剩余W队头每拍相等”。virtual WLAST进入TC后、physical WLAST到内存前，virtual head可先推进；计数夹序是pr≤vr≤pw≤vw。pr<vr时旧physical row的全部payload已交到TC及输出链，不缺未来source/TL，正常内存服务可以排空。本阶段分别观测AW全局顺序与W退休滞后。

关键代码：vendor/axi/src/axi_demux_simple.sv:175–177 只在w_open=0或同w_select时产生新AW；180–181 firstoffer即w_cnt_up，233–253 w_open按Wlast退且保bank owner。tests/nonblock_completion_banked_ctl.sv:281–320输入demux无spill；perbank AW FIFO/COMPLETE见out/natural-write-order-20261008/trans_ctl_write_limit.sv:121–137,721–730。source见out/committed-write-drain-20261008/compact_pretl_owner_fragment.sv:87–123及first child创建/返回；它只供两层匹配且物理AW已经accept的目标前缀。起始private/TC无旧payload，固定正常copy逐byte对应，所以旧parent/child残留不能为未预备下一个AW产生越界W数据。

必须挑错：first-offer尚未AWREADY但W-before-AW；input/output AW/W spill；已有R/private/TCpayload是否真的只属于已accepted AW前缀；bank内header是否有active旁路；physicalAW能否在sameedge早于virtualoffer计数。根直接读源码，native独立审计找反例；DeepSeek整理封装/ID差异，不代替此证明。

最小新执行（待run）：只复制§42六个out文件，改变SameBank参数0、Mdir新目录，增加fragment内无反馈order observer。每次virtual first w_fifo_push/physicalAWaccept打印owner/globalpos/seq/addr；physicalAWaccept须与未覆盖的virtual全局AW行owner/seq一致，帧已有8slot环守恒继续保留。一次+pause-mode=2,+first=v,32KiB；旧四run不重跑，不增加no-pause矩阵。保持原完整copy/guard/owner/held/quiet计分板。如果新增order观察失败，保留原现场并根诊断，不自动把它叫新机制。

执行状态更新：该最小执行已完成，1 build、1 run、完整copy PASS；上述“待run”保留为执行前合同，不再表示当前待办。实际证据与审定见order-evidence.json、build-record.json、run-record.json和codex-result-acceptance.md。

验收：新增观测不反馈控制/队列；保持原数据容量48B、TC配置及父槽；完成copy且双bank确实选0/1，物理34AW逐行兼容virt34AW；pause A可释放，范围仅本运行。实际run只能佐证本实例；排除整个受限模型需上述归纳前提完整成立。生产没有该source prepared-prefix原型，不能外推生产一般不重排。

创新门：如果命题成立，就不能把这套原型中不可达的双W失配当成论文贡献；停止此分支的机制包装。下一设计须先找实际可达约束，或提出有前作差异的协议/证明，不能仅换成通用AW owner FIFO、ROB、VC或短burst并称创新。此阶段不做BIT/上板/SD，不修改生产/Linux/tests。

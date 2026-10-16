# Codex独立判别设计：普通无池准入的写准备进度

目标提高为可争取二区的机制研究，分区不是可由原型保证的成果。本轮先找真实边界，不凭复杂命名增加创新。

直接源码：输入axi_demux_simple在w_open>0时只允许同port AW，不能默认跨bankAW任意重排。原bank TC单状态COMPLETE读只有ar_ready才能返回IDLE，AW-first只在IDLE生效。因此怀疑的更具体环：A0,V0,A1,V1交错的虚拟AW顺序，输入demux尚未允许A1进入bankA；但AR A1提前完成翻译卡在bankA COMPLETE，写前缀准入要等A1物理AW；待A0/V0结束，A1能进入AW FIFO却不能被这个bankTC处理，AR又不能出去。须实际复现，不能把推演当bug。

最小合法干预：仅暂停每DMA第二AW的首次展示，直到另一个当前job的第一AW已进入前端mux；第一AW不暂停。只在双方job_active下启用；用私有AW实际握手count而非mux first-offer计数，释放条件一旦成立即保持到握手，避免VALID撤回。地址/长度/数据、读顺序、真实iDMA/TC/TL不修改。两DMA仍aligned4KiB、两源/目的burst、一个job各。新elaboration参数InterleaveAwEnable1，原0行为不变。观察fixture确实阻塞过第二AW和真实读COMPLETE等待。旧Mode7完整拷贝失败且TC读等自己AW时才支持此环。

如复现，独立候选：在bank完成AR与准入前增加有界translated-AR队列，先释放TC处理写准备，再按真实W前缀发出数据读。仅存头不增加data；ordinary elastic perbank AR FIFO是必须强对照，相同简单FIFO有效不能包装新算法。先compare FIFO1/2是否消环，核容量与最大先行read元数据的关系；没有必要/充分边界和前作差异，不确认论文创新，不进BIT。关键并发RTL由Codex亲自设计与实现，DeepSeek补证据/普通工具。

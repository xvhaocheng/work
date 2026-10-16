# 首次选择与下一 PTE：root 亲核（2026-10-14）

本文件只补本轮决定门，不重做前轮26源／九控制／37费用审查。原文与哈希见本轮 `source-snapshot.json`，RREADY及完整source身份见 `../native-ready/`。

## 合法选择点

两bank各自原生合法队头已进入REQ_TRANS，merged NormalTL尚未首次VALID，且没有既有锁／强制公平选择时，才可比较AB／BA。`rv_iommu_banked_ctl.sv:379-391` 的 eager arbiter不是等PTW可接收才选；`vendor/common_cells/src/stream_arbiter_flushable.sv:35-53` 的LockIn=1，`rr_arb_tree.sv:145-172` 在req_o&&!gnt_i保存选择。PTW忙期间已经offer的另一请求也已承诺，不能到PTW空闲时重新排序。单头不能暂停等第二头。

`rv_iommu_trans_ctl_wrap.sv:625-669` 为每bank AW优先及真实READ admission，`:674-690` 为正常首次VALID，`:714-725` 的COMPLETE握手只是内部地址spill接收，不能当物理DDR接受或R/W/B完成。fullbuffer例外限定原生证书和FullBufferDID=10+b；source2/3不能借source0/1的DID获得许可。不同source同bank的AR FIFO队头不属于本候选两个可互换选项。

## PTE因果链

| 事件 | 可见性与承诺 | 源码 |
|---|---|---|
| 当前PTE首次ARVALID | PTW id=0、len=0、size=3；保持地址直到内部ready | `rv_iommu_ptw.sv:275-279,384-389` |
| 内部AR接受 | 进入PROC_PTE；只是DS输入承诺 | 同上；`rv_iommu_ds_if.sv:81-108` 有7源仲裁和两项AR spill |
| DDR AR接受 | 才进入共享16项mem_q；不能把内部接受当这里发生 | `nonblock_actual_dma_test.cpp:668,1291-1305` |
| DDR PTE返回到PTW | DS返回spill也要经过真实握手，PROC_PTE仅处理收到的PTE | `rv_iommu_ds_if.sv:249-262`；`rv_iommu_ptw.sv:395-397` |
| nonleaf生成下一PTE | 本次PTE的PPN计算下一指针，下一状态MEM_ACCESS；尚未发生的PTE不是免费输入 | `rv_iommu_ptw.sv:507-524` |
| leaf／fault | 按现有完整walk更新或错误终结，不提前截walk | `rv_iommu_ptw.sv:474-505` |

因此下一PTE至少要经过“上一PTE收到并解释→下一MEM_ACCESS→DS仲裁／spill→真实DDR AR准入”。没有一个任意长、可以免费预读全部future PTE的窗口。实际各段拍数、队列占用和首个payload held拍仍未测。

## 强DS优先和首次RVALID

`nonblock_actual_dma_test.cpp:679-719` 每次drive_ddr在前一拍没有held R时，能提升已入队的最老DS请求，保留每ID顺序、完整ID、beat和latency；并非等待整个payload burst。下一PTE没有生成或尚未DDR接受时没有可提升的请求。R一旦VALID&&!READY，`:829-834,1290` 的held状态禁止换队头；只有`:1371-1399` 的真实R握手推进beat／pop。到达16项队列容量也不能取消在先R承诺。

本轮普通合同将DSpriority窗口覆盖共同t0至所有任务、已有R/W/B、faultquiet，不沿用旧代码V-DONE窗口；本轮未改代码或运行该控制。源码旧 `ddr_r_first` 日志在`:1169-1172` 实际记录R握手，不是首次RVALID。旧日志缺held字段不能证明held=0，不重新解析／重跑旧实验补造观测。

## 唯一允许的真实正常copy阻塞输入

当前正常非零一维copy的r_dp_rsp_ready恒1，payload FIFO只经该完整source的真实W握手排空；bank许可不是完整source W租约。RAWCouplingAvail=0导致 `idma_backend.sv:837-857` AW bypass。frontend decouple_aw=0不能推导当前native AW必须等待first R；DEFAULT coupled legalizer仅证明本地R/W metadata成对推进（`idma_legalizer.sv:459-469`），不证明任意独立AW／AR全局前缀可达。

这些边界足以裁决本轮具名实例；不证明全部安全READ/READ的逐拍服务反馈等效。实际production DEFAULT fresh前缀保持unknown。

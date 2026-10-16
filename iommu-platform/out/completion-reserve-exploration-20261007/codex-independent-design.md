# Codex 独立候选 E：按拷贝依赖借用共享接收区（2026-10-07）

本稿在阅读 DeepSeek 创新质疑前由 Codex 独立编写。它是待否决设计，不是实现、实验结果或创新认证。

## 目标与已知证据
当前两 DMA N 使用两个 4096B 私有缓冲，使非 W-owner 的整次 4KiB源读不会反压共享R通路。最新普通whole-walk信用在冷页PN91727/P92334，是已有强性能对照；单靠更改补额不构成本候选。
生产顶层有4 DMA、两个bank，DID10/12共bank0、11/13共bank1；只有前2 DMA享有全拷贝缓冲许可。bank识别不是完整DMA/job依赖识别。源码事实不等于4 DMA死锁已复现。
CSV表明允许的一次256beat数据读会延迟下一层PTE（root1295返回；数据1327→1582；L1 PTE1408AR→1653R）。这是Q下的正常权衡，不能称违反其额度保证。
设计目标是比较相同翻译并发/完成量下，能否用更少物理payload storage维持进度；不预设PN再快某百分比，不把共享缓冲本身称为创新。

## E基本规则
1. 明确两个状态：DRAINABLE=该具体DMA/拷贝job已经是虚拟W路径头且它的物理目的AW翻译成功/可排空；BLOCKED=仍依赖别的W头或自己的翻译。bank号不替代job身份。
2. DRAINABLE 的源读可按其小FIFO流式进行，但必须检查无额外外部循环等待并以公平内存/W服务为前提。仅AW VALID或同bank AW pending不足以证明此状态。
3. 对BLOCKED job，只有在首次选择源AR前，为其预留足以容纳本次完整aligned4KiB拷贝全部源字节的共享R接收区，才授权read-ahead。先限定single job/DMA、长度=4KiB、无fault/abort竞态的已有normal-copy夹具。
4. 所有该job的物理R返回先由共享区吸收，不依赖被阻塞DMA的小FIFO；共享区中的返回不得先注入公共completion R，重新把共享R锁在此job上。只在该job变成DRAINABLE后交还到其DMA，并按真实数据消费/终止记账释放。可研究放置到最终per-DMA返回端口，但不能假设插普通单FIFO即可。
5. 只有一个共享整页slot的版本，暂只允许一个BLOCKED job借用；已有借用被其他job的AW/B或PTW结束事件不能错误释放。身份至少DMA/DID+job epoch，并覆盖全部拆分AR与在途R。
6. 重用现有普通walk预算限制物理服务干扰，作为独立性能护栏；空间保证不推导PTE优先或延迟上界。
7. slot选择/释放不能阻塞现W头的页表请求或源读。饥饿、已选VALID不能撤回、同ID保序、W-before-AW、错误回收必须单独审查。当前规则未证明这些全部成立。

## 可能差异与最强普通对照
- 相对每DMA整页缓冲：将长期逐DMA配置改成只给当前依赖受阻job临时保留。理论payload点从两DMA8192B变为共享4096B+两个3beat私有FIFO48B，**不是**综合面积/性能结果。多DMA并发可能需要多slot；不能主张O(1)保证任意负载性能。
- 相对普通pool/end-to-end-credit：额外信息是具体拷贝的W可排空依赖及延迟交还/epoch生命周期；如果普通pool配正常端点credits已同效果/成本，则E无独立贡献。
- 相对AXI-REALM write-data-ready/fragmentation：改的是IOMMU旁路源读及返回存放/交还规则，但不能以作用位置不同单独认定创新。
- 相对2026 Preemptive VCs：对方已处理DMA R→W端点循环与低成本共享物理链路；我们的AXI RVALID不能随意抢占，现单DDR接口不能直接当NoC VC。其credit/traffic-class分离属于必要强前作。

## 最小去留判别（实施前）
A. 构造两个总空间/已有普通credit相同、但W依赖不同的合法状态；E是否仅改变已经可由普通每ID信用/端点缓冲识别的选择？要给等价或非等价的明确例子。
B. 检查一个shared slot是否只把FIFO反压从DDR移到completion R；若不能避免，终止最简版本。
C. 列出owner逐DMA状态从当前AR/AW/W的可观测接口如何获得，不能用不可实现的oracle。
D. 若保留，之后才做2 DMA最小PoC，与私有fullbuffer、普通共享pool+perIDcredit、owner conservative及walk共比；功能PASS和最小必要流控检查，以及V/A吞吐、占用及小模块综合成本。再决定是否扩4 DMA/板端。
E. 如果只剩传统buffer pooling、未获得新调度决定或可测成本优势，停止E主贡献。可作为性能主线的低成本实现，不宣称领域边界突破。

## Codex追加：四DMA的反例与规则收紧（先于DeepSeek质疑）

单纯把两私有缓冲改成一个共享slot不足以保证4DMA进度：slot已借给bank1的DID11；DID10是W路径头且需要源读；同bank0的DID12源AR先进入bank0不可越过的AR FIFO，但它无slot且不是真正W头。若在TC头才拒绝DID12，DID10源AR被挡在后面；DID11要等DID10写完才归还slot，构成等待环。这是接口级反例，尚未用原RTL运行复现，不能称板卡bug。

因此E的最小规则必须在DMA→共享AR arbiter选择/银行AR FIFO入队**之前**做永久预约：没有排空路径或whole-job slot的AR不能占住可挡住W头的公共队头；授予的slot覆盖同job全部AR，直到R存储/交还生命周期结束。具体DMA jobW头及物理AW就绪信息须真实跟踪，不拿bank近似。已经发出的VALID保持稳定，不撤回预选。

这个收紧避免把风险藏到队列后。普通per-initiator共享pool+pre-admission credit也可能与其等价；若如此，E至多是当前IOMMU的成本优化。论文贡献仍必须来自约束下的新trade-off/可推广设计与实测，而不是两个旧机制并列。

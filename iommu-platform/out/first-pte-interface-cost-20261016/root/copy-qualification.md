# 一种正常COPY对的有限资格，不建立通用依赖分析器

本轮源码驱动设计，未实现或执行。资格只解决“暂缓谁不会新增数据依赖环”，不是免费知道未来收益。

## 静态所有者合同

限制DMA0/1各自一次在途的normal aligned 4096B COPY，使用原各512×8B buffer；其他DMA在该episode不发工作。输入在提交前准备好，CPU不在DMA期间改写这些页；映射、owner和配置保持到全部R/W/B及错误责任排空。允许连续重复相同已核独立COPY，前提是每job仍按原frontend完成链串行。

在quiet配置时由buffer所有者确认物理读写范围：`W_V ∩ (R_A ∪ W_A) = ∅`且`W_A ∩ R_V = ∅`，页表/控制内存不在DMA写目标内。IOVA不等不证明这个条件；源源重叠在只读/不变合同下可以允许。范围不满足、第三设备加入、映射或输入所有权改变时不启用新门，回退普通控制。

这里有正常软件来源而非凭空的隔离需求：当前`linux/drivers/misc/idma.c:354–399`中每个stream分别`dma_alloc_attrs`取得source/destination、初始化内容并建立对应IOMMU映射；`:2448–2455`分别准备V/A streams，`:2408–2410`排除同设备/DID。正常成功分配的同时存活私有buffer及其原map是此独立COPY子场景的来源，不意味着所有任意COPY都独立。驱动还持`:2390`的实验互斥并检查batch inactive（`:2431–2434`）；这不等于新增M资格位已经存在。

M需要一个新增、quiet时设定并保持的`pair_qualified`配置/接线，绑定原DMA0/1及生命周期。局部实现可以固定该已核测试合同；实际可配置部署需要所有者验证、配置/关闭与失败处理，CPU/MMIO费用不能记成零，也不能复用未定义的ABI位。O得到同一资格及现有配置输入的使用机会；不要求O为单纯分段添加本来不需要的资格生成器。

M-ID另需在quiet arm确认所选V的normal-S1根页表整个4KiB位于该profile的DRAM译码窗口，根/context与地址路由在pair内稳定。PTW首笔地址仅由已知根PPN和IOVA顶层index形成（PTW:360–365），无须未来PTE内容。非DRAM根不arm：若P1在另一target成功读取nonleaf、P2才到DRAM，PTW active尚未retire，单靠ID与active会误认P2。owner配置校验/绑定的成本归入新增资格生成；不能把这个条件说成现有fullbuffer证书已覆盖。

## 动态门控谓词

在PTW真实正常S1目的write start时，将当前锁定请求的bank/DID/方向与pair资格绑定。只允许保护精确V（例如DID10/bank0），只抑制目标共享mux尚未锁定/首次offer的精确A payload源（DMA1身份），而非所有非DS、全部other-bank或整个DMA读通道。V自身的源R请求、所有R返回、W/B、DS和CPU/debug的既有义务不暂停。

原fullbuffer证书`iDMA/src/idma_fullbuffer_certificate.sv:14–18,26–41`证明的是一次normal aligned4096 job的容量资格；`:4–5`及wrapper`:173–183`保留单job/退休责任。它**不证明**V所有source已读回，也不检查跨job依赖。原banked控制`:246–268`说明小buffer/other-bank与W owner可能形成R返回循环，前两原512beat buffer及该证书是已有防护，不改变它们以制造窗口。

在上述静态合同下，A不生产V的任何必要数据；门只挡A的新AR，因此不会阻止V自身的数据生产或旧义务排空。H只给新增等待一个有限界，不提供外部READY/完成上界。错误、reset和资格撤销必须保持已承诺事务的正常责任；本轮没有一般活性验证。

## 将正确性资格与旧模型的性能条件分开

“V source已完整”是旧2060/2058数值适用的共同状态，不必把它增设为门的普遍安全前提。若强制到source_done才导出intent，可能使原始start窗口消失。M应在实际start时立即使用已冻结的静态资格，源完整性作为局部后继需要观测的性能条件；不能把新变体的时间直接替换旧模型。

若确需观测source_done，可复用backend本job已接受、legalizer读侧已派完以及读metadata FIFO真实RLAST清空这组事实，额外引出信号并保存本job起点；不能只看fullbuffer permit或某个空FIFO，也不需要先加逐beat通用计数器。精确接线由O侧源码笔记给出，本轮不实现。

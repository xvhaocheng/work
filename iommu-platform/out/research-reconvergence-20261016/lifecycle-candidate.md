# 候选C：范围限定的安全撤回完成，而不是更宽松的失效

2026-10-16，root独占的第三机制空间；基准`c981e01`。状态：建设性设计与定向原文核查，**没有实现、模型运行或性能结论**。本文不是恢复I或旧完成纪元主张。

## 问题和因果链

RISC-V IOFENCE.C的PR/PW覆盖此前已被IOMMU处理的读/写，而非软件本次回收的某个范围。一个正确实现可以等待此前不相干设备的长请求。范围失效精确，并不自动使完成等待范围精确。官方原文：[队列规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)“IOMMU Command-queue Fence commands”；本地既有原文`../independent-directions-20261015/lifecycle/sources/riscv-queues.txt:302–355`。

**这只是规范允许的机制问题，尚不是当前平台测得的瓶颈。**当前CQ的PR/PW仍是TODO，不能把这个不完整实现拿来当正确全局排空的性能对照；`../../cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:348–392`。Linux本树省略PSCID/ADDR的扩大失效也须先修成普通对照，不属于贡献：`../../linux/drivers/iommu/riscv/iommu.c:1777–1817`；IOTLB已有精确匹配`../../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:225–244`。

## 最强具体设计

限定无ATS、S1叶映射、有限活动范围。软件登记`{context, IOVA base, length, generation}`，只在真实回收/权限变更时发起`revoke(scope)`，不是给每个包添加新安全需求。

1. **OPEN**：翻译成功、物理请求首次承诺前，把它关联至已登记scope的旧代计数。已有请求记录携带scope索引及代次；读在所需全局可见点/全部读数据取回后退休，写在所需可见点退休，不能把AW、W或普通B一概当全局可见点。
2. **REVOKING**：关闭该scope的新旧代混入，执行精确失效；匹配的未承诺walk取消或重试，已承诺PTE读保留接收/丢弃责任，不能撤VALID。只抑制受影响范围，其他设备继续。旧IOVA在ACK前不重用于新对象。
3. **ACK**：scope旧代已处理请求均退休、受影响旧walk不会再发布翻译、页表更新已可见，才能给回收完成；处理尚未抵达IOMMU的设备请求仍须正常设备停发/互连栅栏合同。没有这一合同，不能靠代次猜测迟到请求属于哪个对象。
4. **CLOSED**：ACK后才复用范围/标识；槽满、代次将绕回或不能证明范围归属时走正确普通全局路径。

新增至少有K个范围表项、状态与代次、各旧代读写计数，以及每个在途请求的scope索引/代次、匹配比较与退休路径；跨页burst按实际覆盖集合处理或由合法分段拆开，不能只按首地址。新增软件登记、关门、重试及回收责任都收费。数量界不等于服务时限；本设计没有凭空提供DDR/设备完成硬上界。

动作改变的是**一次范围回收究竟等待哪些已承诺请求**；不是改变TLB替换或预测未来PTE。若没有不相干长请求落入旧全局等待前缀，能力没有收益。

## 强普通方案与已发表近邻

| 对照/前作 | 已有能力和限制 | 对本候选的含义 |
| --- | --- | --- |
| 精确IOTINVAL＋正确IOFENCE；设备正常完成证书 | 当前硬件已有按context/IOVA匹配。可信设备的源数据/写可见性若已有充分完成证据，普通软件可能根本不必请求PR/PW全局排空 | 必须先排除多余fence，不能拿driver扩大失效或缺完成追踪作优势 |
| per-domain计数、合法批处理/持久池 | 能消掉跨domain等待；允许相同登记信息与完成接口。持久映射只在相同授权合同下合法 | 候选只有在**同domain不同范围**仍有回收耦合时，才可能留下额外能力；不得人为禁止分domain |
| Intel VT-d Rev5 §6.5.2.4、§6.5.2.5、§6.5.4 | 已有domain/page/PASID选择失效和相关读写drain；PASID-based invalidation要求drain已处理读写 | “支持定向失效/有drain”不是新意。原文未给本次所需的实现成本、精确逐范围退休或实测完成次序，不能反向宣称其必定全局阻塞 |
| IDYLL MICRO2023 §6.2–6.3 | in-PTE目录缩小失效目标，IRMB合并并惰性写回PTE；TLB仍即时失效，miss时并查IRMB | 它减少控制流量，不等于本文scope级已承诺DMA退休；但不能把目录、保守误报、合批当新构件 |
| HATRIC ISCA2017 §4.1–4.2 | co-tag把翻译结构纳入缓存一致性，缩小失效对象；有精度/面积权衡 | 缩小失效范围已有明确硬件前史；它的CPU/虚拟化一致性结果不是本scope回收的I/O证据 |
| Utilizing the IOMMU Scalably ATC2015 §5 / rIOMMU ASPLOS2015 | 延迟失效、延后IOVA复用及ring隐式失效已有前史 | 普通池化/合批只要满足本次授权也应采用；不能默认所有场景都要求逐请求撤权 |

原文位置：Intel既有全文`../independent-directions-20261015/lifecycle/sources/intel-vtd-rev5.txt:6862–6879,7064–7068,8129–8174`；[Intel原件](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf)。IDYLL本地PDF第6–7页/印刷1168–1169，root定向读取§6.2–6.3；[作者/NSF原文](https://par.nsf.gov/servlets/purl/10492257)。HATRIC本地PDF第5–6页，root读取§4.1–4.2；[作者预印本](https://arxiv.org/abs/1701.07517)。IDYLL/HATRIC结构预检为UNAVAILABLE（本环境无pypdf），已成功用Poppler提取指定页；不把这个替代作结构PASS。后两篇2015文献本轮只复用既有核读，不宣称root重新全文阅读。

## 预期收益、反例与当前裁决

若真实回收Q时，Q旧代均已完成，但不相干范围R仍在旧全局等待前缀中，本机制可能缩短安全回收/不可回收内存驻留；必须同时报告Q回收时间、R后台进度、物理保留字节与元数据/CPU/带宽。映射存在不等于物理内存必须保留。

**最小推翻条件**：正常完成证据已足以避免该fence；或普通per-domain/queue计数以相同代价分开等待；或部署允许持久专用池使回收频率可忽略；或scope追踪/同步开销超过被避免的驻留和等待。这些情况下停止该具体版本。

**当前不投入。**不是“成本还不知道所以再开发”：固定正常map/unmap来源通常先等设备完成，尚未取得必须PR/PW且被不相干活跃范围拖住的源绑定见证；本机PR/PW又未实现。VT-d已有相近定向drain能力，其内部范围/成本未核到可以支持新意。仅从RISC-V当前缺接口扩一条命令，或修复旧完成纪元安全问题，都不足以立论文。它保留的是一个明确、可取证的问题，不是一条自动排队的采集/实现任务。

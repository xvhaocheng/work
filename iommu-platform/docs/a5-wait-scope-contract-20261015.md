# A5：真实AXPY的等待范围与控制位置设计

2026年10月15日。**推进A的“完成域与等待范围证书”版本；AXPY保留为真实消费者校准任务。普通按ID等待已经能表达本次动作，实际共享翻译竞争和完整时间收益仍需证据。** 本轮不选定跨方向最优，不恢复A默认排名。

## 1. 真实任务与本轮新事实

固定已有HERO AXPY：32768个FP32元素，64tiles；core8发布，核0–7在四个2048B L1槽上做FMA。原main cold/hot各恢复相同输入，两个kernel共384个job/786432B，映射建立一次、最终撤销一次。源码和A2补丁/编译成果复用，没有运行在当前CVA6四iDMA平台。

| DMA动作 | 经过外侧IOMMU的地址腿 | 留在集群内的地址腿 |
| --- | --- | --- |
| RX/RY：host→L1 | host IOVA的AR/R | L1的AW/W/B |
| WY：L1→host | host IOVA的AW/W/B | L1的AR/R |

**输入的远端R返回结束，不能单独替代L1写完成。** Snitch内部xbar先按TCDM/alias地址分流到本地存储；FPCluster外侧端口才接IOMMU。读写均SID0/SSID0，不是两个context。单DMA不排除跨copy流水，但两路buffer也不证明两路DMA或两个同时翻译请求。

决定性原文：Snitch cluster 575–675的路由/本地存储、545–551外侧连接；Carfield cheshire_wrap 469–488同标签、514–525 IOMMU端口；native mover 332–369完成看最后写B。固定ref/SHA和行号见[新核地址路径](../out/a5-wait-scope-contract-20261015/source/translation-legs.md)。原native cfg bit2/serialize错配继续登记、不修补。

目前只证明结构路径/允许能力。默认应用是否同时向IOMMU提供两个请求、是否真正竞争共享翻译资源，均未核。xbar同ID跨本地/外侧目的排序可能进一步限制输出AW出现，不能把descriptor接受当成已经进入IOMMU。A2三个Hero硬件hook仍未实现、默认拒绝pin；不能称已有原生SVA完整任务。

## 2. 完整任务合同与最小动作

源地址为`mapped_iova + 0x200000000 + tile*2048`，L1目的/源是原四槽；实际运行IOVA、有效context/PTE和时钟未知。`BYPASS_OFFSET`名称不作为绕过IOMMU的证明。全部源参数、输入公式、地址/context、容量、owner、合法前缀、事件和服务前提已写入[冻结合同](../out/a5-wait-scope-contract-20261015/case-contract.json)。

root保留最小 **O-T1**：倒数第二轮只保存后发RY63的原生返回ID，最后轮只等这个ID后进入原barrier；它的有序前缀已覆盖RX63。逻辑增加一个32位值，实际寄存器/poll/fence开销未知。四槽、最后输出、末wait-all/fence/barrier、DONE、CPU cache交接、独立consumer及全部unmap/失效完成/unpin/free保留。未应用/编译变体。

HERO没有当前平台的NEXT寄存器；这里是DMCPY→FE offer→FIFO接受→返回ID。这个返回是接受回执，完成计数来自写B退休。当前程序先wait-all/起始barrier，预取next与当前FMA并行，结束barrier后才发布WY。最后tile没有未来预取覆盖，旧WY62用bank0、K63用bank1。原生错误计数不代表成功，成功/可见性仍须独立合同。

## 3. 方法与正反预测

root具体化的规则：从当前consumer访问取得必需生产者集合D，使用相对接受rank取最后必需h；有序完成前缀f必须超过h。对其余未完成job，检查当前及下一guard前访问是否有未保护的RAW/WAR/WAW冲突，并确认全部owner和尾排空保留；否则返回WAIT、NEED_GUARD或UNKNOWN。对该动作可用，不代表完整时间满足。[规则、条件证明与完整完成推导](../out/a5-wait-scope-contract-20261015/method/rule.md)

两案使用同一默认任务、同一已接受descriptor前缀：本episode起始ID为b，RX63=b+188、RY63=b+189、WY62=b+190已经接受，WY63尚未发布；K0..62已完成、K63尚未开始，旧WY62均未退休。

| 事前合同 | 正例P：本地输入已完成 | 相邻反例N：本地输入未完成 |
| --- | --- | --- |
| 切面 | completed=b+190 | completed=b+189，RY63远端R已结束、本地写/B未结束 |
| O-T1 | 可以通过输入门槛，随后原barrier | 继续等待RY63本地完成 |
| O0 wait-all | 仍等WY62 | 仍等RY63及WY62 |
| 可检验判断 | 原公开ID足以表达本动作，无需新早期接口 | 只凭远端R结束放行会违反必需依赖 |
| 完整时间 | 未知 | 未知 |

两者是**源级部署判断正反合同**；没有改变任务/压力/相位或人为延迟buffer。它们不声称完整AXI轨迹或周期可达，也不要求两案外侧全部握手/可见信号相同。若默认运行不出现正例窗口，撤销本任务重叠机会；若本地输入未完成却被规则放行，规则失败。若以后计算提前而完整consumer/释放无改善，不能记性能预测成功。

N还给出一项具体边界：对本源/API摘要允许的执行，仅“远端R完成”不足以安全发布“输入可用”通知，须有本地完成信息或额外已证明的服务保证。本例的公开ID已经提供所需等待能力。这不是“全部入口优先无效”的证明，也不是指前作采用了这个错误通知。

冻结后实际运行了一次小型离线依赖核对：两次offload与尾部共384个job，正例判可放行、反例判须等待；同信息标准DAG给出相同判断。两案为该手工DAG的下闭集，不能据此推出RTL容量/协议可达、数据正确或时间收益。旧A1四案八次/A3检查未重跑。[脚本、结果及边界](../out/a5-wait-scope-contract-20261015/validation/dependency-results.json)

## 4. 强普通对照与全部费用

| 方案 | 完整机会及费用边界 |
| --- | --- |
| O-T1/O-G公开ID | O-T1本身是普通方案；O-G允许一般循环双ID、先发下一RX、复用旧Y前等旧输出。均未实现，计元数据、poll/barrier、并发与预取损失 |
| 当前用途优先、DONE串行/节流 | 源端可在发布前排序；当前WY须先有计算结果，输出优先要付失去预取重叠的成本。HERO DONE是整次offload；无免费host逐tile许可 |
| 提前准备、原buffer、直接消费 | 原buffer保留，CPU FMA同功能合法；无源给CPU预算排除。尚无地址先于buffer就绪的真实准备空档，不凭空增加prepare收益 |
| IOVA/shadow/cache | Hero显式IOVA软件、当前A2三mode软件均保留，不等于互相原生适配。映射/页表/cache/CQ/撤销费用未知；C2独有mlock/mmap锁和丢失prefix重叠不得强加普通方案 |
| 入口优先/DS-first | 只有实际独立未承诺head与可选择接口存在才适用；不能选择隐藏descriptor或重排已承诺返回。没有证明它们对完整目标全都不足 |

按main的4KiB布局，x/y各131072B/32页；不能把stage117八页任务的成本直接套来。额外reference/expected列为仪器成本，实际pin/页表页数/服务费用仍待核。两次restore、供给/cache、kernel/所有输出、CPU consumer均计入，最终还要全部撤销/释放。不确定错误不能超时free。

完整预测以真实FIFO、AR/R/AW/W/B、TCDM/翻译/DDR服务和上述软件费用给界，输出计算、旧/新输出、DONE、host消费、资源归还向量。同ID B有序不等于所有job串行；未给界就保持UNKNOWN。目前没有完整期限、净收益或“必须前移”的全策略证书。

## 5. 可核查新意与明确决定

MASK §5.2–5.4已有翻译/数据联合服务；DWS §V–VI已有walker分配/借用。RTAS2026 §V–VI已有FIFO外调度、合法分块与完整响应分析；TC2023 §2/§3.7允许job内任意请求时序，另有读写独立/流水不满等假设。准确原文与阅读复用[既有近邻核查](../out/research-novelty-assessment-20261015/analysis-priors.md)，没有新广搜。

本版本拟推进的是：把真实端点**本地/远端完成域、公开完成前缀和owner**转成可检查证书，预测该具体动作应依赖哪个可部署控制，而非一律增加早期翻译接口。如果摘要规则能在未见任务复用，事前导出有用部署判断并经完整结果验证，可形成抽取/部署方法贡献。当前一个手工实例与同信息标准图结果相同，不能把事件图、普通ID或单次边界解释作为独立算法新意；跨任务复用和完整时间尚待证。

**决定：推进这一有界证书方法；共享竞争＋完整收益版本按已知缺口重设计；停止仅限“本动作必须新增早接口”的主张。** 不继续外围软件寻找胜例，不扩大任务搜索，不停止A思想族。三包文件任务仍作功能基线。

下一步最少先核固定xbar同ID跨目的约束和已有IOMMU接收/服务能力，决定默认任务是否还有实际共享窗口；这不需要先补Linux或工具链。如果后续安排原生验证，才需要对应硬件身份、有效IOVA/context、自然RX/RY远端R与本地W/B、WY接受/外侧AW/B以及共享队列同存证据；再给CPU/cache/供给/锁槽/消费/释放费用。可达窗口与服务闭合后，才决定最后两tile的最小协议/时序片段及O-T1固件改动，并另冻一正一反两时间留出。本轮均未提前执行。

本轮Codex-only，root设计/关键核源/实际离线检查/最终裁决，独占Sol席仅核固定地址腿。0 Linux/RTL编译、周期仿真、FPGA、板测、Hero工具链/hook或生产代码改动。原MIG及已结束实验保持原状态。[稳定接受](../out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[导航与核验](../out/a5-wait-scope-contract-20261015/README.md)。

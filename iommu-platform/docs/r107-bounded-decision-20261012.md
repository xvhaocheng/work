# stage108：R107 有限 KV 恢复合同与有界立题裁决

**裁决：条件保留 R107 的有限合同资产，不登记新 G0，不进入 G1。** 本轮把源级需求落实成一个完整、可核对的研究回放，并核清普通准备接口与成本。真实 FPGA 准备、完成及翻译相关残余损失尚未验实，因而不能推进方法立题，也不能将未实现 adapter 当成普通方法的科学失败。

本轮使用实际 `create_goal` 登记，无额外 token 预算；执行依据是 [stage107 有效下一合同](../out/ai-platform-constraints-20261012/next-goal-contract.json)，原样复制到 [本轮输入](../out/r107-bounded-decision-20261012/goal-contract-input.json)。root 与两个独占 Sol／ultra 子席完成应用合同、平台接口和关键裁决；无 DeepSeek、Claude 或 Astra。主实验平台仍为 Genesys2／CVA6＋四 iDMA＋RISC-V IOMMU＋Linux，服务器只承担本轮核源与 CPU 功能检查。

## 有限实例、所有权与自然结束

来源为固定 [vLLM v0.1.0 commit](https://github.com/vllm-project/vllm/tree/67d96c29fba9b72cb4c4edbc26211c208a00ebdd)、[PagedAttention v1 原论文](https://arxiv.org/pdf/2309.06180v1)，以及唯一新增的 [官方 OPT-125m 固定配置](https://huggingface.co/facebook/opt-125m/resolve/27dcfa74d334bc871f3234de431e71c6eeba5dd6/config.json)。模型配置给出12层、hidden768、12 heads、fp16；D=64 和 TP2 后每 worker 6 heads按固定源码计算。TP2、各端8槽、两个序列的 token ID、checkpoint 和字节标签均为明确研究选择，未称原生请求 trace。

一个 group 共享完整16-token前缀，两序列各有不同尾槽。唯一槽数为3、引用数2／1／1。原源码类的实际 CPU 执行得到：A GPU表 `[7,6]`、B `[7,5]`；换出 CPU表相同；恢复后 A `[5,7]`、B `[5,6]`。共享前缀只搬一次。完整前缀后追加尾 token 走新块分支，本例 COW=0；不是删除必需的 COW。

四个不同池由同一可信回放进程拥有：两分片各有 CPU_HOME 与 GPU_RESIDENT。每层 K/V 布局保留来源形状，以普通 layer-major 视图装入各池。DID10／11 是拟 FPGA 回放的真实源码 requester 身份；不是 CUDA stream，也未称已经上板。允许授权 DID10 完成两分片四池，不能人为加入租户隔离。

| 合同量 | 数值与边界 |
|---|---|
| 层／总heads／两分片 | 12／12／每分片6heads |
| 每 K 或 V 槽 | 12,288B，完整16-token槽 |
| 单方向完整恢复 | 144笔，1,769,472B |
| 之前完整换出 | 另计1,769,472B；两方向合计3,538,944B |
| CPU代理有效标记字节 | 663,552B；不是已证明的 native attention 必读量 |
| 尾槽复制填充 | 1,105,920B；全部复制与核验，未报为语义收益 |
| 四池逻辑容量 | 9,437,184B；每池2,359,296B |
| 普通大页准备 backing | 16MiB，8个目标2MiB叶；7,340,032B容量填充属于同一 owner |

源码在准备字典时即可回收旧槽编号；这不代表物理 copy 已完成，更不代表 unpin 或 IOMMU 撤权。拟硬件生命周期是：生产者／此前换出完成→源不可变与缓存交接→当前 issue 前冻结字典→每层两分片的完整 K/V 完成与实际完成ID确认→消费代理核验→全集合完成、排空与缓存归还→真正寿命结束后撤映射／释放。自然结束由有限集合耗尽定义，无计算 delay、到达、deadline 或捏造 EOS。

源码已有同 stream 顺序、refcount、共享去重和每层 event；attention 在覆盖 KV／decode use 前等待事件。**因为这些动作已由准确来源实现，不能把它们本身称为新增 IOMMU 能力。** 本例 live3、恢复保守预留3＋2=5均容纳于8槽；选择已换出 checkpoint 只建立条件恢复义务，不证明物理容量迫使交换。固定源码同一期禁止同时 swap-in／swap-out。

### 真实应用、回放与 FPGA 执行分别验收

源函数／CPU检查实际运行一次，全部144笔、24个 worker-layer、源保持、未使用槽 guard与填充检查通过，并保存六个原始池文件。实际执行的是源类／形状函数的 AST 选取及小型接口代理，不是完整 vLLM／torch／CUDA。研究数据由 CPU 生成，未下载权重、未算 attention、未运行 GPU 或 FPGA。

`Sequence.get_len()` 不证明 native cache-valid 长度：worker把最后一个 token送到 `get_len()-1` 位置，attention在 event之后先更新该位置的 KV，再做 decode。663,552B只是选定静止 checkpoint的代理标记量；真实消费量、消费时刻和 token完成均未验实。不能从代理的全字节要求推出自然应用翻译损失。

冻结合同、任务展开和数据见 [instance](../out/r107-bounded-decision-20261012/instance/finite-contract.md)、[验收](../out/r107-bounded-decision-20261012/instance/acceptance.json)、[原始数据清单](../out/r107-bounded-decision-20261012/instance/raw-manifest.json)。精确来源、行号与hash见 [应用原文](../out/r107-bounded-decision-20261012/instance/source-evidence.json) 和 [root亲核](../out/r107-bounded-decision-20261012/critical/root-source-review.json)。

## 普通基线与合法控制点

| 普通路线 | 完整工作与成本要求 |
|---|---|
| 四个完整4MiB池 | 当前 `MAX_ORDER=10`，只拒绝 `order>MAX_ORDER`，故 order10 在大小上合法；每池映两个2MiB叶，实际成功率／碎片／时间未知 |
| 八个独立2MiB块 | 每池两块，可拼连续IOVA；无需全槽物理连续，也不必每extent多一条descriptor；分配、映射与回退成本完整计入 |
| 合法4KiB回退 | 逻辑四池2304页；若保持全部16MiB backing则4096页，二者分别报告；不能先强迫4KiB制造压力 |
| 稳定池、提前map、驻留 | 全池可在setup准备；逻辑free不撤映射。此前swap与恢复字节仍计入，不用预备窗隐藏成本 |
| 普通批量与连续run合并 | 每component的5→6、6→7可合为24,576B，7→5另12,288B，144项可减为96项，完整字节／层屏障不变；排序、提交与同步成本未知 |
| 授权单DMA完整合并 | 同owner允许DID10访问四池，完整恢复仍为1,769,472B；backing复用，实际domain／别名／同步／服务取舍分别计费 |

旧10MiB整槽需要order12的限制不能套到本例4MiB池。全部地址仍是合同中的拟IOVA与符号PA；没有本轮分配、页表写入或实际DMA。

硬件已经提供 SRC／DST／LEN、NEXT_ID触发和DONE控制，恢复字典在当前首个 issue前可见。缺的是普通有限 manifest、稳定池寿命、授权检查、层事件及消费代理适配；当前 Linux helper只生成内部模式缓冲区、同offset实验，不能直接导入该KV合同。这个实现缺口不等于科学损失，也不证明控制位置在硬件上不存在。

当前源码 IOTLB=16；lookup按VPN、PSCID／GSCID与stage匹配，DID本身不作为直接tag。完整池的8个目标叶小于16，只说明静态足迹；冷填、替换、上下文与alias、实际叶和运行身份仍需证据，**不能据此声称零miss**。提前写页表也不等于预填IOTLB。

Linux S1同步路径的PSCID／ADDR限定被注释，会发全host S1 invalidation与两次IOFENCE调用；void路径未传播相关bool。实际map返回成功不单独证明fence已完成。普通提前／批量映射可以避免把这些操作人为插入每次logical free。CVA6有平台特定FENCE.I缓存交接约定；实际Image、叶、fence、缓存与排空仍未验实。

平台源码与存储配置可核，live bit／Image／DTB／时序未知。生产mode0每DMA3beats，mode1源码首两路512beats与stage106私有512不混用。当前IOMMU独立维护HEAD与旧sources.lock不同，未修改生产源或锁定链。

## 全成本与本轮停止点

已落实分配、全池初始化／打包、domain／IOVA、map／实际leaf检查／IOTINVAL／IOFENCE、缓存交接、提交／补充／poll、层事件／CPU消费代理、双方完整恢复、drain／缓存归还／验证、unmap／free、失败回退与quarantine的接口和计量位置。数值见 [完整清单](../out/r107-bounded-decision-20261012/platform/cost-inventory.json)，全部新 FPGA 运行耗时为未知。

端到端时间必须用实际开始／结束测量；有重叠时不把phase耗时机械相加。首次准备、稳态与回收分列，摊销须有实际使用次数；之前swap、padding、失败分配、额外alias或staging都不可省略。原生权重／activation／计算与通信没有执行，不能由本轮byte replay声称native总成本已覆盖。CPU功能检查耗时不是DMA／推理性能。

本轮有了具体有限合同和源级可达控制点，因此不满足“缺实例／不可达则归档”的条件。另一方面，没有准确普通强对照留下可避免翻译损失的原始数据，不能接受G0／G1、创新或净收益。**条件保留只保留这份合同；重复源码核查或未变CPU回放到此停止。**

下一步仅在实际镜像／时序身份与一个明确普通adapter范围先成立后，做同一实例的一次最小普通准备／完整完成采证，保存全成本、真实leaf/cache/fence/NEXT/DONE/drain及残余归因。普通方法覆盖目标或无法取得这些新输入，就归档此卡；不能加第三场景、任意压力或猜计算。具体可避免翻译损失成立才登记最小G0，G0支持后才定向核前作并设计G1。本轮没有登记下一goal；[下一合同](../out/r107-bounded-decision-20261012/next-goal-contract.json)是未激活且有前提的提案。

root独立记录检查643项通过，平台结构／原文／hash检查200项通过，73项本轮冻结的旧输入保持。已验stage106八案不重跑；旧输出只定向核一份原始summary的边界，未作KV性能证据。原校验器通用断言／hash函数被实际复用；旧ready0／六张量专用events／output验收没有套到新合同，也没有伪造硬件events。两份完整席位产物及root证据保存于 [独立目录](../out/r107-bounded-decision-20261012/)；实际动作、诊断与语义澄清见 [执行账本](../out/r107-bounded-decision-20261012/execution-ledger.json)。

稳定裁决见 [root-result-acceptance.json](../out/r107-bounded-decision-20261012/root-result-acceptance.json)。本轮交付完成只结束这次有界立题裁决，不代表方法论文已完成。

# Codex审定：共享bank读写顺序边界与二区目标（2026-10-07）

## 当前结论

用户要求“推进到2区创新”。本轮已推进到真实RTL可复现的适用边界及一份可检验的新机制规格，**尚未建立论文创新，也不保证分区或录用**。没有修改生产RTL/Linux、编译BIT或动板卡。Codex亲自设计机制、核关键源、实现并执行所有仿真；DeepSeek两次补证据及一处C++验收草案，不执行命令。

先排除两条错误捷径：不能因为输入/输出各有仲裁就断言AW顺序翻转；不能忽略TC内部已有两个完成AR缓冲槽，给它再加普通FIFO就宣称创新。然后找到更实际的边界：**多个DMA共享bank时，早到的非写队头源读在FIFO中挡住当前写所需的源读。**

## 已执行判别

全部为两个真实iDMA、真实TC/TL、测试副本完成准入、行为DDR/合成CPU。不是MIG或板测。三组共享bank对照使用相同独立AR背压夹具、S模式、两次预热、V目标64个aligned4KiB，A持续批次；窗结束后仍完整排空及字节校验。模式有意不启用P/N，先判结构进度，不能外推PN速度。

| 配置 | 活跃DMA payload | 结果 | 窗口V/A完成 | V窗口cycles |
|---|---:|---|---:|---:|
| 每DMA独立bank，完成阶段普通写前缀门 | 48B | PASS | 64/63 | 129548 |
| 两DMA同bank，同写前缀门 | 48B | 停滞，独立V watchdog触发 | 首次预热均未完成 | 无测量窗口 |
| 两DMA同bank，普通完整私有缓冲 | 8192B | PASS | 64/63 | 127659 |
| 两DMA同bank，普通源端准确写准备门 | 48B | PASS | 64/64 | 131464 |

48B/8192B仅DMA有效payload，排除共同TC W FIFO、AR spill、metadata、控制/响应字段；不是总存储、BRAM/LUT面积。后三组改变准入改变请求时序、暖场A数量及窗口工作量，不能从上述单点计算公平性能收益或“3%代价”。源端门是普通强基线，不是新算法。非P/N矩阵。

### 停滞的直接证据

最新清洁诊断 `samebank-causal-s.log`：

- 同bank0；A的首AR只在首次展示前有限延后64cycle，随后正常放行；只阻塞63个source-VALID cycle。AW、地址、长度、数据、真实DMA/TC/TL均不伪造，AR开始展示后保持稳定，测试夹具自行检查。
- `spill=3`：bank0内部完成AR spill的A/B均满。
- `queue_ids=1024 = 0x400`：最低4bit为spill B ID0（V）、次4bit为spill A ID0（V）、第三4bit为TC的当前AR FIFO头ID4（A）。
- `ar_head=0, tc_read_wait=1, tc_state=4`：外部完成AR头是V；bank0 TC为COMPLETE，其当前A读已在队列头等待内部spill接受。两个V请求挡在A之前。
- DDR写队头 `id36, addr0x90100000, len255, got0`：当前写需要A的数据；36是外层DDR prepend32＋原DMA A ID4。
- `caw_q=4`、四个AW均准备，`tc_aw_pending=0`；不是“自己的AW尚未翻译”的原假设。
- `sharedR.head empty`、PTW和TL req/resp均无活动、DDR comp AR累计0。第一笔DMA数据读尚未发出，不是内存带宽饱和或已发R背压。
- 独立V完成watchdog到200008 scenario cycles，两DMA都无完成；夹具延时已结束很久，属于封闭等待状态。保留原失败exit1，不把预期失败算PASS。

因果链：W头A → 需源读A → A完成AR在两个V完成AR之后 → V完成AR不满足当前W前缀准入 → V不出去，A不能越过 → A W无数据。已有两槽仅释放前两次TC翻译，不能消除同bank不同DMA的FIFO HOL。单加更深的全FIFO仍不能让A越过V队头；与可按ID挑选/旁路的队列不同。

### 软件/硬件对应关系及限制

生产 `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:870` 有四DMA；`:1409` 以 `BankIdBit(DmaAxiIdWidth)` 选两个bank，因此DMA0/2、DMA1/3分别共享bank。本轮保留两真实DMA，以测试专用BankIdBit3把原ID0/4都送bank0，是该共享拓扑的最小等价形态；**不是实际四DMA上板复现，也不是认定现行生产fullbuffer路径有这个bug**。生产使用完整私有缓冲且未启用本测试Mode7。未核生产全部4DMA fullbuffer绕行/错误行为。

为了正确判别，完成AR观测/准入按原DMA ID映射，物理W源身份按真实输出mux AW first-offer ID及W LAST独立跟踪，不能拿共用bank的mux port当DMA身份。分类代理随BankIdBit取真正bank，否则A从bank0完成却错误pop bank1标签。首个普通源门运行触发这处新增测试配置的标签错误；Codex修后PASS，失败保留。它不是生产源码错误，且无池停滞发生在任何comp AR捕获之前，标签错误不是停滞原因。最新诊断修了标签位并再次复现同一状态。

## Codex独立候选：按写完成义务暂存读地址，给关键源读保留直接通路

目标是让少量bank服务多个DMA时，保留提前翻译、小数据FIFO及可证明进度。不能只证明小buffer省空间，也不能只证明修了一条FIFO错误。

第一版限定仍为每DMA单job、aligned4KiB、两个2048B源burst、同一DMA readID有序，正常无fault，无取消/映射更新。**尚未实现以下机制。**

1. 在每bank完成AR与真正数据准入之间，暂存暂时不属于可排空写前缀的已翻译AR。保存完整头、原DMA/ID、burst序号，不保存数据。
2. 待写队头转到某DMA，优先发出它的最早可排空源读；只允许跨不同ID挑选，保持同ID原顺序，不能伪造AXI重排许可。
3. 当前可排空的源读，且不存在同ID更早待发AR时，可以不占暂存槽直接进入完成mux。暂存满时仍保留这条通路，避免“关键读也必须先拿空槽”重新闭环。
4. 读地址初次展示前锁定选择与准入；展示后到handshake不改VALID/字段。实际发出的每笔读沿用准确W前缀/返回可排空证书。
5. 缓冲槽先预约再接受非关键完成AR；只有头部被发出才释放，不以TL响应完成或“已经获准”提前回收。应包含TC spill、在途翻译与元数据的真实账本。

实现还必须处理三个接口：当前READ分类FIFO按bank顺序pop，跨ID调度后这个假设失效，须在原完成AR被捕获时取标签并随暂存头/直接通路携带，不能沿用旧顺序分类去测FAST或额度；quiet/in_flight须计入暂存和已锁定待发头；四DMA版本须使用完整原stream ID而不是当前两DMA夹具的一位ID选择。正常权限/错误路径不得因暂存复用丢失，但第一PoC明确只覆盖正常成功翻译，不能声称取消/失效安全。

### 容量问题——下一步要证明，尚不是定理

当前反例是V0、V1挡在A0之前。针对**仅在TC spill之后暂存、不能窥探越过内部spill**的结构，一个外部槽不够：V0占槽，V1仍是内部队头，A0仍不可见。两个槽可移走两笔V头，再让A0通过直接通路。因此先比较H=1/2；这是待RTL验证的容量预测，不声称已跑。

推广尝试：每bank有K个DMA，每DMA最多L笔当前job先行源读，若暂存所有非当前完成义务的头，则外部容量上界候选为`(K-1)L`，与每DMA静态L槽共`KL`比较。本轮K=2、L=2：候选共享2槽＋直接通路，对照普通每DMA2槽共4槽。**全局W队头变化、bank之间竞争、在途请求/旧job及同ID顺序可破坏这个简单式；必须证明或给反例，不能把容量数字当已确认节省。** 长job需要明确L的来源和准入，不能用总传输大小隐含限制。

只有若形成必要/充分的进度条件、可达容量下界、比强普通方案更好的同条件成本/性能，才可能成为增量论文贡献。CAM挑选、共享队列、credit、owner优先本身均为普通构件；普通shared-CAM＋direct-bypass若在相同信息/容量下完全等价，则按该普通实现归类。潜在贡献可以是新的可推广容量/进度结论与低代价实现，不要求每个门电路首创，但本轮未证新结论。

## 前作边界

- [Arm AMBA LTI官方说明](https://www.arm.com/architecture/system-architectures/amba/amba-5)：已有把翻译与数据事务分开、在事务排序满足前请求翻译。不能宣称第一次提前翻译或第一次拆开翻译/数据；LTI完整规范的最新细节本轮官方入口无正文，仅核官方概述，不称已排除其精确流控设计。
- [MMU-aware DMA ICCD2018 §IV-C](https://arxiv.org/pdf/1808.09751)：已有burst元数据、失败排空重放、替代大量数据缓冲。与本候选正常双外存端的W前缀/跨DMA返回进度条件不同，但替代存储不是新概念。
- [Arm DMA350 TRM](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2)、[CTC NOCS2009](https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf)：完整返回空间、接收信用和消息依赖处理是强普通基线。
- [2026 Physically-Aware Preemptive Virtual Channels §III](https://arxiv.org/html/2607.01430v1)：DMA读写耦合的协议死锁及VC/物理通道分离早有研究。本轮不是发现DMA死锁这一主题；差异候选在共享IOMMU bank内的完成AR队头与小控制缓冲界限，仍待近邻查验。
- 原MAP中的AXI-REALM、PTE优先、普通额度、按ID队列及旧专用VC等不能省略。没有找到完全同式不等于不存在。

## 二区目标的验收与停止条件

主线保持共享IOMMU的性能/成本，进度约束支撑性能贡献，不改成安全论文。下一步先做2头暂存＋关键读直接通路的test-only PoC，同时做普通per-DMA双槽队列；加入H=1故障对照。先一组同bank合法反例做功能和排序验收，避免重复完整矩阵/BIT。

随后需要：
- 四真实DMA/两bank及不同bank负载的机制适用性；不能拿单bankS结论代表PN协同。
- 同工作量/同窗口比较完整缓冲、普通源准备门、普通per-ID队列、普通共享可挑选队列/旁路及候选；V和A/所有DMA有效完成速度都报告。
- 从准备/翻译/控制排队/数据返回/待机拆因果；是否保留提前翻译及FAST要实测，不预言更快。
- 参数化进度论证及非法条件的安全回退；对非对齐burst给有效字节区间证明或显式限制，不能套用当前1:1burst关系。
- 综合总控制/数据存储、LUT/FF/时序；只对成功有差异的小机制进BIT/板测。现板资源很紧，先模块级成本，不保证装得下。
- 若普通零额外存储源门或普通shared queue同资源同信息达到全部效果，或新下界只是普通FIFO容量账本，则不称二区创新，回到研究MAP寻找其他具体约束。

## 原始产物与实施范围

`runs.json`保存各次执行及原日志JSON；`stage-evidence.json`仅阶段清点，包含预期失败，不叫全矩阵验收。原诊断/CLI错误/标签错误/覆盖检查错误均保留。构建命令和退出单独保存，不把构建成功当实验PASS。最终测试补丁相对本轮开始的六源快照，见`test-source.patch`；`after/`保留当前源。

DeepSeek第一次123776、第二次177142 provider tokens，合计300918，均deepseek-flash/high/393216、stop、未截断。完整任务、返回各保存。第一份把AW翻转当可能但缺合法因果链，Codex未采纳；第二份正确解释双槽，但可选诊断例把full写成OR、层次路径不完整，Codex未采纳，自己用实际A/B状态与ID观测核因果。

76编译生产SV哈希changed/missing均0。本轮只测试/研究代码，不写卡、不重启、不重载、不生成BIT；新可调度暂存方案未实现、无泛化无死锁或Q2承诺。

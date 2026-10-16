# §76–78 Codex 最终接受（2026-10-09）

真正的在途加入已实现并完成真实 RTL 完整拷贝；但当前性能不能作为独立创新，因为同许可/缓冲/目标W资源下的普通起点共享更快。所有实验只在 out 夹具中进行，无 production/Linux、BIT、上板或 SD 操作。

## 实际执行与验收

§76一SV build/7 DUT、§77一SV build+一C++增量/8 DUT、§78三SV build/14 DUT，合计6成功build步骤、29完整copy PASS，无本轮DUT失败。审计脚本曾各有一份初稿失败，旧稿/原诊断都保留；这不是DUT失败，也没有为修审计重跑RTL。

- audit76.py 实际exit0：7CSV完整AR/rfirst/rlast与真实R counter一致，目标W各256拍、logical R/W各2048B；source8+exe快照九项SHA一致；对§75三组pure-observer回归CSV逐字、原JSON/观测指标相同。
- audit77_78.py 实际exit0：22CSV/LOG/JSON及五套source8+exe快照全部核对；source AR长度与实际R counter、完整copy与目标写入量一致。
- §78 run02关闭加入，与§77 run04 CSV逐字一致，全部旧指标相同；只排除trace路径、独立新INFLIGHT记录及SOURCE四个新观测字段。§78 run13/14迟到许可对照CSV和除trace外全部JSON相同。
- §77旧obj可执行曾被C++增量覆盖，run01–03按真实build记录和immutable build01快照归属；没有假称旧argv路径当前exe仍是旧版本。各run的原argv/result/log/CSV SHA已入ledger。

## 本轮解决的实验模型问题

旧内存模型每idle probe都前移RR指针，随后又可能因R间隔而不发VALID；interval64可以让全部服务槽落在同一选择相位。旧run01 A的AR已接受且ready，却直到旧V的64拍全部完成才取到1拍。这是合法AXI服务的一种偏置，不能当预期公平RR。

Codex将RR指针更新移到实际DDR R handshake，保留held VALID禁止旋转及每ID最老请求顺序。修后§77 run04/05/08均出现4周期resident窗口，rV=rA=J1、rawWV1/rawWA0、旧W promise1/0、E64且旧V仍busy。旧记录保留；旧interval>0的零窗口及公平服务推论限定为旧模型，不对所有旧耗时作重算或宣称interval0全回归。

## 新机制的真实行为

§78在phase2 resident窗口绑定：组合成员在绑定沿成为11，旧V的AR/ID/E/physical LAST保持不变；立即退出旧short状态，阻止A[J,E) fallback；新W先关旧J网格，之后受peer rawW与fullresident门控制。没有撤销旧firstVALID请求或未退休B。

新增局部1个functional FF、9个只观测加入点的FF、0 payload；不是综合面积或完整平台成本。现有所有recipe/metadata/组合逻辑仍需计费。

五次实际加入：run01/04/06的E64、run10的E63、run11的E256。均J1、sourceR257（N256+J1）；E<N时实际A source AR只有[0,1)与共享[E,N)，没有[1,E)独立fallback。E=N只有前缀A AR，无新共享尾AR，两份原logical LAST仍完成。

## 强对照给出的结论

下表是实际issue到DONE周期；平均为(V+A)/2，单条件结果不是统计重复。

| 方法 / run | 源R拍数 | 源AR数 | V周期 | A周期 | 平均周期 |
|---|---:|---:|---:|---:|---:|
| 普通短补读＋尾段共享 / run02 | 320 | 4 | 23492 | 23499 | 23495.5 |
| 在途加入 / run01 | 257 | 3 | 19462 | 19465 | 19463.5 |
| 普通许可已齐就从起点共享，q64 / run03 | 256 | 4 | 19402 | 19411 | 19406.5 |
| 普通起点共享，q256 / run12 | 256 | 1 | 19402 | 19411 | 19406.5 |

加入对run02平均耗时低17.1607%，但普通startup还快57周期、少读1拍。无背压/B0、interval63、E256等另外三组也由startup胜出；interval0加入未发生而startup仍胜出。该场景中A完整own许可在V第一条source AR之前已存在，普通调度可以直接使用，不能把solo启动的约束强加给强对照。

prepare-order0的run13/14均无加入/无startup，R320，V23244/A23246，轨迹相同。只说明此真实迟到许可条件没有新优势，不推广成所有迟到任务都不可能。

## 创新接受标准与未完成部分

[Intel pending-read merge/multicast](https://patents.google.com/patent/US20200402198A1/en) 已有pending合并/动态等待窗/返回mask；[DMA-Latte v3](https://arxiv.org/html/2511.06605v3) §IV-B已有一读两写的broadcast。因此，因为当前普通策略更好且原则已有前作，这组性能不能称独立算法贡献。

下一门槛是实际晚提交任务或晚授权，结合热/冷翻译、连续作业、有限响应保留/历史及不可撤W债，在相同信息/总成本下检验额外能力。特别是现有8B skid在交付首拍后仍可能保留该值；J1需要加入普通保留响应复用对照，不能忽略它。该对照及连续作业/late提交尚未实现、未测试，详见[codex-next-novelty-gate.md](codex-next-novelty-gate.md)。

当前只覆盖immutable正常内存、每次reset的一对2KiB实际DMA、成功own翻译、B3/q2。未测实际remap/fault恢复、任意上下文、连续作业、MIG、FPGA面积/时序或板速。五次实际绑定沿physical_valid都为0；组合同沿成员逻辑经过源码审核，但不能说held physical R的绑定沿已实测。

如果更强普通pending merge/有限history/等待窗口同成本复现全部增量，就不能称独立新规则；如果新许可/义务合同增加实际能力并超过这些对照，才能形成机制贡献。不以模型共识或期刊分区替代这个判别。

## 归属与证据

Codex亲自机制设计、低层并发/终端/模型诊断、关键源核、实际应用/构建/执行及最终接受；一个独立关键席针对J1同沿、terminal环、轮询相位、绑定/E网格和startup合同审源，未执行实验。DeepSeek提出纯观测、C++验收和普通审计代码，初稿字段/模型混淆被实际检查拦住后限定修订；Codex机械应用并执行。六次deepseek-flash/high调用actual provider total tokens合计836285，全部untruncated，默认预算未降，无Claude。

证据：[§76 audit](../shared-read-inflight-window-20261009/audit76-report.json)、[§77 ledger](../shared-read-odd-rendezvous-20261009/execution-ledger.json)、[模型诊断](../shared-read-odd-rendezvous-20261009/codex-model-diagnosis.md)、[§78 audit](audit77_78-report.json)、[§78 ledger](execution-ledger.json)。原源码/可执行快照、原数据、worker完整回复和失败脚本/日志均保留。

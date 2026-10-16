# 10月9日早晨：创新探索的实际结论

夜间任务已在2026-10-09 08:00 JST按约定结束；170次为最后§70–74五阶段，不是整夜累计。

Codex亲自设计、诊断并运行了共享读切换原型；DeepSeek补充独立提案和普通验收脚本。工作在 out/，保留了原始失败与每版源/可执行文件。以下区分真实RTL复制结果与下一步纸面机制；没有上板、BIT/Linux/SD改动或面积结论。

## 现在能确定什么

瓶颈有两个方向：内存读服务稀缺时，少读共同源可以改善整体完成时间；两设备速度或许可进度不同，共享又会把较快设备拖住。读数量更少，不等于V/A都更快。

最后五阶段§70–74共170次DUT执行：152完整copy PASS、8旧版真实watchdog、10严格actual-cutover拒绝。不同配置各运行一次，不是统计重复或板卡结果。§72初版8死锁修后24/24通过；§73最终观察器版6/6通过、总30/30；§74总20/20。严格CSV拍数审定和快照见各阶段的根接受/ledger；§74最终20份CSV、6份构建记录与source/exe版本已全部审定。

| 检验 | 实际结果 | 能说明什么 |
|---|---|---|
| §70共享与完整独立读，限制DDR R服务 | 若干带宽条件整体耗时低约19%–39%，也有V变慢 | 普通广播少读流量有效，有公平取舍；不是独立算法证明 |
| §71局部memory-wait/consumer-hold在线选择 | 两条规则首批22项选择相同，未显示几何规则新增优势 | 不能凭更复杂公式立贡献 |
| §72先补公共prefix，再决定共享或独立 | 修后24/24完整复制；长预算降低读量，也可能增加V等待 | 绑定等待有界，整体完成时间没有该界 |
| §73V推进不必等A完整授权 | 30/30复制；长延迟budget4096使V约7628→11700，慢53.4% | 授权解耦可行；普通独立路径也能完成V，过长等待仍有代价 |
| §74写数据已到后提前发A AR | 最终3条件实际早AR计数1，分别只省12/14/12cycles，约0.13% | 真实重叠成立，但普通流水线可解释，不能单独命名新算法 |

## 真正修掉的问题

§72的新独立A suffix本来仍要求V的翻译记录有效。V已经完成READ，其记录正常释放，A却永久等它，造成8次真实死锁。修为独立A只依赖自身授权，共享仍核双方授权后，全部24完成。失败source/exe/log没有被删除。

另一个是新增双AR offer同拍创建导致诊断计数少加一次；改为数单物理AR接口的实际握手，4组完整回归CSV和原指标精确一致。这个属于原型工程错误，不能当论文创新。§71还有关键席手推的odd-E/pending-B候选环，定向RTL未复现；没有把它冒称实际DUT失败。新共享门等双方实际rawW到E，避免带着未消费前缀数据加入共享。

## 与最强前作的边界

[DMA-Latte v3](https://arxiv.org/html/2511.06605v3)已支持一次源读取送两个目的，并讨论广播/并行选择；[Intel请求合并专利](https://patents.google.com/patent/US20200402198A1/en)已描述pending scoreboard、动态请求窗口与multicast返回。它背景提到IOMMU，不能宣称前作完全没有翻译。

[Torrent](https://arxiv.org/html/2512.17589v1)有grant/data/finish阶段；[AXI multicast crossbar](https://arxiv.org/html/2502.19215v1)处理AW/W/B、多播前排空相关单播。已读正文没有给出本原型完整的独立权限、长READ、logical LAST、旧W/B义务切换合同；这是核查边界，不是证明所有已有研究都没有。

补查[DAC 2026 的 NXP Central DMA 验证摘要](https://63dac.conference-program.com/presentation/?id=ENGPOST070&sess=sess264)：已有跨执行环境传输、multicast和高并发AXI的工程设计。因此“多个域＋DMA广播”也不能直接当首次创新。该摘要不够判断本文拟议的IOMMU授权/在途加入/不可撤义务细节，需进一步核实际机制。

因为广播、独立许可核对、超时和常规流水线已有明确来源，今晚测出的这些功能单独不能称为独立创新。若新的在途加入规则在同信息、同总成本、小缓冲下，减少普通最强合并仍会重复的读拍，并保持独立权限及不可撤协议进度，才可以成为论文机制贡献。当前实际结果没有达到这个判别，不能许诺二区。

## 下一项主线

不再加一个阈值。先查真实RTL是否存在：A在旧V physical R仍在途时追到J<E，双方已写数据也到J、无held logical R、各自许可有效。在这个点只把尚未交付的未来R扇出给A；旧AR/header/RLAST和旧AW/W/B不变。理想source读量从N+E变成N+J，节省E−J拍。**这是下一机制假设，尚未实现/测试，不是今晚结果。**

必须先建立A短prefix[0,J)＋追不上时独立[J,E)的对照夹具，才可记录实际机会；当前A直接补到E，J<E时仍有不可撤在途A child，不能在旧夹具得到0就否定候选。该新夹具和在途加入都未实施。不能硬关V RREADY等A，否则global DDR heldR可以挡住A补读/页表读取。普通late read-merge、短READ共同切分和完整独立路径都进入强对照。如果普通合并逐项重现，就不能称独立创新；如果新规则在这些约束下给出可复查的额外收益与安全边界，才可以继续作为论文主线。

详细合同见 [下一机制判别](../out/shared-read-overlap-closure-20261009/codex-next-mechanism-contract.md)。当前没有把这项未来实现列作完成。

## 证据入口

- [§70：内存服务和共享取舍](../out/shared-read-memory-service-20261009/codex-result-acceptance.md)
- [§71：两条在线选择与失效边界](../out/shared-read-online-choice-20261009/codex-result-acceptance.md)
- [§72：公共prefix、8个真实死锁与修复](../out/shared-read-deferred-binding-20261009/codex-result-acceptance.md)
- [§73：独立授权推进与计数修正](../out/shared-read-permission-independent-20261009/codex-result-acceptance.md)
- [§74：20项完整复制与真实提前AR](../out/shared-read-overlap-closure-20261009/codex-result-acceptance.md)

各目录有execution-ledger.json、原始argv/result/log/CSV、旧失败版本及接受源/可执行快照。8/16/20等数字按各阶段定义，未将source-review、编译成功或Python运行器语法错误当成DUT实验。DeepSeek起草普通验收脚本，Codex审改/执行并亲自作机制设计、复杂根因分析和创新边界判断。

# 18席创新搜索与交叉审核记录

工作时间：2026-10-05—06。用户要求每个模型在每个方向各设一席，即6方向×Codex/Claude/DeepSeek，共18席。

**先看[根审定与下一步计划](../../docs/innovation-council18-20261006.md)。** 完成情况和实际用量见[audit-manifest.json](audit-manifest.json)，当前阶段见[execution-state.json](execution-state.json)。下表导航18份独立提案和18份交叉审核；候选主张26个，不等于26项成立的创新。

## 席位与全文

C=Codex，A=Claude，D=DeepSeek。每席R2正式审另外两种模型的同方向R1，另跨方向抽查一席。每份R1因此有两种其他模型的同方向审核。

| 方向 | 席位 | 完整R1 | 正式R2 | 同方向审核对象 | 跨方向查漏 |
|---|---|---|---|---|---|
| 1 翻译与布局 | C1 | [提案](round1-accepted/C1.md) | [审核](round2/C1.md) | A1、D1 | A2 |
| 1 | A1 | [提案](round1-accepted/A1.md) | [审核](round2/A1.md) | C1、D1 | D2 |
| 1 | D1 | [提案](round1-accepted/D1.md) | [审核](round2/D1.md) | C1、A1 | C2 |
| 2 共享干扰与服务 | C2 | [提案](round1-accepted/C2.md) | [审核](round2/C2.md) | A2、D2 | A3 |
| 2 | A2 | [提案](round1-accepted/A2.md) | [审核](round2/A2.md) | C2、D2 | D3 |
| 2 | D2 | [提案](round1-accepted/D2.md) | [审核](round2/D2.md) | C2、A2 | C3 |
| 3 映射生命周期 | C3 | [提案](round1-accepted/C3.md) | [审核](round2/C3.md) | A3、D3 | A4 |
| 3 | A3 | [提案](round1-accepted/A3.md) | [审核](round2/A3.md) | C3、D3 | D4 |
| 3 | D3 | [提案](round1-accepted/D3.md) | [审核](round2/D3.md) | C3、A3 | C4 |
| 4 间接访问与搬运 | C4 | [提案](round1-accepted/C4.md) | [审核](round2/C4.md) | A4、D4 | A5 |
| 4 | A4 | [提案](round1-accepted/A4.md) | [审核](round2/A4.md) | C4、D4 | D5 |
| 4 | D4 | [提案](round1-accepted/D4.md) | [审核](round2/D4.md) | C4、A4 | C5 |
| 5 GPU内存与观测 | C5 | [提案](round1-accepted/C5.md) | [审核](round2/C5.md) | A5、D5 | A6 |
| 5 | A5 | [提案](round1-accepted/A5.md) | [审核](round2/A5.md) | C5、D5 | D6 |
| 5 | D5 | [提案](round1-accepted/D5.md) | [审核](round2/D5.md) | C5、A5 | C6 |
| 6 跨领域边界 | C6 | [提案](round1-accepted/C6.md) | [审核](round2/C6.md) | A6、D6 | A1 |
| 6 | A6 | [提案](round1-accepted/A6.md) | [审核](round2/A6.md) | C6、D6 | D1 |
| 6 | D6 | [提案](round1-accepted/D6.md) | [审核](round2/D6.md) | C6、A6 | C1 |

`round1-accepted`的accepted只表示正文完整可供审核，不是科学验收。A1/A3/A4使用完整聚焦恢复答复；A2/A5/A6保留原正文并附完整收尾/修订，其中补答的明确修订优先。原始部分输出不删除，也不多算一轮。根审定中的事实修正不回写各模型原话。

## 输入、证据与方法

- [BRIEF.md](BRIEF.md)：共同任务、平台事实、独立提案和审核要求。
- [roster.json](roster.json)：18席方向、原始输入路径、交叉关系。API每次输入另存`*-task.txt`、`*-input.json`、需要时的`*-files.json`。
- [研究地图输入快照](inputs/research-landscape-before-council.md)：保留本次根纠正之前的共同地图；[快照说明](inputs/map-snapshot.json)。现行地图已加入本轮补充，不能据现行内容倒推早期席位已读全部新增材料。
- [PRIMARY_CHECKS.md](PRIMARY_CHECKS.md)：R2共同原文补充及其阅读范围。
- [ROOT_ADJUDICATION_NOTES.md](ROOT_ADJUDICATION_NOTES.md)：根对提案和审核错误的核查；尤其规范边界、旧机制、量纲/上界、平台适用性。
- [26项原候选台账](ledger/D1.md)：DeepSeek D1完成本席研究后，另做事实归档；不是第19席或第三轮。它抽取R1，未整合后来全部修正，最终去留须看根审定。
- 新原文及定向阅读记录在[sources](sources/)；原36篇地图语料仍在[原来源清单](../research-landscape-20261005/source-manifest.json)。C1–C6的R1/R2均披露全文、局部阅读与待核的区别。

Codex使用六个独立子代理；Claude与DeepSeek使用已有本地API桥接，每个席位独立请求上下文、按明确文件交换材料，是研究席位而非有命令能力的自主进程。三类席位不具有相同浏览能力或生成预算。R1不读取其他席候选；R2在同方向三份R1完整后可先行开始，无须因其他方向的余额中断停住。部分Codex R2另读另外两席对自己的审核并回应，外部席位没有再读这些较晚回应；此次不是完全对称的多轮收敛，也不是模型评测。

## 调用恢复与完整性

1. 初次12个API本地尝试受网络限制：6份Claude安全错误JSON、6份DeepSeek空stdout和stderr均留在`round1/network-denied-attempt/`。随后在已有用户授权下经正常权限流程重试。
2. 最初6份Claude R1均到16384输出上限；有部分正文或缺正文，不算完整交付。先对A1/A3聚焦恢复、对A2补收尾，再处理剩余三席。
3. 曾为A4–A6尝试任务级32768上限，遇HTTP400；恢复16384仍为HTTP400。一次分类诊断确认credit/balance/billing错误。**不能把前次失败归因于32768不受支持。** 两批失败记录分别保留在`round1-recovery/rejected-budget-config/`、`insufficient-balance-attempt/`；前一目录是历史名称，不是最终根因结论。临时覆盖已移除，全局配置未改。
4. 用户先确认将充值并保持18席，随后回复“充值了”。之后A4/A5/A6恢复成功，全部Claude R2也成功。没有以其他模型代替缺席Claude；当前无余额阻塞。
5. 共49份按席位命名的API尝试记录，另1份诊断响应，共50项。31项有provider用量：30项研究答复（含6次Claude输出恢复），另1项台账归档。其余19项无用量信息，不据此推断免费。正式交付仍是36份正文（18 R1+18 R2）。

各API完整原始JSON、正文、任务、输入清单、stderr与`*-result.json`均保留。部分早期输入记录没有逐文件hash；较晚调用才加入hash/字节数，不补造历史发送证明。各子组的`api-summary.json`可能被下一子组覆盖，以逐席原始记录及最终审计清单为准。

## 实际预算与用量

| API模型（返回值） | 有用量的响应数 | 输入/prompt | 输出/completion | 范围 |
|---|---:|---:|---:|---|
| claude-opus-5-5 | 18 | 3,983,491 | 199,216 | R1原始6、R1恢复6、R2共6 |
| deepseek-flash | 13 | 1,173,846 | 234,914 | R1共6、R2共6、台账1 |

DeepSeek合计1,408,760 tokens，其中正式研究调用1,266,600，台账142,160。所有成功DeepSeek调用保持thinking enabled/high、requested/effective max_tokens均393216。Claude成功调用使用原16384上限、adaptive thinking/high；输出包含provider计入的思考token。嵌套reasoning/cache明细不再加到总数上。Codex子代理用量未暴露，未填造总token或费用，三模型用量不据此比较效率。

实际验证限于关键原文/规范核查、正式材料完整性和文档导航；没有论文复现实验、源码测试、RTL/Linux变更、构建、上板或SD操作。DeepSeek/Claude提供提案和质疑，DeepSeek另整理台账；Codex执行工具、直接核关键来源并作研究裁决。

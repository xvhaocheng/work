## 本轮新增验证段（供 root 前置保存；只含本轮新增内容）

### 本轮实际执行与未执行
- 执行：定向关键源码阅读；经典/近期原始文献与作者代码核查；机制/证明/手算/成本/条件裁决；源码证据整理与普通文档起草。
- 未执行：模型运行、RTL 修改、Linux 修改、构建、上板、Claude 调用。**这些一律不得记为已验证。**
- 旧 §76–78（6 构建步骤 / 29 次 COPY PASS）**仅引用**，不重跑，**不算本轮验证**。
- 未测：LUT、Fmax、实际本轮周期、Linux CPU 成本。JSON 语法检查**不属实验验证**，也不作验证记录。

### 接受检查（Acceptance Checks，供 root 核对）
1. **候选 A 主手例一致性**：candidate H+24 / global-prefix H+30 / whole-source H+25 / ordinary cursor H+24；完整时刻 candidate 10/14/24 对 whole-source 8/14/25；**旧 V 变慢（8→10）已显式报告**；共同 H 保留；CPU 生产/暂存假设标注未实测。
2. **未混线**：正文主表**不含** lifecycle-redesign 的 26/29.25/34.25（另一套资源假设）；其仅作为未采用手推链接。
3. **A 成本只用 root 参考**：18 义务槽方案 2985 bit / 374 bytes；**未混入 worker 另一配置 416/480 bytes**；明确标注为未对齐字段参考，非面积/净增量/全系统成本；软件可能 O(P)，未声称全系统 O(F+K)。
4. **A 不靠改安全需求造收益**：可信 DMA 无需关权时，可靠源进度可能已足够；普通 cursor/静态端点分段实现同合同视为最简实现，不作发表覆盖证据，不称新 min/区间算法。
5. **A 正例被强对照复现**：ordinary 同合同 cursor 复现 24 → 记"无独有系统收益"。
6. **B 数字与假设**：C0 36/46、候选与 C1 46/26、C2 36/17；邻反例变差；结论为停止独立路线，不建模型。
7. **C 数字与假设**：静态 miss 表与 makespan（260/240）被普通目的分组复现；score 单位 = dimensionless q × hold_cycles（**非 cycles×cycles**）；C 无独有正例；完整 COPY pre-VALID 重排不强制 4 backend/新 AXIID，仅真正多 parent child 交错另收费。
8. **创新表述形式**：使用"因为…所以不能把…作为独立贡献"或"如果…则可形成…"；未出现"尚不能称为独立创新"式空泛定性；未声称二区。
9. **唯一合法输出**：本文件为合法 JSON，仅含 proposal_markdown/current_status_markdown/handoff_entry/validation_entry/uncertainties；未用 markdown 围栏。

### 结果已知结论（非实验，为裁决结论）
- 候选 A：条件主线，设计 only；无独有系统收益证据。
- 候选 B：停止为独立路线；普通 walker 强对照。
- 候选 C：条件备选；无可证伪假设之外的独有正例。
- 推荐为**研究判别顺序**，非论文档次排序。

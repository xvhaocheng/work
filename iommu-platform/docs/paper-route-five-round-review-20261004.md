# Round 5 — 收敛结论（有停止门禁的可行性测试）

> Round 1–2已讨论新路线，Round 5仅解决审稿问题并收敛。目标是可否证的可行性测试。
> 现行约束：主方案 P2/14 + N 同 BIT；后备 B 仅接口证据、本批不实施。
> 完整交叉原文：[五轮证据目录](../out/paper-route-review-20261004/)。实施接口以[冻结规格](composition-v1-spec-20261004.md)为准。本文由DeepSeek起草，Codex审查并修正因果、统计与源码口径。

## 0. 定位
- 当前为**可行性测试 + 停止门禁**；机制不可分即停，不以体量替代判据。
- 主方案：同 BIT 四模式 0=S/1=N/2=P/3=PN；P 固定总 16、V(DID10)2、其余 14；不做 8/8 敏感性。
- **非安全隔离**：不改 PSCID/GSCID/global/stage 匹配与权限，不承诺共享映射独占。
- 后备 B：仅接口证据、本批不实施；无选择性失效接口或空命令复现即停。

## 1. RQ
在单一 BIT、同一 16 条 IOTLB 容量下，对 V(DID10) 划分 2 条保护份额（P）并允许合法命中先服务
（N），**“持续热 V 的容量保护/命中先服务结论”能否外推到间歇 V**？即 gap 存在时，P 是否仍必要、
N 是否仍产生可观测服务收益、P+N 是否超过 max(P,N)？

## 2. Hypothesis（可被否证）
- H1（P）：间歇 V 下 A 仍替换 V；P 减少 V 被A替换的风险/每请求首次 miss。
- H2（N）：N 使合法 V 命中产生真实 fast accepted service（非仅计数）。
- H3（交互）：P+N 优于 max(P,N) 需同时见条目存活改善 + 真实 fast 服务。
- 反例判据（不是统计零假设）：gap 阶段化后收益消失/反转。

## 3. Mechanism（规格级，代码由 Codex 冻结）
- P：用mask逐层限制tree-PLRU选择，V可填入2条、其余设备可填入14条；mask 全 1 逐拍等价旧 PLRU；不改匹配/权限/失效语义。
- N：复用已有有界非阻塞快路径，在主查表等待期间为另一bank的合格命中请求提供服务；并非一般多请求并发翻译器。
- 同 BIT 切换：D22 原子配置写（bits1:0 模式，bits31:8 保护 DID 只 10/11，其余 0）；硬件全静止才接受并
  同拍丢弃 IOTLB/PLRU；64 位 generation 饱和拒绝、不回绕；busy/非法写无副作用，记 last_status；
  软件 mutex + 硬件 quiet；新请求同周期阻止接受。
- 清除仅为微架构驱逐，不替代 IOTINVAL/IOFENCE；不得用 reset 快路径绕排空。
- V gap 复用每设备 gap 硬件，由新 ioctl 0x11 承载；只读 gap-active 记录真实等待。
- raw bank 扩到 128 words（D20–D9F）：64 标量 + 开始/结束各 16 条 128 位 IOTLB 快照；不受旧 64-word 限制。

## 4. Experiment（分阶段，合计 172）
1. Phase 1：4 模式×Solo/A1/A8/A16 短验收 **16**（前置门禁）。
2. Phase 2：S/S 6 对 **12**（A8/Vgap0），仅描述同设置散度。
3. Phase 3：4 模式×Solo/A1/A8/A16×Vgap0/1000/4000×3 交错 block = **144**（探索）。
规则：同 boot 配对、预设轮换、全失败保留、技术错误停、科学阴性继续。

## 5. Baseline
S/N/P/PN 必须同 BIT、同 boot、同仪器、同 512 拍缓冲、同一工作集/到达节奏、同 4 KiB 完整复制；
original 仅作总体开销锚，不混入因果表。

## 6. Metric
V 窗口周期；同窗 A 有效完成字节；V每请求首次 miss、填充位置、PSCID变化的有效条目替换事件（不冒充DID淘汰计数）；fast accepted 真实服务（单列）；
受保护份额违规事件；gap-active 周期；lookup/hit/miss 按 10/11/other；旧 HPM 仅原始保存；
S/S六对仅描述散度，不能直接保证最小可检测效应或显著性；正式样本量与判据须另行冻结。

## 7. Expected Result
- P 减 V 首次 miss/替换 且 N 产真实 fast → 进入有限正式设计（不自动新颖）。
- PN未提供超越较强单项的增量 → 删除“组合必要”；即使V更快，也须结合A同窗进度和完整结构开销，不能只取V一列。
- 机制不可分 → 不进正式试验，按门禁收束。
- 出现到达节奏反例 → 保留受控边界结论；不自动成为方法创新。

## 8. Threats
单 boot 探索无跨 boot 证据；n3 不能声称显著、不能据以发表；DID 不在 tag，P 未证保护匹配域；
fast accept≠hit rate、candidate≠probe；单 FPGA 单 IOMMU 不可泛化；B 未测。

## 9. Paper Contribution
hit-under-miss/分区/token bucket/纳秒级非创新。机制不可分或仅见常识 → 至多负面/表征边界；
仅当非平凡交互边界 + 条目级证据，才可评估为设计空间表征，**仍不承诺可发表**。

## 10. 阻断清单摘要（未验收项不得称已通过）
代码/编译（RTL 层次宽度域、PLRU 路径、DID 宽度匹配域、独立 ABI、D22/128-word bank、同拍清空与generation完成确认、
选择性失效接口只读确认、bitstream 编译）；
仿真（busy 拒绝、同周期请求、cache flush、全 mask 等价、P 边界、匹配/失效不变、V gap 窗口合法、
P4M bank 不变、generation 饱和、旧 0x0d 逐字节兼容）；
上板（四模式无跨区污染、P 条目级、N 真实 fast、PN与单项取舍、S/S散度、样本保留、V gap 真实等待、
交付物、B 去留）；第二平台（安全/QoS 外部有效性，仅标注）。

## 11. 状态汇总
已解决：P2/14、保护 DID 24 位固定 10、generation 64 位饱和、V gap 复用接口、计数口径、raw bank 128 words。
已删除/降级：8/8、安全隔离叙事、DID 分区保护匹配域、original=S、B 作主线、hit-under-miss 等创新主张。
待上板/编译：同 BIT 切换与静止谓词、同拍清空与generation完成确认、P/N/PN 条目级与真实服务、baseline 公平与 MDE、交付产物。
延后：B 选择性失效接口（只读确认，本批不实施）。
保留负结果：v5 仅证 N 可触发（A8 三轮 104.263/92.987/93.048）、N/S在A8中位+4.15%、A16中位−2.69%，未建立稳定改善；N相对原版A8−14.52%、A16−22.09%、原版复用作锚的局限。

## 五轮实际变化

|轮次|Codex/DeepSeek交叉产物|对路线的影响|
|---|---|---|
|1 独立提案|双方在交换前分别保存提案|包括保护组合、映射生命周期、调度到达节奏、共享翻译域、译后反压等方向|
|2 交叉攻击|双方指出新颖性、反事实、软件成本、共享域和不同BIT等混淆|删除“已有技术组合就是创新”、任意效应阈值和不成立的等价对照|
|3 重构|收敛主A与后备B|主A同BIT可分机制；B为选择性失效成本，暂不投入第二套实现|
|4 审稿人模拟|体系结构、OS、系统安全三个角度拒稿式审查|加入2/14强基线、完整静止切换、新ABI、实际gap计数；不作安全隔离主张|
|5 收敛|只解决第4轮问题|冻结172项分阶段探索及停止规则，开始实现；没有宣布创新已成立|

## 本轮新增原始文献核对

- [SP-IMPact作者全文](https://arxiv.org/html/2501.16245v1)：已有cache coloring/内存限流配置及交互分析框架。其展示评价明确未纳入MBR，不能说已完整实测本课题的所有交互。它削弱“交互分析本身新颖”的说法，不证明当前主方案已经被否证。
- [ShareMMU作者页面，MICRO 2026](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted)：已有不可信加速器间安全共享预翻译结果的方向，本次按作者摘要核对；不宣称通读PDF。
- [HyperTRIO作者机构摘要，ISCA 2020](https://collaborate.princeton.edu/en/publications/hypertrio-hyper-tenant-translation-of-io-addresses)：多在途翻译、缓存分区和预取已是先行方向；本次PDF直连失败，摘要与全文证据范围分开。
- 更早的HPCA 2017 hit-under-miss、RTCSA 2025 IOMMU干扰、MemCoRe等见[既有文献矩阵](research-literature-review-20261004.md)。没有证据表明所有候选“前人已经证明走不通”；能够确认的是若干核心手段早已存在。

## 当前实施状态

2026-10-04：关键RTL、新ioctl、CLI及172项Mac执行入口已实现，缓存/观测/前端/TL组合仿真及工具合同、Mac入口测试通过。Linux/OpenSBI完整构建完成，内核为 `6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty`。软件、脚本及SD写入入口已传Mac，未写SD、未上板。首次FPGA构建在布线期间停止，原进程已退出且未生成BIT，终止原因未确认；保留原日志与检查点，正在从已有布局检查点恢复。构建完成不能代替板测验收。

# 当前状态：ISPASS 2027 投稿模式

**目标：FPGA实测刻画论文，投稿ISPASS 2027。摘要：2026-12-11 AoE；全文：2026-12-18 AoE。** 正文9页、IEEE双栏、双盲。[投稿入口与交付](paper/ispass2027/README.md)。

2026-10-17（JST）。**历史G1九次已通过，统一100次基线已全部实际执行并入表。** 本次100次按DS50→DS0→DS50的25/50/25运行，跨3个boot；每个DS下Solo/A1/A8/A16/A64各10次。[新基线结果摘要及稳定行ID](paper/ispass2027/data/baseline100-summary.md)。共享IOMMU研究保持Codex-only。

## 交付与证据

- [E3负载能力与准备包](paper/ispass2027/exp/E3-workloads/capability.md)：实际pathv3/DS0 batch固定4KiB、V/A共享chunk，无法仅改测试程序实现指定64B/1KiB对4KiB子集。已按用户规则写[change-request](paper/ispass2027/exp/E3-workloads/change-request.md)并停止；三类流量未实现、新板测0次。完整矩阵及DS0论文候选150次（预算≤5.5小时）仅规划，[结果页](paper/ispass2027/exp/E3-workloads/subset-results.md)明确未能判断小传输趋势。
- 实板长表17404个标量行＝历史4096＋G1 828＋统一100次12480；行数不是重复次数。来源和确定性重建见[数据目录](paper/ispass2027/data/README.md)和[validation](paper/ispass2027/data/validation.json)。失效仿真980行独立成表，不计入实板。
- [前作/G2报告](paper/ispass2027/related/prior-work.md)已覆盖Bermuda、ETH全文及Rodríguez2024同平台硕论、HERO/Kurth前史等近邻；Procedia2025与Hur/Ro2025全文仍未取得，G2待补核，不宣称首次或无前作。[十图大纲与待补项](paper/ispass2027/outline.md)保持，尚未测的扩展不填结果。
- [G1配套包](paper/ispass2027/board/packages/g1-20261017/README.md)保存9次结果、失败/恢复链及历史dirty身份缺项；[baseline100配套包](paper/ispass2027/board/packages/baseline100-20261017/README.md)对应实际源码、同一payload和DS0/DS50 BIT。本次固定commit见[source.json](paper/ispass2027/board/packages/baseline100-20261017/source.json)，不以活动研究checkout替代实际输入。

## 后续执行方式与边界

- 当前板保持最后DS50＋pathv3 Linux，恢复模块未装入正式测量boot。未来默认AB50/50，同配置测完再切；任务匹配时复用有效的当前DS50 load，避免首轮重复加载。
- 沿[上板skill](paper/ispass2027/board/skill/genesys2-iommu-board/SKILL.md)直接传输、更新、启动、采集，不重复hash、SD回读或独立确认；结果、commit、image、BIT按实验包配套保存，本机二进制不随Git推送。
- 暂停新机制探索、新判别门及旧停止审查；设备数、负载/传输大小、页大小、延迟分布和缓解统一复测仅列待补。旧负结果、MIG及其他暂停分支不恢复。DS变化仍只说明附加延迟敏感性，不是纯PTW成本；行为DDR失效结果仍为仿真。

## 历史

切换前完整状态（含各机制裁决、独立上游贡献、GitHub同步和工程事项）保存在[原文快照](paper/ispass2027/provenance/current-status-before-submission-mode.md)，未改变历史结论或已执行身份。旧总计划不再调度当前任务。

历史版本与实验身份继续由[版本导航](iommu-platform/docs/versions.md)和各轮manifest定位；本次提交与推送由主会话收尾。

# Quality of service for input/output memory management unit

稳定ID：`iommu_qos_patent` · 类型：`patent` · 主题：`patents`

作者：Basu; LeBeane; Van Tassell；年份：unknown；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：公开披露PTW高低优先队列、静态分区、老化提升、按设备TLB分配以及命中率/利用率调优。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：公开技术披露不等于实测有效，不构成法律或专利有效性判断。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：patents近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://patentimages.storage.googleapis.com/b4/26/f7/c968e88b477679/US20190384722A1.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-literature-review-20261004.md:24](../../iommu-platform/docs/research-literature-review-20261004.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-literature-review-20261004.md:24](../../iommu-platform/docs/research-literature-review-20261004.md)：

| US11144473B2 / US20190384722A1 — Quality of service for input/output memory management unit（Basu, LeBeane, Van Tassell） | https://patentimages.storage.googleapis.com/b4/26/f7/c968e88b477679/US20190384722A1.pdf | `out/method88-literature-20261004/web-evidence.json`；`out/method88-literature-20261004/supplement.json`；PDF 14 页 / 776 行，行锚 L531@P10–L562@P10、L546@P10、L549@P10–L553@P10、L622@P11–L630@P11、L680@P11–L691@P11、L717@P12–L763@P12 | 公开披露：高/低优先级 PTW 队列与静态分区（L531@P10-L548@P10）；按设备 allocate/no-allocate 到共享 L2 TLB 的策略（L555@P10-L562@P10）；**老化**：等待超过预定时间的低优先级请求被提升/优先选择（L549@P10-L553@P10）；周期性 hit-rate 测量（例如每 100 µs）并据此调优先级（L622@P11-L630@P11）；按 PTW 利用率调队列（L717@P12-L719@P12） | 专利只记录公开的技术披露，**不证明实测有效**，也不构成法律结论（`out/method88-literature-20261004/web-evidence.json`） | 重叠：优先级/利用率/老化三种资源分配思路已被公开。区别：公开披露 ≠ 效果证明；本课题不得以增加优先级本身当新意 |

[返回总目录](../README.md)

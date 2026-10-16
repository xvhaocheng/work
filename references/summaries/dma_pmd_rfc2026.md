# DMA_PMD RFC 00/22及已核02/04/05/07/08补丁

稳定ID：`dma_pmd_rfc2026` · 类型：`rfc_patch` · 主题：`mapping-lifecycle`

作者：unknown；年份：2026；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：公开补丁按allocator-owned PMD pool复用长期domain映射；首次按domain建图、bitmap管理；最后block全idle后在同锁下阻止重分配，worker卸全部domain图后归还buddy；含watermark、cap和shrinker。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：RFC公开源码未合入/未在本轮运行；整PMD/all-domain退役不等同普通page_pool页保留A/source而单独撤B。镜像06299不是新修订。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：mapping-lifecycle近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://lists.openwall.net/linux-kernel/2026/10/03/1066)
- [来源2](https://lists.openwall.net/linux-kernel/2026/10/03/1068)
- [来源3](https://lists.openwall.net/linux-kernel/2026/10/03/1070)
- [来源4](https://lists.openwall.net/linux-kernel/2026/10/03/1073)
- [来源5](https://lists.openwall.net/linux-kernel/2026/10/03/1079)
- [来源6](https://lists.openwall.net/linux-kernel/2026/10/03/1082)
- [来源7](https://lkml.rescloud.iu.edu/2610.0/06299.html)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/mapping-evidence-input-20261016/pmd/findings.md:9](../../iommu-platform/out/mapping-evidence-input-20261016/pmd/findings.md)
- [iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md:13](../../iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md)
- [iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md:35](../../iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md)
- [iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md:15](../../iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md)
- [iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md:17](../../iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md)
- [iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md:14](../../iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md)
- [iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md:16](../../iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md)
- [iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md:18](../../iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/mapping-evidence-input-20261016/pmd/findings.md:9](../../iommu-platform/out/mapping-evidence-input-20261016/pmd/findings.md)：

唯一含运行数字的来源为旧目录的 rfc00.txt，原始归档是 [RFC 00/22](https://lists.openwall.net/linux-kernel/2026/10/03/1066)。本轮复核全文 1–264 行、正文全部外链以及表格/patch 清单；没有重读全部 patch 或驱动。HTML 原始 17,359 bytes，SHA256 `d54460e2a532bd265088e9e35adf32c2c4739e8d52e03a30356e2446757c8655`，txt 13,087 bytes，SHA256 `7c37f40c618a4ef5c1dd32a0d86796cefe39a0eff519a685f97eb73cb666d064`，均与前轮 manifest 一致，未重复下载。

[iommu-platform/out/mapping-admission-20261016/damn/findings.md:48](../../iommu-platform/out/mapping-admission-20261016/damn/findings.md)：

[镜像06299](https://lkml.rescloud.iu.edu/2610.0/06299.html)的HTML `X-Message-Id`为`20261003212241.3432303-9-lrizzo@google.com`，与上轮保存08/22一致；不是新修订。去掉每个非空行所有空白后，`diff --git`到git版本尾的SHA256双方同为`ca541fa0e6b1fcc30205776d0bdcfa560e9e2bb287cd42ebcbaaf4d89e75e13c`；见`mirror-comparison.json`。HTML archive时间为2026-10-03 17:30:58 -0400，不能拿归档时间代邮件的21:22:27 UTC发送时间。镜像body `dma-pmd-mirror-body.txt`的首次use/bitmap事实，与上轮`mapping08.txt:25–40,378–402`相同；首次按domain建图并保留已经存在，未扩展抓取22项。

[iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md:35](../../iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md)：

| [DMA_PMD RFC](https://lists.openwall.net/linux-kernel/2026/10/03/1066)，00、02、04、05、07、08/22 | 大页复用、域追踪和上述回收控制均已有；08/22 是整页 RW／pool unmap no-op | 不认领这些功能。拟研究的是晚路由下授权可选集合、整页所有权转换及全费用；不能拿更宽权限的配置当同合同赢家 |

[iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md:13](../../iommu-platform/out/mapping-organization-source-20261016/dma-pmd/findings.md)：

| rfc00 | 2026-10-03 21:22:19 UTC，Luigi Rizzo，RFC 00/22 | https://lists.openwall.net/linux-kernel/2026/10/03/1066 | rfc00.txt:2–4,22 |

[返回总目录](../README.md)

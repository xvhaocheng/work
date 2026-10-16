# 现成输入验收：找到真实公开数据，尚不能填入当前网络切面

2026-10-16 JST。**MAP-EVIDENCE-INPUT-1完成。新增取得了具名公开数据集的有限原始字节，而非又一份观测合同；本轮仍未取得固定v6.12工作流的合格L0或完整CPU输入。** 当前保留V1-N/K低完整实现成本整合假设，不新增净收益、新机制或共享IOMMU干扰主张，也不以缺输入否定映射组织思想族。

## 1. 实际找到和读到了什么

| 具名入口 | 实际验收范围 | 可用结论与限制 |
| --- | --- | --- |
| DMA_PMD RFC 00/22 | 复用完整cover bytes，枚举两条正文背景外链；核作者ARM双向流量的吞吐/PMCG样例 | 仅作者聚合参考。未提供逐B map/source episode和CPU执行边界；TX segments同时改变，不能把汇总差额归因B映射 |
| RFC指向的“IOTLB wall”论文与Zenodo7727193 | 新取论文HTML、数据集元数据、归档前8 MiB；列出前70个member，其中最后一个不完整；提取三份完整小文本 | 确有可取得的公开数据；所读样本不是本工作流逐页面映射事件。其余归档未审查，不声称没有相关字段 |
| DAMN，ASPLOS2018 | 原文配置/测量段与Tsafrir、Morrison的对应完整作者条目及全部href | Linux4.7、ConnectX-4、TCP/应用历史参考；所查条目未给论文专属code/raw数据指针，非全球不存在证明 |
| DMA shadowing，ASPLOS2016 | 同上，复用既有原文，不重下PDF | Linux3.19、被测Intel Fortville，发送端ConnectX3；历史平均处理费用不是当前v6.12跨设备转发费用 |
| RFC第一条lore邮件 | HTML与/raw均HTTP200，但实际是访问验证页 | 未取得邮件正文；邮件内是否另有artifact保持unknown，不能把200当读到来源或把失败当无数据 |

[PMD验收](../pmd/findings.md)与[论文入口验收](../paper-artifacts/findings.md)保存出处链、具体行号及scope。派发中将shadowing误标EuroSys的错误已按原文纠正为ASPLOS2016，旧原件不改写。没有重新扫描论文库、全仓或全部patch。

## 2. 新公开数据的实际内容与准入

来源链为已存RFC `rfc00.txt:133` → [论文](https://pmc.ncbi.nlm.nih.gov/articles/PMC10280580/) Data Availability → [数据集](https://zenodo.org/records/7727193)。论文明确主实验Linux5.15、iPerf TCP，IOTLB计数按秒采样；native-XDP A接收后选择B转发/本地UDP不是该主实验。见[保存原文](../iotlb-wall/pmc-article.txt:138)与[数据指针](../iotlb-wall/pmc-article.txt:413)。不将同系列NIC当同一部署。

数据集发布的`raw-data.tar.gz`为791,012,771 bytes。本轮仅HTTP206读取压缩前8,388,608 bytes；解压得到8,705,018 bytes的前缀，gzip尚未结束。**未取全包，也未验发布的整包MD5**。前缀SHA与3份完整member各有本地SHA；这是HTTPS取得字节的定位/完整member检查，不能冒充整包完整性证明。未打开任何PDF图或运行归档中的代码。

选择规则预先限为前缀中首个完整小CSV、首个CPU CSV、首个DAT，未按数值有利与否挑选。实际内容：

| 提取样本 | 原member中的类型/实际结构 | 能否回答当前问题 |
| --- | --- | --- |
| [首个CSV](../iotlb-wall/samples/first-csv.csv) | `...Luigi-patch.../csv/2OBWRITE-avg.csv`；8行，每行3个数，无字段头 | 可定位原图表数据；单位、列语义不能只靠名字补全。无B/PFN/source代际/调用结果 |
| [首个CPU CSV](../iotlb-wall/samples/first-cpu-csv.csv) | `...icelake-iommu-mtusize/iommu-off/csv/8192CPU-soft-avg.csv`；36行，每行6个数，无字段头 | 名称指向CPU分类统计；不是逐DMA调用entry/return，不能从这些数反推完整B map费用或其占比 |
| [首个DAT](../iotlb-wall/samples/first-dat.dat) | `...multirate/tmp/iommu-off-100-lro-on-1024-pps-stats.dat`；含X及median/min/max/mean头，8行 | 是统计输出，不是逐包或逐页事件。文件名与通用列头本身不足以唯一确定所有单位 |

原member名、tar偏移、尺寸与字节hash见[样本manifest](../iotlb-wall/sample-manifest.json)，完整检查范围见[前缀目录](../iotlb-wall/archive-prefix-inventory.json)与[HTTP回执](../iotlb-wall/archive-prefix-receipt.json)。本轮不计算新速度比、复制阈值或假想r；三样本也不代表对整包的随机或穷尽审计。

## 3. 这改变什么，不改变什么

改变的是输入来源状态：现在有一个确实可取得、可定位到member的公开数据集，可以支持其自身身份下的复核；不能再笼统写成“没有公开数据”。但**公开raw-data标签不等于逐事件轨迹，CPU分类统计不等于DMA完整费用，同NIC家族不等于同消费者/驱动切面**。只有两篇旧论文图表或RFC吞吐样例，也无法补出缺失的source/config episode。

所读原文/样本缺少当前成功B map、跨recycle同source代际和完整CPU边界，故当前这些数值仍为null。没有把非匹配输入算成0次再用、0收益或失败实验；没有用旧M0/E3、说明PMD费用账或这些历史数字填它们。L0即使以后成立，也不自动升级accept/CQ/有效完成。

V1-N/K仍指固定B/config、K个预留lazy sparse关联、满额O0、无独立idle eviction，借真实frame/全部片段寿命保护在途。相对[普通R](../../mapping-integration-delta-20261016/owner/integration-analysis.md)和[dma-buf slot/聚合适配](../../mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)的完整实现成本差异，本轮没有新增测量证据。普通O3同信息/同付费接口策略等价仍只限制策略主张，不证明接口已发表、全实现同成本或思想族无价值。物理S/P/C/Q、metadata/PTE、IOVA、带宽和有效完成继续分报；stock无idle B pin，不新增天然payload节省主张。

## 4. 本轮结束条件与最小后继

**现成输入的这批明确入口已完成有界验收，不再自动排队同样的搜索、字段表或条件数字账。** 当前原生收益证据未就绪，保持条件设计资产；不是要求用户重新确认授权、CPU/内存价值或保证痛点。

下一动作由真实新输入触发：若出现可定位的匹配事件记录及配置，先按既有L0规则验收；要判CPU收益则另有真实执行边界。若仅有其他身份资料，先明确新的比较合同，再仅解释它实际支持的结论；不悄悄更换v6.12工作流。若今后扩大本归档检查范围，必须指定具体待解决问题与member，不能只因整包很大就继续下载/扫目录。

[后继触发记录](next-minimal-contract.json)不排队新采集/collector/驱动/RTL/完整性能系统、实验或板测。不自动升位B1/I/D/A5/A2；MIG1792023314继续paused、原10月15—28日窗口保持。独立工程的并发事实在轮末重读合并，未选论文主线。

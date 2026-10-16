# DAMN／DMA shadowing：现成输入定向验收

结论：本次检查了既有两篇论文的代码／数据类指针，以及两位合著者页面中各自对应的条目；没有取得能准入当前 v6.12 mlx5e native-XDP 工作流的原始事件或完整 CPU 记录。两个作者条目均只给论文 PDF／DOI（另有摘要、BibTeX 展开），没有可继续跟随的论文专属 code/raw-data 入口，因此实际跟随实质仓库／数据入口为 **0**。这是限定范围内的缺项，不证明论文在其他位置从未提供 artifact。

本席按 ARS-Codex deep-research 的来源核查方法执行；请求 Sol/high，独立实际模型和强度元数据 unknown。没有使用 DeepSeek／Claude。代码可复现、配置描述、论文汇总图与原始逐事件测量四者分开，不从前两项推出后两项。

## 取得了什么

| 来源 | 取得／复用的输入 | 与当前身份的差异及可用范围 |
|---|---|---|
| DAMN，ASPLOS 2018 | 复用原 PDF/text；新取得作者条目 HTML 原始 bytes。§5.7／§6／§6.1 报告 Linux 4.7、ConnectX-4、netperf TCP RX/TX、memcached，60s、10次平均；图中为 throughput／CPU 汇总。 | 属于修改过 allocator/TCP stack 的历史具名测量参考。同为 ConnectX 不能证明当前 v6.12 mlx5e cyclic/linear/order0 native-XDP A→B 的再用率或 CPU。 |
| DMA shadowing，ASPLOS 2016 | 复用原 PDF/text 和旧 HTTP receipt；新取得作者条目 HTML 原始 bytes。§6 报告 Linux 3.19、被测 Intel Fortville LX710 40GbE，发送端 ConnectX3，netperf／memcached；有平均 processing-time 分解。 | 接收端不是 mlx5e。历史均值不是本工作流的 source/config 生命周期记录，也不能恢复当前完整 map/unmap CPU。 |

原文行入口：[DAMN 配置与方法](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/papers/damn2018.txt:474)、[shadowing 实现](/home/xvhaocheng/work/iommu-platform/out/mapping-organization-source-20261016/root/sources/shadowing2016.txt:399)、[shadowing NIC／方法／平均分解](/home/xvhaocheng/work/iommu-platform/out/mapping-organization-source-20261016/root/sources/shadowing2016.txt:460)。没有把发包机的 ConnectX3 当被测 NIC。

身份更正：既有 `shadowing2016.pdf/txt` 是《True IOMMU Protection from DMA Attacks: When Copy Is Faster Than Zero Copy》，ASPLOS 2016，DOI 10.1145/2872362.2872379；派发中的 EuroSys2016 是误标，已向 root 报告并获接受。[原文版权／会议信息](/home/xvhaocheng/work/iommu-platform/out/mapping-organization-source-20261016/root/sources/shadowing2016.txt:48)。旧文件未改写。

## 指针检查到哪里

1. [Dan Tsafrir 首页](https://dants.github.io/) → [论文列表](https://dants.github.io/index_pubs.html)：只检查 DAMN 和 CIM 两个完整条目的链接。原始 HTML DAMN 位于 2726–2856 行，shadowing 位于 3964–4099 行；各只有摘要／BibTeX 的本页展开、PDF、DOI。完整 `li` 的全部 href 已机械抽取至 [link inventory](/home/xvhaocheng/work/iommu-platform/out/mapping-evidence-input-20261016/paper-artifacts/raw/paper-entry-link-inventory.json)，包含隐藏展开区。对应可直接复核的 availabilities：[DAMN](/home/xvhaocheng/work/iommu-platform/out/mapping-evidence-input-20261016/paper-artifacts/raw/tsafrir-publications.html:2755)、[shadowing](/home/xvhaocheng/work/iommu-platform/out/mapping-evidence-input-20261016/paper-artifacts/raw/tsafrir-publications.html:3993)。
2. [Adam Morrison 作者页](https://www.cs.tau.ac.il/~mad/)：DAMN 原始 HTML 390–394 行、shadowing 415–419 行各只有论文 PDF 链接。[DAMN](/home/xvhaocheng/work/iommu-platform/out/mapping-evidence-input-20261016/paper-artifacts/raw/morrison-home.html:390)、[shadowing](/home/xvhaocheng/work/iommu-platform/out/mapping-evidence-input-20261016/paper-artifacts/raw/morrison-home.html:415)。
3. 两篇已存 text 定向搜索 artifact/source code/raw data/availability/HTTP/Git 等指针表达式，保存命中行至 [pointer search](/home/xvhaocheng/work/iommu-platform/out/mapping-evidence-input-20261016/paper-artifacts/raw/paper-pointer-search.json)。DAMN 的 GitHub 出处是通用 fio、netperf 和被引用的上游 Linux commit；不是这篇论文的 raw measurement。shadowing 命中是 DOI、文献及背景接口。没有跟随这些通用 benchmark／文献链接，也没有重新精读整篇或重新比较新意。

三份新增作者页均 HTTP 200；原始 bytes、hash、HTTP headers／时间与最终 URL 均在 `raw/*-http-receipt.json`。论文 PDF 复用旧 bytes，没有重新下载。SHA-256 与读取段见 [source manifest](/home/xvhaocheng/work/iommu-platform/out/mapping-evidence-input-20261016/paper-artifacts/source-manifest.json)。

## 准入和缺项

两份论文均只能作为其身份下的历史结果／配置参考。当前匹配原始输入计数为 0，当前 successful B maps、跨 recycle B 再用率和完整 CPU 值保持 **null**；不是 0% 再用或失败测量。

本席没有取得 source/config epoch、B map 调用来源和成功返回、丢失／删失／IOVA ABA 配对，也没有当前 DMA frontend 真正 entry/return 的执行 CPU 记录。因此不能计算合格当前 L0，更不能升级为逐帧 accept/CQ、完整 CPU 或收益。代码即使在未查的位置存在，也不能自动补齐既有 raw 数据。

检查范围没有覆盖：其他作者个人档案、整个 GitHub、出版社补充材料页、未链接服务器目录、私人存档／联系作者；不声称穷尽公开 artifact。没有访问失败，本次未取得入口与“访问失败”无关。没有运行 artifact 代码、clone、原生实验、采集／probe／collector、驱动／RTL／ABI 修改或板测。没有更改 canonical 文档和旧研究裁决。

完整机器卡：[input-card.json](/home/xvhaocheng/work/iommu-platform/out/mapping-evidence-input-20261016/paper-artifacts/input-card.json)。交回 root 的接受建议是“限定来源缺项＋具名历史参考及明确迁移限制”，不自动推进开发或重复旧合同。

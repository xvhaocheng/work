# DAMN关键方法核查与本候选的实质比较

2026-10-16 JST；范围按本轮steering-contract，独占DAMN与单一DMA_PMD新镜像。请求Sol/high，实际独立运行模型/强度元数据unknown；未用DeepSeek/Claude。

结论：DAMN已经实现按设备/权限分配、长期映射的独立网络内存、分层缓存和引用计数复用，并明确把CPU首次读取的字节复制到设备不可访问的内存。不能再将这些动作本身当新增。当前候选改问“路由后是否值得把既有源extent接纳到目标domain并保留，或短map，或复制到目标紧凑池”；这与DAMN给出的分配/保护策略有具体问题差别，但该差别本身不证明方法新颖或超越普通累计费用规则。

## 原文入口、核读方式与身份

论文：Alex Markuze、Igor Smolyar、Adam Morrison、Dan Tsafrir，*DAMN: Overhead-Free IOMMU Protection for Networking*，ASPLOS 2018，DOI 10.1145/3173162.3173175。官方作者PDF：[DAMN](https://dants.github.io/papers/damn-asplos-2018.pdf)。复用已有 `out/shared-translation-literature-20261006/papers/damn2018.pdf/.txt`；完整本地文件hash见manifest。本次未重新下载PDF，官方web取得的元数据/正文与本地标题和方法相符，但不声称远端字节hash已核相同。

已有layout文本会把左右栏并在同一行，因此本次在本目录重新生成`damn-raw.txt`及按PDF页编号的`page-XX.txt`。下面pN均为PDF第N页（1-based），不是网页行。精读§2（p2–3）、§5.1–5.7（p5–8）、§6.1关键huge/dense变体（p10）、§6.2–6.4（p11–13），并查看p5–7渲染确认接口表、双栏和shrinker措辞。不是只读摘要；§6其他图表不作全面重审，代码仓库与实际系统未运行。

## 逐项核查

1. **分配时指定设备和权限，释放时不要求调用者知道设备。** §5.1 p5，`page-05.txt:73–87`；Table 2 p6，`page-06.txt:1–8`。`damn_alloc(dev, rights, size)`及pages版本增加device pointer与rights bitmask，权限可read/write/both；NULL device走普通kernel allocator。free内部找回元数据。该接口的已知设备是在分配时传入，不能免费推定它知道RX之后才发生的下一跳路由。

2. **池化隔离和长期映射是核心，不要求每buffer unmap撤权。** §5开头p5，`page-05.txt:42–57`；§5.3 p6，`page-06.txt:9–26`。DAMN管理的页只用于给相应设备授权的网络数据；map查已有IOVA，识别DAMN IOVA的unmap立即返回，后续网络子系统再释放buffer。非DAMN buffer回原DMA API。原文在§1 p2，`page-02.txt:39–48`解释避免与无关敏感kernel data同页。它的安全合同不能被误写成“unmap即结束所有设备访问”。

3. **分层缓存、按device/rights/NUMA组织及refcount生命周期均已实现。** §5.4 p6，`page-06.txt:28–58,68–90`：每device/access-rights组合一个DMA cache；底层缓存物理连续chunk，Linux原型C=16即64KiB，顶层per-core bump allocator；区分页对齐/非对齐请求，按NUMA组织。每次分配/释放增减chunk refcount，零引用返回底层缓存（:68–79）。p7 `page-07.txt:22–61`说明LIFO magazine/global depot、缺页时再分配、普通/中断上下文两套cache；cache初始为空，depot取得chunk时清零并map。因此“需求到达才扩池”“缓存复用”“零引用才回收”“NUMA感知”已有，不能泛称它没有在线或占用相关动作。

4. **读取点复制的作用是防TOCTTOU，不是优化B设备映射接纳。** §5.2 p5，`page-05.txt:88–109`：拦截skbuff访问API，在OS首次访问某字节范围时复制到设备不可达的新buffer并调整skbuff。一般只读header，payload在kernel/user已有复制处脱离设备。§5.6 p8，`page-08.txt:9–23`分别论证TX read-only/初始化零化与RX“OS读取字节必须复制”的一致视图。不能把这个copy直接等同于当前候选向B可DMA紧凑池的copy：目的内存权限和后续用途不同。

5. **威胁模型及零复制边界明确。** §2.1 p2–3，`page-02.txt:94–104; page-03.txt:1–10`：攻击者控制DMA设备，OS/driver/IOMMU可信，不能伪装设备DMA；已授权内容篡改等MITM问题由其他机制处理，设备对设备P2P攻击不在本文目标。§2.2 p3，`page-03.txt:11–40`：面向POSIX endpoint与kernel/user复制；sendfile和zero-copy IP forwarding不适用，回DMA API-based protection；RDMA/DPDK等kernel bypass也不在范围。不能据“zero-copy forwarding回退”就证明DAMN对所有新XDP配置不可能适配，更不能说只要加入一个XDP hook就已覆盖新的保护证明。

6. **Linux部署和本轮XDP输入不是同一实现。** §5.7 p8，`page-08.txt:24–54`：扩展`__alloc_skb`传device，TCP socket可提供device，DAMN-aware flow经`dma_alloc_skb`；§6 p8 `page-08.txt:75–82`明确评估Linux 4.7。本轮输入是官方v6.12 mlx5e native XDP、先直接读包/FIB、再redirect或PASS。本席不独立复核v6.12调用链，准确source行由root接入`v612-source`产物；这里的适配结论限于：**如果原生XDP读取没有经过论文的skbuff accessor，那么DAMN原有拦截点本身不足以证明这些读已获得其TOCTTOU快照保证**。需要覆盖实际data/data_end直接load、重复读和header写等边界，或明确合同T的信任假设使其不再是同一安全目标；这是条件推断，不是已复现XDP漏洞。不得用旧版源码行号冒充v6.12验证。

7. **内存压力：有方案描述，研究原型明确未实现shrinker。** §5.4 p7，`page-07.txt:62–72`写出memory-pressure回收magazine/depot的空闲chunk，同时明确prototype没有实现该功能。§6.3 p12，`page-12.txt:33–41,127–135`又以压力到来调用shrinker的预期行为解释长期占用，不能覆盖前面的实现限制。§6.3实际报告netperf期间内存与普通Linux相近、结束后cache占用保留（:109–135）；这不是已实施pressure实验，也不是已完成有界退役服务。原文的空闲定义指不再有allocated/active DMA buffer，不能据此说其长期IOMMU leaf早已消失。

8. **已有IOTLB footprint/IOVA布局近邻不可忽略。** §6.1 p10，`page-10.txt:115–134`和Table 3 p11，`page-11.txt:58–66`：作者分析2MiB huge IOVA pages，以及metadata高位编码把不同cache的IOVA分散到不同大页、削弱IOTLB效果；dense IOVA减少大页数量。关键限制是其分析变体放弃IOVA中metadata编码，无法free，明确“analysis only”（p10:129–132）。因此它已经认识到池和IOVA组织影响翻译footprint；不能说论文只讨论map/unmap CPU，也不能把此分析变体当完整可部署系统。后续若普通对照补足可释放metadata路径，须计维护成本，不能以“论文没贴该API”自动得新意。

9. **在线接纳/累计费用的确切核读范围。** 本文实际策略是：分配时按已知device/rights选择cache、按chunk供给及refcount/magazine回流、每次depot取得新chunk时初始化映射，读取点按安全需要复制。其生命周期/占用/翻译footprint意识已经见上3/7/8；但核读的§5完整方法和§6.1/6.3没有给出“路由后，对既有源extent×目标domain×generation，根据历史累计费用在copy、短map、保留map之间选择”的在线决策器。这里只限定**这篇论文已核方法**，不推出DAMN代码/后续工作或全球文献没有类似policy。映射admission、缓存替换和累计费用法的更广近邻仍是root后续novelty判断所需材料；本席没有做该领域全面搜索。

## 与本轮候选的具体对照

| 维度 | DAMN已核方法 | 本轮候选（仅任务合同） | 对新意的限制 |
|---|---|---|---|
| 决策对象/时机 | allocation时device/rights cache，CPU首读时复制 | 路由后既有源extent→目标domain的copy/短map/持久接纳，按generation防旧状态复用 | 问题索引和时机有差别，缺字段/API不构成方法贡献 |
| 复制目的 | 设备不可达内存，保证OS读取视图；通常结合user copy | 可由目标B DMA的紧凑池，减少源区对B的映射/footprint | 不是同一种copy，但都必须作为普通可执行动作计入对照 |
| 持久状态 | 每device/rights/NUMA缓存、IOVA metadata、chunk引用 | 源区/目标domain/generation及付费可得历史 | 多一个key或history计数本身是普通工程构造 |
| 生命周期/内存 | chunk refcount；空闲层次缓存；shrinker为未实现提案 | 引用、DMA完成、源/目标pool、映射有效期及退役全账 | 不能称“首次考虑生命周期/footprint”；若费用只在候选新增则比较失真 |
| 保护合同 | 恶意NIC下的OS读取快照＋专用数据范围 | T允许专用池范围给A/B持续授权；S另列条件 | 合同不同不能直接比较同等安全或借旧论文性能外推 |
| 可用结果 | Linux4.7 endpoint评估；zero-copy IP forwarding明确回退 | v6.12混合XDP redirect/PASS，尚非本席实测 | 适配新路径不是自动novelty，需优于具名同信息普通策略 |

因此可保留的精确问题是“目标domain的源extent映射值得何时建立/保留”；不是“首次按设备/权限分池”或“首次按需建图”。如果同信息普通累计费用选择已作相同决定并计足生命周期成本，此实例最多支持部署建议；本席不替root裁决该等价性或设计新规则。

## DMA_PMD新镜像核验

[镜像06299](https://lkml.rescloud.iu.edu/2610.0/06299.html)的HTML `X-Message-Id`为`20261003212241.3432303-9-lrizzo@google.com`，与上轮保存08/22一致；不是新修订。去掉每个非空行所有空白后，`diff --git`到git版本尾的SHA256双方同为`ca541fa0e6b1fcc30205776d0bdcfa560e9e2bb287cd42ebcbaaf4d89e75e13c`；见`mirror-comparison.json`。HTML archive时间为2026-10-03 17:30:58 -0400，不能拿归档时间代邮件的21:22:27 UTC发送时间。镜像body `dma-pmd-mirror-body.txt`的首次use/bitmap事实，与上轮`mapping08.txt:25–40,378–402`相同；首次按domain建图并保留已经存在，未扩展抓取22项。

## 局限与交接

完整PDF方法不等于验证代码实现。DAMN复用本地PDF未重新下载，未运行其Linux4.7系统；性能与内存结果均作者报告。v6.12 XDP源级事实待root接入，不将条件推断升为已验证部署；T/S部署选择、跨domain共享IOMMU服务、在线费用/复用/footprint及普通规则等价性均不由本席确认。没有构建、模型、实验、生产代码或全局文档修改。所有新产物仅在本目录。

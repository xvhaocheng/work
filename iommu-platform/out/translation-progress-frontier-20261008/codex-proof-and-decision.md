# §45 root：受限证明与创新判断

作者/责任：root Codex亲自设计合同、证明、审查原文和实际代码；两名Codex研究席提供关键质疑。DeepSeek起草并修订执行脚本。本文件是架构/分析审定，不是板上结果。

## 1. 精确命题

采用`codex-model-contract.md`全部限制。每个请求正常固定15笔；PTE读只能推进当前请求；没有PWC/跨请求共享/外存spill/免费保留旧checkpoint。令所选具体保存族在预算B下能保存的**当前**位置为S_B，再加入0与终态15；令相邻位置最大间距为G(B)。

**存在使两请求都完成的合法策略，当且仅当q≥G(B)。存在时最少共读30笔。** 这是列明token族的必要充分条件，不是任意B-bit编码或任意微架构的下界，也不是所有公平策略的完成保证。

## 2. 必要性：为何restart不能跨越缺口

设相邻可保留点a<b且b−a>q。假设有第一次完成，考察该请求首次穿过(a,b)的那一turn。在第一次完成前，两个请求都未完成，必须遵守q。每turn开头从原请求0或已付费保存token恢复，因此起点在S_B。首次跨越时起点不能在(a,b)，且不超过a；最多读q笔不能到b。结束在缺口内也只能保存≤a的当前semantic-root或重发0，无法把缺口内位置带入下一turn。矛盾。

‘对方完成后不再强制交接’不破坏证明：这要先存在一次完成，但同样的缺口排除首次完成。反例cycle只是可见无进展轨迹；必然无解靠上述证明与全图耗尽，不靠看到一个cycle。

## 3. 充分性与工作下界

将S_B排序为0=s0<…<sk=15。q≥每个相邻差值时，当前请求只服务到下一checkpoint就主动交接；不必用满q。每次交接保留当前位置而不回退。两个请求交替得到服务，有限个checkpoint后完成；其中一个先完成则另一个可继续完成。不存在额外未计费历史，交换只需合同中的active与单个停车帧。

每笔模型读取最多增加当前请求进度1，任何回退只增加未来工作。因此每请求至少15笔，两请求至少30笔。上述策略恰读30笔，达到该合同内的下界。物理PTE地址并未假设必不同；若允许共享同一PTE读的结果、coalescing或cache，30不再是一般下界。

## 4. 最强普通中间点

q=3可用{3,4,7,8,11,12}这些checkpoint：按3、1、3、1、3、1、3笔推进。完成G子翻译后保存映射；下一turn只读一笔S，然后主动让路。固定三层链最大payload74+tag3=77bit，原请求/context、owner/valid、active、在途和terminal另计。这个位数是具体codec上界，不是面积或一般最小值。

q=4时36bit semantic restart族可按4、4、4、3笔推进而不重读。q=3时该族的0→4缺口无法跨越，两请求每轮即使PTE服务公平仍可无进展。两者不能用‘普通一定每轮用满q’人为混淆。

固定路径4bit phase标签的mixed族81bit支持逐PTE保存；通用27-state、5bit标签的full_current族需82bit。这1bit差异来自合同范围和codec，不是新机制。全阶段静态normal轨迹不验证future fault、大页或动态失效。

两席另构造q2专用普通点：0→1→3→5→7→9→11→13→15，七个内部记录状态只需tag3，加最大payload77得80bit上界。root核此路径相邻距离≤2且对应当前token，不需要额外旧Q，因此接受为纸面普通构造；它未加入五族图运行，更进一步说明五族曲线不是全部普通表示的完整frontier。不得把mixed族81bit当q2一般下界。

## 5. 关键原文直接审定

root实际读取三份原文的关键段落（本轮本地primary副本）：

- Barr2010，`translation-cache2010.txt:260–385`，§3.2/3.3/4.2与表1/2：virtual-prefix partial translation、整path中间PPN、跨层槽、按reload cost/level保留，以及tag/data分别计费。
- Bhargava2008，`nested2008.txt:374–408`，§4.2：NTLB保存guest-PTE的G映射，用于跳过完整nested子walk再访问S页表；该具体保存语义覆盖q3构造的普通能力，但原文没有77bit/q3暂停保证。
- Hong–Kung1981，`hong-kung1981-raw.txt:85–143`，§2：固定DAG、S words fast store、任意slow store、hold/drop/recompute/I/O。它不能直接推出本模型的phase-dependent bit/quantum/完成定理，也不能仅据一般pebbling声称全部nested frontier已有。

精确来源与SHA见`primary/sources.json`，两席对范围的异议见`map-proof-agent.md`、`rules-proof-agent.md`。Rocket2022逐字段对应沿用§44已亲核证据，不重复下载/读取。

## 6. root停止/继续门

因为最强普通checkpoint策略在本合同内达到30笔下界，固定链上的新调度不可能再减少PTE工作；若只是选择保存位置、提前让路或改变phase编码，就不能称独立机制或新理论。不要编译这个容量阈值扫描，数学图已足以判别。

如果转向未知路径、持续到达或外生强制交接，必须先证明候选在同退休信息、同语义、同总资源下达到ordinary dynamic checkpoint / variable-size cache / pinned PWC做不到的点，才可继续主张新贡献。普通可以观察当前phase/level、早yield、保护一项已完成G映射，不能禁止这些能力制造差距。

‘q是最多连续服务’与‘外部能在任意当前PTE后强制打断’是不同合同。后一合同若允许永远在不可保存位置打断并丢弃进度，公平服务也不保证完成；提高保存能力或规定最小可完成段属于普通解法。只更换这个合同不产生创新。

下一主线仍是共享IOMMU翻译干扰，判别点是**真实请求下普通共享状态保留与服务策略是否存在不可兼得的约束**；不能以静态15步图曲线作为论文核心。当前尚未给出普通达不到的成本点，此分支的独立创新判断为否。

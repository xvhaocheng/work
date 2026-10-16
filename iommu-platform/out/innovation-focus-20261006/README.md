# 创新定向核查记录

审定结果：[两个具体边界及停止条件](../../docs/innovation-focus-20261006.md)。日期 2026-10-06。四次 DeepSeek 均成功完整返回，thinking enabled/high、默认最大输出 393216，未降低预算；合计 **499021 provider tokens**。没有 Claude 或新增 Codex 子 agent。

| 任务 | 完整答复 | provider tokens |
|---|---|---:|
| DMA/有限缓冲的强前作 | [dma_boundary.md](dma_boundary.md) | 97275 |
| 生命周期／成本感知候选证据 | [lifetime_boundary.md](lifetime_boundary.md) | 229093 |
| 共享者计数的公式边界 | [sharer-boundary.md](sharer-boundary.md) | 93049 |
| 暂停翻译／排空的条件反例 | [drain-boundary.md](drain-boundary.md) | 79604 |

同名前缀 `*-input.json` 是实际任务输入；`*-raw.json` 保存完整工具返回和实际用量。原答复保留但不全部采纳，**最终判断以审定文档为准**。

## Codex 直接核查与纠正

1. MMU-aware DMA 2018 已有预装锁翻译、burst 元数据失败重放。不能把一般方案当创新，也不能将其 hybrid TLB miss 的错误响应语义移植到当前硬件 PTW。根直接核 IOTLB 匹配与请求完成／错误路径。
2. 生命周期稿的 fan-out × ACK 延迟不是一般完成延迟模型：并行 ACK 常受最长路径、排队和重叠影响。必须区分必须执行的映射更新和策略可避免的容量淘汰；不能把高 fan-out 的总成本全算成某策略新增成本。未来 mapping lifetime 也不能由事后 unmap 事件直接获知。
3. 当前 IOTLB 的 PSCID/GSCID/stage 语义不能简化为每 DID 独立标签。平台没有 ShareMMU 的设备私有 TLB／Permission Tracker；几位新元数据不等于已具备回失效成本路径。
4. SHAREMMU/SHARP 原文已直接核。共享者差值严格大于 `W−1` 才能保证压过完整 LRU 范围；等号只构成平局。第三份模型答复的 `count_stale ≥ W` 不能写成必然压过热项。权重、随机化和真实 LRU 编码必须保留。
5. 双 mask、epoch、aging 不自动成为新机制，也不因由旧构件组合就自动无创新；应看新约束与同信息同开销强比较。权限事件 epoch 对没有权限变化的阶段转换可能完全无效。
6. C2 依赖环是 Codex 提出的条件性扩展假设，DeepSeek 完成定向证据审计。Arm DTI 的 stalling fault 与普通 miss／更新锁不同；它已规定进度关系，故不能主张首次 IOMMU 死锁。模型稿“选择性失效必然消灭一切环”过强：只能排除被选择性规则切断的具体环，若相依写仍落在受影响范围，还须分析。
7. “实际系统不满足前提”表示此条件反例不适用，不等于逻辑上推翻一个条件命题。不能把 paper 未描述有限 FIFO 等同 artifact 确定没有该路径。ZIP 目录已有 DMAController，实际源码读取失败，锁范围仍未知。
8. 旧 v1 guard2 代理模型曾通过部分用例，不代表后来未再失败或当前实际修复只依靠 guard2；不能拿旧模型证明当前漏洞。当前 4 KiB 完整 copy 缓冲／单 lane 一个 copy 可能排除新反例。

## 资料与执行记录

- [web-evidence.json](web-evidence.json)：本轮 22 组原始搜索／网页／关键规范读取；包含失败，不计为全部取得全文。
- [SHARP 原文](papers/sharp2017.pdf)与[文本](papers/sharp2017.txt)：本轮新取得；其余优先复用原文库。TLA 作者 PDF 返回 404，HELM 全文打开失败；ShadowUpdate 仍按摘要限制主张，不能宣称排除了所有最新前作。
- [artifact-web-check.json](artifact-web-check.json)：网页工具无法访问 DOI/记录/API；后续 curl 对 API 成功，不能沿用“artifact 不存在”的结论。
- [artifact-metadata.json](artifact-metadata.json)、[artifact-directory.json](artifact-directory.json)：官方元数据与 ZIP 中央目录。先读 65536 字节尾部，再读 2047162 字节目录，两次 HTTP 206；未下载完整 132141444 字节包。两个源码区间入口均 HTTP 503，返回的是错误页，未当源码保存或读取。
- [audit.json](audit.json)：调用用量和范围。产物导航只检查本轮文档；不是论文复现或平台功能测试。

DeepSeek 提供证据提取与候选审计；Codex 实际检索、下载、直接读关键来源／源码并作裁定。无 RTL/Linux/生产脚本修改、构建、板卡操作或新实验；既有 method88 与 composition172 结论未重计。

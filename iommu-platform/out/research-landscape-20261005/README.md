# 2026-10-05 研究地图证据包

审定入口：[经典—前沿—瓶颈地图](../../docs/research-landscape-20261005.md)。本目录是研究证据，不是 RTL 源码或构建产物。

- 36 篇独立论文、38 份 PDF 全文版本、4 份官方文档；Pond、HELM、CDFD、SoftWalker 为另列的局部原文/摘要索引。下载与版本：[source-manifest.json](source-manifest.json)。下载成功不等于已复现。
- 分组全文阅读：[翻译覆盖与扩展](workers/L1_translation.md)、[共享干扰与 I/O 管理](workers/L2_sharing.md)、[数据搬运与动态分配](workers/L3_data_lifecycle.md)、[迁移/观测与定向纠错](workers/L4_migration_observability_recovered.md)。这些是 DeepSeek 原始答复；判断以审定地图及 [Codex 关键来源审查](ROOT_REVIEW.md) 为准。
- [成稿草案与 Revelator v2 核查](workers/L5_map_draft.md)、[定向修改原答复](workers/L6_map_edits.md)、[最终 Codex 校正](codex-final-edits.json)。原稿及错误未删除，防止把修订后结论伪装成模型初答。
- [调用与核查清单](audit.json)：地图阶段 6 次成功 DeepSeek API 调用，均 deepseek-flash、默认最大预算 393216、未截断；一项本地 NUL 输入拒绝后有一次格式修复提交，未减预算。API worker 只返回文本，工具、原文核查与文件写入由 Codex 执行。
- 用户转向地图前，三份发散提案已返回，存于 [research-expansion](../research-expansion-20261005/round1/)；保留为历史候选，不作为文献空白证据。本次没有续调 Claude 或启动 Codex 子代理，不计为新六席五轮。
- 无新实验、源码修改、测试、构建、上板、写 SD 或论文复现。既有 172/172 功能验收保持，具体论文贡献仍待确立。

后续入口：先根据地图挑少量真实应用条件，比较已有方法依赖的假设与适用强基线；此前三项候选不再预设优先级。CXL 完整放置/一致性、编译器布局/稀疏格式、RDMA 注册与撤销、更广的实时界需另作深读，不能因本包未覆盖而说没有工作。

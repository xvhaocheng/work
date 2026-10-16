# 入口同步回执

2026-10-15。只编辑根AGENTS.md、iommu-platform/AGENTS.md、iommu-platform/README.md、docs/research-history/README.md，另保存本任务回执。未改生产代码、ABI、Linux、RTL、旧out及历史正文；未执行实验、编译、硬件操作或生产命令。

- 根AGENTS.md:42 修正独立比较已完成并链接实际文件；:72—74 增加唯一总计划、当前导航、历史计划索引、全局/分支/硬件边界与并发写入规则；去两个空标题。模型策略全文字节一致。
- iommu-platform/AGENTS.md:4 同步当前入口/历史索引；:7 将stage97 M0定位为可复用资产；:10 增加写前重读合并。模型策略继续引用根规则。
- iommu-platform/README.md:3、7、11 同步当前总计划/导航/状态、D/I/A5安排、MIG暂停与受控v6有限证据；:13—18、33、65、84—85 将v5协议、ABI、示例与九月代码地图标为历史。常用命令代码块及构建说明保持，仅修改v5历史时态。
- docs/research-history/README.md:3 只增加2026-10-14来源快照提示和三个当前入口；主工作区之后历史正文完全一致。

验证：模型策略全文、历史正文、构建命令/说明保留检查均通过，所有本四文档Markdown相对文件链接均存在，空标题已去。完整patch在changes.patch，before原件及SHA在before/、before-sha256.json，after SHA与检查在verification.json。

建议/请求配置为gpt-6.1-sol / medium；实际运行模型与推理强度无法从可用运行元数据独立核对，记unknown。未使用DeepSeek或其他外部模型。

搬迁记录：入口同步产物原位于 `/home/xvhaocheng/work/out/research-plan-implementation-20261015/entry-sync/`，现已整体移动至本批统一目录 `/home/xvhaocheng/work/iommu-platform/out/research-plan-implementation-20261015/entry-sync/`。before原件搬迁后逐项SHA256与before-sha256.json一致；四入口正文未修改。

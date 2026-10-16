# 当前状态 stage 96 2026年10月10日

- 当前总入口：[研究计划.md](研究计划.md)；方法：[研究方法.md](研究方法.md)。已完成执行前增订，保留 Remote PTW → 多 DMA 共享翻译干扰 → 有独立增量的方法贡献这一主干。
- 当前第一任务为 G0 一页失效卡：哪里失败、普通方法为什么处理不了、实际控制点还能做什么。新机制可以未定；已有证据不足时允许一次预先登记的有界基线判别，不以无候选否定主问题。
- 顺序：G0 失效确认 → G1 选择信息/动作及一正一反 → G2 准确前作、因果、完整成本和留出 → G3 立题裁决。G0/G1 需要新口径时先做 M0 最小测量适配。建议累计10工作日、最多六机制问题组加两留出，可提前结束。
- 首轮计划：固定目的 COPY，DID10/11 各64次4KiB，V固定页对、A源/目的各A8环，V/A配置gap显式0，后续任务随上一笔完成就绪。地址、权限、静态S1/S2关闭、预热和初态执行前登记。
- M0 硬条件：双方各恰好发出/完成64笔；正常结束不能STOP A；从首次触发前的共同逻辑t0分别计到硬件最终done。初次准入等待、A尾段、必要排空均计费；软件轮询complete不能充当硬件完成时间。尚未实现或验证此新测量包。
- 信息/动作未选定。区分V当前状态、A上一笔历史、可实现的共享状态；实际ADMIT仅控制下一笔copy。测试top的PolicyEnable为分区参数，A门控端口仍绑常量；旧p1–p6反馈源码不等于已接入。
- CPU背景随V活动启停会影响A尾段。保持SYSTEM拓扑；如需隔离CPU，先做最小显式测试开关并保留两种模式的正确性检查。CpuIssueInterval=0不能当作关闭。
- 旧E2/E3、method88、PN证据按原范围复用；不重启固定A8 p5。Sobel477项仅合同验证、性能缺口未测，保留后续条件候选；行为DDR不替代实板MIG归因。
- 本轮仅计划与关键证据核查；新性能模型、RTL仿真、生产源码修改、构建、上板、Claude均0，无新增Codex子席。DeepSeek三次证据/文稿提案，Codex实质实验设计、核源、修订与验收。
- [根接受](iommu-platform/out/research-plan-comprehensive-20261010-root/root-result-acceptance.json)／[调用账本](iommu-platform/out/research-plan-comprehensive-20261010-root/worker-ledger.json)／[上轮三项审查](iommu-platform/out/research-plan-review-20261010/root-result-acceptance.json)／[stage95原样归档](iommu-platform/docs/status-stage95-before-comprehensive-plan-20261010-root.md)。

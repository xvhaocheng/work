# MAP-OWNER-RECALL-PUBLISH-1 交付入口

2026-10-16 JST。MAP-OWNER-RECALL-PUBLISH-1已完成源码绑定的条件控制协议，保留owner-assisted private alloc.cache-only冷B关联回收及低成本整合假设；未实现，未证独占创新或净收益，未选论文主线。

每source池／固定B配置用有界mailbox，miss先pending再合法kick，短irq-safe锁同序关门与直接napi_schedule；positive-budget owner在complete前只取旧snapshot，每次最多一页，ACK不擦新pending，当前帧仍O0。NO_ELIGIBLE终结本代，terminal ACK后才显式REARM，不自动cache-entry通知或空转。REQUEST_ACCEPTED、OWNER_ACK、UNMAP_DONE、RETIRED分开，必要CPU metadata退休后才返K；无未来owner服务或返槽墙钟保证，ring-only／全在途／无owner／退休不前进可不返槽。

napi_schedule的bool、MISSED、complete和disable均不是应用ACK或DMA排空；enabled普通完成三竞态仅给条件唤醒偏序，实际特殊模式unknown。disable只等所有权释放、不等poll返回；enable不替应用重放pending。poll处于RCU读侧，不等自身grace或反等自己的closer。gate＋kick线性化，解绑后禁止旧NAPI；拟议ctx source-anchor须组合保留现有XDP disconnect，ctx不pin pool，最后source release后不回读pool。实际anchor/ref、锁序和reader集合未实现验证。 持久组织关闭可以保留合法B O0及共享A；真正B/config销毁须禁全部新B DMA（含O0）、CPU静默、实际DMA完成或stop及配置寿命。正常隔离idle页不另要求全B drain；真实core/FW stop语义本轮未取得。

O0本有B-only／A留池，DMA_PMD本有idle＋allocator排他原理。R-OB4K-T-N／O3／slot同享新付费接口可用同协议，本轮无独占策略／控制算法或必须更低费用的依据；这不等已发表网络实现或实测同总成本，保留整合价值。无独立逐TX／peralloc use token不等零新增热费用。CPU、真实S/P/C/Q并集及分配反馈、ctx/mailbox/K-retire metadata、PTE、IOVA、带宽与有效完成分报；回收关联不等payload RAM下降，不强套O0 PFN，native数值null。

下一MAP-OWNER-INTEGRATION-CONTRIBUTION-GATE-1为evidence_gated_not_auto_scheduled：仅有新具体实现／部署／费用差项或新代表性可运行输入才作有界贡献判断；不继续拆相同协议，不重复已结束的数据入口搜索或旧阈值账，不由缺成本自动开发。无需等待一般授权、CPU／内存价值确认或用户保证痛点。

本轮定向新增2个官方固定commit文件及HTTP200 receipts；9项worker primary hash/size、17处root直接读段行界通过，仅provenance。0新状态或性能模型、native／旧实验／旧账／构建、host设备probe、driver／collector／RTL／ABI编辑或板测。Codex-only；NAPI核查请求Sol/high、关闭协议Sol/xhigh、文档Sol/medium；独立actual model/effort metadata unknown，不声称root切换。 B1/I/D/A5/A2及并发GitHub/Linux/RTL独立事项保持；MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；10月15—28日窗口不重启。

## 裁决与精确入口

- [root裁决](root/decision-analysis.md)：源码事实、首版协议、手工正反序列、具名普通对照、资源与决定。
- [假设卡](root/hypothesis-card.json)／[冻结接受](root/root-result-acceptance.json)／[下一贡献gate](root/next-minimal-contract.json)。
- [NAPI核查](napi-source/findings.md)／[API卡](napi-source/api-card.json)／[HTTP回执](napi-source/http-receipts.json)：固定Linux v6.12 commit `adc218676eef25575469234709c2d87185ca223a`；本轮新增 `include/linux/netdevice.h`、`Documentation/networking/napi.rst`。
- [关闭协议核查](close-protocol/findings.md)／[协议卡](close-protocol/protocol-card.json)：source pool、NAPI/channel、ctx、B/config寿命分开。
- [来源清单](root/source-manifest.json)／[来源核对](root/source-verification.json)／[实际执行账](root/execution-ledger.json)。hash与行界仅为provenance；手工偏序不等并发实现或运行证明。
- [前轮cache-only选择](../mapping-recycle-reclaim-20261016/README.md)／[普通R](../mapping-integration-delta-20261016/owner/integration-analysis.md)／[slot适配](../mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)。

## 文档与保留验收

[同步合同](root/canonical-sync-contract.json)／[即时快照清单与初始drift](documentation/before-write-manifest.json)／[更新账](documentation/update-ledger.json)／[变更文件](documentation/changed-files.json)／[文档验收](documentation/verification.json)。

五份canonical以写前即时原文为基准合并；CURRENT分支和独立工程块、计划I起后缀、导航已有成果起后缀逐字保护，HANDOFF和validation完整旧字节作为后缀。14条既存历史document,target坏链照录，不修冻结历史。AGENTS已有用户编辑保留，本席未改／暂存；提交推送由root独立验收后处理。

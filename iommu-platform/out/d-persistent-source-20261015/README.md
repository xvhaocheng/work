# D持续服务来源与完整完成门（2026-10-15）

SSD正常覆写→FTL/GC与DRAM开行/刷新来源均成立；优先SSD，但尚不能接入P/PN选策模型。固定MQSim实际两次私有构建、8次冻结运行＋1次因缺完成触发的只读调试重放：512请求完整512/512且GC=0，2048/4096/8192覆写各缺1/1/2完成，原版/观察版XML 4/4逐字节一致。exit=0/eventlist=0不等于完整完成；三个覆写case为NOT_ESTABLISHED，不是否定GC持续性假设。root核1276项hash与事件守恒，3种负变异均拒绝。

下一最小动作固定2048输入，取得request→child transaction→barrier/PHY→CQE的唯一身份账本；先解决正确性，再接P/PN，不调参或扩大矩阵。XML Request_Count是生成数；AMU条件性删除用户事务路径尚未动态绑定当前缺失CID53，TSU receive_slots=1可能只是旧指针，不能当live证据。 O_phys可重建不代表真实O_host已足够；新增host日志与Z均计采集、保存、传输、推断及切换费用，真实费用unknown。来源事实与内部模拟真值不构成原生接口、性能或novelty收益。

本轮只接受机制来源和实际事件核账；完整覆写边界、同O_host状态对、动作排序差异、及时收费接口和强普通控制净收益均未成立。SSD优先是当前研究判断，DRAM候选保留。模型ns不等于实测或原生周期；不把模拟器规则或缺陷定位写成本项目新意。

## 阅读与原始证据入口

- 来源：[SSD来源卡](ssd/evidence-card.md)、[SSD裁决](ssd/assessment.md)、[DRAM来源卡](memory/card.md)、[DRAM核查](memory/findings.md)、[现有观测接口](observation/source-evidence.md)。
- root：[稳定接受](root/root-result-acceptance.json)、[决定与边界](root/decision-analysis.md)、[关键源码审查](root/direct-source-review.json)、[被动记录审查](root/passive-probe-review.json)、[运行器审查](root/runner-review.json)、[独立核账与负变异](root/boundary-verification.json)。
- 构建与完整hash：[原始source清单](offline/original-source-manifest.json)、[原始source SHA256](offline/original-source.sha256)、[archive SHA256](offline/archive.sha256)、[原版构建](offline/build-result.json)、[原版构建产物](offline/build-artifacts.json)、[观察版构建](offline/instrumented-build-result.json)、[观察版构建产物](offline/instrumented-build-artifacts.json)、[运行前manifest](offline/instrumented-prebuild-manifest.json)、[补丁](offline/instrumentation.patch)、[输入/二进制/日志/XML逐run完整hash账本](offline/runs/execution-summary.json)。
- 原始事件日志：[512](offline/runs/512/instrumented/stderr.log)、[2048](offline/runs/2048/instrumented/stderr.log)、[4096](offline/runs/4096/instrumented/stderr.log)、[8192](offline/runs/8192/instrumented/stderr.log)。每个目录均保留stdout、XML、run-result和pre-run-manifest，原版记录在对应original目录。
- 额外只读调试：[实际debugger日志](offline/diagnostic-2048/debugger.log)、[窄源码诊断](ssd/incomplete-flow-source.md)。AMU条件路径未动态绑定缺失命令，TSU旧指针不作为live证据。
- 后继：[MQ-IDENTITY-1最小合同](root/next-minimal-contract.json)尚未执行，限固定2048请求的唯一身份定位，不调参或扩矩阵；[冻结边界合同](root/boundary-contract.json)保留。
- 文档：[即时写前snapshot/hash](documentation/before-write-manifest.json)、[入口同步回执](documentation/receipt.json)、[本轮执行席账本](root/agent-ledger.json)。

D主线、I轻量not_ready、A5/A2保留及10月15—28日立题窗口不变；[CS1当前Z/费用/菜单局部停止](../d-service-model-20261015/README.md)保持。MIG1792023314继续paused；0原生DMA、Linux/RTL/UAPI修改、collector、FPGA/板操作或旧实验重跑。本轮实际两次私有host构建、8次冻结模拟＋1次失败触发的只读调试重放，不是0 build/0 sim。

共享研究Codex-only。证据任务请求Sol/high，普通实施及文档请求Sol/medium；actual运行元数据unknown，请求不等于运行证明。旧个人研究方法不恢复。

# stage115：两个新产物入口已核，原生 NVMe 证据包仍待取得

2026年10月13日JST。stage115 实际 goal 已登记，无额外预算，**连续第三轮同一缺项后，实际工具已标 blocked，未完成**。目标是取得并核验一份关联同一 C113 原生运行的证据包；仅完成来源入口核查不等于完成这个目标。前轮C113条件保留、当前FPGA不准入及各关闭范围不变。

本轮没有重复旧论文／功能测试／精确issue检索，定向核了两个尚未核过的同源产物入口。root与一个新Sol／ultra、fork-none独占席，Codex-only；无部署、实现、build、设备命令或旧实验重跑。

| 新入口 | 新取得的原始事实 | 本次接受结果 |
|---|---|---|
| 官方DeepSpeedExamples，固定教程关联提交`bdf8e59aede8c8e0577e8d4d557298ca8515268f` | 有`device=nvme`、path=`/nvme_data`的optimizer/parameter模板，B4、双pipeline关闭；启动脚本默认另一配置，NVMe选择需参数或改注释 | 四个原始文件git blob匹配。接受为源模板；没有实际argv／配置选择／run记录或物理路径，不能作为native运行 |
| STRONGHOLD作者发布的SC22复核归档，只查其中ZeRO-Infinity对照 | DOI6887214/v0.1-ae-testing归档含两份真实对照日志：均报告DeepSpeed0.5.7、git hash unknown、单GPU命令及`*-cpu.json`，解析后的param和optimizer device均cpu，nvme_path均null；实际client为`torch.optim.adamw.AdamW` | 日志身份与归档成员bytes接受；backend和CPUAdam执行身份均不匹配，排除为所需NVMe证据包。独立NVMe模板没有关联runlog |

这两个入口仍属于同一ZeRO-Infinity来源取证。没有把STRONGHOLD算法、CPU版本对照或另一个应用加入研究候选。官方旧示例的`max_in_cpu=1`是配置，不是实际H不足；作者NVMe模板使用旧`thread_count`字段，也不能倒灌成v0.16.4的实际p。日志文件名的`ws-4`不能替代命令里的`--num_gpus 1`。没有对这些模板进行部署或运行。

root独立核原归档：5,129,504B，MD5与Zenodo原始metadata一致，SHA256=`f07982a89f179e32f6999d2c5fac15812913e757c9e82b7c0c37c9cb41adac62`；1030文件、results24份，其中只有2份ZeRO-Infinity日志，提取后的两个snapshot逐byte匹配原ZIP成员。这个结果只证明本归档和原日志身份，不代表全世界没有NVMe运行记录。[原始归档核验](../out/stage115-c113-native-bundle-20261013/validation/original-archive-check.json)。

root亲核两日志的命令、版本和已解析offload配置；不能把日志旁的NVMe JSON套到已记录的CPU运行上，亦不能把2022运行当作固定v0.16.4执行。[关键原始记录](../out/stage115-c113-native-bundle-20261013/critical/root-ae-run-review.json)、[官方模板边界](../out/stage115-c113-native-bundle-20261013/critical/official-example-review.json)、[子席完整交付](../out/stage115-c113-native-bundle-20261013/workers/ae-baseline/report.md)。

**未满足的进入条件没有改变：** 至少先有能绑定同一实际NVMe运行的软件／launcher/config、path→FS/block/PCI requester→启用翻译的IOMMU/domain/共同单元及自然peer资料；再核N_i/B/p/H/page/layout、真实数值、成功bytes、DONE/quiet/释放与普通完整成本和残余。本轮没有取得这份包，没有G0/G1或翻译性能接受，也不作科学阴性。

已通过异步问题询问是否有现存证据包本地路径／可访问链接；截至本轮保存时没有回复。未收到信息不能当许可或新事实。新入口的有界检查已结束；没有新原始输入不重复这些来源、重写未变C113卡、建立较小goal或通过新系统部署填补资料。

当前缺项属于stage115实际目标未完成的外部证据条件。此goal的阻塞审计是第一轮，未满足连续三轮的blocked门槛，故保持active；没有标complete、paused或提前blocked。后续继续同一goal，收到关联资料再接受；同一缺项若连续三轮阻止任何有意义进展，则按实际goal工具规则标blocked，不靠重复核查维持active。

[本轮入口裁决和缺项](../out/stage115-c113-native-bundle-20261013/intake-result.json)／[实际goal快照](../out/stage115-c113-native-bundle-20261013/goal-checkpoint-active.json)／[执行账本](../out/stage115-c113-native-bundle-20261013/execution-ledger.json)／[阻塞审计](../out/stage115-c113-native-bundle-20261013/goal-blocking-audit.json)。整体仍阶段一立题，论文未完成。

## 同一goal第二轮（2026年10月13日JST）

再次继续时实际goal仍active，未收到原生资料path/link，也无新的匹配一级资料入口。首次来源核查与原记录不重跑，本轮没有新source/agent/实验/构建或部署。连续缺项审计为2/3，尚不标blocked；不把等待记成goal完成或研究进展。[本轮原始观察](../out/stage115-c113-native-bundle-20261013/turn-2/intake-observation.json)。

## stage115 同一 goal 第三轮：实际 blocked（2026年10月13日JST）

连续第三轮仍未取得同一既有原生 NVMe 运行证据包，也未收到资料路径/链接或新的匹配一级入口。已核官方模板和作者 CPU offload 日志无法闭合实际 NVMe 运行、共同翻译单元/自然 peer、完整完成与普通成本；没有可独立推进的授权工作。达到 goal 工具的连续三轮门槛，实际 update_goal 已标 blocked，原完整 objective、创建身份和无额外预算均保留；未 complete、未 paused、未另建或启动后继 goal。

目标工作停止，保留关联原生证据或用户明确调整范围的输入条件；用户恢复时重新开始阻塞审计。当前 C113 条件保留/FPGA 不准入、各关闭分支不变，未知不作科学阴性。仅同步状态与交接记录，无新增席/外部模型/查源/部署/实现/构建/板测/实验。证据获取目标和论文均未完成。

[第三轮观察](../out/stage115-c113-native-bundle-20261013/turn-3/intake-observation.json)／[实际 blocked 回执](../out/stage115-c113-native-bundle-20261013/goal-blocked-receipt.json)／[记录同步核验](../out/stage115-c113-native-bundle-20261013/validation/blocked-status-check.json)。


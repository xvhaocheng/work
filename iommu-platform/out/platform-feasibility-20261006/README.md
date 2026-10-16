# 实际平台可行性审查记录

日期：2026-10-06；审定结果：[platform-feasibility](../../docs/platform-feasibility-20261006.md)。

**后续用户质疑后的纠正：** 首轮只核最新172和当前profile，遗漏历史P4M/method88。`throttle-history-input.json`、`throttle-history-raw.json`、`throttle-history.md`保存一次定向DeepSeek回查，thinking/high、393216、stop、未截断，共38695 tokens；加首轮两次总275413。Codex直接核P4M原run/audit/statistics和method88正式/离线结果：已做固定gap及七方法反馈准入，method88 88/88，p5四格强对照全负。formal60记录为24在线＋1离线追认，35未见续跑完成证据。已修正主文/选题/状态，不再建议从零建立限速基线。worker将新A强限定为“只能批次级”及将旧p5阴性外推为逐任务反馈普遍证否的说法不采纳：机制时标取决于实际新问题，既有阴性仅限已测条件。此次未重跑实验或改实现。

- `interfaces-input.json` / `evidence-input.json`：两组 DeepSeek 任务与文件范围。
- `interfaces-raw.json` / `evidence-raw.json`：完整 MCP 返回；`interfaces.md` / `evidence.md`：完整 answer，未经删改的证据草稿。
- `audit.json`：实际 provider 元数据和审查范围。
- `critical-source-match.json`：五个关键 RTL 文件与当前 ptwfix 构建快照的 SHA256/逐字节比较，全部相同；不是全树或运行内核身份检查。
- `metrics-spotcheck.json`：从原172项派生 CSV 复算三个条件下 PN/N 的九组对照，无新实验。计算 ΔW=100×(W_PN/W_N−1)，Δr=100×[(A_PN/W_PN)/(A_N/W_N)−1]；A 是同窗口完成 job 数。另直接核读 `explore-b1-g0-a8-mpn/record.json` 的 request、result.p4m、raw 与口径声明。

DeepSeek 两任务均为 deepseek-flash、思考 enabled/high、输出预算393216、finish_reason=stop、truncated=false：interfaces 138087 tokens，evidence 98631 tokens，总236718；包含输入、推理与输出，不是费用估计。Codex执行本地工具并审查关键源码、构建证据与数据，保留最终架构判断；没有调用Claude或新增Codex子agent。

## 原始证据稿的审定修正

1. `evidence.md` 未收到最新 utilization，故列为缺口。Codex补读 ptwfix routed 报告：LUT81.22%、Slice99.60%、BRAM11.24%；不用旧89.80%/99.45%代替。
2. 接口稿“没有硬件描述符队列”过宽。batch 当前每 lane 只给一个逻辑copy；顶层仍配置 JOB_FIFO_DEPTH=8、NUM_AX_IN_FLIGHT=8。512是数据拍缓冲，不是512个descriptor，不能据前端推论整个iDMA无FIFO。
3. 接口稿“运行中不能读计数”过宽。MMIO读出与可用于在线控制的一致、已定义采样接口不同；现有实验冻结后采集，在线使用需核一致性/采样成本。
4. 接口稿缺 policy 顶层。Codex核 ADMIT 及顶层：当前 role=0禁用，role=2旧policy为潜在复用接口，不是现行BIT已有控制器。第一版逐任务控制应在发VALID前准入，不能撤销已背压请求。
5. 接口稿把“逐任务在线”与“批次边界在线选择”混为一谈。现 quiet+clear 可支撑批次边界选择，但有排空/清缓存/热身和重启供给成本；无停顿热切换另需RTL。
6. 同boot三个block不能当三个独立启动；也不能仅凭计数相同断言“实际有效样本数=1”。保留运行重复含义，相关性/跨启动稳定性未证明。
7. 证据稿所说A的任务量缺口已核：驱动把A配为2^32长batch，V窗口结束后STOP/排空。因此A全程completed_bytes不同于同窗有效完成量，不能用两者混比。
8. 证据稿“同gap比较干净”过强：同一故意gap减少一个混杂，但未消除启动相位、缓存前态、软件/DDR影响。不能仅据此做根因结论。旧HPM亦仍为raw_only。
9. A尚无页感知控制数据；B已有描述性条件差异，不能笼统说从未进行任何性能分析，也不能称已有预测/创新。旧172全部已看过，不当留出集。

本轮只做定向读源、身份比较、旧数据算术和研究文档更新。没有运行测试、修改生产代码、构建、上板、连接Mac或写SD；不将模型答复或静态审查当实板验证。

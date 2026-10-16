# NEXT_EXPERIMENT_PLAN（单次部署、分阶段；仅计划，不授权实现/构建/上板）

## 0. 前置约束
- 一个 BIT+payload，运行时可选模式；先 host 检查所有 mode 接口，再考虑 build；尽量少硬件循环，但绝不承诺一次上板一定足够。
- 有界资源/复杂度：基准=unthrottled/Solo + fixed gap + byte/request/miss 计费；若 source interface audit 证明有 richer-type 可用，才加已验证类型加权；另加 actual main occupancy 和 simple V progress feedback。
- 公共 throttle actuator：下一次完整 copy 在 drain 后作动；相同 outer controller 处必须同控制器；不同计费单位分别校准，不强制同一 token 数值。
- 不扫描所有 gap、不做巨大笛卡尔积、不按模式重建。
- 校准选择在板前确定性冻结；holdout 条件不参与调参；若校准不足，不手调 holdout，也不自动升级矩阵。
- 阶段检查 first-error-stop 保留，不自动重跑/剔除；stage inactive/unselected 仍报告。
- richer event type 只做 source interface audit 后最小必要子集；不承诺未知 DDT/PDT 覆盖，不加完整 MRIF FIFO CDC 仪器 wishlist。
- 精确试次数、warmup、顺序与重复待 valid mode/param 冻结后确定；此处不虚构固定数。

## 1. 单次部署阶段表
| 阶段 | 目的 | 板前/板上动作 | 通过条件 | 停止/保留 | 试次/重复 |
|---|---|---|---|---|---|
| 0 Host mode interface check | 确认所有运行时模式接口 | 板前：检查 mode select、common throttle actuator、drain 边界、actual main cost、simple V progress feedback、只读快照、first-error 保留、未选模式报告 | 所有选中模式接口一致；未知/未定义模式拒绝；无隐式重跑 | 失败即停并保留日志；不构建 | 待冻结 |
| 1 Short acceptance | 同一部署先跑短接受 | 板上：对每个选中模式跑短微用例；顺序固定；first-error stop；inactive/unselected 仍报告 | 短用例全通过；计数/边界/错误保留一致；UART warning 记录 | 首个错误停止，不自动重跑/丢弃 | 短用例集合待冻结，不预造数量 |
| 2 Bounded calibration | 板前定义确定性校准选择；板上只跑该子集 | 板上：每模式分别调 unit；限制参数列表；不扫所有 gap；不建大矩阵 | 每模式得到冻结参数；校准覆盖达到预先定义充分性；否则不得进 holdout | 若校准不足，保持未决或停止；不手调 holdout | 校准选择待冻结 |
| 3 Freeze | 冻结参数/阈值/holdout | 板前或板上：写死参数、分析脚本、工程意义阈值、holdout 条件、接受边界 | 冻结记录完整；holdout 未用于调参 | 冻结后不得改参 | 不适用 |
| 4 Frozen formal comparison | 公平对照 | 板上：同一 BIT/仪器/时钟/工作集/完整 copy 作动点；unthrottled/Solo + fixed gap + byte/request/miss（或已验证类型加权）+ actual main occupancy + simple V feedback；同 outer controller；分别校准单位 | 在匹配 V 完成/短时未达目标容差下，occupancy 相对强基线有预冻结工程意义增量；overhead 可接受 | 无增量则停止/撤回新方法主张；不自动升级 | 每模式重复待冻结 |
| 5 Predeclared heldout validation | 预声明 holdout 验证 | 板上：只跑预声明 holdout 条件；不调参；不自动重跑/丢弃 | heldout 上结论方向与 formal 一致且过阈值；overhead 保持可接受 | 失败记录保留；不手调 holdout；不自动扩展矩阵 | holdout 条件待冻结 |

## 2. 公平比较模式清单（具体实现待 Codex 决定）
- unthrottled/Solo。
- 固定 gap 参考。
- byte 计费。
- request 计费。
- miss 计费。
- 若 source interface audit 证明类型可测：按训练期固定类型成本加权的 request 计费；否则不得把未测类型当常数。
- actual main occupancy 计费。
- simple V progress feedback。

所有模式同 BIT、同仪器、同时钟、同工作集、同完整 copy 作动点、同 calibration 预算；分别校准各自单位，不强制同 token 数值。

## 3. 验收与证据保留
- 每阶段 first-error stop；不自动 rerun、不自动 discard。
- 未选/未激活模式仍报告，避免把缺失当通过。
- 保留原始 JSON、UART、params、哈希与 provenance；若板端 fresh download hash/SD readback 缺失，继续按限制报告。
- 若 exact trial count 未在 mode/param freeze 时确定，不得事后补造；可先报告未决，再等待 Codex freeze。


# §35 固定分片位置：八次真实运行

2026-10-08。一个新 SV+C++ 构建、三次仅 C++ 重链接，八次新运行全部 exit0/PASS。没有重编旧 SV 模型，没有为修正汇总脚本重跑 DUT。没有 Linux/BIT/SD/板卡操作。

- [研究判断、前作与下一门槛](../../docs/fragment-placement-boundary-20261008.md)
- [机器验收](analysis.json)／[可读汇总](analysis.md)／[汇总脚本](placement_summary.py)：902 条件通过。§34 两份 CSV 只作一致性参照，不计新运行。
- [Codex 关键审定](codex-review.md)／[实际物理子 AR 核对与平均 DONE 计算](codex-metric-audit.json)
- [状态位声明账本](declared-state.json)／[生产哈希不变](production-check.json)／[实际执行清点](stage-evidence.json)

每项 `{stem}-command.json` 是直接 argv 数组，`{stem}-result.json` 是实际 exit_code／seconds／reused，`.log`、`.csv` 是原始结果：

| 场景 | 前置 | 后置 |
|---|---|---|
| S，V先 | [pre-v-32768.log](pre-v-32768.log) | [post-v-32768.log](post-v-32768.log) |
| S，A先 | [pre-a-32768.log](pre-a-32768.log) | [post-a-32768.log](post-a-32768.log) |
| N，V先 | [pre-n-v-32768.log](pre-n-v-32768.log) | [post-n-v-32768.log](post-n-v-32768.log) |
| N，A先 | [pre-n-a-32768.log](pre-n-a-32768.log) | [post-n-a-32768.log](post-n-a-32768.log) |

N的FAST全0，CSV与对应S相同。跨bank窗口没触发，不是有效快路径收益对照。

V先：前置V/A issue→done=24162/27436，后置=17471/20149，均值25799→18810，降低27.0902%；A先镜像。均34miss/102PTW/2DDTW，TL请求98→66、命中64→32，均64物理源AR/34AW/每流4096R与W拍。不能把全部时差归因重复命中，逻辑入口保存容量不同。

均固定max128beat、原parent256beat、每流1个分片模块父槽、双W owner/seq、一child在途、48B两路iDMA FIFO，无新增R payload。后置还保留virtual关联与原TC队列。模块声明前置2502／后置3568bits、观测160bits另计，非综合／总面积。

Codex设计并实现`prepare.py`、`pretl_owner_fragment.sv`、`pre_actual_dma_top.sv`、`pre_system_top.sv`；`placement_modes_test.cpp`是驱动的S/N机械适配。DeepSeek提出`placement_test.cpp`真实硬件issue/done与译址计数观测补丁，起草与修订`placement_summary.py`；实际应用和执行均由Codex完成。

`worker-*`、`analysis-*`、`revision-*`保留任务、完整原回复与补丁。汇总初稿把两流总var当单流数，漏认三个控制事件的other区域，首次解析exit1／24失败；回传实际诊断修正，最终解析exit0。这是脚本错误，不是DUT失败。Codex另将八份child日志与实际源地址/ID/len/offset的物理AR逐项核对。

仅两真DMA同bank/CONF1/静态页内正常任务/src0/dst3072/各32KiB，全部行为DDR读延迟31。构建保留warnings、未lint／综合／时序。非四DMA/MIG/故障失效/旧PN根因结论。下一从普通父翻译复用开始，不重复八项。

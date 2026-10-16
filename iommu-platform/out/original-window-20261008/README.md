# §37 原请求窗口／父槽进度／普通准备时机

- [研究判断与下一强对照](../../docs/original-window-progress-boundary-20261008.md)
- [容量汇总](analysis.md)／[机器数据](analysis.json)：3新build、14新run，8完整拷贝PASS、6看门狗停滞；collection_valid不等于所有运行成功。
- [普通准备时机](timing-analysis.md)／[机器数据](timing-analysis.json)：1新build、2新run、2PASS。全阶段4build／16run／10PASS／6停滞。
- [实际argv／退出码／source与binary哈希账本](stage-evidence.json)、[生产76SV一致](production-check.json)。逐运行的`*-command.json`是实际argv列表，`*-result.json`是实际进程结果；`.log/.csv`保留全部成功和失败数据。
- [Codex直接核raw容量数值](codex-raw-audit.json)／[直接核准备时机数值](codex-timing-audit.json)。两流平均必须取同一次V先运行；A先仅镜像，不能把不同窗口收益混平均。

`prepare.py`由Codex生成out入口credit副本：每flow原AR接受到原RLAST占一个额度，在既有mux之前限制，physical parent深度另外编译为1／2／4。窗口1／2／4／8由C++显式输入；不增加payload、不改AW/W。`run.py`是首次执行入口，在首个窗口2失败后停止；剩余与扩槽模型实际命令已逐个保存，没有再次调用它覆盖结果。

`prepare-timing.py`由Codex生成更强普通背景准备：只对现有可消费owner准备，但不改变已形成的VALID或缓存寿命；沿用旧显式+reuse=2驱动、原父深度1。模型／C++来源列在各build-command.json，不修改原测试或生产文件。

DeepSeek起草`summarize-initial.py`、修订`summarize.py`、起草`timing_addendum.py`；完整回复/usage在worker、revision、final的raw／answer文件。Codex实际应用、运行、关键源码与数值审定；最后纠正跨窗口收益聚合与误把两方法当相等的检查。初始汇总16失败条件真实对应四停滞运行；后续明确分类。原始汇总`initial-analysis.*`保留。

本目录原型只有normal静态页内两DMA场景。窗口8／单物理父槽在本轨迹成功，窗口2／4却停滞；不能推广为通用单槽无死锁，也不代表生产BIT有相同bug。下一普通多原父header准备／同总预算对照未实现。不重跑这些既有模型，不先上板或BIT。

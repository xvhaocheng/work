# multi-parent-preparation-20261008

本目录保存2026-10-08 §38的运行脚本、模型源码及实际证据。原始入口是 [codex-raw-audit.json](./codex-raw-audit.json)。Codex 亲自设计／检查 RTL 与新颖性论证、执行构建／仿真并最终验收；DeepSeek 起草／修订汇总文档。所有修改限于 out 副本，未改 production/tests/Linux/BIT/SD/board，未综合，未上板。

## 内容

- [codex-raw-audit.json](./codex-raw-audit.json)：原始验收审计，覆盖全部 18 项。`all_raw_accepted=true`，`new_build_count=5`，`new_run_count=18`，`new_pass_count=18`；含 runs、old_post、best_ordinary、causal_witness、functional_state_declared_only、production_unchanged。
- [analysis.md](./analysis.md)：只覆盖最初 8 项新运行的表、与旧 post 的 V 先／A 先对照、声明位账本。
- [codex-next-control.md](./codex-next-control.md)：下一普通强对照规格，未实现、未测、未综合。
- [primary-source-review.md](./primary-source-review.md)：强前作约束与 2025 专利前史核查。
- [../../docs/multi-parent-preparation-boundary-20261008.md](../../docs/multi-parent-preparation-boundary-20261008.md)：§38 报告全文。

## 关键数

- 5 构建／18 运行／18 PASS：8 次普通模式、4 次读优先、2 次观察运行、4 次命中直通；`analysis.md` 只覆盖最初 8 项，根审计覆盖全部 18 项；新增 10 项是 DUT 运行，不是另加根审计。
- 已完成最强普通缓存命中直通 AW 优先 V 先启动 23380/26122 mean24751；旧 post window4 V 先启动 17485/20163 mean18824；均值降低 23.946507%，绝对 5927。它不是普通 TC 旁路，源子片仍进共享 TC。
- prepare0/1/2 均值均 25869.5，但 prepare0/2 CSV+prepseq 相同、prepare1 不同；不是 3 个独立性能点。读优先非统一最好：串行 lazy 25630、串行 prep 25847.5、缓存命中直通 read 24992。
- 声明功能位 pre3316+helper1281=4597 与 post4602 未综合，不是等面积；76 production SV 不变。原窗口高水位 4/4；未来父 30/32。
- 下一最强普通对照（已准备源片物理 AR 在 TC 写未命中期间通过，full permission／lifetime／BC／error／routing，same original window／saved metadata／payload）未实现；下一优先检验普通物理AR旁路。

## 边界

首次汇总解析失败（VAR32 与 64、header0x）是解析／汇总问题，不是 DUT 失败。无故障／失效／世代通用验收，无旧 PN 根因结论，无生产安全回归声明。调用用量只保留在 raw 证据，不在 docs 元数据中汇总。

构建／源码／二进制账本见[stage-evidence.json](stage-evidence.json)。主要模型是[multi_parent_translation.sv](multi_parent_translation.sv)、[compact_pretl_owner_fragment.sv](compact_pretl_owner_fragment.sv)、[命中直通变体](hit_bypass_parent_translation.sv)；所有`*-command.json`保存实际argv，`*-result.json`保存实际退出码，`.log/.csv`保留原始结果。

DeepSeek起草汇总与文档；Codex设计关键RTL、核源码与前作、实际执行和验收。全部worker回复、用量和文档初稿均保存在本目录。



对上一答做聚焦修订，完整原答已保存worker-answer.md；不要重写不变证据和SV。上一任务缺nonblock_completion_pool_late.sv导致1.5“全部判据只有ID位”推断不成立：原can_drain还有descriptor偏移/len/size严格匹配。补齐文件后给更正file:line；sourcegate固定4KiB/Depth8/len255限制仍是真约束。
Codex独立设计并实际写好了本轮普通基线ordinary_copy_owner_gate.sv（单normal job/流，64bit，virtual+physical Whead owner和段序相同、当前AW与AR偏移/len匹配、无R存储，ARVALID sticky保持）。owner_actual_dma_top.sv已实际准备，其Mode0分支SourceReadGateEnable=1才实例化新gate；这是本轮out新接口而非旧test开关生效。生产banked_ctl与无TEST_COMPLETION_GATE维持。
只修订准备Python脚本，不执行任何代码，不改SV设计：
- 当前已有out/write-owner-baseline-20261008/codex-baseline-spec.md、ordinary_copy_owner_gate.sv、owner_actual_dma_top.sv，以及worker原答和production-before.json。不得拒绝这些既有目标，不得覆盖它们。
- 沿用上一轮build-write-limit-command.json，单一模型obj-owner（BUF3,SharedPoolMode0,SourceReadGateEnable1,其他诊断0,无TEST_COMPLETION_GATE），把tests/nonblock_actual_dma_top.sv那一项替换成新out/owner_actual_dma_top.sv，并在它之前插入新out/ordinary_copy_owner_gate.sv，确保模块顺序与唯一替换。system_top副本/C++/TC副本和Mdir迁入新out，保留src sha证据。不自动打开HeaderPark/latepool。
- 先复制自然coupled CONF0 driver（上一轮natural_coupled_test.cpp）作为build driver仅机械复制，不生成巨型新CPP；可额外准备CONF1 driver副本及明确C++ relink命令，但是不重编RTL且默认不运行不必要CONF1已知几何不满足的实验。只准备V先/A先32KiB 两条默认run命令、独立日志CSV/result文件名。不构建不跑。
- 仿真结果脚本如不是必要可不写，已有构建/运行执行由Codex工具做，明确run argv和output files足够。
- 可读原gate模块并提示关键未证，勿把模型不支持等价普通方法负结果/创新。给完整修订脚本一个Python代码块、简洁修订点、根目录绝对路径和验收输出manifest接口。没有秘密/外部API资料；thinking/high默认max393216保持。

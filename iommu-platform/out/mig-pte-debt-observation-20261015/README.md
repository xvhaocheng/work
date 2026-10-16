# MIG首条债务观测：第6次时序未通过，停止并暂停

实际goal1792023314未完成；按用户指令在交付同步后调用paused，不标complete。第6次Make0但最终WNS−0.391ns／598 setup违例；0次新BIT加载、0次工作负载、无真实MIG记录。债务未知，不能写成零。没有第7次构建、失败后物理优化或收紧边界后的采集器改动。

- [完整报告](../../docs/mig-pte-debt-observation-20261015.md)、[稳定根结果](root-result-acceptance.json)、[实际暂停回执](root/goal-pause.json)、[交付核对](root/delivery-verification.json)。
- [失败BIT／MCS／DCP与报告](final-build6/DO_NOT_LOAD.txt)、[逐文件SHA](root/final-build6-manifest.json)、[最终关键路径](root/sixth-final-critical-paths.json)、[第6次完整日志](root/build-attempt-6.log)。禁止加载或烧写失败位流。
- [收紧后的执行合同](root/execution-contract.json)、[判定边界](root/decision-boundary.json)、[继承原观测合同](../shared-service-control-boundary-20261015/native/observation.md)。只选首个合格PTE，不筛正债务，原A8/gap0/V64/warmup256正常4096B COPY保持，实际未执行。
- [源码冻结与构建输入](root/build-inputs-attempt-6.json)、[私有插桩补丁](root/full-private-instrumentation.patch)、[实际参数](root/actual-parameter-check.json)。生产源码未改，旧分支与旧席未恢复。
- [观测器与字段](native/schema.json)、[源owner](capture/source-schema.json)、[源原文](capture/source-evidence.json)、[当前功能接受范围](root/current-native-acceptance.json)、[全部工具费用](root/observation-cost.json)。native120／CDC29／解码器5组及source owner通过，不是板测；OOC通过未替代完整时序。
- [原工具字节归档](root/original-benchmark-copy.json)、[一次运行脚本（未执行）](capture/run_one_observation.sh)、[只读回工具](capture/mig_capture_read.c)、[记录校验器](capture/validate_capture.py)。
- [实施修正与范围](root/observer-implementation-refinement.md)、[前三至五次诊断原件](before-routefix/)、[codec前原件](root/before-compressed-log/)、[任务前五页](before/)、[最终同步前五页](pre-final-sync/)。原通用关联版本、未完成构建和所有日志均保留，不冒作验收。

旧close-platform.tcl的optimize入口本轮禁用；verify-sixth.tcl也因已取得决定性时序失败而未运行。没有加载、采集或增加验证矩阵。未来新机制任务须先给出真实可控点、具名普通不足及改变约束的新动作。当前停止取证，不自动改论文定位；论文未完成。

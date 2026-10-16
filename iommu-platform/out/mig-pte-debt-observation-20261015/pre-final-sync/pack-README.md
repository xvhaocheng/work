# 真实MIG单条债务观测（执行中）

实际goal创建于1792023314，active，无额外token预算。当前未加载新BIT、0次冻结工作负载尝试、没有真实MIG记录，也没有新机制接受结论。LR1及所有具名关闭结论保留；论文未完成。

唯一目标是原A8/gap0 PN、V64、warmup256、mode3、正常4096B COPY窗口内，首个合格PTE的MIG实际AR接受、前序payload剩余R拍及其消解。选择不依赖正债务，不搜时相或额外压力。

- [实际goal登记](root/goal-registration.json)、[执行合同](root/execution-contract.json)、[继承观测合同](../shared-service-control-boundary-20261015/native/observation.md)。
- [私有源快照说明](root/stage-base.json)、[冻结构建输入](root/build-inputs-attempt-6.json)、[实际参数核对](root/actual-parameter-check.json)、[完整插桩差异](root/full-private-instrumentation.patch)。生产源码未改，未恢复关闭分支。
- [MIG观测器](native/mig_read_debt_observer.sv)、[字段布局](native/schema.json)、[跨域读回](native/mig_capture_cdc.sv)、[实施修正及范围](root/observer-implementation-refinement.md)。原通用关联版本不能满足UI5ns，其源码/功能回执/估计时序已保留，不作为硬件通过。
- [源owner记录](capture/mig_source_owner.sv)、[完整源字段](capture/source-schema.json)、[关键源码原件](capture/source-evidence.json)、[私有PTW/TL导出补丁](capture/private-ptw-owner.patch)。
- [当前新功能与独立综合接受](root/current-native-acceptance.json)、[源RAM修正](root/source-RAM-revision.json)、[工具成本](root/observation-cost.json)。120项新native检查、源owner与复位复用检查、29项异步两拍读回检查通过；修正后v2 UI5ns独立综合WNS＋1.020ns（未布线估计），完整平台时序未验收。
- [唯一运行脚本](capture/run_one_observation.sh)、[只读回工具](capture/mig_capture_read.c)、[记录校验器](capture/validate_capture.py)、[身份验收模板](capture/identity-requirements.json)。模板中的false/null等待root证据，不是板测通过。
- 当前全平台构建日志：`root/build-attempt-6.log`，实际运行日志位于`实际Vivado synth_1/impl_1日志`及后继impl_1。前几次仅是新插桩实现诊断，原工作负载均未运行。

真实板端路径已核：本机FT232串口root shell、Digilent 200300BD5148B/xc7k325t JTAG及板端网口。当前内核与历史PN不同，旧加载BIT字节身份未知；新记录必须单独绑定实际BIT/clock/reset/kernel，不能宣称复现旧12.7%慢化。

全平台时序通过→记录实际加载身份→只运行一次冻结工作→核owner/覆盖/错误/逐拍债务→root判定机制空间→同步状态、交接、总/局部计划和验证记录，之后才允许goal complete。正债务也不等于完整任务收益；零债务或缺测不得用来扩大否定问题族。

用户最新上限：只等待第6次构建与必要验收，不再扩展采集器或重构，不自动第7次构建。最终验收只运行root/verify-sixth.tcl（只读已有DCP/BIT）；旧close-platform.tcl的optimize入口本轮禁用。失败、身份/功能不通过、采集无效或需要重构时先保存并同步，再按明确授权暂停实际goal，不标complete。有效零债务接受其单片段范围。

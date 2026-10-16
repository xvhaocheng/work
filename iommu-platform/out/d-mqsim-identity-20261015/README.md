# D-MQSim身份定位、私有修复与持续状态边界

2026年10月15日。固定身份根因与四输入完成门通过；四个合法host＋模拟PCIe排空边界活动GC均0，停止本版cache-off有限覆写残留扩测。下一DRAM行/刷新边界合同尚未执行。

- [root稳定接受](root/root-result-acceptance.json)、[裁决与范围](root/decision-analysis.md)、[根因原文/日志行](root/diagnosis.md)、[初始身份接受](root/identity-acceptance.json)、[缺失请求摘录](root/missing-request-events.json)。
- [观察实现与4次构建账本](implementation/README.md)、[观察补丁](implementation/identity-observer.patch)、[最终构建manifest](implementation/pre-build-manifest.json)、[构建结果](implementation/build-status.json)。实现README保留当时交付记录；root后续实际执行以root接受和run manifest为准。
- [唯一初始身份重放原始日志](implementation/run-2048/stderr.log)、[启动前manifest](implementation/run-2048/pre-run-manifest.json)、[运行结果](implementation/run-2048/status.json)；旧全部native事件/XML完全一致。
- [私有修复合同](root/repair-contract.json)、[root审查](root/repair-review.json)、[私有补丁](implementation/repaired/repair.patch)、[修复交付](implementation/repaired/README.md)、[第五次构建manifest](implementation/repaired/pre-build-manifest.json)、[构建结果](implementation/repaired/build5-status.json)。
- [四case原始运行与manifest](implementation/repaired/runs/)、[逐14848请求核账/748hash/4负变异](root/repair-verification.json)、[被丢请求修复后的自身完成链](root/repaired-target-events.json)。
- [下一DRAM最小来源合同，未执行](root/next-minimal-contract.json)、[文档整合回执](documentation/receipt.json)、[写入前5份入口快照](documentation/before-write/)。
- [上轮来源报告](../d-persistent-source-20261015/README.md)及其全部原始源、8运行、1调试重放与当时未建立裁决原样保留；本轮是后继新证据，不改写旧run。原始入口快照另在[before](before/)。

实际新增5次私有host构建、1次身份重放、4次修复case；root审查后亲自工具执行，独立核账不运行模型。仅四个冻结整页USERIO write-only/cache-off单资源输入获得模型因果完成验收，不是通用上游修复、真实数据字节正确性、SSD实测时间、P/PN信息价值或新意证明。禁止调seed/cache/相位/长度追本版正例，不否定SSD全部状态或D；D/I/A安排、CS1局部停止、旧硬件暂停与生产源码保持。

共享IOMMU Codex-only，无DeepSeek/Claude。普通实现/文档请求Sol/medium，有界正确性设计Sol/xhigh；实际推理元数据unknown，历史模型记录不重写。文档检查没有执行模型、测试、构建或新source调研。

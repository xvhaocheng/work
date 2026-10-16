# 当前状态 stage111 2026年10月13日 R107有限卡归档，普通实板恢复完成

- 整体仍阶段一立题；本有限R107卡归档，无G0/G1，方法论文仍立题。原真实goal交付齐备、无额外预算，待最终工具complete。[稳定裁决](iommu-platform/out/r107-ordinary-pilot-20261012/root-result-acceptance.json)。
- r1实际boot370a9dd6…／6.5.0-rc2-r107-ordinary-stage111-r1／S0匹配config/notes/DTB；f9冻结bit不含当前独立57eb562 RTL。普通4×4MiB／8真实2MiB叶／16MiB backing；一完整priorout＋restore各1,769,472B/432tasks、144whole batches、24worker-layer/12joint、全字节/guard/原UAPI与DONE/quiet及checked释放通过，quarantined0。[实际校验](iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/validation/actual-finite-pilot-validation.json)。
- 先前换出8miss/8S1walk，驻留恢复1728lookup全hit/0walk；付出prepare/priorout成本后普通路径覆盖本合同，未成立新G0翻译残余。REFILL和48/96不同布局的差不全归翻译，不推原生GPU或全共享领域。[报告](iommu-platform/docs/r107-ordinary-pilot-20261012.md)。
- kernel whole nominal21.960s，恢复45.192ms、条件Dell40.557–49.914ms；种子/全字节/输出/构建/部署/回传全部分列。原始周期、名义ns、独立Dell及条件率界不混；SI准确度/精确peak RAM未验实。[成本](iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/costs-and-boundaries.json)。
- 原pre-NEXT EBUSY属于root把配置idle误当DMA quiet；旧失败/guard/source完整保留，未作科学阴性。必要一文件修复实际冻结RTL/精确C29项通过，第二干净构建与同任务授权安装后实际一次完整硬件pilot通过。[失败](iommu-platform/out/r107-ordinary-pilot-20261012/failure-root-cause.json)／[账本](iommu-platform/out/r107-ordinary-pilot-20261012/execution-ledger.json)。
- 下一关口：phase1一个有新增事实的显式需求合同，说明合法大页/premap/residency/batch/授权单DMA为何不可覆盖；真实共享/生命周期/字节/完整成本齐备、具体可避翻译loss成立才G0。后继合同inactive，未登记新goal，不扩场景/压力、不恢复关闭分支、不重跑stage106/108。
- 生产/config/cache/原history与181旧主输入保留；board同bootS0/idle1/quarp0，UART释放。全Codex Sol/ultra，无DeepSeek/Claude。主与初始核验席原生500000/有效475000、最大872000分列已验。[模型证据](codex-sol-runtime-verification-20261012T072912Z-01a12eff/REPORT.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

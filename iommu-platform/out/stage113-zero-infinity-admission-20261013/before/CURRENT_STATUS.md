# 当前状态 stage112 2026年10月13日 新增消费需求裁决完成

- 整体仍阶段一立题。真实goal已由工具标complete、无额外预算，本轮交付齐备；归档C112逐层归并的当前FPGA翻译缺口投影，R107原有限卡继续归档，无G0/G1，方法论文未完成。[稳定裁决](iommu-platform/out/post-r107-demand-gate-20261013/root-result-acceptance.json)。
- 新核固定vLLM67d96c29的RowParallel/GraphAllReduce：12层两处归并、持久GPU buffer复用，完整容量M未给。真实任务存在，但原板joint layer仅全恢复后CPU字节检查，不是attention/collective；新的partial数据、物理requester/共享翻译路径与完成接口未成立。[来源、合同与边界](iommu-platform/docs/post-r107-demand-gate-20261013.md)。
- 同两序列decode的源码形状为8 padded行×768×2B＝每rank12,288B，两活动prefix24,576B；24次rank逻辑输入合计589,824B，独有有效归并结果73,728B。wire/DMA/计算/到达/期限均未验。若新合同授权既有GPU池backing slack，可放在原leaf内，无新增叶；只作普通布局见证，未执行，也不覆盖未知M/全模型资源。
- 既有实板ordinary资产保留：4×4MiB/8真实2MiB叶；priorout/restore各1,769,472B，恢复1728hit/0miss/0walk、完整字节/quiet/checked释放通过。旧成本、假设和来源界限不改；不推C112性能。[原始接受](iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/validation/actual-finite-pilot-validation.json)。
- 归档后路线回phase1来源/需求选择：一个明确的自然独立搬运、实际共享路径及普通控制外新事实先成立，再G0；不扩当前参数/压力、不恢复关闭场景、不建collective模拟器。[inactive下一合同](iommu-platform/out/post-r107-demand-gate-20261013/next-goal-contract.json)。
- 旧三席历史FINAL均已完成、12关键产物入口保持原路径，当前仍pending_init；当前无close接口，未中断/归档/关闭或声称释放，占用原因未知。新席spawn失败一次后root完成核查；若后续确需独占席，先保存交接进入新主会话，保持Sol/ultra。[原始生命周期核验](iommu-platform/out/post-r107-demand-gate-20261013/validation/agent-lifecycle-review.json)。
- 19旧关键输入保留；四个直接依赖取自原官方archive。零生产/config/cache/DB/history改动、板命令、新build/RTL/旧回放/校验器重跑。Codex-only，无DeepSeek/Claude/Astra；本轮未重新核板端身份，最后已验状态见stage111。[账本](iommu-platform/out/post-r107-demand-gate-20261013/execution-ledger.json)。

- 用户已要求换新主会话继续；[完整交接](iommu-platform/out/new-main-handoff-20261013/README.md)、[启动提示词](iommu-platform/out/new-main-handoff-20261013/START_PROMPT.txt)、[下一有界合同](iommu-platform/out/new-main-handoff-20261013/next-goal-contract.json)已保存。新席执行stage113来源选择/可达性裁决，本席未登记新goal或开始研究；旧席未关闭，Sol/ultra与关闭范围不变。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

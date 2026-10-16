# 当前状态 stage109 2026年10月12日 R107执行前提有界裁决完成

- 整体仍为第一阶段“问题与方法立题”。实际goal已登记、无额外token预算；本轮条件保留R107合同资产，执行前提未全过，无普通恢复试验／新G0／G1或论文完成声明。[稳定裁决](iommu-platform/out/r107-execution-gate-20261012/root-result-acceptance.json)。
- 已在Dell串口取得当前riscv64／cpn3-clean Linux，新boot ID db0664f0-2d67-4b0d-91fb-c67d7557abe6，taint0、四idma节点。live DTB与本地hash完全一致；live kernel notes与本地ELF且Image内84B notes一致。eth0为192.168.77.2/24、UP/LOWER_UP；没有改Dell网络或把旧SSH当本次成功。[当前原始证据](iommu-platform/out/r107-execution-gate-20261012/identity/current-live-evidence.json)。
- 可复用候选仅为composition route-opt bit f9fa9434…，冻结24项overlay可对齐，内部50MHz时序WNS+0.022／WHS+0.051ns、失败0。原manifest f15f8a72…为−0.219ns／setup1失败，不能混用；6 clock critical warnings及IO/CDC缺口仍在。[构建证据](iommu-platform/out/r107-execution-gate-20261012/identity/composition-current-evidence.md)。
- JTAG旧wildcard选中两个接口的错误已修；精确主目标1MHz访问成功但PART=Unknown_Device、IDCODE0x00000001，原因未知。当前载入bit、live S/N/P/PN、物理时钟及精确dirty内核源码/config冻结未验实。DT声明CPU50MHz/timebase25MHz，不代替实测；buildmode3/reset S0不代替运行模式。[JTAG原始诊断](iommu-platform/out/r107-execution-gate-20261012/route/jtag-route-diagnosis.md)。
- stage108完整OPT125m有限合同保留：3唯一槽、144完整恢复／1,769,472B、此前换出同量；只source/CPU回放，无native trace/GPU计算。663,552B只是proxy标记。普通96笔合并、同owner单DID、4×4MiB或8×2MiB、合法4KiB/premap/residency/batch均保留；未重跑旧矩阵或CPU回放。[有限合同与成本](iommu-platform/docs/r107-bounded-decision-20261012.md)。
- 必要ordinary adapter范围已核，未实现／分配／映射／恢复；实际leaf/fence/cache/全成本及翻译残余仍未知。下一只补有效FPGA链／本次bit和S0、精确内核冻结、相关时序处置等新事实，再执行同实例普通采证。若接口不可达或普通覆盖则归档此卡，回到阶段一新需求合同，不自动恢复关闭候选／扩场景。未登记后继goal。[下一合同](iommu-platform/out/r107-execution-gate-20261012/next-goal-contract.json)。
- 本轮Codex root＋两Sol／ultra独占席；零DeepSeek／Claude／Astra、新build/DUT/flash/GPU/恢复性能试验。实际有UART登录／只读身份与JTAG采证，两个授权临时设备ACL；现有Linux保持运行、UART释放。129个旧输入不变。[本轮报告](iommu-platform/docs/r107-execution-gate-20261012.md)／[账本](iommu-platform/out/r107-execution-gate-20261012/execution-ledger.json)。
- 本新主会话和独占核验子席实际gpt-6.1-sol／ultra、原生500,000／有效475,000已验实，最大支持872,000分列，未改模型配置；旧258,400历史保留。[本次模型核验](codex-sol-runtime-verification-20261012T072912Z-01a12eff/REPORT.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

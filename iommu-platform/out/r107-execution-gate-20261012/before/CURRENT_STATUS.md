# 当前状态 stage108 2026年10月12日 R107有限合同与有界立题裁决完成

- 整体仍为第一阶段“问题与方法立题”。本轮实际goal已登记、无额外token预算；R107条件保留有限合同资产，没有新G0／G1、创新或净收益验收。[稳定裁决](iommu-platform/out/r107-bounded-decision-20261012/root-result-acceptance.json)。
- 固定vLLM v0.1.0＋官方OPT-125m完整12层／12heads，TP2分片为明确研究选择。一次源函数／CPU全字节回放通过：3唯一槽、144完整恢复、1,769,472B；此前换出另计同量。无原生请求trace／GPU／FPGA执行。[有限合同与数据](iommu-platform/out/r107-bounded-decision-20261012/instance/finite-contract.md)。
- 两序列共享前缀、同可信owner，允许授权单DID合并四池；普通连续run可减至96笔而完整字节不变。live3、恢复保守预留5均容于8槽；不主张容量强迫。663,552B仅为CPU checkpoint代理标记，不是native attention必读量；真实消费时刻未知。
- 普通四完整4MiB／order10池在当前内核大小上合法（只拒order>MAX_ORDER）；八独立2MiB块和合法4KiB回退均保留。四池逻辑9MiB、准备backing16MiB、目标8叶；实际allocation/map/leaf/cache/完整成本未测，8叶<16IOTLB项不证明零miss。[普通合同与成本](iommu-platform/out/r107-bounded-decision-20261012/platform/ordinary-baseline-contract.md)。
- 硬件preissue SRC/DST/LEN/NEXT/DONE已存在；Linux helper缺有限manifest／稳定池／层事件普通adapter，不能当科学损失。logical free不撤整池映射；源码event/refcount已有安全能力。尚无普通强对照未覆盖的翻译损失证据，重复未变源码／CPU回放到此停止。[本轮报告](iommu-platform/docs/r107-bounded-decision-20261012.md)。
- 下一步只在实际Image/DTB/bit/mode/时序身份及明确普通adapter范围成立后，做同一实例一次普通准备／完整完成采证，保留大页／预映射／驻留／批量／授权单DMA。无新输入或普通覆盖则归档此卡；残余可避免翻译损失成立才登记最小G0，之后才G1。下一合同未激活、未登记后继goal。[下一合同](iommu-platform/out/r107-bounded-decision-20261012/next-goal-contract.json)。
- 主平台仍Genesys2／CVA6＋四iDMA＋RISC-V IOMMU＋Linux，服务器辅助。stage106八案和私有credit修复保留、生产未合入；mode0每路3beats／源码mode1前两路512／旧私有512分列，live身份与时序未核。本批root＋两Sol／ultra独占席，零DeepSeek／Claude／Astra、新DUT/build/板测/物理分配/生产修改；root643项、平台200项通过，73旧输入不变。[账本](iommu-platform/out/r107-bounded-decision-20261012/execution-ledger.json)。
- 本新主会话和本轮之前的独占核验子席实际gpt-6.1-sol／ultra、原生500,000／有效475,000已验实；最大支持872,000分列，未改配置。旧258,400历史记录原样保留，不冒称旧席窗口已变。[本次核验](codex-sol-runtime-verification-20261012T072912Z-01a12eff/REPORT.md)。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

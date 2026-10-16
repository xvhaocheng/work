# 当前状态 stage114 2026年10月13日 同一C113原生证据关完成

- 整体仍阶段一立题。C113来源合同条件保留并加强自然NVMe容量需求，当前FPGA翻译实验不准入；无G0/G1、新规则或论文完成。R107/C112及先前关闭范围保持。stage114真实goal已在完整同步交付后由工具标complete，无额外预算，见[实际完成回执](iommu-platform/out/stage114-c113-native-evidence-20261013/goal-completion.json)为准。[稳定裁决](iommu-platform/out/stage114-c113-native-evidence-20261013/root-result-acceptance.json)。
- 新亲核原论文Table1/Figure5c单节点0.5T记录：16参与GPU、MP1、h18432、124层、seq1024、batch/GPU8、参数/optimizer均NVMe。标称FP32 p/m/v约6e12B超过CPU1.5TB上限，加强非人为容量需求；不是exactP/H/filebyte或v0.16.4运行。合法tile/pool驻留、大页、persistentmapping、布局/批量/同owner单设备继续保留。[报告与成本边界](iommu-platform/docs/stage114-c113-native-evidence-20261013.md)。
- 固定v0.16.4/e2dc3eeb官方功能配置补齐：单rank、两Linear2048、tmpdir/zero、B4/p1/q8、pipelinefalse；whole-parameter subgroup非100元素微tile。每稳定成功step有界预期AIO256MiB，非实际DMA/wire/DDR；test源码6次step含checkpoint恢复后同批重做，未运行，数值/成功byte/quiet/实际H未知。[完整条件字节](iommu-platform/out/stage114-c113-native-evidence-20261013/source/config-derived-byte-contract.json)。
- 精确版本原始issue3024为CPUoffload，排除为NVMe运行；精确sha CI查询0条只是不取得run，不是历史从未测试。仍缺同一nativebundle绑定version/launcher/config、path→FS/block/PCI/IOMMU/domain/sharedunit与自然peer、N_i/B/p/H/pages、结果/byte/完成/释放和ordinary完整成本残余。公开资料有界核查收束；无新输入不重复相同检索/重写未变卡、不搭新系统或自动追加第二workflow。[inactive下一合同](iommu-platform/out/stage114-c113-native-evidence-20261013/next-goal-contract.json)。
- Codex-only，root按Sol/ultra要求，新独占Sol/ultra/fork-none子席只核publishedrun/硬件证据并完成；root亲核26组＋PDF4/10/14原图，子席18组原文/视觉和12来源hash。未恢复旧席、修改原生context默认或声称旧席释放；主运行model/effort/window元数据未独立可读，不用旧记录代验。[证据导航与实际账本](iommu-platform/out/stage114-c113-native-evidence-20261013/README.md)。
- 原stage111有限实板ordinary（8真实2MiB叶、恢复1728hit/0miss/0walk与完整字节/quiet/checked释放）和stage113源码/成本原样保留，仅是各自原合同；本轮无新board/GPU/NVMe部署/身份核查、实现/build/RTL/driver、旧测试/实验/校验器重跑或生产/config/cache/DB/history改动。零DeepSeek/Claude/Astra；缺证据不作科学阴性。

- 独立Linux贡献：IOVA／HPM宏已拆成单提交单文件 `8e3a7406fadb`／`77805904b3c0`，推送 `xvhaocheng/linux_iommudirver`；真实目标 `malejo97/linux:wip/riscv-iommu`，非Linux主线缺陷声明。主机回归16/16、32/32通过；创建PR受GitHub集成403阻断，尚无PR。[补丁、说明与预填入口](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。生产Linux改动保留；不改变R107裁决。

- 独立上游贡献：DS／PTW修复`7eb7517`／`078572b`已由用户网页创建上游[PR52](https://github.com/zero-day-labs/riscv-iommu/pull/52)／[PR51](https://github.com/zero-day-labs/riscv-iommu/pull/51)，目标mc/dti-ats，均1提交／5文件、待审。其余PR只读对照：47/49排序缺口应按平台适配；46/48在实际MRIF使用前修；45需当前FIFO路径回归；42可选缓存优化、43功能扩展暂不需要。无实施／新测试。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)。

- 独立RTL维护：AXI 4KiB边界检查页尾误拒绝／INCR半长度漏检及WRAP位宽已修，IOMMU commit `57eb562`（xvhaocheng）；模块回归816,064项全过，旧版112,514项失败。无新FPGA/板测，CVA6／平台锁定指针未更新，独立维护不改变R107裁决。[验收](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)。

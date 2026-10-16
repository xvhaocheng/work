# R107执行前提有界裁决（stage109，2026-10-12）

**条件保留R107有限合同资产。本轮执行前提没有全部通过，不进入普通恢复试验、新G0或G1。** 已核当前Dell可访问的板端Linux；历史构建中确有可复用的优化bit。当前载入bit、运行模式和完整内核源码冻结仍未验实，JTAG未知器件的原因未知。缺实现或身份不明不作为普通方法的科学失败。本关口完成不代表方法论文完成。

本轮具体问题是：同一R107有限回放是否已经具备可辨认、可访问的实际执行环境，以及一个必要且有限的普通adapter范围。依据是stage108稳定裁决及其未改的next-goal-contract；进入条件为实际镜像／source／mode／clock／timing／route与接口闭合，结束条件为推进一次普通采证、列明新事实的条件保留，或有理由地归档。实际goal工具登记，无额外token预算。

| 项目 | 本轮实际证据 | 裁决与限制 |
|---|---|---|
| 当前Linux | UART成功登录；riscv64；6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty；taint0 | 两次只读查询同一新boot ID：db0664f0-2d67-4b0d-91fb-c67d7557abe6；非历史boot |
| 当前DTB／内核构建身份 | live DTB SHA256 aa9cdf4c…与本地3510B完全一致；live84B kernel notes SHA256 9dc6815e…与本地ELF一致 | notes精确包含在Image中，GNU build ID 1e019b55…；证明构建标记对应，不是全运行内存哈希或dirty源码冻结 |
| 当前访问 | /dev/idma0–3；eth0 UP/LOWER_UP、192.168.77.2/24 | Dell UART访问成立；未改Dell网络地址、未把旧Mac网段当本次SSH成功 |
| 可复用优化bit | route-opt/ariane_xilinx.bit，SHA256 f9fa9434…，11,443,722B；与Mac交付包相同 | 静态冻结overlay24项可对齐；不能推出当前运行此bit |
| 两份原始时序 | 原manifest f15f8a72…：WNS−0.219ns，setup失败1；优化版：WNS+0.022ns／WHS+0.051ns，失败0 | 原版拒绝借用优化结果；优化内部约束50MHz通过，但保留6 clock critical warnings、21/23缺输入／输出delay与IO/CDC未完成边界 |
| 当前JTAG | 旧wildcard同时选2目标，已用完整末段修正；主目标1MHz打开成功 | 实际PART=Unknown_Device、IDCODE=0x00000001；原因未知。用户确认电源开、有亮灯，仍不能填入xc7k325t或载入bit |
| 运行模式与时钟 | 冻结build mode3，reset S0；缓冲512/512/3/3。当前DT声明CPU50MHz、timebase25MHz | live S/N/P/PN及物理频率未测；build mode3不是运行PN3，reset常量不是本次观测 |
| 内核与普通接口 | Image／DTB及build note身份明确；完整dirty源码／config冻结未知 | 在已核源码中，caps2不提供当前composition mode，RUN_COMPOSITION会配模式并执行任务，不为读身份跑旧实验 |

原始入口：[本次live证据与完整hash](../out/r107-execution-gate-20261012/identity/current-live-evidence.json)、[认证后UART原日志](../out/r107-execution-gate-20261012/route/live-authenticated-UART.raw.log)、[同boot构建与网络原日志](../out/r107-execution-gate-20261012/route/live-kernel-notes-UART.raw.log)、[JTAG完整枚举](../out/r107-execution-gate-20261012/route/live-JTAG-enumeration.raw.log)、[实际未知器件读取](../out/r107-execution-gate-20261012/route/live-JTAG-exact.raw.log)。[静态构建证据及精确原文行号](../out/r107-execution-gate-20261012/identity/composition-current-evidence.md)分列优化前后bit；root亲核优化原报告159/174行、原版159行、模式／缓冲源码及当前Linux ioctl调用链。

普通adapter范围已成立且有必要：stage108仅有来源／CPU全字节回放，只有同实例普通路径实测才能补实际准备和完成成本。范围是一个可信owner、四稳定池、不可变manifest、完整144笔或合法合并96笔／1,769,472B恢复、24 worker-layer和12共同数据就绪标记。663,552B仍只是CPU checkpoint代理标记，不是native attention必读量；没有原生GPU计算／到达／期限。

保留四完整4MiB/order10或八2MiB/order9、16MiB backing、目标8叶及合法4KiB回退，允许预映射／驻留／批量和授权singleDID合并。Linux大小上可行不等于此次实际分配成功，8目标叶小于16项不证明零miss。实现须提供内核所有的池／边界校验、映射与真实叶、可靠fence/cache handoff、NEXT/DONE完整完成、层依赖、drain/reacquire及全字节／guard验证；普通安全snapshot需受同一锁保护。旧stage106私有credit修复未包含在此bit，不能把其已验结果冒称当前生产正确性；必要修复或合法分段属于普通实现成本。

全部准备／初始化／此前swapout／map／leaf／fence／cache／issue／done／CPU代理／drain／reacquire／verify／unmap／free成本目前未知；首次、稳态、销毁以及失败／回退／隔离费用必须分列。应用逻辑四池9MiB、普通backing16MiB等为既已核的资源合同，不是本轮已分配资源。此轮未实现adapter、分配／映射池、运行恢复或取得翻译损失。[明确接口范围](../out/r107-execution-gate-20261012/critical/ordinary-adapter-scope.json)、[已接受完整成本清单](../out/r107-bounded-decision-20261012/platform/cost-inventory.json)。

需要的新事实依次为：当前有效FPGA链／载入bit的可信身份与普通S0观测；匹配必要adapter的精确内核源码／config冻结；对相应性能结论涉及的clock／IO／CDC缺口给出具体处置。当前Linux已经可读，不再把sudo、串口登录、线缆未接或无driver作为待办。现有Linux保持运行，UART已释放；未为证明历史bit而自动刷写、复位或重启。

这些事实齐备后，才必要实现并执行同一有限普通准备／完成采证，复用已验校验器。普通强对照未覆盖的可达、可避免翻译相关残余成立，才登记最小G0；G0支持后才G1。若平台不能提供合法接口，或完整普通对照／成本已覆盖所需目标，归档这张R107卡，回到整体阶段一以保留文献及E2/E3边界形成新的明确需求合同；不会自动重开行预取、HERO、冷权重或copy等关闭分支，也不会自动增加第三场景。[未激活后继合同](../out/r107-execution-gate-20261012/next-goal-contract.json)。

实际执行包括Mac授权只读参考、当前UART登录／两次成功身份查询（最初未登录查询失败保留）、3次有新证据理由的JTAG步骤。主机仅执行用户授权的两个设备临时ACL和本机系统凭据库保存／非秘密复用引用；未安装driver、改sudoers／用户组、改网络、重启既有服务。密码未进入项目、报告或串口日志。全部工作由Codex root＋两独占Sol／ultra席完成，零DeepSeek／Claude／Astra、旧矩阵重跑、新DUT／FPGA构建／flash／GPU运行。129个stage108及旧输入文件保持不变。[实际goal登记](../out/r107-execution-gate-20261012/goal-registration.json)、[稳定裁决](../out/r107-execution-gate-20261012/root-result-acceptance.json)、[执行账本](../out/r107-execution-gate-20261012/execution-ledger.json)、[核验记录](../out/r107-execution-gate-20261012/validation/root-evidence-check.json)。

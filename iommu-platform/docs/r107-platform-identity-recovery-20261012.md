# stage110 R107平台身份与恢复进入关（2026-10-12）

条件保留R107有限合同。JTAG原因、指定bit载入、新Linux启动及实际S0已核实；旧内核全Image/ELF重现仍未通过，本轮没有普通恢复采证、新G0/G1或论文完成声明。

实际goal登记，无额外token预算，依据stage109有效next-goal-contract。原问题是：当前平台究竟运行什么、必要普通恢复接口应绑定哪份源码。进入恢复的条件是对应的平台/内核/接口成立并有可解释的计时与完整成本；结束条件是这些事实的稳定裁决及明确后续，不扩大场景或重跑旧矩阵。

## 已接受的实际身份

| 项目 | 原始实测与接受范围 |
|---|---|
| JTAG `0x00000001` | CPU DMI TAP默认IDCODE；旧无后缀target误当FPGA。FPGA在B后缀/channel1，实际xc7k325t、`0x43651093`，1MHz |
| 本轮指定bit | `f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca`，10月7日冻结优化bit；不含独立57eb562维护 |
| 实际载入 | 用户明确批准；两次同文件易失载入，第一次同文件掩码回读rc0；第二次不并行回读、DONE/EOS/GWE1、CRC/IDerror0并成功启动 |
| 新boot | `7fd964e5-563f-458d-9617-c890f737f068`；cpn3-clean/riscv64、taint0、四idma节点 |
| 配置／DTB／notes | config `b9af38b7…`、DTB `aa9cdf4c…`、notes `9dc6815e…`与原本地构建一致 |
| 实际模式 | observer V9、composition magic正确、ABI1；config `0xA00`=S0/DID10，generation0/status0/idle1；不是拿reset常量当观测 |

[原始FPGA身份](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/B-FPGA-status.raw.log:55)、[同文件回读成功](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/known-bit-program-and-verify.raw.log:72)、[新boot原始UART](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/live/known-bit-new-boot-software-UART.raw.log:20)、[实际模式原始UART](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/live/known-bit-new-boot-mode-UART.raw.log:29)。模式读取仅改变银行索引并还原，未写mode、ARM、START或NEXT，未执行DMA。第二次载入后没有再次回读；当前判断合用同文件实际加载回执、正常配置状态和新boot/接口值，不伪造第二次回读。

载入前优化与默认两个候选的掩码比较分别报15,848,937／14,894,878差异。两份mask均由保存的routed DCP生成，与原bit配置packet完全相同；没有重新综合或布局布线。此前配置仍无已识别文件，未登记自定义产物未知，用户现场确认原样保留，不把“最近／当前工作区”推成确定hash。根本原因未知，也不能推成bit损坏。[原结果](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/current-bit-verify-result.json)／[默认对照](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/current-default-verify-result.json)／[有限候选清单](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/current-workspace-bit-candidates.md)。

掩码回读的操作依据为AMD 2023.1 [UG908](https://docs.amd.com/r/2023.1-English/ug908-vivado-programming-debugging/Bitstream-Verify-and-Readback-for-FPGAs-and-MPSoCs)，mask指定跳过和比较的配置位；工具实际退出码与原日志决定本轮结果。

## 失败与一次有理由的恢复

第一次已知bit载入后，SD初始化/GPT读取成功，boot image复制中出现`FFFFFFFFFFFFFFFE`，尚未到Linux。冻结C及生成SV反解机器码都对应512B块CRC16不一致；不是超时、容量失效或翻译残余。[失败原UART](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/live/known-bit-reload-boot-UART.raw.log:52)／[原源码及ROM指令](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/sd-boot-failure-evidence.md)。

root只追加一次同已批准bit恢复，移除与SD启动重叠的configuration readback。初始120秒结束时仍有复制进度，依据同bit历史整流程496秒，另作一次最多360秒的被动串口等待，实际190.954秒到登录。两段捕获间的空档明确保留，未补造日志。第二次进入OpenSBI/Linux并通过随后身份与模式查询；这证明恢复可用，未证明前次CRC的完整物理因果。[恢复合同](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/known-bit-recovery-contract.json)／[载入回执](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/known-bit-recovery-no-readback-result.json)／[被动等待回执](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/live/known-bit-boot-extension-result.json)。

原清单bit不是“历史Dell程序直接复现”：旧实际链为Dell构建、Mac openFPGALoader1MHz载入，本次采用Dell现有Vivado2023.1和精确B接口；复用bit、冻结源、SD软件与UART协议。未改Flash、eFuse或SD镜像，未安装驱动、解绑USB或重启主机服务。当前Linux保持运行，UART已释放。

## 内核对应：正证据与未通过项

冻结base `c0f5e13ea81fa30d78f3bb1d6389e2397321e040`、5项tracked overlay、3项lowrisc源码及81,122项完整清单；新空O一次干净构建56.886秒。2079个实际链接输入顺序相同，2078个完整字节相同；root另直接哈希核对idma、IOMMU、io_pgtable及Ethernet四关键对象全同。config、rootfs和最终版本对象也完全相同。[root直接字节核验](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/root-direct-binary-check.json)／[链接输入原结果](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/reproduction-result.json:81)。

完整旧Image SHA `8c19da27…`，本次Image `70ac6480…`，均24,131,072B；ELF也不同，未接受全二进制对应。唯一对象差异为`init/version.o`临时版本banner增加32B，造成链接布局与构建ID等变化，没有发现关键驱动代码对象变化。一次定向metadata恢复曾生成匹配的临时对象，但没有独立副本；外层`make --old-file`未阻止递归Kbuild重生header/object，最终仍不匹配。保留全部失败和实际命令，停止追加恢复。[Image与ELF原结果](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/reproduction-result.json:92)／[详细重现](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/reproduction-result.md)。

因此接受“当前配置与关键编译执行路径有强字节对应”，不接受“完整旧Image已从冻结源精确重现”。下一走新的明确源码冻结普通执行内核与唯一release/notes，并匹配其真实boot；不继续追旧临时横幅或复用旧增量对象冒充来源。

## 普通路径、成本和失效边界

R107仍是已接受的source方法／有限研究回放：3唯一槽、完整144笔/1,769,472B，合法合并96笔同量；此前换出另计同量。四逻辑池9MiB、普通16MiB backing、合法order10/order9大页、4KiB回退、预映射、驻留、批量及同可信owner单DMA访问四池全部保留。663,552B仅proxy标记；本轮无native GPU、计算/到达/期限数据，也无stage106/108回放。[原有限合同与成本](/home/xvhaocheng/work/iommu-platform/docs/r107-bounded-decision-20261012.md)。

必要ordinary adapter仍未实现，真实分配/map/leaf、cache/fence、供给/层依赖、恢复/硬件完成/排空、全字节/guard核验及unmap/free/失败清理成本未知；本轮构建与启动账不能代替这些成本。内核完整对应项未全过，因此未进入普通恢复采证，不把接口实现缺口作为创新或科学阴性。

时序按路径处置：内部20ns约束WNS+0.022/WHS+0.051、失败0；实际DDR→CPU为异步FIFO Gray指针5ns max-delay、slack+4.238，反向最差涉及reset。已有IP结构不支持按六条警告直接判硬件坏，也不支持整体豁免。有限功能原始采证可条件进入，但须所有字节、硬件完成、错误/复位、完整阶段成本；物理秒/微秒与吞吐需独立计时校准。CPU50/mtime25MHz是报告/DT值，CPU派生的2:1比例不是独立实测；全面IO/CDC和更强翻译因果仍未接受。[原路径与准入裁决](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/timing-gate-disposition.md)。

## 稳定决定及下一关口

条件保留同一R107合同。JTAG/已知bit/新boot/S0这些问题已收束；新关口只绑定源码冻结的ordinary内核与必要接口，匹配实际运行身份后做同实例一次完整普通恢复与成本记录。部署先形成具体产物，当前批准不包含新SD镜像写入；没有当前独立计时事实时，保留cycles与host阶段时长分列，不能换算出未经核验的物理耗时。

具体可达且普通未覆盖的翻译相关残余成立后才最小G0；满足G0与前作/全成本审查后才G1。若合法接口不可达或普通控制覆盖目标残余，归档这张R107卡、回阶段一新的需求合同；不重启已关闭候选或扩场景。启动/metadata技术失败不等于这项科学归档条件。

[稳定JSON](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/root-result-acceptance.json)／[未激活下一合同](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/next-goal-contract.json)／[实际执行账本](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/execution-ledger.json)。root＋两独占Sol/ultra；未调用DeepSeek、Claude或Astra。133个stage109文件和46项内核原输入均未变，[保护及UART释放](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/validation/final-preservation-and-UART-release.json)。实goal仅在这些交付与文档同步完成后complete；本轮不代表方法论文完成。

实际`update_goal`已返回complete：工具累计1,181,423 tokens、5151秒（约86分钟），无额外token预算；这不是当前上下文占用或R107运行耗时。[实际工具回执](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/goal-completion.json:1)。

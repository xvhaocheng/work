# 原生服务边界与旧 PN 板端记录：最少缺一个原始因果事件

结论：已验 MIG 参考不重做。旧板端记录能延长到“同构建目录后布线 BIT、一次成功载入、相同 boot 和代表 case”的历史链，但不能从旧 V9 汇总计数得到 **PTE 被 MIG 接受时，前序 payload 还余多少真实 R 握手**。最少缺一个 owner 关联完整的自然 PTE 服务片段。本席只定义这个记录，没有实施新观测。

## 已有绑定及不能跨过的边界

- 20261014 参考绑定的是 work-fpga BIT，SHA `f15f8a72c02da3f19e4ed5beafe3bae61407636b99505d9ace31bf0f9716715a`。
- 历史板验收列 route-opt BIT；现保存文件实际 SHA `f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca`。saved result/Vivado log 明确其输入是同一 work-fpga routed DCP，经 phys-opt 后生成 BIT。这两个 BIT 的字节不同符合后处理历史，不能据此断定功能不同。
- boot01 载入回执记录 Mac `tools/composition-isolated-20261007/ariane_xilinx.bit` 的 loader rc=0 和新 boot `d61e7517-2b15-4702-86f2-3f338ddd5345`；isolated 4096-transfer 代表 case 与 PN-gap64 代表 case 的 boot/meta 均为该 boot/kernel。PN 探测回执记录 0 载入、0 reboot、same_BIT_and_boot。
- 所核 Mac loader 回执未保存载入文件 SHA；该 Mac 路径的文件不在当前本地路径。故接受历史来源链，保持载入文件严格字节身份、整个 BIT 内部与未封存生成 IP 的对应、当前加载身份为未知。不能把 kernel boot_id 当 FPGA BIT readback。

原文、每项实际 SHA 和行号在 [evidence.json](evidence.json)；核心入口：historical-load-receipt、historical-route-opt-vivado、isolated-case-meta、pn-case-meta。没有重做 MIG 原文判定。

## 为什么现有 V9 记录不够

参考 `dma_observer.sv:41–52` 的 V9 有 35 个主计数；代表原 record 保存 35-word observation 和 P4M1 16-word 窗口，未独立保存 V9 main magic。`ariane_peripherals_xilinx.sv:990–1000` 把 shared AR/AW tap 接到 IOMMU **tr 入口**。`dma_observer.sv:388–391` 的 sharedAWwait 只是该入口 `AWVALID && !AWREADY` 的周期和；没有 MIG ARLEN、每拍 MIG R、PTE owner 或 PTE 接受事件。

private stock 通过 ARaccepted/RLAST 每次 ±1（241–268），是 burst 数；它不是 remaining beat debt，且处于翻译/互连/CDC 前。top:1006–1089 表明 MIG destination 位于异步 converter 后，ui clock 为 `ddr_clock_out`。因此旧 stock、同 miss 数及总 AWwait 都不能回推出目标时刻的 `k`。同一 aggregate 允许不同地址、AR顺序、burst长度和 R 进度。mode3/legacy0 还禁用了旧 E3/PTW 历史（peripherals:1060–1089、1164–1185），旧 source 中存在的旁路观察端口不等于记录中有这些事件。

## 单个最少记录合同（待授权/实施，不运行）

固定原有合法 A8/gap0 PN 工作和原定观察窗口，只被动保留该窗口首个合格 PTE P 的 MIG 接受及服务片段，选择规则不依赖 K 是否大于0。保留完整窗口的 PTE 机会总数、选中 occurrence 序号与无 PTE／未记录／错误原因；不能把 D>0 才触发的一条记录当总体分布。不扫相位，不增加 pressure 或双头条件，不重复旧矩阵。

| 观察点 | 必需字段与作用 |
|---|---|
| 实际 MIG AXI64 destination，top `s_axi_ar*` / `s_axi_r*`，`ddr_clock_out` 上升沿 | 每个 accepted AR 的 epoch、全局接受序号、full 6-bit ID、30-bit地址、LEN/SIZE/BURST；逐 R 握手的 RID、RLAST、RRESP，另保留 held R 区段。定义真正已接受读债务和输出回压。 |
| 正常 TL/PTW owner 所在 source `clk` 域 | DID、AR/AW翻译类型/通道、owner generation/cookie、stage/level、PTE地址、PTW发起及接收 occurrence；关联这是哪个自然 miss 的 PTE。 |
| payload owner 源入口/完成源 | DMA端口/DID、descriptor generation、burst occurrence，与 MIG payload L 关联；不能只靠ID前缀称其为V/A任务。 |
| 记录 metadata | 实际 BIT/build/profile/observer身份、已知quiet的ARM起点、reset/calibration epoch、完整固定window覆盖／PTE机会总数／选中序号／无记录原因和错误/overflow/truncation状态，冻结 workload 请求。 |

**精確 debt：** 在 P 的 MIG `ARVALID && ARREADY` 边沿 `tP`，对每个更早接受的 payload occurrence j，

`k_j(tP) = ARLEN_j + 1 − 截至并包括 tP 的关联 RVALID&&RREADY 次数`。

先扣本边沿已握手的 L 拍，再 snapshot；P自身不计前序。记录全部 predecessor 的 owner 分类以排除把 CPU/DDTW 前序债务误称 payload。选择一项 `k_L>0` 作为单个见证，同时保留 `L.tARaccepted, L.tLastRfire, P.tARaccepted, P.tFirstRVALID, P.tFirstRfire`。不能以 RVALID 代 Rfire 或以 RLAST burst 计数代 LEN/逐拍记录。

**owner 与时钟：** full ID 是复用的源/本地标号，不是唯一事务。用 epoch＋接受序号／per-RID FIFO occurrence 关联返回；源端 PTE/payload owner 与 destination 用有序端点 tuple（fullID、地址低30、LEN/SIZE/BURST）及 generation 关联。重复 tuple 无法唯一匹配就拒绝该记录，不能补猜。PTW与DDTW须分开；已有参考ID48/49仅用于有条件导航，不能以所有DS流量都叫PTE。MIG债务和返回先后只用ui clock；source clk时间戳另记，不减跨域 cycle counter，不直接复用旧50MHz换算。ui Hz本席未新验。

**窗口/错误：** ARM前先取得已知quiet/empty的真实接受债务起点，随后连续跟踪，不在ARM、open或close清账。原P4M open若已有carry-in L，必须由此前连续记录的AR和R来带入；仅记录open之后的AR会漏掉L。关闭旧V窗口不丢弃P未完事件。无matched AR的R、RID occurrence错、早/晚/漏RLAST、beat/LEN不守恒、overflow/wrap/truncation、reset/calibration切换、RRESP错误、abort或分类不明均使该因果记录无效。分开 `RVALID&&!RREADY` 与无RVALID；前序held last证明输出回压，不能冒称native bank冲突。

## 这一个记录能裁什么

如果有效记录在 P accepted 后还有 `k_L>0`，且 L.RLAST握手先于P.firstRVALID，则由已验保序源语义与真实owner事实，接受“正常工作流确实遇到PTE前序payload读债务”的机会前提。它不能把整个PTE等待或旧约0.90M周期差额定量归给payload，更不能单独接受独有创新/完整任务收益。

如果 `k=0` 或 P先于L，只否定该片段的前序payload解释；以完整固定窗口机会数及无记录原因分开没有eligible PTE、未捕获和失效。窗口不全、记录丢失、计数溢出或owner不唯一仍为unknown；单记录不证明总体分布。共同选择前已接受的L是公用不可撤销债务，不能计候选收益。后续若要推进机制，root仍须证明一个真实可控制机会或合法新执行动作能够改变该债务，并与固定普通程序比较完整费用。单条服务记录不替代这个机会、普通对照、完整任务比较或完整成本；两个NormalTL队头只是旧F1机制的特定要求，不作为本轮问题地图或观测的通用进入条件。

本席零源码修改、实验/构建/测试/旧validator/trace重解析/板操作/goal调用；未恢复关闭分支或旧席，未调用DeepSeek/Claude/Astra，未再派席。

# §54 正常权限前沿：运行前设计

Codex 根席设计；2026-10-09。out-only；不改生产RTL/Linux，不编BIT、不上板。A/V均为正常、静态成功映射的2KiB copy，CONF129，两context独立READ/WRITE授权。共享源不可变；V目标页内0，A目标页内3072，真实legalizer产生A两个1KiB WRITE parent。

要判别的是：A下一WRITE页未成功授权时，V能否沿已经接受的长source R继续，而不是所有recipient一起等待。WRITE证书来自实际IOMMU成功结果；不把READ授权或旧页PA当作未来WRITE授权。

三种方法同q2 W、同两DMA私有B3 FIFO/单拍共享skid/recipe helper、同实际页表和延迟信息：mode1初始长共享AR，A授权前沿到达后清旧W/B/skid再临时退组，V继续，A随后补读；mode2普通按共同成功WRITE前沿截断共享source AR，下一页成功后共同续读；mode3普通独立读取，V先、A后，分别按成功WRITE前沿截断。mode3在本隔离版本中不再表示§52早恢复取消退组。K=256比较完整认证页；可另核K=2成本点。全部保持原父ID/RLAST和每actualWRITE父独立B。

sourceAR一旦firstVALID不得缩短或撤回；logical R冻结skid不得改members。私有rawAW捕获不受WR门阻挡，新global qAW才受门控。每actualWRITE父之间保留既有完整invalid周期，row1旧证书随之清除；不做零bubble换父。已成功WRITE job-end单调记录18bit；raw_written按job地址累计，groups不随父重置。

初始合同没有外部pause：本版本遇pause直接拒绝，避免把长singleton AR偷称为§51受限q暂停保证。A下一页PTE延迟必须有真实匹配/应用记录；每个目标全部字节及周边未写区校验，物理W每flow256拍、issue/done各一次。模式1预计source384拍/AR2，模式2 source256/AR2，模式3 source512/AR3；这是设计预期，须DUT证明。

如果mode1只在制造的次页授权等待中有收益而普通分片或同信息独立控制可以同成本复现，就不能以长读/退组命名独立算法。如果具体跨权限前沿控制形成强对照没有覆盖的可复查性能/工作量点，再与MMU-aware DMA、DMA-Latte、Torrent等原文核边界。故障恢复、动态失效、多作业、MIG时序、面积均不在本次成功合同内。

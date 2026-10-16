# stage110：SD启动失败的原证据

结论：本次复制boot image时打印的 `FFFFFFFFFFFFFFFE` 是 **每512B块的计算CRC16与收到的CRC16不一致**，不是超时。这个返回分支已从冻结C及冻结生成SV直接反解的机器码分别核到；究竟是SD/SPI/内存链路、CPU读取或并行readback影响，当前证据不能定因。

冻结C的完整入口是 [/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/bootrom/src](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/bootrom/src)，含 `sd.c/sd.h/gpt.c/main.c/spi.c`，没有使用生产C代替。实际FPGA使用 [bootrom_64.sv](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/bootrom/bootrom_64.sv)，SHA `4f36641d7672d98a5a7bb7daf5a215bcd57935c687566e34e66501f1fd28bf13`；[add_sources.tcl:5](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/scripts/add_sources.tcl:5)纳入它，[ariane_xilinx.sv:794](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_xilinx.sv:794)在64bit分支实例化；generic `corev_apu/bootrom/bootrom.sv`在[Makefile:784](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/Makefile:784)被FPGA过滤。

## 具体触发条件及单位

- [sd.h:9](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/bootrom/src/sd.h:9)定义CRC错误为−2；[sd.c:191](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/bootrom/src/sd.c:191)读两字节收到的CRC，[194–197](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/bootrom/src/sd.c:194)与512B数据计算结果比较，不同就取−2；STOP后返回。CMD18命令响应失败另返回−1。
- [gpt.c:94](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/bootrom/src/gpt.c:94)开始复制分区0，再打印实际 `sd_copy` res；[main.c:13](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/bootrom/src/main.c:13)只在成功时跳OpenSBI，否则[22行](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/bootrom/src/main.c:22)永久空循环。已打印终止错误的这一轮不会靠继续等待自行启动。
- 数据token等待[sd.c:173](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/bootrom/src/sd.c:173)和SPI FIFO等待[spi.c:65](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/bootrom/src/spi.c:65)、[110行](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/bootrom/src/spi.c:110)均无时间截止。100是SD dummy字节响应poll次数，10000是CMD0命令尝试次数；50/100是nop循环次数，均不是ms或秒。实际−2分支没有“超时单位”。

为排除“C文本与旧生成ROM可能不一致”的混推，已反解SV的895个64bit词，得到7160B，SHA `b596310ef371c7207caf580fc1a9741a815c75702cecbc17440dc285e4fcca3b`，与同目录生成 `.h` 逐字相同，无编译。机器码[反汇编598行](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/sd-bootrom64-decoded.disassembly.txt:598)在offset0x738比较计算CRC与收到两字节CRC，跳[637行](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/sd-bootrom64-decoded.disassembly.txt:637)的0x7ac `li s0,-2`，STOP后0x796返回a0。C→ROM完整历史重现未做，但本错误路径有生成SV二进制的独立证据。

## 当前与既有成功启动

当前[UART:52](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/live/known-bit-reload-boot-UART.raw.log:52)为10点后CRC失败，[53行](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/live/known-bit-reload-boot-UART.raw.log:53)打印−2，没有OpenSBI/Linux。LBA0x800–0x107ff为65536块/32MiB；GPT印出的字段不等于软件验证了GPT header/table CRC。不过LBA1和partition-entry读取先通过本BootROM的SD数据块CRC检查。

完整成功参考：[clean boot02原UART](/home/xvhaocheng/work/iommu-platform/out/composition-clean-20261006/mac-records/composition-clean-20261006-boot02-uart.raw.log:1)。它的前51行与当前逐行相同（仅规范CRLF），[52行](/home/xvhaocheng/work/iommu-platform/out/composition-clean-20261006/mac-records/composition-clean-20261006-boot02-uart.raw.log:52)为65点+`done!`，[54行](/home/xvhaocheng/work/iommu-platform/out/composition-clean-20261006/mac-records/composition-clean-20261006-boot02-uart.raw.log:54)进入OpenSBI，[99行](/home/xvhaocheng/work/iommu-platform/out/composition-clean-20261006/mac-records/composition-clean-20261006-boot02-uart.raw.log:99)为cpn3-clean Linux。这个参考是旧clean bit；其冻结生成ROM `.sv/.h` 和所核5C/SD.h与当前isolated冻结副本全部逐字相同，因而可用于同BootROM阶段比较，不能把它写成当前isolated bit的原UART。

同isolated artifact的旧[boot01结果](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-boot01/result.json)确实记录成功boot `d61e7517-2b15-4702-86f2-3f338ddd5345`，全过程 `02:27:25–02:35:41 UTC` 共496秒，含初始identity/sync、program与SSH轮询；每command未记录起止，不能给出精确program→SSH时长。它自己的UART只留1B句点（hex2e），不足以对比完整启动。第二轮仍在9点附近持续复制，不能据任意短等待宣告失败。

点每1000个剩余块打印一次，不是时间标记。总65536块时第一点对应第537块，第十点对应第9537块CRC已通过；若捕获完整且确有10点，则失败块只能界到序号9538–10537，不能给精确LBA/CRC。成功32MiB复制应有65点。

## 能确定与未知

可以确定：当前错误在OpenSBI/Linux之前，是多块读数据CRC不一致；相同生成BootROM曾完成整分区复制并启动Linux。不能确定：具体坏块、两CRC值、卡/线/电源状态、SPI/FIFO/AXI/DDR/CPU的哪一环，或masked readback是否有因果作用。停止并行readback后的一次同bit启动可提供有界对照；即使成功，也不能直接宣布物理根因已经查清。

[完整机器证据](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/sd-boot-failure-evidence.json)含绝对源入口、hash、ROM指令、历史耗时的范围及命令时间账本。子席零设备/网络/凭据/编译/源码或共享文档修改；此前内核重现结论不变，完成本证据后停止。

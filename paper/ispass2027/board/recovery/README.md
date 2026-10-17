# 00082与pathv3的既有SPI驱动恢复模块

00082上`/dev/mmcblk0p1`不存在时，updater在写SD之前停止。现场已确认
`MODULES=y`、`SPI_XILINX=n`、`SPI_BITBANG=n`、`OF_OVERLAY=n`，但
`20000000.spi`设备存在。使用已有上游`spi-bitbang`和`spi-xilinx`模块恢复
控制器；无需新MMIO writer、设备树overlay、新FPGA或重建整个内核。

| 配套软件 | 固定Linux源码 | 实际release | 模块本机副本 |
| --- | --- | --- | --- |
| G1 00082 | `b7979a1a06acd8a966ee33558ab98dd93f31c98d` | `6.5.0-rc2-00082-gb7979a1a06ac` | `../packages/g1-20261017/artifacts/spi-recovery-00082/` |
| 新共用pathv3 | `45bf76b15e36cf608dff95013d91f8bedb7b1b8e` | `6.5.0-rc2-ispass2027-pathv3` | `../packages/baseline100-20261017/artifacts/spi-recovery-pathv3/` |

每组只有`spi-bitbang.ko`和`spi-xilinx.ko`，加载顺序如此。
实际编译状态、命令、原始日志位置及副本路径见[g1-modules.json](g1-modules.json)
和[pathv3-modules.json](pathv3-modules.json)。两份构建回执保持原样，实际部署另记在
[deployment.json](deployment.json)。
新pathv3组只供未来软件更新时使用；**正式100次采集期间不安装这些外部模块**，
避免产生`O` taint。历史G1数据及其boot记录不改写。

本轮00082恢复及写卡已完成。模块加载后出现`mmcblk0: p1 p2`；
[write01](../runs/baseline-software-write01/command-8ee51199e7ee401f9ff03d2077bc5fc0.json)
因缺SD设备在写前停止，
[write02](../runs/baseline-software-write02/command-b7c7f8a24c2a4a179809b1f2985fb721.json)
因BusyBox tar不支持`-z`在写前停止。
[write03](../runs/baseline-software-write03/command-275284c8602d40c4a6a1712da181a211.json)
改用`gzip -dc | tar -xf -`解包，两次insmod依序成功；updater输出
`PASS: wrote 33554432 bytes`，随后sync，整条命令exit 0。

00082的外部模块`O` taint发生在历史G1九次执行之后。新pathv3已分别以
[DS50](../runs/baseline100-20261017-01/ds50-first-load/load.json)和
[DS0](../runs/baseline100-20261017-01/ds0-middle-load/load.json)实际启动，
均完成内置identity检查、taint 0，未加载pathv3恢复模块。截至本次记录，
[DS50前段25次](../runs/baseline100-20261017-01/ds50-first/manifest.json)已完成，
[DS0中段](../runs/baseline100-20261017-01/ds0-middle/manifest.json)和
[campaign](../runs/baseline100-20261017-01/campaign.json)仍在进行，不提前记为全部通过。

源码入口是[build_spi_modules.py](build_spi_modules.py)。00082的完整运行config
从已存历史payload的IKCONFIG提取；pathv3使用software01实际`.config`。
外部`obj-m`只编译固定commit中的两个原始`.c`文件，不改驱动源码或运行config。
归档不带Git元数据，make明确传入对应记录中的`KERNELRELEASE`，不是修改模块
vermagic或使用force加载。

为避免重建vmlinux，正常modpost使用software01已有的零CRC`vmlinux.symvers`
导出清单。两个固定commit之间只有`drivers/misc/idma.c`不同，该差异不改导出符号；
SPI、核心、头文件和Kbuild相同，且运行config没有启用MODVERSIONS。来源在manifest
明确记录；没有放宽modpost错误。正常编译错误会立即停止。

现成修复来源：平台提交`8639fa0fdc2904b0b55b94582484f23990fe3c42`在
`configs/linux_defconfig:146`加入`CONFIG_SPI_XILINX=y`。旧平台
`e47b48ef84594d984fab26a78073627bdcc9cfa6`的DTS在`configs/platform.dts:78`
已有Xilinx SPI控制器，在`:91`已有`mmc-spi-slot`。Linux b797源码中，
`drivers/spi/Kconfig:1090`定义tristate并选择SPI_BITBANG；
`drivers/spi/spi-xilinx.c:382`匹配该DT兼容名，`:489`启动bitbang控制器；
`drivers/spi/spi.c:3236`注册其DT子设备；`drivers/mmc/host/mmc_spi.c:1518`
匹配mmc子节点。R107同时把SPI IRQ写法从`<2 2>`整理为`<2>`；旧驱动只取
IRQ索引0，本次使用既有DT，不添加overlay。

后续更新直接使用配套版本的模块与现有updater；不追加镜像hash、SD回读、
短回归或独立重复验收。

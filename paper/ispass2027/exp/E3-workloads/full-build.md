# 完整E3 DS0：独立硬件构建入口

2026-10-17，Root已审并接受BTC3、OBS5容量批及本wrapper／固定pins，允许启动一次独立构建。
本记录冻结输入；执行状态以新out的`build-manifest.json`为准，不用E5的passed代替本次结果。
没有硬件加载、UART、JTAG、SD操作，也不修改任何已完成P0／E5 archive。

- CVA6：`1d587125eee690e623e6ffae13e70f9bbe989adc`，包含已审BTC3及OBS5／Depth64参数。
- Platform：`dfebcc0425bab25567e8514d156650a608530be1`，独立`ispass/a2-workloads`；
  parent `52c5fdd028bbf3b07deaf1d0f7e13e3a271b29d0`，仅两个gitlink及`sources.lock.json`。
- IOMMU：`85b157ebe71a623e326e51fff7b7126fe77e8eb9`不变，原16-entry配置和DS0直连。
- Linux：`da6b69fc007ad5e671dc288dd8038d02e3ebe43d`仅为hardware pins中的已审参考；
  `linux_reference_only=true`、`abi5_software_present=false`。本builder不导出或编译Linux。
  实际ABI5软件从da6另做有界driver批并审查后单独冻结。

实际[wrapper](build_full_fpga.py)、[routed verifier](verify_routed_full.tcl)、
[source receipt](build/full-e3/full-fpga-source.json)、[platform恢复身份](source/platform-full-e3-hardware.json)，
以及已verify的[增量bundle](source/platform-full-e3-hardware.bundle)与[patch](source/platform-full-e3-hardware.patch)均保存。
Platform本地组件没有可写remote；恢复包随顶层分支推送，不冒称组件远端已推送。

## 唯一构建命令

```sh
python3 paper/ispass2027/exp/E3-workloads/build_full_fpga.py \
  --out paper/ispass2027/exp/E3-workloads/artifacts/full-e3-ds0-build01
```

必须从本A2工作树运行；output经resolve后须在本E3/artifacts中且不存在。
`--check-source`已经实际通过，只验证／冻结source receipt，不导出RTL、不启动make或Vivado。
最终source receipt SHA256为`f5102f857fdbb85ee85782c73223f65d901e9db8304dbb6b8b43b8725b5c763c`。
早期check01继承baseline的E3 exclusion文字；仅修正metadata后再冻结，旧check01另存，未用于构建。

模块源码默认Depth4；只有本export的`run.tcl`在原top设置后加入`DMAPathQueueDepth=64`。
Vivado实际读取current_fileset generic，若不是64则立即报错；在synthesis之前写
`src/cva6/corev_apu/fpga/full-e3-top-generic.json`回执。
该overlay先于原`tree_hashes`执行，source receipt同时保存原／修改后Tcl SHA。
所有原clock约束、IP参数和RuntimeOptimized实现策略原样。

原通用flow的默认v3 metadata仅在本wrapper保存时改为实际BTC3／OBS5／INFO2／Depth64；
routed检查仍实际打开checkpoint核CPU20ns、无DS delay cells、无forbidden research cells。
新增版本／容量字段明确命名`source_declared_*`，只表示冻结RTL与实际top generic绑定，
不声称从checkpoint读出MMIO magic，更不冒称板卡readback。

WNS≥0、setup／hold／pulse失败端点0、routing errors0全部沿用；任何实现或时序失败就停，
不自动重试、更换策略或修改约束。实际报告／BIT／MCS本机保存，二进制不进Git。
除原placed utilization外，manifest还绑定原flow最终`Design State: Routed`的完整hierarchical utilization报告。
Root已提示E5 Slice接近资源上限；本次资源／时序尚无结果，必须等本构建实际验收。

新硬件仍不能同当前软件做ABI5正式采集。P045原4096+256与已交BIT／acece包独立保留；
full E3后续16MiB有效字节／一完整V环预热协议不跨BIT混池。

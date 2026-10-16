# 2026-10-07 替换历史隔离：当前上板入口

新BIT：`tools/composition-isolated-20261007/ariane_xilinx.bit`。最终WNS +0.022ns，WHS +0.051ns，时序门槛通过。
Linux仍从SD启动，版本`6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty`；当前boot_id `d61e7517-2b15-4702-86f2-3f338ddd5345`。本轮JTAG加载1次、SD写入0次；网口用于控制与采集，UART短读后已关闭，Linux保持运行。

启动结果：`results/composition-isolated-20261007-boot01`。实验原始结果：`results/composition-isolated-20261007-formal01`。short4+explore18全部通过，22/22、failed0；已完整回传Dell `iommu-platform/out/composition-isolated-20261007/mac-records/`。

定向入口：`tools/composition-isolated-20261007/mac_composition_history.py`，复用原runner，完整22需`--through-phase explore`。SSH helper沿用`tools/composition-clean-20261006/mac_update_reboot.py`，凭据仍由`scripts/run_research2_mac.py`内存读取。已有结果不覆盖或resume；当前不重复执行。

结果：A8/gap0旧P/PN A miss5117/6582，隔离后均2048；双方绝对耗时下降约21%，但新PN仍比P慢12.67%～12.79%。S/N跨版变化≤0.28%。比较跨BIT/boot，不能把残余差额当创新或已确定根因。

Dell报告：`/home/xvhaocheng/work/iommu-platform/docs/composition-history-isolation-20261007.md`。下一项是合法DMA/TL事件顺序与等待归因，尚未执行；不自动重跑172项或追加新机制。

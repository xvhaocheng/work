# fullstats v1 首次上板：21/21在线完成

2026-10-03，执行一次 `platform-research cost-fullstats /tmp/research-fullstats-01`。3短＋18主全部通过交付runner在线判据，cases_done=21、total=21、final_exit=0、FULLSTATS_EXIT=0、collection_failed=0。CLI实参含交付runner自动追加的 `--require-fullstats`。

Linux：6.5.0-rc2-00092-g5853e425caf8。boot_id：9a14f4f9-514e-4d18-93f2-b987b2e3bce6。使用 research-fullstats-20261002/delivery 配套新BIT及payload；JTAG命令 `openFPGALoader -b genesys2 --freq 1000000 artifacts/software/research-fullstats-20261002/delivery/ariane_xilinx.bit` 退出0，IR done1。完整SD/Linux启动、boot_id及实验UART保存在console.raw.log，加载前的零字节原样保留。版本及服务器既有哈希引用DELIVERY.json/SHA256SUMS，没有新做下载哈希或SD回读。

只运行交付固定21项，无额外试次、重跑、代码修改或编译。旧E2与旧版本结果保持原记录。这是新BIT/软件同次启动独立采集，不跨版本拼接性能结论；未做独立离线语义验收或性能分析，在线成功不表示方法M有效。

完整板端JSON、stderr、exit、dmesg、taint、argv和冻结计划在collection归档。实验结束后Linux保持运行，UART已释放。

Dell回传目标：`/home/xvhaocheng/work/iommu-platform/out/mac-research-fullstats-20261003/`，完成回执见DELL_RECEIPT.md。本轮Codex执行已交付工具，未调用DeepSeek。

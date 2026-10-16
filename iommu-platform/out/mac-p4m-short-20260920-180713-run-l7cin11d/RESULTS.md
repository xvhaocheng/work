# P4M 四组交付实验（2026-09-20）

运行用户明确指定的 `platform-p4m short /tmp/p4m-v7-01`。配套 migration-4dma-p4m BIT / Linux6.5.0-rc2-00088-g9f8da9e7a6a0，沿用本次新启动，不重新加载。

按顺序完成 Solo、A1 gap0、A8 gap0、A8 gap1000，每组64 transfers、warmup8、chunk4096。四组板端退出码均0，JSON success均true、status0，V均提交/完成64项；各活动DMA的data_errors/quarantined均0。最终taint0、final_exit0、collection_failed0、P4M_HOST_EXIT0，runner打印complete ok。没有重试或额外矩阵；已退出root并关闭串口。

records.json是完整UART中4份CLI JSON的原样结构提取，保留uint64精度；console.raw.log保留全部原始字节，包括破碎的实时printk及随后完整dmesg回显。按用户要求未新增独立主机验收脚本或跑额外回归；板端内置检查未绕过。正式gap点/重复数尚未冻结，gap1000只是工程点，不能据此宣称正式性能收益。

此前用户要求跳过下载哈希和SD写后回读，该边界保留；本次成功执行不代替时序/CDC/IO审查。DeepSeek被委派整理指标表，Codex负责实际上板命令、原始结果留存和回传。进一步指标说明如返回将作为补充报告保存，不修改原始记录。

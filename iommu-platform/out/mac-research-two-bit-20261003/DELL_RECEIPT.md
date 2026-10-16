# 原始数据回传回执（2026-10-04 JST）

状态：已回传并核对。E2 64项＋method 462项，共526项原始记录；采集进程退出0。仅记录执行与保存状态，不作结果分析或科学结论。

本地：/Users/jojo/Desktop/在mac链接pfga(codex)/results/experiments/20261003-research-two-bit-continued/

Dell：/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/

完整目录已用rsync -a回传，未使用--delete。随后rsync -acn --itemize-changes退出0且无差异输出，确认回传内容一致；原initial-transfer-failure/远端附加目录保持不动。包括完整UART、526项逐项原始记录、退出码/内核日志、计划、版本、运行来源及兼容适配器，既有runner自动文件未删除或改写。

Dell HANDOFF.md、CURRENT_STATUS.md、iommu-platform/docs/validation.md已更新并回读确认。本地MAC_FPGA_HANDOFF.md与AGENTS.md登记最新“只保留原始数据、不做结果分析”要求。E2_DELL_RECEIPT.md是早期先行回传记录，最终状态以本回执为准。

FPGA Linux保持role2/00093运行，没有结束后关机或重载；采集进程已结束，lsof未发现UART占用。首项tar-z停止只恢复采集，没有重跑。

DeepSeek曾返回登录适配建议；Codex完成主机兼容适配、实际执行和原始数据归档回传。此次未新增结果分析、统计或独立验收。

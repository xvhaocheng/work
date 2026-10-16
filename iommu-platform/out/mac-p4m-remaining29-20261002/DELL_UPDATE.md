## 2026-10-02 P4M：剩余29次补齐，原计划60份已齐（跨启动）

Mac本轮实际补跑flat32..60一次，29/29通过原runner内置检查；host_exit=0、final_exit=0、collection_failed=0。29份JSON均success=true、errno=0，双方data_errors=0，V完成4096、A完成数等于提交数，taint0。gap0/1000/2000新增10/10/9份。未追加短验收、未重跑、未扩大矩阵，未修改或重建RTL/内核；Linux保持运行，主机UART已关闭。

先前额外6份flat26..31（seq38..43）已按原runner只读判据离线复核通过，原始CLI退出码在本次输入中缺失，不能称在线通过。原25份（24在线＋1离线）＋这6份＋本轮29份，共60份（各gap20）；合计53在线＋7离线追认，不是同一连续启动60次在线通过。旧“35未运行/reported_running”为历史状态，不覆盖本条。保留所有原始记录；性能分析须保留启动分段，未宣布显著性或最优gap。

用户确认断电后，使用原P4M v7 BIT从现有SD恢复Linux00089；本轮boot_id=e6d264eb-e2a2-43e8-8136-8a7e10363e56。证据目录：/home/xvhaocheng/work/iommu-platform/out/mac-p4m-remaining29-20261002。包含29份JSON、板端全目录归档、完整UART、boot/JTAG来源、6份离线审计与旧原始归档、运行脚本和主机失败尝试记录（该尝试未发送DMA）。collection/report.json确认完整串口取回，无传输错误。原25份证据保留原out目录。

未额外运行独立验收；原runner内置数据/记录/退出状态检查保留。E2建议和其他阶段计划未在本轮执行。DeepSeek起草审计/补缺脚本，Codex审核修正并实际执行、保存和回传。


# P4M 断电恢复与补缺（2026-10-02）

用户确认曾断电，并授权先审计旧部分结果，再仅补缺失试次。

## 已完成

- 使用原 migration-4dma-p4m BIT 恢复启动，没有编译或写 SD/Flash。
- 串口确认 Linux `6.5.0-rc2-00089-gefd9461d6459`，taint=0，path_observe=Y。
- 新 boot_id：`e6d264eb-e2a2-43e8-8136-8a7e10363e56`。旧 /tmp 实验目录已不在新启动中。
- 旧 resume01-partial 六份试次（flat26..31、seq38..43）已按原 runner 的只读函数重新核对，6/6 verified_offline。见 audit/report.json。原输入未修改。
- 六份原始 CLI 退出码在本次审计输入中不可用，标 null；不改称在线通过。缺失一个错误日志的负向检查正确拒绝（exit3）。

## 补缺范围

原25份保留（24在线通过＋1离线追认）；额外6份作为离线复核通过保留。仅补flat32..60共29份：b11-g1000、b11-g0，然后b12..20按原矩阵顺序，gap0/1000/2000分别10/10/9。

新脚本使用 remaining29 模式，绑定上述新boot，要求启动记录seq基线为0、精确00089内核、taint0和path_observeY。原逐试次记录、数据校验、遇错停止判据不变，旧mode不可调用。生成脚本内 pending_audit 文字是生成时快照，六份最终审计状态以本目录 audit/report.json 为准。

这是跨启动的独立补缺分段，不是同一连续启动的60次运行；后续汇总必须保留启动分段标记，不能不加说明地合并性能结论。

29次已完成：原 runner 内置检查全部通过，host_exit=0、final_exit=0、collection_failed=0、cases_done=29。见 remaining29-connected/report.json 和完整 driver.log。未追加短验收、未重跑、未扩大矩阵；Linux保持运行。

原25份＋离线复核6份＋本次在线29份，计划60份已补齐（gap0/1000/2000各20），其中53份在线通过、7份离线追认；不得称为同次启动60次在线通过。本轮只补缺，不宣布统计显著性或最优gap。

remaining29/ 保存初次主机串口打开失败记录：未发送实验；随后 remaining29-connected/ 完成唯一一次29试次补跑。

完整板端证据已取回 collection/unpacked/：29份JSON全部success=true、errno=0，双方data_errors=0，V完成4096，A完成数等于提交数；逐试次原runner检查通过。板端压缩包619880字节，串口传输前后MD5一致；collection/report.json exit_code=0、errors=[]。这只是结果传输核对，没有追加独立验收。

Dell目标：/home/xvhaocheng/work/iommu-platform/out/mac-p4m-remaining29-20261002。上传完成状态以本地DELL_RECEIPT.md为准。

DeepSeek起草审计/补缺脚本；Codex审核修正并实际执行审计、上板补跑、取回和回传。

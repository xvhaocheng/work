# stage111 R107普通恢复采证：构建与部署前提完成，goal仍active

整体仍为阶段一立题的普通基线／全成本关。已用真实goal工具登记，无额外token预算；尚未进行R107 DMA恢复、新G0或G1。R107条件保留，等待最终新SD写入许可；本页是部署前状态，不是本轮最终裁决。

本轮具体问题：已接受的有限KV实例，能否在已核f9优化bit上，合法普通准备并完成所有恢复字节、所有权交接、层依赖及安全释放，并计入完整成本？原有限合同与数据复用stage108，JTAG/bit/S0复用stage110，不重跑旧矩阵或CPU回放。进入实际采证须新kernel真实boot对应通过；结束须同实例一次全工作采证与推进／条件保留／归档裁决，技术失败保留原错误且不得当翻译科学阴性。

- 有限工作：OPT125m完整L12/H12，两个研究shard，3唯一槽、ref2/1/1、COW0；研究代理字节，不是native GPU/trace/attention时间。此前换出与恢复各1,769,472B，换出48与恢复96合法logical runs；各432个4KiB硬件task。原source合同完整144 copies不变。
- 四逻辑池9,437,184B，普通backing16MiB；优先4×4MiB，允许8×2MiB与普通4KiB回退。记录实际PTE/leaf/IOVA/PA/table pages，不把4KiB传输分段冒称小页映射。额外CPU种子引用4,718,592B、用户输出及元数据另计，不制造隔离或容量压力。
- 一个可信owner用DMA0访问全部四池，保留premap/residency/batch/coalescing。实际冻结前端逐task WAIT，完整batch NEXT/DONE独立于每batch局部task观察ID；合法分段对应最多2个AW，避免既有W-credit边界，不合入新RTL。
- 普通DMA API handles，无新增裸IOVA alias；BIDIRECTIONAL实际RW，源只读是owner逻辑与全字节核验，不能称硬件RO。CVA6单hart FENCE.I所有权交接、准备一次checked hostS1 IOFENCE；两phase间不人工flush/remap。先全DONE/globalquiet，再真实leaf撤销→逐4KiB核空→checked fence→IOVA/backing free。prepare/final fence失败、unknown NEXT或未证排空均保留private owner直到重启，不重试释放。
- worker-layer ready在完成DMA、CPU reacquire及完整K/V检查后记录；24 worker-layer、12 joint bookkeeping，不是attention或token完成。完整padding、unused guards及额外backing slack都验证；guard覆盖与logical检查重叠，不重复当独有有效字节。

一次干净kernel/loader/OpenSBI构建59.066s，release `6.5.0-rc2-r107-ordinary-stage111`。六必要additive源改动、三配置改变；81,124源项冻结前后一致。Image 24,131,072B，SHA28354356…，notes SHA0073ec40…；IKCONFIG与final.config逐字相等。payload29,363,656B、SHAade1bd4a…，32MiB分区余4,190,776B。静态RV64 CLI SHA7fb60c67…，主机模拟失败交付通过；新校验器1正／26反离线通过，均非实板结果。

板端原32MiB首分区备份已保存Dell，SHA21ba2506…与板端一致。一次原只读dd；最初151.330s串口期限未收到结束标记，单次300.008s被动延长收到完整count/hash，但行首限定解析漏掉附在R111_HTTPD后面的结束标记，原错误及修正证据均保留。精确dd完成耗时未知。原binary POST被Content-Length约束拒绝，实际header数值未记录、原因未知；一次base64传输恢复，接收45327919 wire bytes并解码为33554432B，接收器独立Dell时间89.989s。上传串口wrapper241.382s超时没有假标PASS，完整接收/hash由独立原始数据证明。新payload下载/hash/updater真实dry-run共Dell229.864s；现有validated updater binary与板端hash相同，真实GPT/sysfs/尺寸路径预检通过，尚无新SD写入。

raw CPU/TIME、kernel nominal ns、board userspace nominal CLOCK_MONOTONIC、独立Dell monotonic分别保存；板端nominal时钟不能冒称独立Dell计时。实际分配/leaf/缓存/屏障/供给/硬件完成/全字节/释放成本，以及普通对照之外可达可避免的翻译残余，全部仍未测；不填入猜测的计算、到达或期限。

最终部署方案见[deployment-proposal](../out/r107-ordinary-pilot-20261012/deployment-proposal.json)：只更新`/dev/mmcblk0p1`，原备份可回滚；GPT/其他分区/Flash不写。新SD许可来自stage110有效next-goal-contract的最终批准步骤，旧许可只含f9易失bit及重启。批准后核padded partition SHA、新boot/config/notes/DTB，再独立校准并做同实例一次普通采证。普通控制覆盖则结束R107卡、回阶段一新显式需求；技术错误或关键事实未验则条件保留并列新事实，不扩大场景或恢复关闭候选。论文未完成。

入口：[源码主审](../out/r107-ordinary-pilot-20261012/design/root-source-review.json)、[失败所有权](../out/r107-ordinary-pilot-20261012/design/failure-ownership-review.json)、[构建与哈希](../out/r107-ordinary-pilot-20261012/build/artifact-manifest.json)、[窗口绑定校验账本](../out/r107-ordinary-pilot-20261012/validation/window-binding-ledger.json)、[原分区备份](../out/r107-ordinary-pilot-20261012/live/SD-backup-root-acceptance.json)、[真实dry-run](../out/r107-ordinary-pilot-20261012/live/payload-dryrun-identity-result.json)、[部署前稳定接受](../out/r107-ordinary-pilot-20261012/predeployment-acceptance.json)、[执行账本](../out/r107-ordinary-pilot-20261012/execution-ledger.json)。181旧主产物/内核输入未变，生产/config/cache/原历史保留；全Codex三独占Sol/ultra席，无DeepSeek/Claude。

## 后续继续：实际执行入口准备完成

newboot identity、三CAPS独立事件区间、one-RUN／超时不重试、完整8原始文件回传均已备齐。21项离线接口/拒绝检查通过，计时数学2正14反通过；这些仅验证适配，不是actual boot／FPGA恢复／timer校准。实际样本为空、实际频率界null，仍未写SD或运行DMA。平均counter rate相对独立Dell CLOCK_MONOTONIC时基；三个样本不能证明后续没有变频/停钟/drift，SI准确度未给也不默认猜ppm。

上一具体SD许可仍待回复，goal active，同一原任务未缩小。执行入口见[postapproval-execution-entry](../out/r107-ordinary-pilot-20261012/design/postapproval-execution-entry.md)，root边界见[execution-adapter-acceptance](../out/r107-ordinary-pilot-20261012/validation/root-execution-adapter-acceptance.json)。

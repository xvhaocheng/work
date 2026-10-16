# COPY warm 的合法 A 补交窗口

## Material Passport

ARS-Codex 有界源码／真实旧日志核查；请求 gpt-6.1-sol/medium，实际 model/effort unknown。共享任务 Codex-only，没有 DeepSeek。范围仅已有 p_row0、pn_row0 的来源明确 COPY 准备投影；不代表 SPDK native。各一次完整 streaming read，没有读 row1、运行旧 checker／模型／构建或修改 raw。

**P 与 PN 都有 256 次合法 A 补交决定；软件真正拿到 DONE 后，只有第一次仍有已接受、未回调／未消费的 V PTE。它在下一 A 首个物理 metadata 请求出现前已经完成。** 两例均未在所选检查点得到“下一 A 首个 payload 与当时那笔 V PTE 仍同时在途”的见证。这只是当前自然来源轨迹的机会核查，不是延后动作的反事实结果，也不能推出整个 V walk 或补交控制没有价值。

| 检查点／计数 | P | PN |
|---|---:|---:|
| A 的 first matching DONE 且后面确有下一 NEXT | 256 | 256 |
| 软件最早 return 时 V hardware busy | 256 | 256 |
| 软件最早 return 时任意 V PTE callback／actual R 未完成 | 1／1 | 1／1 |
| return→下一 A hardware issue 之前曾有 V PTE pending 的窗口 | 1 | 1 |
| 此区间新接受后继 V PTE 的窗口 | 0 | 0 |
| 下一 A 首个物理请求时任意 V PTE pending callback／R | 0／0 | 0／0 |
| 下一 A 首 metadata 时任意 V PTE pending callback／R | 0／0（132例有metadata） | 0／0（132例有metadata） |
| 下一 A 首 payload 时任意 V PTE pending callback／R | 0／0 | 0／0 |

“任意 V PTE”包含当时所有已接受的对应地址请求，而不只对照原 UID；每个检查点也单列“决策时同一批 UID 是否仍 pending”。首 metadata／payload 时 V hardware busy 均为256/256或132/132，但快照没有保留 V generation 号，**是否仍属决策时同一代 V job 为 unknown**。busy 不是整个 walk 在途的证据；检查点无 PTE 也不等于后面不会出现下一层 PTE。

软件决定必须等 `cfg_xact` 返回。私有 harness:1430–1451 在记录 cfg_read 后仍执行一次 tick 才 return，因此报告使用 `DONE capture dram_tick+1`，没有把更早硬件 DONE 当软件已知。窗口只对后面确有下一 A NEXT 的 matching DONE 计数，最后一次 A drain 剔除。下一 NEXT 的日志也是捕获值时刻，不能把它当实际首次提交：首例下一 A hardware issue 已在6157，NEXT记录为6159。源程序在软件 DONE 返回后先 program_single 四次配置再 trigger；合法控制位置位于发布下一 task 之前。

唯一在途 PTE 见证（raw 行号含CSV header，均可按 UID 回查）：

| 顺序 | p_row0 | pn_row0 |
|---|---|---|
| V PTE accepted physical AR | line5994，tick6117，parent225/child226，addr0x40400 | 同行／tick／UID |
| A cfg DONE(id2) 首次匹配 | line6003，capture6134；软件最早6135返回 | 同行／tick |
| 该 PTE callback／实际 R 完整消费 | lines6053／6054–6055，tick6195 | 同行／tick |
| 下一 A 首 metadata physical AR | line6223，tick6448，parent262，addr0x30000 | line6256，tick6448，parent295，addr0x30000 |
| 下一 A 首 payload physical AR | line6869，tick7256，parent268，addr0x90001000 | line9105，tick8353，parent367，addr0x90001000 |

所以这个旧 PTE 在软件决定时还剩60个模型tick；在下一 NEXT 日志捕获时还剩36tick；到下一 A metadata 时已经结束253tick，到首 payload 时 P／PN分别已经结束1061／2158tick。该剩余时间是离线生命周期差值，不是现成在线传感器或延迟测量接口，也未扣取信号／软件／控制成本。PN首 payload 数据以JSON真实见证为准；早期进度消息曾误沿用P的7256，已明确纠正，分析原始提取没有此错误。

守恒交叉核对：每例 cfg NEXT、硬件 issue、硬件 done、软件首次 matching DONE 均 V256/A257；复位 DONE baseline 为1。A257是反馈输出，256次补交＋1个末尾排空，不是强制构造。V PTE地址只认0x20000／0x40400／0x50000／0x50008；A PTE、DDT、payload按已给源范围分role，其他请求保留unknown。

[analysis.json](analysis.json)为两例汇总与逐窗口/见证；[p_row0-analysis.json](p_row0-analysis.json)、[pn_row0-analysis.json](pn_row0-analysis.json)保留原始提取与所有V PTE生命周期。[analyze.py](analyze.py)每次只读取指定一例CSV，[summarize.py](summarize.py)只读已提取JSON计算区间交集；[execution-receipt.json](execution-receipt.json)和[artifact-sha256.json](artifact-sha256.json)记录真实执行与绑定。

本核查未实施延迟、修改M0 gap/guard、冻结有效信号、计算信息价值或运行相同有限工作下的动作对照。不能把P的局部结果外推PN；这里PN已经单独读取，所列数字来自各自真实日志。也不能用这两条轨迹关闭D-live整个机制族。

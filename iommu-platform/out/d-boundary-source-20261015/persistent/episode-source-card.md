# PERSISTENT-SPDK-PERF：正常来源卡

## Material Passport

ARS-Codex / experiment-agent / plan；2026-10-15；Codex-only。请求 `gpt-6.1-sol/high`，实际模型/强度字段未知。来源直接核读；无运行、性能、真实状态/原生部署结论。

**卡状态：持续task/buffer来源成立；stock保pool联合quiet→resume episode不成立。** 可恢复phase只保留为新增adapter的研究投影。

| 字段 | 固定内容/来源边界 |
| --- | --- |
| 外部程序 | SPDK v24.09 perf；commit `19524ad45373e35d5fdc06f4e081444d87ceed80`，非本机实际perf运行 |
| 普通持久对象 | 每worker×namespace context 初始 QD个 `perf_task` 及其 DMA payload/iovs；正常callback复用同一对象，`perf.c:804–844,1603–1628,1566–1573` |
| 自然下一动作 | 对应最终CQ经poll进入callback，退休本次IO后立即 `submit_single_io(task)`；下一LBA按当时offset/公开规则生成；不是固定重放到达日历 |
| 命令与DMA目标 | namespace LBA与host payload buffer分别传入，`861,890–904`。NVMe read的host payload是设备写host方向，不是host→host COPY |
| 正常可持续性 | 无drain/无失败时循环复用至时间/count/exit；是供给持续性，非无穷运行或性能证明 |
| 自然phase | 可选 `-a` warmup→清stats/histogram→继续正式统计；不停止供给，不保共享quiet，`1748–1753,1811–1828` |
| 有限自然终止 | `-d`附加条件（每线程每namespace），或必设`-t`/exit；terminal drain callback逐个free，继而qpair cleanup、main等worker、退出，`1527–1529,1566–1570,1843–1871,3354–3390` |
| jointquiet失败原因 | 单callback不是全context/worker完成；warmup切面只清统计；terminal drain保留不了task对象，且后续销毁qpair。源码没有可恢复pause分支 |
| 一次准备 | env/attach/context/qpair；每slot首次calloc、DMA分配、pattern填充与iovs；正常每IO不重分配payload |
| 每IO准备 | 生成offset/LBA/方向、TSC、qpair选择/命令提交，必要metadata/DIF工作；CQ/poll/callback每笔仍需 |
| warm有/无 | `g_warmup_time_in_sec`静态0，`-a`可选；`-d`与`-a`组合被拒绝，N<QD被拒绝，`245,1889,2562–2564,2886–2893`。不改变旧M0 warm256 |
| 普通公开O_host | 配置、合法pool/mapping/layout信息、已生成命令、已提交与成功/错误历史、slot复用、在途/待补交状态及其合法时间信息；计费预测器能使用 |
| O_phys/内部状态 | 实际映射/leaf/shared资源未绑定；host史不直接含payload/PTW/DRAM实际服务序列，但若公开完整历史/布局/确定规则可重建，必须承认基线能推断；缺trace≠不可知 |
| 下一phase新增能力 | 独立admission pause；已完成slot进idle表而非free；全owner ack与sharedquiet；配置/clear/规定准备；resume旧slot新代际；最终结束才free。全部未实现 |
| 新增完整费用 | pause/resume协调、所有owner尾部、保留pool/qpairs/锁页、poll/CPU、状态/代际、硬件quiet、配置/clear/warm、观测/计算、恢复burst、最终释放；值均unknown |
| 可证伪条件 | 若只走现stock分支且要求保pool又jointquiet并继续，源码即否决；若新增adapter合法但公开历史已充分/规定准备清掉有效持续状态/收益不抵费用，则相应D增量失败 |
| 接受边界 | 不支持原生quiet/resume、真实隐态、自然反号或PN收益；不关闭D，也不要求jointtrace作为所有明示模型的前提 |

完整推理、强普通基线、潜在source relation和未解项见 [findings.md](findings.md)；原文字节和固定行号见 [source-evidence.md](source-evidence.md)。

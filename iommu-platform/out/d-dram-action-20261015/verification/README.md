# 完整准备投影的只读核账

## Material Passport

ARS-Codex 有界 raw ledger validation。请求 Sol/medium；实际 model/effort 元数据 unknown。共享研究 Codex-only，未调用 DeepSeek。独占产物只写本 verification 目录；0 构建、0 模型运行、0 raw 修改。源码、二进制及运行许可由 root 管理。

**四例账全部通过。这对 DRAM 前史在完整准备后的有效 DRAM 状态和公开时钟相同；准备过程的真实轨迹不同。不能据此声称完整 RTL/IOTLB 世界等价、O_host 信息充分或获得额外信息收益。**

| case | CSV rows | V/A warm完成 | strong DRAM tick | next refresh | B已到data-end但recovery尚未到 |
|---|---:|---|---:|---:|---:|
| p_row0 | 2140058 | 256/257 | 1100970 | 1104952 | 961 |
| p_row1 | 2140057 | 256/257 | 1100970 | 1104952 | 961 |
| pn_row0 | 2191512 | 256/257 | 1113940 | 1114316 | 955 |
| pn_row1 | 2191521 | 256/257 | 1113940 | 1114316 | 955 |

A257是反馈输出，与 cfg NEXT 次数、DONE减复位基线1、硬件issue/done及软件summary一致，checker没有写死257。所有warm完成不表示64/64 measurement任务完成：每例在t0和measurement launch之前退出。P/PN耗时为声明的1:1私有模型时钟，不是硬件测量。

[verify.py](verify.py)按行读取CSV，只保留active父子/ownership队列、有限地址索引和计数/hash；不把百万行载入内存。逐父验证64B children覆盖、无跨parent合并、W全strobe和WLAST、每8beat后才允许对应写line供给、FIFO重试/唯一接受、真实RD→20tick→callback→8B实际R、唯一真实WR→16tick数据末→B→34tick恢复→retire、原parent响应顺序。actual_R_data及W payload逐beat独立对照源码page_word模式与PTE/DDT镜像。AR/AW raw只有ID/address/LEN，**size=3/burst=1来自冻结accept()运行guard，不能表述为raw独立记录了这两个字段。**

DRAM actual issued逐条与official trace一致；维护成功enqueue/type、REFab/end、cap4 PRE及32bank row/cap独立重建，终态父子/credits/offers/live和五provider容器/future为零，无Refreshing，next_refresh严格晚于clock，写恢复全部成熟。functional mem/caw/cb/bridge_b_uids/heldR及RTL in_flight0/jobquiet3/CPUquiet均核对。峰值AR4/AW4/credits192，均未超16/16/1024。

[report.json](report.json)为完整正式账；[evidence-bindings.json](evidence-bindings.json)绑定每例raw文件hash和全部6类规范化序列首差；[history-and-source.json](history-and-source.json)补源码检查与8个phase-only核对；[service-bijections.json](service-bijections.json)把逐条trace核对后的真实RD/WR与callback/WR child/data-end/recovery总数闭合。

两mode各自两前史在pre_t0/strongdrain：row、cap、future、state、有效ready、公开clock及nextrefresh全等，均3个开bank、cap非零仅bank2=1/bank3=1/bank20=2。raw snapshot仍有9处差异：3个command_history值及6个command_ready_clock值，全部过期。除currentready成熟，还独立核所有长度大于1的历史窗口 `timestamp+nFAW26<=clock`；nFAW26取固定DDR4.cpp x8/2400表及ACT window4规则。每rank刷新接受/完成数均为floor(end/nREFI)：P117、PN118，不强求尚未来到的下一期。

post-common-reset完整规范化O_phys、DRAM命令、cfg、hardware、actualR、actualB的hash全部不同。两mode相同的最早见证：O_phys第7项（从0计）同addr327680/read，tick2200对2168；命令第13项tick2035同vec下PRE对RD；actualR第20项tick2087对2055；首B tick5208对5176；首hardware差为第三事件DONE tick6129对6097；cfg第1061项DONE值1对2。完整序列首差见bindings。不把条件O_phys比较推广为真实O_host充分性。

**负变异是5种unique类型，各在4例实际小状态上由同checker拒绝，不是20种：** B提前到WRissue、删除WR但保留B、丢W但保留B、重复callback、恢复未成熟却retire。变异仅隔离内存快照，未改raw或模型。每项拒绝诊断、见证行及tick在report。

[diagnostic-1.json](diagnostic-1.json)及[diagnostic-2.json](diagnostic-2.json)保留checker开发诊断：最初将REFab_end错算为issue+nRFC，已按固定源码改为issue+nRFC−1；第二次错把DONE含基线1当计数，改为减基线。两者属于checker错误；没有模型失败或重跑。正式四例只完整核账一次，随后仅补文件hash、早期首差见证、小phase/trace/source核对，未重放全部服务检查。

复查命令是 `python3 verify.py`（只读CSV，覆盖本目录report）；补充执行 `python3 enrich.py`、`python3 history-and-source.py`。如仅查看当前结论，直接读已冻结JSON，无需重扫。

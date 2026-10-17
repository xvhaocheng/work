# G RUN自然完成：待Root审的小批

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-18 JST
- Verification Status: PARTIAL（主机有限检查及RV64对象通过；未构建正式G软件/未板测）
- Version Label: e6-g-run-v1

Linux独立tip `69d8b9d4549c473477bcdc1df7c09ee1bb55528c`，前批prerequisite
`889f75b02f845f622ff5c45dff437aab1870d756`；只改`drivers/misc/idma.c`。
`0bfd2e9a`实现生命周期，`dc2ebdac`补实际target/control回读，`69d8b9d4`修新bank循环有符号比较告警。
完整实际diff及增量bundle见`../linux-g-run.patch/.bundle`，组件回执`../run-component.json`。
只在 `/home/xvhaocheng/work-a1-e6-linux` 开发，旧E5/A2冻结树未改，不推第三方上游。

## 实际调用链与边界

行号针对上述tip的真实`drivers/misc/idma.c`。

| 位置 | 实际行为 |
| --- | --- |
| 122 | G request/raw bank/context放heap，由RUN入口分配释放；不在原observer stack添加raw32数组 |
| 998 | 新G warmup独立V/A各256次软件COPY，gap0自然完成；每lane实际观察/排空确认完成均返回count/bytes |
| 1052、1087 | 测量先A后V；V完成即返回原软件HPM bracket，之后继续等A自己的batch ID；仅G新等待循环每未完成轮合作调度 |
| 1110 | 实际drain/collect后验证quiet、issued=done、natural control9、各4096/16MiB；不可信inventory quarantine/retain |
| 1289、1306 | 原安全e8单次nofault探测复用，持原experiment_lock探测实际cap；CAPS/RUN共用持锁helper，无嵌套锁/新seq |
| 1387 | G错误路径先显式observer abort再STOP，均不取消AXI；旧真实drain证明quiet，未知状态保留buffer |
| 1460、1474 | 旧finish新增可空G context；保留首次真实status原值，不伪造hardware error mask |
| 1498 | G raw32word单遍读到heap，whole/window/carry/ID、预算/quiet/warmup/data检查先并入旧valid，再读原PATH/hist并输出旧全局日志 |
| 1594 | 同一原software seq记录G最终valid、原status/software校验mask/hardware原mask及全部32word原值 |
| 1607、1837 | mapped core仅G分支改变；V HPM bracket后继续A，返回V软件elapsed和双方自然完成的软件elapsed |

原所有pair/P0/multi/pages/workload调用传G NULL；原默认调度策略及MMIO顺序通过完整旧回归逐操作digest核对。
G配置先各自重置/设target4096，再写V0/A所选gap，回读双方target、gap、inactive control1后ARM。
正常成功没有STOP/abort写。V poll/FROZEN保持正常，A可在V前/内/后结束。
A完整16MiB吞吐只使用raw words[13]的自身whole硬件时间，V窗口仍words[16]。
原HPM是软件V bracket，未追认为A whole硬件计时。

失败保留原errno、earlyexhaustion、data_errors、实际计数与原始status/word；
新G error或whole不完整使旧batch/PATH最终valid=0。软件数据错误不制造hardware errorbit。
全buffer源/目的字节仍经原`idma_verify`；已知quiet的错误任务也保留实际data检查，未知quiet不读/不释放。
RUN_READY现在反映RUN软件分派已接入，不表示BIT/正式软件/板测已通过。

## 有限证据

- `runs/run02/manifest.json`：**1176项G实际函数检查通过**，ASan/UBSan、Wall/Wextra/Werror。
  生产warmup/config/自然完成/abort/STOP/drain/collect/verify及完整旧observer prepare/ARM/finish/hist函数逐字抽取。
  host执行真实源/目的ring COPY及原data验证；OBS/PATH/HIST/时间为明确的合成夹具输入，不是RTL事件或论文数据。
- 正例覆盖Solo、A自然前/内/后、3个gap、strict carry/open-edge、零A窗口完成、各warmup256与测量4096、
  成功STOP/abort=0、V HPM后继续A、G32word和hist512桶每次仅一遍读取、同seq原值日志。
- 反例覆盖10个hardware errorbit、多类64bit raw损坏、缺cap/CPU fault模拟、target/gap回读错、
  earlyexhaustion、库存不明retain、zero-ID、deadline/signal、真实数据错误、原hist失败、原status高位错误、
  分配/HPM/用户copy故障。每次技术错误不自动补采。
- `../legacy-regression/run03-final/validation.json`：最终69d版本旧38pair/P0、57multi、97pages、416ABI5及6调度情境全通过；
  旧MMIO digest `9128914394721286305:498206`、nonmulti yield `3976756370770541329:357`与真实46df baseline完全相同。
  旧全部case/assertion保留，只机械增加G type/header/函数提取及旧finish NULL实参。
- `../run-object02/manifest.json`：最终RV64 `idma.o`实际交叉编译passed、0 warning；无Image/rootfs/payload。
  未改maccess对象/异常表证据复用首批原件，不重编。

`runs/run01`原编译失败保留：fixture缺U32_MAX/缩进及新生产G循环int-vs-unsigned；修正后新增run02。
`legacy-regression/run01`仅fixture nofault shim缺const cast导致candidate编译失败；gold已过，修shim后run02通过，
之后run03-final绑定最终源码。未覆盖或重写失败原件，也不把不同run的检查数相加。
本批由Codex实现、执行与检查，未调用DeepSeek；待Root审后才编正式G软件。

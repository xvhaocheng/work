# G Linux独立协议与分批调用链

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-18 JST
- Verification Status: PARTIAL（UAPI/CAPS/纯decode有限通过；RUN尚未实施）
- Version Label: e6-g-linux-design-v1

Root已锁定独立ABI6，CAPS0x11/RUN0x12，和G独立warmup合同。
首批实际实现见`../linux-validation/README.md`；本页后半的DMA调用链是下一小批设计，不是已实现功能。
基础是共同Linux `65b427946152e3aca27babab0a437821729805d4`（已含multi合作调度）；
独占worktree `/home/xvhaocheng/work-a1-e6-linux`、分支`ispass/e6-mitigation`，不改A2树。

## 已锁定输入和实际首批

V0/DID10、一页；可选A1/DID11、八页。每个测量lane固定4096个4KiB COPY，完整16MiB自然结束。
Solo A页数/count/gap均0；V gap0，A gap0/1000/2000。40点=10blocks×Solo/A8gap0/1000/2000，不能自行调参。
flags/reserved必须0，精确结构大小。原pair上限300000ms，ABI5与本轮常用参数60000ms；
G独立合同按Root审阅收敛至60000ms，CAPS/验证/后续CLI一致。

**G的warmup是每个参与lane各256次软件COPY，gap0，均自然完成并排空；Solo仅V256。**
返回各lane实际warmup descriptors/bytes并核256×4096；warmup不进测量4096预算。
这是新G独立协议：旧pair只固定V256，A为背景refill，不能声称warmup语义未变。
所有G条件使用相同warmup；以本G同BIT gap0作对照，不与P0/历史baseline混池。

首批新增独立`idma_g.h`（caps96B/request680B，输入截止byte96、raw32word起于byte232），
只读CAPS及纯input/snapshot helper。RUN ioctl号已保留，但switch尚无RUN case，`CAP_RUN_READY=0`。
旧0x06..0x10及五份旧UAPI字节保留。整个旧DMA、排空、observer finish调用链未改。

CAPS在原experiment_lock下查询实际BTC2、OBS4、两lane e8、G bank768及原HIST_INFO192；
仅写原index选择，不clear/ARM/增加software sequence/启动DMA。
G off的e8是reserved error，采用一次对齐u64 `get_kernel_nofault`读并保留RV64 readq屏障/端序；
CPU load fault转-EOPNOTSUPP。原生对象证据见`../linux-validation/native-proof.md`。
这未证明旧BIT SLVERR的完整CPU/总线行为，正式Root运行仍只在已验G BIT上。

## 实际来源边界

以下是`evidence/drivers/misc/idma.c`的65b原文行号，完整blob/hash见`evidence/index.json`。

| 调用链 | 关键原文位置与本轮意义 |
| --- | --- |
| 原锁和软件seq | lock1367→prepare1391/++seq1136→ARM1483→finish1621→unlock1646，已覆盖整个实验；不加硬件sequence/generation |
| 原warmup | submit A1456；run_window659..727只按V完成推进，其间A反复poll/refill；G需要各lane固定256的新窄helper |
| 原A预算 | 1477配置A为`IDMA_EXPERIMENT_MAX_TRANSFERS`，不是4096 |
| 原V结束后STOP A | 1512及out_drain1524..1528；G正常成功不能走这条无条件STOP收尾 |
| batch poll语义 | idma_submit596..635触发一个batch ID，poll638..657只在这个batch ID最终retire时成功；新G可沿用真实DONE读取 |
| 不明排空 | drain737..757超时quarantine/retain；cleanup300..314不释放retain的buffer，必须保留 |
| 原finish有效性 | 1231..1334包括原34word、AXI守恒、PATH16stage、512桶，G的error/whole完成也必须在旧全局valid日志前加入 |

## 下一小批RUN设计（尚未实施）

复用旧mapped core的配置、buffer/cache、HPM及验证路径，只新增可空G context；旧调用传NULL。
G context与request/raw bank放heap，避免把256B raw bank加入原finish kernel stack。
RUN严格校验输入、victim/optional A身份及精确caps，然后进入原experiment_lock。
CAPS内部的锁函数不能嵌套调用；抽出已经持锁的probe供两入口共用，保持锁顺序。

G prepare仍只调用原observation_prepare一次，分配原`++idma_observation_sequence`；
清前轮快照→实际buffer准备→cache handoff→V/A各256软件warmup且自然排空。
其后batch disable/clear重置测量ID与计数；分别target4096，V e0=0/A e0=选定gap，
回读实际target/gap/control确认均inactive，再一次ARM。不能把前端disable后的gap0误留为测量配置。

正常测量仍先触发A、再V，以V首issue到末done固定旧硬件窗口。
V poll及原FROZEN不是abort；保留V完成时的软件HPM bracket快照，随后继续等A自己的batch ID自然retire。
等待循环采用已接受multi修复的合作调度原则，保留deadline和signal检查，不关闭watchdog/taint门。
A可在V窗前/内/后完成；V软件elapsed与双方自然完成的whole软件elapsed分别返回，
A16MiB吞吐只除以raw words[13]的A自身硬件whole elapsed，不用V窗或晚到的软件poll作分母。

正常成功路径不发DMA STOP；错误/signal/deadline路径先显式observer abort（包括V已FROZEN而A仍live），
再保留现有STOP/真实drain/collect。STOP不取消在途AXI，不能清pending伪造quiet。
不明quiet沿原quarantine/retain，不释放buffer。最终只有真实排空后才CPU cache重获和源/目的数据verify。
两个measured count都必须4096、bytes16777216、自然done且未stopped、无batch错误；
两个warmup count必须256、bytes1048576；Solo所有A结果0。

在同锁、仍未disable/clear任何lane配置之前，逐字读取G bank768..799一次到heap保存，
纯decoder核cap/reserved/flags/error/target/gap/whole计数/时间戳/window守恒/carry/ID。
原OBS4/PATH/hist检查不变；在原finish输出最终valid前将G decoder及完整工作/quiet/data结果并入，
任一G错误或whole未完使旧全局batch/PATH valid=0并返回-EIO，不能只另打一条G invalid。
日志保留全部raw32word，使用同一次原software sequence，不称硬件回显seq。
最后才disable、cleanup、put和unlock。

RUN小批的主机MMIO检查需覆盖V先完成/A先完成、gap0/1000/2000、无A窗内完成、同边沿carry、
warmup独立、正常路径STOP/abort写0次、每类中途失败及unknown quiet retain、数据验证故障、
新G无效传播到原valid、旧ABI helper字节/有限回归及RISC-V目标对象。
经Root审实际diff和这些证据后再考虑共同软件构建；本页不启动RUN、Image、CLI或collector。

# G observer错误与控制合同（待Root锁定，尚未实现）

前端小批2c7c68f已被Root接受。本文件仅定义下一小批的精确行为；不修改observer/Linux，不启动BIT。
32word地址与两个cap沿用已锁布局，另需纠正769的来源事实：现OBS4 RTL没有sequence寄存器，
旧日志seq来自Linux `++idma_observation_sequence`。不能凭空锁存一个旧硬件seq。

## 769与唯一ARM事件

建议769改名`arm_generation`：64bit，reset为0，每次**实际接受**测量ARM递增1，
clear不清它，非法/繁忙ARM不递增，最大值时拒绝新ARM并置bit10，不绕回。
Linux保留原software sequence，在同一次互斥的ARM前/后读generation，验证恰增加1，
再把实际generation与原software seq一并输出；不要求两数相等，不改旧seq语义，不加软件可写新bank。
这只用于原观测回执绑定，不建立新的实验身份体系。

新增tap均来自真实硬件：frontend32bit gap、33bit target，ARM时锁存对应lane值。
batch enable/active、真实issue/done/ID与原AXI/PATH事件也都读实际信号，不抄软件期望。
target必须V4096、A4096（Solo不要求A）；V gap0，A gap32按实际锁存，采集计划再限定0/1000/2000。
配置须在两lane测量ARM前完成；warmup已自然完成/排空，不能把warmup算入固定预算。

## 771 error mask（bits63:11保留0）

| bit | 名称 | 精确触发 |
| --- | --- | --- |
| 0 | ARM_CONFIG | G ARM的batch/path模式、V0/A1选lane/has-A、真实target或V gap不符合固定合同；不部分接受ARM |
| 1 | ISSUE_ORDER | 选中lane出现ID0、首ID非1、后续ID不连续，或已有pending且本拍没有匹配done又收到新issue |
| 2 | DONE_ORDER | 无pending收到done、done ID与真实pending ID不同；无前驱pending的同拍issue+done也拒绝 |
| 3 | BUDGET | whole issued/done超ARM锁存target，whole完成后又有新测量issue/done，或batch inactive而该lane尚未完成目标且非abort |
| 4 | COUNTER_OVERFLOW | ARM起算timer、whole/window计数、gap累计的64bit饱和；不绕回 |
| 5 | WINDOW_SEQUENCE | V open不是该测量首V issue、重复open/close、close不是第4096个V done，或原窗口cycles与新窗口计数不一致 |
| 6 | WINDOW_CONSERVATION | 关窗时start+window A issues−window A dones不等于end，或start/end超一任务在途，或carry/ID边界自相矛盾 |
| 7 | ABORTED | 实际接受显式abort时G观测仍armed/whole live，冻结数据为invalid；不是DMA STOP，也不强制完成计数 |
| 8 | CONTROL_REJECTED | active/不quiet的clear/ARM、非法控制写/strobe，拒绝变更；不清计数、timer、generation或pending |
| 9 | CONFIG_CHANGED | ARM后、该lane whole完成前，其真实gap/target/read-only配置tap与ARM锁存值不符 |
| 10 | GENERATION_EXHAUSTED | generation已为2^64−1而请求ARM，拒绝ARM，不复用旧generation |

局部error各bit粘住，直到一次合法clear或下一次合法ARM；原AXI/PATH/协议sticky错误不被新clear擦除。
连续lane追踪若出现不可信的issue/done顺序，单独live故障保持至硬件reset，禁止通过clear/ARM伪造同步。
局部mask允许反映这个故障，但不能假称清过局部mask便修复了原物理状态。
原OBS4错误、hist overflow、PATH守恒仍独立强制通过；新mask=0不能替代它们。

## 时序与优先级

时钟边沿上的优先级为reset > 接受显式abort > 接受clear > 接受ARM > 正常事件计数。
同一控制端口一个写只能匹配一种动作；invalid write只有粘错，无控制副作用。
新bank只读，index选择不改变测量；768..799之外仍按原允许集合处理。

**ARM**沿用原0x60控制写、full strobe和原bit集合。G启用时再要求：
原observer可ARM、两选中batch inactive、真实job pending0、原AXI outstanding0且PATH idle，
本拍无选中issue/done、没有live追踪故障、generation未耗尽、实际配置合格。
任一失败不改变原phase、测量数据、pending或generation，置对应错误及原非法配置错误。
接受时原phase正常转ARMED，同时清本次测量数据/flags/local error、锁存真实角色/target/gap，generation加1，
测量timer从0开始；不清真实持续lane tracker，不影响DMA。

**正常事件**每lane先消耗旧pending的匹配done，再接受新issue，允许真正的同拍任务交接。
每lane独立从首issue到目标最后done记whole；时间戳以ARM边沿为0，ARM同拍事件不允许。
elapsed=last_done_timestamp−first_issue_timestamp，定义为(first issue,last done]；
另有started flag，timestamp0不能作为未启动哨兵。最后done后该lane的whole counters/elapsed/gap冻结，
live pending继续追踪，必须真实为0；错误或任何后续新任务都会撤销“已自然完成”的可用证明。
V窗与A whole状态独立，A在V窗前/内/后完成都合法。
两lane完成且原排空条件通过后，最终读值冻结；Solo A counters/flags/timestamps全0，terminal只要求V完成。

**V窗口**仍绑定原open/close事件，`(open,close]`累计，不扩大原窗口。
开窗先处理本拍A done/issue，再快照start/ID；开边沿事件不计窗口issue/done，另记open-edge issue。
strict carry为“快照仍pending且不是本拍新issue”的任务；只在其确切ID在窗内done时置carry_done。
关窗先处理本拍A事件并纳入窗口计数，再快照end/ID。
由此保持start+window issue−window done=end，open-edge issue不会混成strict carry。
V whole elapsed必须等于既有V time cycles；A whole使用自己的elapsed，不能借V窗口分母。

**V poll/正常FROZEN**不是abort：原observer正常进入BATCH_READ/FROZEN时，新A whole可以继续计时到自然完成，
V窗口与原PATH/hist保持原冻结边界。不能把现有`phase_n==FROZEN`直接接到新abort，
否则正常V poll会把仍运行的A误判为abort。

**clear**沿用0x60=4/full strobe，G启用时除原条件外还要求没有G whole live、两个batch inactive、
持续lane pending0、原AXI outstanding0/PATH idle、无同拍issue/done、无live追踪故障。
合法clear清本次数据/flags/local errors回OFF，保留generation和原持续tracker；
clear不能终止仍在V窗外工作的A，拒绝时bit8且数据/计数继续照实运行。

**abort**只取显式0x60=2/full strobe，不能从正常FROZEN推断。
若G armed/live，原观测与新测量冻结并置bit7，保留截至该边沿数据、未完成预算及pending快照，
不再把后续完成写进被abort的测量；持续live tracker和原AXI库存仍处理实际后续issue/done/回包，
使排空有真实依据，不强清pending、不STOP/取消DMA。
即使此时原V已FROZEN而A仍whole live，也必须捕获显式abort。
读已完成快照时再次abort可以无测量副作用；不能将其解释为成功完成或清错误。

## 原A-active检查的有限适配

不改变既有has-A、AXI事件或PATH队列。仅G启用且该A已证明issued=done=4096、
最后done被正确匹配、whole有效冻结、live pending0、无局部/持续追踪错误时，
允许原observer在V窗中见到A batch inactive。否则保持旧bit5错误。
G关闭时保留旧检查；G error非0时不能获得豁免。不能把A done当作对AXI/PATH守恒的免检。

本文件仍需Root锁定，尤其769从错误的“旧硬件seq”表述改为实际ARM generation。
锁定后再实现observer/wrapper有限小批，Linux与BIT不随本文件自动启动。

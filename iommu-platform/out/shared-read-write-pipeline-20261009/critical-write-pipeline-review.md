# §66 WRITE/B 并发：独立关键审核

**最新纠正：下面的初审原文保留作为审查历史。build02 的实际 `run04.log` 已否定 §7 对 `Max2 + groups==peer` 的接受，也否定首段“没有进度反例”的宽泛解读。最新有效判断见末尾「run04 反例与 group 门纠正」；修正后的 `groups<=peer` 已直接核源，仅证明 group 门自身允许落后方追平，尚未由本席执行修正后仿真。**

结论：在本轮 **mode2、每 reset 单个 immutable 2KiB normal job、每 flow 一个当前 payload child、同 parent AxID、实际 AXI AW/W/B 合法响应、成功权限及最终服务** 的合同下，未找到使原 parent B 提前完成、漏掉 child B、覆盖 held child 或把旧 parent B 计给新 parent 的并发反例。下面是基于实际源码的有限状态归纳，**不是形式工具证明或仿真验收**。允许两个 pending B 不等于允许两个当前 payload child；这个区别是本证明成立的关键。

首 parent 捕获前的游标曾有一个真实入口语义缺口：旧 `parent.addr=0` 减 `job_dst` 后取 9bit 可得到非零“raw_written”。root 已将三个端点在 `total_q==0` 时屏蔽为 0，本席随后直接读完修改后的 splitter 确认补丁符合要求（见末尾补核）。mode2 的实际 source gate 可以防止这一初始假值兑现为第二个超前 W；补丁仍正确修复了游标定义。跨 page parent B 间隙保留已消费的端点，没有用 `!parent_live_q` 清零。

这是一项普通 WRITE/B pipelining 强对照，不给它独立创新名称。接受性能比较还需要 root 实际构建并执行 full-copy、source R/W 拍数、byte guards、held VALID 检查及 Max1 回归。本席没有构建、运行、读新日志/CSV，且没有改 RTL/executable/实验记录。

## 阅读和版本边界

直接读本目录的 `codex-design-before-run.md`、`q_write_splitter.sv`、`multi_actual_dma_top.sv` 的 wiring/hold/parent/credit 边界、`multi_system_top.sv` 参数传递。经 root 授权补读本目录 `shared_pretl_read.sv` 的 coverage、certification、R 消费门，以及 vendor `axi_mux.sv:264–365`、TC `rv_iommu_trans_ctl_wrap.sv` 的 AW/W FIFO、B 返回和 normal/error 路由。旧 `../shared-read-quantum-credit-20261009/critical-future-credit-review.md` 用作已明确假设的原归纳参考；关键新增推论仍根据本轮实际源核。

除特别注明，以下短文件名的行号对应 **初次审查、游标掩码补丁前** 本目录源。其 SHA256 为：

| 文件 | SHA256 |
|---|---|
| q_write_splitter.sv | `092e44caacdd313796552bbdccf200916839a8a78edfe0f73c3a9c493ab2473f` |
| multi_actual_dma_top.sv | `9256237cbff8d21ff5be97cb7454ef3c303ac73aad9b51fbf8805be6def99d4f` |
| multi_system_top.sv | `dd6f0eb8f5829aadc7e20da454092824bb7238670e4ce62b26c1b0806dbb0b85` |
| shared_pretl_read.sv | `d10e9f7f5180ac65eb71c36b7c4ff1ccbd51541e05792efe03101c2337ee54ba` |

vendor/TC 路径分别为 `cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv`、`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`。

## 1. AW 接受、B 信用与 first VALID 是三个相关事件

令 `P=b_pending_q`，`O=offered_q`。P 是 **splitter 输出 AW 已握手的 child 数减去已消费 B 数**；它不能被表述成 physical DDR AW 数，也不一定等于已经进入 TC AW FIFO 的数。input mux 有 AW spill，接受 splitter AW 可以早于其输出 AW 真正在 TC 入口握手。O 是已有不可撤回的当前 child AW、尚未从 splitter 握手出去的本地预约。`pending_o=P+O` 是观察值（qwriter:44）。

稳定状态的不变量是 `0<=P<=M`、`P+O<=M`，且 O=1 必有 `child_live=1, aw_accepted=0`。复位全部为 0（:62–68）。形成 child 的唯一分支要求没有 live child、P<M（:88–95），因此只加 O、不会突破 M；在 O 持有期间不能形成另一 child，P 只可能被旧 B 减少。输出 AW 握手将 O 清为 0、设 aw_accepted（:101–103）并使 P 加一（:123–128），所以总预约数守恒。BREADY 仅在 P!=0 且 parent B 未挂出时为 1（:54），不可能合法下溢。

AW 与 B 同拍由一个 case 算净值，不是两个互相覆盖的非阻塞加减（:123–128）。M=1 的 counter 只有 1bit，`:129` 的溢出检查本身不能检测 wrap；安全性来自上述形成门和唯一 offered child 的归纳，不能只用这个 assertion 宣称证明。M=2 有 2bit，可以表示 0/1/2；正常归纳仍不依赖触发 3 后再 fatal。

first VALID 后 `hold_i`、quantum、peer credit 的改变都不能撤 O。child fields 只在无 live child 的形成分支写；AW 握手前 WVALID/WREADY 被 aw_accepted=0 挡住（:49–53），因此 held AW 的 child 不可能先被 WLAST 清掉。held AW 检查直接比较完整 channel（:70–76）。

## 2. 当前 W payload 与旧 B 分离，但没有两个 W child 共用寄存器

一个 child 的 W 可以开始，仅当其 AW 已在 splitter 边界真实握手，并且 live、got<count（:51–53）。原 DMA WLAST 在每个 raw W handshake 被核为仅 parent 最后一拍为 1（:104–106）；向下游 child WLAST 则为 got+1==count（:52）。

非末拍只增加 got。真 child WLAST 把 offset 加 count，清 got、live、aw_accepted（:107–113）。下一 child 的形成分支看旧 live，故不能在该末拍同一个时钟边沿覆盖 child header；最早下一拍再形成。这样所有 child 的 W payload 连续、按序、无重叠；允许下一 header 形成时旧 B 未回，仅要求 P<M。旧 B 不再需要 `child_q`，它只需要保持不变的 `parent_q.id`、P 与 error accumulator。

held W 检查比较整个 W channel（:72–76）。在 W 背压时 got/offset 不动、live 不清；自身 WLAST 计算不变，且没有 formation 分支写 child。data/strb/user 的稳定仍需 native DMA 遵守 AXI；本 splitter 没有复制 payload buffer，也没有把 hold 门加到已 offer 的 W 上。

## 3. 同拍事件不提前产生 parent B

parent B 的两个置位路径是：真实最后一个 B 握手且 **此前** offset==total、没有 live child（:115–121），或此前 offset==total、没有 live child且 P=0（:131–133）。这两个条件都蕴含已完成所有 child raw W。offset 仅在真 child WLAST 增长。

| 可达的同拍状态/事件 | 边沿后的结果 | parent B 为什么安全 |
|---|---|---|
| P=1，当前 child O=1；旧 B 与当前 AW 同拍握手 | P 保持 1，O=0，当前 child 可开始 W | 此前 live=1，`:120` 不置 parent B；新 child B 债仍计数 |
| P=2，当前 child 最后一拍 W 与旧 child B 同拍 | P=1，offset 可到 total，live=0 | 旧状态 live=1，不能置 parent B；当前 child 的 B 仍待回 |
| P=1，旧 child 已完成 W、offset<total；旧 B 退休与新 child formation 同拍 | P=0，O=1/live=1 | 形成用的是旧 P<M；此前 offset<total，两条 completion 门都关闭 |
| P=1，offset==total/live=0；最后 B 握手 | P=0，b_valid=1 | 这个 B 的 error 同拍合入，下一拍 parent B 才有效 |
| WLAST 与最后 B 同拍、即使某个 slave 非标准地过早产生 B | live/offset/P 更新后，fallback 最早再下一拍置 B | 旧 live=1 仍阻止提前置 B；这不把非标准 slave 升级为本接口合同 |

第三行对 M=2 成立；M=1 在 P=1 时 formation 不成立，必须先回 B 再形成。上述 AW/B 同拍不会“净 0 所以 parent 完成”：若 AW 真在握手，live 必为 1；最后 B completion 条件明确要求 live=0。

parent BVALID 持有时，下游 BREADY 为 0（:54），没有新 parent AWREADY（:48），没有 child formation（:88）；parent ID/error 不再改变，因此 held parent B 稳定。只有 upstream parent B 握手后才清 parent_live（:135–137）。

## 4. 原 parent 与 native page 边界不会混账

parent 仅在 AWREADY 且 native AWVALID 时捕获（:77–86）。AWREADY 需要 parent_live=0、b_valid=0（:48）。旧 parent 只有在所有 child W/B 完成、其 joined B 又被 upstream 消费后才解除 live，因此新 parent 不能和旧 child B 共存。P 虽未在 capture 分支显式清零，但上述退出不变量已经给出 P=0；把旧 parent last B 与新 parent capture 放在同拍的路径不可达，因为此前 parent_live 或 b_valid 仍阻止 AWREADY。

parent 与 child copy 保留 AxID；每个真实 B 检查 ID 等于 parent ID（:116–117），所以同 parent 的所有 child B 可以只用数量 join。两个 wrapper 的 native AxID 均为 0（top:402、461），input mux 用新增 owner bit 区分 flow（top:677、878–902）；bank ID 与 DMA owner 不是同一个意义（例如 same-bank routing top:1107–1108）。不应把“同 bank”当成“同 DMA”。

parent 接受时 normal/full 8B beat、INCR、无 lock/ATOP、job 内 2KiB、parent 不跨 4KiB 均有 fatal gate（qwriter:78–84）。9bit 总数可以表示 256；child len=count-1 的 8bit 可以表示 255（:93）。在实际 native parent 地址连续的合同下，job-base+offset+got 始终在 0..256，页尾 parent 完成端点 F 与下一个 parent 地址对应的起点 F 一致。B 回来只改计数/error、不会倒退 offset。

**入口修补义务**：`:33–37` 在 total=0 时不能把旧 parent 地址换算成消费/承诺端点。例如 A job dst 页内 offset4088，复位 parent.addr=0，低9bit的 `(0-dst)/8` 会得到 1，尽管一拍 W 都没有消费。root 已承诺让 raw_written/promised_end/next_end 在 total==0 返回 0，正常分支保留现有计算。不按 parent_live 清零，是因为 page parent B 到下一个 AW 捕获之间的真实端点仍应为 F。

为什么旧 mode2 外围结构使这个初始假值不能形成数据死锁反例：source 的 shared_beats 必须同时有两个 certified_end>returned（source:44–49），certified_end 仅在 WRITE cache 成功时推进（:174–175）。top 的 WRITE prepare parent valid 要求 parent_live 或 total!=0（top:1094–1099），因此 A 没捕获首 parent 就没有它的实际 WRITE certificate，无法开始共同 R；V 也就无法消费完第一个 <=Q child 并形成第二个 child。第一个 V child 起点为 0，本身长度<=Q，不需要假值来证明自身 envelope。这个论证只覆盖 mode2 当前入口，不是保留错误游标的理由。

## 5. Error join 不会把 ERROR 当完成捷径

每个实际 B 仍经过 P/BREADY 门、ID 检查和完整 child 数量扣减（:54、115–128）；非零 resp 只写 error_q，不跳过后续 W 或 B（:118）。最后 B 的 error_q 与 b_valid 在同一边沿更新，parent B 下一拍可见，故最后 B 的 error 不会丢失。后续 OKAY 不覆盖已有非零 error，因为只在 resp!=0 时更新。多个非零错误保留最后一个非零 resp；这保证“至少有一次错误则 parent 非 OKAY”，**没有定义 DECERR 相对 SLVERR 的严重度优先级**。

TC actual AW FIFO按顺序接收 translated request（TC:121–139）；W 从真实 TC 输入进 stream FIFO（:222–239），仅在内部已发 AW count 非零时送向 completion demux（:178–179、220、359–360）。normal 与 error 都通过实际 demux（:527–568），TC B 直接返回请求侧而非由 COMPLETE 状态伪造（:362、365–368）；error slave 是 SLVERR 的实际 AXI slave（:583–597）。COMPLETE 仅控制已译地址的 AW/AR 出口与 head pop（:716–725），不能拿它当 B 完成。

这证明 splitter 在收到合法 actual error B 时仍正确 join，**不证明 native DMA 的 fault 恢复或 retry 语义**。正常成功 fixture 没有 error 注入；root 后续正常 copy PASS 也不能伪称实测了 error join。

## 6. Max2 不破坏有序 raw W 的 prefix 兑现条件

释放 private DMA payload 的边界是 qwriter 的 W handshake，经 input mux W FIFO 头送入 TC 路径；它不是 comp/DDR W、TL 的 COMPLETE 或 B。qwriter.raw_written 在这个消费时点推进（:104–113）。TC 仍可缓存这些 W，所以 raw_written 不等于已经落到目标内存的拍数。

input mux 的全局 W-owner 预约在 first common AW offer 时创建：vendor:301–310，即使下游 AW 未 ready 也 push 并锁住 AW；owner token 存在 :329–332。W 只来自 owner FIFO头，真该边界 WLAST 才 pop（:353–365）。top:1013–1017 把真实 input push 的 owner/address/len 送给 source，source:201–203 记录 coverage，而非拿 recipe 成功长度当未来消费。

形成一个新本地 child 前，已有的 **global push** 都早于这个尚未进入仲裁的新 child；coverage 的前一拍值只可能少计、不会借用同时尚未仲裁的对方本地 offer。每 flow 只有一个未完成 raw W child，因此其最新 coverage 是一个连续无 hole 的前缀。允许 M=2 后额外保留的 B 都属于已经完成 raw W 的 child；它们的 owner token 已 pop，coverage 中相应前缀已由真实 raw_written 兑现。即使另一个较新的 child 正在 W，该 flow 的 child 寄存器仍唯一，不会凭 pending B 数增加第二个未完成 W 预约。

因此 `P_peer=max(raw_written_peer, coverage_peer/8)` 是此新 child 成为 raw W头前必已消费的较早 prefix。top:703–705 恰按 OrderedWriteCredit 选择实际消费或这个有序 prefix；policy3 的 new end 门用 64bit计算 `end<=P_peer+Q`（:745–746），不会在 9bit 的 256+Q 上溢出。

自身前缀约束仍由单 live child 保证：new start 等于自己已消费的 raw prefix，child_len<=Q。两个 flow 的共同 R 被真实 private FIFO库存挡住时，最老 W头的 end 已被所有成员的消费前缀+Q覆盖；有最终服务的正常写路径能继续消费并释放库存。这个归纳只排除 private-R 满与最老 raw-W 缺同一下一R的环，仍依赖成功 WRITE certificate、immutable 数据、common source、真实 FIFO容量>=Q 和稳定 VALID。

source 的实际成功前沿还在 :119；在 write_progress 模式消除了旧 logical-last/AW coverage 附加门（:128–132），否则独立 envelope 不能保证整个路径进展。shared skid 每份 logical R真实 handshake 后才扣 pending，全部成员交付才清 skid（:318–322），因此不能跳过尚未接收的成员来伪造 raw/r prefix。Q 不是免费新增 payload：top:1053–1054 要求 DmaBufferDepth>WriteQuantum。

## 7. 普通 policy0 的 group 数不能再按 B 退休

policy0 未 detached 时 formation 要求自己的 groups==peer_groups（qwriter:90；top detached wiring :748）。Max2 在 **child formation/本地 first VALID** 时只增一次 groups（:95–99）；到 B 不再增 groups（:119 的 Max1 条件）。若两者相等且一方先形成，其 groups领先1，该方不能再形成；另一方追平前最多差1。这个性质约束 issued children，而不是 B 延迟。

若仍按 B 计 groups，双方可以同为0，而某方的首 child W完成但 B未回；M2有空余信用时该方又形成第二个 child，于是普通同步控制被悄悄放宽。当前代码关闭了这个问题。policy0 的共同 WRITE recipe/page frontier 切分仍由 top:709–719 对齐；Max1 继续在实际 B 增 groups，B-credit满时 formation不能越过旧B，因此与旧“一个 child等B再发”具有相同正常 handoff 条件。**字节逐项/CSV相同须由 root 实测，源推导不能替代回归数据。**

policy1/2/3 通过 detached_i 的 write_progress 绕过共同 group同步，保留各自 native page grid；policy3 才使用 OrderedWriteCredit 门（top:742–748）。system top 参数默认与到 actual top 的三项传递直接一致（system:81–83、1163–1165），actual top 把 WriteOutstanding 传 MaxOutstanding（actual:739）。M2运行时排除 pause/cutover/独立 mode（:1055–1057）；qwriter只接受M1/2（:140–141）。

## 接受边界

本稿可以作为新增并发 state 的关键源审查证据。root 的最终回归应使用修补后实际构建来源。必须区分源码不变量、build、normal full-copy 实测、error 未注入、未做 synthesis 和 board。允许 pending2、out-only成功 job 并不推广为动态 invalidate/fault、多个 job epoch、持续到达公平性、跨bank重排或生产任意AXI事务的证明。

任何性能增量都先按普通 pipelining 对照评价；pending B metadata/计数在所有 Max2 policy计相同成本，本稿没有精确面积或频率数据。

## 游标修补后直接核源

本席随后直接阅读修改后 `q_write_splitter.sv:1–151`，SHA256 为 `409267ce0ef8b811a0c9d082f4f5196f9c1d364c5ba06500289c1f053387447f`。新 :37–44 将 raw_written/promised_end/next_end 的 total==0 分支置0；正常地址/offset/got计算不变，total在 parent B gap不清零，所以已完成页前缀连续性保留。新 :26、73、77、151 添加只观察 `peak_b_pending_q` 的寄存器/显示，没有用于准入或响应；它从当前 P 取最大值，不改 functional P。

本稿上面的旧 splitter 行号已用初审 SHA固定；修改后的主要对应点是 AW/W/B输出 :55–65、held检查 :78–84、parent捕获 :85–94、child形成 :96–107、真实AW/W退休 :109–120、B/error join :123–129、AW/B净计数 :131–136、parentB门/退休 :139–145。这些功能分支与初审内容一致，故上述并发归纳继续适用。本席没有执行该源的 build 或测试，也没有将旧构建当成此补丁的运行验证。

## run04 反例与 group 门纠正（撤回 §7 的 equality 接受）

**本席此前 §7 的接受有误。** 本席证明了形成 child 时可能产生 `|gV-gA|<=1`，却误称落后方可以在 `groups==peer` 门下追平。实际一方领先1后，落后方同样不满足相等；在 Max2 中 groups又不随 B退休增长，两方会一起失去形成资格。差界是安全性质，不能替代追平的进度条件。撤回此前 §7 关于 equality同步可继续推进的断言，以及基于该断言给整个 policy0 的无阻断接受；不擦除前文，也不撤回独立的 AW/B净计数、原 parent B join、唯一 W payload child 不变量。

### 原始反例已经直接读取

直接读本目录 `run04.log`，SHA256为 `0baac9da21bb98e3caea6b0c2d48a4d38f095c20988ba030cd6403542243e903`：

- :5、20 明确报告持续无握手进展及 watchdog失败；:28–30 是 Q对应运行的 2KiB任务、A目的偏移4088、第一页只1beat。
- :51 显示 source逻辑返回 V/A各17、没有source skid、没有 detach，READ未完成。
- :54–57 显示 V `groups=2 offers=2 offset=9 total=256 live=0 b_valid=0`，A `groups=3 offers=3 offset=16 total=255 live=0 b_valid=0`；Q=8、MaxOutstanding=2，两者 peak pending B分别1/2。
- :58 明确 `completed=false, verified=false`，不能归为 copy PASS。

日志不直接打印终态 `b_pending_q`，因此本稿不把 peak 或 `b_valid=0` 伪称为当前 P=0 的直接日志证明；root另外报告终态两者P=0。反例的 group门诊断不依赖这一额外数值：即便旧 B仍待回，Max2的B退休也不增groups，两边2/3仍不能恢复相等。没有live child，且新formation要求相等，故没有内部事件能改变这两个groups。终态空TC AW队列、无held child也排除了“还在等某个已经形成的 child改变groups”的解释（:3、54–57）。

job相对游标需加parent地址起点：A已经捕获第二native parent，它的起点为1beat，所以 A offset16意味着真实job端点17；V还是起点0的原parent，端点9。policy0的共同子片序列在 F=1/Q8 时是 `[0,1), [1,9), [9,17), ...`。A已形成/完成第三片，V只完成第二片。旧 equality门阻止V形成已具有全部logical R payload的第三片，这是具体、可达的普通同步停顿，不是parent B提前完成或pending计数损坏。

### 实际修正源码与有限归纳

root修改后，本席直接读 `q_write_splitter.sv:94–110`；修正源SHA256为 `d1079adf9a710ba7871749711d8ed1aa82aad24dd8922f40c72e4ec7e60d948e`。:98–99 是：

```systemverilog
(detached_i || (MaxOutstanding > 1 ?
 groups_o <= peer_groups_i : groups_o == peer_groups_i))
```

Max2仍在formation时仅增一次groups（:109），其他 parent/live、B信用、非零quantum、hold 门保持 :96–97；Max1 保留旧相等比较，并继续在实际B退休时增groups。修正没有撤回held child、清pending B或改parent完成门。

只考虑 detached=false 的 Max2两flow同步，令 `d=gV-gA`。从reset的d=0出发，每个边沿只可能各flow formation一次：

| 此前 d | 可满足新的 group形成门 | 形成后的 d |
|---|---|---|
| 0 | 两方都可；其他门可以只放行一方 | 两方同拍形成仍0，只有V为1，只有A为-1 |
| 1 | V领先，V不能再形成；A落后，A可以追平 | A形成后0；A暂未形成则保持1 |
| -1 | A领先，A不能再形成；V落后，V可以追平 | V形成后0；V暂未形成则保持-1 |

所以 `|d|<=1` 仍是归纳不变量，并且如今落后方真的满足门。若两方仍有未发payload、所有其他formation门都允许，group条件不会同时挡住两边：相等时双方都允许，差1时落后方允许。这是**group同步本身的有限进度性质**，不是对 arbitrary downstream服务、recipe失败或shared R全部门的完成证明。

具体run04终态变成 V `2<=3` 可形成 `[9,17)`，A `3<=2` 不可再先发第四片。V形成后groups成为3/3；若真实剩余各门允许，V第三片的8拍payload已由returned17覆盖，故这一步无需再借一个新R来追平。它直接解除反例中的同步条件，不能在尚未跑新仿真时宣称完整job已完成。

### 不同 native parent 分段与 pipeline 的同步含义

group数衡量 **共同 child网格的已形成数**，不是native parent数、B数或字节数。policy0当前mode2/prefetch路径的 top:709–719 将两flow child共同截在A真实第一页末端F，之后从同一job端点F继续按Q分片；未来WRITE recipe提前成功不会抹掉该截点。新A parent捕获重置parent内部offset，但不重置groups（qwriter:93–94）；V仍保留自己的原parent，因此共同child序号跨native parent边界延续。

在此前提下，`gV=gA=n` 表示两边已经形成相同第n片，而它们的rawW完成和B返回可以不同时。允许一方在rawWLAST后、P<M时形成第n+1片，另一方可因旧B或新native parent捕获迟到而暂时停在n；领先方不能再形成第n+2片，落后方随后可以通过 `<=` 形成第n+1片。这正是最多一个共同child的issued领先，不是每拍相同raw进度。P+O<=M、一个当前payload child及parent所有B汇合不变量不被这个group门放宽破坏。

若某个场景取消common page/certificate切分，两flow实际child边界或总child数不同，单纯比较groups不再具有上述共同片序号含义：可能一方完成全部child，而另一方还需更多native短片。这时不能把本证明套为同步完成保证，须改按byte prefix或重新规定网格。当前policy1/2/3通过write_progress设置detached_i绕过该group门（top:748），本修正不把它们的独立native网格变成group同步；它们继续依赖原本各自的payload/credit准入条件。

验收状态：本席直接核一份失败原始log、实际 `<=`补丁和必要top共同网格wiring，已明确撤回原错误断言。root报告build03 w2-c0成功；本席未执行或独立读取该build记录，未读/执行修正后的copy结果。新的full-copy回归由root完成后再接受。此前“模型/审查同意”没有提供验证，run04真实反例具有否决力。

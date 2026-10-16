# AR 因果核查与一个冻结普通对照

结论：payload 有两条源码支持的 AR 延迟边：共享 mux 的可选仲裁，以及已承诺 AR／真实 DDR 容量造成的背压。前一条可由一个当前 VALID 的普通 DS 优先选择器覆盖；后一条不能被该选择器撤销。两条边是否在本轮自然两安全头前缀产生不同的相对 PTE 等待，仍需真实事件见证。本报告不宣称收益、容量饱和或 FPGA 时序。

本席只核 AR。PTE 前驱消费到下一 PTE 出生的 PTW 状态推导、两安全 fresh 可达性和完整 R 阶段由 root／另席负责。本轮采用 root 已确认的 CPUoff、CpuWrites0、DdrAtomics0、PolicyEnable0、SharedPoolMode0、SameBank／ReverseRead 等禁用、read_admit_mode0／owner_only0，保留 NativeN、两个真实 DMA、各一个 DEFAULT 对齐 4KiB、私有 buffer512、DS 名义 50/50。未运行旧实验、模型、测试、构建、validator、仿真、综合或板测，未修改生产源。

## 1. 必须使用实际编译副本

`tests/test_nonblock_actual_dma.py:409–415` 先收 sibling SOURCES，再收 iDMA sources、生产 FIFO 与 SYSTEM extra。`tests/test_nonblock_banked.py:21–45` 明确选择 IOMMU vendor 的 axi_pkg、rr_arb_tree、stream_arbiter、spill_register、counter、axi_mux、axi_demux、axi_demux_simple。SYSTEM extra 在 `tests/test_nonblock_actual_dma.py:173–192` 选择 CVA6 vendor 的 stream_delay、axi_delayer、axi_cut、axi_multicut、axi_xbar_unmuxed、axi_xbar，再加入真实 rv_iommu_ds_if。这是一条混合但各模块唯一的源码链；不能用另一个副本的 mux／demux 推断其实际额度。

`critical-excerpts.txt` 保存所有决定性范围的原文与行号；`source-sha256.json` 保存完整相关文件 SHA256、读取范围和生成时提交。没有编译执行。这里“实际编译副本”指当前 runner 明确列入 SOURCES 的模块副本，不表示本轮重新编译过。

## 2. 原生通路与每个握手的含义

| 边界 | 源码与实际动作 | payload 能直接影响什么 |
|---|---|---|
| PTE 出生 `u_ptw_ar_valid` | system_top:412–422 将 PTW AR 原样打包；出生前推导不属本席 | 此处没有 payload 仲裁；出生是否因前驱 R 延迟推后由 root 判 |
| DS 内部 ARaccept | ds_if:81–108，7 输入 RR/LockIn 后接一个不 bypass 的 spill；PTW 握手为 system_top:1375 | comp 不进入该 arbiter；已有 DS 队列满时，下游 payload 背压可以传回 |
| DS spill 出口 → delayer | system_top:457–480，名义 FixedDelayInput50、Output50；AR 输入延迟独立于 AW/W | 准备期间不以共享 AR READY 决定计数；Ready 后背压能阻止其完成 |
| xbar slave3 ARaccept | `dbg_ds_ar_*` 在 system_top:1411–1412 观察的其实是 delayer 后的 xbar 输入，不是 DS 内部握手 | slave3 前端两项 spill 若满，可反压 delayer；不是与 comp 在此争同一个 spill |
| slave3 demux → DDR 路由 cut | xbar_unmuxed:181–209 的各 source 独立 demux；231–250 每条连接一级 multicut | comp 不直接使用 slave3 per-ID counter；共享下游阻塞可让 DS route cut 满 |
| DDR 共享 mux 选择 | xbar:131–162 将四 source 送一个 mux；实际 mux:409–439 RR/LockIn 后接两项 AR spill | comp2 与 DS3 当前 VALID 竞争；首次输出 VALID 后选择锁住；已接入 spill 的条目不能撤销 |
| DDR ARaccept | DdrAtomics0 使用 system_top:1037–1039 直连；C++:668 READY 当且仅当 `mem_q.size()<16`；1291–1305 接收后入队 | 真正共享 DDR 16 项额度；已接受 payload 只有其 RLAST 握手后退出，不能靠 AR 优先回收 |

source0 CPU 已关闭，source1 固定 0；source2 是 comp，source3 是 DS（system_top:479–480、522、527、870）。DDR 地址路由为 mst0（875–883）；四 source 的 4 位本地 ID 经 mux/axi_id_prepend 添加 2 位 source 前缀，DDR 使用完整 6 位 ID（top:303–306；mux:211–245；axi_id_prepend:90、99–110）。因此 comp／DS 不会仅因本地 ID 数字相同而同属一个 fullID。

## 3. 真实容量与信用：不能把参数名当总读额度

1. DS_if AR spill 两项：spill_register_flushable:38–96 有 A、B 两份数据；READY=`!a_full || !b_full`，输出优先 B。因此内部 PTW ARaccept 是接进这个 spill，不是已到 DDR。背压未传满之前，内部握手可先发生。
2. xbar slave3 的 AR channel spill 两项，selection spill 同步保存目的地；两者表示同两笔事务，不能相加为四笔（demux:132–160）。每条 DDR route 的一级 multicut 再有两项 AR spill（xbar_unmuxed:235–250；multicut:47–74；cut:86–98）。共享 mux 出口又有两项（mux:428–439）。这些都是已有 pipeline 容量，不能算新增控制的收益来源。
3. `MaxMstTrans=8` 在 xbar_unmuxed:192 传给每 source 的 demux。当前实际 `axi_demux_simple:69` 用 `idx_width(8)=3` 位；cf_math_pkg:57–58 给出该定义。4 位 AxiLookBits 形成 16 个 ID counter。每个 counter 的正常 full 条件为数值 7，`full_o=OR(cnt_full)`，另外还含 overflow（demux_simple:537–557、599–618）。这不是“全 source 合计最多 8 读”，也不是“各 ID 独立不影响同 source 的其他 ID”：任一 ID full 会挡住该 source 新的未锁 AR。该 source 的 RLAST 握手减其 counter（355–372）。不把潜在 ATOP 注入当本轮流量。
4. `MaxSlvTrans=8` 到 xbar mux 成为 `MaxWTrans`（xbar:148），实际 mux AR 选择和 spill:409–439 不使用这个参数。它不提供“共享 AR 最多 8”的证据。
5. DDR C++ 实际门槛单独为 16 队列项，每项一笔已接收 AR，不是一个 R beat；LAST 握手删除整个条目（C++:668、1291–1305、1371–1399）。无同拍 pop 的前瞻 READY：READY 是驱动时的队列大小；满队列在本拍 RLAST 后也只能下一拍重新开放。

本轮两份 sole4KiB／buffer512 不能仅凭字节数、缓冲大小或参数 8 推导“已占满 mem_q16”。本席没有 actual owned 16 项见证，也没有对两安全头额外扩大交通。任何 DDR credit 延迟、满 spill 残留和 DS per-ID full 都保持带状态条件的结论。

现有 C++ 的 `ds_priority` 在 full_batch 下仍只到 V 完成数达到 v_transfers 为止（689–698），不是两份工作全部退休到 quiet 的完整窗口。本报告把完整 DS R 优先作为 root 必须明确实施／核验的强对照条件，没有宣称现有开关已满足。每次 drive 才寻找最老 DS、未 held 时 rotate；默认仍是只有 head 扣 latency（699–713），不会因后项已入队就并行准备。新因果探针不能切换 read_latency_overlap 来制造收益。

## 4. 分阶段紧条件界与时间标签

所有周期数是 RTL 偏序推导，不是测量。定义周期 n 为边沿 n 前的稳定握手样本；同周期 VALID&&READY 在边沿 n 接受。改变采样标签可平移记号，不能把下列推导冒充实测。

### 出生 → DS 内部 ARaccept

若 DS 内部 arbiter 未锁、无其他 DS 当前获选、DS AR spill 有空位，出生当拍即可接收，等待严格为 0。comp 不参与这级选择。若已有 DDTW／其他 DS lock 或 spill 满，等待来自那些真实已持有义务以及 spill 的下游 drain；仅知道 onePTW／两 safe payload 不足以给无条件有限周期上界。把内部仲裁等待和后继 DDR 等待分别记账，不能直接把 `dbg_ptw_ar_fire` 当 DDR 接受。

### 内部 ARaccept → delayer 首次输出 VALID

空 DS spill 的一拍 cut 后，stream_delay(50) 的 Idle 在看到 VALID 时加载 50；Valid 逐拍减计数，见到 0 后下一拍才进入 Ready（stream_delay:55–93、109–131；实际 counter／delta_counter 的 load 优先于 en）。所以空闲 delayer 的 `first inputVALID→first outputVALID=52` 拍；名义参数“50”并不是该标签下的 50 拍。内部接受到首次 delayed VALID 为 `1+52=53` 拍。

有更早 DS spill 项或 delayer 已在处理更早请求时，需加其残余服务与背压；该模块直接传 payload、不额外保存请求，也不会让后继的 50 倒数与前项重叠。Ready 状态才把下游 READY 返回上游；payload 在这种背压中必须保持已承诺。

### delayed VALID → DDR accept 的空路

DS demux ARspill 一拍、DDR route multicut 一拍、共享 mux ARspill 一拍：无已有队列、DS demux credit 合法且可用、DS 获选且 DDR READY 持续 1 时，严格为 3 拍。因此同样空路下：

`DS_internal_AR_accept → DDR_AR_accept = 1 + 52 + 1 + 1 + 1 = 56`。

这不是针对当前自然状态的固定预测。前端 spill 空间、目的地／ID 许可、共享选择、输出 spill 和 DDR 容量任一有旧义务时，要另加实际等待。

### 共享 mux 中 RR 的紧选择界（仅 comp2／DS3）

DS 当前持续 VALID，两个入口 head 都稳定，且本次首次选择之前没有锁：实际 RR 状态 0 可先选 comp2；next_idx 按旧 rr_q 的活动集合更新为 2；下一次又选 comp2，再更新为 3，之后选 DS。这由 rr_arb_tree:190–219 与 251–290 的树选择共同推出。但 **CPUoff 原生前史只要曾接受过一个共享 mux AR，RR 状态就恒属于 {2,3}**：每次更新的活动／冻结集合只含端口2、3，next_idx 只能取2或3。这是必要的更紧域边界，不能把 reset 状态0当本轮任意初始化正例。因此有上述前史、未锁且 DS 持续可见时，最多 **1 次 comp 接受机会** 在 DS 前；2 次仅是明确 reset0 的局部条件界。共享 RR 没有“需要等 payload 整个 256 拍 R 完”的规则；AR grant 是事务头握手。

紧周期条件：mux 出口 spill 空、DDR READY 每拍 1、两个候选每拍持续 VALID，无旧 lock。有原生接受前史且 rr_q=2 时，DS 在第2个输入接受机会获选，再过 ARspill 一拍 DDR 接受；`mux-visible→DDRaccept=2` 拍，DS-first 为1拍，纯可选仲裁差严格为1拍。若 rr_q=3，已有 RR 当拍选DS，没有这项差。reset0 条件才是3拍对1拍；本轮不得为造收益置 reset0。这里入口 comp 代表其已有有序 head 队列；不宣称当前两个 safe 选择必能形成具体队列状态。

若已锁 comp，锁本身必须服务；之后仅两输入状态下最多再让一笔 comp 获选。故当 DS 持续可见时，原 RR 总计最多 2 个新的 comp 选择在它前面（含已有锁）；另有共享输出 spill 至多 2 笔旧 AR。总计至多 **4 笔旧／新 comp AR 在 DS DDRaccept 前**，这是事务位置条件界，不是四拍上界，也不是本轮自然可达声明。已有 per-source／route 队列不可和这个 mux 局部界混为一体。

如果从所选前驱开始，实际 DDR AR 服务间隔有已验证有限上界 G，且 DS 本身的 credit／route 可持续进入、没有其他等待，位置界才能换算成时间界；READY 可无限停顿时不存在无条件有限周期界。没有测得 G，不代入任何数值。没有 full/mem_q16 见证，也不以“16”制造一条等待。

## 5. O-DS-AR：一个完整、廉价、提前冻结的普通控制

完整机器合同见 `ordinary-ar-contract.json`。只改 DDR shared mux 的 AR chooser，保留所有既有 spill、ID、额度、delayer、返回和 NativeN。候选只来自 per-route cut 已经输出的当前 VALID head。DS3 当前 VALID 时，首次可选选择 DS；否则当前 VALID head 按固定循环 RR。例外只有一个预先固定的公平逃逸：非 DS 连续等待时，最多先接受 4 次 DS，再服务一笔非 DS。阈值固定 4，不能在结果后调参；本轮 CPUoff 时非 DS 只有 comp2，公平界是接受机会而非时钟。

一旦输出 chooser VALID，选择和整个 AR payload 保持到入既有 output spill 的握手；已有 lock／spill 项全继承。不能通过撤 VALID、改 ID、越过 same fullID head 或偷回 DDR 队列项表达优先。每条 input 只有一份 head；控制不检查 PTE 值、未来 PA、未来 RREADY、隐藏 DDR 时序、两个未来 rollout 或全候选反事实。

覆盖：所有“DS 已经在共享 mux 当前 VALID，选择未锁且未公平逃逸，却让 comp 先被新选中”的额外仲裁。普通控制在这个因果边具有完整可实现动作，不需要 F1 的双 rollout。

不能覆盖：DS 出生之前／delayer 未输出之前的 payload 合法先选；已有首次 VALID lock；已有 output spill 中的 AR；DS 自身上游 DS 队列／delayer 固定准备；DS per-ID／route 真实背压；已接受 DDR mem_q 项；DDR 返回已 held R 与其先后服务；PTE 前驱消费与下一出生；NormalTL safe 头产生的工作量或完整 W/R/B／fault 所有权变化。不能据这份普通控制否定上述边上尚未证明的机制，也不能把它们拼成一份 posthoc 普通程序。

成本：复用既有 AR 数据 mux 和 2 项 output spill，无额外 payload storage 或新额度。一个独立完整选择器可用 RR pointer2位、held index2位、held valid1位、饱和 DS streak3位，共8控制状态位；这是明确设计的状态清单，不是综合面积增量。原 rr_tree 已有其 pointer／lock／冻结 req 状态，不把全部8位都宣称新增，也不宣称精确减省。增加当前 DS-valid 优先选择与公平门的组合延迟；合同请求不新增时序周期，但可达 Fmax／面积／选择周期尚未实现或验证。若实现需要注册观察／选择，必须把全部新增周期和面积计入完整完成与同预算对照。

对照只消费当前端口 VALID、AR内容、已有 lock／握手和完成历史。测试观测可有更多只读 taps，但同一组选／等待事件应让机制与普通控制等价可见，并逐项登记实现所需的生产路由、观察延迟与选择延迟。不能将软件 scoreboard 零成本读层级路径当可实现观察器。

## 6. 当前可见信息与最小新探针接口

现有可见：内部 PTW／DDTW AR fire，delayer 后 DS ARVALID／READY，comp proxy 上下游完整4位 ID／地址／长度，DDR 完整6位 ID／地址／长度／VALID／READY，PTW consume fire。C++ full trace 记录 DDRAR、DDRR 握手和 PTW AR／R fire（992–1047）；旧 `ddr_r_first` 只在首拍 **握手** 时写入（1168–1170），不能当 firstRVALID。已有 `dbg_ds_ar_*` 也不能当 PTE firstVALID。

要对 AR 阶段做可归因的新有界因果探针，最小测试 taps 是：

1. PTW 原始 ARVALID、READY、完整本地 AR头，匹配 walk／request owner 和 PTE ordinal；首次出现事件与已有 `dbg_ptw_ar_fire` 分开。前驱 consume 标识与 root 的 R 阶段接口对齐。
2. DS_if ARspill 出口 VALID／READY／AR头，以及 delayer state／count／首次输出 VALID。只读，允许区分准备52拍和 blocked Ready；已有 delayed VALID／READY 可复用但需携带 ID／地址。
3. slave3 demux 的 ar_id_cnt_full、lookup占用／目的地许可、其 ARspill valid/ready；本轮实际所需 DS ID 的 in_flight，以及 DS route cut 的两项占用／头。没有这些，不能把 upstream stall 归给真实 credits。
4. 共享 mux 的 comp2／DS3当前头 VALID、完整 AR，selected index、held index／lock、RR state、output spill A/B占用及 AR头、input接收 fire。需要记录首次选择与 outputspill 内部 accept，不能只看 DDR fire。
5. DDR mem_q真实项数与每项 fullID／剩余beats／accepted-cycle、head、held R、DDRAR 与首次RVALID和握手。它们是模型审计，不授权生产控制读取未来值；同拍 enqueue/pop 顺序要保持现有语义。

本轮没有添加上述端口或运行新探针。仅因“需要更多探针”不能作为性能或创新证据。只有 native fresh 可达、两个安全头真实行动、R完整优先合同和一个完整普通动作／成本边界都由 root 接受后，才可接入一轮新有界因果验证；任何只平移整个 walk 起点／普通 overlap、或由 O-DS-AR 完整覆盖的收益，按执行合同停止本分支。

# 新被动前缀的 PTE／AR 独立核验

结论：这一次真实两 DMA 原生前缀没有两个 fresh 头，也没有共享 AR 上 comp／DS 同时 VALID。12 笔 PTE 的所有阶段都能从连续 CSV 配对，未发现可归因于 payload 的 AR 等待。因此本前缀不能进入两安全选择的因果 ABBA；不能据此判其他合法前缀不可达或相对阶段全域不变。

本席只执行一次离线原始 CSV 解析，未运行 binary、build、旧 validator、模拟或额外工作流。root 的新原件保留三次失败构建（退出1、1、2）、一次成功构建和一次成功被动 run。解析输出见 `raw-phase-verification.json`，完整解析代码仅用于该新 CSV，见 `verify-new-prefix.py`。

## 接口与实际编译来源

当前 top 相比原 system_top 只有模块改名、新 output 和只读 assign。`safe_pte_stage_top.sv:1499` 的 `pte_ready=ptw_resp.ar_ready` 是 PTW 经 DS 内部 arbiter 进入 DS ARspill；`1505` 的 `ds_ready=ds_resp.ar_ready` 是该 spill 输出到 delayer，不能互换。`1509` 是 delayer 后 xbar slave3 输入接受；`1514` 是 shared mux chooser 进入既有 output ARspill。CPP:906 在低电平 eval 后、rising edge 前输出 CSV，与其后 f_ddr_* 样本一致。

实际 manifest 与成功 build log 选择 IOMMU vendor 的 mux／demux／simple、CVA6 vendor 的 xbar／unmuxed／delayer／cut，与本席前一阶段来源一致。新top及八个决定性模块当前 SHA 与 manifest 匹配；binary SHA 与真实 run-result 匹配。本轮 run 使用 CPP:3236–3241 的 CPUoff、每 DMA 一个4KiB、DS完整优先、head-only准备、ptw_latency31、无tail pause／admit改变；CPP:689 将 DS优先窗口延伸至本次到quiet，未变 R held guard／rotate／准备策略。

## 逐笔 PTE 绑定

CSV 从绝对周期62到4212逐拍连续，共4151行，无缺周期。PTW 固定本地ID0来自实际编译 `rv_iommu_ptw.sv:276`，shared fullID48由 source3前缀形成；DDTW 固定本地ID1，fullID49。每笔按完整fullID FIFO、地址、bank／DID／当前slow owner IOVA／type、`ptw_stage_q`和`main_lvl_q`配对；整个出生到消费区间owner、stage、level均稳定。下表stage／level／type保留原始数值，不把main_lvl索引换成未经核实的层级名称。

| PTE序号 | bank／DID | owner IOVA | type | stage／level | PTE地址 | 出生／内部接受 | DDR接受 | DDR首次RVALID＝握手 | PTW消费 |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0／10 | 0x10001000 | 3 | 0／0 | 0x20000 | 592 | 648 | 680 | 736 |
| 2 | 0／10 | 0x10001000 | 3 | 0／1 | 0x40400 | 737 | 793 | 825 | 881 |
| 3 | 0／10 | 0x10001000 | 3 | 0／2 | 0x50008 | 882 | 938 | 970 | 1026 |
| 4 | 1／11 | 0x20000000 | 2 | 0／0 | 0x30000 | 1551 | 1607 | 1639 | 1695 |
| 5 | 1／11 | 0x20000000 | 2 | 0／1 | 0x60800 | 1696 | 1752 | 1784 | 1840 |
| 6 | 1／11 | 0x20000000 | 2 | 0／2 | 0x70000 | 1841 | 1897 | 1929 | 1985 |
| 7 | 0／10 | 0x10000000 | 2 | 0／0 | 0x20000 | 1992 | 2048 | 2080 | 2136 |
| 8 | 0／10 | 0x10000000 | 2 | 0／1 | 0x40400 | 2137 | 2193 | 2225 | 2281 |
| 9 | 0／10 | 0x10000000 | 2 | 0／2 | 0x50000 | 2282 | 2338 | 2370 | 2426 |
| 10 | 1／11 | 0x20001000 | 3 | 0／0 | 0x30000 | 3248 | 3304 | 3336 | 3392 |
| 11 | 1／11 | 0x20001000 | 3 | 0／1 | 0x60800 | 3393 | 3449 | 3481 | 3537 |
| 12 | 1／11 | 0x20001000 | 3 | 0／2 | 0x70008 | 3538 | 3594 | 3626 | 3682 |

12笔共同相对值：出生到内部接受0拍；到DS spill首次VALID1拍；该VALID到delayer输出52拍；到delayer输出53拍；到mux首次VALID55拍；到DDR接受56拍；DDR接受到首次RVALID32拍，首次RVALID当拍握手；DDR接受到PTW消费88拍，出生到消费144拍。各组三层同owner的前驱消费到下层出生均1拍；跨owner的525／7／822拍不冒充同一walk的层间等待。

12笔 DDRAR都len0、每笔恰好一拍R，fullID／len／LAST守恒；PTW消费的PTE data逐笔与CPP setup_memory:550–564的准确tableword匹配，包括重复root／中间地址也按新事务次序配对。PTE8／9出生时还有两笔payload ID36的DDR义务，剩余175+256、62+256拍，其PTE阶段仍为同一组数值。这只是本trace中的有payload见证，不是改变safe顺序的对照结论。

## 旧 DS、held R、额度与普通 AR 控制

重建全部18笔DDRAR，而非只筛12笔PTE：另有2笔DDTW fullID49（地址0x10280／0x102c0，len7，各8拍R）和4笔payload AR。两笔DDTW分别在129／1088接受、161／1120首次RVALID、272／1231最后R握手，各104拍held，共208；12个PTE出生到消费区间均无任何DDR held R，也无新DDTW AR。它们没有被错误计入PTE等待。

原始事件：两bank同VALID=0、any fresh=0、safe fresh=0；共享mux comp／DS co-VALID=0、mux held=0、PTE内部ARstall=0、delayer后DS ARstall=0、DDR ARstall=0。mem_q峰值3，逐拍 `ARREADY==(mem_q<16)`；同fullID FIFO每笔R准确归属旧AR，未见同ID次序例外。DS spill输出的728拍VALID而READY低，是14笔AR各52拍delayer准备，不能当payload仲裁等待。

因此O-DS-AR在本前缀没有可选comp／DS竞争可消除。没有这个竞争或真实AR阻塞事件，不能生成一个“普通控制不覆盖”的完整收益见证；也不能把O-DS-AR的条件覆盖提升为所有合法前缀的收益结论。

## 字节守恒及证据限制

独立CSV重建每DMA恰好2AR、512拍R／4096字节、2AW、512拍W／4096字节、2B；W按原生AW FIFO绑定fullID，所有R／W LAST及R／B resp无错，最终无重建未退休DDR事务。root实际执行的CPP:3252–3260另逐字节检查两份拷贝内容和计数，成功run退出0。

CSV没有payload DDR Rdata、Wdata／strb，所以本席独立解析证明数量／身份／长度，payload内容相等依据实际CPP断言；PTE_Rdata已打印并独立匹配。functional NEXT_ID／DONE数值未打印；main确实把捕获vID／aID传给wait_done，不能捏造其数字。credit full／lookup／in_flight寄存器和各spill占用没有tap，零等待不等于测得这些寄存器全0。DDTW consume及DS返回ID也缺失；本席完整重建DDR义务与所有DDRheld，DDTW消费端归属由root源码证明和check_quiet负责。

成功manifest argv及log包含实际CPP编译，但manifest.sources未存CPP SHA；本席记录当前CPP SHA和实际运行binary SHA，不伪造缺失的构建时SHA。没有ABBA、不同动作或其他合法前缀，本次否决仅限这一个prefix。

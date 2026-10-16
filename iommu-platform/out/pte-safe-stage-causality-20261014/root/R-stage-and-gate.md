# root：安全返回下PTE阶段等待与准入门

## 新观测与范围

本轮只执行一个新被动前缀：当前两个真实iDMA，每源一个DEFAULT对齐4KiB，buffer512、CPUoff，正常串行NEXT_ID启动source0然后source1。没有warmup、batch、相位搜索、额外作业、buffer变更、NormalTL／AR仲裁修改或DBG驱动启动。共同t0在两个descriptor已正常配置、第一次trigger之前。

根合同允许先核firstVALID合法性；源码只给出 `最后bank-local AW0接受 = job1 first AR入FIFO` 的同步充分条件，不能把任意FIFO初态当实际iDMA。root因而只准入**被动合法性/阶段观察**，没有准入强制ABBA、更改调度或在线预测。原始[准入记录](probe-admission.json)把1运行／不扫描写明。

观察4151个连续preedge（绝对cycle62..4212）：merged双头VALID次数0、fresh安全双READ次数0，共享DDR AR的comp2/DS3 co-VALID次数0。原生单头规则只有当前头这一动作，F1不能等第二头或撤回已VALID。这个prefix没有可分叉的两安全选择点，**ABBA因果比较未执行**；不推出所有合法供给初态都不可能同步双头。

## root亲核的R阶段

`tests/nonblock_actual_dma_test.cpp:679-719` 的DS优先先选择已经DDR接受的最老DS，只有此前R未held才可提升。保持各请求剩余latency/beat/fullID；`:699-713` 默认仅给选中head扣准备latency。它不是对所有已ready请求都work-conserving的模型，不启用overlap更换服务规律。

在“PTE已DDR接受、无既有held payload、无更早DS义务挡在前面、DS返回路有真实余量且PTW等待该PTE”条件下，payload剩余beat和顺序不能再抢占该PTE的准备/首次返回。准入前的AR等待、其他DS及已有held仍是各自独立前提，不能由safe标签自动删除。

本轮private正常copy的iDMA响应consumer恒ready，完整作业512拍真实byte-lane容量足够；但这只排除自身容量溢出，不保证整个DDR永不held。DS delayer返回的名义50逐beat等待可自己产生backpressure。实际208个DDR held周期全部fullID49、两笔DDTW len7各104；不是payload32/36，也不是PTW48。所有12个PTE区间无DDR held R和新DDTW AR。

真实返回链为DDR输出mux Rspill→DS自己的multicut→DS输入demux Rspill→DS Rdelayer→DS_IF Rspill→PTW。comp与DS中间路径不能混成一个容量池；对PTE48，当前一笔PTE必须被PTW消费后才产生下一PTE，不提前使用future PPW/PA。源码 `rv_iommu_ptw.sv:384-397,507-524` 支持这个依赖链；`rv_iommu_ds_if.sv:222-280` 按原DS ID0分发。delayer只传真实payload，ready/valid需经过 `stream_delay.sv:45-97` 的Idle/Valid/Ready；名义50对应本计时标签52，是源码推导而非直接把50作为接口周期。

## 12笔PTE的准确阶段记录

完整owner源/DID/IOVA/type、walk epoch、stage/level、PTE_PA、前驱消费和各级accept见[结构化事件](PTE-stage-events.json)。source0/1只各有一个job，原始IOMMU IDs0/4、DDS PTW DDR ID48，DID10/11与固定S1 Sv39 context保持；numeric NEXT/DONE在main内部匹配，未打印数值，不伪造它。原始firstVALID与握手分开。

| 相对间隔 | 新运行全部样本 |
|---|---:|
| 同walk上一PTE消费→下一PTE首次VALID | 1拍（8个nonleaf续步） |
| PTE首次VALID→PTW进入DS内部spill接受 | 0拍 |
| 内部接受→DS spill到delayer接受 | 53拍 |
| 内部接受→DDR AR接受 | 56拍 |
| DDR AR接受→DDR首次RVALID/握手 | 32拍（冻结head准备31） |
| DDR AR接受→PTW消费 | 88拍 |

DDRW输出spill接受与PTW的内部DS接受不是同一个事件。所有12筆PTE的消费数据匹配原来setup_memory准确tableword。PTE8/9出生时原队列中确实还有source1 payload36义务，间隔仍上述值；这是有真实payload在途的单trace观察，不是改变payload先后的控制因果证明。

## 准入前残余与普通覆盖

实际编译是混合vendor：xbar/cut/delayer来自CVA6；mux/demux/rr/spill/counter来自IOMMU。AR源码给出的可选RR位移，在CPUoff且已有共享AR接受前史时，未锁点最多一个comp input grant；包含一个已有comp lock时最多两个。复位RR0局部“两次自由抢先”不能充当本轮自然前史。grant数量界不是有背压时的绝对时钟界。

一个冻结O-DS-AR在共享AR chooser只优先已有VALID的DS，否则RR；heldfirstVALID保持，固定4次DS的公平逃逸、fullID和全部R/W/B/fault成本保留。它覆盖可选RR位移，不撤回既有ARspill/DDR项、不预测未生成PTE。合同保存但未实现/运行；不能写成实测费用或全部F1覆盖。其8bit具体选择状态是该合同的声明编码，不是已经综合的总delta或总面积。

“MaxMstTrans8／MaxSlvTrans8”也不是一个免费共享8读额度：实际demux每源16个本地ID、3bit counter的正常满值7；共享mux的8管MaxWTrans，另有CPP16 read queue。两sole4KiB各2 READ，不会仅凭此填满16；本运行峰值3，真实满信用/队列延迟未出现。

## 完整完成与限制

每源真实2AR/2AW、512个R beat、512个依AW序归属的W beat、2个B；main逐字节核对两源的4096B目的数据，全部8192B通过。DDR AR/R/LAST、AW/WLAST/B用原fullID守恒且最终无未清义务。实际DMA job_quiet在共同t0后3190/4145拍恢复，所有native/data接口quiet边界为4145拍；DONE轮询记录3196/4150、check_quiet返回4151是后续读/采样上界，不能当精确任务延迟。

原始CSV没有payload WDATA/WSTRB，也没有直接credit寄存器、DDTW消费端或numeric functional ID。独立检验覆盖ordered beat/WLAST/B、PTE数据和真实quiet；W字节结论由**本次实际main的8192B逐字节比较、固定对齐native全mask源约束与beat守恒**联合支持，不称逐beat原始strobe独立验证。错误注入/一般异常未新测，正常error计0、原生fault终结源不改。

结论：**本固定正常前缀不能准入两安全头ABBA；相对PTE阶段差异没有成立。** 不运行最多四新性能条件，不新增在线F1逻辑，不把普通重叠当PTE反馈；F1全族、另一个自然输入前缀和实际FPGA服务保持unknown/open。

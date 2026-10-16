# 两 owner 有限源：第一个条件服务模型提案

2026-10-15。**建议直接实施下面这个小模型；不预设它会产生反号或D信息收益。** 它保留已接受的完成→同task/buffer补发，但另立两owner、带服务时间的研究投影 `D-CS1`；不改旧clockless原型、旧M0、RTL或全局安排。root冻结本文件与`contract.json`后即可交实现。若完整O0已辨认全部相关状态，结果就是这个有限服务族的信息充分性，不注入新隐藏量补救。

## Material Passport

- ARS-Codex / experiment-agent / plan；只做有界模型设计，无代码、仿真、网络或实机操作。请求Sol/xhigh；独立实际运行元数据unknown。
- 直接输入：旧`root/decision-analysis.md:14–16,42–52`、`design/proposal.md:49–84,95–128`，旧`model/prototype.py:5–40`；同批root的`root/information-contract.md`约束信息判别。
- 源支持的是task、generation、buffer、callback、补交和drain语义；下列全部时间、队列拓扑、债务、映射和初始化均为**预声明的合成stress假设**，无真实SSD校准或天然发生声明。实现/结果尚未取得。

## 1. 有效工作、有限资源与唯一差别

每owner有`Q=2`个task、各自稳定且互不别名的4KiB host buffer；参考invocation每owner`N=4`、目标`N=6`次成功NVMe read。LBA按该owner成功提交序号顺序产生，**不是host IOVA**。新generation只在旧generation成功CQ的callback后提交。达到本owner第N次成功提交立刻draining，余下callback只释放task；任意错误使运行失败，不进入同工作量比较。每个动作独立重算完成、poll、补交，不共享submit/CQ日历。

四个buffer公开映射到四个4KiB叶，允许读写、无别名、invocation内不撤销或改权限；每task固定独立ID，最多一个generation在途。共享FIFO IOTLB容量`C=3`，命中不改FIFO次序；一个普通TL、一个串行两级依赖PTW。每个walk的第2笔PTE读只能在第1笔返回后产生。TL查找、快probe各耗1单位，共用查找端口；正常TL不在walk期间查找新普通请求，空出的端口可每单位probe一个排队请求。miss结果在IOTLB插入，cache mutation使先前probe-miss可重查。命中结果占原请求预留结果槽，不能凭未来hit真值许可。

- `P_M`：walk期间可以同预算probe并保存已确认的合法hit结果，但等普通TL解除walk占用后，按请求FIFO释放；可复用准备结果，免去再次查找。
- `PN_M`：相同probe和预留结果槽；walk期间可把已确认cached leaf、权限和ID/容量条件全部满足的请求提前释放到payload队列。其余走同一普通路径。只有这个释放时机不同。

四个command槽、四个payload/结果槽都在成功submit时预留；设备完成后保留其数据直到host写完。TL等待队列上限4；每个memory server至多8笔resident（含服务中）。backpressure时请求和身份保持，成功accept才占资源。此处单页、独立ID、固定映射使安全条件可直接检查；**不是对原N guard的RTL等价证明**。大页、其他pool/QD/pacing未进入首版，不能据固定布局结果声称胜过这些普通方案。

## 2. 不看模式的外部服务方

单设备server、FIFO，命令被接受后排队；每笔开始时采样有限债务`h∈[0,64]`，工作量为`d + floor(h/8)`，到时才生成host payload需求。设备只收到command及accept时刻，不收到P/PN、TL/PTW相位、候选信号或胜负。每单位时间债务减1（下限0）；每次设备工作完成后债务加16（上限64）。其余时间包括poll、quiet、读Z、配置、clear、warm、free都照常冷却。服务完成后立即可做下一笔已排队工作，无须等待本命令CQ；预留输出槽保证有限容量。

主族的未知初态只能由一个前史生成器产生：从`h=0`开始，顺序完成`k∈{0,…,6}`个**内部维护work**，每个base work=1、同一债务规则；再idle `r∈{0,2,4,10,12}`单位。全部维护work已完成、无host命令/payload/CQ/占槽，最后债务作为参考invocation的初态。35个`(k,r)`全枚举，重复债务仍保留其前史权重；未按winner、PTW相位挑选。O0知道生成规则、集合和声明的等权前史权重，具体前史在本观察进程开始前未观测；**如果真实系统已知前史或初态，必须给O0**。这只是研究上的部分可观测假设，不是SSD有该状态的证据。

债务可跨quiet存在，因为它是已完成内部工作的后效，不是被免费丢掉的未完成host工作。参考invocation的完整带时间CPU历史可能已经辨认它；必须实际精确分组后裁决。

## 3. payload、CQ、poll与同一有限终点

device service结束→TL→已翻译payload **device→host写**（长度W单位）→写完成→物理CQ写1单位→CPU poll读回CQ→callback。CQ不早于全部payload写完成。一个共享CPU轮流访问尚未结束的owner；一次访问发起一笔CQ poll读（空poll也耗1单位memory服务），返回时最多消费一个已可见CQ。成功callback耗1单位，随后同步提交同task新代（CPU 1单位）或free（CPU 1单位），再访问另一owner；空poll之后CPU间隔1单位。owner无在途且已drain/free后不再poll。初始化提交按owner、task排序，各耗CPU 1单位；每次成功submit才加计数。CQ FIFO按该owner的CQ写完成次序，poll读完成时取已可见队首。

预冻结两种**研究拓扑控制**，所有服务工作量相同：`coupled`为一个非抢占FCFS memory server，PTE读、payload写、CQ写、poll读竞争它；`split_rw`为独立读/写两个FCFS server，PTE/poll走R，payload/CQ走W。后者检验共享读写竞争是否决定结论，**不是同面积比较**。前者是新增统一server假设，旧M0读mem_q和写队列分离，二者均不称旧M0或RTL等价。

同时到达的memory请求以`(ready_time, kind_rank, owner, task, generation, phase)`排序，rank依次PTE、payload、CQ、poll；其他队列以ready_time和身份排序。每单位边界：先完成memory写，再完成memory读，再处理设备/TL/probe和CPU完成，按上述键入队，最后启动空闲server；所有工作至少1单位，无无限零时转移。债务在每个时间区间先冷却，结束边界的device完成再加热。held请求不因重试重复计入。

每次invocation：CPU分配4个buffer（各1）→公开map准备（各1）→两owner队列建立（各1）→IOTLB clear（1）→正常TL依次warm `b00,b10,b01,b11`四页（实际两级PTE成本，无payload）；再开始源。结束必须双方准确N提交/N成功、所有payload/CQ/poll/TL/设备事务退休、callback和task free完成；随后显式pool map释放4×1及队列销毁2×1，再确认全共享通路quiet。task free不是声称SPDK逐次IOMMU unmap。下一invocation重新分配/映射，用同公开IOVA前必须clear。模式寄存器初始P；改变模式耗CPU 1，不变则无写。额外quiet idle `b`照付并冷却。

主要目标`J`从参考invocation第一项setup之前，到目标invocation全部free、map释放、销毁和quiet之后；另报参考/目标有效工作完成时刻、两owner最后成功callback、全部quiet/free时刻。参考任务也是固定的有用工作，所有策略共用其P_M参考规则。**本模型完整只指这个投影**：SQ/doorbell、PRP/SGL抓取、PCIe、真实CQ/CPU一致性、平台互连和每层映射翻译未展开，分别被CPU submit、设备base work、固定CQ/poll工作聚合；未测真实成本不能用这些单位换成原生速度。

## 4. O0、旧Z、负对照与停止规则

完整O0按root合同：全部公开参数/规则/映射/额度/地址/LBA及无随机种子这一事实、reference P_M所有CPU可见带时间submit、每次poll及空/非空结果、CQ身份/status、callback、generation、buffer、free、边界和已知动作。debug中provider h、内部完成和未来结果只供验证，不给策略。按**同一公开配置的完整O0精确相等**分组，不抹时间或混不同W/Q/映射。

候选Z仅为reference源窗口内三个16bit饱和计数和有效/饱和标志：①PTE已就绪未完成且payload在服务的重叠单位；②walk期间实际合法probe-hit事件数；③普通walk占用单位。split结构中的①只叫重叠，不自动叫阻塞。源窗口从首submit到最后共享事务退休，不含warm。参考结束且quiet后锁存；读出CPU 1＋摘要决策CPU 1单位，计数硬件资源在两模式同预算内，无实测面积或临界路径零代价主张。读出期间provider继续冷却。先算O0/旧Z的离线信息上界；上界0则停止训练或扩矩阵。若正值才实施策略；普通控制获得相同Z，同样可等待0或2单位；不能把等待冷却收益算作信息价值。固定P/PN、O0精确后验的离线下界及其计算成本未知限制必须单列，不能把枚举oracle冒充便宜部署算法。

必须有：①公开确定初态（前史给O0）的重放控制；②full reset将相关状态清至公开0且付reset 1单位；③target边界**在决策后**reset，再独立重做同一前史生成器、付其完整耗时，作为独立刷新负对照。③未来状态与旧Z给定O0独立；旧Z的纯信息价值应为0。它不能跳过维护时长或把初态塞到目标PTW窗口。

最小可执行矩阵见JSON：4个主配置×35前史，先完成一次参考P，再各自重算目标P/PN；4个参数留出在代码/策略冻结后运行。若主配置的完整O0已识别全部状态或同组最佳动作相同，只报告对应族闭合，留出可作实现核验，不拟合Z。不是整族D失败，也不排除更便宜摘要的计算成本价值。任一守恒/代际/容量/CQ-before-payload/模式泄漏/独立刷新失败，先否决实现；不能当物理发现。无反号也保留完整闭环与因果边界，不增设获胜态。

手工sanity（不是已运行结果）：warm后缓存容纳四页且invocation无clear/evict时，没有PTW，P/PN除多余配置费用外应一致；W增大可增加coupled中PTE排队，但split中同一payload不能直接占住R；参考公开初态且确定转移时，完整O0可重放全部状态，Z没有新增信息。这些分别检查提前许可、读写耦合和隐藏信息是否被错误注入。

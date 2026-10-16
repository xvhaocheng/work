为Codex已定义的小状态模型写一个完整Python3脚本提案（单文件、标准库，无命令执行/网络/文件自行读取）。普通实现任务，不替Codex做创新或RTL合法性判断。输出fenced python+接口/限制。目标directory out/four-dma-boundary-20261008；脚本CLI --output DIR 在执行时保存 results.json 与counterexample traces.json。Codex执行/核查。

模型精确契约：
4 streams0,1,2,3, bank=d&1，original AXIID=d*4；每源仅一个正常copy两read/write burst (d,0),(d,1)。固定virtual-W义务顺序 ((0,0),(0,1),(2,0),(2,1),(1,0),(1,1),(3,0),(3,1))，各stream保序，bank切换等前bank所有W末拍；假设当前W匹配物理AW可准备且memory/response有限，非当前read不发数据请求。只抽象翻译输出AR头的停车/派发，不模拟TLB、PTWcycles、FAST、AXI VALID周期稳定性、实际完整iDMA或生产可达性。
每bank输入read顺序为同bank两streams各2个burst保序的shuffle，bank0有6种、bank1有6种。共36个case。fixed sample bank0=((2,0),(2,1),(0,0),(0,1)); bank1=((1,0),(1,1),(3,0),(3,1))。
每bank至多一translated head，state包括readcursor[2]、translated_ready[2]、pool按bank分类或shared tuple/request集合、writecursor。future输入read不是另计的内部存储槽；由上游源保持/有限输入队列按固定顺序供给。本模型不提供physical pipeline occupancy或RTL可达性证明。
transitions：
translate(b): 若该bank ready false &&cursor<len，将ready true（表示有限翻译完成，所有banks共享引擎仅一次transition完成一个translation）；
park(b): 若ready &&capacityrule允许，将目前head放pool，readcursor++、ready false；同一tag不可重复。同stream更早已在pool允许后续park，但不允许派发越过earlier。
direct(b): 若ready head==current write obligation 且该源没有更早parked，直接模拟该唯一物理read以及有限R->W完整排空，writecursor++、readcursor++、ready false。memory阶段原子折叠，这是progress模型不是cycles。direct和park可并存选择，不人为强制park优先。
dispatch-pool: pool包含准确current write obligation 且same-source先前均retired，则移除它、writecursor++。其他读不发memory，避免返回占满小payloadFIFO，此策略与原型write-owner门概念一致。
terminal success: writecursor==8,cursors==4/4,ready false,pool empty；不能提前成功。
deadlock: 非success且无任何enabled transitions（仅抽象模型中的无进度终点），不要把busy但有translate/retire transition说deadlock。
执行BFS遍历所有reachable states，全部transition有定义; visited去重，记录每个case至少1success witness（若可达）和1deadlock witness（若存在），不能把存在success当保证。用单调计数/有限状态排除公平性未执行导致的假停滞。
policies：
local2: 每bank pool<=2，共4槽，无bypass禁用；direct始终允许；
globalH: 两bank共H槽，H=1..6，任何bank可park，direct始终允许；
owner-reserve2-H: 共H槽，H=2或3；ownerbank=current obligation stream&1，当前bank可park只需total<H；非ownerbank可park当total<H且otherbank parked count < H-2（为ownerbank总计最多2头保留容量），owner切换动态重算已有slot分类。不要把当前bank=真正DMAowner，这两个维度分开。
所有policy相同sequence/contracts，同source order/所有8义务全部完成；遍历36cases和fixedsample。每输出policy countsuccess reachable,deadlocks reachable,state count,casecount，并aggregate all36无deadlock或有多少case有至少一deadstate；写清非RTL proof，header_slot counts不含现有原始ARFIFO/源保持descriptor/payload/control。counterexample应含每step action、完整state、当前WownerID/bank及blocked headIDs，不输出很长全部visited states。
额外自检应是实质守恒：所有已parked和ready请求唯一且readcapture/dispatch/writecounter一致，派发严格writeorder；异常状态 raise；不要只测试函数实现镜像或用期望结果强制通过。
请提供complete script，别写项目全局文档/新机制新颖声明；思考high,max保持393216。

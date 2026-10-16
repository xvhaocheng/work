# §41：根 Codex 静态几何判别契约

此契约只验证上一轮理论模型的错误、普通贪心最优性和几个静态计数；不是DUT、真实暂停、物理协议或时钟时间保证。

## 定义与范围

输入N为正整数单位，mandatory source endpoints R0包含0/N；destination completion endpoints D包含0/N；合法source endpoints A包含0/N并覆盖R0。B≥0为可独立停车的有效tail单位；H>0只界所选择目的停止点之间的prefix距离。新增source endpoints C⊆A\R0；不合并、不移动原端点。每新增cut的cost恰为1。

q安全条件为 `[q,min(q+B,N)] ∩ (R0∪C)` 非空；等价于最早source endpoint≥q距q至多B。safeD按升序排列，从0开始相邻可达safe point的gap不得超过H；遇不可跨gap则后面的点也不可达。达到N为该几何问题可行。

新增cut会改变其它q的r_C(q)，所以B>0时每destination node单独cost1的旧DAG不能给真实最少cuts。反例：N10,R0={0,10},D={0,4,6,10},A={0,…10},B2,H4，一个cut6支持q4/q6，真实最优1，旧node-cost模型2。B0同输入需cuts4/6共2。

这些量不含真实请求lookahead、多个W头顺序、shared spill停车能力或译址阻塞。即使几何可行，也不能报告真实RTL已能暂停。

## 指定算法（Codex推演，普通实现交由DeepSeek）

`closure(E)`：从0遍历sorted safeD；当前point与last可达point相差≤H则推进last，否则break；返回最大reachable endpoint。

`greedy`：C空；求x=closure(R0∪C)。若x=N返回C。否则在D∩(x,x+H]选择最大的可支持q，即A∩[q,min(q+B,N)]非空；无此q返回infeasible。选c=max(A∩窗口)，新增c后重新closure，重复。若c已经存在或x不前进，报实现错误，不自动改变定义。

交换证明见`rules-proof-agent.md`：任意最优路径第一个超过x的q_o不大于q*；其支持端点c_o不大于c*；换成最右支持c*保留所有≥q*的旧支持点，而q*可直接到达，故可以接回原后段而不增加cuts。任意有限A下必须选择可支持q，不能无条件选最大D。

`brute`必须独立枚举A\R0的所有子集、按新增数从小到大，独立按端点间距检查从0到N是否可行，取得oracle最优。不得让brute调用greedy或复制greedy决策。两者共享定义型safe函数可以，独立选择所有cut sets。

`naive_node_cost`只作错误模型对照：对每个D node，mandatory支持则cost0；否则A窗口可支持则cost1；按gap≤H的DAG求最小cost。它不记录已选cut支持范围，故可能多算。不能把它当新baseline RTL。

## 唯一有限计算任务

1. 在全整数A下穷举N=1…5，R0/D各自取所有interior子集（0/N固定），B=0…N−1，H=1…N，验证greedy cardinality与独立oracle相同，包括infeasible。理论case总7585。
2. 对任意A进一步穷举N=1…4：A包含0/N，R0取A interior任意子集，D取全grid interior任意子集；B/H同上，理论case总3805。保留与第一组重复的分类，这是两种有意不同的输入覆盖，不假装是独立DUT重复。
3. 记录第一项最小反例的greedy/brute/naive/cuts/支持点；B0控制与一个infeasible例N10,D{0,5,10},H4（任何B）也记录。
4. 计算byte尺度代表例，不作brute：N32768、R0={0,2048,…32768}；D={0,1024,3072,…31744,32768}；A为所有8B对齐端点；B=0/24/1024，H=2048/4096/32768。报cuts、safe stops、最坏safe gap、source physical request数16+|C|，目的burst数17。H/B仅假定静态容量，不宣称现系统实际可独立停车B=24/1024。
5. 对B0/H4096列software tiling endpoints 0,3072,7168,…31744,32768；每segment分别与原source/dest boundary集合求并，累加source/dest有效区间数，验证理想source24/AW17/subjobs9。名义FIFO perDMA24B，两路合计48B。此为静态计数，不是运行后的请求数／TL/PTW／job开销。

报告只写JSON，含model_scope、case counts、mismatch detail（至多first20）、selected witness、byte examples与software tiling。出现greedy与brute不一致时exit非0并保留实际反例，不修结果。单个脚本无外部依赖、无shell/network、不会改生产、不会读其它工程文件。输出完整raw作为一次数学计算证据，不写单元测试，不反复执行。

## 接受条件与新颖性判断

有限oracle检查是证明的补充，不代替一般证明，更不代替物理进度实验。即使全部同效，此算法仍是静态窗口覆盖／分段的普通构造；新名字和数学计算数量不能建立创新。

软件tiling是实际强控制，但会改变硬件subjob IDs/DONE、CPU供给与TL请求；只比较静态计数不得声称同接口同成本。后续资源释放问题必须与现成内部非均匀splitter、fullR/fullWreservation、普通ready queues、正确macrocopy完成合并比较。

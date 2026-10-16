# 共享翻译控制位置与完整任务预测方法

2026年10月15日。方法A1先证明一个控制位置仍能作什么，再在实际服务合同下计算完整任务。它接受源级语义、协议级及实现级三个精度层；低一层通过不升级为高一层的收益。当前交付的是具体分析算法、来源合同和可执行检查器，不把事件图或状态空间求解本身称为新算法。

## 输入与输出

输入合同为 `K=(J,E,R,L,O,I,U,S,C,F,P)`。

- `J/E`：有限有用工作、完整生成依赖和源程序顺序。含准备、descriptor调用返回、真实DMA退休、compute/barrier、cache交接、consumer和释放；后续PTE/载荷只能由真实前驱生成。
- `R/L`：每一级实际容量、端口、credit和所有权生命周期。标清准备cell、输入spill、target spill、返回buffer、payload slot及可合法复用字段。
- `O`：已经首次offer、接受、返回未退完的义务；以源、方向、job、burst和完整ID/源内序号区分，不能只用ID作唯一键。
- `I/U`：各控制位置的观察映射、传播条件及合法动作。只许改尚未由该owner承诺的新选择；对相同观察历史必须作相同动作。具名普通程序与候选获得相同信息和预算。
- `S/C`：具名服务与全部费用。可以是确切常数、已证明区间或符号未知；未知不能当零。源语义中的一步不等于模型周期。
- `F/P`：双方及必要consumer的真实完成谓词；从同一ready开始、由实际执行产生的共同合法前缀，包含其全部费用。

每个字段附来源等级：原始源码/测量、已接受推导、显式适配、条件服务或未知。案例必须声明所使用层级。没有IOMMU绑定的源图可以证明源程序顺序，不能据此推导PTE干扰或FPGA周期。

输出分开记录：

1. **动作证书**：候选ready、不可撤回头、阻塞owner、下一合法选择及其准确控制位置。
2. **完成证书**：所有必要完成节点和生命周期闭合；在完整服务合同下给双方与consumer完成向量，信息不足则输出具体未知和偏序。
3. **部署判别**：现有普通控制足够、需要迁移到更早位置、旧义务使所考察动作无效，或证据不足。判别必须附目标、合同范围和量词。
4. **差异与费用**：完整工作/字节、各方向流量、供给与同步、slot持有、状态/端口/连线/选择时延和损失的重叠。物理未知保持null。

## 算法

### 1 合同编译与准入

从源程序或实际状态机生成事件模板和资源转换。先核接口绑定、accepted job options、slot、读门、AW/W和结果归属，再检查每个完成目标的祖先是否齐全。准备成功不自动产生载荷许可，也不自动释放旧slot。源码的跨线程无全序只证明允许不同交错，不证明某个硬件选择器实际见到了两个头。

对每个可能的条件分支，保留其guard。例如lookup是否miss、结果是否过期、consumer是否已释放slot。若实际guard不可知，分支保留为环境选择或输出未知；不能事后按赢家选cache初态。

### 2 从完整前缀求合法ready集合

状态 `x=(发生事件,活owner,held头,各队列/credit,cache/权限,时间或约束)`。

`Enabled(x)` 只包含所有生成前驱已满足、资源和owner允许的事件。`Offer` 将完整packet绑定到当前owner；`Accept` 把义务移交下一级；`Retire` 只消解其实际完成部分。移交前后的两个实体、同ID顺序、AW/raw-W/target-W/B与上游buffer归还不混为一个done。

若某个更早控制动作改变返回时刻，重新展开后续lookup、walk级次、physical request和consumer释放；不固定原trace后缀。不把同一残余读拍重复计入多个walk阶段。

### 3 明确干预集合

对位置 `c` 和状态 `x`，求 `U_c(I_c(history(x)))`。若有held头，唯一合法输出是保持该完整头至接受；新策略只能处理后继新选择。若观察还没到达，就不能使用该信息。需要阻挡新请求时，先排除为旧AW/W、合法目的许可或buffer排空所必需的请求，防止门控制造资源环。

输入只有固定具名策略时，只评估这些程序。不能以“一个普通DS规则失败”推出所有当前控制失败；不能把每个隐藏状态的事后最优动作拼成在线策略。

### 4 推导完整结果

在有限、已绑定的状态转换上，执行固定策略并保留服务环境的所有允许选择，或给出单一显式服务实例的条件结果。使用memoization合并同一完整状态；任务/资源尚未结束而没有合法事件时，返回死锁/缺假设证书；到达有界探索上限返回UNKNOWN，不假定排空完成。

每条已证明合法的执行顺序，补齐实际资源串行、有限容量和owner边。对于非负确切持续时间，可用标准递推：

`start(v)=max(finish(u) for u in predecessors(v))`

`finish(v)=start(v)+duration(v)`。

这些前驱必须含资源顺序和真实阻塞，不能只有应用DAG。若顺序依赖持续时间，还需满足其race guards；不能先任排顺序再把递推值当可达预测。只有因果图时，上式表达依赖下界/符号约束，不是完整完成值；没有有限服务界时不输出有限WCET。

对所有必要目标输出向量 `(T_A,T_B,T_consumer,T_release,T_quiet)`，不得只选最有利一项。目标由事前固定的deadline、完成向量/最大值、或明确成本约束定义；不存在所有目标通用的“最佳位置”。

精确接口如下，固定策略评估不偷偷升级为策略合成：

```text
Analyze(K, position, fixed_program):
    validate provenance, finite work, all resources, observations and completion goals
    x0 := execute and charge the common legal prefix
    if timing or protocol binding incomplete:
        return source/protocol certificates, symbolic constraints, UNKNOWN full-time result
    frontier := {x0}; seen := {}; outcomes := {}; failures := {}
    while frontier nonempty:
        x := pop(frontier)
        if complete(x): record every owner/consumer/release/quiet finish; continue
        enabled := transitions allowed by dependencies, capacity, owner and old obligations
        a := fixed_program(observation_history(x))
        if a is not legal: record the first violated contract; continue
        successors := all service/environment transitions permitted by K and a
        if no successor: record deadlock or missing service assumption; continue
        advance exact clocks/service counters, create dependent requests, update owners
        enqueue unseen successors; retain resource/causal predecessor certificates
        if declared work/state/horizon bound exhausted: return UNKNOWN for unfinished paths
    return complete outcomes and failures under exactly K and fixed_program
```

只有有限服务上界和有界控制等待都已证明，才可能由此给有限最坏完成。弱公平仅保证最终READY，不能换成周期上界。若要说当前所有策略都不满足目标，需要另外枚举合法策略/求解观察约束下的控制问题；本实现没有做这种通用合成，也不把固定策略比较作为其证明。源合同检查使用事件DAG的祖先闭包，内存/返回容量未绑定时不会调用假想的时序后端。

### 5 给出准确部署结论

| 判断 | 充足条件 | 不足以支持判断的现象 |
| --- | --- | --- |
| 普通入口优先足够 | 固定普通程序在相同合法信息/资源下满足事前完整目标；若只证明当前两头的选择，结论只到局部动作 | PTE头已有效，不等于完整任务一定更快 |
| 需要更早控制 | 若目标只限重排一个source前缀，证明该前缀已承诺且更早位置允许重排即可；若目标是完整完成，则须证明当前位置所有允许且可实现策略均不能满足固定完整目标，并有已计费更早策略满足它 | 局部旧义务不可改不等于完整目标必须更早控制；也不能以单个DS-first失败、信息本地产生得早或接口缺字段作证明 |
| 旧义务使这里无效 | 所考察差异要求撤回该owner已offer/接受的请求；给出具体不可撤回边。若要称整个完整结果无效，还要证明所有后续合法动作的完成投影相同 | 只看到旧payload债务，或一个任务无竞争；都不排除其他位置/后继动作 |
| UNKNOWN | 缺真实ready共现、控制权、service、完整后缀或费用；列出最小缺口 | 不能将unknown写成零收益、撞车或整条idea关闭 |

更早位置不一定是新接口。源端公开完成收据、合法准入、普通串行/预取节流、分段或准备，都应进入最低成本比较。若同付费接口普通门控复现策略，只去掉策略增量，接口/部署价值另核。

## 本轮的具体源端推演

HERO height32产生A2/B4/C2八笔2048B搬运、四个8192 MAC计算和六个2048B槽。A0调用返回后，未发布的A1未来预取与B00当前用途可有不同合法交错；当前K00使用A0/B00，不使用A1。但K00前原程序还要求A1和B01的发布调用返回，不能为了“先消费当前”把这些发布移到barrier之后。

因此可保留两种普通实现作为后继对照：同信息的当前用途优先发布；或在原完成API上节流未来预取，等待B00准备/完成后再发布A1。后者必须计跨线程协调、等待、配置和损失的重叠；原API只有job完成、没有免费translation-complete回调。只把A1/B00的调用次序交换，若它们在共同旧等待中很快都已发布，可能完全不改变下游到达；不能先验许诺收益。

若A1的源发布已完成，晚位置不能改写这个共同source发布前缀；若同一端口已有A1的held完整头，则该端口不能撤回它。**这不证明B00在所有下游端点都不能赶超A1**：多端口、独立路由及返回服务仍需实际绑定。若位置在K11之后，A/B发布机会全部属于共同前缀，剩余C输出及consumer后缀必须保留；不能把早期控制作用记给这个晚位置。n=2且B只读时，原两个B槽可以保留完整B，普通程序可删除B10/B11及对应未创建job的wait；方法必须识别它们不是必须发生的翻译机会。

HERO实际main启用copy-based target，SVM路径关闭；source发布适配不是既有共享IOMMU接口。**本轮源语义可以验证上述动作/数据/所有权，完整时延判别必须返回UNKNOWN。** 不给2048B增加fullbuffer许可，不借用stage117的258周期当HERO cache成本。

## 实现和本轮验收层级

[analyze_contract.py](analyze_contract.py)实现source合同结构检查、有限依赖/ready、合法prefix、source发布前缀保护、B保留、完整consumer祖先闭包、符号完成约束及原生准入缺口检查。它尚未实现上文协议/cache/有限资源的通用状态探索后端；这些绑定缺失时明确返回UNKNOWN。独立执行器按同一冻结源合同执行真实矩阵与字节搬运，以逻辑事件序号记录，不提供总线或周期模型。

四案为正向未发布选择、相邻已发布边界、普通B保留结构留出、K11之后的完整后缀结构留出。它们用于检查来源合同及方法规则；不是原四项完整任务时间预测已经通过。结构留出不用于调整正例规则，不能冒作新的独立应用工作负载。

后续最小改进是绑定一个真实共享IOMMU源端publisher/完成收据及任务，得到实际ready集合、选项和所有权，再给具名服务与供给成本。若方法的全部新增结论都可由已有简单调度或标准工具直接得到，收窄该版独立方法贡献，保留合同抽取和完整完成资产，不关闭共享翻译主问题。

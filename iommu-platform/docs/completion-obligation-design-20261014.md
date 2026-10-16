> 本轮实际goal已complete；324,525 tokens，39分32秒。五页同步和76项新交付核验在此前通过，无额外token预算。关闭本版本，不代表论文完成。

# 完成依赖与翻译用途分别退休：CO1有界裁决

2026年10月14日，日本时间；actualgoal1791971625，无额外token预算。

**关闭CO1这一版的独有准入动作与必要逻辑成本主张，不进入性能microbenchmark。** 具体普通OC1-min能表达其阶段引用保护、释放链排序、最后用途解除及原缓存让路；无需普通初稿的租期或结构深度。这是具名普通程序覆盖，不是已发表前作完整覆盖或整条任务协作方向无效。论文未完成。

用户要求参考A–H报告、结合既有进度独立调整。root保留翻译、数据服务与完整任务的共同评价，暂停没有新完整入口证据的AW版本，亲自提出CO1；一个必要的新Sol席独占构造普通控制与成本。实际root及新席均6.1-sol／ultra／原生有效475000，无DeepSeek/Claude/Astra，旧席未恢复。

## 新设计及因果边界

CO1由可信软件登记有限任务的src/dst/len、正式上下文/映射代、前后继、真实就绪与buffer最终释放者。它保护尚有登记翻译用途的resident页；合法ready任务位于当前buffer释放链时，可优先准入。最后一次翻译结果真正交给原生物理地址owner后，该用途可先解除保护，整任务、R/W/B、consumer与buffer仍活到真实完成。

这里区分两种退休：不再需要IOTLB内容，不等于数据已完成或物理页可回收。root保守选择TC完整成功地址的`axi_comp_req/axi_comp_resp`交接；这是内部demux/spill捕获，不冒充外部DDR接受，也不把源端握手或TL response保存当成同一个点。最后用途、任务/source关联和不再重查的证明都是新增付费反馈。

保护是可牺牲的性能建议，不阻挡正式失效，也不替代W owner、read-admit、同ID顺序或错误终结。保护满但必须服务释放/公平任务时，使用原16项PLRU让路，保留被驱逐页的未来用途，允许后续重walk。没有另一合法ready任务时，不等尚未ready的后继来解除保护。

root发现并纠正一个公平边界错误：偿还defer后若不记录新的被越过head，刚ready的任务可连续被越过两次。最终每次授予都给唯一另一eligible head记token，持续双ready因此成为RR。原稿、手算反例和修正原因保留，没有执行模型或事后换场景赢家。

## 为什么普通控制覆盖本版本

普通席先冻结较丰富OC1-v1，含引用用户位图、释放/祖先排序、结构深度、RR和有限租期。它能表达三类动作，但次序与CO1不同，root不把它说成逐序列等价，也不以其可省字段制造候选成本优势。

随后围绕必要成本问题固定OC1-min：每key两个任务R/W用户位图、独立task/buffer所有权账本、release-path布尔、登记年龄、两源RR及唯一task-slot defer。它采用相同阶段交接事件和同一原缓存escape。root接受这个单一程序作为CO1文字合同的精确实现，不拼接不同场景的普通赢家。

逐事件配对理由是：相同登记置相同用途bit；相同精确最后用途清相同bit；相同native驻留/失效产生相同保护mask；相同就绪/释放关系与defer产生相同选择；同一fill使用相同原生victim。task和buffer退休在两者中都依独立真实完成事件。初态一致且输入/接口服务一致，后续状态与输出可以按这组对应保持，未留下独有动作。

这是一份具体构造与有条件手推，不是RTL形式验证或测得的周期优势。普通组合已经可执行地写清，不以“普通也可以添加候选模块”一句话代替覆盖证据。

## 完整逻辑费用与实际接口缺口

具名配对最多四登记任务、16记录key、两个buffer，native源0/1各一个eligible head；单PTW、16项IOTLB、原生队列与buffer容量保持。四槽不是四个新增并发DMA。

选一个materialized编码点：正式key字段为VPN29、PSCID20、GSCID16、stage2、映射代E、两份4位用户位图及valid。16记录的小计是`1216+16E bit`；年龄8、RR1、defer3为12bit。因此双方均可实现为：

`1228 + 16E + C_common`。

**1228只是组件小计，不是总新增硬件、最小界或面积。** E、source/functional-job关联、最后用途证明、登记与buffer generation、DAG/所有权、cache live读取/更新端口、mask与选择时延、软件供给/轮询、consumer/cache交接、错误/取消/失效及排空的共同费用全部保留。双方允许相同字段复用和相同直接范围表达，不强迫普通另造计数或prepared结果副本。完整类别逐项配对，所以本实现没有必要逻辑资源优势；完整绝对位数、物理成本和时延仍未知，不把未知项置零。

本轮亲核的实际限制进一步表明这些接口不是免费已有能力：

- 生产LaunchPort只给前两源；HOLD严格限64B、DEFAULT、对齐、batch关闭及backend idle。
- 旧ResearchPort只在非零research role的source1；banked非零模式禁止该role组合。一般任务可在软件读取NEXT_ID前合法延后，但通用硬件ADMIT与登记/反馈路由仍须新增计费。
- 当前`ErrorCap=NO_ERROR_HANDLING`，后端error字段固定0。正常最后B到done的链可核，原done不能证明全部错误、consumer、cache及buffer回收；完整错误收据/软件隔离仍为共同未实施合同。

源码是当前脏工作树事实，不证明本轮已加载bit或原生应用配置。

## 正例、相邻反例与准入

固定纸面结构为P:X→Y、C:Y→Z、B:A→B、D:T→U，每项64B，P真实完成及cache交接后C才ready；前两源顺序复用。名义copy载荷256B、正式key七个，实际字节/PTE/R/W/B/完成向量没有测量。结构只用来判断义务与合法动作，没有扩工作集、相位、延迟或并发寻找性能入口。

释放义务可让CO1区别只看当前速率/年龄的规则，但OC1-min也能区别；P的最后X翻译交接后可以unpin X、继续保留C对Y的用途，两者同样做到。没有后继复用、普通驻留/大页/准备已覆盖、保护成本大于rewalk、held VALID、stale epoch、取消/错误和记录不足均作相邻边界。原稿一个“C已完成而同一P仍ready”的非法相邻输入已撤回并保存纠正。

该四项64B结构实际七key、一般上界八key，不能由它产生全部16way保护。满保护循环等待只作一般范围的符号边界；不能当此实例的实测正例，也不补第17页来制造压力。面积、Fmax、CPU/软件成本、应用与净gain均null。

因此准入门false、四个新性能条件执行0。没有重跑8/9月以来的实验、validator或trace，没有新增AW采集、模型、构建、生产RTL/driver/ABI、综合或板操作。

## 后继与交付

CO1本版归档，保留正向生命周期设计和可用普通程序。后继只接超出这个具体程序的新合法动作或完整资源表示成本见证；不同任务/flow名字、未知物理成本或强迫普通保留TTL不算差异。不把该版本继续拆成重复源码goal，也不自动转向C或改变论文定位。

只有差异存活才另立最多四新条件合同；有界合成测量之后再核GPU/AI真实共享路径与FPGA。应用轨迹、面积、Fmax不被前移成所有有界机制实验的前提。原立题窗口继续累计。

导航：[完整产物](../out/completion-obligation-design-20261014/README.md)／[稳定裁决](../out/completion-obligation-design-20261014/root-result-acceptance.json)／[完整成本配对](../out/completion-obligation-design-20261014/root/full-cost-comparison.json)／[后继合同](../out/completion-obligation-design-20261014/next-goal-contract.json)。root直接核14原件/32关键段；独占普通产物、实际模型记录、纠正、原始状态归档和新交付验证均在导航中。五份状态/交接/计划/验证同步、必要新交付检查通过后才实际goal complete；完成回执另存，不代表论文完成。

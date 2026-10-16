# stage117真实缓存交接验证（2026-10-13）

真实WT缓存和控制路径的受控功能验收通过。三项新缓存微测试、single/parallel两个新增案例均通过独立原始证据检查。包1缓存取得和发布时peer仍活跃，但实际消费时peer已完成：本轮消费重叠投影未成立，不调整顺序或压力重跑。论文未完成，无新G0、性能收益或创新声明。

原问题是已接受的逐包协议仍使用不缓存的host直接消费，不能证明缓存可见性。进入条件为原三帧1639B载荷/1675B记录、四span60/65/503/1011B、地址/cookie/DID及驻留映射不变，既有硬件接口可表达有界交接。结束条件为实际缓存/控制接口实现、定向微测试与两个集成案例、原始字节/所有权/成本核验及完整状态交付；root接受和同步后才完成goal。

实际源码绑定Genesys2 WT_DCACHE和cv64a6_imafdc_sv39，Dcache128bit即16B缓存行、8way。实际wt_cache_subsystem与controller接到原互连CPU入口，共享原DDR backing。CPU直接物理端口0x80000000+offset；DMA原IOVA0x10000000+offset经驻留2MiB leaf到同一PA。metadata为池外0x80004000起每包64B实际缓存读写，仍是可信同owner模型，没有虚拟CPU/MMU、guest隔离或生产ABI承诺。

池、表、header和metadata原始初始化均经实际CPU缓存端口；host仅预分配零backing。公开NEXT/DONE调度，匹配后先启动合法后继，再交接完成前缀。WT普通fence不清Dcache，实际fence_i_i输入请求I/D失效，真实Dcache ack和控制器halt释放才完成；I/D各256行完整清空均从实际row信号检查。实际cacheline refill必须位于已交接包容量闭包或本包metadata，peer活跃时不扫全池。A源覆写和metadata标记均等待WT B退休；B原cookie真实缓存消费完成才归还。最后全池/表/metadata读取后重新排空CPUcache和DDR，再释放逻辑映射。

| 微测试 | 实际原始证据 |
|---|---|
| 缺失交接 | DMA已退休，CPU仍返回旧A5缓存字11936128518282651045 |
| 正确交接 | 实际控制确认后返回原JSON预期新字2207613190144 |
| 待完成写／MSHR | 写等待11周期、MSHR等待10周期后才进入清空；完整清空后确认 |

三个微测试在一次新micro运行中完成，另两个集成案例各一次，三次运行同一二进制：`edb148dd8796b1f55b446a1ca79ade2b917d52417316df2f24a6d485c7179fe4`。独立checker不信PASS或统计，重建原JSON、CPU grant/response、cache和DDR全部AXI、公开完成收据、物理metadata与最终内存。8项新缓存篡改被拒绝，1项PASS独立性控制通过；完整checker共12次评估，定向成本标签刷新另外记录，无RTL复跑或旧17项协议测试重放。

| parallel包1阶段 | 共同模型时钟位置 | peer包3 span2 |
|---|---:|---|
| 缓存取得完成 | 12507 | 活跃区间[11488,12810) |
| used发布 | 12637 | 仍活跃 |
| 实际consumer缓存请求/响应 | [12848,12911] | 已完成 |

包2/3实际consumer区间为[13456,13523]/[16056,16487]，同样无peer重叠。包1发布后到源复用完成的211周期内peer结束；这是所测校验/覆写/消费顺序的边界，不证明所有合法消费顺序都不能重叠，也不证明cache fence必须等待所有DMA静止。

每案实际DMA AR/AW/B各6，R总线1672B，有效W1639B恰一次，翻译请求/响应12，lookup12/hit11，PTW_AR2，DDTW_AR为1/2。实际CPU共7927次操作（4088读、3839写），有用读32576B、写30659B，包含初始化和全部最终扫描。实际cache AXI读取2691笔：1155笔16B缓存重填18480B，另1536笔8B不缓存表读取12288B；不能把30768B总读取全当重填。WT AW/B各3870。

每包交接258模型周期，三包共774；启动交接302，终末扫描前交接258，总计1334。reset另有一次清空，I/D清空分别1536周期。单/双DMA验证分别116/132/1576周期；源覆写及退休metadata分别112/99/653与211/99/653周期；consumer含metadata/guard/确认分别77/81/445周期。启动至共同就绪11419周期；最终核验扫描与排空36804周期独立列账，释放位置53416/53320。所有这些是实际端口驱动模型成本；完整CPU指令、软件、应用、Linux和FPGA时间仍null，不将单/双之间的位置差解释为收益或翻译因果。

构建有8次调用（6失败、2成功）：重复测试包、旧工具随机语法限制、5.006内部错误、缺少依赖及5.040 gate优化器错误均保留。隔离的官方5.040加-fno-gate通过；工具5.006/5.040源码编译约30.06/31.00秒，未替换系统工具或修改原RTL。成功构建7约7.81秒，构建8为新增micro释放观测的增量编译约1.61秒；micro进程约0.177秒，两案各约0.700秒，只作宿主执行成本。早期失败构建输入快照未完整保存，重建argv明确标识；最终成功构建8输入快照及实际编译依赖均保留，不能用当前快照冒充历史快照。

root直接审查关键实际源码、顶层、端口驱动、独立checker和原始微测试，要求补齐真实初始化、Icache完整清空、WT store退休与最终读取后的新排空证明。全部工作由root与两个新Sol/ultra席完成，运行观察均gpt-6.1-sol、有效窗口475000，无额外预算或上下文覆盖，不用Astra/DeepSeek/Claude。无生产driver/ABI/RTL改动、板操作或关闭实验复跑。

下一步固定来源支持的交付目标和CPU/单DMA/普通多DMA完整强对照，再一次G0；不重做缓存功能检查，不先要求实证残余再重复同题。直接CPU对照使用自身合法最小交接，不能人为强制同样的DMA全cache清空。继续计校验、源持有、cache重填、供给和释放成本；只有G0得到具体可达可避免翻译损失才进入G1。原两周立题窗口继续累计，本轮结束不代表论文完成。

[稳定裁决](../out/stage117-real-cache-handoff-20261013/root-result-acceptance.json)／[导航](../out/stage117-real-cache-handoff-20261013/README.md)／[完整分阶段成本](../out/stage117-real-cache-handoff-20261013/implementation/costs-and-overlap.json)／[独立checker](../out/stage117-real-cache-handoff-20261013/checker/delivery.json)／[后继范围合同](../out/stage117-real-cache-handoff-20261013/next-goal-contract.json)／[交付核验](../out/stage117-real-cache-handoff-20261013/root/delivery-verification.json)。

本轮完整交付核验后，实际goal已complete：566,137 tokens，38分36秒，无额外token预算；论文未完成。 [实际完成回执](../out/stage117-real-cache-handoff-20261013/root/goal-completion.json)。

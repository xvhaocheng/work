# 两份规则的关键审定（Codex，2026-10-07）

结论：双方均交付具体规则，尚无一条通过创新预审。完整原稿保留；以下纠正适用于读者与下一任务，不把模型共识当证明。

## DeepSeek 方案

按主 PTW 的 S1 最后一级、MEM_ACCESS/PROC_PTE、候选读 AxLEN!=0，拒绝 FAST accept，沿现有 LEAF→REJECT 等普通路径。不是“已翻译后短暂停一下”：被拒后不会在叶级窗口结束时自动重试 N，而是等待 normal_fire/候选撤销。AxLEN!=0 拦的是所有多拍读，2拍也拦；只有单拍读保持允许，原稿表格“短读可允许”的表述不准确。

`main_lvl_q==LVL3` 标识4KiB走访的最后一级请求，不表示已验证为合法叶；只有 PTE 返回后才检查合法性。上级可能直接返回大页，LVL1/2不是绝对的“非叶”；本规则只保护其所指定的最后一级窗口。`e3_level_o=s1_lvl_q` 不是可以随意替代的实时 main_lvl，相位应新增正确接口。

源码支持原 LEAF 拒绝和普通回退，且已接受 OUT 不改；但不能由保留 owner guard 直接推导“没有新增等待环/死锁”。需要覆盖已接受写的数据依赖、非4KiB证书任务以及 fault/失效后的进展。不得直接应用为生产补丁。只抑制在决定时刻的新快接受；此前已接受/已提交的数据仍可干扰 PTE，写路径仍会使用存储资源。

字段定义在 `rtl/rv_iommu_top.sv:135–145`，现无 AxLEN；不只改 accept 一行。默认结构、ATS/debug、serial/banked和测试类型的宽度/赋值需一致；同缓存命中的普通回退不能默认不引入额外访问或等待。

创新判断：walk阶段/深度本身已有前史，MASK §5.3已有3-bit walk-depth标记；§5.4已有PTE优先和数据配额。最后一级+长度+具体准入位置的精确组合尚未证实直接先例，也尚未交付足够独立增量。保留为低成本工程对照，不称论文主线。

## Codex 方案

规则保留：在物理 AR 首次 offer 前，要求当前PTE已越过选定的共同共享入口；每个实际PTE阶段最多预留Q个旁路读beat；PTE阶段进展才重新计量，读返回仅减少 outstanding，不补本阶段 used；旧阶段的未返回债务继续存在。准备译址和提交物理请求分开。排空已承诺写所需的读要有可核的豁免路径，豁免单独报告。

关键纠正：

1. **AR已接受不等于整条读已进入不可抢占服务。** 当前模型 `tests/nonblock_actual_dma_test.cpp:519–524` 能在没有 held R payload时提升DS请求，跨ID R允许交错。已有轨迹2763数据AR、2766PTE AR、2813首个数据R证明两种“承诺”不是同一事件，不能拿这段声称MASK/普通页表优先已无能为力。既有DS优先干预已移除模型中的退化，是必须超过或解释代价的强普通基线。
2. “预约”最多证明**本阶段新产生的 AR offer** 发生在当前PTE共同入口握手后。前一阶段已有VALID/排队/已服务的读仍可跨入下一阶段；不得宣称所有旁路读不会在接收/服务顺序上超过当前PTE。
3. 每阶段的 `used<=Q` 指在该阶段首次预留的量，预留后未握手也占用。阶段改变不能收回旧VALID。outstanding必须覆盖旧请求且根据实际响应身份递减，不能按相邻事件猜归属。
4. 原稿 walk结束分支允许普通发出，可能使 outstanding超过Q；因此不能无条件声称“所有时刻旁路未返回量<=Q”。可保留严格条件：受控准入仅在 `outstanding+beats<=Q` 时增加债务；已存在的无预算/普通回退流量必须计入准入时的剩余压力，并单列。若要全时全局上界，需要另一版规则及对过长burst的进展处理，不能隐藏改动。
5. 原稿没有定义 beats>Q 长期仍活动时的进展保障、共享侧握手代号返回、真实读响应分类、同拍仲裁次序。准备状态的失效/异常/IOFENCE契约也待定。故仅保留设计候选，不能直接照伪代码实现生产RTL。
6. MIG跨ID重排时，入口先后不构成硬延迟保障；原先已占服务的请求/CPU/写数据也不受单纯N读预算控制。不假定MIG服务上界或全部存储都FIFO。

创新判断：具体新增信息是优先级生效边界前的PTE状态与按真实PTE进展计量的旁路承诺；预约、进度信用、整形分别都有前史。还没有证明强普通基线不能用等价信息作同样选择。保留为待查候选，不认定“前人没有”，不作资源/收益承诺。

## 公平判别和停止

若后续继续，先在现成实际DMA模型里比较原PN、普通PTE优先、普通固定credit、DeepSeek规则、Codex规则；先核一个gap0条件，不直接上板/编译BIT。P和同条件N必须纳入最终性能判断，当前缺N数据。每一变量比较都保持工作集、job/burst、CPU生成方式、P策略、窗口定义一致；排空/错误、V耗时、A有效速度、命中机会和实际数据提交均核，不以fast计数本身判收益。

Codex规则需逐项消融入口预约与按进展补许可；如果普通基线同样好，或者仅通过关闭N/全局少发、人工放大DS固定延迟才好，停止创新推荐。模型效果不推出板上MIG效果；能越过普通基线才考虑生产接口与FPGA极紧资源。

## 原源核查

- MASK ASPLOS2018 §5.3–5.4、p8（PDF页7）：https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf 。已有walk-depth标记、Golden FIFO PTE优先、数据Silver配额；配额依据并发walk与受阻warp，不是此处逐PTE返回补许可的具体文本。此差异不足以证明创新。
- AuRORA MICRO2023 §3.4.3、p6：https://people.eecs.berkeley.edu/~ysshao/assets/papers/aurora-micro2023.pdf 。已有按系统干扰/任务要求调整加速器memory request rate，动态控制/换控制位置不是独立创新证据。
- Scheduling PTW ISCA2018、MMU-aware DMA ICCD2018、DWS HPCA2021：已有地图与§10–13前史保持。
- ASAP为MICRO2019的Prefetched Address Translation：https://homepages.inf.ed.ac.uk/bgrot/pubs/ASAP_MICRO19.pdf 。不能与ASPLOS2017 Translation-Triggered Prefetching混写，也没有据摘要推断与当前规则同式。
- NB-Walker MICRO2026全文/精确协议仍未取得，不声称已排除前沿重合。搜索偶见CATO实验室页面，但本次原页404，未把摘要作为机制查验通过。

## 执行与归属

DeepSeek-flash思考/high本轮1次，prompt196289、completion21938、total218227 provider tokens，reasoning18845；默认最大输出393216，未截断、stop。它提出规则与自否决owner方案，未执行命令/测试。Codex独立提出另一规则，并直接核关键RTL、DDR模型、原始轨迹和原论文，纠正两份草案的过强表述。无Claude/新Codex子agent、生产改动、构建、仿真、上板、JTAG/SD/reboot操作。

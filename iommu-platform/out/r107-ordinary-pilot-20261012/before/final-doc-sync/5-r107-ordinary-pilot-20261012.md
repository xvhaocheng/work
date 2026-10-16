# stage111 R107普通恢复采证：实际准备通过，接口错误已修复，r1启动核验中

当前真实goal为active、无额外预算、未complete。整体仍阶段一立题的普通基线／全成本关；R107条件保留。当前安装的是修复版，完整恢复、层proxy就绪和成功释放仍未验实，不进G0/G1，不宣称论文完成。当前状态以本段和CURRENT_STATUS为准，旧报告完整保存在本轮before。

本轮回答：同一有来源有限KV合同，使用合法大页、预映射、驻留、批量和授权单DMA后，普通准备与完整恢复是否可达、全部成本多少、是否还存在可达可避免的翻译损失？进入条件为对应source-frozen kernel实际boot及runtime CAPS；结束条件为完整原始数据与全成本裁决，技术错误不能当科学阴性。

有限合同沿用stage108：OPT125m L12/H12/TP2、三个唯一live槽/ref2/1/1/COW0，144完整恢复copies/1,769,472B；普通coalescing为96 restore logical runs，此前swapout48 logical runs，同量字节；每phase432个4KiB物理task。四逻辑池9,437,184B、普通backing16MiB、私有种子引用4,718,592B另计。真实vLLM来源、有限CPU研究回放与FPGA执行分别陈述；proxy字节不是原生GPU轨迹、attention或计算/到达/期限。legal4KiB回退保留，没有人为隔离或容量压力，没有旧stage106/108重跑。

首次完整安装/身份已通过：payload ade1bd4a…，整32MiB分区hash4ba6bf…、updater含fsync/close返回PASS；combined Dell954.607s。一次同f9重载/被动boot474.311s；实际boot67ad5da4…，release6.5.0-rc2-r107-ordinary-stage111，config/notes/DTB与唯一构建匹配。CLI及六资产板端SHA全通过104.942s。第一次有界RUN后无重复提交，原全部八文件tar9,845,248B两端SHA672fa85d…相等，transport与semantic acceptance分开。

实际准备的正向事实：4×4MiB DMA API分配成功，8个真实2MiB叶覆盖16MiB，2unique table pages，4096个4KiB CPU-PA审计及准备边界status0；不是仅凭8<16推零miss。private single-hart FENCE.I交接适用于此CVA6 WT cache；源DMA权限是BIDI/RW，软件所有者保证源不改，不冒称hardware RO。既有quiesced host-S1 helper提交VMA＋IOFENCE并核错误代次/CQ进度，限定无ATS/无S2/已排空的前序VMA pulse证明，不称通用fence执行完成证明。

实际失败：首个swapout batch在NEXT之前EBUSY，batch_count1但first_id/issued/completed0，DMA字节0；worker-layer0/joint0。root把composition word5配置idle当DMA quiet；冻结RTL在observer ARMED/ACTIVE时将该位置0，即使数据路径已空。因此ARM后首批被guard误拒。strict release也EBUSY，未撤PTE/free，private owner隔离到重启；最终同boot CAPS quarantined1/config idle0与dmesg证实。0 HPM miss、0observer cycle、四个初始化零输出buffer不能当普通方法成功或科学阴性。原源码审查遗漏该状态交互，本次原始记录与精确源码推导纠正其运行解释。

| 已到达阶段 | CPU raw cycles | TIME raw ticks | kernel nominal ns | TIME条件Dell秒界 |
|---|---:|---:|---:|---:|
| 私有种子输入 | 25,814,727 | 12,907,451 | 516,318,400 | 0.463637–0.570289 |
| 分配/map/HPM/init | 119,131,008 | 59,565,548 | 2,382,630,640 | 2.139601–2.631779 |
| 实际leaf审计/准备边界 | 5,740,506 | 2,870,251 | 114,816,240 | 0.103100–0.126816 |
| 换出失败入口，未DMA | 35,575 | 17,786 | 712,040 | 0.000639–0.000786 |
| 整个失败ioctl | 151,356,349 | 75,677,904 | 3,027,102,720 | 2.718359–3.343670 |

成功swapout/restore/全字节/层就绪/最终撤销释放成本仍未测，零占位不能当零成本；stage8只复制元数据，没复制四DMA池，stage9是释放前EBUSY。board userspace nominal input31.203s、ioctl3.065s、hash/fullmemcmp20.597s、raw write1.508s，whole56.419s；独立Dell wrapper57.952s分列。三CAPS给CPU45.266–55.680MHz、TIME22.633–27.840MHz的共同固定率兼容界；换算需同boot/hart/连续counter、无debug或PMU改写/复位/停钟且频率保持。两counter非原子同读也非独立振荡器，SI准确度/漂移未给。不混名义秒与独立物理时长，不加总nested intervals。

最小修复仅独立Linux副本的一个idma_finite.h，拆engine status/BATCH ACTIVE与configuration idle。owned batch前与匹配known DONE后允许observer ARMED/ACTIVE；严格配置、准备屏障、输出所有权和最终release保持原样，unknown NEXT/DONE/未证drain仍保留。实际冻结composition RTL＋精确原/新C helpers29检查通过，原ARM误拒复现；后台忙、batch ACTIVE、隔离、缺席均拒。它不模拟全AXI/backends，也不等于修复版上板成功。

一次独立干净修复构建59.542s，release6.5.0-rc2-r107-ordinary-stage111-r1，payload29,363,656B/e674f609…；UAPI/CLI/数据/f9 bit不变。实际新payloadhash/size/unchanged updater/GPT dryrun234.240s通过；按同一用户任务/同一可恢复首分区动作持续授权部署，不伪造新的hash-specific reply。corrective write+hash959.966s通过，整分区SHA ec13fd80…。同f9重启捕获进行中，仍须新boot/notes/config/DTB/actual CAPS与独立计时后才能新一次真实完整恢复；旧failed guard保留。

稳定当前裁决：条件保留同一R107卡及未完成原goal。新的决定事实只需修复后的合法普通完整执行/成本与残余，不扩场景或恢复已关闭候选。具体可达可避免翻译loss且完整成本成立才最小G0；普通覆盖则归档R107卡、回阶段一新显式需求合同，不否定主问题。当前没有G0/G1。

主要入口：[根因与原始绑定](../out/r107-ordinary-pilot-20261012/failure-root-cause.json)、[真实失败raw](../out/r107-ordinary-pilot-20261012/live/finite-pilot-result/receipt.json)、[部分映射接受](../out/r107-ordinary-pilot-20261012/validation/actual-failed-receipt-subset-acceptance.json)、[成本及未测项](../out/r107-ordinary-pilot-20261012/costs-and-boundaries.json)、[修复patch](../out/r107-ordinary-pilot-20261012/repair-1/validation/finite-idle-minimal.patch)、[针对性回归](../out/r107-ordinary-pilot-20261012/repair-1/validation/idle-probe-result.json)、[修复源审](../out/r107-ordinary-pilot-20261012/repair-1/validation/root-idle-repair-source-acceptance.json)、[r1构建manifest](../out/r107-ordinary-pilot-20261012/repair-1/build/artifact-manifest.json)、[r1授权依据](../out/r107-ordinary-pilot-20261012/repair-1/live/new-SD-user-approval.json)、[账本](../out/r107-ordinary-pilot-20261012/execution-ledger.json)。

181旧主产物/输入不变；生产Linux/RTL、Codex config/cache/原历史保留。专用Ethernet/任务HTTP/UART复用Dell历史方案，无Mac依赖；串口多行/长度与BusyBox无tar-z两实际适配错误及修正原证据保留，未把离线21项扩大成live兼容验收。全Codex Sol/ultra；新增独占席只核counter连续性，后续独占计数语义分配被thread limit拒绝，root亲自完成，没有假报子席执行；无DeepSeek/Claude/Astra。

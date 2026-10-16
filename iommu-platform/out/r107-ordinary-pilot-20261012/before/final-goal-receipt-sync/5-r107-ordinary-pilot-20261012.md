# stage111 R107普通恢复与全成本裁决：归档本有限卡

本轮请求的实际执行与交付已经齐备，真实原goal当前active，待最终工具标complete；无额外预算。整体仍阶段一立题；本有限R107卡归档，无G0/G1。方法论文仍在立题。下述归档仅针对当前合同，不否定共享IOMMU主问题，也不宣称所有普通方案中最快。

本轮问题是：有来源的有限KV恢复在f9 FPGA上，合法普通准备后能否完整完成所有字节、所有权、层依赖及回收，全部成本多少，普通强控制之外还有什么翻译残余？真实vLLM来源、stage108一次有限CPU回放和本轮FPGA iDMA执行分列。原源码和回放不重跑，不扩场景或制造隔离/容量压力。

有限合同沿用stage108：OPT125m L12/H12/TP2、三个唯一live槽/ref2/1/1/COW0，144完整恢复copies/1,769,472B；普通coalescing为96 restore logical runs，此前swapout48 logical runs，同量字节；每phase432个4KiB物理task。四逻辑池9,437,184B、普通backing16MiB、私有种子引用4,718,592B另计。真实vLLM来源、有限CPU研究回放与FPGA执行分别陈述；proxy字节不是原生GPU轨迹、attention或计算/到达/期限。legal4KiB回退保留，没有人为隔离或容量压力，没有旧stage106/108重跑。


真实身份：Genesys2/CVA6＋四iDMA＋RISC-V IOMMU/Linux，实际执行DMA0/DID10/S0(0xA00)。选用Oct7优化f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca；它不含当前生产独立RTL57eb562修复。本轮未构建新FPGA/RTL。最终kernel6.5.0-rc2-r107-ordinary-stage111-r1，boot370a9dd6-7d1a-4190-8395-acaf3e25f69c；config e28aba31…、notes027d3686…、DTBaa9cdf4c…实际匹配，taint0/单hart/四cdev及runtime CAPS通过。

原接口第一次RUN在NEXT前EBUSY：composition word5是排除observer ARMED/ACTIVE的配置idle，root把它误当DMA quiet。没有实际触发DMA，private16MiB及owner保留到已授权重启；原失败/UAPI/零初始化输出/guard/源完整保存，不作科学阴性。必要修复只在独立副本改一个idma_finite.h，owned批次前及精确DONE后读backend status与batch ACTIVE，严格准备/输出/最终释放与unknownNEXT隔离保持；实际冻结composition RTL＋精确原/新C helpers29项检查通过。原 source review的运行解释被这个原始证据纠正。

修复后唯一一次实际硬件pilot完整通过：priorout与restore各1,769,472B/432硬件task，48＋96=144whole batches全部positive NEXT精确DONE、issued=completed、control0/quiet1；两phase各AR/AW/RLAST/B=864，closing R/W stock0。四完整2,359,296B raw pools主机逐字节及SHA相等、原RV64 UAPI三buffer与JSON逐字段相等；24worker-layer和12joint在完整DMA、CPU交接与K/V全字节检查后记录。最终release_status0、quarantined0；同boot收尾CAPS S0/idle1/quarantined0。layer-ready是CPU消费者代理，不是attention/token完成。

| 指标 | 先前换出 | 持续驻留后的恢复 |
|---|---:|---:|
| lookup/hit/miss | 1728/1720/8 | 1728/1728/0 |
| HPM S1 walk增量 | 8 | 0 |
| IOTLB fill | 8 | 0 |
| 同窗无invalidate/跨PSCID替换 | 0/0 | 0/0 |
| observer整体周期 | 1,230,498 | 2,213,541 |
| HARDWARE段周期 | 300,171 | 297,327 |
| REFILL段周期 | 930,327 | 1,916,214 |

原始计数互证translation确实启用，零miss不是Bare旁路或没有发DMA。HPM缺席A槽 inhibited，不能把A零解释成DID11测量；composition/observer没有该场景独立背景请求。48/96批次和地址布局不同，不能把2844 HARDWARE差归为8walk因果；REFILL含真实frontend task交接与软件批次供给，不都叫CPU或翻译等待；AR/AW not-ready为0也不证明所有内部/命中固定延迟为0。

实际ordinary backing4×4MiB、8个2MiB叶共16MiB，2unique table pages。全部4096个4KiB的CPU-PA与PTE权限/别名核查通过；合法4KiB回退、8×2MiB、premap/residency/batch和authorized singleDMA均保留，未强迫小页。两phase间只有CPU cache交接，没有IOTLB flush/remap；冷状态在实际priorout中支付，不将warm恢复准备成本设0。source真实BIDI/RW，immutable是可信owner合同/全字节检查，不是硬件RO。4KiB硬件task是已核f9 DEFAULT串行前端及2AW正确性边界，不是2MiB映射改为4KiB页或人为限制child。

| 阶段 | CPU raw cycles | TIME raw ticks | kernel名义秒 | TIME条件Dell秒界 |
|---|---:|---:|---:|---:|
|种子输入/guard预检|25800105|12900096|0.516016360|0.463107–0.569951|
|分配/map/HPM/init|118556138|59278044|2.371124720|2.128050–2.619019|
|真实leaf/准备边界|5743228|2871600|0.114870160|0.103089–0.126873|
|先前换出/供给/完成|1323403|661696|0.026471560|0.023754–0.029235|
|换出后CPU全字节核验|379185356|189592670|7.583714160|6.806276–8.376572|
|恢复cache交接|3080|2201|0.000124600|0.000079–0.000097|
|恢复/供给/完成|2259481|1129738|0.045192160|0.040557–0.049914|
|最终CPU全池/余量核验|449803938|224902008|8.996089440|8.073862–9.936607|
|原始池及记录交付用户|95466545|47733290|1.909331600|1.713600–2.108949|
|PTE撤销/检查/回收|19260126|9630355|0.385231320|0.345725–0.425488|
|HPM/引用等收尾|471631|236073|0.009463120|0.008475–0.010430|
|整个ioctl（含上述阶段）|1098019706|549009556|21.960371360|19.709150–24.256306|

完整成功ioctl名义21.960371360s；实际恢复插桩阶段名义45.192160ms，TIME条件Dell界40.556956–49.914060ms；实际先前换出名义26.471560ms、条件界23.754497–29.235057ms。全字节/leaf审计、种子/guard生成、输出交付是本次研究/采证成本，全部计入，但不是原生GPU计算或生产恢复的强制执行时长。board userspace input/load31.071327800s、open/ioctl/close21.987280760s、SHA/fullmemcmp20.948132200s、raw写1.850882320s、whole before receipt76.007587960s；独立Dell整个CLI wrapper77.765375332s。stage11和user/host包含下层，不重复加总。

三次同新boot CAPS的全部pair给CPU45.267–55.711MHz、TIME22.634–27.856MHz共同固定率兼容区间（相对Dell CLOCK_MONOTONIC）。counter连续、无debug/PMU重写/复位/停钟、校准至执行频率保持是换算条件；CPU与TIME读点非原子，TIME是SBI模拟读CLINT而非独立振荡器。严格SI准确度/漂移未测，raw、kernel/board nominal、独立Dell墙钟与条件界分列，不猜精确频率/吞吐。

资源完整合同：逻辑四池9,437,184B、DMA backing16,777,216B、kernel私有种子4,718,592B、batch容量26,112B、leaf容量360,448B、run4520B；userspace另有seed4,718,592B/output9,437,184B/同量record buffers。unused logical guard5,898,240B与额外backing slack7,340,032B覆盖13,238,272B，和logical检查重叠，不能再当新增有效字节相加。两table页8192B，其中原有root/新增量未单列；allocator/pagecache/transient精确peak RAM未验实，不从容量合同猜峰值。最终按实际叶撤销→每4KiB核空→private checked boundary→IOVA/backing free，源码与原始stage9/release0相互绑定；post-revoke PTE和raw slack没有单独返回字段。既有helper在严格无ATS/无S2/已排空下提交VMA＋IOFENCE，以CQ fetch-head越过后续fence证明前序VMA pulse，不升级为通用fence执行完成证明。

工程成本分列：r0干净kernel/loader/OpenSBI59.066s、必要r1修复构建59.542s；实际原分区backup收件89.989s（精确dd完成时长未知）；r0/r1 write＋fsync＋whole hash954.607/959.966s；同f9两次volatile reload/boot捕获474.311/474.153s；r1 payload下载/hash/dryrun234.240s；r1资产下载/hash103.654369776s；r1原始tar回传UART114.838118173s，内嵌receiver28.834s。原32MiB备份仍在Dell，最终SD整分区SHAec13fd80…；只写首分区，两次安装是原接口及必要同任务修复，未写Flash/GPT/其他分区，未伪造新的hash-specific reply。原失败及串口长度/BusyBox tar特性/嵌套校验器路径三类真实适配错误全部留证；实际接受调用复用原校验器，未改其实现或复跑已验矩阵。

裁决理由：普通合法大页实际可得，付出实际prepare/priorout/full-cost后，同一映射/驻留/批量/单DMA完成全部当前字节与依赖，恢复的1728lookup全hit、0walk。该合同没有提供普通强控制外具体可达可避免翻译残余，故结束这份all-ready有限R107卡，不进G0/G1。不是全领域阴性、不作全部固定hit延迟消失或原生多GPU性能的结论。

后续路线：回阶段一，用保留的GPU/AI原文和E2/E3因果边界形成一个有新增事实的显式需求卡，给准确任务/owner/lifetime/bytes、为什么上述合法普通控制不可覆盖、共享路径与完整成本；具体残余成立才最小G0。已关闭分支不复活，不扩大强度/场景凑候选，也不自动登记后继goal。新事实未给时不开新实验。

入口：[根裁决](../out/r107-ordinary-pilot-20261012/root-result-acceptance.json)、[完整raw receipt及四池/三UAPI](../out/r107-ordinary-pilot-20261012/repair-1/live/finite-pilot-result/receipt.json)、[复用校验器实际接受](../out/r107-ordinary-pilot-20261012/repair-1/validation/actual-finite-pilot-validation.json)、[全成本/条件界](../out/r107-ordinary-pilot-20261012/repair-1/costs-and-boundaries.json)、[失败根因](../out/r107-ordinary-pilot-20261012/failure-root-cause.json)、[修复源审](../out/r107-ordinary-pilot-20261012/repair-1/validation/root-idle-repair-source-acceptance.json)、[原源/构建identity](../out/r107-ordinary-pilot-20261012/repair-1/build/artifact-manifest.json)、[当前board quiet](../out/r107-ordinary-pilot-20261012/repair-1/validation/final-board-state-acceptance.json)、[执行账本](../out/r107-ordinary-pilot-20261012/execution-ledger.json)、[后继inactive合同](../out/r107-ordinary-pilot-20261012/next-goal-contract.json)。

生产source、模型config/cache/原history及181旧主输入保留；独立Linux/RTL贡献不改变此裁决。全Codex，主/独占席按Sol/ultra；counter席只做独占源问题，root亲核关键原文/实际源和raw；无DeepSeek/Claude/Astra。原论文目标继续。

# stage117续行：C117R3的三个实际结果

> 2026-10-13 后续复核修订：本报告三项功能/正常完成接受保持。原文“普通满足/本轮投影收束”只指当时的功能任务，不能据其认定并发性能已经被覆盖或关闭C117R3性能问题。Dell CPU记录归主机功能/局部开销，非FPGA CVA6基线；后续G0直接检验待证损失，无需先证明残余再重复实验。[新关口合同](../out/stage117-plan-revision-g0-20261013/revised-gates.json)／[本轮准入](../out/stage117-plan-revision-g0-20261013/root/gate-acceptance.json)。


同一C117R3任务已完成三项有界结果：**可执行包/所有权适配、Dell直接CPU基线、真实iDMA＋启用翻译IOMMU的单DMA RTL基线**。CPU与单DMA都完整交付同一批数据；普通直接复制已满足这次有限任务，没有建立可避免的共享翻译干扰残余，因此不进入G0/G1，也不把适配或直接复制当作论文新机制。这里的“三个结果”来自同一工作流。root已亲核并接受实质结果；最终裁决见[验收](../out/stage117-three-results-20261013/root/critical-review.json)。

## 固定输入与完成含义

来源仍是已准入的DPDK vhost `vm2vm=1` 本地单播：A已有完整Ethernet帧，B预发布自己的buffer、descriptor和cookie。此次输入明确是**generated functional fixture**：三个60、65、1514B完整帧，包含Ethernet头、排除FCS，合计1639B；每包另有12B virtio header，接收record合计1675B。1514B包用503＋1011B两span，其余各一span；地址不强制64/4096B整齐，末span跨4KiB边界。三个used head ID为7、11、19，第三个head下有两个descriptor，`num_buffers=1`。[冻结输入](../out/stage117-three-results-20261013/common/fixture.json)，SHA256 `97763f1ae630a22d7231a9862967dea5df01e19741dbdcfd59c58111edc97da7`。

输入没有声称实际捕获的包分布、自然到达、共同就绪peer或压力。A/B在有限实验中表示软件生命周期；没有部署guest隔离。直接A→B一次复制是合法普通路径。producer直接放置、共享存储、页借用或重映射若能保留实际地址、邻居和生命周期合同，仍然开放；测试里“只返回A指针、B原存储未写”确实不满足原cookie消费。

## 结果1：适配与真正的接收接口

适配实际执行`Posted → Published → HandedOff → Consumed → Released`。只有全部读取/写入完成后才能发布header/used，源完成前拒绝复用；`receive()`交还三个原B cookie的实际payload view（offset＋span长度），调用者逐cookie明确归还后才能释放。observer实际读取返回view的全部帧字节；独立common consumer另按原posted descriptor链核header、used ID/长度/顺序、1675B接收record与6517B目的邻接字节。

正常交付通过；16项定向检查全部通过，覆盖提前used、源早复用、consumer归还前释放、错view地址、错cookie/长度/顺序/header、尾字节损坏、邻接覆盖以及“DONE看似成立却没复制”等。整个源区在退休前保持原样；退休后改写1639B源数据，B仍可完整消费。它是有限单线程软件所有权合同，不能阻止合同外任意CPU写入；该相邻错误由独立字节检查检出。[结果1](../out/stage117-three-results-20261013/result1-adapter.json)／[适配实现](../out/stage117-three-results-20261013/cpu/adapter_cpu.cpp)／[原始检查](../out/stage117-three-results-20261013/cpu/run.json)。

## 结果2：CPU直接基线与成本

Dell的Intel Core i9-14900K实际编译执行，GCC11.4、C++17、`-O2`。四笔直接复制全部提交、接受和完成，载荷逻辑读1639B、写1639B；header另写36B，没有host staging，也没有给CPU强加DMA cache或IOMMU操作。

最终一次有限运行的已计时生命周期为**29,830ns**：实际交付阶段合计5128ns，验证observer合计24,344ns，计时/阶段记录间隙358ns。范围包含工作区分配、输入准备、供给、复制、源退休、header/used发布、实际B view交付、caller归还与释放；逐字节/guard/source复用检查单列并保留在总成本。各phase详见原始JSON。交付phase总和不是另跑一次无checker基线，交错observer会影响cache状态；单次未绑核计时不能推出吞吐或稳定性能。

工作区为16KiB；Adapter对象160B，used容量初始24B、发布后32B；冻结fixture的frame和元数据容量共2039B；caller views占96B，供给临时span容量64B。observer明确使用16KiB touched栈、8KiB源预期栈及动态集合，计时记录容量312B；allocator开销/峰值RSS/真实物理叶大小未知。没有把host基础页大小4096B冒充实际物理映射证明。进程启动、冻结输入元数据物化、生成与编译、负例及JSON输出在该计时范围之外，外部命令时间另有账本。[结果2与全部已知/未知成本](../out/stage117-three-results-20261013/result2-cpu.json)／[编译运行账本](../out/stage117-three-results-20261013/cpu/execution-ledger.json)。

普通执行与UBSan通过。初次ASan出现持续`DEADLYSIGNAL`且无栈，被手动终止；此失败保留，原因未定。一次有界诊断关闭ASan的SIGSEGV处理器并禁用leak检测后，address＋undefined检查通过；不把初次失败写成通过。诊断脚本曾误把1MiB输出文件限制同时用于编译产物，已改为只限制运行输出，原错误记录保留。

## 结果3：真实单DMA与IOMMU RTL

使用真实现有iDMA DEFAULT精确src/dst/length路径，`N_TRANS=1`，batch、受控launcher及候选门控关闭；实际实例有两个DMA，只向一个发请求。真实IOMMU执行Sv39非等值翻译`0x10000000 → 0x80000000`，安装一片2MiB叶并在四笔间驻留，无人为flush。实际仿真初始化16KiB工作区、4KiB DDT和8KiB页表；2MiB是合法模拟地址覆盖范围，真实物理大页的分配/保留并未完成或计时。

四笔NEXT ID 2—5、frontend/backend握手、hardware DONE及寄存器完成均对应。载荷有6组AW/B，209个R beat＝1672B总线读取，211个W beat＝1688B总线lane字节，其中WSTRB有效写恰好1639B；非对齐总线额外字节不能当有用帧。实际DDR快照与trace独立核查完整覆盖，源保留、quiet后发布used、源复用、原B消费及最终quiet/release全部通过。frame digest与CPU一致：`16233906472448960885`。

配置至quiet为**1227 RTL周期**；具体条件是`ptw_latency=1`、wrapper的DS固定延迟50/50及DDR行为内存，无真实CPU/cache。12次IOTLB lookup出现1次初始miss和11次hit。host仿真至memory teardown为30,352,817ns，含模型求值、观测和快照等；其每字节consumer检查为10,946ns。header/consumer发生在host，不能说只占0个硬件周期或已包含于1227周期。CPU墙钟时间、模型运行墙钟时间和RTL周期各自保留，不计算相互速度比。[结果3完整交付及限制](../out/stage117-three-results-20261013/dma/results/delivery.json)／[原始run](../out/stage117-three-results-20261013/dma/results/run.json)／[独立快照核验](../out/stage117-three-results-20261013/dma/results/artifact-verification.json)。

本次没有FPGA上板、真实Linux pin/map/unmap、CPU cache clean/invalidate、native DPDK/QEMU或错误/超时恢复部署。仿真只验证正常成功；沿用的Verilator配置关闭RTL原生assertion，C++协议/字节/守恒检查有效，构建warning保留。quiet后清空行为内存闭合有限仿真生命周期，不能替代真实硬件缓存与映射撤销成本。

## 本轮路线决定

三个结果补上了原先“只有来源、适配尚未实现”的缺口，给出可复用的完整包、原cookie消费和ordinary完成核验。因为同一有限输入已被CPU及驻留大页单DMA完整满足，而且没有自然peer或普通控制之外的完成损失，本轮不能据初次翻译miss提出新干扰机制，也不靠扩压力/小页/多DMA矩阵制造缺口。

本投影的这一轮普通判别收束；保留适配和证据。下一具体关口是同源工作流确有独立就绪的有用包任务，并取得同平台CPU/单DMA连同映射、缓存、consumer和排空的完整可比成本；若其中出现普通方法之外、可达且可避免的翻译损失，才按来源合同进入G0。当前不自动开新选源或部署goal，论文主问题仍开放、论文未完成。用户指定的Astra/ultra执行两项独占任务，root亲核接受；无DeepSeek或Claude。生产driver/RTL/ABI未改，隔离代码和构建产物均在本轮out目录，旧阶段与独立贡献保留。

本轮实际goal已complete；[稳定接受](../out/stage117-three-results-20261013/root-result-acceptance.json)／[完成回执](../out/stage117-three-results-20261013/root/goal-completion.json)。

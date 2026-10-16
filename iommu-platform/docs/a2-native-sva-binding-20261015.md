# 真实SVA软件链与发布所有权绑定

2026年10月15日。**已找到与DATE2025直接关联、实际启用IOVA零拷贝的公开HeroSDK源码链，替换了此前“只取得旧SDK／关闭SVM”的来源缺口；但准确论文实验版本和完整任务生命周期仍未闭合。本轮保留A，交付具体方法增量、接口绑定和最小真实SVA适配方案，未进入时间验证。** 原四案八次源语义结果照常继承，不重跑、不升级为周期结果；两周窗口仍为10月15—28日。

## 已定位的实际软件版本

关键入口是 [hero-tools固定版本47339e0](https://github.com/pulp-platform/hero-tools/tree/47339e040c43ab4d44d9a870bc307251084a1ba4)，`carfield/main`，作者2025-02-03提交。它的[platforms.mk:113–115](https://github.com/pulp-platform/hero-tools/blob/47339e040c43ab4d44d9a870bc307251084a1ba4/platforms/platforms.mk#L113)明确clone论文的`date_iommu_evaluation`，包含FP32 AXPY及其Linux驱动、库、target和双缓冲DMA；不再以旧HEROv2关闭SVM代替这条来源。

SDK gitlink为`fb56489c97b41471b6b9d707f0a38b3bc789a76e`，LLVM runtime为`d8cffb8d7294483b45732283719e250116fbe6c0`。SDK的`_iommu`配置指作者Linux分支，已固定取得[Linux e4b0725](https://github.com/CyrilKoe/linux/tree/e4b0725a5089f6cc0fc5afec06f3091d98dffa68)，Makefile确为6.9.0。然而SDK普通配置和hero.mk仍用6.1.22，IOMMU配置指可变分支；缺论文实际选用的配置、构建命令及commit日志。因此接受“公开enabled实现谱系已定位”，不接受“准确论文实验快照已复现”。[完整引用链、26份关键原件、URL与哈希](../out/a2-native-sva-binding-20261015/sources/date-software.json)／[定向调研报告](../out/a2-native-sva-binding-20261015/sources/date-software.md)。

已核链为：

```text
AXPY do_map=1 → libhero hero_iommu_map_virt → IOCTL_IOMMU_MAP
→ pin_user_pages + independent iommu_map → scalar IOVA OpenMP target
→ fixed LLVM mailbox runtime → Snitch core8 publisher → async DMA
→ target DONE → host consumer → free
```

其中驱动用`0x80000000+(UVA mod 0x40000000)`分配IOVA，并另建LLC bypass别名。它是显式pin和独立页表的零拷贝；不能写成同UVA或直接共享`mm->pgd`。`device(1)`虽名MEMCPY，但这里只复制IOVA/alpha/n标量，bulk数据仍由DMA走IOMMU；不能据名称判SVA关闭。论文§III-B及§IV-A的映射/供给与应用开销仍有效，§IV-B kernel范围不能当完整任务范围。

## 完整任务被哪三项具体义务挡住

| 边界 | 决定性源码 | 本轮判断与需要的修正 |
| --- | --- | --- |
| 最后输出与target DONE | `axpy_snitch.c:131–146`发末输出后只有barrier/打印；固定runtime `dma_wait_all`是显式busy等待；LLVM `rtl.cpp:493–509`仅等DEVICE_DONE | 没有末DMA完成前置DONE的协议保证。补最后真实wait，并核R/W/B及target/host同步。打印可能覆盖尾部不能作保证 |
| 独立consumer与热跑恢复 | `axpy_main.c:96–98,125–154`在do_map时让`y_dev_virt=y_host`，恢复自拷、host原地计算后比较同一数组 | 补不可变原输入、独立reference及热跑恢复；实际输出可见后比较全部字节。不能把现有比较通过当数据正确 |
| 解除映射与pin所有权 | `hero_iommu.c:35–59`建map但记录list全注释；`carfield_fops.c:82–85`release为空；应用165–172直接free | 补map返回值/partial rollback、可追踪buffer owner、unmap→实际失效完成→unpin→free。模块卸载不替代每buffer生命周期 |

root直接核应用、driver、LLVM等待、当前Linux及关键RTL；[原文摘录与哈希](../out/a2-native-sva-binding-20261015/root/source-evidence.json)。以上只裁决取得的公开版本，不据此否定论文其他版本的测量或SVA方向。新AXPY每tile512 floats=2048B、四个L1槽共8KiB；论文相符的32768输入候选产生192个descriptor，但未运行，也未把它们接成当前平台的多lane任务。

## 当前平台真正能发布什么

[接口和owner证书](../out/a2-native-sva-binding-20261015/interface/publisher-owner.md)绑定了41份当前源文件和96段原文，分别列出默认源码、条件profile和既有夹具。

- 当前板**源码默认**NonblockMode=0：四DMA各24B数据缓冲、单TC；不是旧夹具两DMA各4KiB/banked，更不是当前已加载板身份。
- 每lane normal前端从COUNT到backend response一直WAIT，所以最多一个未退休job；JobFIFO=8不是8个可独立发布的任务。源端可选位置在NEXT发布前；需要两个async先返回时只能新增有界software pending或阻塞submit，须计费用与槽持有。
- 普通2048B CONF=0合法，但无FullBuffer额外许可；64B HOLD、4KiB batch ADMIT不泛化。首次offer、FIFO接受、backend接受、raw/target WLAST、B和DONE分别记账。
- DID10..13与固定PID20..23、PV、完整AXIID均绑定。标准Linux SVA要求PCI PASID/IOPF，当前platform iDMA未启用；不能忽略失败返回或强行设pasid_enabled。
- 已核pv0/非特权/PDTV0的DC.FSC direct Sv39用户页路径；须满足U/R/A及写W/D，reserved位正确。pin不冻结UVA或A/D，CPU aging和退出/失效需另处理。

这些绑定使A新增几项具体判别：不能从同lane两个source async调用推两个硬件pending；不能从调用无全序推DDR双头；不能从DONE推成功/consumer；不能从默认缓冲和旧夹具选出拼接配置。[方法增量与规则](../out/a2-native-sva-binding-20261015/design/method-increment.md)已落实输入身份、发布转换、owner释放谓词及准入。通用时序后端仍未实现。

## 保留核心思想的具体重设计

下一版本优先实现**单mm、固定pinned用户页、non-PASID、DC.FSC借用同一进程Sv39页表**的受限真实SVA；CPU数值地址直接进入DMA，DID10/11独占。它不提供缺页重试/ATS/跨进程并发或generic PASID SVA。具名普通对照为相同UVA的独立shadow页表及DATE式IOVA零拷贝，不能把更强命名当收益。

[可执行适配方案](../out/a2-native-sva-binding-20261015/design/minimal-sva-adaptation.md)给出BEGIN/REGISTER/SUBMIT/WAIT/CONSUMED/END语义、修改位置和失败处理；[详细软件设计](../out/a2-native-sva-binding-20261015/platform/software-binding.md)列出实际Linux API。必要修改是domain transition摘旧链/rollback、borrowed root不得被free、mm/pin/notifier/A-D与异常排空、可回收PSCID、独占两DID和完整错误费用。没有这些修改，不能把只改DC.FSC称已启用SVA。

优先复用stage117已验证的本CVA6 FENCE.I整I/Dcache交接，root已核Linux指令→commit/controller→WB/MSHR排空→flush确认的静态连接；新增ABI仍需验单hart和实际加载身份，完整计清空/重填/metadata/consumer，258旧模型周期不搬为新成本。启动禁Dcache只是更重的备用方案。

另提出并静态核通一个不增加私有翻译反馈的context完成协议：全链quiescent/noATS时，独占CQ提交两DID精确IODIR、host VMA、8B AV marker、后继noAV fence；新token加模ring的**后继fetch**进位证明前marker B成功与先前匹配cache失效commit。它不承担DMA排空、不证明sentinel自身完成或无late FQ错误。[精确条件与6个反例](../out/a2-native-sva-binding-20261015/interface/publisher-owner.md#7-root新增-quiescent-context-barrier有界反例核验)。该协议未实现/运行，只是本轮亲自推演后保留的最小可实现方案。

唯一native功能输入采用stage117三包/四span形状：8页arena，60/65/503/1011B四次普通copy，总1639B payload，必要文件consumer交付1675B。先用公开DONE全局串行，再独立核双lane进展，不改当前24B容量、不加许可。实际文件输入及确定预期已生成：[二进制](../out/a2-native-sva-binding-20261015/design/three-frames.bin)／[完整地址、依赖与资源合同](../out/a2-native-sva-binding-20261015/design/native-task-input.json)，SHA256 `914ec5d1fe933aca831497dcdd4a8599ede1e22053303cf9453832b5c3f0f12f`。adapter尚未实现，该输入不是已执行任务，更不是时间正例。

普通当前用途优先、公开DONE节流/串行、原buffer保留、CPU组包/直接writev及适用DS-first/F4均获得同信息/资源的机会；CPU无需承担DMA专用flush。当前平台没有Snitch/TCDM，CPU AXPY加DDR staging可被原buffer直接计算删掉，因此不把这种移植作为A正例。

## 新意和时间验证的接受范围

MASK已联合PTE/data服务（PDF7–8 §§5.2–5.4），DWS已给非抢占steal的walker局部界（PDF5／印刷630 §V）；TC2023能用任意job内请求时序给有条件保守界，ICRT/AXI-REALM已覆盖可选队列、组合服务、外置调节和AW/W成本。沿用[A1的实质原文比较](../out/control-position-method-20261015/literature/analysis-delta.md)，不重新主张优先翻译、DAG、carry-in或SVA接口本身新颖。

**条件novelty论证：** A拟从真实调用、地址/context、前端并行度、first-offer及owner闭包抽取可部署控制集合，再预测普通入口足够、需前移或旧义务不可改时的双方/consumer完整结果。A2已经产出实际会改变这些结论的绑定和最小公开接口方案。如果后续冻结未用任务中能预测出有意义的部署或完整费用差异，就可形成共享翻译部署方法推进；目前只接受源码/静态规则和重设计，独立贡献及时间预测仍待证。补driver缺陷本身不算研究新意，也不因已有数学工具便关闭A。

[完整验证合同](../out/a2-native-sva-binding-20261015/validation/full-task-contract.json)将四个时间案例全部标为未准入：**本轮没有伪造正例、反例或两个时间留出。** 缺准确构建/生命周期、当前native任务、自然fresh-head选择、有限写/B服务及真实host/cache/consumer费用。现有行为DDR写/B使用无显式容量deque，旧FIFO L和CPU proxy不能补这些输入。源序号不转周期、拍数不转MIG债务，未知费用不填零。

从共同任务ready计输入供给、准备/map、publish/poll、全部R/W/B、cache/consumer、槽归还、失效/unpin/free与最终quiet；输出双方、consumer、release和quiet向量。只有源码/接口/服务闭合并有具名普通差异，才事前冻结一正一邻反及两时间留出、区间/误差与失败规则，一次执行这组有界验证；不能改压力/phase、cache初态或任意时延找胜例。[下一实施合同](../out/a2-native-sva-binding-20261015/next-decision-contract.json)。

## 实际执行与状态

本轮只运行新输入生成和交付检查，0新源语义episode、0时间模型/RTL/FPGA运行或构建、0板操作、0生产RTL/Linux/driver/ABI修改。固定输入生成2次（修改manifest后重生成，字节相同）；没有将其当DMA验证。公开原件、当前关键源hash和原暂停回执检查分别留存。

Codex-only：root承担实质设计、关键源码和最终接受；三个Sol/ultra独占席分别核DATE谱系、当前软件、publisher/owner及后继marker反例，没有DeepSeek/Claude。原MIG goal1792023314保持paused，build6 WNS−0.391ns/598setup失败、0新采集、实际债务未知；原ACV-2x2、旧R2/RCS1及八次源语义不重开。研究和论文未完成，本轮完成的是A2来源绑定与有依据的可执行重设计交付。

[稳定接受结果](../out/a2-native-sva-binding-20261015/root-result-acceptance.json)／[导航](../out/a2-native-sva-binding-20261015/README.md)／[交付核验](../out/a2-native-sva-binding-20261015/root/delivery-verification.json)。

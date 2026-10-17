# Root A 板测与代码线：2026-10-17 JST

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17 JST
- Verification Status: UNVERIFIED（尚无本轮新增板测）
- Version Label: root-a-status-v1

## 今日进展

- 已创建独立 worktree `/home/xvhaocheng/work-root-a`，分支 `ispass/root-a`，起点 main `01f981a9c603fa909fdb2fb22450aa7c5197271f`。未切主目录分支，未改共享汇总文件。
- 已分派 A1 接续 DS25/DS100 构建与 E1b 运行包、A2 准备 E3、A3 核查 E2/E5/E4/E6。子席按用户指定的 gpt-6.1-sol / ultra 启动；主会话保持当前模型，不切配置，遵守共享 IOMMU Codex-only；子 agent 不操作板。
- 已审核并将三席的第一批准备分支合入 `ispass/root-a`：E1b 构建/采集/任务内长表，E3 原始能力证据与 P0 方案，E6 DS0 角色互换采集器。Root 工作树的17项调度/采集接口、6项独立长表、15项历史分析拒绝检查、14项角色采集协议检查实际通过；均为主机检查，不记为新板测。
- 已发现并接续原有两个运行中的 DS 构建，输出在主工作区 `iommu-platform/out/ispass2027-20261017-ds25-01`、`...-ds100-01`；不重复构建、不改变其输入。现已验收通过：WNS均+0.177ns，WHS为+0.048/+0.045ns，WPWS均+0.062ns，setup/hold/pulse失败端点与routing errors均0，CPU 50MHz；delayer单元120/145。原始报告与binary清单已合入任务分支。
- 已建立[唯一板测队列](../board/queue.md)。UART 身份回执确认板仍为 DS50/pathv3，boot `b1eaa85f-01d7-4976-9dac-b57cc8af49b7`、taint=0，四 DMA 节点齐全；该初始回执仅代表当时身份；随后发现独立DS会话在12:53:13Z已加载DS25启动旧pathv3。用户已授权联系该会话，由Root统一排程并接续其两档100次结果。本worktree12:56:37Z重复启动被UART锁在触硬件前拒绝，失败原样保留，不重启替代；任何时刻仍仅一个实际板任务。
- 已整理历史统一100次的论文可用 PATH 数据：[逐试次阶段CSV](../exp/E5-latency/baseline100/trial-stages.csv)共1600行、160组，每组10试次。Root 从原始records重算，JSON和CSV完全一致；保留 count/sum/max、boot/sequence、配置和原始命令入口，不生成请求分位数。DS0 A8 的 V读stage1跨试次均值中位数为142.504517周期，最大观察值795周期；这是阶段统计，不是新板测或请求p99。

## 关键数字

- 本轮计划 E1b：两档 × 五条件 × 十次 = 100 次新增板测。
- E3 优先小子集：三种 V 大小 × 三种 A 条件 × 五次 = 45 次；尚未测量。
- 历史基线每点平均采集开销约 15.7 秒，仅作队列预算；不作新硬件性能结果。

## 阻塞与决定项

E3 已直接核实真实 pathv3 batch 在 CLI、driver、RTL 均固定 4096B，并且 V/A 共用 chunk；全局实验锁也不允许通过两个进程拼接。A2 的 `exp/E3-workloads/p0-change-option.md` 给出只覆盖45点的 packed-ring、新独立长度操作、真实字节验证和 batch 地址推进方案。用户已明确回复“批准仅P0最小改动、构建和45点板测”。Root 接受该限定方案，已指派 A2 分小批实施和验证；完整三类负载、其他 RTL 扩展未获此次扩围。DS25/100 仍先使用已写 pathv3 payload，新版本身份和结果另存。

E2 当前只有两条流、一个 `aggressor_id`，三至四路需软件接口扩展；E4 已确认实验driver逐4KiB建图，2MiB需显式软件映射及实际leaf验证，[请求](../exp/E4-pagesize/change-request.md)保持未实施。E5 baseline v3 仅导出 sum/count/max 等聚合，不导出逐请求样本或直方图，只能复用运行中已有的 mean/max。E6 角色互换的动态角色 ARM 与 PATH mux 已直接核读，现有 BIT/driver 可以执行，独立 collector 已通过16项主机协议检查。E4/E5 需要的内核、driver 或 RTL 变化及 E6 缓解构建仍只提交 change-request，等待用户决定。

E2 实际冻结硬件 `ariane_soc::RVIOMMUCfg.NumHpmCounters=31`，6槽是软件会话限制。Root 已接受先做有界 multi HPM 软件接口：四个DID×三event共12槽，保留旧pair/Solo六槽及安全恢复语义；不改 RTL。多干扰者驱动批次须在 P0 驱动接口稳定后审查整合，V精确硬件窗口保留，未被PATH选中的设备不得冒充已有逐设备PATH观测。

P0 前端三处RTL变更已直接审查，79项有限真实frontend验证通过；独立DS0新BIT正在构建。P0驱动共用旧mapping/cache/排空路径，38项实际函数模型检查和RISC-V对象编译通过；已与通过68场景迭代及实际对象编译的multi HPM源码合入内部Linux稳定提交 `f81c82c24a8c6833c14a1d5c6fa315a8b26e1345`。新payload等待multi driver整合后统一生成，不提前写卡。上述检查不等于新板测或cache/RTL事件性能证明。

新P0/E2/E6采集器的共享计数采用[同窗PATH stage0完成数](../board/observer-shared-counts.md)，保留私有入口和各stage的独立守恒，避免把mux spill库存边界误判为DMA错误。Root对既有100试次实际回放，200条读/写共享等式全部匹配；新几何仍不假定私有/共享残差恒为0。原E1b采集器当前不变，若实际触发解析问题保留完整raw后处理，不重测替换。

软件配置集中排程已明确：旧pathv3先完成E1b两档和旧DS0上的角色互换，然后只更新一次共同新payload，继续原DS0 BIT的E2及P0新DS0 BIT的小子集。这样角色对照保留与历史baseline相同Linux，不回写旧镜像。P0以外的三形态、页大小、直方图和缓解扩展均不随编译许可顺带实施。

## 待合并内容

最终仅推送任务分支。共享结果表的新增行由实验目录 CSV 提供，README/CURRENT_STATUS 更新建议在最终交接中列明；本会话不直接编辑这些共享文件，也不自行合 main。

用户已明确允许实验途中持续同步**已采集、论文可用的数据**；逐试次数据、任务内统一CSV和阶段统计在有真实样本后分批提交，明确n及未完成条件，不等整轮结束。日志保留作追溯，不以推送日志冒充同步论文数据。准备分支已推送 `origin/ispass/root-a`；BIT/payload 留在本机。当前唯一活动DS campaign在主工作区`board/runs/ds-sweep-20261017-01/`，Root不改其活动文件，只在稳定边界冻结本任务CSV；原会话两档结束后保持DS100并交还板卡。

## 后续授权与编译进展（2026-10-17 13:03Z）

用户已补充授权未来清单中的必要RTL/Linux改动由agent在独立branch实现、编译，随后统一排队上板。原E4/E5及其余负载的只读请求状态据此转为准备：E4交A3、E5交A1、E3其余形态交A2，均先完成当前关键交付，再给出有界设计和实际代码审查；E6历史缓解最后准备，不恢复新机制探索。先前等待决定的段落保留为当时状态，以本更新为准。

Root已直接审查E2 multi最终驱动/ABI/CLI、未知静默状态不标drained与STOP前in-flight修正，实际源码hash吻合最终有限测试：旧pair/P0 38场景有序MMIO trace与稳定M完全相同，新multi57有限场景和45CLI场景通过，真实RV64 idma.o/iommu-hpm.o编译通过，multi栈1312B。Linux稳定提交`acece3a3ab7d72ba1f936c1984b82a4a55f28d46`已被接受，平台`52c5fdd028bbf3b07deaf1d0f7e13e3a271b29d0`固定其gitlink和lock；A2已开始一次共同payload编译，含原三程序、P0和多设备CLI，release后缀`-ispass2027-p0-multi-v1`。主机验证不代替板测。

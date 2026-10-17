# Linux 既有历史与当前源码增量整理（2026-10-17）

已保留原 Linux 提交链，把当前 **8 个 tracked dirty 文件与 6 个未跟踪源文件**拆成 9 个实际新提交。没有重写整个内核历史，没有 checkout 全内核，没有执行构建、主机测试、RTL 仿真、实验或板测。原 HEAD、旧分支/tag、索引、14 份原件和 remote 保持原样。

新增提交使用本次实际日期与当前 Git 身份 `xvhaocheng <xvhaocheng@gmail.com>`；共享 IOMMU 本次工作为 Codex-only。临时副本和独立索引位于 `iommu-platform/out/source-organization-20261017/worktrees/linux/`，原 Linux 工作树没有切换、清理或 stash。

## 固定身份与边界

| 角色 | 提交 | 说明 |
| --- | --- | --- |
| 驱动来源 | `d371fb0f95d5bae20af8135f976ffd8a06bfaa23` | `malejo97/linux` 的 `wip/riscv-iommu`，是当前 HEAD 的既有祖先；本次只读查询仍指向同一 SHA，见 [原输出](upstream-ls-remote.txt) |
| 原固定软件 L0 | `c0f5e13ea81fa30d78f3bb1d6389e2397321e040` | 当前已提交软件；已包含 HPM、DMA 实验、batch/path/E3/P4M/research2，**不是 stock Linux，也不是最小普通 DMA 配置** |
| 普通修正候选累积 | `1ae4817d975a49127616361ffba9aa01fbef3d67` | 仅在 L0 上拆入页表释放、endpoint-list 初始化和保守 CQ tail 不一致拒绝；没有新增研究接口，仍继承 L0 的旧研究驱动，未运行验证 |
| 当前完整源码 | `32b96fe373e56611c9eee787580c1e475d7b52e9` | 与本次冻结的实际源码完全对应，包含原来 6 个未跟踪源文件 |

最终完整内核 tree：`08b19b0475fb7dc4f18573f1da40c317302e6e3c`。完整树身份由原 L0 tree 加逐字节、模式和路径完全相同的 14 文件增量取得；不需要为整理读取或复制所有未变内核文件。冻结入口是 [frozen-input.json](frozen-input.json) 与 [frozen-delta.tar.gz](frozen-delta.tar.gz)。

本轮“原版性能基线”的来源固定工作在 RTL 侧；Linux 这里只作为另行明确的固定软件依赖，没有原生性能验收。关闭研究开关也不能把本链叫作 stock：LDOM 修改了普通 attach/free、标识符分配、PASID 上限和生命周期行为。

## 九层实际新提交

| ID | 新提交 | 实际内容 |
| --- | --- | --- |
| LF1 | `4bd68b4` | `io_pgtable.c` 的最低层表页递归释放，以及 entry0 为空时仍释放已拥有的 child 页 |
| LF2 | `ae019f0` | 修正 endpoint 重复初始化 `regions`，正确初始化 `domains` 链表节点 |
| LF3 | `1ae4817` | 仅提取既有 dirty 中 `tail != last` 时返回 false 的保守处理；不带 `cq_posted`、ownership 或研究字段。它是**未验证修正候选，位于最小适配基线之外** |
| LCQ | `33cb80f` | 研究 CQ 独占、posted/overflow/epoch/poison、marker/token 证书、post/IRQ/remove/init 钩子。IRQ 直接 CSR 写入也属于本层，未当作普通适配 |
| LDOM | `1f528f1` | 研究域的 private PSCID/GSCID、与 PASID 分池、borrowed-mm/root、attach/detach/free 与经证书约束的回收；保留其对普通通路的影响，未伪装为 stock 修复 |
| LSVA | `3a41cb0` | 私有、同步、特权、固定任务 SVA/shadow/IOVA UAPI 与 iDMA 入口、资源保留和设备移除锁；不是通用 Linux SVA，也未运行 |
| LCOMP_ABI | `29efb4e` | 独立声明 S/N/P/PN composition ioctl UAPI |
| LCOMP | `b28ad3a` | composition 配置、捕获、恢复、session2 参数与调用方适配；可选阶段日志明确随研究实现进入，不属于普通性能基座 |
| LNET | `32b96fe` | 独立保留 LowRISC Ethernet 源码与 Kconfig/Makefile 集成，完成冻结终点 |

LCQ 先提供自洽的 certificate/supported/barrier 接口，没有 borrowed-root、私有 ID、domain mutex 或 domain alloc/release 声明；LDOM 再补域接口与生命周期。LSVA 时还没有 composition 控制。这些边界已经静态检查；新中间版本的编译、并发正确性及实际运行没有验证。

表格是 Git 堆叠顺序。[manifest.json](manifest.json) 与 [steps.json](steps.json) 分别保存 `parent` / `stack_prerequisites` 和 `semantic_requires`。例如 lowrisc 与 SVA 没有功能依赖，composition 也不需要先启用 SVA；它们仍按实际线性补丁链交付。没有声明可以任意挑选、重排或组合后直接运行。

## 旧历史继续复用

[existing-history-index.json](existing-history-index.json) 索引了 28 个既有提交的原 SHA、作者/日期、tree、父节点、改动路径和是否为 L0 祖先。已有适配、缓存/IOVA 修复、被动观察、batch、path、E3、P4M、E2、research2 等功能提交保持原样；索引不改变旧实验的来源与结论。

两个原有独立贡献也保留：IOVA 修复 `8e3a7406fadb629d8e6b715fe83079046926de3c` 与 HPM 宏修复 `77805904b3c039e0c4f69a06552c9f0ced5a89ec`。它们都不在 L0 祖先链上；当前生产 IOVA 等价修改来自 `3b47ab1645e4`，该提交还包含 CVA6 DMA 缓存适配。当前 HPM 宏修改不在 dirty 中，所以本轮没有把它偷偷合入、也没有制造一个空的“宏修复”新提交。

LowRISC 源文件的既有版权归属和硬件说明原样保留。本次只核其现有源码与平台入口；没有另做外部版本逐字比较，不能把新整理提交的作者当成原驱动作者。

## 补丁、精确恢复与验收

- [manifest.json](manifest.json)：固定身份、9 层新 SHA/tree、两类依赖、来源、恢复前提与限制。
- [critical-source-evidence.json](critical-source-evidence.json)：决定拆分边界的路径、行号和原文片段。
- [series](series)、[patches/](patches/)：9 份标准补丁；每一步已在独立索引实际应用，tree 与对应新提交逐项相同。
- [linux-delta-series.bundle](linux-delta-series.bundle)：仅新增量；恢复前提是拥有原 L0 `c0f5e13...` 的仓库，绝不包含整部 Linux 历史。
- [reconstruction-verification.json](reconstruction-verification.json)：实际静态验收回执；[bundle-verification.txt](bundle-verification.txt) 是 Git 原始 bundle 验证输出。
- [original-working.patch](original-working.patch)、[original-staged.patch](original-staged.patch)：原 8 个 tracked 文件的工作差异与原 staged 差异。

在**另一个已经拥有 L0 的恢复仓库**中，可以恢复精确的新提交：

```sh
git fetch /absolute/path/linux-delta-series.bundle \
  refs/heads/codex/organize-20261017-linux-research-current:refs/heads/recovered-linux-current
git rev-parse recovered-linux-current
git rev-parse 'recovered-linux-current^{tree}'
```

期望分别为上述当前完整源码 SHA 与完整内核 tree。补丁重新创建提交时，元数据可能不同；只有 bundle 路径承诺恢复精确 SHA。

本次还实际向新建 bare 对象目录解包 bundle：解包时临时只读使用原对象库满足 prerequisite/thin-pack 基对象；**移除 alternates 后**，逐一验证 9 个新 commit、本地 tree 对象及全部 14 个增量 blob 都仍能读取且哈希一致。这没有复制完整 Linux 历史，也不冒称已从空目录恢复未包含的旧内核源码。完整恢复仍需要上面的 L0 prerequisite。

[organize.py](organize.py) 与 [verify.py](verify.py) 是本机整理/静态验收的记录，含绝对路径及防覆盖断言，不是构建或实验入口。构造过程中一个只读匹配断言提前停止，已保留此前生成的两项新引用，随后用新临时索引继续；没有重写已有提交或原索引。

最终已实际核验：9 步 patch/tree、14 原件及新增 blob、完整树身份、bundle 新对象恢复、原 HEAD/旧 refs/tag/索引/status/remote 保全。没有运行任何测试、构建、仿真、性能实验或板测；没有向上游或顶层远端推送，也没有改写历史实验身份。

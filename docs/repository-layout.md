# 仓库、代码与产物地图

本页回答“东西在哪里、该读哪一层”。研究安排见[当前状态](../CURRENT_STATUS.md)与[唯一总计划](../研究计划.md)，研究结果见[成果索引](research-results.md)。目录名、测试名或构建成功本身都不代表机制成立。

## 工作区结构

| 路径 | 角色与阅读方式 |
| --- | --- |
| [iommu-platform](../iommu-platform/README.md) | 当前开发与实验平台：Genesys 2 / CVA6 / 四 iDMA / RISC-V IOMMU / Linux。代码修改从这里进入 |
| [根研究导航](../研究导航.md)、[docs](./) | 当前入口、整理后的结果与历史索引；HANDOFF 保留阶段过程 |
| [references](../references/README.md) | 文献与阅读目录；原文可在不同证据轮次中，沿目录定位，不凭文件名推核读状态 |
| [tools](../tools/) | 工作区工具；[github_sync.py](../tools/github_sync.py)负责同步筛选、大文件分片/还原，[对应测试](../tools/test_github_sync.py)验证工具行为 |
| [tools/claude-advisor](../tools/claude-advisor/README.md) | 独立工具资产。存在不代表本研究允许调用，调用遵守当前模型规则 |
| [platform-archives](../platform-archives/) | 旧平台、恢复验证、历史 bundle 与退役代码；保持原件，默认不开发、不当最新状态 |
| `.vscode/`、工作区配置 | 编辑器导航和目录隐藏配置；隐藏不等于删除仓库 |
| `codex-*-verification-*` 等根目录记录 | 模型/上下文/运行核验资产，按具体任务入口查，不能把请求配置当实际运行证明 |

旧工程 `/home/xvhaocheng/riscv-iommu-demo-refact` 与学位论文 `/home/xvhaocheng/thesis` 在本机另处；它们是历史与背景入口，不是当前开发根。工作区保留原有 22 个内部 Git 仓库及组件历史；本轮地图不裁剪组件、不合并历史、不改变拓扑。顶层公开仓库汇总实际工作文件；内部组件提交、父级指针与来源锁是另一层关系，详见[平台版本说明](../iommu-platform/README.md#git-与版本)及[工作规则](../AGENTS.md)。

## 平台各层

下表路径均可从平台根定位；只进入与你的决定问题相关的一层。

| 层 | 实际入口 | 能找到的证据／边界 |
| --- | --- | --- |
| 平台编排 | [Makefile](../iommu-platform/Makefile) → [scripts/platform.py](../iommu-platform/scripts/platform.py)；[configs](../iommu-platform/configs/) | PROFILE、构建与导出流程；[sources.lock.json](../iommu-platform/sources.lock.json)固定来源/提交。`make check` 会写 manifests，并非纯读取 |
| CVA6 / FPGA 集成 | [ariane_peripherals_xilinx.sv](../iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv)、[ariane_xilinx.sv](../iommu-platform/cva6/corev_apu/fpga/src/ariane_xilinx.sv) | 四 DMA、IOMMU、互连、观测与控制接线；实际参数须回生产实例 |
| IOMMU | [rv_iommu_top.sv](../iommu-platform/cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv)、[组件 README](../iommu-platform/cva6/corev_apu/riscv-iommu/README.md) | 事务控制、翻译、IOTLB/PTW、DS 与软件接口。属于 CVA6 内部独立组件，不是根目录另一个生产实现 |
| iDMA | [组件 README](../iommu-platform/cva6/corev_apu/iDMA/README.md)、[src](../iommu-platform/cva6/corev_apu/iDMA/src/) | frontend/backend、legalizer、读写与完成；源码由 CVA6 跟踪，不能只看外层名义接口推可达能力 |
| Linux 实验驱动 / ABI | [idma.c](../iommu-platform/linux/drivers/misc/idma.c)、[idma.h](../iommu-platform/linux/include/uapi/linux/idma.h) | 启停、批次/配对、实验 ioctl、供给、轮询、排空与数据核验；驱动返回完成不自动等于 cache/free/撤权 |
| Linux 正常 DMA / IOMMU | [dma-iommu.c](../iommu-platform/linux/drivers/iommu/dma-iommu.c)、[RISC-V iommu.c](../iommu-platform/linux/drivers/iommu/riscv/iommu.c) | map/unmap、IOVA、gather/CQ 与失效范围；只定向读调用链，不默认扫描整个 Linux |
| 启动固件 | [OpenSBI](../iommu-platform/opensbi/README.md) | 平台启动与 timer 相关历史；独立来源与版本由 lock 定位 |
| 用户态实验 | [bench](../iommu-platform/bench/)：通用 [idma_experiment.c](../iommu-platform/bench/idma_experiment.c)、[E3 controlled](../iommu-platform/bench/idma_e3_controlled.c)、[composition](../iommu-platform/bench/idma_composition.c)、[SVA task](../iommu-platform/bench/idma_sva_task.c) | 参数、协议字段、数据/错误输出；不同程序对应不同测量分母，不互换合同 |
| 测试与模型 | [tests](../iommu-platform/tests/)、[nonblock_actual_dma_test.cpp](../iommu-platform/tests/nonblock_actual_dma_test.cpp)、[system top](../iommu-platform/tests/nonblock_actual_dma_system_top.sv) | 主机契约、生产模块回归、行为 DDR 与有限系统模型、板端 smoke；层级须看对应报告，不能一概称原生 DMA 或板测 |
| 脚本 | [scripts](../iommu-platform/scripts/)、[composition_plan.py](../iommu-platform/scripts/composition_plan.py)、[analyze_full_batch_g0.py](../iommu-platform/scripts/analyze_full_batch_g0.py) | 构建、打包、恢复、采集、分析。分析器也可能写文件，不能在历史产物目录随手重跑 |
| 文档与补丁来历 | [docs](../iommu-platform/docs/)、[validation](../iommu-platform/docs/validation.md)、[patches](../iommu-platform/docs/patches.md)、[provenance](../iommu-platform/provenance/) | 协议、报告与已纳入提交的迁移补丁；provenance 是审查副本，不要重复应用 |
| 基础镜像 | [assets/README](../iommu-platform/assets/README.md) | 固定 rootfs 基础与来源限制；不是全用户空间已从源重建 |

原版、必要适配、独立修复与研究扩展的固定提交见[源码组织](../iommu-platform/provenance/source-organization-20261017/README.md)，历史运行身份见[对应索引](../iommu-platform/provenance/source-organization-20261017/history/README.md)。新增引用与恢复包保留原组件历史；五个源码profile仅供静态导出，不是已构建／已运行的Make配置。

内部活动分支、实际组件HEAD与恢复方式见[内部仓库整合](../iommu-platform/provenance/internal-repository-integration-20261017/README.md)。代码先在内部Git提交并同步父子指针／lock，再同步work文件；公开恢复包保留内部历史，并不把顶层目录变成gitlink。

## 按问题进入代码

| 问题 | 最小入口链 |
| --- | --- |
| 谁被接受、谁还可重排？ | [事务控制 wrapper](../iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv) → 实际 FIFO/spill/mux；端点按 AW 因果[勘误](../iommu-platform/docs/aw-barrier-causal-20261014.md)区分 |
| hit/miss、PTW、等待如何计量？ | [IOTLB](../iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv) → [TL wrapper](../iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv)；E3 [受控协议与结果](../iommu-platform/docs/e3-controlled-results-review.md) |
| P/PN 配置和切换有什么代价？ | [dma_composition_control.sv](../iommu-platform/cva6/corev_apu/fpga/src/dma_composition_control.sv) → 驱动与 [composition_plan](../iommu-platform/scripts/composition_plan.py)；核 quiet/代次，不能假设免费在线切换 |
| 软件时间与硬件时间怎样分？ | [dma_observer.sv](../iommu-platform/cva6/corev_apu/fpga/src/dma_observer.sv) → [dma_path_observer.sv](../iommu-platform/cva6/corev_apu/fpga/src/dma_path_observer.sv) → [E4 报告](../iommu-platform/docs/e4-results-review.md)；完整任务另查 M0 |
| A5 输入、输出和 wait ID 覆盖哪条腿？ | 先读 [translation-legs](../iommu-platform/out/a5-wait-scope-contract-20261015/source/translation-legs.md)和[规则](../iommu-platform/out/a5-wait-scope-contract-20261015/method/rule.md)，再按其固定源码/行号核回执 |
| mapping 来源是不是当前平台 Linux？ | 先看各轮 source-manifest：近期 mlx5e/v6.12 为外部固定来源，不能与平台 Linux 或当前 Dell 运行身份混为同一部署 |

精确源码行号和 hash 放在每轮证据 manifest 中；本页只维持模块入口，避免复制易漂移的行号。

## `out/`、原始数据与归档边界

[平台 out](../iommu-platform/out/)混有不同性质的文件，**不能整个当作可删除构建缓存**：

| 类别 | 常见内容 | 如何使用 |
| --- | --- | --- |
| 构建产物 | Linux Image、rootfs、OpenSBI payload、FPGA 导出树、BIT/MCS、工具中间文件 | 由 profile/manifest 绑定身份；构建成功不等于时序通过或已加载 |
| 原始实验与回传 | `records.json`、UART、CSV、原始数据、summary/assessment | 保留原件与采集身份；原始值优先于摘要，不能把后续复核写成旧执行成功 |
| 有限模型与源码证据 | source/findings、固定引用、合同、模型代码/trace/results、source manifests | 条件结果与原生测量分开；模型限制以合同和验收为准 |
| 研究裁决与交付 | `README.md`、`root-result-acceptance.json`、decision、next-minimal-contract、execution ledger | 从[成果索引](research-results.md)进入指定轮次；旧后继并非自动排队任务 |
| 文档写前快照 | `before*`、documentation manifests、verification | 用于追溯修改，不是另一套当前计划；不因整理丢弃旧记录 |

同步筛选与大文件清单由[根 README](../README.md)和[同步工具](../tools/github_sync.py)说明；研究数据与历史归档保留，真实凭据及构建产物排除。不要凭 `out/` 名称批量删除或全量重建。

退役 method88 代码在 [platform-archives/retired-code-20261005](../platform-archives/retired-code-20261005/)，保留原路径与 manifest；共享 E2/串口/分析模块仍在活动平台。恢复副本与 bundle 不保证包含最新实验提交。需要恢复时查对应说明与实际版本，不把快照覆盖当前工作树。

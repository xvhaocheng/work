# stage114：补齐自然 NVMe 容量需求，C113 仍不准入翻译实验

2026年10月13日JST。实际登记 stage114 goal，无额外预算；Codex-only，root与一个必要新Sol／ultra、fork-none独占子席。完整交付和状态同步后，真实goal已由工具标complete，见[实际完成回执](../out/stage114-c113-native-evidence-20261013/goal-completion.json)；只完成本关口，方法论文仍处于阶段一立题。

**本轮推进了原生需求证据：原论文有真实的单节点大模型 NVMe 优化器实验，全 CPU 驻留不能覆盖其中约0.5T参数这一规模。C113来源合同继续条件保留；实际共享翻译路径和普通控制之后的损失没有成立，当前FPGA实验不准入。** 不把原论文、固定代码的功能测试和当前硬件运行混成一条证据，不把缺运行记录当科学阴性。

[稳定裁决](../out/stage114-c113-native-evidence-20261013/root-result-acceptance.json)／[原文导航](../out/stage114-c113-native-evidence-20261013/README.md)／[root直接核证](../out/stage114-c113-native-evidence-20261013/critical/root-source-review.json)。原C113任务/owner/生命周期/完整成本入口见[stage113](stage113-zero-infinity-admission-20261013.md)，其符号合同原样保留，不重新执行。

## 一个工作流，三种证据等级

本轮仍只有 C113 **DeepSpeed ZeRO-Infinity NVMe 分块 CPU-Adam 优化器**工作流。直接代码继续固定 `v0.16.4/e2dc3eeb1923073e32739596a4fd051417d4ff92`，没有更换候选或恢复旧分支。为补既有原生配置，定向查了同一官方仓库的功能测试、官方原论文及补充材料，以及精确版本的原始问题记录；不是新增应用场景矩阵。

| 材料 | 本轮新增可接受事实 | 不能赋予它的证明能力 |
|---|---|---|
| 原论文v1，Table1/Figure5c的一条0.5T单节点记录 | 作者报告了真实实验，模型、node/GPU参与数、placement、batch字段成立；自然容量需求能算边界 | 没有launch/config/raw trace；软件提交、FS/PCI/IOMMU/domain、实际H/B/p和逐块完成未知 |
| 固定v0.16.4官方NVMe checkpoint测试 | 明确单rank模型形状、path来源、默认开关、读写操作与功能检查，得到可复核字节合同 | 测试源码不是已运行记录；tmpdir不证明NVMe；不是double-pipeline完成或翻译性能验证 |
| 精确版本检索的原始issue3024 | 唯一命中0.16.4的comment实际CPU offload，可按配置排除 | `nvme_path`字符串与另一个comment里的版本号不能拼成NVMe原生运行 |

## 冻结版本的官方配置和字节

官方测试 `TestNVMeCheckpointing` 的world_size=1，模型为两层2048×2048的Linear（含bias、empty_grad=false）；`nvme_path`取`tmpdir/zero`。源码先训2步、保存checkpoint、继续2步，再destroy／reinit／load该checkpoint并对相同两批再训2步，共6次`step()`调用；实际非overflow成功更新数没有记录。optimizer=Adam，`sub_group_size=100`，AIO block=1MiB。param/optimizer offload有五种参数化组合；本轮只对“optimizer=nvme、param=none”这一源码子情况核优化器字节，未运行任何组合，也没有把测试工作流改成生产需求。

没有覆盖的默认值是optimizer buffer_count=4、pipeline_read/write=false、AIO p=1、queue_depth=8、use_gds=false。因此选中的是单AIO句柄的`PartitionedOptimizerSwapper`，不是已经开启双流水的`PipelinedOptimizerSwapper`。不能把测试类名字或NVMe device枚举当自然独立物理搬运者的证据。[测试原文](../out/stage114-c113-native-evidence-20261013/source/DeepSpeed/tests/unit/runtime/zero/test_nvme_checkpointing.py:22)、[默认值](../out/stage114-c113-native-evidence-20261013/source/DeepSpeed/deepspeed/runtime/swap_tensor/constants.py:19)、[单句柄](../out/stage114-c113-native-evidence-20261013/source/DeepSpeed/deepspeed/runtime/swap_tensor/partitioned_optimizer_swapper.py:32)。

`sub_group_size=100`在所核代码中按完整parameter累加后分组，**不会把每个大weight切成100元素的小tile**。world_size=1且四个parameter都超过100，两个weight各4,194,304元素，两个bias各2048元素；weight组达到1MiB交换阈值，bias组驻留CPU。这一源核避免用配置数字制造额外请求数。

| 源码推导量 | 值与条件 |
|---|---:|
| 全部参数元素数 | 8,392,704 |
| 两个可交换weight元素合计 | 8,388,608 |
| 两个bias元素合计 | 4096；未强制交换 |
| optimizer pinned池 | 4×16MiB＝67,108,864B，按所核默认与最大组；非实测分配 |
| 稳定成功step的state读／写 | 各100,663,296B（p/m/v三个文件） |
| 前置大gradient写／更新前读 | 各33,554,432B |
| 有界梯度＋optimizer请求字节 | 268,435,456B＝256MiB；不是实际DMA/wire/DDR计数 |

这些数字依赖成熟Adam状态、非overflow、梯度段完整送达、原生文件长度与源码期望相符。首次state初始化、checkpoint、param=nvme的额外交通、训练算子和消费者发布均在边界外另计。全p/m/v＋FP32梯度＋FP16参数的逻辑存储为151,068,672B；这不包括FP16梯度、激活、工作区、线程、descriptor、模型runtime或额外pool，不能当实际peak或H。原论文的20B/parameter总model-state口径也不能拿来替换三个state文件的12B/parameter口径。[完整条件与机器合同](../out/stage114-c113-native-evidence-20261013/source/config-derived-byte-contract.json)。

来源函数在checkpoint前后比较同批的标量loss，包含destroy／reinit／load路径；没有每个AIO返回字节、数值数组、request/DMA映射、quiet与最终撤权的原始trace。测试还可能因为fp16或AsyncIO不兼容而skip。本轮未执行，不能声称它通过或失败。

精确冻结sha的GitHub Actions查询当前返回0条；这只说明该次查询没有取到run，不能证明历史从未执行CI。所核A6000 workflow仅选inference_v2；nightly workflow的runner标签和pytest marker也不是该NVMe测试成功日志，更没有存储mount与IOMMU证据。[原始查询](../out/stage114-c113-native-evidence-20261013/source/frozen-commit-actions-runs.json)。

## 接受一条真实已发表需求，保留运行版本界限

只将原论文 **Table1/Figure5c 的0.5T单节点记录**固定为本轮published需求见证：GPT-like、hidden=18K（K=1024，即18432）、124层、seq=1024、batch/GPU=8、MP=1、FP16参数与optimizer state都在NVMe。正文说明单节点实验参与16个V100 SXM3 32GB GPU。16个参与GPU不是已核native launcher的world_size；rank映射仍未知。[原论文v1](https://arxiv.org/abs/2104.07857v1)；root亲看PDF第10页[Table1](../out/stage114-c113-native-evidence-20261013/critical/arxiv-v1-page10.png)，子席精确字段、原文/hash和完整来源见[独占报告](../out/stage114-c113-native-evidence-20261013/workers/published-boundary/report.md)。

Figure2b给单节点名义GPU0.5TB、CPU1.5TB、NVMe28TB。对约0.5T参数，仅FP32 master参数、m、v三项就约6×10¹²B，超过CPU容量；再加梯度/activation等只会增加需求。容量单位和参数标签是论文报告口径，不能推出精确文件长度或实际H，但足以说明此已发表规模的全CPU驻留不能实现相同工作。这是来源自身需求，不是给当前FPGA人为限制内存。

这条接受只排除了该大模型规模的“全部状态常驻CPU”对照；**固定tile的pinned池、合法大页、持久mapping、布局/批量和同owner单设备仍全部允许**。全模型放不进CPU不等于IOTLB放不进、必有重复miss，或普通固定buffer失效。源已有tile reuse与read/write/compute重叠，不能改名为新增方法。[Figure2b原图](../out/stage114-c113-native-evidence-20261013/workers/published-boundary/source/arxiv-v1-page04.png)。

原论文2021实验没有固定软件提交，不能当v0.16.4运行。不同表也不合并：Figure2a约0.5T的hidden为20K，与Table1的18K属于不同字段；Appendix Table4的1T是MP4且batch关系与正文无MP实验不同；Table8的64GPU×batch4与total128字段不闭合，保留原样，不自行修成新run。作者人为把GPU空间分成2GB块的碎片实验也不是本轮自然压力证据。官方SC21 PDF是另一个报告slides，不是原始launcher或测量日志。

## 原始记录筛选结果和实际路径

精确仓库issue检索 `nvme "0.16.4"`只命中3024。原始2023正文版本0.7.7，optimizer／param device均为cpu，且async_io不兼容；2025明确提0.16.4的comment也使用cpu，pipeline未启用。故这个记录不属于NVMe optimizer运行。另一条pipeline/path检索的23个命中只作为有界来源线索，未接受其他应用或运行，也不以检索未得来宣称全世界没有记录。[原body／comment](../out/stage114-c113-native-evidence-20261013/source/native-record-adjudication.json)、[实际查询账本](../out/stage114-c113-native-evidence-20261013/source/native-record-search-ledger.json)。

当前没有一份被接受的原生run材料把同一任务绑定到`path→mount/FS→block backend→PCI function→IOMMU translated domain→共享硬件单元`，也没有自然同时活动peer的事务／owner证据。原论文证明应用层GPU/NVMe/CPU协作和作者测得吞吐；PCIe共享或节点共置不能证明启用并共用主机翻译服务。不能把NVLink/NVSwitch计算通信、CPU算术或两个AIO句柄直接当共享IOMMU requester。

stage113亲核的NVMe request map/unmap、持久buffer与临时mapping区别仍成立，但只在实际设备走该驱动与翻译domain时相关。本轮没有重复核那条完整调用链、读新板身份或把本地Linux驱动部署到论文系统。未知仍为未知。

## 普通强对照与成本裁决

| 对照／成本 | 本轮新增核证或保留边界 |
|---|---|
| 全CPU驻留 | 已发表0.5T单节点规模超名义CPU上限；固定版本小功能模型未证明自然H不足。二者不能混成一项失效 |
| 大页／4KiB回退、布局／批量 | 继续允许；pin、NVMe名字和论文容量都不提供实际leaf、IOVA或SG布局 |
| pool预映射／持久映射 | 固定tile使mapping工作集与全模型容量不同；具体driver/API、权限、方向、CPU/device sync与终止释放成本未给，不能免费假定已实现或禁止这个普通对照 |
| 稳定scratch／同owner单DMA／CPU消费者 | 保留合法聚合和真实CPU更新，不强制不同DID。未握手的ready或CPU标量loss不等价DMA完整完成 |
| 准备与供给 | 分配/清零/pin、m/v lazy init、fd/stat/fs metadata、AIO context/thread、gradient生产/归约、SG/PRP/PTE/IOVA；配置部分已得，实际成本和分配成功未知 |
| 执行与消费者 | NVMe queue/media/DMA/translation、clip/Adam、低精度发布、下游训练/最终输出；论文汇总吞吐不是这些分量的实际新记录 |
| 验证、排空、释放 | IOevent成功bytes/错误、数值、全部request完成、quiet、sync/unmap/fence、fd/线程/context/pool撤权；来源检查覆盖不完整，完整成本未知 |
| 任何新实现／转换 | FPGA producer/consumer adapter或driver registration没有接受合同；resource/metadata/ports/latency/switch/rewarm均不能填零 |

准确前作：NVMe tile复用、预取／回写和CPU重叠是ZeRO-Infinity既有机制；本轮未产生新方法规则。性能：未接受可归因的ordinary外翻译loss或净收益。安全：源等待／依赖不是新安全能力；所核完整完成与释放证据缺项不能被计数掩盖。

## 本关口裁决与下一动作

接受的是**真实published offload需求＋一份固定版本功能配置和完整条件字节**。源合同比stage113更具体；C113仍条件保留，当前FPGA实验不准入，不登记G0/G1。现板有限接口的消费者和数据任务仍不是这个优化器或NVMe系统，不能通过缩小模型、缩H、强制remap或拆出假DID来跨过门槛。

正条件是同一既有运行资料明确软件/配置/shape/budget、实际翻译拓扑和自然peer，并提供完整数值/byte/completion/释放、可执行普通对照及可归因损失。相邻反条件是根本bypass／未启用IOMMU、仅一个requester而无并行peer、CPU能驻留的功能模型、或合法固定tile mapping已消除所声称残余。没有这些原始记录，不判断性能为零。

**下一步先取一份关联同一运行的已有证据包**，内容为版本/launcher/config、path与FS/block/PCI/domain/shared-unit、自然活动peer、N_i/B/p/H/page与完整结果/完成/成本。当前公开材料核查已收束；没有新原始输入时，不重复相同检索、重新生成未变需求卡或启动部署来补证。下一[inactive合同](../out/stage114-c113-native-evidence-20261013/next-goal-contract.json)没有登记或执行，不自动追加第二workflow。

旧结果与关闭分支保留。本轮无工作流运行、GPU/FPGA部署、build/RTL/driver实现、旧实验/校验器重跑或配置/数据库/历史改写。本轮完成不代表论文完成。

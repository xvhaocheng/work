# 独立修复基线构建

本轮的源码入口是 [manifests/source-manifest.json](manifests/source-manifest.json)，
平台版本锁是 [manifests/sources.lock.json](manifests/sources.lock.json)，两组配置见
[manifests/profiles.json](manifests/profiles.json)。生产工作树保持原分支；构建从固定
Git 对象导出到独立输出目录。

硬件以 CVA6 `ba8cdd10659a777ccff0080bf020be64b5c1e064`、IOMMU F5
`85b157ebe71a623e326e51fff7b7126fe77e8eb9` 为修复底座，批量供给与被动观测
来自有界的八文件差量。最终 CVA6 为 `8aac3fcd345af7686a55af1867783fd6b91a0069`，
平台配对提交为 `885ed0a7ea90ed42dae2b7a2dadfdd0acba15273`。
DS0 使用直接 DS 连线；DS50 单独启用 50/50 delayer 诊断。两者共用 path v3、
四 DMA、16 项原 PLRU IOTLB，不引入 E3、O1、banked 或 P/N/PN。
自主 batch 供给是明示的工作负载夹具，不称纯上游平台。

E1b后续补点新增`--variant ds25`与`--variant ds100`：仍选择上述同一平台/CVA6/IOMMU
commit，只在导出的诊断顶层将输入/输出延迟改为25/25、100/100。
新profile由[baseline-source.json](baseline-source.json)声明，完整构建入口、参数差量及结果见
[DS曲线报告](ds-sweep.md)与[实验包](packages/ds-sweep-20261017/README.md)。
两档均50MHz、WNS +0.177ns；WHS分别+0.048/+0.045ns，WPWS均+0.062ns，
setup/hold/pulse失败端点与routing errors均0，delayer单元数120/145。
这两次构建将既有routed配置核查纳入构建收尾，并记录routing errors；未修改约束或实现策略。

Linux 固定 `45bf76b15e36cf608dff95013d91f8bedb7b1b8e` 的驱动/UAPI；CLI、
内核配置与 DTS 取历史平台 `a2e847539bb638662f98dc920de0e8fd63d046e3`。
共用软件仅加入静态 timeout 和 SD updater，使用 UART 控制，不移植网口驱动。
OpenSBI 为 `e8d21bfdc2dd753314627968e496c3a1188f6dfb`。

本轮实际构建已通过，统一交付入口是 [delivery.json](delivery.json)。共用 payload
的内核 release 为 `6.5.0-rc2-ispass2027-pathv3`，29,363,656 字节，SHA256 为
`2de1e1551bc86c9651a39a46a0c4ceac7f630a593766109cadc6027102ceddd5`。

| 配置 | Routed WNS / WHS / WPWS (ns) | setup / hold / pulse 违例 | CPU 时钟 | Delayer 网表单元 |
| --- | --- | --- | --- | --- |
| DS0 直接连线 | +0.177 / +0.047 / +0.062 | 0 / 0 / 0 | 50 MHz | 0 |
| DS50 诊断 50/50 | +0.177 / +0.033 / +0.062 | 0 / 0 / 0 | 50 MHz | 130 |

两配置的 routable nets 全部完成布线，routing errors 均为 0。完整 BIT/MCS
路径与哈希、endpoint 总数、时序报告、构建日志、checkpoint 及网表核查均由
delivery 关联。最终重核了 19 份产物/checkpoint，五个生产组件工作树仍与构建前一致；
见 [manifests/final-build-verification.json](manifests/final-build-verification.json)。
以上是构建验收，板上验收及正式样本由主任务另行记录。

## 重建已有源码组合

以下从 `/home/xvhaocheng/work` 执行。`--source` 是 **JSON 文件**，不是目录。
每个 `--out` 必须是不存在的新目录；不要复用本轮输出或覆盖历史 manifest。

```sh
python3 paper/ispass2027/board/build_baseline.py software \
  --source paper/ispass2027/board/manifests/source-manifest.json \
  --out iommu-platform/out/ispass2027-software-rebuild01 --jobs 16

python3 paper/ispass2027/board/build_baseline.py fpga --variant ds0 \
  --source paper/ispass2027/board/manifests/source-manifest.json \
  --out iommu-platform/out/ispass2027-ds0-rebuild01

python3 paper/ispass2027/board/build_baseline.py fpga --variant ds50 \
  --source paper/ispass2027/board/manifests/source-manifest.json \
  --out iommu-platform/out/ispass2027-ds50-rebuild01
```

`prepare` 仅用于本轮固定引用尚不存在时的首次准备；本轮已经执行。
它以旧值全零创建 `refs/heads/codex/ispass2027-baseline-20261017`，已有该引用就拒绝。
已有源码清单及已恢复的源码对象时，直接执行上述 software/fpga，不再次 prepare。
首次准备的原命令为：

```sh
python3 paper/ispass2027/board/build_baseline.py prepare \
  --out iommu-platform/out/ispass2027-20261017-source01
```

## 构建身份与验收

每个输出目录在执行前建立 `build-manifest.json`，记录实际选用源码、参数、
输入哈希、命令、环境和时间；软件与硬件输出各自保留日志。Vivado 使用
`/tools/Xilinx/Vivado/2023.1/bin`，许可证环境沿用既有构建配置。

本轮实际执行的构建入口原字节固化在
[manifests/build_baseline.as-dispatched.py](manifests/build_baseline.as-dispatched.py)，
SHA256 为 `df50e64c1ee272e2a331d185326b93b4624fb8d799f3643bef5f76152e955f8a`，
与三份 build manifest 中的 `script_sha256` 一致。两次 FPGA 构建结束后，
常规入口补上参数和固定源码身份检查：缺少 source、非文件 source、错误 variant、
非正 jobs 或不同组件提交均在创建输出前拒绝。它按实际组件及父仓库 gitlink
核对 F5；该 host 输入校验没有改变本轮源码或产物。校验结果见
[manifests/build-input-verification.json](manifests/build-input-verification.json)。

软件须核实际 `kernel.release`、payload SHA256、RISC-V ELF 身份及 payload
不超过第一分区 32 MiB。FPGA 须核最终 routed WNS、setup/hold/pulse-width
违例数、50 MHz 时钟，以及 `verify_routed.tcl` 的物理配置检查。
**进程退出 0 不能替代 manifest 的时序状态**；`timing_failed` 不允许加载。
正 WNS 只适用于报告中的约束，不代表完整 IO/CDC 签核。

本轮 [短回归记录](manifests/regression/regression-manifest.json) 的七个入口均已
实际通过：12 次旧 frontend 逐周期对照、11 个 batch 场景、4 个 frontend/observer
联测、28 个常规和 6 个缩位 observer 场景、17 个 PATH 集成场景及路径溢出/清除检查、
69 个 CLI host case 和实际驱动函数的 44 个控制流、3 个缓存、96 个小 chunk 组合。
测试是 RTL 定向事件或
stubbed MMIO/CLI 控制流，不提供 FPGA 性能或完整 HPM 语义证明。
历史测试目录没有独立 DS delayer 单测；其覆盖限于两配置真实顶层 elaboration、
布线配置核查及后续另记的板上功能验收。

对上述重建目录运行同一短回归可使用：

```sh
python3 paper/ispass2027/board/run_regression.py \
  --out iommu-platform/out/ispass2027-regression-rebuild01 \
  --cva6 iommu-platform/out/ispass2027-ds0-rebuild01/src/cva6 \
  --linux iommu-platform/out/ispass2027-software-rebuild01/src/linux
```

它冻结历史 `a2e8475` 的匹配测试，并以指定旧提交的原字节作为 frontend 对照
fixture；仅把 Git 查询替换为读取该冻结 fixture，以适配没有 `.git` 的导出目录。
全部测试构建输出位于新 regression 目录，不写入 FPGA 或 Linux 源码副本。

`verify_routed.tcl` 仅打开已生成的 checkpoint。其调用形式为：

```sh
/tools/Xilinx/Vivado/2023.1/bin/vivado -mode batch -nojournal -nolog \
  -source /home/xvhaocheng/work/paper/ispass2027/board/verify_routed.tcl \
  -tclargs /绝对路径/ariane_xilinx_routed.dcp 0 /新目录/routed-boundary.json
```

DS0 的第二个参数是 `0`，DS50 是 `1`。调用及输出路径均应记录到对应验证 manifest。
构建/网表验收和板上功能、正确性及 100 次正式测量分别记录。

## 源码恢复

[manifests/git-recovery.json](manifests/git-recovery.json) 记录增量 bundle、
完整 patch、前提提交和 SHA256；
[manifests/patch-replay-verification.json](manifests/patch-replay-verification.json)
保存两份 patch 在独立 Git index 中实际重放后与目标 tree 完全一致的结果。

CVA6 增量 bundle 的 prerequisite 是已有来源整理包中的 `ba8cdd1`；平台最小
bundle 的 prerequisite 是本轮开始时的 `6090449`。缺少后者的完整历史时，
使用完整平台 patch、锁和配置恢复等价源码组合，不把本机 commit SHA 的存在
误写成远端已拥有全部前提对象。各 vendor 仓库保持独立的 Git 边界。

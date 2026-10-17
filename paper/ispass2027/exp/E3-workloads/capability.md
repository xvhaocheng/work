# E3 三类流量：实际 pathv3 接口能力核查

> 此页核查原pathv3，不把后来P0能力倒填旧版本。后续用户已批准仅P0最小driver/RTL修改与45点；
> 当前实现、有限验证与构建状态见 [p0-implementation.md](p0-implementation.md)，新板测仍未完成。
> 最新授权已允许完整论文负载的必要RTL/Linux扩展；现阶段仅提交[完整有界接口提案](full-bounded-design.md)，
> 尚未实现ABI5。以下原pathv3能力证据及最初限制作为历史保留。

## Material Passport

- 类型：代码能力核查与主机参数验证；不是实验结果或性能测量。
- 日期：2026-10-17（JST）。状态：`BLOCKED_INTERFACE_CHANGE_REQUIRED`。
- 范围：冻结 pathv3 用户程序、UAPI、driver 与 DS0 实际 RTL；未访问硬件，未修改这些输入。
- 结论：现有硬件 batch 只支持 **4096B、同形顺序 N 页回环**；三类目标负载和独立 V/A 大小不能仅靠新用户态程序实现。变更边界见 [change-request.md](change-request.md)。
- P0限定的具体最小方案见 [p0-change-option.md](p0-change-option.md)：独立V/A长度、packed-ring地址推进、bytes/验证及旧ABI兼容；仅为方案，未实施。

## 实际源码与引用约定

以下 `路径别名:行号` 均指实际构建副本，不是当前活动研究 checkout。

| 别名 | 根目录或文件 |
| --- | --- |
| `SW` | `iommu-platform/out/ispass2027-20261017-software01` |
| `C` | `SW/inputs/bench/idma_experiment.c` |
| `U` | `SW/src/linux/include/uapi/linux/idma.h`；实际编译 include 为同一构建下的 `SW/uapi/linux/idma.h` |
| `D` | `SW/src/linux/drivers/misc/idma.c` |
| `HW` | `iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6` |
| `R` | `HW/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv` |
| `W` | `HW/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv` |
| `P` | `HW/corev_apu/fpga/src/ariane_peripherals_xilinx.sv` |

身份入口为 [software-build.json](../../board/software-build.json) 和
[fpga-ds0-build.json](../../board/fpga-ds0-build.json)。冻结副本由这些 manifest 定位：

- [软件构建记录](../../board/manifests/software-build.json)第14–16行：Linux `45bf76b15e36cf608dff95013d91f8bedb7b1b8e`、OpenSBI `e8d21bfdc2dd753314627968e496c3a1188f6dfb`、平台软件输入 `a2e847539bb638662f98dc920de0e8fd63d046e3`。第18、26–27行记录 CLI、driver、UAPI 的输入哈希；第40–42行列实际 include 与 CLI 路径。
- [DS0 构建记录](../../board/manifests/fpga-ds0-build.json)第15行起的选用来源：CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069`、IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`。这是本次读取的硬件基座。
- 构建副本不存在时，应沿以上固定 commit/来源包恢复；不能用活动 `iommu-platform/linux` 或活动 `bench/idma_experiment.c` 无声明替换。

## 参数与真实调用链

`C:22–40,64–67,124–129,350–362` 给出的现有参数是 device、aggressor ID、V/A 页数、**一个共享 chunk**、V 完成数、预热数、timeout、batch/caps。默认 V/A 各源缓冲和目的缓冲分别为 1/8 页，chunk=4096，V measured transfers=4096，warmup=256，timeout=60000ms。

`U:47–61` 的输入只有 `victim_pages`、`aggressor_pages`、`chunk_bytes`、`victim_transfers`、`warmup_transfers` 等标量；没有独立 V/A chunk、源/目的地址、offset 列表、pitch、tile、descriptor 数组或用户缓冲句柄。`U:49–50` 要求 ABI1 flags/reserved=0，不能借保留字段偷偷增加语义。`U:78–85` 只有既有实验与 batch 操作；`D:1058–1068` 分派这些操作，未知命令返回 ENOTTY。

调用链为 `C:395` → `IDMA_IOC_RUN_BATCH`/`RUN_EXPERIMENT` → `D:1063–1066` → `idma_run_experiment_mode()`。关键约束如下。

| 项目 | 实际行为与原证据 |
| --- | --- |
| page 与页数 | `D:56–57,646–660`：PAGE_SIZE 必须为4096，V/A各1–4096页，每条流每个S/D缓冲最多16MiB，timeout≤300000ms。页数是足迹，不是单描述符长度。 |
| 非 batch chunk | `D:654–658`：`0 < chunk ≤ 4096` 且 `4096 % chunk == 0`。在64B–1536B范围内仅64/128/256/512/1024这些整除长度可用；1500B、1536B均不合法。 |
| batch chunk | `C:144–147` 在参数阶段拒绝非4096B；`D:863–865` 在内核再次强制4096B，V task≤2^20。只删除CLI检查不会使driver接受。 |
| V/A大小 | `D:901–906` 为双方传入同一个 `e.chunk_bytes`。可独立改变页数，不能独立改变每次复制的长度或形态。 |
| 多进程/不同fd | `D:85–86,868,1030` 的全局 `idma_experiment_lock` 覆盖准备、映射、执行、排空、验证和清理；不同设备和不同fd的两个实验也串行。不能用两个Solo进程构造不同长度的同步共跑。 |
| 其他用户接口 | `D:147–152,1044–1077`：open只绑定该设备，fops仅open/release/ioctl；无mmap/read/write和用户descriptor/缓冲入口。未知ioctl返回ENOTTY；不存在可由新CLI组合出的底层COPY接口。 |
| 非 batch 地址推进 | `D:341–366` 写入`src_iova+offset`、`dst_iova+offset`及真实`chunk`，完成提交后`offset += chunk`，仅等于`pages*4096`时回零。这是紧密连续填充的环，不是逐包跳到下一页。 |
| 非 batch 供给 | `D:409–440` 软件观察完成并逐次补发，不能代作硬件 batch 流量。 |
| batch 配置 | `D:520–534` 仅写target count与pages；`D:591–598` 仍通过`idma_submit()`写S/D/4096长度后触发批次。没有descriptor buffer。 |
| batch统计 | `D:556–566` 读取hardware issued/completed，按`completed*PAGE_SIZE`计算bytes，按已触及的连续页前缀验证目的缓冲。只放宽长度检查会连计数与验证一起失真。 |

## DS0 RTL 证据：固定几何不是 CLI 的误限

`P:1018–1029` 实际实例化 `dma_core_wrap_intf`，`IS_TWO_D=0`、`BATCH_PORT=1`；`W:123–138,385–406` 将该配置连到一维 `idma_reg64_frontend`。vendor 目录中存在 2D frontend 文件，不能据此宣称当前BIT启用了2D。

- `R:96–100`：batch 合法条件明确为 `num_bytes.q == 4096`、S/D均4KiB对齐、1–4096页、DEFAULT复制。
- `R:127–143,274–286`：batch寄存器是control、target、pages、completed、issued；没有可编程步长或descriptor表。
- `R:194–196`：普通路径的底层请求长度取`num_bytes.q`，不是硬件后端永远只能4KiB。当前端到端限制须同时考虑UAPI、driver与batch模式。
- `R:337–346`：S/D使用同一`transf_offset_q`，每次固定加`0x1000`，到`batch_pages<<12`归零。以0为首offset，第i项为`4096*(i mod N)`，长度固定4096B。

因此 **1页chunk上限、N页足迹与16–64KiB请求能力是三个不同概念**。把`--victim-pages 16 --chunk 4096`叫作64KiB描述符是不正确的。

## 三类目标流量与可用子集

| 目标 | 当前 batch | 当前非 batch 的有界能力 | 阻塞 |
| --- | --- | --- | --- |
| NIC：64B–1.5KiB，N页环形 | 小包全部不支持 | 同一共享chunk的64/128/256/512/1024B，紧密连续填充N页环 | 1536B不整除页；独立V/A大小和可配置slot/页间步长无入口。用户所需具体包槽布局不能由“N页”自动推定。 |
| storage：16–64KiB请求，跨大量页顺序访问 | 仅能产生4KiB页任务回环 | 单次长度仍≤4096；无16/32/64KiB请求入口 | 大缓冲不等于大请求。把4/8/16个4KiB任务归组仅是离线解释，会改变描述符完成/供给语义，不能宣称已实现目标流量。 |
| accelerator：二维跨步分块，可多个1D描述符 | 不支持非平凡2D布局 | 只能连续`offset+=chunk`；无逐描述符offset | 软件无法传递二维展开后的1D列表。原生2D模块未在此配置启用。退化连续布局不代表二维跨步分块。 |

以下只是**已有接口可表达的命令示例，未执行硬件**；实际运行仍需既有caps、HPM、内存分配及错误检查成功。

```sh
# 既有硬件batch：V=1页、A=8页；双方仍均为4KiB任务。
idma_experiment --device /dev/idma0 --aggressor 1 --batch \
  --victim-pages 1 --aggressor-pages 8 --chunk 4096 \
  --transfers 4096 --warmup 256 --timeout-ms 60000

# 仅非batch的64B顺序环子集：双方均为64B；属于软件补发对照。
idma_experiment --device /dev/idma0 --aggressor 1 \
  --victim-pages 1 --aggressor-pages 8 --chunk 64 \
  --transfers 4096 --warmup 256 --timeout-ms 60000
```

## buffer 组织、计数窗口与数据验证

`D:282–328` 为每条流分配独立源/目的缓冲，各大小为`pages*4096`，使用`DMA_ATTR_FORCE_CONTIGUOUS`，独立找S/D IOVA并逐4KiB映射；所有缓冲由内核拥有，用户程序不能填写其数据布局或地址。`D:304–308` 初始化源模式及目的反模式，batch也使用相同数据缓冲而非descriptor queue。

`D:920–946` 先做软件预热、批次前排空A，再令V/A batch offsets从0开始。`D:941–943` 的V target是请求的固定次数，A target为2^32，A并非用户另给的等工作量固定任务。`D:948–977` 在观察arm后读before HPM、启动软件计时、先发A再发V、观察V最终完成、停止A、读after HPM。HPM窗口包住软件窗口且非原子；`victim_elapsed_ns`含launch/poll，不能代作path硬件窗口。

`D:978–1017` 排空双方后收集batch完整issued/completed并做数据验证。A completed包含startup和最终drain，不是仅V硬件窗口的吞吐；batch refill gap未采样。`D:737–840` 验证并输出冻结observer/path数据，打印发生在排空/验证后。原CLI在`C:322–343` 已明确这些语义。

`D:475–517` 校验全部源字节和目的字节；目的预期按`touched_end`连续前缀定义，前缀外仍应为反模式。这适合当前连续/页环，**不适合直接复用给带洞2D**。以后如改变布局，必须用实际触及区间/bitmap保持完整验证，不能只忽略洞或把数据验证关闭。`C:403–421` 另核对data_errors/quarantined、测量状态、完成数及`transfers*chunk`。

## 主机验证：已做，不能升级为板测

在临时编译单元中把原`C`以`#define main original_idma_main`包含，主机wrapper仅调用原`parse()`和`validate_caps()`。合成caps明确设为ABI1、4KiB页、4096最大页、300000ms和DID_HPM/SW_REFILL/HW_BATCH；这些值来自上述driver，不是实机caps读数。未调用原main、open或ioctl，未加载BIT，未执行内核driver。

可复核入口为 [check_capability_host.py](check_capability_host.py)，完整逐项回执为 [host-capability-results.json](host-capability-results.json)。该工具同时保存以下19项参数检查和二维几何检查；编译产物仅存在临时目录，退出即清理。复现命令（输出路径选新的名称可保留原回执）：

```sh
python3 paper/ispass2027/exp/E3-workloads/check_capability_host.py \
  --output /tmp/e3-host-capability-recheck.json
```

[主机检查源码](check_capability_host.py)与[完整回执](host-capability-results.json)保留原始结果。
重跑时不指定`--output`即可输出至终端；指定时必须使用新文件，不覆盖旧回执。
GCC 11.4.0，`-O2 -Wall -Wextra -Werror`编译通过；19项检查全部符合预期：

| 项目 | 个数 | 实际观察 |
| --- | --- | --- |
| 非batch 64/128/256/512/1024/4096B | 6 | 原parse与合成caps检查均接受 |
| 非batch 1500/1536/16384/65536B | 4 | caps阶段拒绝，`pages, chunk or timeout exceed reported capability limits` |
| batch 64/1536/16384/65536B | 4 | arguments阶段拒绝，`batch requires 4096-byte copies and at most 1048576 tasks` |
| batch 4096B；双方4096页 | 2 | 参数与合成caps检查接受；未声称分配/实板成功 |
| 4097页 | 1 | caps阶段拒绝 |
| `--victim-chunk 64`、`--row-bytes 1024` | 2 | arguments阶段unknown argument |

另做一项主机几何算术检查：8行×256元素、每元素4B、S/D pitch=4096B、tile=2行×128元素，按`tile_y → tile_x → tile内行`展开为16个512B的一维复制。offsets为：

```text
0,4096,512,4608,8192,12288,8704,12800,
16384,20480,16896,20992,24576,28672,25088,29184
```

确认offset唯一、区间不重叠、总复制8192B、地址跨度29696B、每个S/D缓冲需8页。这个序列是**未来接口所需的合成语义例证**；当前UAPI不能传递，未生成硬件流量。固定batch的8个4096B页任务会触及32768B，不能替代它。以上算术和参数检查仅为主机验证，未产生E3性能结果。

## 当前接受与停止条件

A2本轮进一步将原CLI/UAPI保存为未修改的[source副本](source/idma_experiment.pathv3.c)，
哈希与既有software-build manifest一致；以 [capability_probe.c](capability_probe.c)
和 [verify_capability.py](verify_capability.py) 重现并扩充至21项检查：
原19项加batch1024B拒绝、独立`--aggressor-chunk`参数拒绝。
完整逐项输出、主机编译器版本、命令和实际产物身份见 [host-validation.json](host-validation.json)。
此新工具另外静态交叉编译原CLI，未访问设备或执行原main。

静态产物本机保存在
`/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/host-check01/idma_experiment.pathv3`，
517336 bytes，SHA256 `1e6a776bd6b7ed9613016aaae858b0d504e13643f00cde0b343161aa38e8de92`。
它是原CLI重编译，不具备新E3能力；不因有新文件名就要求Root上传或更新内核。
完整调用链关键原文另存 [source-evidence.txt](source-evidence.txt)，便于离线review。

接受：本报告证明实际接口边界，保留旧4KiB batch与原有完整验证。拒绝：通过删除CLI限制、把大页数写成大请求、以软件补发混入batch基线、以连续退化布局替代二维流量。依用户“需要改driver/RTL则写change-request并停下”的边界，本轮在[变更请求](change-request.md)处停止，E3三类硬件流量未实现、未上板。

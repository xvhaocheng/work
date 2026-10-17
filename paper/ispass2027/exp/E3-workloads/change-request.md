# E3 变更请求：当前 batch 几何不能表达三类目标流量

> 后续仅P0最小方案已获用户批准并实施中，见[p0-implementation.md](p0-implementation.md)。
> 本页完整三形态请求仍未获扩展，不代表已经实现16–64KiB或2D。

## Material Passport

- 日期：2026-10-17（JST）。状态：`REQUIRED_NOT_IMPLEMENTED`。
- 范围：必要接口变更的证据与接受要求；不是新增机制提案或实施授权。
- 对应证据：[capability.md](capability.md)，其中`C/U/D/R/W/P`别名解析到实际pathv3软件和DS0冻结源码。
- 本轮已停止用户态流量实现；未修改driver/RTL/现有CLI，未构建新镜像或BIT，未执行E3板测。

## 必须变更的直接证据

**先决策P0即可：** [p0-change-option.md](p0-change-option.md) 将修改限定为
V64B/1KiB/4KiB对A4KiB、45次的packed-ring批量流量，列出逐文件接口、
NIC紧密环／每页槽几何选择、真实len与offset、driver bytes/touched验证、
新版本UAPI与旧4KiB兼容及验收。以下完整三形态请求仍保留为后续边界，
不要求为P0一并实现16–64KiB或2D。

| 需求 | 当前不可表达处 | 最低必要涉及层 |
| --- | --- | --- |
| batch NIC 64B–1536B | `C:144–147`、`D:863–865`、`R:96–100`三层均锁4096B；`R:344–346`步长固定4096B | CLI、driver与batch RTL的长度/地址推进；仅去掉CLI检查无效 |
| 独立V/A大小和形态 | `U:47–61`仅一份chunk，无独立形态；`D:901–906`双方同用chunk | 明示且兼容的UAPI与driver；batch各lane实际可编程几何也需与之配合 |
| storage 16–64KiB真请求、跨大量页 | `D:654–658`单次≤4KiB且整除页；`R:98` batch长度锁4KiB，`R:344`步长锁4KiB | driver、batch RTL及统计/验证；页数扩大不足以实现 |
| 二维跨步分块，多个1D描述符合成 | `U:47–61`无地址或offset/descriptor；`D:341–366`只做连续偏移；`R:337–346`固定页步长；`P:1028`实际IS_TWO_D=0 | UAPI与driver必须能明确展开并传递布局；维持batch供给时需支持其地址/长度序列的RTL。是否采用原生2D并未决定 |

普通RTL的`R:194`可接收`num_bytes.q`长度，因此不能把结论扩大为整个iDMA后端不具备大复制/任意长度能力。但当前已验证的**端到端batch路径**确有硬约束。本请求不声称只改一个层即可完成。

## 必须一起审查的计数和正确性

1. `D:561–565`的completed bytes固定按`completed*4096`，touched范围按连续页前缀计算。可变长度或带洞布局必须改为真实descriptor bytes与触及区间，防止“参数是小包、实际上仍传4KiB”或跳过未触字节校验。
2. `D:475–517`当前验证整条源和目的、前缀外反模式；二维带洞场景必须保留所有这些正确性要求，采用实际区间/bitmap验证。不能取消数据验证以适配布局。
3. `D:941–943,948–1017`定义A长跑/停止排空、V固定工作量、HPM软件包络与完整batch统计；`D:737–840`定义observer/path完整性。改变task粒度时必须明确descriptor、AXI transaction和application request三种计数，不能悄悄沿用4KiB解释。
4. ABI1 `U:49–50`规定flags/reserved为0。新的参数必须明确版本或独立操作，不能覆盖保留字并继续声称原ABI合同。旧`RUN_BATCH`和历史4KiB模式需保持可复现。

## 若后续授权实施，验收接口应表达什么

以下是需求，不是已决定的实现方案：

- V与A分别记录工作负载形态、单次/行长度、足迹、ring/span、请求数及合成方式；有效长度实际传到设备，不能只写JSON标签。
- NIC明确64B至1536B的包槽布局、跨页与ring尾部处理；“N页环”不自动等于每包隔4096B，也不自动等于紧密填充。
- storage明确16/32/64KiB请求的连续S/D地址、跨4KiB边界、足够大的顺序工作集、尾部与回环；如采用多个1D descriptor，须同时报告application request数、descriptor数和实际供给方式。
- accelerator允许按明确tile顺序展开1D行片段：`offset=(tile_y*tile_rows+row)*pitch+tile_x*tile_row_bytes`。S/D pitch分别定义，复制字节与地址跨度分别统计；[capability.md](capability.md)的16×512B例证可作为未来host几何验收样例。
- host验证各形态的bounds、ring/尾部、完整复制量与触及集合；driver/RTL验证真实长度、地址、停止排空、counter/observer语义及数据完整性。随后才能请求对应板测验收，不能把本轮host参数检查当作硬件验证。

## 本轮可继续使用的既有子集

4KiB硬件batch、独立V/A页数与既有统计/验证可以原样使用。64/128/256/512/1024B仅可用在**双方共享长度、连续环、软件补发**的既有实验接口，不能冒充所请求的独立三类batch负载。1500/1536B与16–64KiB目前连此软件接口也不能通过校验。

当前交付是能力证据、合成语义例证及此变更请求；完整E3矩阵保持计划状态。未经用户后续明确扩大范围，不实施这些driver/RTL改动，不启动E3硬件流量。

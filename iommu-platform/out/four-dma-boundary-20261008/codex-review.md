# Codex 审定：四 DMA 请求头容量边界（2026-10-08）

## 当前结论

真实生产拓扑是四 DMA、两翻译 bank；本轮只完成源码接口核查与受限抽象状态搜索，没有实现或运行四真实 DMA RTL。盲目共享头槽确实能在模型里形成容量分配停滞，但普通属主预留已经排除本模型的停滞，因此不能把共享头槽或属主预留包装为新机制。

## 已直接核查的生产接口

- `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:870`：NumDMADevices=4。
- 同文件1228的四输入mux、1255起四次实例；本地读ID0经mux得到完整源ID0/4/8/12。BankIdBit=2（1409），所以DMA0/2到bank0，DMA1/3到bank1；bank身份不等于DMA身份。
- 同文件1268：仅前两路在N模式用512拍buffer，其余3拍；1280/1465起完整copy保证及观测仅覆盖前两路/DID10/11。不能从四路实例化推出当前N机制已经支持四路。
- 当前实际DMA测试已有四输入mux，但只接两个真实iDMA，输入2/3置零；cfg selector、调试mask和late-pool源数组仍为两路。
- `tests/nonblock_completion_pool_late.sv:461`的完整ID检查按源数组下标，不是按翻译bank下标。同bank两源测试已有正确映射。扩四路需扩完整source身份、标签FIFO、元数据及两个W顺序的源标记，不能只扩大bank位数。
- `rv_iommu_pkg.sv`中的trans_req保留DID，TC仍保留原AR。DeepSeek证据稿称“TC只知道bank、不知道设备”不成立。它称same-bank必然fault12的推断也不成立；这些推断不采用。
- `idma_backend.sv:836–857`已有R-AW bypass；不能假设每个源AR自然等到自身物理目的AW准备好才进入。
- 原AR被接受后，输入AXI demux的读ID/路由计数直到真实RLAST才释放（`vendor/axi/src/axi_demux_simple.sv:354–372`）。把已接受的完成AR直接丢弃并稍后“重放”不是可用透明回退；需独立翻译接口或正确保留/退休整个AXI事务。

## 抽象模型的明确合同

四源d=0..3；bank=d&1；每源一个活动普通4KiB copy，两读写burst标记(d,0)/(d,1)。只研究translated AR请求头，不存payload。

固定写义务顺序为0a,0b,2a,2b,1a,1b,3a,3b。每bank内两个源各自保序，跨源任意交错：每bank六序列，共36组合。每bank只有一个可见完成头；有限槽可park头以暴露其后请求。当前写义务的准确源读允许直通，或者从暂存槽派发；同源次序必须保持。

动作包括translate、park、direct/dispatch。一次派发原子地完成这段R→W并推进写cursor，**假设对应AW可准备且内存/R/W服务有限返回**。不模拟实际TC main/spill/nfast深度、AXI VALID周期、PTW/FAST、DDR拍、job/失效epoch，也不证明该写顺序与读交错在生产RTL可达。新增头槽数不含既有AR FIFO及源持有描述符。

Breadth-first搜索所有可达状态，非终止且没有可用动作才是dead state。成功路径存在不代表所有路径保证进度；每种策略同时统计成功可达和停滞可达。这个有限单批模型没有无限重试或外部不公平等待，不是一般形式验证。

## 已执行结果

| 策略 | 槽数 | 成功可达的组合 | 可达停滞的组合 | 搜索状态 |
|---|---:|---:|---:|---:|
| 每bank独立2头（local2） | 4 | 36/36 | 0/36 | 3816 |
| 盲目全局共享 | 1 | 9/36 | 33/36 | 1176 |
| 盲目全局共享 | 2 | 36/36 | 30/36 | 2688 |
| 盲目全局共享 | 3 | 36/36 | 30/36 | 4092 |
| 盲目全局共享 | 4 | 36/36 | 30/36 | 5340 |
| 盲目全局共享 | 5 | 36/36 | 18/36 | 6192 |
| 盲目全局共享 | 6 | 36/36 | 0/36 | 6660 |
| 普通当前W-bank预留2头 | 2 | 36/36 | 0/36 | 1656 |
| 普通当前W-bank预留2头 | 3 | 36/36 | 0/36 | 3060 |

九策略乘36＝324次**抽象分析**，累计34680个可达状态（不同策略/组合间不去重）。不是324次RTL/板卡实验，也没有性能/面积结果。

反例：bank0头顺序2a,2b,0a,0b；bank1为1a,1b,3a,3b；当前写等0a。共享H2先存2a和1a后耗尽；bank0可见2b，bank1可见1b，所需0a仍被挡在后面。无可派发头，也没有槽可继续暴露0a。普通预留把两槽留给当前W的bank，即能先park2a/2b再直通0a。

“共享6槽才无停滞”仅是这个固定合同下的盲目分配策略结果；普通预留2槽也通过，所以不是通用最低容量或新容量定理。保留源码映射及危险状态作为后续验证输入，不升级为创新。

## 记录与分工

[完整结果](results.json) / [成功及停滞见证](traces.json) / [模型脚本](header_progress_model.py) / [DeepSeek证据稿](evidence-answer.md) / [DeepSeek模型提案](model-answer.md)。

Codex独立定义状态/策略/合同，直接核关键源、纠正证据稿、审查实际Python状态转换并执行。DeepSeek整理接口证据与起草模型脚本，没有执行命令。两次high/thinking/max393216完整返回，合计189747 provider tokens。没有生产RTL/Linux/BIT/板卡操作。


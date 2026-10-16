# 独立HERO有限源语义执行器

作者：Codex gpt-6.1-sol／ultra独占席 `/root/a_recent_contracts`。只编写 `semantic_executor.py` 和本文，不修改冻结输入或生产代码，不运行八案，不调用DeepSeek／Claude，不恢复旧goal／ACV实验。

输入为root冻结的 `cases/frozen-contract.json`，收到时SHA256为 `fe077ddbd64508909ac1fbec072ac2ec6297163589a529e89c4b1dbd5d772e5b`。执行器直接读取实际schema：`input`、`pending_descriptor_capacity`、`kind_order`、`current_need_tie_order`以及四案的完整nodes。P/N/H2各44节点、H1为40节点。

## 作用范围

每个logical_step原子执行一个合法ready事件，期间执行真实字节复制或矩阵算术。logical_step只有事件序号含义，没有持续时间、速率或重叠性能含义。kind_order及publication_fifo来自冻结合同；它们只选一条允许的源语义线性序，不是HERO/CVA6/MIG硬件保证。

原HERO运行copy-based device(1)，SVM分支为if(0)。本执行器只检查程序DAG、真实值和六槽生命周期允许某个顺序，不证明原API的确切物理可达性。原子publication节点是显式未实现的适配：内部配置write／trigger、真实backend ID、AXI/TC/PTW/cache全部未建模。所有CPU、服务、缓存、函数、FPGA时间及总线AR/R/AW/W/B计数都保持null；不会把结果当性能准入或原四项时间预测通过。

## 固定选择及公共前缀

所有节点先经过显式predecessor和publication_fifo资格筛选。ready publication最多2个，超过即输入合同失败；不增加描述符或payload容量。

- `source_A_first`：publication按固定A0,A1,B00,B01,B10,B11,C0,C1排序。
- `source_current_need`：publication按冻结节点的`first_use_rank`，同rank才按`contract.current_need_tie_order`。只使用声明的静态用途信息，不看服务时间、未来返回或最终参考结果。
- 其他事件按冻结kind_order，同kind按合同nodes原次序。

P/H1从host_init/reference/map/allocate实际执行到pub_A0，P显式检查此时A1和B00均enabled。N真实执行包含pub_A0、pub_A1的完整前缀，此后不撤回或重排它们。H2从初始化实际生成source_A_first到K11，前缀所有事件保留；仅之后切换目标policy。K11处如实记录C0是否尚未语义完成，未欠债则该子条件未覆盖；不增加延迟。每案两个固定policy，默认最多8次；失败立即停止，没有自动重试。

## 字节、计算与所有权

host_init实际生成A/B字节，保留原C/D仅初始化width*height字节的边界，未初始化部分用A5毒值占位。host_reference用独立的32×32×32标量数学计算写入D，不使用`C=2*A`捷径。map_in建立明确逻辑副本，不声称实现原OpenMP运行时或WT可见性。

每笔h2d completion实际复制2048B到指定槽；d2h实际读取完整C槽写入目标C，并逐字节检查目标只写一次。四个compute节点各执行8192次模2^32 MAC，按原A/B槽和C半条带索引写真实结果，逐字节检查四个tile无遗漏／重复。host consumer在map_out及独立visibility事件后，读取C/D并比较全部1024个元素，之后才允许host_free。

每个输入槽保留尚未消费的真实compute-use集合，因此不允许B10覆盖尚供K00使用的b0，也不允许提前释放仍有消费者的A/B。C槽必须两个column tile全部生产，再允许DMA读；DMA owner在publication取得、completion释放。H1保留原B00/B01及同两个B槽，两个额外事件只检查内容/owner，不创建或等待B10/B11。最终检查8或6笔完整copy及各一次publication/completion/wait、32768个kernel MAC、六槽free、所有owner/FIFO排空、输入A/B未变、host消费完成。

所有allocation、DMA、resident及compute读写lease均在JSONL记录acquire/read/write/release/free；resident lease与计算临时lease分开。map-in/out字节是合同内逻辑副本字节，原运行时总线字节仍null。16KiB/12KiB是copy工作量，方向分别按h2d/d2h计，不混成AXI R+W总线量。

## root审查与后续调用接口

root重点审查 `validate_case` 的完整后缀和唯一job检查、`publication_rank` 的冻结普通规则、`execute` 的槽use集合／真实compute／consumer，以及 `run` 的完整前缀和H2收据。root直接审查后才授权执行本工具；本席没有运行。

默认一次完整八案调用形式：

```text
python3 /home/xvhaocheng/work/iommu-platform/out/control-position-method-20261015/validation/semantic_executor.py --contract /home/xvhaocheng/work/iommu-platform/out/control-position-method-20261015/cases/frozen-contract.json --output-dir <root指定且尚不存在的绝对输出目录>
```

可用`--case`／`--policy`精确选择已授权的失败项，程序不自行重复成功项。输出目录若已存在则拒绝，防止覆盖原日志。每次run输出完整events.jsonl、result.json和消费时捕获的output-c.bin；失败追加failure.json，保留此前事件，不输出通过。总summary.json记录输入／执行器SHA、实际完成次数及全部null边界。

仅静态语法检查，不导入执行器、不调用load_contract／run、不生成语义结果或__pycache__。实际检查结果由本席交付消息记录；语法正确不等于八案通过。

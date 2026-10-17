# E5：现有延迟字段与分布能力

## Material Passport

- 2026-10-17；固定baseline100的observer v3；只读核查与既有记录离线整理。
- **不新增E5板测**。复用其他已授权运行自带的PATH sum/full/max，零计数输出N/A。
- 请求级CDF、p50、p99目前不可采。以下最小RTL请求未实施，须另行决定。
- 模型Codex-only；未调用DeepSeek；A3没有打开UART/JTAG/FPGA。

实际来源使用 [E2的SW/HW前缀](../E2-device-scaling/capability-and-plan.md)；证据原文与hash见 [source-evidence.json](source-evidence.json)。

## 现有版本究竟保存了什么

`HW/src/cva6/corev_apu/fpga/src/dma_path_observer.sv:3–23`每个stage仅Depth4在途FIFO、32bit时戳、48bit总和、32bit计数/最大值。`37–44`暴露8字段：entered/full/carry/sum/max/opening/closing/area；`63–81`入队时戳在完成时转为age并合并到sum/max。**没有样本导出或直方图**。`119–142`只有4lane×4stage×8word，共128word，`dma_observer.sv:97–98`映射到结果索引64..191。32bit ARM计时溢出会报错（`dma_path_observer.sv:163–164`）。逐请求时戳存在于内部scoreboard，不等于可从寄存器读取逐请求时戳。

`SW/src/linux/drivers/misc/idma.c:776–839`读取同一128word，按16条PATH日志输出；除sum/full均值和有限max之外没有分布。E4最初协议仅H/P/R总和、最大值和计数，参见`iommu-platform/E4_OBSERVER.md:13–18,59–63`，也不能推造分位数。

历史v6来自独立E3研究配置，**不是当前baseline**。`iommu-platform/E3_CONTROLLED.md:160–172`明确固定V0/A1、有限target1，E3 bank仍v2；`E3_QUEUE_PAIR.md:60,78–90`的256word包含触发目标A/V、最多两条插入请求及计数摘要。历史固定源码可由E3交付manifest追源；当前活动研究文件`cva6/corev_apu/fpga/src/dma_e3_observer.sv:17–29,72–96,206–217`也体现有界两目标记录与两插入上限，但**仅作协议核对，未当实际baseline输入**。不能把四个有限配对当完整批次CDF。

## 本轮能交付的离线统计

`summarize_path.py`接收现有collector的`records.json`，逐试次/条件/DS/role/channel/stage保留sum、full、max、原始command和boot；输出每试次sum/full以及跨试次描述统计。full=0时mean与max都为null/N/A，不能把无样本补成0周期。不同DS不合并，跨boot数量单列；不会把多试次均值的中位数称为请求p50，不会把最大观察值称为p99。

路径阶段分开统计，不能把各段独立max相加为同一请求的端到端max；carry样本不进入该stage的sum/full。H/整批时间、stage等待、软件时间属于不同口径。

本轮已整理baseline100的100个既有试次，得到1600个stage行、160组（每组10试次）。[摘要](baseline100/summary.md)、[完整分组及输入hash](baseline100/summary.json)、[逐试次stage表](baseline100/trial-stages.csv)。配置分组包含既有manifest中的BIT、内核、可用源码身份及完整CLI输入，不把不同设备/长度/配置无声合并。所有Solo A空组保持null；DS50两个boot，DS0一个boot。

## 最小被动RTL请求（未实施）

若后续确需请求分布，可在现有`dma_path_stage`的`active && matched && whole`完成分支同时更新有界直方图，保持现有sum/max与所有数据通路控制不变。最小第一版只选择一个**明确命名的lane/stage**（例如V读stage1），把所选stage的hist附加到原计划运行；它给出该阶段请求分布，不是完整COPY或端到端请求延迟。

- 固定33桶：age=0单桶；随后`[2^k,2^(k+1)-1]`，k=0..31。32bit计数溢出粘性报错，hist总数必须等于该stage的full；carry单独保留，不隐含丢弃。原exact max继续提供最大观察值。
- 33word可放当前空闲结果索引192..224，225..255放标识、配置、计数/错误等元数据，仍用现有8bit选择器；原解码目前不接受这些索引，必须显式扩展并更新版本/能力识别，旧接口不读成零。所选lane/stage和桶边界应在ARM前固定并随结果输出。
- p50/p99只能报告所在桶的**区间**，不能伪装精确周期；桶宽若不满足论文图需求，Root需选择更细的桶表或更大的样本记录，而非离线插值制造精度。CDF须画累计桶数并披露分辨率。
- 被动回归需覆盖0、边界、同拍入出、carry、freeze、abort、计数溢出，hist/full/sum/max关系及功能通路逐周期不变；新BIT独立构建/时序/资源与软件读取接口都需验证。当前没有启动这些工作。

若目标必须是输入请求到RLAST的端到端分布，需新增跨全部路径的有序匹配/深度边界；现有单stage Depth4不能直接假定足够。这超出上述最小stage histogram请求，应由Root单独裁定。现有均值/最大值不能替代这一缺口。

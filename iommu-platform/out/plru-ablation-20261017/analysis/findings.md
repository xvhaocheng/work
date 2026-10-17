## Material Passport

ARS-Codex experiment-agent run/validate；本文件是运行输出归约，单位为行为DDR模型中的RTL时钟步。

P：IOTLB分区；PN：同一分区再启用独立命中读通路。两者共同采用页表读优先与最多连续4次的服务规则。R表示A设备循环写入的目的页数，另有1个固定源页。

已保留24次attempt，18个配置有有效主结果；所有attempt有效：True。

每次运行保留两路各64次4KiB COPY、真实配置/补发、每个硬件DONE与最终quiet。命中按接受的逻辑请求计；未接受的独立命中读探测单列。

| 变体 | 组织 | R | V/A遍历 | 完整返回 | 有效项淘汰 |
|---|---|---:|---:|---:|---:|
| v0 | P | 1 | 2/2 | 76331 | 0 |
| v0 | P | 16 | 2/65 | 101334 | 58 |
| v0 | P | 8 | 2/37 | 90290 | 30 |
| v0 | PN | 1 | 2/2 | 76869 | 0 |
| v0 | PN | 16 | 2/65 | 130104 | 58 |
| v0 | PN | 8 | 2/37 | 106570 | 30 |
| v1 | P | 1 | 2/2 | 76331 | 0 |
| v1 | P | 16 | 2/65 | 101334 | 57 |
| v1 | P | 8 | 2/23 | 84581 | 15 |
| v1 | PN | 1 | 2/2 | 76869 | 0 |
| v1 | PN | 16 | 2/65 | 130104 | 57 |
| v1 | PN | 8 | 2/23 | 94481 | 15 |
| v2 | P | 1 | 2/2 | 76331 | 0 |
| v2 | P | 16 | 2/65 | 101334 | 51 |
| v2 | P | 8 | 2/9 | 79059 | 0 |
| v2 | PN | 1 | 2/2 | 76869 | 0 |
| v2 | PN | 16 | 2/65 | 130104 | 51 |
| v2 | PN | 8 | 2/9 | 82733 | 0 |

阶段账分两部分：慢设备连续父任务的ready→issue→backend→first R→source complete→last B→DONE，加最后DONE→quiet，严格等于完整返回时间；翻译pending、PTW active、payload读写占用、完成发现另按时间区间并集报告，同时保留相交长度，不能相加为节省时间。

SID来自同周期上层fill脉冲及唯一活跃PTW owner的DID字段，IOTLB模块本身没有SID输入；关联后的原始字段、事件行号和周期转换保存在victims/及cases/。

完整主结果见comparison.csv；页级命中/缺失/遍历见pages/；各attempt错误、时间账、PTW与真实DDR事务、原始哈希见cases/和summary.json。

重复v0-p-r8：2次，完整返回范围[90290, 90290]，结果及父/事件字节一致=True。

重复v0-pn-r8：2次，完整返回范围[106570, 106570]，结果及父/事件字节一致=True。

重复v1-p-r8：2次，完整返回范围[84581, 84581]，结果及父/事件字节一致=True。

重复v1-pn-r8：2次，完整返回范围[94481, 94481]，结果及父/事件字节一致=True。

重复v2-p-r8：2次，完整返回范围[79059, 79059]，结果及父/事件字节一致=True。

重复v2-pn-r8：2次，完整返回范围[82733, 82733]，结果及父/事件字节一致=True。

<!-- paired-effects -->

R8的直接观察：V0有39次填充，其中30次淘汰有效项；淘汰条目仅way2/way3各15次。A固定源在way8，目的页0/2/4/6各8次遍历，1/3/5/7各1次；最终有效位0x01ff，获配但未使用的way9..15共7个。

V1改成分区紧凑树后为25次遍历，仍有15次有效淘汰（均way2）；仅目的页0/4各8次，其他目的页各1次。14叶树仍非完全平衡，不能把V1称为精确LRU。V2为11次冷遍历、零有效淘汰，A全部8个目的页及固定源各1次。

P下V1比V0完整返回少5709步（6.32%）。慢设备V的source complete→last B减少6944步，同时backend→first R增加1241步；first R→source complete及last B→DONE均不变，其余小差项见stage-comparison.csv。单PTW真实active为23734→17942步，这些区间与payload重叠，不能另加到完整时间收益中。
P下V2比V0完整返回少11231步（12.44%）。慢设备V的source complete→last B减少20327步，同时backend→first R增加9096步；first R→source complete及last B→DONE均不变，其余小差项见stage-comparison.csv。单PTW真实active为23734→5826步，这些区间与payload重叠，不能另加到完整时间收益中。
PN下V1比V0完整返回少12089步（11.34%）。慢设备V的source complete→last B减少17979步，同时backend→first R增加5846步；first R→source complete及last B→DONE均不变，其余小差项见stage-comparison.csv。单PTW真实active为40025→27849步，这些区间与payload重叠，不能另加到完整时间收益中。
PN下V2比V0完整返回少23837步（22.37%）。慢设备V的source complete→last B减少43530步，同时backend→first R增加19651步；first R→source complete及last B→DONE均不变，其余小差项见stage-comparison.csv。单PTW真实active为40025→9521步，这些区间与payload重叠，不能另加到完整时间收益中。

R1与R16的三个变体遍历数及完整返回均相同；改善限于本轮R8可容纳足迹。R8启用独立命中读通路相对仅分区的完整返回差仍为V0 16280、V1 9900、V2 3674步，精确LRU没有消除全部组织差异。

18个配置及6个必要重复共24次运行全部通过原始父任务、数据、握手、翻译和victim逐笔核对。V0六个条件的完整返回、完整结果JSON、parents和events原字节均精确复现历史；六对R8重复也各自结果及parents/events字节完全一致，观察到确定性、无测得波动。

<!-- lookup-tables -->

汇总查数表（只导出现有18个主结果，不重新运行仿真或归约）：[逐设备命中/缺失/遍历](device-translation.csv)（36行）、[逐设备翻译、PTW、payload与完成发现时间并集](device-timelines.csv)（36行）、[每配置慢设备连续里程碑及最终尾段](complete-stages.csv)（18行）。字段口径及输入hash见[table-exports.json](table-exports.json)，可运行export_tables.py重新导出。

命中表仅计实际接受的逻辑翻译：首次普通查找及接受的独立命中读；未接受探测与填充后重查不算新请求，每设备命中+缺失=接受数。SID来自这些实际请求的DID字段。翻译pending为[请求接受,响应接受)，不含接受前背压；PTW active按真实非IDLE状态计。payload读为[DDR AR接受,RLAST接受+1)，写为[DDR AW接受,B接受+1)，包含排队、服务和最后响应采样步，并非纯总线busy；完成发现为[硬件DONE,软件精确DONE轮询结束)。这些区间并集可重叠，跨列或跨设备都不能相加为完整时间或收益；只有慢设备非重叠里程碑表的各阶段加最终DONE→quiet尾段严格相加为完整返回。

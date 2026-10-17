# A失效变慢与死翻译移除：最终诊断

最终`build02`的28组均通过，本报告直接计算[最终campaign](../runs/final-28/campaign.json)、各组`events.csv`和`command-latency.csv`。它们是固定F5修复基座、16项原PLRU、两真实iDMA和单FIFO行为DDR的局部仿真，不是Linux/CPU或板测。旧m1/m2/m3诊断保持原PR=PW=0及两fence过程；新ML矩阵另带明确PR/PW测试overlay，二者不混算。

## (a) 单跑的粒度处理增量确为0拍

A-alone包含same/different两类、m1宽/m2 PSCID/m3 VA+PSCID，共6组；每组256轮失效。**三种失效每一轮的submit→第一fence完成都是62拍，IOTINVAL decode→第一fence完成都是30拍，实际inval脉冲→第一fence完成也都是30拍。** 四个“精细减宽”配对的逐命令差值，256条的最小、最大、总和全部为0；没有用平均值掩盖正负抵消。

六组A硬件最后DONE均75,937拍，A最终失效结束均76,150拍；A128次walk也完全相同。故在这个RTL/命令过程/存储模型下，“范围更精细本身需要额外处理拍数”的实测增量为精确0。

边界：这不是所有IOMMU或所有时钟/物理实现的零成本证明。RTL中三种范围仍是同拍组合匹配，FPGA关键路径和频率没有在这里测量。

## (b) 这批mode差分100%出现在共跑交互中

下表完整A时间都取最后失效结束，而非最后DMA DONE。

| 配对 | A-alone精细−宽 | 共跑精细−宽 | 差分交互项 |
| --- | ---: | ---: | ---: |
| same：m2−m1 | 0 | 0 | 0；百分比不定义 |
| same：m3−m1 | 0 | +19,512 | +19,512；100% |
| different：m2−m1 | 0 | +19,512 | +19,512；100% |
| different：m3−m1 | 0 | +19,512 | +19,512；100% |

共跑宽A为194,749，精细为214,261，增加10.02%；A-alone都是76,150。这里的100%严格指`(共跑mode差−单跑mode差)/共跑mode差`。每组只运行一次、确定性精确比较；不是置信区间，也不是声称100%额外周期都由某一种DDR阻塞造成。单跑没有B预热，共跑包含相同的B预热前缀，因而差分条件还包括由该前缀形成的缓存/替换状态；不能把交互项扩大成一个孤立的内存资源因果比例。

## A完整时间的互斥占用分解

下表b8的“共跑精细”同时代表same-m3、different-m2/m3；same-m2与宽相同。每拍只能计入一个主类，三列各自严格加回完整A时间。

| 逐拍主类 | A-alone，三mode均相同 | 共跑宽 | 共跑精细 | 共跑精细−宽 |
| --- | ---: | ---: | ---: | ---: |
| host/config/其余 | 2,237 | 4,028 | 3,706 | −322 |
| flush忙 | 26,112 | 99,110 | 136,964 | +37,854 |
| COPY中A PTW活跃 | 10,112 | 43,628 | 35,125 | −8,503 |
| COPY中A payload待R，且未归PTW | 2,048 | 2,048 | 2,048 | 0 |
| 其他COPY | 35,641 | 45,935 | 36,418 | −9,517 |
| 完整A | 76,150 | 194,749 | 214,261 | +19,512 |

A PTW桶含walk启动、AR握手等待、PTE返回等待和PTW计算；它不是纯PTE内存阻塞。payload待R桶包含已进readq但尚未收到A R的周期，也含队首startup；有PTW时优先归PTW。其他COPY可以包括payload服务、写腿、B响应或别的进度。以上是固定优先级的状态占用，不是分别可回收的收益。

flush再按CQ状态互斥拆分：

| flush内类别 | A-alone | 共跑宽 | 共跑精细 | 精细−宽 |
| --- | ---: | ---: | ---: | ---: |
| CQ FETCH/REGISTER | 20,736 | 89,578 | 128,081 | +38,503 |
| IOTINVAL DECODE | 256 | 256 | 256 | 0 |
| IOFENCE DECODE | 512 | 512 | 512 | 0 |
| 其他flush，含IDLE/host轮询间隔 | 4,608 | 8,764 | 8,115 | −649 |
| 合计 | 26,112 | 99,110 | 136,964 | +37,854 |

上一轮原trace还能把FETCH/REGISTER分开：b8宽FETCH895/REGISTER88,683拍，精细FETCH772/REGISTER127,309拍；DECODE都768拍。新增38,626拍集中在REGISTER等命令读数据返回；FETCH反而少123拍。旧trace的精确区间计算仍保留在[prior-cycles.json](prior-cycles.json)，没有被本轮覆盖。

逐命令第一fence时间也显示等待位置发生改变：共跑宽submit→firstfence平均334.617拍（66–615），精细437.039拍（62–599）；但decode/inval→firstfence反而由平均164.168拍降至47.797拍。不能把完整A变慢改写成“每一段命令时间都变慢”；主要等待移到失效解码之前的CQ取数。

## B payload是某些CQ读的确定FIFO前驱，但只给下界

原日志没有每拍DDR R事件，不能唯一重建每个请求实际RLAST时刻，也不能把所有REGISTER时间精确分摊给B。可以使用服务模型和已接受AR给保守下界：256beat请求接受于`tB`后，即使立即成为队首、无背压，也要16个完整startup周期加256个beat，最早RLAST为`tB+272`；在此之前接受的CQ请求必仍排在该B请求之后。

具体例子来自[concurrent-different-b8-m2/events.csv](../runs/final-28/concurrent-different-b8-m2/events.csv.gz)：

- 周期48,814，B job0的第二个256beat payload读在DDR接受，地址`0x80000800`。其最早RLAST是49,086。
- 周期48,822，CQ读在DDR接受，ID51、地址`0xa0080020`，长度两beat。此时B最早还要264拍才能完成，所以CQ至少等到这个B完成下界之后才能做自己的FIFO队首服务。
- 对应CQ REGISTER从48,819到49,374，共555拍。264是一个确定的前驱等待下界；没有把555拍全部归B，也没有把下界与其他重叠桶相加。

这类可直接证明的前驱见证在六个共跑legacy诊断中共660条：宽/same-m2各63条，细粒度各157条。完整逐条记录在[cq-fifo-witnesses.csv](cq-fifo-witnesses.csv)。某条未被这个简单下界捕获，不代表它没有B前驱；脚本只选“即使最快也尚未完成”的可证情形。

## ML-FQ与ORACLE的服务事件确实逐周期相同

ORACLE仅在unmap后下一沿额外清valid，后续FQ容量、10ms计时、域失效和PR/PW fence与ML-FQ完全相同。四对比较均没有首次服务分歧：

| domain / B页数 | 每边逐条匹配的服务事件数（含warm） | A/B完成时间差 | A/B walk差 |
| --- | ---: | ---: | ---: |
| same / 4 | 56,348 | 0 / 0 | 0 / 0 |
| same / 8 | 59,034 | 0 / 0 | 0 / 0 |
| different / 4 | 56,348 | 0 / 0 | 0 / 0 |
| different / 8 | 59,034 | 0 / 0 | 0 / 0 |

比较包括lookup的时间、key、hit/miss（只屏蔽归因cause位，保留PSCID）、DDR AR/AW/W边沿/B、COPY launch/DONE、walk/PTE读、MMIO、CQ提交/doorbell/decode/fence、FQ入队/释放/超时；每对序列SHA256相同。没有要求ORACLE删除事件、tag快照或victim“是否死/是否valid”等观察值相同，它们正是受干预的状态。完整类型计数和相等hash在计算JSON中。

因此，即使FQ保留了死A翻译，把这些valid立即清掉，也没有改变本轮任一关键服务事件。源码解释是PLRU victim不看valid；被清除的A key不再访问，不改变活跃lookup命中或PLRU树，后续fill仍覆写同一way。本轮没有添加空项优先或修改PLRU。结论只限制这个实现和负载下的死容量收益，不宣称所有替换器都如此。

## 可重算入口

源脚本：[analyze_results.py](analyze_results.py)。从最终campaign执行后保存：

- [results-calculations.json](results-calculations.json)：28组来源、hash、逐指标统计、单跑逐命令零差、交互差分、FQ/ORACLE服务序列比较。
- [command-calculations.csv](command-calculations.csv)：逐命令原始时间与decode/pulse后延迟计算。
- [wall-calculations.csv](wall-calculations.csv)与[interaction-calculations.csv](interaction-calculations.csv)：互斥桶及差分的完整数字。
- [cq-fifo-witnesses.csv](cq-fifo-witnesses.csv)：可证明的FIFO前驱与最小完成下界。

脚本检查28组passed、summary hash、命令计数与时长守恒、wall之和、flush之和及每条decode/inval同拍；没有重跑仿真或修改原始数据。原CSV若由归档脚本无损gzip，源脚本也支持直接读`.csv.gz`。RTL解释、实际frozen源行号及测试hook边界见[rtl.md](rtl.md)。

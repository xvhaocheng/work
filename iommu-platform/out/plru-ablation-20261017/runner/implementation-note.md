# 运行与归约说明

本轮仅在独立输出目录复制10月17日双真实iDMA仿真环境。原85个SystemVerilog输入及原C++文件共86项均与旧`build01/build-manifest.json`的SHA-256精确一致，见[source-baseline-check.json](source-baseline-check.json)。`shared-overlay/`是旧overlay的逐字复制。新构建依次选择各变体overlay、shared-overlay、原平台源；构建脚本强制原85项逻辑路径全部存在，仅允许IOTLB文件与原hash不同。三个构建均保存实际输入路径、hash、命令、构建日志和本机二进制。

C++与旧版唯一的行为外改动是在reset释放后、首个released上升沿之前打印`IOTLB_CYCLE_ANCHOR,<absolute_cycle>,0`。不增加时钟步、eval或DUT赋值。IOTLB只读计数器在reset期间清零，首个released上升沿显示0。因此相对t0周期为`local_cycle + anchor.absolute_cycle - anchor.local_cycle - result.t0`；本轮offset为−26。归约还要求每条victim记录与同周期上层fill脉冲精确相等、与唯一活跃PTW的VPN及owner相符。SID取上层owner的DID；不声称IOTLB存在SID输入。

`execute.py`只接受root已审核回归且hash相符的变体及通过构建的二进制，保留每次attempt。每次运行的硬超时为120秒；仿真还保留原100万总步与每lane20万无进展看门狗。构建硬超时900秒、最多8个编译进程。`matrix.py`只补齐固定18项主配置及各变体R8 P/PN各一次重复；已有失败attempt时停止，不自动重试。

原始stdout、stderr、victim行、parents、events、结果与receipt保留在每个attempt。events同时保留本机CSV和确定头部的gzip；生成后实际解压核对字节数及SHA-256。`verify_artifacts.py`在完成后再次逐文件核对receipt、源码输入、二进制与gzip字节恢复，输出`analysis/artifact-verification.json`。同步可排除原始events.csv与obj构建物，保留gzip及全部其他记录。

新`analysis/analyze.py`调用`parent_reduce.py`中继承的父任务、真实DDR握手和数据归约。后者取自旧`analysis/analyze.py`，只保留共同归约函数；旧报告生成与旧结论未执行。新归约逐lane强制接受请求=命中+缺失=256，且缺失=唯一PTW遍历；接受的独立命中读计为命中，未接受的探测不计入主命中/缺失。每条victim验证有效位、历史驻留VPN、eligible位、fill脉冲与walk守恒。

完整时间按最终慢设备连续父任务的ready→issue→backend→first R→source complete→last B→DONE，再加最后DONE→quiet拆分，各段严格相加为完整返回时间。翻译pending、PTW active、payload访存占用、完成发现另按半开区间并集及交集记录，不能相加为收益。PTW active直接使用`ptw_state != IDLE`的状态变化区间，并强制与原`raw.ptw_active_ticks`相同；发起脉冲比active状态早一步，未将这一额外步算入active。

重放分析运行`python3 analysis/analyze.py`；仅有gzip时相同脚本自动读取gzip。逐配置结果在`analysis/comparison.csv`，全attempt明细在`analysis/cases/`。V0另外逐配置核对历史完整返回、完整结果JSON、parents原字节、events解压原字节；R8重复比较结果JSON及父/事件原字节。

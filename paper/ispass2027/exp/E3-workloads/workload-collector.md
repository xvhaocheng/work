# 完整E3采集协议与候选150点

后续窄软件profile准备见[两个精确profile](workload-software-profiles.md)：保留原65b路径，新增Root要求的正式G软件，
实际硬件仍只接纳1d587。下列原126/5/30项冻结证据原样保留，不冒充新profile的执行源。

`collect_workload.py`只供Root执行，A2没有访问硬件。它使用独立ABI5 CLI，绝不改P045。
默认新协议：V有效16MiB＋V一完整几何环预热；burst预期由几何源码模型与已审真实backend有限证据共同约束，
再独立检查实际AXI计数。64/1024/4096来自A1 run03，16/64KiB来自long04，256/1536槽位来自packet01。
零长／NO_ERROR_HANDLING／backend非driver STOP的已知边界保留在manifest绑定的原backend summary，不声称覆盖非法域。

## 计划与时间预算

固定30 cells×5 block，共150候选新点：六size×ring V×Solo/A1/A8=90；
canonical NIC256B、storage64KiB、tile16KiB，各对Solo及三种canonical A=60。
新P1最小环与canonical几何分开记录；不与旧P045合池。各block旋转size/条件顺序，保留同block可配对关系。

可先用`--max-cases 30`完成第一完整block，manifest记partial_limit。
Root根据实际`wall_seconds`／每点命令耗时预估全部150点和bit/boot开销，决定≤6h论文子集，
未有实际数据前不以P0耗时承诺150点板时。若执行续接，`--resume-run`只读旧成功prefix，
不覆盖活动文件、不再执行已完成点；原run状态必须partial_limit/partial_budget/passed。
任何failed run（含softlockup taint）不能静默续接或替换失败点；新轮必须由Root绑定新source/boot/run。

默认max-wall-seconds21600，启动下一点前保留130s余量；续接累加先前collector wall time。
这只界定采集程序的预算，Root仍须计入BIT/boot/上传等板时。程序按现有90s timeout＋120s UART命令边界执行，
不增加sleep或降低数据、PATH、HPM、排空、taint检查。每个点失败即停止，原raw和成功prefix保留，无RUN重试。

## 身份与观测

实际只接受已审新软件tuple65b427/ec99648，CVA1d source-reference，release full-e3-v1；
正式full-E3 BIT须自身passed、DS0/50MHz、BTC3/OBS5/Depth64/INFO2及loaded BIT SHA一致。
FPGA原manifest的Linux da6 reference不改；软件CVA source-reference不冒充上板BIT。

`workload_records.py`核ABI、每lane真几何/row长度、actual completed×row_bytes、warmup与measured distinct页/字节、
verified union。A actual prefix包含startup/final drain，不填整tile，不按V count推断A工作。
完整16段PATH库存/级间守恒及V全窗完整性保留；shared AR/AW只核PATH0 full+carry之和，
private-vs-shared有符号差仅诊断（见Root的board/observer-shared-counts.md）。
重新检查boot、TAINT前后均0、CLI退出0、fresh sequence与返回observer_sequence一致。

复用A1已审V4/V5 `hist_records.py`的字节相同副本`source/hist_records.v5.as-reused.py`，不改共享helper。
ABI5另严格要求info version5/schema2/depth64/raw0x0040000020202002；stage仍schema1。
32桶×16stage计数与PATH full/sum/max一致，overflow、缺失段、未来seq均拒绝。

## 实际host验证

[126项协议检查](test-results/workload-records02-report.json)以真正CLI main的accepted fixture构造合成UART，
使用真实有限backend loader。包括各合法几何、partial A、1536不同burst、bytes/coverage/union、
合法closing spill差、共享/私有各自守恒、未知版本/depth/format、taint16384和fresh seq反例。
原01测试把“未来seq”放到TAINT_AFTER之外，该行按合同不属于dmesg，造成测试自身预期错误；
[原失败说明](test-results/workload-records01-failure-derived.json)保留，修正fixture位置后02通过，未为此改生产parser。

[实际collector模拟串口](test-results/workload-collector01-report.json)通过5项完整入口检查：
30点试跑、只读prefix续接仅执行剩120点、每30cells的n均5、首个taint错误停止保留2点和原失败raw、
failed run不得续接、默认生产入口拒绝任何bound fixture。
[原始执行输出](test-results/workload-collector01-run.log)保留。internal测试模式不能由CLI开启，
它始终传播fixture_only=true及synthetic evidence_level到manifest和每条record；不能发布为FPGA数据。
回执中的collector/parser SHA与本批提交源逐字节相同。

本批没有新FPGA实测。正式命令由Root在实际full-E3 BIT与新boot0冻结后执行；
所需参数为output、load/fpga/software manifest、三份backend evidence、board-tools，output必须新建。

## Root审查后的聚焦身份修订

原126项/5项结果与9b中间源码原样保留。本批只改host collector，不重编Image/不改driver或RTL。
当前collector SHA=`dc2802d0ff240dd0801a0db6f49bb7372ee4d71e45f43040b28f78ac4a3c7fcc`。

resume逐项比较schema、四工具SHA（script/parser/hist-helper/geometry-model）、protocol、P0独立标记、
kernel、boot、BIT、ds、clock、evidence_level、fixture_only、计划及所有bound source路径/SHA。
max-cases可增大，不能缩小原target；原source改变不能沿用旧prefix。
现场与resume共用accept：raw.command须逐字等于该planned point的实际shell命令，
cached数据须与原raw解析一致，同boot整个prefix＋后续seq严格连续。首点仍依真实PRE_SEQ核fresh＋1。

实际启动前递归识别E2/E4已接受的fixture_only/is_fixture/simulation_only/synthetic_fixture/mock_fixture、
synthetic/mock/fixture evidence标签、HOST-FIXTURE boot；并识别非空fixtures容器。
标记可藏在dict/list任意层，默认actual入口在构造UART/输出前拒绝；解码后trial同样检查。
internal test模式仍只能代码调用且始终synthetic，不能由CLI开放旁路。

[30项actual collector入口修订回归](test-results/workload-collector02-report.json)通过：
原partial30→120完整续接/首错停止/failed不续接全部保留；每个冻结字段独立篡改均在UART前拒绝；
现场及prefix命令篡改、prefix/live/续接边界跳seq、cached-vs-raw不一致拒绝；
nested flag、synthetic evidence标签、fixtures容器均在actual-start拒绝。
[完整新原始输出](test-results/workload-collector02-run.log)与as-tested source snapshot另存。
[独立gap定向检查](test-results/workload-resume-gap03-report.json)还将cached和raw一起改成合法seq3，
二者一致、单点fresh有效，仍因全prefix1→3断档在UART前拒绝，证明未仅靠cache不一致挡住反例。
没有新增板测、没有重试错误点或发布synthetic为paper data。

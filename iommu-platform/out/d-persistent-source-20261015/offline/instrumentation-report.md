# MQ-BOUNDARY-1 被动观测与输入交付

补丁已完整编译，退出码0，耗时11.404秒；**模拟器运行数0，8个case均未开始，等待root审查**。

## 合同与校验

先读取 `../root/boundary-contract.json` 并通过其SHA256校验，固定hash `0ae19e94432a22d2173fa2116687f58642ee1246722fe327972cce95d16d1a70`。固定源码commit为51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16。原目录与原binary全部保留；构建后核对175个原文件及62个原构建产物，改变均0。

`instrumentation.patch` 是相对原源码的unified diff，SHA256 `55c51e05dd472b592280633455717c06824e6c3e6970a28bef211b0eea245759`。`instrumented/` 为完整私有副本，但复制时排除原build/和MQSim；构建前二者均不存在，不会使用旧对象。只改9个源文件/头文件（包含新observer头），Makefile、main和默认处理均未改变。

`instrumented-prebuild-manifest.json` 在本次构建前记录186个文件：完整副本原始文件+新头、patch和全部四case配置/trace。其hash `65fde22d9ef4cd308b66f4ca5d4b4933b23828a93b5b970af72976f3ef08f24c`，构建后所有预登记文件改变0。

## 被动观测含义

`src/d_boundary_probe.h` 的C++11 inline函数静态State在程序中共享一个实例，**仅支持本合同单flow、单scenario、每case新进程**；未实现跨scenario observer复位。

- 每次PCIe push累计pending；本次Consume及其触发的后续事件安排全部结束后减pending并读两条实际队列size。Consume期间新增消息仍计入pending，防止queue先pop产生假排空。
- Trace生成至target后设generation_done；生成时及PCIe_Root_Complex的virtual CQE消费完整返回后，读取generated/serviced/target/waiting/active软件队列。派生trace的CQ head doorbell已在此之前发出。
- GC start在原Ongoing_erase_operations.insert后记录，end在原ERASE移除后记录；保存channel/chip/die/plane/block，独立set对重复start、无对应end设observer_invalid。覆盖静态WL插入点，但合同输入禁用静态WL。
- GC计数为**保护中的块维修operation**，包括尚未执行、搬页读写、擦除各阶段，不能称NAND erase busy。
- 完整event callback返回后判断host边界；generation done、gen=served=target、waiting=active=0、PCIe pending及两队列0、observer有效时仅记录首次边界。退出记录最终状态、剩余事件数和stop标志，不改变引擎停止行为。
- host payload接收仅在原no-op分支记录事件，不宣称实际数据字节或真实DMA。

新增输出只写stderr NDJSON；不注册模拟事件，不改变原state/事件调度/stop。observer计数单独维护，下溢只记录invalid，不干预模拟器。实际被动性及语义等价还待root审patch与授权的4组对照验证，不能由编译成功代替。

## 输入与实际参数

`inputs/ssdconfig.xml` 保留官方配置其余字段，仅按合同覆盖几何与选项；四份 `inputs/workload-N.xml` 各1scenario/1TRACE_BASED flow，TURNED_OFF、资源全0、occupancy0、execute100、Relay_Count1、NANOSECOND。File_Path是四份trace各自的绝对路径。

`inputs/write-N.trace` 长度512/2048/4096/8192；前512条顺序页0..511，之后以Python Random(913)随机覆盖，复用最长公共前缀；LBA=page*16、size16 sectors，arrival=100000+i*100000ns，type0写；全行已按公式检查。完整XML参数与输入验收见 `input-validation.json`。

SSD_Device.cpp原样传maxGC=10；GC构造中软阈值max(floor(64*0.05),10)=10 blocks，硬阈值max(floor(64*0.005),1)=1 block。没有将配置比率误报为实际软阈值3，也没有修改上限。NAND时间沿用示例，不是实机校准。

## 构建及账本

命令：`timeout --signal=TERM --kill-after=5s 180s make -j2`，cwd为instrumented/；必要环境与工具版本沿用同host本次先前留存的 `build-environment.json` / `gpp-version.txt` / `make-version.txt` / `timeout-version.txt`。完整输出 `instrumented-build.log`，实际结果 `instrumented-build-result.json`，全部62个新产物hash `instrumented-build-artifacts.json`。

binary `instrumented/MQSim`：14428320 bytes，SHA256 `8c91050e57825e80eda4016c0b1f333a9b31ebc3e79b306cb26a8dd577c510a4`。

准备过程一次assert检查发现payload no-op文本有两处，未执行编译；保留半成品 `instrumented-preparation-failed1/`。随后将匹配限定Write_to_memory分支重建副本；初步静态检查补加Engine头后生成最终patch和prebuild清单，首次实际编译通过。未运行任何MQSim程序，包括help。

requested model/effort: gpt-6.1-sol/medium；actual model/effort unknown（无可核运行元数据）。共享研究Codex-only，无DeepSeek调用。全部写入offline/，未改canonical/生产Linux/RTL/ABI，未恢复MIG1792023314或旧实验。

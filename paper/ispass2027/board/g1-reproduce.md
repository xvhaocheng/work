# G1历史复现与统一基线

本轮由Codex实际执行构建、JTAG/UART控制和板测，用户已授权。**历史G1独立9次已通过；统一基线正式100次已全部完成，两种DS下五条件各10次。** 执行状态与原始回执在[runs](runs/)；构建、真实启动和正式样本分别记录。更新日期2026-10-17（JST）。

用户最新要求按配置集中测完，再切另一套。后续100次默认DS50的50次→DS0的50次，
两者共用Linux，不再默认回切；同配置内仍轮换五条件。本轮调整提出时已经进入DS0，
因此保留实际ABA的25/50/25安排并全部完成，没有重测或重新启动另一轮100次。

## 现存合格配对

以下hash已在Dell实际重算。首选batch配对的本轮真实加载身份见后述G1回执；备选文件的存在不代表本轮加载过，也不倒填旧实验的板端下载身份。

| 用途 | 文件（相对工作区根） | SHA256 |
| --- | --- | --- |
| G1首选DS50 batch BIT | `iommu-platform/out/e4-batch-20260916/delivery/ariane_xilinx.bit` | `59ebd4c76d63941b2115d01c736649f2b409b5615f21bd3937a4f9f0524fcdf2` |
| G1软件00082 | `iommu-platform/out/e4-batch-20260916/delivery/fw_payload.bin` | `c18a7680d8f69f54b08f6752322a949bacbce24b5872b67eab44f9edb363c017` |
| DS50 path备选BIT | `iommu-platform/out/e4-path-20260916/delivery/ariane_xilinx.bit` | `62f822d19851da0309b1a7e085197661caa72f6366fccf34d721fa418b941c29` |
| path软件00083 | `iommu-platform/out/e4-path-20260916/delivery/fw_payload.bin` | `031025c1fb1551ab0e82610027f76106e7b2efe3052b45cda62ea39dc0173c6a` |
| 历史DS0 batch BIT | `iommu-platform/out/baseline-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit` | `c6555d14245935820224a2cae2d835a65bade81fe993d5f8d626ec0ff2724050` |

三者最终routed报告均WNS **+0.177ns**，setup失败端点0；batch50/path50/batch0的WHS分别+0.020/+0.024/+0.040ns，WPWS均+0.062ns。报告位于各profile的`fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt`第159行，profile依次为`migration-4dma-batch`、`migration-4dma-path`、`baseline-4dma-batch`。时钟50MHz。仍有clock/CDC/IO方法告警，不称完整板级签核。

身份来源：[batch交付](../../../iommu-platform/out/e4-batch-20260916/delivery/delivery.json)、[path交付](../../../iommu-platform/out/e4-path-20260916/delivery/delivery.json)、[历史DS0 manifest](../../../iommu-platform/out/manifests/fpga-baseline-4dma-batch.json)。00082预期release为`6.5.0-rc2-00082-gb7979a1a06ac`，00083为`6.5.0-rc2-00083-g45bf76b15e36`。

## Dell部署与返回通路

1. UART为`/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AV0KRCHT-if00-port0`，115200/8N1。`uart.py`使用TIOCEXCL及主机锁，不改变DTR/RTS；新输出目录必须不存在。初查已取得R107内核及boot ID，无实验进程，taint0。
2. 直连以太网Dell为192.168.77.1，板为192.168.77.2；初查R107时网络可用，历史00082/83没有当前LowRISC网口驱动。G1及随后pathv3基线使用UART，不依赖SSH。
3. 本轮已只读核GPT：第一分区起始2048、65536扇区，即32MiB；第二分区ext4只读挂载失败，未格式化/改分区。32MiB原启动分区备份及当时的板端/主机SHA已经保存，原始备份不进Git。这是已完成的历史操作，不是后续默认重做步骤。
4. UART返回通路已经用全字节值文件验证，历史payload中也已确认所需BusyBox applet存在。`uart.py --upload`仅创建`/tmp`文件，**本身不写SD**。后续默认不再做remote SHA或重复传输测试；本次已经发出的压缩payload传输按原协议完成，不中断重发。
5. 传输完成直接解压，然后运行`platform-sd-update --device /dev/mmcblk0p1 --image FILE`正式写卡，再启动并进入矩阵。保留更新器内置检查、写入错误停止和日志；不另加dry-run、镜像SHA或SD整分区回读。更新器本身不重启。
6. JTAG只加载易失BIT。`program_and_boot.py`复用Vivado原生hw_server流程，唯一选择Digilent序号`200300BD5148B`和`xc7k325t`，在加载前核实际文件SHA，保存命令/结果及启动UART，再核release、boot ID和taint。它不声称配置readback hash已验证。现在支持`--credential-helper`，只在观察到root的Password提示后调用既有helper，私密接收认证阶段，避免意外回显进入raw日志；一次拒绝或超时即停，不自动重试。

所有已执行细节、超时和原始日志见`runs/preflight-20261017-01/`。历史做过的回读据实保留；后续直接引用已有构建时序与manifest身份，不默认下载核hash、重复回读SD、重跑短回归或额外重审DCP。任何超时先被动收尾，不能把未结束的SD操作自动重交。

既有本地helper的非秘密路径为
`iommu-platform/out/r107-execution-gate-20261012/route/login_current_board.py`。
它通过`supplied_board_credential()`在内存中复用已有用户供给凭据；不把helper内容、
凭据或认证会话复制进文档。该路径需要在执行主机存在。纯host四项PTY验证见
[auth-validation.json](auth-validation.json)，不能替代板端identity检查。

长UART操作可用[dispatch.py](dispatch.py)启动**一个有限任务**，避免主机工具会话超时
直接终止子命令。它不是自动重试器或常驻调度器。返回`dispatched_only`只说明已派发；
必须检查对应`result.json`的`returncode`，以及子命令自己的load/upload/采集回执。
`result.json`缺失时不能推定成功或失败。超时、host lost或失去前台连接后，先只读检查
已有PID、进度和日志并收尾原任务；占用UART的任务未结束前不要另开控制或重新派发。

以下是从工作区根未来重新加载历史配对的命令例，使用对应已就位的SD软件，不另加镜像回读。`NEW`必须
替换成未使用的新名字，所有输出目录均须不存在。**它不是本轮已经执行的命令回执。**

```sh
python3 paper/ispass2027/board/dispatch.py \
  --output paper/ispass2027/board/runs/g1-load-host-NEW -- \
  python3 paper/ispass2027/board/program_and_boot.py \
  --bit iommu-platform/out/e4-batch-20260916/delivery/ariane_xilinx.bit \
  --expected-sha256 59ebd4c76d63941b2115d01c736649f2b409b5615f21bd3937a4f9f0524fcdf2 \
  --expected-kernel 6.5.0-rc2-00082-gb7979a1a06ac \
  --credential-helper iommu-platform/out/r107-execution-gate-20261012/route/login_current_board.py \
  --output paper/ispass2027/board/runs/g1-load-NEW
```

加载任务成功结束后直接采集，采集器读取该`load.json`；无需再加人工确认。
仅重新连接一个既有Password提示、没有捕获fresh boot/login
的会话，不适用这条完整加载入口；已启动的旧进程也不会自动采用磁盘上的新登录代码。

## 本轮G1已完成的证据

接受目录是[runs/g1-20261017-03](runs/g1-20261017-03/)，
[review.json](runs/g1-20261017-03/review.json)与
[independent-check.json](runs/g1-20261017-03/independent-check.json)均通过。
加载身份使用[runs/g1-login-20261017-01/load.json](runs/g1-login-20261017-01/load.json)：
首选DS50 batch BIT及00082软件、已验证kernel/boot身份，九个唯一实际命令执行实例
全部处于同一个boot；不是九次独立启动。

| 条件 | 实际样本数 | 硬件时间中位数（ms） | 冻结参考（ms） | 冻结±2% |
| --- | --- | --- | --- | --- |
| Solo | 3 | 56.49366 | 56.5 | pass |
| A1 | 3 | 107.10562 | 107 | pass |
| A8 | 3 | 154.95348 | 155 | pass |

接受记录来自同一boot的sequence 1—9：run01实际执行第一条，parser因JSON行前缀
停止；run02离线接回该条后继续执行其余八条，最后sequence 9的live printk
`part=t`又被误当成额外observer记录，触发`complete observer parts`失败。
run03只从这九条既有原始命令回执离线恢复并重核，**没有打开UART，没有新测量**。
独立检查逐条保存原始来源和SHA，确认九个实际执行实例唯一、恢复新增测量为0。
run01/run02的旧failed manifest和全部raw日志均原样保留；不通过补测或删除失败历史
凑满样本。

原[runs/g1-load-20261017-01/load.json](runs/g1-load-20261017-01/load.json)
对应JTAG实际编程和fresh boot已成功，但旧wrapper遗漏Password处理，随后被host中断，
原failed回执保留。既有login helper一次完成认证，再追加上面的接受load身份；
没有第二次JTAG加载或重启。接受回执保留原回执hash和中断原因，不把旧failed改写成成功。

## G1九次命令与验收

本轮G1已经完成，后续新基线流程不重跑这九次；下列入口只用于另行要求的历史复现。
未来重做G1时，历史BIT/历史软件就位并取得`status=boot_identity_verified`的
`load.json`后，从工作区根运行。这里使用新的输出名，不覆盖本轮接受目录：

```sh
python3 paper/ispass2027/board/collect.py --phase g1 \
  --output paper/ispass2027/board/runs/g1-NEW \
  --expected-kernel 6.5.0-rc2-00082-gb7979a1a06ac \
  --load-manifest paper/ispass2027/board/runs/g1-load-NEW/load.json
```

采集器实际运行以下三条，各3次，按区组轮转顺序；固定V0/A1、4KiB、4096项、预热256。G1与正式100次不混样。

```sh
platform-timeout 120 idma_experiment --batch --device /dev/idma0 --aggressor none --aggressor-pages 1 --victim-pages 1 --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000
platform-timeout 120 idma_experiment --batch --device /dev/idma0 --aggressor 1 --aggressor-pages 1 --victim-pages 1 --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000
platform-timeout 120 idma_experiment --batch --device /dev/idma0 --aggressor 1 --aggressor-pages 8 --victim-pages 1 --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000
```

每次保存原CLI JSON、退出码、完整UART及dmesg。原CLI不携带observer sequence；采集前记录dmesg水位`PRE_SEQ`，成功运行后要求恰好下一个sequence、3条batch记录齐全，同boot身份不变。核数据、quarantine、提交/完成、H+gap、AXI占用/请求返回及A覆盖；不以旧同条件观测替代缺失的新观测。

`collect.py`的`review()`从原始cycles换算硬件ms=`cycles/50000`；每条件必须3个有效试次，比较`|median/reference−1|≤0.02`，reference=56.5/107/155ms。边界用Decimal比较避免浮点误拒。失败样本保留，不删除、补采或挑选到通过。历史实际中位数56.55586/107.09810/154.98571ms另作描述，不擅改冻结门槛。

离线采集器兼容检查见[collector-compatibility](../data/collector-compatibility.json)：真实历史V0 batch/path通过，角色不符、旧序号及缺样负例拒绝。PTY传输协议检查只测试主机代码，不能替代实板结果。

## 新统一基线与100次矩阵

新构建入口：[build_baseline.py](build_baseline.py)，来源：[baseline-source.json](baseline-source.json)，软件：[software-build.json](software-build.json)。固定CVA6修复底座`ba8cdd1`、F5 IOMMU `85b157e`，从`e39dce2`提取8文件批量供给/被动观测增量；DS0为direct wire，DS50为显式50/50额外延迟，16项PLRU，无E3/O1/P/N/PN/banked/新服务规则。软件固定45bf的驱动/UAPI和历史CLI，新release为`6.5.0-rc2-ispass2027-pathv3`，两种硬件共用同一payload。该平台是明确列出必要修复、观测和供给夹具的原版基线，不以关闭研究树P/N开关冒充stock。

两份新BIT已实际完成构建及网表验收，均50MHz、routed WNS **+0.177ns**、
setup/hold/pulse失败端点均0、routing errors均0。DS0/DS50的WHS分别+0.047/+0.033ns，
WPWS均+0.062ns；网表delayer单元分别0/130。完整交付及命令见
[BUILD.md](BUILD.md)、[delivery.json](delivery.json)，原始报告见
[DS0 timing](reports/ds0/ariane_xilinx_timing_summary_routed.rpt)、
[DS50 timing](reports/ds50/ariane_xilinx_timing_summary_routed.rpt)；
对应build manifest与网表核查也由delivery关联。上板直接读取这些已有结果，不要求重建、重跑短回归或独立DCP复审。正WNS限于现有报告约束，不代表完整IO/CDC签核。

| 新产物 | 文件（相对工作区根） | SHA256 |
| --- | --- | --- |
| DS0 BIT | `iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit` | `e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6` |
| DS50 BIT | `iommu-platform/out/ispass2027-20261017-ds50-01/src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit` | `580b34014914a1bee8541ed1e23b36a164bfc0fea3a2f691f56870b44dc35e77` |
| 共用payload | `iommu-platform/out/ispass2027-20261017-software01/fw_payload.bin` | `2de1e1551bc86c9651a39a46a0c4ceac7f630a593766109cadc6027102ceddd5` |

新payload的9,971,990字节gzip文件已经通过UART传完，见
[runs/baseline-payload-upload01](runs/baseline-payload-upload01/)。该在途命令保留旧协议的内置SHA步骤，后续默认不做remote SHA。压缩updater的
309,584字节传输也已完成，见
[runs/baseline-updater-upload01/upload-result.json](runs/baseline-updater-upload01/upload-result.json)，
这些传输回执的`sd_written=false`只描述传输阶段。实际首次updater在写入前因00082没有SPI控制器驱动而停止；
补入同版本上游`spi-bitbang.ko`、`spi-xilinx.ko`后，SD及两个分区已出现。
解包使用`gzip -dc FILE | tar -xf -`，该BusyBox不支持`tar -z`；两次写前失败记录保留在
`runs/baseline-software-write01/`和`runs/baseline-software-write02/`；
[write03实际命令](runs/baseline-software-write03/command-275284c8602d40c4a6a1712da181a211.json)
已exit 0，updater输出PASS，32MiB写入和sync已完成。
驱动源码、构建命令及对应软件包见[恢复说明](recovery/README.md)。模块只用于更新，
新内核正式采集期间未加载，三个正式boot均保持taint 0。新pathv3实际启动身份见
[DS50](runs/baseline100-20261017-01/ds50-first-load/load.json)和
[DS0](runs/baseline100-20261017-01/ds0-middle-load/load.json)，以及
[最后DS50](runs/baseline100-20261017-01/ds50-last-load/load.json)。
[campaign.json](runs/baseline100-20261017-01/campaign.json)已为`completed`，
[launch result](runs/baseline100-20261017-01-launch/result.json)为returncode 0；
三子run均passed，实际100次完成，无新增失败或补采。

本轮实际ABA为`ds50-first`前25次、`ds0-middle`中间50次、`ds50-last`后25次，
共三个boot，实际先后及身份全部保留。旧三阶段入口继续兼容本轮历史执行。

| 实际阶段 | 样本数 | 原始结果 | 启动身份 | 状态 |
| --- | --- | --- | --- | --- |
| DS50前段 | 25 | [records](runs/baseline100-20261017-01/ds50-first/records.json) · [review](runs/baseline100-20261017-01/ds50-first/review.json) | [load](runs/baseline100-20261017-01/ds50-first-load/load.json) | passed |
| DS0中段 | 50 | [records](runs/baseline100-20261017-01/ds0-middle/records.json) · [review](runs/baseline100-20261017-01/ds0-middle/review.json) | [load](runs/baseline100-20261017-01/ds0-middle-load/load.json) | passed |
| DS50后段 | 25 | [records](runs/baseline100-20261017-01/ds50-last/records.json) · [review](runs/baseline100-20261017-01/ds50-last/review.json) | [load](runs/baseline100-20261017-01/ds50-last-load/load.json) | passed |

下表仅展示上述原始记录：硬件ms=`observer.time.cycles/50000`；DS50合并前后原始
样本后取每条件10次中位数，DS0同为每条件10次。它不重用G1样本，也不新增验收。

| 条件 | 每DS样本数 | DS50硬件中位数ms | DS0硬件中位数ms | DS0的V HPM快照中位数：IOTLB miss / S1 PTW |
| --- | --- | --- | --- | --- |
| Solo | 10 | 56.54995 | 56.53868 | 0 / 0 |
| A1 | 10 | 107.00204 | 107.09009 | 0 / 0 |
| A8 | 10 | 154.93961 | 118.22441 | 2 / 2 |
| A16 | 10 | 162.78033 | 121.73598 | 2 / 2 |
| A64 | 10 | 168.70064 | 124.60276 | 2 / 2 |

DS50的V IOTLB miss/S1 PTW快照中位数均0；两DS的V S2 PTW中位数均0。
DS0 A8/A16/A64的2/2按原记录保留。HPM由软件前后采样、非原子，覆盖软件窗口，
不是上表硬件窗口中的精确miss/PTW计数。

本轮[启动时协议](runs/baseline100-20261017-01/protocol-at-launch.json)保存
[原ABA runner](packages/baseline100-20261017/metadata/run_baseline100.aba-as-dispatched.py)、
[前两阶段collector](packages/baseline100-20261017/metadata/collect.baseline100-as-dispatched.py)的身份；
[最后阶段collector原字节](packages/baseline100-20261017/metadata/collect.ds50-last-as-executed.py)
另行配套保存，身份取该run的原manifest。
最后collector仅扩展phase接口，本轮原阶段与测量口径保持。结果、代码、镜像和BIT
集中在[baseline100包](packages/baseline100-20261017/README.md)。

**后续默认AB：一套先测完50次，再切另一套完成50次。** `ds50-all`和`ds0-all`
各十区组，每个BIT只加载一次；从新加载开始时共两次boot。已有有效DS50 boot可用
`--initial-load-manifest`复用，省去首次JTAG加载和启动。两者共用同一已写好的payload，
切BIT不重写SD。DS不是运行时CLI开关。既有启动与采集脚本保留内置身份、正确性及失败停止检查，
不增加独立短功能验收或重复确认关口。由runner调用的两个底层采集接口为：

```sh
python3 paper/ispass2027/board/collect.py --phase ds50-all --output paper/ispass2027/board/runs/baseline100-NEW/ds50-all --expected-kernel 6.5.0-rc2-ispass2027-pathv3 --load-manifest paper/ispass2027/board/runs/baseline100-NEW/ds50-all-load/load.json
python3 paper/ispass2027/board/collect.py --phase ds0-all --output paper/ispass2027/board/runs/baseline100-NEW/ds0-all --expected-kernel 6.5.0-rc2-ispass2027-pathv3 --load-manifest paper/ispass2027/board/runs/baseline100-NEW/ds0-all-load/load.json
```

每区组Solo/A1/A8/A16/A64各一次，顺序按区组轮转；每配置10区组、每条件10次，共100次。
每次仍使用采集器内置的16条同seq PATH记录及阶段守恒检查；A跨窗口的area可大于sum，
不能套用V边界将合法A样本误拒。每条件10次不等于10个独立启动，boot分别保留；
新基线不要求重现旧版本耗时。

[run_baseline100.py](run_baseline100.py)默认完成AB两阶段加载与采集，不自动重试。
常规直接使用以下runner命令，由它调用上面两个采集入口。复用已有成功写卡回执，
同payload无需再次写SD；`NEW`换成未用的新目录名。以下仅是未来另有实验任务时的入口，
不会因本轮完成而再执行。板仍处于本轮最后DS50 boot时，可复用该load回执；需要全新
DS50加载时省略`--initial-load-manifest`。

```sh
python3 paper/ispass2027/board/dispatch.py --output paper/ispass2027/board/runs/baseline100-NEW-launch -- \
  python3 paper/ispass2027/board/run_baseline100.py --output paper/ispass2027/board/runs/baseline100-NEW \
  --software-write paper/ispass2027/board/runs/baseline-software-write03/command-275284c8602d40c4a6a1712da181a211.json \
  --initial-load-manifest paper/ispass2027/board/runs/baseline100-20261017-01/ds50-last-load/load.json \
  --credential-helper iommu-platform/out/r107-execution-gate-20261012/route/login_current_board.py
```

历史G1的独立9次与本轮统一100次均已有实际完成回执；后续实验保留独立目录、代码与配套产物，不覆盖本轮数据。

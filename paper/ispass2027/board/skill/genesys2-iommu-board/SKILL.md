---
name: genesys2-iommu-board
description: Run explicitly requested experiments on the Dell-connected Genesys2 RISC-V IOMMU platform. Reuse board connection, software update, native JTAG boot, logging, and recovery procedures with the current experiment's artifacts, commands, and acceptance rules. Never start hardware for ordinary reading.
---

# Genesys2 IOMMU board

这是本平台后续实验共用的上板流程。先读根 AGENTS、CURRENT_STATUS，再读**本次实验**
的说明、版本清单和运行命令，以当前用户任务确定执行范围。调用本skill不自动授权
新实验，不恢复暂停工作，也不默认要求先跑G1或任何固定矩阵。

G1、DS0/DS50和100次矩阵只属于一个使用实例，见
[ISPASS实验实例](references/ispass2027.md)；仅在本次任务需要该实例时读取。

**按用户最新要求直接推进。** 默认不重复下载哈希、不做SD写后回读、不增加独立回验、
短测或确认关口。使用已有构建清单记录身份，保留程序内置检查、错误停止与全部原始日志。
已有合格产物直接使用；只有明确要求构建时才使用本次版本对应的构建入口。
不将某个实验的源码commit、硬件参数或构建脚本固定为所有未来实验的基线。

所有路径相对 `/home/xvhaocheng/work`。复用 `paper/ispass2027/board/` 的脚本，
每次使用新输出目录，不覆盖历史。

**按配置集中采集。** 同一Linux/BIT上的所有条件和重复一次测完，再切另一套。
能共用Linux镜像时只切BIT；配置已经就位时直接采集。不默认安排回切、逐点重启
或仅为观察漂移增加一次启动。区组轮换在同配置内完成，启动与阶段差异照实记录。

## 配套实验包

为本次实验选定或建立独立实验包；当前包入口为 `board/packages/`，运行记录放
`board/runs/`。每包关联实验说明、结果、实际源码commit/构建清单，以及本地
`artifacts/` 中的image/BIT。不同实验和版本分开，不假定它们共用同一软件镜像。
image指实际写SD的 `fw_payload.bin`，不能拿当前源码HEAD替代运行版本。
二进制留本机，Git同步清单、源码恢复材料与结果。

从当前任务和已有清单取得以下信息，复用现有manifest，不另建审查体系：

- 实验目标、实际源码/构建版本、BIT、image及预期内核版本；
- 本次实验的命令、参数、条件、次数、顺序和输出目录；
- 哪些计数/观测在该版本可用，以及本次结果的判定规则；
- 需要更新软件或BIT，还是复用当前已知配置。

设备角色、传输大小、页大小、工作集、重复次数和通过阈值都来自本次实验。
现有清单足够时直接执行；只有缺失会影响正确执行的关键输入时才询问。

## 按需构建

在记录实际源码身份后，按对应平台版本的构建说明生成产物，使用独立输出，
保留命令、构建结果和时序报告。已有可用产物不重复构建或重新审查。
`build_baseline.py` 与 `board/BUILD.md` 是ISPASS修复基线的专用入口；未来其他
版本使用各自入口，不强制套用该脚本的固定commit或DS配置。

## 连接和更新

UART为 `/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AV0KRCHT-if00-port0`，115200/8N1。
脚本独占端口；不并行打开终端。UART是控制主通路，旧内核不依赖以太网。

用 `uart.py --output NEW_DIR --command 'COMMAND' --timeout SECONDS` 执行普通命令。
传文件用：

```sh
python3 paper/ispass2027/board/uart.py --output NEW_DIR \
  --upload LOCAL_FILE --remote /tmp/NEW_FILE --timeout 2400
```

传输完成后解压，并直接运行 `platform-sd-update --device /dev/mmcblk0p1 --image FILE`。
历史内核使用传入 `/tmp` 的静态updater。更新器本身检查设备和分区，写完同步；
不再先跑dry-run或随后整分区回读。不格式化、不重分区。
本机已有原始启动分区备份，位置见当前运行记录；不要求每次更新重复备份。

本轮00082和pathv3镜像没有编入SPI控制器驱动。需要在这些内核下写SD时，
直接使用各自实验包中的同版本恢复模块，先`insmod spi-bitbang.ko`，再
`insmod spi-xilinx.ko`，随后运行updater；不强制加载不匹配模块。
压缩归档用`gzip -dc FILE | tar -xf - -C DIR`解开，旧BusyBox不支持`tar -z`。
模块仅供软件更新，更新后正常启动新内核即可；正式矩阵期间不加载外部模块。
来源及本轮实测记录见[恢复说明](../../recovery/README.md)。其他版本按其实际驱动配置处理。

耗时命令可用 `dispatch.py --output NEW_LAUNCH_DIR -- python3 ...` 启动一次有限任务。
查看 `result.json` 和底层运行结果；派发成功不代表实验成功。它不自动重试。

## 加载和启动

原生Vivado位于 `/tools/Xilinx/Vivado/2023.1/bin/`，指定JTAG线缆
`200300BD5148B`、FPGA `xc7k325t`。使用已有交付清单中的BIT和SHA参数：

```sh
python3 paper/ispass2027/board/program_and_boot.py \
  --bit ABSOLUTE_BIT --expected-sha256 MANIFEST_SHA \
  --expected-kernel EXACT_RELEASE --output NEW_LOAD_DIR \
  --credential-helper LOCAL_EXISTING_CREDENTIAL_HELPER
```

它加载易失BIT并捕获启动日志；内置核对内核和boot ID。沿用现有用户提供的板端凭据，
只在密码提示后发送一次，登录接收不写raw日志。不把凭据放参数、环境变量或产物。
本机既有helper引用为
`iommu-platform/out/r107-execution-gate-20261012/route/login_current_board.py`。
不把该引用误当作随skill分发的凭据。

## 采集与结束

执行**本次实验说明中的命令**，保存原始输出、退出码、启动身份和参数。普通
实验命令可直接通过通用串口入口执行：

```sh
python3 paper/ispass2027/board/uart.py --output NEW_RUN_DIR \
  --command 'EXPERIMENT_COMMAND_WITH_ITS_ARGUMENTS' --timeout EXPERIMENT_TIMEOUT
```

占位符由本次任务替换。已有该实验的采集器时优先复用，并按本次说明运行条件、
次数和顺序。现有 `collect.py --phase ...` 仍是G1/ISPASS矩阵专用采集器，
不把任意后续实验强塞进它的专用phase。

将通用操作检查与实验结果判定分开：串口独占、命令完成、正确内核/启动身份、
更新器错误停止等沿用现有脚本；DMA正确性、完成/排空、计数守恒及同sequence
检查仅在对应版本和采集器适用时使用。不要求没有PATH接口的版本输出16条PATH
记录，也不把4096次、4KiB、预热256或历史±2%阈值当成通用要求。

保留本次采集器适用的内置检查，不因“通用”而静默关闭已有检查。如只是完整原始
输出的解析问题，先恢复已完成点；只有采集器提供并适用时才用
`--resume-run OLD_RUN`，仅执行余点；
不补采来筛选过关。实际技术错误则停止并保留日志，不盲目重交写卡或实验。
主机超时不等于板端已停，先接续现有输出；不增加一轮回读验收。

运行完成后更新配套实验包、该实验的数据表和状态；保留实际错误和合法性能偏离，
不以历史G1性能判断所有未来实验。区分硬件时间、软件时间和观测窗口，不从已有
均值推造分位数。按本次任务约定保留板端状态；默认保持Linux运行，关闭本次主机连接。
不因使用本skill而自动追加机制、负载、设备数或其他实验。

# ABI5 CLI与补充backend边界

`source/idma_workload.c`是独立用户态程序，不替换原`idma_experiment`、`idma_p0`、`idma_scaling`或`idma_pages`。
CLI要求ABI5/BTC3/OBS5/Depth64与DID HPM，版本/大小/几何/字节/coverage错误立即失败，EINTR也不重试RUN。
默认V0和Solo，V/A的shape、operation size、allocation pages分别配置；A启用时固定DMA1/DID11。

## 默认协议与命令

默认V有效字节16MiB，V预热一完整几何环。descriptor数=logical operations×rows/op，
tile为多个串行row COPY；A运行量是返回的实际row数，不按整tile或V工作量推算。

- `ring`：最小线性环，pages=max(1,size/4096)。用于新六size×Solo/A1/A8切片。
- `nic`：默认8页；短包每页紧密slot。16/64KiB改为线性长COPY控制，不称NIC包。
- `storage`：默认256页线性环。
- `tile`：固定256页、64 matrix rows、16KiB pitch；row_bytes=min(size,1024)。
  size64/256/1024是地址顺序退化控制。

默认JSON protocol=`equal16MiB_fullVring_v1`。1536不整除16MiB，必须显式`--operations`，
任何显式operations/warmup override均标`explicit_counts_v1`，不能自动放入150新点主协议。
CLI打印完整输入geometry、counts、warmup rows/bytes、测量distinct bytes/pages、warmup pages及verified union。
Solo的A geometry为零，A角色与size元数据为null。

Root后续在已绑定软件/BIT后单点命令示例（本文件未执行板测）：

```sh
idma_workload --victim-shape ring --victim-size 64
idma_workload --victim-shape ring --victim-size 65536 --aggressor 1 --aggressor-pages 8
idma_workload --victim-shape nic --victim-size 256 --aggressor 1 --aggressor-shape storage --aggressor-size 65536
idma_workload --victim-shape tile --victim-size 16384 --aggressor 1 --aggressor-shape nic --aggressor-size 256
```

## 实际CLI验证与产物

[167项actual main回执](test-results/workload-cli02-report.json)通过ASan/UBSan与-Werror：
72主轴条件、9个canonical V/A组合、24个独立A shape/size、caps、显式1536/partial tile计数、
参数/overflow/reserved合同、旧版本/OBS/depth/HPM、错误data/bytes/coverage/union/未排空、EINTR只调用一次。
默认V geometry与先前独立Python地址模型比较，确认16MiB及完整V环warmup，不将mapped pages冒充活跃足迹。
01先通过166项；02新增CLI对V in-flight错误拒绝，未覆盖/改写01。

RV64静态ELF本机路径`artifacts/workload-cli02/idma_workload`，517448B，
SHA256=`7cd7ae4075404ec53bc3ef20437ac72a5341d8ae49ecb46350a1de6bf11a2119`，readelf确认RISC-V且无INTERP。
这只是CLI产物，未称已生成最终共同payload。回执标fixture_only=true，fake ioctl不构成板测。
复跑：`python3 paper/ispass2027/exp/E3-workloads/verify_workload_cli.py --output paper/ispass2027/exp/E3-workloads/artifacts/workload-cli-ROOT01`。

## 补充真实backend三项

[实际summary](test-results/packet-backend01-summary.json)、[独立原事件复算](test-results/packet-backend01-event-audit.json)、
[编译/执行manifest](test-results/packet-backend01-manifest.json)均passed。
复用已审long04的冻结真实idma_backend与memory/guards/AXI hold检查，只替换fixture main为三项：

| descriptor | 实际AR/AW | 实际ARLEN/AWLEN | 实际read/write bytes |
|---|---:|---|---:|
| 256B，page offset0 | 1/1 | 31 | 256/256 |
| 1536B，page offset0 | 1/1 | 191 | 1536/1536 |
| 1536B，page offset1536 | 2/2 | 63、127 | 1536/1536 |

[len表与原始握手事件](test-results/packet-backend01-events.jsonl)带原行号；AR/RLAST与AW/B守恒、
所有R/W字节、完整memory及未触及guards、响应和有限排空通过。
[源绑定](test-results/packet-backend01-production-bind.json)确认每个显式编译backend生产源与full-E3的1d587相同。
原执行source和脚本保存在`source/packet-backend01/`。没有修改backend、driver或frontend。
被动Depth4/16仅复用long04 fixture，不代替full-E3全SoC路径Depth64证明或最终BIT实现验收。

known exclusions继续保留：A1 run02零长response-under-backpressure缺陷不进入合法域；
NO_ERROR_HANDLING不传播AXI SLVERR给软件；backend停止供给排空不等同driver/frontend STOP证明。
这批是实际RTL有限证据，0个新板测点；三个预期burst数现在有实测DUT依据，不再仅源码推导。

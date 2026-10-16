# composition-v1 交付与上板说明（2026-10-04）

## 2026-10-05 现行入口：composition-ptwfix route-opt + net4trace（172/172 通过）

> 本节是当前操作入口；下方“composition-trim route-opt + net3 + 172”一节及其后的串口 SD 上板 / cpn1 示例均标为历史，只保留原始记录。本轮完整 172 已通过功能验收；性能和研究收益仍需单独分析。

### 当前硬件与状态

- 现行 Mac BIT：`tools/composition-ptwfix-20261005/ariane_xilinx.bit`；Dell 验收对应 [out/composition-ptwfix-20261005/route-opt/ariane_xilinx.bit](ariane_xilinx.bit)。不要用旧 composition-trim/net3 BIT 代替当前 BIT。
- 验收记录 [acceptance.json](acceptance.json)：route-opt BIT 最终时序通过，WNS +0.031 ns、WHS +0.019 ns、WPWS +0.062 ns；276241/276241 线路布通、errors 0；native BIT DRC errors 0、组合环 / 锁存环 / 无时钟 0。完整板级 IO/CDC 签核仍未完成。
- RTL 修复：沿用 DS 写通道来源对齐修复，并修正 PTW 合法叶分支（`PTW_legal_leaf_branching`）。针对性仿真 [targeted-test-acceptance.json](targeted-test-acceptance.json) 由旧失败转为 7/0 通过；旧 legal 4KiB 读、写被分别误报为 cause 13、cause 15。该范围只覆盖 PTW 接口与既有 TL 回归，不等于板级 PN 验收。
- 已加载当前 BIT 并启动 Linux/SSH：boot_id `2695d848-8589-491e-bf7f-1de7af5ea9cb`；此前失败的 PN4096 实际 1/1 实板通过（V_completed 4096、A_completed 7244、V/A data errors 0、taint 0）。
- 完整冻结 172 已在 `results/composition-ptwfix-20261005-formal01` 一次完成：172/172 通过、0 失败（short 16、noise 12、explore 144），原始目录已回传。[板测验收](board-acceptance.json)。旧 net3 和 DS-only 两轮各 52/1/119 的记录保留，未并入本次结果。

### 当前内核与网络

- 现行内核：`6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty`。
- Mac `192.168.77.1`，FPGA `192.168.77.2`，USB 网卡百兆，JTAG `1000000`。SD 常驻板卡，正常工作走以太网/SSH；UART 只作故障诊断，同一时间只能有一个采集者。

### Mac 项目根目录

```sh
cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
```

### 日常：同步并重载当前 BIT（不带 `--payload` = 不写 SD）

- 执行前先确认并结束已有 XVC / 其他 JTAG 占用，确保同一时间只有一个 JTAG 使用者。

```sh
tools/method88-venv/bin/python scripts/mac_update_reboot.py \
  --run --transport ssh \
  --credential-script scripts/run_research2_mac.py \
  --host-ip 192.168.77.1 \
  --board-host 192.168.77.2 \
  --bit tools/composition-ptwfix-20261005/ariane_xilinx.bit \
  --expected-kernel "6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty" \
  --out "results/composition-ptwfix-$(date +%Y%m%d-%H%M%S)-reload" \
  --boot-timeout 900 \
  --jtag-hz 1000000
```

- 固定流程：正常工作走以太网、SD 留板卡；仅 RTL 改变时 sync + JTAG 重载指定 BIT、不写 SD；只有 Linux 改变时才加 `--payload <fw_payload.bin>`，此时 `--expected-kernel` 必须换成该软件实际交付的 `uname -r` 版本，不沿用旧 net4trace；网传后只写 32 MiB 第一原始分区并 sync，再用 JTAG 重载已验收 BIT，不覆盖整卡。
- 不带 `--payload` 时工具仍先 sync 再 JTAG 重载；默认 dry-run，只有 `--run` 才执行；`--out` 必须每次新目录，不覆盖、不 resume。
- 不加入常规 payload hash、写后回读、重复 build 或额外预跑；保留工具已有身份核对与退出码。

### 异常卡死：先保存证据，再释放 JTAG 后手动恢复（此前已确认完全卡死的 DS 恢复例外，不是默认流程）

先保存可取得的故障输出；UART 诊断保留且只能有一个采集者。确认既有 JTAG XVC 已正常退出后，使用下方 openFPGALoader 手动加载同一已验收 BIT；这不是脚本现有 rescue CLI 开关：

```sh
/opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000 \
  tools/composition-ptwfix-20261005/ariane_xilinx.bit
```

这一步不写 SD；恢复后用 SSH 读取实际内核、boot_id 与网络状态。恢复成功不代表失败实验已通过；免 sync 仅适用于此前已确认完全卡死的 DS 恢复历史例外（本次 PTW 上板走正常 sync 流程），不能固化为默认流程。

### 跑冻结 172（纯网口 SSH attach；以下供今后明确启动新一轮使用）

```sh
tools/method88-venv/bin/python tools/composition-v1-20261004/mac_composition.py \
  --run --transport ssh \
  --board-host 192.168.77.2 \
  --credential-script scripts/run_research2_mac.py \
  --ssh-helper scripts/mac_update_reboot.py \
  --through-phase explore \
  --expected-kernel "6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty" \
  --attach-timeout 60 \
  --out "results/composition-ptwfix-$(date +%Y%m%d-%H%M%S)"
```

- `--through-phase explore` 才跑冻结 172，一次执行已含前序阶段；`--out` 必须不存在。一次只执行一遍 172。
- 失败即停：技术失败保留现场后停止并定位，不自动重跑；禁止自动改 172 计划或阈值、禁止用重试掩盖失败；科学阴性（fast==0、miss==0、无收益）不是技术失败。
- 结束把原始目录（含失败现场）回传 Dell，释放 UART/JTAG 占用而 Linux 保持运行。
- 进入正式 172 前关闭 trace 是当前内核要求（`echo N > /sys/module/idma/parameters/composition_trace`）；单次 PN4096 仅属本轮排错，不是 172 的一部分，也不作为每次 172 的常规额外预跑。
- 凭据只通过现有 `scripts/run_research2_mac.py` helper；不读、不输出密码或密钥；不要把 `root@` 写进 `--board-host`。

### 本轮全 172 验收结果

- `status=complete`、`planned=attempted=completed=172`、`failed=0`，全部 case 的 problems 为空；每项 taint=0、恢复状态=0、参与传输的数据错误=0。40 项 solo 没有 A，不把空 A 当成缺失结果。新 BIT 上 PN 单项和本轮正式 172 使用同一 boot，单项不计入 172。
- UART/JTAG 占用已释放，Linux 保持运行；本轮 JTAG 加载 1 次、SD 写入 0 次。这是功能验收，不等于性能收益、机制创新或完整板级 IO/CDC 签核。

---

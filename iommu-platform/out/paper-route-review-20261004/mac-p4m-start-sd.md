# P4M start-control / Linux00089 上板交接

交付：`artifacts/software/p4m-start-control-20260920/delivery`，来源 Dell `out/p4m-start-control-20260920/delivery`。

## 本次写卡

2026-09-20 已将新 payload 写入已登记的 7,817,134,080 字节 SD 卡第一原始启动分区，未改第二分区。原启动区备份和写卡记录在 `results/sd/update-wvxp4i78/`。跳过下载哈希及写后回读；状态为 `written_not_readback_verified`，不表示板上通过。复用原 P4M v7 BIT，没有重新编译或下载 BIT。

可复用命令（不要在未确认 SD 设备时直接照抄）：

```sh
python3 scripts/fetch_p4m_start.py
sudo /opt/homebrew/bin/python3 scripts/sd_card.py update --disk /dev/disk5 --release p4m-start-control-20260920 --skip-payload-hash --skip-readback
diskutil eject /dev/disk5
```

脚本校验已登记卡的容量、UUID、偏移和 GPT，备份后仅写第一分区并同步。此文件中的 disk5 是本次设备，不保证下次编号相同。

## 插回 FPGA 后

1. FPGA 断电，插回 SD，连接 JTAG/串口，再供电。
2. 开启完整串口采集。若断电导致配置丢失，加载既有 `migration-4dma-p4m` BIT：`artifacts/software/p4m-baseline-20260920/delivery/ariane_xilinx.bit`，不是 E3 BIT。
3. 登录后确认 `uname -r` 为 `6.5.0-rc2-00089-gefd9461d6459`。
4. 本轮已授权执行 `platform-p4m-start all /tmp/p4m-start-01`，目录必须不存在。先4次短验收，全部通过后8次4096/256对照，固定 BPPBBPPB；遇错停止，不自动重跑或扩矩阵。保留交付 runner 的内置检查。
5. 完整结果、JSON、串口、启动记录及失败样本回传 Dell 独立目录，更新 HANDOFF 和 docs/validation.md。当前尚未上板，不宣称新版本通过。

DeepSeek 起草版本下载入口和写卡脚本接入；Codex 审阅并实际执行写卡。未新增主机回归、独立验收或哈希/回读测试。

# composition-v1 交付与上板说明（2026-10-04）

一次人工写 SD + 一次加载 BIT；启动后四个模式 S / N / P / PN（mode 0/1/2/3）在**同一次 boot、同一个 BIT** 内由驱动 ioctl 0x11 写 live config（offset 2）运行时切换。切换不重写 SD、不重载 BIT、不重启；全部 172 用例在一次启动内跑完。

## 交付文件（平铺到 `tools/composition-v1-20261004/`）

| 文件 | 说明 |
|---|---|
| `ariane_xilinx.bit` | FPGA bitstream（只加载一次） |
| `fw_payload.bin` | 写入 SD 第一分区的 payload（29,363,656 字节，只写一次） |
| `mac_composition.py` | Mac attach-only runner |
| `composition_plan.py` | 172 用例冻结计划（runner 导入） |
| `mac_research2.py` | 提供 `SerialConsole`（runner 导入） |
| `DELIVERY.json` | 交付清单；`fw_payload.bin` 的 sha256 为 null，按用户要求跳过哈希核对 |
| `SPEC.md` | 现有规格说明 |
| `RESEARCH_REVIEW.md` | 现有研究评审 |
| `README.md` | 本说明 |

`mac_composition.py` 从**自身所在目录**加载 `mac_research2.py`、`composition_plan.py`，这三个 `.py` 必须平铺在同一目录。

运行依赖：`tools/method88-venv/bin/python`（含 pyserial）+ 同目录 `mac_research2.py`(SerialConsole) / `composition_plan.py`；板卡侧默认 `/usr/bin/idma_composition`、`/dev/idma0`。

## 1. SD 写卡（人工；Codex 不代做）

写卡脚本 `scripts/sd_card.py`（**已确认存在**；本次装审查过的 composition-v1 release）。它**只接受已登记的这张卡**：

- 整卡 7,817,134,080 字节、512 字节块、外置物理盘、`GUID_partition_scheme`；
- 第一分区：32 MiB、偏移 1 MiB、LBA 2048–67583、类型 `7412F7D5-A156-4B13-81DC-867174929325`、UUID `46298C61-BAC2-44C9-A8FF-8FA61CF2BEE0`；
- 第二分区：UUID `0AF525DD-76E6-44E8-A5CD-4A66DC5697B8`，保留不动。

这些 UUID 是这张卡的固有身份，用于核对并**继续沿用**；任一项不符即拒绝。写入对象是**整卡原始第一分区**（payload 后补零到 32 MiB），**不是普通 FAT 文件拷贝**，第二分区不写。

```sh
cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
diskutil list
```

从 `diskutil list` 读出整卡编号，把下面的 `/dev/diskN` 换成实际值（例如 `/dev/disk5`）。本次实际编号未知，**必须替换，不得沿用旧编号**。

```sh
sudo tools/method88-venv/bin/python scripts/sd_card.py update \
  --disk /dev/diskN \
  --release composition-v1-20261004 \
  --skip-payload-hash --skip-readback
```

写完安全弹出：

```sh
diskutil eject /dev/diskN
```

- `--skip-payload-hash`：`DELIVERY.json` 里 `fw_payload.bin` 的 sha256 为 null，按用户要求跳过，不设为阻塞项。
- `--skip-readback`：按用户要求跳过写后回读。
- 二者只允许用于 `update` 且 release 为 `composition-v1-20261004`。不要求下载哈希 / 回读 / 解包。

## 2. 上板（Codex 通过 ssh 到 Mac 执行；SSH 目标是 Mac，不是板上的 Linux）

1. SD 插入**断电的** Genesys 2，接好串口与 JTAG，再上电。
2. 先做串口采集（记录启动输出）。
3. 加载一次 BIT：

```sh
/opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000 'tools/composition-v1-20261004/ariane_xilinx.bit'
```

4. 等 Linux 起到 root shell。

加载 BIT（flash）与 attach runner 是两步，**不写进同一自动函数**。

## 3. 运行全部 172（Codex 执行）

```sh
cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
tools/method88-venv/bin/python tools/composition-v1-20261004/mac_composition.py \
  --run \
  --through-phase explore \
  --out results/composition-v1-20261004-01 \
  --expected-kernel '6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty'
```

- 不用先跑 `--plan`：`--run` 会先生成完整 `plan.json` 再执行，不要为看计划单独跑一次。
- `--through-phase` 默认 `short`（只 16）；要跑全 172 必须 `explore`。直接一次 `explore`，不要先 `short` 再全量（会重复 short）。
- `--expected-kernel` 用本次实际 release `6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty`。runner 先 `uname -r`，不等即在任何用例前停止。
- `--out` 必须**不存在**（`results/composition-v1-20261004-01`）；已存在会直接拒绝，不覆盖、不 resume、不自动重跑。
- 失败即停：技术性失败写 `STOPPED.txt` 并停止，不自动重跑。科学阴性（fast==0、miss==0、P/PN 无收益）不算技术失败，不因此停止。
- 串口被占用时用只读 `lsof` 检查并拒绝，不杀进程。
- runner 只 attach：不写 BIT、不写 SD、不重启。

## 4. 结果回传

完成与失败的原始结果**全部**回传 Dell（本批目的地，不覆盖已有数据）：

```
xvhaocheng@dell:/home/xvhaocheng/work/iommu-platform/out/mac-composition-v1-20261004-01/
```

跑完 Linux 保持运行，不关机 / 不重启 / 不重加载 BIT / payload。

## 5. 状态

构建状态：Linux已完成；BIT待构建完成后更新。尚未上板。

## 6. 一句话操作清单

1. 人工：`diskutil list` → 替换 `/dev/diskN` → `sd_card.py update`（含 `--skip-payload-hash --skip-readback`）→ `diskutil eject`。
2. SD 插入断电板卡，接串口/JTAG，上电；Codex 串口采集后加载 BIT，等 root shell。
3. Codex 一次 `--through-phase explore` 跑完 172。
4. 结果（含失败）回传 Dell；Linux 继续运行。

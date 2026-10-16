# composition-v1（2026-10-04）Mac 上板交接（追加）

交付平铺到 `tools/composition-v1-20261004/`：`ariane_xilinx.bit`、`fw_payload.bin`、`mac_composition.py`、`composition_plan.py`、`mac_research2.py`、`DELIVERY.json`、`SPEC.md`、`RESEARCH_REVIEW.md`、`README.md`。三个 `.py` 必须同目录。

一次人工写 SD（第一分区）+ 一次加载 BIT；同一次 boot 内运行时切换 S/N/P/PN，一次跑完 172。`mac_composition.py` 只 attach，不写 BIT / 不写 SD / 不重启。

## SD（人工先做）

- 脚本 `scripts/sd_card.py` 已确认存在，本次装审查过的 composition-v1 release。
- 只接受已登记卡：整卡 7,817,134,080 字节外置 GPT；第一分区 32 MiB、偏移 1 MiB、类型 `7412F7D5-A156-4B13-81DC-867174929325`、UUID `46298C61-BAC2-44C9-A8FF-8FA61CF2BEE0`；第二分区 UUID `0AF525DD-76E6-44E8-A5CD-4A66DC5697B8` 保留。UUID 为固定身份，核对用并沿用；不符即拒绝。
- 写整卡原始第一分区，非 FAT 拷贝；第二分区不动。

```sh
cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
diskutil list
sudo tools/method88-venv/bin/python scripts/sd_card.py update \
  --disk /dev/diskN \
  --release composition-v1-20261004 \
  --skip-payload-hash --skip-readback
diskutil eject /dev/diskN
```

- `/dev/diskN` 为占位，按 `diskutil list` 实际编号替换，不沿用旧编号。`--skip-payload-hash` 因 DELIVERY.json 的 sha256 为 null 按用户要求跳过；`--skip-readback` 按用户要求跳过回读。

## BIT（Codex，ssh 到 Mac；SSH 目标是 Mac 非板 Linux）

- 运行前先做串口采集。

```sh
/opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000 'tools/composition-v1-20261004/ariane_xilinx.bit'
```

- flash 与 attach runner 分开，不写进同一自动函数。

## 运行

```sh
cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
tools/method88-venv/bin/python tools/composition-v1-20261004/mac_composition.py \
  --run --through-phase explore \
  --out results/composition-v1-20261004-01 \
  --expected-kernel '6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty'
```

- 不用先 `--plan`（`--run` 会先生成 `plan.json`）。`--through-phase` 默认 `short`（16），要 172 必须 `explore`。`--out` 必须不存在；失败即停、不重跑、不 resume。

## 结果

- 原始结果（含失败）全部回传 Dell，不覆盖：`xvhaocheng@dell:/home/xvhaocheng/work/iommu-platform/out/mac-composition-v1-20261004-01/`。Linux 保持运行。

## 状态

构建状态：Linux已完成；BIT待构建完成后更新。尚未上板。

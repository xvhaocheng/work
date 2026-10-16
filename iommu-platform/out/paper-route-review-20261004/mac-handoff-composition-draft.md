# composition-v1（2026-10-04）Mac 上板交接（追加）

交付：平铺到 `/Users/jojo/Desktop/在mac链接pfga(codex)/tools/composition-v1-20261004`，文件为 `ariane_xilinx.bit`、`fw_payload.bin`、`mac_composition.py`、`composition_plan.py`、`mac_research2.py`、`README.md`。

## 本次是什么
一次写 SD（第一分区）+ 一次加载 BIT；启动后在同一次 boot 内运行时切换 S/N/P/PN 四模式，一次跑完 172。`mac_composition.py` 只 attach，不写 BIT / 不写 SD / 不重启。

## SD
- 目标：整卡第一个 GPT 分区，32 MiB，偏移 1 MiB，LBA 2048–67583，类型 `7412F7D5-A156-4B13-81DC-867174929325`；第二分区保留，不写入。
- 先 `diskutil list` 确认真实整卡编号，不沿用旧 `/dev/disk5`。
- 用本机既有写卡流程写入 `fw_payload.bin`；写卡脚本不在本交付包文件清单内（需确认路径/可用性）。不沿用旧 SHA、旧备份目录、旧日期。不要求下载哈希 / 回读 / 解包。
- 完成后 `diskutil eject`。

## BIT
- 一次加载 `ariane_xilinx.bit`（openFPGALoader `/opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000`）。由 Codex 执行；runner 不做这步。

## 运行
```sh
/Users/jojo/Desktop/在mac链接pfga(codex)/tools/method88-venv/bin/python mac_composition.py \
  --run --out ./results/run-20261004-01 \
  --expected-kernel '6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty' \
  --through-phase explore
```
- `--through-phase` 默认 `short`，只 16；要 172 必须 `explore`。不要先默认 short 再单独跑全量（会重复 short）。
- `--out` 必须不存在；失败即停、不重跑、不 resume。
- 依赖：venv python + pyserial + 同目录 `mac_research2.py`(SerialConsole) / `composition_plan.py`。

## 结果
- 原始结果（含失败）全部回传 Dell 独立目录，不覆盖；Linux 保持运行。

## 局限
- FPGA 仍在编译，未上板；不声称通过。无新 BIT 时序 / 上板验收。

# G1：历史DS50 batch配对与已通过结果

本包对应2026-10-17完成的历史G1：Solo/A1/A8各3次，9个唯一实际执行、同1个boot，冻结±2%全部通过。完整配对记录见[manifest.json](manifest.json)。

| 找什么 | 入口 |
| --- | --- |
| 实际结果与中位数 | [accepted/review.json](results/accepted/review.json)；56.49366 / 107.10562 / 154.95348 ms |
| 独立离线核对 | [independent-check.json](results/accepted/independent-check.json) |
| 本次启动身份 | [load.json](results/load-identity/load.json)，内核 `6.5.0-rc2-00082-gb7979a1a06ac` |
| 配套SD镜像 | [artifacts/fw_payload.bin](artifacts/fw_payload.bin)，已有SHA `c18a7680…` |
| 配套BIT | [artifacts/g1-ds50-batch.bit](artifacts/g1-ds50-batch.bit)，已有SHA `59ebd4c7…` |
| 代码与历史构建记录 | [source.json](source.json)、[historical-delivery.json](metadata/historical-delivery.json)、[sources.lock.json](metadata/sources.lock.json) |
| 复现说明 | [g1-reproduce.md](metadata/g1-reproduce.md) |

`fw_payload.bin`是实际写入SD第一分区的OpenSBI＋Linux payload。此包只配历史batch BIT/00082；新基线的两份BIT及pathv3软件在旁边的[baseline100-20261017](../baseline100-20261017/)。

结果恢复链是[run03 accepted](results/accepted/) → [run02](results/recovered-run02/) → [run01](results/raw-run01/)。run01和run02的原始命令/UART及failed manifest保留；run03只离线恢复，没有新测量。九个原始命令及已有SHA逐一见independent-check。

历史软件和FPGA manifest均记录dirty树。因此**完整实际执行树的单一commit为unknown**；source.json保留当时构建base和delivery声明的Linux/CVA6完整commit，不能拿当前HEAD填补，也不能把dirty base称为完整源码身份。

`artifacts/`中是本机实际副本，已按仓库规则忽略，不进Git。其余metadata与结果入口是相对链接。全部SHA直接引用既有delivery/manifest；本次整理未新增hash或板上验证。

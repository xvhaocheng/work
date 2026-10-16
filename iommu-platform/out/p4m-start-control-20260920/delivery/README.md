# P4M start-control 交付（软件-only，复用 FPGA BIT）

本目录由 `out/p4m-start-control-20260920/finalize_delivery.py` 生成：software manifest 为 passed，build-status 中 software=passed/exit_code=0、fpga=reused/rebuilt=false、board=not_run，且 validation 检查全部 exit_code=0、board_tested=false。板上未运行，本目录不声明板卡通过，也不声称性能收益。

- profile: `migration-4dma-p4m`（FPGA 相关硬件配置与上一次交付一致，未改）
- kernel: `6.5.0-rc2-00089-gefd9461d6459`
- FPGA BIT（复用上一次交付）: `ariane_xilinx.bit` SHA256 `7af1ccbcb36306d5439eeab9b39d8fdfa266f70ebfedd7c0a4f0dd1be6825d0e`
- FPGA MCS（复用上一次交付）: `ariane_xilinx.mcs` SHA256 `222665cd8f44d33eee72cd36e80d30bb6ffc89c9301a447e1553a48720578555`
- payload（新）: `fw_payload.bin` SHA256 `fc8e8a73d3e71e127d7d995518f668382391ef0920418d16ccb66d716543291d`
- Image（新）: `Image` SHA256 `11f1447649d806b5faafcbea532bb2c2b1c96fcdd3050d40c99c65a90c4696df`
- dtb（新）: `platform.dtb` SHA256 `0439c6c944dbb83af0ee3c3f637c0e952d8a75bfedda9cbc3e06c31fe7542f4a`
- rootfs（新）: `rootfs.cpio` SHA256 `98940f4edcb39efbf4bac8614c7cbbdd59a1746e46702dbe30312fa699a388f0`（内含新的 `usr/bin/platform-p4m-start`，与本目录 `platform-p4m-start` 内容一致且权限为 0755）
- CLI（新）: `idma_experiment` SHA256 `72d09b2efc7e85eeb40d1f8f4c3ca1b06d9c47687965d88794dcdad662602560`

## 为什么复用 BIT，不重编 FPGA
本次是 SOFTWAREONLY 变更：FPGA RTL 与 profile 未改，`sources.lock.json` 中除 `linux` 外的全部组件（含 cva6、riscv-iommu 及其它 submodule）与上一次交付逐项 commit 完全一致（脚本逐项校验，要求组件集合与 commit 都相同）。因此不重编 FPGA，直接复用上一次交付的 BIT/MCS。新 lock 与旧 lock 的整体 hash 不同，只是因为 `linux` 已更新（新 payload 需要相应内核），不能据此判断硬件变化，也不要求整份 lock 相等。

## 板卡运行
仅在新 payload 启动后再执行；本目录保留当前 v7 BIT，不要与本 BIT 混用其它轮次的 payload：
```
platform-p4m-start all /tmp/p4m-start-01
```
- 4 次短验收：每次 64 transfers、warmup 8；
- 另有 8 次 `BPPBBPPB` 序列，4096/256，chunk 4096；
- 以上为工程检查，不是正式参数结论，也不构成性能收益声明；具体语义以 `platform-p4m-start` 脚本为准。

SD 更新沿用既有原始分区流程；本目录不提供、不发明新的偏移或烧卡步骤，也不做自动写入。板上未运行；时钟/CDC/IO 审查边界沿用既有结论，无新增 timing signoff。成功构建只证明构建本身。

本目录含 delivery.json、SHA256SUMS、validation.json、build-status.json、baseline.json、manifests、sources.lock.json、profile、P4M_START_CONTROL.md 及上述产物；SHA256SUMS 递归列出除自身外全部文件并已逐项复核。复用产物明确标注：FPGA manifest 以 `fpga-migration-4dma-p4m.reused.json` 命名，BIT/MCS 在 delivery.json 中标记 `reused: true`。

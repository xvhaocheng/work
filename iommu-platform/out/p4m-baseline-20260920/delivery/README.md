# P4M 基线交付

本交付只在 build-status 的 software/fpga 均为 passed、exit_code=0，board 为 not_run，且 validation 检查通过后由 finalize_delivery.py 生成；板上未运行，不声明板卡通过。

- profile: `migration-4dma-p4m`
- kernel: `6.5.0-rc2-00088-g9f8da9e7a6a0`
- BIT: `ariane_xilinx.bit` SHA256 `7af1ccbcb36306d5439eeab9b39d8fdfa266f70ebfedd7c0a4f0dd1be6825d0e`
- MCS: `ariane_xilinx.mcs` SHA256 `222665cd8f44d33eee72cd36e80d30bb6ffc89c9301a447e1553a48720578555`（标准 manifest 只列 BIT，MCS 单独记录）
- payload: `fw_payload.bin` SHA256 `504eee4c50c06944afe2ab43570c7e518827551659659321c42e38be77f944de`
- Image: `Image` SHA256 `785a0f17d796791dab11d9578feebce63f2e051036bd80131452c9b36139c9ee`
- dtb: `platform.dtb` SHA256 `0439c6c944dbb83af0ee3c3f637c0e952d8a75bfedda9cbc3e06c31fe7542f4a`
- rootfs: `rootfs.cpio` SHA256 `e898032b6d0e1a07fc8db15e5928531047d00cd2af990aba2024e363d414ba65`

必须同时使用本目录的 BIT 与 payload；不要与其他轮次的 BIT/payload 混用。

新版系统已安装 platform-p4m；板上运行：
```
platform-p4m short /tmp/p4m-v7-01
```

短验收 4 次，每次 64 transfers、warmup 8、4 KiB：Solo、A1 gap0、A8 gap0、A8 gap1000（仅工程检查，不是正式 gap 点）。脚本保留 JSON、stderr、dmesg 与 taint；错误、quarantine、timeout 或缺少命令时停止。

SD 更新沿用既有原始分区流程，将 payload 写入既有 payload 分区；本目录不提供、不发明新的偏移或烧卡步骤。板上未运行；时钟/CDC/IO 审查边界沿用，无新增 timing signoff。成功构建只证明构建，主机模型不等于板卡验收。

本目录包含 delivery.json、SHA256SUMS、validation.json、build-status.json、manifests、sources.lock.json、profile 与上述产物；SHA256SUMS 递归列出除自身外全部文件并已逐项复核。P4M_BASELINE.md 为交付副本，其 build-status 链接指向本目录的 build-status.json。

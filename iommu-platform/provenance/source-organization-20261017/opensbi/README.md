# OpenSBI 既有适配身份

保留原 `platform` 分支 HEAD `e8d21bfdc2dd753314627968e496c3a1188f6dfb` 及其 timer/platform 适配历史，来源祖先为 `48f91ee9c960f048c4a7d1da4447d31e04931e38`。没有重新排列旧历史或声称它是未修改上游。

当前未提交的 `platform/fpga/ariane/platform.c` 只增加 `fdt_reset.h` 和 `fdt_reset_init()`。已单独固定为 `9ed1f82340fe0db0352f0a14ec1caada5aef0c2a`，新增引用 `codex/organize-20261017-opensbi-reset`，不改原 checkout 或索引。

[manifest.json](manifest.json) 记录来源、tree 与哈希；[补丁](0001-reset-registration.patch)、[原差异](original-working.patch)、[源文件](platform.c) 和 [增量 bundle](source.bundle) 保留精确内容。Bundle 要求已存在原 HEAD prerequisite；恢复时应在独立仓库操作。本轮实际静态恢复记录在上层 [verification.json](../verification.json)。未编译、运行或上板，未把该新提交追认为历史固件身份。

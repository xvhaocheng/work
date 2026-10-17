# 构建与恢复

新任务先从[版本入口](versions.md)选择当前开发树、原版/适配/修复锚或历史实验的冻结源码。源码组合 registry 只供静态导出，不能作为下文的 Make `PROFILE`；原版 CVA6 的原始 IOMMU child 与单独上游目标版本也不可混配。

每次后续实验按[实验身份记录](../../docs/agent-policy.md#实验身份记录)保留平台及实际组件的 branch/完整 commit、dirty patch/overlay、锁与父子指针、实际编译输入哈希、构建/运行参数和结果关联。仅引用源码而未执行的 Linux/固件须单列；当前 checkout 不替代独立导出副本的身份。已有 manifest 字段不足时，另存实验身份文件并与原 manifest 互链，保留原件。

工具：本机已用 Vivado 2023.1、riscv64-linux-gnu GCC 11.4.0、riscv64-unknown-elf GCC 10.2.0、Verilator 4.038、Python 3.10 完成本次相应尝试。HPM 定向测试不需要 Bender；当前 FPGA Makefile 使用固定源文件清单和固定子模块，不在构建时拉取依赖。

可选本机配置：在不跟踪的 `local.json` 中写 `riscv_prefix`、`cross_compile`、`bare_cross_compile`。`RISCV` 环境变量可覆盖默认前缀，`vivado` 从 PATH 查找。许可证使用 Vivado 自身的环境和配置；脚本不自动建立 SSH 隧道。

本次按用户提供的文档，已验证完整地址可直接获得许可证：

```bash
export XILINXD_LICENSE_FILE=2100@opus4.apal.cs.waseda.ac.jp
make fpga PROFILE=migration-4dma
```

因此本次无需 SSH 转发，也未修改 SSH 主机校验设置。若在限制网络的执行环境运行 Vivado，需要允许构建进程连接许可证服务器。

`2100@localhost` 只适用于已经配置好 SSH 转发的机器；本机直接使用上面的完整地址。

Genesys 2 的板卡定义保存在 `configs/boards`，附带 Digilent 许可证、来源提交和文件 SHA256。构建入口将它复制到 FPGA 构建目录并显式设置 `board.repoPaths`，不依赖个人 `~/.Xilinx` 中的板卡目录。

FPGA 出错后保留了构建树，便于检查 Vivado 日志。要从完全干净的导出树重试，先将 `out/<PROFILE>/fpga-src` 改名归档，再执行 `make fpga`。不要在失败目录混入其他 PROFILE。为避免误用旧产物，脚本不会自动覆盖已有构建树。

如仅恢复许可证且没有修改脚本、提交或 PROFILE，也可以在已经导出的 `fpga-src/cva6` 中按日志中的 Make 命令继续构建；这种手动续跑必须另存版本和结果记录。正式归档优先使用从干净目录启动的完整入口。

内核使用 `O=out/linux`；OpenSBI 使用独立 O 目录；lloader 仅复制 Git 跟踪源码到 `out/lloader`；FPGA 使用 Git archive 导出各级固定源码到 out。组件源码不会被普通构建污染。

rootfs 从已保存的基础 newc 镜像生成，替换所有旧 benchmark 二进制，加入本次交叉编译得到的程序和板上 smoke 脚本。打包不需要 root、fakeroot 或创建宿主机设备节点。

`out/manifests/` 保存构建动作、状态、平台提交、dirty 状态、lock hash、配置及产物 SHA256。FPGA 构建失败不会产生有效的新 bitstream；已有旧 bitstream 只在迁移备份中留作对照。

复现是从固定源码、配置和工具生成可追溯产物，不承诺 Vivado bitstream 或 Linux 编译时间戳逐字节一致。

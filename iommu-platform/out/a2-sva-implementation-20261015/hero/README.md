# A2 HeroSDK 固定源码修补与真实编译

本目录交付可审查补丁、已应用的独立源码副本、实际编译产物和故障记录。**硬件撤销、失效完成和缓存后端尚未实现；默认 `hero_platform_gate.c` 在 pin/map 前返回 `-EOPNOTSUPP`。因此这些是已编译的修补，不能称为已运行的 SVA、已闭合的硬件生命周期或可部署的论文软件栈。** 原来源、当前平台 Linux、旧实验和全局文档未在本子任务中修改。

入口为 [result.json](result.json)、[版本与配置锁](config/version-lock.json)、[原始改动哈希](evidence/changed-source-manifest.json)；补丁是 [HeroSDK](patches/hero-tools-47339e0-a2.patch) 和 [SDK Linux 6.9 选择](patches/cva6-sdk-fb56489c-linux69-lock.patch)。两个补丁均对准确原 archive 检查可应用。

## 已改代码

- `source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c:107` 保持原 FP32 FMA，修正内联汇编为读写、早期覆盖指针和 memory clobber；不再修改只读输入操作数。145 行在末输出后等待 DMA busy 消失，147 行 fence，149 行再汇合后返回 dispatcher。物理 busy 含义仍需同一硬件验证。
- `axpy_main.c` 分开保留不可变原输入、独立预期结果和 DMA buffers；冷、热各自恢复、完成缓存交接、offload、CPU 交接和逐元素 consumer。成功 unmap 后才 free；失败 map 可能被内核保留时，应用保守保留该 allocation 到进程退出。此修补 profile 只接受 `do_map=1/noise=0` 与 512 的整数倍，不从 `/dev/mem` 改全局 DDTP。
- `sw/hero-driver/common/hero_iommu.c` 保存两别名的逐页映射进度、pins 和 owner；检查每次 map 返回、部分 pin、IOVA 冲突和范围。失败回滚必须撤销两别名并收到显式完成后才 unpin。短 unmap、quiesce/cache/completion 失败保留页、owner 和 module reference，阻止新映射。
- `carfield_fops.c` 以 open 文件及 mm 为 owner，加设备级锁，增加逐 buffer unmap 与 CPU/device 缓存交接 ioctl；copy-to-user 失败回滚，close 只清理自己的映射。force-unload 在本 profile 禁用、sysfs bind/unbind 属性关闭。该固定静态 SoC profile 没有验证动态 hot-unplug。
- SDK 和环境脚本显式选择 Linux `e4b0725`（6.9.0），没有 6.1.22 或可变内核分支回退。

## 实际编译与检查

- 固定 `Linux e4b0725` 已配置、`modules_prepare`，真实改变单元及 relocatable `carfield.o` 已编译。只编模块对象，未构建该 6.9 内核的完整 `vmlinux/Module.symvers/.ko`。
- 从真实 `LLVM d8cffb8` 源码完成 native compiler/tools（2859 步）和两项 Hero pass；准确取得 Git 对象并核工作树后增量修正版号，最终 `clang --version` 是真实 d8。RV32 newlib/libgloss、compiler-rt、DATE Carfield target runtime 及 RV64 `omp/omptarget/Hero plugin` 均实编。新加 libffi 3.4.2、elfutils 0.184、zlib 1.2.11 沿固定 SDK 的 Buildroot 配方版本并核原 hash。
- 真实 Hero OpenMP 的拆分、两个 pass、合并、host/device 编译、链接和 device image 抽取完成。最终 [RV64 ELF](evidence/axpy-hero-d8-rv64.elf)、[RV32 ELF](evidence/axpy-hero-d8-rv32.elf) 和 [设备汇编](evidence/axpy-hero-d8-rv32.s) 已保留。宿主变体使用明确标识的 Ubuntu GCC 11.4/glibc 2.35 sysroot，不能说成论文准确 Buildroot 运行。
- 直接编译实际 `hero_iommu.c` 的 kernel/hardware fake 测试共 12 例通过；包括真实默认 gate 提前拒绝、partial pin/map、短撤销、完整 unmap 但 completion 失败、quarantine、owner 隔离。实际 host main 的 CPU fake 后端 9 例通过。它们是软件故障检查，未执行真实 kernel ioctl、DMA 或 FPGA。

## 已解决的链接 ABI 冲突

首次系统 GNU host 链接虽然 exit 0，实际报告两条 `unsupported relocation type 0x3d`，**该 ELF 已否决，原件在 `evidence/rejected/`**。它们位于 allocated `.rela.eh_frame`，不是可忽略的 debug relocation。固定 LLVM 把 `R_RISCV_32_PCREL` 编为 61，系统 GNU 编为 57；同版 LLD 又不能处理 Ubuntu `crt1.o` 的 ALIGN relaxation。

最终 host 链接采用 [受限兼容脚本](scripts/hero-host-ld-compat.py)：只对标为固定 d8 的 RV64 对象，在副本的 `.rela.eh_frame` 内将 61 改为同一 S+A−P、32 位操作的 57。只改变实际两处 type 字节，原对象、EH 数据、symbol、offset、addend 不变；其他 section 出现 61 会拒绝。device 仍由固定 d8 LLD 链接。原始依据见 [源定义与实现](evidence/relocation-abi-primary-evidence.txt)，逐条收据见 `evidence/host-link-compat-1007745.json`。

[最终 ELF 检查](evidence/final-elf-acceptance.json)确认：无 61 遗留、动态只有标准 COPY/JUMP_SLOT 21 项；8 个 FDE 均指向可执行区，固定 LLVM unwind 解析无错误；没有误指 native build 库的绝对 RUNPATH。原 GNU 的 DWARF 0x22/0x23 展示警告保留，未等同为运行错误，也不当作完整调试器兼容保证。未执行 loader 或目标程序。

## 复现入口与限制

依赖源、配置和原始 archive 均在 `source/`、`config/`。`scripts/build-llvm-minimal.sh`、`build-hero-dependencies.sh`、`build-host-runtime-deps.sh`、`build-openmp-rv64.sh`、`compile-hero-offload.sh` 记录实际各阶段命令。独立 kernel 编译见 `compile-kernel.sh`，软件故障检查见 `test-driver-faults.sh` 与 `test-host-lifecycle.py`。完整日志在 `logs/`，最终编译日志是 `hero-offload-host-abi-compat.log`。编译成本不等于任务或 DMA 运行成本。

保留的失败包括：初始无固定工具链、Bash recipe 被 `/bin/sh` 执行、host sysroot 布局、缺 libgloss、缺运行库、首个 OpenMP staging 路径错误、最初误取外层 Git 版本串、GNU/LLD ABI 冲突和对应修正。一轮 ASan 复跑发生递归信号处理器循环，原始日志保留；同一实际测试二进制关闭递归 SIGSEGV handler 后通过，独立 UBSan 也通过，具体 sanitizer 运行时根因未定。早期依赖编译使用的 compiler 已来自 d8 源码，但当时自动版本串曾误取外层仓库，历史产物与日志未被伪改。

下一步是真正实现并审查同一 pinned Carfield endpoint/DID 的全 publisher quiesce/drain、带错误传播的 IOTLB/context completion receipt 和 CPU/LLC/cache handoff。不能直接套当前平台另一 endpoint 的 CQ helper，也不能把 `iommu_unmap` 或 CQ head 前进当作实际完成。三个 hook 完成前，不进入真实运行或性能测试。

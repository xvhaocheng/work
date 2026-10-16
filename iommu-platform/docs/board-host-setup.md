# 首次上板的电脑分工（2026-09-15）

## 当前信息与建议

用户用 M1 Pro MacBook 工作；代码和编译环境在实验室服务器，另有 Windows 电脑。Windows 的处理器/系统版本、板卡连接位置与介质准备状态尚未确认。

用户随后明确不想同时使用两台本地电脑。更新建议：服务器继续编译，M1 Mac 原生使用 openFPGALoader 下载 `.bit` 并查看串口；如需 Vivado Hardware Manager 界面，再尝试 Parallels + Windows 11 ARM + Lab Edition。实体 Windows 仅作后备，不是用户已接受的方案。

M1 上的 Parallels 常规使用 ARM 客体系统；Vivado 2023.1 官方支持 x86/x86-64 平台，但不能据此断言 ARM 上不可用。NUS 课程团队记录 M1 Max/macOS 15.4/Parallels 20.2.2/Windows 11 成功运行 Vivado，并要求另装较新的 Digilent Adept Runtime 连接 Nexys 4；Digilent 论坛有 M1 Max/M2 + Vivado 2025.1 + Basys 3 的作者实测。均不等于当前 Genesys 2 + 2023.1 组合已验证。Linux 的 Rosetta/Docker 社区方案也存在，包含 2023.1，但 USB/XVC 支持有额外限制。

openFPGALoader 项目兼容表明确列出 `genesys2`（xc7k325tffg900），内存加载和 Flash 均标 OK；Homebrew 提供 Apple Silicon 原生包。两者支持单 Mac 下载方案的可行性判断，尚无用户机器实测。先验证 USB/JTAG 识别，再下载易失 `.bit`；不直接写 Flash。它不替代 Vivado 的综合、实现或完整 ILA 调试。

macOS 有 Homebrew 时安装：`brew install openfpgaloader`。将服务器生成的正确 `.bit` 下载到 Mac 后，命令形式为 `openFPGALoader -b genesys2 /实际路径/ariane_xilinx.bit`。此处仅给步骤，未在服务器安装 Mac 工具或执行下载。

## JTAG 与 SD 的区别

- FPGA 电路配置：可以用 JTAG 下载 `.bit`；现有 `cva6/corev_apu/fpga/scripts/program_genesys2.tcl` 已给出 Hardware Manager 下载步骤。易失配置断电后需要重新加载；固定硬件后可考虑将 `.mcs` 写入板载配置 Flash。
- CVA6 启动 Linux：现有 FPGA Boot ROM 源码 `src/bootrom/src/main.c` 调用 `gpt_find_boot_partition()`，`gpt.c` 从 SD 的 GPT 第一分区读取原始数据到 DDR 后启动。因此推荐首次使用 JTAG 加载硬件、SD 启动软件，不能把普通文件复制当作已确认的制卡流程。
- 不用 SD 的软件加载：仓库 README 和 `fpga/ariane.cfg` 提供 RISC-V Debug Module/OpenOCD/GDB 入口；有调试加载能力的依据，但当前平台完整 Linux 的无 SD 启动未验证。需另核对调试链、DDR 就绪、镜像地址和启动参数，不是 Vivado 选择 `Image` 即可运行。
- 精确制卡布局、加载文件及生成 Boot ROM 与源码的一致性，在实际准备介质时核对；本次只定向阅读源码和说明，没有上板证据。

## 下一步

优先确认 Mac 的 macOS/Homebrew、线缆和板卡连接，验证原生下载工具；需要 Vivado 界面时再准备 Parallels 路线。更新软件交付以包含已提交的小 chunk 校验修复及新增 smoke，再制作对应 SD 镜像。首次按 JTAG 配置、串口启动、smoke、E0 校准推进。已有 FPGA 构建结果可用于首次启动准备，时钟约束审查待办保留；本次未重建、下载或修改源码。

## 官方资料

- [openFPGALoader 板卡支持表](https://trabucayre.github.io/openFPGALoader/compatibility/board.html)
- [Homebrew Apple Silicon 包](https://formulae.brew.sh/formula/openfpgaloader)
- [openFPGALoader 下载命令](https://trabucayre.github.io/openFPGALoader/guide/first-steps.html)
- [NUS 课程团队的 Parallels 实测说明](https://nus-cg3207.github.io/labs/getting_started/vivado_install_guide/)
- [Digilent 论坛作者的 Basys 3 实测（非厂商认证）](https://forum.digilent.com/topic/32526-report-on-vivado-working-on-arm-macos-to-program-basys-3/)
- [Linux/Rosetta 社区方案及 USB 限制](https://github.com/ichi4096/vivado-on-silicon-mac)

- [AMD 2023.1 下载页：Lab Edition 与许可证说明](https://www.amd.com/en/support/downloads/adaptive-socs-and-fpgas/development-tools/2023-1.html)
- [Vivado 2023.1 支持的平台](https://docs.amd.com/r/2023.1-English/ug973-vivado-release-notes-install-license/Supported-Operating-Systems)
- [Parallels：Apple 芯片上的 Windows 安装](https://kb.parallels.com/en/125375)

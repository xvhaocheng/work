当前 Dell **已有实际 USB 候选入口，但当前用户没有直接访问 UART 的权限**。FT232R 串口唯一号 `AV0KRCHT` 映射到 `/dev/ttyUSB0`，与旧 Mac 的 `/dev/cu.usbserial-AV0KRCHT` 一致；Digilent Adept `200300BD5148` 的两个接口映射到 `/dev/ttyUSB1`、`/dev/ttyUSB2`。不能写成 Dell 没有物理连接。原始节点和 USB 枚举见 [tools-and-serial.stdout](raw/tools-and-serial.stdout) 与 [usb-enumeration.stdout](raw/usb-enumeration.stdout)。

新增事实出现后，root 明确授权本席做一次针对 `ttyUSB0` 的只读占用／权限／sysfs核验。其 USB 祖先为 `0403:6001`、FTDI、FT232R USB UART、serial `AV0KRCHT`。设备为字符设备 `188:0`、`crw-rw----`、`root:dialout`。当前 uid/euid 均 `1001`、用户 `xvhaocheng`，组列表没有 `dialout`；`os.access` 的读和写均为 false。**root 若使用同一当前用户执行，不能直接进行 UART 身份查询。**本席未提权、改组、改权限、改udev或配置。见 [ttyUSB0-access-and-association.stdout](raw/ttyUSB0-access-and-association.stdout)。

`lsof` 和修正后的 `fuser` 都 exit1、stdout/stderr 为空，仅表示当前非特权可见范围没有找到占用者，不能证明 root／其他不可见进程未占用。没有关闭任何进程。第一次 `fuser -v -- /dev/ttyUSB0` 被本机 fuser 解释为没有文件参数，原始用法错误完整保留；一次修正为 `fuser -v /dev/ttyUSB0` 后获得上述结果。见 [原始错误](raw/ttyUSB0-fuser.stderr)、[修正后的stdout](raw/ttyUSB0-fuser-corrected.stdout)、[修正后的stderr](raw/ttyUSB0-fuser-corrected.stderr)、[lsof stdout](raw/ttyUSB0-lsof.stdout)、[lsof stderr](raw/ttyUSB0-lsof.stderr)。

本席没有打开 UART、发送字节、改 termios／DTR、扫描 JTAG 或取得当前板端 uname/boot_id/DT。物理 USB 候选已成立，**当前运行 Genesys2/CVA6 的可执行路线仍未验实**，不能据此验收 BIT/mode/时序身份或启动 R107 普通采证。FTDI/Digilent USB 识别也不能单独证明后端板、当前映像和硬件配置。

本机是 `xvhaocheng-dell`、x86_64、Linux `6.8.0-60-generic`。当前网络 USB 为 ASIX `0b95:1790`；`enx6c1ff7cbdb07` 为 UP，没有显示 IP。`enp0s31f6` 为 `192.168.51.30/23`，tailscale0 为 `100.74.171.73/32`，未显示本机 `192.168.77.x` 地址。该快照仅说明本机地址，不能否定所有远程或间接路线。见 [host-os.stdout](raw/host-os.stdout)、[local-network-interfaces.stdout](raw/local-network-interfaces.stdout)。

10月5日记录的 `192.168.77.1→192.168.77.2` 是 Mac en11 与板 eth0；10月7日仍记录 `existing_Mac_SSH_runner`。`configs/board-network.interfaces:6-10` 固定的板端地址针对旧 Mac 私网，不提供当前 Dell 的明确目标、认证或路由。检查的 `configs/local.json`、根 `local.json`、`config/local.json` 均不存在。旧地址没有 SSH 探测；凭据脚本和私钥均未读取。历史投影见 [historical-route-projections.stdout](raw/historical-route-projections.stdout)，配置见 [project-route-configs.stdout](raw/project-route-configs.stdout)。

`scripts/mac_update_reboot.py:988-1004` 要求显式 board-host、host-ip 和现有 credential-script/ssh-key；`scripts/mac_composition.py:738-744` 要求显式 board-host、credential-script、ssh-helper。更新器 `101-114` 的 `StrictHostKeyChecking=no`／ConnectTimeout=10 与本关安全探测要求不一致；其执行路径还会同步、重载 BIT 和重启，故本席未用该 runner 探测。`scripts/mac_nonblock_pilot.py:55` 的 Mac 默认设备路径不存在，但 Linux 同号设备已在 `/dev/ttyUSB0`，两者必须分列。见 [route-source-interfaces.stdout](raw/route-source-interfaces.stdout)。

`ssh`、`lsusb`、`ip` 在 PATH；`openFPGALoader`、`vivado`、`hw_server`、`openocd` 未在 PATH 找到。这不是未安装的判定，未扫安装树、个人目录、私钥或许可证。

本席在一次枚举和 root 明确新增的有目的只读权限核验后结束。当前任务范围不允许更改权限或提权，当前网络入口也未验实，因此没有安全可用的当前板端身份查询路线。root 应结合身份与普通 adapter 结果，按下一合同裁决整体 R107 卡；不能用“Dell 无物理连接”作为理由。这里没有形成原生平台实验、普通对照失效、残余翻译损失或创新证据。

完整事实与限度见 [route-evidence.json](route-evidence.json)，本席关口结果见 [gate-result.json](gate-result.json)。共11项只读记录命令：首7项与权限关联命令 exit0，lsof／修正fuser为上述无可见占用exit1，第一次fuser为已保留的用法错误exit1。原始 stdout/stderr、实际起止时间／耗时、argv 见 [execution-ledger.json](execution-ledger.json)。此前说明与合同的读取由工具输出保留，未为其伪造执行时间。零远程连接、零串口打开／写入、零 JTAG、零重启／镜像配置变更、零构建／测试／实验。全部产物仅写本 route 目录。

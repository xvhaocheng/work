**旧target选择器错误已解决；当前有效FPGA链身份仍未验实，原因未知。**root的完整枚举显示2个target：`.../200300BD5148` 与 `.../200300BD5148B`；旧 `*200300BD5148*` 同时选入2项，`llength != 1` 因而报错。此次原错误是尾部通配符过宽，不能写成缺target或缺driver。[决定性枚举原日志第61行起](live-JTAG-enumeration.raw.log)、[行号提取与实际TCL](raw/jtag-root-enumeration-evidence.stdout)。

按路径末段严格等于 `200300BD5148` 修正选择器后，root在1MHz成功open／refresh主target，客户端exit0；但只得到1个 `Unknown_Device_0`，`PART=Unknown_Device`、32位 `IDCODE=00000000000000000000000000000001`（0x00000001）。root已close target并终止／回收任务自有server，没有program／reset／flash。[实际状态原日志第38–55行](live-JTAG-exact.raw.log)、[实际结果与退出码](live-jtag-exact-result.json)、[行号原始提取](raw/jtag-root-exact-query-evidence.stdout)。

因此当前Digilent cable发现及精确主target打开能力已证明，**对应Genesys2有效FPGA链／器件和当前配置身份未证明**。Unknown_Device的原因仍未知，不能推断断电、某种FPGA型号、未program或当前BIT损坏；也不能据client exit0就验收BIT/source/mode或时序。两次root动作分别补足列表缺失与精确选择，只解决各自有新证据的问题，本席不要求第三次设备探测。

最小合法修复仅为精确target选择，不涉及driver、USB绑定或映像变更。[精确selector片段](jtag-route-diagnosis-exact-selector.tcl)保留给root审查；[此前有界动作方案](jtag-route-diagnosis-next-action.json)已明确标注root执行完成，不再作为待重跑计划。初始“根因未知”的判断保留在机器记录中，本次新证据将选择器问题收束；Unknown_Device是后续独立未解的身份问题。[机器可读诊断](jtag-route-diagnosis.json)。

先前只读host核验确认USB001/007为Digilent `0403:6010`、serial `200300BD5148`、授权状态1，当前用户raw USB读写均true，ACL为 `user:xvhaocheng:rw-`；两个USB interface当前无kernel driver symlink。Xilinx Digilent安装步骤只复制权限udev规则，当前精确用户ACL已给予所需raw设备访问；规则文件虽未装，库与cable_data目录存在，实际两个target与成功open证明此前选择错误不应归因于缺规则。[设备元数据](raw/jtag-current-usb-metadata.stdout)、[ACL](raw/jtag-current-usb-acl.stdout)、[安装入口原文](raw/jtag-driver-install-interfaces.stdout)、[launcher原文](raw/jtag-launcher-interface.stdout)。这些事实不解释Unknown_Device，未据此安排安装／解绑／重绑。

本席共9项记录的只读文件／host检查均exit0；未启动server、打开／写设备、安装驱动、改udev／ACL或读取凭据。实际target枚举、精确选择与状态查询由root执行，本席亲读原日志并纳入结论。[原始命令、stdout/stderr、时间与退出码账本](jtag-route-diagnosis-ledger.json)。只写本route目录，未改共享文档。子席到此结束，由root稳定裁决执行前提；本结果不构成普通对照失败、残余翻译损失或创新证据。

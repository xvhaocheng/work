### 最新（2026-10-05 net3）

- 主操作说明只认 `docs/software-update-20261004.md` 的“net3补充”一节，不新增其它 README。

- Mac：USB 10/100 LAN（en11）已持久设为 192.168.77.1/255.255.255.0、不配 router（exit0）；纯网流程不用 UART，JTAG 保留用于重载 BIT。

- 正式命令（先 `cd '/Users/jojo/Desktop/在mac链接pfga(codex)'`）：
```
tools/method88-venv/bin/python scripts/mac_update_reboot.py --run --transport ssh --credential-script scripts/run_research2_mac.py --board-host 192.168.77.2 --host-ip 192.168.77.1 --payload artifacts/software/software-update-autonet-20261005/delivery/fw_payload.bin --bit tools/nonblock-pilot-20261004/banked-hit-v5.bit --expected-kernel "$(cat artifacts/software/software-update-autonet-20261005/delivery/kernel-release)" --out "results/linux-update-$(date +%Y%m%d-%H%M%S)" --boot-timeout 600 --update-timeout 3600 --jtag-hz 1000000
```

- 登录复用 `scripts/run_research2_mac.py` 现有响应，只做运行时内存取值；不新增板端免密授权、不打包公钥；缺凭据/不唯一/认证失败即停。

- net3 已从SD启动并自动联网；板上状态 `通过（写卡、同步、JTAG重载、开机自动联网及SSH身份验收exit0）`、boot_id `1a955cce-bcdd-4f0a-9012-6e0c7eda8322`，原始结果已回传。

实板入口exit0，总耗时34.45分钟；写卡/JTAG各1次，实验0。重载后无需UART配置网络；保留JTAG，Linux仍运行。主操作说明在docs/software-update-20261004.md，入口自身result.json保存每阶段及最终状态。DeepSeek起草，Codex审查修正并实板验收。

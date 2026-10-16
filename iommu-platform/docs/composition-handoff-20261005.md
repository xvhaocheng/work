# composition-v1 转交另一对话（2026-10-05）

用户最新要求：后续交给另一对话。本对话完成交接后不继续上板或实验；已有独立后台编译保留。用户已明确授权：配套SD已写好并插入FPGA，编译通过后由接手对话经SSH Mac直接上板执行既定实验，无需再次询问。

## 当前事实

- **最终状态01:40:09 JST：恢复编译failed、exit_code=1、gates_passed=0。** `recovery-tcl.log`最终错误为Negative timing slack WNS=-3.268ns、WHS=+0.042ns；并有上述31处布线冲突。无BIT/MCS，不能上板。失败DCP最终147646979字节，仅供诊断；不要将其当已验收产物。后续先处理资源/布线及实际失败路径，再重新构建，配套SD已就绪，无需因此重复写卡。

- **后续实查01:39 JST：本次恢复布线结束但失败。** 日志Route 35-2确认31处node overlaps、Design is not legally routed；Route 35-77为Router completed with failures。已保存routed DCP（包含失败布线状态，不能视为合法布线），当时进程仍生成后续报告，尚无BIT/MCS或Tcl最终结果。接手读最终state/报告后处理，不上板此结果；`route_design completed successfully`仅表示命令执行结束，不代表设计布线合格。

- 最后实际进程核查：日本时间2026-10-05 00:44左右，恢复编译运行约1小时56分钟。launcher PID3064145、Vivado PID3064187仍在；日志00:44:03更新，Phase4.3布线冲突节点降至194。该数字不是完成百分比或剩余时间估计。
- 当时无routed DCP、BIT或MCS。接手必须读取实时状态，不能把本快照或JSON的running当作编译成功。
- 首次构建已中断且原因未知；当前恢复直接使用同一源码的physopt检查点，不是重新综合。不要重复启动构建或杀现有Vivado。
- 已有完整Linux/OpenSBI构建和相关主机测试通过。本轮配套内核为 `6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty`。
- Mac写卡记录 `results/sd/update-6p5rn8l6/status.json` 实际status=written_not_readback_verified、ejected=true，release=composition-v1-20261004；payload29363656字节。按用户要求跳过哈希/回读。用户现确认已插入FPGA。
- Mac串口最后检查空闲；本轮新BIT尚未传输/加载，172项尚未启动。实验输出目录最后确认不存在。
- 工作区另一个软件任务已产出cpn2-net1，但本次SD/实验配套的是cpn1；不要因CURRENT_STATUS顶部另一软件任务而自动替换本轮版本。

## 接手入口与动作

1. 先读取 `out/paper-route-review-20261004/recovery-v1/recovery-state.json`。该独立后台构建会自行写complete/failed和真实退出状态。正常等待只关注完成状态，按用户要求避免反复巡查日志。若running但疑似中断，再核对实际进程。
2. complete后仅做一次必要最终验收：`recovery-tcl-result.json`、`reports/timing_summary_routed.rpt`、`reports/route_status.rpt`、`reports/check_timing_routed.rpt`和非空BIT/MCS。Tcl要求setup/hold存在且非负，原生write_bitstream DRC不降级；另核查最终脉宽/失败端点、内部未约束端点/时钟/组合回路。旧完整IO/CDC审查尚未完成，不声称全约束审计完毕。
3. 失败只检查实际错误，保留失败证据；不上传/上板未接受BIT，不自动放松约束。布线资源高且有拥堵警告，优先依据 [资源审查](composition-routing-cost-20261005.md) 判断后续，不凭耗时认定已失败。
4. 成功后从 `out/paper-route-review-20261004/recovery-v1/ariane_xilinx.bit` 交付至Dell `out/composition-v1-delivery-20261004/`及Mac `tools/composition-v1-20261004/`。更新DELIVERY.json的真实构建状态及必要文档，不需要解包、额外哈希/回读或重复主机测试。
5. `ssh mac` 已可连接；Mac项目路径 `/Users/jojo/Desktop/在mac链接pfga(codex)`。加载前只确认UART没有其他用户；保存启动串口输出，调用一次 `/opt/homebrew/bin/openFPGALoader -b genesys2 --freq 1000000 tools/composition-v1-20261004/ariane_xilinx.bit`，等root shell，关闭临时采集再启动既有runner。Mac解释器 `tools/method88-venv/bin/python`，UART `/dev/cu.usbserial-AV0KRCHT`。
6. 在Mac项目根目录按下方命令一次运行全部172项，不要先short再全量而重复short。

```sh
tools/method88-venv/bin/python tools/composition-v1-20261004/mac_composition.py \
  --run --through-phase explore \
  --out results/composition-v1-20261004-01 \
  --expected-kernel '6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty'
```

7. 原runner自行检查内核/bootid/taint/ABI和数据正确性；保持技术错误停止、不自动重跑、不删样本，科学阴性不作技术失败。结束或失败均完整回传Dell独立目录 `out/mac-composition-v1-20261004-01/`，更新交接/验证记录，Linux保持运行。

## 既有计划与边界

- 172项=16短+12同模式对照+144探索。S/N/P/PN四模式共用一个新BIT，在同一boot中运行时切换；不是四个IOMMU。P固定2/14份额；这是可行性验证，创新和改善尚未成立。
- [冻结规格](composition-v1-spec-20261004.md)、[五轮路线审查](paper-route-five-round-review-20261004.md)、[Mac完整操作说明](composition-v1-mac-20261004.md)。已有工具已传Mac，无需重新编写实验runner。
- DeepSeek已返回一个可选启动包装器的初稿，但Codex要求删额外检查/简化；修订所在工具调用被用户打断。初稿仅保存为 `out/paper-route-review-20261004/mac-launch-unaccepted-proposal.json`（若文件存在），未接受、未安装、未执行。接手可直接按既有上板说明操作，不以完成此包装器为前置。
- **当前没有安装“编译结束自动上板”任务。必须由接手对话完成最终验收、加载和启动实验。** 只有Vivado恢复编译在独立后台运行。
- 分工延续：DeepSeek常规证据/脚本/文档，Codex关键RTL/驱动/复杂诊断及最终验收；不必重做已有五轮或全面扫描仓库。

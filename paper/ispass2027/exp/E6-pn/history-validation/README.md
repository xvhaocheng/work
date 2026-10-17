# P_shared：IOTLB/TL独立小批

2026-10-18 JST，Codex-only。组件 **`5b7dfbc70e458244a8e5682e5ae3f8571320254c`**，直接 parent 为已接受 N 核心 `755cd723b20457418734700fae9e8d6f91d988c8`；仅两文件 **9增2删**。实际源码、patch、增量bundle及verify在 [history01](../component-source/history01/)，唯一prerequisite为755cd723。没有向第三方upstream推送。

IOTLB新增默认0编译参数`HistoryControlEnable`、内部显式输入`history_isolate_i`；只在既有 `partition_local_active` 前加 `(!HistoryControlEnable || history_isolate_i)`。TL公开history输入默认1，并显式传到IOTLB。`allocation_mask`、eligible tree、victim、stable fill DID、N probe/accept/touch、cache权限/内容/invalid和所有always_ff原字节保持。当前两个模块**零新增状态FF**；未来193控制字才需1个FF，尚未接。

Root已锁后续接口：193低2bit原模式、bit2history_shared/reset0，仅word0/1/2/3/6合法，4/5/7或任何高位拒绝；编译关闭时bit2读0且写6拒绝。194bit63固定RO history_supported，低位不变。**本批没有实现MMIO/quiet原子写或其拒绝逻辑**，不将这份接口说明当RTL通过。P_shared仍原single-controller/3beat，N/backend选择仍bit0；这些全局连接不在两文件diff中。

## 实际有限DUT

[summary.json](summary.json)记录实际计数/commit字节绑定；[evidence](evidence/)保留全部冻结输入、完整命令、原日志、trace无损gzip。原manifest的运行时HEAD/dirty不重写，归档commit另行按原字节绑定。

| 最终检查 | 实际结果 | 原件入口 |
| --- | --- | --- |
| IOTLB history-iotlb03 | 2959拍、8994次eval、49849检查 | [manifest](evidence/history-iotlb03/manifest.json)、[run](evidence/history-iotlb03/run.log) |
| 真实TL history-n03 | 476拍、14验证记录、2988检查、2真Naccept/4probe | [manifest](evidence/history-n03/manifest.json)、[run](evidence/history-n03/run.log) |
| 默认关闭history-defaultoff01 | 新RTL运行原已接受N testbench：4773拍、173记录、32577检查、4真accept | [manifest](evidence/history-defaultoff01/manifest.json)、[run](evidence/history-defaultoff01/run.log) |

IOTLB同时实例实际新runtime输入、编译关闭但history输入有噪声、固定shared、旧d2a local/shared参数实例及原85b fullmask gold。reference只重命名module标识，不改算法。逐eval比tags/content/q/n PLRU、victim与lookup结果；fullmask/PolicyEnable0始终比85b。包含随机S1/S2/global/page标记/invalid、resident-way局部归属、真实共享历史污染反例、0003/fffc fill一hot边界、同拍fill/touch用旧victim、clear压过update/touch、zero mask及两种history独立reset后clear。共享额外600拍随机mask/invalid；没有新增victim算法。

TL参考为755cd723原TL/IOTLB：local参考仅重命名module并绑定原IOTLB；shared参考另把原已存在的`PartitionLocalHistory`参数设0（唯一验证配置变换），不重写whole-tree touch。每eval比完整ready/response、HPM/fault、DDTW/PDTW/PTW/MSIPTW AXI，以及两cache tags/content/q/n PLRU/victim。候选实际缓存、实际原walker和真实FLAT context；两种history各接受一次真正的N hit，probe不touch、touch只真accept，fast响应held时slow到达仍不变，P mask/victim持续按stable DID落0003/fffc。另核P_shared runtimeN0正常TL路径无Naccept。

shared+N独立输入组合仅作内部模块composition stress，**不是公开MODE7**；Root锁定7拒绝合同保持，后续control批实现。默认关闭兼容检查复用旧testbench，目的是验证新默认参数/公开默认输入确实不破坏已接受RTL，不是新论文重复样本。所有验证记录均非论文trial，板点数0。

## 原失败与修正口径

生产9增2删在所有上述run之间未改。

- history-iotlb01在history改到新参考但flush尚未生效的pre-edge强比旧q，引发断言；02跳过clear边沿后仍强比clear后invalid旧content。**现有clear只invalid tags和清PLRU，不清content数组**，历史路径不同的invalid旧bytes可以不同，不应要求切换两份历史gold时全字节相等。03改为各history独立reset、从同一内容快照出发逐拍比对应旧gold，再测试各自clear胜过fill/touch；不靠弱化有效cache检查通过。当前没有原子MODE切换RTL，不能声称这些case验证了尚未实现的控制器。
- history-n01的test wrapper将带默认值的旧/新TL输入直接alias到同一顶层input，Verilator首次eval的INITIALSTATIC把C++此前给的N=1覆盖成0（原trace清楚为0）。02在reset内先执行初始化再恢复受测控制，并显式核值；03补P_shared N0正常路径。没有因此改生产N逻辑，也没有把01普通TL握手当真Naccept。
- IOTLB编译/随机过程原告警、TL原DDTW UNOPTFLAT及test-type ATS bank未驱动告警完整保存；未屏蔽新失败、未改PTW/DS/package。

## 展开风险与当前接受理由

[elaboration-risk.json](elaboration-risk.json)取自实际history-iotlb03生成模型，保存原generated文件路径、SHA、行号和明确derived赋值摘录。编译启用的predicate生成结果是`history_isolate_i AND 原predicate`；编译关闭的两份生成赋值均无history输入。源码diff没有时序状态/lookup/victim改动，与上述实际逐拍等价一致，当前没有证据要求放弃开关。

这只是有限Verilator展开，不是综合网表或Fmax估计。新history输入扇出到多层既有hit→PLRU更新qualifier，仍可能落在该D路径；目前不承诺零时序代价。后续整体集成前由Root据实际diff/有限证据选择是否启用；50MHz约束、DS0 WNS>=0/三失败端点0/routing0、至多一次历史postroute优化、失败即停规则不变，不为开关反复build或改约束。开关版235独立板点，关闭fallback205；S15未实板通过不得构建DS50。

复查（只核原件/trace/source，不重新仿真）：

```sh
python3 paper/ispass2027/exp/E6-pn/history-validation/check_evidence.py --iommu-root /home/xvhaocheng/work-e6-pn-iommu
```

有限重新执行入口为`run_history_iotlb.py`、`run_history_n.py`，完整参数在原manifest；输出必须新目录，build180s/run30s、j2、无硬件访问。模型/原生窗口元数据未暴露，未改模型缓存或手动窗口；DeepSeek未参与。

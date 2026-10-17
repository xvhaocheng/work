# P-only RTL小批：原版gold与有限真实DUT

2026-10-18 JST；Codex-only直接实现/读源/验证。组件提交
**`d2a813d67bcfd0295d8934c3cc027d24d3195d63`**，直接父提交
`85b157ebe71a623e326e51fff7b7126fe77e8eb9`，组件工作树
`/home/xvhaocheng/work-e6-pn-iommu`、分支`ispass/e6-pn`。

只改两个生产文件，75插入/10删除：IOTLB增加默认关闭的mask/clear/局部history，TL增加默认关闭的P端口/参数并从稳定`req_data.did`选择固定DID10的0003及其余fffc。
P关闭fullmask/local inactive；不加TouchEnable/N、banked、observer/research端口，不改normal gate/response arbiter。PTW、DS owner修复、package、MSI/MRIF能力均保留85b原字节。
当前未把组件pin进CVA6/platform、未实现模式MMIO/quiet写保护；外层调用仍默认关闭。`policy_clear/partition`只是在下一wrapper批需全quiet执行的功能输入，本批不能声称已完成系统级模式控制。

[summary.json](summary.json)记录实际计数、源码commit绑定和范围。
[两份生产原字节与恢复入口](../component-source/p-only01/)含完整patch及incremental Git bundle；bundle仅依赖85b，verify原输出已保存，未创建remote或推送第三方upstream。

| 实际执行 | 有界结果 | 原始入口 |
| --- | --- | --- |
| IOTLB最终run04 | 2256周期、6821次eval、19065项检查通过 | [manifest](evidence/p-iotlb04/manifest.json)、[原run.log](evidence/p-iotlb04/run.log)、[cycle CSV无损gzip](evidence/p-iotlb04/cycle-trace.csv.gz) |
| TL run01 | 3541周期、200笔真实翻译、20252项检查、113fill/335PTW AR，通过 | [manifest](evidence/p-tl01/manifest.json)、[原run.log](evidence/p-tl01/run.log)、[逐请求](evidence/p-tl01/cases.csv)、[cycle CSV无损gzip](evidence/p-tl01/cycle-trace.csv.gz) |

这些是有限RTL检查数和翻译请求数，**不是板测n或论文样本**。FPGA综合/时序/板点均0，无新硬件性能结论。

IOTLB wrapper同时实例化candidate-P、默认关闭candidate、fullmask启用candidate、共享history反例，以及85b原字节gold。gold只改module identifier；所有q/n tags/content/PLRU、victim和lookup结果逐eval比较。随机有限序列覆盖S1/S2、global/权限/大页标志及invalidation；P方向测固定mask内一hot填充、resident-way归属（故意以对侧fill mask做hit）、真实共享history污染反例、同拍fill/touch用旧victim、clear胜过update/touch、zero mask无fill。

TL wrapper实例化真实TL/DDTC/DDTW/PTW/IOTLB与85b gold，保留MSI_BT_MRIF、PC与扩展64B DC，输入真实FLATcontext但payload在MSI范围外。gold除声明重命名外，只将内部IOTLB实例绑定到85b gold原字节。S的99笔请求逐拍比ready、完整response/DS请求、HPM/fault及cache tags/PLRU；默认关闭实例持续与gold比较。P的98笔请求核稳定fill DID/mask、A churn后V两页不rewalk；另外3笔核clear后真实rewalk/2MiB/RO权限错误。S序列还实际走4KiB/2MiB/1GiB和RO写错误。共享C++内存只提供合法DDT/PTE读返回及确定AR/R/response反压，不替换TL/walker/cache为公式，也没有自造backend completion。

已执行的所有run保留，未覆盖：

- [p-iotlb01](evidence/p-iotlb01/manifest.json)是test wrapper的cfg enum隐式0被Verilator5.040拒绝；改为显式enum后run02通过，生产RTL未变。
- [p-iotlb02](evidence/p-iotlb02/manifest.json)原通过记录保留。随后新增真实shared反例/同拍fill-touch覆盖，不把原已过范围重跑冒充新独立证据。
- [p-iotlb03](evidence/p-iotlb03/run.log)新增反例的priming前提失败：先V touch已让shared/P两实例A驻留路不同，下一次V touch不必再改shared A victim。原失败wrapper/C++/trace全保留。run04把A priming放在首次V touch之前，保持两实例初始A placement相同，再测V touch；真实shared victim变化、P victim不变，两项均通过。

两个最终run执行时的candidate原字节已与`d2a813d`逐字节核对，见summary的`source_commit_binding`。原manifest诚实保留执行时`HEAD=85b + dirty diff`，没有事后改成归档commit；归档commit绑定另列。每个evidence目录含实际source快照、完整命令/原始log与hash；gzip CSV解压后与manifest的原SHA一致。

Verilator告警原样保留（`-Wall -Wno-fatal`，TL开启`--assert`）：除命名/未用/宽度等，TL有原DDTW `en_msi_check`的UNOPTFLAT路径及reset assertion的SYNCASYNCNET。DDTW、PTW、MRIFC执行输入均与85b逐字节相同，没有以P改动修正或屏蔽这些源。该仿真告警不代替Vivado routed loops/timing验收；后续完整构建的既定硬门仍必须通过，当前未综合。

复现入口（只在Root需要时执行；输出必须新目录）：

```sh
python3 p-validation/run_p_iotlb.py --iommu-root /home/xvhaocheng/work-e6-pn-iommu --verilator <Verilator-5.040> --out artifacts/<new-iotlb-run>
python3 p-validation/run_p_tl.py --iommu-root /home/xvhaocheng/work-e6-pn-iommu --deps-root <frozen-baseline-iommu-with-vendor> --verilator <Verilator-5.040> --out artifacts/<new-tl-run>
```

工作目录为本任务E6-pn目录。两个runner分别限制build180s/run30s、并行编译2进程，拒绝覆盖已有目录；实际完整路径与命令已在manifest。没有调用旧out runner，也不写主树或实际构建输入。

待Root直接审两文件实际diff、gold/原始回执后再接下一批。当前没有N实际拒绝、双backend/controller、MMIO mode、cooldown或A/union实现；没有完整系统S等价、MRIF实际事务、物理timing或板测通过声明。

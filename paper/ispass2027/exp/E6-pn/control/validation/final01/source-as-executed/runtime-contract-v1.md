# E6-PN：运行时接口增补（Root冻结，尚未整栈实现）

2026-10-18 JST。补充并优先于早期 `software-plan.md` 的寄存器草案；不改变原软件、S15命令、旧OBS3/PATH布局或已接受小批的历史记录。当前无完整PN BIT、无PN板点。

DMA0沿用INDEX `+0x70` / DATA `+0x78`；独立PN bank为192..199。

| index | 最终合同 |
| --- | --- |
| 192 | 只读 `0x49444d41504e0001`。独立PN能力v1，不以OBS3 magic冒充。 |
| 193 | bits1:0为原S/N/P/PN；bit2=`history_shared`，reset为0；其他位0。仅0/1/2/3/6合法，分别S/N/P隔离/PN隔离/P共享。4/5/7拒绝。N/backend选择只看bit0，容量分区只看bit1；P共享仍2/14及原single-controller/3beat。 |
| 194 | bit0=`mode_write_eligible`，bit1=`last_mode_write_rejected`，bit2=`certificate_fault`，bit3=`capture_busy`，bit4=`dataflow_quiet`，bit5=`original_observer_busy`，bits62:6为0；bit63为常量`history_control_supported`。全部只读。 |
| 195/196 | 原V窗口内DID10/11的真实N accept饱和32bit计数；不是probe。 |
| 197/198 | A自身/双方共同完整工作的硬件周期。分母和最终有效性分别保存，不借用V窗口。 |
| 199 | bit0 frozen，bit1 A有效，bit2 union有效，bit3 A端点溢出，bit4 union端点溢出，bit5边沿/角色错误，bit6 N溢出，bit7未知DID；其余0。任意bits3..7均使两有效位为0。Solo A有效位为0。 |

`HistoryControlEnable`默认关闭；完整构建前据有限源码/展开风险审查决定启用。关闭时193 bit2恒0，值6写入必须拒绝，194 bit63为0；不静默降级成隔离P。使能时默认仍隔离，只有显式193=6共享。IOTLB只在既有partition-local-active条件加使能/隔离门，不改mask、victim或数据查找。

193仅接受全8字节写，须所有DMA frontend/job/backend、AXI债务/持有VALID、两个TC、TL/各walker/invalidation及必要数据结构访问均quiet，原observer既非ARM也非active，新capture非busy，certificate无fault。同一接受边沿原子更新3bit并清IOTLB tag/PLRU；同值合法写也flush。拒绝不改模式、cache、counter或数据路径，只置194 bit1；该拒绝位reset清、下次合法接受MODE写清，读不清。194 bit0是完整配置接受资格，不能只拿TC state==IDLE代替。

`dataflow_quiet`不包含新capture的busy状态；observer冻结必须输入该独立quiet，否则会形成“等自己冻结才quiet”的死锁。`mode_write_eligible`才组合dataflow quiet、原OBS非busy、capture非busy、certificate无fault。具体源码tap仍须独立实现与实际有限测试，本合同不冒称已有完整quiet证明。

V供给间隔沿已接受frontend小批：DMA0 `+0xb0`全WSTRB/idle-only u32，`+0xb8`只读 `0x49444d4147415001`；reset gap0，原batch disable不清gap。S15保持reset S/gap0，不写MODE/GAP，也不增加flush。只读helper会写INDEX选择器，不写模式或数据路径配置。

含历史开关时：DS0 S15为15；主矩阵为5配置×3条件×10=150；隔离P/PN的A8 gap0/1000/4000独立补充30；同源DS50原四模式A8各10=40。合计235，各phase独立。若完整构建前因显著风险放弃开关则205，并记录P共享30点未运行及原因。DS50与gap补充不增加P共享配置；DS50构建仍须真实DS0 S15三格中位偏差均≤0.5%。

区组和顺序在采集计划中明确保存；“集中同一配置”以同Linux/BIT/DS为边界，在其中轮转运行时因子，各点前必须quiet设置和确认读回。比较只在实际同phase/DS/boot及已定义完整区组内配对，跨boot/S15与历史基线不构造配对。不得仅用相同的重复编号冒充同一实际区组。

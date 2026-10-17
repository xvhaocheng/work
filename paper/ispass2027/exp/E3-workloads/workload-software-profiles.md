# Full E3 的两个精确软件 profile

本小批只改host身份选择、有限检查及出口。完整E3正式板点仍0；不改driver/Image/RTL、正在续编的FPGA或G自己的实验协议。
Root为减少一次SD回写，接受准备额外精确G profile；正式使用仍须Root审实际小diff/回执后绑定实际新boot。

`workload_software_profiles.py`只允许：

| profile | Linux | platform | 软件CVA reference | release |
|---|---|---|---|---|
| full_e3_65b_v1 | 65b427946152e3aca27babab0a437821729805d4 | ec99648e3c9a1e48c1e9721e3662bb632eb439bd | 1d587125eee690e623e6ffae13e70f9bbe989adc | 6.5.0-rc2-ispass2027-full-e3-v1 |
| g_shared_69d_v1 | 69d8b9d4549c473477bcdc1df7c09ee1bb55528c | 6491dfd76fc92dc020d17166b327da93605f58b1 | cfb9a0a8bb70b32be6036bc112a5189d6e2aa0ab | 6.5.0-rc2-ispass2027-g-v1 |

65b分支保留原tuple/status/reference-only/idma.c/UAPI检查，默认kernel仍相同；不回写任何旧host结果或原冻结源码。
新G分支另外固定schema=ispass2027-g-software-v1、整份正式passed manifest原字节SHA：
`4bee1eadd8d8102e652129833f171ba869e5edd8a1a039973d13559a3a0b2f6c`。
原manifest的旧prepared note不更改、不据此推断构建状态；判断依据是正式passed身份及实际Image/命令记录。
G idma.c固定dfdad8be…39158，ABI5 UAPI与旧版相同f7b17880…713f，
正式Image固定76241803…de36b；共同原软件manifest固定7415dcc4…bcc4。
固定保留414个rootfs entries与五个旧idma CLI（experiment/P0/scaling/pages/workload）及timeout/updater的原ELF SHA，
其中idma_workload仍7cd7ae40…a2119。代码列出全部完整SHA；不会只按release前缀或说明文本接纳任意G软件。

两profile的实际FPGA要求始终独立：CVA必须1d587、DS0/50MHz、BTC3/OBS5/Depth64/INFO2，BIT SHA与实际load receipt一致。
cfb9只能作为G软件archive引用，绝不等于已加载完整E3 BIT。
FPGA原manifest的Linux da6 reference-only角色不变，新G软件也不伪装成65b原payload。

collector与analyzer共用窄helper，默认expected-kernel由精确profile选出；显式不匹配kernel仍拒绝。
profile与helper SHA加入原resume冻结字段（原四工具SHA和六source原bytes仍全部保留），
不同profile/source/boot或工具版本不能沿用旧prefix。完整E3协议/150plan/命令/几何/数据/排空/HPM/PATH/hist没有改动。
analyzer快照包含新增helper原字节，所有输出仍绑定实际kernel/boot，G fixture仍不能发表成板数据。

## 实际有限验证

- 新source下旧65b、G两个actual collector完整入口各32项通过：pilot30→只执行剩120、150cell分布、fail-stop、
  exactcommand、seq、所有冻结身份篡改、nested fixture及publication拒绝。G测试始终保留正式软件manifest原字节，
  fixture标记放在load/FPGA/run，而不篡改正式软件manifest绕过SHA。
- `verify_workload_profiles.py`定向typed+actual入口27项：未知schema/tuple/release/reference、driver/UAPI、所有保留exec、
  rootfs/parent/Image/status与仅JSON空白变化均拒绝；65b→G及G→65b续接均拒绝；cfb9冒充actual BIT拒绝。
  G出口重新解析150条原fake-UART，内部始终synthetic/0板测，默认发表拒绝。
- 最新analyzer27项回归通过；加入第五工具helper的身份与46文件快照检查。
  旧65b与新G的合成硬件/字节/覆盖/PATH/HPM统计和配对完全相同，host命令wall实测耗时与软件身份另计。

回执分别为test-results/workload-profile-{legacy01,g01,finite02,analysis01}-report.json，原finite01及旧全部冻结回执保留。
本批source snapshot与G正式manifest副本另存source/workload-software-profile01/，不包含二进制或新板数据。

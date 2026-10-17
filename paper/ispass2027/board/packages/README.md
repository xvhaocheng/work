# 实验配套包

每个实验或完整采集批次集中保存：结果入口、实际代码commit/构建清单，以及本机`artifacts/`中的image和BIT。条件、启动及配置之间的配对写在同包manifest中。

| 实验 | 结果与版本 | image/BIT |
| --- | --- | --- |
| [十组历史实验](history-20261017/README.md) | E1、batch/扩展/DS0/回切、path、E3v6、E2、P4M、composition；4096条长表行 | 每实验manifest配对既有提交、实际内核及本机BIT/payload路径；缺失为unknown |
| [历史G1复现](g1-20261017/README.md) | 9个唯一实际试次，G1已通过；结果及失败/恢复来源均已关联 | 00082 payload + 历史batch DS50 BIT |
| [统一100次基线](baseline100-20261017/README.md) | 正式100次全部完成；DS50/DS0下五条件各10次，实际ABA三阶段25/50/25、三个boot，结果/加载/执行源码均已关联 | 同一新payload + DS50/DS0两个BIT |

包内`manifest.json`对应条件、二进制和结果，`source.json`指向代码commit、构建记录及`metadata/`中的版本锁、patch/bundle等恢复材料。G1已关联原始、失败、恢复和accepted结果；baseline100已关联三阶段原始records/review、三个load、campaign/launch结果及启动时协议和实际源码快照。G1的9次与正式100次分开统计。

实际写SD的是`fw_payload.bin`；baseline100另存的Linux `Image`只是组件。二进制保留在同包本机`artifacts/`，不进Git；Git同步manifest、元数据、源码恢复材料和结果入口。G1历史构建含dirty树，完整实际执行树的单一commit明确为`unknown`；不以当前HEAD替代，也不为补齐旧身份重建或重跑。配套整理沿用已有记录，不增加重复哈希、SD回读或独立确认。
历史系列保持原归档，配套manifest提供统一入口；[数据说明](../../data/README.md)保留详细口径。后续实验沿用本配套结构。

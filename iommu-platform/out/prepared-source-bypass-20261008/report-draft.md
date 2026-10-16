# §39 普通已准备源物理旁路：阶段汇总（草案）

拟路径：`docs/prepared-source-bypass-boundary-20261008.md`  
日志证据相对路径：`../out/prepared-source-bypass-20261008/...`

## 执行与证据
六项运行（off/cache/wait × v/a）均 evidence_ok、6 个 DUT PASS；1 个新 build exit_code=0、reused=False。off 两个 CSV 与 §38 bypass-aw-first 对应 CSV 逐字节相同。汇总脚本本轮只提案，未执行；以下数字来自根已提供证据。

## 关键发现
1. cache 的 A-first 不是镜像：V-first(v,a)=17704/20246，A-first(v,a)=20279/17737；交换后两流各 +33 cycles；V-first mean=18975，A-first mean=19008。不能当镜像复制；off/wait 为真镜像。
2. 致命检查必须为 0：DIRECT39-STATS offered/busy、DIRECT39-HOLD held、BYPASS38-STATS held；direct+fallback=64 与实际 DIRECT39-AR 行数，以及 tc_state=2/write1 计数与 write_wait 一致。off direct=0/fallback=64；cache direct=62/fallback=2（write_wait V25/A16）；wait direct=64/fallback=0（write_wait 24/24）。
3. 六项 TL=66、hit=32、miss=34、PTW=102、DDTW=2；源 physAR=64、目的 AW=34；每流 4096 R/W；bytesguards quiet。V 先均值 off=24751、cache=18975、wait=19565；旧 post=18824。

## 精确数学与边界
以 cache 均值 18975 作分母，post 比最强普通 cache 低 0.795783926%，残差 151；原 5927 差距缩小 97.45234%。Codex 独立结论：普通物理旁路已有 Arm LTI 1.1/1.2.2/1.2.3 前史，且消除 >97% 原差距，不能把旧 23.95% 或位于 TL 之后包装为独立机制。若 151 只是一般仲裁/准备时机/固定流水差，不能立新算法；只有同约束普通旁路/预约/队列不能提供的进度或成本性质、具体前作未覆盖，才可立题。不是创新已成立，不是生产 PN 根因，不承诺二区。

## 根已核与范围
根已核 criticalRTL/normalTL 权限、ID0/4->1/5 和 R 还原、源单全局 child 独占 AR 路由；static 为正常 PoC，非 fault/epoch/invalidation 通用证明。下一步：基于明确真实寿命约束 + W 义务找独有不变量，不继续刷普通 prefetch 百分点。

## 归属
DeepSeek 汇总/报告草案；Codex 架构、执行与关键审核。Arm 来源：https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/IHI0089A_amba_lti.pdf

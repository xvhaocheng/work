# §75 Codex 机制判别与下一门槛

本阶段是 ordinary ShortPrefix + fallback 夹具和可加入窗口观测，不是真正的 in-flight join。源码设计、E=N边界诊断/修复及实际执行由 Codex 完成；驱动cutover-after-rv参数由 DeepSeek 提案、Codex审查应用。独立关键席仅审查并发边界，无测试执行。

## 新实测

29次DUT执行，27次完整copy PASS，run17/run21两次旧版本终端拒绝（route not bound，不是watchdog/死锁），分别是真短prefix和旧E-prefix对照。修复后对应run22/run23均PASS；完整工作量和源/目的payload仍须独立audit。另7次非法CLI在构造DUT前被拒绝，不混入DUT数。

ShortPrefix只在0<J<E激活：A发不可改的[0,J)，逻辑LAST仍按原256拍parent；A原有短W闭合后立即普通fallback，不让V等A。每个future新W先全payload resident，短prefix端点需剪裁，原先VALID的AR/R/AW/W和pendingB不撤。J来自实际V r/W承诺闭包（最小模式round-even）；因此当前激活J是偶数，奇数原生A页尾并不等于测试oddJ。

当前各有效short观测均没有 oldVbusy && rV=rA=rawWV=rawWA=J<E 的加入机会。run06/run24 J8/E64释放时rV46/rawWV44/rawWA8；run14 J2/E128释放时rV26/rawWV24/rawWA2。run26/27把J延到24/32，V仍到58/74。run28 J64时已超时独立继续，rV134，不能误称原[0,128)仍在途。uniform B=0/64/128/256/1024/4096与R间隔0/8/32只说明这些条件没有窗口，不证明一切真实场景都不可能。

V先取得WRITE成功后发source READ，A WRITE仍在实际helper准备（如run17 PREP4日志V WR2423、V READ2426、A WR在以后），因此J捕获后A还需独立许可与prefix工作。单个held物理R不得撤/绕过，这也限制A追赶；不能用先冻结V RREADY的做法制造窗口。

## E=N 的真实缺陷

old_end=256使可共享future suffix为空，旧deferred规则仍等4096或join_ready；V原READ成功项已逻辑完成而释放，不能再要求未来pair join。两作业DONE先于路由决策，严格终端CHECK拒绝。修复在oldV真正物理退休且rV=E=N时立即绑定独立（share=0），不删终端检查、不撤旧请求，理论sourceR仍512。旧source/executable/log保留；非生产RTL故障结论。

## 经典前作扩大了否决范围

1998 Hua/Cai/Sheu的Patching已提出晚到client补前缀同时接现有multicast。2001 Eager/Vernon/Zahorjan综述与HMSM还明确列出piggybacking：动态改变服务速度让stream位置对齐再合并，可用较小buffer；并专门处理有限buffer。故“补前缀后合尾/改变速度追赶/用小buffer”三个短语不能当新贡献。

- Patching 原文§3.1/§3.2（pp.4–5）：https://www.cs.columbia.edu/~danr/courses/6762/Spring01/week8/patch.pdf
- Minimizing Bandwidth Requirements for On-Demand Data Delivery 原文引言与§5.3：https://www.cs.usask.ca/faculty/eager/TKDEpaper.pdf
- Intel pending scoreboard + dynamic read merge/multicast return：https://patents.google.com/patent/US20200402198A1/en

以上对照来自读过的正文，不能声称原作者未解决一切权限/硬件问题。本源使用实际独立READ/WRITE证书、物理不撤、logical独立LAST、B3/q2和global单skid；这些属于具体问题条件，组合不自动等于创新。

## Codex 后续可被否证的新设计问题

精确rawW同步是安全的充分条件，是否过强需另核，不能把当前0窗口当一般不可达证明。可能的更宽join门：双方logicalR位置T相同、各自已firstVALID W承诺端点<=T、所欠W payload全resident、双方授权/PA/属性相同、无A物理child和冻结skid；rawW允许分别落后<=既有2拍。新W在T先关闭旧网格，然后才打开共同q2，旧B仍活；不得直接切phase3丢掉旧V路由。这个门、其循环自由性与actual可达窗口都未实现/验证，不能把纸面宽门计成成功。

下一实测应先用可解释的各设备目标服务差与合法跨ID B退休模型核真实catch-up机会；uniform FIFO B会让慢V也挡A的B。B模型改动必须保留同ID顺序、heldB稳定、W顺序和默认逐字回归。不能为了成功强制选地址或暂停V，不能把合成负载当应用trace。若加入只复现普通stream-merging/late-read-combine，不能称独立创新；若能给出前作未覆盖的不可撤协议边界、同总资源实现和真实增量，才可形成论文贡献。

## 更宽门的源码必要条件（Codex 源核；未实现）

关键席独立质疑与root逐条件推导得到：仅改变oldV recipient不够。加入沿要组合req=11且禁止A[T,E)fallback；joined后必须解除旧ShortPrefix的永久T clip，改成先闭合旧W网格再按peer rawW门打开T起新W；旧V ID/LAST路由仍保留到E。phase1还冻结V的R，故要明确joined推进phase2的条件，或允许锁定会员等待V自己的resident W闭包，不能隐含等A。全部原W承诺<=T且logicalR=T保证旧W不用未来R填补，但仍需现TC/全局W顺序以及公平B服务，不能称所有环境下形式证明。

当前SHORT75 observer只在A rawW=T后采样；因此它的0不否定rawW未全闭合的更宽窗口。下一优先核宽门能否真实出现、旧W循环能否排除，再判断是否需要异步目标B模型。这里只是机制合同，实际源码仍ShortPrefix＋普通fallback，没有joined state、req=11或宽窗计数。

§75新增1个functional FF（short_released）与134个observer FF（96计数＋36位置＋2条件），另有组合门；0新增payload。未综合，不能声称面积与旧法相同。全平台metadata、旧recipes、控制状态仍要共同计费。

## 宽门的具体拒绝反例（未来错误接法；非当前DUT故障）

关键席提出、root按当前eligible/private-slot门核过可达顺序：T8/E64，双方r8，rawWV6/rawWA7，旧W承诺不超过8。若只锁oldV_joined且后来允许双方接新beat8，不显式撤ShortPrefix旧pending，那么在V旧W先排一拍、A旧W尚未排尽时可以按各自lead<2接收该beat。冻结skid交付后rA变9；等rawWA到8时，旧release仍要求rA==8，已不成立。top会永久clip A新W至8，现有两拍有限先读耗尽后R也停，形成内部环。若A W先关闭则未必遇到此序列，反例不等于所有时序必败。

因此正确未来绑定沿必须显式退出short_pending，且用joined网格门：start<T仅剪裁到T、start>=T走peer rawW检查。原short释放谓词不能原封不动留着等自行解锁。这个反例限制了新实现的正确性，不证明这样的机制是创新或已成功。

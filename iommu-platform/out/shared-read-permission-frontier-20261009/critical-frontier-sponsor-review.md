# §54：长读的推进保证、自动退组与强普通对照

Codex 独立关键并发/权限审核，2026-10-09。只读本stage shared_pretl_read、actual top和q writer；未构建、运行或改功能RTL。根席在审核期间加入build03 sponsor admission与mode0对照，本文直接核了最新行。此前缺guard的版本仅作为设计反例，不写成最新版缺陷。根席报告此前6个actual copy PASS；本席不重复验收统计，也不将当前在跑的mode0写成已通过。

## 判定

最新版 [shared_pretl_read.sv:183](/home/xvhaocheng/work/iommu-platform/out/shared-read-permission-frontier-20261009/shared_pretl_read.sv:183) 明确强制mode1的**V WRITE成功前沿覆盖整条将首VALID的physical child**，接受这个安全门。当前只有A可自动退组，因此“V或A任意一个覆盖整段”不够：若只有A覆盖而V到128，V仍留在required mask并停住长R。当前保证推进的成员必须是V。

普通pagecut→V独立suffix→A补读可以同信息/同recipe/同payload buffer达到R384、AR3；candidate对应R384、AR2。两者的实际区别因此缩到一个physical AR重发，以及相关调度/请求代价，不能以普通必须在页界让V等A为主张。新的mode0需要实测，但其协议路径已经能具体构造。

## 1. 确切 admission invariant

设child起点a、长度L、末端b=a+L；F_f是flow f已经真实WRITE成功、连续覆盖的job end，以beat计。为保证在共享DDR返回头不能被未来PTE overtaking的模型中完成该child，需要：

1. 每个初始recipient的真实READ recipe覆盖其将得到的source区间；PA相同、normal-memory属性兼容；不可只借另一个DID的授权。
2. 在physical AR**第一次VALID之前**选定一个将持续接收至b的成员s，它已有有效WRITE recipe覆盖[a,b)，而且其q-W供给不依赖新walk、不被已退出成员的group barrier卡住；即F_s≥b。
3. 所有未覆盖到b的成员都可在自己的F_f边界退出，且退出前能排空既有不可撤q-W/B和冻结skid；这些成员不能以未来WRITE walk为继续原长R的前置条件。
4. 成功证书/配置在该child内有效；不允许s途中暂停、失效或被正常控制从mask拿掉。若允许最后recipient消失，必须另有显式尾R排空/停车合同；本stage没有这个路径。

这里“推进成员”不只是`max(F)>b`这个数字。它同时要有真实正常WRITE授权、能持续通过R credit/coverage门、以及能绕过后台walk返回自身已保存的WR response。当前helper的成功recipe可在background preparation pending时复用；actual q-W对新child另查cache_valid，缺证书不会首offer。

当前实现只清required bit1（89），只以A到前沿触发自动退组（164–175）。所以s必须取V=0，精确门为：

```systemverilog
(mode_i != 1 ||
 certified_end_q[0] >= returned_q[0] + shared_beats)
```

该门在179–195的新child注册处检查，先于offered_q产生的firstVALID；held AR不被缩短/撤回。mode0/2的shared_beats39–45则直接限制到两个已认证前沿的共同末端。

F_V来自136–137：只有cache_valid_i[V][1]成立才推进。actual top971–972的write_recipe_end由真实rawAW起点/长度减job dst得到，不是source.w。当前V dst页起点、2KiB length使其一actualWRITE parent覆盖整个256beat；其private parent/recipe在全部数据和B之前保持生存。

新guard防止不安全长AR，不自动保证所有合法destination offset都有进展。例如V/A均只有128认证时，mode1现在在起点拒绝长256，若没有fallback就会等待。窄合同需明确V整个child已有full-parent认证（根席计划V dst pagealign、A offset0或3072）；若扩一般offset，则无推进成员时应选择短pagecut或另有排空路，不能把guard当成通用liveness算法。

## 2. 旧版缺guard的真实可达设计反例

两个源同normal immutable 2KiB页，两个destination都从页内3072开始，故两流第一actualWRITE parent均1024B/128beat，下一父跨页。第一页READ/WRITE均真实许可成功，F_V=F_A=128；第二页不在IOTLB，需真实walk。

旧mode1 shared_beats直接取remaining256，而旧admission只检查F_V/F_A大于当前cursor0，因而offer并accept长AR256。正常完成前128beat和两个旧W前缀后：

- A到F_A触发退组；只清A的required位。
- V仍required，但R beat128被`beat_index >= F_V`挡住（91–92）。
- V的第二rawAW可私有捕获，但第二WRITE preflight需要PTE返回，排在held source R后；V没有前沿外的recipe，所以不能让R越过128。

形成`held长R → V第二WRITE walk → F_V推进 → held长R`。不是权限非法，也不是不公平内存；所有旧父可以正常退休，卡住的是未知下一页翻译与不可撤读。若只有A的F_A=256而V=128，旧“至少一个full”检查也救不了此实现，因为V没有退出路径。最新版183拒绝上述长AR；这正是guard的因果作用，不是已运行的新失败。

## 3. 自动退组的旧q/skid义务

新版仍有以下实际保障：

- source136–137只把真实WR成功end存进F；R ingress91–102逐beat查当前required成员的F，不向A交付前沿外payload。
- 221–226的physical handshake冻结skid_pending mask，230–234逐成员logical handshake清位；已有logical RVALID保持（129–132），不能在frontier事件中撤回。
- 164–166先将A置detach_pending，a_hold79使新A qchild停发；q writer72–80只禁止**新**child，已offered AW和既有W/B继续。
- 166只有q_quiet且无A skid才真正退组；q_quiet是q writer24的!child_live，并在实际子B81–100退休后成立。pending prefix中没有偷偷吞AW/W/B。
- actual top685的新q-W额外要求当前row1 cache_valid；686在退组后解除peer-group等式，V不因A停在旧group而停止。A的新globalAW还受V logical RLAST fence，避免缺A数据的AW排在V续流前。

必须保留一个细节：该stage允许在active时借用至多两个已有private payload槽（source93–102）。因此A可能已经logical返回到128，但旧parent最后两拍尚未有global qAW；old q/B quiet时这些数据仍停在A私有FIFO。它们不是冻结skid，也不是已global首offer的W，允许退组后私有保留；**不能**把此时解释为A首actualWRITE parent或rawAW/B已退休。A raw_written可能小于returned，parent仍live，之后仍需用自己的证书完成这两拍；观察/成本均应计这份private payload。

如果另写“temporary drop保证A的first raw parent完全退休”则与当前quiet条件不等价。若要求这项更强合同，需要额外检查raw_written达到F、rawB已接受；本次不需要它来使V长R推进。

retain分支170–172也有限定：必须新WR证书已成功、整个skid为空且两个logical cursor相同，才撤掉pending继续同一cohort。否则只能退组，不允许将已分叉cursor重新拼成同拍共享；已经首VALID的child不变。

## 4. 同读量的更强普通路径

令共同第一前沿F=128，N=256；V已经认证至N。强普通可执行以下三条physical AR，均保留同一256beat logical source parent与独立context授权：

| physical AR | recipient | beats |
|---|---|---:|
| src[0,128) | V/A | 128 |
| src[128,256) | V | 128 |
| src[128,256) | A，取得第二页WRITE证书后 | 128 |

总R384、AR3；候选一条shared-long256加A补读128，总R384、AR2。都写两个目标各256拍。普通不需要额外payload保留或从未获授权的目标返回结果；它选择在共同前沿结束第一个AR，然后按已经认证的V前沿继续单独读。

关键顺序不能省略：先私有保留A的下一actualAW，但A新的global qAW与下一页preparation暂缓到V logical RLAST；V独立suffix不被A新PTE请求先入共同DDR返回队列阻挡。最新版actual top981–985已给mode0加这项**新准备**eligibility门，rawAW capture仍开；此前parent退休的invalid gap确保第二页prep没有在门生效前抢发held offer。配合已有A AW fence与peer-group bypass，以上是具体可实现的强普通构造，不是口头“普通也能抄算法”。

mode0 source164也到共同前沿触发A退出，200–210选择V solo，再在Vdone后A solo；solo_beats49–52受自身认证前沿限制。两者共用state/recipe/skid/q-W，固定256 read quantum可给每个suffix一次AR。当前首次mode0运行由根席验收，本席不声称已PASS。

若A下一WR在前沿处已成功且cursor未分叉，普通还可继续共享后128，总R256/AR2。mode1在该条件下也可能retain而免补读；不能把冷walk的R384结论当作所有trace必然流量。

## 5. 本轮增量与不能强迫所有flow关闭的边界

因为上述普通同信息路径达到同payload、相同允许未完成A parent、同样V继续，它直接否定“普通必须等所有flow的第二页授权才能让V前进”。本轮candidate在这条冷第二页trace保留的具体增量是**跨A授权前沿继续已accept的长physical AR，少发一条V suffix AR**；不是额外减少源payload，也不是独享WRITE-preflight或独立推进能力。额外请求的真实cycles与后台walk次序须实测，不能从AR数推断幅度。

如果mode0实际在同成本/同许可/同q-W义务下也同效，则不能继续用mode2共同pagecut等待或mode3全独立R512来论证大幅独有收益。若mode0只多一AR且candidate有稳定收益，可保留这个明确而较窄的微架构差异，再核其前作和完整资源；本审查不凭“所有普通都可再加任何模块”否定它。

V的保证推进承诺只要求持续至当前child末端，不要求整个系统所有flow关闭；A的parent/下一页walker可以继续私有等待，V完成后A再推进。反过来，若允许V也暂停、V第二页未授权或证书失效，当前stage就失去这个保证，不能强制所有flow停住而仍声称解决了独立进度。源码134显式排除external pause，单job/static正常合同仍须写明。跨页一般化和全成员停止的尾R义务是另一个合同，本次不替它作证明。

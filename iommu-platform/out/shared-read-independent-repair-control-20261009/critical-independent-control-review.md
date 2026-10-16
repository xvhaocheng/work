# §69：独立 V continuation 的窄协议审核

Codex 亲自审当前 source/top/q splitter 与 §68 对应文件的实际 diff、严格 C++ guard。未修改功能源码、构建、运行，也没有以构建日志或待跑配置证明通过。本稿只审新增 ShareRepairSuffix=0 路径；不重新宣称 §68 全架构已通过。

**结论：当前 mode3/A初始cursor0、单 static/immutable 正常2KiB job、两独立 READ/完整 WRITE recipe 合同下，未发现新增 AR稳定、双路元数据或 physical RLAST 阻断。V continuation 的1bit offer可以依赖停止游标/原header/static PA重建；A main metadata保持。严格 R512 不是任意初始 FA 的通用公式。Share1 的实际 diff 保持旧行为，但 CSV exact 必须由 root 执行。**

## 1. V first-VALID 不会覆盖一个 A first-VALID

[shared_pretl_read.sv:281](shared_pretl_read.sv:281) 的真实创建门：

```systemverilog
if (ParallelPrefixRepair && !ShareRepairSuffix && cutover_phase_q == 2 &&
    !independent_v_offer_q && !offered_q && !old_v_busy_q &&
    returned_q[0] == old_end_q && old_end_q < 256 &&
    (busy_q || returned_q[1] == 256) && cache_valid_i[0][0]) begin
    independent_v_offer_q <= 1;
    child_count_o <= child_count_o+1;
end
```

main A builder [source:288](shared_pretl_read.sv:288) 要求 `!offered && !busy && !skid`；它真正创建 A AR 还须 `returnedA < repair_end` [292](shared_pretl_read.sv:292)。因此两个创建事件不可能同时成立：V门的 `busy` 分支排除 A builder；V门的 `returnedA==256` 分支排除 A未完成条件。A已offer但未handshake时，`!offered` 也阻止 V 创建。

同沿 A physical LAST 与 V offer 创建合法：旧状态 busyA1允许创建，A LAST清 busy并生成A skid。该 offer只抢接下来唯一 AR 输出，不覆盖A已接受的 metadata。待A最后skid交付后 returnedA256，builder不再发A重复AR。

现有 V offer 保持期间，A builder不会再创建：首次V创建时A已busy；A结束后的 physical-LAST skid会阻止builder，skid交完时A已returned256。不能把此结论扩到多个 A物理children或多个jobs；那时入口必须额外仲裁/保存metadata。

## 2. 用原 parent/停止游标重建 held AR 的充分条件

[source:123](shared_pretl_read.sv:123) 在 `independent_v_offer` 时输出原 V parent，只改 `addr=V_READ_PA+rV*8`、`len=255-rV`。它没有新header寄存器，安全需要下列实际不变量：

1. 创建前 `old_v_busy==0` 已表示旧 V physical RLAST 真正握手；但这还不够。额外 `rV==E` 保证旧最后skid已交付到 V，所有旧 V logical data已到达。旧 LAST接受后 rV仍E−1、skid仍持有时不能创建。
2. 未接受 continuation 时没有新的V physical R；正常target不能给尚未接受AR产生响应。A response 只推进 A cursor。因此 V offer held期间rV保持E。
3. 原 V parent在本轮只capture一次：[source:222](shared_pretl_read.sv:222)，其 ID0/attributes保持；READ_PA来自 job-life成功recipe，当前 static无invalidate。V尚未logical256，不可能先正常jobdone清该recipe。因此 held header/PA不漂移。[source:196](shared_pretl_read.sv:196) 有实际 held-AR稳定assert，仍需运行触发它，本文不是assert执行证据。
4. V AR真正握手后仅清独立offer、置 `old_v_busy`：[source:374](shared_pretl_read.sv:374)。这时 rV可以随响应推进，组合重建header已不再有效，故不会违反held VALID。

最小会破坏本证明的扩展：在Voffer held时允许invalidate/regrant改PA、再次captureVparent，或在接受continuation前让旧Vdata继续推进rV。那时单offerbit不够，必须固定其header/cursor/recipe generation；当前一次正常静态合同排除了这些事件。

## 3. 两路响应和真实 RLAST，不借 main A metadata 表示 V

top [595](multi_actual_dma_top.sv:595) 给direct physicalID附加bit0；[984](multi_actual_dma_top.sv:984) 返回时清bit0，所以独立逻辑 route为 V0/A4，completion physical port分别1/5。后续mux还会扩ID，这里不是DDR最终ID编号。原ID不同是必要条件，不能把这里的两DMA原rawID0误说成同一个muxID。

[source:133](shared_pretl_read.sv:133) 按 `old_v_busy && response.id==Vparent.id` 选择 V；A仍使用 `child_q/offset/beats/got`。V handshake没有修改这些 A字段；V响应也只推进 old-V route。A physical LAST只清 busy，V LAST只清 old_v_busy：[source:384](shared_pretl_read.sv:384)。global skid接受一拍后须先交完其member，再接受下一 physical beat，故V route的下一index可以用rV，而不另加got寄存器。

V LAST判断 [source:386](shared_pretl_read.sv:386)：

```systemverilog
returnedV+1 == ((!ShareRepairSuffix && returnedV >= E) ? 256 : E)
```

旧V child每个接受的index都< E，旧LAST在E−1；continuation从E开始，LAST只在255。因此无需新segment tag。旧LAST接收后rV尚E−1、old_v_busy已0且skid占用，不能把它当作续段重复接受。E=256时不创建continuation，旧LAST按E256检查，合法。逻辑LAST仍以每flow原parent len255判定 [source:170](shared_pretl_read.sv:170)，不是以物理E判定。

没有把R隧道完成冒充job完成：raw R分别交到原iDMA元数据，target WRITE仍真实TC，qwriter原B要等每个真实childB后才给原parent；当前 `b_valid/parent_live` 门 [q:125](q_write_splitter.sv:125)、[q:141](q_write_splitter.sv:141) 未改变。这里的审查没有新增cancel/伪造physicalRLAST。

## 4. 新 W admission 和 E 边界

Share0保持phase2直到结束；[top:742](multi_actual_dma_top.sv:742) 始终要求新child end≤该flow实际returned，再first-VALID。只有一未耗尽payload child [q:96](q_write_splitter.sv:96)，payload cursor来自实际rawW [q:114](q_write_splitter.sv:114)。所以普通独立A长R被FIFO背压时，全局W头不会是新建且尚缺R的child；其数据已经到实际私有FIFO，正常W/TC/memory最终服务可以释放FIFO。Max2只是已耗尽payload后待B的旧义务。

V在rV<E时的新W child裁到E：[top:713](multi_actual_dma_top.sv:713)。**这不强制每次实际W都在E结束**：一旦rV==E，clip即解除，即使rawW仍<E；例如E105、rawW104，可等continuation交付index105后创建全resident [104,106)。这没有数据环，但不能按“在E必有q1”制定固定AW数量验收。仍须严格W payload256/256、真实每AW/WLAST配对和原parent/B完成；qW count从实际trace推导。

这也是Share0强普通控制的关键能力：不再要求V/A suffix共同游标或共享R；不同native A页界F/oddE不构成缺R的已承诺W头。F255的原B→AW2bubble仍存在，但最后R可进私有FIFO；phase2 parallel路径不恢复旧terminal AWcoverage环 [source:154](shared_pretl_read.sv:154)。本文需要正常AW/W/B最终服务，无任意cycle bound。

## 5. 严格计数与 Share1 等价范围

mode3强制启动cut时A cursor0 [source:264](shared_pretl_read.sv:264)。Share0下，V已发physical前缀累计E、continuation N−E；A main读全N。因此总R=`E+(N−E)+N=512`，无需discard任何Vdata。若E256则没有continuation，但仍是512。AR条数则是已发V前缀child数加A1加( E<256 ? Vcontinuation1 : 0 )；不能将512解释为固定两笔AR。

若扩到mode1先共同返回A前缀FA>0，实际A builder从FA开始，不是0；总R为 `2N−FA`。当前macro严格512只适用于mode3/FA0；它会拒绝这种范围外轨迹，并非任意模式的正确公式。应继续固定fixture，不能拿当前guard支持其他模式。

[multi_test.cpp:1547](multi_test.cpp:1547) 的 `REPAIR_INDEPENDENT_CONTROL` 仅将真实cutover的source expected设为2N；默认仍E+(N−FA)。J/FA/E范围检查、logical每flow全2048B [1565](multi_test.cpp:1565)、目的physical256/256 [1578](multi_test.cpp:1578)、错误区域禁止、后续bytechecks均未在diff中放松。错误message仍写旧公式不影响实际comparison，但不能据message误报ordinary期望。编译必须配对Share0+macro、Share1无macro；E256两公式重合时仅R512无法识别selector配置，要保存实际compile selector/merged观测。

Share1下独立offer入口恒false，输出mux及ARhandshake新增分支从不选择V独立offer；repair_end=E，Wclip恒保持原条件，LAST条件仍用E。与§68 diff显示source逻辑行为保持，top只透传参数/修observer owner，C++默认guard保持。因此有源码等价依据，**没有 CSV exact 实测依据**；CSVexact/root运行仍是最终验收。

## 6. 新成本与停止判定

相对§68只新增1functional `independent_v_offer` bit；repair_end是组合线，不是9bit寄存器。它复用原Vparent、PA、rV/E和既有old_v_busy路由，未加payload。源中“Only new functional state”旧注释不能忽略此新增bit。完整综合/时序面积未知；Share0实际会有更长R工作，与Share1并非同work对照。

独立长读、resident-before-first-VALID W与Max2流水线均是普通强对照，不构成机制创新。若该对照在相同真实权限、private容量和服务下缩小candidate优势，应以实际V/A完成/源R工作评判。本稿接受的是新增协议接口在窄合同下的可行性，没有预测PASS、因构建完成声称验证、或把一般memory公平性证明成固定cycle界。

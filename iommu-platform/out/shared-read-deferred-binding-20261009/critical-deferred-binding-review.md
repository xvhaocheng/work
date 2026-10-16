# §72 common prefix / deferred binding：关键协议审查

Codex 亲自读本目录实际 source/top/system/C++ 新 diff 与必要接口。没有运行、构建、改 RTL 或验收 CSV。下文旧版失败数量/状态由 root 实际执行后告知；本文直接复核源码因果，不声称本席执行过 DUT。行号是当前 A-only 修复后的源码。

**结论：root 报告的 build01 八个 watchdog 是真实的独立 A suffix 授权寿命错误。我的首条“暂未见新阻断”漏掉了它，应撤回作为寿命验收。当前 source:342–343 已把独立 A builder 改为仅需 A own READ/WRITE 证书，符合该分支；共享仍检查双证书。修后窄合同下未发现新的 late-binding/held-AR/RLAST/双offer阻断。rawV/rawA=E 的 shared门闭合数据债，不要求旧B quiet。timer只界定等待绑定：预算B在第B+1个计数条件成立的沿作决定，不是 V/A 或新AR完成时限。build02仍须 root 真实执行验收。**

## 1. 必须保留的真实失败和修复

root告知：build01 24次中01/03/04/05/07/09/10/15八次真实watchdog，终点 `V returned256/done1; A returnedE; main offered/busy/skid均0`。四份build01 source/exe已由root封存；本文没有改日志或把反例称为纸面替代实测。

旧A phase2 builder要求两方 `cache_valid[0][0] && cache_valid[1][0]`。V完成独立continuation后，READ parent有效条件正常退出：[top:1093](multi_actual_dma_top.sv:1093) 需要 `paV==1 && doneV==0`，所以即使V job的WRITE/B尚未完全退休，logical RLAST就足以使其READ准备项失效。A只读完公共prefixE，不需要再借V payload，却错误依赖V证书，因而未来suffix永远不能创建。旧§69一次A full-N AR早已offer，不触发这个新增prefix→suffix依赖；我的前述消息只核header/路由，漏掉了跨新AR builder的授权寿命。

当前实际 [source:342](shared_pretl_read.sv:342)：

```systemverilog
if (returnedA < repair_end && cache_valid_A_READ && certified_end_A == 256 &&
    (!repair_shared_active || (cache_valid_V_READ && certified_end_V == 256))) ...
```

独立模式只从原A READ PA/header发A未来AR，A logical还<E/N、自己的READ parent仍有效，目的仍由自己的真实WRITE recipe/TC授权，故无需V成功证书继续驻留。共享suffix [352](shared_pretl_read.sv:352) 仍检查双READ/WR和完整prefix；V尚停在E<N，不会提前logical256使其证书失效。这个修正没有伪造V/A权限，也没有把V缓存项人为延寿。当前源审支持修正；不等于build02已PASS。

## 2. late binding 不修改旧 prefix 的 header / expected LAST

Deferred reset先设 `repair_shared_q=1`，所以A firstbuilder保存原 `[FA,E)` 到 `child_q.addr/len/offset/beats`：[source:342](shared_pretl_read.sv:342)。这些是寄存器；deadline后 `repair_end` 组合变为256，不会改已offer的child_q或已接受的got/beatcount。prefix held AR继续输出同一header，prefixphysical LAST继续按保存beats检查 [440](shared_pretl_read.sv:440)。只有busy/offered/skid全空时才重用mainmetadata发A `[E,N)`；最后prefix skidded beat未交给A时无法提前创建suffix。

deadline允许在prefix AR held、prefix R active或prefix skid held时绑定独立；它改变的是**未来**builder/continuation门，既不withdraw VALID，也不改变现有R的logical owner/ID/last。旧A prefix全部完成后cursor才到E；新A suffix第一次handshake才把maingot清零。

绑定条件 [source:320](shared_pretl_read.sv:320) 还要求 `old_v_busy==0 && returnedV==E`：旧Vphysical LAST已真实握手，且其最后skid已logical交付。因此 `repair_shared_active` 翻0时，不可能还接受旧V index<E的response并错误将其LAST endpoint改成256。独立V continuation从E重新AR handshake后才置old_v_busy，其未来LAST正确用N。[source:435](shared_pretl_read.sv:435) A route仍使用保存main metadata，V route仍用自己的原parent/游标。

共用原parent READ PA作为重建源仍要求单job/static无invalidate；本轮模式不会在heldV AR期间logical完成V，因此V READ parent/PA稳定。此审查不支持撤销/remapping中复用证书。

## 3. 新的双 AR offer 情形和同沿事件

V创建门 [source:331](shared_pretl_read.sv:331) 要求无main offered、无oldV busy、rV=E，以及 mainA busy 或rA=N。A创建门 [338](shared_pretl_read.sv:338) 要求main无offered/busy/skid、rA<repair_end。因此同沿不能新建V与A两个offer：V的busy分支排除A builder，V的rA=N分支排除A未完成条件。

**新路径可出现两个内部offerflag同时为1，仍不构成两个输出AR：** V continuation offer已held时，A prefix可以physical/logical完成；主builder可保存新的A suffix并置main offered，而V独立offer尚未接受。source组合输出优先V；V header由停止在E的cursor重建。V handshake只清Voffer/置oldVbusy [425](shared_pretl_read.sv:425)，不会清main A offered或覆写A header。下拍A suffix成为输出，直到自己的握手保持稳定。原A prefix此时已真实RLAST、skid空，metadata重用安全。不能为图省事assert两个内部offerflag永不共存。

同沿A prefix LAST、V AR接受也安全：V握手置oldVbusy；A响应按pre-edge旧route只清mainbusy/生成A skid，二者不是同一busy字段。A prefix LAST/skid交付同沿不能再接受下一physicalR，因为global skid背压，故未交完的prefix末beat不会被新suffix覆盖。

当前行为DDR model至少延迟到已接受AR后的时刻：[CPP:1104](multi_test.cpp:1104)、[3020](multi_test.cpp:3020)。本证明按这条真实模型接口，不推广到AR/R同沿零延迟slave：若新V AR与第一V R同沿、A仍busy，旧oldVbusy0时incoming-V尚未成立，现有ready可能按A route接受V ID并fatal。该边界继承独立route实现，本次DUT模型排除了它；若将来宣称任意AXI slave协议完整性，需对未捕获ID的RREADY作门或同沿路由处理。

## 4. timer的确切界限和优先级

[source:320](shared_pretl_read.sv:320) 的计数条件只看phase2、未decided、oldV物理busy0、rV=E，不要求A ready/AR/R/skid或W/B已经完成。非等待ready的counter因此可在heldA prefix期间到期。

对于B>0，wait初值0。前B个条件成立而未join的沿依次加到B；第B+1个沿看到旧wait≥B并绑定。若join早ready，该沿立即绑定且不再增wait。B0在第一个符合条件沿绑定独立。精确可声称 `waited<=B`、在至多B+1个符合条件沿决定；从phase1、cutover请求或第一个AR发出开始没有这个界限。

如果到期和join_ready同沿，正预算选择share，因为赋值实际是 `Budget!=0 && join_ready` [326](shared_pretl_read.sv:326)。B0即使ready也故意独立。不能称“只要到期就绝对独立”或把这个沿外推为严格B个cycle完成。预算≤65534由 [467](shared_pretl_read.sv:467) 拒绝越界；16bit wait到B后不继续加，无wrap。

V prefix尚未physical/logical到E的时间不计；决定后heldA AR/R、W/B和未来互连服务仍可无限长（AXI握手本身不承诺bounded service）。若prefix已结束而A suffix尚未接受，现有V创建门也可能等A suffixAR接受使mainbusy1；预算不保证V下一AR立即offer。root文档“policy wait bounded, not end-to-end”正确，必须保留。

## 5. full rawE门的数据闭包、odd E 和 pending B

`repair_join_ready` [source:87](shared_pretl_read.sv:87) 要求rV=rA=wV=wA=E且source old/main route、offered、skid全空；sharedsuffix builder [352](shared_pretl_read.sv:352) 再硬核双rawW=E。未绑定时repair_shared默认1，phase2两个新W都裁E [top:719](multi_actual_dma_top.sv:719)，并要求新child全部payload真实returned [749](multi_actual_dma_top.sv:749)。所以决定share以前不存在end>E的registered W。

rawW=E意味着前缀E个payload已由实际raw W握手消费；当前onepayload-child在实际rawWLAST释放，promise不会仍欠E之后的数据。旧pendingB只保存已耗尽payload的退休义务，TC/物理W may lag但不需要新的logicalR；正常memory最终服务即可排空。join不必等所有childB或originalparentB，这也允许A原WRITE parent继续活着跨E。[top:735](multi_actual_dma_top.sv:735) phase3裁两方prefix是额外防线。它关闭了§71手推候选的前提，不把该候选升级为DUT复现；root已告知其8固定share+8adaptive延迟B定向实跑均PASS、未复现候选环。

odd E下，share决策的rawE门**实际强制**V先发prefix末q1并消费它；之后从E重新q2，末端N再q1。V children为 `ceil(E/2)+ceil((256−E)/2)=129`；E even为128。A强制cut集合为实际native页界F与E：按排序后的{0,F,E,256}去重、分段长度的ceil/2求和。F=1/E105的share可以达到A129/V129，而不是笼统固定128。J的原closure若为偶数不额外增加q1；ClosureAtBurstEnd时J=E。

**独立timeout不保证在E发生W q1。** 翻独立后，clip只保留到各flow logicalreturned<E；游标到E而rawW仍<E时，未来continuation返回的payload可能允许resident q2跨E，合法。尤其ClosureAtBurstEnd0/minimal-even-J时，不能只据公共physicalR的prefixE就固定V q1/AW数。ClosureAtBurstEnd1已在phase1实际wV=E则V确有该边界。实际AW/B token/每flow W256必须严格统计；不能用所有R prefix一律强制Wcut的ceil公式误判独立控制。

## 6. R公式、配置与未验证项

mode3/FA0强制 [source:301](shared_pretl_read.sv:301)；Adaptive/Deferred也现明确拒绝Parallel0和mode非3 [465](shared_pretl_read.sv:465)。system/top传同Deferred/Budget值 [system:1168](multi_system_top.sv:1168)、[top:1007](multi_actual_dma_top.sv:1007)，没有testbench future服务输入参与选择。

shared physicalwork为V前缀E+A前缀E+共同suffix(N−E)=E+N；独立为V前缀E+A前缀E+V独立suffix(N−E)+A独立suffix(N−E)=2N512。公共prefix独立比§70 whole-A控制多一笔AR，不能藏起来。E=N则没有两个suffix，512两公式相同，sharebit1不代表真实merge。

C++ [1568](multi_test.cpp:1568) 沿用bound-route R期望与decided门，未根据测得R选择公式；[1810](multi_test.cpp:1810) 只新增wait/budget observer。它不会独立重演timer transition或证明B+1沿，当前精确counter规则来自本次源码手推，尚需root实际observer验证。完整bytes、W256/256、真实AW/WLAST/B以及watchdog必须保持。mode3不cutover时保留512旧fallback；未decided的reset share值不能报告为一次deferred选择。

源码额外声明16bit wait，无新payload；预算常参/unused Adaptive采样寄存器的实际优化面积需综合，不能仅据声明给等面积结论。按root当前A-only修复，源审支持继续build02实际验证。没有把本稿的手工证明、root旧阶段PASS或正在构建当作新copy PASS。

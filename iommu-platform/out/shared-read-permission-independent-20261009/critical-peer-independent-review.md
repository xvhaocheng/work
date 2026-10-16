# §73：独立权限、双 offer 与计数边界关键审核

Codex 直接读当前 `codex-design-before-run.md`、`codex-source.patch`、实际 source/top/system 和 helper 的必要权限链。只作源码及并发推演；未构建、运行或修改功能 RTL。以下行号对应本稿保存时的当前源码，包含 root 新改的 AR 握手计数；`codex-source.patch` 是此前功能变更记录，仍显示旧 create-time 计数，不能用它代表最新 observer。

## 决定

在当前两个有限、静态正常 job、mode3/FA0、DeferredRepair+ParallelPrefixRepair、完整真实 READ/WRITE 授权、Q2/private B3 和非零 AR→R 延迟的合同内，未发现 §73 去掉 A 授权启动栏杆所新增的权限、held-header 或双路元数据阻断。允许按此范围继续 root 的实际验收。

本次确实发现一个新并发 observer 错误：V continuation 与 A builder 同沿创建，各作一次 `child_count <= old+1`，会丢一个创建事件。root 当前已改为只在单个物理 AR 输出实际握手时计数；我直接确认所有 create-time 写入均删除。这个修复没有改变数据或授权控制。

这不证明任意 AXI slave、错误/撤销/重映射或持续多 job 的普遍安全。root 已报告首批 24 配置完整 copy PASS；这是 root 执行的结果，本审核没有独立跑 DUT，也没有以它认定 build02 新 observer 已完成严格计数验收。

## 1. 去掉的仅是 peer-grant 启动栏杆

当前 `shared_pretl_read.sv:134–139`：

```systemverilog
... busy_q && !skid_q && pa_q[0] == 1 && pa_q[1] == 1 &&
done_q[0] == 0 && done_q[1] == 0 &&
cache_valid_i[0][0] && certified_end_q[0] == 256 &&
(PeerIndependentCutover ||
 (cache_valid_i[1][0] && certified_end_q[1] == 256)) &&
closure_target < 256 && closure_target <= child_offset_q+child_beats_q &&
closure_target >= promised_write_end_i[0];
```

因此 V 可以在 A READ 或第二页 WRITE 还未成功时关闭自己的 W 数据债，但仍必须已捕获 **双方**原始 raw AR。它解除翻译授权的交叉等待，不解除 A 软件供给/原始 parent 到达的等待，也没有给 A 数据授权。V 的全 WRITE 前沿仍要求 256，自己的 READ 必须实时有效；已经 firstVALID 的 V W 仍必须被 J 覆盖。

源 `:291–320` 只锁定 cutover 的 J/E 与阶段；mode3 要求 A cursor 仍为 0。phase1 只有实际 `returnedV==rawWrittenV==J`、无 skid、已承诺 W 不超 J 才进入 phase2（`:321–327`）。top `:728–733` 剪裁 **新** V W 到 J；既有 child 的 header 没有被这个组合量重写。原始 parent 留在 `parent_q[f][0]`，不因 cutover 清除。

V continuation 创建改为 `PeerIndependentCutover || busyA || returnedA==256`（`:342–346`），自己的 READ 有效即可。V 自己尚未 logical RLAST 时，helper READ parent 仍 live；top `:1093–1098` 以 `active && pa==1 && done==0` 保持 slot0。A 后续独立 builder 只依赖 A READ+WRITE256（源 `:352–353`），不依赖已经退休、正常失效的 V READ。这也保留 §72 实测寿命错误的修复。

## 2. A 未授权时没有隐式数据接收；共享需要重新通过双门

源 `:352–359` 从实际 A parent、A PA 和 A cursor 建造独立 A AR；没有 A READ 或 WRITE256 就不能建立该 AR。phase2 route 对旧 V 响应只要求 `01`，对 main A 响应要求 `10`（`:155–165`）；每个实际 recipient 又要求该 beat 在自己的 certified WRITE end 内（`:167`）。V 自己推进不会增加 A `returned`，真实 logical R 握手才增加对应 cursor（`:466–470`）。

新的 `repair_pair_authorized`（`:87–94`）同时检查：双方实时成功 READ、两方 WRITE256、源 PA 完全相等、原始 cache/prot/qos/region/user 相等。这个量同时参与 deferred share 选择（`:95–97`）和实际共享 suffix builder（`:362–366`）。后者另要求两方 R/W cursor 都精确等于 E、旧 V route 退休、E<256。故最早在 **双方自己的 prefix 都完成且授权相容** 后才让一个 physical R 扇出给两方。

PeerIndependentCutover 跳过启动时的相等 PA/属性 fatal（`:305–311`），没有跳过最终共享门。不相等时 timeout 选择独立读取，按自己的 PA/属性取数；不会把 V payload 当 A payload。要求属性相等是当前保守合同，不表示任意属性差异都架构上禁止合并。

实际授权不是一个人工置位的 source `w`：top `:1052` 使用 helper 的 `reuse_cache_valid`；helper `:71–78` 的 key 包括 DID/IOVA/PID/PID-valid/priv/bank 与独立 READ/WRITE 类型。helper `:103–106` 要求当前 parent live、generation 相同、normal 无 error/ignore/MRIF，并对 WRITE 看 `w`、对 READ 看 `r`（或真实 TL bypass）。`:218–230` 只有真实 TL 返回、当前完整 key/seq 相符才能 fill。top `:1083–1085` 的 A 全 WRITE 前沿要求第一页、第二页均成功；`:1111–1114` 的 WRITE context 来源仍是实际 raw AW。

`certified_end` 是单 job 的单调前沿（源 `:243–244`）；它本身不是可跨 flush/remap 使用的永久证书。当前静态 job 无这些事件。实际 qW firstVALID 仍受自己当前页 WR cacheValid 门（top `:1053–1056,749`），没有仅凭旧前沿绕过目的 WRITE 权限。

## 3. 两个 offer 可同沿创建，头和退休记录没有重叠

可达新序列：phase2 已选择 independent，旧 V 真 RLAST 退休，`rV==E<256`，main 空；A 此时刚满足自己的授权。

1. 同一沿前 `independent_v_offer=offered=busy=old_v_busy=0`。V 创建块 `:342–346` 置 `independent_v_offer=1`；A 创建块 `:348–360` 写 main `child_q/offset/beats/members` 并置 `offered=1`。
2. 下一周期输出 V，因为 `:148–152` 对 independent V offer 给定优先级。main A 的完整 header 保持在 `child_q`，未被 V 创建修改。V 的 header 从原 V parent、稳定 rV=E 和 PA 推导。
3. V VALID 被 hold 时，旧 V route 未建立、没有合法新 V R，rV 不会变化；source parent、实时 READ PA 在当前静态映射与尚未 logical LAST 的寿命内保持。因此 header 保持。已有 main A firstVALID 时，`!offered` 又会阻止新 V offer 夺取输出；不能越过一个已 hold 的 A header。
4. 实际 V AR 握手 `:434–436` 只清 V offer 并置 oldVbusy；不清 `offered`、不改 main A 元数据。之后输出并接受 A（`:437–438`）。A ID 与 V ID 分别保留各自原 parent 0/4，完成侧 top `:602` 加 direct bit 得 1/5，返回 `:995–997` 清 direct bit。
5. 返回按 captured oldVbusy 与 ID 区分，V 旧段 LAST 端点是 E，独立 continuation 从 rV≥E 起端点是 256（`:445–451`）；A 按自己存下的 offset/beats/LAST 判定。各 physical LAST 只清自己的 busy（`:459–463`），logical LAST 仍从原始 len255 与该 flow cursor 生成（`:195–196`）。

timeout 与 A prefix 创建同沿也合法：该沿的 builder 读旧 `repair_shared_active=1`，固定生成 `[rA,E)`；随后 choice 变 independent 仅决定新的 suffix。既有 child 的 len/beat 数没有被组合 `repair_end` 改写。若 A 授权一直未到，timeout 后首次 A builder 可直接生成 `[0,256)`，并不必经过一个尚未 offer 的 `[0,E)`。因此不能固定宣称每条 independent 路径都有同样的 prefix AR 数。

以上不支持零周期 AR 与首 R 同沿。新 route 在 AR 握手沿以后才有效，top `:620–621` 的无 route 检查以及源 `:155–165` 使用旧 busy 会使这类 slave 落在已有不支持边界；这是设计稿明确排除的条件，不作为本次已修复或普遍 AXI 合规结论。

## 4. W 进度与等待的实际界限

phase2 **任何新 W** 都必须数据已到真实 DMA：top `:749–753` 对 `next_end*8>source_returned` hold。选择 shared 前双方新 W 均剪裁 E（`:719–725`）；绑定 shared 又等待真实 `rawWrittenV==rawWrittenA==E`，所以不会带着一个缺 payload 的跨 E 旧 W 转进联合 q2 网格。已经 firstVALID 的旧 W/B 保持由 qwriter 完成，不要求所有旧 B 先 quiet。

独立选择后每方仍有上述数据已到门，不会为了 A 未授权的 suffix 先建立一个缺 payload 的 A W 头，再反过来卡 V R。mode3 的 A 原 cursor0、前期 hold 同样阻止无 payload 的 A W firstVALID。§73 没有新增该内部环。

仍不能声称 V 必定在 A PTW 完成之前结束：共享 DDR 若先有一个 held 的 A PTE 或其他不可撤响应，V continuation 必须等它获服务。AXI 本身也不给 R/W/B 的 cycle 上界。policy timer `:331–339` 从旧 V 真物理退休且 rV==E 后才启动；预算只限制 **该状态下的绑定等待**，不是从 A miss 或 job 开始计算的 completion deadline。正 budget 等 B 次计数后在下一合格沿决策，零 budget 当场选 independent。没有 A 授权时可以选择 independent，不会因 A grant 缺失停止该 timer。

terminal E=256 没有可共享 suffix，也没有 V continuation；A 自己的独立授权 builder 在 timeout 后仍可工作，V READ slot 已退休不影响它。故“V 结束→V READ 无效→A suffix 永远不发”的 §72 已实测错误不被这次改动重新引入。

## 5. Observer 修复的准确合同

原 functional patch 所示两个创建块可能同沿各写 `old+1`，只保留一次。最小例就是 §3 的双创建；两笔 AR 之后分别握手、copy 可以正确，但原 child_count 少 1。此前 copy PASS 不能自动证明那个累计 count 正确，也不能把纸面可达的双创建说成已观察到某条具体 run。

当前实际源只有 reset `:210` 和：

```systemverilog
// shared_pretl_read.sv:223
if (m_ar_valid_o && m_ar_ready_i)
    child_count_o <= child_count_o+1;
```

我直接 rg 确认 V、A、shared、solo 所有 create-time `child_count_o` 写入均删除。`a_child_count_o` 仍是 A builder 的创建计数，语义另行保留；不能在途中把两者当同类计数比较。

单物理 AR 输出使一拍最多一个被接受的 child。该 counter 精确数 **controller completion-side AR 接受事件**，不是创建事件，也不包含纯 TL/PTW 请求。完整 copy 的所有原 logical LAST 已退休时，相关 source AR 必然已被接受；在当前不取消、不另行分割该源 child 的传输合同中，与最终实际 source AR 基数一致。它不能保证中间采样时 created==accepted，也不能替代 root 的 DDR 侧独立严格计数。

只读消费者链：actual top `:1035`→system `:1349` `fragment_children`→`:1476` `dbg_frontier_wait`；CPP `:1803` 打印、`:3011/3015` 窗口快照、`:4353–4357,4373–4381` terminal/delta/JSON。未发现功能控制读这个值。功能输入不消费它，故改 observer 事件不改变 AR 路由或 W 进度。窗口结果现在数区间中的 accepted AR，后续报告必须明确这个含义；未运行本稿的 build02。

## 6. 接受范围与机制判断

本稿接受的是“V 自己的有限授权与数据债闭合不必等 A 完整授权”的窄实现。独立 return 使用 own grant，共享 return 使用双方 grant；这消除了一个实现额外施加的跨成员栏杆。因为常规 shared-fetch+private request info 的独立授权、独立 route 与延后 cohort admission 就能具体达到这个行为，**去掉此栏杆与 timeout 选择本身不能作为独立创新贡献**。真实异步授权下是否有新增复用收益与不可撤义务的成本差异，仍须由 root 的强普通对照及已执行证据判断。

未验证：错误 grant 运行时恢复、flush/remap、multiple jobs/epoch reuse、真正零延迟 R、未约束持续到达、综合面积/板上行为。当前 denied/fault fatal 与静态权限寿命限制保留，不从本次首批 copy PASS 外推这些性质。

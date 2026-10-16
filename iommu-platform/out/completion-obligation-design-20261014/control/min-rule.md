# OC1-min：固定的最小阶段引用计数 pin 对照

2026-10-14，版本 `OC1-min-frozen-v1`。这是 root 点名“TTL/rank 是否必要”的有界后续任务：在 OC1-v1 保存冻结并读取 CO1 卡之后，按普通模块构造一份更小固定程序。v1 原件与配对保留，不挑场景赢家，不声称这份完整组合已被发表。

结论：标准阶段引用计数 pin 不需要 per-key TTL 或 structural critical rank。下述程序用同一登记账本、阶段退休、release 布尔、登记年龄、RR 与一个唯一 defer token，实现 CO1 的有限两源合法动作及共同进度合同。两源限定确保“被越过者下次服务”指唯一另一个 head。

## 固定范围和共同 paid 接口

`N<=4` 登记 task，`K<=16` 正式 page key，`B<=2` 真实有 owner 的数据 buffer，原 cache16，单正常 TL/PTW；仅 native source0/1，每源至多一个实际 eligible preVALID head。task 槽可顺序复用 source0/1；不增四源并发或改变 native buffer512/3、in-flight 容量。

实际 HOLD 仅严格 aligned 64B DEFAULT；任意范围的两源合法软件 pre-kick/generic gate 属共同新增付费接口。未握手 VALID 也不能被登记当许可。形式 key/context/epoch、actual resident/invalid/fill、最后翻译用途 handoff、完整 R/W/B、CPU consumer/cache ownership、buffer owner/release、source/functional-job/task-slot 关联均按 `rule.md` 的相同严格含义计费。

同一固定逻辑实现可容纳每 task 多页和重复使用，但必须有完整登记及精确最后用途证明；没有该证明则双方禁用提前解除。这里不是未来 PTE/DDR 查询、不是新 prepared translation pool。

## 完整状态

1. 共同 task 表：src/dst/len、formal context/epoch、前驱依赖、真实 ready、输入/输出 buffer 权利、实际退休条件、source/functional-job 关联、完整任务生命周期状态。
2. 共同 buffer 表：真实 owner、ownership generation、handoff 状态及唯一最终 release task。translation bit 不能清 buffer。
3. `read_users[k]`、`write_users[k]`：各 N 位；每 task/key/side 一个尚需翻译的阶段引用。`refs=read|write` 与计数均可组合派生，不强制另造 refcount/duplicate range 表。
4. `reg_age[task]`：本有限四任务登记 episode 的序号 0–3，各 2 bit；由可信登记给出。episode 槽不在有旧回调时复用，episode/global generation 计入共同源关联合同。
5. 两源 `rr` 指针 1 bit。
6. 唯一 `defer=(valid, task_slot)`：1+2 bit。关联的是某个真实任务槽，不是单独 source 数字；如果取消、epoch/stale、已发出或失去 eligible，绝不凭 token 虚发。

无 TTL、pin_enable、预测 latency、structural depth、动态 deadline、第二份结果、逃生 cache/slot/walker。cache 保护是当前资料的函数：

```text
protected_way[w] := valid-current-native-way(w)
                    AND exists k: actual_native_match(k,w)
                                  AND (read_users[k] OR write_users[k]) != 0
```

对 CO1 首卡仅正式 S1/4KiB/non-global keys 使用同样保守 key 匹配，其余走普通路径。superpage/global 不被错误拆成免费权限；一般扩展是否成立另付正式查找/生命周期费用。被 evict 的 key 仍有 refs，也只表示未来用途仍活；不表示它继续 resident。原始 native tag/PLRU 已在 cache 存储，不强制普通再复制；获得当前 tag/epoch 资料的 read/fill/inval 路由与同步费用全计。

## 完整事件程序

同事件顺序固定为 invalid/stale 优先，其后 native drain 与 phase handoff、真实 task/data 退休、登记/ready，再选择。没有后台 tick。

```text
REGISTER(t):
    校验有限 DAG、全部页/range、key/context/epoch、buffer owner/release 和界限
    若不合格: 不部分宣布完整保护；进入对应普通 fallback
    合格: 保存 task/age/关联；为每 key 的尚需 R/W 用途置 users bit

TRANSLATION_END(t, side, k, proven-last-handoff):
    若 task/slot/source/epoch 对不上或证明不够: 不清 bit，停止该提前解除 hint
    否则 clear read_users[k][t] 或 write_users[k][t]
    不修改 task_done、buffer owner、CPU/cache 交接或其他侧的 bit

NATIVE_FILL / EVICT / INVALIDATE:
    使用原生真实、当前正式状态重新生成 protected mask
    invalidation 永远不等待 pin；旧 epoch 的 hint 停止
    refs 不因 eviction 清零；不凭 refs 复用一个已驱逐 PA

TASK_AND_OWNER_RETIRED(t):
    只有完整 R/W/B、必要 CPU/cache 交接和 data owner 退休都满足，才满足图边并释放 buffer
    t 残留的 translation bit 只能在可信 no-further-lookup/drain 证明后清
    如果 defer 指向 t，清 token

CANCEL/ERROR(t):
    首 VALID 之前确认未发: 清该 task 用途，后继不当成功；合法数据 owner 单独处理
    COUNT/held VALID/issued 后: 保留 task/data 义务，原生 drain/error 终结；不撤回请求
    只有精确 no-further-translation 证明可解对应 phase 引用
    defer 不作额外许可；stale callback 不解 buffer、不发布成功依赖
```

当前生产 `dma_core_wrap` 配的是 NO_ERROR_HANDLING，native frontend done 不携完整错误反馈。因此 normal-success 完成链可从源码追踪，但 cancellation/error 的完整生产接入与成功证明仍是共同新增/未实现合同。无法认证错误时冻结成功依赖，走 native drain 与软件确认；不因 pin/unpin 伪造成功。

## 固定资格、release_path 与选择程序

`eligible(t)` 与 v1 相同：真实 ready、前驱指定退休、正确 buffer/owner acquire、实际 preVALID head、native gate 前置条件。`release(t)` 是布尔：t 自身或已登记 DAG 后继为某个目前 occupied buffer 的真实最终 release task，且该 buffer owner/generation 对得上。DAG 可达关系只来自登记，未 ready 任务不能仅因这个布尔发出。

`order(E)` 固定按 `release=true` 优先、登记年龄较小优先、两源 RR 打破 tie 排序。RR 只在实际 grant 后转到另一源。当前观察到全部用途 resident 时无需新 victim；否则保守按 cold 处理，不从 tag-only hit 推导权限，也不预测未产生的 PTE。

```text
E := 当前 eligible heads （size<=2）
if E 为空: 不发新供给；已接受工作与必要反馈持续排空

if defer.valid:
    if defer.task in E:
        i := defer.task
        clear defer
        grant(i)                      # 必要时用原16项原生PLRU牺牲保护
        if E 有唯一另一head j:
            defer := j                # 新的一次越过也登记；连续双ready时成为RR
    else:
        clear defer                   # 不虚发失去资格/已取消任务
        执行下面普通选择
else:
    i := order(E) 的第一项
    if NOT release(i) AND i 可能需新fill AND 无 unprotected victim AND E 有另一head:
        j := E 中唯一另一个head
        defer := i
        grant(j)                      # 不绕回无限HOLD；必要时原生PLRU让路
    else:
        grant(i)                      # 无另一eligible时立即让路，绝不等未ready后继
        if E 有唯一另一head j:
            defer := j                # 明确记录被本次选择越过的唯一head
```

每次实际 grant 都给唯一另一 eligible head 记 token，包括偿还旧 token 的轮次；连续双 ready 时因此成为 RR。每个不断 eligible 的两源 head 至多被另一个成功 grant 越过一次，下次有合法选择机会先服务它；没资格时不承诺授予。CPU、native 响应或 buffer 不合法时没有时间/完成 oracle。三个以上 eligible heads 时，单 token 不能保证所有被越过者下一轮都服务；本卡不泛化这种合同，不通过新增活源制造差异。

所有 native fill 统一：若 `~protected_mask` 非零，用该 mask 的原生 PLRU；若全保护、既有承诺已需要实际新 fill、release/fair grant 或无另一 eligible 的普通推进必须进行，则用 `all_ways` 的原生 PLRU 执行这次 fill。只有原 16 项，无新增回退存储。cache update 的同步/稳定要求与 v1 相同；实现不了就禁用动态保护并回 native PLRU。

## 循环等待、容量和不可能完成的边界

已承诺 fill 永远可牺牲 soft pin，ready release 永远可在合法数据/native 边界推进；后台被跳过最多一个选择机会，无其他 eligible 时立即普通让路。因此 phase pin 无须 TTL 也不会永久等待未 ready 的未来 user。

真实 buffer/resource cycle 仍可能存在，DAG 无环不自动证明两个 buffer 可调度；无合法 acquire/release path 就拒绝优化登记并报告，不能覆盖 owner。invalid/stale、超过任务/key/buffer界限、callback/generation不匹配、真实闸门未启用、最后用途证明缺失的 fallback 与 v1 一样保留完整 task/data 义务。若作用域整个信息不可用，清 soft protection，回 native/RR/合法软件供给。

## 与 CO1 的全族状态/事件配对（有限合同）

| CO1 有限状态/事件 | OC1-min 普通状态/事件 | 输出关系 |
| --- | --- | --- |
| 已登记尚需 task/key/RW 用途 | 两个 users 位图；计数派生 | 同一真实最后 handoff 清相同 bit；同一 refs 生相同保护集合。 |
| task/data/buffer 尚活 | 独立共同 task 状态和 buffer owner 表 | cache unpin 不解任何完整完成/ownership义务。 |
| release_path | `release(t)` 布尔可达查询 | 同 inputs/owner generation 下相同；不需 structural depth。 |
| 登记年龄、source RR | reg_age、两源 rr | 相同序号和 tie 时同一普通优先序。 |
| 两源一次越过必服务 | 唯一 defer token、上面的偿还规则 | 在 root 接受同一精确 token 定义后，逐选择映射；原含糊的四源解释不作等价声明。 |
| preVALID允许/HOLD；COUNT不可撤回 | grant 写实际gate/software pre-kick；OFFERED 后不回退 | 同 legal entry 与 snapshot 时同合法供给动作。 |
| 全保护必须推进 | 原16项 all_ways PLRU escape | 不添资源；被换出 future refs 保留，容许后续真实 rewalk。 |
| invalid/stale/cancel/error/capacity | 同共同保守 fallback/drain | 不阻失效，不假退休，不发布错误成功依赖。 |

这是对有限输入/事件族的明确状态和动作构造，不是仅说“普通还能添加同模块”。尚未实现/执行，没有真实 performance trace、收益、Fmax、面积或错误安全证明。

若 root CO1 接受上面的同一 token 及 fill-mask 稳定定义，则它未留下普通无法表达的 allocation/admission/phase-retirement 动作。若 CO1 选择另一合法公平序列，应把该具体序列与本程序区别记录；不同选择不自动证明新能力、低成本或优越完成时间。CO1 的物理费用与净性能仍 null，发表前作覆盖仍独立 unknown。

严格 4×64B aligned 实例每任务 R/W 各最多一页，全部登记 key 实际最多 8，故全 16-way 保护状态不能仅由这四任务产生。全保护 escape 是一般多页/软件 pre-kick合同的符号边界，不能拿64B实例加压力walker制造入口。

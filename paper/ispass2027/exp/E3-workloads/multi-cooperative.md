# Multi专用合作调度小批

Root批准的单个候选，实际Linux提交`65b427946152e3aca27babab0a437821729805d4`，直接parent为46df。
只改`drivers/misc/idma.c`，9行增加/5行删除；[原始diff](source/multi-cooperative-review.patch)、
[commit patch](source/linux-multi-cooperative.patch)、[增量bundle](source/linux-multi-cooperative.bundle)、
[组件身份](source/linux-multi-cooperative.json)齐备。Linux只有上游remote，未创建remote或向上游推送。

## 精确边界

`idma_run_window_many`增加显式bool cooperative。旧pair/P0/pages/workload的wrapper传false；
ABI3 multi warmup直接调用传true，N2/N3/N4同一策略。
`idma_batch_window_many`利用原multi!=NULL，仅multi在每个未完成循环请求cond_resched。
原先先poll V、处理完成并立即return的顺序不变；未先sleep、未等待A后才认V完成。

没有增加sleep，也未改HZ、PREEMPT、watchdog、RTL、UAPI、计数、供给预算、cache、STOP/drain或unknown-quiet的retain规则。
这是合作调度点修复，不声称cond_resched必然切换任务或给出硬实时上界，更不声称已经证明/消除softirq或timer根因。
[旧acece只读核查](diagnostics/e2-softlockup-acece/findings.md)与原失败JSON SHA/精确时间分解另存；
旧五点纯prefix和taint失败尾保持原身份，未写入或重分类Root数据。

## 实际验证

[实际函数回归](test-results/multi-cooperative01-validation.json)对46df gold与候选均运行：
38 pair/P0、57 multi、97 pages、416 ABI5/兼容情境全通过ASan/UBSan与-Werror。
完整旧MMIO顺序摘要仍`9128914394721286305:498206`；
排除multi后的调度请求位置/次数摘要仍`3976756370770541329:357`。
调度stub不推进时间或执行真实scheduler，所以MMIO对照只说明生产函数请求顺序一致，不证明板上时序相同。

新增6个政策情境：N2/N3/N4各让V CPU COPY延迟2个poll、batch延迟3个poll；
候选每种设备数均有warmup767次/batch3次cond_resched请求，旧gold均0/0。
warmup767=256×(2个pending+1个完成)-最终完成1轮；每次最终完成直接返回。
旧pair software/batch各1025个pending poll仍只触发1024门的一次请求，未采用multi新策略。
立即完成的batch无调度请求；warmup的255个非最终完成循环按multi合同请求调度。
所有57个multi数据/partial start/STOP/drain/unknownquiet测试保留，未删错误情境。

[真正RV64 idma.o](test-results/multi-cooperative-obj01-manifest.json)通过，113608B，
SHA256=`8a72987862d0ece054ef1d2e0ea263cb501f60e8e21a263d7598bfa498090071`。
编译与native测试的生产source SHA相同：`38316b66af557509653d11c9035c7c9d0e093aa68b632449710c88f6adab13e5`。
本地对象为`artifacts/multi-cooperative-obj01/kernel/drivers/misc/idma.o`；未编Image/payload。

Root可用冻结最小source/gold复跑，不更改任何组件worktree：

```sh
python3 paper/ispass2027/exp/E3-workloads/verify_cooperative_driver.py \
  --linux paper/ispass2027/exp/E3-workloads/source/multi-cooperative/linux \
  --baseline-driver paper/ispass2027/exp/E3-workloads/source/multi-cooperative/baseline-idma.c \
  --output paper/ispass2027/exp/E3-workloads/artifacts/multi-cooperative-ROOT01
```

output必须新建。Root验收本批后，才从旧4788/46df候选更新独立platform pins和软件builder，
一次构建新共同软件供E2/E4/完整E3。旧未执行软件候选、da6 ready包及P0包保留；
新正式采集必须新source/boot/manifest、taint0，失败即停且不抹旧失败。

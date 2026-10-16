# §78 Codex：在途绑定与强普通反证

§77 已实测三个4周期 resident 窗口。这里的 RTL 已完成三构建/14项完整copy PASS，5项真实加入；强普通起点共享更快，最终接受见 codex-result-acceptance.md。

## 绑定合同

仅 phase2、双方 own READ/WRITE 真成功且源 PA/属性相同、rV=rA=J<E、无 A child/offer 和 frozen skid、旧 V 仍在途、两方旧 W promise<=J、rawW<=J 且各落后<=2 时绑定。zero-budget 独立对照不绑定。

绑定沿组合 required=11，随后 old_v_joined 锁定旧 V 的后续收件人；冻结 skid 的原成员不修改。原物理 AR、ID、E 和 RLAST 都保留。清 short_pending 的释放状态，并在绑定沿及以后禁止 A[J,E) fallback。新 W 先闭合到 J，之后按 peer rawW 门打开共同 q2；旧 E 前的最后 qW 仍在 E 剪裁，待双方 rawW=E 才发新的共享尾段。已 firstVALID 的 W 和未退休 B 不撤销。

局部增加1个 functional FF（joined），9个只观测加入点的 FF，0 payload；组合门与原平台 metadata 尚须计费，未综合。

初始 A 已交付0、作业N256时，正确执行应读取 N+J 拍：A独立前缀J、旧V physical E、共享新尾N-E。必须从实际 joined/加入点端口验证；terminal v_cut 可能已被新尾的 E 覆盖，不能拿它冒充原 J。

## 强普通对照是必需的

出现窗口的 prepare-order2 条件中，A READ/WRITE 在 V 发第一个 source AR 前已全部成功。因此一个普通调度器可以在所有许可已就绪时，从零直接共享，不需要补读后追赶。新增 OrdinarySharedStartup 使用相同独立许可、PA/属性、FIFO、qW/B 资源和读分片，只在双方相同游标且真正 full authorized 时走原有共享读分支。

如果在途加入仅优于故意让 V 单独启动的控制，而普通 startup 已完成相同效果或更好，就不能用这个场景称独立性能创新。下一有意义的边界必须是许可或作业真实迟到、已有物理请求不能撤回，同时普通强对照无法同信息/同成本复现的新行为。

## 文献边界

[Intel pending-read merge/multicast](https://patents.google.com/patent/US20200402198A1/en) 已有 outstanding request 合并、动态等待窗口和 return mask；[DMA-Latte v3](https://arxiv.org/html/2511.06605v3) §IV-B 已有一次 source READ 给两个目的的广播 DMA。仅“读一次/动态合读”不能当新贡献。

原子成员变更与不可撤 W 数据债的具体小缓冲合同可以研究，但因为 broad streaming/pending merge 已有前作，必须给出前作或强普通控制没有的额外能力及实测收益，不能只重新命名。当前保留这个条件判断，尚未形成独立贡献结论。

机制由 Codex 实质设计和关键源核；C++ 验收由 DeepSeek 提案、Codex 实际审查/应用/运行。此文不声称模型共识等于验证。

# §50：Codex 根级设计与实际验收

2026-10-09。这是新 out-only 真实 iDMA / IOMMU RTL 仿真，不是 FPGA 板上实验，也不是 §49 事件模型。生产 RTL、Linux、BIT、SD 没有改动。本阶段创新设计、两次跨模块死锁诊断、RTL 和最后验收由 Codex 亲自完成；DeepSeek 作者普通页表/驱动/观察器提案，Codex 审读、应用及执行。

## 接受什么

最终 RTL（build06）有 12 个纳入验收的完整 copy：run14–21、run24–27。每次两真实 DMA 各 issue/done 一次、各返回 2048B、两个目标各写 256 拍，源和目标未拷贝区域逐字节检查；真实 virtual AW/W 长度与队列关闭、physical R/W 和暂停检查保留。run20/21 的周期背压、B 延迟 64 cycles、10-cycle 恢复请求均 PASS；pin 在 3181 恢复、实际 A 退出在 3254，A 没有提前抢占 V 的长 W。

run03–07、10–11 是更早 RTL 版本的 7 个成功完整 copy，不能混作最终 12 个。27 次新执行中，6 次失败保留：run01 是我把 raw mux ID4 误写成6的保护断言错误；run02 是未来 WRITE walk 与 held shared R 的真实依赖环；run08/09 是恢复 longAW 抢在 V 前的真实依赖环；run12/13 已完成字节验证，但 observer 漏计同周期 AW/W，严格物理拍数失败。run22/23 虽退出0，却用了未识别的 +ptw-lat，实际仍31，排除延迟敏感性对比；正确100延迟是 run26/27。没有把失败/无效参数改写为成功。

## 机制和必要修正

两个原始读 parent 分别保留 ID（实际0/4）、进度和 RLAST；两个 context 的 source READ 及 destination WRITE recipe 各经真实 TL 成功，才能开始共享物理读。目标每个 W child 仍走真实 TC，WR recipe 只是同页授权结果复用，不使用 source 的 w 位授权 destination。两个方案共享相同接口、缓存和所有四 recipe；CONF129/cache0010 同用，out wrapper 的缺失 cache 也一致补齐。

q-W adapter 在全局 mux 前保存 raw 2KiB AW，首 VALID 的短 AW 不撤回，真正收齐 child B 才给原 B，每流只保留一个未退休 child。共享 R 由一拍 skid 逐流交付；mask 按已首 offer 的 virtual W 范围冻结，已出现的 RVALID 不撤回。A 旧 q 的 R/W/B 清完才退出；V 沿既有 physical AR 继续，A 以后按实际 cursor 补读。

run02 证明，只准备 source READ 不足以保证此共享长读的进度：未来 A destination walk 被放在长 R 后面，长 R 又等 future W，形成环。目标 WRITE 提前成功的同页 recipe 门修复这一具体循环；这属于已有 translate-first 原理的应用，不能单独命名为新算法。run08/09 又证明，resume pin 提前释放时，A 的 suffix AW 不能抢在 V 未结束的 shared R/W 前。`a_hold_o` 因此还要求 V logical RLAST 已退休；只阻新 child，不撤已有 AW/W。原 raw AW/B 和未完成 job 仍私有保留，quiet 指 A 共享服务义务已清，不等于 backend_idle。

## 结果与贡献判断

- 无暂停，DDR模型延迟31：candidate V/A issue→done 5657/5650，q2 tile8579/8572，平均时间约减34.1%；physical AR1 vs128，源payload同256拍，两目标同256拍，WRITE child各128。延迟100正确生效后为6692/6685 vs18377/18370，说明优势明显随每个读请求的模型代价变化，不能当 MIG 实测。
- 暂停10000 cycles、相同cut64B：candidate AR2，tile AR7，均源R504、两目标W256；V3438 vs3571（约3.72%），A13450 vs13496（约0.34%）。闭合共享读的V/A为15635/15628，但 tile 是必要较强对照，不能只拿 closed 宣称大收益。
- 同样cut，B64＋周期背压：提前恢复 candidate V/A3763/4293，tile3802/4332；长暂停时3763/13712 vs3802/13712。candidate V只约1.03%领先，长暂停 A持平。AR少不等于综合速度大幅领先。

因为共享读、同源识别、fork/退组/补传已有前作，34%不能直接称独立创新。此次实际新增证据是：在译址和不可撤销 W/R 的具体约束下，这个组织能工作，并显示长读与短暂停写的取舍。**如果**更强普通可变分片/有限先读控制在同总缓存、同信息、同暂停债务约束下无法同时得到该进度与成本收益，且关键控制未被前作覆盖，**就可以**立独立微架构贡献；**如果**普通控制同效同成本，**就不能**将当前实现称为新算法。当前不承诺期刊分区。

本轮直接复核 [DMA-Latte v3 §IV-B/VI](https://arxiv.org/html/2511.06605v3)：源地址/size 识别 broadcast、一次读多写已明确已有。复核 [Multicast AXI XBAR §II-A](https://arxiv.org/html/2502.19215v1)：finite-buffer W 顺序死锁和 acquire/commit 已有；该文讨论的是 multicast AW/W/B，并不由此证明其覆盖我们的 READ/TL/early-resume 组织，也不能仅靠接口不同认定新颖。

## 完整成本与下一门

48B 是两真实 iDMA 的 B3 payload FIFO，8B 是新 shared skid payload，合计56B仅这三个位置；实际 XBAR/cuts、TC W FIFO、原 parent/recipe/header、控制和观察 state 全部另有成本。run02 长物理流已接受8拍而源控制器仅交付2拍，正说明其他公共 pipeline 不能漏计。没有综合 LUT/FF/timing，不称56B全系统，也不称已证明同面积最优。

两个 mode 是同一 RTL/二进制运行时切换，因此此次功能比较共用全部硬件，但 ordinary tile 可以做静态裁剪、改变 read tile/先读额度；当前 q2 control 不能写成已穷尽最强普通方法。下一先强化这种普通控制并列全资源/暂停义务预算，再扩多个页面的 READ/WRITE 成功前沿：物理 read 不得越过尚未授权 destination 的范围，resume 仍要守写序。比较双流平均/最坏时间、pause债务、重读量与状态成本，随后才决定生产整合、综合和上板。

窄合同：每 reset 一次正常2KiB作业/流、A 至多一次 pause/resume 且 V 继续推进、单页、same bank、静态成功许可、immutable共享源、CONF129；不覆盖连续job、ATS/device/atomic、动态invalidation/fault、跨页持续进度。退出后 W suffix 允许扩成长段，故不能声称第二次暂停仍有 q 债务界；同时暂停所有成员也未验证。这是后续长期调度必须处理的合同边界。历史原型 `integrate.py` 是初始接入步骤，后续关键修正已在最终文件，复现应直接使用最终源码与 build-argv.json，不重跑该初始化脚本。

完整原始结果及SHA见 execution-ledger.json；两次架构失败源分别保存在 before-write-recipe/、before-resume-fence/，严格观察器失败源另保存在 multi_test-before-counter-fix.cpp。所有原始失败日志留存。

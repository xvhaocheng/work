# §42 已承诺写请求的排空边界（2026-10-08）

完成 1 次新构建、4 次真实双 DMA RTL 仿真；恢复后完整拷贝 4/4 通过。变更限本轮 out 副本，未改生产 RTL、Linux 或原 tests；未做 BIT、上板或写 SD。

表中时间单位为周期，数据量单位为字节；暂停窗口均为 10000 周期。

| 用例 | request→first A quiet | pause-end V W | A pause-end W |
|---|---:|---:|---:|
| 严格停 V 先 | 窗口内无 | 11264 | 0 |
| 普通强对照 V 先 | 4115 | 32768 | 7168 |
| 严格停 A 先 | 窗口内无 | 0 | 0 |
| 普通强对照 A 先 | 5436 | 19456 | 7168 |

**A 侧**：普通强对照首次 quiet 时 7 个真正新生 1 KiB child，A R/W=7168/7168；parents 4→4，无新增 raw parent needed；A job 未完成；第 4 个 2 KiB parent 的 1 KiB suffix 仍由 private 保留。Aoffers 4→4 表示内部写义务数未增加。V 先时 request 的门后 AW held=1，至 cycle 5724 封口时已清；A 先时 held=0。内部 A4 计数早于其输出握手（原始 CSV2638，cycle 6616），不能把输出晚握手当成新增写义务。

**V 侧**：V 先 request 时 V_R=9216、V_W=9208；首次 quiet 时 V_R=V_W=17408。必要 future 字节下界 ≥8192 R / 8200 W；加 A 7168/7168，合计 ≥15360 R / ≥15368 W。普通强对照在快照取等号。这是字节工作量下界，不是周期/带宽上界。V 的 8192 future R 可含已 inflight read，不能换算为 8 个新 V_AR；8200 W 差值来自 8 字节旧 R 领先。

**A4 前驱**：有序 AW/W 允许回溯重建 V 先前的 17 KiB 前缀；V 第 9 笔 AW 见原始 CSV2635，cycle 5733，累计 end 17408；A 第 4 笔见 CSV2638，cycle 6616。实现控制器不计算 online optimal multi-DMA bound。虚拟目的地址 V 0x10010c00、A 0x20008c00。

**翻译/缓存**：仅记录 mode2A hit31/miss35/PTW105，其余三跑 hit32/miss34/PTW102；不作根因归因，也不作简单带宽证明。两次请求起始的翻译状态不同，不能把 4115 与 5436 的差直接归给队列顺序或带宽。

**成本与覆盖**：新增功能状态 ≥65 bit（1 held + 64 frontier）；diag counter/header 为额外未综合状态，不声明不计面积/free。实测覆盖 V 先时门后 held=1、A 先时 held=0；没有单独测得 RR 已锁而尚未 push 的现场，该保持条件只作源码审查。

**模式边界**：mode1 有限显式停止不是生产死锁；mode2 普通 allow 允许源侧 7 个 NEW 1 KiB child、无 NEW AW。

因为普通强对照已经达到这两个现场的最低后续数据工作量，这一步不能立独立新机制。

**下一步**：多 bank 两 W 序 + 源 child 精确前缀释放约束；普通 greedy/非均匀 splitter/短 burst 同 payload/metadata/mapping/释放点对照；若只剩单一有序队列守恒，按 principal innovation 停分支。不承诺最终等级。

**限制**：静态 mapping、CONF1、同 bank、单 job；不外推 partial strobe、不等 W 序、fault/invalidation、公平性、多 bank、硬实时、通用周期界；不做 no-pause/§39 性能优势声明。

**归属与证据**：Codex 负责 RTL 设计/关键评审/构建运行/最终验收；DeepSeek 提供 harness/证据/草案；native audit 只读源码，未执行。无 Claude。全部 4 个 `mode*-events.json` 已由 root 生成，4 份原始日志均由 Codex 复核；`var` 为正常 TC source AR fallback 计数，与 physical bypass 不同，source64 全部 checks valid。过程账本中的格式非法 patch/revision 失败仅来自 out 原始 worker 响应，不影响技术结论。

[执行账本](../out/committed-write-drain-20261008/stage-evidence.json) · [根级结果与下界推导](../out/committed-write-drain-20261008/codex-result-acceptance.md) · [源码审查](../out/committed-write-drain-20261008/native-control-review.md) · [下一创新判别](../out/committed-write-drain-20261008/codex-next-decision.md)

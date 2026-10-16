# §52：提前恢复保留共享组的强普通策略

Codex亲自设计/审读/执行；复用§51 DeepSeek普通driver。一次新SV build成功、一次C++ parser增量relink成功，8新完整copy PASS，根独立核每个CSV声明source R和目标physical W退休计数。out-only真实RTL/行为DDR，非实板/面积。每reset双流各一个2KiB、四项成功recipe、静态immutable源、q2-W与q-singleton。

规则mode3：detach_pending排到A q-W/B quiet且没有A frozen skid时，若resume pin已释放、共享skid为空、两流logical R cursor相等，就清pending而保原共享组。否则按原协议退出/repair。没有撤AR/RVALID，没有重置job/cursor，旧AW/W/B必须兑现。逻辑仍是保留/退组的普通有限状态策略，不用速度直接命名新算法。

## 实际结果

pin pause10，B64、READY周期背压（早恢复专门设置require-quiet=0，不宣称窗口内quiet）：

| 起动/请求 | 原sticky退出 V/A | 保组 V/A | source AR/R |
|---|---:|---:|---:|
| V先、一次 | 13277/28076 | 13855/13848 | 125/504 → 1/256 |
| A先、一次 | 13270/28077 | 13846/13849 | 125/504 → 1/256 |
| V先、两次 | 13277/28076 | 13855/13848 | 125/504 → 1/256 |

两次请求都实际trigger/seal/resume，未在两次之间reset；这组故意不要求10-cycle内quiet。V先的平均耗时减33.01%、A减50.68%、V增加4.35%。这是双流取舍，不说两者都更快；减少duplicate source payload同时保共享写调度会略延长V。

长10000pause、require-quiet=1：两方法均V5161/A19848、AR125/R504，run07/run08 CSV逐字节相同。真正已退出并产生进度差后，当前保组规则不取消必需补读。

## 研究判断

因为恢复时尚无进度差，短暂停的无条件退出/repair是旧策略成本，非AXI/IOMMU必要下界。上述规则补强公平普通基线；如果只是该取消规则与普通组成员管理同决策同成本，就不能称独立新算法。仍要探索迟到翻译与不可撤source R的具体差异；也须给普通已存在private FIFO以有限先读机会，避免以过强coverage门人为拖慢对照。

当前成功不覆盖动态fault/invalidation、所有成员pause或连续job。真实DMA NO_ERROR_HANDLING边界继续成立。完整源/argv/log/metric/SHA见execution-ledger.json；无production/Linux/BIT/board动作。夜间工作继续至08:00 JST。

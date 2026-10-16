# §39 普通已准备源读物理旁路：离线证据汇总（脚本提案输出）

本文件由 `summarize_revised.py` 生成，仅做离线证据汇总；不代表 RTL 验证，也不构成独立创新结论。

- 输出目录：`/home/xvhaocheng/work/iommu-platform/out/prepared-source-bypass-20261008`
- §38 参照目录：`/home/xvhaocheng/work/iommu-platform/out/multi-parent-preparation-20261008`
- 钟域：SV 事件 cycle 与 TB issue/done cycle 不同钟原点，脚本不做跨域算术。
- 六场景全部收录：是
- 声明状态：RAW4 functional_bits=3317、PREP4 functional_bits=1281、合计 4598（仅声明，非综合面积/总成本）。
- 顶层构建：status=evidence_ok，exit_code=0，reused=False

## 六个运行

| 运行 | physical-bypass | 状态 | direct | fallback | nat var | write_wait | TL accept/resp | V-first TB V/A/mean |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| off-v-32768 | 0 | evidence_ok | 0 | 64 | 64 | 0 | 66/66 | 23380/26122/24751.0 |
| off-a-32768 | 0 | evidence_ok | 0 | 64 | 64 | 0 | 66/66 | - |
| cache-v-32768 | 1 | evidence_ok | 62 | 2 | 2 | 25 | 66/66 | 17704/20246/18975.0 |
| cache-a-32768 | 1 | evidence_ok | 62 | 2 | 2 | 16 | 66/66 | - |
| wait-v-32768 | 2 | evidence_ok | 64 | 0 | 0 | 24 | 66/66 | 18146/20984/19565.0 |
| wait-a-32768 | 2 | evidence_ok | 64 | 0 | 0 | 24 | 66/66 | - |

## V-first 比较（TB issue/done cycle，A-first 不进入均值）

- **off**：mean=24751.0 cycles（单位：TB issue/done）
  - vs_old38_best_ordinary：ref=24751 cycles，Δ=0 cycles
    - 分母=旧参考值 24751：100*0/24751 = 0.0%
  - vs_old_post_window4_depth4：ref=18824 cycles，Δ=5927 cycles
    - 分母=旧参考值 18824：100*5927/18824 = 31.4864003399915%
    - 分母=本次均值 24751（与上一行分母不同，不是同一个数）：100*5927/24751 = 23.946507211829825%
    - 旧差距 5927 cycles 缩小：残差 5927 cycles，100*0/5927 = 0.0%（分母=旧差距）
  - 条件判断：普通强对照的性能比较；独立创新判定见 Codex 报告。
- **cache**：mean=18975.0 cycles（单位：TB issue/done）
  - vs_old38_best_ordinary：ref=24751 cycles，Δ=-5776 cycles
    - 分母=旧参考值 24751：100*-5776/24751 = -23.336430851278735%
  - vs_old_post_window4_depth4：ref=18824 cycles，Δ=151 cycles
    - 分母=旧参考值 18824：100*151/18824 = 0.8021674458138547%
    - 分母=本次均值 18975（与上一行分母不同，不是同一个数）：100*151/18975 = 0.7957839262187089%
    - 旧差距 5927 cycles 缩小：残差 151 cycles，100*5776/5927 = 97.45233676396153%（分母=旧差距）
  - 条件判断：普通强对照的性能比较；独立创新判定见 Codex 报告。
- **wait**：mean=19565.0 cycles（单位：TB issue/done）
  - vs_old38_best_ordinary：ref=24751 cycles，Δ=-5186 cycles
    - 分母=旧参考值 24751：100*-5186/24751 = -20.952688780251304%
  - vs_old_post_window4_depth4：ref=18824 cycles，Δ=741 cycles
    - 分母=旧参考值 18824：100*741/18824 = 3.93646408839779%
    - 分母=本次均值 19565（与上一行分母不同，不是同一个数）：100*741/19565 = 3.787375415282392%
    - 旧差距 5927 cycles 缩小：残差 741 cycles，100*5186/5927 = 87.49789100725494%（分母=旧差距）
  - 条件判断：普通强对照的性能比较；独立创新判定见 Codex 报告。

## A-first 镜像一致性检验（不混平均）

- off：mirror_ok=True
  - V-first(v,a)=23380/26122；A-first(v,a)=26122/23380
  - 交换后两流逐项差：Δv=+0，Δa=+0 cycles
  - 均值：V-first 24751.0，A-first 24751.0，mean Δ=0.0
  - 检验 V-first(v,a) 是否等于 A-first(a,v)；after_swap_delta=(aa-vv, av-va)；A-first 不参与均值
- cache：mirror_ok=False
  - V-first(v,a)=17704/20246；A-first(v,a)=20279/17737
  - 交换后两流逐项差：Δv=+33，Δa=+33 cycles
  - 均值：V-first 18975.0，A-first 19008.0，mean Δ=33.0
  - 检验 V-first(v,a) 是否等于 A-first(a,v)；after_swap_delta=(aa-vv, av-va)；A-first 不参与均值
- wait：mirror_ok=True
  - V-first(v,a)=18146/20984；A-first(v,a)=20984/18146
  - 交换后两流逐项差：Δv=+0，Δa=+0 cycles
  - 均值：V-first 19565.0，A-first 19565.0，mean Δ=0.0
  - 检验 V-first(v,a) 是否等于 A-first(a,v)；after_swap_delta=(aa-vv, av-va)；A-first 不参与均值

## 异常与失败证据

- 异常 `a_first_mirror_ok=False` @ `cache (mirror)`：mirror_ok=False：V-first(v,a)=17704/20246，A-first(v,a)=20279/17737；交换后两流逐项差 Δv=+33、Δa=+33 cycles；V-first mean=18975.0，A-first mean=19008.0，mean Δ=33.0；不构成镜像复制，A-first 不进入均值。


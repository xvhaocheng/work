# G11 引用关系有界纠正

## 0. 核对前提与结论

- 我无法执行 `rg`；以下核对完全基于所给 `cuptw2026.txt`、`softwalker2025.txt` 全文文本，结论与 Codex 的 `rg` 结果一致：**`cuptw2026` 全文（含正文与 [1]–[70] 参考文献表）没有任何 “SoftWalker” 字符串**。
- 因此：`softwalker2025 → cuptw2026` 的 `actual_citation` **不成立**，应改为 `mechanism_comparison`（任务要求）；不新增任何其它关系；`mgvm2022` 关系保持不变。

---

## 1. 需要修订的错误列表

### 错误 A（核心）：`actual_citation` 关系捏造，且定位错位
- 现说法：`softwalker2025.relations` 中 `{"target":"cuptw2026","type":"actual_citation","source":"cuptw2026 L1523-L1527 reference [37] cites SoftWalker"}`（markdown 报告 L348–L351；独立 JSON `G11_gpu_expansion-papers.json` L172–L177）。
- 事实核对：
  - `cuptw2026` L1534–L1536 的 **reference [37] 是 SnakeByte**（Jiwon Lee, Ju Min Lee, Yunho Oh, William J Song, and Won Woo Ro. 2023），不是 SoftWalker。
  - `cuptw2026` L1523–L1527 实为 **reference [33]（Khairy et al., LASP / locality-centric data and threadblock management）与 reference [34]（Kotra et al., Increasing GPU translation reach）**，既不是 [37]，也与 SoftWalker 无关。
  - `cuptw2026` Related Work（Section 9，L1371–L1399）通篇只引 NHA/MASK/Marching Page Walkers [36]/SnakeByte [37]/Barre Chord [25]/speculative translation [53] 等，**未引 SoftWalker**。
- 结论：不是“定位写错”，而是**关系不存在**。不能因“两篇机制相近”推引用。

### 错误 B：导航摘要把该捏造引用写进正文
- `G11_gpu_expansion.md` L12 末句：“SoftWalker 在 related work 中被 cuPTW 引用。”——**不成立**，需替换。
- 同句其余部分（MGvm/Barre/cuPTW/CLAP 的描述）本身无误，只需删改末句。

### 错误 C：对照矩阵把 SoftWalker 的调度策略误写成“SM idle 周期执行”
- `G11_gpu_expansion.md` L211（softwalker2025 行，“何时必须完成”列）：“…PW Warp 在 SM idle 周期执行”——与原文冲突：SoftWalker 给 PW Warp 的是**最高调度优先级**，不是“只在 idle 时才执行”。需替换（详见 §2）。
- 注：SoftWalker §5.4 的 “先硬件 walker、无空闲才转软件 walker” 是 **hybrid 版本的硬件/软件 walker 选择**，与 “PW Warp 调度优先级” 是两个不同轴，不能混用。

---

## 2. 准确 replacement 文本

### 2.1 替换 L12 末句（错误 B）
> **与旧文关系**：MGvm 解决 MCM GPU 的 L2 TLB/PTE 远端访问与 HSL 切换；Barre Chord 用 coalescing group 和计算式翻译减少 IOMMU/PTW；cuPTW 把页表遍历搬到 idle CU + translation wavefront。CLAP 明确对比 MGvm/F-Barre。**SoftWalker 与 cuPTW 之间没有引用关系**：cuPTW 全文与参考文献表均未出现 SoftWalker，其 related work（L1371–L1399）只引 NHA/MASK/Marching Page Walkers [36]/SnakeByte [37]/Barre Chord [25] 等，且原报告所指 reference [37] 实为 SnakeByte（L1534–L1536），L1523–L1527 则是 [33] LASP 与 [34] Kotra et al.。二者仅可作机制对比：SoftWalker 的 PW Warp 被赋予**最高调度优先级**（`softwalker2025` §4.2，报告已引 L680–L710），而 cuPTW 的 translation wavefront **只在无 kernel wavefront 可发射/资源空闲时才发射**（`cuptw2026` L634–L637、L661–L663）。

### 2.2 替换 L211 矩阵单元格“何时必须完成”列（错误 C）
- 完整版：
> TLB miss 后、warp replay/resume 前；PW Warp 为每 SM 专用 warp 且被赋予**最高调度优先级**（`softwalker2025` L680–L710），依赖不规则负载大量 stall 周期（约 90%，L592–L598）；对 regular 负载需 hybrid（L1013–L1026、L1337–L1347）。与 cuPTW“仅当无 kernel wavefront 可发射时才执行”不同（`cuptw2026` L634–L637、L661–L663）。
- 若需控制单元格长度，可简写为：
> TLB miss 后、warp replay/resume 前；PW Warp（每 SM 专用）具**最高调度优先级**（`softwalker2025` L680–L710），靠不规则负载 ample stall 周期（约 90%，L592–L598）。

### 2.3 新增/替换“SoftWalker vs cuPTW 调度差异”对比段（替代被删的引用断言；建议置于 C1 第 1 点附近）
> **SoftWalker vs cuPTW 的调度/发射策略差异（原文依据）**
> - **SoftWalker**：PW Warp 是每 SM 的专用 warp，被赋予**最高调度优先级**，理由原文为“delays in page walks can stall the execution of other warp instructions and degrade overall performance”；Normal 版本依赖不规则负载充足的 stall 周期（约 90% warp-scheduler cycles 为 memory/scoreboard stall）来抵消占用的 issue 周期；只有 **hybrid** 版本才把请求优先交给 hardware PTW。证据：`softwalker2025` L680–L710（最高优先级）、L592–L598（~90% stall）、L1013–L1026（hybrid：先硬件 walker，无空闲才转软件）、L1337–L1347（regular app 4.3%–10.9% slowdown）。
> - **cuPTW**：translation wavefront 仅在 **no kernel wavefronts 可发射 / 资源空闲** 时才被 wavefront scheduler 发射，明确以 “preventing interference with kernel execution” 为约束（让位于 kernel、填 idle issue slot，不抢占 kernel warp）。证据：`cuptw2026` L634–L637、L661–L663。

---

## 3. JSON 局部替换对象（只替换 `softwalker2025.relations` 中 `cuptw2026` 一项）

以下对象在**两处**都要替换（对象内容一致）：
- `G11_gpu_expansion.md` 内嵌 JSON L349；
- `G11_gpu_expansion-papers.json` L173–L177。

**删除该对象：**
```json
{
  "target": "cuptw2026",
  "type": "actual_citation",
  "source": "cuptw2026 L1523-L1527 reference [37] cites SoftWalker"
}
```

**替换为：**
```json
{
  "target": "cuptw2026",
  "type": "mechanism_comparison",
  "source": "softwalker2025 L680-L710 (PW Warp = highest scheduling priority) vs cuptw2026 L634-L637, L661-L663 (translation wavefronts issued only when no kernel wavefronts available / resources idle); no citation exists in either direction"
}
```

替换后，`softwalker2025.relations` 应为两项（`cuptw2026` 的 `mechanism_comparison` + 原有 `mgvm2022` 的 `mechanism_comparison`，后者不改）：

```json
"relations": [
  {
    "target": "cuptw2026",
    "type": "mechanism_comparison",
    "source": "softwalker2025 L680-L710 (PW Warp = highest scheduling priority) vs cuptw2026 L634-L637, L661-L663 (translation wavefronts issued only when no kernel wavefronts available / resources idle); no citation exists in either direction"
  },
  {
    "target": "mgvm2022",
    "type": "mechanism_comparison",
    "source": "softwalker2025 L1739-L1754"
  }
]
```

> 未新增任何关系；`clap2025` / `fshpt2024` / `laegis2026` / `oasis2025` 的 relations 与 evidence 均不改。

---

## 4. 完整必要原文定位（供 Codex 复核）

| 用途 | 文件 | 行 | 内容要点 |
|---|---|---|---|
| reference [37] = SnakeByte（非 SoftWalker） | `cuptw2026.txt` | L1534–L1536 | “`[37] Jiwon Lee, Ju Min Lee, Yunho Oh, William J Song, and Won Woo Ro. 2023. Snakebyte …`” |
| 误标为 [37] 的 L1523–L1527 实为 [33]/[34] | `cuptw2026.txt` | L1523–L1527 | L1523–L1525 = [33] Khairy et al.（LASP）；L1526–L1527 = [34] Kotra et al. |
| cuPTW related work 未引 SoftWalker | `cuptw2026.txt` | L1371–L1399 | 只提 NHA/MASK/Marching Page Walks [36]/SnakeByte [37]/Barre Chord [25] 等 |
| cuPTW 仅空闲才发射 translation wavefront | `cuptw2026.txt` | L634–L637 | “if all wavefronts … not ready to issue, the wavefront scheduler opportunistically issues translation wavefronts … prevents interference with kernel execution” |
| cuPTW “Sent to Memory” 阶段同条件 | `cuptw2026.txt` | L661–L663 | “when no kernel wavefronts are available in the wavefront pools …” |
| SoftWalker PW Warp = 最高调度优先级 | `softwalker2025.txt` | L680–L710（报告已引该段；其中含 “The PW Warp is given the highest scheduling priority…”，约句位于该段中后部） | 专用、structurally isolated PW Warp；最高优先级 |
| SoftWalker 依赖大量 stall 周期 | `softwalker2025.txt` | L592–L598 | “scheduler cannot issue… nearly 90% … memory and scoreboard stalls” |
| SoftWalker hybrid（先硬件 walker） | `softwalker2025.txt` | L1013–L1026 | “prioritizes sending page walk requests to hardware page walkers … once no free hardware walkers remain … software walkers” |
| SoftWalker regular app 退化 | `softwalker2025.txt` | L1337–L1347 | 2dc/histo/red/scan slowdown 4.3%/8.6%/10.9%/3.1% |

---

## 5. 假设与未决项
- 假设：所给两份全文文本完整、行号即引用行号。
- 未决：`softwalker2025` “highest scheduling priority” 句的**精确单行号**因双栏排版在文本抽取中可能偏移±2–3 行；已用报告原本引用的 L680–L710 作为安全锚点并给出原句，Codex 可据此定精确行。除此之外的行号均与所给文本直接对齐。
- 验收检查（建议）：改后 `cuptw2026`/`softwalker2025` 两文件中不含任何带 `actual_citation` 的互指；`softwalker2025.relations[0].type == "mechanism_comparison"` 且 source 同时锚定 `softwalker2025` 与 `cuptw2026` 原文；L12 与 L211 不再出现“被 cuPTW 引用/related work 引用”表述。
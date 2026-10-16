# P4M 诚实吞吐/数据正确性可行性(纯静态源码证据)

## 一、结论(先行)

1. **00093 现有 `IDMA_IOC_RUN_P4M`(0x0d)+ 现有 `bench/idma_experiment --p4m` 这条链路本身,不经过旧 `IDMA_RESEARCH_COST` / RESEARCH2 / E3 controlled 代码**,其判定依据只落在“公共 observer 的 P4M bank + 通用计数 + 路径 bank”上,理论上可以给出“硬件窗口内的 A 完成计数 + 窗口周期”这种硬件窗口吞吐,以及 `idma_verify()` 的逐字节数据正确性校验。
2. 但有两条**硬依赖**必须同时满足,才谈得上“能跑”:
   - **HPM 必须存在且 `riscv_iommu_hpm_acquire` 成功**:`idma_run_session2()` 在 `idma.c:2367` 取 HPM,失败即 `goto out_cleanup`;CLI 侧在 `bench/idma_experiment.c:179-181` 也强制要求 `CAP_DID_HPM|CAP_P4M|CAP_HW_BATCH`。
   - **路径 observer 必须存在并被校验**:因为 `o->path = o->p4m || ...`(`idma.c:981`),P4M 运行会置 arm command bit26,并在 `idma_observation_finish()` 校验 path bank 64..191(`idma.c:1076-1105`)。也就是说 **P4M 并不“独立于 dma_path_observer”**,它与旧 COST/E3 *无关*,但与 path observer *有关*。
3. 旧 `RESEARCH2_CAPS`、cost、E3 trace **完全不参与** `RUN_P4M` 的验证;p4m 只要求 observer magic 为 V8/V7(`idma.c:982`),当前 `dma_observer.sv:42-46` 在 `ResearchEnable=1` 下正好给 V8。所以“irrespective old cost/E3 trace”对 *cost/research 观察器*成立,对 *path 观察器*不成立(需单列)。
4. 关于“新的 N 元数据/快速 accept RO 计数器追加到未来 bank”,旧 `RESEARCH2_CAPS` 只读 4 个字(0xF00..0xF03)+ observer magic,且 CLI 还额外禁止 0xF03 出现 0xF 之外的特征位(`bench/idma_research.c:1306,1364`),**确实存在真实“读不到”的访问缺口**;若要在“不假装 role2、不跑旧 cost”的前提下读它们,需要一个新的只读接口,理由与最小接口见第五节。

---

## 二、RUN_P4M 端到端判据(file:line)

### 1) 入口与批次语义
- `idma_run_p4m()`:拷贝用户结构、`idma_p4m_clear_outputs()`、`idma_p4m_validate()`,再调 `idma_run_session(..., batch=true, pair=NULL, diag=NULL, queue_pair=false, &p, research=NULL)`(`linux/drivers/misc/idma.c:2618-2636`)。
- `idma_run_session()` 直接转 `idma_run_session2(..., cfg=NULL)`(`idma.c:2609-2616`)。**cfg==NULL ⇒ 不会走 `idma_research2_preflight` / `pre_arm` / `method_freeze` / `capture_ext`**(`idma.c:2291-2295, 2450-2454, 2534-2556`)。这正是“不经旧 cost/E3 判据”的证据。
- `idma_p4m_validate()`(`idma.c:1293-1319`):victim 必须 id0/did10;`chunk_bytes==PAGE_SIZE`;`victim_transfers∈[1, IDMA_BATCH_MAX_TRANSFERS]`;`warmup<=IDMA_EXPERIMENT_MAX_TRANSFERS`;有 A 时 `aggressor_id==1`;无 A 时 `aggressor_gap_cycles==0`。
- 批处理附加约束:`idma_run_session2()` 对 batch 强制 `chunk_bytes==PAGE_SIZE && victim_transfers<=BATCH_MAX`(`idma.c:2278-2281`)。

### 2) caps / observer magic 精确检查
- 驱动侧 caps:始终声明 `SW_REFILL|HW_BATCH|E3_PAIR|E3_DIAG|E3_QUEUE_PAIR|E3_CONTROLLED|P4M|RESEARCH`;`DID_HPM` 仅当 `riscv_iommu_hpm_supported(dev)` 为真时补上(`idma.c:2699-2712, 2731-2734`)。
- observer magic 常量:驱动 `IDMA_OBS_MAGIC_V8=0x49444d414f425308`(`idma.c:52`),RTL `dma_observer.sv:42-46` 在 `ResearchEnable` 时输出同一值;`P4MEnable` 单独时是 V7=…5307。`idma_observation_prepare()` 对 p4m 只接受 V8/V7(`idma.c:982`)。
- 通用 did 门:`o->control->did != 10` 直接拒绝(`idma.c:996`);RTL 侧该 observer 绑在 `i==0`,`DEVICE_ID=24'(10+i)`(`ariane_peripherals_xilinx.sv:1182, 1186`)。
- P4M bank 常量:驱动 `IDMA_P4M_BANK_MAGIC=0x49444d4150344d01`(`idma.c:57`),RTL word448 同值(`dma_observer.sv:172`)。

### 3) PIO 触发 vs controlled launcher
- RUN_P4M 走**普通 PIO/NEXT_ID 触发**:`idma_batch_window → idma_batch_start`,在关中断段读 `IDMA_NEXT_ID_REG_OFFSET` 触发(`idma.c:538-567`),之后 `idma_batch_window` 轮询 `IDMA_DONE_REG_OFFSET`(`idma.c:866-899`)。
- controlled launcher(bit29 / 0xb0 区)只在 `IDMA_IOC_RUN_E3_CONTROLLED` 使用(`idma.c:1021-1022, 2721-2722`);`idma_observation_prepare()` 对 p4m 明确要求 `!o->e3 && !o->controlled && !o->delay128`(`idma.c:1014-1018`)。RTL ARM 接受逻辑也要求 P4M 必须 **不置** bit27/28/29,且必须置 bit25(batch)、victim 端口 0(`dma_observer.sv:476-489`)。故 P4M 与 launcher 完全解耦。

### 4) quiet
- RUN_P4M **不读** `research_dma_quiet`。quiet 只在 research arm/cost 与 `dma_research_observer` 内生效(RTL `dma_observer.sv:98-104, 491`;`dma_research_observer.sv:287-288, 1137-1141`)。P4M 的 ARM 接受条件不含 quiet(`dma_observer.sv:476-524`)。所以 quiet 不是 P4M 的前提。**注意**:RTL 在 P4M arm 时仍会拉 `research_arm_o`(`dma_observer.sv:491`),但 `dma_research_observer` 的 `mode_q` 默认=NONE,该 arm 分支对 NONE 是 no-op(`dma_research_observer.sv:1137-1181`),因此不会污染 P4M 结果。

### 5) observer bank 依赖
- P4M bank:索引 448..463,共 16 字;索引写入许可在 `dma_observer.sv:451`,读多路在 `dma_observer.sv:161`。驱动读 `IDMA_P4M_OBS_WORDS=35` + `IDMA_P4M_WORDS=16`(`idma.c:1385-1388`),RTL `Count=35`(`dma_observer.sv:47`),word 映射 448..463 与 `idma.c:1393-1475` 一一对应。
- **路径 bank 64..191 是 P4M 的隐藏依赖**(见上 `idma.c:981, 1076-1105`),其存在依赖 `PathEnable`,在 `ariane_peripherals_xilinx.sv:948, 951` 由 `InclIOMMU` 打开。

### 6) 数据正确性
- batch 模式下触碰范围来自 `IDMA_BATCH_COMPLETED_OFFSET`(`idma.c:842-853`),随后 drains 并 `idma_verify()` 对 V/A 逐字节校验 src/dst pattern(`idma.c:2513-2524, 756-803`),失败即 `-EILSEQ`(`idma.c:799`)。
- `idma_p4m_finish()` 再做 bank 级一致性:magic、`window[1]==observation[0]`、flags(bit1 closed、bit0==has_a、bit2 strict)、`window[12]==0`、`observation[7]==observation[8]==victim_transfers`、`window[13]==window[14]==victim_transfers`、ID 在界内、守恒/边界约束(`idma.c:1391-1476`)。

### 7) 窗口与吞吐来源
- 窗口周期 `window[1] = p4m_cycles_q = count_n[0]`,即 observer 在 `active` 相内的自计周期(`dma_observer.sv:368-369, 401-409`);窗口语义是 **(open, close]**,开窗在 ARM 接受沿 + victim 首次 issue,关窗在 V 的 done 计数达到 `target_q` 时(`dma_observer.sv:292-317, 331-353, 388-413`)。
- A 窗口完成数 `window[3] = p4m_a_done_q`(RTL 451/实际映射 448+3),窗口内 A issue=window[2]。
- CLI 侧:`validate_p4m()`(`bench/idma_experiment.c:272-414`)复算同一套约束;`print_p4m()` 用 `w[3]*4096*50e6/cycles` 得到 A 的窗口字节速率(`bench/idma_experiment.c:448-451, 495`)。**V 吞吐未直接打印**,但可由 `observation[7]`(V done=victim_transfers)与 `hardware_window_ns` 推导;50MHz/20ns 是 CLI 硬编码假设,不是板级标定。

---

## 三、“诚实吞吐 + 数据正确性”的判定

- **可行(基于源码判定,未执行验证)**:P4M 窗口计数 + A 完成 + `idma_verify()` 足以构成“硬件窗口内的 A 吞吐 + 全字节数据正确性”;且这条链路不引用旧 cost/RESEARCH2/E3 判据。
- **前提**(缺一不可):
  1. observer 呈现 V8/V7(`idma.c:982`)——当前 `dma_observer.sv:42-46` 满足;
  2. `PathEnable=1` 且 `dma_path_observer` 行为正确,否则 `idma.c:1076-1105` 会判 invalid;
  3. DMA 核 batch 引擎(0x80 区 BATCH_MAGIC/BATCH_CONTROL/BATCH_TARGET/BATCH_PAGES/COMPLETED/ISSUED)存在,否则 `idma.c:2320-2328, 2423-2425` 直接失败;
  4. 若用 A 造吞吐,gap 寄存器 0xd0/0xd8 必须存在(`idma.c:1344-1369, 2429-2436`);
  5. HPM 可获得(`idma.c:2367`)。
- **Solo 注意**:无 A 时 `w[3]=0`,CLI 的 A 速率恒为 0(`bench/idma_experiment.c:443, 448-451`);Solo 只能给出 V 的窗口时长与全字节校验,V 速率需自行由 raw 字段推导。**要“吞吐数字”建议带 A。

---

## 四、确切 CLI(若上面前提成立)

先查 caps(需 `CAP_DID_HPM|CAP_P4M|CAP_HW_BATCH` 同时为真,见 `bench/idma_experiment.c:179-181`):

```
./idma_experiment --device /dev/idma0 --caps
```

Solo P4M(无 A,`--a-gap-cycles` 必须为 0,`bench/idma_experiment.c:143-147`):

```
./idma_experiment --device /dev/idma0 --p4m \
  --aggressor none --a-gap-cycles 0 \
  --victim-pages 1 --aggressor-pages 8 \
  --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000
```

带 A 的 P4M(吞吐主测量;gap 可选 0/500/1000/2000/4000,内核只对 “无 A 且 gap≠0” 报错,`idma.c:1310-1311`;CLI 亦仅限制 Solo,`bench/idma_experiment.c:139-147`):

```
./idma_experiment --device /dev/idma0 --p4m \
  --aggressor 1 --a-gap-cycles 0 \
  --victim-pages 1 --aggressor-pages 8 \
  --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000
```

约束汇总:`chunk` 必须 4096;`victim_transfers ≤ IDMA_BATCH_MAX_TRANSFERS(1<<20)`;`victim_pages/aggressor_pages ≥ 1`;`run_timeout_ms>0`(`bench/idma_experiment.c:150-166`;`idma.c:1312-1315, 2279-2281`)。

---

## 五、RESEARCH2_CAPS / 读访问缺口评估

- `IDMA_IOC_RESEARCH2_CAPS`(0x10)在 `idma_research2_caps_ioctl()` 里只读 4 个字 0xF00..0xF03 + observer magic(`idma_research.h:826-861`),并且要求 `role∈{E2=1, METHOD=2}`、observer magic=V8。
- CLI 侧同样只认 `magic/version/role/features/observer_magic`,并对 features 做**精确匹配**:role1 要求 0x7、role2 要求 0xF,且禁止 `~0xF` 位(`bench/idma_research.c:1288, 1306, 1364`;`idma_research2_caps` 结构见 `linux/include/uapi/linux/idma.h:249-254`)。
- **真实缺口**:因此“追加新的 N 元数据/快速 accept 只读计数器”若放在 0xF00 附近或复用 0xF03 特征位,旧 caps 接口都读不到、或被 CLI 直接判失败;唯一现成的通用读 bank 通道是 `RUN_RESEARCH/2` 的 raw dump(`idma_research.h:102-140`;raw 4096 字覆盖 research 索引 0..4095,`bench/idma_research.c:1516`),但它**必须**跑旧 cost/E2 会话并满足全套校验与 role(`idma_research.h:341-445, 484-520`)。
- 同时 RTL research 读多路只解码少数区间(`dma_research_observer.sv:1421-1601`,如 0x100..0x160/0x200..0x800/0x800..0xC00/0xC00..0xC40/0xD00..0xD70/0xF00..0xF10/0xD80..0xD90),追加 bank 必须落在已解码区间或同步扩展该 mux。
- **若确需新接口,最小接口(仅陈述,不设计)**:一个只读、带界长的 bank 读取 ioctl,in={abi_version,struct_size,base,count,reserved},out={u64[]},门禁为 `idma_experiment_lock` + observer magic V8 + `access_ok` + 索引上界;不设 role、不触发 arm/cost。**理由唯一**:在不假装 role2、不启动旧 cost/E2 的前提下,让用户态读到未来追加的只读计数器。

---

## 六、不确定性与未提供依赖

1. **`dma_path_observer.sv` 未提供**:P4M 的最终有效性依赖 `idma.c:1076-1105` 对 path bank 64..191 的校验,无法在本证据内确认其字段语义。
2. **DMA 核 batch/gap/status/NEXT_ID/DONE/ACTIVE_CYCLES 寄存器未提供**:这些不属于 `dma_observer.sv`(它只覆盖 cfg 0..0x7f),`idma.c:2320-2328, 2423-2436, 1344-1369` 的正确性无法核验。
3. **HPM 实现未提供**:`idma.c:2367` 必须 acquire 成功;虽然 P4M 不声明 HPM 翻译统计,但缺少 HPM 时整轮直接失败。
4. **`IOMMUResearchRole` 实际值未知**(`ariane_peripherals_xilinx.sv:33, 986, 1011, 1575`),只影响 research2 caps/role,不影响 RUN_P4M。
5. **“NewN” 架构细节(2 bank 事务控制器、AW/AR slot 减半、原始 ID origin bit、demux/mux 的 W 关联、共享 TL/PTW、单 bank-tagged 翻译响应口、MSI_BT_ONLY profile)均不在本次提供文件中**,无法据此确认 P4M 字段或 lane 映射可原样复用——现有 RTL 的 lane 选择完全由 ARM 的 `[9:8]/[17:16]` 与 DID 10/11 驱动(`dma_observer.sv:270-287, 331-353`;`dma_research_observer.sv:94-95, 135-138`),这属于需要 Codex 主导的跨模块改动,不在本任务范围。
6. 50MHz/20ns 是 CLI 固定假设(`bench/idma_experiment.c:495`),并非板级标定。
7. 本次为纯静态源码判读,**未做任何编译/上板执行验证**。

---

## 七、明确不做/不主张

- 不主张“已验证通过”或任何实测吞吐/正确性数字。
- 不提出 Linux 侧仅为了“多一个 capability bit”的改动:`RUN_P4M` 路径本身不需要 Linux 变更;是否要新 ioctl 只取决于第五节所述“读访问缺口”,与 P4M 吞吐/正确性无关。
- 不重写设计、不改接口;NewN 相关架构结论留给 Codex。
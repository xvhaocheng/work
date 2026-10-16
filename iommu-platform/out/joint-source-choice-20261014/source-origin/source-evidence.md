# 现有 composition 正常来源：软件 origin / owner 序列核查

实际任务：`actualgoal1791943603`。本席仅核当前软件调用来源与完成合同；root 独占 RTL 首次 VALID、admission、feedback、普通规则与因果裁决。没有运行程序、ioctl、模型、构建、测试、旧 validator、trace 解析或板端操作。所有判断都是当前文件字节的源码事实或明确的时间未知项。

## 决定结论

**此现存入口只启动实例 0（DID10）及可选实例 1（DID11），不启动 DMA2/3。** 原因是 tool 固定 `aggressor_id=1`，composition 继承的 P4M validator 又拒绝 `victim.id != 0 / did != 10` 与非 1 的 aggressor；observer arm 再要求 `did == 10+id`。`--device` 可改路径，但不能靠改路径通过上述实例限制。四个硬件实例存在不能充当此入口已激活四源或异构长/短源的证据。见 E01、E03、E10。

**有 A 时测量开始是 CPU 有序 A→V 两次 NEXT_ID trigger read，并非同边沿联合 GO，也不是等 A 完成再启动 V。** 两组 descriptor 都先配置，才在 IRQ/preemption 关闭的短段内先读 A 的 NEXT_ID，再读 V 的 NEXT_ID；有 ID 后才在段外提交软件所有权。这里的“先”只指软件 trigger read 顺序。它不证明首 AR/AW VALID、成功握手、IOMMU 首选择或 MIG 接受顺序，也不证明两个首请求必然同时待选或必然不能同时待选。见 E06。

**来源包含已有的有成本前缀：映射/初始化/CPU flush，以及 W>0 时的真实软件 warmup；但没有确定的逐周期 DDR/translation 历史。** W 是 V 的完成数，A 的 warmup 完成/提交数与最后页依实际轮询时序而变。测量之前 A 残余任务被 drain，batch 配置把双方 first descriptor 的 next_offset 重置为 0。不可把未来 PTE、DDR 服务、翻译热状态或 warmup 成本当作免费条件。见 E04、E05、E09。

**软件可以提供 lane / batch / observer-window 完成合同，但不能单靠这些聚合值把某个待选 A/B head 和对应完成因果接起来。** 当前源码封存不等于历史已构建 Linux、tool 或当前加载 Image 的字节验收；本席未获得这样的历史 seal，因此 old run 与 current source 的精确同一性为 unknown。

## 可导航事实与边界

| 判别题 | 当前源码可证明 | 不能由该事实推出 | 原文入口 |
|---|---|---|---|
| 入口与 active owner | tool 一次 composition ioctl；V实例0/DID10，A为none或实例1/DID11；仅两条 `streams[2]` | 本机 live FPGA身份、其他路径是否另有活跃源，或实例2/3在此入口活跃 | E01 / E03 / E10 |
| 每个 copy job 长度 | V/A `chunk_bytes=4096`，`PAGE_SIZE=SZ_4K` 必须成立；V一页，A的allocation/ring为1/8/16页 | 每个 AXI AR/AW burst 的 AxLEN、拍数或同 DDR debt长度；A的16页不是单job65536B | E01 / E03 / E04 / E07 |
| 首 descriptor 页 | allocation 与映射确定后，batch_configure 将双方next_offset置0；配置写 `src_iova+offset` / `dst_iova+offset`、4096B、conf0 | 实际数值IOVA/PA或物理排布；后续自主测量job页次须由 BatchPort RTL证明 | E04 / E05 / E07 |
| 软件 warmup 页次 | submit_commit 使offset按4096B前进，等于size时回0；V一页反复page0；A若继续submit则按映射offset环行 | A warmup恰好W次、每页都热、测量两首头已就绪、translation或DDR留存状态 | E04 / E05 |
| 正常启动顺序 | warmup W>0先submit A再run V；测量先A trigger后V trigger；两组配置都先于任何测量 trigger | 同时发出首AR/AW、A首读一定MIG先行、首AW相对首AR的次序 | E05 / E06 |
| 测量job数与终界 | V finite target=N∈[1,4096]；A target=2^32，V观察完成后stop A；stop不取消已经发出的AXI | A恰好N、A/V严格锁步、A最后一job属于V冻结窗口内部 | E02 / E07 / E08 |
| 完成与在途 | 当前driver以DONE==transfer_id清软件in_flight；drain后读issued/completed，等量且V.completed==N；验证源/目的字节 | 软件in_flight等于硬件AR/AW库存，单个named head在首选择处的identity或其独有完成收益 | E02 / E08 / E10 |
| quiet与恢复 | config前读raw word5==1；退出恢复先等待它为1，随后mode0/DID10配置及generation ack；不强清未完成DMA/response | word5在硬件中具体覆盖哪些库存/CDC、何时清空全部DDR历史；这些属root RTL问题 | E09 |
| 历史来源 | 本次现存源码 SHA 与行号可重核 | 当前driver/tool SHA等于过去Image或binary，过去日志的每个job来自本次读取版本，live加载身份 | snapshot / provenance gap |

## 现存正常前缀：参数化事件顺序，不制造周期表

合法 CLI 必须给出 mode、a-pages、v-gap、transfers=N、warmup=W；tool只有device与timeout默认值。source-only plan常量另有short N/W=16/16与noise/explore/default N/W=4096/256，它们只是已有源常量。root后续明确固定唯一既有source条件 `short-mn-a1`：mode=1(N)、a_pages=1、v_gap=0、N=16、W=16。新输入仅授权定向核该short条目定义，本席未运行计划生成/runner、未展开其他case/phase或改变时间参数。见 E01、E11、E13。

1. composition/P4M输入校验 → 持experiment lock → 取得可选实例1 → observer/batch/gap preflight → 请求mode/DID配置并ack → 分别分配、初始化、映射全部source/destination缓冲 → acquire HPM → CPU flush。config发生在warmup之前。映射建立不等于IOMMU缓存或DDR服务提前到位。
2. W=0时，batch分支不提交warmup A，`idma_run_window(count=0)`立即返回，不制造warming job。W>0且A存在时，先普通submit A，再普通submit V；循环先poll/refill V，再poll/refill A。V第W次完成被观察时立即返回，不再poll/refill A。A可能还在途，随后drain。成功warmup终界V已经无软件in_flight；A drain失败走保留缓冲/quarantine。
3. `idma_batch_configure(v,N)`和可选`idma_batch_configure(a,2^32)`分别program count/pages/enable，并都把next_offset置0。program已有V gap（0/1000/4000）与A gap0；记录软件in_flight起点、arm observer、snapshot HPM。清warmup软件offset不是翻译缓存清空证明。
4. 配置A与V首copy descriptor（各offset0 / 4096B / conf0），再A NEXT_ID trigger read，再V NEXT_ID trigger read；正常路径不在两trigger之间等待DMA completion。software ktime戳只是读调用括号，不能转换成首AXI issue戳。
5. `idma_batch_window`只poll V 的DONE返回group/transfer_id。观察到V完成后冻结的hardware window按接口已终结；driver记录V时长/软件in_flight终点，stop A，拍HPM after。CPU观察/stop latency期间A可能已推进，source不提供其数字。
6. STOP双方、drain双方、读硬件issued/completed、要求issued==completed与V.completed==N；保留/quarantine的lane不能释放。若无error，CPU flush后验证双方所有source及touched/untouched destination字节。observer/P4M完成检查、disable batch、capture128 raw words、quiet后restore mode0/DID10，然后释放映射/缓冲。composition传入`pair=NULL`，不会进入同allocation第二round；tool也只有一次ioctl。

这些步骤证明已有合法软件来源；步骤间CPU开销、AXI时序、翻译miss/hit、首VALID间距、MIG接受与前序读债务仍须root用同一reference RTL与独立原始事实裁决。不得在这里添加额外DMA、并行负载、额外warmup或人为相位。

## 完成证据怎样关联、怎样不能关联

- driver把输出分为victim/aggressor，返回各DID；observer arm把V/A实例写入命令，sequence将本次software window的日志/结果串起来。P4M合同核V exact N，以及A opening/closing stock、已退役ID区间、carry守恒；这是窗口与job组的身份/完成边界。
- 软件 `in_flight` 只有每stream一个bool；NEXT_ID返回值被存成transfer_id，DONE相等才清。自主batch的issued/completed另由硬件计数收集；它不是每个AR/AW接受时刻的逐项journal。A在V window外的startup与final drain成本进入A batch stats，UAPI明确如此。
- 每次重复copy采用相同实例/offset pattern，验证值不编码“第几个job”或“某个首选head”。warmup已写过的destination也可能已有相同内容。因此data verification与计数完成应共同使用；data pattern单独不能证明指定测量head的执行或首选择因果。
- observer/P4M检查与本文读到的日志是窗口聚合和ID区间合同。composition另完整复制128 raw words，其中64..127由本席软件检查不解码；不能依据“不解码”断言reference RTL没有额外证据。若root要映射一个具体A/B choice head，必须检验对应RTL信号/记录的owner、channel、稳定VALID承诺、许可握手与complete归属；该问题本席未代做。
- 本席没有解析任何旧record，所以没有“某旧完成样本已满足这些源码check”的新断言，也没有新的完成样本。即使旧record聚合计数合法，它与当前source/hardware的一致性及named head的映射仍须独立seal与证据。

## 软件来源与硬件引用的限制

本轮 `source-snapshot.json` 封存的是现在可读的软件文件字节和唯一允许的硬件reference文件字节。没有历史tool binary、Linux Image、构建命令/源码seal或live load测量，因此**historical byte seal未建立**。不以“同ABI”“代码看似相同”或旧日志时间戳补齐这个缺口。

唯一硬件引用E12原文是 `BUFFER_DEPTH = (IOMMUNonblockMode != 0 && i < 2 ? 512 : 3)`，不是无条件512。若该reference参数启用nonblock，则软件入口活跃的0/1同为512；2/3为3但此入口不启动它们。LAUNCH_PORT条件 `InclIOMMU && i<2` 也只是端口存在；composition软件走NEXT_ID普通batch触发，不调用E3 controlled PREP/GO路径。引用不是live FPGA identity验收，也不重新开启任何关闭的R107/C112/R1/R2分支。

## Root 新输入冻结的唯一具体入口与清理顺序

- 原plan确包含 `short-mn-a1`；`MODE_N=1` / `MODE_NAMES[1]=n` (24-29)，`A_PAGE_CHOICES`含1、`SHORT_TRANSFERS/WARMUP=16/16` (34-38)，short循环逐mode/逐A pages生成gap0条目 (146-150)，field map为`_make_case` (125-140)，short ID由116-119给出。按这些静态循环它是short第6项，N的a0 solo先于该a1双源；未调用build_plan或读取任何case结果。
- 唯一具体条件是mode1、A1页、V1页、gap0、N16、W16；其他值仅作为入口合同范围被记录，不是新的选择或矩阵。这不是新试验。
- 此条件的源码调用顺序为 **mode/config/clear ack → allocation/map/init → HPM acquire与CPU flush → warmup → A drain → batch count/pages/enable → gap readback → observer arm与snapshot → A trigger → V trigger**。原文见driver2436-2507、2519-2559、2579-2592（E05/E07/E09）；warmup不是先于mode配置。
- driver在warmup成功后到首测量trigger之间没有显式调用IoTLB/DDTC invalidation；它写batch/gap/observer寄存器。是否这些写操作在reference RTL产生缓存清理必须由root检查，本席不把“无显式软件invalidate”冒充硬件缓存保留证明。CPU flush也不是IoTLB/DDTC命中证明。
- N16/W16使V恰有16次已观察warmup完成；A1页至少先提交一个warmup copy，再被轮询或最终drain，但A总次数未固定。这形成真实且有成本的四个source/destination映射页访问前缀；四页的translation缓存最后留存、DDTC状态、DDR debt与测量首head时刻仍unknown，不能宣称无PTE或free warm cache。

## 原文证据

以下摘录逐行直接来自snapshot的文件字节。完整SHA与全部摘录范围保存在 `source-snapshot.json`；范围之外没有暗含历史trace或硬件观测。

### E01 — CLI boundary and its single ioctl

文件：`/home/xvhaocheng/work/iommu-platform/bench/idma_composition.c`。SHA256：`9d4392064a9e623e872c506e5967ce7f8b5d45b18b3c804c12a3941e112e7aad`。

`bench/idma_composition.c:3-15`

```text
    3  * idma_composition.c - one bounded same-BIT composition trial.
    4  *
    5  * Issues exactly one IDMA_IOC_RUN_COMPOSITION ioctl and prints one single-line
    6  * JSON record (schema idma-composition-v1).  It never re-runs a case, never
    7  * opens /dev/mem and never touches the research2 role interface.  A failure
    8  * still prints the JSON record and exits non-zero.
    9  *
   10  * Frozen composition ABI: four logical modes (0=S 1=N 2=P 3=PN), victim pages
   11  * 1, aggressor none or DMA instance 1 with 1/8/16 pages, 4096-byte chunks,
   12  * victim gap 0/1000/4000 cycles, aggressor gap 0.  protected_did is fixed to 10
   13  * and no other value can be requested from this tool.  Old HPM fields are
   14  * carried as raw observations only; they are not validated banked-mode metrics.
   15  */
```

`bench/idma_composition.c:48-54`

```text
   48 #define PROTECTED_DID       10u
   49 #define AGGRESSOR_INSTANCE  1u
   50 #define DEFAULT_CHUNK_BYTES 4096u
   51 #define DEFAULT_TIMEOUT_MS  15000u
   52 #define MIN_TRANSFERS       1u
   53 #define MAX_TRANSFERS       4096u
   54 #define MAX_WARMUP          4096u
```

`bench/idma_composition.c:70-81`

```text
   70 	puts("Usage: idma_composition [options]\n"
   71 	     "  --device PATH       Victim device (default /dev/idma0)\n"
   72 	     "  --mode N            Logical mode 0=S 1=N 2=P 3=PN (required)\n"
   73 	     "  --a-pages N         Aggressor pages 0/1/8/16; 0 means Solo (required)\n"
   74 	     "  --v-gap N           Victim gap cycles 0/1000/4000 (required)\n"
   75 	     "  --transfers N       Victim measured transfers 1..4096 (required)\n"
   76 	     "  --warmup N          Warmup transfers 0..4096 (required)\n"
   77 	     "  --timeout-ms N      Run timeout (default 15000)\n"
   78 	     "  --abi               Print the tool ABI as one JSON line, no hardware\n"
   79 	     "  --help              Show this help\n"
   80 	     "protected_did is fixed to 10; the aggressor is DMA instance 1 with DID 11.\n"
   81 	     "One JSON record per run; a failure still prints the record and exits non-zero.");
```

`bench/idma_composition.c:179-205`

```text
  179 	if (!(seen & (1u << OPT_MODE)) || !(seen & (1u << OPT_A_PAGES)) ||
  180 	    !(seen & (1u << OPT_V_GAP)) || !(seen & (1u << OPT_TRANSFERS)) ||
  181 	    !(seen & (1u << OPT_WARMUP))) {
  182 		snprintf(err, size,
  183 			 "--mode, --a-pages, --v-gap, --transfers and --warmup are required");
  184 		return -1;
  185 	}
  186 	if (o->mode > 3) {
  187 		snprintf(err, size, "--mode must be 0..3");
  188 		return -1;
  189 	}
  190 	if (!(o->a_pages == 0 || o->a_pages == 1 || o->a_pages == 8 || o->a_pages == 16)) {
  191 		snprintf(err, size, "--a-pages must be 0, 1, 8 or 16");
  192 		return -1;
  193 	}
  194 	if (!(o->v_gap == 0 || o->v_gap == 1000 || o->v_gap == 4000)) {
  195 		snprintf(err, size, "--v-gap must be 0, 1000 or 4000");
  196 		return -1;
  197 	}
  198 	if (o->transfers < MIN_TRANSFERS || o->transfers > MAX_TRANSFERS) {
  199 		snprintf(err, size, "--transfers must be 1..%u", (unsigned int)MAX_TRANSFERS);
  200 		return -1;
  201 	}
  202 	if (o->warmup > MAX_WARMUP) {
  203 		snprintf(err, size, "--warmup must be 0..%u", (unsigned int)MAX_WARMUP);
  204 		return -1;
  205 	}
```

`bench/idma_composition.c:393-415`

```text
  393 static void build_request(const struct options *o, struct idma_composition *r)
  394 {
  395 	memset(r, 0, sizeof(*r));
  396 	r->abi_version = IDMA_COMPOSITION_ABI;
  397 	r->struct_size = (__u32)sizeof(*r);
  398 	r->mode = o->mode;
  399 	r->protected_did = PROTECTED_DID;
  400 	r->victim_gap_cycles = o->v_gap;
  401 
  402 	r->p4m.experiment.abi_version = IDMA_EXPERIMENT_ABI;
  403 	r->p4m.experiment.struct_size = (__u32)sizeof(struct idma_experiment);
  404 	r->p4m.experiment.flags = 0;
  405 	r->p4m.experiment.aggressor_id = o->a_pages ? AGGRESSOR_INSTANCE
  406 						    : IDMA_EXPERIMENT_NO_AGGRESSOR;
  407 	r->p4m.experiment.victim_pages = 1;
  408 	/* Solo keeps a nonzero page count; the kernel ignores it without an aggressor. */
  409 	r->p4m.experiment.aggressor_pages = o->a_pages ? o->a_pages : 1;
  410 	r->p4m.experiment.chunk_bytes = DEFAULT_CHUNK_BYTES;
  411 	r->p4m.experiment.run_timeout_ms = o->timeout_ms;
  412 	r->p4m.experiment.victim_transfers = o->transfers;
  413 	r->p4m.experiment.warmup_transfers = o->warmup;
  414 	r->p4m.aggressor_gap_cycles = 0;
  415 }
```

`bench/idma_composition.c:471-499`

```text
  471 	memset(&o, 0, sizeof(o));
  472 	memset(&req, 0, sizeof(req));
  473 	memset(&result, 0, sizeof(result));
  474 	o.device = "/dev/idma0";
  475 	o.timeout_ms = DEFAULT_TIMEOUT_MS;
  476 
  477 	rc = parse_args(argc, argv, &o, err, sizeof(err));
  478 	if (rc == 1) {
  479 		usage();
  480 		return EXIT_SUCCESS;
  481 	}
  482 	if (rc == 2) {
  483 		print_abi();
  484 		return EXIT_SUCCESS;
  485 	}
  486 	if (rc < 0)
  487 		goto done;
  488 
  489 	build_request(&o, &req);
  490 
  491 	stage = "open";
  492 	fd = open(o.device, O_RDWR | O_CLOEXEC);
  493 	if (fd < 0)
  494 		goto syscall_error;
  495 
  496 	stage = "composition";
  497 	attempted = 1;
  498 	result = req;
  499 	rc = ioctl(fd, IDMA_IOC_RUN_COMPOSITION, &result);
```

### E02 — ABI: job counters versus software in_flight

文件：`/home/xvhaocheng/work/iommu-platform/linux/include/uapi/linux/idma.h`。SHA256：`190d8250ff18f1ce197d7d67ee1d9e9e384e917e2a17257ecb0b4ecc2d1d3774`。

`linux/include/uapi/linux/idma.h:8-10`

```text
    8 #define IDMA_EXPERIMENT_ABI 1U
    9 #define IDMA_EXPERIMENT_MAX_TRANSFERS (1ULL << 32)
   10 #define IDMA_EXPERIMENT_NO_AGGRESSOR ((__u32)~0U)
```

`linux/include/uapi/linux/idma.h:35-51`

```text
   35 struct idma_experiment_stats {
   36 	__u64 submitted;
   37 	__u64 completed;
   38 	__u64 completed_bytes;
   39 	__u64 active_cycles;
   40 	__u64 iotlb_miss;
   41 	__u64 s1_ptw;
   42 	__u64 s2_ptw;
   43 	/* Observed completion to observed resubmission, incl. MMIO; not HW idle. */
   44 	__u64 refill_gap_ns_sum;
   45 	__u64 refill_gap_ns_max;
   46 	__u64 refill_gap_samples;
   47 	/* Submitted but not yet observed complete in software; not HW overlap. */
   48 	__u32 in_flight_at_start;
   49 	__u32 in_flight_at_end;
   50 	__u32 data_errors;
   51 	__u32 quarantined;
```

`linux/include/uapi/linux/idma.h:54-80`

```text
   54 struct idma_experiment {
   55 	/* Input: all reserved words and flags must be zero in ABI 1. */
   56 	__u32 abi_version;
   57 	__u32 struct_size;
   58 	__u32 flags;
   59 	__u32 aggressor_id;
   60 	__u32 victim_pages;
   61 	__u32 aggressor_pages;
   62 	__u32 chunk_bytes;
   63 	__u32 run_timeout_ms;
   64 	__u64 victim_transfers;
   65 	__u64 warmup_transfers;
   66 	__u32 reserved[8];
   67 	/* Output: status is a negative errno or zero, even when ioctl fails. */
   68 	__s32 status;
   69 	__u32 victim_did;
   70 	__u32 aggressor_did;
   71 	__u32 hpm_configured;
   72 	__u32 measurement_started;
   73 	__u32 measurement_complete;
   74 	__u64 victim_elapsed_ns;
   75 	/* Snapshot intervals bracket V's window; HPM is not an atomic snapshot. */
   76 	__u64 snapshot_before_ns;
   77 	__u64 snapshot_after_ns;
   78 	__u64 drain_ns;
   79 	struct idma_experiment_stats victim;
   80 	struct idma_experiment_stats aggressor;
```

`linux/include/uapi/linux/idma.h:87-91`

```text
   87 /* Separate operation/JSON semantics, same validated input layout; reserved=0.
   88  * Batch stats count hardware tasks; refill gaps are not sampled. A counts include
   89  * its startup and final drain, rather than just the V hardware window.
   90  */
   91 #define IDMA_IOC_RUN_BATCH _IOWR('i', 0x08, struct idma_experiment)
```

`linux/include/uapi/linux/idma.h:256-273`

```text
  256 /* Same-BIT composition experiment. This is independent of RESEARCH2 roles.
  257  * Inputs: first eight u32s and the existing p4m inputs. Outputs start at status.
  258  * Four logical modes: 0 S, 1 N, 2 P, 3 PN. P reserves entries 0..1 for DID.
  259  * Old HPM fields remain raw observations, not validated banked-mode metrics.
  260  */
  261 #define IDMA_COMPOSITION_ABI 1U
  262 #define IDMA_COMPOSITION_WORDS 128U
  263 #define IDMA_COMPOSITION_MAGIC 0x49444d4143504e01ULL
  264 struct idma_composition {
  265 	__u32 abi_version, struct_size, mode, protected_did;
  266 	__u32 victim_gap_cycles, reserved[3];
  267 	struct idma_p4m p4m;
  268 	__s32 status, restore_status;
  269 	__u32 configured, raw_captured;
  270 	__u64 generation, restored_generation;
  271 	__u64 raw[IDMA_COMPOSITION_WORDS];
  272 };
  273 #define IDMA_IOC_RUN_COMPOSITION _IOWR('i', 0x11, struct idma_composition)
```

### E03 — Composition/P4M validation confines this entry to software instances 0 and optional 1

文件：`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c`。SHA256：`e5e954c62e07d43bdd40d721eb10a176696ca179d4a565d1feeb5b13073efd47`。

`linux/drivers/misc/idma.c:151-178`

```text
  151 static DEFINE_MUTEX(idma_devices_lock);
  152 static struct idma_device *idma_devices[IDMA_MAX_DEVS];
  153 /* All experiment preparation, IOVA mappings and DMA execution are serialized. */
  154 static DEFINE_MUTEX(idma_experiment_lock);
  155 
  156 static int idma_reserve_instance_id(void)
  157 {
  158 	int id;
  159 
  160 	mutex_lock(&idma_devices_lock);
  161 	for (id = 0; id < IDMA_MAX_DEVS; id++) {
  162 		if (!idma_devices[id]) {
  163 			idma_devices[id] = ERR_PTR(-EBUSY);
  164 			mutex_unlock(&idma_devices_lock);
  165 			return id;
  166 		}
  167 	}
  168 	mutex_unlock(&idma_devices_lock);
  169 
  170 	return -ENOSPC;
  171 }
  172 
  173 static void idma_publish_instance(struct idma_device *idma_dev)
  174 {
  175 	mutex_lock(&idma_devices_lock);
  176 	if (idma_dev->id >= 0 && idma_dev->id < IDMA_MAX_DEVS)
  177 		idma_devices[idma_dev->id] = idma_dev;
  178 	mutex_unlock(&idma_devices_lock);
```

`linux/drivers/misc/idma.c:191-206`

```text
  191 static struct idma_device *idma_get_instance(u32 id)
  192 {
  193 	struct idma_device *idma_dev;
  194 
  195 	if (id >= IDMA_MAX_DEVS)
  196 		return NULL;
  197 
  198 	mutex_lock(&idma_devices_lock);
  199 	idma_dev = idma_devices[id];
  200 	if (IS_ERR(idma_dev))
  201 		idma_dev = NULL;
  202 	if (idma_dev)
  203 		get_device(idma_dev->miscdev.parent);
  204 	mutex_unlock(&idma_devices_lock);
  205 
  206 	return idma_dev;
```

`linux/drivers/misc/idma.c:303-317`

```text
  303 static int idma_read_did(struct platform_device *pdev, u32 *did)
  304 {
  305 	struct of_phandle_args args;
  306 	int ret;
  307 
  308 	ret = of_parse_phandle_with_args(pdev->dev.of_node, "iommus",
  309 					 "#iommu-cells", 0, &args);
  310 	if (ret)
  311 		return ret;
  312 	if (args.args_count != 1 || args.args[0] >= BIT(24))
  313 		ret = -EINVAL;
  314 	else
  315 		*did = args.args[0];
  316 	of_node_put(args.np);
  317 	return ret;
```

`linux/drivers/misc/idma.c:1313-1338`

```text
 1313 static int idma_p4m_validate(struct idma_device *victim,
 1314 			     const struct idma_p4m *p)
 1315 {
 1316 	const struct idma_experiment *e = &p->experiment;
 1317 	bool has_a;
 1318 
 1319 	if (victim->id != 0 || victim->did != 10)
 1320 		return -EINVAL;
 1321 	if (memchr_inv(p->reserved, 0, sizeof(p->reserved)))
 1322 		return -EINVAL;
 1323 	if (e->abi_version != IDMA_EXPERIMENT_ABI || e->struct_size != sizeof(*e))
 1324 		return -EPROTO;
 1325 	if (e->flags || memchr_inv(e->reserved, 0, sizeof(e->reserved)))
 1326 		return -EINVAL;
 1327 	has_a = e->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;
 1328 	if (has_a && e->aggressor_id != 1)
 1329 		return -EINVAL;
 1330 	if (!has_a && p->aggressor_gap_cycles)
 1331 		return -EINVAL;
 1332 	if (e->chunk_bytes != PAGE_SIZE)
 1333 		return -EINVAL;
 1334 	if (!e->victim_transfers || e->victim_transfers > IDMA_BATCH_MAX_TRANSFERS)
 1335 		return -EINVAL;
 1336 	if (e->warmup_transfers > IDMA_EXPERIMENT_MAX_TRANSFERS)
 1337 		return -EINVAL;
 1338 	return 0;
```

`linux/drivers/misc/idma.c:2780-2816`

```text
 2780 static long idma_run_composition(struct idma_device *victim, void __user *uarg)
 2781 {
 2782 	struct idma_composition *p;
 2783 	struct idma_experiment *e;
 2784 	long ret;
 2785 
 2786 	if (!capable(CAP_SYS_RAWIO))
 2787 		return -EPERM;
 2788 	p = memdup_user(uarg, sizeof(*p));
 2789 	if (IS_ERR(p))
 2790 		return PTR_ERR(p);
 2791 	memset(&p->status, 0, sizeof(*p) - offsetof(struct idma_composition, status));
 2792 	idma_p4m_clear_outputs(&p->p4m);
 2793 	e = &p->p4m.experiment;
 2794 	ret = -EPROTO;
 2795 	if (p->abi_version != IDMA_COMPOSITION_ABI || p->struct_size != sizeof(*p))
 2796 		goto out;
 2797 	ret = -EINVAL;
 2798 	if (p->mode > 3 || (p->protected_did != 10 && p->protected_did != 11) ||
 2799 	    (p->victim_gap_cycles != 0 && p->victim_gap_cycles != 1000 &&
 2800 	     p->victim_gap_cycles != 4000) ||
 2801 	    memchr_inv(p->reserved, 0, sizeof(p->reserved)) ||
 2802 	    p->p4m.aggressor_gap_cycles || e->victim_pages != 1 ||
 2803 	    (e->aggressor_pages != 1 && e->aggressor_pages != 8 && e->aggressor_pages != 16) ||
 2804 	    e->victim_transfers > 4096 || e->warmup_transfers > 4096)
 2805 		goto out;
 2806 	ret = idma_p4m_validate(victim, &p->p4m);
 2807 	if (!ret)
 2808 		ret = idma_run_session2(victim, e, true, NULL, NULL, false,
 2809 				       &p->p4m, NULL, NULL, p);
 2810  out:
 2811 	p->status = ret;
 2812 	e->status = ret;
 2813 	if (copy_to_user(uarg, p, sizeof(*p)))
 2814 		ret = -EFAULT;
 2815 	kfree(p);
 2816 	return ret;
```

### E04 — Mapped buffer construction; first descriptor and software ring bookkeeping

文件：`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c`。SHA256：`e5e954c62e07d43bdd40d721eb10a176696ca179d4a565d1feeb5b13073efd47`。

`linux/drivers/misc/idma.c:354-398`

```text
  354 static int idma_stream_prepare(struct idma_stream *s, struct idma_device *idma,
  355 			       u32 pages, u32 chunk,
  356 			       struct idma_experiment_stats *stats)
  357 {
  358 	struct page *src_page, *dst_page;
  359 	size_t i;
  360 	int ret;
  361 
  362 	s->idma = idma;
  363 	s->dev = idma->miscdev.parent;
  364 	s->stats = stats;
  365 	s->domain = iommu_get_domain_for_dev(s->dev);
  366 	if (!s->domain || s->domain->type == IOMMU_DOMAIN_IDENTITY)
  367 		return -EOPNOTSUPP;
  368 	s->size = (size_t)pages * PAGE_SIZE;
  369 	s->chunk = chunk;
  370 	s->src = dma_alloc_attrs(s->dev, s->size, &s->src_dma, GFP_KERNEL,
  371 				 DMA_ATTR_FORCE_CONTIGUOUS);
  372 	if (!s->src)
  373 		return -ENOMEM;
  374 	s->dst = dma_alloc_attrs(s->dev, s->size, &s->dst_dma, GFP_KERNEL,
  375 				 DMA_ATTR_FORCE_CONTIGUOUS);
  376 	if (!s->dst)
  377 		return -ENOMEM;
  378 	for (i = 0; i < s->size / sizeof(u64); i++) {
  379 		((u64 *)s->src)[i] = idma_pattern(s, i);
  380 		((u64 *)s->dst)[i] = ~idma_pattern(s, i);
  381 	}
  382 	src_page = idma_buffer_page(s->src);
  383 	dst_page = idma_buffer_page(s->dst);
  384 	if (!src_page || !dst_page)
  385 		return -EFAULT;
  386 	ret = idma_find_iova(s->domain, s->size, &s->src_iova, &s->dst_iova);
  387 	if (ret)
  388 		return ret;
  389 	ret = idma_iommu_map_range(s->domain, s->src_iova,
  390 				   page_to_phys(src_page), s->size, PAGE_SIZE);
  391 	if (ret)
  392 		return ret;
  393 	s->mapped_src = true;
  394 	ret = idma_iommu_map_range(s->domain, s->dst_iova,
  395 				   page_to_phys(dst_page), s->size, PAGE_SIZE);
  396 	if (ret)
  397 		return ret;
  398 	s->mapped_dst = true;
```

`linux/drivers/misc/idma.c:411-419`

```text
  411 static void idma_submit_config(struct idma_stream *s)
  412 {
  413 	if (s->diagnostic && s->batch_started)
  414 		s->diagnostic->launch_begin_ns = ktime_get_ns();
  415 	s->offset = s->next_offset;
  416 	writeq(s->src_iova + s->offset, s->idma->reg + IDMA_SRC_ADDR_REG_OFFSET);
  417 	writeq(s->dst_iova + s->offset, s->idma->reg + IDMA_DST_ADDR_REG_OFFSET);
  418 	writeq(s->chunk, s->idma->reg + IDMA_NUM_BYTES_REG_OFFSET);
  419 	writeq(0, s->idma->reg + IDMA_CONF_REG_OFFSET);
```

`linux/drivers/misc/idma.c:422-467`

```text
  422 static void idma_submit_commit(struct idma_stream *s, u64 id, bool measured, u64 now)
  423 {
  424 	s->transfer_id = id;
  425 	if (s->diagnostic) {
  426 		if (s->batch_started)
  427 			s->diagnostic->launch_end_ns = now;
  428 		else
  429 			s->diagnostic->warm_submitted++;
  430 	}
  431 	if (s->last_complete_ns && measured) {
  432 		u64 gap = now - s->last_complete_ns;
  433 
  434 		s->stats->refill_gap_ns_sum += gap;
  435 		s->stats->refill_gap_ns_max = max(s->stats->refill_gap_ns_max, gap);
  436 		s->stats->refill_gap_samples++;
  437 	}
  438 	s->last_complete_ns = 0;
  439 	s->deadline_ns = now + (u64)IDMA_XFER_TIMEOUT_US * NSEC_PER_USEC;
  440 	s->in_flight = true;
  441 	s->next_offset += s->chunk;
  442 	if (s->next_offset == s->size)
  443 		s->next_offset = 0;
  444 	if (measured)
  445 		s->stats->submitted++;
  446 }
  447 
  448 static int idma_submit(struct idma_stream *s, bool measured)
  449 {
  450 	u64 now, id;
  451 
  452 	idma_submit_config(s);
  453 	id = readq(s->idma->reg + IDMA_NEXT_ID_REG_OFFSET);
  454 	if (!id) {
  455 		/*
  456 		 * Without a valid ID we cannot prove that buffers are no longer
  457 		 * used.  A failed submit must not keep the previous transfer ID,
  458 		 * because the drain path matches DONE against transfer_id.
  459 		 */
  460 		s->transfer_id = 0;
  461 		idma_quarantine(s);
  462 		return -EIO;
  463 	}
  464 	/* Include configuration writes and the NEXT_ID trigger in refill cost. */
  465 	now = ktime_get_ns();
  466 	idma_submit_commit(s, id, measured, now);
  467 	return 0;
```

### E05 — Warmup is V-counted, A-prefixed, drained; measured bases reset

文件：`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c`。SHA256：`e5e954c62e07d43bdd40d721eb10a176696ca179d4a565d1feeb5b13073efd47`。

`linux/drivers/misc/idma.c:690-749`

```text
  690 static int idma_run_window(struct idma_stream *v, struct idma_stream *a,
  691 			   u64 count, u64 deadline, bool measured, u64 *end_ns)
  692 {
  693 	u64 completed = 0;
  694 	u32 spins = 0;
  695 	int ret;
  696 
  697 	if (!count)
  698 		return 0;
  699 	if (ktime_get_ns() >= deadline) {
  700 		ret = -ETIMEDOUT;
  701 		goto out;
  702 	}
  703 	if (signal_pending(current)) {
  704 		ret = -EINTR;
  705 		goto out;
  706 	}
  707 	ret = idma_submit(v, measured);
  708 	if (ret)
  709 		goto out;
  710 	for (;;) {
  711 		if (ktime_get_ns() >= deadline) {
  712 			ret = -ETIMEDOUT;
  713 			break;
  714 		}
  715 		if (signal_pending(current)) {
  716 			ret = -EINTR;
  717 			break;
  718 		}
  719 		ret = idma_poll(v, measured);
  720 		if (ret < 0)
  721 			break;
  722 		if (ret == 1) {
  723 			if (++completed == count) {
  724 				/* Capture V's end before polling/refilling A or taking snapshots. */
  725 				*end_ns = v->last_complete_ns;
  726 				return 0;
  727 			}
  728 			ret = idma_submit(v, measured);
  729 			if (ret)
  730 				break;
  731 		}
  732 		if (a) {
  733 			ret = idma_poll(a, measured);
  734 			if (ret < 0)
  735 				break;
  736 			if (ret == 1) {
  737 				ret = idma_submit(a, measured);
  738 				if (ret)
  739 					break;
  740 			}
  741 		}
  742 		if (!(++spins & 1023))
  743 			cond_resched();
  744 		cpu_relax();
  745 	}
  746  out:
  747 	*end_ns = ktime_get_ns();
  748 	return ret;
  749 }
```

`linux/drivers/misc/idma.c:2490-2507`

```text
 2490 	/*
 2491 	 * Platform workaround: this CVA6 has no DMA snoop of the WT data cache.
 2492 	 * Its FENCE.I drains CPU writes and invalidates D-cache as well as I-cache.
 2493 	 * Hand initialized buffers to DMA before warmup; never flush per transfer.
 2494 	 */
 2495 	local_flush_icache_all();
 2496 	deadline = ktime_get_ns() + (u64)e.run_timeout_ms * NSEC_PER_MSEC;
 2497 	if (diag) {
 2498 		idma_snapshot(hpm, v, a, &before, active_before);
 2499 		diag->warm_begin_ns = ktime_get_ns();
 2500 	}
 2501 	idma_comp_stage(v->composition_trace, v->dev, "warm-start", 0);
 2502 	if (a && (!batch || e.warmup_transfers)) {
 2503 		ret = idma_submit(a, false);
 2504 		if (ret)
 2505 			goto out_drain;
 2506 	}
 2507 	ret = idma_run_window(v, a, e.warmup_transfers, deadline, false, &end);
```

`linux/drivers/misc/idma.c:2519-2559`

```text
 2519 	idma_comp_stage(v->composition_trace, v->dev, "warm-done", ret);
 2520 	if (ret)
 2521 		goto out_drain;
 2522 	/* Every measured V task starts at the same address after warmup. */
 2523 	v->next_offset = 0;
 2524 	v->last_complete_ns = 0;
 2525 	if (a)
 2526 		a->last_complete_ns = 0;
 2527 	if (batch) {
 2528 		/* Software warmup is drained before enabling the autonomous mode. */
 2529 		if (a) {
 2530 			ret = idma_drain(a);
 2531 			if (ret)
 2532 				goto out_drain;
 2533 		}
 2534 		if (diag) {
 2535 			diag->warm_drained_ns = ktime_get_ns();
 2536 			idma_snapshot(hpm, v, a, &after, active_after);
 2537 			idma_snapshot_delta(&diag->lane[0].warm_hpm, 0, &before, &after,
 2538 					    active_before[0], active_after[0]);
 2539 			if (a)
 2540 				idma_snapshot_delta(&diag->lane[1].warm_hpm, 1, &before, &after,
 2541 						    active_before[1], active_after[1]);
 2542 			diag->warm_complete = 1;
 2543 		}
 2544 		ret = idma_batch_configure(v, e.victim_transfers);
 2545 		if (!ret && a)
 2546 			ret = idma_batch_configure(a, IDMA_EXPERIMENT_MAX_TRANSFERS);
 2547 		idma_comp_stage(v->composition_trace, v->dev,
 2548 				"batch-config-done", ret);
 2549 		if (ret)
 2550 			goto out_drain;
 2551 	}
 2552 	if (p4m) {
 2553 		ret = idma_gap_program_readback(v, composition ? composition->victim_gap_cycles : 0);
 2554 		if (!ret && a)
 2555 			ret = idma_gap_program_readback(a,
 2556 							p4m->aggressor_gap_cycles);
 2557 		idma_comp_stage(v->composition_trace, v->dev, "gap-done", ret);
 2558 		if (ret)
 2559 			goto out_drain;
```

### E06 — Ordinary software batch start: ordered A NEXT_ID then V NEXT_ID, not hardware simultaneous launch

文件：`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c`。SHA256：`e5e954c62e07d43bdd40d721eb10a176696ca179d4a565d1feeb5b13073efd47`。

`linux/drivers/misc/idma.c:482-503`

```text
  482  * Bounded software batch start.  Both register sets are configured before any
  483  * NEXT_ID trigger.  Only the ordered A-then-V trigger reads and their software
  484  * timestamps run with IRQs and preemption disabled.  That section is
  485  * non-sleeping and never covers commit bookkeeping, polling, drain,
  486  * quarantine, allocation, logging or cond_resched.
  487  *
  488  * The deadline and the pending signal are checked twice: once before the
  489  * configuration writes and again inside the protected section before the first
  490  * trigger.  A request that expires in between is dropped without triggering
  491  * either device, without keeping either untriggered batch_started claim and
  492  * without keeping their launch stamps.
  493  *
  494  * a_triggered/v_triggered mean the corresponding NEXT_ID read was executed,
  495  * even when it returned zero.  a_id/v_id carry the raw readback (zero on
  496  * failure).  irq_span_ns is the ktime bracket around the protected trigger
  497  * reads; the real IRQ-off interval extends a bounded few instructions on both
  498  * sides and is not a cycle-accurate or interrupt-latency measurement.
  499  *
  500  * Failure ownership matches the ordinary batch path: a stream whose trigger
  501  * read produced a valid ID is committed even when the other stream failed, so
  502  * the caller can still stop and drain it; a stream whose trigger read returned
  503  * zero is quarantined, and an untriggered V is released instead.
```

`linux/drivers/misc/idma.c:505-537`

```text
  505 static int idma_batch_start(struct idma_stream *v, struct idma_stream *a,
  506 			    u64 deadline, u64 *end_ns,
  507 			    struct idma_batch_start *start)
  508 {
  509 	unsigned long flags;
  510 	u64 a_id = 0, v_id = 0;
  511 	int ret;
  512 
  513 	if (ktime_get_ns() >= deadline) {
  514 		ret = -ETIMEDOUT;
  515 		goto out;
  516 	}
  517 	if (signal_pending(current)) {
  518 		ret = -EINTR;
  519 		goto out;
  520 	}
  521 
  522 	/*
  523 	 * Ownership and configuration are outside the protected trigger section.
  524 	 * Mark batch_started before the trigger so failure handling keeps the
  525 	 * same STOP/drain retention ownership as the ordinary batch path.
  526 	 */
  527 	if (a) {
  528 		a->batch_started = true;
  529 		idma_submit_config(a);
  530 	}
  531 	v->batch_started = true;
  532 	idma_submit_config(v);
  533 
  534 	start->attempted = true;
  535 	idma_comp_stage(v->composition_trace, v->dev, "pair-enter", 0);
  536 	preempt_disable();
  537 	local_irq_save(flags);
```

`linux/drivers/misc/idma.c:544-607`

```text
  544 	if (ktime_get_ns() >= deadline) {
  545 		ret = -ETIMEDOUT;
  546 		goto out_untriggered;
  547 	}
  548 	if (signal_pending(current)) {
  549 		ret = -EINTR;
  550 		goto out_untriggered;
  551 	}
  552 
  553 	if (a) {
  554 		start->a_begin_ns = ktime_get_ns();
  555 		a_id = readq(a->idma->reg + IDMA_NEXT_ID_REG_OFFSET);
  556 		start->a_end_ns = ktime_get_ns();
  557 		start->a_triggered = true;
  558 		start->a_id = a_id;
  559 		if (!a_id) {
  560 			/*
  561 			 * V was only configured, never triggered: release its
  562 			 * ownership and stamps, then log A's quarantine with
  563 			 * interrupts restored.
  564 			 */
  565 			local_irq_restore(flags);
  566 			preempt_enable();
  567 			v->batch_started = false;
  568 			if (v->diagnostic) {
  569 				v->diagnostic->launch_begin_ns = 0;
  570 				v->diagnostic->launch_end_ns = 0;
  571 			}
  572 			idma_quarantine(a);
  573 			ret = -EIO;
  574 			goto out;
  575 		}
  576 	}
  577 
  578 	start->v_begin_ns = ktime_get_ns();
  579 	v_id = readq(v->idma->reg + IDMA_NEXT_ID_REG_OFFSET);
  580 	start->v_end_ns = ktime_get_ns();
  581 	start->v_triggered = true;
  582 	start->v_id = v_id;
  583 
  584 	/* Commit bookkeeping never runs with IRQs or preemption disabled. */
  585 	local_irq_restore(flags);
  586 	preempt_enable();
  587 	idma_comp_stage(v->composition_trace, v->dev, "pair-return", 0);
  588 
  589 	if (!v_id) {
  590 		/*
  591 		 * A was triggered with a valid ID: publish it before V is
  592 		 * quarantined so the caller can still stop and drain A.
  593 		 */
  594 		if (a)
  595 			idma_submit_commit(a, a_id, false, start->a_end_ns);
  596 		idma_quarantine(v);
  597 		ret = -EIO;
  598 		goto out;
  599 	}
  600 
  601 	if (a)
  602 		idma_submit_commit(a, a_id, false, start->a_end_ns);
  603 	idma_submit_commit(v, v_id, false, start->v_end_ns);
  604 
  605 	start->irq_span_ns = start->v_end_ns -
  606 		(a ? start->a_begin_ns : start->v_begin_ns);
  607 	return 0;
```

`linux/drivers/misc/idma.c:609-624`

```text
  609 out_untriggered:
  610 	/* Nothing was triggered: drop both ownership claims and their stamps. */
  611 	if (a) {
  612 		a->batch_started = false;
  613 		if (a->diagnostic) {
  614 			a->diagnostic->launch_begin_ns = 0;
  615 			a->diagnostic->launch_end_ns = 0;
  616 		}
  617 	}
  618 	v->batch_started = false;
  619 	if (v->diagnostic) {
  620 		v->diagnostic->launch_begin_ns = 0;
  621 		v->diagnostic->launch_end_ns = 0;
  622 	}
  623 	local_irq_restore(flags);
  624 	preempt_enable();
```

`linux/drivers/misc/idma.c:639-664`

```text
  639 /*
  640  * Log contract (version 1), emitted only after the observation finisher and
  641  * all error aggregation.  Timestamps are software ktime brackets around the
  642  * protected trigger reads; they are not hardware issue timestamps, cycle
  643  * accurate or an interrupt-latency proof.  a_triggered/v_triggered mean the
  644  * corresponding NEXT_ID read was attempted; a_id/v_id are the raw readbacks
  645  * and may be zero on failure.  a_to_v_ns is valid only when both A and V
  646  * trigger reads were attempted.  A start that failed after configuration but
  647  * before any trigger reports attempted=1 with both trigger flags zero and
  648  * irq_span_ns=0.
  649  */
  650 static void idma_batch_start_log(struct device *dev,
  651 				 const struct idma_batch_start *start,
  652 				 u64 sequence, int error)
  653 {
  654 	u64 a_to_v_ns = 0;
  655 
  656 	if (start->has_a && start->a_triggered && start->v_triggered)
  657 		a_to_v_ns = start->v_begin_ns - start->a_begin_ns;
  658 	dev_info(dev,
  659 		 "idma_batch_start version=1 seq=%llu method=irq_pair v_did=%u a_did=%u attempted=%u has_a=%u a_triggered=%u v_triggered=%u a_id=%llu v_id=%llu a_begin_ns=%llu a_end_ns=%llu v_begin_ns=%llu v_end_ns=%llu irq_span_ns=%llu a_to_v_ns=%llu error=%d\n",
  660 		 sequence, start->v_did, start->a_did, start->attempted,
  661 		 start->has_a, start->a_triggered, start->v_triggered,
  662 		 start->a_id, start->v_id, start->a_begin_ns, start->a_end_ns,
  663 		 start->v_begin_ns, start->v_end_ns, start->irq_span_ns,
  664 		 a_to_v_ns, error);
```

### E07 — Finite V / large A batch; window completion and A stopping

文件：`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c`。SHA256：`e5e954c62e07d43bdd40d721eb10a176696ca179d4a565d1feeb5b13073efd47`。

`linux/drivers/misc/idma.c:821-880`

```text
  821 /* Batch mode still uses DEFAULT read/write copies. STOP never cancels AXI. */
  822 static int idma_batch_configure(struct idma_stream *s, u64 count)
  823 {
  824 	void __iomem *base = s->idma->reg;
  825 
  826 	if (readq(base + IDMA_BATCH_MAGIC_OFFSET) != IDMA_BATCH_MAGIC)
  827 		return -EOPNOTSUPP;
  828 	if (readq(base + IDMA_BATCH_CONTROL_OFFSET) & 2)
  829 		return -EBUSY;
  830 	writeq(0, base + IDMA_BATCH_CONTROL_OFFSET);
  831 	writeq(count, base + IDMA_BATCH_TARGET_OFFSET);
  832 	writeq(s->size / PAGE_SIZE, base + IDMA_BATCH_PAGES_OFFSET);
  833 	writeq(1, base + IDMA_BATCH_CONTROL_OFFSET);
  834 	s->batch_configured = true;
  835 	s->next_offset = 0;
  836 	return readq(base + IDMA_BATCH_CONTROL_OFFSET) == 1 ? 0 : -EIO;
  837 }
  838 
  839 static void idma_batch_stop(struct idma_stream *s)
  840 {
  841 	if (s && s->batch_configured && s->batch_started)
  842 		writeq(2, s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
  843 }
  844 
  845 static int idma_batch_collect(struct idma_stream *s)
  846 {
  847 	u64 status, issued, completed;
  848 
  849 	if (!s || !s->batch_configured)
  850 		return 0;
  851 	if (s->retain)
  852 		return -EIO;
  853 	status = readq(s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
  854 	if (status & 2) {
  855 		idma_quarantine(s);
  856 		return -EIO;
  857 	}
  858 	if (s->batch_started) {
  859 		issued = readq(s->idma->reg + IDMA_BATCH_ISSUED_OFFSET);
  860 		completed = readq(s->idma->reg + IDMA_BATCH_COMPLETED_OFFSET);
  861 		s->stats->submitted = issued;
  862 		s->stats->completed = completed;
  863 		s->stats->completed_bytes = completed * PAGE_SIZE;
  864 		if (s->reverse)
  865 			s->touched_tail = max(s->touched_tail,
  866 				      (size_t)min(completed, (u64)(s->size / PAGE_SIZE)) * PAGE_SIZE);
  867 		else
  868 			s->touched_end = max(s->touched_end,
  869 				     (size_t)min(completed, (u64)(s->size / PAGE_SIZE)) * PAGE_SIZE);
  870 		if (issued != completed || issued > IDMA_EXPERIMENT_MAX_TRANSFERS)
  871 			return -EIO;
  872 	}
  873 	return status >> 32 ? -EIO : 0;
  874 }
  875 
  876 static void idma_batch_disable(struct idma_stream *s)
  877 {
  878 	if (s && s->batch_configured && !s->retain)
  879 		writeq(0, s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
  880 }
```

`linux/drivers/misc/idma.c:882-915`

```text
  882 static int idma_batch_window(struct idma_stream *v, struct idma_stream *a,
  883 			     u64 deadline, u64 *end_ns,
  884 			     struct idma_batch_start *start)
  885 {
  886 	u32 spins = 0;
  887 	int ret;
  888 
  889 	ret = idma_batch_start(v, a, deadline, end_ns, start);
  890 	if (ret)
  891 		return ret;
  892 	v->deadline_ns = deadline;
  893 	for (;;) {
  894 		if (ktime_get_ns() >= deadline) {
  895 			ret = -ETIMEDOUT;
  896 			break;
  897 		}
  898 		if (signal_pending(current)) {
  899 			ret = -EINTR;
  900 			break;
  901 		}
  902 		ret = idma_poll(v, false);
  903 		if (ret < 0)
  904 			break;
  905 		if (ret == 1) {
  906 			*end_ns = v->last_complete_ns;
  907 			return 0;
  908 		}
  909 		if (!(++spins & 1023))
  910 			cond_resched();
  911 		cpu_relax();
  912 	}
  913 	*end_ns = ktime_get_ns();
  914 	return ret;
  915 }
```

`linux/drivers/misc/idma.c:2561-2562`

```text
 2561 	e.victim.in_flight_at_start = v->in_flight;
 2562 	e.aggressor.in_flight_at_start = a && a->in_flight;
```

`linux/drivers/misc/idma.c:2579-2603`

```text
 2579 	ret = idma_observation_arm(&observation, victim, aggressor, e.victim_transfers);
 2580 	idma_comp_stage(v->composition_trace, v->dev, "arm-done", ret);
 2581 	if (ret)
 2582 		goto out_drain;
 2583 	stamp = ktime_get_ns();
 2584 	if (diag)
 2585 		diag->arm_end_ns = stamp;
 2586 	idma_snapshot(hpm, v, a, &before, active_before);
 2587 	start = ktime_get_ns();
 2588 	e.snapshot_before_ns = start - stamp;
 2589 	e.measurement_started = 1;
 2590 	idma_comp_stage(v->composition_trace, v->dev, "prewindow", 0);
 2591 	ret = batch ? idma_batch_window(v, a, deadline, &end, &batch_start) :
 2592 		idma_run_window(v, a, e.victim_transfers, deadline, true, &end);
 2593 	e.victim_elapsed_ns = end - start;
 2594 	e.measurement_complete = !ret;
 2595 	idma_comp_stage(v->composition_trace, v->dev, "window-return", ret);
 2596 	e.victim.in_flight_at_end = v->in_flight;
 2597 	e.aggressor.in_flight_at_end = a && a->in_flight;
 2598 	/* Stop autonomous A after V observation; hardware window is already frozen. */
 2599 	if (batch) {
 2600 		idma_batch_stop(a);
 2601 		if (ret)
 2602 			idma_batch_stop(v);
 2603 	}
```

### E08 — Drain, collect, equality checks, and post-drain data verification

文件：`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c`。SHA256：`e5e954c62e07d43bdd40d721eb10a176696ca179d4a565d1feeb5b13073efd47`。

`linux/drivers/misc/idma.c:667-687`

```text
  667 /* 1: completion observed; 0: pending; negative: failed. Never refills itself. */
  668 static int idma_poll(struct idma_stream *s, bool measured)
  669 {
  670 	u64 done;
  671 
  672 	if (!s->in_flight)
  673 		return 0;
  674 	done = readq(s->idma->reg + IDMA_DONE_REG_OFFSET);
  675 	if (done != s->transfer_id)
  676 		return ktime_get_ns() > s->deadline_ns ? -ETIMEDOUT : 0;
  677 	s->last_complete_ns = ktime_get_ns();
  678 	if (s->diagnostic && !s->batch_started)
  679 		s->diagnostic->warm_completed++;
  680 	s->in_flight = false;
  681 	if (!s->batch_configured)
  682 		s->touched_end = max(s->touched_end, s->offset + s->chunk);
  683 	if (measured) {
  684 		s->stats->completed++;
  685 		s->stats->completed_bytes += s->chunk;
  686 	}
  687 	return 1;
```

`linux/drivers/misc/idma.c:751-769`

```text
  751 static int idma_drain(struct idma_stream *s)
  752 {
  753 	u64 done;
  754 	int ret;
  755 
  756 	if (!s->in_flight)
  757 		return s->retain ? -EIO : 0;
  758 	ret = readq_poll_timeout(s->idma->reg + IDMA_DONE_REG_OFFSET, done,
  759 				 done == s->transfer_id, 0, IDMA_XFER_TIMEOUT_US);
  760 	if (ret) {
  761 		idma_quarantine(s);
  762 		return ret;
  763 	}
  764 	s->in_flight = false;
  765 	if (s->diagnostic && !s->batch_started)
  766 		s->diagnostic->warm_drained++;
  767 	if (!s->batch_configured)
  768 		s->touched_end = max(s->touched_end, s->offset + s->chunk);
  769 	return 0;
```

`linux/drivers/misc/idma.c:778-787`

```text
  778 	/* Verify untouched bytes as well, and source integrity independently. */
  779 	for (i = 0; i < s->size; i++) {
  780 		u64 pattern = idma_pattern(s, i / sizeof(u64));
  781 		u8 value = ((u8 *)&pattern)[i % sizeof(u64)];
  782 		u8 expected = (i < s->touched_end ||
  783 			       i >= s->size - s->touched_tail) ? value : (u8)~value;
  784 		u8 src = READ_ONCE(((u8 *)s->src)[i]);
  785 		u8 dst = READ_ONCE(((u8 *)s->dst)[i]);
  786 
  787 		if (src != value || dst != expected) {
```

`linux/drivers/misc/idma.c:2613-2654`

```text
 2613  out_drain:
 2614 	idma_comp_stage(v->composition_trace, v->dev, "drain-start", ret);
 2615 	if (batch) {
 2616 		idma_batch_stop(v);
 2617 		idma_batch_stop(a);
 2618 	}
 2619 	if (cfg && ret)
 2620 		idma_research2_method_abort(victim, cfg, ret);
 2621 	stamp = ktime_get_ns();
 2622 	drain_ret = idma_drain(v);
 2623 	if (drain_ret && !ret)
 2624 		ret = drain_ret;
 2625 	if (a) {
 2626 		drain_ret = idma_drain(a);
 2627 		if (drain_ret && !ret)
 2628 			ret = drain_ret;
 2629 	}
 2630 	e.drain_ns = ktime_get_ns() - stamp;
 2631 	if (batch) {
 2632 		drain_ret = idma_batch_collect(v);
 2633 		if (drain_ret && !ret)
 2634 			ret = drain_ret;
 2635 		drain_ret = idma_batch_collect(a);
 2636 		if (drain_ret && !ret)
 2637 			ret = drain_ret;
 2638 		if (!ret && e.victim.completed != e.victim_transfers)
 2639 			ret = -EIO;
 2640 	}
 2641 	idma_comp_stage(v->composition_trace, v->dev, "drain-end", ret);
 2642 	riscv_iommu_hpm_release(hpm);
 2643 	if (!ret) {
 2644 		/* Both lanes are drained: reacquire CPU visibility before any check. */
 2645 		local_flush_icache_all();
 2646 		verify_ret = idma_verify(v);
 2647 		if (verify_ret)
 2648 			ret = verify_ret;
 2649 		if (a) {
 2650 			verify_ret = idma_verify(a);
 2651 			if (verify_ret && !ret)
 2652 				ret = verify_ret;
 2653 		}
 2654 	}
```

### E09 — Composition configuration, opaque raw capture, and quiet-only restore

文件：`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c`。SHA256：`e5e954c62e07d43bdd40d721eb10a176696ca179d4a565d1feeb5b13073efd47`。

`linux/drivers/misc/idma.c:2274-2306`

```text
 2274 /* New, independent hardware bank. All calls hold idma_experiment_lock. */
 2275 #define IDMA_COMPOSITION_BASE (512U + 0xd20U)
 2276 
 2277 static u64 idma_composition_read(struct idma_device *d, unsigned int word)
 2278 {
 2279 	writeq(IDMA_COMPOSITION_BASE + word, d->reg + IDMA_OBS_INDEX_OFFSET);
 2280 	return readq(d->reg + IDMA_OBS_DATA_OFFSET);
 2281 }
 2282 
 2283 static int idma_composition_configure(struct idma_device *d, u32 mode, u32 did,
 2284 				      u64 *generation)
 2285 {
 2286 	u64 old, now, status, config = ((u64)did << 8) | mode;
 2287 
 2288 	if (idma_composition_read(d, 0) != IDMA_COMPOSITION_MAGIC ||
 2289 	    idma_composition_read(d, 1) != IDMA_COMPOSITION_ABI)
 2290 		return -EOPNOTSUPP;
 2291 	old = idma_composition_read(d, 3);
 2292 	if (old == U64_MAX)
 2293 		return -EOVERFLOW;
 2294 	if (idma_composition_read(d, 5) != 1)
 2295 		return -EBUSY;
 2296 	writeq(IDMA_COMPOSITION_BASE + 2, d->reg + IDMA_OBS_INDEX_OFFSET);
 2297 	writeq(config, d->reg + IDMA_OBS_DATA_OFFSET);
 2298 	/* Same-clock atomic clear/config/ack: ordered read observes that edge. */
 2299 	status = idma_composition_read(d, 4);
 2300 	now = idma_composition_read(d, 3);
 2301 	if (status == 1)
 2302 		return -EBUSY;
 2303 	if (status || now != old + 1 || idma_composition_read(d, 2) != config)
 2304 		return -EIO;
 2305 	*generation = now;
 2306 	return 0;
```

`linux/drivers/misc/idma.c:2309-2340`

```text
 2309 static int idma_composition_capture(struct idma_device *d,
 2310 				    struct idma_composition *p, int error)
 2311 {
 2312 	u64 config = ((u64)p->protected_did << 8) | p->mode;
 2313 	unsigned int i;
 2314 
 2315 	for (i = 0; i < IDMA_COMPOSITION_WORDS; i++)
 2316 		p->raw[i] = idma_composition_read(d, i);
 2317 	p->raw_captured = 1;
 2318 	if (error)
 2319 		return error;
 2320 	if (p->raw[0] != IDMA_COMPOSITION_MAGIC || p->raw[1] != 1 ||
 2321 	    p->raw[2] != config || p->raw[3] != p->generation ||
 2322 	    p->raw[6] != 4 || p->raw[9] || p->raw[10] != config ||
 2323 	    p->raw[11] != p->generation || !p->raw[7] ||
 2324 	    p->raw[8] != p->p4m.window[1] || p->raw[34] != 3)
 2325 		return -EIO;
 2326 	for (i = 12; i <= 18; i += 3)
 2327 		if (p->raw[i] != p->raw[i + 1] + p->raw[i + 2])
 2328 			return -EIO;
 2329 	if (p->raw[24] > p->raw[8] || p->raw[25] ||
 2330 	    (!p->victim_gap_cycles && p->raw[24]) ||
 2331 	    (p->victim_gap_cycles && p->p4m.experiment.victim_transfers > 1 &&
 2332 	     !p->raw[24]) || p->raw[30] || p->raw[32] || p->raw[33])
 2333 		return -EIO;
 2334 	/* Reserved words are an ABI check, not experimental outcome filtering. */
 2335 	if (p->raw[35])
 2336 		return -EIO;
 2337 	for (i = 40; i < 64; i++)
 2338 		if (p->raw[i])
 2339 			return -EPROTO;
 2340 	return 0;
```

`linux/drivers/misc/idma.c:2343-2354`

```text
 2343 static int idma_composition_restore(struct idma_device *d,
 2344 				    struct idma_composition *p)
 2345 {
 2346 	u64 deadline = ktime_get_ns() + NSEC_PER_SEC;
 2347 
 2348 	/* Never force a cache clear across an unfinished DMA or response. */
 2349 	while (idma_composition_read(d, 5) != 1) {
 2350 		if (ktime_get_ns() >= deadline)
 2351 			return -ETIMEDOUT;
 2352 		cond_resched();
 2353 	}
 2354 	return idma_composition_configure(d, 0, 10, &p->restored_generation);
```

`linux/drivers/misc/idma.c:2436-2457`

```text
 2436 	if (composition) {
 2437 		idma_comp_stage(comp_trace, victim->miscdev.parent,
 2438 				"pre-config", 0);
 2439 		ret = idma_composition_configure(victim, composition->mode,
 2440 				composition->protected_did, &composition->generation);
 2441 		idma_comp_stage(comp_trace, victim->miscdev.parent,
 2442 				"post-config", ret);
 2443 		if (ret)
 2444 			goto out_put;
 2445 		composition->configured = 1;
 2446 		composition->restore_status = -EINPROGRESS;
 2447 	}
 2448 	ret = idma_stream_prepare(v, victim, e.victim_pages, e.chunk_bytes, &e.victim);
 2449 	if (ret)
 2450 		goto out_cleanup;
 2451 	v->composition_trace = comp_trace;
 2452 	if (a) {
 2453 		ret = idma_stream_prepare(a, aggressor, e.aggressor_pages,
 2454 					  e.chunk_bytes, &e.aggressor);
 2455 		if (ret)
 2456 			goto out_cleanup;
 2457 		a->reverse = cfg && cfg->reverse;
```

`linux/drivers/misc/idma.c:2688-2701`

```text
 2688 	if (batch) {
 2689 		idma_batch_disable(v);
 2690 		idma_batch_disable(a);
 2691 	}
 2692 	if (composition && composition->configured) {
 2693 		int capture_ret = idma_composition_capture(victim, composition, ret);
 2694 
 2695 		if (!ret)
 2696 			ret = capture_ret;
 2697 		composition->restore_status = idma_composition_restore(victim, composition);
 2698 		if (!ret)
 2699 			ret = composition->restore_status;
 2700 		idma_comp_stage(v->composition_trace, v->dev, "restored",
 2701 				composition->restore_status);
```

### E10 — Observer owner IDs, aggregate/window checks, and absence of the E3 pair rerun in composition

文件：`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c`。SHA256：`e5e954c62e07d43bdd40d721eb10a176696ca179d4a565d1feeb5b13073efd47`。

`linux/drivers/misc/idma.c:985-1026`

```text
  985 /* Called with the experiment lock, before preparing or submitting any DMA. */
  986 static int idma_observation_prepare(struct idma_observation *o, bool enabled)
  987 {
  988 	u64 magic;
  989 
  990 	if (!enabled)
  991 		return 0;
  992 	o->sequence = ++idma_observation_sequence;
  993 	o->control = idma_get_instance(0);
  994 	if (!o->control)
  995 		return -ENODEV;
  996 	magic = readq(o->control->reg + IDMA_OBS_MAGIC_OFFSET);
  997 	/* V9 retains window accounting but physically omits the old path timers. */
  998 	o->path = magic != IDMA_OBS_MAGIC_V9 &&
  999 		(o->p4m || o->e3 || (o->batch && READ_ONCE(idma_path_observe)));
 1000 	if (o->p4m && magic != IDMA_OBS_MAGIC_V9 &&
 1001 	    magic != IDMA_OBS_MAGIC_V8 && magic != IDMA_OBS_MAGIC_V7)
 1002 		return -EOPNOTSUPP;
 1003 	if (o->controlled && magic != IDMA_OBS_MAGIC_V6 &&
 1004 	    magic != IDMA_OBS_MAGIC_V8 && magic != IDMA_OBS_MAGIC_V7)
 1005 		return -EOPNOTSUPP;
 1006 	if (o->queue_pair && magic != IDMA_OBS_MAGIC_V5 &&
 1007 	    magic != IDMA_OBS_MAGIC_V6 && magic != IDMA_OBS_MAGIC_V8 && magic != IDMA_OBS_MAGIC_V7)
 1008 		return -EOPNOTSUPP;
 1009 	o->e3_magic = (magic == IDMA_OBS_MAGIC_V5 || magic == IDMA_OBS_MAGIC_V6 ||
 1010 		       magic == IDMA_OBS_MAGIC_V8 || magic == IDMA_OBS_MAGIC_V7) ? IDMA_E3_MAGIC_V2 : IDMA_E3_MAGIC;
 1011 	if (o->e3 && magic != IDMA_OBS_MAGIC_V4 &&
 1012 	    magic != IDMA_OBS_MAGIC_V5 && magic != IDMA_OBS_MAGIC_V6 &&
 1013 	    magic != IDMA_OBS_MAGIC_V8 && magic != IDMA_OBS_MAGIC_V7)
 1014 		return -EOPNOTSUPP;
 1015 	if (o->control->did != 10 ||
 1016 	    (magic != IDMA_OBS_MAGIC_V9 && magic != IDMA_OBS_MAGIC_V8 &&
 1017 	     magic != IDMA_OBS_MAGIC_V7 && magic != IDMA_OBS_MAGIC_V6 &&
 1018 	     magic != IDMA_OBS_MAGIC_V5 && magic != IDMA_OBS_MAGIC_V4 &&
 1019 	     magic != IDMA_OBS_MAGIC_V3 &&
 1020 	     (o->path || (magic != IDMA_OBS_MAGIC_V2 && (o->batch || magic != IDMA_OBS_MAGIC)))))
 1021 		return -EOPNOTSUPP;
 1022 	/* Clear previous frozen results; never take ownership of an active session. */
 1023 	if (readq(o->control->reg + IDMA_OBS_CONTROL_OFFSET) & 3)
 1024 		return -EBUSY;
 1025 	writeq(4, o->control->reg + IDMA_OBS_CONTROL_OFFSET);
 1026 	return readq(o->control->reg + IDMA_OBS_CONTROL_OFFSET) ? -EIO : 0;
```

`linux/drivers/misc/idma.c:1029-1057`

```text
 1029 static int idma_observation_arm(struct idma_observation *o,
 1030 				struct idma_device *v, struct idma_device *a, u64 count)
 1031 {
 1032 	u64 command = 1 | (o->batch ? BIT_ULL(25) : 0) | (o->path ? BIT_ULL(26) : 0);
 1033 
 1034 	if (o->p4m) {
 1035 		if (o->e3 || o->controlled || o->delay128)
 1036 			return -EINVAL;
 1037 		command |= BIT_ULL(30);
 1038 	}
 1039 	if (o->e3)
 1040 		command |= BIT_ULL(27) | (o->delay128 ? BIT_ULL(28) : 0);
 1041 	if (o->controlled)
 1042 		command |= BIT_ULL(29);
 1043 
 1044 	if (!o->control)
 1045 		return 0;
 1046 	if (o->controlled && count != 1)
 1047 		return -EINVAL;
 1048 	if ((u32)v->id >= 4 || v->did != 10U + v->id ||
 1049 	    (a && ((u32)a->id >= 4 || a->did != 10U + a->id)))
 1050 		return -EOPNOTSUPP;
 1051 	command |= (u64)v->id << 8;
 1052 	if (a)
 1053 		command |= BIT_ULL(24) | ((u64)a->id << 16);
 1054 	writeq(count, o->control->reg + IDMA_OBS_TARGET_OFFSET);
 1055 	writeq(command, o->control->reg + IDMA_OBS_CONTROL_OFFSET);
 1056 	o->armed = true;
 1057 	return readq(o->control->reg + IDMA_OBS_CONTROL_OFFSET) == 1 ? 0 : -EIO;
```

`linux/drivers/misc/idma.c:1060-1094`

```text
 1060 /* Drain/verify first. Frozen reads and logging are outside all timed windows. */
 1061 static int idma_observation_finish(struct idma_observation *o,
 1062 				   const struct idma_experiment *e, int run_error)
 1063 {
 1064 	u64 values[IDMA_OBS_VALUES], path[16][8], status;
 1065 	void __iomem *base;
 1066 	bool valid;
 1067 	int i;
 1068 
 1069 	if (!o->control || !o->armed)
 1070 		return 0;
 1071 	base = o->control->reg;
 1072 	status = readq(base + IDMA_OBS_CONTROL_OFFSET);
 1073 	if (status & 3) {
 1074 		writeq(2, base + IDMA_OBS_CONTROL_OFFSET);
 1075 		status = readq(base + IDMA_OBS_CONTROL_OFFSET);
 1076 	}
 1077 	for (i = 0; i < IDMA_OBS_VALUES; i++) {
 1078 		writeq(i, base + IDMA_OBS_INDEX_OFFSET);
 1079 		values[i] = readq(base + IDMA_OBS_DATA_OFFSET);
 1080 	}
 1081 	valid = !run_error && status == 4 &&
 1082 		values[7] == e->victim_transfers && values[8] == values[7] &&
 1083 		values[9] == (o->batch ? 1 : values[7]) &&
 1084 		values[0] == values[1] + values[2] + values[3] &&
 1085 		values[0] == values[10] + values[11] + values[12] + values[13];
 1086 	/* Opening stock + accepted requests must equal returns + closing stock. */
 1087 	for (i = 0; i < 2; i++) {
 1088 		u64 opening = values[30 + i], closing = values[32 + i];
 1089 
 1090 		if ((opening >> 16) + values[14 + 6 * i] !=
 1091 		    (closing >> 16) + values[16 + 6 * i] ||
 1092 		    (opening & 0xffff) + values[15 + 6 * i] !=
 1093 		    (closing & 0xffff) + values[17 + 6 * i])
 1094 			valid = false;
```

`linux/drivers/misc/idma.c:1126-1155`

```text
 1126 	if (o->batch) {
 1127 		u64 read_delay;
 1128 
 1129 		writeq(34, base + IDMA_OBS_INDEX_OFFSET);
 1130 		read_delay = readq(base + IDMA_OBS_DATA_OFFSET);
 1131 		valid = valid && values[2] == 0;
 1132 		dev_info(o->control->miscdev.parent,
 1133 			 "idma_batch_obs seq=%llu part=time valid=%u status=%#llx v_did=%u a_did=%u cycles=%llu h=%llu gap=%llu hmax=%llu gapmax=%llu issued=%llu done=%llu batch_polled=%llu done_to_read=%llu\n",
 1134 			 o->sequence, valid, status, e->victim_did, e->aggressor_did,
 1135 			 values[0], values[1], values[3], values[4], values[6],
 1136 			 values[7], values[8], values[9], read_delay);
 1137 	} else {
 1138 		dev_info(o->control->miscdev.parent,
 1139 			 "idma_obs seq=%llu part=time valid=%u status=%#llx v_did=%u a_did=%u cycles=%llu h=%llu p=%llu r=%llu hmax=%llu pmax=%llu rmax=%llu issued=%llu done=%llu polled=%llu\n",
 1140 			 o->sequence, valid, status, e->victim_did, e->aggressor_did,
 1141 			 values[0], values[1], values[2], values[3], values[4], values[5],
 1142 			 values[6], values[7], values[8], values[9]);
 1143 	}
 1144 	dev_info(o->control->miscdev.parent,
 1145 		 "%s seq=%llu part=axi_v neither=%llu v_only=%llu a_only=%llu both=%llu ar=%llu aw=%llu rlast=%llu b=%llu ar_wait=%llu aw_wait=%llu start=%#llx end=%#llx\n",
 1146 		 (o->batch ? "idma_batch_obs" : "idma_obs"), o->sequence,
 1147 		 values[10], values[11], values[12], values[13],
 1148 		 values[14], values[15], values[16], values[17], values[18],
 1149 		 values[19], values[30], values[32]);
 1150 	dev_info(o->control->miscdev.parent,
 1151 		 "%s seq=%llu part=axi_a ar=%llu aw=%llu rlast=%llu b=%llu ar_wait=%llu aw_wait=%llu start=%#llx end=%#llx shared_ar=%llu shared_aw=%llu shared_ar_wait=%llu shared_aw_wait=%llu\n",
 1152 		 (o->batch ? "idma_batch_obs" : "idma_obs"), o->sequence,
 1153 		 values[20], values[21], values[22], values[23],
 1154 		 values[24], values[25], values[31], values[33], values[26],
 1155 		 values[27], values[28], values[29]);
```

`linux/drivers/misc/idma.c:1392-1444`

```text
 1392 static int idma_p4m_finish(struct idma_observation *o,
 1393 			   const struct idma_experiment *e,
 1394 			   struct idma_p4m *p, int run_error)
 1395 {
 1396 	u64 flags, done, first, last, start_id, end_id;
 1397 	bool has_a = e->aggressor_did != IDMA_EXPERIMENT_NO_AGGRESSOR;
 1398 	bool valid = !run_error;
 1399 	unsigned int i;
 1400 
 1401 	if (!o->control || !o->armed)
 1402 		return run_error ? run_error : -EIO;
 1403 	p->sequence = o->sequence;
 1404 	p->observer_status = readq(o->control->reg + IDMA_OBS_CONTROL_OFFSET);
 1405 	for (i = 0; i < IDMA_P4M_OBS_WORDS; i++)
 1406 		p->observation[i] = idma_observer_indexed(o, i);
 1407 	for (i = 0; i < IDMA_P4M_WORDS; i++)
 1408 		p->window[i] = idma_observer_indexed(o, IDMA_P4M_BANK_BASE + i);
 1409 
 1410 	/* Frozen record: owned sequence, exact frozen status, live counter. */
 1411 	if (!p->sequence || p->observer_status != 4)
 1412 		valid = false;
 1413 	if (p->window[0] != IDMA_P4M_BANK_MAGIC)
 1414 		valid = false;
 1415 	if (!p->window[1] || p->window[1] != p->observation[0])
 1416 		valid = false;
 1417 	flags = p->window[15];
 1418 	if (!(flags & BIT_ULL(1)))
 1419 		valid = false;
 1420 	if (flags & ~(BIT_ULL(0) | BIT_ULL(1) | BIT_ULL(2)))
 1421 		valid = false;
 1422 	if (!!(flags & BIT_ULL(0)) != has_a)
 1423 		valid = false;
 1424 	if (p->window[12])
 1425 		valid = false;
 1426 	if (p->observation[7] != e->victim_transfers ||
 1427 	    p->observation[8] != e->victim_transfers)
 1428 		valid = false;
 1429 	if (p->window[13] != e->victim_transfers ||
 1430 	    p->window[14] != e->victim_transfers)
 1431 		valid = false;
 1432 
 1433 	/* Every batch identifier must stay inside the encodable range. */
 1434 	done = p->window[3];
 1435 	first = p->window[10];
 1436 	last = p->window[11];
 1437 	start_id = p->window[8];
 1438 	end_id = p->window[9];
 1439 	if (p->window[2] > IDMA_EXPERIMENT_MAX_TRANSFERS ||
 1440 	    done > IDMA_EXPERIMENT_MAX_TRANSFERS ||
 1441 	    start_id > IDMA_EXPERIMENT_MAX_TRANSFERS ||
 1442 	    end_id > IDMA_EXPERIMENT_MAX_TRANSFERS ||
 1443 	    first > IDMA_EXPERIMENT_MAX_TRANSFERS ||
 1444 	    last > IDMA_EXPERIMENT_MAX_TRANSFERS)
```

`linux/drivers/misc/idma.c:1453-1497`

```text
 1453 	} else {
 1454 		if (p->window[4] > 1 || p->window[5] > 1 ||
 1455 		    p->window[6] > 1 || p->window[7] > 1)
 1456 			valid = false;
 1457 		if (p->window[4] + p->window[2] !=
 1458 		    p->window[3] + p->window[5])
 1459 			valid = false;
 1460 		if (p->window[6] > p->window[4] ||
 1461 		    p->window[6] > p->window[3])
 1462 			valid = false;
 1463 		if (!!(flags & BIT_ULL(2)) !=
 1464 		    (p->window[4] && !p->window[7]))
 1465 			valid = false;
 1466 		/* An opening edge issue hands off a single stock entry. */
 1467 		if (p->window[7] && p->window[4] != 1)
 1468 			valid = false;
 1469 		/* Strict carry retires a carried entry only when work was done. */
 1470 		if (!!p->window[6] !=
 1471 		    (!!(flags & BIT_ULL(2)) && done > 0))
 1472 			valid = false;
 1473 		if (!!p->window[4] != !!p->window[8])
 1474 			valid = false;
 1475 		if (!!p->window[5] != !!p->window[9])
 1476 			valid = false;
 1477 		if (done) {
 1478 			if (!first || !last)
 1479 				valid = false;
 1480 			if (last != first + done - 1)
 1481 				valid = false;
 1482 			/* The batch opens on the opening stock's first ID. */
 1483 			if (p->window[4] && first != start_id)
 1484 				valid = false;
 1485 			/* The closing stock continues past the last retired ID. */
 1486 			if (p->window[5] && end_id != last + 1)
 1487 				valid = false;
 1488 		} else if (p->window[4]) {
 1489 			/* Nothing retired: the carried entry closes unchanged. */
 1490 			if (p->window[5] != 1 || end_id != start_id ||
 1491 			    first || last)
 1492 				valid = false;
 1493 		} else if (first || last) {
 1494 			valid = false;
 1495 		}
 1496 	}
 1497 	return valid ? 0 : -EIO;
```

`linux/drivers/misc/idma.c:2703-2735`

```text
 2703 	if (pair) {
 2704 		if (observation.armed) {
 2705 			observation_ret = idma_e3_finish(&observation, &e, pair->observation[round]);
 2706 			if (observation_ret && !ret)
 2707 				ret = observation_ret;
 2708 		}
 2709 		e.status = ret;
 2710 		pair->result[round] = e;
 2711 		pair->sequence[round] = observation.sequence;
 2712 		if (batch)
 2713 			idma_batch_start_log(victim->miscdev.parent, &batch_start,
 2714 					     observation.sequence, ret);
 2715 		if (!ret)
 2716 			pair->rounds_completed++;
 2717 		if (!ret && round == 0) {
 2718 			round = 1;
 2719 			idma_stream_reseed(v);
 2720 			idma_stream_reseed(a);
 2721 			memset(&e.status, 0, sizeof(e) - offsetof(struct idma_experiment, status));
 2722 			e.victim_did = victim->did;
 2723 			e.aggressor_did = a ? aggressor->did : IDMA_EXPERIMENT_NO_AGGRESSOR;
 2724 			idma_put_instance(observation.control);
 2725 			observation = (struct idma_observation) { .batch = true, .e3 = true,
 2726 				.delay128 = (!diagnostic || queue_pair) && pair->first_delay == 0,
 2727 				.queue_pair = queue_pair };
 2728 			ret = idma_observation_prepare(&observation, true);
 2729 			if (ret)
 2730 				goto out_cleanup;
 2731 			goto run_round;
 2732 		}
 2733 	} else if (batch) {
 2734 		idma_batch_start_log(victim->miscdev.parent, &batch_start,
 2735 				     observation.sequence, ret);
```

### E11 — Existing plan source constants only; no import, plan generation, validation, or records

文件：`/home/xvhaocheng/work/iommu-platform/scripts/composition_plan.py`。SHA256：`52fb3530ee87e3e72f49660007083feb5e2a45f4acebb9d3d30dd187cf3f710c`。

`scripts/composition_plan.py:1-13`

```text
    1 #!/usr/bin/env python3
    2 # -*- coding: utf-8 -*-
    3 """
    4 composition_plan.py - frozen plan and record validator for the same-BIT
    5 composition experiment (idma-composition-v1).
    6 
    7 Import-only: no third-party dependencies, no serial port, no device node.  It
    8 provides the fixed 172-case plan in execution order, the per-case board CLI
    9 arguments, the frozen-contract validator and the narrow window metrics.
   10 
   11 Nothing here re-runs, re-orders or reselects a sample.  A scientific negative
   12 (fast == 0, miss == 0, a P/PN throughput regression) is never turned into a
   13 technical failure; only out-of-contract evidence is.
```

`scripts/composition_plan.py:22-57`

```text
   22 PHASES = ('short', 'noise', 'explore')
   23 
   24 MODE_S = 0
   25 MODE_N = 1
   26 MODE_P = 2
   27 MODE_PN = 3
   28 MODE_ORDER = (MODE_S, MODE_N, MODE_P, MODE_PN)
   29 MODE_NAMES = {MODE_S: 's', MODE_N: 'n', MODE_P: 'p', MODE_PN: 'pn'}
   30 # N is enabled in N and PN; a fast accept while N is disabled is a violation.
   31 N_ENABLED_MODES = frozenset((MODE_N, MODE_PN))
   32 N_DISABLED_MODES = frozenset((MODE_S, MODE_P))
   33 
   34 A_PAGE_CHOICES = (0, 1, 8, 16)
   35 V_GAP_CHOICES = (0, 1000, 4000)
   36 
   37 SHORT_TRANSFERS = 16
   38 SHORT_WARMUP = 16
   39 NOISE_A_PAGES = 8
   40 NOISE_TRANSFERS = 4096
   41 NOISE_WARMUP = 256
   42 NOISE_PAIRS = 6
   43 EXPLORE_TRANSFERS = 4096
   44 EXPLORE_WARMUP = 256
   45 EXPLORE_BLOCKS = 3
   46 
   47 DEFAULT_TRANSFERS = 4096
   48 DEFAULT_WARMUP = 256
   49 DEFAULT_TIMEOUT_MS = 15000
   50 DEFAULT_DEVICE = '/dev/idma0'
   51 
   52 PROTECTED_DID = 10
   53 AGGRESSOR_INSTANCE = 1
   54 AGGRESSOR_DID = 11
   55 CHUNK_BYTES = 4096
   56 PLATFORM_HZ = 50000000
   57 CYCLE_NS = 20
```

`scripts/composition_plan.py:72-80`

```text
   72 RAW_MAGIC = 0
   73 RAW_ABI = 1
   74 RAW_LIVE_CONFIG = 2
   75 RAW_LIVE_GENERATION = 3
   76 RAW_LAST_STATUS = 4
   77 RAW_LIVE_QUIET = 5
   78 RAW_WINDOW_FLAGS = 6
   79 RAW_WINDOW_SEQUENCE = 7
   80 RAW_WINDOW_CYCLES = 8
```

### E12 — The sole permitted hardware reference: conditional buffer heterogeneity and launch-port binding

文件：`/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`。SHA256：`9eaa9edbadef37c5323e15f3c2b201723af1cc58806517ab5f36f92c0d591666`。

`out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1260-1286`

```text
 1260                 .AXI_SLV_ID_WIDTH   ( ariane_soc::IdWidthSlave ),
 1261                 .AXI_USER_WIDTH     ( AxiUserWidth             ),
 1262                 .JOB_FIFO_DEPTH     ( 8                        ),
 1263                 // N v2: a frozen batch job is one aligned 4 KiB copy. Keep
 1264                 // its full source data until the paired write can drain, so
 1265                 // a read-ahead burst cannot block the shared R return path.
 1266                 // S and N use the same capacity; other DMA/legacy modes keep
 1267                 // the existing three-beat buffer. Larger/multi-job streams
 1268                 // are outside this prototype's dependency argument.
 1269                 .BUFFER_DEPTH       ( IOMMUNonblockMode != 0 && i < 2 ? 512 : 3 ),
 1270                 .NUM_AX_IN_FLIGHT   ( 8                        ),
 1271                 .MEM_SYS_DEPTH      ( 0                        ),
 1272                 .RAW_COUPLING_AVAIL ( 0                        ),
 1273                 .IS_TWO_D           ( 0                        ),
 1274                 .BATCH_PORT (1'b1),
 1275                 .OBSERVER_PORT      ( i == 0                   ),
 1276                 .LAUNCH_PORT        ( InclIOMMU && i < 2       ),
 1277                 .RESEARCH_PORT      ( InclIOMMU && IOMMUResearchRole != 0 && i == 1 ),
 1278 
 1279                 .DEVICE_ID          ( 24'(10 + i)              ),
 1280                 .PROCESS_ID         ( 20'(20 + i)              ),
 1281                 .AxID               ( '0                       )
 1282             ) i_dma (
 1283                 .obs_issue_o(obs_issue[i]), .obs_done_o(obs_done[i]), .obs_poll_o(obs_poll[i]),
 1284                 .batch_active_o(batch_active[i]),
 1285                 .batch_gap_active_o(batch_gap_active[i]), .job_quiet_o(job_quiet[i]),
 1286                 .fullbuffer_job_o(fullbuffer_jobs[i]),
```

### E13 — Root指定exact short case的静态定义与field map

文件：`/home/xvhaocheng/work/iommu-platform/scripts/composition_plan.py`。SHA256：`52fb3530ee87e3e72f49660007083feb5e2a45f4acebb9d3d30dd187cf3f710c`。只因root新输入追加定向读取116-150；没有执行生成器、validator、runner或读取records。

`scripts/composition_plan.py:116-150`

```text
  116 def _case_id(phase, mode, a_pages, v_gap_cycles, block, pair, slot):
  117     name = MODE_NAMES[mode]
  118     if phase == 'short':
  119         return 'short-m%s-a%d' % (name, a_pages)
  120     if phase == 'noise':
  121         return 'noise-p%d-s%d-m%s-a%d' % (pair, slot, name, a_pages)
  122     return 'explore-b%d-g%d-a%d-m%s' % (block, v_gap_cycles, a_pages, name)
  123 
  124 
  125 def _make_case(phase, mode, a_pages, v_gap_cycles, transfers, warmup, block, pair, slot):
  126     return {
  127         'id': _case_id(phase, mode, a_pages, v_gap_cycles, block, pair, slot),
  128         'phase': phase,
  129         'block': block,
  130         'pair': pair,
  131         'slot': slot,
  132         'mode': mode,
  133         'mode_name': MODE_NAMES[mode],
  134         'a_pages': a_pages,
  135         'v_gap_cycles': v_gap_cycles,
  136         'transfers': transfers,
  137         'warmup': warmup,
  138         'timeout_ms': DEFAULT_TIMEOUT_MS,
  139         'auto_rerun': False,
  140     }
  141 
  142 
  143 def build_plan():
  144     """Return the full frozen 172-case plan in original execution order."""
  145     cases = []
  146     for mode in MODE_ORDER:
  147         for a_pages in A_PAGE_CHOICES:
  148             cases.append(_make_case('short', mode, a_pages, 0,
  149                                     SHORT_TRANSFERS, SHORT_WARMUP,
  150                                     None, None, None))
```

# 定向接口证据包：P4M/研究控制寄存器/rootfs（只读，未执行任何测试）

> 说明：以下全部为对随附源码的静态阅读证据。我**没有**编译、没有跑板、没有改文件。凡是推断而非源码明说的，都标了「推断」。文末列出未知项。

---

## 0. 导航摘要

**P4M 入口与调用链（全部在 `linux/drivers/misc/idma.c`）**

```
idma_ioctl case IDMA_IOC_RUN_P4M            idma.c:2723
  → idma_run_p4m(idma, uarg)                idma.c:2618
    → idma_p4m_clear_outputs                idma.c:2625 (定义 1283)
    → idma_p4m_validate                     idma.c:2626 (定义 1293)
    → idma_run_session(victim,&p.experiment, batch=true, pair=NULL,
                       diagnostic=NULL, queue_pair=false, p4m=&p,
                       research=NULL)       idma.c:2629
      → idma_run_session2(..., cfg=NULL)    idma.c:2614-2615 (定义 2254)
         observation = {.batch=1,.p4m=1}    idma.c:2262-2263
         aggressor = idma_get_instance(1)   idma.c:2296-2308
         idma_gap_preflight(victim,aggressor) idma.c:2312-2316
         idma_observation_prepare           idma.c:2317
         idma_stream_prepare(v/a)           idma.c:2330-2339
         local_flush_icache_all()            idma.c:2376
         warmup A submit + idma_run_window   idma.c:2382-2387
         idma_batch_configure(v / a)         idma.c:2423-2425
         idma_gap_program_readback(v,0)      idma.c:2430   ← V gap 钉死
         idma_gap_program_readback(a,gap)    idma.c:2432-2433
         idma_observation_arm                idma.c:2455
         idma_batch_window → idma_batch_start idma.c:2465 / 491
         drain + idma_batch_collect          idma.c:2485-2511
         idma_verify (含 FENCE.I)             idma.c:2515-2524 / 756
         idma_observation_finish             idma.c:2526
         idma_p4m_finish                     idma.c:2530 (定义 1372)
```

**寄存器页**
- 观测页索引/数据：`0x70`/`0x78`（`IDMA_OBS_INDEX_OFFSET`/`IDMA_OBS_DATA_OFFSET`，idma.c:43-44）。
- P4M bank：`448..463`（idma.c:56-57；RTL dma_observer.sv:451-464）。
- 研究 bank：基址 512（idma_research.h:4；`idma_research_read/write` 加 512，idma_research.h:46-56）。
- 批处理 gap：`0xd0` + magic `0xd8`（idma.c:83-85）。
- 批处理控制/目标/页数/完成/派发：`0x88/0x90/0x98/0xa0/0xa8`（idma.c:59-63）。

**六个问题速查**

| # | 结论 | 关键证据 |
|---|---|---|
| 1 | V gap 被 `idma_gap_program_readback(v, 0)` 硬钉为 0；gap 硬件是**每设备独立**、V 上同样存在；复用同一硬件可行，但 `struct idma_p4m` 无 V gap 输入字段，`reserved` 被强校验为 0 → 只能新增 ioctl/新结构 | idma.c:2430,2432-2433,1355-1370,1344-1353,1301；uapi:187,193-194 |
| 2 | 是：V 固定 id0/did10，A 固定 id1；方向（reverse）扩展在 P4M 路径被 `cfg=NULL` 阻断 | idma.c:1299,1308,2629,2338；idma_research.h:178-182,17 |
| 3 | 研究 bank 通过 index≥512 读写，结构上**碰不到** P4M bank(448..463)；需扩展研究索引常量、配置/冻结函数、cap feature 位；verify/缓存/P4M 窗口逻辑不需改 | dma_observer.sv:91,93-94,449-455,162；idma_research.h:46-56,198-246,264-290 |
| 4 | 新工具加进 `rootfs.py EXECUTABLES` + `platform.py bench()`；kernel/firmware 命令已固定，无需为工具改动 | rootfs.py:8-19,70-72；platform.py:156-166,172-182,184-201 |
| 5 | 可以：现有 index/data 读机制覆盖 study bank ≥512 的任意字，新输出字（lookup/hit/miss、mode、generation）可放研究 bank；只需扩展 kernel 硬编码校验与 helper 预期布局 | dma_observer.sv:162,450,453；idma_research.h:46-50,116-134；mac_nonblock_pilot.py:123-131 |
| 6 | 最小改动 4~6 个文件（见 §4）；接受测试见 §6 | — |

---

## 1. V gap 为何强制 0 / 可否复用现有 gap 硬件

### 事实
- 在 P4M 会话中 V 的 gap 被写成常量 0：
  - `idma.c:2430` `ret = idma_gap_program_readback(v, 0);`
  - `idma.c:2432-2433` `idma_gap_program_readback(a, p4m->aggressor_gap_cycles);`
- gap 寄存器/魔数是**每设备**独立的：
  - `IDMA_BATCH_GAP_OFFSET 0xd0`、`IDMA_BATCH_GAP_MAGIC_OFFSET 0xd8`、`IDMA_BATCH_GAP_MAGIC 0x49444d4147415001`（idma.c:83-85）。
  - `idma_gap_preflight` 对 victim **和** aggressor 各查一遍 magic（idma.c:1347-1351）。
  - `idma_gap_program_readback` 写的是 `s->idma->reg + 0xd0`（idma.c:1357,1362），即传入哪个 stream 就写哪个设备 → V 的 gap 硬件本身存在。
- gap 字段命名与校验只面向 aggressor：
  - `aggressor_gap_cycles`（uapi:193）。
  - Solo（无 A）时 gap 必须 0：`if (!has_a && p->aggressor_gap_cycles) return -EINVAL;`（idma.c:1310-1311）。
  - 研究2 METHOD 路径把 gap 限制在 500/1000/2000/4000 且要求 `policy_mode==0`（idma.c:1334-1336）。
- V 是定义观测窗口的那条 lane：RTL 在 V 完成计数达到 target 时闭窗（dma_observer.sv:396-409，`count_n[8]` 由 `done_i[victim_q]` 驱动 :391-393，关闭时 `p4m_closed_n=1`、`p4m_cycles_n=count_n[0]` :405-408）。

### 为何 0（推断，源码无一句直接注释）
- V lane 的语义是「被测对象」：窗口周期数 `p4m_cycles` 就是 `count_n[0]`（RTL :408），而 `count_n[0]` 在 `active`（HARDWARE/POLL_WAIT/REFILL）期间每拍 +1（dma_observer.sv:130,369）。若给 V 注入 gap，会让被测窗口里出现人为空闲，污染唯一被 pilot 当指标用的 `v_window_cycles`（mac_nonblock_pilot.py:427）。gap 因此只留给 A，用来制造「间歇 A」对照。
- 这是**推断**；源码只体现了「字段名=aggressor、校验只对 A 生效、V 被写 0」这三件事。

### 可否安全复用来做「间歇 V」，只需最小新 ioctl 且不改旧 ABI
- 机制上可行：`idma_gap_program_readback(v, <u32>)` 已经把整条写-读回校验路径对任意 stream 实现好了（idma.c:1355-1370），包括 sticky backend error 检查（1366-1368）。
- 但**旧 ABI 里没有 V gap 的输入位**：
  - `struct idma_p4m` 只有 `aggressor_gap_cycles` + `reserved[7]`（uapi:191-199）。
  - `reserved` 被强制全 0：`memchr_inv(p->reserved, 0, sizeof(p->reserved))`（idma.c:1301）。
  - uapi 注释明确「Input: …，reserved 必须为 0」（uapi:187）。
  - 所以**不能**用 reserved 偷渡 V gap 而不破坏旧 ABI 契约。
- 结论：给 V gap 必须**新增 ioctl + 新请求结构**（例如 `IDMA_IOC_RUN_P4M_VGAP`，新 cmd 码，紧接现有 0x10 之后的 0x11），旧 `IDMA_IOC_RUN_P4M`(0x0d) 保持 byte-for-byte 语义。旧结构校验（`idma_p4m_validate`）保持不动。
- 校验侧注意点（如果做 INTERMITTENT-V 而非 INTERMITTENT-A）：
  - `idma_p4m_finish` 只对 A 做守恒/ID 连续性检查（idma.c:1427-1476 的 `else` 分支），V 只要求 `observation[7]==observation[8]==victim_transfers`、`window[13]==window[14]==victim_transfers`（idma.c:1406-1411）。V gap 不破坏这些。
  - `validate_p4m`（bench，bench/idma_experiment.c:272-414）的 Solo 分支要求 `w[2..12]==0`（:344-349），A 分支检查 `a_*`。V gap 不影响。
- **未知（硬件）**：BatchPort/DMA 的 gap 是否在 victim lane 同样实现、以及 gap 周期是否计入 observer 的 `count_n[0]`，随附文件无法判定（gap 属 DMA/BatchPort RTL，未提供）。

---

## 2. P4M 是否固定 V0/A1；方向扩展的阻断点

### 固定角色
- V 固定：`if (victim->id != 0 || victim->did != 10) return -EINVAL;`（idma.c:1299）。
- A 固定：`if (has_a && e->aggressor_id != 1) return -EINVAL;`（idma.c:1308）。
- 通用约束（任意会话）：`(u32)v->id >= 4 || v->did != 10U + v->id || (a && ((u32)a->id >= 4 || a->did != 10U + a->id))` → EOPNOTSUPP（idma.c:1028-1030）；观测控制端 `o->control->did != 10` 也硬校验（idma.c:996）。
- 所以「V0/A1」是**硬编码**，不是配置项。

### 方向（reverse）扩展的阻断
- reverse 只来自 research2 config：`a->reverse = cfg && cfg->reverse;`（idma.c:2338）。
- P4M 路径 `idma_run_p4m → idma_run_session → idma_run_session2(..., cfg=NULL)`（idma.c:2629 → 2614-2615），所以 **P4M 下 `cfg==NULL`，`a->reverse` 恒为 false**。
- reverse 的实际下发布点：`idma_research2_set_reverse()`（idma_research.h:178-182），只被 `idma_research2_pre_arm` 调用（idma_research.h:333），而 pre_arm 只在 `if (cfg)` 下被调（idma.c:2450-2454）。
- 能力位门槛：`IDMA_RESEARCH2_FEATURE_REVERSE_A`（idma_research.h:17），E2 角色要求 EXTENDED_E2|E2_TIMING|REVERSE_A（idma_research.h:463-466）；METHOD 角色只要求 METHOD 掩码（:476-478）。
- 约束：reverse 需要 A 存在——`if (r->a_pages == 0 && (cfg->policy_mode || r->gap_cycles || cfg->reverse)) return -EINVAL;`（idma_research.h:434-436）。
- `reverse` 在 uapi 合法域为 0/1：`if (cfg->required_role != role || cfg->reverse > 1) return -EINVAL;`（idma_research.h:348）。

### 小结
- 若要给 P4M 加方向扩展，最小做法是新增一条能携带 `reverse`（以及 V gap 等）的 ioctl，并在 `idma_run_session2` 里把它映射到 `idma_research2_set_reverse` 或直接置 `a->reverse`，同时保证 `a != NULL`。不要改 `idma_p4m_validate` 与旧结构。
- 若「方向扩展」指的是**角色互换**（V 跑在 instance1、A 跑 instance0），则被 idma.c:1299 与 :1308 直接阻塞，且 `idma_observation_arm` 的 `v->id<<8`/`a->id<<16` 编码（idma.c:1031-1033）也按此约定。

---

## 3. 研究控制寄存器：接入点、配置时序、需扩展的函数

### RTL 侧接口（已存在，可承载新控制寄存器）
- 索引输出：`research_index_o = index_q - 13'd512;`（dma_observer.sv:91，13-bit）。
- 写使能：`research_write_o = ResearchEnable && cfg_valid_i && cfg_write_i && cfg_wstrb_i==8'hff && cfg_addr_i==7'h78 && index_q>=13'd512;`（dma_observer.sv:93-94）。
- 读数据：`else if (ResearchEnable && index_q >= 13'd512) cfg_rdata_o = research_data_i;`（dma_observer.sv:162）。
- 索引寄存器写范围：`… || (ResearchEnable && cfg_wdata_i>=512 && cfg_wdata_i<8192) …`，`index_n = cfg_wdata_i[12:0]`（dma_observer.sv:450,453）。
- 生命信号（供「静止」判断）：
  - `research_quiet_o = !(|batch_active_i || |ar_valid_i || |aw_valid_i || |track_bad_q)` 且所有 lane 的 reads/writes 为 0（dma_observer.sv:100-104）。
  - `research_open_o = ResearchEnable && p4m_q && phase_q==ARMED && issue_i[victim_q]`（:98）。
  - `research_close_o = ResearchEnable && p4m_closed_n && !p4m_closed_q`（:99）。
  - `research_arm_o` 在 ARM 分支且 `cfg_wdata_i[30]`（P4M 位）时置位（:491）。

### 关键不变量：研究控制访问碰不到 P4M bank
- P4M bank 是**只读组合字**（dma_observer.sv:169-190），只在 close 时齐活（:401-409）。
- 研究写走 `7'h78`，要求 `index_q >= 512`（:455）；写 `7'h70` 只改 `index_q`（:453），P4M 的 448..463 不受影响。
- 结论：**新控制寄存器放进研究 bank（≥512）结构上不会改 P4M 窗口**。这就是「P4M 窗口不要改变」的机制保证。

### Kernel 侧可复用函数
- 读/写：`idma_research_read`（idma_research.h:46-50）、`idma_research_write`（:52-56）。
- 静止等待：`idma_research_quiet(d, deadline)`（:58-69）。
- 命令+相位校验：`idma_research_command(d, command, expected_phase)`（:71-79）。
- 会话开始/清场：`idma_research_begin`（:81-100，先查 V8 magic、quiet、CLEAR、写 MODE）。
- 「warmup 之前配置」的现成模板：`idma_research_e2_cfg` 在 warmup 之前就 `idma_research_begin`（idma.c 调用于 idma_research.h:596），随后写 `0x30/0x31` 地址（:605-606）、`idma_research_command(…,1,1)`（:607），**然后**才 `idma_run_window(..., r->warmup, ...)`（:613）。这是把「软件在 warmup 前配置」落地的直接参照。
- 「结束后静止恢复」模板：`idma_run_session2` 尾部 `idma_research_quiet(victim, now+1s)` → `idma_research2_method_freeze` → `idma_research_capture_ext`（idma.c:2534-2556；函数分别是 idma_research.h:58-69,264-290,522-533）。
- 数据校验：`idma_verify`（idma.c:756-803）；缓存维护：`local_flush_icache_all()`（idma.c:2376,2515；bench 说明见 :776-779，FENCE.I 兼做 D-cache 失效，属平台 workaround，非通用一致性修复）。**这两者不需要为新控制寄存器改动**。

### 需要扩展的函数/参数（候选，按最小面）
1. `idma_research.h`：新增 bank 相对索引常量（现有模式见 :5-11, :21-36, :44）与配置/冻结/回读函数，参照 `idma_research2_method_configure`（:198-246）、`_freeze`（:264-290）、`_readback_ok`（:292-320）、`_abort`（:248-262）。
2. `idma_research2_caps_ioctl` + `idma_research2_preflight` / `_extra_validate`：加新 feature 位并在 caps 里回显（:826-861, :484-520, :447-482）。
3. `idma_run_session2`：把新配置调用插到 warmup（idma.c:2382-2387）之前、`idma_observation_arm`（:2455）之前；冻结/采集插到 drain 之后、`idma_observation_finish`（:2526）附近，保证 `p4m_finish` 读到的窗口字节不变（`idma_p4m_finish` 只读 bank 448..463，idma.c:1385-1388）。
4. `idma_research_capture`（:102-140）/`_capture_ext`（:522-533）：为新输出字增加校验，但**读取机制不用改**。

---

## 4. rootfs 集成与 kernel/firmware 实际编译命令

### rootfs
- 新增可执行只在 `scripts/rootfs.py:8-19` 的 `EXECUTABLES` 里加一行（名字→`out/bench/...`）；数据文件放 `DATA`（:21-23）。
- repack 的写入位置与权限固定为 `usr/bin/<name>`、`S_IFREG|0755`（rootfs.py:70-72）；数据 `0644`（:74-77）；输出 `out/rootfs.cpio`（:101-103）。
- 冻结 runner 用的板端 CLI 就是 `/usr/bin/idma_experiment`（mac_nonblock_pilot.py:53 默认值），即 rootfs 里 `'idma_experiment': 'out/bench/idma_experiment'`（rootfs.py:12）。**新工具若要从 mac runner 调用，必须先加进 EXECUTABLES 并重打 rootfs。**
- `LEGACY_NAMES` 会清除旧名（rootfs.py:7,65-66,86-87）；新名字不要在旧名集合里。

### bench 编译（新工具）
`scripts/platform.py:156-166`：
```
riscv64-linux-gnu-gcc -O2 -static -Wall -Wextra -I out/bench-uapi <bench/x.c> -o out/bench/x
```
UAPI 头先被拷到 `out/bench-uapi/linux/idma.h`（platform.py:153-155）。

### kernel / firmware（`platform.py:172-201`，无需为新工具改动，只需先 rootfs）
```
# kernel()
cp configs/linux_defconfig  out/linux/.config
linux/scripts/config --file out/linux/.config --set-str INITRAMFS_SOURCE out/rootfs.cpio
make -C linux ARCH=riscv O=out/linux CROSS_COMPILE=<CC> olddefconfig
make -C linux ARCH=riscv O=out/linux CROSS_COMPILE=<CC> -j<jobs> Image
dtc -I dts -O dtb -o out/linux/platform.dtb configs/platform.dts

# firmware()
make CROSS_COMPILE=<BARE_CC> ARCH=rv64 \
     IMAGE=out/linux/arch/riscv/boot/Image \
     DTB=out/linux/platform.dtb TARGET=linux-rv64-cva6     # cwd out/lloader
make -C opensbi O=out/opensbi CROSS_COMPILE=<CC> \
     PLATFORM=fpga/ariane FW_PAYLOAD=y \
     FW_PAYLOAD_PATH=out/lloader/linux-rv64-cva6.bin -j<jobs>
```
- `CC`/`BARE_CC`/`RISCV` 来自 `local.json`（platform.py:16-20，默认 `riscv64-linux-gnu-`、`riscv64-unknown-elf-`）。
- `platform.py software` = `kernel()` + `firmware()`（:382）；`rootfs()` 内部先 `bench()`（:168-170）。

### 冻结 runner
- `scripts/mac_nonblock_pilot.py` 是**主机侧**脚本，不放进 rootfs。它在运行期把静态 reader ELF 通过 UART heredoc 上传到板端 `/tmp`（:1164-1332），ABI `idma-nonblock-window-v1`（:123）。
- **未提供**：reader ELF 的构建规则（文件内自述「The ELF is built later」，:121-122）。这是 rootfs/编译链的缺口。
- runner 有硬门槛 `EXPECTED_UNAME_R='6.5.0-rc2-00093-gc0f5e13ea81f'`（:64）；一旦内核源码变（改了 UAPI/驱动重编），`uname -r` 变化会让 runner 直接拒跑（:1098-1100）。这是「冻结 runner + 内核改动」的显式冲突点。

---

## 5. 新 ABI 输出字（lookup/hit/miss、fast、mode、generation）能否用现有 read index 读

**结论：能（读机制层面），但要扩校验层。**

- 读机制：`writeq(512+index, 0x70); readq(0x78)`（idma_research.h:46-50）。RTL 对 `index>=512` 返回 `research_data_i`（dma_observer.sv:162），`0x70` 可写到 `<8192`（:450,453），`research_index_o` 为 13 位（:91）→ 研究 bank 相对索引 0..7679 全可寻址。
- kernel 整块抓取：`IDMA_RESEARCH_RAW_WORDS=4096`（uapi:214），`idma_research_capture` 循环 `raw[i]=idma_research_read(d,i)` 后整块 `copy_to_user`（idma_research.h:116-134）。所以新增输出字只要落在研究 bank，**现有 read index + raw dump 即可读出**，无需新寄存器读口。
- 现有「lookup/hit/miss/fast」布局参照（helper 侧，非 kernel）：
  - 16 字窗口：`READER_MAGIC`(0)、`version`(1)、`mode`(2)、`frozen flags`(3)、`sequence`(4)、`cycles`(5)；
  - `READER_FAST_WORDS=(6,7,8,9,10)` = Vread/Vwrite/Aread/Awrite/Other；`READER_NORMAL_WORDS=(11,12,13)` = V/A/Other；`[14]=accepted 总和`；`[15]=errors`（mac_nonblock_pilot.py:124-131,485-489）。
  - 可选诊断 bank 16 字，`[15]=fast_accepts=sum(core fast 6..10)`（mac_nonblock_pilot.py:136-141,557-568）。
- 校验层必须同步扩展（否则新字被忽略或被判非法）：
  - kernel：`idma_research_capture` 里对 `raw[0xc00]`、`raw[0xd00]`、`raw[6]`、`raw[0xc19]` 等硬编码校验（idma_research.h:118-132）；
  - `idma_research2_extra_validate` 对 E2 timing/cost 固定值的校验（idma_research.h:447-482）；
  - runner：`validate_helper_window` / `validate_helper_diagnostic`（mac_nonblock_pilot.py:451-569）。
- **generation / mode**：现成的「代际」载体是 `o->sequence`（idma.c:976，每 observation +1），以及 P4M `window[1]`/观察 `observation[0]` 的周期计数（idma.c:1385-1396）。新增 mode/generation 字可放研究 bank 新索引，并让 runner 校验其单调性——参照已有的 `previous_seq` 递增检查（mac_nonblock_pilot.py:508-510）。
- **P4M 与旧 HPM 的边界（重要）**：不要用旧 HPM 当「N」的有效指标。uapi 明确「HPM 能力是初步的，不是板级标定」（uapi:21-22）；runner 把 HPM 标为「raw-only, incomplete under nonblock; not a validated metric」，并且 pilot 指标只剩 `v_window_cycles` 与 `a_completed_jobs_window`（mac_nonblock_pilot.py:21-23,418-430,1636-1639）。新 N 指标必须由新的缓存输出字（本研究 bank）承载，而不是复用 HPM 采样。

---

## 6. 最小需修改文件清单 + 接受测试建议

### 最小文件清单（按依赖顺序）

| 文件 | 改动（最小面） | 依据 |
|---|---|---|
| `linux/include/uapi/linux/idma.h` | 新增 ioctl cmd（建议 0x11）+ 新请求结构（携带 V gap / reverse / 新控制寄存器配置）；新增 cap feature 位常量；**不改**现有 struct/reserved 契约 | uapi:187,193-194,253-254 |
| `linux/drivers/misc/idma.c` | 新 ioctl handler（仿 `idma_run_p4m`:2618-2636）；`idma_run_session2` 增参以传 V gap / reverse / 控制寄存器并使 `cfg` 非 NULL；把 `idma_gap_program_readback(v, …)` 从 0 改为参数（:2430）；新校验函数 | idma.c:2430,2338,2629 |
| `linux/drivers/misc/idma_research.h` | 新 bank 索引常量；新配置/冻结/回读/cap 函数（仿 :198-320,:826+）；capture 校验扩展 | dma_observer.sv:91-94,162；idma_research.h:46-56,102-140 |
| `cva6/corev_apu/fpga/src/dma_observer.sv`（或新 nonblock RTL） | 新控制寄存器与缓存输出字、mode/generation；`research_data_i` 扩位；保持 P4M bank 448..463 组合只读不变 | dma_observer.sv:162,169-190,449-455 |
| `scripts/platform.py` | `bench()` 加一行编译新工具（:156-166）；如涉 RTL 参数，新增 profile 并复用 `fpga()` 参数替换（:254-263） | platform.py:156-166,244-263,347-348 |
| `scripts/rootfs.py` | `EXECUTABLES` 加新工具（:8-19） | rootfs.py:8-19,70-72 |

（可选的 board 脚本 `tests/board-*.sh` 未随附，无法给出改动。）

### 建议接受测试（我不能执行，仅提案）
1. **ABI 向后兼容**：旧 `IDMA_IOC_RUN_P4M`(0x0d) + 旧 `struct idma_p4m`（reserved 全 0）仍成功；旧结构里塞非 0 reserved 仍 `-EINVAL`（对 idma.c:1301 回归）。
2. **新 cap 位**：`IDMA_IOC_EXPERIMENT_CAPS`(0x06) 或 `IDMA_IOC_RESEARCH2_CAPS`(0x10) 回显新 feature 位（对照 idma_research.h:458-460）。
3. **V gap 生效与窗口合法性**：新 ioctl 传 V gap>0 时，`p4m_finish` 仍要求 `observation[7]==observation[8]==victim_transfers` 且守恒/闭窗（idma.c:1406-1411,1397-1411）；A 守恒不受影响。失败应显式 `-EIO`，绝不静默。
4. **研究寄存器不扰动 P4M**：同一 run 中配置新控制寄存器前后，P4M bank 448..463 字节与 `p4m_cycles` 不变；`observation_status==4`（idma.c:1391）。
5. **静止恢复**：结束路径 `idma_research_quiet` 在 1s 内返回 0，否则隔离/quarantine（idma.c:2534-2544）；`dmesg` 无 WARNING/BUG（runner 的 `WARNING_PATTERNS`，mac_nonblock_pilot.py:171-178）。
6. **校验/缓存不变**：`idma_verify` 与 `local_flush_icache_all` 调用点（idma.c:2376,2515,756-803）行为不变。
7. **runner 层面**：`validate_record`/`validate_helper_window` 对新字扩展后，`mode` 与 `generation` 单调性检查通过（mac_nonblock_pilot.py:338-415,451-511）。
8. **构建链**：`platform.py bench → rootfs → kernel → firmware` 全绿，manifest 产出（platform.py:323-342）。

---

## 7. 明确未知（无法从随附文件判定）

1. **V lane 的 gap 硬件语义**：gap 周期是否计入 observer 的 `active` 周期（`count_n[0]`，dma_observer.sv:130,369），以及 BatchPort 是否对 victim lane 实现 gap。gap 属 DMA RTL，未提供。
2. **「非阻塞/缓存」RTL 本体**：lookup/hit/miss、fast、mode、generation 这些输出字在当前 `dma_observer.sv` 里**不存在**（该文件只有 ResearchEnable 通道，无缓存计数）。它们来自 `configs/migration-4dma-n-*.json` 所指的新 nonblock 设计，未随附。
3. **reader ELF 的构建规则**：`mac_nonblock_pilot.py:121-122` 明说「built later」，无编译命令；其 ABI `idma-nonblock-window-v1` 与 `READER_MAGIC` 只定义了契约，无实现。
4. **RTL 回归目标**：`platform.py:283-304` 的 `hpm-test` 引用 `tests/hpm_test_top.sv`/`tests/hpm_test.cpp`/旧 commit `7fbb2b99…`，这些文件未提供，无法给出针对新观察者/缓存的回归命令。
5. **写 INDEX 于运行中是否安全**：RTL 范围检查（dma_observer.sv:450,453）不查 `busy`，是否允许在 P4M 已开始后改索引、以及是否需要在 Kernel 强制「ARM 前配置」由驱动侧纪律决定；文件未给结论。
6. **`RESEARCH_MODE`/`RESEARCH_PHASE` 的完整取值表**：`idma_research.h:5-11` 只给了本会话用到的常量（MODE=4、COMMAND=5…），新模式的合法相位序列需新 RTL 定义。
7. **冻结 runner 与内核改动冲突**：`EXPECTED_UNAME_R`（mac_nonblock_pilot.py:64）在重编内核后必然失配，未给出更新/绕过路径。
8. **HPM 是否可作为 N 的任何部分**：明确不可当有效 N 指标（uapi:21-22；mac_nonblock_pilot.py:21-23,429,1636-1639），但「N 的替代指标应精确是什么」需由新 ABI/RTL 定义——随附文件未定义。

---

## 8. 复用接口一览（避免重造）

- 读/写研究 bank：`idma_research_read/write`（idma_research.h:46-56）。
- 静止判定：`idma_research_quiet`（:58-69）；命令-相位握手：`idma_research_command`（:71-79）。
- 会话生命周期：`idma_research_begin`（:81-100）、`idma_research_capture/_ext`（:102-140,522-533）、`_freeze/_abort`（:264-290,248-262）、`_pre_arm`（:322-339）。
- 能力/回读：`idma_research2_preflight`（:484-520）、`_extra_validate`（:447-482）、`_method_readback_ok`（:292-320）、`idma_research2_caps_ioctl`（:826+）。
- 下 gap：`idma_gap_preflight`（idma.c:1344-1353）、`idma_gap_program_readback`（:1355-1370）。
- 观测窗口：`idma_observation_prepare/arm/finish`（idma.c:970-1007,1009-1038,1041-1144）。
- P4M 收尾校验：`idma_p4m_finish`（idma.c:1372-1478）。
- 批处理配置/停止/采集/关闭：`idma_batch_configure/stop/collect/disable`（idma.c:806-864）。
- 缓存 workaround：`local_flush_icache_all()`（idma.c:2376 等；含 bench 注释 :776-779）——**不要为新功能新增按传输 flush**。

---

## 9. 窄实施草案（伪代码级，不重写源码）

```
// 1) uapi 新增（不改旧结构）
#define IDMA_IOC_RUN_P4M_V2 _IOWR('i', 0x11, struct idma_p4m_v2)
struct idma_p4m_v2 {
    struct idma_experiment experiment;   // 复用现有校验
    __u32 aggressor_gap_cycles;          // 旧语义
    __u32 victim_gap_cycles;             // 新
    __u32 flags;                         // bit0 = reverse A
    __u32 reserved[5];                   // 必须全 0，沿用 memchr_inv 校验
    /* 以下同 struct idma_p4m 的输出区 */
    ...
};

// 2) idma.c
static long idma_run_p4m_v2(dev, uarg) {
    copy_from_user; clear outputs;
    validate (新函数，内部仍调用 idma_p4m_validate 对 experiment 部分);
    要求 no_aggressor => victim_gap_cycles==0 && reverse==0;   // 与 1310 一致
    build cfg-like { reverse } ;                                  // 使 a->reverse 可达 (2338)
    return idma_run_session2(victim, &p.experiment, /*batch*/true, NULL, NULL,
                             false, &p, /*research*/NULL, &cfg_like);
}
// session 内：
//   warmup 段保持不变 (2382-2387)
//   batch configure 不变 (2423-2425)
//   idma_gap_program_readback(v, p->victim_gap_cycles);   // 替换 2430 的常量 0
//   idma_gap_program_readback(a, p->aggressor_gap_cycles); // 2432-2433 不变
//   cfg_like.reverse 经 2338 生效
// 新控制寄存器（若需要）：
//   位置：warmup 之前（对齐 idma_research_e2_cfg 的 :596-607 模式）
//   结束：drain 后 quiet -> freeze -> capture（对齐 2534-2556）
//   绝不触碰 448..463（idma_p4m_finish 只读该段:1385-1388）

// 3) idma_research.h：新增 bank 相对索引常量 + 配置/冻结/回读函数（仿 METHOD 块）
// 4) dma_observer.sv / 新 RTL：research_data_i 扩字，P4M bank 组合只读不变
// 5) platform.py bench() + rootfs.py EXECUTABLES：编译并打包新工具
```

**验收关键**：旧 `IDMA_IOC_RUN_P4M`(0x0d) 行为逐字节不变；新 ioctl 仅在 V gap/reverse 上扩展；P4M 窗口字节不变；失败一律显式 errno，绝不静默。
## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: validate（原始碼與既有記錄核查，不重跑實驗）
- Origin Date: 2026-10-18 JST
- Verification Status: ANALYZED
- Version Label: e2_irq_diagnosis_v1
- 執行：Codex-only；本席沒有 DeepSeek 呼叫，也沒有 UART/JTAG/SD、核心或 RTL 修改、建置或上板操作。

# E2 N4_A8：timer 再入與 CPU 進展的有界診斷

**可接受的結論是「實際發生大量 hrtimer 處理後重編程仍已過期，並在 softirq 入口剛開中斷處被 S timer IRQ 再次打斷」；尚不能裁定最上游原因為 CPU/AXI 壅塞、timer 比較器失效，或特定 callback 耗時。** 單純將驅動 `cond_resched()` 改得更頻繁沒有消除第六點故障。不得把軟體卡住的數十秒寫成 V 的硬體服務時間。

Root 已於同 boot 做一次有限只讀診斷後重載其他 BIT；本席不要求保留板卡等待，也不啟動另一個板點。以下「下一診斷」只是限定 proposal，Root 決策與執行。

## 1. 先看哪些原件

| 原件／產物 | 內容與界限 |
|---|---|
| [snapshot-manifest.json](snapshot-manifest.json) | 每個只讀快照的來源、UTC cutoff、byte 數與 SHA256；活動檔案未寫入 |
| [00-uart.raw.log](00-uart.raw.log) | 新輪前五點及第六點七次 watchdog；2026-10-17T15:42:10.433061Z 固定讀取既有 141683 bytes；原檔最後修改約15:41:05Z |
| [01-manifest.json](01-manifest.json)、[02-records.json](02-records.json) | 新輪來源與 failed/5 cases；`runtime_commits` 才是實際軟硬體，`selected_source` 是舊硬體包來源 |
| [舊失敗 stdout](03-command-fa2f354e990b4f75a77692b55a203131.stdout.txt) | 舊 acece 軟體第六點完整 CLI 與 dmesg；原 JSON 同目錄保存 |
| [舊事後診斷 stdout](04-command-9429e883ecfa47c19d522365d5392663.stdout.txt) | 舊 boot uptime282.45s、timer21082、IOMMU IRQ0；不是故障期間的差分 |
| [新有限只讀診斷 stdout](console02/command-0514e2d6078b4acf9f600372033bbaf1.stdout.txt) | Root 一次 CTRL-C 後的兩組 `/proc`；原 UART byte、command JSON 與 recovery.json 均在 console02/ |
| [source-manifest.json](source-manifest.json) | 本席直接以固定 commit 匯出的40份必要原始碼；source-snapshot/ 保持各原檔行號 |
| [calculations.json](calculations.json)、[analyze.py](analyze.py) | 離線數值、七個 EPC/cause/pending、快照完整性驗證；可直接重算 |

新 boot `5df16afb-ce70-4098-a71f-4ff462079c1e`，核心 `6.5.0-rc2-ispass2027-full-e3-v1`。Linux `65b427946152e3aca27babab0a437821729805d4`；實際 CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069`、IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`；OpenSBI `e8d21bfdc2dd753314627968e496c3a1188f6dfb`；DS0 BIT SHA256 `e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6`。build-manifest 的 CVA6 `1d587...` 只是軟體建置引用，不能拿它替代已載入 BIT 的8aac版本。

## 2. 已經足以成立的觀測

### 2.1 EPC 是開 SIE 後、呼叫任何 softirq action 前

新 vmlinux 的實際反組譯 [new-softirq.disasm](new-softirq.disasm)：

```text
ffffffff808a4dc8  lw    s11,0(a3)      # 讀取 local_softirq_pending()
ffffffff808a4e10  sw    zero,0(a4)     # 清 pending
ffffffff808a4e14  csrsi sstatus,2      # local_irq_enable()
ffffffff808a4e18  sext.w a5,s11        # watchdog 的 EPC
```

來源：[kernel/softirq.c:527](source-snapshot/linux/kernel/softirq.c#L527)、[同檔:533](source-snapshot/linux/kernel/softirq.c#L533)；`h->action(h)` 在553行，時間／重啟上限檢查更晚在569–577行。故此 PC **不是正在執行 TIMER 或 RCU softirq handler 的證據**，也不是 CPU 正在執行 DMA MMIO/fence 的證據。

新 raw 的七個報告入口行為1266、1289、1312、1334、1356、1380、1403；每組都是 EPC `ffffffff808a4e18`、cause `8000000000000005`、s11=`2`，即被 S timer IRQ 打斷，該次softirq迭代已取出的 pending 僅為 TIMER bit（可能是首次進入或restart）。例：raw1270–1287。`badaddr=000d879b` 正好等於該 EPC 指令編碼，不能當成一次 uaccess/page-fault 地址；cause 明確是中斷。

七次 current 分別為 idma_scaling174（兩次）、kthreadd2、klogd60、migration/0 15、kworker/0:1 9、kworker/0:2 175，tp 隨之改變。它們是被中斷的 current，不是七個已證實的根因。至少有跨 task 進展，故「idma 唯一迴圈永遠不讓出 CPU」與「CPU 完全停止執行指令」都不能解釋全部觀測。長時間偏向 IRQ 的 CPU 進展不足仍未排除。

watchdog 在 hard hrtimer callback 內以 `get_irq_regs()` 印被中斷的 regs，並檢查排程側餵狗時間：[kernel/watchdog.c:426](source-snapshot/linux/kernel/watchdog.c#L426)、443–464、515–530；不是以「被印出的函式必為耗時根因」作檢測。

### 2.2 不是只有一行模糊警告：已有累計 retry/hang 原始數據

新輪第一個合格點就出現 `hrtimer: interrupt took 13511600 ns`（raw15）；dmesg 後續重印相同 timestamp，不應重複計數。13.5116ms 是這一次 hrtimer interrupt 的整段處理與重試時間，為4ms tick的3.3779倍；不是每個 callback 的耗時。舊輪第一點也已有13.93168ms警告（舊 stdout202）。

Root 的 console02 兩次真實輸出提供：

| 欄位 | 第一次 | 第二次 | 差值 |
|---|---:|---:|---:|
| timer_list `now` (ns) | 594413723840 | 596086255520 | 1672531680 |
| timer IRQ | 31768 | 31941 | 173 |
| hrtimer `nr_events` | 31579 | 31752 | 173 |
| `nr_retries` | 32547 | 32547 | 0 |
| `nr_hangs` | 10836 | 10836 | 0 |
| `max_hang_time` (ns，原樣) | 2191806800 | 2191806800 | 0 |
| jiffies | 4295040901 | 4295041319 | 418 |
| context switches | 15754 | 15903 | 149 |

精確入口：console stdout3/159（IRQ）、35/191（now）、104–107/260–263（hrtimer）、122/278（jiffies）、27/183（ctxt）。timer_list 確實有輸出；不是僅檔案存在的推測。

在實際原始碼 [hrtimer.c:1827](source-snapshot/linux/kernel/time/hrtimer.c#L1827)–1872，只有 `tick_program_event(expires_next,0)` 返回過期失敗才增加 `nr_retries`，第三次失敗增加 `nr_hangs` 並向未來退避。故10836次 hang 證明發生過大量「處理完再計時時 next expiry 已過」；不僅是 softirq 符號。`max_hang_time` 是32-bit cast後紀錄的整段 hrtimer wall duration，包含 callback、clocksource/陷入與可能的 watchdog 輸出；不能把2.1918s寫成單一 MMIO 延遲，也不能當作無截斷的全 boot 最大值。

418 / 1.67253168 = **249.92053 jiffies/s**；兩個 now 之間 idle_sleeptime 增0.99078392s。恢復後的短窗吻合HZ250、NO_HZ_IDLE停 tick 的行為，且 hang/retry不再增加；不支持持續的固定倍率或永久黏住 timer。這些 `/proc` 欄位循序讀取、並非原子快照；now與jiffies同源，因此不能充當對主機真實秒的獨立25MHz校準。累計10836次也不能全歸屬第六點，因為第一點已有警告、無第六點之前的counter快照。

`/proc/stat` hardirq欄為0不代表沒有IRQ：實際配置 `CONFIG_TICK_CPU_ACCOUNTING=y`、`CONFIG_IRQ_TIME_ACCOUNTING` 關閉，且長時間在softirq入口被嵌入timer時會影響分類。此處不拿CPU百分比判定根因。

### 2.3 舊失敗的硬體與軟體窗口確實分離

舊 stdout321：V `13105262/50e6 = 0.26210524s`；CLI `victim_elapsed_ns=23013559440`，即23.01355944s；硬體 `done_to_read=1136467216/50e6=22.72934432s`。三 A 分別完成451521、450239、450219個4KiB COPY。舊點最後STOP/drain/data/masks通過，但taint16384仍不接受。新失敗沒有同等完整的第六點 STOP/drain/data 回執，不能把舊輪收尾結果移植給它；CTRL-C後看到prompt也不等於 DMA quiet。

## 3. 實際 timer／CPU／I/O 呼叫鏈

### 3.1 Hart、時基及中斷路由

- Linux DTS [platform.dts:14](platform.dts#L14) 為25MHz，CPU0 hart0、clock-frequency50MHz；`riscv,isa` 沒有sstc。Linux [time.c:26](source-snapshot/linux/arch/riscv/kernel/time.c#L26) 直接把該 timebase 註冊給clocksource與clockevent。[timer-riscv.c:141](source-snapshot/linux/drivers/clocksource/timer-riscv.c#L141) 將 RV_IRQ_TIMER 映射到hart-local控制器，實際 `/proc/interrupts` 是 Linux IRQ11、local cause5；不是PLIC的IOMMU IRQ13/14，也不是DTS的APB timer@18000000。
- OpenSBI [platform.c:28](source-snapshot/opensbi/platform/fpga/ariane/platform.c#L28)–60為一hart、25MHz、CLINT base02000000、64-bit MMIO、first_hartid0；mtimecmp=02004000，mtime=0200bff8。
- 固定 RTL [ariane_xilinx.sv:708](source-snapshot/cva6/corev_apu/fpga/src/ariane_xilinx.sv#L708)–759：hart_id0，rtc每CPU clock翻轉，CLINT用其上升沿計數，故50MHz/2=25MHz。[clint.sv:92](source-snapshot/cva6/corev_apu/clint/clint.sv#L92)、184–207為mtime遞增與 `mtime>=mtimecmp` level comparator。[historical-build-excerpts.txt](historical-build-excerpts.txt) 保存實際已建 BIT 50MHz時鐘與WT-cache綜合原句。
- 核心config是PREEMPT_NONE、HZ250、NO_HZ_IDLE、high-res，且實際timer_list `hres_active=1`、clockevent handler=`hrtimer_interrupt`、mode3、min_delta_ns4001。tick_sched_timer以TICK_NSEC前移：[tick-sched.c:1478](source-snapshot/linux/kernel/time/tick-sched.c#L1478)–1502。250Hz不是timer在idle也必須每秒產生250次IRQ的保證。

目前三層靜態時基相合。沒有原始證據支持「DT把CPU50MHz錯當mtime50MHz」或「錯hart的比較器被寫入」。不做新的時間配置修補。

### 3.2 `rdtime` 實際需要 M-mode 模擬及 AXI MMIO

Linux [timex.h:51](source-snapshot/linux/arch/riscv/include/asm/timex.h#L51)–68 `get_cycles64()`讀CSR_TIME；[timer-riscv.c:66](source-snapshot/linux/drivers/clocksource/timer-riscv.c#L66)–81將同讀法用於clocksource/sched_clock。固定 CVA6 [csr_regfile.sv:427](source-snapshot/cva6/core/csr_regfile.sv#L427)–435僅實作cycle/instret等，沒有CSR_TIME；501行unknown CSR產生read_access_exception。

OpenSBI [sbi_illegal_insn.c:35](source-snapshot/opensbi/lib/sbi/sbi_illegal_insn.c#L35)–78模擬CSR，`sbi_emulate_csr_read(CSR_TIME)` [sbi_emulate_csr.c:69](source-snapshot/opensbi/lib/sbi/sbi_emulate_csr.c#L69)–78再讀timer value；[sbi_timer.c:47](source-snapshot/opensbi/lib/sbi/sbi_timer.c#L47)–49、84–88、144–174選用platform timer，最後到 [aclint_mtimer.c:24](source-snapshot/opensbi/lib/utils/timer/aclint_mtimer.c#L24)–31、54–60的64-bit CLINT讀取。實際fw ELF [opensbi-timer.disasm](opensbi-timer.disasm)也顯示rd64最後是`ld a0,0(a0)`，wr64是`sd a1,0(a2)`。

所以 `ktime_get_ns()`、sched_clock、timer處理中的多次時間讀取並非獨立、廉價的原生TIME CSR。它們會做特權陷入與CPU AXI存取，這是**可見的耦合路徑**；尚無這次故障的每次成本或AXI等待量。

### 3.3 Timer mask、rearm 與「過短的相對時間」缺口

Linux timer IRQ先清STIE再呼叫event_handler：[timer-riscv.c:118](source-snapshot/linux/drivers/clocksource/timer-riscv.c#L118)–125。rearm先用`rdtime+delta`算next_tval，設STIE，再SBI TIME ecall，最後無條件回傳0：[同檔:34](source-snapshot/linux/drivers/clocksource/timer-riscv.c#L34)–50；[new-next-event.disasm](new-next-event.disasm)與[new-riscv-timer.disasm](new-riscv-timer.disasm)是實際vmlinux。

OpenSBI先MMIO寫mtimecmp，再清STIP、開MTIE；機器timer IRQ則關MTIE、設STIP：[sbi_timer.c:124](source-snapshot/opensbi/lib/sbi/sbi_timer.c#L124)–137。這說明沒有「Linux從不清STIP」的直接錯誤；但仍無載入後比較器/MTIP/STIP隨時間的直接讀回。

clockevents在call前用`expires-ktime_get()`判斷是否已過期；最小事件約4.001us：[clockevents.c:326](source-snapshot/linux/kernel/time/clockevents.c#L326)–336、233–249。RISC-V set_next_event沒有call後逾期檢測，若陷入、MMIO或返回耗時已超過所給delta，仍回傳成功。**這是一個需量測的具體可驗證假說，不是本報告發現的已證實驅動bug。** 同樣，nr_hangs本身證明的是call前已過期的重試，不能證明每次SBI rearm都超時。

### 3.4 CPU、DMA壓力與 fence/cache 能說到哪裡

CPU I$/D$/PTW先經WT cache與 [wt_axi_adapter.sv:94](source-snapshot/cva6/core/cache_subsystem/wt_axi_adapter.sv#L94)–122 的CPU內部兩路RR，再到top slave0；IOMMU_DS與completion另占top slave，最後共用top xbar與DRAM：[ariane_xilinx.sv:157](source-snapshot/cva6/corev_apu/fpga/src/ariane_xilinx.sv#L157)、287–315、723–724、842–845；四DMA先匯到IOMMU：[ariane_peripherals_xilinx.sv:868](source-snapshot/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv#L868)、1005–1014。CLINT是另一個xbar目的端，但M-mode堆疊、指令miss、CPU write-through等仍會使用DRAM，不能由「CLINT不是DRAM」推出timer服務不受壓力影響；也不能單憑共用xbar就聲稱CPU被仲裁餓死。

Linux `readq()`後有`fence i,r`、`writeq()`前有`fence w,o`：[mmio.h:133](source-snapshot/linux/arch/riscv/include/asm/mmio.h#L133)–148；OpenSBI timer用的是relaxed 64-bit I/O，沒有這些顯式fence。CVA6 FENCE commit等待LSU store queue與D-cache writebuffer空：[cva6.sv:564](source-snapshot/cva6/core/cva6.sv#L564)–579、[commit_stage.sv:218](source-snapshot/cva6/core/commit_stage.sv#L218)–221；**WT配置的普通FENCE不清整個D-cache**，[controller.sv:82](source-snapshot/cva6/core/controller.sv#L82)–94有明確ifdef。FENCE.I另會flush/cache-drain，不能把兩者混同。

multi pending輪每次先ktime_get_ns、再idma_poll中readq(DONE)及時間讀取，然後才cond_resched：[idma.c:947](source-snapshot/linux/drivers/misc/idma.c#L947)–968、638–647。頻繁yield不保證CPU能越過即時再入timer，也不會讓硬體自主A停下。ioctl的uaccess只在請求入口copy_from_user與收尾copy_to_user：[idma.c:1838](source-snapshot/linux/drivers/misc/idma.c#L1838)、1997；本批報告cause5而非page/access fault，未發現uaccess錯誤的正證據。這不構成全面排除記憶體損壞。

## 4. 假說的接受／否決狀態

| 假說 | 本次裁決 | 尚缺什麼才能判因 |
|---|---|---|
| CPU完全不執行／單一idma迴圈獨占，增加yield即可充分修復 | 否決其充分解釋；七樣本跨task，65b策略仍失敗 | 不排除CPU可用進展被timer/記憶體壓力壓低 |
| 高解析timer頻繁超過next expiry、IRQ再入妨礙softirq入口進展 | 現象已支持：10836 hangs/32547 retries，七cause5與同EPC | 根因仍需把handler、rearm、CPU記憶體等待分開 |
| 固定timebase/HZ倍率錯誤、錯hart | 靜態證據與恢復短窗反對；不作修補 | 缺獨立主機秒校準；不能排除只在故障窗的clocksource異常 |
| 永久STIP不清／永久timer硬體黏住 | 源碼正常清/重設；CTRL-C後hang不再加，反對永久故障 | 缺故障窗mtimecmp/MTIP/STIP的direct trace |
| CPU AXI/DRAM/uncached MMIO/fence進展受三A壓力限制，令timer超period | 具體可行且存在實際共用路徑，未證實 | 無CPU req→grant→response、retired/cycle、SBI成本測量 |
| 特定timer callback過長或watchdog輸出放大 | 尚可；hrtimer queue與printk在hardIRQ | 需callback與首次故障前時間；後來watchdog輸出不能解釋首個warning/首次stall |
| IOMMU fault/command IRQ storm | 舊診斷及新兩次對應IRQ計數皆0，不支持 | 這不排除無IRQ的IOMMU/DDR壓力；也沒有全故障窗其他IRQ差分 |
| Softirq TIMER/RCU action自身無限迴圈 | EPC在action前，沒有正證據；不能由栈符號判定 | 需action entry/exit或其他PC trace；本次不加猜測修補 |

## 5. 下一次最小必要診斷：先保留現有結論，若Root需判因才做一個限定包

**已完成的最小方案**就是Root一次CTRL-C後的兩組只讀`/proc`；它已得到真正可用的rearm/hang數字。不要為再取相同累計值重回E2，也不要在已換BIT後把不同boot的值補成同窗證據。

若Root之後決定必須區分「SBI rearm太晚」與「handler本身太慢」，只提議以下一個小診斷核心包，**本席未實施、未編譯、未授權新板輪**：

1. 不改UAPI、DMA配置/預算、HZ、PREEMPT、watchdog、taint、STOP與drain。僅在`riscv_timer_interrupt()`累計入口數與event_handler前後的**原生cycle/instret差值**，保留sum/max及少量最近記錄；不要在IRQ內printk、配置trace framework或額外使用ktime/rdtime。
2. 在`riscv_clock_next_event()`重用本來就讀的raw time，記錄`raw_now, delta, next_tval`及SBI call前後原生cycle差；另外少量記錄返回後SIP/STIE，不能以STIP=1單獨判定錯誤，因為事件可能已合法過期。
3. 既有timer_list before/after負責nr_hangs差分。低壓與N4_A8只在Root之後授權的同一個有界診斷包中採集，且不算正式論文效能點。callback長尾若占主因才加第二層拆分，不預先鋪大量probe。

輸出只在Root已確認正式STOP/drain流程完成後，以一個獨立read-only節點讀出；若故障而未收尾，先保留原始watchdog與UART，不在IRQ或失敗timeout回呼內大量列印。ring最多256條固定大小記錄，滿後不覆寫關鍵首批事件；常態只保留少量整數sum/max/count。不得每輪新增rdtime/MMIO讀取、動態分配、鎖、fence或printk。實作前以反組譯核定新增CSR及store數，原生counter不允許或成本超界就停止，不降級成trap讀取。先以同配置低壓控制檢查擾動：若新增probe令控制窗口偏移超過預定1%或自身引入hang，該包不能用來定因；1%是proposal接受條件，並非已量得的上界。

一次有限板時proposal最多同boot一個N2_A1控制與一個N4_A8診斷，不重跑矩陣、不替換失敗點；每點沿用既有120s外層硬上限與60000ms核心預算，連同正常啟動／收尾安排為單次不超過10分鐘時段，失敗不自動重試。即使timeout不能證明硬體quiet，仍由Root沿既有保留／恢復流程處置，不等候額外probes。這是未執行的change proposal；不阻擋現成E4/E5。

原生cycle/instret硬體實作見[csr_regfile.sv:428](source-snapshot/cva6/core/csr_regfile.sv#L428)–435、530–538；正常FPGA配置ENABLE_CYCLE_COUNT=1，[ariane_pkg.sv:255](source-snapshot/cva6/core/include/ariane_pkg.sv#L255)–266。probe需先核對S-mode counter權限與指令成本，避免把trap模擬的counter當成低侵入量測；本次未量測probe成本。

| 測量結果 | 可以接受的結論 | 必須否決的過度結論 |
|---|---|---|
| 故障窗大量`SBI_elapsed_cpu_cycles >= 2*delta`（50MHz/25MHz），且返回已pending；handler其他部分不占大頭 | rearm call耗掉已設定的relative interval，可直接解釋立即再入；需保留次數/分布而非只選max | 仍不能只由此叫作AXI starvation；需再定位SBI內部MMIO/指令miss成本 |
| SBI成本遠小於delta，而event_handler完整耗時及nr_retries/hangs劇增 | 優先查callback／clocksource讀取／共用記憶體所花時間 | 不得未量就改timer period或關高解析timer |
| IRQ內instret/cycle大幅下降，且壓力消失後恢復；同時rearm/handler wall cycle上升 | 支持CPU服務進展下降 | 不能在沒有CPU AXI waiting證據時區分AXI、cache miss或其他pipeline stall |
| 與此前rearm一致的足夠future比較值，在不足delta elapsed時已pending／timer重入，且time/cycle比例已獨立驗證 | 才值得集中查timer write ordering/compare/STIP硬體路徑 | 不得拿讀值已過期、stale前一事件或不同clock-domain數值當失效 |
| 新probe版本不再故障、或沒有正式故障窗樣本 | 診斷未定；記錄觀測擾動 | 不宣稱根因消除或probe本身是修復 |

若只能做一個追加讀取，先取得原生cycle與rdtime對**主機elapsed**的頻率比例，也必須由Root放入授權診斷包；當前兩次timer_list的249.92Hz是同源一致性，不是獨立時鐘驗證。

## 6. 驗證、未解項與交接

本席直接讀固定關鍵原始碼、實際vmlinux/fw ELF、舊CLI及新原始UART；`python3 analyze.py`驗證所有快照長度/SHA256、40份來源檔、七組EPC/cause/pending及計算。沒有對已完成板測做重複驗收，沒有重跑內核測試、建置或實驗。報告為ANALYZED，非已重現根因／已驗證修復。 新撰報告、分析程式及JSON的whitespace檢查通過；原UART的CR/尾端空白與固定來源檔既有空白原樣保留，未為diff-check改寫證據bytes。

ARS統計誤謬檢視11/11：Simpson、生態推論、Berkson、collider、base-rate、regression-to-mean均不適用於本次無統計模型的確定算術；survivorship以保留失敗點處理；look-elsewhere與forking-paths不作顯著性聲稱；correlation/causation與reverse-causality保留為核心限制——timer hang、CPU壓力與watchdog輸出可互相放大，不能只選一個相關觀測定因。

尚缺：故障期間CPU AXI延遲、原生retired/cycle、SBI rearm latency、具體callback耗時、timebase對主機秒的獨立比對、新失敗第六點的完整STOP/drain/data回執。這些缺項不能用後來CTRL-C成功、換BIT後成功、或其他boot的合格點倒填。

Root下一步可直接用第2節的已證實事實更新主診斷，並獨立決定是否值得採用第5節的有限proposal。本分支只寫本目錄，未改共享README/CURRENT_STATUS/資料表/Root主診斷。

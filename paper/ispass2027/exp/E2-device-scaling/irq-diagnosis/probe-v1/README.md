# E2 timer probe v1：實際 patch，等待 Root Image 驗收

Linux worktree：`/home/xvhaocheng/work-e2-irq-diagnosis-linux`，branch `ispass/e2-irq-diagnosis`。
基底 `65b427946152e3aca27babab0a437821729805d4`；完整終點 **`1facb309758e3d41622355de71dbc033cd0fac97`**，含初版 `eb5776816e784b71952248b452b469ad65fc0bdd` 與 pending-sample 定位修訂。
只改 `drivers/clocksource/timer-riscv.c`；[完整差異](timer-probe.patch)、[完整來源](timer-riscv.proposal.c)、[增量Git bundle](timer-probe-from-65b.bundle)可還原。
未向外部 Linux upstream 推送，未更新平台 gitlink、未建 Image、未操作硬體。實作與有限對象檔編譯均由 Codex 完成，無 DeepSeek。

## 原生 counter 的證據閉環

此處的「native」依據固定已載入硬體與固件的實作及權限，不是只看 `rdcycle` 指令。

| 原始證據 | 所能證明的事 |
|---|---|
| [CVA6 csr_regfile.sv:428](../source-snapshot/cva6/core/csr_regfile.sv#L428)–435、1024 | CSR_CYCLE/INSTRET直接讀cycle_q/instret_q；MCOUNTEREN可讀寫低32位 |
| [同檔:1606](../source-snapshot/cva6/core/csr_regfile.sv#L1606)–1612、1641–1645 | Linux實際HS/S-mode的counter權限由MCOUNTEREN對應bit決定；SCOUNTEREN只限制U-mode |
| [同檔:530](../source-snapshot/cva6/core/csr_regfile.sv#L530)–538、[ariane_pkg.sv:255](../source-snapshot/cva6/core/include/ariane_pkg.sv#L255)–266 | 正常FPGA配置cycle每clock增，instret按實際retire增；包括M-mode執行 |
| [sbi_hart.c:54](permissions/opensbi/lib/sbi/sbi_hart.c#L54)–69、472–485、512–540 | OpenSBI在M-mode偵測MCOUNTEREN存在後寫入-1，初始化呼叫鏈會執行該賦權；固定RTL已實作該CSR，偵測不會因未實作而被略過 |
| [sbi_pmu.c:717](permissions/opensbi/lib/sbi/sbi_pmu.c#L717)–725 | 固定固件另一個MCOUNTEREN寫點也是-1；全lib定向搜尋沒有撤銷S權限的寫點 |
| [實際fw ELF反組譯](firmware-counter-permission.disasm) | `sbi_hart_reinit`在80006ff4載-1、80006ff6寫mcounteren，與固定來源一致 |
| [Linux perf:701](permissions/linux/drivers/perf/riscv_pmu_sbi.c#L701)–733 | Linux PMU啟停寫SCOUNTEREN是調整U-mode存取，沒有將S-mode的MCOUNTEREN關閉 |

[permissions-manifest.json](permissions-manifest.json)保存五份必要新原件的固定commit和SHA256。若換硬體／固件，以上保證必須重新核對；debugfs的counter_contract是來源合同標記，不是假裝S-mode能直接讀回MCOUNTEREN。沒有在板上加一個會trap的M-mode CSR探測。

目前核心 [kernel.config:4825](../kernel.config#L4825) 已 `CONFIG_DEBUG_FS=y`，RV64。實際build DTS根compatible [platform.dts:6](../platform.dts#L6) 是 `eth,ariane-bare-dev`；probe init gate逐字使用它。`model=eth,ariane-bare`是另一個欄位。只在此compatible且無SSTC的配置建立介面；啟停／快照另嚴格拒絕非單online CPU0、非hart0。

## 介面與序列化

- 預設off。`/sys/kernel/debug/riscv_timer_probe/control`為0600；讀取0/1。寫1只允許off時重設後啟用，active時再寫1回`-EBUSY`；寫0凍結，重複寫0保持原凍結快照；其他值`-EINVAL`。
- `/sys/kernel/debug/riscv_timer_probe/snapshot`為0400；active時open回`-EBUSY`。off時每次open在進程上下文獨立`kmemdup`一份，之後讀／seek不受重新enable影響，close釋放。沒有開中斷時邊印邊讀活動state。
- 控制以mutex、cpus_read_lock及get_cpu固定單CPU0；reset只在off時進行，publish enable/freeze時短暫local_irq_save。writer只允許CPU0、hart0、單online CPU、IRQs disabled，因此同hart timer與rearm不會同時改state；IRQ端不拿鎖、不配置記憶體、不印文字。
- `1`須在壓力前、`0`與讀snapshot須在Root按既有流程確認STOP/drain後。介面本身無法知道DMA是否已quiet，不能把freeze成功當drain證明。若壓力尚未退出，不為輸出這份snapshot延長板卡等待。

## 計數點、採樣及可解讀界限

kind1是STIE清除後的event_handler呼叫，kind2是既有SBI set_timer呼叫。兩者各自記錄count、cycle sum/max、instret sum/max；handler包含巢狀rearm及其probe成本，不能將兩組sum相加。起點cycle在instret之前讀、終點instret在cycle之前讀，固定read/call開銷未扣除。

rearm重用原有一次`get_cycles64()`得到的raw_now，保存delta與next_tval。**沒有新增rdtime/ktime/MMIO讀取**。SBI計時起點晚於原raw_now讀取，中間包含原設STIE及probe分支／呼叫，因此`rearm_cycles >= 2*delta`只能作50MHz/25MHz假設下已耗盡相對interval的充分線索；較小值不能證明整個raw_now→返回窗口未逾期。

每類事件每64次、或該類出現新的cycle max，才進64筆固定環；保留最近64筆「選中的」事件。這種抽樣有偏，**不能當CDF、分位數或事件頻率樣本**。aggregate count/sum/max涵蓋所有有效探針事件。CSV包含sequence/kind、cycle_start、cycles、instret、sample_cycle、raw_now、delta、next_tval、SIP/SIE；kind1的raw_now/delta/next_tval為0。

選中樣本的`sample_cycle`、SIP、SIE在任何aggregate/ring寫之前連續讀取。`counter_end_cycle = cycle_start + cycles`可重建；pending觀測稍晚，不能當SBI精確返回瞬間，也不能以STIP=1單獨斷言bug。最終實際反組譯 [probe-debugfs.disasm](probe-debugfs.disasm) 的probe_end在0x58/0x5c/0x60依序為rdcycle/SIP/SIE，第一個state store在0x66；含在入口函式實際反組譯 [probe-entrypoints.disasm](probe-entrypoints.disasm)。時間差可能含M-mode中斷，未設定虛假的固定cycle上界。

## 已完成的有限驗證

- [compile-receipts.json](compile-receipts.json)：沿用共用65b Image的實際compiler flags、generated headers與config，只將輸出和依賴檔導到本目錄ignored artifacts。現有DEBUG_FS=y probe、DEBUG_FS編譯排除probe、65b原版排除probe，三者均編譯成功且無compiler診斷；最後修訂重編兩個probe對象，未重編未變的原版。
- [no-debugfs-equivalence.json](no-debugfs-equivalence.json)：編譯排除DEBUG_FS時，**整段.text機器bytes與全部.text重定位記錄**都與65b原版相同；不是只比較函式名。
- 實際DEBUG_FS=y但runtime off：直接核 [probe-entrypoints.disasm](probe-entrypoints.disasm) 與完整含重定位的反組譯；enabled=false走原callback／SBI分支，不讀新增SSTATUS/CYCLE/INSTRET/SIP/SIE。STIE的clear/set順序、原有一個next-event rdtime、set_next_event返回0、timer IRQ返回IRQ_HANDLED均保留。off路徑仍有額外flag load/branch及部分register操作，**不宣稱bitwise等同或零擾動**；timer IRQ栈仍為原16B。
- [checkpatch.log](checkpatch.log)：0 errors / 0 warnings / 0 checks。原始時間讀取的三個source sites未增加；IRQ helpers無printk、ktime、額外rdtime、MMIO、allocation或mutex。
- [validation.json](validation.json)：來源hash、實際config hash、object hashes與有限核查結論。原生CSR新讀取每事件一般5次（包含SSTATUS檢查），選中樣本8次；enable與首次freeze各另1次cycle。state5752 bytes，ring64×88 bytes。這是靜態數目，**不是已量得的成本上限**。

未執行：control錯誤碼／per-open生命週期的板上測試、probe實際cost、負載下擾動、timer根因重現及修復驗證。內核state機器已直接源碼審查，但不能將對象編譯冒充runtime驗證。

## Root下一個接受點

Root先審完整終點及上述權限原證據、off分支等價、sample點和序列化，才決定是否建一次診斷Image。若採用，沿已提有界包：同boot一個N2_A1控制與一個N4_A8，既有120s外層／60000ms核心預算不改，不自動重試、最多10分鐘時段，輸出只在退出壓力後。若probe本身引入hang、控制偏移超界或counter不前進，診斷未定，不把消失的故障當修復。保持watchdog、taint與所有正式驗收；這不阻擋既有E4/E5，也不把這個診斷版本的數據納入正式點。

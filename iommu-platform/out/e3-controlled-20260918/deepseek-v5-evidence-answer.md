以下内容只做只读证据整理与脚本草案；我未运行任何命令、未写文件、未触发硬件。事实索引只引用你给出的原始 JSON/UART 可读副本与协议文档；`host-validation.json`、`short-assessment.json`、`main-assessment.json` 作为输入证据列出，不代表我执行过这些测试。`console.readable.log` 是经 `uart-readable-provenance.json` 说明转换后的可读副本，原始 `console.raw.log` 未在我这里提供。

---

## 1. 事实索引

### 1.1 文件与全局计数

- 短阶段原始记录：`out/mac-e3-queue-20260918-130545-run-d01lc9wp/short-records.json`，3 个 case，seq 1..6。
- 主阶段原始记录：`out/mac-e3-queue-20260918-130545-run-d01lc9wp/main-records.json`，10 个 case，seq 7..26。
- 摘要：`short-summary.json`、`main-summary.json`；评估：`short-assessment.json`、`main-assessment.json`。
- 运行元数据：`run.json`。
- 主机侧验证输入：`host-validation.json`。
- 可读 UART 副本：`out/e3-controlled-20260918/console.readable.log`；其 provenance 在 `out/e3-controlled-20260918/uart-readable-provenance.json`，声明源文件为 `out/mac-e3-queue-20260918-130545-run-d01lc9wp/console.raw.log`，源 SHA256 `187e1ae40d970531ba6365628e521267e590dcacc2c9747ce0c91ef4a52840e7`，可读副本 SHA256 `e086e25c4cb6bd4c0fcac897624851370c5cafeec3862027cdb27a58fd0bb3b3`，转换方式为 UTF8 backslashreplace，控制字符 U+0000..U+001F 除 TAB/CR/LF 外转 literal hex，`original_nul_count=1`。
- 协议约束：`E3_QUEUE_PAIR.md:56` 说明预期 13 对、26 子轮、494 条观测；`run.json:24-27` 记录 `main_pairs=10`、`total_rounds=26`、`unique_observation_lines=494`、`taint_after=0`；`run.json:36-38` 记录 `short_pairs=3`、`short_passed=true`、`main_passed=true`。
- 版本：`run.json:40,44` Linux `6.5.0-rc2-00086-gd2cd32d78918`；`run.json:47` 协议 `E3 v5: 3 short + 10 formal pairs, no retry`。
- 原始未验证项：`run.json:9,30,46` 记录 `bit.hash_verified=false`、`sd_readback_verified=false`、`software_sha256=null`；这些是记录边界，不是已验证相等。

### 1.2 逐子轮事实表 A：D/触发、身份、PTE/level、旧 head、Vfirsthit、插入、target

说明：
- `A身份`/`V身份` 取自 `round_checks[].queue_summary.records`；无触发时该字段为 `null`，只列 observer 的 `a_did`/`v_did`。
- `level/PTE` 用 `rounds[].e3_words[8]`/`[9]`；无触发时通常为 `0/0`。
- `旧head` 用 `queue_summary.head` 与 `gates.trigger_head_is_v`。
- `insert` 用 `queue_summary.insert_total/inserted/truncated/intervening`。
- `target` 用 `round_checks[].target` 四项：`v_remaining/v_queue/v_service/a_remaining`；无触发为 `null`。
- 行号给的是记录/round_checks 的大致行段，便于导航。

| seq | 文件:行（记录/检查） | D/触发 | A身份 | V身份 | level/PTE | 旧head | Vfirsthit | insert(总/存/截断; intervening) | target(v_rem,v_queue,v_service,a_rem) |
|---:|---|---|---|---|---|---|---:|---|---|
| 1 | `short-records.json:33-366 / 703-1086` | 0/0 | 无记录(observer a_did=4294967295) | 无记录(observer v_did=10) | 0/0 | raw0 v0 did=null id=null iova=null is_v=0; trigger_head_is_v=0 | null | 0/0/false; [] | null |
| 2 | `short-records.json:367-700 / 1087-1469` | 128/0 | 无记录(observer a_did=4294967295) | 无记录(observer v_did=10) | 0/0 | raw0 v0 did=null id=null iova=null is_v=0; trigger_head_is_v=0 | null | 0/0/false; [] | null |
| 3 | `short-records.json:1503-1837 / 2174-2560` | 0/0 | 无记录(observer a_did=11) | 无记录(observer v_did=10) | 0/0 | raw0 v0 did=null id=null iova=null is_v=0; trigger_head_is_v=0 | null | 0/0/false; [] | null |
| 4 | `short-records.json:1838-2172 / 2562-2948` | 128/0 | 无记录(observer a_did=11) | 无记录(observer v_did=10) | 0/0 | raw0 v0 did=null id=null iova=null is_v=0; trigger_head_is_v=0 | null | 0/0/false; [] | null |
| 5 | `short-records.json:2983-3316 / 3653-4106` | 128/1 | A: seq13 ID4 DID11 IOVA274875867136 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2195337208 | raw4294970116 v1 did11 id4 iova274875834368 is_v=0; trigger_head_is_v=0 | 0 | 5/2/true; seq14 ID4 DID11 IOVA274875869184 write1 tr17449 sel17986 hit1; seq1 ID0 DID10 IOVA274875813888 write1 tr17655 sel17993 hit0 | 1115/1178/986/266 |
| 6 | `short-records.json:3317-3650 / 4107-4549` | 0/1 | A: seq107 ID4 DID11 IOVA274875863040 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2193147896 | raw4294969856 v1 did10 id0 iova274875809792 is_v=1; trigger_head_is_v=1 | 0 | 1/1/false; seq108 ID4 DID11 IOVA274875865088 write1 tr44854 sel45605 hit1 | 280/1365/406/271 |
| 7 | `main-records.json:33-366 / 703-1156` | 0/1 | A: seq13 ID4 DID11 IOVA274875867136 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2195337208 | raw4294970116 v1 did11 id4 iova274875834368 is_v=0; trigger_head_is_v=0 | 0 | 5/2/true; seq14 ID4 DID11 IOVA274875869184 write1 tr17236 sel17645 hit1; seq1 ID0 DID10 IOVA274875813888 write1 tr17408 sel17652 hit0 | 995/1094/994/136 |
| 8 | `main-records.json:367-700 / 1157-1600` | 0/1 | A: seq69 ID4 DID11 IOVA274875850752 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2193147896 | raw4294969856 v1 did10 id0 iova274875809792 is_v=1; trigger_head_is_v=1 | 0 | 1/1/false; seq70 ID4 DID11 IOVA274875852800 write1 tr34035 sel34781 hit1 | 280/1274/407/271 |
| 9 | `main-records.json:1633-1967 / 2304-2757` | 0/1 | A: seq13 ID4 DID11 IOVA274875867136 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2195337208 | raw4294970116 v1 did11 id4 iova274875834368 is_v=0; trigger_head_is_v=0 | 0 | 5/2/true; seq14 ID4 DID11 IOVA274875869184 write1 tr17404 sel17811 hit1; seq1 ID0 DID10 IOVA274875813888 write1 tr17605 sel17818 hit0 | 979/1047/984/136 |
| 10 | `main-records.json:1968-2302 / 2758-3200` | 128/1 | A: seq67 ID4 DID11 IOVA274875846656 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2193147896 | raw4294969856 v1 did10 id0 iova274875809792 is_v=1; trigger_head_is_v=1 | 0 | 1/1/false; seq68 ID4 DID11 IOVA274875848704 write1 tr33452 sel34333 hit1 | 410/1401/406/401 |
| 11 | `main-records.json:3234-3568 / 3905-4358` | 128/1 | A: seq13 ID4 DID11 IOVA274875867136 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2195337208 | raw4294970116 v1 did11 id4 iova274875834368 is_v=0; trigger_head_is_v=0 | 0 | 5/2/true; seq14 ID4 DID11 IOVA274875869184 write1 tr17272 sel17807 hit1; seq1 ID0 DID10 IOVA274875813888 write1 tr17473 sel17814 hit0 | 1114/1182/987/264 |
| 12 | `main-records.json:3569-3903 / 4359-4801` | 0/1 | A: seq55 ID4 DID11 IOVA274875854848 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2193147896 | raw4294969856 v1 did10 id0 iova274875809792 is_v=1; trigger_head_is_v=1 | 0 | 1/1/false; seq56 ID4 DID11 IOVA274875856896 write1 tr30422 sel31168 hit1 | 280/1261/407/271 |
| 13 | `main-records.json:4835-5169 / 5506-5893` | 0/0 | 无记录(observer a_did=11) | 无记录(observer v_did=10) | 0/0 | raw0 v0 did=null id=null iova=null is_v=0; trigger_head_is_v=0 | null | 0/0/false; [] | null |
| 14 | `main-records.json:5170-5504 / 5894-6279` | 128/0 | 无记录(observer a_did=11) | 无记录(observer v_did=10) | 0/0 | raw0 v0 did=null id=null iova=null is_v=0; trigger_head_is_v=0 | null | 0/0/false; [] | null |
| 15 | `main-records.json:6314-6648 / 6985-7438` | 0/1 | A: seq13 ID4 DID11 IOVA274875867136 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2195337208 | raw4294970116 v1 did11 id4 iova274875834368 is_v=0; trigger_head_is_v=0 | 0 | 5/2/true; seq14 ID4 DID11 IOVA274875869184 write1 tr17378 sel17785 hit1; seq1 ID0 DID10 IOVA274875813888 write1 tr17579 sel17792 hit0 | 980/1048/989/136 |
| 16 | `main-records.json:6649-6983 / 7439-7880` | 128/1 | A: seq13 ID4 DID11 IOVA274875867136 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2195337208 | raw4294970116 v1 did11 id4 iova274875834368 is_v=0; trigger_head_is_v=0 | 0 | 5/2/true; seq14 ID4 DID11 IOVA274875869184 write1 tr17295 sel17830 hit1; seq1 ID0 DID10 IOVA274875813888 write1 tr17496 sel17837 hit0 | 1114/1182/987/264 |
| 17 | `main-records.json:7926-8259 / 8596-9049` | 0/1 | A: seq13 ID4 DID11 IOVA274875867136 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2195337208 | raw4294970116 v1 did11 id4 iova274875834368 is_v=0; trigger_head_is_v=0 | 0 | 5/2/true; seq14 ID4 DID11 IOVA274875869184 write1 tr17553 sel17961 hit1; seq1 ID0 DID10 IOVA274875813888 write1 tr17754 sel17968 hit0 | 979/1048/984/136 |
| 18 | `main-records.json:8260-8594 / 9050-9491` | 0/1 | A: seq71 ID4 DID11 IOVA274875854848 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2193147896 | raw4294969856 v1 did10 id0 iova274875809792 is_v=1; trigger_head_is_v=1 | 0 | 1/1/false; seq72 ID4 DID11 IOVA274875856896 write1 tr34962 sel35708 hit1 | 280/1274/407/271 |
| 19 | `main-records.json:9526-9860 / 10197-10650` | 128/1 | A: seq13 ID4 DID11 IOVA274875867136 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2195337208 | raw4294970116 v1 did11 id4 iova274875834368 is_v=0; trigger_head_is_v=0 | 0 | 5/2/true; seq14 ID4 DID11 IOVA274875869184 write1 tr17222 sel17757 hit1; seq1 ID0 DID10 IOVA274875813888 write1 tr17423 sel17764 hit0 | 1114/1182/987/264 |
| 20 | `main-records.json:9861-10195 / 10651-11092` | 0/1 | A: seq53 ID4 DID11 IOVA274875850752 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2193147896 | raw4294969856 v1 did10 id0 iova274875809792 is_v=1; trigger_head_is_v=1 | 0 | 1/1/false; seq54 ID4 DID11 IOVA274875852800 write1 tr29239 sel29995 hit1 | 285/1285/406/276 |
| 21 | `main-records.json:11127-11461 / 11798-12251` | 0/1 | A: seq13 ID4 DID11 IOVA274875867136 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2195337208 | raw4294970116 v1 did11 id4 iova274875834368 is_v=0; trigger_head_is_v=0 | 0 | 5/2/true; seq14 ID4 DID11 IOVA274875869184 write1 tr17066 sel17474 hit1; seq1 ID0 DID10 IOVA274875813888 write1 tr17238 sel17481 hit0 | 982/1080/981/136 |
| 22 | `main-records.json:11462-11796 / 12252-12693` | 128/1 | A: seq53 ID4 DID11 IOVA274875850752 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2193147896 | raw4294969856 v1 did10 id0 iova274875809792 is_v=1; trigger_head_is_v=1 | 0 | 1/1/false; seq54 ID4 DID11 IOVA274875852800 write1 tr29180 sel30066 hit1 | 410/1404/406/401 |
| 23 | `main-records.json:12728-13062 / 13399-13785` | 128/0 | 无记录(observer a_did=11) | 无记录(observer v_did=10) | 0/0 | raw0 v0 did=null id=null iova=null is_v=0; trigger_head_is_v=0 | null | 0/0/false; [] | null |
| 24 | `main-records.json:13063-13397 / 13786-14172` | 0/0 | 无记录(observer a_did=11) | 无记录(observer v_did=10) | 0/0 | raw0 v0 did=null id=null iova=null is_v=0; trigger_head_is_v=0 | null | 0/0/false; [] | null |
| 25 | `main-records.json:14207-14541 / 14878-15331` | 128/1 | A: seq13 ID4 DID11 IOVA274875867136 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2195337208 | raw4294970116 v1 did11 id4 iova274875834368 is_v=0; trigger_head_is_v=0 | 0 | 5/2/true; seq14 ID4 DID11 IOVA274875869184 write1 tr17207 sel17742 hit1; seq1 ID0 DID10 IOVA274875813888 write1 tr17408 sel17749 hit0 | 1110/1178/998/264 |
| 26 | `main-records.json:14542-14875 / 15332-15773` | 0/1 | A: seq71 ID4 DID11 IOVA274875854848 | V: seq1 ID0 DID10 IOVA274875809792 | 0/2193147896 | raw4294969856 v1 did10 id0 iova274875809792 is_v=1; trigger_head_is_v=1 | 0 | 1/1/false; seq72 ID4 DID11 IOVA274875856896 write1 tr34826 sel35572 hit1 | 280/1265/407/271 |

### 1.3 逐子轮事实表 B：窗口/ARM 前史、暖机、软件启动区间、硬件 cycles

说明：
- `win78-85` 是 `e3_words[78:86]`，格式 `hit(Vr,Vw,Ar,Aw)/miss(...)`；对应 `queue_summary.history.pre_hit/pre_miss`。
- `ARM86-93` 是 `e3_words[86:94]`，格式 `TR(Vr,Vw,Ar,Aw)/COMP(...)`；对应 `pre_tr/pre_comp`。协议 `E3_QUEUE_PAIR.md:86` 明确：78–93 是触发沿之前快照，78–85 只累计 active 窗口内查询，86–93 从 ARM 后累计，可能含窗口打开前已完成请求；不能仅靠 window-entered 还原 A 窗口前完成数。
- 暖机格式：`lane0 sub/comp/drained/inflight, HPM(active,iotlb,s1,s2)`；`lane1` 同理。lane0=V，lane1=A。
- 软件区间：`V launch ns`、`A launch ns`，来自 `round_checks[].diagnostic_summary.launch_intervals`；A 缺失时为 `null`。这些是软件区间，不是硬件发射时刻。
- `hw cycles` 来自 `round_checks[].observer.time.cycles`。

| seq | win78-85 hit/miss | ARM86-93 TR/COMP | 暖机 lane0 | 暖机 lane1 | V launch ns | A launch ns | hw cycles |
|---:|---|---|---|---|---|---|---:|
| 1 | hit(0,0,0,0)/miss(0,0,0,0) | TR(0,0,0,0)/COMP(0,0,0,0) | 0/0/0/0, HPM(0,0,0,0) | 0/0/0/0, HPM(0,0,0,0) | 165418173040-165418261280 | null | 6842 |
| 2 | hit(0,0,0,0)/miss(0,0,0,0) | TR(0,0,0,0)/COMP(0,0,0,0) | 0/0/0/0, HPM(0,0,0,0) | 0/0/0/0, HPM(0,0,0,0) | 165445526920-165445668440 | null | 5485 |
| 3 | hit(0,0,0,0)/miss(0,0,0,0) | TR(0,0,0,0)/COMP(0,0,0,0) | 32/32/0/0, HPM(21987,2,2,0) | 32/31/1/1, HPM(22452,2,2,0) | 167887005720-167887587400 | 167886334440-167886608280 | 83112 |
| 4 | hit(0,0,0,0)/miss(0,0,0,0) | TR(0,0,0,0)/COMP(0,0,0,0) | 32/32/0/0, HPM(21117,0,0,0) | 32/31/1/1, HPM(21107,0,0,0) | 167980947760-167981642400 | 167951395600-167951669480 | 83484 |
| 5 | hit(0,0,0,0)/miss(0,0,0,0) | TR(2,2,14,14)/COMP(0,0,12,12) | 0/0/0/0, HPM(0,0,0,0) | 0/0/0/0, HPM(0,0,0,0) | 170817552760-170817715520 | 170817376960-170817460280 | 114478 |
| 6 | hit(0,1,0,0)/miss(0,1,0,1) | TR(2,2,108,108)/COMP(0,2,106,106) | 0/0/0/0, HPM(0,0,0,0) | 0/0/0/0, HPM(0,0,0,0) | 170905992400-170906273760 | 170905326080-170905607080 | 113467 |
| 7 | hit(0,0,0,0)/miss(0,0,0,0) | TR(2,2,14,14)/COMP(0,0,12,12) | 256/256/0/0, HPM(170223,2,2,0) | 256/255/1/1, HPM(374798,512,512,0) | 533161499360-533161661920 | 533161330960-533161407320 | 7326451 |
| 8 | hit(0,1,0,0)/miss(0,1,0,1) | TR(2,2,70,70)/COMP(0,2,68,68) | 256/256/0/0, HPM(169764,2,2,0) | 256/255/1/1, HPM(297041,318,318,0) | 533451713080-533490254480 | 533451279960-533451394400 | 7262430 |
| 9 | hit(0,0,0,0)/miss(0,0,0,0) | TR(2,2,14,14)/COMP(0,0,12,12) | 256/256/0/0, HPM(170164,2,2,0) | 256/255/1/1, HPM(374617,512,512,0) | 536653590640-536653754200 | 536653421400-536653498200 | 7327197 |
| 10 | hit(0,1,0,0)/miss(0,1,0,1) | TR(2,2,68,68)/COMP(0,2,66,66) | 256/256/0/0, HPM(169910,2,2,0) | 256/255/1/1, HPM(296008,316,316,0) | 536947353600-536947665360 | 536946948280-536947038040 | 7266816 |
| 11 | hit(0,0,0,0)/miss(0,0,0,0) | TR(2,2,14,14)/COMP(0,0,12,12) | 256/256/0/0, HPM(169806,2,2,0) | 256/255/1/1, HPM(375138,512,512,0) | 540183043800-540183207240 | 540182875440-540182951760 | 7331737 |
| 12 | hit(0,1,0,0)/miss(0,1,0,1) | TR(2,2,56,56)/COMP(0,2,54,54) | 256/256/0/0, HPM(169742,2,2,0) | 256/255/1/1, HPM(295515,314,314,0) | 540477319880-540477628440 | 540476975040-540477080760 | 7264867 |
| 13 | hit(0,0,0,0)/miss(0,0,0,0) | TR(0,0,0,0)/COMP(0,0,0,0) | 256/256/0/0, HPM(170210,2,2,0) | 256/255/1/1, HPM(169584,2,2,0) | 543770779440-543771347640 | 543770122200-543770394960 | 5365244 |
| 14 | hit(0,0,0,0)/miss(0,0,0,0) | TR(0,0,0,0)/COMP(0,0,0,0) | 256/256/0/0, HPM(169355,0,0,0) | 256/255/1/1, HPM(168643,0,0,0) | 543985993240-543986679640 | 543967968200-543985551760 | 5348232 |
| 15 | hit(0,0,0,0)/miss(0,0,0,0) | TR(2,2,14,14)/COMP(0,0,12,12) | 256/256/0/0, HPM(169710,2,2,0) | 256/255/1/1, HPM(375156,512,512,0) | 547261918080-547262081800 | 547261748800-547261825720 | 7327501 |
| 16 | hit(0,0,0,0)/miss(0,0,0,0) | TR(2,2,14,14)/COMP(0,0,12,12) | 256/256/0/0, HPM(170104,2,2,0) | 256/255/1/1, HPM(294841,312,312,0) | 547553311000-547553474520 | 547553139960-547553219120 | 7324625 |
| 17 | hit(0,0,0,0)/miss(0,0,0,0) | TR(2,2,14,14)/COMP(0,0,12,12) | 256/256/0/0, HPM(169910,2,2,0) | 256/255/1/1, HPM(374508,512,512,0) | 550965266920-550965429840 | 550965093440-550965174840 | 7319345 |
| 18 | hit(0,1,0,0)/miss(0,1,0,1) | TR(2,2,72,72)/COMP(0,2,70,70) | 256/256/0/0, HPM(169842,2,2,0) | 256/255/1/1, HPM(297008,318,318,0) | 551254897000-551255205880 | 551254463160-551254578320 | 7267359 |
| 19 | hit(0,0,0,0)/miss(0,0,0,0) | TR(2,2,14,14)/COMP(0,0,12,12) | 256/256/0/0, HPM(169813,2,2,0) | 256/255/1/1, HPM(374974,512,512,0) | 554699803800-554699967240 | 554699635080-554699711440 | 7327259 |
| 20 | hit(0,1,0,0)/miss(0,1,0,1) | TR(2,2,54,54)/COMP(0,2,52,52) | 256/256/0/0, HPM(170011,2,2,0) | 256/255/1/1, HPM(295632,314,314,0) | 554975928240-555030110800 | 554975589360-554975690200 | 7271204 |
| 21 | hit(0,0,0,0)/miss(0,0,0,0) | TR(2,2,14,14)/COMP(0,0,12,12) | 256/256/0/0, HPM(169810,2,2,0) | 256/255/1/1, HPM(374638,512,512,0) | 558486875560-558487037560 | 558486707680-558486783560 | 7321221 |
| 22 | hit(0,1,0,0)/miss(0,1,0,1) | TR(2,2,54,54)/COMP(0,2,52,52) | 256/256/0/0, HPM(169876,2,2,0) | 256/255/1/1, HPM(295085,314,314,0) | 558778254520-558778552280 | 558777916120-558778017120 | 7269282 |
| 23 | hit(0,0,0,0)/miss(0,0,0,0) | TR(0,0,0,0)/COMP(0,0,0,0) | 256/256/0/0, HPM(169669,2,2,0) | 256/255/1/1, HPM(169783,2,2,0) | 562327226120-562327807840 | 562326553840-562326827800 | 5353545 |
| 24 | hit(0,0,0,0)/miss(0,0,0,0) | TR(0,0,0,0)/COMP(0,0,0,0) | 256/256/0/0, HPM(168785,0,0,0) | 256/255/1/1, HPM(168877,0,0,0) | 562545158360-562545824160 | 562528004840-562544690160 | 5346760 |
| 25 | hit(0,0,0,0)/miss(0,0,0,0) | TR(2,2,14,14)/COMP(0,0,12,12) | 256/256/0/0, HPM(170122,2,2,0) | 256/255/1/1, HPM(374418,512,512,0) | 566115457080-566115620400 | 566115288680-566115365160 | 7325317 |
| 26 | hit(0,1,0,0)/miss(0,1,0,1) | TR(2,2,72,72)/COMP(0,2,70,70) | 256/256/0/0, HPM(169832,2,2,0) | 256/255/1/1, HPM(296904,318,318,0) | 566406950840-566407259920 | 566406518040-566406632360 | 7266494 |

### 1.4 可从原始字段直接验证的相等/不同项（不给因果结论）

- 全局：
  - 26 轮 `victim.submitted==victim.completed==transfers`，`completed_bytes==completed*4096`；所有轮 `data_errors=0`、`quarantined=0`、`s2_ptw=0`，且 `s1_ptw==iotlb_miss`。见各 `rounds[].victim`。
  - 26 轮 `rounds[].hpm_configured=measurement_started=measurement_complete=1`，`status=0`。
  - 短阶段 3 对、主阶段 10 对，共 13 对；seq 1..26 连续；UART 可读副本中同一 seq 的重复 dmesg 行在给定内容里是重复副本，必须按“同 seq 且字段完全相同”去重。494 = 26 × (3 batch + 16 path)。
  - `run.json` 与 `main-summary.json` 的顶层 `run.status` 均为 `completed_local_only`；`short-summary.json` 顶层 `run.status` 仍是 `short_running`，按 `E3_QUEUE_RESULTS.md:12` 只作为较早快照。
  - `short-assessment.json` 为 `{"passed":true,"errors":[],"cases_seen":3}`；`main-assessment.json` 为 `{"passed":true,"errors":[],"cases_seen":10}`。
  - `host-validation.json:2-14` 记录 `field_unittests.passed=true, tests_run=35`、`runner_shell_syntax=passed`、四个负向捕获检查均为 `true`、`legacy_00085_short_main_regression=passed`、`final_main_uart_snapshot_byte_identical=true`、`short_assessment_passed=true`、`main_assessment_passed=true`、`hardware_reruns=0`。这些是输入证据，不是我运行结果。
  - `run.json:9,30,46` 的 `bit.hash_verified=false`、`sd_readback_verified=false`、`software_sha256=null` 是直接记录；不能据此声称 BIT/SD/软件已按预期哈希验证。

- 触发覆盖：
  - 短阶段 seq5、seq6 触发；seq1–4 不触发。
  - 正式 A8：seq7–12、15–22、25–26 共 16 轮全部触发；`main-summary.json` 正式 A8 触发 16/16。
  - 正式 A1：seq13、14、23、24 共 4 轮不触发；A1 触发 0/4。
  - 所有触发轮 target 存在且 `v_first_hit=0`；所有无触发轮 `target=null`、`queue_summary.records=null`、`insert_total=0/inserted=0/truncated=false/intervening=[]`、`head.valid=0`。
  - `E3_QUEUE_PAIR.md:94` 的“旧队头 A”直接覆盖：`trigger_head_is_v=0` 且 `head.is_v=false` 的触发轮有 seq5、7、9、11、15、16、17、19、21、25；其中正式 A8 为 9 轮。`trigger_head_is_v=1` 且 `head.is_v=true` 的触发轮有 seq6、8、10、12、18、20、22、26；其中正式 A8 为 7 轮。

- 截断与插入：
  - `truncated=true` 的轮：seq5、7、9、11、15、16、17、19、21、25。正式 A8 截断 9 轮；`run.json:33` 记录 `formal_a8_truncated_rounds=9`。
  - 截断轮均为 `insert_total=5, inserted=2, truncated=true`，只保存最多前两条 intervening；不能把 `inserted=2` 当完整插入轨迹。非截断触发轮为 `insert_total=1, inserted=1, truncated=false`。
  - 所有保存的 intervening 身份均为 `ID0/DID10` 或 `ID4/DID11`；保存项 `tr<=selected`，且 `selected` 落在触发 A 翻译响应与目标 V 选中之间（按 `E3_QUEUE_PAIR.md:90` 检查）。

- 78–85 与 86–93 的区分：
  - `trigger_head_is_v=0` 的触发轮，78–85 的 `pre_hit/pre_miss` 全 0；86–93 多数为 `TR(2,2,14,14)/COMP(0,0,12,12)`。
  - `trigger_head_is_v=1` 的触发轮，78–85 出现 `Vw` 的 `hit=1/miss=1`，即 `pre_hit=(0,1,0,0)`、`pre_miss=(0,1,0,1)`；86–93 的 `TR` 按 seq 不同为 `(2,2,56..108,56..108)`，`COMP` 为 `(0,2,54..106,54..106)`。
  - 无触发轮 78–93 均为 0。
  - 注意：78–85 只累计 active 窗口内查询；86–93 从 ARM 后累计，可能包含窗口打开前已完成的 A 请求。不能仅凭 `window` 的 stage `entered` 还原窗口前完成数，也不能把 86–93 与仅窗口内的 stage entered 做简单相等/大小断言。

- D0/D0 自然轮差：
  - `main-records.json` case1 seq7 与 seq8 均为 D0 且都触发，但旧 head 不同（seq7 A 头，seq8 V 头）、PTE 不同（2195337208 vs 2193147896）、insert 不同（5/2/true vs 1/1/false）、target 不同（995/1094/994/136 vs 280/1274/407/271）、78–85/86–93 不同。
  - `main-records.json` case6 seq17 与 seq18 同样为 D0/D0 且都触发，旧 head、PTE、insert、target、78–93 均不同。
  - 这直接说明：不能把 D128−D0 的差异自动归因于 D 延迟；前史、身份、旧 head、插入状态可能已不同。

- 可直接验证的成对相同/不同：
  - 所有触发轮 V 记录均为 `seq=1, ID0, DID10, IOVA=274875809792`；A 记录为 `ID4, DID11`，但 A seq/IOVA 在不同轮不同。
  - 所有触发轮 `V first_hit=0`、`A first_hit=0`；所有触发轮 `records.v.seq=1`，`records.a.seq` 随轮变化。
  - D0/D128 的部分轮 `e3_words[3]` 与 `rounds[].delay` 一致；诊断 1 的两轮均 `delay=0`；诊断 2 的第二轮 delay = 128−first_delay。
  - 暖机：warmup=256 时 lane0 均为 `256/256/0/0`，lane1 均为 `256/255/1/1`；warmup=32 时 lane0 `32/32/0/0`，lane1 `32/31/1/1`；warmup=0 时两 lane 全 0。A8 正式轮 lane1 `warm_hpm.iotlb_miss` 为 512（case1 seq7/8、case2–8、case10 等），A1 轮为 2；这是直接不同，不是因果结论。
  - 硬件 cycles：正式 A8 约 7.26M–7.33M cycles，正式 A1 约 5.34M–5.36M cycles；短 seq3–6 为 83k–114k cycles。可直接由 observer.time.cycles 验证。

---

## 2. 复核脚本草案：`server-review-v5.py`

说明：以下脚本只读运行；默认读取脚本同目录或传入的 run 目录中的 `console.raw.log`，按 raw bytes 计算 SHA256，并按 `uart-readable-provenance.json` 同口径生成可读文本用于解析。脚本不修改任何输入文件；输出 `server-review-v5.json` 到 run 目录；任何检查失败或异常均写报告并返回非 0。

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
server-review-v5.py

Read-only independent review of E3 v5 / Linux 00086 returned short/main JSON
and the original UART console.raw.log.  No hardware rerun, no input mutation.

Usage:
    python3 server-review-v5.py [RUN_DIR] [--uart PATH]

Default RUN_DIR = directory containing this script.
Default UART   = RUN_DIR/console.raw.log

Output:
    RUN_DIR/server-review-v5.json
Exit:
    0 if all "check" items pass; 1 otherwise.
"""

from pathlib import Path
from collections import defaultdict, Counter
import argparse
import hashlib
import json
import re
import sys
import traceback

OBSERVER_MAGIC = 0x49444D414F425305
E3_BANK_MAGIC = 0x49444D4145330002  # recorded, not required in e3_words[0]
EXPECTED_LINUX = "6.5.0-rc2-00086-gd2cd32d78918"
EXPECTED_PROTOCOL = "E3 v5: 3 short + 10 formal pairs, no retry"
EXPECTED_TOTAL_ROUNDS = 26
EXPECTED_UNIQUE_OBS = 494
EXPECTED_SHORT_PAIRS = 3
EXPECTED_MAIN_PAIRS = 10

FIELDS = ("entered", "full", "carry", "sum", "max", "start", "end", "area")
BATCH_PARTS = ("time", "axi_v", "axi_a")
PATH_COMBOS = tuple((r, c, s) for r in ("a", "v") for c in ("r", "w") for s in range(4))

REPORT = {}

def chk(name, cond, detail=None, mode="check"):
    """mode='check' -> failure on false; mode='record' -> only record."""
    ok = bool(cond)
    e = {"name": name, "mode": mode, "ok": ok}
    if detail is not None:
        e["detail"] = detail
    REPORT.setdefault("checks", []).append(e)
    if mode == "check" and not ok:
        REPORT.setdefault("failures", []).append(e)
    if mode == "record":
        REPORT.setdefault("record_only", []).append(e)
    return ok

def sha256_bytes(b):
    return hashlib.sha256(b).hexdigest()

def sha256_file(p):
    return sha256_bytes(p.read_bytes())

def raw_to_readable(raw: bytes) -> str:
    """Match uart-readable-provenance.json transform."""
    s = raw.decode("utf-8", errors="backslashreplace")
    out = []
    for ch in s:
        o = ord(ch)
        if o < 32 and ch not in "\t\n\r":
            out.append("\\x%02x" % o)
        else:
            out.append(ch)
    return "".join(out)

def kv(s):
    return {
        k: int(v, 16) if v.startswith("0x") else int(v)
        for k, v in re.findall(r"(\w+)=(0x[0-9a-fA-F]+|\d+)", s)
    }

def extract_phase(text, phase):
    m = re.search(r"E3_BEGIN phase=" + re.escape(phase) + r"\b", text)
    if not m:
        raise RuntimeError("missing E3_BEGIN phase=%s" % phase)
    tail = text[m.end():]
    e = re.search(r"E3_EXIT phase=" + re.escape(phase) + r" rc=(\d+)", tail)
    if not e:
        raise RuntimeError("missing E3_EXIT phase=%s" % phase)
    return tail[:e.start()], int(e.group(1))

def parse_case_headers(block):
    out = []
    pat = re.compile(
        r"E3_CASE id=(\d+) aggressor=(none|1) pages=(\d+) transfers=(\d+) "
        r"warmup=(\d+) first=(\d+)(?:\s+diagnostic=(\d+))?"
    )
    for m in pat.finditer(block):
        out.append({
            "case": int(m.group(1)),
            "aggressor": m.group(2),
            "pages": int(m.group(3)),
            "transfers": int(m.group(4)),
            "warmup": int(m.group(5)),
            "first": int(m.group(6)),
            "diagnostic": int(m.group(7)) if m.group(7) is not None else None,
        })
    return out

def parse_uart_records(block):
    out = []
    for line in block.splitlines():
        s = line.strip()
        if s.startswith('{"schema":"idma.e3_'):
            out.append(json.loads(s))
    return out

def parse_observer(block):
    obs = defaultdict(dict)   # seq -> key -> parsed dict
    occ = Counter()           # (seq,key) -> occurrences
    conflicts = []
    for m in re.finditer(r"idma_batch_obs seq=(\d+) part=(\w+) ([^\r\n]+)", block):
        seq = int(m.group(1))
        part = m.group(2)
        d = kv(m.group(3))
        key = ("batch", part)
        occ[(seq, key)] += 1
        old = obs[seq].get(key)
        if old is not None and old != d:
            conflicts.append({"seq": seq, "key": key, "old": old, "new": d})
        obs[seq][key] = d
    for m in re.finditer(r"idma_path_obs seq=(\d+) role=([va]) channel=([rw]) stage=(\d+) ([^\r\n]+)", block):
        seq = int(m.group(1))
        role = m.group(2)
        ch = m.group(3)
        stage = int(m.group(4))
        d = kv(m.group(5))
        key = ("path", role, ch, stage)
        occ[(seq, key)] += 1
        old = obs[seq].get(key)
        if old is not None and old != d:
            conflicts.append({"seq": seq, "key": key, "old": old, "new": d})
        obs[seq][key] = d
    return obs, occ, conflicts

def stage_check(p, label):
    entered, full, carry, total, maximum, start, end, area = p
    chk(label + ": start+entered==full+carry+end", start + entered == full + carry + end)
    chk(label + ": carry<=start", carry <= start)
    chk(label + ": full<=entered", full <= entered)
    chk(label + ": max<=sum<=full*max", maximum <= total <= full * maximum)
    chk(label + ": area>=sum", area >= total)

def rec_obj(r):
    return {
        "seq": r[0], "id": r[1], "did": r[2], "iova": r[3],
        "event": r[4], "tr": r[5], "selected": r[6],
        "trans_request": r[7], "first_query": r[8],
        "trans_response": r[9], "comp": r[10], "return": r[11],
        "first_hit": r[12], "write": r[13],
    }

def decode_head(raw):
    valid = (raw >> 32) & 1
    did = (raw >> 8) & 0xFF
    aid = raw & 0xFF
    is_v = (valid == 1 and did == 10 and aid == 0)
    return valid, did, aid, is_v

def check_round(rec, rc, inp, q, phase, case_id, round_index, diag, obs):
    n = inp["transfers"]
    seq = q["sequence"]
    w = q["e3_words"]
    ctx = f"{phase} case{case_id} round{round_index} seq{seq}"
    a_did_expected = 4294967295 if inp["aggressor_id"] == 4294967295 else 11

    # ---- basic record fields ----
    chk(ctx + ": status/start/complete/hpm", q.get("status") == 0 and q.get("measurement_started") == 1
        and q.get("measurement_complete") == 1 and q.get("hpm_configured") == 1)
    chk(ctx + ": round sequence/delay/trigger mirror", rc.get("sequence") == seq
        and rc.get("delay") == w[3] and rc.get("trigger") == w[2])
    expected_delay = inp["first_delay"] if round_index == 0 else (0 if diag == 1 else 128 - inp["first_delay"])
    chk(ctx + ": delay schedule", q.get("delay") == expected_delay)

    # ---- e3_words basic ----
    chk(ctx + ": e3_words length 256", len(w) == 256)
    chk(ctx + ": e3_words uint64", all(isinstance(z, int) and 0 <= z < (1 << 64) for z in w))
    chk(ctx + ": e3_words[0]==observer magic", w[0] == OBSERVER_MAGIC,
        {"e3_words[0]": w[0], "observer": hex(OBSERVER_MAGIC)})
    chk(ctx + ": e3_words[1]==0", w[1] == 0)
    chk(ctx + ": trigger in {0,1}", w[2] in (0, 1))
    chk(ctx + ": e3_words[3]==delay", w[3] == q.get("delay"))
    chk(ctx + ": e3-level/PTE indices recorded", True, {"level": w[8], "pte": w[9]}, mode="record")

    # ---- observer time / AXI ----
    t = rc["observer"]["time"]
    vax = rc["observer"]["axi_v"]
    aax = rc["observer"]["axi_a"]
    chk(ctx + ": observer valid/status", t.get("valid") == 1 and t.get("status") == 4)
    chk(ctx + ": issued/done", t.get("issued") == n and t.get("done") == n)
    chk(ctx + ": gap==n-1", t.get("gap") == n - 1)
    chk(ctx + ": cycles==h+gap", t.get("cycles") == t.get("h") + t.get("gap"))
    chk(ctx + ": v_did==10", t.get("v_did") == 10)
    chk(ctx + ": a_did", t.get("a_did") == a_did_expected)
    chk(ctx + ": axi_v partition sum", sum(vax[k] for k in ("neither", "v_only", "a_only", "both")) == t.get("cycles"))
    chk(ctx + ": axi_v ar/aw/rlast/b", vax.get("ar") == vax.get("aw") == vax.get("rlast") == vax.get("b") == 2 * n)
    chk(ctx + ": axi_v start/end 0", vax.get("start") == 0 and vax.get("end") == 0)
    for zname, z in (("axi_v", vax), ("axi_a", aax)):
        chk(ctx + f": {zname} ptr invariant",
            (z.get("start", 0) >> 16) + z.get("ar", 0) == z.get("rlast", 0) + (z.get("end", 0) >> 16)
            and (z.get("start", 0) & 0xFFFF) + z.get("aw", 0) == z.get("b", 0) + (z.get("end", 0) & 0xFFFF))
    chk(ctx + ": shared ar/aw", aax.get("shared_ar") == vax.get("ar") + aax.get("ar")
        and aax.get("shared_aw") == vax.get("aw") + aax.get("aw"))

    # ---- victim/aggressor counters ----
    for zname, z in (("victim", q["victim"]), ("aggressor", q["aggressor"])):
        chk(ctx + f": {zname} completed_bytes", z.get("completed_bytes") == z.get("completed") * 4096)
        chk(ctx + f": {zname} submitted==completed", z.get("submitted") == z.get("completed"))
        chk(ctx + f": {zname} data_errors/quarantined/s2", z.get("data_errors") == z.get("quarantined") == z.get("s2_ptw") == 0)
        chk(ctx + f": {zname} s1_ptw==iotlb_miss", z.get("s1_ptw") == z.get("iotlb_miss"))
    chk(ctx + ": victim submitted/completed==n", q["victim"].get("submitted") == n and q["victim"].get("completed") == n)
    if inp["aggressor_id"] == 4294967295:
        chk(ctx + ": solo aggressor zero", q["aggressor"].get("submitted") == 0 and q["aggressor"].get("completed") == 0)

    # ---- UART batch/path equality ----
    ub = {p: obs[seq].get(("batch", p)) for p in BATCH_PARTS}
    chk(ctx + ": UART batch equals round_checks.observer", ub == rc["observer"])
    upaths = {}
    for key in PATH_COMBOS:
        role, ch, stage = key
        upaths[key] = obs[seq].get(("path", role, ch, stage))
    pmap = {(p["role"], p["channel"], p["stage"]): p["fields"] for p in rc["paths"]}
    chk(ctx + ": path count == 16", len(pmap) == len(upaths) == 16)
    for key, p in pmap.items():
        u = upaths.get(key)
        chk(ctx + f": path {key} present/valid", u is not None and u.get("valid") == 1)
        chk(ctx + f": path {key} fields match", u is not None and p == [u.get(f, 0) for f in FIELDS])
        stage_check(p, ctx + f": path {key}")
        if key[2] < 3:
            nxt = pmap.get((key[0], key[1], key[2] + 1))
            chk(ctx + f": path {key} full+carry==next entered", nxt is not None and p[1] + p[2] == nxt[0])
        if key[0] == "v":
            chk(ctx + f": path {key} V full", p[0] == p[1] == 2 * n and p[2] == p[5] == p[6] == 0 and p[3] == p[7])

    # ---- e3 old path stages 96..223 ----
    for lane in range(4):
        for k in range(4):
            off = 96 + lane * 32 + k * 8
            p = w[off:off + 8]
            chk(ctx + f": e3 path lane{lane} stage{k} len", len(p) == 8)
            stage_check(p, ctx + f": e3 path lane{lane} stage{k}")
            if k < 3:
                nxt = w[off + 8:off + 16]
                chk(ctx + f": e3 path lane{lane} stage{k} full+carry==next entered", p[1] + p[2] == nxt[0])
            if lane < 2:
                chk(ctx + f": e3 path lane{lane} V full", p[0] == p[1] == 2 * n and p[2] == p[5] == p[6] == 0 and p[3] == p[7])

    # ---- old final hit/miss and COMP writes ----
    chk(ctx + ": final hit/miss lane sums", w[17] + w[21] == w[18] + w[22] == 2 * n)
    for role in (0, 1):
        p = w[224 + role * 16:240 + role * 16]
        chk(ctx + f": comp role{role} conservation", p[13] + p[0] == p[1] + p[11] and p[1] == p[2] + p[3])
        if role == 0:
            chk(ctx + ": V comp counts", p[0] == p[1] == p[2] == 2 * n and p[3] == p[4] == p[11] == p[13] == 0)
            vw3 = pmap.get(("v", "w", 3), [0, 0, 0, 0, 0, 0, 0, 0])
            chk(ctx + ": V comp sum relation", p[5] + p[7] == p[9] == vw3[3])

    # ---- queue_summary common ----
    qs = rc["queue_summary"]
    chk(ctx + ": qs magic/error/trigger", qs.get("magic") == w[0] and qs.get("error") == w[1] and qs.get("trigger") == w[2])
    chk(ctx + ": qs aggressor_present", qs.get("aggressor_present") == (inp["aggressor_id"] != 4294967295))
    trig = w[2] == 1
    trunc = bool(w[62])
    expected_class = "nontrigger" if not trig else ("truncated" if trunc else "triggered")
    chk(ctx + ": classification", qs.get("classification") == expected_class)
    gates = qs.get("gates", {})
    chk(ctx + ": gates map to e3_words[25..31]",
        gates.get("ptw_handshakes") == w[25] and gates.get("trigger_class") == w[26]
        and gates.get("with_waiting_v") == w[27] and gates.get("without_waiting_v") == w[28]
        and gates.get("old_head_not_v") == w[29] and gates.get("waiting_cycles") == w[30]
        and gates.get("trigger_head_is_v") == w[31])
    chk(ctx + ": gate self-consistency", w[25] >= w[26] and w[26] == w[27] + w[28]
        and w[29] <= w[27] and w[30] >= w[27])
    chk(ctx + ": trigger vs waiting-V consistency", (w[2] == 1) == (w[27] != 0))
    chk(ctx + ": trunc fields", w[62] == (1 if w[12] > 2 else 0) and w[63] == min(w[12], 2))
    chk(ctx + ": qs insert total/saved/truncated",
        qs.get("insert_total") == w[12] and qs.get("inserted") == w[63] and qs.get("truncated") == bool(w[62]))

    # head semantics
    raw, iova = w[94], w[95]
    head = qs.get("head", {})
    valid, did, aid, is_v = decode_head(raw)
    chk(ctx + ": head raw", head.get("raw") == raw)
    if valid:
        chk(ctx + ": head decoded", head.get("valid") == 1 and head.get("did") == did
            and head.get("id") == aid and head.get("iova") == iova and head.get("is_v") == is_v)
    else:
        chk(ctx + ": invalid head null fields", head.get("valid") == 0 and head.get("did") is None
            and head.get("id") is None and head.get("iova") is None and iova == 0)
    chk(ctx + ": trigger_head_is_v==head.is_v", gates.get("trigger_head_is_v") == (1 if is_v else 0))

    # history: 78..85 and 86..93
    hist = qs.get("history", {})
    chk(ctx + ": win78-85 pre_hit/pre_miss", hist.get("pre_hit") == w[78:82] and hist.get("pre_miss") == w[82:86])
    chk(ctx + ": ARM86-93 pre_tr/pre_comp", hist.get("pre_tr") == w[86:90] and hist.get("pre_comp") == w[90:94])
    chk(ctx + ": final hit/miss", hist.get("final_hit") == w[17:21] and hist.get("final_miss") == w[21:25])
    chk(ctx + ": A pre-window count not inferred from window entered", True,
        "boundary: do not use window-entered to reconstruct pre-window A completions", mode="record")

    # intervening records
    intervening = []
    for off in (46, 54):
        r = w[off:off + 8]
        if any(r):
            intervening.append({
                "seq": r[0], "id": r[1], "did": r[2], "iova": r[3],
                "write": r[4], "tr": r[5], "selected": r[6], "hit": r[7],
            })
    chk(ctx + ": intervening equals queue_summary", intervening == qs.get("intervening"))
    for r in intervening:
        chk(ctx + f": intervening identity seq{r['seq']}", (r["id"], r["did"]) in ((0, 10), (4, 11)))
        chk(ctx + f": intervening TR<=selected seq{r['seq']}", r["tr"] <= r["selected"])
    chk(ctx + ": intervening selected nondecreasing",
        [r["selected"] for r in intervening] == sorted(r["selected"] for r in intervening))

    # timings
    tim = qs.get("timings", {})
    chk(ctx + ": timings base",
        tim.get("config_d") == w[3] and tim.get("response_received") == w[4]
        and tim.get("response_first") == w[5] and tim.get("response_consumed") == w[6]
        and tim.get("actual_added") == w[7] and tim.get("arm_freeze") == w[16])

    if trig:
        aa = w[32:64]
        vv = w[64:96]
        chk(ctx + ": response diff w5-w4==1+D", w[5] - w[4] == 1 + w[3])
        chk(ctx + ": response order", w[6] >= w[5])
        chk(ctx + ": actual_added==D", w[7] == w[3])
        chk(ctx + ": insertion count idx13==3", w[13] == 3)
        chk(ctx + ": insertion saved idx14==2", w[14] == 2)
        chk(ctx + ": waiting cycles idx15>0", w[15] > 0)
        chk(ctx + ": A identity ID4/DID11", aa[1] == 4 and aa[2] == 11)
        chk(ctx + ": V identity ID0/DID10", vv[1] == 0 and vv[2] == 10)
        chk(ctx + ": A event bits", (aa[4] & 0x3F) == 0x3F)
        chk(ctx + ": V event bits", (vv[4] & 0x3F) == 0x3F)
        for zname, z in (("A", aa), ("V", vv)):
            for k in range(5, 11):
                if z[4] & (1 << (k - 4)):
                    chk(ctx + f": {zname} time monotonic {k}", z[k + 1] >= z[k])
        chk(ctx + ": V TR<=response_received<V selected", vv[5] <= w[4] < vv[6])
        chk(ctx + ": A first_query<=response_received<A trans_response", aa[8] <= w[4] < aa[9])
        chk(ctx + ": A trans_response<=V selected", aa[9] <= vv[6])
        chk(ctx + ": A return may be outside window (record only)", True,
            {"A_return": aa[11], "arm_freeze": w[16]}, mode="record")
        derived = {
            "v_first_hit": vv[12],
            "intervening": w[12],
            "v_remaining": vv[6] - w[4],
            "v_queue": vv[6] - vv[5],
            "v_service": vv[9] - vv[7],
            "a_remaining": aa[9] - w[4],
            "v_pre_wait": w[4] - vv[5],
            "a_pre_service": w[4] - aa[7],
        }
        chk(ctx + ": target equals derived", rc.get("target") == derived)
        if "a_translation_response" in tim:
            chk(ctx + ": timings a_translation_response", tim.get("a_translation_response") == aa[9])
        if "v_selected" in tim:
            chk(ctx + ": timings v_selected", tim.get("v_selected") == vv[6])
        if qs.get("records") is not None:
            chk(ctx + ": records A", qs["records"].get("a") == rec_obj(aa))
            chk(ctx + ": records V", qs["records"].get("v") == rec_obj(vv))
        for r in intervening:
            chk(ctx + f": intervening selected in trigger A resp..V selected seq{r['seq']}",
                aa[9] <= r["selected"] <= vv[6])
    else:
        chk(ctx + ": nontrigger records null", qs.get("records") is None)
        chk(ctx + ": nontrigger target null", rc.get("target") is None)
        chk(ctx + ": nontrigger no A/V trigger records", all(z == 0 for z in w[32:96]))
        chk(ctx + ": nontrigger no response times", all(z == 0 for z in w[4:16]))
        chk(ctx + ": nontrigger insert zero",
            qs.get("insert_total") == 0 and qs.get("inserted") == 0
            and qs.get("truncated") is False and qs.get("intervening") == [])
        chk(ctx + ": A return outside window boundary", True,
            "nontrigger: no A/V target records; A return may be outside window", mode="record")

    # ---- warm diagnostic ----
    d = q.get("diagnostic", {})
    lanes = d.get("lane", [])
    chk(ctx + ": warm_complete and two lanes", d.get("warm_complete") == 1 and len(lanes) == 2)
    if len(lanes) == 2:
        chk(ctx + ": warm time order",
            d.get("warm_begin_ns", 0) <= d.get("warm_end_ns", 0)
            <= d.get("warm_drained_ns", 0) <= d.get("arm_begin_ns", 0) <= d.get("arm_end_ns", 0))
        for lane, l in enumerate(lanes):
            if lane == 1 and inp["aggressor_id"] == 4294967295:
                chk(ctx + ": solo lane1 zero", all(v == 0 for k, v in l.items() if k != "warm_hpm")
                    and all(v == 0 for v in l.get("warm_hpm", {}).values()))
                continue
            chk(ctx + f": warm lane{lane} submitted==completed+drained",
                l.get("warm_submitted") == l.get("warm_completed") + l.get("warm_drained"))
            chk(ctx + f": warm lane{lane} drained==inflight<=1",
                l.get("warm_drained") == l.get("warm_in_flight") and l.get("warm_drained") in (0, 1))
            size = (1 if lane == 0 else inp["aggressor_pages"]) * 4096
            submitted = l.get("warm_submitted", 0)
            last = ((submitted - 1) * 4096) % size if submitted else 0
            nxt = (submitted * 4096) % size
            chk(ctx + f": warm lane{lane} offsets", l.get("warm_last_offset") == last and l.get("warm_next_offset") == nxt)
            chk(ctx + f": arm_end<=launch_begin<=launch_end",
                d.get("arm_end_ns", 0) <= l.get("launch_begin_ns", 0) <= l.get("launch_end_ns", 0))
            h = l.get("warm_hpm", {})
            chk(ctx + f": warm lane{lane} HPM s1==iotlb/s2==0",
                h.get("iotlb_miss") == h.get("s1_ptw") and h.get("s2_ptw") == 0)
            if inp["warmup"] == 0:
                chk(ctx + f": warm lane{lane} zero when warmup 0", submitted == 0 and all(v == 0 for v in h.values()))
        # diagnostic_summary derived
        if inp["aggressor_id"] == 4294967295:
            expected_a_return = None
        else:
            expected_a_return = lanes[0].get("launch_begin_ns", 0) - lanes[1].get("launch_end_ns", 0)
        derived_ds = {
            "warm_loop_ns": d.get("warm_end_ns", 0) - d.get("warm_begin_ns", 0),
            "warm_drain_ns": d.get("warm_drained_ns", 0) - d.get("warm_end_ns", 0),
            "arm_width_ns": d.get("arm_end_ns", 0) - d.get("arm_begin_ns", 0),
            "arm_begin_to_v_launch_end_ns": lanes[0].get("launch_end_ns", 0) - d.get("arm_begin_ns", 0),
            "a_return_to_v_begin_ns": expected_a_return,
            "launch_intervals": [
                None if (lane == 1 and inp["aggressor_id"] == 4294967295) else {
                    "launch_begin_ns": l.get("launch_begin_ns", 0),
                    "launch_end_ns": l.get("launch_end_ns", 0),
                    "launch_width_ns": l.get("launch_end_ns", 0) - l.get("launch_begin_ns", 0),
                }
                for lane, l in enumerate(lanes)
            ],
            "semantics": "software intervals only; not exact hardware issue times",
        }
        chk(ctx + ": diagnostic_summary equals derived", rc.get("diagnostic_summary") == derived_ds)

def compare_summary(summary, records, phase):
    chk(phase + ": summary cases count", len(summary.get("cases", [])) == len(records))
    for srec, rrec in zip(summary.get("cases", []), records):
        case_id = srec.get("case")
        chk(phase + f": summary case {case_id} matches record case", case_id == rrec.get("case"))
        chk(phase + f": summary case {case_id} valid", srec.get("valid") is True)
        sr = srec.get("rounds", [])
        rr = rrec.get("record", {}).get("rounds", [])
        rcs = rrec.get("round_checks", [])
        chk(phase + f": summary case {case_id} two rounds", len(sr) == len(rr) == len(rcs) == 2)
        for i, (a, b, rc) in enumerate(zip(sr, rr, rcs)):
            seq = b.get("sequence")
            chk(phase + f": summary case {case_id} round{i+1} seq", a.get("sequence") == seq)
            chk(phase + f": summary case {case_id} round{i+1} delay/trigger",
                a.get("delay") == b.get("delay") and a.get("trigger") == b.get("trigger"))
            cyc = rc["observer"]["time"]["cycles"]
            chk(phase + f": summary case {case_id} round{i+1} hardware_ms",
                abs(a.get("hardware_ms", -1) - cyc / 50000.0) < 1e-6,
                {"summary": a.get("hardware_ms"), "derived": cyc / 50000.0})
            chk(phase + f": summary case {case_id} round{i+1} software_ms",
                abs(a.get("software_ms", -1) - b.get("victim_elapsed_ns", -1) / 1e6) < 1e-6)
            chk(phase + f": summary case {case_id} round{i+1} V/A miss",
                a.get("v_miss") == b["victim"]["iotlb_miss"] and a.get("a_miss") == b["aggressor"]["iotlb_miss"])
            chk(phase + f": summary case {case_id} round{i+1} V/A ptw",
                a.get("v_ptw") == b["victim"]["s1_ptw"] and a.get("a_ptw") == b["aggressor"]["s1_ptw"])
            chk(phase + f": summary case {case_id} round{i+1} target",
                a.get("target") == rc.get("target"))
            chk(phase + f": summary case {case_id} round{i+1} diagnostic_summary",
                a.get("diagnostic_summary") == rc.get("diagnostic_summary"))
            chk(phase + f": summary case {case_id} round{i+1} queue_summary",
                a.get("queue_summary") == rc.get("queue_summary"))

def main():
    global REPORT
    parser = argparse.ArgumentParser()
    parser.add_argument("run_dir", nargs="?", default=None)
    parser.add_argument("--uart", default=None)
    args = parser.parse_args()

    run_dir = Path(args.run_dir).resolve() if args.run_dir else Path(__file__).resolve().parent
    uart_path = Path(args.uart).resolve() if args.uart else run_dir / "console.raw.log"

    REPORT = {
        "script": "server-review-v5.py",
        "run_dir": str(run_dir),
        "uart_path": str(uart_path),
        "checks": [],
        "failures": [],
        "record_only": [],
        "facts": {"rounds": []},
        "constraints": [
            "Read-only: no hardware run, no build, no input rewrite.",
            "A-return may be outside the observation window; no window containment assertion is made.",
            "Pre-window A completion counts are not reconstructed from window-entered counts.",
            "host-validation.json and assessments are treated as supplied input evidence, not as tests run here.",
            "No causal interpretation is made.",
        ],
    }

    try:
        # ---------- read inputs ----------
        names = [
            "short-records.json", "main-records.json",
            "short-summary.json", "main-summary.json",
            "short-assessment.json", "main-assessment.json",
            "host-validation.json", "run.json",
        ]
        paths = {n: run_dir / n for n in names}
        missing = [str(p) for p in paths.values() if not p.exists()]
        if missing:
            raise RuntimeError("missing input files: " + ", ".join(missing))
        if not uart_path.exists():
            raise RuntimeError("missing UART raw file: " + str(uart_path))

        before = {n: sha256_file(p) for n, p in paths.items()}
        before["console.raw.log"] = sha256_file(uart_path)
        raw = uart_path.read_bytes()
        raw_sha = sha256_bytes(raw)
        nul_count = raw.count(b"\x00")
        readable = raw_to_readable(raw)

        REPORT["input_sha256_before"] = before
        REPORT["raw_sha256"] = raw_sha
        REPORT["original_nul_count"] = nul_count
        chk("UART raw NUL count == 1", nul_count == 1, {"nul_count": nul_count})
        chk("UART raw SHA256 recorded", True, {"sha256": raw_sha}, mode="record")

        short_records = json.loads(paths["short-records.json"].read_text(encoding="utf-8"))
        main_records = json.loads(paths["main-records.json"].read_text(encoding="utf-8"))
        short_summary = json.loads(paths["short-summary.json"].read_text(encoding="utf-8"))
        main_summary = json.loads(paths["main-summary.json"].read_text(encoding="utf-8"))
        short_assessment = json.loads(paths["short-assessment.json"].read_text(encoding="utf-8"))
        main_assessment = json.loads(paths["main-assessment.json"].read_text(encoding="utf-8"))
        host_validation = json.loads(paths["host-validation.json"].read_text(encoding="utf-8"))
        run = json.loads(paths["run.json"].read_text(encoding="utf-8"))

        # ---------- run metadata ----------
        chk("run.json status completed_local_only", run.get("status") == "completed_local_only", run.get("status"))
        chk("run.json main_passed true", run.get("main_passed") is True)
        chk("run.json short_passed true", run.get("short_passed") is True)
        chk("run.json main_pairs==10", run.get("main_pairs") == EXPECTED_MAIN_PAIRS)
        chk("run.json short_pairs==3", run.get("short_pairs") == EXPECTED_SHORT_PAIRS)
        chk("run.json total_rounds==26", run.get("total_rounds") == EXPECTED_TOTAL_ROUNDS)
        chk("run.json unique_observation_lines==494", run.get("unique_observation_lines") == EXPECTED_UNIQUE_OBS)
        chk("run.json taint_after==0", run.get("taint_after") == 0)
        chk("run.json linux_version expected", run.get("linux_version") == EXPECTED_LINUX
            and run.get("expected_linux_version") == EXPECTED_LINUX)
        chk("run.json protocol expected", run.get("protocol") == EXPECTED_PROTOCOL)
        chk("run.json observer_magic expected", run.get("observer_magic") == hex(OBSERVER_MAGIC),
            {"run": run.get("observer_magic"), "expected": hex(OBSERVER_MAGIC)})
        chk("run.json bit hash/sd readback flags recorded", True,
            {"bit.hash_verified": run.get("bit", {}).get("hash_verified"),
             "sd_readback_verified": run.get("sd_readback_verified")}, mode="record")
        chk("run.json main-summary status matches", main_summary.get("run", {}).get("status") == "completed_local_only")
        chk("short-summary is earlier snapshot", True,
            {"short_summary_run_status": short_summary.get("run", {}).get("status")}, mode="record")

        # ---------- assessments / host evidence ----------
        chk("short-assessment exact", short_assessment == {"passed": True, "errors": [], "cases_seen": 3}, short_assessment)
        chk("main-assessment exact", main_assessment == {"passed": True, "errors": [], "cases_seen": 10}, main_assessment)
        hv = host_validation
        chk("host-validation field_unittests", hv.get("field_unittests", {}).get("passed") is True
            and hv.get("field_unittests", {}).get("tests_run") == 35, hv.get("field_unittests"))
        chk("host-validation runner_shell_syntax", hv.get("runner_shell_syntax") == "passed")
        ncc = hv.get("negative_capture_checks", {})
        chk("host-validation negative capture checks",
            all(ncc.get(k) is True for k in (
                "wrong_bank_rejected", "missing_diagnostic_marker_rejected",
                "nonzero_exit_rejected", "wrong_schema_rejected")), ncc)
        chk("host-validation legacy regression", hv.get("legacy_00085_short_main_regression") == "passed")
        chk("host-validation final UART snapshot byte-identical", hv.get("final_main_uart_snapshot_byte_identical") is True)
        chk("host-validation short/main assessment flags",
            hv.get("short_assessment_passed") is True and hv.get("main_assessment_passed") is True)
        chk("host-validation hardware_reruns==0", hv.get("hardware_reruns") == 0)
        chk("host-validation attribution recorded", True, hv.get("attribution"), mode="record")

        # ---------- UART phases ----------
        short_block, short_rc = extract_phase(readable, "short")
        main_block, main_rc = extract_phase(readable, "main")
        chk("short E3_EXIT rc=0", short_rc == 0, short_rc)
        chk("main E3_EXIT rc=0", main_rc == 0, main_rc)

        all_blocks = {"short": short_block, "main": main_block}
        expected_counts = {"short": 3, "main": 10}
        uart_records = {}
        headers = {}
        observers = {}
        occs = {}
        conflicts = {}
        for phase, block in all_blocks.items():
            uart_records[phase] = parse_uart_records(block)
            headers[phase] = parse_case_headers(block)
            observers[phase], occs[phase], conflicts[phase] = parse_observer(block)
            chk(phase + ": E3_CASE_EXIT=0 count", len(re.findall(r"^E3_CASE_EXIT=0\s*$", block, re.M)) == expected_counts[phase])
            chk(phase + ": E3_TAINT_BEFORE=0", re.search(r"^E3_TAINT_BEFORE=0\s*$", block, re.M) is not None)
            chk(phase + ": E3_TAINT_AFTER=0", re.search(r"^E3_TAINT_AFTER=0\s*$", block, re.M) is not None)
            chk(phase + ": E3_COMPLETE cases", re.search(r"^E3_COMPLETE phase=" + phase + r" cases=" + str(expected_counts[phase]) + r"\s*$", block, re.M) is not None)
            chk(phase + ": expected Linux version in phase", EXPECTED_LINUX in block)
            chk(phase + ": no duplicate observer conflict", len(conflicts[phase]) == 0, conflicts[phase][:3])

        # ---------- compare UART JSON records to files ----------
        for phase, records in (("short", short_records), ("main", main_records)):
            chk(phase + ": UART JSON record count", len(uart_records[phase]) == len(records),
                {"uart": len(uart_records[phase]), "file": len(records)})
            chk(phase + ": case header count", len(headers[phase]) == len(records))
            for h, x in zip(headers[phase], records):
                rec = x["record"]
                inp = rec["input"]
                chk(phase + f": case {h['case']} header vs JSON",
                    h["case"] == x["case"]
                    and h["aggressor"] == ("none" if inp["aggressor_id"] == 4294967295 else "1")
                    and h["pages"] == inp["aggressor_pages"]
                    and h["transfers"] == inp["transfers"]
                    and h["warmup"] == inp["warmup"]
                    and h["first"] == inp["first_delay"])
                expected_schema = "idma.e3_diag.v1" if h["diagnostic"] == 1 else "idma.e3_queue_pair.v1"
                chk(phase + f": case {h['case']} schema vs diagnostic", rec.get("schema") == expected_schema,
                    {"header_diag": h["diagnostic"], "schema": rec.get("schema"), "expected": expected_schema})
            # UART records exact order
            chk(phase + ": UART JSON records equal file records",
                uart_records[phase] == [x["record"] for x in records])

        # ---------- pair/round counts ----------
        total_rounds = 0
        all_seqs = []
        for phase, records in (("short", short_records), ("main", main_records)):
            for x in records:
                rec = x["record"]
                total_rounds += len(rec["rounds"])
                all_seqs.extend([q["sequence"] for q in rec["rounds"]])
                chk(phase + f": case {x['case']} valid", x.get("valid") is True)
                chk(phase + f": case {x['case']} success/errno/status",
                    rec.get("success") is True and rec.get("errno") == 0 and rec.get("status") == 0,
                    {"success": rec.get("success"), "errno": rec.get("errno"), "status": rec.get("status")})
                chk(phase + f": case {x['case']} run_attempted/stage",
                    rec.get("run_attempted") is True and rec.get("stage") == "run")
                chk(phase + f": case {x['case']} rounds attempted/completed",
                    rec.get("rounds_attempted") == rec.get("rounds_completed") == 2)
                chk(phase + f": case {x['case']} two rounds/checks",
                    len(rec["rounds"]) == len(x["round_checks"]) == 2)
                inp = rec["input"]
                chk(phase + f": case {x['case']} victim id", inp.get("victim_id") == 0)
                chk(phase + f": case {x['case']} aggressor id", inp.get("aggressor_id") in (1, 4294967295))
                chk(phase + f": case {x['case']} mapping shape",
                    len(rec.get("mapping", [])) == 8
                    and all(z % 4096 == 0 for z in rec.get("mapping", []))
                    and all(rec.get("mapping", [])[:4]))
                if inp.get("aggressor_id") == 4294967295:
                    chk(phase + f": case {x['case']} solo mapping tail zero", rec.get("mapping", [])[4:] == [0, 0, 0, 0])
                else:
                    chk(phase + f": case {x['case']} non-solo mapping tail nonzero", all(rec.get("mapping", [])[4:]))
                if phase == "short":
                    expected = {1: (8, 0), 2: (64, 32), 3: (64, 0)}[x["case"]]
                    chk(phase + f": case {x['case']} short transfers/warmup", (inp["transfers"], inp["warmup"]) == expected)
                else:
                    chk(phase + f": case {x['case']} main transfers/warmup", inp["transfers"] == 4096 and inp["warmup"] == 256)

        chk("total subrounds == 26", total_rounds == EXPECTED_TOTAL_ROUNDS, total_rounds)
        chk("sequences == 1..26", sorted(all_seqs) == list(range(1, 27)), sorted(all_seqs))

        # ---------- per phase per case per round checks ----------
        facts_rows = []
        for phase, records in (("short", short_records), ("main", main_records)):
            obs = observers[phase]
            for x in records:
                rec = x["record"]
                inp = rec["input"]
                diag = 1 if rec.get("schema") == "idma.e3_diag.v1" else 2
                for j, (q, rc) in enumerate(zip(rec["rounds"], x["round_checks"])):
                    check_round(rec, rc, inp, q, phase, x["case"], j + 1, diag, obs)
                    facts_rows.append({
                        "phase": phase,
                        "case": x["case"],
                        "round": j + 1,
                        "seq": q["sequence"],
                        "delay": q["delay"],
                        "trigger": q["trigger"],
                        "schema": rec.get("schema"),
                        "A_record": rc["queue_summary"].get("records", {}).get("a") if rc["queue_summary"].get("records") else None,
                        "V_record": rc["queue_summary"].get("records", {}).get("v") if rc["queue_summary"].get("records") else None,
                        "level": q["e3_words"][8],
                        "pte": q["e3_words"][9],
                        "head": rc["queue_summary"].get("head"),
                        "trigger_head_is_v": rc["queue_summary"].get("gates", {}).get("trigger_head_is_v"),
                        "V_first_hit": rc.get("target", {}).get("v_first_hit") if rc.get("target") else None,
                        "insert_total": rc["queue_summary"].get("insert_total"),
                        "inserted": rc["queue_summary"].get("inserted"),
                        "truncated": rc["queue_summary"].get("truncated"),
                        "intervening": rc["queue_summary"].get("intervening"),
                        "pre_hit": q["e3_words"][78:82],
                        "pre_miss": q["e3_words"][82:86],
                        "pre_tr": q["e3_words"][86:90],
                        "pre_comp": q["e3_words"][90:94],
                        "target": rc.get("target"),
                        "warm_lane": q.get("diagnostic", {}).get("lane"),
                        "launch_intervals": rc.get("diagnostic_summary", {}).get("launch_intervals"),
                        "hardware_cycles": rc["observer"]["time"]["cycles"],
                        "software_ms": q.get("victim_elapsed_ns", 0) / 1e6,
                    })
        REPORT["facts"]["rounds"] = facts_rows

        # ---------- unique observation count ----------
        unique_obs = 0
        per_seq = {}
        for phase, obs in observers.items():
            for seq, d in obs.items():
                per_seq[seq] = len(d)
                unique_obs += len(d)
        chk("unique observations == 494", unique_obs == EXPECTED_UNIQUE_OBS, {"unique": unique_obs, "per_seq": per_seq})
        for seq in range(1, 27):
            d = observers["short" if seq <= 6 else "main"].get(seq, {})
            chk(f"seq {seq}: 3 batch + 16 path", sum(1 for k in d if k[0] == "batch") == 3
                and sum(1 for k in d if k[0] == "path") == 16, {"keys": list(d.keys())[:5]})
        # duplicate occurrences recorded
        occ_summary = {}
        for phase, c in occs.items():
            for (seq, key), count in c.items():
                occ_summary[f"{phase}:{seq}:{key}"] = count
        REPORT["uart_occurrences"] = occ_summary
        REPORT["uart_duplicate_conflicts"] = {p: conflicts[p] for p in conflicts}

        # ---------- summaries ----------
        compare_summary(short_summary, short_records, "short")
        compare_summary(main_summary, main_records, "main")

        # ---------- final input hashes ----------
        after = {n: sha256_file(p) for n, p in paths.items()}
        after["console.raw.log"] = sha256_file(uart_path)
        REPORT["input_sha256_after"] = after
        chk("input files unchanged", before == after, {"before": before, "after": after})
        chk("readable transform matched provenance transform declaration", True,
            {"transform": "UTF8 backslashreplace; control U+0000..U+001F except TAB/CR/LF escaped as literal hex"},
            mode="record")

        REPORT["ok"] = len(REPORT["failures"]) == 0

    except Exception as exc:
        REPORT["fatal_error"] = str(exc)
        REPORT["traceback"] = traceback.format_exc()
        REPORT["ok"] = False
        REPORT.setdefault("failures", []).append({"name": "fatal_error", "mode": "check", "ok": False, "detail": str(exc)})

    out = run_dir / "server-review-v5.json"
    out.write_text(json.dumps(REPORT, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")

    if REPORT.get("ok"):
        print("PASS: server-review-v5.json written to", out)
        return 0
    print("FAIL: server-review-v5.json written to", out)
    for e in REPORT.get("failures", [])[:20]:
        print(" -", e.get("name"), e.get("detail", ""))
    return 1

if __name__ == "__main__":
    sys.exit(main())
```

---

## 3. 脚本输入、输出、假设、未检查项、建议命令

### 输入
- `RUN_DIR`，默认脚本所在目录。
- `RUN_DIR/console.raw.log`：原始 UART bytes；脚本用 raw bytes 计算 SHA256，并按 provenance 同口径转换：
  - `utf-8` + `errors="backslashreplace"`；
  - U+0000..U+001F 除 TAB/CR/LF 外转 literal `\xNN`；
  - 保留正常 JSON/dmesg 行结构。
- `RUN_DIR/short-records.json`、`main-records.json`。
- `RUN_DIR/short-summary.json`、`main-summary.json`。
- `RUN_DIR/short-assessment.json`、`main-assessment.json`。
- `RUN_DIR/host-validation.json`。
- `RUN_DIR/run.json`。

### 输出
- `RUN_DIR/server-review-v5.json`。
- 报告含：输入 SHA256 前后值、raw SHA256、NUL 计数、检查项与失败项、仅记录边界项、逐轮事实表、UART 重复出现次数、重复冲突、汇总一致性检查。
- 失败时返回非 0，并把原因写入 `failures` / `fatal_error`。

### 关键假设
- Dell 实际运行目录中存在 `console.raw.log`；若缺失，脚本会失败并写报告，不会自动重跑硬件。
- `console.raw.log` 的转换与 `uart-readable-provenance.json` 所述一致。
- `short-records.json`/`main-records.json` 中的 `record` 与 UART 中 stdout JSON 逐字对应；若不同，脚本报失败。
- `short-summary.json`/`main-summary.json` 只作为摘要输入；`short-summary.json` 顶层 `run` 是较早快照，不作为最终状态。
- `host-validation.json` 与两个 assessment 是输入证据，脚本只核对其字段，不声称重新执行。

### 明确仅记录或边界，不做检查/推断
- A 返回可能落在观测窗口外：脚本只记录 `A_return` 与 `arm_freeze`，不要求 `A_return <= arm_freeze`。
- A 窗口前完成数不可用 `window entered` 直接还原：脚本不做该还原，只记录 78–85 与 86–93。
- BIT/SD/软件哈希验证：`run.json` 中 `bit.hash_verified=false`、`sd_readback_verified=false`、`software_sha256=null` 仅记录。
- `short-summary.json` 顶层 `run.status=short_running` 仅记录为较早快照。
- 不做因果结论、不比较 D128−D0 的效果、不提出下一步实验设计。

### 建议执行命令
在 Dell 实际回传目录中：

```sh
python3 server-review-v5.py /path/to/20260918-130545-run-d01lc9wp
```

若脚本不在该目录，或 UART raw 不在默认位置：

```sh
python3 server-review-v5.py /path/to/20260918-130545-run-d01lc9wp --uart /path/to/20260918-130545-run-d01lc9wp/console.raw.log
```

执行后检查：

```sh
cat /path/to/20260918-130545-run-d01lc9wp/server-review-v5.json | python3 -m json.tool
```

若脚本返回非 0，先看报告中的 `fatal_error`、`failures`、`input_sha256_before/after`、`uart_duplicate_conflicts`，不要自动重跑硬件。
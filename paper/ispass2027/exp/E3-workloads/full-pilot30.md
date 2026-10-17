# 完整E3首30点：Root唯一调用与预算

full-E3正式数据目前0。只在唯一resume01得到全部原时序/routing/DS0/50MHz边界通过、Root加载实际BIT并核真实G kernel新boot0后运行。
以下入口不会加载BIT、写SD或自动开始第二阶段；A2未执行它，也不访问UART。
collector保留全部数据/排空/HPM/PATH/hist/taint与fresh seq检查，首错停止、不重试。

`run_full_pilot_root.sh`固定Root的当前collector、正式G软件manifest（4bee…）、三backend及本次resume01 FPGA manifest。
默认只30点（一完整150候选block，30cells×n1）；不自动采150。
Root设置三个实际值：E3_LOAD_MANIFEST=真正load receipt、E3_RUN_OUTPUT=全新run目录、E3_SETUP_SECONDS=本E3实际BIT/load/boot等开销秒数。
setup未知不填0假装已测；collector cap为21600−setup，pilot与continuation使用同一cap，旧resume累计wall规则保留。

```bash
bash /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/run_full_pilot_root.sh
```

首30点通过后，Root调用现有analyzer生成不可变snapshot、17列长表及summary；正式命令见full-paper-export.md。
新的profile helper纳入工具SHA绑定，期望actual kernel是6.5.0-rc2-ispass2027-g-v1，
软件CVA cfb9是reference-only，实际BIT仍必须1d587，不能因为G kernel共用就换成G BIT。

用真正pilot summary和实测setup生成只读预算报告：

```bash
python3 /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/pilot30_budget.py \
  --analysis "$E3_PILOT_ANALYSIS" --setup-seconds "$E3_SETUP_SECONDS" --output "$E3_NEW_BUDGET_JSON"
```

预算工具只接受真实30板点、30cells各n1的当前协议；拒绝fixture/缺cell/未知实际collector时间。
它以首block完整collector wall×5预估150，包含命令外parser/setup开销；另加实测板设置开销和原下一点130秒余量。
所有cell已实际执行，不借P0或别的shape时长。实际耗时仍可能变动，fits只作Root锁定论文子集的估值，不是完成保证。
执行硬边界仍是原collector：累计wall cap、每点90秒程序/120秒UART边界、下一点130秒余量，预算不足保留partial_budget。

仅Root据实际预算决定续150时，设置E3_RESUME_RUN=原成功pilot目录、E3_RUN_OUTPUT=另一个全新目录，
并保留相同load/source路径、工具源、E3_SETUP_SECONDS和kernel/boot/BIT，再运行相同脚本。
它会只执行剩120点，新records含原30点一次；不得把两个prefix接成180点。
若不适合150，保留实际30或预算prefix，未测cell仍n0/N/A，不补齐、不与P045合池。

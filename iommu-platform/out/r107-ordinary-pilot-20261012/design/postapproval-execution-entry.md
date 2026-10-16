# 实际新内核与单次恢复执行入口（待原SD批准）

当前新SD许可未收到；实际newboot、timer样本、DMA实验均0，goal active。这里只完成必要执行适配，不将goal缩为构建成功。

1. 用户对 deployment-proposal 的明确批准后，由root保存真实reply及具体目标/payload SHA到live/new-SD-user-approval.json；不能从自动goal继续指令、等待时长或离线测试产生批准。
2. 按既有SD updater写目标首分区、fsync、完整padded SHA后，复用同f9已批准bit重启并被动采集；不并行masked readback。此处尚未执行。
3. 已登录的新boot只读采集 live/postboot-identity.command，保存原UART record；live/postboot_gate.py核唯一chosen uname/newUUID/config/notes/DTB/taint/四cdev/单hart。缺字段拒绝，不能用旧boot填。
4. live/postboot-fetch-data.command只下载已冻结的CLI＋六资产14,678,624B、核独立SHA，不再下载29MB内核到新RAM。CLI RV64 SHA仍7fb60c67…。失败只修具体缺失传输，不启动RUN。
5. live/collect_finite_timer.py在真实批准/boot前提后仅三次CAPS，命令间10s，完整Dell brackets与rawCSR由host/calibrate_finite_timer.py算条件区间。SI准确度未给，未来rate稳定性未证，不将nominal或2:1当独立物理证据。当前只有2正14反synthetic测试。
6. RUN前再用live/pre-pilot-caps.command轻量核同boot/current CAPS并由parse_caps_probe.py保存live/pre-pilot-caps-and-boot.json。live/finite-pilot-once.py校批准/boot/固定live CAPS/30s freshness，再exclusive创建一次attempt guard并发起一个RUN。输出异常/timeout保留同guard；timeout不证明terminal，不能再RUN或reset；检查同进程或保存原状态。
7. CLI返回成功或失败均保留完整原receipt/4pool/3UAPI文件；live/finite-result-export.command以tar.gz＋base64送到task_http_results.py的有限receiver。它只收exact8 regularfiles，拒绝链接/路径越界/重复/错length/超界解压/覆盖旧目录；hardware_semantic_success始终unverified。root再跑现有host/validate_finite_pilot.py亲核原数据与字节。
8. 形成真正全成本、stable R107推进/条件保留/归档、相关docs与ledger；所有本轮交付实际完成才goal complete。无G0/G1，除非已满足原明示进入条件。普通覆盖仅结束本R107卡，回phase1新显式需求，不恢复关闭分支。

offline检查21项通过；用于boot/parser/archive/single-run拒绝路径与syntax，没有实际UART/网络/FPGA/GPU/旧回放。新result server文件尚未启动，当前task listener仍旧版本，PID和端口已只读核实；实际使用时仅替换我们自己的task listener，不重启其他服务。

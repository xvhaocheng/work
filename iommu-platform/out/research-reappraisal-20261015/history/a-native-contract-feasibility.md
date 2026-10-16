# A：实际iDMA完整任务夹具的入口优先验证可行性

2026-10-15；Codex独占定向源核；未运行模型、validator、构建或生产修改。

**可以从现有真实iDMA夹具构造一个全新的、固定有限、从reset到双方完成的source合同，验证普通current-VALID DS入口优先的合法部署；不能直接使用现有`--ds-priority`或旧M0命令。** 所需私有适配是入口AR仲裁、固定小任务入口和owner事件记录。源码本身不保证这个唯一任务一定出现所需竞争／held见证；前瞻合同须把“未出现则未覆盖、停止、不调整参数”冻结。它可以是A分析方法的普通部署检验，不要求DS规则有策略独有性，也不据规则普通就否定分析方法。

## 实际源码提供了什么

| 边界 | 直接原文 | 本轮结论 |
|---|---|---|
| 真DMA及容量 | [nonblock_actual_dma_top.sv:352](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_top.sv:352)、[同文件:548](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_top.sv:548) | 两个真实dma_core_wrap、8在途、FIFO8、默认512×64bit buffer；SharedPoolMode0使用真实certificate，owner_only0不抑制。 |
| fullbuffer合法性 | [idma_fullbuffer_certificate.sv:16](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:16) | 只有正常、页对齐、4096B且容量够的job获得permit；本合同全部4KiB，不能换为64B。 |
| 真翻译及DS | [system_top.sv:7](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_system_top.sv:7) | NormalTL／DDTW／PTW和rv_iommu_ds_if实际生成请求；DS50/50保持。不同页表级请求由真实返回内容驱动，不直接注入未来PTE。 |
| 合法入口owner | [system_top.sv:885](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_system_top.sv:885)、[axi_xbar.sv:131](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_xbar.sv:131)、[axi_mux.sv:409](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:409) | 真实4输入xbar，每目的端mux；AR的LockIn仲裁位于原AR spill之前，正是可替换新选择的位置。 |
| 完整完成 | [test.cpp:2764](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2764)、[同文件:2837](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2837)、[同文件:2937](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2937) | 双方配置后、首NEXT前共同t0；双方自然完成，不STOP；每路独立watchdog，quiet、寄存器及真实issue/done交叉检查可以复用。 |
| 原始字节／事务 | [test.cpp:2415](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2415)、[同文件:2994](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2994) | 源保持、目标全字节、未用目标guard、AR→R、AW→B、W有效字节和errors均有实查路径。 |

## 四个必须明确的缺口

1. **现有DS开关处于错误控制层。** [test.cpp:668](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:668)给行为DDR队列接受AR；[692—697](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:692)的`std::rotate`把已经接受的DS读提升。新合同固定`ds_priority=false`、`read_latency_overlap=false`、write-tail暂停0，保留原head FIFO服务；新选择只发生于mux首次输出VALID之前。

2. **旧完整模式并非任意有限任务入口。** Python在[303](/home/xvhaocheng/work/iommu-platform/tests/test_nonblock_actual_dma.py:303)及C++在[3322](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:3322)限制64/64、warm256、A1或A8、latency31；[report_full:3002](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:3002)又硬编码262144B。不能绕过参数检查运行后还声称原validator支持新任务。应建私有具名入口，显式接受唯一新合同，复制已核完整完成计量并把预期字节改为由本合同2×4096推导；不改旧入口或重跑旧M0。

3. **现有trace没有入口承诺身份。** [full trace:1002](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:1002)记录DDR AR/AW/R/W/B握手、PTW和DMA事件；[system debug:1411](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_system_top.sv:1411)的DS及DDR握手分处不同端点。它们不能代替本DDR mux的current heads、fresh choice、首次offer、held和spill输入接受。需新增被动观察，不能将两个现有握手用同ID随意拼接。

4. **非空覆盖不是源码已经证明的事实。** 即使两个真实DMA都运行，也不保证一拍有DS与载荷同时成为该mux可选头；小合同也不保证出现该selector自身未握手的held。新增观测只记录真实发生，不能强设IOTLB／队列初态、暂停selector等待凑齐头、挑命中相位、加背景流量或扩大任务寻找正例。

## 建议冻结的唯一新完整任务 ACV-2x2

以下是**待root采用的具体合同，尚未执行或声称已达到覆盖**。这是有界诊断copy任务，来源为现有actual_dma源码明确的地址、页表、frontend和正常copy语义；没有原生应用／GPU到达或期限的声明。

| 项目 | 固定值及合法来源 |
|---|---|
| 工作 | V/DID10两次4096B copy，均从IOVA0x10000000到0x10001000；A/DID11两次4096B copy，按普通2页环依次搬页0、页1；总4copy，16KiB有用完成字节。 |
| A地址 | A源IOVA0x20000000、0x20001000；目的0x20002000、0x20003000；PA源0x90000000、0x90001000，目的0x90100000、0x90101000。 |
| 表与数据 | 调用现有`setup_memory(2)`初始化明确Sv39页表及不同源／目标字节，然后真实reset；V及A合计6个payload页，小于16项IOTLB，不制造容量压力。[常量:78](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:78)、[页表/数据:531](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:531)。 |
| 冷前缀 | warmup0，不预填IOTLB、不复制旧M0快照、不手动挂入mem_q。现有reset实际置rst低8tick、高4tick；队列只在共同reset清空。[reset:1536](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:1536)、[warm0:2143](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2143)。 |
| 模式及资源 | 普通N，runtime nonblock1、PolicyEnable0、SharedPoolMode0、owner_only0、所有额外read-admit/header/pool fixture关闭；CPU/debug0；不增加buffer、在途、walker、端口或压力。 |
| 发布 | 双方各target2、gap0、REG_CONF0／普通DEFAULT，原`configure_batch`真实MMIO配置；共同t0后立即按A→V两个NEXT读触发，所有延迟0，不事件触发另一个job、不扫launch order。[configure/start:2023](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2023)。 |
| 服务合同 | DS输入/输出延迟50/50；C++额外读setup31、原单head顺序R服务及队列容量16；原W/B行为及同ID顺序保留。它是明确的条件行为内存合同，**不是MIG周期模型或板收益**。 |
| 两个具名普通程序 | 同一任务分别从reset执行O-RR、O-DS-first/F4。只替换DDR mst0 mux的AR仲裁，DS=source3，nonDS RR；每4次有竞争的DS进入原output spill后给nonDS一次逃逸；held／下游不变。旧M0及新意图均不用。 |
| 完成／消费者 | 每路2issue/2done、实际有效读写各8192B；两路总读写各16384B。正常4KiB分为256beat burst时应各4AR/4AW/4B、1024R/W；以实际头LEN／SIZE和WSTRB核完整字节。目标全字节及源保持/未用页guard验过才记录host consumer receipt；最后quiet／无剩余owner。此consumer是离线数据接受，无CPUcache或应用计算收益主张。 |
| 计时／成本 | 共同t0→双方lastdone及quiet；从初始化/reset→最终host验字节另列完整生命周期，不把初始化/检查设免费。记录选择状态／读口／mux/READY路径；逻辑全成本可先列，面积/Fmax/CPU/FPGA为未测。 |
| 次数与停止 | 最多2个新完整episode：同一任务各普通程序一次。没非空覆盖就记未覆盖并结束，不重试/改参数；功能失败只据实际诊断作有界修正，原失败保留。不将2episode写成统计重复。 |

这里“新”指真实新任务输入与新合法控制位置；不是把旧full64改名。2页A环由现有`setup_memory(ap)`和`configure_batch(...pages...)`自然支持，仅旧验收入口人为冻结1/8；私有新合同显式允许唯一值2，不改变生产DMA能力。

## 最小私有实现边界

现有runner由[test_nonblock_banked.py:16](/home/xvhaocheng/work/iommu-platform/tests/test_nonblock_banked.py:16)和[44](/home/xvhaocheng/work/iommu-platform/tests/test_nonblock_banked.py:44)选择**IOMMU vendor**的`axi_mux.sv`，而原board接入研究绑定的是CVA6 vendor的另一份副本。不能把该夹具结果冒充选中board文件逐字相同或已过板时序。

一个最小私有工程可以只给当前夹具选中的`axi_xbar/axi_mux`副本增加默认关闭的DDR专用AR策略参数，经`gen_mst_port_mux[i]`仅在`i==0`启用；内层DMA mux及dummy peripheral保持旧分支。其AR数据仍来自ID prepend后的原`slv_ar_chans`，正常DS-first/F4选择器替换`i_ar_arbiter`，`i_ar_spill_reg`及AW/W/B/R不动。源清单只替换这两个精确私有文件，不链接两份同名vendor模块。被动事件tap在同一个arbiter/spill边界，避免从别的副本代证。

这样验证的是**当前source模型中合法原生AXI接口上的普通入口部署**。若要升级为10月7日board生成参考的逐字绑定，还需将精确选中CVA6 mux/xbar及依赖作为独立具名源核／集成；此核查没有完成，也不在本报告授权下更换生产。无需把该物理绑定缺项解释成A分析方法无价值。

## 事先固定的判断与非空覆盖门

第一项预测只在实际`!held && DS_current_valid && nonDS_current_valid && fairness_allows_DS`时成立：新selector的首次输出必须是当前source3完整AR，不能借未来意图／未来PTE；与O-RR的区别须按真实RR状态决定，不预设RR一定选载荷。每次firstoffer至接受，完整AR字段必须稳定，source/方向/源内occurrence序号一致。

第二项预测覆盖两种不同债务，分别记录：

- **本selector held**：先前payload首次offer且未被原spill接受，随后DS变有效时，仍保持原payload全部字段，不按新DS撤回。只有实际出现该前缀才记此负例被覆盖。
- **已由下游接受的共同旧债务**：payload已进spill／后端FIFO后，改变新仲裁不能删除或移动它；严格FIFO模型中后接受PTE不能越过前序未退休payload。必须用真实接受/返回occurrence配对，并按相同共同前缀比较；两程序首次分歧后的新债务不能伪称共同债务。

完整记录至少包括该selector每拍输入valid/fullAR、fresh/held/selected、输出firstoffer、spill输入接受、spill输出接受、DDR接受及R/W/B退休；关联到DMA job和walk层级。新请求由真实RTL和返回内容重新生成。旧trace只有DDR握手，不能用它离线挪时间替代这个运行。

两条局部不变量成立且4copy、完整字节、consumer receipt与quiet全部通过，只能接受“该普通部署合法及所覆盖控制边预测正确”。若有真实两头竞争但O-RR也选DS，则规则正确但没有对照排序差；若无竞争或无held，分别报告该项未覆盖。**不预言T_V或T_A必降，更不强求正收益；完整完成向量用于检验分析方法是否区分局部排序变化与完整任务结果。** 若root要求本轮必须同时有非空正例与held负例后才准入，则目前源码证据不够，确切缺项就是唯一固定任务的真实owner事件；不能补写一个初态。

## 可供root决策的结论

- 可以准入“这个唯一source合同的普通部署、功能与非空覆盖检查”，代价是上述私有小适配和两个新episode；不是旧实验重跑，也不需要新意图或返回队列重排。
- 不能在运行前宣称该合同必定提供全部四项前瞻验证、任务净收益或board可部署性。传播迟到／hidden-slack两条留出不由本合同提供，应继续标未核。
- 若不准任何新私有实现，现有开关与trace确实无法完成所问验证；不能把`--ds-priority`执行成功当入口优先证据。缺的是特定控制位置与身份记录，不是分析方法被普通规则“覆盖”。

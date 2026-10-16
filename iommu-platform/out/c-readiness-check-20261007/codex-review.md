# C 可达性与普通对照：Codex 审定（2026-10-07）

**本轮5/5短仿真通过；只完成路径门槛判别，没有实现C，创新仍未成立。** Codex预先定义夹具与否证条件，亲自核关键源码、真实事件和架构结论。DeepSeek起草C++夹具及离线整理程序，Codex纠正补丁格式/字符串引用及bank诊断名称、应用并执行。仅重编现成模型的C++驱动和链接一次，没有重生成SV模型、生产RTL/Linux改动或板卡操作。

## 1. 这次改变的条件

预热仍是V原来的源/目的两个页；测量改为合法但从未访问的新目的页：IOVA 0x10002000 → PA 0x80002000，叶PTE地址0x50010。不是操作系统缺页。A8、V64次4KiB、gap0、warm256、模型延迟31及原DS 50/50路径保持。默认旧模式93487周期精确复现；新页在旧模式未写入。源码：
- `tests/nonblock_actual_dma_test.cpp:444`新PTE；1644–1645预热仍用旧目的；1872仅测量选新目的；1945后quiet/完整字节检查。
- 唯一测试补丁见[fixture-applied.patch](fixture-applied.patch)，原文件和旧binary保留。
- [runs.json](runs.json)记录每个真实命令、exit、全部JSON报告及最终PASS；[acceptance.json](acceptance.json)逐项验收5/5。

## 2. 当前测量：单次路径试验，不是统计性能论文结果

| 条件 | P的V完成窗口 | PN的V完成窗口 | PN相对P | A吞吐 P / PN（B/cycle） |
|---|---:|---:|---:|---:|
| 新冷目的页，FIFO DDR模型 | 92334 | 94427 | V耗时+2.267%；A吞吐−2.217% | 2.8391 / 2.7762 |
| 同上，DS读优先返回干预 | 80252 | 79911 | V耗时−0.425%；A吞吐+0.427% | 3.2665 / 3.2804 |

窗口内V/A均完成64个4KiB拷贝。A吞吐按完成字节262144/W定义，含窗口首作业carry-in的常规完成计数，不是逐作业仅窗口内读取字节。窗口后的未完成请求随后STOP/排空检查；WINDOW打印本身不构成PASS。合成CPU请求数随反馈时序变化（FIFO P289/PN313，优先P87/PN80），不能声称CPU流量完全相同。没有新冷页S/N组，不能说PN超过P/N。

DS优先是在C++模型里，从已接收DDR读队列挑最老DS请求优先返回；保留同ID顺序与已持有RVALID，允许不同ID返回交错。它不是DUT AR仲裁修改，也未证明板上MIG可实现相同返回选择。保留为因果干预/理想参考，不能用它代替可实现的入口普通强基线，或宣布所有候选都被现成硬件覆盖。

## 3. 路径证据与未达到的预测

[首V作业证据](acceptance.json)及[cold-events-analysis.json](cold-events-analysis.json)：
- FIFO P：V源DDR AR在16、26；新目的叶PTE AR1554、返回1601、V fill1657、目的DDR AW1664/1674、V done2181。
- FIFO PN：同源AR16、26；叶PTE AR2107、返回2160、fill2216、目的AW2223/2230、V done2740。
- 两种优先干预也都是源AR16/26早于新目的叶读/填充。四组均只有一次V walk/fill；全局新增3个V页表读，源仍命中但首作业不走N。
- **首V两个source capture的FAST位均为0。** 所以证明了“源已缓存、目的未翻译时源数据可进入共享服务”的一般可达性；未达到预稿中的“同一V作业FAST源读先于自己的冷目的翻译”严格预测。不能将后面V的62/14个FAST误绑定到首作业。
- 旧热页日志128/128源burst也早于物理目的AW；旧fillV=0。缺AW不能反推目的翻译未完成，尤其AW可能被输入W owner挡住。

Codex直接核：
1. `rv_iommu_trans_ctl_wrap.sv:625–651`AW-first只针对该TC可见请求；完整4KiB缓冲证书可放行源AR。
2. `rv_iommu_banked_ctl.sv:235–276`输入AW/W demux的owner及未填写计数；它不同于completion mux的物理owner。
3. `rv_iommu_trans_ctl_wrap.sv:329–357`正常物理请求使用成功翻译的spaddr；正常DDR AW证明该请求已译好，但不提供精确翻译时刻。
4. `tests/nonblock_actual_dma_system_top.sv:1292–1307`wait flags低bit0是另一bank占W，bit4是软件事件附加的stall标记；17不是TLB miss。
5. `rv_iommu_tl_wrap.sv:303–305`N窗口只允许主PTW另一bank候选。不能把每个PTWactive/FAST pulse都当数据角色。

## 4. 更清楚的阻塞链：另一作业的提前数据

[first-walk-interference.json](first-walk-interference.json)记录：
- PN中V目的walk期间，A下一作业两条FAST源读capture1221/1227，DDR AR1226/1232，先于V根PTE AR1263。A源数据为0x90001000/0x90001800；它的目的AW在2728/2735才握手，输入W路径此前由V占用（bank1 wait17）。
- FIFO P没有N：A同页数据AR1671/1681在V fill1657后；V根PTE读1259→1291，模型延迟32。FIFO PN根PTE读1263→1840，延迟577，增加545周期。
- 优先干预PN仍允许A的同两条早读，但V根PTE1263→1295，延迟32。V总PTWactive FIFO P455/PN1010；优先P/PN均435。这支持本模型的服务等待机制，不是需要多少理论带宽的测量。
- V的已返回源数据及目的翻译、输入写owner释放、A等待的数据形成完成依赖；“有返回空间所以安全”不等于“这次提前读能马上推进完成”。

不能把上述545/555等差值相加；它们是同一等待链不同投影。也未用它解释完整2093周期差，更未证明旧实板PN约12.7%退化全部来自同一原因。新冷目的是人为路径夹具，旧热工作集没有V walk。

## 5. 创新审定与下一项

C的“自己的目的是否译好”作为唯一决策依据偏弱：首作业P/PN都能源先行；这次额外FAST发生在A，而关键页表遍历属于占W路径的V。仍沿多DMA共享IOMMU性能主线，但必须把翻译准备和实际可执行性分开。

**不升级C为论文主方法，也不宣布C已被完全否证。** 在两bank、同一主PTW仍活动的稳定片段，FAST候选bank与主bank相反；如果主bank也是输入W owner，该FAST读属于非owner；如果两者相反，FAST可属于正在排空的owner。这一类差异用现成owner、主bank及读标签就能表达，未必需要C新增两端prepare表。此处是条件性架构推演，不是所有动态/多作业状态的等价证明。

下一先把普通对照写清：在相同返回空间和合法在途规则下，保留owner排空所需源读，对非owner提前读设有界额度；再对照C的额外目的prepare信息，寻找**相同owner/PTW关系与容量下仍须作不同选择的真实反例**。若无法给出，或普通owner限流等效且成本更低，就停止C主贡献推荐。不得把上述owner规则重新包装成创新；不自动再做BIT、172或大参数矩阵。

这一路需要可实现的AR前准入、真实job/段绑定，不能偷看之后的PTE miss，也不能撤回已声明VALID/已接收的AXI事务。完全不发非owner读与固定小额度、页表优先、两端预翻译/普通预约均须公平对照；已退下的B没有因新夹具自动恢复推荐。

## 6. 前作和限制

重新核[MMU-aware DMA ICCD2018 §IV-C](https://arxiv.org/pdf/1808.09751)：其混合IOMMU/DMA按burst记录地址、length、AXI与transfer ID、读写类型，失败后排空重发，不能把burst身份/准备/恢复泛称新贡献。它不是本平台同式的成对readiness额度规则，也未证明本候选已被逐项做过。另核[rIOMMU ASPLOS2015 §4/§5.4](https://dants.github.io/papers/riommu-asplos-2015.pdf)基于ring的下一rPTE预取；并非这里两端拷贝控制。此前MASK、AXI-REALM、Arm DMA-350及Criticality等限制继续成立，NB-Walker全文缺口保持。

[AMD UG586 Round-Robin](https://docs.amd.com/r/en-US/ug586_7Series_MIS/Round-Robin?contentId=4PIcMe5PnbQ~08AbSve5Ng)描述读/写地址通路选择，不能据此认定支持本模型的PTE-vs-data读返回重排。板端QoS可用性及效益未验证。检索未找到完全同式不等于创新成立。

DeepSeek三次完整high/最大393216、未截断：285238＋74532＋133029＝492799 provider tokens；完整任务/答案/原JSON保留。它没有执行测试或写工作树。Codex独立预稿、关键设计/机制分析、来源核查、修补、执行和最终验收。没有Claude或新Codex子agent。


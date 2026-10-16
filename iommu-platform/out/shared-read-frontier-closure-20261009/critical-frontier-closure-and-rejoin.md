# §55：前沿末拍关闭W义务与中途合流

Codex关键推演，2026-10-09。直接只读本stage共享read/q-writer及DDR RR关键行；没有改RTL、构建或运行。root正在验收build01。本文仅接受当前q2、A第一前沿128、V整段256已认证、静态正常权限、external pause禁用的合同。

## 判定

接受新增 [shared_pretl_read.sv:101](/home/xvhaocheng/work/iommu-platform/out/shared-read-frontier-closure-20261009/shared_pretl_read.sv:101) `beat_index+1 == certified_end_q[f]` 禁止借槽：它使A的frontier最后R进入skid之前，末q-W已经global first offer。于是A到128后的pending只禁止新child，末q仍是必须兑现的旧AW/W/B；首actualWRITE parent的B和下一页rawAW/preparation不再被错误延到V末尾。这是实际义务闭包修正，不是调整统计口径。

中途“停V、补A gap、再合流”不能从memory RR推导安全。现read控制器只有一个physical child route；即使扩两route，已VALID且held的V R也不能让A补读超车，还可能有V Whead阻A排空。默认回退应继续V到RLAST，再补A。只有新WR已经成功、两个cursor相等且无skid时，直接retain不需要gap修复，也不能停V等未就绪的结果。

## 1. 末拍coverage门为什么不让group barrier自锁

当前borrow允许R领先raw_written至多2拍（read99–102），但frontier末拍127现在必须已有A Wcoverage128。A pending由returned_A≥F_A=128才触发（165–166），所以在末AW first offer之前不会触发该pending。

具体F128/q2事件链：

1. 前一q62需要R124/125；这些都已返回，故它的W/B不依赖被新门挡住的R127，可以退休。
2. 双方groups可到63。V末q63即使先offer也不能在缺R127时完成B，所以不会先把groups推进64而令A生成q63的peer条件失配。
3. q writer72–80在双方groups=63、当前WR有效、A pending尚未发生时创建A末q63，global first offer提升coverage到128。
4. coverage门放行R127，冻结对应skid mask并按各logical握手返回。A returned达到128后才pending；q writer的hold只限制新child，不撤旧offered AW/W。
5. A末q63真正W/B退休前child_live仍为1；qquiet不能提前成立。随后返回first raw parent B、捕获下一页AW，后台WR准备可与V剩余长R同时存在。

因此新门在这个齐边界合同中没有“要R127才有末AW、要末AW才有R127”的循环。它要求AW的first offer而非physical accept，也没有改原logical RLAST或提前生成B。firstVALID保持仍由read129–132、q writer54–59检查。

这不等于通用任意frontier证明：例如A dst offset4088，首actualWR只有1拍，q=2且V q2 W在W序前。R0返双方后A到F1；V旧q需要R1才WLAST，A旧1拍W/B又等V Whead；required A在R1被F1挡住，detach还等A B，形成环。当前offset0/3072避免此非齐边界。扩任意前沿时需共同切W边界，或先在R义务齐备时解除A未来R成员位、随后独立等待旧W/B quiet；不能免费推广本guard。

## 2. 前沿关闭后，WR2确实可以提前请求，但RR仍是条件

read167等旧qB+无A skid才detach；q writer99–100返原B后清parent_live，rawcapture不受source hold限制，新的actualAW提供真实WR2键。V的full sponsor门184与已保存WR response使它不依赖WR2；退组也解除peer-group barrier，V可继续自己的q-W和旧physical R。故WR2请求能够进入DDR，区别于§54只在V末尾才产生请求的负例。

实际RR [multi_test.cpp:981](/home/xvhaocheng/work/iommu-platform/out/shared-read-frontier-closure-20261009/multi_test.cpp:981) 仅在不同full RID的beat boundary切换；988明确要求`!ddr_r_held`，1036–1040才发布当前RVALID，2169/2700保存held状态。V继续消耗时有机会出现可轮转边界；WR2是否在旧sourceRLAST前返回必须由实际时间戳/rotation验收。selection非零、或AR已经接受，都不能单独证明PTE响应已穿过held data R。

## 3. gap修复的两个独立阻塞

若WR2成功时V cursor=v>A cursor=a=128，重合流须先给A正确source[a,v)，使其cursor真正达到v；不能直接跳cursor、重放physical RLAST或把V已经消费的旧数据当作现skid。正常immutable source允许重新读这段，但当前1拍skid不保留整个gap。

**R通道反例**：旧V physical AR长4，A在第1拍退组，V又消费第2拍。此时V下一拍R2已经VALID；controller冻结V等待A补R1，再发/accept一个A gap AR。DDR必须保持旧V R2，无法切到A R1；controller又要A追齐才对V READY。所有请求正常、权限成功，RR也不能轮转held beat。即使最初停V时R2尚未VALID，memory在gap请求latency未到前仍可合法先offerV R2，随后同样锁住。

当前read179的新AR还要求!offered/!busy/!skid，221–223只接受唯一child ID，所以代码根本没有同时发A gap AR的第二route。增加它也不自动消除上述物理单通道环。

**W序反例**：即便不同RID的R可独立回来，冻结V时若旧V q/long W还缺下一V R，它仍占Whead；A gap数据填满有限private B后不能向自己的W排空。gap>B时A追齐也会停住。要冻结V追赶，必须先关V新AW、补齐所有已首offer V W所需R并清其W/B，才能保证A gap有写出口。若V剩余longW已首offer，这个闭包本身要求V整尾，等价于默认回退。

## 4. 可用的安全选择

- 当前成本下：继续V sponsor至旧physical RLAST/最后logical R，然后按A实际cursor补读，保持原RAW父和真实READ/WRITE recipe。这不等所有flow关闭，只是不给A补读制造更早W/R锁。
- 无gap快速路径：仅在新WR证书已经有效、两个cursor相同、整个skid为空时retain。read171–173当前正检查这些条件；不得把“等下一证书或等A追赶”塞进这个路径。当前serialized raw parent合同里该机会是否实际出现要看trace，不称必然可达收益。
- 真正中途repair：必须另证明不同RID response即使V停止接收也能服务A，或提供有界且足够的V tail停车；同时关闭V旧W依赖。仅普通AXI RR没有这项保证。有限停车须覆盖gap读延迟期间所有先到V数据；如果memory可合法先持续给V，最坏需要剩余V tail，不能只数新skid1拍。

丢弃旧R尾、释放route后再读A gap和两方共享suffix也是合法immutable-source回退，但尾R仍有物理工作。设old长R N、A退出a、V停v；旧R全尾已消耗N，加gap(v−a)与新共享suffix(N−v)，总R=2N−a，等于默认N+(N−a)，还增加AR，不提供免费流量改进。

新closure guard可以真实打开“WR2与长data R并存”的下一判别；它不同时证明mid-gap合流。若实际RR让WR2及时成功，可在保持V连续进度的前提下考察是否有不需停V的恢复信息/存储收益；当前single route与有限payload下应保留上述回退，不用暂停V来创造看似较小的补读gap。

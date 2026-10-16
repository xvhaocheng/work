# 两BIT共用软件：实施合同 v2（2026-10-03）

用户已授权设计后实现、验证和编译。此文件冻结实施接口；实现及主机测试完成，共用软件00093与两BIT构建和必要时序门槛通过；方法版使用独立记录的物理优化产物。未上板，收益及创新尚待实验验证。

## 交付与兼容

两profile：`migration-4dma-research-e2-v2`（role=1）、`migration-4dma-research-method-v2`（role=2），50MHz/DS50/50、16项IOTLB、同设备树及软件。legacy E3详细追踪保持关闭。原research v8/COST/fullstats ABI不变，原84/21清单不变。当前00092缺必要软件接口，本次集中构建一份新payload；之后切换BIT不改SD。Mac脚本核对哈希、role和软件接口，保存第一段到Mac后才切换第二BIT并重新启动。

新增ioctl `IDMA_IOC_RUN_RESEARCH2`（0x0f），新struct包含旧`struct idma_research base`和定长config；原0x0e的输入布局及参数谓词不变。config包含ABI/size、required_role、A访问顺序、policy_mode、window_cycles、target_bytes、deadband_bytes、initial_budget、min_budget、max_budget、budget_step、credit_cap、hit_weight、miss_weight、ptw_weight、reserved。具体C布局与寄存器宏已写入UAPI。输出仍使用base/raw4096，不用用户指针暗传配置。

新增只读能力区F00=0x49444d4132434150（IDMA2CAP），F01=1，F02=role，F03=feature bits：bit0 extended E2、bit1 E2 probe timing、bit2 reverse A order、bit3 method control（仅role2）；其余保留0。旧硬件无magic必须在启动DMA前拒绝新ioctl。CAPS查询只读，不改变实验状态。role写死profile参数，不由用户伪造。F10为A反向顺序配置及读回，bit0有效，仅quiet且观察未开始时可写。

## E2扩展

base.mode仍为1，新ioctl允许W0/1/4/7/8/16，A次数0（仅W0）或1/8/16/112；warm2/probe1/4KiB/固定5,000,000周期A阶段/2000ms不变。A顺序0正向/1反向，反向在batch起始从最后页开始，页内不变；短前缀记录真实覆盖而不假称完整工作集。每例重新准备映射和初态，旧E2完整证据链继续校验。

E2时间块D80=0x49444d4145325431（IDMAE2T1），D81=1，D82=error，D83=flags（issue_seen/select_seen/done_seen bits0..2），D84=probe issue epoch，D85=首次V select epoch，D86=V done epoch，D87=done-issue周期，D88=first_select-issue周期，D89=V REQ_TRANS/AWAIT_RESP占用，D8A=V COMPLETE周期，D8B=V select次数，D8C=V hit次数，D8D=V miss次数；D8E..D8F保留。仅成功接受PROBE后、V issue至done区间记录；完成沿纳入，issue沿作为起点。清零仅沿既有合法CLEAR。重复issue/done、done无issue、错误响应/不支持/溢出应报错。V归属DID10；hit/miss的存在性与时间差独立。

这里只测首次select前等待、实际翻译控制占用和整次DMA完成时间；总时长减去翻译并不等于纯等待。5,000,000周期不是替换代价。时间块可在两BIT都有，但正式E2扩展按role1执行。

## DMA放行接口

frontend/wrapper新增默认关闭的ResearchPort、policy_enable/allow/abort和batch_reverse输入。只有A实例启用；普通旧实例参数关闭时行为不变。反向顺序在batch创建时锁存，仅影响页间偏移。新增ADMIT状态，不发backend VALID。

策略ARM在双方完全quiet时完成；M会在第一份batch及每份完整copy之后经过ADMIT，只有backend_idle且allow才进入COUNT。COUNT一旦提出VALID须保持到ready，不允许策略撤回；当前copy和已接受事务照常完成。WAIT完成后才能进入ADMIT；已有gap先走GAP再准入。STOP与显式ABORT仍当拍处理；新出现的协议/统计错误当拍关闭ADMIT准入，锁存后的sticky错误从下一周期驱动终止。ADMIT或GAP退出不制造obs_done；COUNT/WAIT的已承诺copy排空后退。backend idle与copy完成需结合验证，不能靠软件轮询单份。

## 方法控制（独立模块，bank E00起）

role2才允许ARM；所有方法对照采用同一合法ADMIT边界。mode0 unlimited（仍记录）；1 bytes（A issue扣4096）；2 request（select A扣1）；3 miss（A lookup miss扣1，零miss不扣，明确为不保护hit竞争的反例）；4 observed weighted events（A lookup hit/miss按配置权重＋归属A PTW返回事件按ptw_weight扣费，不能称完整DDT/PDT类型覆盖）；5 actual（owner A且REQ_TRANS/AWAIT_RESP逐周期扣1）；6 V-only反馈gap；7 stale actual（上一完整计费窗A周期总数在下一窗末扣除，首窗无历史，明确这是滞后对照而非纯噪声）。

计数型/实际型同一外层反馈：有符号64bit余额C，ARM初值initial_budget；每拍先扣费，窗末按旧b补給、仅正端credit_cap封顶，再更新下一窗b；allow基于这个计算后的余额>=0。负债不裁掉，不在V起止重置。溢出/观测错误sticky后禁止新放行，仍排空已有copy，保留账目并标记不可信。ARM/CLEAR仅quiet且非armed；FREEZE须quiet；ABORT禁止新放行并继续在途计费到FREEZE；FREEZE保留abort状态位，CLEAR才清除，配置在armed期间禁止修改。所有模式不抢占请求。

window配置有界，默认16384周期。V batch_active为活动依据，成功DMA done每次4096B；只对从首拍到末拍均V-active的完整窗更新。完整活动但零完成也更新；部分窗不更新b。target低于观测则提高b，低于target则降低b；deadband和step配置，b限制[min,max]且计费模式min>0。mode6反馈方向相反（V慢则增gap），b为gap周期，min可0；A done装载当前gap，首份不加gap，等待与计费模式公用ADMIT。V结束不清计账。

记录完整活动窗数量、V完成字节和目标不足次数/不足字节总和、V每窗完成次数直方图（0..30及>=31）、最长V活动无完成周期、A放行许可关闭周期（没有frontend等待输入，不能称实际阻塞周期）、总计费、总补给、期末余额、预算min/max/期末；总时间、V/A同窗工作继续使用已有COST/P4M/fullstats。统计窗与V硬件共同窗不同，输出分开标明。所有硬件信号同外围clk域接入，owner来自已审e3_ctl，PTW事件归属须结合当前owner核查。

E00 magic=0x49444d414d435431（IDMAMCT1）；E01 version1；E02 error；E03 status bit0 armed/bit1 frozen/bit2 abort；E04 command（1 ARM/2 ABORT/3 FREEZE/4 CLEAR）；E05 mode；E06 E；E07 target bytes；E08 deadband bytes；E09 initial budget；E0A min；E0B max；E0C step；E0D cap；E0E hit weight；E0F miss weight；E10 ptw weight。配置均32bit无符号，写入高位非零拒绝；其他写地址拒绝。具体统计输出E20..E3F及E40..E5F直方图以模块头及分析器映射定义；直方图总和等于E20完整活动窗口数，不是E21完成次数。

## 实验与验收

E2：新版小量同版对照＋W7/8顺序复核＋W8/16前缀，X3随同采集，不重跑旧60。M：短验收、有限校准、冻结参数、核心A8多目标和A1/A16边界、顺序独立验证、计费/滞后消融；清单/重复数及校准算法在runner提交前冻结，不扩大为全笛卡尔积。科学阴性照计划继续，技术错误首错停，不自动重跑或删数据。

必要验证：新UAPI旧路径回归、配置/能力拒绝、role不匹配前拒绝、E2计时与反向前缀真实RTL、准入首份与VALID保持/STOP排空、每拍账目和窗口边沿/溢出/零进度、runner冻结和校准/留出隔离、Mac切BIT前结果保存与错误停止；主机证据与板测严格区分。

## 板前冻结的v2实验清单

下列次数是工程选择，不保证统计显著性；具体可执行清单由生成器确定性生成并保存哈希。技术失败停止，不能为了达到数量自动重跑。

E2共64项：短4（W0正向N0、W1正向N112、W8反向N8、W16反向N1）；边界6条件各5（W0正向N0、W1正向N112、W7正向N112、W8正向N112、W7反向N112、W8反向N112）；前缀W8/16正向各N1/8/16各5，共30。每组5轮循环移位顺序，所有用例同时采集首访计时。新对照自成版本，旧60不并入。

方法共462项：短9（Solo/unlimited-A8各1、policy1..7 A8各1）；参考12（Solo/A1/A8/A16 unlimited各3）；校准89（固定gap0/500/1000/2000/4000在A8各1共5，7策略×3目标×4候选共84）；正式192（固定gap/策略1..7，3目标×8交错轮）；边界40（固定gap/weighted/actual/V-only，A1/A16，150目标各5）；独立反向验证120（8方法×3目标×5交错轮，A8 reverse=1）。除短64/8，均V4096次/预热256、4KiB。参考和校准reverse=0；反向测试不参与方法调参，E2反向数据不可用于选择方法参数。

固定目标120/150/170 MiB/s，E=16384周期；target_bytes=round(target*1048576*E/50000000)，deadband=4096。所有计费模式min=1（bytes min4096）；max=4*initial，cap=2*max；step至少1。候选初始值×步幅：bytes初始32768/65536×步幅1024/4096；request32/64×1/4；miss12/24×1/2；actual/stale4000/8000×125/500；V-only初始gap1000/2000×步幅100/500，min0/max4000/cap4000；weighted初始6000、min1/max24000/cap48000，(hit,miss,ptw)=(5,200,100)/(5,800,0)×step188/750。weighted只代表这些可观测事件，不宣称完整类型模型。

校准每目标按V速率落在[target,1.05*target]且所有技术检查通过者中取A同窗速率最高，平手按冻结candidate_id；如无可行者，选择V相对目标误差最小者并标记unmatched（平手candidate_id），仍执行预定正式用例，不能宣称保护匹配。边界/反向沿用同目标A8选出的参数。参数冻结文件记录全部候选数据/选择理由，不看正式或反向数据重新选参。

比较预注册：主比较以同轮同目标为配对单位。只有双方V中位速率差≤5%、完整活动窗不足目标比例差≤10个百分点，且达到目标要求时，才讨论匹配保护下A收益；A相对增益5%作为暂定工程意义参考，另报告全部配对差异/区间。阈值是本次研究的工程选择，不是显著性或等价证明；目标不可达/匹配不成也保留全部数据并报告。启动/切BIT采集段单独标记，不能以窗口数量替代run重复。

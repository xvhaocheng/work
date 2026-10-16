# A：近期成果输入合同与源码约束（2026-10-15）

本席交付的是方法的必要输入与接受约束。root已选择固定HERO `mm-large.c` 的原生 `height=32` 参数作为完整8copy／16KiB源合同入口。它有真实应用源码、独立线程和自然槽生命周期，可用于新的**有限源模型**；没有据此接受native DDR、原HERO共享IOMMU/SVM、完整CPU应用或FPGA性能。stage117作为真实缓存／完成／成本参考保留，原实验不重跑。

作者为Codex gpt-6.1-sol／ultra独占席。0新实验、0模型执行、0构建、0生产变更、0板操作、0旧validator回放；不操作goal。只写本目录这两个文件。全部28个输入的当前SHA256、42组精确file:line及必要原文在[recent-contracts.json](recent-contracts.json)；原始验收JSON保持。

## 1. root可直接采用的输入边界

| 从哪项成果继承 | A模型必须做到 | 不能据此推出 |
|---|---|---|
| stage117真实WT交接 | DMA收据、实际cache可见、used发布、源复用、真实consumer、归还与排空是不同节点；cache drain/refill/metadata本身进入真实资源竞争 | cache取得提前或used已发布就等于consumer提前；258模型周期是异平台CPU常数 |
| R2生命周期 | source AW、upstream spill、TC input、TL owner、target spill、shared AW首次offer／reservation、真实AW接受逐级记录；prepared成功只转移翻译结果所有权 | TL结果就可发布数据／释放槽；内部AW接受等于外部AW接受；raw WLAST就是B退休 |
| R2普通成本 | 普通对照可以合法复用字段、原spills、原W信用和原序；双方同付新增读口／信息接口 | 强制对照另存完整头、把1329位状态小计当总面积／物理优势 |
| RCS1完整结果 | 重新生成实际owner关键路径；同时比较所有任务max／sum和完整向量，记录真实RWB／consumer／release | 保护更多完整翻译集合、减少walk或PTE局部提前就说明完整任务更快 |
| RCS1勘误 | 64B及本次2048B没有现成FullBuffer permit；actual accepted job资格决定许可 | “能放进buffer”就可越过原read guard |
| AW因果修正 | source侧AW先进入两槽spill可释放private AW；上游AW次序、当前job ordinal、read资格决定后继 | decouple_rw=0或input AW未接受就说明第二AR必未生成 |

## 2. 真实任务来源及完整工作量

固定官方HERO commit `c8481a930a15a385d4f58f519bed83d2e75e2a73`，`mm-large.c` SHA256 `8b16b51169bb31b7eb290eb5f309ddfbba25fa94e3fae3b327baadf491360186`。本次直接读本地原件，未联网重取或运行。

[源码284](/home/xvhaocheng/work/iommu-platform/out/workflow-stream-contract-20261012/source-evidence/hero/openmp-examples/mm-large/mm-large.c:284)–305接受height参数、夹到[32,512]并按原规则计算stripe。height32得到width=height32、stripe_height16、每条带2048B、两条带、六个SPM槽共12288B。完整任务是A2＋B4＋C2共8copy＝16384B；四个计算tile共32768次MAC，C输出4096B。这些是静态源码计数，非新模型结果。16KiB是copied bytes：原HERO外存端为A/B共12KiB读、C4KiB写，另有SPM对应读写；若条件适配为DDR→DDR copy，则有用R/W各16KiB，不能混称原HERO外存总线量。不得把每copy切成独立“完整任务”，也不能裁掉启动/尾部复制。

- 原件44–70给定参数与六个槽；80–107中thread0和thread1分别启动初始A/B，没有跨线程源码总序。
- 114–161中A预取／当前A等待与C(s−1)输出／旧C等待属于不同线程。164–208中B下一条带提前发起，等待当前B后进入共同barrier。
- 214–228的`omp for`无`nowait`；当前A/B被计算读，C被写。合法槽复用须等对应实际计算/同步完成。
- 238–273等待最后C写回，然后退出parallel并释放六槽。28–37、367–410还规定目标返回／host比较每个C元素及host free。

**关键适配边界：** 原main 367–379运行`device(1)`的copy-based `target map`，382–404的SVM分支为`if(0)`并注明未正常工作。原libhero-target/pulp/hero-target.c:58–85是另一套MMIO配置/trigger ABI，134–137解析stream与序号；不能把它的stream自动当成当前AXI ID、DID、四DMA前端。新模型若绑定共享IOMMU，是显式的条件适配，不能称原HERO native SVM运行。root必须亲读这段原文。

height32只有两条带，不具有`t+2<n_stripes`的内部先行窗口；本次A来源是独立pending请求及自然依赖，不能借B1的`t+2`窗口作它的正例。height128等其他原生尺寸的静态计数只在JSON作导航，本席不建议扩参／压力搜索，也未给其正反例标签。

## 3. A确实允许改变的选择，与未核部分

**源码允许的选择**是：独立线程已经产生、且槽／数据依赖已满足的新请求，在该owner首次不可撤回offer之前由一个明确控制器选择顺序。例如初始A0与B0没有应用语义上的相互数据依赖；后继A/B/C还各受原wait/barrier限制。允许不同的应用合法发起次序，不等于所选native下游会同时看到两个头。

**协议允许的选择**是：只有在实际无held、两个候选真实可见且均获准时，才改变新的TL或DDR头。COUNT的VALID一旦首次发布必须稳定；ADMIT/HOLD确有“不发backend VALID”的状态，但其当前资格有限，不能免费视作对所有HERO job通用的预测／prepare接口。源码：`cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:519–524,588–610`。

**本席未证实的选择**是：把这八个source调用绑定到当前前端、正式context/页映射、实际burst与原guard以后，是否在某个具体控制位置自然形成双头共现。没有这一绑定，只能接受有明确服务/发起合同的独立源模型，不能接受native控制可达性声明。源代码允许、条件模型可达、实际native可达三层分别记账。

普通对照必须得到相同未来可计算信息、相同slots/ports及同种合法动作。若候选靠新增snapshot、job关联、prepare或结果采纳前门，接口本身和对照均显式付费。

## 4. 每条请求必须保留的事件链

账本身份至少是`source + job ordinal + direction + burst ordinal + fullAXIID + bank`，再关联正式DID/PID/stage/context、必要mapping epoch和payload slot generation。单个AXI ID可有多笔请求；payload generation与mapping epoch不互代。

最低事件依次区分：地址可算→控制器实际可见→source首次VALID→source握手进入upstream spill→TC input握手→NormalTL首次VALID/接受→TL response接受→target内部AW/AR移交→shared output首次VALID/预留→真正下游AR/AW接受→raw WLAST→target WLAST→target B捕获→实际source B接受→公开DONE匹配→必要cache可见→实际consumer/收据→槽release→最终quiet/释放。

不同链可并发，不能强加一条不存在的全局顺序。决定性原文：

- `cva6/vendor/pulp-platform/axi/src/axi_mux.sv:47–54,188–205,259–270`：upstream AW/AR默认spill，AW仲裁LockIn；source握手与TC input不等。
- `cva6/corev_apu/riscv-iommu/vendor/common_cells/src/spill_register_flushable.sv:38–96`：真实A/B两份完整packet/valid；不是一个象征slot。
- `.../rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:540–557,674–725`：target各通道spill；TL COMPLETE在内部packet接受结束，不等于DDR或任务结束。
- `.../vendor/axi/src/axi_mux.sv:283–311`：W路由预留可在AW首次offer时发生，AW尚未accept；必须与真实接受分开。
- `.../rtl/transaction_controller/rv_iommu_banked_ctl.sv:523–555`：最终write debt直到真正上游B消费，read debt直到上游RLAST；错误sink写也须排空。

旧AW“同current job、A0在B1之前、normal256beat、output前18beat容量”不能形成B2/A0的必要条件，只对原声明范围适用，不能套到改job/deburst后的全族。原条件与修正见`out/aw-barrier-causal-20261014/root/entry-and-predecessor.md:11–31`；此轮不恢复AW分支。

## 5. 资源、read guard与缓存的硬约束

`idma_fullbuffer_certificate.sv:14–18`要求**整个actual accepted normal copy恰4096B且源／目的4KiB对齐**，容量至少4096B。`.../systems/cva6_reg/dma_core_wrap.sv:173–183`从实际frontend接受和backend响应维持证书。本次2048B条带permit=0；不能按容量足够推许可，也不能拆成假4KiB证书。HERO malloc本身也不保证4KiB对齐。

`rv_iommu_banked_ctl.sv:243–276`保留AW/W真实owner、首次可见AW的读准入预留及raw WLAST；`rv_iommu_trans_ctl_wrap.sv:625–647`只让符合原read_admit或真实FullBuffer/DID的AR进入。每bank READ/WRITE共用一个TL owner；不能用不存在的额外tag或结果pool绕开阻挡。对source-only模型若不实例化这些native部件，应标为未建/不主张native结论，而非默许它们零成本已通过。

HERO原任务使用SPM，所以不能人为要求它承受stage117整cache清空。若条件适配将C或consumer置于本平台非一致性CPU cache，则必须给出真实可见性合同、WT drain、metadata顺序及实际refill；不能只插一个免费的“可见”事件。stage117提供正确性范本：

- `implementation/real_cache_dma.cpp:91–111`明确acquire→validation→WT-drained used→源覆写退休→缓存consumer→consumer/归还metadata。
- `implementation/cpu_cache.hpp:23–33`等WB空、MSHR退、cache AW/B守恒，之后完成fence ack/halt/flush。
- 生产`cva6/core/controller.sv:82–112`显示WT普通FENCE不清Dcache，FENCE.I控制输入请求I/D失效；`wt_dcache_missunit.sv:398–405,477–490`先排空再完整清空。

stage117测得258周期针对所测端口驱动共同模型时钟，不能搬成HERO CPU指令／Linux／应用时间，不能在新竞争条件下无条件固定为258；原micro待完成WT/MSHR会额外等待。来源报告`docs/stage117-real-cache-handoff-20261013.md:9–29`已有准确null边界。

## 6. 完整完成与最低成本向量

root应在运行前声明测量的是“完整源kernel合同”还是“完整应用交付”。前者至少完成八笔真实copy、四tile实际数据计算、每笔对应R/W/B、两次C写回和最后wait、六槽release及quiet；不得称完整应用CPU-inclusive。后者还包括实际target mapfrom可见、host consumer比较1024个C元素/4096B、host free和所需缓存交接。可核consumer字节/值而把未建CPU时间留null，但不能把缺少的可见性路径写成已发生。

每一策略最低报：各逻辑任务consumer／release完成向量、max、sum、启动前缀与尾部排空；各copy真实R/W/B和成功状态；源/槽持有bytecycles；全部PTE读/bytes及翻译owner；有用字节与总线补齐分列；供给/poll/MMIO和计算/同步、cache drain/refill/metadata、校验/consumer/归还、终末observer及observer后的新quiet分列。源task、helper、后台若存在须真实完成，不能只报目标job。

守恒必须检查每个有效W字节恰一次、读返回覆盖、AW/B匹配、TL接受/响应恰一次、所有spill/held/owners终末排空、无未归还slot，最终矩阵/guards正确。只看DONE、PTE计数、R-last或PASS字符串不足。正常成功范围可先冻结，NO_ERROR_HANDLING的done不能认证一般fault／reset／cancel／invalidation。

静态bits不等于物理面积；新增信息生成、owner/ordinal关联、传播/CDC、读写口、仲裁/max算术、selector延迟、丢掉的数据重叠、软件注册及原生公共组件都计。未测CPU／应用／LUT／FF／BRAM／Fmax／能耗／板端值为null。R2原1329＋common逐项等价继续有效，不重新把原位复用包装成独有收益。

## 7. 接受条件与root须亲读的入口

本席交付足以固定输入合同；是否执行由root把算法、服务合同、准确源发起适配和正例／邻例／两留出补齐决定。当前缺项是：M1 source到当前native或独立条件模型的确切映射与选项；M2自然common-prefix候选共现；M3真实context/residency/依赖PTE；M4完整compute/cache/service的时基或具名条件；M5统一普通策略/总预算和事前预测；M6实现／软件成本。JSON逐项保留unknown，不能用旧stage117或RCS1轨迹代填。

root最低亲读：

1. HERO原件80–107、114–228、238–273、284–305与**367–404**，确认自然发起/slot及copy-based边界。
2. stage117 `real_cache_dma.cpp:91–145`和`cpu_cache.hpp:23–33`，确认consumer、fresh drain及逻辑release。
3. `rv_iommu_banked_ctl.sv:243–276`、`rv_iommu_trans_ctl_wrap.sv:625–725`与FullBuffer证书14–18，确认真实读资格。
4. upstream mux47–54/259–270、target mux283–311，确认AW端点/预留。
5. RCS1报告首行勘误及23–38，确认集合与完整收益分开；旧32项条件引擎不能重标native。

本轮不恢复MIG goal1792023314、已结束ACV-2x2、R2、RCS1或AW旧实验；不改全局文档和production。本文不作全面novelty裁决。

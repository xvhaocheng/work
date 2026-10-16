# Codex独立新规则草案：译址与写进度共同决定源读边界（2026-10-08，§29）
用户要求推进创新，已有方法只用于比较。Codex独立提出以下草案，尚未实现/认证创新，不能包装新专用lane/优先级/普通credit。
## 动机与本轮新证据
现成同资源DS-return priority两真实iDMA P/PN实测88989/91547cycles，V/A各64，FAST0/62；DS从DDR接受到首R平均及最大均32cycles，A PTWactive均13489。PN仍慢2.8745%，不能从这个差直接认定Wowner单因果；CPU读221/253、工作量不是全相同。生产DS与comp发起端口已分开，最终同DDR接口。
## 独立研究问题
D曾固定长源R+只存后缀。本轮增加新的决策量：源R长度本身不再固定。源/目的合法分段不同、目的准备程度与共享写顺序随时间改变；不同copy会争少量返回容量。可否用依赖驱动的动态分段，以很少payload状态获得长读效率且保证关键翻译能进展？
不是认为“可用普通原语表达”即可否定创新；同时不能只给构造出的普通方法换名字。必须比较最强自然简单规则。
## 规格G（候选）
先限64-bit、8-byte对齐、普通copy、静态有效映射。将源合法最大片段Lmax、目的虚拟/物理写顺序、有效目的译址覆盖映射到copy逻辑byte。
p = 当前逻辑起点开始，有确切目的准备+无更早写依赖+独立写服务承诺的连续可消费长度。命中/瞬时WREADY不够。与accepted AR的同ID顺序、未消费返回头一致。
每个新源片段在iDMA legalizer产生虚拟AR首次VALID之前选择长度L并冻结；不是收AR后截断、不是abort/retry。physical AXI,len,Rlast和逻辑copy byte计数严格保持。
可行域：0<L<=Lmax，尾部预约max(0,L-p)<=free_credit；必须保留当前义务所需的头/服务槽，预约byte退休才释放，不能由一个p重复承诺多个AR。
### 初版两模式选择
- 若p覆盖Lmax：发Lmax长R，可横跨多个已备且真能依次消费的目的W碎片；不需为直通前缀预约payload。
- 若0<p<Lmax：优先在p边界结束R，给后续目的翻译留下返回通路；后续准备进展再发下一R。是否借free_credit延伸超p留给第二判别，第一版不引入估计器/固定周期阈值。
- 若p=0：不发无空间保证的payload R；仍允独立目的准备/安全普通整返回预约回退，必要的当前owner源读必须有安全可行路径。不得用“等p”制造一条源读与目的准备互等的新环。
这两模式的自然最强对照：greedy longest-feasible L=min(Lmax,p+free_credit)、始终共同分段、始终独立最大段+准确尾部池、固定各种burst长度（取各负载oracle最优）、普通独立PTE/返回VC及同资源DS优先。已知greedy能写出相同hot/cold决定时不能声称独有。
### 条件算例（非RTL结果）
源页首,len2048；目的偏移3072=>W1024+1024。两W均有独立可消费合同且写序连续时，G可R2048（1个AR），固定共同分段2AR。只有首W符合条件时G R1024，等目的进展再R1024；固定独立R2048则需可靠缓存后1024或能让PTE越过被堵返回的独立服务。不是最低容量定理；W2准备能否提前可达及其接口成本必须核。
单DMA只用p/credit的greedy规则可能已全部覆盖此例；这会停止“新算法”推荐。进一步价值只能来自多DMA竞争下改变L的具体决定/总状态代价，不能加空泛criticalityscore冒充新意。
## 真实接口/实现边界
iDMA idma_legalizer.sv的decouple_rw及r_num_bytes_possible/r_num_bytes本来按src/dst几何与option生成片段；插入动态cap必须在地址offer前latch并同时更新r_tf/src manager/datapath offset/tailer。不能改len但漏改字节数。需要合法export的copy/准备进度接口，不能测试层次oracle零代价。现CONF0对齐旧实验不能产生本例，不能解释旧PN差。
无新的RTL/Linux改动/构建；本轮唯一新执行为已有binary的2项DS-priority基线干预。
## 前作危险点
iDMA几何分段/decouple、AXI-REALM runtime configurable fragment+Wbuffer、TAPA runtime coalescing+flow control、CTC动态packet ending/credit、MMU-aware DMA page-bounded bursts+faultreplay均已读。AMD专用VC/MASK队列优先不是新点。查到TI2025AdaptiveBurst原专利允许FIRST/LAST提前结束/延伸进行中burst、FIFO/priority事件（不是本方案标准AXI既定len合同）；另查到2026ECCadaptiveDMA仅二手摘要/DOI无法拿正文，不能据此排除。Author-host VDC2011显示根据页/Buffer状态发可行最长burst，但未核目的翻译依赖。
本轮希望DS独立找“该规则是不是本质ordinarygreedy/只同样credit产生burst”，给反例/形式化区别/最近邻；不能靠没搜到确认首创。


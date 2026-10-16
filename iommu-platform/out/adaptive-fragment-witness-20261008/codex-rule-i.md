# Codex独立规格I：译址后、内存提交前按目的可排空范围重分段（2026-10-08，§30）
## 从G/H推进的关键变化
G在源虚拟AR首次VALID前动态选L，但idma_backend将legalizer AR先装入fall_through_register（:613–627），W还要经过fall-through、TC翻译/physicalAW；此后AR可能已VALID，不能等待p之后改该ARlen。还需先出W才能产生p，单纯把cap放legalizer有因果/接口问题。
新的I选择另一个合法决策边界：接受并保留原逻辑AR地址/ID/length；源翻译成功后、每个下游物理AR首次VALID前选分片长度。不是取消已VALID/已接受物理AR。
## 合同
上游逻辑R为2048，64bit共256拍。下游可R1024+1024（各128拍）或R2048一条。上游仍256拍/原ID/单RLAST；第一下游分片的RLAST不发给上游，最后原请求剩余计数归零才发RLAST。所有字节/错误保持，sameID顺序、VALID保持；不能对非INCR/ATOP/跨协议随意启用。
原源AR本来不越页，因此源译址所得物理连续区覆盖完整逻辑R，不须为物理子片每次重跑TLB。目的W多页必须独立取得合法译址及具体写顺序，不靠瞬时WREADY承诺未来。
基础控制（候选，未认证新算法）：每次从已译源AR剩余物理连续片中发最长、已具有目的消费保证的前缀；合法后缀预约是可选扩展，初版不借池。
已备目的两W且写序连续=>单物理R2048；首W1024已备、次W未备=>先R1024，释放物理R服务，再等待W2后发R1024；p0等待安全owner进度并保证W/DS控制能继续。上游原R可跨间隔保持未完成，不锁物理DDR队头。
## 相对强简单方法的拟议不同点
- 固定源/目的共同分段：冷形状也2R，但对冷热都重复原源请求/源TLB检查，热形状不能恢复1R。
- 固定独立长R+整返回预约：热1R效率，但冷需缓冲后缀；精确ordinary cut-through只存后缀仍需1024额外容量。
- 固定post-IOMMU AXI fragmenter：也避免重复源译址，但固定小cap冷热都2R，固定大cap冷仍需安全尾池；必须比它，不声称拆分/reassembly首创。
- 已发表针对同copy目的有效准备、虚拟/物理写序选择post-translation片边界的fragmenter若已有，I新规则就可能重叠；普通greedy放同位置/同p与I当然等价，不能仅凭它可表达就说已有发表，但须计入相同接口/状态成本。
## 最小真实判别
现真iDMA src0/dst3072 copy2048、CONF0共同R1024+1024 / CONF1源R2048目的W1024+1024；先复用compiled system、只配置C++并核实际DDR AR/W2ready/virtualRlast、正确字节和排空。先判断目的W2能否早于源R开始合法准备。CONF1原小FIFO已安全且p总覆盖完整R时无成本空间=>停；冷p只有1024而原长R卡住或需尾池才有实现空间。
任何当前目的p不在接口里都计额外 metadata/logic，没有免费层次oracle、未来PTE预知或共享PTE/payload信用。
## 未解决
元数据配对/原R串行头、fault中途原剩余字节如何返回、映射失效/epoch、token寿命。先静态有效映射普通alignedcopy做最小机制，不用它推导全Linux/故障正确性。物理fragmenter通用liveness未证明，无性能/面积/板卡结果。


# §57 补读接力：运行前根级设计

问题：A因未准备下一WRITE退出共享R后，即使其下一WRITE已真实成功，现有实现仍让V读完、A串行补完。直接停住旧V physical R等A gap AR会形成单R held环；只增加交错也可能留下V旧W队首等数据、A补读FIFO不能继续的环。

候选协议：A successful WRITE job end达到256、双方own READ recipe仍有效且V旧physical child busy时，冻结V新缺payload W；V logicalR继续到全部已firstVALID V W需求实际进入DMA FIFO且没有skid。记录J=Vreturned，再真实接收并丢弃旧physical child未返回逻辑的数据直到其真实RLAST；不撤AR/R，不造B/逻辑RLAST。之后用A own READ recipe从其cursor FA补读至256，FA..J仅A，J..256同时供应双方独立logicalR。repair prefix期间V只能继续全payload已有W，repair到J再开放V新W；A恢复W门不再等V logicalRLAST。保护V own WRITE全长及READ权限；静态immutable正常内存源、原属性匹配、单job。原物理R退休不等于DMA原逻辑读父退休。

只保留1个physical child和原8B skid；拟增phase2bit、vcut9bit、oldend9bit约20个功能状态位，另计所有已有recipe/parents/metadata和观察器，未经综合不可作面积结论。A WRITE迟到或FIFO memory无真实跨ID进展时不进入，保持V继续的安全回退。

强对照：cutover=0同一硬件/夹具；mode0普通共享前缀/独立后段也允许相同新规则，辨别该协议与长AR选择的不同贡献；mode2共同R/W切分与mode3独立读。若仅减少AR常数不改善整体点，不立性能创新。若两个burst组织加入同一新规则后同效，应视为规则的集成/ablation，不把“可改控制器加入候选”当已有前作。

driver新接口（普通夹具提案范围）：top input shared_source_cutover_i（bool，默认0）；outputs dbg_shared_cutover（实际进入sink/repair）、dbg_shared_cutover_vcut_bytes（J*8）、dbg_shared_cutover_oldend_bytes（旧physical child终点*8）。CLI +source-cutover=0|1，+cutover-require=0|1（默认0）。严格source R expected：若实际cutover，则oldend_beats+(256-initial_A_cut_beats)；否则沿用mode3 512、detached512-cut、shared256。所有目标内容/未写区、logicalR/W、issue/done原断言保留。另打印SOURCE-SPONSOR-HANDOVER-JSON真实opt-in/actual/J/oldend及derived expected。不改memory模型、不伪造事件。

## 构建前关键修订与初步执行（04:35 JST）

关键审查给出q2错位反例：FA1/J3，V已有1拍未承诺payload，A先发[3,5)而V后发[2,4)，R4被V两拍库存门挡住会再成环。实际修订：触发同沿阻新R及V缺payload新W；J=ceil(max(Vreturned,Vpromised)/2)*2固定，V继续到J并全部真实rawW取走（raw_written==J、无skid）才sink。A修补prefix末child截J；joint new W start<=peer actual raw_written，维持共同q2网格而非已不相等旧group计数。强制keep-q1，sink/repair physical READ error直接fatal（没有故障恢复合同）。20个新功能状态位，组合门不等面积。短K可在old child到E后补读；source物理拍数E+(256−FA)，不能套固定511。

初步配对run05/06：F1/PTE0/RR1，接力V/A5907/5905 vs5152/7472，平均5906 vs6312（−6.43%），V+14.65%，A−20.97%；source511两者同。强共同切分run12 6154/6153，源256。F128/PTE0 run03/04接力6016/6014 vs5404/6573，平均略更差；F1/PTE1000 run07/08接力6803/6801 vs5152/7472，更差。不能无条件接力或声称全优。

实际late WRITE不及V尾段：F128/PTE1000 run02 requireactual失败，原完整数据检查先过但非接受PASS；run09允许回退正确完成、与关闭run01 CSV逐字同，且run01与§56run09 CSV逐字同。保留require失败，不能把opt-in当actual。

继续判别普通variable K+相同接力协议能否形成更强点；如果可以，区别新规则集成与旧published具体覆盖，不用任意改写普通控制器作为前作证据。所有原始run记录仍保留。

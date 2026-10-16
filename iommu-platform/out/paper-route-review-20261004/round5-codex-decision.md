# Round 5 — Codex收敛决定（编译前规格）
本轮不加新研究路线。主A是论文可行性门禁：同一物理BIT控制容量保护P与命中先服务N，检验持续热V的结论能否推广到间歇V。P/N本身已知，组合不自动是创新。后备B只做接口证据，不在本批实现。负结果原样保留，若只得到缓存常识或无法超出原结构开销，不包装新方法论文。

## 已解决/界定Round4
- P固定总16条、V(DID10)2条，其余14条；不增加8/8敏感性，防弱基线。保护DID用24位，允许10或11作为接口范围，本批实验固定10。不是DID安全tag：不改PSCID/GSCID/global/stage匹配与权限。共享映射不承诺独占。
- 同BITprofile nonblock_mode=3，四模式0=S,1=N,2=P,3=PN；同512拍DMA0/1缓冲。旧mode1/2/role接口不变。原版只作总体开销锚，绝不假装S就是原版。
- 配置bank研究相对索引D20..D5F，新magic/version。D22单次原子配置写：bits1:0模式，bits31:8保护DID（只10/11），其余0。硬件全静止才接受并同拍丢弃IOTLB有效条目/PLRU；64位generation饱和后拒绝配置，不能回绕。busy/非法写无模式/缓存副作用，记录last_status。软件mutex+硬件quiet；同周期DMA/翻译新请求阻止接受。TL quiet包括正常/快路径及待交付响应、walker/fault/ATS/debug；bank无在途，DMA无batch/issue/通道在途。
- 清除只是微架构驱逐，不代替IOTINVAL/IOFENCE。不得用reset快路径绕过排空。配置每次包括相同模式也清理；generation增加说明实际接受，不是异步未完成命令。
- 新ioctl0x11独立结构，旧P4M reserved仍全0。包含原P4M、新mode/protectedDID/victim_gap_cycles与独立raw64words。配置在映射准备前/预热前；采样仍P4M(open,close]，随后排空、禁用batch、恢复S只在quiet成功。失败保留原始结果，不强制清cache掩盖挂起。
- 正常首次lookup/hit/miss分别按10/11/other计数，来源research.lookup+hit+稳定主请求DID。fast accepted单列；两者不能冒充同一命中率。旧HPM只原始保存、不作新结构有效指标。
- 新bank保存首/末IOTLB16条各128位、fill/replacement来源与保护份额违规事件，用于分辨替换路径。若64words不够扩大新struct，旧ABI不动；具体以实施冻结表为准。
- V gap复用已存在每设备硬件等待，新增只读gap-active引出计数，记录真实窗口gap周期；V总窗口含有意等待，不扣名义gap声称服务延迟。固定源1页+目的1页，4KiB完整复制；不扩展长突发资格。
- 角色固定V0/A1，本批不额外做互换/反向。明确外部有效性限制。

## 分阶段计划（实现后再冻结准确数目）
1. 4模式×Solo/A1/A8/A16短验收16项，数据/身份/新ABI/模式generation/窗口/零错误检查；模拟必须覆盖busy拒绝、请求同周期、cacheflush、全mask旧PLRU等价、P分配边界、lookup权限/失效不变。
2. S/S 6对12项（A8/Vgap0），仅描述同设置散度，不用预定2/5/10%“显著”阈值。
3. 4模式×Solo/A1/A8/A16×Vgap0/1000/4000 ×3交错block=144项探索试验；A gap0，4096×4KiB，warmup256。同boot内配对；需要新启动复现才能外推稳定性。模式按预先固定轮换顺序，所有失败/偏离保留，技术错误停不自动重跑。
4. 未看到足够机制可分性/稳定收益就不做数百次正式试验。机制门禁：P减少V首次miss/跨份额替换；N产生实际fast；PN是否有超单项的增量及A代价。PN不超单项就删除“组合必要”。即使超单项也不自动新颖。
交付实际BIT+Linux payload+CLI/冻结runner+SD说明；板测结果未产生前不写“已证明收益”。


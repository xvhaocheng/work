# 同BIT容量保护×命中先服务：实施规格 v1（2026-10-04）

**2026-10-05 裁剪修订**：仅composition profile（IOMMUNonblockMode=3）不再实例化旧research事件记录器与旧非阻塞诊断器，其旧bank读0、不提供旧capability；下文D00–D1F“不变”为修订前描述。新D20–D9F bank、128×u64接口、开/关快照和P/N方法逻辑均保留，Linux新接口不依赖被移除bank。[修复与验证范围](composition-trim-20261005.md)。
状态：五轮收敛已完成；此文件冻结实现接口，未称上板通过。主profile migration-4dma-composition，nonblock_mode=3，16 IOTLB条，50MHz，4DMA；DMA0/1同512拍缓冲。逻辑mode0=S/1=N/2=P/3=PN；P固定low2保护DID（本批10）、其余14。默认reset S, DID10。不是新安全隔离，不改缓存tag/命中/权限/规范失效。主PTW/MSI稳定req_data.did决定fill份额。
缓存P选择为mask约束tree-PLRU，逐层仅在子树存在eligible叶时遵从PLRU，否则走另一子树；全mask逐拍等价旧PLRU。P关闭总16条。PolicyEnable参数默认0，旧profile功能不动。
## 控制与观测（研究相对索引D20–D9F，实际索引=512+relative）
新128×u64 bank；index=relative-D20。D00-D1F既有非阻塞观测不变，新profile其Mode=3让旧工具拒绝。只有offset2可写。
|offset|内容|
|---|---|
|0|magic 0x49444d4143504e01|
|1|ABI 1|
|2|live config：bits1:0 mode，bits31:8 protected DID（10或11）；其它0。写此字触发原子配置/清IOTLB|
|3|live generation，64位从0递增，最大值时拒绝再写|
|4|last config status：0成功或复位，1busy，2非法数据/不支持16条，3generation耗尽|
|5|live quiet：完整硬件quiet，且本观测不是armed/active，无同时arm/open/close|
|6|window flags：bit0armed bit1active bit2frozen|
|7|window sequence，ARM递增，饱和置error|
|8|window cycles，只计(open,close]|
|9|window error bits：0bad_event，1counter_overflow，2fast_when_N_disabled，3bad_geometry_or_stage，4partition_violation|
|10|ARM锁存window config|
|11|ARM锁存window generation|
|12/13/14|V(DID10)正常首次lookup/hit/miss|
|15/16/17|A(DID11)正常首次lookup/hit/miss|
|18/19/20|other正常首次lookup/hit/miss|
|21/22/23|V/A/other真实fast accepts（独立于normal首次lookup）|
|24/25|V/A GAP状态实际占用周期|
|26/27/28|V/A/other填充事件|
|29|被替换有效条目的PSCID不等于新条目PSCID的事件数；不是DID淘汰计数|
|30|P分配位置违反mask的事件数|
|31|规范失效发生周期数|
|32|首次lookup中不是normal或非stage1-only的事件数|
|33|N关闭时fast事件数|
|34|protected mask=3|
|35|reserved0|
|36/37|OPEN前/ CLOSE后PLRU（close同周期touch/update结果不要求PLRU-next，37明确是close边沿前PLRU）|
|38/39|首次V/A正常lookup的PSCID（未见时0）；不是DID安全tag|
|40–63|reserved0|
|64–95|OPEN前16条，每条low64再high64，actual IOTLB entries|
|96–127|CLOSE后16条，entries_next，包含close同周期update/inval|
只有窗口active计数，open边沿不计，close边沿计；ARM清旧window但不改cache，OPEN采前状态，CLOSE采next。config不抹掉冻结窗口；新ARM重置。fast只在accept_i && fast_i时计，违规fast_i无accept记bad_event。填充正常/权限不改变；update+invalidate按research真实effective update。
配置必须全quiet，拒绝无mode/cache/generation副作用。模块clear_o为接受写同周期组合脉冲，同posedge更新config/gen并清cache，不需要异步两阶段等待；后续CPU读generation是完成确认。busy等只改last_status。全部同clock，TL quiet包含stateIDLE、无req_valid/输入req/ATS/debug、无fastbusy/响应/故障、无walker更新/有效端口/规范inval；top还须无bank在途及TR/COMP/DS通道valid，DMA quiet覆盖所有lane且排除同周期issue/launch活动。软件mutex串行，失败不能强制flush在途。
## 新Linux ABI（独立ioctl0x11）
struct idma_composition（新增不改旧结构）：u32 abi_version,struct_size,mode,protected_did,victim_gap_cycles,reserved[3]; struct idma_p4m p4m; s32 status,restore_status; u32 configured,raw_captured; u64 generation,restored_generation; u64 raw[128]。
inputs到reserved及p4m既有inputs；output全部kernel清零。仅CAP_SYS_RAWIO、V0/DID10、Vpages1、A=none或1/DID11、Apages1/8/16、chunk4096，gapV=0/1000/4000，Agap0；单ioctl一次完整试验，旧ABI不改变。
在全局experiment mutex内，软件/硬件preflight，原子写config并检查magic/v/live config/generation/status；在stream_prepare和warmup前配置。会话复用现有prepare/cacheworkaround/warmup/drain/verify/P4M计时。只新接口传Vgap替代常量0。结束后先捕获raw窗口，排空/禁用batch后再quiet恢复S（保护DID10），记录restore_status/新gen；失败排空仍失败则不强制恢复。所有错误原样返回并保留能读取的结果。
CLI idma_composition输出单行JSON包括输入、P4M、raw128、状态、新硬件ABI和kernel/boot由runner记录。旧HPM存为raw，禁止当新结构有效指标。
## 实验与接受
16short=4mode×Solo/A1/A8/A16（transfers16,warmup16, Vgap0）；S/S6对12（A8,Vgap0,long4096,warmup256）；144探索=3block×3Vgap×4workload×4mode。共172，分阶段执行；技术错误停止、无重跑/剔除。N计数为0属于科学/触发观察，不能自动技术失败；不因P/PN吞吐无收益而停止后续阴性采样。探索不等于确认。首批单boot，不冒充跨boot稳定性。
固定顺序用预先生成manifest，block轮换模式顺序，参数先于样本。主表同物理BIT同boot四mode，原版仅全结构开销锚。V窗口包括故意等待，列实际gap周期不把扣名义值当服务延迟。保留开始/结束快照、模式gen、boot、版本、失败UART。后续决定能否进入论文方法线：单P/N已知，组合不自动创新；PN不超单项删组合必要，只有缓存常识则不包装方法论文。

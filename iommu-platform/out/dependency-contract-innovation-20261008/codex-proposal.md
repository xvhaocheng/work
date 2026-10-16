# Codex 独立设计草案（2026-10-08，§28；先于本轮 worker 返回）

主线仍多DMA共享IOMMU性能/成本。§27部分后缀预约D尚未实现、未认证创新。本轮不默认做D PoC，因为“准确普通cut-through+逐段credit”可能就是D；先把信息/服务合同缺口作为研究对象。

## 已直接核的接口边界

tests/nonblock_actual_dma_top.sv:638–646 直接从两个iDMA内部be_valid/be_ready/idma_rsp/ttype/burst_req取job事件、src/dst/length；late_pool.sv:22–24把这些当输入，:101–115计算逻辑偏移。它们不是普通AXI-IOMMU消息的固有字段。
AXI扩展typedef.svh:37–81包含地址/ID/length/attr/DID/PID/MMU属性及通用USER，未定义copy级依赖语义；不能由DID=同DMA推出这个R供给当前W。
iDMA legalizer的r_num_bytes/w_num_bytes、offset/tailer只在内部读/写datapath配置；AXI len表示总返回拍，包括尾部未用bytes。只看len不能保证知道copy有效字节边界。
生产TC COMPLETE在物理AR/AW地址请求ready后就回IDLE（trans_ctl_wrap.sv:716–726），不等R响应RLAST；这提供让后续翻译继续进入的局部条件，但没有证明外部返回、PTE服务独立。
注意：上述是接口事实，不是新信息不可能定理；同一设备可通过静态语义、其它观察或者新增接口提供这些信息。

## 候选E：用少量已承诺的copy语义换保守返回容量

不以“metadata sideband第一次”为创新。MetaSys/XMem、DMA descriptor、CTC已有强对照。

边界问题：在相同AR/AW可见前缀、相同DID/长度下，两合法端点的内部R→W数据依赖可以不同：一者R0供当前W，另一者当前W依赖后面的R1、R0不能立即消费。只知道当前AW已译的通用桥不能保证正的可排空前缀p。此为条件化不可辨识构造，未给真实RTL见证/普遍下界。

提出接口草案：
- 受信任DMA导出copy_seq、src/dst有效逻辑byte区间及普通copy类型，AR/AW关联copy_seq和byte_offset，未消费不能重用seq。
- IOMMU仍独立核地址/权限；语义不得充当绕过权限的授权。
- 控制器只为已准备目的译址、确占写序、无更早依赖的逻辑前缀发出“可独立消费”的容量承诺；瞬时WREADY不能当保证。
- 在物理AR首次VALID前，exact mapping与承诺共同给出p，后缀L-p独占预约。已承诺p不可在没增加预约时撤回/缩短。
- sideband/缺字段/不支持类型在准入前退到p=0整预约；不能承诺发出之后再无资源地fallback。
- 静态映射首判；epoch/fence/故障是必要扩展，不能称完成。

可能贡献是“接口信息与安全容量的条件边界+较小可部署接口”，不是credit算法本身；准确普通C得到同信息就应同效，贡献比较必须计接口/状态代价。信息导出若只是job窥探搬到wire则工程化，不足机制创新。

## 候选F：先准备首读/首写翻译，再承诺共享W顺序

对应当前CONF0、对齐copy，而非依赖特殊错位负载。利用上一轮V FAST已回但等待A Wowner的真实现象。

- 提供不提交AXI AW的advisory src/dst翻译准备，分别按READ/WRITE权限核查；结果只存少量首fragment handle，不锁整个job页集合。
- 尚无真实AW VALID时，按实际“首R/首W都已准备+可给到服务”的copy头选择W承诺者；保持同device/id必要顺序，不撤回持有VALID。
- 选择后固定handle/seq，把正常DMA请求交给既有AXI路径，并及时退还handle；多个copy首次选择不能使用失效的hit判断。
- 只声称避免等待首片段翻译的W占有，不能保证W数据瞬时到齐/所有NoC环路无死锁。
- 与普通joint pretranslation+ready queue（本身可能相同）、ATS/LTI、socdma2018全传输TLB锁定/重试、AXI-REALM完整写数据后AW同合同比较。

F可能是已有ready队列的应用，须查LTI/MMU-aware DMA后再定；不因为其更能改善旧PN就认创新。

## 这轮审定条件

1. E若最强普通C同信息同容量同逻辑成本，D/E只作为工程对照，研究问题可保留而不认证机制。
2. F若已有LTI或直接先行给出同样前准备、句柄寿命及共享W承诺规则，同样降级。
3. 寻找真实可达且最强普通对照需要更多数据/状态容量或少吞吐的差异；只出现普通准入错误不算新规则。
4. 阅读原论文与源接口，先不生产RTL/Linux/BIT或扩矩阵。


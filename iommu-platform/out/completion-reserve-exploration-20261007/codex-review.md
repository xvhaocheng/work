# Codex 审定：E2 按可排空的读段选择预约，创新仍待证

2026-10-07；本轮只有源码/既有轨迹/原文核查与架构推演。没有新仿真、综合、BIT、Linux/生产源码修改或板卡操作。最新实际测试仍是 owner-baseline 的 8/8；本文的 E/E2 未实现。

## 结论
整 job 共享缓冲 E1 不能凭借共享/预约作为创新主方法。保留一个**成本预研候选 E2**：只有明确能给当前写通路提供数据的源读才允许小 FIFO 流式返回；其余提前读在共享仲裁选择前预约足够接收全部 burst 的暂存空间。预约与数据消费对应，不能以“PTW结束”“同bank已发AW”代替。尚未证明它超出最强普通依赖门控+共享pool，也没有面积或收益结果。
目标是共享 IOMMU 的翻译并发与成本，性能主线保持。论文可有增量微架构/实测贡献，不要求所有原语首创；但仅传统pool移植或PN略胜P仍不足。

## 已确认边界与原始证据
1. 普通 walk 的Q收费仅覆盖PTW active、FAST、另一bank、非AW豁免的新offer。源码 tests/nonblock_read_admission.sv:114–154,248–291；旧debt不随walk边界清。它不是PTE等待周期或全流量上界，已有 outstanding 模式亦必须作强对照。不能将符合额度的等待称“违反保证”。
2. cold-walk-pn事件：root DDR AR1263/R1295；另一bank源AR1268，256beat数据R1327→1582；下一层PTE AR1408/R1653。下一PTE等245cycles，root32cycles；CPU读也在同共享DDR，因此不把额外213全部归因于这一条DMA读。window CSV由 nonblock_actual_dma_test.cpp:403–406 限定win_open，DDR ARLEN在888、Rlatency在891–895；不是warm轨迹。
3. 生产4 DMA：ariane_peripherals_xilinx.sv:870–872,1255–1286,1409,1466。DID10/12→bank0，11/13→bank1；id由 axi_id_prepend.sv:89–90 的{port_index,original_id}得到，bank取追加端口号LSB。全缓冲只前2 DMA：512beat=4096B，其余3beat；证书只DID10/11。上层 ariane_xilinx.sv:818–824明确InclDMA=1；当前N/BIT参数需按既有构建锁核，不能把默认N=0当当前BIT实参。
4. TC每bank一个AR FIFO，在准入选择后才消费：rv_iommu_trans_ctl_wrap.sv:146–164,646,716–727；bank owner是粗粒度：rv_iommu_banked_ctl.sv:256–278。两个DMA测试不能证明4DMA全状态活性。此轮未复现4DMA bug。
5. 同一copy可以AW和AR并行，不能说需要两个job重叠。实际配置 RAW_COUPLING_AVAIL=0（peripherals:1272），idma_backend.sv:837–857把AW独立经fall-through送往transport；transport直接连接AR199–201/AW288–291。证书串行job不排除同job读写流水。
6. 共享R sink/延迟交还、准确写通路头、byte reservation均不在现接口中。axi_demux的per-ID outstanding counter不是可用字节空间证明，不能直接拿来当接收credit。

## E1反例的正确范围与DeepSeek审定
Codex原稿的DID10 W头、DID12同bank AR在前、共享slot被DID11占用，是**新增job级门在FIFO头拒绝时的反例**：12等slot，10等12从队头离开，11等10排空以归还slot。它不声称现未改RTL会按job门拒绝12，原bank-owner逻辑确实会允许同bank12；原RTL的另一种R反压风险未实测。
DRAINABLE须同时检查真实W路由的头与对应目的物理AW的翻译/接收准备。仅AW成功不够，DID12的AW成功也不让它越过DID10占用的W FIFO。
保留反方关于额外状态/数据通路缺口和普通pool重叠的质疑，纠正以下过度推论：
- 反方表格“bank=i[1]”为笔误，实际是端口i[0]。
- “在飞AW与待发AR需要两个job”不成立，见以上coupler bypass。
- 反方将“共享仲裁**选择前**预约”按“共享输出ARVALID后反压”审查，范围错位。后者确实全局HOL；前者可在per-input仲裁eligibility中做，不撤回已选VALID，也不必改外部DMA的AXI协议。但需要新控制接口/中心状态，不是免费已有功能。
- 现证书为私有容量保证，不是已经实现动态shared pool；per-ID in-flight计数不能等同字节credit。
- 硬件只有串行job时DMA身份可先足够，epoch不能当新意/必要开销；abort/错误及旧R跨job重用才需要额外生命周期证明。
- 不能推导“同容量共享吞吐一定不优于私有”，普通pool避免静态容量搁置与输入调度可能不同，需实验；E1没有已证优势也成立。

完整未经修订反方保留 critic-answer.md；以上是Codex直接源码审定，不能称模型一致。

## E2：把必要接收容量与可排空依赖对应
先限定2 DMA、每DMA一个normal aligned4KiB copy、相同64bit数据宽度与src/dst burst划分，无跨页/atomic/异常取消。初稿E1给BLOCKED预约整job4KiB，E2研究预约“已经承诺且目前还不能交还的完整读burst”，未承诺的下一段停在共享选择前。

1. **可流式排空的读段**：源字节对应的目的write burst已获真实输入/输出W路由顺序中的可排空位置，且对应物理AW翻译成功/已准备；无其他外部循环依赖。仅“同DMA/同bank有AW”不够；同job第二源burst可能对应另一个未到头的write burst。
2. 这些读段可用现小FIFO流式接收，前提是W/内存公平服务能持续产生消费；允许有限R反压，不推出PTE延迟上界。
3. 其他提前读：在DMA侧共享AR仲裁作出不可撤回选择之前，保留 bytes=(ARLEN+1)*2^ARSIZE；若无空间，不能占用会阻塞排空者的公共队头。已经承诺的offer计入空间，不能等到AR handshake后才扣。
4. 共享暂存区吸收返回。不能把仍等W路径的返回提前塞到公共completion R，再反压旧owner。需要per-DMA重放端点或等效的已验证返回调度，以及ID/长度/last/顺序记账。
5. 容量按实际交还消费释放；PTW/物理AR接收不释放。副本存在、在途R与预约计数分清，防止重复扣/放。
6. 旧whole-walk/在途护栏用于内存服务公平，空间预约不替代它；保持主walker/旧W头的独立进度。4DMA扩展前须解决同bank队头和多DID别名。

**一个具体非等价判别状态**：共享接收区已被A提前读占满，V是可排空写burst的真头且对应源读尚未发出。无差别“每个AR都必须有整响应空间”的普通规则会拒绝V；E2允许V通过小FIFO给当前W头流式供数，从而使A最终获得写路径并归还暂存区。这个状态说明E2不是纯空间计数；**普通共享pool若也加入准确owner/可排空门，即可能同式，必须作为最强对照**。当前bank读guard已有相关思路，不能把这段流式owner准入单独包装成全新原理。

**更小暂存假设**：既有模型ARLEN=255、64bit，单读burst2048B。若每个不能排空的AR都预先覆盖全部返回、其余AR保持在选择前、旧W头能独立流式进度，一个共享2048B区可能替代当前两份4096B私有区。理论payload为2048+2×3×8=2096B，相对8192B减少74.41%。此为**假设配置点**，不是已证最小容量下界、无死锁定理、LUT/BRAM节省、吞吐结果或可直接改现RTL的许可。
并行borrower增多或write-read分段不一致可能需要更多slot/元数据。整copy或每DMA O(1)任意负载性能均未证明。若每burst只有3beat受约束且需要碎分read，必须另比AXI-REALM普通burst splitter；本轮不引入fragmentation。

## 强前作与差异检验
| 前作/对照 | 已有内容 | E2必须证实的增量或限制 |
|---|---|---|
| [CTC NOCS2009 §3–4](https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf) | 接收credit、共享单queue/计数器、传输前容量握手，释放跟消费 | 共享/预约非新；固定AXI完整burst承诺、可排空copy依赖下的更小容量/开销是否有实测价值 |
| [Arm DMA350 TRM §4.3.4](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2) | 发读前足够完整response空间；写有完整burst数据才发 | 允许经过精确依赖证明的owner流式读，而对阻塞旁路才预约；必须比较允许streaming的强普通版本 |
| [AXI-REALM2025 §II-B/§V](https://arxiv.org/pdf/2501.10161) | 拆burst、write-data-ready、周期预算与可实现低成本 | 控制单DDR共享IOMMU的返回依赖，不因位置不同就算创新；面积/频率/吞吐要实测 |
| [Preemptive VCs 2026 §III](https://arxiv.org/html/2607.01430v1) | DMA读写端点循环，分流/credit/可抢占逻辑VC，成本性能比较 | 问题不是新发现；外部AXI RVALID不能直接抢占，保持现单接口的真实实现代价才是比较范围 |
| [AMD IOMMU VC patent US7480784B2](https://patents.google.com/patent/US7480784B2/en) | IOMMU隐式访问与P2P堵塞用专用VC/系统路由约束 | 不能宣称首次为IOMMU提供进度通路；不采用专用VC的局部边界需明确 |
| [MASK2018 §5.4](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf) | 页表优先、Golden/Silver/Normal队列与应用配额 | 空间安全不等于页表优先；whole-walk强性能基线保留 |

以上是关键段定向再核；其中2026 VC等已有地图/§12曾核，不说全部“本轮首次新增”，不说领域前沿已全部排除。

## 下一最小工作与去留
1. 先把“匹配当前write burst”的信息能否由现AR/AW/W、job descriptor得到写成接口/状态表。别用job DID单独推断字节依赖；若需大量传入元数据，使成本超过私有buffer，就停止最小版本。
2. 验证2DMA单job的边界状态：共享区满时真排空者仍能开始、同job第二burst不得错豁免、已选VALID/读写顺序和源数据正确。**此轮未运行这些测试**。
3. 若接口判断通过，再做一个共享2KiB+小FIFO PoC，仅共比：现8KiB私有缓冲、普通shared pool全响应预约、普通shared pool+准确owner门、保守owner、whole-walk/在途强基线。共同软件预热、V/A字节量与窗口，报告吞吐/延迟和小模块综合成本，不能只有PN>P。
4. 若与准确owner普通pool同式/同成本或吞吐明显下降而无有价值资源优势，停止E2主方法。若形成可推广的依赖条件/实现边界并有可信成本性能优势，可考虑增量微架构论文；不是以primitive是否首次发明作为唯一门槛。
5. 不自动做BIT或4DMA全矩阵。下一主线是**低成本维持非阻塞翻译的具体实现约束与收益**，E2只是先试候选；原P/N/PN组合和已修历史不推倒。

## 分工和审计
DeepSeek-flash两次 high/max393216，均stop、未截断，397336+112154=509490 provider tokens，完成源/事件证据整理与独立反方，未执行或改文件。Codex独立E1/E2与反例设计、关键RTL/原文直接复核、纠错及最终判断。无Claude/新Codex子agent。


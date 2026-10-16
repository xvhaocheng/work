# 待完成输入与费用接口

尚未最终裁决；以下为新增必要费用类别，数字总量未知。

| 观察 | 正确所有者与有效期 | 必须收费/回退 |
|---|---|---|
| actual decouple_rw/deburst | 已accepted job的legalizer opt，而非可被重写的frontend live config | native已有各1bit；新增读取/packet-job绑定与原子快照收费，不能以FullBuffer代替 |
| AW bypass full/valid及AW0 raw handshake | 原job第一个存活AW、1slot native lifetime | RO事件/端点和关联收费；不得计成已accepted AW直到握手 |
| un-emitted AR1 | accepted4096B job剩余意图、legalizer r_tf有效/地址/length | native字段可以读取但接口和关联收费；不是可选合法共享head，不能提前合成/发送 |
| A TC current read/spill ready | 同jobP的慢response与COMPLETE real AR spill接受 | 稳定packet、正常资格、native FIFO/credit收费；不能借当前已前进metadata替代 |
| P stage/fullwalk history | 当前req_data/PTW state，历史只能来自已完成正常walk | 未知leaf/epoch/class/overflow回退；计时记录/端口/更新/算术/延迟收费 |
| raw W owner/output W/B | 每job每原端点独立 | 原3组10bit/bank可构造不代表充分；B/AW、未发请求、时间与历史另计 |
| original subsequent supply | 原frontend/batch/job接受/退休关系 | 不因一方提前完成就停止其原后继，不能免费预知未接受请求 |

父轮4bit顺序证书仍限normal aligned non-deburst；20bit是一般burst替代。68bit仅组件小计，不能写成新总成本。ordinary可删未用选项/history/包络。完整新增bit/port/update/select cycles、FF/LUT/BRAM/area/Fmax/CPU/application/netgain全为null。新增公开API/RTL/ABI为0。

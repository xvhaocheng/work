# §60：用已消费字节限制 W 承诺，取消共同 W 网格

Codex亲自提出及推演关键RTL。尚未构建/执行。本项同时是对接力方案的强简单挑战。

§59真实F255失败证明：R255强求A AW2 coverage，而AW2需A旧W/B；A旧W被V已firstVALID qW2[254,256)挡，V缺R255。phase3已有完整正常WR授权，允许最后R在原两拍借槽中交入、保留原W/B，可闭环。R/W不同原父与不同页尾不要求相同logicalLAST。

新准入：共同source R仍按两方成功WRITE前沿裁物理AR，每方实际WRITE child仍独立原页内合法。每个新qW child start必须 <= peer实际raw_written；长度<=2；停用按child数比较groups和共同F网格裁剪。源R在无对应W承诺时可用现有最多2拍的R−W存量，终止R不再强求未来AW存在。每个目标的当前页仍必须自身正常WRITE recipe成功；未成功前沿的source R禁止，不能用WRITE1替WRITE2。

若未启用自主下一页预取，仍保留共同成功WRITE前沿的child裁剪：避免V承诺一个跨F的W又等待尚须A首B才能创建的WRITE2许可。预取启用时WRITE2来自已捕获首AW、自主真实TL请求，无首B依赖；可以保持各自native qW网格。旧physical source AR在F内的最后拍最多进入两拍private存量后真RLAST退休，才允许未来正常TL响应进展；不是抢占held R。此正常最终服务前提不提供失效/fault恢复。

受限进度推导：若共同R在r因某方i存量r−w_i=2被挡，任何已经firstVALID/入全局W头的child j在创建时start_j<=当时w_i<=现在w_i、len_j<=2，所以end_j<=w_i+2=r。共同prefix r已真实送入各DMA，故该W头不需要未来R，可按公平正常服务排空。页父B→next AW间隔可能使某方没有新child，但其旧W同样拥有全部数据；接收logical RLAST不会替代原W/B。证明不依赖共同页尾/q网格，不是cycle期限；只涵盖固定两flow共享读取、无退组、q2、正常响应。

接口：新root input shared_write_progress_i（默认0），新观察输出 dbg_shared_write_progress_active（sticky：实际新child在此准入下firstVALID形成）；source owner input write_progress_i。driver +write-progress=0|1，只准natural/shared2048、physical-bypass2、keepq1、无handover/pause；与write-prefetch可0或1。完整源/目标/guard/实际R/W/DONE原检查不变，新终端观察不能从CLI推断。opt0必须与§58同条件CSV完全一致。

比较同sourceR256/成功前沿/WR预取槽/B3/q2，目标是排除接力收益只是共同F网格导致。若它同成本追平，就不能用旧接力优势立新算法；若字节承诺规则在错位页尾上有进度/成本优势，仍需核最强credit/data-first/集体互连前作后判贡献。算法新增组合比较/现有反馈连线，暂不报综合面积。只out，不改生产/BIT/板卡。

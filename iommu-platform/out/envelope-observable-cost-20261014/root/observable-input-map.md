# 输入、所有权与寿命映射

源码入口均为本轮冻结的[source-snapshot.json](source-snapshot.json)，逐行原文[source-excerpts.md](source-excerpts.md)。下表区分当前事实、可付费构造的观察和合法历史预测；没有端点实际记录时保持unknown。

| 输入 | 真实来源/寿命 | 可复用和必须计费 | 边界 |
|---|---|---|---|
| L packet/full ID/bank | TC目标demux SpillAr=1存header/select；banked_ctl输出mux SpillAr=0；trans_ctl_wrap:625–729 | 用仍存活packet，原mux/路由ID不截短；关联读取/组合逻辑计费 | TC当前req metadata可能已前进，不能代替L |
| 当前job身份 | frontend:110–179/321–375；batch_issued/completed各33bit，batch_id64bit；sole job accepted至实际B侧done | sole current job时可读native ordinal/WAIT状态，未必另加job tag FF；端口/生命周期检查计费 | obs_issue_id=issued+1只在issue事件有效，不能在WAIT当当前job |
| P owner/当前walk | tl_wrap:165–214/280–328/454–468，慢req_data至真实响应握手；PTW:384–421/480–515 | 稳定owner/fullsource/阶段RO读取；gate可用pending事件记录 | 其他bank native N hit不覆盖owner；未来leaf/error/nextPTE未知 |
| read admission/fullbuffer | fullbuffer_certificate:1–47；periph:1393–1415，pending/eligible/fault原3bit，512beat原容量 | 正常4KiB aligned DEFAULT sole accepted job资格原生复用 | 不是未来AW许可，也不代表R数据已到或全任务无错 |
| 剩余R | 同job原physical R接受/原DMA实际R数据端点与512总量 | 从0..512进度可用10bit直接计数；选定端点和ID/job关联 | legalizer r_tf.length是未发请求，不能当DDR剩余；跨端点不能混减 |
| 已到数据/剩余W | DEFAULT实际R到byte buffer；raw W与physical输出W分开 | 每个端点0..512各10bit可构造；原buffer/FIFO容量不复制payload | w_num_beats是当前burst8bit，不是整job进度；nativecredit不是剩余字节 |
| 已预留W次序 | output axi_mux新AW firstVALID→w_fifo_push，actual WLAST→pop，原8个bank-index槽 | 第一次reserve事件/头/valid；4bit窄域certificate或20bit一般计数二选一 | reservation不是accepted AW；AW可held，FIFO无LEN/job tag |
| 输入W阻塞owner | input demux w_select/w_open/lock，真实raw WLAST归还 | 原owner可RO复用，但必须绑定当前packet/job及实际W端点 | 外层signed w_unfilled不是唯一owner，也不是actual payload owed |
| future peer AW资格 | raw W owner释放、真实peer AW header、write NormalTL/hit/credit/physicalAW准备 | 只能将已拥有义务作事实；未接受AW时刻是显式条件预测，所有记录/接口计费 | 不能隐藏在跨策略相同H或预读未接受AW/PTE；这是本轮literal缺项 |
| actual AW accept / B owed | AWVALID&&READY与真实BVALID&&READY；backend B/last-burst metadata，transport:390–454 | 必须分别关联每burst/job；原metadata可合法读取，另需剩余/计时状态参数化 | WLAST释放W owner不退休job；AW reservation不能直接加到accepted B owed |
| 正常完成/错误资格 | backend NO_ERROR_HANDLING使idma_rsp.error=0；真实RRESP/BRESP/IOMMU fault另读 | done用于原退休，normal history还要端点RESP/错误资格；事件读取与粘滞记录计费 | done不证明RESP正常、数据校验/字节守恒成功 |
| a,g,b,h,H,U / B预测 | 已完成且同端点、同clock、同身份/服务类、正常的R/PTE/walk/W/B记录 | 时间戳宽T、历史类K、未完成记录M、溢出epoch/有效位参数化；无效回退 | 当前历史数量/真实数值未取得；不能用固定50/52替代DDR或完整walk |
| max/sum/age/RR | 对两个完整job的预测完成，不只R-last或PTE响应 | 算术/快照/持有选择/年龄/公平记录及选择延迟计费 | 跨jobB尾不同；CA不能先验当max；overflow不能wrap比较 |

原source0/1 DMA的功能AXI ID与xbar/PTW source bits保持完整，physical gate局部ID与xbar端点ID不同；不凭数值相同建立job身份。last-burst metadata必须真实匹配原B，错误路径照原生走。

所有必要native对象在CPU/AXI观察域可用时不要求新增MIG CDC；从MIG另读内部状态/事件就必须计CDC、对齐与握手，不能将隐藏DDR状态变免费输入。历史预测可以不准确，但必须明示所预测的AW/W/B项及其成本；“必须当前事实”不用于否定所有合法预测器。

本轮实际prefix、normal历史、a/g/b/h/H/U/Z数值、实际bytes/PTE/R/W/B/complete/quiet与收益全部null；没有从旧trace取样，也没有把源宽度当测量结果。

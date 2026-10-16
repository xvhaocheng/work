# §60–61 根判断：简单数据齐备准入追平，继续查容量/量子边界

§60两次SV/C++构建＋一次C++增量、10份完整双DMA copy PASS；§61一次SV/C++构建＋一次C++增量、10份完整copy PASS。各阶段10CSV独立核source AR拍数及两目标实际W都256，原完整bytes/guard/logicalLAST/DONE检查过。§60 opt0及§61三对opt0/1 CSV SHA与此前对应逐字同；两份execution-ledger和accepted快照完整记录。新实际启用由RTL真正qchild形成的sticky记录证实，enum2直接驱动2bit，未被bool ternary压成1。

| 条件（同B3/q2/成功权限/WR预取） | 旧按groups共同推进 | peer字节进度 | 普通自身payload齐备 |
|---|---:|---:|---:|
| F1/PTE0 | 6098/6097 | 5596/5604 | 5596/5596 |
| F128/PTE0 | 5662/5653 | 5164/5163 | 5178/5177 |
| F255/PTE0 | 5676/5675 | 5176/5191 | 5190/5188 |
| F1/PTE1000 | 7098/7097 | 6596/6604 | 6596/6596 |
| F1/PTE1000/B64/背压/A先 | 15207/15206 | 14701/14782 | 14701/14711 |
| F128/PTE0/B64/背压/A先 | 13770/13761 | 13254/13266 | 13262/13274 |

均source R256，未添加payload；只是停用共同groups数的B后barrier，改firstVALID形成前的准入。peer方案用已有对方actualrawW保证W头数据闭包；普通方案只等当前child所有自身actualR到齐，不依赖peer。后者有data-first/store-and-forward前作。F128两新方案均值只差14cycles（0.27%），F1普通更好。因此约8.7%的旧barrier收益不能据此命名peer独立新算法，也不归接力机制。暂未综合完整total成本，两个输入比较不等同等面积。

根已直接读条件链：source完整共同R只到各已成功WRITE前沿；普通方案下一child actualR必须覆盖其整个end才创建firstVALID；之后AW/W/B继续不可撤。logical terminalR可以借最多2拍private容量而不等下一AW，jobDONE仍由最后WRITE B。普通首页F1可以先形成resident A qW1，而V缺第二拍的qW2不占W头；A首B结束后正常产生下一页WRITE许可，R余段继续。每个currentWRITE child仍用自己的真实key/成功recipe。

如果改变q及对应private容量后peer方案有稳定进度/成本优势，并且最强credit/order-aware前作没有同规则，就可继续论证贡献；如果强普通仍同效，则停止这个机制主张。下一限定mode2/nohandover，把q从2拓至8/32并将真实FIFO配到q+1，同一预算下比较三策略；不是靠加容量单独给候选。8/32未运行。

Codex设计关键规则、直接核源、执行和接受；DeepSeek普通C++开关/观察器提案。正常静态同reset单job/immutable源合同，无生产/Linux/BIT/FPGA/SD动作；没有fault/invalidation、多job、真实DDR或面积结论，无Claude。

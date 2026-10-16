# 可观察输入、寿命与完整费用

## 所有权与端点

| 输入 | 原字段/事件及寿命 | 费用/限制 |
|---|---|---|
| current job与实际选项 | accepted frontend/backend job、sole WAIT、legalizer opt/remaining source/dst/length；新job须原B退休后合法接受 | 可在sole寿命读native ordinal/opt，不读取可被改写的live config冒充accepted字段；读取/匹配/快照收费 |
| AWd/ARemit | private AW fall-through source握手；legalizer pair/independent进度；已生成AR meta/native FIFO | 不把sourceVALID reservation当DMA AW handshake；未发意图不是合法head；source/job配对与events收费 |
| upstream AW packet/order | 原两槽spill header/full/valid、held有序sequence、完整source ID | source DMA header可以前进，旧spill packet需其原job绑定；不能给每bank凭空两个独占槽 |
| AWi/TC current | input demux global接受、真实w_select/w_open；bank AW/AR FIFO各原4项；current TC FSM | raw AW、DMA AW和TC targetAW独立；外层signedcounter不替代owner；RO/ref/ordinal/快照收费 |
| P fullwalk/current owner | 慢req_data至真实slow响应接受；PTW stage/level与当前DS接受/真实返回 | N只different bank，未来PTE内容/leaf/count不是免费输入；completed正常walk历史预测，类/epoch/时间收费 |
| physical AR L/Q与AWo | target demux header/select spill可比TC当前metadata活得更久；output muxAW首次VALID预留 | 不改变full ID/held；4bitprefix新构造与原native8slot不同；packet/job关联收费 |
| raw W/out W | input WL LAST与bank WFIFO16/targetspill2/outputmux分别接受 | raw可比physical领先至原缓存容量，不能当DDR写终点；各进度/字节/数据资格收费 |
| BR/正常退休/错误 | actual B接受＋backend lastmetadata；真实RRESP/BRESP/IOMMU fault；frontend WAIT->原next issue | NO_ERROR_HANDLING error0不证明成功；normal history排错，B尾独立；当前两job之后原batch/CPU/供给不能free STOP |

source width/原生容量为数据结构事实，不是实际交易完成数据。正常4KiB/64bit non-deburst每job应有2个256beat AR/AW、512beat R/W、2份B；所有PTE/DDR实际bytes/transactions/RESP/quiet未观察，不能声称已核守恒。error时按原source终结，不强套成功计数。

## 最小完整费用的比较

结构化逐规则账本见[cost-ledger.json](cost-ledger.json)。RR/currentDS+RR/earlyPause不强制服务history；owner可省完整AW/B预测；cheapDebt与dualGap只计自己的合法记录，原相同信息/容量可用不等于所有规则必须携带整套观察器。

原private AW1slot、global AW spill2slot共享、TC各4项、bank WFIFO16、target W spill2以及output reservation8slot全部继承，候选没有加buffer/credit。观察这些原字段仍有新增读取/接口/匹配/fanout/快照/时间费用。native字段可合法复用才省新FF，不能把RO看信号算免费。

父轮三组每bank10bit进度共60bit＋prefix4＋fault2＋pendingheld2＝列出组件68bit。它未包括本轮不同AW端点/源spill/选项/未发工作关联与B记账，也未含T/K/M时间/history/class/epoch/公平/score pipeline。一般prefix20bit替代4bit，不重复加。**68bit不是完整总量或已实现FF/面积**。

完整新增状态用cost-ledger中的参数式逐项收费：gate、association、option/order/snapshot、job/生成进度、open timestamps/history、normal/error/epoch/wrap、fair/age、score pipeline。读取端口、history update、包络前处理、max/sum/tie/overflow、选择延迟、CDC与实际CPU/poll/STOP/drain另列。

root本轮不给T/K/M猜值；原4/8在途不能自动当全部观察器记录数，尤其原后继可在一方done后继续。CPU/AXI端点可同域捕获时不强造MIG CDC；若读MIG域事件就计CDC与时钟/事件一致性，隐藏DDR不能免费使用。

完整bit/port/update/select/CDC周期、FF/LUT/BRAM/area/Fmax/CPU/application/netgain为空。没有合成/RTL/板测，节点数不等于工程面积，必要数字费用仍是进入下一验证阶段的缺项E5。

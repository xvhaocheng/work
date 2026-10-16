# §54 根验收：跨两页正常WRITE授权前沿

2次SV build成功、2次C++增量构建成功；16次真实完整copy全部PASS，CSV source AR声明拍数与每目标实际W256拍由根另核。数据全区及周边未写区、logical R各2048B、issue/done各一次由夹具检查。新夹具初稿一处printf转义不匹配退回DeepSeek，修订后精确应用；该失败没有执行DUT。完整配置/结果/SHA见execution-ledger.json。

模式含义仅本隔离版本：mode1长共享读/权限前沿暂退；mode2普通共同按成功页截断；mode3普通独立V先A后；mode0更强普通共享首段→V单独后段→A补段，并把A下一页新准备推迟到V logical RLAST。所有模式q2 W、相同已有helper/私有B3/skid，静态成功映射；不支持外部pause。mode1 admission必须V完整WRITE认证覆盖已firstVALID长AR，V是不可退出的sponsor；任意双方短认证不能继承进展保证。

A目标页内3072，实际两1KiB WRITE parent；延迟只注入实际下一页leaf 0x70048的单个读请求，每次非零delay均有exact matched/applied=1。不是fault、不是所有A流量限速。

|方法|次页delay0 V/A|delay10000 V/A|source R拍/AR|
|---|---|---|---|
|mode1 长读暂退|5400/7032|5400/17032|384/2|
|mode2 共同pagecut|6136/6127|16136/16127|256/2|
|mode3 独立V后A|4707/7823|4707/17823|512/3|
|mode0 强普通prefix/suffix|5419/7051|5419/17051|384/3|

强普通对照把新方法优势压到19cycles。B64+READY背压时mode0/1都为13510/29196、sourceR同384，仅AR3/2。因为普通同信息同流量几乎同效，不能把‘成功前沿暂退补读’单独包装为新算法。相对共同等待的V收益真实，但已有更强控制可达到。

6份同overlap模型、RR0/1成对运行也全部通过。RR1 selections=415/415/287但rotations/crossID=0，结果成对完全相同；只证明选项执行，**没有证明实际交错发生**。terminal natural phase=0是实验结束后的状态字段，不假称整个实验未激活或已交错。

关键新根诊断：A logical到128时可能仍有两拍在私有FIFO，尚未global W offer。严格detach hold把这些未承诺W留到V结束，A首rawB/第二rawAW/第二WRITE prepare因此也推迟到V RLAST。run14 A2PREP5463晚于sourceRLAST5382，解释无真实交错机会。下一§55在每成功WRITE前沿末拍要求已有Wcoverage，检查能否及时退休首raw父、准备次页并中途恢复共享。

暂退quiet只表示旧global qW/B/skid义务关闭，不代表全部private payload或首rawWRITE父已退休。没有fault/invalidation/multiplejob/面积/板卡验证。DeepSeek跨页/交错夹具及修订提案，Codex亲自设计、源核、应用、执行和根验收；无Claude。

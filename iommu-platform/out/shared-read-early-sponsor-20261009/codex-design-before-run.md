# §59：先独立启动，再接力共享尾段

Codex亲自提出并负责关键RTL。尚未构建或运行。

问题：旧接力先等待V/A初始READ及WRITE都成功再共享首段。强普通WR预取在较大F已经胜出，小F剩余差距未能排除初始cohort等待。新判别让V只依赖自身真实READ/WRITE成功就开始；A拥有独立原source logical parent，但在V结束前不发新W。A首WRITE真实AW已被qwriter捕获后，普通slot2正常预取下一WRITE页。

mode3新增可选handover。实际A两页WRITE均正常成功、双方READ均正常成功且源PA/normal属性一致后，复用§57协议：同拍停止增加V未来W/R债，取偶数J覆盖所有firstVALID W；V逻辑R/W实际到J、旧payload债闭合后，真实消费旧physical R到RLAST；从A逻辑cursor（本模式为0）发新的physical R，前J只给A，后段给双方，保持各自原logical ID/RLAST。原A页内WRITE父边界F仍独立合法；repair前缀内部qW到J裁齐，联合尾段按peer真实raw_written门避免W头环。

关键安全检查：mode3从未做初始共享匹配，所以必须在接力准入显式核两个成功源PA及cache/prot/qos/region/user一致，并要求两原AR已捕获。READ不能充当WRITE许可。A下一页许可来自真实首AW上下文、真实TL WRITE响应；未来许可成功才bootstrap接力。不存在AR/RVALID撤回、logical RLAST伪造或源payload新缓存。

强对照同job/normal属性/原B3 FIFO/qW2、同真实WR预取：mode3+prefetch1+handover0（V独立后A串行）；mode2+prefetch1（普通共同切分）；旧短K16接力。禁止把模式配置当实际接力，必须真实phase>=2。opt0的stage58对应轨迹是精确回归；极晚许可安全保持原串行。实际source R若接力为E+256（A原cursor0）；未接力mode3为512。

只支持同reset单job/static immutable源/分离目标/正常成功许可。prefetch可mode2或3，mode2禁止cutover，keepq1，无外部pause。真实有效运行后比较两方issue-to-DONE、mean和both-job makespan，及源读拍/AR、PTW/WR recipe完整成本。与§57相比不增加新的功能state，但与无接力强对照相比仍有11功能位及新增组合逻辑，观察位另计。没有综合面积、fault/invalidation、多job或板卡结论。

如果在同预取强对照上获得可解释的独有进度/成本改善，且最强前作没有同规则，才能作为独立机制候选；如果收益由提前启动/普通预取完全解释，则不立新算法。普通广播、预取、消费后重读原则已有前作，本文只查独立许可前沿下的数据债闭合与逻辑服务接力规则。

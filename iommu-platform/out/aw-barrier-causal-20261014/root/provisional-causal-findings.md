# 历史初稿：已归档，跨端点不等式撤回

本页原文完整保存在before/pre-ultra-provisional-causal-findings.md，指纹见before/provisional-manifest.json。初稿第4条把DMA源侧AW握手误等同input demux的AW接受，漏掉原上游SpillAw=1的两槽spill。

ultra亲核后：coupled第二AR只需第一AW在DMA源侧握手；该握手可先于input demux接受及B raw WLAST。因此不能从decouple_rw=0、FullBuffer或单个private AW slot直接推出第二AR等待B raw WLAST。正式结果见causal-chain.md、mechanism-card.md及root-result-acceptance.json。历史medium记录与actual blocked回执均保留，未改写。

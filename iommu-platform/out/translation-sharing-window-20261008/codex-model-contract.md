# §48 根级模型合同：只判缓存/步调的必要性

模型是有限逻辑译址请求trace，不是DUT、cycle仿真或完整copy实验。两个已登记32KiB copy，同context/source，destination各不相同，source/destination均4KiB对齐，8页；每页两个2KiB burst，按每flow D(p),S(p),D(p),S(p)发译址，保持该flow顺序。这是选择的合法有限顺序，不冒称§43实测geometry（旧destination偏移3072且两个context不同）。

三种trace：A整job后V（最大源页lead8）；A先4页后逐页V/A交替，A结束后补V（lead最多4）；逐页A/V配对（lead最多1）。两job在开始均登记；调度本身只需要job进度，不含尚未登记任务。

容量E=4/8/16，每mapping同key/context/permission大小计1槽。无PWC/coalescer；一cold leaf translation需3笔PTE。四普通对照：
1 全局LRU，正常插入；
2 registered-use dead-first/LRU：已登记每flow规则/当前index推导剩余lookup次数，仅判断是否还有用途（不见未来global排序），incoming无剩余用途可不填；cache full先选无剩余用途者再LRU。这即root上轮remaining-use规则的最强直接普通等价实现，不能把其对LRU优势称独有。
3 每DMA一destination last-page slot＋E-2 shared-source LRU slots，所有physical mappings总数E，没有免费私有大缓存。它使用source共享、dest私有的job分类信息；额外owner/ref/ports另讨论，不称完整相同面积。
4 full-trace offline Belady with optional admission作为读取下界参照，它获得更多未来global顺序信息，不当可实现同信息candidate。

输出各case request_count/unique_keys/misses/hits/3*misses、每flow misses与trace。总24 unique mappings是首次translation下界；oracle不能低于24，candidate不能比oracle少。另小N=2/E=2和3的有限trace用独立memo DP枚举evict/bypass核oracle（两个容量即可，不大扫描）。

目的：看shared-input错开是否真有非compulsory repeats，哪些普通组织/普通page pacing已经消掉；只miss/PTE工作，绝不转换实际cycles/速度，也不声称满足最新producer irreversible W debt。页面配对需下一核W依赖/供给/失效安全；固定barrier是普通强对照不是新算法。


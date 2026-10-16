# §46 root：resident-way历史与requester历史的区别

root亲自提出共享alias例和接口不可区分证明，直接核IOTLB/TL/fast模块/既有测试，并实读2008专利原文。两席交叉质疑。没有新增RTL、仿真、数学运行、BIT或上板；下面是源码构造，不是新实测。

## 1. 当前能成立的行为

IOTLB tag=`VPN/PSCID/GSCID/stages/valid`，无DID；`:101–139`lookup不检查policy fillmask。因此DID10/11引用同一合法地址空间时可以命中同一条目。fillmask由TL中稳定main请求DID选择(`tl_wrap:669–673`)，不禁止另一设备读取该条目。

`iotlb:384–401`的PartitionLocalHistory按被命中的resident way（i<2或background）选择可更新PLRU节点，不按真实请求DID。`tests/composition_iotlb_isolated_test.cpp:570–576`明确要求V-way在A-mask下命中也更新protected节点7：旧合同有意支持main A忙时fast V命中，不能把其通过说成requester隔离已经通过。

因而旧合同是**不同resident分区各自的PLRU节点隔开**；不是**A发起的任何命中都不能修改V历史**。共享context alias暴露两个合同的区别，不据此称旧独立context实验失败或源码不遵守已有物理way定义。

## 2. 合法两way反例

用`codex-sharing-reachability.md`中的同PSCID/root构造。V protected ways0/1已保存P0=IOVA0x10000000和P1=0x10001000。可通过先fill P0、显式hit P0、再fill P1、显式hit P1得到两条均valid且PLRU节点7=0，下一V fill victim为way0/P0。

随后A（DID11）正常读取P0，得到合法共享hit。它的fill policy_mask=0xfffc，但当前history由resident way0归属决定，节点7改为1。下一V读取第三页P3=0x10003000，cold fill在V mask0x0003下选择way1，淘汰P1；若A未命中P0，则淘汰P0、P1保留。P3对应原fixture的leaf PTE0x50018（值0x200008DF→SPA0x80002000）。读取合法，无global绕行、无权限/映射变化。

这里只改变未来victim，未测平均A/V速度；一次源构造不推出PN变慢的旧原因、板上重现或统计收益。

## 3. 为什么不能直接用main fillmask过滤touch

考虑相同resident V/P0条目与相同mask0xfffc的两个合法事件：

- main请求为A，fast V请求命中P0：V自己的命中应更新V requester历史。
- main请求为A，它本身通过普通路径命中共享P0：若要求requester隔离，这次A命中应不更新V requester历史。

在IOTLB输入边界，两者的lookup、VPN、PSCID、GSCID/stage、touch=1与policy_mask可完全相同。`nonblock_hit:54–55`使用fast请求的IOVA与其自身DC的PSCID，`:84–100`核自有DC/权限，`:103–112`接受时产生touch；`tl_wrap:271–277,675–681`选择fast或main lookup并传touch布尔，却没有传hit requester DID/class。当前IOTLB确定性逻辑没有信息区分两者。把mask0xfffc都当A会错误压制第一种V hit；继续用resident way则不能隔离第二种A alias。

需要把**实际lookup来源**的class或等价区分信息与accepted touch一起传过这个边界；两类用一个class bit可以区分，原DID/context/holding/配置稳定与有效标记仍需按实际pipeline计费。这是局部充分工程接口，不是总最小位数、LUT或已实现代码。

## 4. 强普通修正与明确前作

普通修正可以保持shared hit允许，而仅在真实requester的可分配resident子区更新其local历史。fast touch由fast已accept请求class驱动，main touch由main原请求class驱动，fill依旧由稳定main owner驱动。不能用bank代替DID、不能把当前占用walker的请求当fast hit owner。当前旧代码未实现此sideband，也未运行修正。

root实读[US7415575B1：Shared cache with client-specific replacement policy](https://patents.google.com/patent/US7415575B1/en)，公开2008-08-19、priority2005-12-08。Fig7的步骤702/706/708接收clientID，允许访问全部TLB entry，但只按实际requesting client更新对应局部历史；claim6同样区别。多restricted-client各subset/独立recency也有直接正文。它没有本RTL的node eligibility或fast/main接口验收，但已覆盖共享hit与requester-specific replacement的基本机制。

因为该机制有具体直接前作，补真实hit class与历史过滤不能作为独立创新。它可以成为普通强对照/工程合同补充。只有另外证明同资源与合法共享下一个前作/强普通做不到的性能或必要性点，才可提出新的论文主贡献；当前未给。

## 5. 最小后续与本轮界限

若日后决定采用requester noninterference合同，才需要一个同PSCID/shared-root的TL端到端定向例和普通filter对照，检查main A/fast V与main V/fast A身份、完整返回、cache hit及后续victim；复用现成bench，不改已有物理way测试的预期来虚构旧功能失败。当前无独立机制差异，不先做该实现或BIT，不重复旧测试。

# R8未满名义分区仍重复walk：实际替换源码核查

2026-10-17；Codex-only；有界源码诊断。仅创建本文件，未改源码、未编译、未运行仿真或新条件。下文区分既有轨迹事实、源码推导和未测内部状态，不代root作论文主线裁决。

## 结论

**A的14项是可分配way掩码，不是“≤14个不同页必然全部驻留”的保证。**实际实现为保留原16叶二叉树形状的masked tree-PLRU，无invalid-way-first；partition-local history只隔离V/A各自可选择的历史位，没有将A十四项改造成LRU或平衡的14叶结构。反复访问固定源会把目的替换导向一个子树；不等大的子树又各获一半选择，足以在A仅9页时产生重复miss。

本次初始访问顺序与源码可推导出具体结构：A目的先落way2、A源后落way8；源反复命中令后续目的分配偏向ways2–7，其中两项子树2–3和四项子树4–7交替被选。R8偶数目的4页在两项中轮换，奇数目的4页在四项中驻留。因此预期目的walk为`32个偶数parent + 4个首次奇数parent =36`，再加A源1、V两页2，合39。**既有P和PN轨迹的页身份/parent模式均精确符合这个推导。**内部way分配未直接落盘，不能把推导伪称为实测occupancy或victim快照。

该现象可归为当前普通替换实现的容量利用/替换局限；不是从“9<14”即可宣布的功能bug，也没有证据要求新增付费信息接口。普通LRU、invalid-first或不同替换组织是否足够改善完整结果，本轮没有实施，不能声称已经修好或能节省多少周期。

## 实际编译身份

以`../build01/build-manifest.json`的`sources`与`argv`为准，本次核对以下当前文件SHA256均与编译记录相同：

|简称|实际来源|SHA256|
|---|---|---|
|I|`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv`|`2868d6de26c8d98d2a750a0d78f6ce96cb896b9253b82b070a3e3772f37e9d26`|
|T|`../overlay/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv`|`c329c377a757abbf5d8d453800f154bf734c2a0acde8051670fe8eeea5a6afe5`|

即TL来自新测试overlay，IOTLB来自实际组件源码；不是误读旧out中同名模块。manifest为`-GPolicyEnable=1`、`-GPairPolicy=0`；P/PN runtime均partition=true。T:663–666启用TouchEnable/PolicyEnable/PartitionLocalHistory，T:675–679把DID10分到ways0–1，把A DID11分到ways2–15。PSCID匹配仍按I:105–106；本诊断不依赖DID被误作cache tag或namespace混淆。

## 源码决定了什么

1. **全相联查找与替换掩码分开。**I:101–137遍历全部有效tag，按stage/PSCID/GSCID/VPN匹配；I:365–372仅用allocation_mask构造eligible tree。地址页号没有在lookup中直接固定到某一组。本次不应写成“IOVA索引冲突”或物理容量仅6项。
2. **没有空way优先。**I:405–422对每个候选way沿原树逐层计算choose_right，输入只有eligible child和`plru_tree_q`；不读tags.valid。I:317–331据replace_en填tag/content。因此其他eligible way即使无效，也不保证先被使用。fill本身不在I:375–424更新PLRU；完成后该请求的实际lookup hit才触碰历史。
3. **命中把victim方向指向另一子树。**I:384–400计算`new_index=~((i>>(shift-1))&1)`并写对应历史位。热源若在根的右子树，反复命中就把下一victim指向左子树；这是tree-PLRU规则本身，不需要V驱逐A。
4. **partition-local history不是容量保证。**I:353–362识别0–1/2–15固定分区。I:394–400只更新该resident way所属分区两边都有eligible叶的节点。V只有0–1，不能更新A需要的根方向或A内部可选择节点；N接受的V hit也按resident way归属，而不是拿main mask错误触碰A。A内部根左右分别有6/8项，左子树再分2/4项；二叉PLRU每个可选择节点仍用单个方向位，未按叶数比例分配。
5. **冷初态固定。**I:463–472将tags/content/tree清0；policy_clear也在I:424清树。本轮没有parent间清空或变更映射。这个解释依赖实际冷历史和初始访问顺序，不扩张成所有供给顺序的保证。

## 从源码手工推导本次形状

以下为源码推导，未运行新替换模型或修改仿真：

- A mask为`fffc`，初态tree=0；根先选左、左节点先选左，但0–1不eligible，被迫转向2–3，先取way2。因此首个A目的0落way2。
- 目的0填完后的normal hit令根指向右；V hit按partition-local规则不改这个A根位。所以后续首次A源填入右子树最低way8。
- 对A源way8的hit令根指向左；每个后续parent的源请求又访问同一页。目的都在左6项中选择，其命中/填后触碰可以把根指回右，但下个源hit再指回左。右侧其余7项因此没有被目的自然利用。
- 左6项在原树中是2项的ways2–3和4项的ways4–7。每次目的在一个child被命中后，node1方向转向另一个child。源way8不触碰node1。于是目的序列在这两个child间交替，而非按2:4容量分配。
- 从零历史手推前八个目的victim为`2,4,3,6,2,5,3,7`。到目的4时已经重用way2，尽管way5、way7尚未填满；这直接说明为何“总footprint9<14”不能推出只冷填11次。

本次R8的偶数页0/2/4/6循环进入两项child，全部32次访问miss；奇数页1/3/5/7进入四项child，仅首轮4次miss。R16则偶数8页对2项、奇数8页对4项，两边都超出，各64个目的parent均需walk。R1的目的与源分别驻留即可，与总4次冷walk相容。

## 既有原始轨迹检查

未重新运行六条件。读取已有`../analysis/summary.json`定位后，本席独立流读四个P/PN、R8/R16原始`events.csv`，只按真实`ptw_start`及IOVA页分类，结果为：

|已有条件|V两页|A源|A目的页walk分布|全部walk|
|---|---:|---:|---|---:|
|P R8|2|1|p0/2/4/6各8次；p1/3/5/7各1次|39|
|PN R8|2|1|p0/2/4/6各8次；p1/3/5/7各1次|39|
|P R16|2|1|p0..15各4次|67|
|PN R16|2|1|p0..15各4次|67|

`summary.json.cases[*].walks`保留父身份，R8 P/PN两者A目的walk的j恰为`{0,2,...,62}∪{1,3,5,7}`。该模式不是只拿总39反向凑解释；页级分布和正常hit也同向支持。

可直接导航的P R8原始证据（均为`../runs/base-p-r8/attempt01/events.csv`的真实行号）：

|事件|cycle|行号|含义|
|---|---:|---:|---|
|首次A目的0 lookup / walk|569|330 / 332|目的先于A源，IOVA `0x20100000`。|
|首次A源walk|1979|1374|IOVA `0x20000000`；中间V源walk在行975。|
|A源hit→目的1首miss|3955 / 4455|6253 / 8349（walk8351）|源先命中，再处理新目的，符合根方向推导。|
|j8目的0重复miss / walk|15159|44129 / 44131|正常首查tag9，下一行链绑定walk12。|
|j9目的1正常hit|16697|49269|正常首查tag13；没有新walk。|
|j10目的2重复miss / walk|17721|53687 / 53689|偶数页重复walk。|

tag9与13的差别为runner的physical-hit位；它们都是normal first检查，不是把N probe miss或驻留重复检查算作walk。PN对应j8/j9/j10的首查在`../runs/base-pn-r8/attempt01/events.csv:45405/50697/55092`，分别miss/hit/miss；全量页计数如表。`fill`事件只给normal bank，未提供replace_mask或resident entries；故具体way分配仍是源码推导。

## 本轮接受与保留的问题

可接受：当前masked tree-PLRU足以解释低于名义14项footprint仍有目的重读；partition-local history解决跨分区历史污染，不解决分区内部不平衡及热源路径偏置。R8不能再被称为“保证首填后无walk”的控制，应保留它实际观测到的替换局限。

不可据此认领：已经独立测得每一次victim、cache损坏/错译、最优ordinary基线、普通替换修改的完整加速、或新机制必需性。invalid-first仅可避免空way尚在时的提前覆盖，不自动等于后续不会发生tree-PLRU循环；只加它是否足够也未验证。是否安排后继普通优化或何种机制，仍由root按合同与原始等待证据裁决。

# 三个替换策略测试副本

只修改本目录中的独立IOTLB副本，外部端口、查找、页大小、权限内容与失效逻辑保持原有定义。生产文件没有写入。生成器是[make_variants.py](make_variants.py)，输入原件是[discovery-rv_iommu_iotlb.sv](discovery-rv_iommu_iotlb.sv)，其SHA256 `2868d6de26c8d98d2a750a0d78f6ce96cb896b9253b82b070a3e3772f37e9d26`与10月17日原始build01 manifest一致。实际组件HEAD是`57eb5621802ef9ed8d77c684b9a03205c6630391`，但基座来自已有工作树而非直接取这个提交的文件；[source-identity.json](source-identity.json)保存完整路径和hash。

- V0：功能逻辑逐字保留；只在`ifndef SYNTHESIS`中添加被动逐fill追踪。
- V1：对既有固定分区（ways0–1和ways2–15）使用按way升序、递归近半分的tree-PLRU。14叶根由原6/8改为7/7，子树不再沿用原mask留下的2/4切分；14不是2的幂，下一层仍有3/4。这是mask布局的近均衡消融，不是严格无偏算法、唯一bug修复或LRU。其余mask和配置使用原树选择。原合法hit/touch事件、按resident way决定owner、无invalid-first、fill不更新历史全部保留。紧凑树单独保存14个历史位（背景13＋保护1），不会让保护分区触碰背景树。保留旧树及其更新仅为回退与观测；这份功能测试实现不用于报告面积成本。
- V2：同样16个物理way与eligible mask，采用精确LRU全序（rank 0最旧，15最新），优先填入最低编号的无效eligible way；无空way时选最旧eligible way。合法hit移至MRU，实际提交的fill也移至MRU；同一边沿先处理旧条目的hit，再处理fill，victim始终按边沿前状态决定。clear优先于fill并重置顺序；普通失效只清valid。跨分区hit只改变全局序号，不改变另一个分区内条目的相对顺序。V2是标准LRU参照，空项优先和fill-MRU也是其定义的一部分，因此V0→V2不能解释为只有mask布局发生改变。

V1仍保留tree-PLRU本身的热源偏置及非stack性质：反复命中右半的源页仍能把目的替换限制在左半。没有预设其R8一定降到冷填数量。

追踪为提交边沿之前的真实寄存器状态，每行格式：

```text
IOTLB_VICTIM,0|1|2,local_cycle,instance,pscid,gscid,request_vpn_hex,victim,evicted_vpn_hex,evicted_valid,valid_bitmap_hex,eligible_mask_hex,legacy_plru_hex,kind,active_tree_hex,lru_ranks_hex
```

`kind`是`invalid_fill`或`valid_evict`；只有update被真实提交且未被clear/失效阻止才输出。`valid_bitmap`是驻留有效位，`eligible_mask`是可分配way，两者不是同一概念。cycle在reset低时清零，首个reset释放边沿为0，与runner的`IOTLB_CYCLE_ANCHOR`对应。模块没有设备SID输入；PSCID/GSCID不冒充SID，真实设备由上层PTW/translation owner事件关联。V1的active_tree低13位为14叶近均衡树内部节点的先序编码，bit13为保护分区；其余配置active_tree就是legacy_plru。V2的LRU序号每way四位打包，way0占最低四位。

# 2×128beat普通分段对照的原接口边界

2026-10-16；root新反例驱动的最小补核；请求Sol/high，actual metadata unknown。**应保留固定2×128作为强普通对照；当前reg64生产接口没有现成的read-only128运行时选项，但backend有可复用的分段能力，不能因frontend缺项排除这个适配。** 本次只读具名接口，没有新模型、构建或生产改动。当前frontend/legalizer hash与前次历史绑定一致。

## 1. 当前已有能力

- 当前 `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:365–373`：`decouple_aw=0`；`decouple_rw=CONF.decouple`；`src_max_llen=dst_max_llen=0`；`src_reduce_len=dst_reduce_len=CONF.deburst`。所以现成deburst把读写两侧都限制到一个数据beat，不是只读，也不是可选128beat。`serialize`赋值在`:375–376`停用。
- 当前generated `idma_reg64_frontend_reg_pkg.sv:28–47` 的CONF只有decouple/deburst/serialize/pv/ttype/cache，没有独立source长度或128字段。`idma_reg64_frontend.hjson:50–67`的老CONF描述更少，不能拿它替代实际generated接口；决定结论的是当前frontend的硬连接与reg_pkg。
- 当前 `idma_legalizer.sv:139–178`已有分别来自src/dst的reduce与max_llen：64bit下source reduce=1、max_llen=7时按1024B边界（128beat）合法化。`:201–214`说明**若decouple_rw=0，读写都用两侧较短边界**，仅更改source上限也会改变W分段；要read128/write256须独立source limit并选择decouple_rw=1，还要保持实际buffer、AW/W配对及进度合同。现有CONF.decouple可以选该bit，但独立source_limit没有软件入口。
- `idma_legalizer.sv:289–308`把这些options纳入一个输入job；`:448–469`依据coupled/decoupled分别推进两机并在两者完成后接下一request。它不是运行中任意撤回已声明AR或重写已接受burst的接口。firstVALID之后的L仍须保持完整字段；对“剩余一段”作不同选择，须在该段产生/首次offer前定义适配位置。

因此，最小新普通适配可以从已有legalizer参数接线入手，而不是重造DMA：静态固定source上限或暴露一个受限runtime source_limit均可分析。若策略只对最后一个256beat拆2×128、此前保持原长段，还需明确按job/段位置选择和计费，不能把一个全job静态128设置说成仅增加一个AR。相同4096B job若全部source都从256降128，通常两原AR变四AR；只拆指定剩余256才是一原AR变二AR。这里是对齐/无额外页边界下的逻辑计数，不是实测费用。

## 2. 若改成普通两个子COPY，应计哪些费用

用两个1024B subCOPY代替剩余2048B的一次COPY，与保留4096 parent只拆其读burst是不同程序。

| 项目 | 已核源码合同 | 能断言的范围 |
|---|---|---|
| 配置/提交/完成 | frontend `:325–327`每job含src/dst/length，`:490–497`读next_id发起，`:519–559`发request后等trans_complete，`:620–628` issue/retire管理ID | 两subCOPY须各有合法request/完成、共同parent汇合及CPU/descriptor供给；精确MMIO写数、CPU周期和中断数依程序，不能凭空计常数 |
| AW/W/B | legalizer `:251–267`每child有自身write工作，`:347–415`生成对应AW/数据元信息；当前普通合同 `out/shared-service-control-boundary-20261015/controls/ordinary-program.md:147–153`保留每AW对应B及完整parent退休 | 对齐两个1024B子COPY会有两次对应AW而非原单2048B的一次，W有效字节相同，AW/B次数及元信息/owner生命周期须计；不能把额外write侧费用藏为只有一个AR |
| 翻译 | TC `:625–681`分别从AW/AR捕获地址/DID/type并请求翻译 | 新AR/AW分段会增加该请求路径的调用次数；同页缓存命中可能无需新增PTW，不等于每child多一次cold walk，也不等于lookup免费 |
| CQ/cache/许可 | 本次具名frontend/TC接口没有每subCOPY强制发IOMMU CQ命令的合同 | 同映射、上下文与所有权无需改变时，不能自动加“每child一次CQ/invalidate”。若提前准备/变更许可另要求CQ/cache同步，按具体程序计；当前精确次数unknown |
| 返回容量与资格 | `idma_fullbuffer_certificate.sv:14–18`只给正常aligned4096 job资格；TC `:645–647`依fullbuffer和匹配DID绕owner读准入；frontend `:153–157`原batch也要求4096 | 1024B子job不保留原4096 fullbuffer证书，第二子job地址还未必4KiB对齐。故不能把子COPY当S状态下“仍可绕V owner读准入”的免费同义实现；需重新导出合法顺序，或选择保留4096 parent的legalizer分段适配 |

最后一项只限定**该具名subCOPY实现**，不否定普通分段策略或其接口价值。read-only128适配如保持原4096 parent，原fullbuffer资格不会仅因读burst缩短而自动失效；仍须检验decouple条件、W配对和buffer进度。

## 3. 交给design的直接约束

2×128可以先作为来源支持的条件动作，按完整接口成本计；无需等native实现后才能发现它会追平earlylease。模型至少区分三个程序：原生两侧单拍deburst；保留原parent、独立read128的付费适配；改变parent为子COPY的普通软件程序。若read128已经用更少/相当完整成本达到相同makespan，就结束该版本的性能差异主张；frontend缺项不足以挽救它。反之也不能因为策略同样简单就把新增接线/配置/选择/验证费用记零。

新增定向范围与当前sha256追加在 `evidence.json` 的 `split_control_supplement`，未改先前来源条目。

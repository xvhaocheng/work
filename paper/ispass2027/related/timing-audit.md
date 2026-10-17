# 论文板测 BIT 时序与加载身份审计

审计日期：2026-10-18 JST。范围是 [outline.md](../outline.md) 和用户提供的
《G3 论文主张与五条结论（草稿）》所用板测，以及用户明确列出的全部历史
结果族。G3 标题称“五条”，正文实际编号 1–6；本文按正文编号，6 是仿真。
没有修改原数据、报告、论文大纲或 G3 附件，没有重新构建、运行实验或操作
FPGA；只读 Dell 原件和 Mac 已保存的文件/日志。

## Material Passport

Origin Skill: academic-research-suite / experiment-agent；Mode: validate
（加载身份与静态时序证据核对）；Verification Status: SOURCE_AUDITED。
这不是重新运行 Vivado、重新测量板卡或完整 CDC/IO 签核，也不重新验收
科学统计、前作差异及未完成的 E6。Codex 分组取证，root 直接审查关键原件、
核对报告数值和字节哈希后形成本文；未调用外部模型。

## 判定方法与哈希口径

先沿总账的轮次/后继更正找到原采集记录，再接加载文件、交付/构建清单、
最终报告、同一 checkpoint 生成 BIT 的日志。**最终**包含明确生成后继 BIT
的 route-opt；不能把较早的负裕量报告移接给优化后的另一份 BIT，也不能把
当前被覆盖的构建目录视为原运行身份。

- **P：程序文件 SHA 已记录。** 历史加载记录保存了非空文件 SHA，接成功
  编程和对应启动；它不是 FPGA 内部配置回读。E1 历史记录也是此类，但
  不补写不存在的 `hash_verified=true`。
- **E：交付期望/声明 SHA。** 历史加载 `sha256=null`、未核验，或只有
  程序路径与声明哈希。按用户规则列交付清单期望值，明确标记
  **“未在板上核验”**。可在披露此限制后作对应配置的历史量化；不能写为
  已核验实际配置字节。
- **U：历史 SHA 未知。** 连交付清单中的期望哈希也未定位到。表中仅另列
  今日存档/交付包候选 SHA；不得将它填回历史字段或冒称历史期望值。
  这种情况下，正裕量属于具名构建，严格按 SHA 审核的板测定量主张仍待补证。

表中 WNS/TNS/WHS 单位为 ns，S/H 是 setup/hold **失败端点数**，不是路径数。
每项使用全局 `Design Timing Summary`；报告均指向 50MHz 配置/STA 的
CPU/IOMMU 时钟，不把它称为独立实测频率。所有“可”都只针对本项时序
资格及所列身份范围，不代替原运行的正确性、供给、统计与观测边界检查。
STA 失败不证明每个读数错误，也不能给出误差大小；本审计不据 WNS 推算
所谓安全降频。UART/DONE/数据一致不能替代时序闭合。

## 总表

“否”表示不能充当论文正式定量证据；历史数据仍原样保留。“暂缓”表示
最终构建时序通过，但历史加载的字节身份尚未闭合，不能写成时序失败。
相同 BIT 的不同采集段分别列出；DS sweep 的 DS0/50 是复用，不增加试次。

| 结果/采集组 | BIT SHA256及其性质 | 身份等级 | 最终 WNS / TNS / WHS（ns） | 失败端点 S / H | 论文定量主张资格 | 证据 |
| --- | --- | --- | ---: | ---: | --- | --- |
| E1 | `df7c2400555bba8a95c4ab2b31526fa7958e669dd4c3bfd216140d01de22d5e3` | P：程序文件SHA | +0.177 / 0.000 / +0.010 | 0 / 0 | 可（P） | [E01](#e01) |
| E4 | `41fa23d0aee476b603111df192c612f3d40e844067211564d2b8b175a60b9b90` | E：期望SHA，**未在板上核验** | +0.177 / 0.000 / +0.027 | 0 / 0 | 有条件可（E，披露身份缺口） | [E02](#e02) |
| batch 主矩阵60 | `59ebd4c76d63941b2115d01c736649f2b409b5615f21bd3937a4f9f0524fcdf2` | E：期望SHA，**未在板上核验** | +0.177 / 0.000 / +0.020 | 0 / 0 | 有条件可（E，披露身份缺口） | [E03](#e03) |
| batch 扩展30 | `59ebd4c76d63941b2115d01c736649f2b409b5615f21bd3937a4f9f0524fcdf2` | E：期望SHA，**未在板上核验** | +0.177 / 0.000 / +0.020 | 0 / 0 | 有条件可（E，披露身份缺口） | [E04](#e04) |
| 历史 DS0 | `c6555d14245935820224a2cae2d835a65bade81fe993d5f8d626ec0ff2724050` | E：期望SHA，**未在板上核验** | +0.177 / 0.000 / +0.040 | 0 / 0 | 有条件可（E，披露身份缺口） | [E05](#e05) |
| 历史 DS50 回切 | `59ebd4c76d63941b2115d01c736649f2b409b5615f21bd3937a4f9f0524fcdf2` | E：期望SHA，**未在板上核验** | +0.177 / 0.000 / +0.020 | 0 / 0 | 有条件可（E，披露身份缺口） | [E06](#e06) |
| path v3 正式30 | `62f822d19851da0309b1a7e085197661caa72f6366fccf34d721fa418b941c29` | E：期望SHA，**未在板上核验** | +0.177 / 0.000 / +0.024 | 0 / 0 | 有条件可（E，披露身份缺口） | [E07](#e07) |
| E3 v4 | `a99ce9565b82449ca5eb0a8afc18c8b79cdf9e23a5a4e9197aafae02b5df98d4` | E：期望SHA，**未在板上核验** | -8.961 / -7133.546 / +0.039 | 4321 / 0 | **否：时序失败，仅历史观察** | [E08](#e08) |
| E3 v5 | `8f90865ef1fb4f747e7a1f16c3a4992adf4499c56356e0d8d1679b8962d7342e` | E：期望SHA，**未在板上核验** | -9.717 / -7453.361 / +0.045 | 5034 / 0 | **否：时序失败，仅历史观察** | [E09](#e09) |
| E3 v6 | `b55f5afbba667a7540ed003dd724d078e0e6e4bff5d30fe7e796abba5b7f2ecc` | E：期望SHA，**未在板上核验** | -6.470 / -8455.683 / +0.026 | 5496 / 0 | **否：时序失败，仅历史观察** | [E10](#e10) |
| E2 role1 新版64 | `319b415e0a717565fa11df7bf23c1c9cc6ac8e283cdbaea791e795c9b2924d40` | E：期望SHA，**未在板上核验** | +0.177 / 0.000 / +0.028 | 0 / 0 | 有条件可（E，披露身份缺口） | [E11](#e11) |
| P4M 短测4 | `7af1ccbcb36306d5439eeab9b39d8fdfa266f70ebfedd7c0a4f0dd1be6825d0e` | E：期望SHA，**未在板上核验** | -10.091 / -16096.983 / +0.045 | 6455 / 0 | **否：时序失败，仅历史观察** | [E12](#e12) |
| P4M pilot21 | `7af1ccbcb36306d5439eeab9b39d8fdfa266f70ebfedd7c0a4f0dd1be6825d0e` | E：期望SHA，**未在板上核验** | -10.091 / -16096.983 / +0.045 | 6455 / 0 | **否：时序失败，仅历史观察** | [E13](#e13) |
| P4M formal60：25+6+29 | `7af1ccbcb36306d5439eeab9b39d8fdfa266f70ebfedd7c0a4f0dd1be6825d0e` | E：期望SHA，**未在板上核验** | -10.091 / -16096.983 / +0.045 | 6455 / 0 | **否：时序失败，仅历史观察** | [E14](#e14) |
| research84 | `7e7024fc00a722b2ed9a78ab63223af70a54bcca423407643385e6c729e0bc62` | E：期望SHA，**未在板上核验** | +0.177 / 0.000 / +0.037 | 0 / 0 | 有条件：仅已接受成功子集，非84全过 | [E15](#e15) |
| fullstats21 | `be463eeb2b72b85df47477ea4982c4ed5289bca2829015c2d25bc6a62c9c95a7` | E：期望SHA，**未在板上核验** | -0.944 / -674.399 / +0.038 | 2490 / 0 | **否：时序失败，仅历史观察** | [E16](#e16) |
| method462 | `31e18ee96e6edd47d444ed3cf9f4279cfa1ba494b53497d37183a3d7d0c33651` | E：期望SHA，**未在板上核验** | +0.048 / 0.000 / +0.028 | 0 / 0 | 有条件可（E，披露身份缺口） | [E17](#e17) |
| method88：两boot | `31e18ee96e6edd47d444ed3cf9f4279cfa1ba494b53497d37183a3d7d0c33651` | E：期望SHA，**未在板上核验** | +0.048 / 0.000 / +0.028 | 0 / 0 | 有条件可（E，披露身份缺口） | [E18](#e18) |
| N v3 S | `594252c1424555256c5598ada1927a67760d3e49652785fc604545ebaa745bc0` | P：程序文件SHA | +0.177 / 0.000 / +0.031 | 0 / 0 | 可作该版描述；fast=0，不能证明绕过收益 | [E19](#e19) |
| N v3 N | `ecfd9749578cf4e9920755aed36670c2b59834e66e148f8916ca8cd29deda33e` | P：程序文件SHA | +0.145 / 0.000 / +0.038 | 0 / 0 | 可作该版描述；fast=0，不能证明绕过收益 | [E20](#e20) |
| N v4 S | `1784bb10d31e3c0caa1cdc5df0d9a95992a92e15e7b9c4f1278a5df5e51aac98` | P：程序文件SHA | +0.124 / 0.000 / +0.046 | 0 / 0 | 可作该版描述；fast=0，不能证明绕过收益 | [E21](#e21) |
| N v4 N | `3e4a4af644fb1625e3fc782fb38eb43713e2eda12d183acf00e9f0945ad3a011` | P：程序文件SHA | +0.177 / 0.000 / +0.039 | 0 / 0 | 可作该版描述；fast=0，不能证明绕过收益 | [E22](#e22) |
| N v5 S | `baca9197c69d5faf23c381953bf464460952dd97b3b26f65902e7f7b0a702f66` | P：程序文件SHA | +0.177 / 0.000 / +0.035 | 0 / 0 | 可作该版描述；保留缓冲/跨BIT混杂 | [E23](#e23) |
| N v5 N | `204edcc2d4f6a76adf2a39c7f9f62f455734978513a8dda82a0720fd11ea5994` | P：程序文件SHA | +0.177 / 0.000 / +0.038 | 0 / 0 | 可作该版描述；保留缓冲/跨BIT混杂 | [E24](#e24) |
| composition-clean172 | 历史未知；今日候选 `d65b98bc15b4c18c84da175fa430c891082af29c0bbf5f99ac0b785af209684c` | U：历史SHA未知 | +0.169 / 0.000 / +0.038 | 0 / 0 | **暂缓严格定量：缺历史SHA** | [E25](#e25) |
| composition-isolated22 | 历史未知；今日候选 `f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca` | U：历史SHA未知 | +0.022 / 0.000 / +0.051 | 0 / 0 | **暂缓严格定量：缺历史SHA** | [E26](#e26) |
| composition-gap18 | 历史未知；今日候选 `f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca` | U：历史SHA未知 | +0.022 / 0.000 / +0.051 | 0 / 0 | **暂缓严格定量：缺历史SHA** | [E27](#e27) |
| composition ptwfix172（前史） | 历史未知；今日候选 `1c709458c33a1e0e04fc5745f627f71780c1208c98223094c14da27e3a4228d3` | U：历史SHA未知 | +0.031 / 0.000 / +0.019 | 0 / 0 | **暂缓严格定量：缺历史SHA** | [E28](#e28) |
| G1：唯一9次 | `59ebd4c76d63941b2115d01c736649f2b409b5615f21bd3937a4f9f0524fcdf2` | P：程序文件SHA | +0.177 / 0.000 / +0.020 | 0 / 0 | 可（P） | [E29](#e29) |
| baseline100 DS0 中50 | `e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6` | P：程序文件SHA | +0.177 / 0.000 / +0.047 | 0 / 0 | 可（P） | [E30](#e30) |
| baseline100 DS50 前25/后25 | `580b34014914a1bee8541ed1e23b36a164bfc0fea3a2f691f56870b44dc35e77` | P：程序文件SHA | +0.177 / 0.000 / +0.033 | 0 / 0 | 可（P） | [E31](#e31) |
| DS sweep DS25 新50 | `48896ebf59f65df339a369fba30e1b5b0c77d2b1e49916f921bdc91c09da10fb` | P：程序文件SHA | +0.177 / 0.000 / +0.048 | 0 / 0 | 可（P） | [E32](#e32) |
| DS sweep DS100 新50 | `11f0f2d6bce196707df98262d8f21cf361634d5599c273a75bbc33622a3be9e1` | P：程序文件SHA | +0.177 / 0.000 / +0.045 | 0 / 0 | 可（P） | [E33](#e33) |
| DS sweep DS0（复用） | `e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6` | P：程序文件SHA | +0.177 / 0.000 / +0.047 | 0 / 0 | 可（P） | [E34](#e34) |
| DS sweep DS50（复用） | `580b34014914a1bee8541ed1e23b36a164bfc0fea3a2f691f56870b44dc35e77` | P：程序文件SHA | +0.177 / 0.000 / +0.033 | 0 / 0 | 可（P） | [E35](#e35) |

35个采集/复用条目对应26份不同最终报告和26个不同存档/候选BIT身份；这不是35次独立实验。5份最终报告有setup失败（E3三版、P4M、fullstats），所有所查最终报告的hold及pulse-width失败端点均为0。各报告THS均为0。U类有4条（含ptwfix前史），对应3个候选构建，未计为历史加载SHA已核验。

## 对 outline 和 G3 结论的影响

G3 以本轮附件的原文为准，定位到第19、23–28、38–40、53行；没有把其
“请导师判断”或待补实验建议当成新执行指令。附件的文件名、SHA 与路径
在本文末尾登记。

1. **G3 结论1：时序资格保留。** DS0 的统一基线程序文件 SHA 与正裕量
   最终报告接通；105.6%、95.7%/90.8%及10.4%/16.4%不会因这次时序审计
   被撤回。但“只是带宽平分”仍须遵守原 B-1 的响应记账近似与跨窗事务
   误差口径；时序通过不额外证明严格带宽守恒。本文未重新归约这些数字。
2. **G3 结论2：保留统一基线的分段观测，撤下未闭合版本的逐请求定量
   因果背书。** G3第24行已正确排除 E3 v6 的127–130周期；但第9/11行的
   “逐请求干预”、第39行的“一个具名页表遍历响应”实板贡献，以及第53行
   同类差异点仍依赖 E3，必须同步限定为历史诊断，或待时序闭合的新干预
   数据。统一基线的25→143/223周期是每试次路径均值的统计，不能自动
   替代具名 hit 请求的受控干预。outline 第10、24、41行也需同样调整。
3. **G3 结论3：四档 DS 的时序资格保留。** DS0/25/50/100 的四个不同
   程序文件 SHA 均接通对应正裕量报告。它仍是额外 DS 延迟敏感性，不能
   因通过时序审计而升格为远端内存/CXL 的实测或纯 PTW 费用证明。
4. **G3 结论4：没有发现最终 route-opt 时序失败，但“历史板测时序合格”
   需要加载身份限定。** isolated22/gap18 对应的最终构建为+0.022ns，
   不能用其优化前−0.219ns否决；然而旧加载没有 SHA，也未找到历史交付
   期望 SHA。12.7%、11.4%及供给间隔趋势可保留为带身份缺口的历史观察，
   暂不作为本次严格按 SHA 放行的主定量证据。G3第15、26、30行的候选
   头条继续等待统一 DS0 基座 E6；本次不执行它。行为 DDR 的排队解释
   仍是仿真，不能由本时序审计补成实板根因。
5. **G3 结论5：拆开处理分区与 P4M。** clean/isolated 最终构建分别为
   +0.169/+0.022ns，历史加载身份缺口与结论4相同，分区0.1%/21.5%不能
   无限定标为已完成 SHA 审核。P4M 的38%–57%及31%–48%按交付期望关联到负裕量
   构建，不能进入论文正式定量主张；G3原有“审计前不作定量主张”应更新
   为“该历史构建时序失败，仅历史观察，待合格统一基座复测”。
6. **G3 结论6：不属于 BIT 时序审计。** 28组失效实验是 RTL/行为 DDR
   仿真，不能写成已通过 FPGA 时序签核；本次也没有复核其科学结果。

outline 图2/3/5的 batch、统一基线/path、E2可保留各自条件内量化；图4
不能以 E3 v6 作时序签核后的定量因果证据；图6需移除 P4M 正式百分比，
并对 composition 标注历史身份限制。E1/E4、research84/fullstats21、
method462/88、N各版不能因为名称相近就互相承接时序资格。

## 处理顺序与边界

fullstats21 的分组时延/最大值只保留为未闭合时序版本的历史观察，不能
用已离线验收替代实现时序验收。research84 的独立旧BIT则通过时序，仍
只可报告已接受子集；method462/88 的后继优化通过，原四格负结果不因
优化前报告失败而被抹除。N v3–v5的时序资格也不证明fast通路实际行使
或排除缓冲/跨启动混杂。

- **保留并替代主叙事的基础量化：** 已接通程序 SHA 的统一 baseline100
  与 DS 四点曲线，可替代部分老版本的总慢化/分段观测；不能替代 E3
  的具名逐请求干预，也不能替代缓解机制的新基座效果。
- **历史观察：** E3 v4–v6、时序失败的 P4M，以及其他表中失败/未绑定的
  项，保留原数据、报告、负结果和版本标签，不推定数值全错，不回填
  新的历史哈希，不通过增加重复次数消除时序缺陷。
- **补证或统一基座重测：** composition 优先寻找当年保留的交付/hash
  回执；找不到就维持身份未知。若论文仍依赖其主定量结论，交由板测线
  在时序合格且有加载文件身份的统一基座上复测。E3/P4M同理。这里只
  列建议，不构建、不触板、不恢复机制探索或旧调度。
- **不误撤后继：** method 的后继清单绑定的布线后优化 BIT 与原失败 BIT 分开；
  research84 也与后来的 fullstats 版本分开。原版负报告不能覆盖另一
  SHA 的后继正报告，正报告也不能替失败旧版背书。

## 逐项证据入口

本节路径相对仓库根；Mac路径前缀为 `/Users/jojo/Desktop/在mac链接pfga(codex)/`。报告日期是原报告头原文（未记录时区），不以mtime代替。报告SHA是本次所见字节；仅当构建清单保存了同值时，才同时具有历史清单绑定。下列链接指向原件，不复制日志进Git。

<a id="e01"></a>

### E01：E1

- 最终报告：[iommu-platform/out/migration-4dma-scfix/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-scfix/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Wed Sep 16 00:54:02 2026`，阶段 `final routed (not another BIT or role2 optimized report)`。报告SHA256：`01007899d3a79a99eedd212f811159eaaaa67d2ed721bc11a34ebdf94d263149`。
- 构建/最终report→DCP→BIT→导出：[iommu-platform/out/manifests/fpga-migration-4dma-scfix.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-scfix.json#L2)（:2,29）；[iommu-platform/out/board-bringup-20260915-mac/fpga-build.log](../../../iommu-platform/out/board-bringup-20260915-mac/fpga-build.log#L19425)（:19425,19440,19442,19457,19491,19788,19840）。
- 加载与采集身份：`Mac:results/jtag/20260916-024539-program-sdrf6hn1/status.json:2,12,14,15,22,25,27`；`Mac:results/experiments/20260916-032650-run-rbo67pvd/run.json:2,3,4,5,7,8,17,19,34`。P：程序文件SHA；无配置回读SHA。

<a id="e02"></a>

### E02：E4

- 最终报告：[iommu-platform/out/migration-4dma-observe/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-observe/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Wed Sep 16 04:46:38 2026`，阶段 `final routed (not another BIT or role2 optimized report)`。报告SHA256：`31f3953decde234cec9736731c2d0f7bffcefd1734223533a51284a2d46ef35f`。
- 构建/最终report→DCP→BIT→导出：[iommu-platform/out/manifests/fpga-migration-4dma-observe.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-observe.json#L2)（:2,29）；[iommu-platform/out/e4-observer-20260916/fpga-resume.log](../../../iommu-platform/out/e4-observer-20260916/fpga-resume.log#L18426)（:18426,18441,18443,18458,18492,18789,18841）。
- 加载与采集身份：`Mac:results/jtag/20260916-094008-program-onemjmah/status.json:2,12,14,15,16,17,24,27,29`；`Mac:results/experiments/20260916-094001-run-kzi_s0zc/run.json:2,3,4,5,7,8,9,10,19,21,23,24,39`。E：期望SHA，**未在板上核验**；无配置回读SHA。

<a id="e03"></a>

### E03：batch 主矩阵60

- 最终报告：[iommu-platform/out/migration-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Wed Sep 16 11:04:56 2026`，阶段 `final routed (not another BIT or role2 optimized report)`。报告SHA256：`1cf41c3d122dd56b9b719cfeaede43dab77197f5991b02206d6341adfec7c0a4`。
- 构建/最终report→DCP→BIT→导出：[iommu-platform/out/manifests/fpga-migration-4dma-batch.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-batch.json#L2)（:2,29）；[iommu-platform/out/e4-batch-20260916/fpga-build.log](../../../iommu-platform/out/e4-batch-20260916/fpga-build.log#L19473)（:19473,19488,19490,19505,19539,19836,19888）。
- 加载与采集身份：`Mac:results/jtag/20260916-111636-program-kfkpxjq3/status.json:2,12,14,15,16,17,24,27,29`；`Mac:results/experiments/20260916-112954-run-9sa69rhc/run.json:2,3,4,5,7,8,9,10,22,23,40`。E：期望SHA，**未在板上核验**；无配置回读SHA。

<a id="e04"></a>

### E04：batch 扩展30

- 最终报告：[iommu-platform/out/migration-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Wed Sep 16 11:04:56 2026`，阶段 `final routed (not another BIT or role2 optimized report)`。报告SHA256：`1cf41c3d122dd56b9b719cfeaede43dab77197f5991b02206d6341adfec7c0a4`。
- 构建/最终report→DCP→BIT→导出：[iommu-platform/out/manifests/fpga-migration-4dma-batch.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-batch.json#L2)（:2,29）；[iommu-platform/out/e4-batch-20260916/fpga-build.log](../../../iommu-platform/out/e4-batch-20260916/fpga-build.log#L19473)（:19473,19488,19490,19505,19539,19836,19888）。
- 加载与采集身份：`Mac:results/jtag/20260916-111636-program-kfkpxjq3/status.json:2,12,14,15,16,17,24,27,29`；`Mac:results/experiments/20260916-114036-run-8pjv_djc/run.json:2,3,4,5,7,8,9,10,22,23,39`。E：期望SHA，**未在板上核验**；无配置回读SHA。

<a id="e05"></a>

### E05：历史 DS0

- 最终报告：[iommu-platform/out/baseline-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/baseline-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Wed Sep 16 13:04:52 2026`，阶段 `final routed (not another BIT or role2 optimized report)`。报告SHA256：`16fea4eb82ac71f08c8559b34ab2f5c49eecf7dd7296af436abe67acbcbd1d74`。
- 构建/最终report→DCP→BIT→导出：[iommu-platform/out/manifests/fpga-baseline-4dma-batch.json](../../../iommu-platform/out/manifests/fpga-baseline-4dma-batch.json#L2)（:2,29）；[iommu-platform/out/e4-ds0-batch-20260916-fpga.log](../../../iommu-platform/out/e4-ds0-batch-20260916-fpga.log#L19422)（:19422,19437,19439,19454,19488,19785,19837）。
- 加载与采集身份：`Mac:results/jtag/20260916-132252-program-0lt8xim7/status.json:2,12,14,15,16,17,24,27,29`；`Mac:results/experiments/20260916-132332-run-ebgxhirr/run.json:2,3,4,5,7,8,9,10,22,23,27,28`。E：期望SHA，**未在板上核验**；无配置回读SHA。

<a id="e06"></a>

### E06：历史 DS50 回切

- 最终报告：[iommu-platform/out/migration-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Wed Sep 16 11:04:56 2026`，阶段 `final routed (not another BIT or role2 optimized report)`。报告SHA256：`1cf41c3d122dd56b9b719cfeaede43dab77197f5991b02206d6341adfec7c0a4`。
- 构建/最终report→DCP→BIT→导出：[iommu-platform/out/manifests/fpga-migration-4dma-batch.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-batch.json#L2)（:2,29）；[iommu-platform/out/e4-batch-20260916/fpga-build.log](../../../iommu-platform/out/e4-batch-20260916/fpga-build.log#L19473)（:19473,19488,19490,19505,19539,19836,19888）。
- 加载与采集身份：`Mac:results/jtag/20260916-135658-program-fy_gygjt/status.json:2,12,14,15,16,17,24,27,29`；`Mac:results/experiments/20260916-135544-run-eawziww2/run.json:2,3,4,5,7,8,9,10,22,30`。E：期望SHA，**未在板上核验**；无配置回读SHA。
- 原135735-status在编程进行时exit1/configuration=null；后续141129-status才成功。两者分别保留：`Mac:results/jtag/20260916-135735-status-zo78fyl6/status.json:2,13,14,16,17,18`；`Mac:results/jtag/20260916-141129-status-ikb5vhvx/status.json:2,13,14,16,18,19,20,21,22,23,24,26,27,28`；不能用早期失败快照充当成功验收。

<a id="e07"></a>

### E07：path v3 正式30

- 最终报告：[iommu-platform/out/migration-4dma-path/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-path/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Wed Sep 16 16:15:08 2026`，阶段 `final routed (not another BIT or role2 optimized report)`。报告SHA256：`61b447320dae6aa892ab8af275db78b15bf700047a526380a6c43453e5b40009`。
- 构建/最终report→DCP→BIT→导出：[iommu-platform/out/manifests/fpga-migration-4dma-path.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-path.json#L2)（:2,29）；[iommu-platform/out/e4-path-20260916/fpga.log](../../../iommu-platform/out/e4-path-20260916/fpga.log#L18506)（:18506,18521,18523,18538,18572,18869,18921）。
- 加载与采集身份：`Mac:results/jtag/20260916-164517-program-2dck17yw/status.json:2,9,12,14,15,16,17,24,27,29`；`Mac:results/experiments/20260916-164633-run-vvguros8/run.json:2,3,4,5,7,8,9,10,22,27,28`。E：期望SHA，**未在板上核验**；无配置回读SHA。

<a id="e08"></a>

### E08：E3 v4

- 最终报告：[iommu-platform/out/migration-4dma-e3/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-e3/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,160）；日期 `Wed Sep 16 18:23:53 2026`，阶段 `final routed (not another BIT or role2 optimized report)`。报告SHA256：`7bf198bfc389d5e14778831a09de8919b2b1a4ee9a61916fef375caaa2db2734`。
- 构建/最终report→DCP→BIT→导出：[iommu-platform/out/manifests/fpga-migration-4dma-e3.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-e3.json#L2)（:2,29）；[iommu-platform/out/e3-service-20260916/fpga.log](../../../iommu-platform/out/e3-service-20260916/fpga.log#L20652)（:20652,20655,20668,20670,20685,20719,21014,21067）。
- 加载与采集身份：`Mac:results/jtag/20260917-222806-program-qsdv5dxv/status.json:2,12,14,15,16,17,24,27,29`；`Mac:results/experiments/20260917-222837-run-p0lbvbv7/run.json:2,3,4,5,7,8,9,10,22,29,30`。E：期望SHA，**未在板上核验**；无配置回读SHA。

<a id="e09"></a>

### E09：E3 v5

- 最终报告：[iommu-platform/out/migration-4dma-e3-queue/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-e3-queue/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,160）；日期 `Fri Sep 18 09:52:59 2026`，阶段 `final routed (not another BIT or role2 optimized report)`。报告SHA256：`7f1d890a02c501c39518b8dffced7bc820d1ae6363e27fb380c4dfbc80801ab6`。
- 构建/最终report→DCP→BIT→导出：[iommu-platform/out/manifests/fpga-migration-4dma-e3-queue.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-e3-queue.json#L2)（:2,29）；[iommu-platform/out/e3-followup-20260918/fpga.log](../../../iommu-platform/out/e3-followup-20260918/fpga.log#L21831)（:21831,21834,21847,21849,21864,21898,22193,22246）。
- 加载与采集身份：`Mac:results/jtag/20260918-130413-program-vhjdluu7/status.json:2,12,14,15,16,17,24,27,29`；`Mac:results/experiments/20260918-130545-run-d01lc9wp/run.json:2,3,4,5,7,8,9,10,22,41,43,44,45`。E：期望SHA，**未在板上核验**；无配置回读SHA。

<a id="e10"></a>

### E10：E3 v6

- 最终报告：[iommu-platform/out/migration-4dma-e3-controlled/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-e3-controlled/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,160）；日期 `Fri Sep 18 15:20:47 2026`，阶段 `final routed (not another BIT or role2 optimized report)`。报告SHA256：`def11159e4ac13d8322990934bb0bb480aa6c2112e9ee76fe90da1d74aec594f`。
- 构建/最终report→DCP→BIT→导出：[iommu-platform/out/manifests/fpga-migration-4dma-e3-controlled.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-e3-controlled.json#L2)（:2,29）；[iommu-platform/out/e3-controlled-20260918/fpga.log](../../../iommu-platform/out/e3-controlled-20260918/fpga.log#L20324)（:20324,20327,20340,20342,20357,20391,20686,20739）。
- 加载与采集身份：`Mac:results/jtag/20260918-164548-program-l38he0k6/status.json:2,12,14,15,16,17,24,27,29`；`Mac:results/experiments/20260918-164635-run-rk48bhte/run.json:2,3,4,5,7,8,9,10,22,28,29,32,35,40,43`。E：期望SHA，**未在板上核验**；无配置回读SHA。

<a id="e11"></a>

### E11：E2 role1 新版64

- 最终报告：[iommu-platform/out/migration-4dma-research-e2-v2/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-research-e2-v2/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sat Oct  3 16:04:42 2026`，阶段 `final routed (not another BIT or role2 optimized report)`。报告SHA256：`0b64b6ed9ed5eb507d3843074f9a5afd7d04ff8f3f46d11152e308e3a3e6a04e`。
- 构建/最终report→DCP→BIT→导出：[iommu-platform/out/manifests/fpga-migration-4dma-research-e2-v2.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-research-e2-v2.json#L2)（:2,31）；[iommu-platform/out/two-bit-20261003/migration-4dma-research-e2-v2.retry-portfix.log](../../../iommu-platform/out/two-bit-20261003/migration-4dma-research-e2-v2.retry-portfix.log#L19005)（:19005,19020,19022,19037,19072,19367,19420）。
- 加载与采集身份：`Mac:results/experiments/20261003-research-two-bit-continued/e2/flash.json:8,10`；[iommu-platform/out/mac-research-two-bit-20261003/e2/runs/e2-boundary-w7-reverse-n112-r3/metadata.json](../../../iommu-platform/out/mac-research-two-bit-20261003/e2/runs/e2-boundary-w7-reverse-n112-r3/metadata.json#L22)（:22,23,26,28,29,31,32,35,36,37）。E：期望SHA，**未在板上核验**；无配置回读SHA。
- role1交付/报告绑定：[iommu-platform/out/mac-research-two-bit-20261003/DELIVERY.json](../../../iommu-platform/out/mac-research-two-bit-20261003/DELIVERY.json#L3)（:3,4,5,6,7,8,9,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187）；不要用role2的postroute替代。

<a id="e12"></a>

### E12：P4M 短测4

- 最终报告：[iommu-platform/out/migration-4dma-p4m/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-p4m/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,160）；日期 `Sun Sep 20 17:40:57 2026`，阶段 `final_routed`。报告SHA256：`8b15a36ea959164869a3c855a886f8d8584facc150a27b323bb775d07a17fbbe`。
- 交付、加载及构建绑定：[iommu-platform/out/mac-p4m-short-20260920-180713-run-l7cin11d/boot-run.json](../../../iommu-platform/out/mac-p4m-short-20260920-180713-run-l7cin11d/boot-run.json#L7)（:7,8,9）；[iommu-platform/out/p4m-baseline-20260920/fpga.log](../../../iommu-platform/out/p4m-baseline-20260920/fpga.log#L20056)（:20056,20059,20072,20074,20089,20123,20414,20418,20420,20423,20465,20470,20471,20527）；[iommu-platform/out/manifests/fpga-migration-4dma-p4m.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-p4m.json#L2)（:2,29）。E：期望SHA，**未在板上核验**；无配置回读SHA。
- 另存单条worst报告：[iommu-platform/out/migration-4dma-p4m/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt](../../../iommu-platform/out/migration-4dma-p4m/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt#L4)（:4,15,16,18,22）；头日期 `Sun Sep 20 17:44:13 2026`，slack `-10.091ns`。这是同一构建的单条路径报告，不是新优化产物；全局资格仍以上述summary为准。
- 使用限制：最终同BIT报告setup未闭合；保留历史功能/探索记录，不作50MHz时序合格定量证据。

<a id="e13"></a>

### E13：P4M pilot21

- 最终报告：[iommu-platform/out/migration-4dma-p4m/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-p4m/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,160）；日期 `Sun Sep 20 17:40:57 2026`，阶段 `final_routed`。报告SHA256：`8b15a36ea959164869a3c855a886f8d8584facc150a27b323bb775d07a17fbbe`。
- 交付、加载及构建绑定：[iommu-platform/out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/boot-run.json](../../../iommu-platform/out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/boot-run.json#L7)（:7,8,9）；[iommu-platform/out/p4m-baseline-20260920/fpga.log](../../../iommu-platform/out/p4m-baseline-20260920/fpga.log#L20056)（:20056,20059,20072,20074,20089,20123,20414,20418,20420,20423,20465,20470,20471,20527）；[iommu-platform/out/manifests/fpga-migration-4dma-p4m.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-p4m.json#L2)（:2,29）。E：期望SHA，**未在板上核验**；无配置回读SHA。
- 另存单条worst报告：[iommu-platform/out/migration-4dma-p4m/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt](../../../iommu-platform/out/migration-4dma-p4m/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt#L4)（:4,15,16,18,22）；头日期 `Sun Sep 20 17:44:13 2026`，slack `-10.091ns`。这是同一构建的单条路径报告，不是新优化产物；全局资格仍以上述summary为准。
- 使用限制：最终同BIT报告setup未闭合；保留历史功能/探索记录，不作50MHz时序合格定量证据。

<a id="e14"></a>

### E14：P4M formal60：25+6+29

- 最终报告：[iommu-platform/out/migration-4dma-p4m/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-p4m/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,160）；日期 `Sun Sep 20 17:40:57 2026`，阶段 `final_routed`。报告SHA256：`8b15a36ea959164869a3c855a886f8d8584facc150a27b323bb775d07a17fbbe`。
- 交付、加载及构建绑定：[iommu-platform/out/mac-p4m-formal60-20260920-215406-run-cicggil4/BIT-delivery.json](../../../iommu-platform/out/mac-p4m-formal60-20260920-215406-run-cicggil4/BIT-delivery.json#L11)（:11）；[iommu-platform/out/mac-p4m-remaining29-20261002/boot/jtag-status.json](../../../iommu-platform/out/mac-p4m-remaining29-20261002/boot/jtag-status.json#L14)（:14,16,27）；[iommu-platform/out/p4m-baseline-20260920/fpga.log](../../../iommu-platform/out/p4m-baseline-20260920/fpga.log#L20056)（:20056,20059,20072,20074,20089,20123,20414,20418,20420,20423,20465,20470,20471,20527）；[iommu-platform/out/manifests/fpga-migration-4dma-p4m.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-p4m.json#L2)（:2,29）。E：期望SHA，**未在板上核验**；无配置回读SHA。
- 另存单条worst报告：[iommu-platform/out/migration-4dma-p4m/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt](../../../iommu-platform/out/migration-4dma-p4m/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt#L4)（:4,15,16,18,22）；头日期 `Sun Sep 20 17:44:13 2026`，slack `-10.091ns`。这是同一构建的单条路径报告，不是新优化产物；全局资格仍以上述summary为准。
- 使用限制：图6历史P4M增益不能作为时序合格定量主证据；保留60项与实际恢复顺序，附未闭时序和加载SHA缺口。

<a id="e15"></a>

### E15：research84

- 最终报告：[iommu-platform/out/migration-4dma-research-v1/fpga-src-before-fullstats-20261002/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-research-v1/fpga-src-before-fullstats-20261002/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Fri Oct  2 05:01:50 2026`，阶段 `final_routed`。报告SHA256：`72b2b4e068d5587ac8721fb6ead931d6ea9603d3778feee56d5d5f92c006d582`。
- 交付、加载及构建绑定：[iommu-platform/out/mac-research-v1-20261002/DELIVERY.json](../../../iommu-platform/out/mac-research-v1-20261002/DELIVERY.json#L29)（:29）；[iommu-platform/out/mac-research-v1-20261002/README.md](../../../iommu-platform/out/mac-research-v1-20261002/README.md#L3)（:3,10）；[iommu-platform/out/next-experiment-20261002/delivery/fpga-build-manifest.json](../../../iommu-platform/out/next-experiment-20261002/delivery/fpga-build-manifest.json#L2)（:2,30）。E：期望SHA，**未在板上核验**；无配置回读SHA。
- 另存单条worst报告：[iommu-platform/out/migration-4dma-research-v1/fpga-src-before-fullstats-20261002/cva6/corev_apu/fpga/reports/ariane.timing.rpt](../../../iommu-platform/out/migration-4dma-research-v1/fpga-src-before-fullstats-20261002/cva6/corev_apu/fpga/reports/ariane.timing.rpt#L4)（:4,15,16,18,22）；头日期 `Fri Oct  2 05:05:03 2026`，slack `+0.177ns`。这是同一构建的单条路径报告，不是新优化产物；全局资格仍以上述summary为准。
- 使用限制：该旧BIT时序通过；可有界描述成功子集，但保留80成功/2失败/2未运行及加载输入SHA未核验，不能称84全过。

<a id="e16"></a>

### E16：fullstats21

- 最终报告：[iommu-platform/out/migration-4dma-research-v1/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-research-v1/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Fri Oct  2 12:41:52 2026`，阶段 `final_routed`。报告SHA256：`26b28a9feaff6717a184babfb60b8a671f7258ef7901553d88d96be69aede963`。
- 交付、加载及构建绑定：[iommu-platform/out/mac-research-fullstats-20261003/DELIVERY.json](../../../iommu-platform/out/mac-research-fullstats-20261003/DELIVERY.json#L137)（:137）；[iommu-platform/out/mac-research-fullstats-20261003/README.md](../../../iommu-platform/out/mac-research-fullstats-20261003/README.md#L5)（:5）；[iommu-platform/out/research-fullstats-20261002/delivery/fpga-build-manifest.json](../../../iommu-platform/out/research-fullstats-20261002/delivery/fpga-build-manifest.json#L2)（:2,30）。E：期望SHA，**未在板上核验**；无配置回读SHA。
- 另存单条worst报告：[iommu-platform/out/migration-4dma-research-v1/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt](../../../iommu-platform/out/migration-4dma-research-v1/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt#L4)（:4,15,16,18,22）；头日期 `Fri Oct  2 12:45:01 2026`，slack `-0.944ns`。这是同一构建的单条路径报告，不是新优化产物；全局资格仍以上述summary为准。
- 使用限制：最终fullstats BIT的setup未闭合，最差路径直达cost-groups观测寄存器；不作时序合格分组时延/最大值主证据，保留历史。

<a id="e17"></a>

### E17：method462

- 最终报告：[iommu-platform/out/two-bit-20261003/method-direct-arm-postroute/timing.rpt](../../../iommu-platform/out/two-bit-20261003/method-direct-arm-postroute/timing.rpt#L4)（:4,10,159）；日期 `Sat Oct  3 18:31:26 2026`，阶段 `final_postroute_optimization`。报告SHA256：`d324ca603d14bedc8cada45f8c4f1c96baad5f967d58919735629c0e485fe6de`。
- 交付、加载及构建绑定：[iommu-platform/out/two-bit-20261003/method-direct-arm-postroute/manifest.json](../../../iommu-platform/out/two-bit-20261003/method-direct-arm-postroute/manifest.json#L4)（:4,23,70,83,98）；[iommu-platform/out/mac-research-two-bit-20261003/DELIVERY.json](../../../iommu-platform/out/mac-research-two-bit-20261003/DELIVERY.json#L14)（:14）；[iommu-platform/out/mac-research-two-bit-20261003/method/flash.json](../../../iommu-platform/out/mac-research-two-bit-20261003/method/flash.json#L2)（:2,10）。E：期望SHA，**未在板上核验**；无配置回读SHA。
- 使用限制：应使用通过时序的direct-arm postroute版本；可作该配置描述性定量结果，保留实际load SHA未记录、单boot与匹配控制限制。

<a id="e18"></a>

### E18：method88：两boot

- 最终报告：[iommu-platform/out/two-bit-20261003/method-direct-arm-postroute/timing.rpt](../../../iommu-platform/out/two-bit-20261003/method-direct-arm-postroute/timing.rpt#L4)（:4,10,159）；日期 `Sat Oct  3 18:31:26 2026`，阶段 `final_postroute_optimization`。报告SHA256：`d324ca603d14bedc8cada45f8c4f1c96baad5f967d58919735629c0e485fe6de`。
- 交付、加载及构建绑定：[iommu-platform/out/two-bit-20261003/method-direct-arm-postroute/manifest.json](../../../iommu-platform/out/two-bit-20261003/method-direct-arm-postroute/manifest.json#L4)（:4,23,70,83,98）；[iommu-platform/out/method88-20261004/delivery/DELIVERY.json](../../../iommu-platform/out/method88-20261004/delivery/DELIVERY.json#L12)（:12）；[iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/method/boots/1.json](../../../iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/method/boots/1.json#L4)（:4）；[iommu-platform/out/mac-research-two-bit-20261003/method/boot.json](../../../iommu-platform/out/mac-research-two-bit-20261003/method/boot.json#L4)（:4）；[iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/method/boots/2.flash.json](../../../iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/method/boots/2.flash.json#L2)（:2,10）；[iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/method/boots/2.json](../../../iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/method/boots/2.json#L4)（:4）。E：期望SHA，**未在板上核验**；无配置回读SHA。
- 使用限制：通过postroute；boot1与462同boot可直接衔接，boot2有同role2路径编程rc0而无输入SHA。可报告两boot四格负结论，不能按基础-.150否决。

<a id="e19"></a>

### E19：N v3 S

- 最终报告：[iommu-platform/out/n-prototype-20261004/v3-accepted-builds/migration-4dma-n-serial/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/n-prototype-20261004/v3-accepted-builds/migration-4dma-n-serial/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sun Oct  4 11:13:33 2026`，阶段 `final_routed`。报告SHA256：`da714aa0c0f6fdfa72c3d062a18b29446e6d26c7c15a6b34b8bc9b1b04995f7c`。
- 交付、加载及构建绑定：[iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-serial-v3-01/flash.json](../../../iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-serial-v3-01/flash.json#L3)（:3,13,14）；[iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-serial-v3-01/manifest.json](../../../iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-serial-v3-01/manifest.json#L471)（:471）；[iommu-platform/out/n-prototype-20261004/serial-v3-build-acceptance.json](../../../iommu-platform/out/n-prototype-20261004/serial-v3-build-acceptance.json#L19)（:19）。P：程序文件SHA；无配置回读SHA。
- 另存单条worst报告：[iommu-platform/out/n-prototype-20261004/v3-accepted-builds/migration-4dma-n-serial/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt](../../../iommu-platform/out/n-prototype-20261004/v3-accepted-builds/migration-4dma-n-serial/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt#L4)（:4,15,16,18,22）；头日期 `Sun Oct  4 10:57:12 2026`，slack `+0.201ns`。这是同一构建的单条路径报告，不是新优化产物；全局资格仍以上述summary为准。
- 使用限制：该BIT在现有约束下setup/hold通过且加载输入SHA已记录；v3/v4 fast0，不据此证明绕过收益；只描述该版实测，完整IO/CDC仍未签核。

<a id="e20"></a>

### E20：N v3 N

- 最终报告：[iommu-platform/out/n-prototype-20261004/v3-accepted-builds/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/n-prototype-20261004/v3-accepted-builds/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sun Oct  4 11:12:20 2026`，阶段 `final_routed`。报告SHA256：`1f0d9f429aa76d27a8247a27244596ea311a40c8a3b831b965a80d01aecd459c`。
- 交付、加载及构建绑定：[iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-hit-v3-01/flash.json](../../../iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-hit-v3-01/flash.json#L3)（:3,13,14）；[iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-hit-v3-01/manifest.json](../../../iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-hit-v3-01/manifest.json#L471)（:471）；[iommu-platform/out/n-prototype-20261004/hit-v3-build-acceptance.json](../../../iommu-platform/out/n-prototype-20261004/hit-v3-build-acceptance.json#L19)（:19）。P：程序文件SHA；无配置回读SHA。
- 另存单条worst报告：[iommu-platform/out/n-prototype-20261004/v3-accepted-builds/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt](../../../iommu-platform/out/n-prototype-20261004/v3-accepted-builds/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt#L4)（:4,15,16,18,22）；头日期 `Sun Oct  4 11:15:57 2026`，slack `+0.145ns`。这是同一构建的单条路径报告，不是新优化产物；全局资格仍以上述summary为准。
- 使用限制：该BIT在现有约束下setup/hold通过且加载输入SHA已记录；v3/v4 fast0，不据此证明绕过收益；只描述该版实测，完整IO/CDC仍未签核。

<a id="e21"></a>

### E21：N v4 S

- 最终报告：[iommu-platform/out/n-prototype-20261004/v4-accepted-builds/migration-4dma-n-serial/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/n-prototype-20261004/v4-accepted-builds/migration-4dma-n-serial/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sun Oct  4 13:18:54 2026`，阶段 `final_routed`。报告SHA256：`768e0a0199e74fe5eff845e84c8cdc84de98f94e2ce1db1c0566b311922a35af`。
- 交付、加载及构建绑定：[iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-serial-v4-01/flash.json](../../../iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-serial-v4-01/flash.json#L3)（:3,13,14）；[iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-serial-v4-01/manifest.json](../../../iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-serial-v4-01/manifest.json#L471)（:471）；[iommu-platform/out/n-prototype-20261004/serial-v4-build-acceptance.json](../../../iommu-platform/out/n-prototype-20261004/serial-v4-build-acceptance.json#L22)（:22）。P：程序文件SHA；无配置回读SHA。
- 另存单条worst报告：[iommu-platform/out/n-prototype-20261004/v4-accepted-builds/migration-4dma-n-serial/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt](../../../iommu-platform/out/n-prototype-20261004/v4-accepted-builds/migration-4dma-n-serial/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt#L4)（:4,15,16,18,22）；头日期 `Sun Oct  4 13:22:19 2026`，slack `+0.124ns`。这是同一构建的单条路径报告，不是新优化产物；全局资格仍以上述summary为准。
- 使用限制：该BIT在现有约束下setup/hold通过且加载输入SHA已记录；v3/v4 fast0，不据此证明绕过收益；只描述该版实测，完整IO/CDC仍未签核。

<a id="e22"></a>

### E22：N v4 N

- 最终报告：[iommu-platform/out/n-prototype-20261004/v4-accepted-builds/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/n-prototype-20261004/v4-accepted-builds/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sun Oct  4 13:24:16 2026`，阶段 `final_routed`。报告SHA256：`19442662e5a00646292d1ea7348196cd30adbe4d864c0eccc28557c5aaa60aa0`。
- 交付、加载及构建绑定：[iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-hit-v4-01/flash.json](../../../iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-hit-v4-01/flash.json#L3)（:3,13,14）；[iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-hit-v4-01/manifest.json](../../../iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-hit-v4-01/manifest.json#L471)（:471）；[iommu-platform/out/n-prototype-20261004/hit-v4-build-acceptance.json](../../../iommu-platform/out/n-prototype-20261004/hit-v4-build-acceptance.json#L22)（:22）。P：程序文件SHA；无配置回读SHA。
- 另存单条worst报告：[iommu-platform/out/n-prototype-20261004/v4-accepted-builds/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt](../../../iommu-platform/out/n-prototype-20261004/v4-accepted-builds/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt#L4)（:4,15,16,18,22）；头日期 `Sun Oct  4 13:27:30 2026`，slack `+0.177ns`。这是同一构建的单条路径报告，不是新优化产物；全局资格仍以上述summary为准。
- 使用限制：该BIT在现有约束下setup/hold通过且加载输入SHA已记录；v3/v4 fast0，不据此证明绕过收益；只描述该版实测，完整IO/CDC仍未签核。

<a id="e23"></a>

### E23：N v5 S

- 最终报告：[iommu-platform/out/migration-4dma-n-serial/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-n-serial/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sun Oct  4 15:38:22 2026`，阶段 `final_routed`。报告SHA256：`99abab12f38f9fbd109c480f50e974a3fecc2c0a8ed16bfaf5e14baead3d0395`。
- 交付、加载及构建绑定：[iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-serial-v5-01/flash.json](../../../iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-serial-v5-01/flash.json#L3)（:3,13,14）；[iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-serial-v5-01/manifest.json](../../../iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-serial-v5-01/manifest.json#L471)（:471）；[iommu-platform/out/n-prototype-20261004/serial-v5-build-acceptance.json](../../../iommu-platform/out/n-prototype-20261004/serial-v5-build-acceptance.json#L19)（:19）。P：程序文件SHA；无配置回读SHA。
- 另存单条worst报告：[iommu-platform/out/migration-4dma-n-serial/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt](../../../iommu-platform/out/migration-4dma-n-serial/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt#L4)（:4,15,16,18,22）；头日期 `Sun Oct  4 15:41:57 2026`，slack `+0.177ns`。这是同一构建的单条路径报告，不是新优化产物；全局资格仍以上述summary为准。
- 使用限制：该BIT在现有约束下setup/hold通过且加载输入SHA已记录；可小样本描述当版S/N及不稳定性；跨BIT/boot、512拍对原版3拍缓冲的混杂阻止纯N归因，完整IO/CDC仍未签核。

<a id="e24"></a>

### E24：N v5 N

- 最终报告：[iommu-platform/out/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sun Oct  4 15:45:27 2026`，阶段 `final_routed`。报告SHA256：`074aa813d887aaf094ef7c350f061653807ec45a43894a827864d9841f725c7b`。
- 交付、加载及构建绑定：[iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-hit-v5-01/flash.json](../../../iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-hit-v5-01/flash.json#L3)（:3,13,14）；[iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-hit-v5-01/manifest.json](../../../iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-hit-v5-01/manifest.json#L471)（:471）；[iommu-platform/out/n-prototype-20261004/hit-v5-build-acceptance.json](../../../iommu-platform/out/n-prototype-20261004/hit-v5-build-acceptance.json#L19)（:19）。P：程序文件SHA；无配置回读SHA。
- 另存单条worst报告：[iommu-platform/out/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt](../../../iommu-platform/out/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.timing.rpt#L4)（:4,15,16,18,22）；头日期 `Sun Oct  4 15:48:42 2026`，slack `+0.177ns`。这是同一构建的单条路径报告，不是新优化产物；全局资格仍以上述summary为准。
- 使用限制：该BIT在现有约束下setup/hold通过且加载输入SHA已记录；可小样本描述当版S/N及不稳定性；跨BIT/boot、512拍对原版3拍缓冲的混杂阻止纯N归因，完整IO/CDC仍未签核。

<a id="e25"></a>

### E25：composition-clean172

- 最终报告：[iommu-platform/out/composition-clean-20261006/route-opt/reports/timing_summary_routed.rpt](../../../iommu-platform/out/composition-clean-20261006/route-opt/reports/timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Tue Oct  6 21:19:14 2026`，阶段 `Physopt postRoute`。报告SHA256：`bd367c009c1701867979c501d7313e6b37ae028c6ca3f5bbe6158f624411fc62`。
- 加载身份：[iommu-platform/out/composition-clean-20261006/mac-records/composition-clean-20261006-boot02/result.json](../../../iommu-platform/out/composition-clean-20261006/mac-records/composition-clean-20261006-boot02/result.json#L2)（:2,3,10,12,102,105）；[iommu-platform/out/composition-clean-20261006/mac-records/composition-clean-20261006-boot02/04-fpga-load.stdout](../../../iommu-platform/out/composition-clean-20261006/mac-records/composition-clean-20261006-boot02/04-fpga-load.stdout#L3)（:3,4,58,60）。U：历史SHA未知；无配置回读SHA。
- 构建与绑定：[iommu-platform/out/composition-clean-20261006/route-opt/optimize.tcl](../../../iommu-platform/out/composition-clean-20261006/route-opt/optimize.tcl#L19)（:19,20,21,23,140,143,153,156,157,188,206）；[iommu-platform/out/composition-clean-20261006/route-opt/result.json](../../../iommu-platform/out/composition-clean-20261006/route-opt/result.json#L3)（:3,4,5,7,8,9,13,14）；[iommu-platform/out/composition-clean-20261006-board/board-acceptance.json](../../../iommu-platform/out/composition-clean-20261006-board/board-acceptance.json#L23)（:23,26,27,30）。
- 实际优化/写BIT日志：[iommu-platform/out/composition-clean-20261006/route-opt/vivado.log](../../../iommu-platform/out/composition-clean-20261006/route-opt/vivado.log#L111)（:111,243,492,496,514）；同一优化流程先生成报告，通过gate后才写BIT。
- 今日候选文件：`iommu-platform/out/composition-clean-20261006/route-opt/ariane_xilinx.bit`，SHA256 `d65b98bc15b4c18c84da175fa430c891082af29c0bbf5f99ac0b785af209684c`。这不是历史加载哈希，也没有找到可冒充的历史期望SHA。
- 被后继替代的原生报告：[iommu-platform/out/composition-clean-20261006/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/composition-clean-20261006/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,159）；WNS/TNS/WHS `-0.131/-0.145/+0.038ns`，S/H `2/0`。只对原生BIT失败，不据此否决优化后的另一份BIT。

<a id="e26"></a>

### E26：composition-isolated22

- 最终报告：[iommu-platform/out/composition-isolated-20261007/route-opt/reports/timing_summary_routed.rpt](../../../iommu-platform/out/composition-isolated-20261007/route-opt/reports/timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Wed Oct  7 11:29:17 2026`，阶段 `Physopt postRoute`。报告SHA256：`a9366b911444e6b7407e44b8aaffa391deea7d7e55673999874f60e796f547ac`。
- 加载身份：[iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-boot01/result.json](../../../iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-boot01/result.json#L2)（:2,3,10,12,75,78）；[iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-boot01/03-fpga-load.stdout](../../../iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-boot01/03-fpga-load.stdout#L3)（:3,4,58,60）。U：历史SHA未知；无配置回读SHA。
- 构建与绑定：[iommu-platform/out/composition-isolated-20261007/route-opt/optimize.tcl](../../../iommu-platform/out/composition-isolated-20261007/route-opt/optimize.tcl#L19)（:19,20,21,23,140,143,153,156,157,188,206）；[iommu-platform/out/composition-isolated-20261007/route-opt/result.json](../../../iommu-platform/out/composition-isolated-20261007/route-opt/result.json#L3)（:3,4,5,7,8,9,13,14）；[iommu-platform/out/composition-isolated-20261007/board-acceptance.json](../../../iommu-platform/out/composition-isolated-20261007/board-acceptance.json#L6)（:6,7,16,18,19,20,24,25）。
- 实际优化/写BIT日志：[iommu-platform/out/composition-isolated-20261007/route-opt/vivado.log](../../../iommu-platform/out/composition-isolated-20261007/route-opt/vivado.log#L111)（:111,223,474,478,496）；同一优化流程先生成报告，通过gate后才写BIT。
- 今日候选文件：`iommu-platform/out/composition-isolated-20261007/route-opt/ariane_xilinx.bit`，SHA256 `f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca`。这不是历史加载哈希，也没有找到可冒充的历史期望SHA。
- 被后继替代的原生报告：[iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,159）；WNS/TNS/WHS `-0.219/-0.219/+0.051ns`，S/H `1/0`。只对原生BIT失败，不据此否决优化后的另一份BIT。

<a id="e27"></a>

### E27：composition-gap18

- 最终报告：[iommu-platform/out/composition-isolated-20261007/route-opt/reports/timing_summary_routed.rpt](../../../iommu-platform/out/composition-isolated-20261007/route-opt/reports/timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Wed Oct  7 11:29:17 2026`，阶段 `Physopt postRoute`。报告SHA256：`a9366b911444e6b7407e44b8aaffa391deea7d7e55673999874f60e796f547ac`。
- 加载身份：[iommu-platform/out/pn-board-path-20261007/board-acceptance.json](../../../iommu-platform/out/pn-board-path-20261007/board-acceptance.json#L4)（:4,8,10,12,13,15）；[iommu-platform/out/pn-board-path-20261007/mac-records/pn-gap64-20261007-formal01/boot.json](../../../iommu-platform/out/pn-board-path-20261007/mac-records/pn-gap64-20261007-formal01/boot.json#L2)（:2,3）。U：历史SHA未知；无配置回读SHA。
- 构建与绑定：[iommu-platform/out/composition-isolated-20261007/route-opt/optimize.tcl](../../../iommu-platform/out/composition-isolated-20261007/route-opt/optimize.tcl#L19)（:19,20,21,23,140,143,153,156,157,188,206）；[iommu-platform/out/composition-isolated-20261007/route-opt/result.json](../../../iommu-platform/out/composition-isolated-20261007/route-opt/result.json#L3)（:3,4,5,7,8,9,13,14）；[iommu-platform/out/composition-isolated-20261007/board-acceptance.json](../../../iommu-platform/out/composition-isolated-20261007/board-acceptance.json#L6)（:6,7,16,18,19,20,24,25）。
- 实际优化/写BIT日志：[iommu-platform/out/composition-isolated-20261007/route-opt/vivado.log](../../../iommu-platform/out/composition-isolated-20261007/route-opt/vivado.log#L111)（:111,223,474,478,496）；同一优化流程先生成报告，通过gate后才写BIT。
- 今日候选文件：`iommu-platform/out/composition-isolated-20261007/route-opt/ariane_xilinx.bit`，SHA256 `f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca`。这不是历史加载哈希，也没有找到可冒充的历史期望SHA。
- 被后继替代的原生报告：[iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,159）；WNS/TNS/WHS `-0.219/-0.219/+0.051ns`，S/H `1/0`。只对原生BIT失败，不据此否决优化后的另一份BIT。

<a id="e28"></a>

### E28：composition ptwfix172（前史）

- 最终报告：[iommu-platform/out/composition-ptwfix-20261005/route-opt/reports/timing_summary_routed.rpt](../../../iommu-platform/out/composition-ptwfix-20261005/route-opt/reports/timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Mon Oct  5 17:54:37 2026`，阶段 `Physopt postRoute`。报告SHA256：`d6a9344049e5d99706a64357c7e6da587b5d40d51369f3ae4ebdfc8a7f150764`。
- 加载身份：[iommu-platform/out/composition-ptwfix-20261005/mac-records/results/composition-ptwfix-20261005-boot01/result.json](../../../iommu-platform/out/composition-ptwfix-20261005/mac-records/results/composition-ptwfix-20261005-boot01/result.json#L2)（:2,3,10,12,75,78）；[iommu-platform/out/composition-ptwfix-20261005/mac-records/results/composition-ptwfix-20261005-boot01/03-fpga-load.stdout](../../../iommu-platform/out/composition-ptwfix-20261005/mac-records/results/composition-ptwfix-20261005-boot01/03-fpga-load.stdout#L3)（:3,4,58,60）。U：历史SHA未知；无配置回读SHA。
- 构建与绑定：[iommu-platform/out/composition-ptwfix-20261005/route-opt/optimize.tcl](../../../iommu-platform/out/composition-ptwfix-20261005/route-opt/optimize.tcl#L19)（:19,20,21,23,140,143,153,156,157,188,206）；[iommu-platform/out/composition-ptwfix-20261005/route-opt/result.json](../../../iommu-platform/out/composition-ptwfix-20261005/route-opt/result.json#L3)（:3,4,5,7,8,9,13,14）；[iommu-platform/out/composition-ptwfix-20261005/board-acceptance.json](../../../iommu-platform/out/composition-ptwfix-20261005/board-acceptance.json#L3)（:3,5,6,22,23,27,29）。
- 实际优化/写BIT日志：[iommu-platform/out/composition-ptwfix-20261005/route-opt/vivado.log](../../../iommu-platform/out/composition-ptwfix-20261005/route-opt/vivado.log#L111)（:111,302,553,557,575）；同一优化流程先生成报告，通过gate后才写BIT。
- 今日候选文件：`iommu-platform/out/composition-ptwfix-20261005/route-opt/ariane_xilinx.bit`，SHA256 `1c709458c33a1e0e04fc5745f627f71780c1208c98223094c14da27e3a4228d3`。这不是历史加载哈希，也没有找到可冒充的历史期望SHA。
- 被后继替代的原生报告：[iommu-platform/out/composition-ptwfix-20261005/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/composition-ptwfix-20261005/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,159）；WNS/TNS/WHS `-0.384/-5.586/+0.019ns`，S/H `56/0`。只对原生BIT失败，不据此否决优化后的另一份BIT。

<a id="e29"></a>

### E29：G1：唯一9次

- 最终报告：[iommu-platform/out/migration-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt](../../../iommu-platform/out/migration-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Wed Sep 16 11:04:56 2026`，阶段 `Routed`。报告SHA256：`1cf41c3d122dd56b9b719cfeaede43dab77197f5991b02206d6341adfec7c0a4`。
- 加载身份：[paper/ispass2027/board/runs/g1-login-20261017-01/load.json](../../../paper/ispass2027/board/runs/g1-login-20261017-01/load.json#L2)（:2,3,4,7,8,17,20,21,22,23,24）；[paper/ispass2027/board/runs/g1-load-20261017-01/program.tcl](../../../paper/ispass2027/board/runs/g1-load-20261017-01/program.tcl#L16)（:16,17）；[paper/ispass2027/board/runs/g1-load-20261017-01/program.log](../../../paper/ispass2027/board/runs/g1-load-20261017-01/program.log#L43)（:43,44,46,48）。P：程序文件SHA；无配置回读SHA。
- 构建与绑定：[iommu-platform/out/manifests/fpga-migration-4dma-batch.json](../../../iommu-platform/out/manifests/fpga-migration-4dma-batch.json#L29)（:29）；[iommu-platform/out/e4-batch-20260916/delivery/delivery.json](../../../iommu-platform/out/e4-batch-20260916/delivery/delivery.json#L30)（:30,31,32,33,35,36,38）；[iommu-platform/out/e4-batch-20260916/fpga-build.log](../../../iommu-platform/out/e4-batch-20260916/fpga-build.log#L19473)（:19473,19539,19832,19836,19940）。

- G1原wrapper曾中断，原 `g1-load-20261017-01/load.json:7` 保留失败；接受入口为 `g1-login-20261017-01/load.json:25–27` 的恢复身份记录，明确没有第二次JTAG或重启。不能把恢复当补采。

<a id="e30"></a>

### E30：baseline100 DS0 中50

- 最终报告：[paper/ispass2027/board/reports/ds0/ariane_xilinx_timing_summary_routed.rpt](../../../paper/ispass2027/board/reports/ds0/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sat Oct 17 18:59:16 2026`，阶段 `Routed`。报告SHA256：`63d0f080197fa9ca616933e7c08acb4ef4b5699699c5e53bcef6a28b45be337f`。
- 加载身份：[paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle-load/load.json](../../../paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle-load/load.json#L2)（:2,3,4,7,8,17,20,21,22,23,24）；[paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle-load/program.tcl](../../../paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle-load/program.tcl#L16)（:16,17）；[paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle-load/program.log](../../../paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle-load/program.log#L43)（:43,44,46,48）。P：程序文件SHA；无配置回读SHA。
- 构建与绑定：[paper/ispass2027/board/manifests/fpga-ds0-build.json](../../../paper/ispass2027/board/manifests/fpga-ds0-build.json#L2521)（:2521）。

<a id="e31"></a>

### E31：baseline100 DS50 前25/后25

- 最终报告：[paper/ispass2027/board/reports/ds50/ariane_xilinx_timing_summary_routed.rpt](../../../paper/ispass2027/board/reports/ds50/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sat Oct 17 18:53:51 2026`，阶段 `Routed`。报告SHA256：`efc6fd518b4d74a340d051cbdfa45db913cb8215de9a8e7d6aa29e49b553cae2`。
- 加载身份：[paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first-load/load.json](../../../paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first-load/load.json#L2)（:2,3,4,7,8,17,20,21,22,23,24）；[paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first-load/program.tcl](../../../paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first-load/program.tcl#L16)（:16,17）；[paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first-load/program.log](../../../paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first-load/program.log#L43)（:43,44,46,48）。P：程序文件SHA；无配置回读SHA。
- 构建与绑定：[paper/ispass2027/board/manifests/fpga-ds50-build.json](../../../paper/ispass2027/board/manifests/fpga-ds50-build.json#L2521)（:2521）。

- DS50后25次另有[加载回执](../board/runs/baseline100-20261017-01/ds50-last-load/load.json#L4)（:2–4,7–8,19–23），同一程序SHA `580b34014914a1bee8541ed1e23b36a164bfc0fea3a2f691f56870b44dc35e77`，新boot `b1eaa85f-01d7-4976-9dac-b57cc8af49b7`，编程exit0、程序SHA已核验、配置未回读；两个boot分别保留。

<a id="e32"></a>

### E32：DS sweep DS25 新50

- 最终报告：[paper/ispass2027/board/packages/ds-sweep-20261017/metadata/ds25/ariane_xilinx_timing_summary_routed.rpt](../../../paper/ispass2027/board/packages/ds-sweep-20261017/metadata/ds25/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sat Oct 17 21:48:25 2026`，阶段 `Routed`。报告SHA256：`51bddfbb202e984b544b81508b4e86c5e16d3ccaf3a9fcaa653c0a8d437ee898`。
- 加载身份：[paper/ispass2027/board/runs/ds-sweep-20261017-01/ds25-all-load/load.json](../../../paper/ispass2027/board/runs/ds-sweep-20261017-01/ds25-all-load/load.json#L2)（:2,3,4,7,8,17,20,21,22,23,24）；[paper/ispass2027/board/runs/ds-sweep-20261017-01/ds25-all-load/program.tcl](../../../paper/ispass2027/board/runs/ds-sweep-20261017-01/ds25-all-load/program.tcl#L16)（:16,17）；[paper/ispass2027/board/runs/ds-sweep-20261017-01/ds25-all-load/program.log](../../../paper/ispass2027/board/runs/ds-sweep-20261017-01/ds25-all-load/program.log#L43)（:43,44,46,48）。P：程序文件SHA；无配置回读SHA。
- 构建与绑定：[paper/ispass2027/board/packages/ds-sweep-20261017/metadata/ds25/build-manifest.json](../../../paper/ispass2027/board/packages/ds-sweep-20261017/metadata/ds25/build-manifest.json#L2549)（:2549）。

<a id="e33"></a>

### E33：DS sweep DS100 新50

- 最终报告：[paper/ispass2027/board/packages/ds-sweep-20261017/metadata/ds100/ariane_xilinx_timing_summary_routed.rpt](../../../paper/ispass2027/board/packages/ds-sweep-20261017/metadata/ds100/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sat Oct 17 21:48:09 2026`，阶段 `Routed`。报告SHA256：`a75acb0b6fa21f6a3e6f91faeb07cbd685c723fffbcd95b77941c8b9b13d2c2e`。
- 加载身份：[paper/ispass2027/board/runs/ds-sweep-20261017-01/ds100-all-load/load.json](../../../paper/ispass2027/board/runs/ds-sweep-20261017-01/ds100-all-load/load.json#L2)（:2,3,4,7,8,17,20,21,22,23,24）；[paper/ispass2027/board/runs/ds-sweep-20261017-01/ds100-all-load/program.tcl](../../../paper/ispass2027/board/runs/ds-sweep-20261017-01/ds100-all-load/program.tcl#L16)（:16,17）；[paper/ispass2027/board/runs/ds-sweep-20261017-01/ds100-all-load/program.log](../../../paper/ispass2027/board/runs/ds-sweep-20261017-01/ds100-all-load/program.log#L43)（:43,44,46,48）。P：程序文件SHA；无配置回读SHA。
- 构建与绑定：[paper/ispass2027/board/packages/ds-sweep-20261017/metadata/ds100/build-manifest.json](../../../paper/ispass2027/board/packages/ds-sweep-20261017/metadata/ds100/build-manifest.json#L2549)（:2549）。

<a id="e34"></a>

### E34：DS sweep DS0（复用）

- 最终报告：[paper/ispass2027/board/reports/ds0/ariane_xilinx_timing_summary_routed.rpt](../../../paper/ispass2027/board/reports/ds0/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sat Oct 17 18:59:16 2026`，阶段 `Routed`。报告SHA256：`63d0f080197fa9ca616933e7c08acb4ef4b5699699c5e53bcef6a28b45be337f`。
- 加载身份：[paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle-load/load.json](../../../paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle-load/load.json#L2)（:2,3,4,7,8,17,20,21,22,23,24）；[paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle-load/program.tcl](../../../paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle-load/program.tcl#L16)（:16,17）；[paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle-load/program.log](../../../paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle-load/program.log#L43)（:43,44,46,48）。P：程序文件SHA；无配置回读SHA。
- 构建与绑定：[paper/ispass2027/board/manifests/fpga-ds0-build.json](../../../paper/ispass2027/board/manifests/fpga-ds0-build.json#L2521)（:2521）。

<a id="e35"></a>

### E35：DS sweep DS50（复用）

- 最终报告：[paper/ispass2027/board/reports/ds50/ariane_xilinx_timing_summary_routed.rpt](../../../paper/ispass2027/board/reports/ds50/ariane_xilinx_timing_summary_routed.rpt#L4)（:4,10,159）；日期 `Sat Oct 17 18:53:51 2026`，阶段 `Routed`。报告SHA256：`efc6fd518b4d74a340d051cbdfa45db913cb8215de9a8e7d6aa29e49b553cae2`。
- 加载身份：[paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first-load/load.json](../../../paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first-load/load.json#L2)（:2,3,4,7,8,17,20,21,22,23,24）；[paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first-load/program.tcl](../../../paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first-load/program.tcl#L16)（:16,17）；[paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first-load/program.log](../../../paper/ispass2027/board/runs/baseline100-20261017-01/ds50-first-load/program.log#L43)（:43,44,46,48）。P：程序文件SHA；无配置回读SHA。
- 构建与绑定：[paper/ispass2027/board/manifests/fpga-ds50-build.json](../../../paper/ispass2027/board/manifests/fpga-ds50-build.json#L2521)（:2521）。

- DS50后25次另有[加载回执](../board/runs/baseline100-20261017-01/ds50-last-load/load.json#L4)（:2–4,7–8,19–23），同一程序SHA `580b34014914a1bee8541ed1e23b36a164bfc0fea3a2f691f56870b44dc35e77`，新boot `b1eaa85f-01d7-4976-9dac-b57cc8af49b7`，编程exit0、程序SHA已核验、配置未回读；两个boot分别保留。

## 审计输入、补充限制与检查

- G3附件：`/home/xvhaocheng/.codex/attachments/d6610f6c-1444-4276-8fb3-ca76306fe6b9/G3 论文主张与五条结论（草稿）.md`；SHA256 `5d56a3fe074055a649f839b17d0df5ccb378b81ba546bfb0d724ac92e956841c`。原文未修改、未复制入Git。第19/23–28/39/53行为上述结论映射的关键范围。
- outline为本次main快照的 `paper/ispass2027/outline.md`；SHA256 `fe5ba5ecf135626db9fae6fc42b0cfb17bf0a06c44e186d8d814ecfc0fd9384e`。读入时顶层HEAD `f8f368215e82c7d232d40ed69707325f2d916b78`；此为审计上下文，不冒充各实验执行commit。
- composition未找到期望SHA的边界已由既有 `paper/ispass2027/board/packages/history-20261017/composition.json:384–407`显式保留。isolated当前交付存档 `iommu-platform/out/composition-isolated-20261007/mac-package.tar` 的 `ariane_xilinx.bit` 成员哈希为 `f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca`，与当前优化BIT相同；这增强交付候选关联，但不补认证历史Mac加载字节。gap18与isolated22同boot `d61e7517-2b15-4702-86f2-3f338ddd5345`、JTAG0，沿用同一身份缺口。
- method优化前为WNS−0.150ns/TNS−0.179ns、2个setup端点失败；后继manifest明确只把旧报告作为来历，实际交付 `31e18e…` 与最终+0.048ns报告同时被hash绑定。method88 boot1与462相同（`93406a68-fc8a-4040-9dc6-ae4e4a670d36`），boot2为 `236b6316-cca3-49be-b52b-48f3963fa515`，只有路径/rc0而无输入SHA，仍属E类。此审计不重新开启已关闭的p5收益主张。
- fullstats21的观测统计不能作为时序闭合实现的定量依据：最差20ns域路径进入observer `cost_groups/max_comp_q` 的CE；P4M最差路径进入旧E3 observer `error_o`。不能仅因端点位于观测块就宣称违例与论文数据无关。research84是另一个早期BIT，保留80成功/2失败/2未运行，不能写84全过；N v3/v4的fast=0及v5缓冲/跨启动混杂也不因时序通过而消失。
- 本次root检查了35项表中数值、26份不同最终报告的完整SHA与全局时序行、27个存档路径的BIT SHA（26种字节身份），并核对497条本机引用的路径/行、已有摘录或保存哈希；所有表值匹配。Mac原历史元数据只读抽查，未执行JTAG/status/program命令。原数据提取器、旧validator和Vivado均未运行。
- 限定搜索未找到P4M/fullstats对应相同BIT的更晚合格route-opt；不据此声称历史绝无其他构建。对缺证项保持未知，不用当前checkout或别的角色/版本报告补齐。整个审计只新增本文，不改CSV、旧报告、outline、G3或共享状态。

# 只读来源与最小边界

下列行号针对本目录`evidence/`中的逐字节源副本。每个Git文件的完整commit、Git blob、SHA256和原仓库入口见`evidence/index.json`。
这些是设计参考，不是新实验身份；未来执行前仍按既有build/run manifest记录实际选择源码。

## 历史执行与组织提交分开

| 对象 | 当时记录 | 当前可恢复来源与限制 |
| --- | --- | --- |
| P4M | platform2fbf7aeaff73c0a9fb7ddbf00c4718165d4a16ea；CVA6 e822bc9d8adf246a5d098585fb229744bafd9cc1；IOMMU137f5e5fb1f2cc65a1346359226b16247c3f8445；Linux9f8da9e7a6a08fb19134146241018eaff0eee124 | build manifest与lock角色见原历史身份行；不能把所选lock冒充完整dirty执行树。新组织P4M功能commit是c37c7eef45909ea2fa50637454bafa26ad64064d，不回填为旧板测HEAD |
| P/N/PN clean/isolated | platform0a570deda89636631462bb188d39c058409df364；lock CVA6 a1b08934391833aed21e6ad63b545634040808ac、IOMMU238a39e66a07b27a67fb5749df40691682555027；Linuxc0f5e13ea81fa30d78f3bb1d6389e2397321e040 | 实际采用workspace overlay，isolated另有production.patch；不能只按lock构建复现。7个关键IOMMU执行副本与新P2字节一致的证据另存，不将此推广成全部旧构建闭包已证明 |
| 新组织研究源 | CVA6991074e7fca9eddd33c61d72767c3729f573dc3e；IOMMU0b7939445298a23ea3e131ffd4e003cebb15ac59 | 精确dirty源捕获/功能分层锚，不是旧实验运行时的完整commit；不得将默认mode0称原版 |

历史原manifest/overlay/production.patch与结果报告已复制到`evidence/historical/`；
原身份CSV的完整三行选摘在`historical-identity-rows.json`，保留未知checkout/dirty与证据角色。
`isolated-source-comparison.json`逐项列出7个实际旧编译源路径和新组织源SHA，全部equal=true。
该比较是现在对原字节的核验，没有新执行或重写历史。

## 精确功能锚

| 功能 | 组织commit | 必需源码/行号与语义 |
| --- | --- | --- |
| 原配置 | CVA6 8aac3fcd345af7686a55af1867783fd6b91a0069 | `baseline-config/corev_apu/tb/ariane_soc_pkg.sv:110`：8 outstanding、W32、IOTLB16、DDTC8、ATS0、MSI_BT_MRIF |
| P4M gap | CVA6 c37c7eef45909ea2fa50637454bafa26ad64064d，源自历史e822 | `p4m-organized/.../idma_reg64_frontend.sv:63`旧d0/d8；:148复位；:159 STOP；:186写宽度；:520非最后完成进GAP；:533 STOP优先到期。移植有限状态/计数，不带旧held-launch或整体旧frontend |
| P4M window | 同c37 | `p4m-organized/corev_apu/fpga/src/dma_observer.sv:144`旧448..463；:241持续lane任务跟踪；:265 `(open,close]`，冻结后不积累。不能覆盖E5 bins |
| B1两bank | IOMMU6a47d91fd19161430e1ef2a59041e2b23870f17c | `research-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:13` ID/lifecycle合同；:136参数拒绝；:158全局容量分割；:246读准入；:291额外demux tracker；:316每bank实例。需要transaction wrapper/top/pkg ABI与Bender，不能只加入一个新文件 |
| N1合格命中旁路 | IOMMU011e7b5f82e4d4fe03aec765067d399ffded4c2d | `research-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:47` probe启动；:84 DC/leaf资格；:100真实accept/touch；:140有限FSM；`rv_iommu_tl_wrap.sv:282`两tag/normal gate；:301只在另一bank PTW窗口；:346回包仲裁；另需真实DDTC/IOTLB probe/touch |
| P1分配mask | IOMMUb35d23078036e4ce6e22c728b303b68a3fe9d56f | 分配及quiet clear接口；其实现依赖N1接口，不能宣称任意cherry-pick直接可编译 |
| P2局部历史 | IOMMU0b7939445298a23ea3e131ffd4e003cebb15ac59 | `research-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:353`2/14；:379 resident-way touch所有者；:414 eligible PLRU；`rv_iommu_tl_wrap.sv:658`参数及稳定PTW DID fill mask |
| fullbuffer许可 | CVA6c7527202b8c93d751d73bf34892df3883c0454af；RAM选择f4432405308bb27a5708b7bb4b9d2116b78373a2；接线991074... | `research-cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:1`全文件只有普通页对齐4096；`.../ariane_peripherals_xilinx.sv:1269`512而非3；:1394 MSI_BT_ONLY；:1446模式接线；`research-iommu/.../rv_iommu_trans_ctl_wrap.sv:646`许可仅放宽原read admission |
| quiet模式/观测 | CVA646cfd935b2106e8e92d7d7ade2d1d222913a3c82 | `research-cva6/corev_apu/fpga/src/dma_composition_control.sv:29`活动配置端口；:77 magic/ABI；:83 S/N/P/PN值。虽含观测，mode/clear真实改变硬件，不称纯被动 |
| 旧软件 | Linux9f8da9e7a6a08fb19134146241018eaff0eee124 | `p4m-linux/drivers/misc/idma.c:1086`输入；:1114 preflight；:1127编程回读；:1142窗口/计数守恒；`include/uapi/linux/idma.h:200`旧ioctl0x0d冲突 |

上述`.../`仅为表内显示简写，完整相对路径均在index.json，原副本保持原行号。
B1/N1/P1/P2完整功能步骤与语义依赖在`iommu-selected-steps.json`；
CVA6对应5步在`cva6-selected-steps.json`。`iommu-baseline-to-research-diffstat.txt`列12个实际差异文件，
其中E3 queue/delay等不是本方法语义必需，不能整体研究树替换85b后声称最小P/N。
PTW里的旧E3 taps与pkg研究类型要逐项分离；不借移植恢复E3 active delay、旧策略/O1记录器或无关顶层端口。

## 与接受中接口的直接冲突

- E5 `accepted-e5/.../dma_observer.sv:43` index10bit；:109 INFO192；:110 hist256..767；:258 index写合法集合。
  历史448..463不是空位。
- E5 P0 `accepted-e5/.../idma_reg64_frontend.sv:97`只允许64/1024/4096、源/目的页对齐；:280 BTC2。
  `dma_core_wrap.sv:335`默认BUFFER_DEPTH=3，所需4096整页容量不能凭参数名推定存在。
- 接受BTC3参考 `accepted-btc3-reference/.../idma_reg64_frontend.sv:194` d0 columns写；:348读；:349 d8..f8保留。
  该commit为2db78c0927d6e70b819b46651df86e82ab139710，只用于冲突核查，不成为本E6基座。
- 共同Linux `accepted-common-linux/include/uapi/linux/idma_pages.h:92` CAPS0x0d/RUN0x0e；
  P0 0x09/0x0a、multi0x0b/0x0c均保留，不可覆盖旧P4M头。

## 旧结果必须保留的解释边界

P4M完整60每gap n20跨采集/boot，53在线接受、7离线追认，不能说同boot60在线通过；
启动序号环形淘汰导致的第25例原runner失败仍是失败记录，离线追认未改原run。
旧短例/pilot、整轮HPM与硬件窗口混杂不自动进入新统一基线数据。

P2隔离后A未命中数差消失，历史PN仍比P慢12.67%–12.79%（n3描述性）。
真实DMA模型曾构造下游PTE/数据服务竞争，只证明该模型可达，未解释板上全部差额；
旧一次route-opt使历史BIT通过，不授权新实现改约束/反复策略搜索。
这些原报告完整保留，新的DS0样本也不能推翻或覆盖旧DS50样本身份。

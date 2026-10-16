```json
[
  {
    "id": "activeforward2018",
    "topic": "accelerators",
    "title": "Active forwarding: Eliminate IOMMU address translation for accelerator-rich architectures",
    "authors": [
      "Hsueh-Chun Fu",
      "Po-Han Wang",
      "Chia-Lin Yang"
    ],
    "year": 2018,
    "venue": "DAC 2018 (55th Design Automation Conference)",
    "doi": "10.1145/3195970.3195984",
    "source_url": "https://scholars.lib.ntu.edu.tw/entities/publication/5b7e8d80-0aaf-4d94-947b-8a966bb6567f",
    "metadata_source": "NTU Scholars 机构题录（DOI、摘要、作者单位）；dblp 记录 conf/dac/FuWY18；DAC 2018 官方 final program（55th DAC）",
    "metadata_note": "机构题录与 dblp 元数据可核；仅机构摘要，未获 PDF，性能数字未独立验证。",
    "relevance": "core",
    "publication_status": "published",
    "reading_status": "abstract_or_indexed_excerpt",
    "fulltext_available": false,
    "notes": "与现有 accelerators 线的关系：ontable2017 讨论加速器侧页表遍历，AF 则完全绕过 IOMMU 翻译，把加速器输入数据从 CPU cache 直接前送到 scratchpad；与 cryptommu2023 的第三方加速器访问控制/隔离属不同问题。机构摘要称相对 hostPageWalk 平均提升 8%、降低加速器功耗 22.1%，但未获全文、未复现，仅作机构摘要级记录。"
  },
  {
    "id": "cdfd2026",
    "topic": "multigpu",
    "title": "Coarse-Grained Duplication First, Fine-Grained Deduplication Later: Duplication-Centric Multi-GPU Memory Management",
    "authors": [
      "Xiangyue Huang",
      "Yanan Guo",
      "Yuanchao Xu"
    ],
    "year": 2026,
    "venue": "ISCA 2026",
    "doi": "10.1109/ISCA66397.2026.00109",
    "source_url": "https://www.iscaconf.org/isca2026_dev/program/",
    "metadata_source": "ISCA 2026 官方程序（Session 7C）；dblp 记录 conf/isca/HuangGX26（DOI、pages 1465-1479）；作者主页 ddantalion.github.io 与 yananguo.com",
    "metadata_note": "题录与 DOI 可核；作者 PDF 404，未获摘要/全文，不采信聚合性能数字。",
    "relevance": "core",
    "publication_status": "published",
    "reading_status": "metadata_only",
    "fulltext_available": false,
    "notes": "与现有 shadowupdate2026（多 GPU 失效传播减少页故障）、clap2025（chiplet 局部性内存映射）、clover2026（GPU 页表副本存储）同属多 GPU UVM 数据移动/映射优化线：CDFD 关注跨 GPU 复制的粗细粒度权衡（先粗粒度复制、后细粒度去重）。证据限制：仅 ISCA 2026 官方程序、dblp 题录与作者主页；作者 PDF 404，未获全文，未采用聚合网站的性能数字。"
  },
  {
    "id": "libra2026",
    "topic": "multigpu",
    "title": "LIBRA: A High-Accuracy, Cost-Aware, and Coordinated Multi-GPU Page Prefetcher",
    "authors": [
      "Xiangyue Huang",
      "Yanan Guo",
      "Yuanchao Xu"
    ],
    "year": 2026,
    "venue": "ISCA 2026",
    "doi": "10.1109/ISCA66397.2026.00158",
    "source_url": "https://www.iscaconf.org/isca2026_dev/program/",
    "metadata_source": "ISCA 2026 官方程序（Session 9B）；dblp 记录 conf/isca/HuangGX26a（DOI、pages 2238-2253）；作者主页 ddantalion.github.io 与 yananguo.com",
    "metadata_note": "题录与 DOI 可核；作者 PDF 404，无摘要，不采信聚合性能数字。",
    "relevance": "core",
    "publication_status": "published",
    "reading_status": "metadata_only",
    "fulltext_available": false,
    "notes": "与 cdfd2026 同作者组、同在 ISCA 2026；LIBRA 面向多 GPU 页预取/迁移协调。与现有 clap2025、clover2026、shadowupdate2026 的多 GPU 映射/副本/失效传播线相邻，但尚未核读其预测器与成本模型。证据限制：仅 ISCA 2026 官方程序、dblp 题录与作者主页；作者 PDF 404，无摘要可核，未采用聚合网站技术数字。"
  },
  {
    "id": "helm2025",
    "topic": "lifecycle",
    "title": "HELM: Characterizing Unified Memory Accesses to Improve GPU Performance under Memory Oversubscription",
    "authors": [
      "Nathan Jones",
      "Tyler Allen",
      "Rong Ge"
    ],
    "year": 2025,
    "venue": "SC '25",
    "doi": "10.1145/3712285.3759812",
    "source_url": "https://doi.org/10.1145/3712285.3759812",
    "metadata_source": "ACM Digital Library 出版社页面（摘要、作者机构、DOI、pages 490-504）",
    "metadata_note": "出版社页面有摘要与索引片段；未取得 PDF，artifact 未运行。",
    "relevance": "core",
    "publication_status": "published",
    "reading_status": "abstract_or_indexed_excerpt",
    "fulltext_available": false,
    "notes": "与现有 shadowupdate2026 的失效传播机制互补：HELM 从 UVM 驱动 telemetry 提取 Fault Density、Duplicate Faults、Fault Span 等指标，刻画统一内存访问模式，用于选择迁移/放置策略，不修改映射传播协议；与 clap2025/clover2026 形成“策略选择 vs 页映射/副本机制”的互补。证据限制：仅 ACM 出版社页面摘要与索引片段，未取得 PDF；未独立复现作者报告的加速比。"
  },
  {
    "id": "ctl2026",
    "topic": "accelerators",
    "title": "CTL: A Case for CXL Device-Managed Hugepages",
    "authors": [
      "Sangbeom Jeon",
      "Sang-Hoon Kim"
    ],
    "year": 2026,
    "venue": "IEEE Computer Architecture Letters 25(1)",
    "doi": "10.1109/LCA.2026.3661563",
    "source_url": "https://sslab.ajou.ac.kr/news/2026-02-cal/",
    "metadata_source": "作者实验室页面 sslab.ajou.ac.kr（accepted 公告）；J-GLOBAL 题录（卷期 25(1):69-72、DOI）；IEEE CAL 卷期目录页",
    "metadata_note": "作者页仅确认 accepted；出版状态/DOI 未在出版社页面核实；不采用聚合摘要。",
    "relevance": "core",
    "publication_status": "accepted",
    "reading_status": "metadata_only",
    "fulltext_available": false,
    "notes": "与 accelerators 现有 ontable2017（加速器侧页表遍历）、cryptommu2023（第三方加速器访问控制）同 topic 但问题层次不同：CTL 在 CXL 设备侧用细粒度基页对外提供大页，以缓解 CXL 扩展下 TLB reach 不足导致的地址翻译开销；与 clover2026 的页表副本存储相邻但方向相反（副本存储优化 vs 设备侧大页管理）。证据限制：作者实验室页面仅确认 accepted，未获取摘要；未核 IEEE 出版社终稿，故不记 published；DOI 与卷期来自 J-GLOBAL 第三方题录，待核；不采用聚合网站摘要。"
  },
  {
    "id": "sagepte2026",
    "topic": "foundations",
    "title": "SagePTE: Co-locating guest and host PTEs to accelerate virtual memory translation",
    "authors": [
      "Amir Noohi",
      "Mostafa Derispour",
      "Rodrigo C. O. Rocha",
      "Antonio Barbalace",
      "Artemiy Margaritov"
    ],
    "year": 2026,
    "venue": "MICRO 2026",
    "doi": null,
    "source_url": "https://www.research.ed.ac.uk/en/publications/sagepte-co-locating-guest-and-host-ptes-to-accelerate-virtual-mem/",
    "metadata_source": "University of Edinburgh Research Explorer 机构题录（摘要、Accepted/In press - 8 Jul 2026、ISBN、禁运）；MICRO 2026 官方程序（Session 2C Distinguished Papers I）",
    "metadata_note": "MICRO 2026 未召开，按 accepted_upcoming；无 DOI 记录，未猜。",
    "relevance": "foundation",
    "publication_status": "accepted_upcoming",
    "reading_status": "abstract_or_indexed_excerpt",
    "fulltext_available": false,
    "notes": "机构摘要：仅在 guest 页表中就地影子 leaf PTE，按需把 guest/host 映射合入 128-bit 扩展项，复用 128-bit CAS 实现无 trap 的 leaf 同步，Alloy 验证隔离，ARM 上 4KB 平均 1.30×、THP 1.22×。与现有 nbwalker2026 同属 MICRO 2026 地址翻译工作，与 foundations 中 shared1993/synergistic2010/translation_ranger2019 的 TLB/翻译结构线相邻；聚焦嵌套翻译 leaf 开销，与设备 IOMMU 线不同。限制：仅机构题录摘要，作者手稿禁运至 2026-11-04，未获全文，机制细节未独立核读。"
  },
  {
    "id": "transfetch2026",
    "topic": "foundations",
    "title": "TRANS-FETCH: Driving Translation Prefetching with Deep Lookahead from L2 Data Prefetch Events",
    "authors": [
      "Cansong Zhou",
      "Xiaohang Wang",
      "Peng Liu"
    ],
    "year": 2026,
    "venue": "MICRO 2026",
    "doi": null,
    "source_url": "https://www.microarch.org/micro59/program/",
    "metadata_source": "MICRO 2026 官方主程序（Session 6A, Day 2, Nov 3）",
    "metadata_note": "仅官方题录，无摘要；MICRO 2026 未召开，按 accepted_upcoming；未猜 DOI。",
    "relevance": "foundation",
    "publication_status": "accepted_upcoming",
    "reading_status": "metadata_only",
    "fulltext_available": false,
    "notes": "与 latpc2025（GPU TLB 预取与 MSHR 压缩）及 MICRO 2026 同会 Trail（temporal TLB prefetching）同属翻译预取线；题名表明用 L2 数据预取事件做深层 lookahead 来驱动翻译预取，但仅大会题录，未获摘要/全文，不能推断其预测结构或与硬件数据预取器的耦合方式。"
  },
  {
    "id": "maverick2026",
    "topic": "foundations",
    "title": "Maverick: Leveraging Address Translation to Mitigate Memory Latency in Server Workloads",
    "authors": [
      "Ho Je Lee",
      "Minchan Kim",
      "Jaewon Kwon",
      "Jain Koo",
      "Yongju Lee",
      "Won Hur",
      "Jiwon Lee",
      "Won Woo Ro"
    ],
    "year": 2026,
    "venue": "MICRO 2026",
    "doi": null,
    "source_url": "https://www.microarch.org/micro59/program/",
    "metadata_source": "MICRO 2026 官方主程序（Session 2B: Memory System Power, Refresh & Predictive Prefetching, Day 1, Nov 2）",
    "metadata_note": "仅官方题录，无摘要；MICRO 2026 未召开，按 accepted_upcoming；未猜 DOI。",
    "relevance": "foundation",
    "publication_status": "accepted_upcoming",
    "reading_status": "metadata_only",
    "fulltext_available": false,
    "notes": "作者单位为 Yonsei Univ. / LG Electronics / Uppsala Univ.，与现有 latpc2025、shadowupdate2026、snakebyte2023 的 Yonsei / Won Woo Ro 线同源；题名指向用地址翻译缓解服务器负载内存延迟，与 foundation 翻译结构线相关。证据限制：仅 MICRO 2026 官方题录（Session 2B），无摘要，未获全文；不采用 SemiHub 等聚合导览的分类。"
  }
]
```

来源依据
- `activeforward2018`：NTU Scholars 机构题录含作者、DOI、摘要与单位；dblp（`conf/dac/FuWY18`）确认 2018 与 DOI、页码；DAC 2018 官方程序 PDF 可见题名 `58.4` 与作者。
- `cdfd2026` / `libra2026`：ISCA 2026 官方程序网页给出 Session 7C / 9B 的标题与作者单位；dblp 记录给出 DOI 与页码；作者主页（ddantalion.github.io、yananguo.com）确认 “Accepted by ISCA 2026 / To appear”。未采用聚合站的英文摘要性能数字。
- `helm2025`：ACM DL 出版社页面有摘要、作者机构、DOI 与页码；只有索引片段，未取得 PDF。
- `ctl2026`：作者实验室页面 sslab.ajou.ac.kr 明确 “accepted to IEEE Computer Architecture Letters”，日期 2026-02-03；J-GLOBAL 题录给出卷期；IEEE CAL 目录页可核卷期。未核出版社页面，故不记 published。
- `sagepte2026`：Edinburgh Research Explorer 机构题录含摘要、Accepted/In press、ISBN、禁运与会议信息；MICRO 2026 官方程序 Session 2C 可核。
- `transfetch2026` / `maverick2026`：MICRO 2026 官方主程序 Session 6A / 2B 可核题名、作者、单位；无摘要。

待核疑问
1. `ctl2026`：IEEE Xplore 出版社页面未取得；DOI `10.1109/LCA.2026.3661563` 与卷期来自 J-GLOBAL 第三方题录，是否最终与出版社记录一致需核。
2. `cdfd2026` / `libra2026`：ISCA 2026 已召开且 dblp 给出页码/DOI，但出版社页面未直接抓取；若需严格区分 “published” 与 “accepted”，需核 IEEE 出版页。
3. `sagepte2026`：禁运至 2026-11-04，作者手稿下载受限；机构摘要与 MICRO 程序一致，但正式页码/DOI 待会后确认。
4. `transfetch2026` / `maverick2026`：目前只有 MICRO 2026 官方题录，无摘要、无 DOI；机制与性能均不能推断。
5. `activeforward2018`：机构摘要中的 8% / 22.1% 为作者机构摘要级数字，未独立核读全文。
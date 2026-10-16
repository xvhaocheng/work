# 共享地址翻译文献目录

截至 2026-10-06。本目录由离线脚本生成，不联网。阅读层次：`fulltext_worker_reviewed` 表示模型已读本地全文并留证据卡；`root_spotchecked` 表示 root 对关键点另做核查；`metadata_only`/`abstract_or_indexed_excerpt` 仅题录或摘要层面。全文证据卡不是复现实验，也不声称逐字审读。

- 独立工作：72
- 全文工作：59
- 全文版本：60
- 仅题录工作：13

## foundations

| 年份/状态 | 题目 | PDF | 证据卡 | 相关性 |
|---|---|---|---|---|
| 1993 / published | <a id="shared1993"></a>[Shared translation lookaside buffers on multiprocessors and a performance study](https://scholars.nthu.edu.tw/esploro/outputs/journalArticle/Shared-translation-lookaside-buffers-on-multiprocessors/9957767288906774) | — | — | foundation |
| 2010 / published | <a id="icc2010"></a>[Inter-Core Cooperative TLB Prefetchers for Chip Multiprocessors](https://mrmgroup.cs.princeton.edu/papers/abhattac-asplos2010.pdf) | [icc2010](papers/icc2010.pdf) | [icc2010](workers/G5_foundations.md) | foundation |
| 2010 / published | <a id="synergistic2010"></a>[Synergistic TLBs for High Performance Address Translation in Chip Multiprocessors](https://pure.psu.edu/en/publications/synergistic-tlbs-for-high-performance-address-translation-in-chip) | — | — | foundation |
| 2010 / published | <a id="translation_cache2010"></a>[Translation Caching: Skip, Don't Walk (the Page Table)](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf) | [translation_cache2010](papers/translation_cache2010.pdf) | [translation_cache2010](workers/G5_foundations.md) | foundation |
| 2011 / published | <a id="slltlb2011"></a>[Shared Last-Level TLBs for Chip Multiprocessors](https://mrmgroup.cs.princeton.edu/papers/abhattac-hpca2011.pdf) | [slltlb2011](papers/slltlb2011.pdf) | [slltlb2011](workers/G5_foundations.md) | foundation |
| 2012 / published | <a id="colt2012"></a>[CoLT: Coalesced Large-Reach TLBs](https://users.ece.cmu.edu/~omutlu/pub/MICRO2012-proceedings.pdf) | [colt2012](papers/colt2012.pdf) | [colt2012](workers/G8_grit_colt.md) | foundation |
| 2013 / published | <a id="direct_segments2013"></a>[Efficient Virtual Memory for Big Memory Servers](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) | [direct_segments2013](papers/direct_segments2013.pdf) | [direct_segments2013](workers/G5_foundations.md) | foundation |
| 2018 / published | <a id="nocstar2018"></a>[Scalable Distributed Last-Level TLBs Using Low-Latency Interconnects](https://guilhermecox.github.io/dw/gcox-micro18.pdf) | [nocstar2018](papers/nocstar2018.pdf) | [nocstar2018](workers/G5_foundations.md) | nearby |
| 2019 / published | <a id="translation_ranger2019"></a>[Translation Ranger: Operating System Support for Contiguity-Aware TLBs](https://doi.org/10.1145/3307650.3322223) | — | — | foundation |
| 2020 / published | <a id="mitosis2020"></a>[Mitosis: Transparently Self-Replicating Page-Tables for Large-Memory Machines](https://apanwariisc.github.io/publications/asplos-2020-mitosis/mitosis-asplos20.pdf) | [mitosis2020](papers/mitosis2020.pdf) | [mitosis2020](workers/G7_supplement.md) | nearby |
| 2021 / published | <a id="vmitosis2021"></a>[Fast Local Page-Tables for Virtualized NUMA Servers with vMitosis](https://www.csa.iisc.ac.in/~arkapravab/papers/asplos21_vMitosis.pdf) | [vmitosis2021](papers/vmitosis2021.pdf) | [vmitosis2021](workers/G5_foundations.md) | nearby |
| 2023 / published | <a id="contiguitas2023"></a>[Contiguitas: The Pursuit of Physical Memory Contiguity in Datacenters](https://www.pdl.cmu.edu/ftp/associated/KZhao-Contiguitas.pdf) | [contiguitas2023](papers/contiguitas2023.pdf) | [contiguitas2023](workers/G5_foundations.md) | nearby |
| 2023 / published | <a id="victima2023"></a>[Victima: Drastically Increasing Address Translation Reach by Leveraging Underutilized Cache Resources](https://arxiv.org/pdf/2310.04158) | [victima2023](papers/victima2023.pdf) | [victima2023](workers/G5_foundations.md) | nearby |
| 2025 / published | <a id="cross2025"></a>[To Cross, or Not to Cross Pages for Prefetching?](https://ease-lab.github.io/ease_website/pubs/CROSS_HPCA25.pdf) | [cross2025](papers/cross2025.pdf) | [cross2025](workers/G5_foundations.md) | nearby |
| 2026 / accepted_upcoming | <a id="cato2026"></a>[Don't Get Stuck in Traffic: A Case for Contention-Aware Translation Offloading](https://www.microarch.org/micro59/program/) | — | — | foundation |
| 2026 / published | <a id="revelator2026v2"></a>[Revelator: Rapid Data Fetching via System-Software-Guided Hash-based Speculative Address Translation](https://arxiv.org/pdf/2508.02007v2) | [revelator2026v2](papers/revelator2026v2.pdf) | [revelator2026v2](workers/G5_foundations.md) | nearby |

- **shared1993** Shared translation lookaside buffers on multiprocessors and a performance study — Wei-Tsung Sun, Yunn-Yen Chen, Jih-Kwon Peir, Chung-Ta King；Journal of Information Science and Engineering 9(1)；doi未知；source=https://scholars.nthu.edu.tw/esploro/outputs/journalArticle/Shared-translation-lookaside-buffers-on-multiprocessors/9957767288906774；topic=foundations；status=published；relevance=foundation；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: 机构题录及摘要；早期共享TLB思想前史，不等同当代设备IOMMU。
  - 版本 shared1993：无本地文件；reading=abstract_or_indexed_excerpt；status=published；year=1993；venue=Journal of Information Science and Engineering 9(1)；doi=未知；source=https://scholars.nthu.edu.tw/esploro/outputs/journalArticle/Shared-translation-lookaside-buffers-on-multiprocessors/9957767288906774；root_spotchecked=False
    - notes: 机构题录及摘要；早期共享TLB思想前史，不等同当代设备IOMMU。
- **icc2010** Inter-Core Cooperative TLB Prefetchers for Chip Multiprocessors — Abhishek Bhattacharjee, Margaret Martonosi；ASPLOS'10 (Pittsburgh, PA, USA)；doi未知；source=https://mrmgroup.cs.princeton.edu/papers/abhattac-asplos2010.pdf；topic=foundations；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 icc2010：PDF [papers/icc2010.pdf](papers/icc2010.pdf); text [papers/icc2010.txt](papers/icc2010.txt); 报告 [workers/G5_foundations.md](workers/G5_foundations.md); 证据卡 [workers/G5_foundations-papers.json](workers/G5_foundations-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2010；venue=ASPLOS'10 (Pittsburgh, PA, USA)；doi=未知；source=https://mrmgroup.cs.princeton.edu/papers/abhattac-asplos2010.pdf；root_spotchecked=False
- **synergistic2010** Synergistic TLBs for High Performance Address Translation in Chip Multiprocessors — Shekhar Srikantaiah, Mahmut T. Kandemir；MICRO 2010；10.1109/MICRO.2010.26；source=https://pure.psu.edu/en/publications/synergistic-tlbs-for-high-performance-address-translation-in-chip；topic=foundations；status=published；relevance=foundation；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: 机构摘要明确容量共享、翻译迁移和复制；未取得全文。
  - 版本 synergistic2010：无本地文件；reading=abstract_or_indexed_excerpt；status=published；year=2010；venue=MICRO 2010；doi=10.1109/MICRO.2010.26；source=https://pure.psu.edu/en/publications/synergistic-tlbs-for-high-performance-address-translation-in-chip；root_spotchecked=False
    - notes: 机构摘要明确容量共享、翻译迁移和复制；未取得全文。
- **translation_cache2010** Translation Caching: Skip, Don't Walk (the Page Table) — Thomas W. Barr, Alan L. Cox, Scott Rixner；ISCA'10 (Saint-Malo, France)；doi未知；source=https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf；topic=foundations；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 translation_cache2010：PDF [papers/translation_cache2010.pdf](papers/translation_cache2010.pdf); text [papers/translation_cache2010.txt](papers/translation_cache2010.txt); 报告 [workers/G5_foundations.md](workers/G5_foundations.md); 证据卡 [workers/G5_foundations-papers.json](workers/G5_foundations-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2010；venue=ISCA'10 (Saint-Malo, France)；doi=未知；source=https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf；root_spotchecked=False
- **slltlb2011** Shared Last-Level TLBs for Chip Multiprocessors — Abhishek Bhattacharjee, Daniel Lustig, Margaret Martonosi；HPCA 2011；doi未知；source=https://mrmgroup.cs.princeton.edu/papers/abhattac-hpca2011.pdf；topic=foundations；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://collaborate.princeton.edu/en/publications/shared-last-level-tlbs-for-chip-multiprocessors/
  - 版本 slltlb2011：PDF [papers/slltlb2011.pdf](papers/slltlb2011.pdf); text [papers/slltlb2011.txt](papers/slltlb2011.txt); 报告 [workers/G5_foundations.md](workers/G5_foundations.md); 证据卡 [workers/G5_foundations-papers.json](workers/G5_foundations-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2011；venue=HPCA 2011；doi=未知；source=https://mrmgroup.cs.princeton.edu/papers/abhattac-hpca2011.pdf；root_spotchecked=False
- **colt2012** CoLT: Coalesced Large-Reach TLBs — Binh Pham, Viswanathan Vaidyanathan, Aamer Jaleel, Abhishek Bhattacharjee；2012 IEEE/ACM 45th Annual International Symposium on Microarchitecture (MICRO)；10.1109/MICRO.2012.32；source=https://users.ece.cmu.edu/~omutlu/pub/MICRO2012-proceedings.pdf；topic=foundations；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 colt2012：PDF [papers/colt2012.pdf](papers/colt2012.pdf); text [papers/colt2012.txt](papers/colt2012.txt); 报告 [workers/G8_grit_colt.md](workers/G8_grit_colt.md); 证据卡 [workers/G8_grit_colt-papers.json](workers/G8_grit_colt-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2012；venue=2012 IEEE/ACM 45th Annual International Symposium on Microarchitecture (MICRO)；doi=10.1109/MICRO.2012.32；source=https://users.ece.cmu.edu/~omutlu/pub/MICRO2012-proceedings.pdf；root_spotchecked=False
- **direct_segments2013** Efficient Virtual Memory for Big Memory Servers — Arkaprava Basu, Jayneel Gandhi, Jichuan Chang, Mark D. Hill, Michael M. Swift；ISCA'13 (Tel-Aviv, Israel)；doi未知；source=https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf；topic=foundations；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 direct_segments2013：PDF [papers/direct_segments2013.pdf](papers/direct_segments2013.pdf); text [papers/direct_segments2013.txt](papers/direct_segments2013.txt); 报告 [workers/G5_foundations.md](workers/G5_foundations.md); 证据卡 [workers/G5_foundations-papers.json](workers/G5_foundations-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2013；venue=ISCA'13 (Tel-Aviv, Israel)；doi=未知；source=https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf；root_spotchecked=False
- **nocstar2018** Scalable Distributed Last-Level TLBs Using Low-Latency Interconnects — Srikant Bharadwaj, Guilherme Cox, Tushar Krishna, Abhishek Bhattacharjee；MICRO 2018；10.1109/MICRO.2018.00030；source=https://guilhermecox.github.io/dw/gcox-micro18.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知
  - 版本 nocstar2018：PDF [papers/nocstar2018.pdf](papers/nocstar2018.pdf); text [papers/nocstar2018.txt](papers/nocstar2018.txt); 报告 [workers/G5_foundations.md](workers/G5_foundations.md); 证据卡 [workers/G5_foundations-papers.json](workers/G5_foundations-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2018；venue=MICRO 2018；doi=10.1109/MICRO.2018.00030；source=https://guilhermecox.github.io/dw/gcox-micro18.pdf；root_spotchecked=True
- **translation_ranger2019** Translation Ranger: Operating System Support for Contiguity-Aware TLBs — Zi Yan, Daniel Lustig, David Nellans, Abhishek Bhattacharjee；ISCA 2019；10.1145/3307650.3322223；source=https://doi.org/10.1145/3307650.3322223；topic=foundations；status=published；relevance=foundation；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: 作者PDF的搜索缓存与出版社题录可核；本地全文下载失败。
  - 版本 translation_ranger2019：无本地文件；reading=abstract_or_indexed_excerpt；status=published；year=2019；venue=ISCA 2019；doi=10.1145/3307650.3322223；source=https://doi.org/10.1145/3307650.3322223；root_spotchecked=False
    - notes: 作者PDF的搜索缓存与出版社题录可核；本地全文下载失败。
- **mitosis2020** Mitosis: Transparently Self-Replicating Page-Tables for Large-Memory Machines — Reto Achermann, Ashish Panwar, Abhishek Bhattacharjee, Timothy Roscoe, Jayneel Gandhi；ASPLOS '20；10.1145/3373376.3378468；source=https://apanwariisc.github.io/publications/asplos-2020-mitosis/mitosis-asplos20.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 mitosis2020：PDF [papers/mitosis2020.pdf](papers/mitosis2020.pdf); text [papers/mitosis2020.txt](papers/mitosis2020.txt); 报告 [workers/G7_supplement.md](workers/G7_supplement.md); 证据卡 [workers/G7_supplement-papers.json](workers/G7_supplement-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2020；venue=ASPLOS '20；doi=10.1145/3373376.3378468；source=https://apanwariisc.github.io/publications/asplos-2020-mitosis/mitosis-asplos20.pdf；root_spotchecked=False
- **vmitosis2021** Fast Local Page-Tables for Virtualized NUMA Servers with vMitosis — Ashish Panwar, Reto Achermann, Arkaprava Basu, Abhishek Bhattacharjee, K. Gopinath, Jayneel Gandhi；ASPLOS '21 (Virtual, USA)；10.1145/3445814.3446709；source=https://www.csa.iisc.ac.in/~arkapravab/papers/asplos21_vMitosis.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 vmitosis2021：PDF [papers/vmitosis2021.pdf](papers/vmitosis2021.pdf); text [papers/vmitosis2021.txt](papers/vmitosis2021.txt); 报告 [workers/G5_foundations.md](workers/G5_foundations.md); 证据卡 [workers/G5_foundations-papers.json](workers/G5_foundations-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2021；venue=ASPLOS '21 (Virtual, USA)；doi=10.1145/3445814.3446709；source=https://www.csa.iisc.ac.in/~arkapravab/papers/asplos21_vMitosis.pdf；root_spotchecked=False
- **contiguitas2023** Contiguitas: The Pursuit of Physical Memory Contiguity in Datacenters — Kaiyang Zhao, Kaiwen Xue, Ziqi Wang, Dan Schatzberg, Leon Yang, Antonis Manousis, Johannes Weiner, Rik van Riel, Bikash Sharma, Chunqiang Tang, Dimitrios Skarlatos；ISCA '23 (Orlando, FL, USA)；10.1145/3579371.3589079；source=https://www.pdl.cmu.edu/ftp/associated/KZhao-Contiguitas.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 contiguitas2023：PDF [papers/contiguitas2023.pdf](papers/contiguitas2023.pdf); text [papers/contiguitas2023.txt](papers/contiguitas2023.txt); 报告 [workers/G5_foundations.md](workers/G5_foundations.md); 证据卡 [workers/G5_foundations-papers.json](workers/G5_foundations-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2023；venue=ISCA '23 (Orlando, FL, USA)；doi=10.1145/3579371.3589079；source=https://www.pdl.cmu.edu/ftp/associated/KZhao-Contiguitas.pdf；root_spotchecked=False
- **victima2023** Victima: Drastically Increasing Address Translation Reach by Leveraging Underutilized Cache Resources — Konstantinos Kanellopoulos, Hong Chul Nam, F. Nisa Bostanci, Rahul Bera, Mohammad Sadrosadati, Rakesh Kumar, Davide Basilio Bartolini, Onur Mutlu；MICRO 2023；doi未知；source=https://arxiv.org/pdf/2310.04158；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 victima2023：PDF [papers/victima2023.pdf](papers/victima2023.pdf); text [papers/victima2023.txt](papers/victima2023.txt); 报告 [workers/G5_foundations.md](workers/G5_foundations.md); 证据卡 [workers/G5_foundations-papers.json](workers/G5_foundations-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2023；venue=MICRO 2023；doi=未知；source=https://arxiv.org/pdf/2310.04158；root_spotchecked=False
- **cross2025** To Cross, or Not to Cross Pages for Prefetching? — Georgios Vavouliotis, Marti Torrents, Boris Grot, Kleovoulos Kalaitzidis, Leeor Peled, Marc Casas；HPCA 2025；doi未知；source=https://ease-lab.github.io/ease_website/pubs/CROSS_HPCA25.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 cross2025：PDF [papers/cross2025.pdf](papers/cross2025.pdf); text [papers/cross2025.txt](papers/cross2025.txt); 报告 [workers/G5_foundations.md](workers/G5_foundations.md); 证据卡 [workers/G5_foundations-papers.json](workers/G5_foundations-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2025；venue=HPCA 2025；doi=未知；source=https://ease-lab.github.io/ease_website/pubs/CROSS_HPCA25.pdf；root_spotchecked=False
- **cato2026** Don't Get Stuck in Traffic: A Case for Contention-Aware Translation Offloading — Osang Kwon, Yongho Lee, Sungbin Jang, Kyeongchan Kim, Seokin Hong；MICRO 2026；doi未知；source=https://www.microarch.org/micro59/program/；topic=foundations；status=accepted_upcoming；relevance=foundation；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: 大会题录已核；作者页面搜索缓存介绍CXL分层内存的争用感知PTW卸载，实时页面404；未全文核读。
  - 版本 cato2026：无本地文件；reading=abstract_or_indexed_excerpt；status=accepted_upcoming；year=2026；venue=MICRO 2026；doi=未知；source=https://www.microarch.org/micro59/program/；root_spotchecked=False
    - notes: 大会题录已核；作者页面搜索缓存介绍CXL分层内存的争用感知PTW卸载，实时页面404；未全文核读。
- **revelator2026v2** Revelator: Rapid Data Fetching via System-Software-Guided Hash-based Speculative Address Translation — Konstantinos Kanellopoulos, Konstantinos Sgouras, Harsh Songara, Andreas Kosmas Kakolyris, Vlad-Petru Nitu, Spiros Galanopoulos, Rahul Bera, Konstantina Koliogeorgi, Rakesh Kumar, Onur Mutlu；ISCA 2026；10.1109/ISCA66397.2026.00147；source=https://arxiv.org/pdf/2508.02007v2；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://doi.org/10.1109/ISCA66397.2026.00147
  - metadata_note: 本地保存arXiv v2作者稿；正式出版元数据由IEEE核定。
  - 版本 revelator2026v2：PDF [papers/revelator2026v2.pdf](papers/revelator2026v2.pdf); text [papers/revelator2026v2.txt](papers/revelator2026v2.txt); 报告 [workers/G5_foundations.md](workers/G5_foundations.md); 证据卡 [workers/G5_foundations-papers.json](workers/G5_foundations-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2026；venue=ISCA 2026；doi=10.1109/ISCA66397.2026.00147；source=https://arxiv.org/pdf/2508.02007v2；root_spotchecked=False
    - metadata_note: 本地保存arXiv v2作者稿；正式出版元数据由IEEE核定。

## gpu

| 年份/状态 | 题目 | PDF | 证据卡 | 相关性 |
|---|---|---|---|---|
| 2014 / published | <a id="pichai2014"></a>[Architectural Support for Address Translation on GPUs: Designing Memory Management Units for CPU/GPUs with Unified Address Spaces](https://courses.grainger.illinois.edu/ece598ms/fa2021/papers/paper233.pdf) | [pichai2014](papers/pichai2014.pdf) | [pichai2014](workers/G7_supplement.md) | core |
| 2014 / published | <a id="gpummu2014"></a>[Supporting x86-64 Address Translation for 100s of GPU Lanes](https://research.cs.wisc.edu/multifacet/papers/hpca14_gpummu_appendix.pdf) | [gpummu2014](papers/gpummu2014.pdf) | [gpummu2014](workers/G1_gpu.md) | core |
| 2017 / published | <a id="mosaic2017"></a>[Mosaic: A GPU Memory Manager with Application-Transparent Support for Multiple Page Sizes](https://research.ece.cmu.edu/safari/pubs/mosaic-micro17.pdf) | [mosaic2017](papers/mosaic2017.pdf) | [mosaic2017](workers/G1_gpu.md) | nearby |
| 2018 / published | <a id="mask2018"></a>[MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf) | [mask2018](papers/mask2018.pdf) | [mask2018](workers/G1_gpu.md) | core |
| 2018 / published | <a id="neighborhood2018"></a>[Neighborhood-aware address translation for irregular GPU applications](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf) | [neighborhood2018](papers/neighborhood2018.pdf) | [neighborhood2018](workers/G1_gpu.md) | core |
| 2018 / published | <a id="walk_scheduling2018"></a>[Scheduling Page Table Walks for Irregular GPU Applications](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf) | [walk_scheduling2018](papers/walk_scheduling2018.pdf) | [walk_scheduling2018](workers/G1_gpu.md) | core |
| 2020 / published | <a id="valkyrie2020"></a>[Valkyrie: Leveraging Inter-TLB Locality to Enhance GPU Performance](https://sarchlab.org/valkyrie_pact_2020.pdf) | [valkyrie2020](papers/valkyrie2020.pdf) | [valkyrie2020](workers/G1_gpu.md) | nearby |
| 2021 / published | <a id="walk_stealing2021"></a>[Improving GPU Multi-tenancy with Page Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) | [walk_stealing2021](papers/walk_stealing2021.pdf) | [walk_stealing2021](workers/G1_gpu.md) | core |
| 2023 / published | <a id="orchestrated2023"></a>[Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs](https://libingyao.github.io/assets/files/DAC2023.pdf) | [orchestrated2023](papers/orchestrated2023.pdf) | [orchestrated2023](workers/G1_gpu.md) | nearby |
| 2023 / published | <a id="snakebyte2023"></a>[SnakeByte: A TLB Design with Adaptive and Recursive Page Merging in GPUs](https://yonsei.elsevierpure.com/en/publications/snakebyte-a-tlb-design-with-adaptive-and-recursive-page-merging-i/) | — | — | core |
| 2024 / published | <a id="avatar2024"></a>[A Case for Speculative Address Translation with Rapid Validation for GPUs](https://zoon17.github.io/pdfs/avatar_micro24.pdf) | [avatar2024](papers/avatar2024.pdf) | [avatar2024](workers/G7_supplement.md) | core |
| 2024 / published | <a id="star2024"></a>[STAR: Sub-Entry Sharing-Aware TLB for Multi-Instance GPU](https://libingyao.github.io/assets/files/STAR_MICRO24.pdf) | [star2024_final](papers/star2024_final.pdf)<br>[star2024](papers/star2024.pdf) | [star2024_final](workers/G7_supplement.md)<br>[star2024](workers/G1_gpu.md) | core |
| 2025 / published | <a id="lamp2025"></a>[LAMP: A Locality-Aware TLB Sharing Framework for Multi-Tenant GPUs via TLB Comprehensive Profiling](https://hpcn.exeter.ac.uk/hpcc2025/Conference_Programme.pdf) | — | — | core |
| 2025 / published | <a id="latpc2025"></a>[LATPC: Accelerating GPU Address Translation Using Locality-Aware TLB Prefetching and MSHR Compression](https://yonsei.elsevierpure.com/en/publications/latpc-accelerating-gpu-address-translation-using-locality-aware-t/) | — | — | core |
| 2026 / published | <a id="cuptw2026"></a>[cuPTW: Leveraging Idle Compute Units for Massively Parallel GPU Page Table Walks](https://maxkev1n.github.io/publications/sigmetrics-2026/3805633.pdf) | [cuptw2026](papers/cuptw2026.pdf) | [cuptw2026](workers/G7_supplement.md) | core |
| 2026 / accepted_upcoming | <a id="nbwalker2026"></a>[NB-Walker: Non-blocking Page Table Walker to Enhance Address Translation in NUMA GPUs](https://www.microarch.org/micro59/program/) | — | — | core |

- **pichai2014** Architectural Support for Address Translation on GPUs: Designing Memory Management Units for CPU/GPUs with Unified Address Spaces — Bharath Pichai, Lisa Hsu, Abhishek Bhattacharjee；ASPLOS '14；10.1145/2541940.2541942；source=https://courses.grainger.illinois.edu/ece598ms/fa2021/papers/paper233.pdf；topic=gpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 pichai2014：PDF [papers/pichai2014.pdf](papers/pichai2014.pdf); text [papers/pichai2014.txt](papers/pichai2014.txt); 报告 [workers/G7_supplement.md](workers/G7_supplement.md); 证据卡 [workers/G7_supplement-papers.json](workers/G7_supplement-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2014；venue=ASPLOS '14；doi=10.1145/2541940.2541942；source=https://courses.grainger.illinois.edu/ece598ms/fa2021/papers/paper233.pdf；root_spotchecked=False
- **gpummu2014** Supporting x86-64 Address Translation for 100s of GPU Lanes — Jason Power, Mark D. Hill, David A. Wood；HPCA 2014；10.1109/HPCA.2014.6835965；source=https://research.cs.wisc.edu/multifacet/papers/hpca14_gpummu_appendix.pdf；topic=gpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://arch.cs.ucdavis.edu/gpu/memory/2014/02/15/gpummu.html
  - 版本 gpummu2014：PDF [papers/gpummu2014.pdf](papers/gpummu2014.pdf); text [papers/gpummu2014.txt](papers/gpummu2014.txt); 报告 [workers/G1_gpu.md](workers/G1_gpu.md); 证据卡 [workers/G1_gpu-papers.json](workers/G1_gpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2014；venue=HPCA 2014；doi=10.1109/HPCA.2014.6835965；source=https://research.cs.wisc.edu/multifacet/papers/hpca14_gpummu_appendix.pdf；root_spotchecked=False
- **mosaic2017** Mosaic: A GPU Memory Manager with Application-Transparent Support for Multiple Page Sizes — Rachata Ausavarungnirun, Joshua Landgraf, Vance Miller, Saugata Ghose, Jayneel Gandhi, Christopher J. Rossbach, Onur Mutlu；MICRO-50 (The 50th Annual IEEE/ACM International Symposium on Microarchitecture), Oct 14-18 2017, Cambridge, MA, USA；10.1145/3123939.3123975；source=https://research.ece.cmu.edu/safari/pubs/mosaic-micro17.pdf；topic=gpu；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 mosaic2017：PDF [papers/mosaic2017.pdf](papers/mosaic2017.pdf); text [papers/mosaic2017.txt](papers/mosaic2017.txt); 报告 [workers/G1_gpu.md](workers/G1_gpu.md); 证据卡 [workers/G1_gpu-papers.json](workers/G1_gpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2017；venue=MICRO-50 (The 50th Annual IEEE/ACM International Symposium on Microarchitecture), Oct 14-18 2017, Cambridge, MA, USA；doi=10.1145/3123939.3123975；source=https://research.ece.cmu.edu/safari/pubs/mosaic-micro17.pdf；root_spotchecked=False
- **mask2018** MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency — Rachata Ausavarungnirun, Vance Miller, Joshua Landgraf, Saugata Ghose, Jayneel Gandhi, Adwait Jog, Christopher J. Rossbach, Onur Mutlu；ASPLOS 2018 (23rd ACM International Conference on Architectural Support for Programming Languages and Operating Systems), Williamsburg, VA, USA；10.1145/3173162.3173169；source=https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf；topic=gpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 mask2018：PDF [papers/mask2018.pdf](papers/mask2018.pdf); text [papers/mask2018.txt](papers/mask2018.txt); 报告 [workers/G1_gpu.md](workers/G1_gpu.md); 证据卡 [workers/G1_gpu-papers.json](workers/G1_gpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2018；venue=ASPLOS 2018 (23rd ACM International Conference on Architectural Support for Programming Languages and Operating Systems), Williamsburg, VA, USA；doi=10.1145/3173162.3173169；source=https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf；root_spotchecked=False
- **neighborhood2018** Neighborhood-aware address translation for irregular GPU applications — Seunghee Shin, Michael LeBeane, Yan Solihin, Arkaprava Basu；2018 51st Annual IEEE/ACM International Symposium on Microarchitecture (MICRO), pp. 352-363；10.1109/MICRO.2018.00036；source=https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf；topic=gpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 neighborhood2018：PDF [papers/neighborhood2018.pdf](papers/neighborhood2018.pdf); text [papers/neighborhood2018.txt](papers/neighborhood2018.txt); 报告 [workers/G1_gpu.md](workers/G1_gpu.md); 证据卡 [workers/G1_gpu-papers.json](workers/G1_gpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2018；venue=2018 51st Annual IEEE/ACM International Symposium on Microarchitecture (MICRO), pp. 352-363；doi=10.1109/MICRO.2018.00036；source=https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf；root_spotchecked=False
- **walk_scheduling2018** Scheduling Page Table Walks for Irregular GPU Applications — Seunghee Shin, Guilherme Cox, Mark Oskin, Gabriel H. Loh, Yan Solihin, Abhishek Bhattacharjee, Arkaprava Basu；2018 ACM/IEEE 45th Annual International Symposium on Computer Architecture (ISCA), pp. 180-192；10.1109/ISCA.2018.00025；source=https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf；topic=gpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 walk_scheduling2018：PDF [papers/walk_scheduling2018.pdf](papers/walk_scheduling2018.pdf); text [papers/walk_scheduling2018.txt](papers/walk_scheduling2018.txt); 报告 [workers/G1_gpu.md](workers/G1_gpu.md); 证据卡 [workers/G1_gpu-papers.json](workers/G1_gpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2018；venue=2018 ACM/IEEE 45th Annual International Symposium on Computer Architecture (ISCA), pp. 180-192；doi=10.1109/ISCA.2018.00025；source=https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf；root_spotchecked=False
- **valkyrie2020** Valkyrie: Leveraging Inter-TLB Locality to Enhance GPU Performance — Trinayan Baruah, Yifan Sun, Saiful A. Mojumder, José L. Abellán, Yash Ukidave, Ajay Joshi, Norman Rubin, John Kim, David Kaeli；PACT 2020 (2020 International Conference on Parallel Architectures and Compilation Techniques), Virtual Event, GA, USA；10.1145/3410463.3414639；source=https://sarchlab.org/valkyrie_pact_2020.pdf；topic=gpu；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 valkyrie2020：PDF [papers/valkyrie2020.pdf](papers/valkyrie2020.pdf); text [papers/valkyrie2020.txt](papers/valkyrie2020.txt); 报告 [workers/G1_gpu.md](workers/G1_gpu.md); 证据卡 [workers/G1_gpu-papers.json](workers/G1_gpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2020；venue=PACT 2020 (2020 International Conference on Parallel Architectures and Compilation Techniques), Virtual Event, GA, USA；doi=10.1145/3410463.3414639；source=https://sarchlab.org/valkyrie_pact_2020.pdf；root_spotchecked=False
- **walk_stealing2021** Improving GPU Multi-tenancy with Page Walk Stealing — B. Pratheek, Neha Jawalkar, Arkaprava Basu；2021 IEEE International Symposium on High-Performance Computer Architecture (HPCA), pp. 626-639；10.1109/HPCA51647.2021.00059；source=https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf；topic=gpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 walk_stealing2021：PDF [papers/walk_stealing2021.pdf](papers/walk_stealing2021.pdf); text [papers/walk_stealing2021.txt](papers/walk_stealing2021.txt); 报告 [workers/G1_gpu.md](workers/G1_gpu.md); 证据卡 [workers/G1_gpu-papers.json](workers/G1_gpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2021；venue=2021 IEEE International Symposium on High-Performance Computer Architecture (HPCA), pp. 626-639；doi=10.1109/HPCA51647.2021.00059；source=https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf；root_spotchecked=False
- **orchestrated2023** Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs — Bingyao Li, Yueqi Wang, Xulong Tang；DAC 2023；doi未知；source=https://libingyao.github.io/assets/files/DAC2023.pdf；topic=gpu；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://libingyao.github.io/
  - metadata_note: 作者网页及DAC2023官方节目核定；本地作者稿引文出现2024，版本更新时间不确定。
  - 版本 orchestrated2023：PDF [papers/orchestrated2023.pdf](papers/orchestrated2023.pdf); text [papers/orchestrated2023.txt](papers/orchestrated2023.txt); 报告 [workers/G1_gpu.md](workers/G1_gpu.md); 证据卡 [workers/G1_gpu-papers.json](workers/G1_gpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2023；venue=DAC 2023；doi=未知；source=https://libingyao.github.io/assets/files/DAC2023.pdf；root_spotchecked=False
    - metadata_note: 作者网页及DAC2023官方节目核定；本地作者稿引文出现2024，版本更新时间不确定。
- **snakebyte2023** SnakeByte: A TLB Design with Adaptive and Recursive Page Merging in GPUs — Jiwon Lee, Ju Min Lee, Yunho Oh, William J. Song, Won Woo Ro；HPCA 2023；10.1109/HPCA56546.2023.10071063；source=https://yonsei.elsevierpure.com/en/publications/snakebyte-a-tlb-design-with-adaptive-and-recursive-page-merging-i/；topic=gpu；status=published；relevance=core；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: 机构摘要可核递归合并和TLB覆盖；未获全文，不推断复杂更新成本。
  - 版本 snakebyte2023：无本地文件；reading=abstract_or_indexed_excerpt；status=published；year=2023；venue=HPCA 2023；doi=10.1109/HPCA56546.2023.10071063；source=https://yonsei.elsevierpure.com/en/publications/snakebyte-a-tlb-design-with-adaptive-and-recursive-page-merging-i/；root_spotchecked=False
    - notes: 机构摘要可核递归合并和TLB覆盖；未获全文，不推断复杂更新成本。
- **avatar2024** A Case for Speculative Address Translation with Rapid Validation for GPUs — Junhyeok Park, Osang Kwon, Yongho Lee, Seongwook Kim, Gwangeun Byeon, Jihun Yoon, Prashant J. Nair, Seokin Hong；2024 57th IEEE/ACM International Symposium on Microarchitecture (MICRO)；10.1109/MICRO61859.2024.00029；source=https://zoon17.github.io/pdfs/avatar_micro24.pdf；topic=gpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 avatar2024：PDF [papers/avatar2024.pdf](papers/avatar2024.pdf); text [papers/avatar2024.txt](papers/avatar2024.txt); 报告 [workers/G7_supplement.md](workers/G7_supplement.md); 证据卡 [workers/G7_supplement-papers.json](workers/G7_supplement-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2024；venue=2024 57th IEEE/ACM International Symposium on Microarchitecture (MICRO)；doi=10.1109/MICRO61859.2024.00029；source=https://zoon17.github.io/pdfs/avatar_micro24.pdf；root_spotchecked=False
- **star2024** STAR: Sub-Entry Sharing-Aware TLB for Multi-Instance GPU — Bingyao Li, Yueqi Wang, Tianyu Wang, Lieven Eeckhout, Jun Yang, Aamer Jaleel, Xulong Tang；MICRO 2024；doi未知；source=https://libingyao.github.io/assets/files/STAR_MICRO24.pdf；topic=gpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://libingyao.github.io/
  - version_note: 作者主页列为MICRO2024正式版本；目录首选。与arXiv版本合并计为一项工作，正文细节以各自版本为准。
  - 版本 star2024_final：PDF [papers/star2024_final.pdf](papers/star2024_final.pdf); text [papers/star2024_final.txt](papers/star2024_final.txt); 报告 [workers/G7_supplement.md](workers/G7_supplement.md); 证据卡 [workers/G7_supplement-papers.json](workers/G7_supplement-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2024；venue=MICRO 2024；doi=未知；source=https://libingyao.github.io/assets/files/STAR_MICRO24.pdf；root_spotchecked=False
    - version_note: 作者主页列为MICRO2024正式版本；目录首选。与arXiv版本合并计为一项工作，正文细节以各自版本为准。
  - 版本 star2024：PDF [papers/star2024.pdf](papers/star2024.pdf); text [papers/star2024.txt](papers/star2024.txt); 报告 [workers/G1_gpu.md](workers/G1_gpu.md); 证据卡 [workers/G1_gpu-papers.json](workers/G1_gpu-papers.json)；reading=fulltext_worker_reviewed；status=preprint；year=2024；venue=arXiv preprint (arXiv:2404.18361v1 [cs.DC], 2024-04-29)；文件内无会议/DOI/版权信息；doi=未知；source=https://arxiv.org/pdf/2404.18361；root_spotchecked=False
    - version_note: arXiv预印本题名不同；与正式版作为同一工作的一组版本归并。
- **lamp2025** LAMP: A Locality-Aware TLB Sharing Framework for Multi-Tenant GPUs via TLB Comprehensive Profiling — Hang Yang, Chen Li, Xuanyi Li, Xiaowen Chen, Jianzhuang Lu, Yang Guo；HPCC 2025；doi未知；source=https://hpcn.exeter.ac.uk/hpcc2025/Conference_Programme.pdf；topic=gpu；status=published；relevance=core；reading=metadata_only；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: 仅官方节目题录核对；未据标题推断具体协议或效果。
  - 版本 lamp2025：无本地文件；reading=metadata_only；status=published；year=2025；venue=HPCC 2025；doi=未知；source=https://hpcn.exeter.ac.uk/hpcc2025/Conference_Programme.pdf；root_spotchecked=False
    - notes: 仅官方节目题录核对；未据标题推断具体协议或效果。
- **latpc2025** LATPC: Accelerating GPU Address Translation Using Locality-Aware TLB Prefetching and MSHR Compression — Yeonan Ha, Jiho Park, Hanna Cha, Jiwon Lee, Joonsung Kim, Won Woo Ro, Youngsok Kim；MICRO 2025；10.1145/3725843.3756069；source=https://yonsei.elsevierpure.com/en/publications/latpc-accelerating-gpu-address-translation-using-locality-aware-t/；topic=gpu；status=published；relevance=core；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: 机构摘要明确warp内VPN规律、MSHR压缩和批量页表遍历；PDF403，未全文核读。
  - 版本 latpc2025：无本地文件；reading=abstract_or_indexed_excerpt；status=published；year=2025；venue=MICRO 2025；doi=10.1145/3725843.3756069；source=https://yonsei.elsevierpure.com/en/publications/latpc-accelerating-gpu-address-translation-using-locality-aware-t/；root_spotchecked=False
    - notes: 机构摘要明确warp内VPN规律、MSHR压缩和批量页表遍历；PDF403，未全文核读。
- **cuptw2026** cuPTW: Leveraging Idle Compute Units for Massively Parallel GPU Page Table Walks — Zihang Chen, Tianao Ge, Lieven Eeckhout, Hongyuan Liu, Jiayi Huang；Proc. ACM Meas. Anal. Comput. Syst. 10, 2, Article 35 (June 2026)；10.1145/3805633；source=https://maxkev1n.github.io/publications/sigmetrics-2026/3805633.pdf；topic=gpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知
  - 版本 cuptw2026：PDF [papers/cuptw2026.pdf](papers/cuptw2026.pdf); text [papers/cuptw2026.txt](papers/cuptw2026.txt); 报告 [workers/G7_supplement.md](workers/G7_supplement.md); 证据卡 [workers/G7_supplement-papers.json](workers/G7_supplement-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2026；venue=Proc. ACM Meas. Anal. Comput. Syst. 10, 2, Article 35 (June 2026)；doi=10.1145/3805633；source=https://maxkev1n.github.io/publications/sigmetrics-2026/3805633.pdf；root_spotchecked=True
- **nbwalker2026** NB-Walker: Non-blocking Page Table Walker to Enhance Address Translation in NUMA GPUs — Zihang Chen, Lieven Eeckhout, Hongyuan Liu, Tianao Ge, Xinkai Wang, Jiayi Huang；MICRO 2026；doi未知；source=https://www.microarch.org/micro59/program/；topic=gpu；status=accepted_upcoming；relevance=core；reading=metadata_only；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: 大会题录、作者主页、公开artifact可核；尚未取得论文全文。artifact没有运行，也不当作正文。
  - 版本 nbwalker2026：无本地文件；reading=metadata_only；status=accepted_upcoming；year=2026；venue=MICRO 2026；doi=未知；source=https://www.microarch.org/micro59/program/；root_spotchecked=False
    - notes: 大会题录、作者主页、公开artifact可核；尚未取得论文全文。artifact没有运行，也不当作正文。

## multigpu

| 年份/状态 | 题目 | PDF | 证据卡 | 相关性 |
|---|---|---|---|---|
| 2021 / published | <a id="leasttlb2021"></a>[Improving Address Translation in Multi-GPUs via Sharing and Spilling aware TLB Design](https://jiemingyin.github.io/docs/MICRO2021.pdf) | [leasttlb2021](papers/leasttlb2021.pdf) | [leasttlb2021](workers/G2_multigpu.md) | core |
| 2022 / published | <a id="mgvm2022"></a>[Designing Virtual Memory System of MCM GPUs](https://www.csa.iisc.ac.in/~arkapravab/papers/MICRO22_MGvm.pdf) | [mgvm2022](papers/mgvm2022.pdf) | [mgvm2022](workers/G2_multigpu.md) | core |
| 2023 / published | <a id="transfw2023"></a>[Trans-FW: Short Circuiting Page Table Walk in Multi-GPU Systems via Remote Forwarding](https://par.nsf.gov/servlets/purl/10422616) | [transfw2023](papers/transfw2023.pdf) | [transfw2023](workers/G2_multigpu.md) | core |
| 2024 / published | <a id="barre2024"></a>[Barre Chord: Efficient Virtual Memory Translation for Multi-Chip-Module GPUs](https://seonjinna.github.io/assets/pdf/BarreChord_isca24.pdf) | [barre2024](papers/barre2024.pdf) | [barre2024](workers/G2_multigpu.md) | core |
| 2025 / published | <a id="clap2025"></a>[Leveraging Chiplet-Locality for Efficient Memory Mapping in Multi-Chip Module GPUs](https://skku-compasslab.github.io/compasslab/publications/papers/250715-micro-clap/) | — | — | core |
| 2026 / published | <a id="clover2026"></a>[Clover: Storage-Efficient Page Table Replication in Wafer-Scale GPUs](https://scholarx.skku.edu/item/cb5add4f-a64d-45b1-8982-b7b845ec6d77) | — | — | core |
| 2026 / published | <a id="hdpat2026"></a>[HDPAT: Hierarchical Distributed Page Address Translation for Wafer-Scale GPUs](https://sarchlab.org/hdpat_hpca_2026.pdf) | [hdpat2026](papers/hdpat2026.pdf) | [hdpat2026](workers/G2_multigpu.md) | core |
| 2026 / preprint | <a id="wafertrans2026"></a>[WaferTrans: Enabling IOMMU-free Distributed Virtual Address Translation for Wafer-scale GPUs](https://arxiv.org/pdf/2609.06125v1) | [wafertrans2026](papers/wafertrans2026.pdf) | [wafertrans2026](workers/G2_multigpu.md) | core |

- **leasttlb2021** Improving Address Translation in Multi-GPUs via Sharing and Spilling aware TLB Design — Bingyao Li, Jieming Yin, Youtao Zhang, Xulong Tang；MICRO '21: 54th Annual IEEE/ACM International Symposium on Microarchitecture, October 18-22, 2021, Virtual Event, Greece；10.1145/3466752.3480083；source=https://jiemingyin.github.io/docs/MICRO2021.pdf；topic=multigpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 leasttlb2021：PDF [papers/leasttlb2021.pdf](papers/leasttlb2021.pdf); text [papers/leasttlb2021.txt](papers/leasttlb2021.txt); 报告 [workers/G2_multigpu.md](workers/G2_multigpu.md); 证据卡 [workers/G2_multigpu-papers.json](workers/G2_multigpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2021；venue=MICRO '21: 54th Annual IEEE/ACM International Symposium on Microarchitecture, October 18-22, 2021, Virtual Event, Greece；doi=10.1145/3466752.3480083；source=https://jiemingyin.github.io/docs/MICRO2021.pdf；root_spotchecked=False
- **mgvm2022** Designing Virtual Memory System of MCM GPUs — Pratheek B, Neha Jawalkar, Arkaprava Basu；MICRO 2022 (evidence: L1618 mentions MICRO 2022; cross-file Barre2024 L1919-L1921 cites MICRO 2022, pp. 404-422)；doi未知；source=https://www.csa.iisc.ac.in/~arkapravab/papers/MICRO22_MGvm.pdf；topic=multigpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知
  - 版本 mgvm2022：PDF [papers/mgvm2022.pdf](papers/mgvm2022.pdf); text [papers/mgvm2022.txt](papers/mgvm2022.txt); 报告 [workers/G2_multigpu.md](workers/G2_multigpu.md); 证据卡 [workers/G2_multigpu-papers.json](workers/G2_multigpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2022；venue=MICRO 2022 (evidence: L1618 mentions MICRO 2022; cross-file Barre2024 L1919-L1921 cites MICRO 2022, pp. 404-422)；doi=未知；source=https://www.csa.iisc.ac.in/~arkapravab/papers/MICRO22_MGvm.pdf；root_spotchecked=True
- **transfw2023** Trans-FW: Short Circuiting Page Table Walk in Multi-GPU Systems via Remote Forwarding — Bingyao Li, Jieming Yin, Anup Holey, Youtao Zhang, Jun Yang, Xulong Tang；2023 IEEE International Symposium on High-Performance Computer Architecture (HPCA)；10.1109/HPCA56546.2023.10071054；source=https://par.nsf.gov/servlets/purl/10422616；topic=multigpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知
  - 版本 transfw2023：PDF [papers/transfw2023.pdf](papers/transfw2023.pdf); text [papers/transfw2023.txt](papers/transfw2023.txt); 报告 [workers/G2_multigpu.md](workers/G2_multigpu.md); 证据卡 [workers/G2_multigpu-papers.json](workers/G2_multigpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2023；venue=2023 IEEE International Symposium on High-Performance Computer Architecture (HPCA)；doi=10.1109/HPCA56546.2023.10071054；source=https://par.nsf.gov/servlets/purl/10422616；root_spotchecked=True
- **barre2024** Barre Chord: Efficient Virtual Memory Translation for Multi-Chip-Module GPUs — Yuan Feng, Seonjin Na, Hyesoon Kim, Hyeran Jeon；2024 ACM/IEEE 51st Annual International Symposium on Computer Architecture (ISCA)；10.1109/ISCA59077.2024.00065；source=https://seonjinna.github.io/assets/pdf/BarreChord_isca24.pdf；topic=multigpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知
  - 版本 barre2024：PDF [papers/barre2024.pdf](papers/barre2024.pdf); text [papers/barre2024.txt](papers/barre2024.txt); 报告 [workers/G2_multigpu.md](workers/G2_multigpu.md); 证据卡 [workers/G2_multigpu-papers.json](workers/G2_multigpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2024；venue=2024 ACM/IEEE 51st Annual International Symposium on Computer Architecture (ISCA)；doi=10.1109/ISCA59077.2024.00065；source=https://seonjinna.github.io/assets/pdf/BarreChord_isca24.pdf；root_spotchecked=True
- **clap2025** Leveraging Chiplet-Locality for Efficient Memory Mapping in Multi-Chip Module GPUs — Junhyeok Park, Sungbin Jang, Osang Kwon, Yongho Lee, Seokin Hong；MICRO 2025；10.1145/3725843.3756090；source=https://skku-compasslab.github.io/compasslab/publications/papers/250715-micro-clap/；topic=multigpu；status=published；relevance=core；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: 作者摘要：页大小/连续性与chiplet数据局部性的权衡；出版社PDF403，机制细节待全文。
  - 版本 clap2025：无本地文件；reading=abstract_or_indexed_excerpt；status=published；year=2025；venue=MICRO 2025；doi=10.1145/3725843.3756090；source=https://skku-compasslab.github.io/compasslab/publications/papers/250715-micro-clap/；root_spotchecked=False
    - notes: 作者摘要：页大小/连续性与chiplet数据局部性的权衡；出版社PDF403，机制细节待全文。
- **clover2026** Clover: Storage-Efficient Page Table Replication in Wafer-Scale GPUs — Sungbin Jang, Yongho Lee, Seokin Hong；IEEE Computer Architecture Letters 25(2)；10.1109/LCA.2026.3718473；source=https://scholarx.skku.edu/item/cb5add4f-a64d-45b1-8982-b7b845ec6d77；topic=multigpu；status=published；relevance=core；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: 机构摘要：分区与k-hop副本放置，限制访问跳数并减小全复制存储；一致性细节待全文。
  - 版本 clover2026：无本地文件；reading=abstract_or_indexed_excerpt；status=published；year=2026；venue=IEEE Computer Architecture Letters 25(2)；doi=10.1109/LCA.2026.3718473；source=https://scholarx.skku.edu/item/cb5add4f-a64d-45b1-8982-b7b845ec6d77；root_spotchecked=False
    - notes: 机构摘要：分区与k-hop副本放置，限制访问跳数并减小全复制存储；一致性细节待全文。
- **hdpat2026** HDPAT: Hierarchical Distributed Page Address Translation for Wafer-Scale GPUs — Daoxuan Xu, Ying Li, Yuwei Sun, Jie Ren, Yifan Sun；HPCA 2026；doi未知；source=https://sarchlab.org/hdpat_hpca_2026.pdf；topic=multigpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=https://2026.hpca-conf.org/details/hpca-2026-main-conference/45/HDPAT-Hierarchical-Distributed-Page-Address-Translation-for-Wafer-Scale-GPUs
  - 版本 hdpat2026：PDF [papers/hdpat2026.pdf](papers/hdpat2026.pdf); text [papers/hdpat2026.txt](papers/hdpat2026.txt); 报告 [workers/G2_multigpu.md](workers/G2_multigpu.md); 证据卡 [workers/G2_multigpu-papers.json](workers/G2_multigpu-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2026；venue=HPCA 2026；doi=未知；source=https://sarchlab.org/hdpat_hpca_2026.pdf；root_spotchecked=True
- **wafertrans2026** WaferTrans: Enabling IOMMU-free Distributed Virtual Address Translation for Wafer-scale GPUs — Xinru Tang, Jingxiang Hou, Guanghong Wu, Yang Hu, Shouyi Yin；arXiv preprint arXiv:2609.06125v1；doi未知；source=https://arxiv.org/pdf/2609.06125v1；topic=multigpu；status=preprint；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知
  - 版本 wafertrans2026：PDF [papers/wafertrans2026.pdf](papers/wafertrans2026.pdf); text [papers/wafertrans2026.txt](papers/wafertrans2026.txt); 报告 [workers/G2_multigpu.md](workers/G2_multigpu.md); 证据卡 [workers/G2_multigpu-papers.json](workers/G2_multigpu-papers.json)；reading=fulltext_worker_reviewed；status=preprint；year=2026；venue=arXiv preprint arXiv:2609.06125v1；doi=未知；source=https://arxiv.org/pdf/2609.06125v1；root_spotchecked=True

## io

| 年份/状态 | 题目 | PDF | 证据卡 | 相关性 |
|---|---|---|---|---|
| 2015 / published | <a id="riommu2015"></a>[rIOMMU: Efficient IOMMU for I/O Devices that Employ Ring Buffers](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf) | [riommu2015](papers/riommu2015.pdf) | [riommu2015](workers/G3_io.md) | core |
| 2015 / published | <a id="iommu_scalable2015"></a>[Utilizing the IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf) | [iommu_scalable2015](papers/iommu_scalable2015.pdf) | [iommu_scalable2015](workers/G3_io.md) | core |
| 2018 / published | <a id="ssr2018"></a>[Interference from GPU System Service Requests](https://www.csa.iisc.ac.in/~arkapravab/papers/iiswc2018_interference.pdf) | [ssr2018](papers/ssr2018.pdf) | [ssr2018](workers/G3_io.md) | nearby |
| 2023 / published | <a id="iotlb_wall2023"></a>[Overcoming the IOTLB wall for multi-100-Gbps Linux-based networking](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) | [iotlb_wall2023](papers/iotlb_wall2023.pdf) | [iotlb_wall2023](workers/G3_io.md) | core |
| 2024 / published | <a id="fastsafe2024"></a>[Fast & Safe IO Memory Protection](https://www.qizhecai.com/papers/fands.pdf) | [fastsafe2024](papers/fastsafe2024.pdf) | [fastsafe2024](workers/G3_io.md) | core |
| 2025 / published | <a id="bermuda2025"></a>[Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems](https://arxiv.org/pdf/2508.19670) | [bermuda2025](papers/bermuda2025.pdf) | [bermuda2025](workers/G3_io.md) | core |
| 2025 / published | <a id="spimpact2025"></a>[SP-IMPact: A Framework for Static Partitioning Interference Mitigation and Performance Analysis](https://arxiv.org/pdf/2501.16245) | [spimpact2025](papers/spimpact2025.pdf) | [spimpact2025](workers/G7_supplement.md) | nearby |

- **riommu2015** rIOMMU: Efficient IOMMU for I/O Devices that Employ Ring Buffers — Moshe Malka, Nadav Amit, Muli Ben-Yehuda, Dan Tsafrir；ASPLOS '15 (ACM International Conference on Architectural Support for Programming Languages and Operating Systems)；10.1145/2694344.2694355；source=https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf；topic=io；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 riommu2015：PDF [papers/riommu2015.pdf](papers/riommu2015.pdf); text [papers/riommu2015.txt](papers/riommu2015.txt); 报告 [workers/G3_io.md](workers/G3_io.md); 证据卡 [workers/G3_io-papers.json](workers/G3_io-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2015；venue=ASPLOS '15 (ACM International Conference on Architectural Support for Programming Languages and Operating Systems)；doi=10.1145/2694344.2694355；source=https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf；root_spotchecked=False
- **iommu_scalable2015** Utilizing the IOMMU Scalably — Omer Peleg, Adam Morrison, Benjamin Serebrin, Dan Tsafrir；USENIX ATC '15 (USENIX Annual Technical Conference)；doi未知；source=https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf；topic=io；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 iommu_scalable2015：PDF [papers/iommu_scalable2015.pdf](papers/iommu_scalable2015.pdf); text [papers/iommu_scalable2015.txt](papers/iommu_scalable2015.txt); 报告 [workers/G3_io.md](workers/G3_io.md); 证据卡 [workers/G3_io-papers.json](workers/G3_io-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2015；venue=USENIX ATC '15 (USENIX Annual Technical Conference)；doi=未知；source=https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf；root_spotchecked=False
- **ssr2018** Interference from GPU System Service Requests — Arkaprava Basu, Joseph L. Greathouse, Guru Venkataramani, Ján Veselý；IISWC 2018；doi未知；source=https://www.csa.iisc.ac.in/~arkapravab/papers/iiswc2018_interference.pdf；topic=io；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://www.csa.iisc.ac.in/~arkapravab/papers.html
  - metadata_note: 作者列表题名为Interference from Accelerator System Service Requests，本地稿题名为Interference from GPU System Service Requests；保留版本题名差异。
  - 版本 ssr2018：PDF [papers/ssr2018.pdf](papers/ssr2018.pdf); text [papers/ssr2018.txt](papers/ssr2018.txt); 报告 [workers/G3_io.md](workers/G3_io.md); 证据卡 [workers/G3_io-papers.json](workers/G3_io-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2018；venue=IISWC 2018；doi=未知；source=https://www.csa.iisc.ac.in/~arkapravab/papers/iiswc2018_interference.pdf；root_spotchecked=False
    - metadata_note: 作者列表题名为Interference from Accelerator System Service Requests，本地稿题名为Interference from GPU System Service Requests；保留版本题名差异。
- **iotlb_wall2023** Overcoming the IOTLB wall for multi-100-Gbps Linux-based networking — Alireza Farshin, Luigi Rizzo, Khaled Elmeleegy, Dejan Kostić；PeerJ Computer Science 9:e1385；10.7717/peerj-cs.1385；source=https://dejankostic.com/documents/publications/iotlb-peerj23.pdf；topic=io；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 iotlb_wall2023：PDF [papers/iotlb_wall2023.pdf](papers/iotlb_wall2023.pdf); text [papers/iotlb_wall2023.txt](papers/iotlb_wall2023.txt); 报告 [workers/G3_io.md](workers/G3_io.md); 证据卡 [workers/G3_io-papers.json](workers/G3_io-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2023；venue=PeerJ Computer Science 9:e1385；doi=10.7717/peerj-cs.1385；source=https://dejankostic.com/documents/publications/iotlb-peerj23.pdf；root_spotchecked=False
- **fastsafe2024** Fast & Safe IO Memory Protection — Benny Rubin, Saksham Agarwal, Qizhe Cai, Rachit Agarwal；ACM SIGOPS 30th Symposium on Operating Systems Principles (SOSP '24), November 4-6, 2024, Austin, TX, USA；doi未知；source=https://www.qizhecai.com/papers/fands.pdf；topic=io；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知
  - 版本 fastsafe2024：PDF [papers/fastsafe2024.pdf](papers/fastsafe2024.pdf); text [papers/fastsafe2024.txt](papers/fastsafe2024.txt); 报告 [workers/G3_io.md](workers/G3_io.md); 证据卡 [workers/G3_io-papers.json](workers/G3_io-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2024；venue=ACM SIGOPS 30th Symposium on Operating Systems Principles (SOSP '24), November 4-6, 2024, Austin, TX, USA；doi=未知；source=https://www.qizhecai.com/papers/fands.pdf；root_spotchecked=True
- **bermuda2025** Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems — Diogo Costa, José Martins, Sandro Pinto；RTCSA 2025；10.1109/RTCSA66114.2025.00027；source=https://arxiv.org/pdf/2508.19670；topic=io；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=https://ieeexplore.ieee.org/document/11153110/
  - 版本 bermuda2025：PDF [papers/bermuda2025.pdf](papers/bermuda2025.pdf); text [papers/bermuda2025.txt](papers/bermuda2025.txt); 报告 [workers/G3_io.md](workers/G3_io.md); 证据卡 [workers/G3_io-papers.json](workers/G3_io-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2025；venue=RTCSA 2025；doi=10.1109/RTCSA66114.2025.00027；source=https://arxiv.org/pdf/2508.19670；root_spotchecked=True
- **spimpact2025** SP-IMPact: A Framework for Static Partitioning Interference Mitigation and Performance Analysis — Diogo Costa, Gonçalo Moreira, Afonso Oliveira, José Martins, Sandro Pinto；NG-RES 2025 (OASIcs)；10.4230/OASIcs.NG-RES.2025.2；source=https://arxiv.org/pdf/2501.16245；topic=io；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 spimpact2025：PDF [papers/spimpact2025.pdf](papers/spimpact2025.pdf); text [papers/spimpact2025.txt](papers/spimpact2025.txt); 报告 [workers/G7_supplement.md](workers/G7_supplement.md); 证据卡 [workers/G7_supplement-papers.json](workers/G7_supplement-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2025；venue=NG-RES 2025 (OASIcs)；doi=10.4230/OASIcs.NG-RES.2025.2；source=https://arxiv.org/pdf/2501.16245；root_spotchecked=False

## accelerators

| 年份/状态 | 题目 | PDF | 证据卡 | 相关性 |
|---|---|---|---|---|
| 2015 / published | <a id="border2015"></a>[Border Control: Sandboxing Accelerators](https://research.cs.wisc.edu/multifacet/papers/micro15_border_control.pdf) | [border2015](papers/border2015.pdf) | [border2015](workers/G4_accelerators.md) | core |
| 2016 / published | <a id="svm2016"></a>[Observations and Opportunities in Architecting Shared Virtual Memory for Heterogeneous Systems](https://www.csa.iisc.ac.in/~arkapravab/papers/ispass16.pdf) | [svm2016](papers/svm2016.pdf) | [svm2016](workers/G4_accelerators.md) | foundation |
| 2017 / published | <a id="ontable2017"></a>[Efficient Virtual Memory Sharing via On-Accelerator Page Table Walking in Heterogeneous Embedded SoCs](https://doi.org/10.1145/3126560) | — | — | core |
| 2017 / published | <a id="accelerator_translation2017"></a>[Supporting Address Translation for Accelerator-Centric Architectures](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf) | [accelerator_translation2017](papers/accelerator_translation2017.pdf) | [accelerator_translation2017](workers/G4_accelerators.md) | core |
| 2018 / published | <a id="dvm2018"></a>[Devirtualizing Memory in Heterogeneous Systems](https://research.cs.wisc.edu/multifacet/papers/asplos18_dvm.pdf) | [dvm2018](papers/dvm2018.pdf) | [dvm2018](workers/G4_accelerators.md) | core |
| 2018 / published | <a id="socdma2018"></a>[Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine](https://arxiv.org/pdf/1808.09751) | [socdma2018](papers/socdma2018.pdf) | [socdma2018](workers/G4_accelerators.md) | core |
| 2020 / preprint | <a id="sparta2020"></a>[SPARTA: A Divide and Conquer Approach to Address Translation for Accelerators](https://arxiv.org/pdf/2001.07045) | [sparta2020](papers/sparta2020.pdf) | [sparta2020](workers/G4_accelerators.md) | core |
| 2023 / published | <a id="cryptommu2023"></a>[CryptoMMU: Enabling Scalable and Secure Access Control of Third-Party Accelerators](https://www.cs.yale.edu/homes/abhishek/alam-micro23.pdf) | — | — | core |
| 2025 / published | <a id="riscvsva2025"></a>[Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs](https://arxiv.org/pdf/2502.17398v1) | [riscvsva2025](papers/riscvsva2025.pdf) | [riscvsva2025](workers/G4_accelerators.md) | core |
| 2026 / accepted_upcoming | <a id="sharemmu2026"></a>[ShareMMU: Supporting Secure Address Translation Sharing among Untrusted Accelerators](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf) | [sharemmu2026](papers/sharemmu2026.pdf) | [sharemmu2026](workers/G4_accelerators.md) | core |

- **border2015** Border Control: Sandboxing Accelerators — Lena E. Olson, Jason Power, Mark D. Hill, David A. Wood；MICRO-48；10.1145/2830772.2830819；source=https://research.cs.wisc.edu/multifacet/papers/micro15_border_control.pdf；topic=accelerators；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 border2015：PDF [papers/border2015.pdf](papers/border2015.pdf); text [papers/border2015.txt](papers/border2015.txt); 报告 [workers/G4_accelerators.md](workers/G4_accelerators.md); 证据卡 [workers/G4_accelerators-papers.json](workers/G4_accelerators-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2015；venue=MICRO-48；doi=10.1145/2830772.2830819；source=https://research.cs.wisc.edu/multifacet/papers/micro15_border_control.pdf；root_spotchecked=False
- **svm2016** Observations and Opportunities in Architecting Shared Virtual Memory for Heterogeneous Systems — Jan Vesely, Arkaprava Basu, Mark Oskin, Gabriel H. Loh, Abhishek Bhattacharjee；2016 IEEE International Symposium on Performance Analysis of Systems and Software (ISPASS)；doi未知；source=https://www.csa.iisc.ac.in/~arkapravab/papers/ispass16.pdf；topic=accelerators；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 svm2016：PDF [papers/svm2016.pdf](papers/svm2016.pdf); text [papers/svm2016.txt](papers/svm2016.txt); 报告 [workers/G4_accelerators.md](workers/G4_accelerators.md); 证据卡 [workers/G4_accelerators-papers.json](workers/G4_accelerators-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2016；venue=2016 IEEE International Symposium on Performance Analysis of Systems and Software (ISPASS)；doi=未知；source=https://www.csa.iisc.ac.in/~arkapravab/papers/ispass16.pdf；root_spotchecked=False
- **ontable2017** Efficient Virtual Memory Sharing via On-Accelerator Page Table Walking in Heterogeneous Embedded SoCs — Pirmin Vogel, Andreas Kurth, Johannes Weinbuch, Andrea Marongiu, Luca Benini；ACM Transactions on Embedded Computing Systems 16(5s)；10.1145/3126560；source=https://doi.org/10.1145/3126560；topic=accelerators；status=published；relevance=core；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: ESWEEK官方节目及机构题录可核；ETH下载500、机构及出版社下载403，未获全文。
  - 版本 ontable2017：无本地文件；reading=abstract_or_indexed_excerpt；status=published；year=2017；venue=ACM Transactions on Embedded Computing Systems 16(5s)；doi=10.1145/3126560；source=https://doi.org/10.1145/3126560；root_spotchecked=False
    - notes: ESWEEK官方节目及机构题录可核；ETH下载500、机构及出版社下载403，未获全文。
- **accelerator_translation2017** Supporting Address Translation for Accelerator-Centric Architectures — Jason Cong, Zhenman Fang, Yuchen Hao, Glenn Reinman；IEEE International Symposium on High Performance Computer Architecture (HPCA)；10.1109/HPCA.2017.19；source=https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf；topic=accelerators；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 accelerator_translation2017：PDF [papers/accelerator_translation2017.pdf](papers/accelerator_translation2017.pdf); text [papers/accelerator_translation2017.txt](papers/accelerator_translation2017.txt); 报告 [workers/G4_accelerators.md](workers/G4_accelerators.md); 证据卡 [workers/G4_accelerators-papers.json](workers/G4_accelerators-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2017；venue=IEEE International Symposium on High Performance Computer Architecture (HPCA)；doi=10.1109/HPCA.2017.19；source=https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf；root_spotchecked=False
- **dvm2018** Devirtualizing Memory in Heterogeneous Systems — Swapnil Haria, Mark D. Hill, Michael M. Swift；ASPLOS'18；10.1145/3173162.3173194；source=https://research.cs.wisc.edu/multifacet/papers/asplos18_dvm.pdf；topic=accelerators；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 dvm2018：PDF [papers/dvm2018.pdf](papers/dvm2018.pdf); text [papers/dvm2018.txt](papers/dvm2018.txt); 报告 [workers/G4_accelerators.md](workers/G4_accelerators.md); 证据卡 [workers/G4_accelerators-papers.json](workers/G4_accelerators-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2018；venue=ASPLOS'18；doi=10.1145/3173162.3173194；source=https://research.cs.wisc.edu/multifacet/papers/asplos18_dvm.pdf；root_spotchecked=False
- **socdma2018** Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine — Andreas Kurth, Pirmin Vogel, Andrea Marongiu, Luca Benini；Proceedings of the 36th IEEE International Conference on Computer Design (ICCD)；doi未知；source=https://arxiv.org/pdf/1808.09751；topic=accelerators；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 socdma2018：PDF [papers/socdma2018.pdf](papers/socdma2018.pdf); text [papers/socdma2018.txt](papers/socdma2018.txt); 报告 [workers/G4_accelerators.md](workers/G4_accelerators.md); 证据卡 [workers/G4_accelerators-papers.json](workers/G4_accelerators-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2018；venue=Proceedings of the 36th IEEE International Conference on Computer Design (ICCD)；doi=未知；source=https://arxiv.org/pdf/1808.09751；root_spotchecked=False
- **sparta2020** SPARTA: A Divide and Conquer Approach to Address Translation for Accelerators — Javier Picorel, Seyed Alireza Sanaee Kohroudi, Zi Yan, Abhishek Bhattacharjee, Babak Falsafi, Djordje Jevdjic；venue未知；doi未知；source=https://arxiv.org/pdf/2001.07045；topic=accelerators；status=preprint；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 sparta2020：PDF [papers/sparta2020.pdf](papers/sparta2020.pdf); text [papers/sparta2020.txt](papers/sparta2020.txt); 报告 [workers/G4_accelerators.md](workers/G4_accelerators.md); 证据卡 [workers/G4_accelerators-papers.json](workers/G4_accelerators-papers.json)；reading=fulltext_worker_reviewed；status=preprint；year=2020；venue=未知；doi=未知；source=https://arxiv.org/pdf/2001.07045；root_spotchecked=False
- **cryptommu2023** CryptoMMU: Enabling Scalable and Secure Access Control of Third-Party Accelerators — Faiz Alam, Hyokeun Lee, Abhishek Bhattacharjee, Amro Awad；MICRO 2023；doi未知；source=https://www.cs.yale.edu/homes/abhishek/alam-micro23.pdf；topic=accelerators；status=published；relevance=core；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: 搜索缓存可见作者原稿，直接下载404；仅核题录与摘要，未完成全文核读。
  - 版本 cryptommu2023：无本地文件；reading=abstract_or_indexed_excerpt；status=published；year=2023；venue=MICRO 2023；doi=未知；source=https://www.cs.yale.edu/homes/abhishek/alam-micro23.pdf；root_spotchecked=False
    - notes: 搜索缓存可见作者原稿，直接下载404；仅核题录与摘要，未完成全文核读。
- **riscvsva2025** Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs — Cyril Koenig, Enrico Zelioli, Luca Benini；DATE 2025；10.23919/DATE64628.2025.10992962；source=https://arxiv.org/pdf/2502.17398v1；topic=accelerators；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://past.date-conference.com/proceedings-archive/2025/START.pdf
  - metadata_note: 本地保存arXiv作者稿；DATE2025官方program核定会议，DOI由题录交叉核定。
  - 版本 riscvsva2025：PDF [papers/riscvsva2025.pdf](papers/riscvsva2025.pdf); text [papers/riscvsva2025.txt](papers/riscvsva2025.txt); 报告 [workers/G4_accelerators.md](workers/G4_accelerators.md); 证据卡 [workers/G4_accelerators-papers.json](workers/G4_accelerators-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2025；venue=DATE 2025；doi=10.23919/DATE64628.2025.10992962；source=https://arxiv.org/pdf/2502.17398v1；root_spotchecked=False
    - metadata_note: 本地保存arXiv作者稿；DATE2025官方program核定会议，DOI由题录交叉核定。
- **sharemmu2026** ShareMMU: Supporting Secure Address Translation Sharing among Untrusted Accelerators — Faiz Alam, Mohamed Tarek Ibn Ziad, Yuanchao Xu, Abdulrahman Mahmoud, Greg Byrd, Aamer Jaleel；MICRO 2026；doi未知；source=https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf；topic=accelerators；status=accepted_upcoming；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted
  - 版本 sharemmu2026：PDF [papers/sharemmu2026.pdf](papers/sharemmu2026.pdf); text [papers/sharemmu2026.txt](papers/sharemmu2026.txt); 报告 [workers/G4_accelerators.md](workers/G4_accelerators.md); 证据卡 [workers/G4_accelerators-papers.json](workers/G4_accelerators-papers.json)；reading=fulltext_worker_reviewed；status=accepted_upcoming；year=2026；venue=MICRO 2026；doi=未知；source=https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf；root_spotchecked=True

## lifecycle

| 年份/状态 | 题目 | PDF | 证据卡 | 相关性 |
|---|---|---|---|---|
| 2010 / published | <a id="unitd2010"></a>[UNified Instruction/Translation/Data (UNITD) Coherence: One Protocol to Rule Them All](https://people.ee.duke.edu/~sorin/papers/hpca10_unitd.pdf) | [unitd2010](papers/unitd2010.pdf) | [unitd2010](workers/G6_lifecycle.md) | foundation |
| 2011 / published | <a id="didi2011"></a>[DiDi: Mitigating The Performance Impact of TLB Shootdowns Using A Shared TLB Directory](https://cgi.di.uoa.gr/~vkarakos/papers/pact11_didi_mitigating_tlb_shootdowns.pdf) | [didi2011](papers/didi2011.pdf) | [didi2011](workers/G6_lifecycle.md) | core |
| 2017 / published | <a id="site2017"></a>[Avoiding TLB Shootdowns through Self-invalidating TLB Entries](https://www.csa.iisc.ac.in/~arkapravab/papers/pact2017_final_version.pdf) | [site2017](papers/site2017.pdf) | [site2017](workers/G6_lifecycle.md) | core |
| 2017 / published | <a id="hatric2017"></a>[Hardware Translation Coherence for Virtualized Systems](https://arxiv.org/pdf/1701.07517) | [hatric2017](papers/hatric2017.pdf) | [hatric2017](workers/G6_lifecycle.md) | core |
| 2018 / published | <a id="granularity2018"></a>[A Case for Granularity Aware Page Migration](https://www.csa.iisc.ac.in/~arkapravab/papers/ics_18_granularity_aware_migration.pdf) | [granularity2018](papers/granularity2018.pdf) | [granularity2018](workers/G6_lifecycle.md) | nearby |
| 2018 / published | <a id="latr2018"></a>[Latr: Lazy Translation Coherence](https://sanidhya.github.io/pubs/2018/latr.pdf) | [latr2018](papers/latr2018.pdf) | [latr2018](workers/G6_lifecycle.md) | core |
| 2020 / published | <a id="batch_uvm2020"></a>[Batch-Aware Unified Memory Management in GPUs for Irregular Workloads](https://hparch.gatech.edu/papers/kim-asplos20.pdf) | [batch_uvm2020](papers/batch_uvm2020.pdf) | [batch_uvm2020](workers/G6_lifecycle.md) | nearby |
| 2020 / published | <a id="griffin2020"></a>[Griffin: Hardware-Software Support for Efficient Page Migration in Multi-GPU Systems](https://sarchlab.org/hpca2020.pdf) | [griffin2020](papers/griffin2020.pdf) | [griffin2020](workers/G6_lifecycle.md) | core |
| 2021 / published | <a id="fptmigration2021"></a>[Efficient Physical Page Migrations in Shared Virtual Memory Reconfigurable Computing Systems](https://www.esa.informatik.tu-darmstadt.de/assets/publications/materials/2021/2021_FPT_TK.pdf) | [fptmigration2021](papers/fptmigration2021.pdf) | [fptmigration2021](workers/G6_lifecycle.md) | core |
| 2023 / published | <a id="idyll2023"></a>[IDYLL: Enhancing Page Translation in Multi-GPUs via Light Weight PTE Invalidations](https://par.nsf.gov/servlets/purl/10492257) | [idyll2023](papers/idyll2023.pdf) | [idyll2023](workers/G6_lifecycle.md) | core |
| 2024 / published | <a id="bypassd2024"></a>[BypassD: Enabling fast userspace access to shared SSDs](https://cgi.di.uoa.gr/~vkarakos/papers/asplos24_bypassd.pdf) | [bypassd2024](papers/bypassd2024.pdf) | [bypassd2024](workers/G6_lifecycle.md) | core |
| 2024 / published | <a id="grit2024"></a>[GRIT: Enhancing Multi-GPU Performance with Fine-Grained Dynamic Page Placement](https://yueqiwang42.github.io/assets/pdf/papers/GRIT_HPCA24.pdf) | [grit2024](papers/grit2024.pdf) | [grit2024](workers/G8_grit_colt.md) | core |
| 2024 / published | <a id="suv2024"></a>[SUV: Static analysis guided Unified Virtual Memory](https://www.csa.iisc.ac.in/~arkapravab/papers/MICRO24_SUV.pdf) | [suv2024](papers/suv2024.pdf) | [suv2024](workers/G6_lifecycle.md) | nearby |
| 2026 / published | <a id="observuvm2026"></a>[Observability-aided GPU Memory Oversubscription](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) | [observuvm2026](papers/observuvm2026.pdf) | [observuvm2026](workers/G6_lifecycle.md) | nearby |
| 2026 / published | <a id="shadowupdate2026"></a>[Reducing Page Faults via Invalidation-Based Mapping Propagation in Multi-GPU Systems](https://yonsei.elsevierpure.com/en/publications/reducing-page-faults-via-invalidation-based-mapping-propagation-i/) | — | — | core |

- **unitd2010** UNified Instruction/Translation/Data (UNITD) Coherence: One Protocol to Rule Them All — Bogdan F. Romanescu, Alvin R. Lebeck, Daniel J. Sorin, Anne Bracy；HPCA 2010；10.1109/HPCA.2010.5416643；source=https://people.ee.duke.edu/~sorin/papers/hpca10_unitd.pdf；topic=lifecycle；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 unitd2010：PDF [papers/unitd2010.pdf](papers/unitd2010.pdf); text [papers/unitd2010.txt](papers/unitd2010.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2010；venue=HPCA 2010；doi=10.1109/HPCA.2010.5416643；source=https://people.ee.duke.edu/~sorin/papers/hpca10_unitd.pdf；root_spotchecked=False
- **didi2011** DiDi: Mitigating The Performance Impact of TLB Shootdowns Using A Shared TLB Directory — Carlos Villavieja, Vasileios Karakostas, Lluis Vilanova, Yoav Etsion, Alex Ramirez, Avi Mendelson, Nacho Navarro, Adrián Cristal, Osman S. Unsal；PACT 2011；10.1109/PACT.2011.65；source=https://cgi.di.uoa.gr/~vkarakos/papers/pact11_didi_mitigating_tlb_shootdowns.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 didi2011：PDF [papers/didi2011.pdf](papers/didi2011.pdf); text [papers/didi2011.txt](papers/didi2011.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2011；venue=PACT 2011；doi=10.1109/PACT.2011.65；source=https://cgi.di.uoa.gr/~vkarakos/papers/pact11_didi_mitigating_tlb_shootdowns.pdf；root_spotchecked=False
- **site2017** Avoiding TLB Shootdowns through Self-invalidating TLB Entries — Amro Awad, Arkaprava Basu, Sergey Blagodurov, Yan Solihin, Gabriel H. Loh；PACT 2017；doi未知；source=https://www.csa.iisc.ac.in/~arkapravab/papers/pact2017_final_version.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 site2017：PDF [papers/site2017.pdf](papers/site2017.pdf); text [papers/site2017.txt](papers/site2017.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2017；venue=PACT 2017；doi=未知；source=https://www.csa.iisc.ac.in/~arkapravab/papers/pact2017_final_version.pdf；root_spotchecked=False
- **hatric2017** Hardware Translation Coherence for Virtualized Systems — Zi Yan, Guilherme Cox, Ján Veselý, Abhishek Bhattacharjee；ISCA 2017；10.1145/3079856.3080211；source=https://arxiv.org/pdf/1701.07517；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 hatric2017：PDF [papers/hatric2017.pdf](papers/hatric2017.pdf); text [papers/hatric2017.txt](papers/hatric2017.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2017；venue=ISCA 2017；doi=10.1145/3079856.3080211；source=https://arxiv.org/pdf/1701.07517；root_spotchecked=False
- **granularity2018** A Case for Granularity Aware Page Migration — Jee Ho Ryoo, Lizy K. John, Arkaprava Basu；ICS '18；10.1145/3205289.3208064；source=https://www.csa.iisc.ac.in/~arkapravab/papers/ics_18_granularity_aware_migration.pdf；topic=lifecycle；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 granularity2018：PDF [papers/granularity2018.pdf](papers/granularity2018.pdf); text [papers/granularity2018.txt](papers/granularity2018.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2018；venue=ICS '18；doi=10.1145/3205289.3208064；source=https://www.csa.iisc.ac.in/~arkapravab/papers/ics_18_granularity_aware_migration.pdf；root_spotchecked=False
- **latr2018** Latr: Lazy Translation Coherence — Mohan Kumar, Steffen Maass, Sanidhya Kashyap, Ján Veselý, Zi Yan, Taesoo Kim, Abhishek Bhattacharjee, Tushar Krishna；ASPLOS '18；10.1145/3173162.3173198；source=https://sanidhya.github.io/pubs/2018/latr.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 latr2018：PDF [papers/latr2018.pdf](papers/latr2018.pdf); text [papers/latr2018.txt](papers/latr2018.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2018；venue=ASPLOS '18；doi=10.1145/3173162.3173198；source=https://sanidhya.github.io/pubs/2018/latr.pdf；root_spotchecked=False
- **batch_uvm2020** Batch-Aware Unified Memory Management in GPUs for Irregular Workloads — Hyojong Kim, Jaewoong Sim, Prasun Gera, Ramyad Hadidi, Hyesoon Kim；ASPLOS '20；10.1145/3373376.3378529；source=https://hparch.gatech.edu/papers/kim-asplos20.pdf；topic=lifecycle；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 batch_uvm2020：PDF [papers/batch_uvm2020.pdf](papers/batch_uvm2020.pdf); text [papers/batch_uvm2020.txt](papers/batch_uvm2020.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2020；venue=ASPLOS '20；doi=10.1145/3373376.3378529；source=https://hparch.gatech.edu/papers/kim-asplos20.pdf；root_spotchecked=False
- **griffin2020** Griffin: Hardware-Software Support for Efficient Page Migration in Multi-GPU Systems — Trinayan Baruah, Yifan Sun, Ali Tolga Dinçer, Saiful A. Mojumder, José L. Abellán, Yash Ukidave, Ajay Joshi, Norman Rubin, John Kim, David Kaeli；2020 IEEE International Symposium on High Performance Computer Architecture (HPCA)；10.1109/HPCA47549.2020.00055；source=https://sarchlab.org/hpca2020.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 griffin2020：PDF [papers/griffin2020.pdf](papers/griffin2020.pdf); text [papers/griffin2020.txt](papers/griffin2020.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2020；venue=2020 IEEE International Symposium on High Performance Computer Architecture (HPCA)；doi=10.1109/HPCA47549.2020.00055；source=https://sarchlab.org/hpca2020.pdf；root_spotchecked=False
- **fptmigration2021** Efficient Physical Page Migrations in Shared Virtual Memory Reconfigurable Computing Systems — Torben Kalkhof, Andreas Koch；ICFPT 2021；10.1109/ICFPT52863.2021.9609831；source=https://www.esa.informatik.tu-darmstadt.de/assets/publications/materials/2021/2021_FPT_TK.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://www.esa.informatik.tu-darmstadt.de/team/tk
  - 版本 fptmigration2021：PDF [papers/fptmigration2021.pdf](papers/fptmigration2021.pdf); text [papers/fptmigration2021.txt](papers/fptmigration2021.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2021；venue=ICFPT 2021；doi=10.1109/ICFPT52863.2021.9609831；source=https://www.esa.informatik.tu-darmstadt.de/assets/publications/materials/2021/2021_FPT_TK.pdf；root_spotchecked=False
- **idyll2023** IDYLL: Enhancing Page Translation in Multi-GPUs via Light Weight PTE Invalidations — Bingyao Li, Yanan Guo, Yueqi Wang, Aamer Jaleel, Jun Yang, Xulong Tang；MICRO '23；10.1145/3613424.3614269；source=https://par.nsf.gov/servlets/purl/10492257；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知
  - 版本 idyll2023：PDF [papers/idyll2023.pdf](papers/idyll2023.pdf); text [papers/idyll2023.txt](papers/idyll2023.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2023；venue=MICRO '23；doi=10.1145/3613424.3614269；source=https://par.nsf.gov/servlets/purl/10492257；root_spotchecked=True
- **bypassd2024** BypassD: Enabling fast userspace access to shared SSDs — Sujay Yadalam, Chloe Alverti, Vasileios Karakostas, Jayneel Gandhi, Michael Swift；ASPLOS '24, Volume 1；10.1145/3617232.3624854；source=https://cgi.di.uoa.gr/~vkarakos/papers/asplos24_bypassd.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 bypassd2024：PDF [papers/bypassd2024.pdf](papers/bypassd2024.pdf); text [papers/bypassd2024.txt](papers/bypassd2024.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2024；venue=ASPLOS '24, Volume 1；doi=10.1145/3617232.3624854；source=https://cgi.di.uoa.gr/~vkarakos/papers/asplos24_bypassd.pdf；root_spotchecked=False
- **grit2024** GRIT: Enhancing Multi-GPU Performance with Fine-Grained Dynamic Page Placement — Yueqi Wang, Bingyao Li, Aamer Jaleel, Jun Yang, Xulong Tang；2024 IEEE International Symposium on High-Performance Computer Architecture (HPCA)；10.1109/HPCA57654.2024.00085；source=https://yueqiwang42.github.io/assets/pdf/papers/GRIT_HPCA24.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知
  - 版本 grit2024：PDF [papers/grit2024.pdf](papers/grit2024.pdf); text [papers/grit2024.txt](papers/grit2024.txt); 报告 [workers/G8_grit_colt.md](workers/G8_grit_colt.md); 证据卡 [workers/G8_grit_colt-papers.json](workers/G8_grit_colt-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2024；venue=2024 IEEE International Symposium on High-Performance Computer Architecture (HPCA)；doi=10.1109/HPCA57654.2024.00085；source=https://yueqiwang42.github.io/assets/pdf/papers/GRIT_HPCA24.pdf；root_spotchecked=True
- **suv2024** SUV: Static analysis guided Unified Virtual Memory — Pratheek B, Guilherme Cox, Jan Vesely, Arkaprava Basu；MICRO 2024；doi未知；source=https://www.csa.iisc.ac.in/~arkapravab/papers/MICRO24_SUV.pdf；topic=lifecycle；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知
  - 版本 suv2024：PDF [papers/suv2024.pdf](papers/suv2024.pdf); text [papers/suv2024.txt](papers/suv2024.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2024；venue=MICRO 2024；doi=未知；source=https://www.csa.iisc.ac.in/~arkapravab/papers/MICRO24_SUV.pdf；root_spotchecked=False
- **observuvm2026** Observability-aided GPU Memory Oversubscription — Pratheek B, Khushit Shah, Arkaprava Basu；ISCA 2026；doi未知；source=https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf；topic=lifecycle；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://www.csa.iisc.ac.in/~arkapravab/papers.html
  - 版本 observuvm2026：PDF [papers/observuvm2026.pdf](papers/observuvm2026.pdf); text [papers/observuvm2026.txt](papers/observuvm2026.txt); 报告 [workers/G6_lifecycle.md](workers/G6_lifecycle.md); 证据卡 [workers/G6_lifecycle-papers.json](workers/G6_lifecycle-papers.json)；reading=fulltext_worker_reviewed；status=published；year=2026；venue=ISCA 2026；doi=未知；source=https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf；root_spotchecked=False
- **shadowupdate2026** Reducing Page Faults via Invalidation-Based Mapping Propagation in Multi-GPU Systems — Junsung Kim, Dongho Ha, Sungwoo Kim, Wonho Cho, Sungbin Kim, Yufei Ding, Won Woo Ro；ISCA 2026；10.1109/ISCA66397.2026.00110；source=https://yonsei.elsevierpure.com/en/publications/reducing-page-faults-via-invalidation-based-mapping-propagation-i/；topic=lifecycle；status=published；relevance=core；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=未知
  - notes: 机构摘要明确失效时传播新映射、在途迁移tracker与拷贝完成后放行；作者PDF按钮无有效目标，未获全文。
  - 版本 shadowupdate2026：无本地文件；reading=abstract_or_indexed_excerpt；status=published；year=2026；venue=ISCA 2026；doi=10.1109/ISCA66397.2026.00110；source=https://yonsei.elsevierpure.com/en/publications/reducing-page-faults-via-invalidation-based-mapping-propagation-i/；root_spotchecked=False
    - notes: 机构摘要明确失效时传播新映射、在途迁移tracker与拷贝完成后放行；作者PDF按钮无有效目标，未获全文。

## 待核 null 字段

- accelerator_translation2017.metadata_source
- accelerator_translation2017/accelerator_translation2017.metadata_source
- avatar2024.metadata_source
- avatar2024/avatar2024.metadata_source
- barre2024.metadata_source
- barre2024/barre2024.metadata_source
- batch_uvm2020.metadata_source
- batch_uvm2020/batch_uvm2020.metadata_source
- border2015.metadata_source
- border2015/border2015.metadata_source
- bypassd2024.metadata_source
- bypassd2024/bypassd2024.metadata_source
- cato2026.doi
- cato2026.metadata_source
- cato2026/cato2026.doi
- cato2026/cato2026.metadata_source
- clap2025.metadata_source
- clap2025/clap2025.metadata_source
- clover2026.metadata_source
- clover2026/clover2026.metadata_source
- colt2012.metadata_source
- colt2012/colt2012.metadata_source
- contiguitas2023.metadata_source
- contiguitas2023/contiguitas2023.metadata_source
- cross2025.doi
- cross2025.metadata_source
- cross2025/cross2025.doi
- cross2025/cross2025.metadata_source
- cryptommu2023.doi
- cryptommu2023.metadata_source
- cryptommu2023/cryptommu2023.doi
- cryptommu2023/cryptommu2023.metadata_source
- cuptw2026.metadata_source
- cuptw2026/cuptw2026.metadata_source
- didi2011.metadata_source
- didi2011/didi2011.metadata_source
- direct_segments2013.doi
- direct_segments2013.metadata_source
- direct_segments2013/direct_segments2013.doi
- direct_segments2013/direct_segments2013.metadata_source
- dvm2018.metadata_source
- dvm2018/dvm2018.metadata_source
- fastsafe2024.doi
- fastsafe2024.metadata_source
- fastsafe2024/fastsafe2024.doi
- fastsafe2024/fastsafe2024.metadata_source
- granularity2018.metadata_source
- granularity2018/granularity2018.metadata_source
- griffin2020.metadata_source
- griffin2020/griffin2020.metadata_source
- grit2024.metadata_source
- grit2024/grit2024.metadata_source
- hatric2017.metadata_source
- hatric2017/hatric2017.metadata_source
- hdpat2026.doi
- hdpat2026/hdpat2026.doi
- icc2010.doi
- icc2010.metadata_source
- icc2010/icc2010.doi
- icc2010/icc2010.metadata_source
- idyll2023.metadata_source
- idyll2023/idyll2023.metadata_source
- iommu_scalable2015.doi
- iommu_scalable2015.metadata_source
- iommu_scalable2015/iommu_scalable2015.doi
- iommu_scalable2015/iommu_scalable2015.metadata_source
- iotlb_wall2023.metadata_source
- iotlb_wall2023/iotlb_wall2023.metadata_source
- lamp2025.doi
- lamp2025.metadata_source
- lamp2025/lamp2025.doi
- lamp2025/lamp2025.metadata_source
- latpc2025.metadata_source
- latpc2025/latpc2025.metadata_source
- latr2018.metadata_source
- latr2018/latr2018.metadata_source
- leasttlb2021.metadata_source
- leasttlb2021/leasttlb2021.metadata_source
- mask2018.metadata_source
- mask2018/mask2018.metadata_source
- mgvm2022.doi
- mgvm2022.metadata_source
- mgvm2022/mgvm2022.doi
- mgvm2022/mgvm2022.metadata_source
- mitosis2020.metadata_source
- mitosis2020/mitosis2020.metadata_source
- mosaic2017.metadata_source
- mosaic2017/mosaic2017.metadata_source
- nbwalker2026.doi
- nbwalker2026.metadata_source
- nbwalker2026/nbwalker2026.doi
- nbwalker2026/nbwalker2026.metadata_source
- neighborhood2018.metadata_source
- neighborhood2018/neighborhood2018.metadata_source
- nocstar2018.metadata_source
- nocstar2018/nocstar2018.metadata_source
- observuvm2026.doi
- observuvm2026/observuvm2026.doi
- ontable2017.metadata_source
- ontable2017/ontable2017.metadata_source
- orchestrated2023.doi
- orchestrated2023/orchestrated2023.doi
- pichai2014.metadata_source
- pichai2014/pichai2014.metadata_source
- riommu2015.metadata_source
- riommu2015/riommu2015.metadata_source
- shadowupdate2026.metadata_source
- shadowupdate2026/shadowupdate2026.metadata_source
- shared1993.doi
- shared1993.metadata_source
- shared1993/shared1993.doi
- shared1993/shared1993.metadata_source
- sharemmu2026.doi
- sharemmu2026/sharemmu2026.doi
- site2017.doi
- site2017.metadata_source
- site2017/site2017.doi
- site2017/site2017.metadata_source
- slltlb2011.doi
- slltlb2011/slltlb2011.doi
- snakebyte2023.metadata_source
- snakebyte2023/snakebyte2023.metadata_source
- socdma2018.doi
- socdma2018.metadata_source
- socdma2018/socdma2018.doi
- socdma2018/socdma2018.metadata_source
- sparta2020.venue
- sparta2020.doi
- sparta2020.metadata_source
- sparta2020/sparta2020.venue
- sparta2020/sparta2020.doi
- sparta2020/sparta2020.metadata_source
- spimpact2025.metadata_source
- spimpact2025/spimpact2025.metadata_source
- ssr2018.doi
- ssr2018/ssr2018.doi
- star2024.doi
- star2024/star2024_final.doi
- star2024/star2024.doi
- star2024/star2024.metadata_source
- suv2024.doi
- suv2024.metadata_source
- suv2024/suv2024.doi
- suv2024/suv2024.metadata_source
- svm2016.doi
- svm2016.metadata_source
- svm2016/svm2016.doi
- svm2016/svm2016.metadata_source
- synergistic2010.metadata_source
- synergistic2010/synergistic2010.metadata_source
- transfw2023.metadata_source
- transfw2023/transfw2023.metadata_source
- translation_cache2010.doi
- translation_cache2010.metadata_source
- translation_cache2010/translation_cache2010.doi
- translation_cache2010/translation_cache2010.metadata_source
- translation_ranger2019.metadata_source
- translation_ranger2019/translation_ranger2019.metadata_source
- unitd2010.metadata_source
- unitd2010/unitd2010.metadata_source
- valkyrie2020.metadata_source
- valkyrie2020/valkyrie2020.metadata_source
- victima2023.doi
- victima2023.metadata_source
- victima2023/victima2023.doi
- victima2023/victima2023.metadata_source
- vmitosis2021.metadata_source
- vmitosis2021/vmitosis2021.metadata_source
- wafertrans2026.doi
- wafertrans2026.metadata_source
- wafertrans2026/wafertrans2026.doi
- wafertrans2026/wafertrans2026.metadata_source
- walk_scheduling2018.metadata_source
- walk_scheduling2018/walk_scheduling2018.metadata_source
- walk_stealing2021.metadata_source
- walk_stealing2021/walk_stealing2021.metadata_source

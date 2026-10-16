# 覆盖、未知与维护

这是一份**项目已有引用的集中目录**。它不取代原专题的完整笔记、原件、作者证据卡和root接受记录，不宣称重新检索或系统查新。

## 输入与去重

- 首要种子为 `iommu-platform/out/shared-translation-literature-20261006/catalog.json` 的123项，原preferred version、publication status、reading status、root抽核标记和全文证据位置保留。
- 广域种子为 `research-landscape-20261005/source-manifest.json` 及既有逐篇卡。补入AXI/实时分析、映射生命周期/DMA_PMD、缓存/租赁成本、DMA数据流、形式化翻译、KV/LLM等后续项目引用。
- URL库存扫描根Markdown、根`docs/`、`iommu-platform/docs/`、`iommu-platform/out/`受控深度内的Markdown叙述记录。`out/`递归按`build_library.py`的`PRUNE`排除源码、构建及before等目录；根`docs/`保留其中已有的历史审阅和存档Markdown。不扫描Linux/LLVM/RTL源树或HTTP工具JSON日志。`url-inventory.json.scanned_files`给出确切输入清单。
- DOI转小写键，arXiv的abs/html/pdf与版本号按论文编号归并，原版本URL仍保留；去掉utm参数和fragment。已确认作者镜像和旧/新版本通过固定映射归并。不同版本的机制/评估不可任意互换。
- GitHub源码按仓库与**显式固定版本**归组；没有固定版本的入口不假装冻结。规范章节/不同版本可独立作为技术条目，不将其计为论文。专利属于公开技术披露，不作性能或法律结论。
- 作者主页、会议程序入口、安装说明、下载工具、徽章、项目上传/运行URL等不自动算文献；排除项仍有来源和disposition。部分身份待核的机构题录、镜像仍可能与其他记录重复，不能按总条目数计算独立工作数。

## 阅读与证据字段

`worker_fulltext_record`是历史worker全文提取记录，不是用户已读标记。`local_partial`只表示项目已保留局部核查，须看scope；`metadata_or_mention`也可能包含后来尚未结构化的局部笔记，但不因此自动升为全文。

`legacy_root_spotchecked`和`legacy_reading_status`只描述10月6日原目录。MASK、DAMN、NB-Walker等后来证据见`root_read_evidence`及原项目核查来源，不能由旧false否认后来的部分阅读，也不能由后来的源码核查反推论文全文结论。论文PDF存在只是availability。原件SHA-256验证只证明本地字节一致，不验证论文正确性。

每卡正文先给已知题录与可用总结；未单独抽取的问题、未核的方法、未知作者/年份/完整题名显式unknown。`source_context`是项目历史记录，已保留其原`file:line`；它不自动代表当前采纳，也不覆盖最新用户合同。摘要里的本地相对链接转为纯文字，引用入口链接到原文件，防止原上下文换目录后产生坏链。完整引用位置在catalog，摘要只列有用的部分入口。

## 已知遗漏边界

1. 没有扫描项目里每份Linux/第三方源码的参考资料或每篇PDF的参考文献表；那会引入大量项目并未实际使用的论文。
2. 没有把历史原始HTTP结果中的全部搜索命中和失败URL当“用过的参考文献”。只保存为项目正文/证据卡引用的项。
3. 无URL且未进入种子目录的只言片语可能遗漏；无法用本轮目录证明全工作区每一个名字都已识别。
4. 仅题录/摘要/缺全文项保留，后续若用于决定性机制比较，须按该具体问题补读原件；本整理不启动此后继。
5. 局部PDF清单受控，未匹配不是“全工作区绝无原件”；少量canonical文件位于本轮明确排除的源码/历史归档目录时，只链接已有核查记录，不迁移或重命名。
6. 元数据尚未规范化的记录**不能直接作为最终论文参考文献表**；独立工作计数尚需完成未解决别名/题录核对。缺失明细见`reading-gaps.md`和每卡unknown字段。

## 后续维护

- 新引用优先先用DOI/arXiv/准确title比对catalog，复用原ID；读到新版本/新范围则追加带日期的证据，不覆盖旧阅读身份。
- 核心人工修订写入`curated-updates.json`，明确新条目写入`supplemental-records.json`。`build_library.py`与`index_template.py`生成catalog、摘要和索引；不要手改生成卡后立即运行重建覆盖。`generated-files.json`记录本工具拥有的卡片与symlink；重建只清理由其记录且已不在catalog的生成文件，未登记的手写文件保留。
- 当输入文档改变、确需刷新file:line时运行 `python3 references/build_library.py`，再运行 `python3 references/validate_library.py`。没有输入变化不重复生成；不下载新文献，不改变研究裁决。
- 原PDF始终留canonical路径，本库只建相对symlink；新增引用和核读记录随项目正常同步。原件若因已授权任务迁移，要同时更新原路径索引并验证哈希。

本轮请求配置为Sol/high；工具未提供独立actual model/effort运行元数据，实际字段为unknown。不以请求参数冒充运行证明。

"""Rendering helpers for the root library; no source discovery or external lookup."""
from pathlib import Path
import collections
TYPE_NAMES={'research_paper':'论文／作者稿','preprint':'已标明预印本','technical_document':'规范与官方技术文档','rfc':'RFC','rfc_patch':'公开补丁／邮件讨论','patent':'公开专利披露','source_code':'软件源码／固定版本','artifact_metadata':'artifact元数据','research_record':'题录／机构记录，身份待整理','thesis':'学位论文'}
TOPICS={'gpu':'GPU翻译与并发','multigpu':'多GPU／分布式翻译','foundations':'经典翻译与覆盖','accelerators':'异构加速器翻译','io':'I/O与IOMMU','lifecycle':'映射更新与生命周期','data-lifecycle':'数据搬运／动态内存／缓存分配','axi-realtime':'AXI与实时分析','mapping-lifecycle':'持久映射与回收','resource-management':'资源管理与普通强对照','security':'安全近邻','formal-analysis':'一般模型与形式化分析','dma-dataflow':'DMA／数据流','patents':'专利技术披露','standards-software':'标准与官方接口','software-artifacts':'源码与artifact','workflow-tools':'研究工具／方法资料','supplementary-research':'补充引用，题录与摘要待整理'}
def render(ref,records,stats,levels):
 (ref/'topics').mkdir(exist_ok=True)
 def title(r):return (r['title'] if r['title']!='unknown' else r.get('display_label',r['id'])+'〔题名待整理〕').replace('|','／').replace('\n',' ')
 def rows(rs,prefix):
  return '\n'.join(f"| [{title(r)}]({prefix}{r['summary_path']}) | `{r['id']}` | {r['year']} | {TYPE_NAMES.get(r['type'],r['type'])} | {levels[r['reading_level']]} |" for r in rs)
 header='| 资料／独立摘要 | 稳定ID | 年份 | 类型 | 阅读证据 |\n|---|---|---|---|---|\n'
 topics=collections.defaultdict(list)
 for r in records:topics[r['topic']].append(r)
 for topic,rs in sorted(topics.items()):
  (ref/'topics'/(topic+'.md')).write_text('# '+TOPICS.get(topic,topic)+'\n\n每行是一条资料记录；同一工作的已确认镜像/版本归并，身份未解决者不能计为独立论文。\n\n'+header+rows(rs,'../')+'\n\n[返回文献入口](../README.md)\n')
 (ref/'summaries/README.md').write_text('# 逐条摘要入口\n\n每条资料独立ID与Markdown卡片，使用[主题目录](../README.md)定位；机器读取使用[catalog.json](../catalog.json)。摘要包括已知题录、原始来源、问题/机制、支持边界、限制及项目引用file:line。unknown不补造。\n\n'+header+rows(records,'../')+'\n')
 papers=[r for r in records if r['originals']]
 (ref/'papers/README.md').write_text('# 本地原件索引\n\n此目录只放指向canonical PDF的相对symlink，原件仍在原专题目录。没有重新下载、复制或改写PDF字节。每个摘要和catalog保存SHA-256；打开摘要可定位已核txt/章节与项目证据。PDF存在不等于全文已读。\n\n| 资料 | PDF索引 | canonical原件 |\n|---|---|---|\n'+'\n'.join(f"| [{title(r)}](../{r['summary_path']}) | [PDF]({Path(o['index_path']).name}) | [{o['path']}](../../{o['path']}) |" for r in papers for o in r['originals'])+'\n\n[返回入口](../README.md)\n')
 gaps=[r for r in records if r['reading_level']=='metadata_or_mention']
 missing=[r for r in records if not r['originals'] and r['type'] in ['research_paper','preprint','research_record','thesis']]
 unknown=sum(r['bibliography_status']!='complete_from_project_record' for r in records)
 (ref/'reading-gaps.md').write_text('# 未读／核读范围待整理与缺原件\n\n这里保留已有引用入口，不将它们作为机制覆盖或创新成立的证明。部分记录实际有后续局部阅读，已在卡片dated evidence中另列；旧worker/fulltext状态不替代root全文精读。没有启动补全文或新检索。\n\n## 仅题录、摘要、提及或范围待整理\n\n'+header+rows(gaps,'')+'\n\n## 未在受控本地PDF清单中匹配到原件\n\n此处指**本次未匹配**，不声称全工作区绝无PDF。规范、网页和源码通常没有PDF，不计入此表。\n\n'+header+rows(missing,'')+'\n\n[返回入口](README.md)\n')
 type_table='| 类型 | 记录数 |\n|---|---:|\n'+'\n'.join(f'| {TYPE_NAMES.get(k,k)} | {v} |' for k,v in stats['by_type'].items())
 topic_table='| 主题 | 条目数 |\n|---|---:|\n'+'\n'.join(f'| [{TOPICS.get(k,k)}](topics/{k}.md) | {len(v)} |' for k,v in sorted(topics.items()))
 read_table='| 阅读证据 | 记录数 |\n|---|---:|\n'+'\n'.join(f'| {levels[k]} | {v} |' for k,v in stats['by_reading_level'].items())
 readme=f'''# 项目参考文献与资料库

2026-10-16整理。这里集中项目**已经引用或核查过**的论文、规范、RFC/公开补丁、专利披露和技术资料，供新agent复用。共 **{stats['references']} 条资料记录、{stats['original_pdf_links']} 个本地PDF索引**；这不是同样数量的独立论文，也不是一次新的系统查新。

[逐条摘要总表](summaries/README.md) · [机器目录](catalog.json) · [本地原件](papers/README.md) · [未读与缺原件](reading-gaps.md) · [覆盖与维护规则](COVERAGE.md) · [验证结果](verification.json)

## 从当前问题开始

- **M0首PTE票据／接口实现成本**：先读 [MASK](summaries/mask2018.md) → [AXI-REALM](summaries/axi_realm2025.md) → [iDMA](summaries/idma_tc2024.md)。MASK已联合考虑PTE/data与配额，REALM已有合法分段/预算；这些前史限定当前增量。当前模型和接口成本问题见[当前状态](../CURRENT_STATUS.md)，不是文献库选择了论文主线。
- **映射组织、持续授权、回收／CPU与内存成本**：[DAMN](summaries/damn2018.md) → [rIOMMU](summaries/riommu2015.md) → [Utilizing the IOMMU Scalably](summaries/iommu_scalable2015.md) → [IOTLB Wall](summaries/iotlb_wall2023.md) → [F&S](summaries/fastsafe2024.md) → [DMA_PMD RFC](summaries/dma_pmd_rfc2026.md)。明确区分来源已实现能力、公开补丁和普通适配。
- **共享翻译干扰与强前作**：[Bermuda](summaries/bermuda2025.md)、[DWS](summaries/walk_stealing2021.md)、[Scheduling PTW](summaries/walk_scheduling2018.md)、[Orchestrated](summaries/orchestrated2023.md)、[ShareMMU](summaries/sharemmu2026.md)、[DATE2025 RISC-V SVA](summaries/riscvsva2025.md)。
- **分析方法边界**：[RTAS2026 DPU](summaries/aromolo_rtas2026.md) → [TC2023完整job界](summaries/bounding_multiax2023.md) → [AXI-ICRT](summaries/axi_icrt2023.md)。FIFO外调度、合法分块、预算和一般响应分析均有前史。
- **补基础与广域近邻**：从[经典翻译](topics/foundations.md)、[多GPU](topics/multigpu.md)、[数据搬运/动态内存](topics/data-lifecycle.md)选读，不必先读完全部库。

## 按主题查找

{topic_table}

## 分清资料类型与阅读深度

{type_table}

{read_table}

`worker_fulltext_record`表示历史worker对已存全文作了证据提取；不表示root或用户逐篇全文精读。`local_partial`保留具体章节/源码核查范围。`metadata_or_mention`不作全文机制结论。每条的`legacy_reading_status`、`legacy_root_spotchecked`保留旧123专题状态，后来的核读单独列`root_read_evidence`，不让旧false抹去后来证据。

共有 **{unknown} 条**题录字段仍显式不完整（`unknown`或仅项目标签），可直接追到已有来源，不能伪装成规范的最终参考文献表。已知题名、作者与年份来自既有记录；没有根据标题补机制。历史引用、当前采用、原文作者结果和本项目复现分开。

## 覆盖到哪里

受控扫描 **{stats['narrative_files_scanned']} 份项目叙述文档**，规范化 **{stats['url_inventory']} 个外部URL**；其中 {stats['catalogued_urls']} 个已归入条目，{stats['unclassified_navigation_or_operations_urls']} 个保留导航/操作排除标记，详见[url-inventory.json](url-inventory.json)。原共享翻译 **123项全部保留**，STAR不同版本仍归一项；后来AXI/RTAS、映射/RFC、缓存/数据流、KV/LLM和形式化近邻另行补入。

仅按精确DOI、arXiv编号、已确认镜像/工作身份归并；未完成身份核对的题录记录保留为待整理，**条目数不能用于独立工作计数**。没有穷尽历史源树、PDF参考文献表、原始HTTP日志或未带链接的每个名称；这些潜在遗漏明确留在[COVERAGE.md](COVERAGE.md)。本轮没有新联网检索、论文复现实验、RTL/Linux修改、构建、硬件操作或研究状态恢复。

本轮由Codex整理并核验文件链接/原件哈希；历史目录中的DeepSeek全文证据提取归属继续保留，不将其改写成Codex本轮重读。
'''
 (ref/'README.md').write_text(readme)

# 论文离线 replay 输入审计

## Material Passport

Origin Skill: academic-research-suite / experiment-agent；Mode: validate（输入闭包与可移植性审计）；Verification Status: STATIC_SOURCE_AND_BYTE_IDENTITY_AUDIT；Version Label: replay-input-audit-v1。未重跑硬件或RTL，未浏览文献。

标准数值重建需要 **17份数据输入＋10份控制/依赖文件**；不需要UART、stdout、事件trace、BIT、Image或整个历史out树。逐文件真实相对路径、SHA256、字节数、代码引用、必要性和当前/原树可用性见 `replay-input-audit.json` 的 `files`。审计保留源码所要求的完整表和原始试次，不把标量行当独立重复。

## 最小目录树与实际读取

```text
<replay-root>/paper/ispass2027/
  analysis/build.sh, requirements.txt
  analysis/scripts/{test_analysis,baseline_extra,statistics_common,merge_results,data_adapter,plot_figures,write_figure_notes,validate_outputs}.py
  analysis/inputs/baseline100-source-audit.json
  analysis/history-{number,composition}-inputs.json
  board/packages/baseline100-20261017/source.json
  board/runs/baseline100-20261017-01/
    {ds0-middle,ds50-first,ds50-last}/{records,manifest}.json
    {ds0-middle,ds50-first,ds50-last}-load/load.json
  data/{board_results,invalidation_sim_results}.csv
  exp/E7-capacity-sim/{results.csv,analysis-import.json}
```

- `baseline_extra.py:20–29,95–130`固定从自身向上四层取得根目录，读取3×records/manifest/load、source.json、源码摘录及canonical表。`record`包含逐试次JSON和原始命令**定位符**，不是UART日志内容；load.json含必要既有身份字段及可删的板标识元数据，但没有内嵌串口日志。`baseline100-summary.json/md`不在运行读取链，留作参考即可。
- `baseline_extra.py:103–105`只核源码摘录commit与source.json一致，数值重建不打开Git源码库；12份完整源文件hash和精确摘录已在审计JSON列明。它不是重新核验历史加载字节。`baseline_extra.py:326–335`重新核输入hash并写输出，`--check`需要已有对应输出才会逐字节比较。
- `data_adapter.py:100–122`读取canonical表和旁边固定位置的baseline source.json；`merge_results.py:11–18`默认只扫本树exp。`--canonical`不能单独搬去任意目录，因为身份清单位置仍由该CSV的父目录推导；可移植包应保留树深。
- `plot_figures.py:240–269`读composition最小快照而不打开24个历史原record；`384–387`读E3八个own-service值快照。其余历史数字及PN/E2宏来自canonical表。原composition comparison/record和E3 review仅用于重核快照，列为optional原文证据，不混进最低运行闭包。
- `plot_figures.py:288–306`图9只读E7 CSV和完成receipt，不读取jobs、summary、原日志或源码；`413–421`另直接读取失效CSV。少E7输入会静默降为缺图骨架，因此发布验收必须要求图9仍是simulation、12格齐全，不能只看进程退出0。
- `plot_figures.py:393–400`若缺B1 JSON会省略3个聚合宏；严格验收须检查17宏，不能只检查图存在。标准build先运行B1，会从17份数据完整重建。

## 独立RTL编译层

冻结manifest已有 **114个文件：94 source、10 include、10 license**，7个include roots；每个文件在本树可得，审计逐项复核SHA。`source_order`和include roots原样在JSON列出；不能只打包94个 `.sv` 后漏头文件/许可证。固定CVA6/IOMMU/register_interface/axi_slice身份也保留，Linux/CPU不参与该仿真。

现有 `run.py:23–47`会核全部114文件并按source_order构建，另加3个实验inputs。现有 `prepare.py:10,26–28,88–94`每次仍从历史REFERENCE/runner读取3个参考fixture，而不是仅用已提交inputs；其输出保护不容许覆盖不同fixture。`run.py:42–43`还执行git以记录当次checkout身份。干净源码包应由独立便携builder直接使用冻结114文件和既有3个inputs，而不是复制整个out树或假冒原checkout。

现有 `validate.py:24–40`先读12份summary/run/jobs，再按run.source_manifest的**绝对路径**读build manifest，逐条按build.sources绝对路径读原始per-build inputs/冻结source；因此只搬results/summary/jobs并不能让该验证器原样成功。该问题交由独立E7便携构建/验收席处理，本审计不改其文件、不重跑。

## 路径、日志与匿名边界

1. 数值运行并不需要原始UART、events或stdout。E7的jobs.csv是逐任务数值表，属于独立验收/再分析所需数据；它与事件日志不同。12份summary/run清单和3份build清单是身份与检查证据，标准图9仍只需CSV/receipt。
2. baseline records中的raw_command_path、load/manifest、history快照、E7 build/run清单及生成的merge/trace含作者本机路径，有的load记录还含板serial。最低包需要**明确标记的派生副本**，保留科学操作数、boot分组、source commits和bitsha，把原件/派生件hash与删改映射留在包外private-map。不能把删改文件宣称原字节。
3. 把目录换个名字不足以匿名：`data_adapter.py:95–105,156–157`及`plot_figures.py:327`会把当前绝对路径写回生成产物，`write_figure_notes.py`还含原树路径示例。受限包内适配应只把这些追溯位置改为包根相对路径，并限制输入解析不出包；不改变统计函数、seed、公式或选择条件。
4. 保留的科学commit/hash仍可能与公开源码关联；本地用户名/路径/作者仓库URL扫描通过不能等同会议匿名审查通过。个人网络/serial值不写入本Git审计报告；完整私有映射只放忽略的packages目录。
5. 此最低包只能声明**已记录数据的离线数值重建**，不能声明无日志情况下端到端硬件重现，更不能补认证历史真实加载字节。

## 可执行接口建议

- 保留上面的 `paper/ispass2027`层次；标准命令仅 `bash paper/ispass2027/analysis/build.sh`。Python、NumPy/Matplotlib和字体工具单独列依赖，不打包venv，不要求网络访问。无Poppler时现有validate会标字体未检查，严格交付验收应要求实际字体检查成功。
- 导出器使用B1 `inputs_sha256`白名单，加上述5份输入和10份控制；禁止递归复制board/out目录。生成全新目录/归档，禁止改原主文/数据。
- 包内派生适配仅处理标识字符串、路径序列化和输入边界；用原科学numeric对象、17宏及10图状态做严格对照。公开包声明DERIVED_REDACTED，包外保存原/派生hash、转换规则及补丁。
- E7便携源码/编译由独立席交付。论文源/LaTeX与全文PDF由Root合并；本审计包不承诺携带付费文献或完整工具链。

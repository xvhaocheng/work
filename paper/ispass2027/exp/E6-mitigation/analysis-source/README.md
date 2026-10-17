# G完整40点分析候选

实际入口`../analyze_g.py`，[精确源码身份](component.json)，
[最终源码副本](../analysis-validation/run01/source/)，[41项有限检查](../analysis-validation/run01/check03-report.json)。
代码与验证由Codex完成；未运行新G板测，正式数据仍0点。本批待Root实际审阅。
原采集器、parser、hist helper、driver和已编Image均不改。

正式只接受status=passed、recorded_cases=40、10完整区组的原G合同。
读取采集manifest／records及每点原raw，固定已接受0202 collector/parser/helper SHA，
逐份核对load/FPGA/software/backend绑定源SHA，并要求正式G软件原manifest SHA4bee1ead…；
OpenSBI/reference角色/UAPI/CLI、passed G BIT50MHz DS0/OBS4/Depth4/hist/actual generic与合成inventory、
load BIT／kernel／boot均复用已验接口。只读缓存字节的[binding适配diff](cached-binding-adaptation.diff)
保存；不经E5 CLI rebind，不假设存在`result`，G只读`result_buffer`。

每个原raw必须成功且command精确匹配40点计划，再用原`g_records.parse`全量重解析；
缓存中的CLI／原观察值／32word／PATH／hist／derived metrics都须与重解析逐项一致。
同boot连续sequence、case/block/condition顺序、40条独立raw路径全部检查。
缺点、failed/running、修改缓存、原raw失效、改变绑定源或工具源均拒绝，不丢点或补采。
production拒绝collection／cache／任意bound source／raw／parsed item的fixture标记，
并要求采集manifest及缓存明确为fpga_board／fixture_only=false。

每个采集输入只读实际bytes一次，验证完才创建全新输出目录，不覆盖旧结果。
`inputs/`复制所有实际读取bytes，snapshot index记录原路径／新路径／SHA／大小；
每点保留原raw路径，并增添`snapshot_raw_path`导航新副本。
输入副本、summary、CSV及snapshot index均只读，输出bundle目录也只读。
输入原件从不改写；新输出已存在立即拒绝。

正式运行接口（仅待Root实际G40完成后，不自行跑）：

```sh
python3 paper/ispass2027/exp/E6-mitigation/analyze_g.py \
  --run <实际passed40采集目录> --output <全新独立分析目录>
```

输出为`analysis.json`、`long-table.csv`、`snapshot.json`及`inputs/`。
CSV沿用E1b/E3原17列，table_schema原字节SHA423a745a…；每个scalar的
raw_path/source_locator指向新summary的精确JSON pointer，row_id唯一。
原table/describe/full-E3分析源码均在本目录`*as-reused.py`保留，describe为原Type7精确提取。

每条件实际10区组，返回n/min/median/max/Q1/Q3/IQR；不存在的Solo A或零full延迟为N/A、有效n0。
V固定16MiB吞吐只用raw10 own whole，A只用raw13 own whole；全部完成字节／warmup工作一起保留。
同block V/Solo、gap1000/2000对本Ggap0的V时间比、A own时间比、A own吞吐比及吞吐变化分别统计。
配对统计是10个ratio本身的median/IQR，不用ratio of medians替代。
V window A issue/done/carry/open-edge独立保留；HPM始终标原非原子software V bracket，
software V elapsed与whole elapsed各自标注，不充作hardware whole分母。

每trial、每stage保存full、32counts、原桶p50/p90/p95/p99区间及carry/start/end；
full0为None，valid_latency_trial_n=0。条件层明示有效trial数、零full trial数、
所有trial的full范围和每个trial的full/总full权重。
`auxiliary_pooled_full_transactions`仅为辅助事务加权汇总，明显单列；
不会把其分位桶区间叫作10trial分位统计，也不把A窗口hist当A自身whole分布。
CSV中每trial桶边界n=0或1，辅助pooled行n留空，另在condition写明trial数／有效trial数／权重；
零full不会被标为有效n10。原桶不插值成精确分位点。

有限验证复跑：

```sh
python3 paper/ispass2027/exp/E6-mitigation/analysis-validation/verify_g_analysis.py \
  --cli-report paper/ispass2027/exp/E6-mitigation/cli-validation/run01/report.json \
  --output paper/ispass2027/exp/E6-mitigation/artifacts/root-g-analysis01
```

最终41项passed，生成38448行synthetic CSV，每一行17列／pointer／单位与标签检查；
实际输出仅存在ignored artifacts测试目录，无paper输出。CLI无fixture绕过开关。
内部显式_fixture_mode始终标fixture_only=true、source_board_cases=0，CSV experiment/sample_set/
evidence_level均synthetic。先前37/39项原回执保留，不替代最终hist边界验收。

[数值反例](../analysis-validation/run01/numeric-regression.json)：配对比1..10得median5.5/IQR4.5，
显著区别于ratio of medians；gapown吞吐比为1/2与1/4。
全零A stage有效trial0；另一条件10区组full=0,2,…18、有效trial9、权重full/90，
逐区组桶范围保留，pooled mean按full加权而非trial均值。
这些都是合成protocol fixture，不能用作论文性能、DMA或BIT验收证据。

# §33 实际证据入口

- [创新判定](../../docs/posttl-independent-verdict-20261008.md)
- [Codex RTL设计与审查](codex-review.md)
- [六项汇总](analysis.md) / [516项检查](analysis.json)
- [构建argv](build-command.json) / [退出码](build-result.json) / [日志](build.log)
- 每项 `fixed/dynamic-v/a-32768`、另 `fixed/dynamic-v-32768-lat100` 的 `-command.json/-result.json/.log/.csv` 保存原始记录。
- [新RTL](posttl_owner_fragment.sv)、[out系统副本](system_top.sv)、[原驱动派生副本](natural_fragment_test.cpp)、[集成准备](prepare_integration.py)、[DeepSeek草案经修订验收脚本](analyze.py)。
- [生产76SV检查](production-check.json) / [声明状态非面积](declared-state.json) / [阶段清点](stage-evidence.json)。
- 三次DeepSeek调用完整原答和任务见worker-*文件；thinking high/max393216，均stop/非truncated。实际总usage658044 provider tokens；Codex亲自设计RTL、原源审查及执行，DeepSeek未执行/写文件。
- 初次准备脚本接口空格匹配失败后修；1次实际SV+C++构建成功，6运行全部PASS；分析脚本首次SyntaxError由Codex错误截取代码围栏造成，补全完整稿并应用字段/分组修订后516项通过，无新RTL重编/重跑。
- 无原tests、生产RTL/Linux/BIT/SD/板卡改动；既有失败及诊断文件均保留。不是通用进度/实板/旧PN根因或二区验收。

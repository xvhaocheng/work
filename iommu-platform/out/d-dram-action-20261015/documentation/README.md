# 本轮入口同步与字节保护

仅同步root稳定接受到五份当前入口和本轮README；不运行模型、构建、旧实验、raw核账或硬件操作。文档请求gpt-6.1-sol/medium，实际model/effort元数据unknown；没有DeepSeek/Claude调用。

[检查回执](receipt.json)保存实际check数、本地链接计数、新增坏链与baseline原有坏链的独立列表，以及保护块前后SHA256。检查只验证内联Markdown本地目标存在，支持文件链接末尾的:行号；不访问外网、不验证URL或fragment语义，也不以字符串检查代替研究结论审查。[原字节备份目录](before/)保存五文件完整原字节，[备份manifest](before/manifest.json)给出字节数/hash；[同步manifest](sync-manifest.json)保存修改前后整文件hash。

CURRENT_STATUS的A5/A2块和独立工程事项、研究计划的I节至末尾、研究导航的已有成果节至末尾均完全保护；HANDOFF和validation只prepend，原整文件逐字节作为suffix保留。根/平台AGENTS检查前后hash，不修改；稳定接受JSON、生产source和已绑定实现交付快照不在改动范围。

[sync.py](sync.py)是本轮单次同步记录，拒绝覆盖baseline，不应重运行；[check.py](check.py)只读五入口/README/备份并写本目录receipt，可用于重核文档保护和链接。baseline历史坏链原样单列，不算本轮新增坏链；旧记录中的未执行状态与构建时交付文字按各自阶段保留。

[首次文档诊断](initial-check-diagnostic.json)记录首次检查时receipt自身尚未创建的4条自引用缺失；回执落盘后重新检查。这是文档产物创建次序问题，未修改研究输入或重跑实验。
首次检查器还曾将5条文件:行号链接当作文件名，正式检查已识别行号后缀并正确解析；首次诊断保留，不作为最终坏链统计。

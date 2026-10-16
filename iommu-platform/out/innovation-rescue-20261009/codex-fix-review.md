# run03 局部补丁审定

DeepSeek 返回局部 replacements，root 已逐项直接审读。修正 true unretired firstAW−B、done group平衡、solo admission误报、longW commit后的延迟resume，并真实传递4个新场景的READY注入。

Codex 在应用前发现一个关键遗漏：新 do_resume 仅处理 EXIT_STRATEGIES 且尚未 exited；已经 EXIT 的 A，以及 closed/independent 对照，在收到 resume 时仍保留 paused=True。root 在该分支加 else 清除 paused，不改变未drain的 deferred 分支。这是根级并发状态修正，不能声称 DeepSeek 原补丁已通过；原响应与 run02代码保留。

受影响旧的306场景保留，加入4交互场景；root接着执行 run03，结果由原始输出判定。仅受限事件模型。

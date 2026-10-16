# 2026-10-04 N 工程试验交接

由 Dell 经 SSH 自动推进，Mac 不需要另开串口或人工更新 SD。两新版本正在编译，未宣称上板成功。

沿用 SD 上 Linux00093；原版 original-serial 先做 15 项对照。后续 banked-serial / banked-hit 各 15 项，分别排除拆分入口本身和开启命中绕过的影响。只用 P4M 硬件窗口时间和同窗口 A 完成数；新版本另用只读工具记录实际绕过事件。原来的 COST/E3 翻译计数不能用于 N。

工具：tools/nonblock-pilot-20261004/。结果：results/nonblock-pilot-20261004-*。错误即停止，不自动重跑或删样本。original-01 在打开串口前因默认路径漏 /dev/ 停止，未写 BIT、未执行实验；已修正后创建 original-02。

第一轮尚未完成。全部原始数据保留并回传 Dell；两新 BIT 必须编译及必要时序验收通过后才运行。DeepSeek 起草工具，Codex 审查和实际执行。

更新：original-03 15/15 已完成，数据已回 Dell；无数据错误/内核警告，taint=0。启动 161dcc93-c6fe-4344-845c-f32f925cf183 经一次 Ctrl-C 恢复启动脚本，源 original-02 尚未执行实验。恢复目录和原始证据保留，不需重跑。脚本已修正 bare #、登录/password、新输出 30 秒静默才恢复，以及大块 UART 启动标记。两新 BIT 仍编译中。

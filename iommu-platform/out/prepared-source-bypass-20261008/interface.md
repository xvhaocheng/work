# §39 普通已准备源读物理旁路：汇总接口

Codex 负责 RTL、路由、关键判断和执行。DeepSeek 只提案一个离线 Python 汇总程序，不运行 DUT、不改文件。

目录为当前文件所在目录。六个新运行：off-v/a-32768、cache-v/a-32768、wait-v/a-32768；分别 +physical-bypass=0/1/2，均 +reuse=2 +prepare-order=0，其他参数沿用 §38 bypass-aw-first。off 必须与 §38 对应 CSV 逐字节相同。旧 post window4/depth4 只读，不重跑。

各有 -command.json（argv 数组）、-result.json（exit_code、reused=false 等）、.log、.csv。build-command.json/build-result.json 记录一个新构建。失败现场保留，汇总不得自动视为 PASS。

沿用 RAW4-DECL/STATS、PREP4-DECL/STATS、POSTTL-STATS、NATURAL-JSON、PLACEMENT-METRICS-JSON。增加 DIRECT39-STATS mode direct fallback write_wait offered busy observation_bits 和 DIRECT39-AR cycle owner parent addr tc_state tc_write。原生单位都是 cycles；SV 事件 cycle 和 TB issue/done 不同钟原点，不能混算。

RAW4 功能位3317（原3316加一位固定 route）；helper1281；合4598仅声明状态，非综合面积/总成本。观察统计独立声明不参与控制。

源原父32，物理源子片64，目的AW34，各路4096R/W。nat var 是正常 TC 入口子片数，即 fallback，可为0；不能继续硬性要求64。direct+fallback=64。缓存直通后的 source reuse 数不再要求64；TL数量直接报告并对比，若偏离66须报出，不猜性能原因。缓存最后 slow/hit/valid/offer 全0，父槽 live0/0、高水位<=4，off direct0、fallback64；开关启用应 direct>0，并报告 write_wait（真实TC等待写翻译期间发出的直接AR）。所有 held 属性和 R 路由关键断言由 Codex RTL/C++ 验证，汇总不冒称其作者或重跑验证。

以 V 先启动同次实验计算 V/A issue_to_done_cycles 与双方均值；A 先只作镜像一致性检验，不混平均。比较旧 §38 最好普通24751和旧 post18824；不能把差距直接说成独立创新。用如果/因为给条件判断。

要求 complete Python3 标准库脚本 summarize.py，以 __file__ 定位 ROOT/OUT，生成 analysis.json + 简洁中文 analysis.md。检查真实证据，异常给 file:line 或字段证据；无需读取整个源码/仓库。默认检查六个场景，失败不能消失。输出不要写“已经执行/通过”，只说脚本提案。

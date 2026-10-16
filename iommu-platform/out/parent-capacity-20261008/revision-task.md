修订你本轮草拟的 out/parent-capacity-20261008/summarize.py，限此验收汇总脚本，不修改RTL、不做研究路线判断。输出完整替换脚本一个锚定 python 代码块。实际诊断: python3 summarize.py 报 AttributeError: 'list' object has no attribute 'get' 在 check_run cmd_json.get("command")。实际command.json是直接argv字符串数组，result.json为dict。请读随附新运行及复用基准样例。
修订要求:
1 接受并严格验证实际 direct list 命令接口；exit_code必须来自result.json，绝不日志推断。明确6 fresh +2 reused reference，不把引用算成新实验。
2 CSV pwlast也是一个真实W数据拍；逐region的W计数包含pw和pwlast，共4096。rfirst/rlast只是端点标记不能额外计为R拍。格式/数字/事件错误须抛错，禁止 silently continue。
3 逐命令验证 +mode=s,+natural=1,+natural-bytes=32768,+natural-first对应side,+ptw-latency=31,+warmup=0,+read-admit=none，无CPU或强制fixture/优先模式。旧引用命令含固定selector但新depth命令没有，允许此前固定默认实现的接口，勿假设未给出的参数。
4 验证32 POSTTL-PARENT与64 POSTTL-CHILD，owners0/1各16 parent,seq0..15，每父256拍、每child128拍，childoff顺序0,128且合法连续；对应CSV每flow逻辑源AR ID0/4（VAR）和物理源AR ID32/36（PAR），物理地址区间连续源覆盖/无重复，W真实总数。读实际日志字段，勿凭猜测正则。
5 把新depth1/2/4 CSV与对应旧depth8逐字节比较，输出每项 sha256 和等同 bool；该等同只证当前外部轨迹，不能推断8-slot最大实际占用1。
6 所有8结果exit0且真实PASS；守护/字节/quiet以原日志真实证据与程序返回为准。识别任何FATAL、watchdog/等待状态，如有失败保留失败真相并非零退出。
7 保留declared_state_bits对账，但明确仅新模块声明state、无综合/总面积，depth1=3568,2=3910,4=4594,8=5962,观测160。2394bit节省仅此metadata，不能写为新算法收益。
8 脚本只读现成原始数据并写out summary.{json,md}，不重新仿真。输出小而导航清晰；对未验证项明示，不捏造。修订不要扩展任务范围。

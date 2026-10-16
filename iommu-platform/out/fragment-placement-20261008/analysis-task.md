普通有界验收汇总脚本。请复用提供的上一轮 summarize-revised.py 方法，输出完整可执行 placement_summary.py 一个锚定python围栏，定位在out/fragment-placement-20261008。本轮8项全部fresh: pre/post-{v,a}-32768 表示S；pre/post-n-{v,a}-32768表示N。各有-command.json直接argv list/-result.json dict/.log/.csv，所有文件已生成，只解析现有raw，不运行模型。
要求:
1 严格exit_code0/PASS/NATURAL-JSON verified+completed true，任何fatal/watchdog/SCENARIOFAIL/缺失失败不可吞；argv mode=S或N对应stem、lat31/natural32768/warmup0/read-admitnone/first对应，无CPUfixture。
2 pre/post原parent32/child64，每owner父0..15每父2片128、实际同ID打印次序offset=2048*p+[0,1024]，并匹配实际CSV每owner PAR ID32/36、物理地址base+offset、len127和连续32KiB；物理W pw+pwlast每flow4096；各32物理R burst端点只是mark不加为拍数；AW34、每流AW17保持。原逻辑iDMA parent=32 via stats，CSV中var是IOMMU入口：pre64，post32，不混淆。IOMMU入口flowID0/4。
3 PLACEMENT-METRICS-JSON字段来自真实新增C++观察，TL pre98/post66, hit64/32, miss34/PTW102/DDTW2均当前实际，不要写成既定理论。各flow真实issue/done counts=1、delta=done-issue>0；用来表格比较Vfirst两flow的issue-to-done延迟以及双方平均，Afirst镜像独立启动顺序，不是随机统计重复。CSV最后DDR W作另列，非DONE。不能把done-wait轮询顺序当实际done。
4 N现成路径FAST0；源码n_window仅异bankPTW窗口，而本case同bank，两种placement N输出CSV各与S对应逐字节相同。可以核等同，但结论必须'本状态N未触发，不能据此证明有效快路径无法替代'。
5 新postS CSV还应与out/parent-capacity-20261008/depth1-{v,a}-32768.csv逐字节等同，说明观测补丁不改变旧事件；旧仅参考不计9/10项新运行。
6 写analysis.json/md、输出conditions/pass/fail、所有实际raw场景文件paths、metrics、端点，严格有失败exit1。字节guard/quiet来自真实driver原CHECK和literalverified，不声称脚本另验全内存。不新增理论/论文结论；不推断全部约7287cycle来自32 hit，本实验也改变提前请求与AW反馈。
7 模块成本由Codex另审，脚本不预填面积。不发送密码。
files给两S两N日志/argv/result、pre模块和旧脚本作为实际接口，无CSV全文；脚本按schema自行读各CSV，不因没附CSV就猜事件。三项原建模接受计数NA值含IOMMU入口VAR不同如上已明确。
避免上一轮write_md顶层嵌套KeyError，输出函数接口自洽；保留所有失败真相；标准库即可。

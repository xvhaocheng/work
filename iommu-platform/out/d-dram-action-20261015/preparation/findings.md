# M0完整准备：源码和旧日志核查

**不能从旧M0日志恢复完整、保持READY反馈的warm物理入口。** warm是V256次＋反馈决定数量的A环形4KiB COPY，包含真实读、写和完成；旧CSV只记录t0后的接受事件，不能用256条普通read代替。没有构建、运行模型或修改源码。方法为ARS-Codex bounded source verification；建议Sol/high，实际model/effort元数据未知。

| 决定问题 | 核查结论与原文入口 |
| --- | --- |
| 原来的准备次序 | 宿主`setup_memory`写map → rst低8tick、高4tick → 读取V/A DONE基线 → warm → A尾drain → 源/目的宿主数据核对及A批状态核对 → A批配置 → V批配置 → t0 → A NEXT、V NEXT。见C++531–590、1536–1544、1599–1605、1683–1688、2093–2338、2764–2870。 |
| IOTLB clear／quiet | C++准备段没有独立IOTLB clear，也没有warm后完整`check_quiet`调用；reset内部cache失效语义不在本席已核范围，置null。不能把warm DONE/非busy核对写成已执行的TL＋DMA＋memory队列quiet。完整quiet是两批自然结束后才调用，C++1691–1725、2909–2914。 |
| P/PN warm相同吗 | 同一COPY反馈程序、不同运行模式：P N=0/P=1，PN N=1/P=1，从reset tick起即生效，不是t0才切换。A8原log9行：P warm357173周期，PN374366，差17193；两者V256、A257、尾drain1121周期。DS优先变体各自warm与本模式相同，因优先规则只t0后生效。A1为297276/297815，尾drain569。 |
| 准备是否计费 | 旧T_return起点在两批配置完成后的t0；reset、warm、A尾drain和批配置全部排除。warm本身有消耗且两mode不同；旧指标不是cold-start端到端成本。C++2764–2770、2837–2869；旧a8-p/pn.log9、21行。 |
| 两owner重供给 | A先p0，V后固定单页；每圈先poll V，完成则立即重编程V并NEXT；V达到256时在再次poll/refill A前break；否则再poll/refill A、tick。A每次提交p递增并绕环。最后只drain当时未收尾A，不再启动下一A。静态能确定该程序，不能猜每次poll何时匹配或AR/AW何时被接受。C++2093–2229。 |
| 物理地址与字节数 | V读0x80000000、写0x80001000；A8读0x90000000+p×0x1000、写0x90100000+p×0x1000，p=0…7。单次4096B。旧warm合计V256/A257，每方向2101248B；A环前p0重复33次，其余各32次。DDT位于0x10280/0x102c0，各初始化64B；V指针PTE0x20000/0x40400、leaf0x50000/0x50008；A指针0x30000/0x60800、leaf0x70000…0x70078（逐8B）。精确IOVA、PTE值和A1变化在JSON。C++78–93、535–581。 |
| 旧trace覆盖 | A8 P/PN.csv均首事件t0+3，含实测AR349/AW256/R65629/W65536/B256、issue/done各128；warm、cfg/poll、失败offer与READY水平皆未记录。旧log11行仅全段总数。减去测量CSV可得pre-t0聚合AR1457/AW1026/R263101、DDTW2/PTW429；这是计数，不是请求顺序。CPU关闭，全段cpu AR0；poll经cfg直达DMA，不经过DDR。SV55–57，C++998–1051、1290–1399、2559–2565。 |

最小后继执行器必须以原物理镜像、P/PN全程模式和上述真实COPY反馈程序为输入，连接cfg ready/error/rdata、准确NEXT/DONE、物理AR/AW/W/R/B及硬件issue/done；保留64/64、gap0、warm256、100000周期cfg限时、200000周期V进展/A尾drain/quiet/通道限时及7900480周期总预算。内存模型改变反馈后A供给数也可能改变，不能先固定257作为普适规则。若新合同要求warm后显式全队列quiet或独立clear，须明确记为新增动作和成本；不反写为旧M0已做。只发WR/AW不足以代表COPY完成，W、B、硬件DONE和队列排空须分别核账；内存系统write完成映射留root裁决。

机器合同：[preparation-contract.json](preparation-contract.json)；逐段原文：[source-evidence.md](source-evidence.md)；原log行号/CSV完整事件计数：[observed-log-coverage.json](observed-log-coverage.json)；**仅测量段**原始接受地址目录：[observed-physical-address-catalog.json](observed-physical-address-catalog.json)。合同绑定每个当前源码/旧日志/旧CSV的SHA256；历史执行账本未提供完整source/binary hash，当前源码与旧二进制的字节同一性未证，不以这次hash代替历史构建凭证。没有生成虚构warm trace。

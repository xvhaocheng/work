# G Linux首批：UAPI、只读CAPS与纯decode

Root已审首批实际3文件diff，并要求将G独立timeout收敛到60000ms。
冻结tip `889f75b02f845f622ff5c45dff437aab1870d756`，直接来源共同65b；
`b2586bfc`首批实现、`889f75b0`仅timeout修订。内部branch不推第三方上游，增量bundle保留。

范围仅`include/uapi/linux/idma_g.h`、`drivers/misc/idma-g.h`和`idma.c`新CAPS区域/分派。
RUN0x12已保留，尚无分派；CAP_RUN_READY=0。没有改旧DMA生命周期、旧ABI或G RTL。
协议与下一批调用链见`../linux-design/README.md`。

- `runs/run02/manifest.json`及`fixture.log`：最终60s版本实际抽取CAPS/nofault函数、原纯helpers，
  ASan/UBSan、-Wall/-Wextra/-Werror、non-PIE，**329项通过**。全部数据是fixture输入，不是板测/性能。
- 覆盖独立ABI96B caps/680B request、输入边界、G whole/flags/error/时间、window守恒/IDs/carry、
  A在V窗前/内/后结束及Solo零A；只读CAPS的锁、两lane cap、错误/缺设备、唯一index写、
  nofault模拟错误和copy_to_user错误。没有用stub模拟结果证明真实CPU fault恢复。
- `object02/manifest.json`：最终driver实际RV64交叉编译passed，0 warnings；仅`idma.o`。
  `object01`的maccess源码与object未改，原生exception-table/ld证据直接复用，不重编或重验。
- `native-proof.md`：实际caller的两个size8/e8调用、fence i,r、原生ld及exception-table entry。
  它只验证本机源码/对象，未验证旧BIT SLVERR的全CPU/总线行为。

`runs/run01`是首批300000上限的已通过328检查原件；`object01`对应同一b258来源。
Root要求60s后新增run02/object02，不覆盖旧件，不把旧检查数叠加为最终覆盖。
新输出二进制只在本机`../artifacts/`或被忽略的fixture文件，未构建Image/rootfs/payload或操作硬件。
原五份旧UAPI与原pair/observer核心函数字节保持，见`component.json`。

本批由Codex实现、执行并检查；未调用DeepSeek。下一批RUN实际代码仍需Root独立审阅。

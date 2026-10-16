# §59 根接受：独立早启动与接力，包含真实末拍反例

两次SV/C++构建＋一次C++增量、27次新执行：26完整双DMA copy PASS，run13真实F255停滞保留；最终build03针对性8copy全PASS。26份成功CSV另核source声明拍数=实际R期望、每目标256实际W，完整bytes/guard/logicalLAST/jobDONE均过。mode2普通prefetch关闭接力的run06与§58 run02 CSV逐字相同。build02失败源/二进制存before-terminal-debt-fix，最终版本存accepted-build03，ledger记录所有版本。

| 对照（均有真实未来WRITE预取） | V/A作业耗时 | source R/AR |
|---|---:|---:|
| F1共同切分，K256 | 6098/6097 | 256/2 |
| F1独立串行，K256 | 4707/7045 | 512/2 |
| F1早启动接力，K16，最终规则 | 5672/5680 | 368/8 |
| F1早启动接力，K2，最终规则 | 5616/5624 | 298/22 |
| F255早启动接力，K16，修正真实反例 | 5654/5681 | 368/8 |
| F1/PTE1000/B64/背压/A先，K16最终接力 | 15217/15298 | 320/5 |

F1/K2平均5620比共同强预取6097.5低7.83%，但多读42拍、AR更多；V相比独立串行4707增加19.31%。对照对应实际makespan单独在ledger记录。平均、makespan及单方保护不混作同一目标。极晚PTE10000仍安全退回原串行，实际cutover0；RR0/K16也完整接力通过，但不因此宣称可抢占held R。

实际反例run13：V/A r255，V qW[254,256)占W头缺R255；A qW[254,255)自带R254却在其后，A第一原B不回→AW2 coverage256不存在；source最后R255原门等AW2。两页WRITE许可其实均已正常成功，所以问题是数据承诺/coverage，不是缺权限。根只在phase3/full-WR/q2接力去掉last/frontier额外AW coverage要求，保留实际授权和最多两拍private存量；R255可交入，先排V旧W、再排A旧W/B、再捕获AW2写掉保留末拍。真实iDMA R/W metadata独立，最后jobDONE仍须最后WRITE B。关键源码复核见critical-early-sponsor-review.md:41。

J<F且F为奇数时双方native W网格会错开，不能把“联合尾段始终共同q2网格”当证明。实际安全门是新W start<=peer真实raw_written、child<=2、原private存量及已齐WRITE许可；阻塞发生在两拍存量时，老W头的数据末端不超过已交付R。根据此继续检验不做接力的字节进度准入强简单方法。

因为独立早启动、WRITE预取和一般补传已有前作，本表只能保留数据债闭合/逻辑服务接力这个机制候选。如果同信息/成本简单控制也同效，就不能用这组收益立独立创新；如果接力在更强对照下保留独有进度或成本性质，且核清最强前作，再形成论文机制与证明。尚无综合面积、动态fault/invalidation、多job、FPGA或Linux结果。

仅out副本，production/Linux/BIT/JTAG/SD/board动作0。DeepSeek提供有限C++开关修订，Codex亲自设计、诊断真实死锁、改RTL、实际执行与接受；关键独立Codex源审，无Claude。

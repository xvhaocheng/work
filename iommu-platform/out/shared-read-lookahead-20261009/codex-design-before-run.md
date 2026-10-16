# §53：强普通有限先读的根设计

2026-10-09。沿§52每reset一次对齐2KiB/flow、静态四成功recipe、immutable源、B3/q2、A可pause/V继续。Codex亲自设计，不将已有FIFO先读称独立算法。

现有q-W coverage门拒绝下一组数据，即便原iDMA的private FIFO空闲。新普通共用控制准许logical returned - actual raw W accepted < 2时提前一拍；单共享skid只保一个待交付beat，每成员在capture时最多一个reserved逻辑return。raw W accepted用q adapter已有offset与live got组合，不增计数寄存器/数据缓冲。保证暂借two private slots，不能把FIFO总深度当无限free空间。暂停后A不能再借，但capture前冻结的A return必须交付；已有q-W所需R照常完成。

退出后solo AR只覆盖已first-offer W真实尚欠R：私有先读可能把原cursor推到下一个q中间，不能固定再读q并越过当前W末端。ARlen因此min(q, W_end-R_cursor, remaining)，0不offer。只在新header第一次VALID前决定，之后稳定。末个logical R还要求对应W已offer，使V RLAST前的旧AW顺序栅栏仍成立。

暂停trigger按A offer目标且当前W未到WLAST，去掉必须end>rbytes的旧前提；预先取到data的W仍可暂停。与旧结果比较须说明同Woffer cutoff而R cursor可能不同，不伪造同Rcut。quiet不是DMA FIFO空或backend_idle；private保留的逻辑已返回数据也计B3。

验收：真实完整源/目标、未拷贝区域、全部targetphysicalW256和R守恒；无pause长读、q2与q256；早resume保组；长窗口quiet和两次pause；保所有失败。若强普通同策略同效，收益是强基线能力，不是创新证据。

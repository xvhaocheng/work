# §55 运行前：WRITE前沿必须关闭已有数据债务

根Codex诊断：§54完整copy正确，但A在logical128时可借2拍privateFIFO而没有对应global AW。auto detach hold随后禁止这笔未承诺W，首raw父B及下页header/WRITE prepare被延到V末尾。RR开启只有selection，没有任何cross-ID rotation；不可把此负例当作DDR交错无效。

本轮仅先改一条R准入：beat_index+1到达当前成功WRITE end时，必须已global Wcoverage，不能再借private2槽。原有joblast255要求推广到真实权限前沿末拍。这样退组request出现前，最后q2 W已firstVALID，是必须继续的旧义务；qquiet在其实际B后成立，首raw B可及时退休，不改firstVALID、不伪造RLAST/B。

沿用§54四模式/静态成功权限/V全长sponsor/q2 W/相同缓冲，不支持外部pause。先验证真正A2prepare与长data R并存、RR0/1交错计数，再判断中途补gap合流有没有必要/可实现。单R频道可能在V暂停追赶时持有V RVALID而堵A repair，须安全回退，不能假设内存接受请求后可撤R。

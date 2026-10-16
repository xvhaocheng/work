# §58 强普通 next-WR page prefetch 与一般8B跨页夹具

运行前根级架构设计。MMU-aware DMA2018已公开TLB prefetch/逐burst retry，2019 ExaNeSt thesis2025 arXiv再发布已公开source/destination fault-aware提前触页。不是把这些机制命名新创新；用于排除§57收益仅因A第二WRITE实际原parent迟到。

input shared_write_prefetch_i bool（default0）；CLI +write-prefetch=0|1，仅natural/sharedsource2048、mode2/common R-W clip、keep-q1、无cutover/外部pause。root将第一次实际rawAW提供的DID/PID/priv/WRITE key用于两目标页的纯translation preparation。slot1固定first destination page，slot2（existing Depth4未用）固定A next page，各key独立/full success PA，用真实TL/PTW响应填充，绝不提前伪造原DMA AW/data。slot1/2 job-lifetime静态有效不覆盖；实际WRITE child按页匹配helper正确槽，当前write cache门按当前rawAW页选择。不允许两slot同页导致多匹配。

OWNER write_recipe_end累计两成功页可全长256；最强普通初期写q仍在A第一页结束F共同截断，双方从F同q2 grid起步，不能因提前成功cert256就忽略native WR1 F拍与V q2错位。必须防missing-payload W队首环。启用只限mode2是第一公平强对照合同，其他mode不会静默改变。额外成功future-page recipe占实际helper槽2，需要计费（不能把未用array槽当零面积），与§57新增phase/vcut有不同成本。

driver有界普通任务：+a-dst-offset允许0或8B对齐2056..4088，覆盖F1..255。所有nonzero都是actual跨第二页；源/目标9pages内容/untouched、实际拍数、PTE0x70048匹配延迟原验收保留。stage57三种旧offset行为不变。top增加input shared_write_prefetch_i; output dbg_shared_write_prefetch_ready（actual A next-WR成功valid）观测。CLI validation order independent，+write-prefetch=1要求mode2/keep-q1/natural/shared2048/cutover0/pause0，打印WRITE-PREFETCH-JSON optin/actualready，不能把plan当成功。不改memorymodel、不要移除任何checks。

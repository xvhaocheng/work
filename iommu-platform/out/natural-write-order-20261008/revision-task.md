只做已交付普通生成脚本的小范围修订，不重写整个脚本或调度设计，不执行/写文件。实际运行原脚本失败：
Traceback apply_natural.py:831 AssertionError: nat_vaw_valid count 2 != 3
TOP_REPL_PORTS+ASSIGN实际只出现port nat_vaw_valid和assign lhs nat_vaw_valid两次，rhs是i_dma.tr_aw_valid，不是nat_vaw_valid。脚本至今未生成system_top/natural_test/build文件。

交付一个短小Python精确替换脚本 apply_worker_revision.py：仅修改 out/natural-write-order-20261008/apply_natural.py，保留原答复/原生成脚本副本（Codex会先保存）；每anchor assert唯一。要求：
1 改错误自检为准确count2、准确注释，不删自检。
2 +natural flag严格0|1、+natural-bytes严格完整数字2048|32768拒绝trailing垃圾（parser里endptr核）；natural-first/trace验证已有保持。
3 natural仅mode=s及无owner/read_admit/ds_priority/cpu-write组合可运行；不让run_natural reset(false)与mainpolicy参数产生矛盾（可以ifnatural &&mode!="s" return2，run_natural明确policy_partition=false，owner_only=false）。CPU/batch/window不引入；实际本轮执行所有这些已是0/none。
4 终验必须核虚拟W无unattributed/len mismatch；若可能存在合法W-before-AW，这个观察器暂无法配对，CHECK报“observer cannot pair legal W-before-AW; not a DUT verdict”，不要宣称DUT违约。终验data byte正确之外观察结果要可靠。
只上述四个改动，不改变DDR规则、任务长短/IOVA/PA/tick采样或原正常run_scenario。
构建TEST_COMPLETION_GATE宏、计数器观察和生产根因判定由Codex自行审定执行，不交给你也不要在本revision做。
短摘要+完整短脚本 fenced python即可。

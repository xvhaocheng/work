只修订随附 summarize-revised.py 的3个局部错误，输出适用 apply_patch 的完整精确补丁，不输出整文件，不修改其他文件。
实际运行: summary.json写成功且8个结果全PASS、fresh6/reused2，但write_md第464行KeyError:'total'。main传入的是顶层summary，write_md误用summary['total']等；顶层notes/not_evaluated也不在该层(main写的是summary['summary']['notes']之外? 仔细读真实脚本：主字典最后notes/not_evaluated在顶层，write_md后面summary["notes"]需要保持)。修正总数读取在一个局部变量/counts或调用接口，保证后面的notes读取仍正确。
此外Codex验收的两个确切缺口：
1 最后exit判定必须包含6项trace_equivalence等同全部True，而非只allrunsPASS/accounting；输出summary字段trace_equivalence_ok。
2 childoffset检查现在sorted()抹掉实际顺序。检查每owner 16父按0..15，每父实际打印顺序offset=[base,base+1024]；同时每owner打印全64? 两owner各32children，已有其余checks。引入实际顺序校验，不把排序coverage误称顺序验收。
3 scan_log NATURAL-JSON JSONDecodeError目前pass可能吞坏记录。只有一条预期记录，非法JSON需DataError注明，不能丢弃；不涉及DUT重跑。请保持其他行为/真实结果不变，给补丁和简短解释，不重构/添加依赖/新实验。

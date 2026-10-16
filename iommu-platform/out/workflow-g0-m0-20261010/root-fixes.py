from pathlib import Path
p=Path('out/workflow-g0-m0-20261010/proposed/tests/nonblock_actual_dma_test.cpp')
s=p.read_text()
def rep(a,b):
 global s
 assert s.count(a)==1,(a[:100],s.count(a))
 s=s.replace(a,b)
rep('const bool in_v = (a0 >= V_DST_PA_LO && a1 < V_DST_PA_HI);','const bool in_v = (a0 >= v_measure_dst_pa() && a1 < v_measure_dst_pa() + PAGE_BYTES);')
rep('CHECK(in_v || in_a,\n                          "full-batch AW payload outside the completion payload map");','CHECK(chn == 0 ? in_v : in_a,\n                          "full-batch AW payload outside its channel destination map");')
rep('CHECK(in_v || in_a,\n                      "full-batch AR payload outside the completion source map");','CHECK(chn == 0 ? in_v : in_a,\n                      "full-batch AR payload outside its channel source map");')
rep('    void report_full(const char* name, bool enable) {','    void report_full(const char* name, bool enable) {\n        CHECK(!policy_partition || top->dbg_policy_elaborated,\n              "p/pn require a PolicyEnable=1 elaboration");')
rep('    if (full_watchdog <= 0) {','    if (start_delay > 2000000u || v_start_delay > 2000000u || a_start_delay > 2000000u) {\n        std::fprintf(stderr, "launch delays must be <= 2000000 cycles\\n");\n        return 2;\n    }\n    if (!full_batch && (start_delay || v_start_delay || a_start_delay || launch_order != "av" || full_watchdog != 200000L)) {\n        std::fprintf(stderr, "launch delays/order/watchdog require full-batch\\n");\n        return 2;\n    }\n    if (full_watchdog <= 0) {')
rep('    // Per-delay cap used when SIZING the scenario budget.  A larger requested\n    // launch delay is accepted by the harness but cannot inflate the budget\n    // without bound.','    // Per-delay cap, enforced by argument validation before simulation.')
rep('    // never read win_open; win_open is set at t0 ONLY for the pre-existing\n    // "ds_priority && win_open" model intervention.','    // never read win_open. DS priority uses full_era and V completion directly.')
rep('    // scenario above is byte-for-byte unchanged.','    // scenario above keeps its original measurement contract.')
rep('        CHECK(n_ddr_ar > 0, "no shared-DDR reads observed");','        CHECK(top->dbg_cpu_rd_err == 0, "synthetic CPU read error");\n        CHECK(n_ddr_ar > 0, "no shared-DDR reads observed");')
p.write_text(s)
p=p.with_name('test_nonblock_actual_dma.py');s=p.read_text()
rep('    if opts.start_delay < 0 or opts.v_start_delay < 0 or opts.a_start_delay < 0:\n        ap.error("--start-delay/--v-start-delay/--a-start-delay must be >= 0")\n    if opts.full_watchdog <= 0:', '    if any(v is not None and not 0 <= v <= 2000000 for v in\n           (opts.start_delay, opts.v_start_delay, opts.a_start_delay)):\n        ap.error("launch delays must be 0..2000000")\n    if opts.full_watchdog is not None and opts.full_watchdog <= 0:')
p.write_text(s)
p=Path('out/workflow-g0-m0-20261010/revision-block-1.py');s=p.read_text()
rep('    return recs[0]', '    if len(recs) != 1:\n        raise SystemExit("expected exactly one full-batch record")\n    if "nonblock_actual_dma_test: PASS" not in Path(path).read_text() or "SCENARIO-FAIL" in Path(path).read_text():\n        raise SystemExit("run must finish with PASS and no SCENARIO-FAIL")\n    return recs[0]')
rep('    check_channel(by_id["A"], target, fails)', '''    check_channel(by_id["A"], target, fails)
    for label in CHANNEL_IDS:
        c = by_id[label]
        if len(c.get("ready", [])) == target and len(c.get("done_cyc", [])) == target and len(c.get("issue_cyc", [])) == target:
            _need(c["ready"][0] == rec.get("t0"), label + ": first ready != t0", fails)
            _need(c["first_issue"] == c["issue_cyc"][0], label + ": first issue mismatch", fails)
            _need(c["final_done"] == c["done_cyc"][-1], label + ": final done mismatch", fails)
            _need(rec.get("T_" + label) == c["final_done"] - rec.get("t0", 0), label + ": elapsed time mismatch", fails)''')
rep('    add("clear_quiet",','    add("shift_t0", lambda r: r.__setitem__("t0", r["t0"] + 1))\n    add("false_T_V", lambda r: r.__setitem__("T_V", r["T_V"] + 1))\n    add("clear_quiet",')
Path('out/workflow-g0-m0-20261010/proposed/tests/test_full_batch_contract.py').write_text(s)

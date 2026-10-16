以下为**源码级修订提案**，未执行编译/仿真；请 Codex 在本地落盘后验证。修订范围仅 `tests/composition_control_test.cpp`，不改生产 RTL、不放宽 assert。

## 1. 三处失败根因确认

| 失败 | 根因 | 证据 |
|---|---|---|
| `open plru got0 want0x12345678` | `idle()` 会清 `plru_i`；测试先写 `plru_i` 再调 `idle()`，导致 open 采样到 0 | 测试 line 227-228；`idle()` line 62 `dut->plru_i = 0;`；RTL open 采样 line 261 `plru_open_d = plru_i;` |
| `close plru got0 want0xcafebabe` | 同上，先写 `plru_i` 再 `idle()`，close 采样到 0 | 测试 line 342-344；RTL close 采样 line 273 `plru_close_d = plru_i;` |
| `repl_pscid2 got3 want2` | 实际替换事件为 3 次：entry0、entry1、entry2；entry1 虽触发 partition violation，但 RTL 仍把 valid 且 PSCID 改变的事件计入 `W_REPL` | 测试 line 313-315、321-323、340-347；RTL line 404-409 对 `entries_i[repl_idx]` valid 且 PSCID 变化无条件 `cnt_d[W_REPL]++` |

## 2. 精确 old/new JSON 修该 3 处

```json
[
  {
    "file": "tests/composition_control_test.cpp",
    "line": 227,
    "old": "    dut->plru_i = 0x12345678;\n    idle(); dut->open_i = 1; tick(); idle(); settle();",
    "new": "    idle();\n    dut->plru_i = 0x12345678;\n    dut->open_i = 1; tick(); idle(); settle();",
    "why": "idle() line 62 clears plru_i; open captures plru_i at RTL line 261, so set plru after idle."
  },
  {
    "file": "tests/composition_control_test.cpp",
    "line": 342,
    "old": "    dut->plru_i = 0xCAFEBABE;\n    idle();\n    dut->close_i = 1;",
    "new": "    idle();\n    dut->plru_i = 0xCAFEBABE;\n    dut->close_i = 1;",
    "why": "idle() line 62 clears plru_i; close captures plru_i at RTL line 273, so set plru after idle. 后续 update_i/replace_mask_i/update_did_i 保持在 close_i 之后设置即可。"
  },
  {
    "file": "tests/composition_control_test.cpp",
    "line": 355,
    "old": "    check(\"repl_pscid2\", rd(29), 2);",
    "new": "    check(\"repl_pscid2\", rd(29), 3);",
    "why": "三次实际替换：entry0 lines 313-315、entry1 lines 321-323（Pviol 但仍是实际事件）、entry2 lines 340-347；RTL lines 404-409 每次都 W_REPL++。"
  }
]
```

## 3. 新增 `test_legal_modes()` 插入点

建议插在 `test_abnormal_lookup()` 之后、`test_gen_full()` 之前：当前文件 line 409 与 line 411 之间。

锚点 JSON：

```json
{
  "file": "tests/composition_control_test.cpp",
  "old": "static int test_gen_full() {",
  "new": "static void test_legal_modes() {\n    ...\n}\n\nstatic int test_gen_full() {"
}
```

完整函数代码：

```cpp
static void test_legal_modes() {
    for (int m = 0; m < 4; m++) {
        reset_dut();

        uint64_t gen_before_cfg = rd(3);
        bool clear = wr_cfg(((uint64_t)10 << 8) | (uint64_t)m, true, true);
        check("legal cfg clear", clear ? 1 : 0, 1);
        check("legal cfg status", rd(4), 0);
        check("legal cfg mode", rd(2) & 3, (uint64_t)m);
        check("legal cfg gen", rd(3), gen_before_cfg + 1);

        idle(); dut->arm_i = 1; tick(); idle(); settle();
        check("legal armed", (rd(6) >> 0) & 1, 1);
        uint64_t win_cfg = rd(10);
        uint64_t win_gen = rd(11);
        check("legal win cfg", win_cfg, rd(2));
        check("legal win gen", win_gen, rd(3));

        for (int i = 0; i < 16; i++) {
            set_entry(dut->entries_i, i, false, 0);
            set_entry(dut->entries_next_i, i, false, 0);
        }
        idle(); dut->open_i = 1; tick(); idle(); settle();
        check("legal active", (rd(6) >> 1) & 1, 1);

        for (int i = 0; i < 3; i++) {
            idle();
            dut->lookup_i = 1; dut->lookup_did_i = 10; dut->normal_i = 1;
            dut->stage1_i = 1; dut->stage2_i = 0; dut->lookup_hit_i = 1;
            dut->lookup_pscid_i = 0x200 + i;
            tick(); idle(); settle();
        }
        check("legal lookup_v", rd(12), 3);
        check("legal hit_v", rd(13), 3);
        check("legal miss_v", rd(14), 0);
        check("legal conserve v", rd(13) + rd(14), rd(12));

        if (m == 1 || m == 3) {
            idle(); dut->accept_i = 1; dut->fast_i = 1; dut->fast_did_i = 10;
            tick(); idle(); settle();
            check("legal fast_v", rd(21), 1);
            check("legal fast_n", rd(33), 0);
        } else {
            check("legal fast_v off", rd(21), 0);
            check("legal fast_n off", rd(33), 0);
        }
        check("legal fast_a", rd(22), 0);
        check("legal fast_o", rd(23), 0);

        set_entry(dut->entries_i, 0, true, 1);
        set_entry(dut->entries_next_i, 0, true, 2);
        idle(); dut->update_i = 1; dut->replace_mask_i = 1 << 0; dut->update_did_i = 10;
        tick(); idle(); settle();
        check("legal fill_v", rd(26), 1);

        set_entry(dut->entries_i, 2, true, 1);
        set_entry(dut->entries_next_i, 2, true, 2);
        idle(); dut->update_i = 1; dut->replace_mask_i = 1 << 2; dut->update_did_i = 11;
        tick(); idle(); settle();
        check("legal fill_o", rd(28), 1);

        if (m == 2 || m == 3) {
            check("legal part zero", rd(30), 0);
            check("legal err part zero", (rd(9) >> 4) & 1, 0);
        }

        idle(); dut->gap_active_i = 1; tick(); idle(); settle();
        check("legal gap_v", rd(24), 1);

        idle(); dut->close_i = 1; tick(); idle(); settle();
        check("legal frozen", (rd(6) >> 2) & 1, 1);
        check("legal err zero", rd(9), 0);

        uint64_t frozen_cycles = rd(8);
        uint64_t frozen_cfg = rd(10);
        uint64_t frozen_gen = rd(11);
        clear = wr_cfg(((uint64_t)10 << 8) | 0, true, true);
        check("legal postclear", clear ? 1 : 0, 1);
        check("legal postmode S", rd(2) & 3, 0);
        check("legal postcycles", rd(8), frozen_cycles);
        check("legal postwin cfg", rd(10), frozen_cfg);
        check("legal postwin gen", rd(11), frozen_gen);
        check("legal posterr", rd(9), 0);
    }
}
```

## 4. `main()` 调用插入

锚点 JSON：

```json
{
  "file": "tests/composition_control_test.cpp",
  "old": "    test_reset_and_config();\n    test_window_events();\n    test_abnormal_lookup();\n\n    delete dut;",
  "new": "    test_reset_and_config();\n    test_window_events();\n    test_abnormal_lookup();\n    test_legal_modes();\n\n    delete dut;"
}
```

## 5. 假设与验收检查

假设：
- `rd(29)` 是总替换计数 `W_REPL`，不是 PSCID2 专属计数；名称 `repl_pscid2` 沿用原测试。
- `test_legal_modes()` 只默认 generation 构建下运行；gen-full 构建仍在 main 入口通过 `test_gen_full()` 提前返回。
- 模式编码：S=0、N=1、P=2、PN=3；N 启用模式指 mode 1 和 3。
- “frozen 后写 config 恢复 S” 按实际 RTL 解释为 `mode_q` 恢复为 `MODE_S`，窗口 `win_cfg_q/win_gen_q` 保留；state 仍为 `S_FROZEN`，测试不假设 state 回 `S_OFF`。

建议验收检查：
1. default 构建：`open plru`、`close plru`、`repl_pscid2` 三处失败应消失。
2. `test_legal_modes()` 对 m=0..3 每轮 `rd(9)==0`；N/PN 轮 `rd(21)==1 && rd(33)==0`，S/P 轮 `rd(21)==0 && rd(33)==0`。
3. 每轮 `rd(13)+rd(14)==rd(12)`；P/PN 轮 `rd(30)==0` 且 `err[4]==0`。
4. 每轮 frozen 后写 config：`rd(2)&3==0`、`rd(8)`/`rd(10)`/`rd(11)` 与写前一致、`rd(9)==0`。
5. gen-full 构建仍 exit 0。
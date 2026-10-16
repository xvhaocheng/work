## 结论（有界测试兼容修复）

三处判定：

1. **SV 注释**：`tests/composition_control_top.sv:5` 以 `// Verilator ...` 开头，Verilator 把 `//` 后第一个词是 `verilator`（大小写不敏感）的注释当成元指令，于是报 `Unknown verilator comment`。必须让该行注释不再以 `verilator` 起头。改动第 4–6 行注释块，去掉起首的 “Verilator”。这不是放宽检查，纯注释改写。

2. **C++ `set_entry`**：`tests/composition_control_test.cpp:97` 硬编码 `VlWide<64>&`。改为 `template <typename Wide>` 接受任意实际生成的存储类型（无论是 4.038 的 `VlWide<64>` 还是旧的 `WData/IData[64]` 数组）；函数体 `flat[base]` 索引对两者都成立，`(IData)` 转换与赋值语义不变。断言全部保留。

3. **generationfull 判定**：**无需改动**。`main()` 已在 `tests/composition_control_test.cpp:427` 用 `if (rd(3) == 0xFFFFFFFFFFFFFFFFULL)`（即 `rd3==max`）选择 `test_gen_full()`，与 `test_gen_full()` 内部 `:412/:417` 的判据一致，不依赖任何 C++ 端宏。当前 `--define` 只传 SV 是正确的，保持原样。python `tests/test_composition_control.py:41-42` 只在 `gen_full` 构建附加 `+define+CVC_TEST_GEN_FULL`（仅 SV 可见），符合该方案，无需改脚本。

## 精确 old/new（仅替换，未重发完整代码）

```json
{
  "replacements": [
    {
      "file": "tests/composition_control_top.sv",
      "old": "// No SV timing constructs.  The flat packed entry ports are retained for\n// Verilator public access and converted internally to the unpacked arrays used\n// by the production control block.",
      "new": "// No SV timing constructs.  The flat packed entry ports are retained for\n// direct public access and converted internally to the unpacked arrays used\n// by the production control block."
    },
    {
      "file": "tests/composition_control_test.cpp",
      "old": "static void set_entry(VlWide<64> &flat, int idx, bool valid, uint32_t pscid) {",
      "new": "template <typename Wide>\nstatic void set_entry(Wide &flat, int idx, bool valid, uint32_t pscid) {"
    }
  ]
}
```

替换 2 的调用点（`tests/composition_control_test.cpp:223`、`:224`、`:312`、`:313`、`:320`、`:321`、`:339`、`:340`、`:385`）无需修改，模板实参由 `dut->entries_i` / `dut->entries_next_i` 的实际类型推导。

## 未验证 / 假设项（交由 Codex 执行验证）

- `cva6/corev_apu/fpga/src/dma_composition_control.sv` 源码未提供，**未验证** `CVC_TEST_GEN_FULL` 是否确实把 generation 饱和到 `0xFFFFFFFFFFFFFFFF`，也未验证默认清零。`rd3` 选择逻辑只有在 SV 侧真正响应该宏时才成立。
- Verilator 4.038 对 `logic [15:0][127:0]` 端口实际生成的 C++ 成员类型**未实测**。假设其为 `VlWide<64>`（2048/32 位字）。若该类型**没有非 const `operator[]`**，`flat[base] = ...` 仍会编译失败；此为唯一残余风险，备选是索引其内部存储（不要引入现代类）。模板方案对 “`WData/IData[64]` 数组” 这一旧类型假设可直接工作。
- 顶层模块端口在 `--cc --exe` 下是否默认公开（`dut->entries_i` 可直接访问）：**未实测**；原注释提到的 “public access” 已删除，若 4.038 需要 `/*verilator public*/` 才能访问这些端口，则需另行补充公共性声明（当前判断是顶层端口默认公开，故未加）。
- `+define+CVC_TEST_GEN_FULL` 追加在文件参数之后是否被 4.038 接受为等价 `-D`：**未实测**（预期接受）；若要更稳可移到文件之前，但任务要求仅在有需要时改，故未改。
- 未运行任何构建/测试；`exit 2` 目前只归因于题面给出的 `Unknown verilator comment`。替换 1 是否同时消除 4.038 对第 5 行的误判，需实际重跑 `python3 tests/test_composition_control.py` 确认；替换 2 的编译兼容性同样需重跑确认。

## 建议验收检查

- `python3 tests/test_composition_control.py` 默认构建：编译通过、打印 `PASS`、返回 0。
- 同命令 `gen_full` 构建：编译通过、`main()` 经 `rd(3)==max` 进入 `test_gen_full()`、返回 0。
- 确认编译日志中不再出现 `Unknown verilator comment`，且 `set_entry` 无模板实例化/类型不匹配错误。
- 逐项确认原有 `check(...)` 断言数量与内容未减少（本次未触碰任何断言）。
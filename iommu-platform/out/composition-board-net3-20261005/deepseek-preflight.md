# 只读分析：新 BIT 上板后执行既定 172 项实验的前置结论

> 说明：以下全部基于本次提供的文件（`scripts/mac_composition.py`、`scripts/composition_plan.py`、`scripts/mac_research2.py`、四份 doc）。我未执行任何命令，未上板、未加载 BIT、未写卡。凡不能从这些源码直接证明的，均标为**未确定**，不假设 ABI/裁剪安全。BIT 选择、最终执行、板测判决由 Codex/用户负责。

---

## 0. 导航摘要（权威来源）

| 主题 | 权威来源 | 关键行 |
|---|---|---|
| 172 项冻结计划、每例参数、记录校验 | `scripts/composition_plan.py` | 24–60, 143–198, 224–440 |
| attach-only runner、--abi 能力门、--out 语义、停机 | `scripts/mac_composition.py` | 33–49, 111–124, 174–239, 262–499, 502–572 |
| 串口协议、帧标记、超时、`SerialConsole` | `scripts/mac_research2.py` | 48–62, 81–82, 322–519, 579–613 |
| 上板/运行/回传操作说明 | `docs/composition-v1-mac-20261004.md` | 19–21, 57–88, 90–98 |
| 冻结接口（含裁剪修订说明） | `docs/composition-v1-spec-20261004.md` | 3, 6–7, 42–48 |
| 裁剪范围与 route-opt 验收 | `docs/composition-trim-20261005.md` | 13–18, 36–42, 45 |
| net3 实板入口、SSH 重载 | `docs/software-update-20261004.md` | 137–199 |

**重要前提**：`docs/composition-v1-mac-20261004.md:79` 示例里的 `--expected-kernel` 是 `6.5.0-rc2-cpn1-...`，**已过期**。本轮实际内核为 `6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty`（用户给定 + `docs/software-update-20261004.md:143`）。沿用旧示例会在第一个用例前即停机（见 Q2）。

---

## 1) 172 项单次执行命令、依赖、参数、输出/停机/回传

### 1.1 计划构成（可核算，`composition_plan.py`）
- short：`len(MODE_ORDER)=4` × `len(A_PAGE_CHOICES)=4` = 16（`:146–150`）
- noise：`NOISE_PAIRS=6` × 2 slot = 12（`:151–155`）
- explore：`EXPLORE_BLOCKS=3` × 3 gap × 4 a_pages × 4 mode = 144（`:156–165`）
- 合计 `count = 172`（`:175`；与 spec `:48` 一致）
- 单例 argv 固定为：`--device <dev> --mode <0/1/2/3> --a-pages <0/1/8/16> --v-gap <0/1000/4000> --transfers <16|4096> --warmup <16|256> --timeout-ms 15000`（`:189–198`，常量 `:37–49`）。protected DID 不在 argv 内，由 CLI 默认 10，校验时按 `PROTECTED_DID=10` 断言（`:52`, `:253`）。

### 1.2 单次执行命令（172 全量）
```
cd '/Users/jojo/Desktop/在mac链接pfga(codex)'
tools/method88-venv/bin/python tools/composition-v1-20261004/mac_composition.py \
  --run \
  --through-phase explore \
  --out results/composition-v1-20261004-01 \
  --serial /dev/cu.usbserial-XXXX \
  --expected-kernel '6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty'
```
- `--through-phase explore` 是**必需**：默认是 `short`，只跑 16（`mac_composition.py:515`）。
- `--out` 目录**必须不存在**，否则 `SystemExit('--out already exists')`（`:555–556`）。不用先跑 `--plan`（`:551–554` 只在 `--plan` 或未给 `--run` 时生成计划；doc `:82`）。
- `--serial`：源码默认硬编码 `/dev/cu.usbserial-AV0KRCHT`（`:33`），实际设备名不同时必须显式覆盖。
- `--board-cli` 默认 `/usr/bin/idma_composition`（`:34`），`--device` 默认 `/dev/idma0`（`:35`）；仅在板端不同时覆盖。
- **一次执行**：`--run` 内部先生成完整 `plan.json` 再顺序跑 172（`:450–487`）；无 resume、无自动重跑（`:457–458`, `:487`）。

### 1.3 依赖
- Python 环境含 pyserial（`mac_research2.py:322–332`、`run_preflight` 里的 pyserial 检查路径）；doc 指定 `tools/method88-venv/bin/python`（doc `:21`）。
- **三个 .py 必须平铺在同一目录**：runner 从**自身所在目录**加载 `composition_plan.py` 与 `mac_research2.py`（`mac_composition.py:72–98`；doc `:19`）。因此 `tools/composition-v1-20261004/` 内需同时有 `mac_composition.py`+`composition_plan.py`+`mac_research2.py`。`mac_research2.SerialConsole` 必须暴露 `start/close/run_shell/drain_input/disable_echo`（`:95–97`），现有副本满足。
- `lsof` 可执行文件需在 PATH（`:117–120`）——macOS 上在 `/usr/sbin/lsof`，若 PATH 缺 `/usr/sbin` 会抛 `CompositionError('lsof is not available')` 直接停机。**未确定**：调用时 PATH 是否包含 `/usr/sbin`。
- `openFPGALoader` **不是**本 runner 的依赖（加载 BIT 是独立步骤）。

### 1.4 输出物（`--out` 目录）
- 顶层：`plan.json`、`manifest.json`（`:451–469`）、`boot.json`（`:473`）、`run.log`（`:146–153`）、`console.raw.log`/`console.events.log`（`:562–563`）、`dmesg.before.txt`、`dmesg.<phase>.end.txt`、`dmesg.failure.txt`（`:476/485/492/497`）、`summary.json`（`:425–437`）。
- 每例：`cases/<cid>/record.raw.json`、`record.json`、`stderr.txt`、`metrics.json`、`meta.json`（`:311–326, 358–393`）。
- 停机：`STOPPED.txt`（写入 `stopped_at/reason/phase/case`，`:158–169`）。
- **注意**：compose runner 不打包板端目录、不算 SHA、不传板端归档；`record` 经 UART 帧直接取回（`:281–291`）。因此**不需要** SSH/scp 板端产物；也不做板端哈希核对。

### 1.5 错误停机 / 回传
- 技术失败即停：任何 `CompositionError` → `_stop()` + `dmesg.failure.txt` + `summary.status='stopped'`，随后**重新抛出**，进程非零退出（`:495–499`）。`KeyboardInterrupt` → `status='interrupted'`（`:490–494`）。
- 科学阴性不是技术失败（`:420–422`；`composition_plan.py:227–229, 422`）。
- 回传：runner **不**做回传；按 doc `:90–96` 把 `--out` 整个目录送到 `xvhaocheng@dell:/home/xvhaocheng/work/iommu-platform/out/mac-composition-v1-20261004-01/`（不覆盖已有）。**未确定**：实际 scp/rsync 命令与目标目录是否已存在；需以 Codex 侧既有回传流程为准。

### 1.6 前置的 BIT 加载（独立步骤，不选 BIT）
按用户给出与 doc `:63–65`、`software-update-20261004.md:168–176`，加载是两步：
- 直接方式：`openFPGALoader -b genesys2 --freq 1000000 '<BIT>'`
- 或 `scripts/mac_update_reboot.py --run --transport ssh --credential-script scripts/run_research2_mac.py --board-host 192.168.77.2 --host-ip 192.168.77.1 --bit '<BIT>' --expected-kernel '<same>' --out "results/linux-reload-<ts>" --boot-timeout 600 --jtag-hz 1000000`（**省略 `--payload` 即只同步+重载**，doc `:176`）。
- **未确定/待核**：`scripts/mac_update_reboot.py` 源码未提供；需确认 SSH 模式下是否占用/释放 UART、是否要求板端可 SSH（net3 已验收静态网，doc `:137–199`）。

---

## 2) UART 登录/独占、SSH 支持、实际协议与超时、当前 Linux 能否复用

### 2.1 独占要求（只读，不杀进程）
- `check_uart_ownership`（`mac_composition.py:111–124`）：设备必须存在，否则停机；`lsof -- <dev>` 返回 0 且 stdout 非空 → 判定被占用并停机。**不 kill**。因此运行前必须关闭任何临时串口采集终端（doc `:67`）。
- `main` 在创建 `--out` 前先做该检查（`:557`），再 `mkdir`（`:561`）、再打开串口（`:562–565`）。

### 2.2 登录：**runner 不做登录**
- `_wait_for_shell`（`:174–193`）只反复 `true` 探测并要求 `exit 0`，**不发送用户名/口令、不处理 login 提示**。`SerialConsole` 也只 `disable_echo`（发 `stty -echo`，`mac_research2.py:407–419）后发帧命令。
- 推论：**启动前 UART 必须已经是登录后的 root shell（或系统自动 root 登录）**。否则命令会被当作登录名/口令输入，`attach` 30s 超时后停机（`:175/193`）。
- doc 假设“Linux 起到 root shell”（doc `:67`）。**未确定**：本轮内核是否自动 root 登录；`scripts/run_research2_mac.py` 有登录响应逻辑（`software-update-20261004.md:163,172`），但 compsotion runner **不导入**它。

### 2.3 是否支持 SSH
- **`mac_composition.py` 不支持 SSH**：无 `--transport` 之类参数（参数表 `:502–519`），只用 `mac_research2.SerialConsole`（串口）。
- SSH 仅用于**加载 BIT 的独立步骤**（`mac_update_reboot.py --transport ssh`）；172 项执行本身仍走 UART。
- 当前 net3 Linux 具备 SSH/静态网（`software-update-20261004.md:137–199`），因此“能否复用”在链路层面=可以，但**没有实现 SSH 版 runner**，本任务不得新建框架，故只能走 UART。

### 2.4 实际协议与超时
- 物理：pyserial，默认 115200、8N1（`mac_research2.py:62, 322–332`；`SerialConsole.start` 读超时 0.1，`:363`）。
- 帧协议（`mac_research2.py:335–519`）：每条命令一行；先打印由随机 token 拼出的 `__R2B_<t1>_<t2>__` 开始标记、执行命令、再打印 `__R2E_<t1>_<t2>__` 与退出码（`BEGIN_PREFIX/END_PREFIX` `:81–82`；token `:461–462, 488–490`；标记由 shell 变量拼装，避免回显误判 `:511–517`；`read_until` `:427–442`；退出码正则校验 `:472–473`）。`run_shell` 拒绝含换行的命令（`:482–483`），可选 `cwd`/`exit_var`（`:493–505`）。
- runner 超时（`mac_composition.py:39–42`，可用同名参数覆盖 `:516–518`）：
  - `attach_timeout=30s`，`command_timeout=60s`，`case_timeout=300s`，`dmesg=60s`。
- 身份门（`:195–239`）：`uname -r` 必须 == `--expected-kernel`（否则停机）；`boot_id` 必须为 UUID；`tainted` 必须为 `0`（`:218–219`）；`idma_composition --abi` 必须 exit 0 且 JSON 满足 `schema='idma-composition-tool-v1'`、`record_schema='idma-composition-v1'`、`composition_abi=1`（`:233–238`）。每例前后二次读 taint，并要求 `boot.after == boot_id`（`:335–340`）。
- 当前 Linux 能否复用：能——需要 net3 已提供的 root shell、`/dev/idma0`、`/usr/bin/idma_composition`、`tainted=0`。任一不满足即在前几秒停机（这是**安全门**，不是缺陷）。

---

## 3) composition 裁剪两个旧 observer 对“runner 能力检查 / 数据验收”的影响

### 3.1 能力检查：**不受影响**（有源码依据）
- composition runner 的能力门只查板端 CLI 的 `--abi`：`mac_composition.py:221–238`，三项 `schema/record_schema/composition_abi`（常量 `:48–49`）。**完全不查 `--caps2`、不查 observer_magic。**
- 对照：research2 runner 的能力门才依赖被裁掉的 observer：`mac_research2.py:579–613`，尤其要求 `observer_magic == OBSERVER_MAGIC (0x49444D414F425308)`（`:49, 610–612`），并校验 `features`（`:54, 604–609`）。裁剪会打断的是**research2 的 caps 路径**，不是 composition runner。
- 裁剪范围：`docs/composition-trim-20261005.md:14–16`——仅 composition profile（`IOMMUNonblockMode=3`）禁用 `dma_research_observer` 与 `dma_nonblock_observer`，两旧 bank 读出 0、不假造 capability、`research_reverse=0`；保留 `PathEnable/P4MEnable/ResearchEnable`、通用 `dma_observer`/path 统计、composition 128 rawwords、开/关快照、IOTLB/P/N 逻辑。spec 修订注明确“旧 D00–D1F 不变为修订前描述，现读 0”（`docs/composition-v1-spec-20261004.md:3`）。
- 结论：**只要板端 `idma_composition --abi` 仍返回三项正确值，裁剪不会卡住 runner 的能力门。**（裁剪文档称无 Linux/ABI/CLI 改动，`composition-trim-20261005.md:45`；但这只是文档结论，需实测 `--abi` 或读 CLI 源码确认——见 Q4。）

### 3.2 数据验收：**存在未证实依赖，不能因“ABI 兼容”就判无风险**
- 每例验收 `validate_case` 要求 `result.p4m` 为对象，并强制：
  - `p4m.observer_status == 4`（`P4M_OBSERVER_STATUS_OK`，`composition_plan.py:65, 302–320`，判等在 `:316–317`）；
  - `p4m.window` 恰 16 words、`p4m.observation` 恰 35 words（`:66–67, 308–315`）、`p4m.sequence > 0`（`:318–320`）；
  - `raw` 恰 128 words（`:60, 292–296`）。
- 冻结一致性 `_validate_frozen` 进一步要求：
  - `raw[6]` 冻结位、`raw[7]>0`、`raw[8]>0` 且 `raw[8]==window[1]`（`:383–392`）；
  - `raw[33]==0`（N 关闭时不得有 fast 事件，`:395–397`）；
  - V/A/other lookup 守恒（`:399–407`）；
  - `window[13]==window[14]==transfers`，`observation[7]==observation[8]==transfers`（`:424–429`）；
  - Solo/aggressor 结构（`:431–439`）。
- **风险点**：`p4m.window/observation`/`observer_status` 来自哪一路 observer，**无法从本三份源码证明**。裁剪移除的是 `dma_research_observer`/`dma_nonblock_observer`；若 `p4m` 恰好由被移除的 observer 供数，则 `observer_status` 可能≠4 或 window/observation 计数不满足 `==transfers`，导致**每例首例即判失败并停机**。trim doc 只声明保留 `P4MEnable` 与通用 path 统计（`:16`），并未给出 `observer_status=4` 在裁剪后仍成立的证据。→ 列为**未确定，必须由 RTL/驱动源证实**（Q4）。
- 反向风险（假阴性/弱化）：`raw[33]==0` 若由被裁掉或置零的计数器产生，则该检查可能**恒真**，掩盖真实“N 关闭时 fast”事件。同样需确认 `raw[33]` 来自保留的 composition 128-word bank（spec `:19, 33`）。
- 已明确不参与验收、因此不受裁剪影响的项：`hpm` 标为 raw_only、非有效指标（`mac_composition.py:420`；`composition_plan.py:478`）；`raw[29]/[30]/[31]/[32]/[34]/[35]` 未被 `_validate_frozen` 断言（`:362–440` 未引用）。这降低但也说明验收覆盖面窄。

---

## 4) 当前已知的关键证据缺口与所需下一文件

1. **裁剪后 `p4m.observer_status==4` 与 window/observation 供数链**（最高优先）。需读：实现 P4M window/observation 与状态位的 RTL，及裁剪 diff；驱动中填充 `struct idma_p4m` 的代码（`docs/composition-v1-spec-20261004.md:43`）。没有它，Q3.2 无法签核。
2. **板端 CLI 与设备就绪**：`bench/idma_composition.c`（或对应 CLI 源）确认 `--abi` 字段；确认 board 上 `/usr/bin/idma_composition`、`/dev/idma0` 存在。runner 会实测但不预检。
3. **实际 BIT 身份绑定**：runner **无 `--bit`、不校验 BIT 哈希**，manifest 只记录 kernel/cli/device（`mac_composition.py:453–469`）；`tools/` 中的 BIT 与实际加载的 BIT 无法由 runner 绑定。需确定 BIT 身份记录方式（加载日志或板端只读字段），否则 172 结果无法与具体 BIT 对应。
4. **`scripts/mac_update_reboot.py` 源**：确认 `--transport ssh` 无 `--payload` 重载模式下是否占用/释放 UART、是否触发 `boot_id` 变化、正确参数与超时。
5. **内核 release 权威文件**：`artifacts/software/software-update-autonet-20261005/delivery/kernel-release`（`software-update-20261004.md:150`）——用于填 `--expected-kernel`，不要硬猜。
6. **登录/自动 root shell 事实**：`scripts/run_research2_mac.py`（登录响应来源，`software-update-20261004.md:163`）与本轮串口是否已 root；决定是否需要开跑前手工登录。
7. **待确认副本一致性**：本次读的是 `scripts/mac_composition.py`；实际执行的是 `tools/composition-v1-20261004/mac_composition.py`。需确认两者一致（行数/内容），否则本文行号引用可能不适用于实跑副本。
8. **回传路径可用性**：Dell 目标目录是否可写/是否已存在（doc `:92–96` 要求不覆盖）。
9. **`tainted` 现值**：net3 驱动/环境是否导致 `/proc/sys/kernel/tainted != 0`（`:218–219` 会直接停机）。
10. **`lsof` PATH**：`/usr/sbin` 是否在 runner 进程 PATH（Q1.3）。

---

## 5) 运行预计时长（仅给证据可支持的范围）

- 结构证据：172 例；其中 16 short（16 transfers）、12 noise（4096 transfers）、144 explore（4096 transfers）（`composition_plan.py:37–45, 143–178`）。
- **唯一硬上界**：`case_timeout` 默认 300s（`mac_composition.py:41, 518`）。若假想每例都逼近超时：172 × 300s = 51,600s ≈ **14.3 h**；但任一例超时本身即技术失败→停机（`:341–342, 411–412`），因此该值只是绝对天花板，**不是预计值**。
- 可算的“故意等待”下界（仅编程 gap，非整例）：
  - explore `v_gap=4000`：4096 transfers × 4000 + warmup 256 × 4000 ≈ 1.74e7 cycles ÷ 50 MHz ≈ **0.35 s/例**；
  - `v_gap=1000`：≈ **0.087 s/例**；`v_gap=0`：0。
  （`PLATFORM_HZ=50000000`、`CYCLE_NS=20`，`composition_plan.py:56–57`；gap 语义见 spec `:48`——V 窗口含故意等待。）
- **结论**：由现有文件**无法给出可信的总时长区间**——掌握的数据只覆盖“编程等待”，而噪声/探索每例 16 MiB 的 V 数据搬运（4096×4096 B，spec `:48`）速率未知，文件内无实测每例墙钟。给不出“预计 30/60/120 分钟”之类数字。
  - 旁证**不可**用于本实验：`software-update-20261004.md:193–195` 的 24.34/34.45 分钟是“软件更新+JTAG 重载+联网验收”，明确“科学实验 0 次”，不能外推 172 项。
  - 若需真实区间，需要一份**已实测的每例耗时**（本批证据未提供）；在不要求“短跑再全量”的前提下，只能以 300s/例 为界并如实标注未知。

---

## 6) 未确定项 / 假设清单

**假设（用户给定，未由文件复核）**：实际运行内核 `6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty`；SD 更新与静态网络已验收；本轮不重编软件、不写卡；BIT 未定（可能 composition-trim route-opt，可能用户另一新编译）。
**未确定**：Q4 第 1–10 项；`tools/` 副本与 `scripts/` 副本一致性；UART 设备名；是否自动 root 登录；`lsof`/`openFPGALoader` 路径；Dell 回传细节。

**安全行为提示（结论性，非保证）**：即便加载的是“用户另一未确认新编译”，runner 会在身份门处（`--abi` schema/record_schema/composition_abi，`mac_composition.py:233–238`）在**任何用例前**停机，不会产生错误归属的数据。但该门**不校验 BIT 哈希**（见 Q4.3），故不能替代“BIT 身份证据”。

---

## 7) 建议的验收检查（供 Codex 执行验证，非已通过）

1. 运行前：`ls -l` 三个 .py 同目录；`lsof -- /dev/cu.usbserial-XXXX` 为空；`/proc/sys/kernel/tainted`=0（经既有 UART/SSH 只读）。
2. 运行前（只读）：板端 `uname -r` == 期望；`idma_composition --abi` 返回三项正确；`/dev/idma0` 存在。
3. 运行后：`--out` 下 `summary.json` 的 `completed==172 && failed==0`（`:415–437`）方为技术通过；出现 `STOPPED.txt` 即技术失败，记录 `reason/phase/case`（`:158–169`）。
4. 逐例核对 `cases/<id>/meta.json.problems` 为空、`metrics.json` 与 `record.json` 存在（`:393, 358–364`）。
5. 科学判读独立于技术通过：`fast==0/miss==0/PN 无收益` 不改变“技术通过”，也不自动构成机制/收益结论（`mac_composition.py:420–422`；`composition_plan.py:227–229`）。

（本文为只读结论与命令草案；请由 Codex 核对 Q4 所列文件后再执行，未执行任何步骤。）
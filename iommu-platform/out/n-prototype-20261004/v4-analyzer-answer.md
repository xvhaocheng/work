# Findings summary

- The existing analyzer (`scripts/analyze_nonblock_pilot.py`) already threads helper core words (`words[0..15]`), and the runner-facing diff you supplied writes a second 16-word bank into `helper.json` as `diagnostic = { words: [...16...], core_fast_sum: <int> }` and a verbatim `diag-window.json`.
- The diagnostic bank layout is confirmed by `cva6/corev_apu/fpga/src/dma_nonblock_observer.sv:216-232` (index 16..31) and `…:219-220` (`{overflow, 60'd0, frozen, active, armed}`), plus `rv_iommu_nonblock_hit.sv:114-131` (seen/stage event bits) and `rv_iommu_tl_wrap.sv:285-286` (bit0 = base S1 PTW, bits 15:1 = hit-module diag).
- I augmented (not replaced) the analyzer: CLI default path is byte-for-byte the old flow; `--diagnostics` is additive; new JSON fields are additive; only the RESULTS.md contrast column headers are renamed as requested.
- I could not run anything. All code below is a proposal; acceptance commands are at the end.

Assumptions (flagged):
1. Diagnostic source is `helper.json`'s `diagnostic` object (runner diff), with `diag-window.json` as an optional cross-check when present.
2. `flags` low-63 must equal `4` (frozen) and bit63 overflow must be `0`; I check both explicitly.
3. `core_fast_sum` is an optional verifier only; absence is not a problem.
4. Existing JSON keys (`layout_original_to_serial`, `bypass_serial_to_hit`) are kept for compatibility; new `column_labels`/`definition` strings carry the requested renaming. If you want the JSON keys themselves renamed, that is a one-line change but may break existing consumers.

---

FILE: scripts/analyze_nonblock_pilot.py

````python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""离线汇总冻结的 N 工程 pilot 三个 runner 输出目录（proposal，尚未执行）。

用途
====

把 ``scripts/mac_nonblock_pilot.py`` 的三次运行输出（每个 BIT 一次、每次十五个
冻结用例）汇总成 **一个 JSON** 加一份中文 ``RESULTS.md``。本脚本只做离线读取与
描述性汇总，不接触硬件，不含镜像 / JTAG / 串口逻辑。

输入
====

    --original DIR   label = original-serial 的 runner 输出目录
    --serial   DIR   label = banked-serial   的 runner 输出目录
    --hit      DIR   label = banked-hit      的 runner 输出目录

每个目录由 runner 写成：``manifest.json`` / ``plan.json`` / ``summary.json`` /
``boot.json`` / ``flash.json`` / ``caps.json`` / ``reader-upload.json`` /
``STOPPED.txt``，以及 ``cases/<case-id>/`` 下的 ``case.json`` / ``metrics.json`` /
``record.json`` / ``exit.txt`` / ``helper.json`` / ``helper.exit.txt`` /
``helper-window.json``。本脚本只按**冻结计划里的 15 个 case id**逐个打开这些确切
路径，不做目录枚举、不做哈希、不从设备读回。

第二 16 字诊断 bank（可选 ``--diagnostics``）
=============================================

``--diagnostics``（默认关闭）要求 banked-serial / banked-hit 每个 case 的
``helper.json`` 带第二 16 字**只读诊断 bank**（``diagnostic`` 对象）。该 bank 与
冻结的 16 字 core ABI **并存**，绝不替换 core words 0..15，也不作为 HPM / miss /
PTW 计数。开关关闭时本脚本行为与旧版完全一致（仅新增只读字段）。

诊断校验（仅 ``--diagnostics``）：
  * ``diagnostic.words`` 必须是 16 个 uint64（非 bool、0..2**64-1）；
  * ``words[0]`` magic == 0x49444D414E444701，``words[1]`` version == 1；
  * ``words[2]`` mode == 该 label 的 mode；
  * ``words[3]`` frozen 低位 == 4，且 bit63 溢出位为 0；
  * ``words[4]`` seq、``words[5]`` cycles 与 core 窗口一致；
  * ``words[15]`` fast_accept == Σ(core fast words 6..10)（64 位不溢出）；
  * 若 ``diagnostic.core_fast_sum`` 存在，作为额外核对（必须等于同一和）。

诊断语义（只读，不反馈 DMA/AXI）：
  * 采样窗口与 core 一致，均为 (open, close]；reset / ARM 清零，close 冻结；
  * 跨窗口的部分 probe 合法：start/eval/leaf 计数之间**不假设完整生命周期关系**；
  * ``seen`` 是 16 位 sticky mask，位义见 ``DIAG_SEEN_BITS``；
  * fast 事件数不是命中率，也不是全部 miss 数；fast>0 只表示“本轮观测到真实
    accept”，不构成 novelty；fast 全 0 是**科学上未触发**，**不是技术失败**。

输出
====

    <out>/nonblock-pilot-analysis.json   唯一 JSON 汇总
    <out>/RESULTS.md                     中文报告

``--out`` 必须与三个输入目录互不重叠；原始文件一律不修改。

口径（与冻结计划一致，未做任何扩展）
====================================

* 窗口周期数取 ``record.json`` 的 ``p4m.raw_window[1]``，单位是 1 cycle。
* 真实 V 固定工作量 = ``transfers * 4096`` 字节（``--chunk 4096``）。
* 窗口内 A 完成数 = ``raw_window[3]``（``bench/idma_experiment.c:487`` 明确
  ``a_completed_jobs_window = w[3]``，并与 ``p4m.a_completed_jobs_window``
  交叉核对）；A 的窗口内工作量 = ``a_jobs * 4096`` 字节；绝不用整轮 A 计数代替。
* 50 MHz 只用于把周期换算成时间和吞吐（推导量，不是直接测量）。V 吞吐与
  **同窗口** A 吞吐都用同一 ``raw_window[1]`` 窗口换算；不用不同窗口的 A
  完成数直接评价 A 速度。
* 短组只做验收，不与长组混合；长组每条件列出每次值 + 中位/min/max，不做显著性。
* 失败 / 残缺数据逐项保留 raw 并分类，只作诊断；性能汇总与两段效应只有在同一
  比较条件的三个重复技术上全部合格时才发布，绝不把失败残缺数值与有效样本混合，
  也不剔除不利但合法的合格样本。
* 两段对照列已按较弱因果措辞重命名：
  `S/原版结构+缓冲变化` = banked-serial − original-serial（来源受限于旧 legacy3
  与 S/N 512 缓冲差异，非纯布局因果）；
  `N/S变化` = banked-hit − banked-serial（描述性，本轮 fast 全 0 时不构成绕过证据）。
  original 与 banked-hit 的直接差值不用于归因绕过。
* 发布前的技术资格会汇合**该 case 的全部质量 issue**（含 a_jobs / exit_txt /
  metrics / helper / 诊断 bank 交叉核对）以及该 label 的 plan / boot / flash /
  caps / summary 必要质量问题；任一存在即该 case（及受影响 label）技术上不合格，
  compute_effects 不发布。合法偏低 / 波动样本、以及 fast=0，不记为 quality
  failure，不剔除。
* 直接按 runner validate_record() 的源逐实际字段复核 record.schema / success /
  error / result.status / victim|aggressor.data_errors / quarantined，以及
  p4m.bank_magic / observer_status / raw_window 与 raw_observation 的一致性；
  不导入 runner、不写设备、不重算哈希。
* helper 窗口是 ``idma-nonblock-window-v1``，16 字，计数区间为 (open, close]；
  序号一律取 ``words[4]``；快速路径事件数不是命中率，也不是全部 miss 数。
* 旧 HPM / COST / E3 / miss / PTW 数据不是 N 机制证据，也绝不用作有效 metric。

退出码
======

    0  三个 label 的 15 个用例全部 complete，且没有任何质量问题
    1  仍出完整汇总，但存在未运行 / 失败 / 不完整 / 质量问题
    2  用法或环境错误（参数、目录重叠、无法创建 --out）
"""

from __future__ import annotations

import argparse
import json
import pathlib
import re
import statistics
import sys
import time
from typing import Any, Dict, List, Optional, Sequence, Tuple

# ---------------------------------------------------------------------------
# 常量
# ---------------------------------------------------------------------------

ANALYSIS_SCHEMA = 'nonblock-pilot-analysis-v1'
RESULT_JSON_NAME = 'nonblock-pilot-analysis.json'
RESULT_MD_NAME = 'RESULTS.md'

LABELS: Tuple[str, ...] = ('original-serial', 'banked-serial', 'banked-hit')

CLOCK_HZ = 50_000_000          # 50 MHz；只用于把 cycle 换算成时间/吞吐
CHUNK_BYTES = 4096             # --chunk 4096，真实 V 的工作量单位
UINT64_MAX = (1 << 64) - 1

# 冻结计划（与 scripts/mac_nonblock_pilot.py 的 build_case_plan() 一致）
SHORT_TRANSFERS = 64
SHORT_WARMUP = 16
LONG_TRANSFERS = 4096
LONG_WARMUP = 256
LONG_ROUND_ORDERS: Tuple[Tuple[str, ...], ...] = (
    ('Solo', 'A1', 'A8', 'A16'),
    ('A16', 'A8', 'A1', 'Solo'),
    ('Solo', 'A1', 'A8', 'A16'),
)
SHORT_CONDITIONS: Tuple[str, ...] = ('Solo', 'A1', 'A8')
LONG_CONDITIONS: Tuple[str, ...] = ('Solo', 'A1', 'A8', 'A16')

# runner 侧 schema / magic（只用于核对，不改写任何东西）
MANIFEST_SCHEMA = 'nonblock-pilot-manifest-v1'
PLAN_SCHEMA = 'nonblock-pilot-plan-v1'
SUMMARY_SCHEMA = 'nonblock-pilot-summary-v1'
CASE_SCHEMA = 'nonblock-pilot-case-v1'
FLASH_SCHEMA = 'nonblock-pilot-flash-v1'
HELPER_WINDOW_JSON_SCHEMA = 'nonblock-pilot-helper-window-v1'
DIAG_WINDOW_JSON_SCHEMA = 'nonblock-pilot-diag-window-v1'

# --- record.p4m.raw_window 字段定位 ---------------------------------------
#
# scripts/mac_nonblock_pilot.py 的 validate_record() 只钉死了下面四个下标：
#   [0]  == P4M_BANK_MAGIC
#   [1]  == raw_observation[0]（窗口周期数）
#   [13] == case transfers（V issued）
#   [14] == case transfers（V done）
# 窗口内 A 完成数由 bench/idma_experiment.c:487 确认：
#   print_u64("a_completed_jobs_window", (uint64_t)w[3])，即 raw_window[3]；
#   与 record.p4m.a_completed_jobs_window 一致。
# 因此本脚本：
#   * 一律把 raw_window 16 字原样存进 JSON 并列进 RESULTS.md，作为可人工定位的证据；
#   * A 的窗口内完成数取 p4m.raw_window[RAW_WINDOW_A_JOBS_INDEX]（= 3），
#     并以 p4m["a_completed_jobs_window"] 作交叉核对，绝不使用整轮 A 计数代替。
P4M_BANK_MAGIC = 0x49444D4150344D01
P4M_WINDOW_WORDS = 16
RAW_WINDOW_MAGIC_INDEX = 0
RAW_WINDOW_CYCLES_INDEX = 1
RAW_WINDOW_V_ISSUED_INDEX = 13
RAW_WINDOW_V_DONE_INDEX = 14
RAW_WINDOW_A_JOBS_INDEX: Optional[int] = 3   # == a_done_window == p4m.a_completed_jobs_window

# runner validate_record() 钉死、但本脚本原先未逐字段核对的关键值
RECORD_SCHEMA = 'idma.p4m.v1'
P4M_OBSERVER_STATUS_OK = 4
P4M_OBS_WORDS = 35
# 运行身份 / 能力块的必要值
CAPS_SCHEMA = 'idma.experiment.v1'
BOOT_TAINT_OK = '0'
BOOT_ID_RE = re.compile(
    r'^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-'
    r'[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$')

# --- helper 窗口（ABI idma-nonblock-window-v1，16 字）---------------------
HELPER_SCHEMA = 'idma-nonblock-window-v1'
HELPER_WORDS = 16
HELPER_MAGIC = 0x49444D414E424C01
HELPER_VERSION = 1
HELPER_MODE_INDEX = 2
HELPER_FLAGS_INDEX = 3
HELPER_FLAGS_FROZEN = 4
HELPER_SEQ_INDEX = 4
HELPER_CYCLES_INDEX = 5
HELPER_FAST_SLICE = (6, 7, 8, 9, 10)
HELPER_NORMAL_SLICE = (11, 12, 13)
HELPER_ACCEPT_INDEX = 14
HELPER_ERRORS_INDEX = 15
HELPER_ERRORS_OK = 0
HELPER_MODE_FOR_LABEL: Dict[str, int] = {'banked-serial': 1, 'banked-hit': 2}
# 计数窗口语义：(open, close]，即不包含 open 时刻那一次事件。

# --- 第二 16 字诊断 bank（index 16..31，与 core ABI 并存）-----------------
#
# 与 dma_nonblock_observer.sv 的 index 16..31 对应；本脚本只读 helper.json 里的
# diagnostic 对象，不做任何改写。core words 0..15 语义完全不变。
DIAG_MAGIC = 0x49444D414E444701
DIAG_VERSION = 1
DIAG_WORDS = 16
DIAG_MODE_INDEX = 2
DIAG_FLAGS_INDEX = 3
DIAG_FLAGS_FROZEN = 4
DIAG_OVERFLOW_BIT = 63
DIAG_SEQ_INDEX = 4
DIAG_CYCLES_INDEX = 5
DIAG_SEEN_INDEX = 6
DIAG_FAST_ACCEPT_INDEX = 15

# seen 位义（与 rv_iommu_nonblock_hit.sv 的 diag_o 及 tl_wrap 的 bit0 一致）
DIAG_SEEN_BITS: Tuple[Tuple[int, str, str], ...] = (
    (0,  'base_s1_ptw_active',
         '宽口径 base S1 PTW 活动；可能含 implicit/debug，不等于 fast 候选'),
    (1,  'full_fast_window', '完整 fast 窗口谓词'),
    (2,  'candidate_in_window', '窗口内候选有效'),
    (3,  'probe_start', 'probe start'),
    (4,  'dc_eval', 'DC 评估'),
    (5,  'dc_good', 'DC 通过稳定阶段'),
    (6,  'leaf_eval', 'LEAF 评估'),
    (7,  'leaf_good', 'LEAF 合格'),
    (8,  'dc_msi_reject', 'DC MSI 拒绝（DDTC 命中）'),
    (9,  'leaf_msi_reject', 'LEAF MSI 拒绝（IOTLB 命中）'),
    (10, 'leaf_lookup_miss', 'LEAF 查找未命中'),
    (11, 'fast_accept', 'fast accept'),
    (12, 'dc_lookup_miss', 'DC 查找未命中'),
    (13, 'dc_other_reject', 'DC 其他拒绝'),
    (14, 'leaf_other_reject', 'LEAF 其他拒绝'),
    (15, 'runtime_enable', '运行时 enable'),
)

# 第二 bank words 7..15 → 阶段计数（word 6 是 seen）；word 15 == fast_accepts
DIAG_COUNTER_FIELDS: Tuple[Tuple[int, str], ...] = (
    (7,  'probe_starts'),
    (8,  'dc_evaluations'),
    (9,  'dc_good'),
    (10, 'leaf_evaluations'),
    (11, 'leaf_good'),
    (12, 'dc_msi_rejects'),
    (13, 'leaf_msi_rejects'),
    (14, 'leaf_lookup_misses'),
    (15, 'fast_accepts'),
)

_INT_TEXT_RE = re.compile(r'-?[0-9]+')


def utc_now() -> str:
    return time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime())


# ---------------------------------------------------------------------------
# 小工具：只读精确路径，失败只返回错误字符串，不抛异常
# ---------------------------------------------------------------------------

def load_text_file(path: pathlib.Path) -> Tuple[Optional[str], Optional[str]]:
    try:
        if not path.is_file():
            return None, 'missing: ' + path.as_posix()
    except OSError as exc:
        return None, 'stat failed: ' + path.as_posix() + ': ' + repr(exc)
    try:
        return path.read_text(encoding='utf-8', errors='replace'), None
    except OSError as exc:
        return None, 'read failed: ' + path.as_posix() + ': ' + repr(exc)


def load_json_file(path: pathlib.Path) -> Tuple[Any, Optional[str]]:
    text, err = load_text_file(path)
    if err is not None:
        return None, err
    try:
        return json.loads(text or ''), None
    except ValueError as exc:
        return None, 'invalid JSON: ' + path.as_posix() + ': ' + str(exc)


def parse_int_text(value: Optional[str]) -> Optional[int]:
    if value is None:
        return None
    text = value.strip()
    if not _INT_TEXT_RE.fullmatch(text):
        return None
    try:
        return int(text)
    except ValueError:
        return None


def int_or_none(value: Any) -> Optional[int]:
    if isinstance(value, bool) or not isinstance(value, int):
        return None
    return value


def all_ints(values: Any) -> bool:
    return isinstance(values, list) and all(
        isinstance(item, int) and not isinstance(item, bool) for item in values)


def all_uint64(values: Any) -> bool:
    return (isinstance(values, list)
            and all(isinstance(item, int) and not isinstance(item, bool)
                    and 0 <= item <= UINT64_MAX for item in values))


def sum_uint64(values: Sequence[int]) -> Tuple[int, bool]:
    """返回 (和, 是否溢出 64 位)。只用于诊断核对。"""
    total = 0
    for value in values:
        total += value
        if total > UINT64_MAX:
            return total, True
    return total, False


def same_or_within(candidate: pathlib.Path, parent: pathlib.Path) -> bool:
    try:
        candidate.resolve().relative_to(parent.resolve())
        return True
    except ValueError:
        return False
    except OSError:
        return str(candidate) == str(parent)


def delta(a: Optional[float], b: Optional[float]) -> Optional[float]:
    if a is None or b is None:
        return None
    return b - a


def delta_pct(a: Optional[float], b: Optional[float]) -> Optional[float]:
    if a is None or b is None or a == 0:
        return None
    return 100.0 * (b - a) / a


def median_or_none(values: List[float]) -> Optional[float]:
    if not values:
        return None
    return statistics.median(values)


def window_seconds(cycles: Optional[int]) -> Optional[float]:
    if cycles is None or cycles <= 0:
        return None
    return cycles / float(CLOCK_HZ)


def throughput_mib_s(byte_count: Optional[int],
                     cycles: Optional[int]) -> Optional[float]:
    """推导吞吐：bytes / (cycles / 50 MHz)，单位 MiB/s；不是直接测量。"""
    seconds = window_seconds(cycles)
    if byte_count is None or seconds is None:
        return None
    return (byte_count / (1024.0 * 1024.0)) / seconds


# ---------------------------------------------------------------------------
# 冻结计划
# ---------------------------------------------------------------------------

def build_frozen_plan() -> List[Dict[str, Any]]:
    """返回 15 个冻结用例（顺序 = 执行顺序），与 runner 的 build_case_plan() 一致。"""
    plan: List[Dict[str, Any]] = []
    order = 0
    for condition in SHORT_CONDITIONS:
        order += 1
        plan.append({
            'order': order,
            'id': 'short-' + condition.lower(),
            'group': 'short',
            'condition': condition,
            'round': None,
            'transfers': SHORT_TRANSFERS,
            'warmup': SHORT_WARMUP,
        })
    for round_number, order_tuple in enumerate(LONG_ROUND_ORDERS, start=1):
        for condition in order_tuple:
            order += 1
            plan.append({
                'order': order,
                'id': 'long-r%d-%s' % (round_number, condition.lower()),
                'group': 'long',
                'condition': condition,
                'round': round_number,
                'transfers': LONG_TRANSFERS,
                'warmup': LONG_WARMUP,
            })
    return plan


# ---------------------------------------------------------------------------
# 问题收集器
# ---------------------------------------------------------------------------

class Analysis:
    def __init__(self) -> None:
        self.quality: List[Dict[str, Any]] = []
        self.observations: List[Dict[str, Any]] = []

    def q(self, label: Optional[str], case: Optional[str], code: str, detail: str) -> None:
        self.quality.append({
            'scope': label, 'case': case, 'code': code, 'detail': detail})

    def o(self, label: Optional[str], case: Optional[str], code: str, detail: str) -> None:
        self.observations.append({
            'scope': label, 'case': case, 'code': code, 'detail': detail})


# ---------------------------------------------------------------------------
# helper 窗口校验（按冻结 ABI 规格）
# ---------------------------------------------------------------------------

def validate_helper_words(label: str, words: Any,
                          expected_cycles: Optional[int],
                          expect_v_movement: bool = True,
                          expect_a: bool = False) -> List[str]:
    problems: List[str] = []
    if not all_ints(words) or len(words) != HELPER_WORDS:
        if isinstance(words, list):
            shape = 'len=%d' % len(words)
        else:
            shape = repr(words)[:80]
        return ['helper words 不是 %d 个整数: %s' % (HELPER_WORDS, shape)]

    if words[0] != HELPER_MAGIC:
        problems.append('helper words[0] magic %r != %#x' % (words[0], HELPER_MAGIC))
    if words[1] != HELPER_VERSION:
        problems.append('helper words[1] version %r != %d' % (words[1], HELPER_VERSION))

    expected_mode = HELPER_MODE_FOR_LABEL.get(label)
    if words[HELPER_MODE_INDEX] != expected_mode:
        problems.append('helper words[2] mode %r != %r (label=%s)'
                        % (words[HELPER_MODE_INDEX], expected_mode, label))

    if words[HELPER_FLAGS_INDEX] != HELPER_FLAGS_FROZEN:
        problems.append('helper words[3] flags %r != %d (冻结位)'
                        % (words[HELPER_FLAGS_INDEX], HELPER_FLAGS_FROZEN))
    if words[HELPER_ERRORS_INDEX] != HELPER_ERRORS_OK:
        problems.append('helper words[15] errors %r != %d'
                        % (words[HELPER_ERRORS_INDEX], HELPER_ERRORS_OK))

    if expected_cycles is not None and words[HELPER_CYCLES_INDEX] != expected_cycles:
        problems.append('helper words[5] cycles %r != p4m.raw_window[1] %r'
                        % (words[HELPER_CYCLES_INDEX], expected_cycles))

    fast = sum(words[i] for i in HELPER_FAST_SLICE)
    normal = sum(words[i] for i in HELPER_NORMAL_SLICE)
    if words[HELPER_ACCEPT_INDEX] != fast + normal:
        problems.append('helper 计数守恒失败: words[14]=%r != Σfast %r + Σnormal %r'
                        % (words[HELPER_ACCEPT_INDEX], fast, normal))

    # --- 机制活跃性 guard（banked label；只对已有 words 追加问题，不改写） ----
    # V: fast 6=Vread, 7=Vwrite; normal 11=V
    # A: fast 8=Aread, 9=Awrite; normal 12=A
    if expect_v_movement or expect_a:
        if words[HELPER_ACCEPT_INDEX] <= 0:
            problems.append('helper words[14] accepted 不是 > 0，但该用例跑了 '
                            'V/A 传输（全 0 机制窗口不得判为通过）')
    if expect_v_movement:
        v_counts = (words[6], words[7], words[11])
        if sum(v_counts) <= 0:
            problems.append('helper V fast+normal 计数 %r 之和不是 > 0，但该用例'
                            '跑了 V 传输' % (v_counts,))
    if expect_a:
        a_counts = (words[8], words[9], words[12])
        if sum(a_counts) <= 0:
            problems.append('helper A fast+normal 计数 %r 之和不是 > 0，但该用例'
                            '跑了 A aggressor' % (a_counts,))

    if expected_mode == 1:
        fast_words = [words[i] for i in HELPER_FAST_SLICE]
        if any(value != 0 for value in fast_words):
            problems.append('Mode1 (banked-serial) 快速计数必须全 0，实际 %r' % (fast_words,))
    return problems


# ---------------------------------------------------------------------------
# 第二 16 字诊断 bank 校验（仅 --diagnostics）
# ---------------------------------------------------------------------------

def validate_helper_diagnostic(label: str, diagnostic: Any,
                               core_window: Any,
                               core_fast_sum: Any = None) -> List[str]:
    """校验可选第二诊断 bank（``helper.json`` 的 ``diagnostic`` 对象）。

    只读传入对象；不导入 runner、不改写任何字段。fast 计数为 0 不是问题。
    """
    problems: List[str] = []
    if not isinstance(diagnostic, dict):
        return ['helper diagnostic 不是对象']

    words = diagnostic.get('words')
    if not isinstance(words, list) or len(words) != DIAG_WORDS:
        if isinstance(words, list):
            shape = 'len=%d' % len(words)
        else:
            shape = repr(words)[:80]
        return ['helper diagnostic.words 不是 %d 个整数: %s' % (DIAG_WORDS, shape)]
    if not all(isinstance(v, int) and not isinstance(v, bool) for v in words):
        return ['helper diagnostic.words 含非整数或 bool（要求 uint64）']
    if any(v < 0 or v > UINT64_MAX for v in words):
        return ['helper diagnostic.words 超出 uint64 范围']

    if words[0] != DIAG_MAGIC:
        problems.append('diagnostic words[0] magic %r != %#x' % (words[0], DIAG_MAGIC))
    if words[1] != DIAG_VERSION:
        problems.append('diagnostic words[1] version %r != %d'
                        % (words[1], DIAG_VERSION))

    expected_mode = HELPER_MODE_FOR_LABEL.get(label)
    if words[DIAG_MODE_INDEX] != expected_mode:
        problems.append('diagnostic words[2] mode %r != %r (label=%s)'
                        % (words[DIAG_MODE_INDEX], expected_mode, label))

    flags = words[DIAG_FLAGS_INDEX]
    overflow = (flags >> DIAG_OVERFLOW_BIT) & 1
    low = flags & ((1 << DIAG_OVERFLOW_BIT) - 1)
    if overflow != 0:
        problems.append('diagnostic words[3] bit63 overflow=%d != 0' % overflow)
    if low != DIAG_FLAGS_FROZEN:
        problems.append('diagnostic words[3] frozen 低位 %r != %d'
                        % (low, DIAG_FLAGS_FROZEN))

    if core_window is None:
        problems.append('诊断校验缺少 core 窗口，无法核对 seq/cycles/fast')
        return problems
    if not (isinstance(core_window, list) and len(core_window) == HELPER_WORDS
            and all_ints(core_window)):
        problems.append('core 窗口不是 %d 个整数，无法核对诊断' % HELPER_WORDS)
        return problems

    if words[DIAG_SEQ_INDEX] != core_window[HELPER_SEQ_INDEX]:
        problems.append('diagnostic words[4] seq %r != core words[4] %r'
                        % (words[DIAG_SEQ_INDEX], core_window[HELPER_SEQ_INDEX]))
    if words[DIAG_CYCLES_INDEX] != core_window[HELPER_CYCLES_INDEX]:
        problems.append('diagnostic words[5] cycles %r != core words[5] %r'
                        % (words[DIAG_CYCLES_INDEX], core_window[HELPER_CYCLES_INDEX]))

    total, ovf = sum_uint64([core_window[i] for i in HELPER_FAST_SLICE])
    if ovf:
        problems.append('core fast Σ 6..10 溢出 64 位')
    elif words[DIAG_FAST_ACCEPT_INDEX] != total:
        problems.append('diagnostic words[15] fast_accept %r != Σ(core 6..10) %r'
                        % (words[DIAG_FAST_ACCEPT_INDEX], total))

    if core_fast_sum is not None:
        if isinstance(core_fast_sum, bool) or not isinstance(core_fast_sum, int):
            problems.append('diagnostic.core_fast_sum 不是整数: %r' % (core_fast_sum,))
        else:
            if core_fast_sum != total:
                problems.append('diagnostic.core_fast_sum %r != Σ(core 6..10) %r'
                                % (core_fast_sum, total))
            elif core_fast_sum != words[DIAG_FAST_ACCEPT_INDEX]:
                problems.append('diagnostic.core_fast_sum %r != diagnostic words[15] %r'
                                % (core_fast_sum, words[DIAG_FAST_ACCEPT_INDEX]))
    return problems


def fast_summary_from_core(words: Any) -> Optional[Dict[str, Any]]:
    """从 core 16 字快速切片导出 V/A/other 计数。fast 全 0 是合法观察。"""
    if not all_ints(words) or len(words) != HELPER_WORDS:
        return None
    v_total = words[6] + words[7]
    a_total = words[8] + words[9]
    other = words[10]
    total = v_total + a_total + other
    return {
        'v_read': words[6],
        'v_write': words[7],
        'a_read': words[8],
        'a_write': words[9],
        'other': other,
        'v_total': v_total,
        'a_total': a_total,
        'total': total,
        'all_zero': total == 0,
    }


def diagnostic_summary(words: Any) -> Optional[Dict[str, Any]]:
    """从诊断 16 字导出 seen 位与阶段计数（不含 words 本身）。"""
    if not all_uint64(words) or len(words) != DIAG_WORDS:
        return None
    seen = words[DIAG_SEEN_INDEX]
    return {
        'magic': words[0],
        'version': words[1],
        'mode': words[2],
        'flags': words[3],
        'overflow': (words[3] >> DIAG_OVERFLOW_BIT) & 1,
        'seq': words[4],
        'cycles': words[5],
        'seen': seen,
        'seen_bits': [name for bit, name, _ in DIAG_SEEN_BITS if (seen >> bit) & 1],
        'counts': {name: words[idx] for idx, name in DIAG_COUNTER_FIELDS},
        'fast_accept': words[DIAG_FAST_ACCEPT_INDEX],
    }


# ---------------------------------------------------------------------------
# validate_record（源级复刻）
# ---------------------------------------------------------------------------

def validate_record_fields(spec: Dict[str, Any],
                           record: Dict[str, Any]) -> List[str]:
    """按 runner validate_record() 源逐字段复核必要的 record 值。

    只读传入对象；不导入 runner（避免 Mac/serial 依赖与副作用），不改写任何字段。
    合法偏低/波动的数值不在此判定。
    """
    problems: List[str] = []
    if record.get('schema') != RECORD_SCHEMA:
        problems.append('record.schema %r != %s'
                        % (record.get('schema'), RECORD_SCHEMA))
    if record.get('success') is not True:
        problems.append('record.success %r 不是 true' % (record.get('success'),))
    error = record.get('error')
    if isinstance(error, str) and error.strip():
        problems.append('record.error %r' % (error,))

    result = record.get('result')
    if not isinstance(result, dict):
        problems.append('record.result 不是对象')
    else:
        if result.get('status') != 0:
            problems.append('record.result.status %r != 0'
                            % (result.get('status'),))
        for side in ('victim', 'aggressor'):
            stats = result.get(side)
            if side == 'aggressor' and spec['condition'] == 'Solo':
                if stats is not None:
                    problems.append('Solo 用例的 result.aggressor 不是 null')
                continue
            if not isinstance(stats, dict):
                problems.append('result.%s 不是对象' % side)
                continue
            if stats.get('data_errors') != 0:
                problems.append('result.%s.data_errors %r != 0'
                                % (side, stats.get('data_errors')))
            if stats.get('quarantined') != 0:
                problems.append('result.%s.quarantined %r != 0'
                                % (side, stats.get('quarantined')))
    return problems


# ---------------------------------------------------------------------------
# 单个 case
# ---------------------------------------------------------------------------

def analyze_case(an: Analysis, label: str, spec: Dict[str, Any],
                 case_dir: pathlib.Path,
                 diagnostics: bool = False) -> Dict[str, Any]:
    entry: Dict[str, Any] = {
        'id': spec['id'],
        'order': spec['order'],
        'group': spec['group'],
        'condition': spec['condition'],
        'round': spec['round'],
        'transfers': spec['transfers'],
        'warmup': spec['warmup'],
        'expected_v_bytes': spec['transfers'] * CHUNK_BYTES,
        'dir': case_dir.as_posix(),
        'state': 'not_run',
        'state_reasons': [],
        'case_json_present': False,
        'exit': None,
        'framed_exit': None,
        'taint_before': None,
        'taint_after': None,
        'runner_problems': [],
        'metrics_json': None,
        'v_window_cycles': None,
        'v_window_ns': None,
        'v_throughput_mib_s': None,
        'a_jobs_in_window': None,
        'a_jobs_field': None,
        'a_window_bytes': None,
        'a_throughput_mib_s': None,
        'raw_window': None,
        'raw_window_ok': None,
        'raw_observation_len': None,
        'raw_observation': None,
        'p4m_has_a': None,
        'helper': None,
        'helper_ok': None,
        'fast': None,
        'diagnostic': None,
        'qualified': False,
        'qualified_reasons': [],
    }

    case_id = spec['id']
    if not case_dir.is_dir():
        an.q(label, case_id, 'case_not_run', 'case 目录不存在: ' + case_dir.as_posix())
        return entry

    case_doc, err = load_json_file(case_dir / 'case.json')
    if err is not None:
        entry['state'] = 'incomplete'
        entry['state_reasons'].append(err)
        an.q(label, case_id, 'case_json_unreadable', err)
        return entry
    if not isinstance(case_doc, dict):
        entry['state'] = 'incomplete'
        entry['state_reasons'].append('case.json 不是 JSON 对象')
        an.q(label, case_id, 'case_json_not_object', 'case.json 不是 JSON 对象')
        return entry

    entry['case_json_present'] = True
    entry['exit'] = int_or_none(case_doc.get('exit'))
    entry['framed_exit'] = int_or_none(case_doc.get('framed_exit'))
    entry['taint_before'] = case_doc.get('taint_before')
    entry['taint_after'] = case_doc.get('taint_after')
    raw_problems = case_doc.get('problems')
    if isinstance(raw_problems, list):
        entry['runner_problems'] = [str(item) for item in raw_problems]
    else:
        an.q(label, case_id, 'case_problems_shape',
             'case.json problems 不是列表: ' + repr(raw_problems)[:120])

    state = 'complete'
    if entry['exit'] is None:
        an.q(label, case_id, 'case_exit_missing', 'case.json exit 缺失或不是整数')
        state = 'failed'
    elif entry['exit'] != 0:
        an.q(label, case_id, 'case_exit_nonzero', 'case.json exit=%d' % entry['exit'])
        state = 'failed'
    if entry['taint_after'] != '0':
        an.q(label, case_id, 'taint_after',
             'case.json taint_after=%r' % (entry['taint_after'],))
        state = 'failed'
    if entry['runner_problems']:
        an.q(label, case_id, 'runner_problems', '; '.join(entry['runner_problems']))
        state = 'failed'
    entry['state'] = state

    # --- exit.txt 交叉核对 -------------------------------------------------
    exit_text, err = load_text_file(case_dir / 'exit.txt')
    if err is not None:
        an.q(label, case_id, 'exit_txt_missing', err)
    else:
        parsed = parse_int_text(exit_text)
        if parsed is None:
            an.q(label, case_id, 'exit_txt_unparsable',
                 'exit.txt=%r' % ((exit_text or '').strip()[:40],))
        elif entry['exit'] is not None and parsed != entry['exit']:
            an.q(label, case_id, 'exit_txt_mismatch',
                 'exit.txt=%d != case.json exit=%d' % (parsed, entry['exit']))

    # --- metrics.json ------------------------------------------------------
    metrics_doc, err = load_json_file(case_dir / 'metrics.json')
    if err is not None:
        metrics_doc = None
        if state == 'complete':
            an.q(label, case_id, 'metrics_missing', err)
    elif not isinstance(metrics_doc, dict):
        metrics_doc = None
        an.q(label, case_id, 'metrics_not_object', 'metrics.json 不是 JSON 对象')
    entry['metrics_json'] = metrics_doc

    # --- record.json / p4m -------------------------------------------------
    p4m: Optional[Dict[str, Any]] = None
    record_doc, err = load_json_file(case_dir / 'record.json')
    if err is not None:
        an.q(label, case_id, 'record_unreadable', err)
    elif not isinstance(record_doc, dict):
        an.q(label, case_id, 'record_not_object', 'record.json 不是 JSON 对象')
    else:
        for message in validate_record_fields(spec, record_doc):
            an.q(label, case_id, 'record_field', message)
        candidate = record_doc.get('p4m')
        if isinstance(candidate, dict):
            p4m = candidate
        else:
            an.q(label, case_id, 'record_p4m_missing', 'record.p4m 不是对象')

    if p4m is not None:
        entry['p4m_has_a'] = p4m.get('has_a')

        if p4m.get('bank_magic') != P4M_BANK_MAGIC:
            an.q(label, case_id, 'p4m_bank_magic',
                 'p4m.bank_magic=%r != %#x'
                 % (p4m.get('bank_magic'), P4M_BANK_MAGIC))
        if p4m.get('observer_status') != P4M_OBSERVER_STATUS_OK:
            an.q(label, case_id, 'p4m_observer_status',
                 'p4m.observer_status=%r != %d'
                 % (p4m.get('observer_status'), P4M_OBSERVER_STATUS_OK))

        window = p4m.get('raw_window')
        if all_ints(window) and len(window) == P4M_WINDOW_WORDS:
            entry['raw_window'] = list(window)
        else:
            if isinstance(window, list):
                shape = 'len=%d' % len(window)
            else:
                shape = repr(window)[:80]
            an.q(label, case_id, 'raw_window_shape',
                 'record.p4m.raw_window 不是 %d 个整数: %s' % (P4M_WINDOW_WORDS, shape))

        observation = p4m.get('raw_observation')
        if isinstance(observation, list):
            entry['raw_observation_len'] = len(observation)
            if all_ints(observation):
                entry['raw_observation'] = list(observation)
            if len(observation) != P4M_OBS_WORDS:
                an.q(label, case_id, 'raw_observation_shape',
                     'raw_observation 不是 %d 字: len=%d'
                     % (P4M_OBS_WORDS, len(observation)))

        if (entry['raw_window'] is not None
                and entry['raw_observation'] is not None
                and len(entry['raw_observation']) > 0
                and entry['raw_window'][RAW_WINDOW_CYCLES_INDEX]
                != entry['raw_observation'][0]):
            an.q(label, case_id, 'raw_window_observer_mismatch',
                 'raw_window[%d]=%r != raw_observation[0]=%r'
                 % (RAW_WINDOW_CYCLES_INDEX,
                    entry['raw_window'][RAW_WINDOW_CYCLES_INDEX],
                    entry['raw_observation'][0]))

        cycles = None
        if entry['raw_window'] is not None:
            cycles = entry['raw_window'][RAW_WINDOW_CYCLES_INDEX]
        else:
            cycles = int_or_none(p4m.get('cycles'))
        if cycles is None or cycles <= 0:
            an.q(label, case_id, 'v_cycles_invalid', 'cycles=%r' % (cycles,))
            cycles = None
        entry['v_window_cycles'] = cycles

        p4m_cycles = int_or_none(p4m.get('cycles'))
        if cycles is not None and p4m_cycles is not None and p4m_cycles != cycles:
            an.q(label, case_id, 'p4m_cycles_mismatch',
                 'p4m.cycles %r != raw_window[1] %r' % (p4m_cycles, cycles))

        if p4m.get('closed') is not True:
            an.q(label, case_id, 'p4m_not_closed', 'record.p4m.closed=%r' % (p4m.get('closed'),))

        raw_window_ok = entry['raw_window'] is not None
        if entry['raw_window'] is not None:
            w = entry['raw_window']
            if w[RAW_WINDOW_MAGIC_INDEX] != P4M_BANK_MAGIC:
                an.q(label, case_id, 'raw_window_magic',
                     'raw_window[0]=%r != %#x' % (w[0], P4M_BANK_MAGIC))
                raw_window_ok = False
            for index, name in ((RAW_WINDOW_V_ISSUED_INDEX, 'V issued'),
                                (RAW_WINDOW_V_DONE_INDEX, 'V done')):
                if w[index] != spec['transfers']:
                    an.q(label, case_id, 'raw_window_v_' + name.replace(' ', '_'),
                         'raw_window[%d]=%r != transfers %d'
                         % (index, w[index], spec['transfers']))
                    raw_window_ok = False
        if entry['v_window_cycles'] is None:
            raw_window_ok = False
        entry['raw_window_ok'] = raw_window_ok

        if cycles is not None:
            entry['v_window_ns'] = cycles / float(CLOCK_HZ) * 1e9
            entry['v_throughput_mib_s'] = throughput_mib_s(
                spec['transfers'] * CHUNK_BYTES, cycles)

        # --- A 的窗口内完成数 / 同窗口 A 吞吐 -----------------------------
        # bench/idma_experiment.c:487 -> a_completed_jobs_window == raw_window[3]。
        a_value = int_or_none(p4m.get('a_completed_jobs_window'))
        a_field = 'record.p4m.a_completed_jobs_window' if a_value is not None else None
        if RAW_WINDOW_A_JOBS_INDEX is not None and entry['raw_window'] is not None:
            index = RAW_WINDOW_A_JOBS_INDEX
            raw_value = entry['raw_window'][index]
            if a_value is not None and a_value != raw_value:
                an.q(label, case_id, 'a_jobs_mismatch',
                     'raw_window[%d]=%r != p4m.a_completed_jobs_window=%r'
                     % (index, raw_value, a_value))
            a_value = raw_value
            a_field = 'record.p4m.raw_window[%d]' % index
        entry['a_jobs_in_window'] = a_value
        entry['a_jobs_field'] = a_field
        if a_value is None:
            an.q(label, case_id, 'a_jobs_unavailable',
                 'record.json 里没有窗口内 A 完成数（raw_window[3] 与 '
                 'p4m.a_completed_jobs_window 均缺失），不退回整轮 A 计数')
        else:
            entry['a_window_bytes'] = a_value * CHUNK_BYTES
            entry['a_throughput_mib_s'] = throughput_mib_s(
                entry['a_window_bytes'], cycles)
            if spec['condition'] == 'Solo' and a_value != 0:
                an.o(label, case_id, 'solo_a_jobs_nonzero',
                     'Solo 用例的窗口内 A 完成数 = %d（窗口内 A 字节 = %d，仅作观察）'
                     % (a_value, entry['a_window_bytes']))

    # --- metrics.json 与 record 的交叉核对 ---------------------------------
    if metrics_doc is not None:
        for key, name in (('condition', 'condition'), ('round', 'round'),
                          ('transfers', 'transfers'), ('warmup', 'warmup')):
            if metrics_doc.get(key) != spec[key]:
                an.q(label, case_id, 'metrics_field_mismatch',
                     'metrics.json %s=%r != 计划 %r'
                     % (name, metrics_doc.get(key), spec[key]))
        m_cycles = int_or_none(metrics_doc.get('v_window_cycles'))
        if (entry['v_window_cycles'] is not None and m_cycles is not None
                and m_cycles != entry['v_window_cycles']):
            an.q(label, case_id, 'metrics_cycles_mismatch',
                 'metrics.v_window_cycles=%r != raw_window[1]=%r'
                 % (m_cycles, entry['v_window_cycles']))
        m_a = int_or_none(metrics_doc.get('a_completed_jobs_window'))
        if (entry['a_jobs_in_window'] is not None and m_a is not None
                and m_a != entry['a_jobs_in_window']):
            an.q(label, case_id, 'metrics_a_jobs_mismatch',
                 'metrics.a_completed_jobs_window=%r != record 派生值 %r'
                 % (m_a, entry['a_jobs_in_window']))

    # --- helper.json（只对 banked-serial / banked-hit）----------------------
    helper_expected = label in HELPER_MODE_FOR_LABEL
    helper_path = case_dir / 'helper.json'
    helper_entry: Dict[str, Any] = {
        'expected': helper_expected,
        'present': helper_path.is_file(),
        'schema': None,
        'mode': None,
        'seq': None,
        'cycles': None,
        'flags': None,
        'errors': None,
        'fastcounts': None,
        'normalcounts': None,
        'fast_total': None,
        'normal_total': None,
        'accepted_total': None,
        'words': None,
        'exit': None,
        'problems': [],
    }
    entry['helper'] = helper_entry

    diag_entry: Dict[str, Any] = {
        'expected': bool(diagnostics and helper_expected),
        'present': False,
        'words': None,
        'magic': None,
        'version': None,
        'mode': None,
        'flags': None,
        'overflow': None,
        'seq': None,
        'cycles': None,
        'seen': None,
        'seen_bits': None,
        'counts': None,
        'fast_accept': None,
        'core_fast_sum': None,
        'core_fast_sum_present': False,
        'problems': [],
        'ok': None,
    }
    entry['diagnostic'] = diag_entry

    helper_ran_expected = (
        helper_expected and entry['exit'] == 0 and entry['taint_after'] == '0')

    if helper_path.is_file():
        if not helper_expected:
            an.q(label, case_id, 'helper_unexpected',
                 'label %s 不应有 helper.json（该 BIT 无 RO 硬件）' % label)
        helper_doc, err = load_json_file(helper_path)
        if err is not None:
            helper_entry['problems'].append(err)
        elif not isinstance(helper_doc, dict):
            helper_entry['problems'].append('helper.json 不是 JSON 对象')
        else:
            abi = helper_doc.get('abi', helper_doc.get('schema'))
            helper_entry['schema'] = abi
            if abi != HELPER_SCHEMA:
                helper_entry['problems'].append(
                    'helper ABI/schema %r != %s' % (abi, HELPER_SCHEMA))

            words = helper_doc.get('words')
            if all_ints(words) and len(words) == HELPER_WORDS:
                helper_entry['words'] = list(words)
            else:
                if isinstance(words, list):
                    shape = 'len=%d' % len(words)
                else:
                    shape = repr(words)[:80]
                helper_entry['problems'].append(
                    'helper words 不是 %d 个整数: %s' % (HELPER_WORDS, shape))

            helper_entry['mode'] = int_or_none(helper_doc.get('mode'))
            helper_entry['cycles'] = int_or_none(helper_doc.get('cycles'))
            # 序号以 words[4] 为唯一权威来源；reader emit_json 也输出
            # windowseq == w[4]（bench/idma_nonblock_read.c:71），此处只在
            # 该字段真实存在时做交叉核对，不假设它一定存在。
            if helper_entry['words'] is not None:
                helper_entry['seq'] = helper_entry['words'][HELPER_SEQ_INDEX]
                emitted_seq = int_or_none(helper_doc.get('windowseq'))
                if emitted_seq is not None and emitted_seq != helper_entry['seq']:
                    helper_entry['problems'].append(
                        'helper.json windowseq %r != words[4] %r'
                        % (emitted_seq, helper_entry['seq']))
            fast = helper_doc.get('fastcounts')
            normal = helper_doc.get('normalcounts')
            helper_entry['fastcounts'] = list(fast) if all_ints(fast) else None
            helper_entry['normalcounts'] = list(normal) if all_ints(normal) else None

            if helper_entry['words'] is not None:
                w = helper_entry['words']
                helper_entry['flags'] = w[HELPER_FLAGS_INDEX]
                helper_entry['errors'] = w[HELPER_ERRORS_INDEX]
                helper_entry['fast_total'] = sum(w[i] for i in HELPER_FAST_SLICE)
                helper_entry['normal_total'] = sum(w[i] for i in HELPER_NORMAL_SLICE)
                helper_entry['accepted_total'] = w[HELPER_ACCEPT_INDEX]
                if helper_entry['fastcounts'] is not None and \
                        helper_entry['fastcounts'] != [w[i] for i in HELPER_FAST_SLICE]:
                    helper_entry['problems'].append(
                        'fastcounts 与 words[6..10] 不一致')
                if helper_entry['normalcounts'] is not None and \
                        helper_entry['normalcounts'] != [w[i] for i in HELPER_NORMAL_SLICE]:
                    helper_entry['problems'].append(
                        'normalcounts 与 words[11..13] 不一致')
                expect_v = int_or_none(spec.get('transfers')) not in (None, 0)
                expect_a = bool(entry.get('p4m_has_a'))
                helper_entry['problems'].extend(validate_helper_words(
                    label, w, entry['v_window_cycles'],
                    expect_v_movement=expect_v, expect_a=expect_a))
                helper_entry['mechanism_expect_v'] = expect_v
                helper_entry['mechanism_expect_a'] = expect_a
                # core fast V/A 计数（与诊断无关，v3 也适用）
                entry['fast'] = fast_summary_from_core(w)

            # --- 可选第二 16 字诊断 bank ---------------------------------
            diagnostic = helper_doc.get('diagnostic')
            diag_entry['present'] = isinstance(diagnostic, dict)
            if diagnostics and helper_expected:
                core_fast_sum = (diagnostic.get('core_fast_sum')
                                 if isinstance(diagnostic, dict) else None)
                diag_entry['core_fast_sum_present'] = isinstance(diagnostic, dict) \
                    and ('core_fast_sum' in diagnostic)
                diag_entry['problems'].extend(validate_helper_diagnostic(
                    label, diagnostic, helper_entry['words'], core_fast_sum))
                if isinstance(diagnostic, dict):
                    dwords = diagnostic.get('words')
                    if all_uint64(dwords) and len(dwords) == DIAG_WORDS:
                        diag_entry['words'] = list(dwords)
                        derived = diagnostic_summary(dwords)
                        if derived is not None:
                            diag_entry.update(derived)
                    diag_entry['core_fast_sum'] = int_or_none(core_fast_sum)

    helper_exit_text, err = load_text_file(case_dir / 'helper.exit.txt')
    if err is not None:
        if helper_ran_expected:
            an.q(label, case_id, 'helper_exit_missing', err)
    else:
        parsed = parse_int_text(helper_exit_text)
        if parsed is None:
            an.q(label, case_id, 'helper_exit_unparsable',
                 'helper.exit.txt=%r' % ((helper_exit_text or '').strip()[:40],))
        else:
            helper_entry['exit'] = parsed
            if parsed != 0:
                an.q(label, case_id, 'helper_exit_nonzero',
                     'helper exit=%d' % parsed)

    if helper_ran_expected and not helper_path.is_file():
        an.q(label, case_id, 'helper_missing',
             'CLI exit 0 且 taint 0，但 helper.json 缺失')

    for problem in helper_entry['problems']:
        an.q(label, case_id, 'helper_window', problem)
    for problem in diag_entry['problems']:
        an.q(label, case_id, 'helper_diagnostic', problem)

    helper_ok = None
    if helper_entry['words'] is not None:
        helper_ok = not helper_entry['problems']
    elif helper_expected:
        helper_ok = False
    entry['helper_ok'] = helper_ok

    diag_ok: Optional[bool] = None
    if diag_entry['words'] is not None:
        diag_ok = not diag_entry['problems']
    elif diagnostics and helper_expected:
        diag_ok = False
    diag_entry['ok'] = diag_ok

    # --- helper-window.json 交叉核对 ---------------------------------------
    if helper_ran_expected and helper_entry['words'] is not None:
        window_doc, err = load_json_file(case_dir / 'helper-window.json')
        if err is not None:
            an.q(label, case_id, 'helper_window_json_missing', err)
        elif not isinstance(window_doc, dict):
            an.q(label, case_id, 'helper_window_json_not_object',
                 'helper-window.json 不是 JSON 对象')
        else:
            stored = window_doc.get('window')
            if not all_ints(stored) or len(stored) != HELPER_WORDS:
                an.q(label, case_id, 'helper_window_json_shape',
                     'helper-window.json window 不是 %d 个整数' % HELPER_WORDS)
            elif list(stored) != helper_entry['words']:
                an.q(label, case_id, 'helper_window_json_mismatch',
                     'helper-window.json window != helper.json words')
            if window_doc.get('schema') != HELPER_WINDOW_JSON_SCHEMA:
                an.q(label, case_id, 'helper_window_json_schema',
                     'helper-window.json schema=%r' % (window_doc.get('schema'),))

    # --- diag-window.json 交叉核对（可选，仅 --diagnostics）-----------------
    if diagnostics and helper_expected and diag_entry['words'] is not None:
        diag_window_path = case_dir / 'diag-window.json'
        if diag_window_path.is_file():
            window_doc, err = load_json_file(diag_window_path)
            if err is not None:
                an.q(label, case_id, 'diagnostic_window_json_unreadable', err)
            elif not isinstance(window_doc, dict):
                an.q(label, case_id, 'diagnostic_window_json_not_object',
                     'diag-window.json 不是 JSON 对象')
            else:
                nested = window_doc.get('diagnostic')
                stored = nested.get('words') if isinstance(nested, dict) else None
                if not all_uint64(stored) or len(stored) != DIAG_WORDS:
                    an.q(label, case_id, 'diagnostic_window_json_shape',
                         'diag-window.json diagnostic.words 不是 %d 个 uint64' % DIAG_WORDS)
                elif list(stored) != diag_entry['words']:
                    an.q(label, case_id, 'diagnostic_window_json_mismatch',
                         'diag-window.json diagnostic.words != helper.json diagnostic.words')
                if window_doc.get('schema') != DIAG_WINDOW_JSON_SCHEMA:
                    an.q(label, case_id, 'diagnostic_window_json_schema',
                         'diag-window.json schema=%r' % (window_doc.get('schema'),))

    # --- 技术合格判定（决定该轮是否可进入性能汇总/效应）-------------------
    reasons: List[str] = []
    if entry['state'] != 'complete':
        reasons.append('state=%s' % entry['state'])
    if entry['raw_window_ok'] is not True:
        reasons.append('raw_window 未通过校验')
    if entry['v_window_cycles'] is None:
        reasons.append('V 窗口周期缺失')
    if entry['v_throughput_mib_s'] is None:
        reasons.append('V 吞吐不可算')
    if entry['a_jobs_in_window'] is None:
        reasons.append('窗口内 A 完成数缺失')
    if entry['a_throughput_mib_s'] is None:
        reasons.append('同窗口 A 吞吐不可算')
    if helper_expected and entry['helper_ok'] is not True:
        reasons.append('helper 窗口未通过校验')
    if diagnostics and helper_expected and diag_entry['ok'] is not True:
        reasons.append('诊断 bank 未通过校验')
    entry['qualified'] = not reasons
    entry['qualified_reasons'] = reasons

    return entry


# ---------------------------------------------------------------------------
# 单个 label
# ---------------------------------------------------------------------------

def finalize_label_qualification(an: Analysis, label: str,
                                 info: Dict[str, Any]) -> None:
    '''把该 label 的全部质量 issue 折进每个 case 的技术资格与 label 汇总。

    该 case 的任何质量 issue（含 a_jobs/exit_txt/metrics/helper/诊断交叉核对）或
    该 label 的 plan/boot/flash/caps/summary 等必要问题，都使受影响 case 技术上
    不合格；合法偏低/波动样本与 fast=0 不产生 issue，仍保留并参与汇总。
    '''
    label_level = [issue for issue in an.quality
                   if issue['scope'] == label and issue['case'] is None]
    for case_entry in info['cases']:
        merged = list(case_entry.get('qualified_reasons') or [])
        for issue in an.quality:
            if issue['scope'] != label:
                continue
            if issue['case'] not in (case_entry['id'], None):
                continue
            text = '%s: %s' % (issue['code'], issue['detail'])
            if text not in merged:
                merged.append(text)
        case_entry['qualified_reasons'] = merged
        case_entry['qualified'] = not merged
    info['label_quality_issues'] = [
        '%s: %s' % (issue['code'], issue['detail']) for issue in label_level]
    info['technical_qualified'] = (
        not label_level
        and all(case_entry['qualified'] for case_entry in info['cases']))



def summarize_label(an: Analysis, label: str, directory: pathlib.Path,
                    plan: List[Dict[str, Any]],
                    diagnostics: bool = False) -> Dict[str, Any]:
    info: Dict[str, Any] = {
        'label': label,
        'path': directory.as_posix(),
        'exists': directory.is_dir(),
        'manifest': None,
        'summary_status': None,
        'summary_unexpected_warnings': [],
        'stopped': None,
        'boot': None,
        'flash': None,
        'caps_present': None,
        'reader_upload': None,
        'plan_ids': None,
        'cases': [],
        'label_quality_issues': [],
        'technical_qualified': None,
    }

    if not info['exists']:
        an.q(label, None, 'input_dir_missing',
             'runner 输出目录不存在: ' + directory.as_posix())
        for spec in plan:
            info['cases'].append(analyze_case(an, label, spec,
                                              directory / 'cases' / spec['id'],
                                              diagnostics=diagnostics))
        finalize_label_qualification(an, label, info)
        return info

    # --- manifest.json ------------------------------------------------------
    manifest, err = load_json_file(directory / 'manifest.json')
    if err is not None:
        an.q(label, None, 'manifest_unreadable', err)
    elif not isinstance(manifest, dict):
        an.q(label, None, 'manifest_not_object', 'manifest.json 不是 JSON 对象')
    else:
        listed = manifest.get('cases')
        info['manifest'] = {
            'schema': manifest.get('schema'),
            'run_id': manifest.get('run_id'),
            'label': manifest.get('label'),
            'bit': manifest.get('bit'),
            'device': manifest.get('device'),
            'planned': manifest.get('planned'),
            'completed': manifest.get('completed'),
            'failed': manifest.get('failed'),
            'remaining': manifest.get('remaining'),
            'started_at': manifest.get('started_at'),
            'updated_at': manifest.get('updated_at'),
            'listed_cases': len(listed) if isinstance(listed, list) else None,
        }
        if manifest.get('schema') != MANIFEST_SCHEMA:
            an.q(label, None, 'manifest_schema',
                 'manifest.schema=%r != %s' % (manifest.get('schema'), MANIFEST_SCHEMA))
        if manifest.get('label') != label:
            an.q(label, None, 'manifest_label_mismatch',
                 'manifest.label=%r != %r' % (manifest.get('label'), label))
        if manifest.get('planned') != len(plan):
            an.q(label, None, 'manifest_planned',
                 'manifest.planned=%r != %d' % (manifest.get('planned'), len(plan)))

    # --- summary.json -------------------------------------------------------
    summary, err = load_json_file(directory / 'summary.json')
    if err is not None:
        an.q(label, None, 'summary_unreadable', err)
    elif not isinstance(summary, dict):
        an.q(label, None, 'summary_not_object', 'summary.json 不是 JSON 对象')
    else:
        info['summary_status'] = summary.get('status')
        if summary.get('schema') != SUMMARY_SCHEMA:
            an.q(label, None, 'summary_schema',
                 'summary.schema=%r != %s' % (summary.get('schema'), SUMMARY_SCHEMA))
        if summary.get('status') != 'complete':
            an.q(label, None, 'summary_status',
                 'summary.status=%r（运行没有干净结束）' % (summary.get('status'),))
        warnings = summary.get('unexpected_warnings')
        if isinstance(warnings, list) and warnings:
            info['summary_unexpected_warnings'] = [str(w) for w in warnings]
            an.q(label, None, 'summary_unexpected_warnings',
                 '; '.join(str(w) for w in warnings[:20]))

    # --- STOPPED.txt --------------------------------------------------------
    stopped_path = directory / 'STOPPED.txt'
    if stopped_path.is_file():
        stopped_text, _ = load_text_file(stopped_path)
        info['stopped'] = (stopped_text or '').strip()[:500]
        an.q(label, None, 'stopped_marker',
             'STOPPED.txt 存在: ' + ' | '.join(info['stopped'].splitlines()))

    # --- plan.json ----------------------------------------------------------
    plan_doc, err = load_json_file(directory / 'plan.json')
    if err is not None:
        an.q(label, None, 'plan_unreadable', err)
    elif not isinstance(plan_doc, dict):
        an.q(label, None, 'plan_not_object', 'plan.json 不是 JSON 对象')
    else:
        if plan_doc.get('schema') != PLAN_SCHEMA:
            an.q(label, None, 'plan_schema',
                 'plan.json schema=%r != %s' % (plan_doc.get('schema'), PLAN_SCHEMA))
        listed = plan_doc.get('cases')
        if isinstance(listed, list):
            ids = [item.get('id') for item in listed if isinstance(item, dict)]
            info['plan_ids'] = ids
            if ids != [spec['id'] for spec in plan]:
                an.q(label, None, 'plan_mismatch',
                     'plan.json 的 case id 顺序与冻结 15 例不一致')
        else:
            an.q(label, None, 'plan_cases_shape', 'plan.json cases 不是列表')

    # --- boot.json / flash.json / caps.json / reader-upload.json ------------
    boot, err = load_json_file(directory / 'boot.json')
    if err is not None:
        an.q(label, None, 'boot_unreadable', err)
    elif not isinstance(boot, dict):
        an.q(label, None, 'boot_not_object', 'boot.json 不是 JSON 对象')
    else:
        info['boot'] = {
            'uname_r': boot.get('uname_r'),
            'boot_id': boot.get('boot_id'),
            'taint': boot.get('taint'),
            'boot_id_before': boot.get('boot_id_before'),
        }
        uname_r = boot.get('uname_r')
        if not isinstance(uname_r, str) or not uname_r.strip():
            an.q(label, None, 'boot_uname_missing', 'boot.uname_r 缺失')
        boot_id = boot.get('boot_id')
        if not isinstance(boot_id, str) or not BOOT_ID_RE.fullmatch(boot_id.strip()):
            an.q(label, None, 'boot_id_invalid',
                 'boot.boot_id=%r 不是 UUID' % (boot_id,))
        if boot.get('taint') != BOOT_TAINT_OK:
            an.q(label, None, 'boot_taint',
                 'boot.taint=%r != %r' % (boot.get('taint'), BOOT_TAINT_OK))
        before = boot.get('boot_id_before')
        if (isinstance(before, str) and before.strip()
                and isinstance(boot_id, str)
                and before.strip() == boot_id.strip()):
            an.q(label, None, 'boot_id_unchanged',
                 'boot_id == boot_id_before=%r（flash 后 boot 未更换）' % (boot_id,))

    flash, err = load_json_file(directory / 'flash.json')
    if err is not None:
        an.q(label, None, 'flash_unreadable', err)
    elif not isinstance(flash, dict):
        an.q(label, None, 'flash_not_object', 'flash.json 不是 JSON 对象')
    else:
        info['flash'] = {
            'schema': flash.get('schema'),
            'bit': flash.get('bit'),
            'bit_sha256': flash.get('bit_sha256'),
            'returncode': flash.get('returncode'),
            'note': 'sha256 由 runner 记录；本脚本未重新计算任何哈希',
        }
        if flash.get('schema') != FLASH_SCHEMA:
            an.q(label, None, 'flash_schema',
                 'flash.schema=%r != %s' % (flash.get('schema'), FLASH_SCHEMA))
        if flash.get('returncode') != 0:
            an.q(label, None, 'flash_returncode',
                 'flash.returncode=%r != 0' % (flash.get('returncode'),))

    caps, err = load_json_file(directory / 'caps.json')
    if err is not None:
        info['caps_present'] = False
        an.q(label, None, 'caps_unreadable', err)
    elif not isinstance(caps, dict):
        info['caps_present'] = False
        an.q(label, None, 'caps_not_object', 'caps.json 不是 JSON 对象')
    else:
        info['caps_present'] = True
        parsed = caps.get('parsed')
        if not isinstance(parsed, dict):
            an.q(label, None, 'caps_parsed_missing', 'caps.json.parsed 不是对象')
        else:
            if parsed.get('schema') != CAPS_SCHEMA:
                an.q(label, None, 'caps_schema',
                     'caps.parsed.schema=%r != %s'
                     % (parsed.get('schema'), CAPS_SCHEMA))
            if parsed.get('success') is not True:
                an.q(label, None, 'caps_success',
                     'caps.parsed.success=%r 不是 true' % (parsed.get('success'),))

    reader, err = load_json_file(directory / 'reader-upload.json')
    if err is None and isinstance(reader, dict):
        info['reader_upload'] = {
            'schema': reader.get('schema'),
            'reader_mode': reader.get('reader_mode'),
            'elf_bytes': reader.get('elf_bytes'),
            'chunk_count': reader.get('chunk_count'),
            'hash_readback': reader.get('hash_readback'),
        }
    if label == 'original-serial' and isinstance(reader, dict):
        an.q(label, None, 'reader_unexpected',
             'original-serial 不应有 reader-upload.json')

    # --- 逐 case ------------------------------------------------------------
    for spec in plan:
        info['cases'].append(analyze_case(
            an, label, spec, directory / 'cases' / spec['id'],
            diagnostics=diagnostics))

    # --- helper 序号（words[4]）在计划顺序上必须严格递增 -------------------
    sequences: List[Tuple[int, str, int]] = []
    for case_entry in info['cases']:
        helper = case_entry.get('helper') or {}
        seq = helper.get('seq')
        if isinstance(seq, int) and not isinstance(seq, bool):
            sequences.append((case_entry['order'], case_entry['id'], seq))
    sequences.sort()
    previous: Optional[Tuple[int, str, int]] = None
    for current in sequences:
        if previous is not None and current[2] <= previous[2]:
            an.q(label, current[1], 'helper_sequence',
                 'helper words[4] 序号 %d 未超过前一有效窗口 %s 的 %d'
                 % (current[2], previous[1], previous[2]))
        previous = current

    finalize_label_qualification(an, label, info)

    return info


# ---------------------------------------------------------------------------
# 汇总计算
# ---------------------------------------------------------------------------

def compute_completeness(labels: Dict[str, Dict[str, Any]],
                         plan: List[Dict[str, Any]]) -> Dict[str, Any]:
    per_label: Dict[str, Any] = {}
    all_complete = True
    for label in LABELS:
        counts = {'complete': 0, 'failed': 0, 'incomplete': 0, 'not_run': 0}
        for case_entry in labels.get(label, {}).get('cases', []):
            counts[case_entry['state']] = counts.get(case_entry['state'], 0) + 1
        counts['total'] = sum(counts.values())
        per_label[label] = counts
        if counts['complete'] != len(plan):
            all_complete = False
    return {
        'expected_cases_per_label': len(plan),
        'expected_total_cases': len(plan) * len(LABELS),
        'per_label': per_label,
        'all_labels_present': all(labels.get(l, {}).get('exists') for l in LABELS),
        'all_cases_complete': all_complete,
    }


def compute_short(labels: Dict[str, Dict[str, Any]]) -> Dict[str, Any]:
    per_label: Dict[str, Any] = {}
    for label in LABELS:
        rows = []
        for case_entry in labels.get(label, {}).get('cases', []):
            if case_entry['group'] != 'short':
                continue
            rows.append({
                'id': case_entry['id'],
                'condition': case_entry['condition'],
                'state': case_entry['state'],
                'cycles': case_entry['v_window_cycles'],
                'ns': case_entry['v_window_ns'],
                'mib_s': case_entry['v_throughput_mib_s'],
                'a_jobs': case_entry['a_jobs_in_window'],
                'a_bytes': case_entry['a_window_bytes'],
                'a_mib_s': case_entry['a_throughput_mib_s'],
                'a_jobs_field': case_entry['a_jobs_field'],
                'raw_window_ok': case_entry['raw_window_ok'],
                'helper_ok': case_entry['helper_ok'],
                'qualified': case_entry['qualified'],
            })
        rows.sort(key=lambda item: item['id'])
        per_label[label] = rows
    return {
        'note': '短组只做验收（3 例），不与长组混合，不参与两段对照比较，不做显著性。'
                'A 吞吐为同一窗口内的 A 完成字节速率，不是总线吞吐。',
        'per_label': per_label,
    }


def _gated_series(rounds: List[Dict[str, Any]], key: str,
                  publishable: bool) -> Dict[str, Any]:
    """publishable 为假时只保留逐轮原值，不产出 median/min/max。"""
    values = [r[key] for r in rounds
              if isinstance(r[key], (int, float)) and not isinstance(r[key], bool)]
    numeric = [float(v) for v in values]
    if not publishable:
        return {'values': values, 'n': len(numeric),
                'median': None, 'min': None, 'max': None, 'published': False}
    return {
        'values': values,
        'n': len(numeric),
        'median': median_or_none(numeric),
        'min': min(numeric) if numeric else None,
        'max': max(numeric) if numeric else None,
        'published': True,
    }


def compute_long(labels: Dict[str, Dict[str, Any]]) -> Dict[str, Any]:
    per_condition: Dict[str, Any] = {}
    series_keys = ('cycles', 'v_mib_s', 'a_jobs', 'a_bytes', 'a_mib_s')
    for condition in LONG_CONDITIONS:
        by_label: Dict[str, Any] = {}
        for label in LABELS:
            rounds = []
            for case_entry in labels.get(label, {}).get('cases', []):
                if case_entry['group'] != 'long' or case_entry['condition'] != condition:
                    continue
                rounds.append({
                    'round': case_entry['round'],
                    'id': case_entry['id'],
                    'state': case_entry['state'],
                    'qualified': case_entry['qualified'],
                    'qualified_reasons': case_entry['qualified_reasons'],
                    'cycles': case_entry['v_window_cycles'],
                    'v_mib_s': case_entry['v_throughput_mib_s'],
                    'a_jobs': case_entry['a_jobs_in_window'],
                    'a_bytes': case_entry['a_window_bytes'],
                    'a_mib_s': case_entry['a_throughput_mib_s'],
                })
            rounds.sort(key=lambda item: (item['round'] is None, item['round']))
            publishable = (len(rounds) == len(LONG_ROUND_ORDERS)
                           and all(item['qualified'] for item in rounds))
            by_label[label] = {
                'rounds': rounds,
                'rounds_qualified': publishable,
                'series': {key: _gated_series(rounds, key, publishable)
                           for key in series_keys},
            }
        per_condition[condition] = {
            'condition': condition,
            'by_label': by_label,
            'all_three_qualified': all(by_label[l]['rounds_qualified']
                                       for l in LABELS),
        }
    return {
        'note': 'long 组每条件给出三轮每次原值 + 中位/min/max（描述性，不做显著性）。'
                '第 2 轮顺序为反向，请按轮次看。仅当同一条件三个 label 各自三轮'
                '全部技术合格时才发布 median/min/max；否则只保留逐轮原值并标为仅诊断。'
                'V 吞吐与同窗口 A 吞吐分列，不用不同窗口的 A 完成数比较 A 速度。'
                'fast 全 0 与 activation 边界并列见第 7 节（fast 全 0 不是技术失败）。',
        'series_keys': list(series_keys),
        'per_condition': per_condition,
    }


def compute_fast_diagnostics(labels: Dict[str, Dict[str, Any]]) -> Dict[str, Any]:
    """逐条件（Solo/A1/A8/A16，n=3）汇总 fast V/A 计数、seen 位与阶段计数。

    只读已有逐 case 字段；不做任何因果推断。fast 全 0 记录为事实，不作为失败。
    """
    per_condition: Dict[str, Any] = {}
    banked_labels = [l for l in LABELS if l != 'original-serial']
    for condition in LONG_CONDITIONS:
        by_label: Dict[str, Any] = {}
        for label in LABELS:
            rounds = [c for c in labels.get(label, {}).get('cases', [])
                      if c['group'] == 'long' and c['condition'] == condition]
            rounds.sort(key=lambda item: (item['round'] is None, item['round']))
            fast_totals: List[Optional[int]] = []
            fast_v: List[Optional[int]] = []
            fast_a: List[Optional[int]] = []
            fast_accepts: List[Optional[int]] = []
            seen_union = 0
            seen_any = False
            stage_totals = {name: 0 for _, name in DIAG_COUNTER_FIELDS}
            activation = {'base_s1_ptw': 0, 'full_window': 0,
                          'candidate': 0, 'runtime_enable': 0}
            diag_rounds = 0
            for case_entry in rounds:
                fast = case_entry.get('fast') or {}
                fast_totals.append(fast.get('total'))
                fast_v.append(fast.get('v_total'))
                fast_a.append(fast.get('a_total'))
                diag = case_entry.get('diagnostic') or {}
                fast_accepts.append(diag.get('fast_accept'))
                seen = diag.get('seen')
                if isinstance(seen, int) and not isinstance(seen, bool):
                    seen_any = True
                    diag_rounds += 1
                    seen_union |= seen
                    if (seen >> 0) & 1:
                        activation['base_s1_ptw'] += 1
                    if (seen >> 1) & 1:
                        activation['full_window'] += 1
                    if (seen >> 2) & 1:
                        activation['candidate'] += 1
                    if (seen >> 15) & 1:
                        activation['runtime_enable'] += 1
                counts = diag.get('counts') or {}
                for _, name in DIAG_COUNTER_FIELDS:
                    value = counts.get(name)
                    if isinstance(value, int) and not isinstance(value, bool):
                        stage_totals[name] += value
            defined = [t for t in fast_totals
                       if isinstance(t, int) and not isinstance(t, bool)]
            by_label[label] = {
                'fast_totals': fast_totals,
                'fast_v_totals': fast_v,
                'fast_a_totals': fast_a,
                'fast_accept_totals': fast_accepts,
                'fast_present': bool(defined),
                'all_fast_zero': bool(defined) and all(t == 0 for t in defined),
                'seen_union': seen_union if seen_any else None,
                'seen_bits': ([name for bit, name, _ in DIAG_SEEN_BITS
                               if (seen_union >> bit) & 1] if seen_any else None),
                'stage_totals': {name: stage_totals[name]
                                 for _, name in DIAG_COUNTER_FIELDS},
                'diag_rounds': diag_rounds,
                'activation_rounds': activation,
            }
        per_condition[condition] = {
            'condition': condition,
            'by_label': by_label,
            'all_banked_fast_zero': all(
                by_label[l]['fast_present'] and by_label[l]['all_fast_zero']
                for l in banked_labels),
            'any_banked_fast_nonzero': any(
                by_label[l]['fast_present'] and not by_label[l]['all_fast_zero']
                for l in banked_labels),
        }
    return {
        'note': ('fast V/A/other 计数来自 core words 6..10（V=w6+w7, A=w8+w9, '
                 'other=w10）；seen 为 sticky 16 位掩码；阶段计数来自第二 bank '
                 'words 7..14。fast 事件数不是命中率，也不是全部 miss 数；'
                 'fast 全 0 = 科学上未行使，不是技术失败。'),
        'seen_bit_map': [{'bit': bit, 'name': name, 'meaning': meaning}
                         for bit, name, meaning in DIAG_SEEN_BITS],
        'stage_fields': [{'word': idx, 'name': name}
                         for idx, name in DIAG_COUNTER_FIELDS],
        'per_condition': per_condition,
    }


def _effect_pair(low: Optional[float], high: Optional[float]) -> Dict[str, Any]:
    return {
        'from_median': low,
        'to_median': high,
        'delta': delta(low, high),
        'delta_pct': delta_pct(low, high),
        'published': low is not None and high is not None,
    }


def _empty_effect() -> Dict[str, Any]:
    return {
        'original_median': None,
        'serial_median': None,
        'hit_median': None,
        's_over_original': _effect_pair(None, None),
        'n_over_s': _effect_pair(None, None),
    }


def compute_effects(long_summary: Dict[str, Any]) -> Dict[str, Any]:
    per_condition: Dict[str, Any] = {}
    for condition, block in long_summary['per_condition'].items():
        by_label = block['by_label']
        reasons: List[str] = []
        if set(by_label) != set(LABELS):
            reasons.append('缺少某些 label 的三轮数据')
        for label in LABELS:
            item = by_label.get(label) or {}
            if not item.get('rounds_qualified'):
                reasons.append('%s 三轮未全部技术合格' % label)
        v_section = _empty_effect()
        a_section = _empty_effect()
        if not reasons:
            v_medians = {label: by_label[label]['series']['v_mib_s']['median']
                         for label in LABELS}
            a_medians = {label: by_label[label]['series']['a_mib_s']['median']
                         for label in LABELS}
            for section, medians in ((v_section, v_medians),
                                     (a_section, a_medians)):
                section['original_median'] = medians['original-serial']
                section['serial_median'] = medians['banked-serial']
                section['hit_median'] = medians['banked-hit']
                section['s_over_original'] = _effect_pair(
                    medians['original-serial'], medians['banked-serial'])
                section['n_over_s'] = _effect_pair(
                    medians['banked-serial'], medians['banked-hit'])
        per_condition[condition] = {
            'published': not reasons,
            'reasons': reasons,
            'v_throughput_mib_s': v_section,
            'a_throughput_mib_s': a_section,
        }
    return {
        'definition': {
            's_over_original': ('banked-serial − original-serial（结构+缓冲差异；'
                                '来源受限于旧 legacy3 vs S/N 512 缓冲，不是纯布局'
                                '因果）：original -> bankedSerial'),
            'n_over_s': ('banked-hit − banked-serial（同布局不同 enable 的描述性'
                         '变化；本轮 fast 全 0 时不构成绕过证据）：'
                         'bankedSerial -> bankedHit'),
        },
        'column_labels': {
            's_over_original': 'S/原版结构+缓冲变化',
            'n_over_s': 'N/S变化',
        },
        'attribution_warning': (
            '不允许用 original-serial 与 banked-hit 的直接差值归因绕过效应；'
            'N/S变化必须以 banked-serial 为中介，且在本轮 fast 全 0 时只能作'
            '描述性窗口周期差保留。'),
        'metric_note': (
            'V 吞吐与同窗口 A 吞吐均以 raw_window[1] 为窗口、50 MHz 换算（推导量）；'
            'A 速度只用同窗口 A 吞吐评价，不用不同窗口 A 完成数直接比较。'
            '跨 boot / 布局+缓冲差异不得作因果外推。'),
        'publish_rule': (
            '同一比较条件在 original-serial/banked-serial/banked-hit 三个 label 各自'
            '三轮（共 3×3 次）技术上全部合格时才发布；否则只列 reasons，不发布数值。'),
        'per_condition': per_condition,
        'statistics': '仅描述性中位/min/max，不做显著性检验。',
    }


# ---------------------------------------------------------------------------
# RESULTS.md 渲染
# ---------------------------------------------------------------------------

def fmt_num(value: Any, digits: int = 3) -> str:
    if value is None:
        return '—'
    if isinstance(value, float):
        if value.is_integer():
            return str(int(value))
        text = ('%.*f' % (digits, value)).rstrip('0').rstrip('.')
        return text if text else '0'
    return str(value)


def fmt_pct(value: Optional[float]) -> str:
    if value is None:
        return '—'
    return '%+.1f%%' % value


def fmt_float(value: Any, digits: int = 3) -> str:
    if value is None:
        return '—'
    return '%.*f' % (digits, value)


def _seen_bits_text(value: Any) -> str:
    if not isinstance(value, int) or isinstance(value, bool):
        return '—'
    return '0x%04X' % value


def render_markdown(doc: Dict[str, Any]) -> str:
    lines: List[str] = []

    def add(text: str = '') -> None:
        lines.append(text)

    completeness = doc['completeness']
    state_text = ('完整' if completeness['all_cases_complete'] and not doc['quality_issues']
                  else '不完整 / 存在问题')

    add('# N 非阻塞工程 pilot 离线汇总（RESULTS.md）')
    add()
    add('- 生成时间：%s' % doc['generated_at'])
    add('- 分析脚本：`scripts/analyze_nonblock_pilot.py`（离线汇总工具；可用性见本报告验收项）')
    add('- 汇总 JSON：`%s`' % RESULT_JSON_NAME)
    add('- 数据状态：**%s**（脚本退出码 = %d）' % (state_text, doc['exit_code']))
    add('- 时钟换算假设：%.0f MHz（1 cycle = %.1f ns，仅用于时间/吞吐推导）'
        % (CLOCK_HZ / 1e6, 1e9 / CLOCK_HZ))
    add('- 真实 V 固定工作量：short = %d B，long = %d B（= transfers × %d）'
        % (SHORT_TRANSFERS * CHUNK_BYTES, LONG_TRANSFERS * CHUNK_BYTES, CHUNK_BYTES))
    add('- 诊断 bank：%s' % ('已读取（`--diagnostics`）'
                             if doc['diagnostics']['enabled']
                             else '未启用（默认路径，行为与旧版一致）'))
    add()
    add('> 本文件由脚本按固定模板生成，是描述性汇总，不是实验结论声明；')
    add('> 不含显著性统计，不含 novelty 判断，不对未运行/失败项做推测或补值，')
    add('> 不修改任何原始文件。fast 事件数不是命中率，也不是全部 miss 数。')
    add()

    # ---- 1. 来源 ---------------------------------------------------------
    add('## 1. 数据来源与运行身份')
    add()
    add('| label | 目录 | 存在 | run_id | BIT | device | summary.status | STOPPED |')
    add('|---|---|---|---|---|---|---|---|')
    for label in LABELS:
        info = doc['inputs'][label]
        manifest = info.get('manifest') or {}
        stopped = '有' if info.get('stopped') else '无'
        add('| %s | `%s` | %s | %s | `%s` | %s | %s | %s |' % (
            label,
            info['path'],
            '是' if info['exists'] else '否',
            manifest.get('run_id') or '—',
            manifest.get('bit') or '—',
            manifest.get('device') or '—',
            info.get('summary_status') or '—',
            stopped,
        ))
    add()
    add('> 来源约束：original-serial 来源受限于旧 legacy3，而 S/N 为 512 缓冲；'
        'S/原版差异同时包含结构与缓冲存储差异，不得作因果外推。')
    add()

    # ---- 2. 完整度 -------------------------------------------------------
    add('## 2. 完整度矩阵（每 label 冻结 15 例，三 label 共 45 例）')
    add()
    add('| label | 完成 | 失败 | 不完整 | 未运行 | 合计 |')
    add('|---|---|---|---|---|---|')
    for label in LABELS:
        counts = completeness['per_label'][label]
        add('| %s | %d | %d | %d | %d | %d |' % (
            label, counts['complete'], counts['failed'],
            counts['incomplete'], counts['not_run'], counts['total']))
    add()
    add('| order | case | group | condition | round | state | cycles | A jobs |')
    add('|---|---|---|---|---|---|---|---|')
    for label in LABELS:
        for case_entry in doc['cases'][label]:
            add('| %d | %s / %s | %s | %s | %s | %s | %s | %s |' % (
                case_entry['order'], label, case_entry['id'],
                case_entry['group'], case_entry['condition'],
                fmt_num(case_entry['round']), case_entry['state'],
                fmt_num(case_entry['v_window_cycles']),
                fmt_num(case_entry['a_jobs_in_window']),
            ))
    add()

    # ---- 3. 质量问题 -----------------------------------------------------
    add('## 3. 质量问题清单（不剔除、不修补、不重跑）')
    add()
    if not doc['quality_issues']:
        add('无。')
    else:
        add('| # | scope | case | code | detail |')
        add('|---|---|---|---|---|')
        for index, item in enumerate(doc['quality_issues'], start=1):
            detail = str(item['detail']).replace('|', '\\\\|')
            add('| %d | %s | %s | %s | %s |' % (
                index, item['scope'] or '—', item['case'] or '—',
                item['code'], detail[:400]))
    add()
    add('> fast 计数全 0 **不是**质量问题：它只表示本轮未观测到真实 accept。')
    add()
    if doc['observations']:
        add('### 3.1 观察（不改变退出码）')
        add()
        add('| # | scope | case | code | detail |')
        add('|---|---|---|---|---|')
        for index, item in enumerate(doc['observations'], start=1):
            detail = str(item['detail']).replace('|', '\\\\|')
            add('| %d | %s | %s | %s | %s |' % (
                index, item['scope'] or '—', item['case'] or '—',
                item['code'], detail[:400]))
        add()
    add('### 3.2 runner 记录的 case problems（原样保留）')
    add()
    any_problem = False
    for label in LABELS:
        for case_entry in doc['cases'][label]:
            if case_entry['runner_problems']:
                any_problem = True
                add('- `%s/%s`: %s' % (label, case_entry['id'],
                                       '; '.join(case_entry['runner_problems'])))
    if not any_problem:
        add('无。')
    add()

    # ---- 4. 短组验收 -----------------------------------------------------
    add('## 4. 短组验收（3 例，只做验收，不与长组混合）')
    add()
    add('> %s' % doc['short_acceptance']['note'])
    add()
    add('| label | case | condition | state | qualified | cycles | ns | V MiB/s | A jobs | A B | A MiB/s | A 字段来源 | raw_window 一致 | helper 一致 |')
    add('|---|---|---|---|---|---|---|---|---|---|---|---|---|---|')
    for label in LABELS:
        for row in doc['short_acceptance']['per_label'][label]:
            add('| %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s |' % (
                label, row['id'], row['condition'], row['state'],
                '是' if row['qualified'] else '否',
                fmt_num(row['cycles']), fmt_float(row['ns'], 1),
                fmt_float(row['mib_s'], 2), fmt_num(row['a_jobs']),
                fmt_num(row['a_bytes']), fmt_float(row['a_mib_s'], 2),
                row['a_jobs_field'] or '—',
                {True: '是', False: '否', None: '—'}[row['raw_window_ok']],
                {True: '是', False: '否', None: '不适用'}[row['helper_ok']],
            ))
    add()

    # ---- 5. 长组 ---------------------------------------------------------
    add('## 5. 长组：每条件每次值 + 中位/min/max')
    add()
    add('> %s' % doc['long_per_condition']['note'])
    add()
    series_titles = (
        ('cycles', '窗口周期数 raw_window[1]'),
        ('v_mib_s', 'V 吞吐 MiB/s（transfers×4096 / 窗口）'),
        ('a_jobs', '同窗口 A 完成数 raw_window[3]'),
        ('a_bytes', '同窗口 A 字节（w3×4096）'),
        ('a_mib_s', '同窗口 A 吞吐 MiB/s'),
    )
    for condition in LONG_CONDITIONS:
        block = doc['long_per_condition']['per_condition'][condition]
        add('### 5.%s 条件 %s' % (LONG_CONDITIONS.index(condition) + 1, condition))
        add()
        for key, title in series_titles:
            add('**%s** — %s' % (key, title))
            add()
            add('| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |')
            add('|---|---|---|---|---|---|---|---|---|')
            for label in LABELS:
                item = block['by_label'][label]
                series = item['series'][key]
                by_round = {r['round']: r[key] for r in item['rounds']}
                add('| %s | %s | %s | %s | %s | %s | %s | %s | %d |' % (
                    label, '是' if item['rounds_qualified'] else '否',
                    fmt_num(by_round.get(1)), fmt_num(by_round.get(2)),
                    fmt_num(by_round.get(3)),
                    fmt_num(series['median']), fmt_num(series['min']),
                    fmt_num(series['max']), series['n']))
            add()
        add('> 条件整体合格 = %s；不合格时该条件 median/min/max 为空。'
            % ('是' if block['all_three_qualified'] else '否'))
        add()

    # ---- 6. 两段对照 -----------------------------------------------------
    add('## 6. 两段对照（仅 long 组，弱因果表述）')
    add()
    add('- `S/原版结构+缓冲变化`：`%s`'
        % doc['effects']['definition']['s_over_original'])
    add('- `N/S变化`：`%s`' % doc['effects']['definition']['n_over_s'])
    add('- **%s**' % doc['effects']['attribution_warning'])
    add('- %s' % doc['effects']['metric_note'])
    add('- 发布规则：%s' % doc['effects']['publish_rule'])
    add('- %s' % doc['effects']['statistics'])
    add()

    diag_block = doc['diagnostics']
    labels_text = doc['effects']['column_labels']

    def effect_table(kind: str, heading: str) -> None:
        add('### %s' % heading)
        add()
        add('| condition | published | orig 中位 | serial 中位 | hit 中位 |'
            ' %s Δ | %s Δ%% | %s Δ | %s Δ%% |'
            % (labels_text['s_over_original'], labels_text['s_over_original'],
               labels_text['n_over_s'], labels_text['n_over_s']))
        add('|---|---|---|---|---|---|---|---|---|')
        for condition in LONG_CONDITIONS:
            item = doc['effects']['per_condition'][condition]
            section = item[kind]
            s_pair = section['s_over_original']
            n_pair = section['n_over_s']
            add('| %s | %s | %s | %s | %s | %s | %s | %s | %s |' % (
                condition, '是' if item['published'] else '否',
                fmt_num(section['original_median']),
                fmt_num(section['serial_median']),
                fmt_num(section['hit_median']),
                fmt_num(s_pair['delta']), fmt_pct(s_pair['delta_pct']),
                fmt_num(n_pair['delta']), fmt_pct(n_pair['delta_pct']),
            ))
        add()
        for condition in LONG_CONDITIONS:
            item = doc['effects']['per_condition'][condition]
            fast_block = diag_block['per_condition'][condition]
            add('- 条件 %s：all_fast_zero = %s；any_fast_nonzero = %s'
                % (condition,
                   '是' if fast_block['all_banked_fast_zero'] else '否',
                   '是' if fast_block['any_banked_fast_nonzero'] else '否'))
            if not item['published']:
                add('- 条件 %s 未发布：%s'
                    % (condition, '；'.join(item['reasons']) or '数据不合格'))
        add()

    effect_table('v_throughput_mib_s', '6.1 V 吞吐（MiB/s）收益 / 代价')
    effect_table('a_throughput_mib_s', '6.2 同窗口 A 吞吐（MiB/s）收益 / 代价')

    # ---- 7. 诊断 bank ----------------------------------------------------
    add('## 7. 诊断 bank（可选 `--diagnostics`；只读，不反馈 DMA/AXI）')
    add()
    if not diag_block['enabled']:
        add('本次未启用 `--diagnostics`；未读取第二 16 字诊断 bank。')
        add('旧 16 字 core ABI 与 v3 raw 兼容性未受影响；默认路径行为不变。')
        add()
    else:
        add('> %s' % diag_block['note'])
        add()
        for bit, name, meaning in DIAG_SEEN_BITS:
            add('- seen bit%d `%s`：%s' % (bit, name, meaning))
        add()
        add('解释许可与禁止（固定）')
        add()
        for note in diag_block['notes']:
            add('- %s' % note)
        add()
        add('### 7.1 逐 case fast 计数（来自 core words 6..10）')
        add()
        add('| label | case | rnd | fast V | fast A | fast other | fast Σ | all0 |')
        add('|---|---|---|---|---|---|---|---|')
        for label in LABELS:
            for case_entry in doc['cases'][label]:
                fast = case_entry.get('fast') or {}
                add('| %s | %s | %s | %s | %s | %s | %s | %s |' % (
                    label, case_entry['id'], fmt_num(case_entry['round']),
                    fmt_num(fast.get('v_total')), fmt_num(fast.get('a_total')),
                    fmt_num(fast.get('other')), fmt_num(fast.get('total')),
                    {True: '是', False: '否', None: '—'}[fast.get('all_zero')],
                ))
        add()
        add('### 7.2 逐 case 诊断 seen 位与阶段计数（第二 bank words 6..15）')
        add()
        add('| label | case | seen | 阶段计数（probe/dc_eval/dc_good/leaf_eval/leaf_good/dc_msi_rej/leaf_msi_rej/leaf_miss/fast_acc） | diag_ok |')
        add('|---|---|---|---|---|')
        for label in LABELS:
            for case_entry in doc['cases'][label]:
                diag = case_entry.get('diagnostic') or {}
                counts = diag.get('counts') or {}
                counter_text = '/'.join(
                    fmt_num(counts.get(name)) for _, name in DIAG_COUNTER_FIELDS)
                add('| %s | %s | %s | %s | %s |' % (
                    label, case_entry['id'], _seen_bits_text(diag.get('seen')),
                    counter_text,
                    {True: '是', False: '否', None: '不适用'}[diag.get('ok')],
                ))
        add()
        add('### 7.3 长组每条件 fast/activation 汇总（Solo/A1/A8/A16，n=3）')
        add()
        for condition in LONG_CONDITIONS:
            block = diag_block['per_condition'][condition]
            add('#### 条件 %s（all_fast_zero = %s）'
                % (condition, '是' if block['all_banked_fast_zero'] else '否'))
            add()
            add('| label | fast V r1/r2/r3 | fast A r1/r2/r3 | fast Σ r1/r2/r3 | fast_acc r1/r2/r3 | all0 | seen 并集 | seen bits |')
            add('|---|---|---|---|---|---|---|---|')
            for label in LABELS:
                item = block['by_label'][label]
                add('| %s | %s | %s | %s | %s | %s | %s | %s |' % (
                    label,
                    '/'.join(fmt_num(v) for v in item['fast_v_totals']),
                    '/'.join(fmt_num(v) for v in item['fast_a_totals']),
                    '/'.join(fmt_num(v) for v in item['fast_totals']),
                    '/'.join(fmt_num(v) for v in item['fast_accept_totals']),
                    {True: '是', False: '否', None: '—'}[item['all_fast_zero']],
                    _seen_bits_text(item['seen_union']),
                    ','.join(item['seen_bits'] or []) or '—',
                ))
            add()
            add('| label | base_s1_ptw | full_window | candidate | runtime_enable | probe_starts | dc_eval | dc_good | leaf_eval | leaf_good | dc_msi_rej | leaf_msi_rej | leaf_miss | fast_acc |')
            add('|---|---|---|---|---|---|---|---|---|---|---|---|---|---|')
            for label in LABELS:
                item = block['by_label'][label]
                act = item['activation_rounds']
                stage = item['stage_totals']
                add('| %s | %d/%d | %d/%d | %d/%d | %d/%d | %s | %s | %s | %s | %s | %s | %s | %s | %s |' % (
                    label,
                    act['base_s1_ptw'], len(item['fast_totals']),
                    act['full_window'], len(item['fast_totals']),
                    act['candidate'], len(item['fast_totals']),
                    act['runtime_enable'], len(item['fast_totals']),
                    fmt_num(stage.get('probe_starts')),
                    fmt_num(stage.get('dc_evaluations')),
                    fmt_num(stage.get('dc_good')),
                    fmt_num(stage.get('leaf_evaluations')),
                    fmt_num(stage.get('leaf_good')),
                    fmt_num(stage.get('dc_msi_rejects')),
                    fmt_num(stage.get('leaf_msi_rejects')),
                    fmt_num(stage.get('leaf_lookup_misses')),
                    fmt_num(stage.get('fast_accepts')),
                ))
            add()
        add('> 计数窗口 (open, close]，跨窗口的部分 probe 合法；start/eval/leaf 之间'
            '不假设完整生命周期关系。seen bit0 是宽口径 base S1 PTW，可能含 '
            'implicit/debug。若未见 bit2（candidate）或 probe 为 0，只能说本轮未'
            '观测到合格窗口候选，不能说明没有 cache 命中。DC good=0 且无显式 MSI '
            'reject 时不能唯一归因 MSI。fast>0 只表示本轮真实 accept，不是命中率，'
            '也不是 novelty；fast 全 0 是科学上未行使，不是技术失败。')
        add()

    # ---- 8. 原始字段证据 -------------------------------------------------
    add('## 8. 原始字段证据')
    add()
    add('### 8.1 p4m.raw_window 逐 case（16 字，原样，未做任何解释性替换）')
    add()
    header = '| label | case | ' + ' | '.join('w%d' % i for i in range(P4M_WINDOW_WORDS)) + ' |'
    add(header)
    add('|---|---|' + '---|' * P4M_WINDOW_WORDS)
    for label in LABELS:
        for case_entry in doc['cases'][label]:
            window = case_entry['raw_window']
            if window is None:
                cells = ['—'] * P4M_WINDOW_WORDS
            else:
                cells = [str(value) for value in window]
            add('| %s | %s | %s |' % (label, case_entry['id'], ' | '.join(cells)))
    add()
    add('字段定位：')
    add()
    add('- 由 `scripts/mac_nonblock_pilot.py` 的 `validate_record()` 钉死：'
        '`w0` = bank magic，`w1` = 窗口周期数（1 字 = 1 cycle，== `raw_observation[0]`），'
        '`w13` = V issued，`w14` = V done（后三者应等于该 case 的 transfers）。')
    add('- 由 `bench/idma_experiment.c:487` 确认：`w3` = 窗口内 A 完成数'
        '（`a_completed_jobs_window`），与 `p4m.a_completed_jobs_window` 交叉核对；'
        '同窗口 A 字节 = `w3 × 4096`。')
    add('- 其余 `w2`、`w4..w12`、`w15` 本脚本只原样保存，不做语义解释。')
    add()
    add('### 8.2 A 窗口内完成数的取值来源（逐 case）')
    add()
    add('| label | case | A jobs | 字段来源 | 状态 |')
    add('|---|---|---|---|---|')
    for label in LABELS:
        for case_entry in doc['cases'][label]:
            if case_entry['group'] != 'long':
                continue
            add('| %s | %s | %s | %s | %s |' % (
                label, case_entry['id'],
                fmt_num(case_entry['a_jobs_in_window']),
                case_entry['a_jobs_field'] or '未取得',
                case_entry['state']))
    add()
    add('### 8.3 helper 窗口摘要（完整 16 字见 JSON `cases[*].helper.words`）')
    add()
    add('| label | case | mode | seq=words[4] | cycles | flags | errors | Σfast | Σnormal | accepted | 来源 |')
    add('|---|---|---|---|---|---|---|---|---|---|---|')
    for label in LABELS:
        for case_entry in doc['cases'][label]:
            helper = case_entry['helper'] or {}
            if not helper.get('present'):
                continue
            add('| %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s |' % (
                label, case_entry['id'],
                fmt_num(helper.get('mode')), fmt_num(helper.get('seq')),
                fmt_num(helper.get('cycles')), fmt_num(helper.get('flags')),
                fmt_num(helper.get('errors')), fmt_num(helper.get('fast_total')),
                fmt_num(helper.get('normal_total')), fmt_num(helper.get('accepted_total')),
                'helper.json'))
    add()
    add('### 8.4 p4m.raw_observation')
    add()
    add('`raw_observation` 是旧 HPM 观测数组（本脚本只记录长度，值在 JSON 里原样保留）：')
    add()
    add('| label | case | len(raw_observation) |')
    add('|---|---|---|')
    for label in LABELS:
        for case_entry in doc['cases'][label]:
            add('| %s | %s | %s |' % (
                label, case_entry['id'],
                fmt_num(case_entry['raw_observation_len'])))
    add()
    add('> 旧 HPM / COST / E3 / miss / PTW 数据 **不作为 N 机制证据，也不作为有效 '
        'metric**；banked 结构下 `victim/aggressor.iotlb_miss/s1_ptw/s2_ptw` 一律'
        '不得使用。helper 的快速路径事件数 **不是命中率，也不是全部 miss 数**。')
    add()

    # ---- 9. 不确定性 -----------------------------------------------------
    add('## 9. 不确定性、未运行项与未做的事')
    add()
    for item in doc['uncertainties']:
        add('- %s' % item)
    add()
    add('未运行 / 失败 / 不完整的用例已逐条列在第 2 节与第 3 节，不做补值、不做剔除、不自动重跑。')
    add()
    return '\n'.join(lines) + '\n'


# ---------------------------------------------------------------------------
# 顶层
# ---------------------------------------------------------------------------

def build_diagnostic_notes() -> List[str]:
    return [
        'seen bit0 是宽口径 base S1 PTW 活动，可能包含 implicit/debug 触发；'
        'bit1 才是完整 fast 窗口谓词，bit2 才是窗口内候选有效。',
        '未出现 bit2（candidate in window）或 probe start = 0 时，只能说明本轮'
        '未观测到合格窗口候选，不能说明没有 cache 命中。',
        'DC good 计数为 0 且没有显式 MSI reject 时，不能唯一归因 MSI；只有显式 '
        'reject 计数才支持该归因。',
        'fast 计数 > 0 表示本轮真实 accept，不是命中率，也不是 novelty；'
        'fast 全 0 只表示机制未被触发（科学上未行使），不是技术失败。',
        '诊断采样跨窗口时 start/eval/leaf 计数之间无需守恒，不假设生命周期完整。',
        '旧 HPM / miss / PTW 字段在 banked 结构下不是有效 metric，绝不作为机制证据。',
        'S 来源受限于旧 legacy3 vs S/N 512 缓冲差异：跨 boot 与布局+缓冲差异不得作因果外推。',
    ]


def build_uncertainties() -> List[str]:
    return [
        '`bench/idma_experiment.c:487` 明确窗口内 A 完成数 = `raw_window[3]`；'
        '本脚本据此设 `RAW_WINDOW_A_JOBS_INDEX = 3`，并与 '
        '`record.p4m.a_completed_jobs_window` 交叉核对，绝不使用整轮 A 计数。',
        '窗口周期数取自 `raw_window[1]`，单位按 1 字 = 1 cycle 处理；'
        'ns 与 MiB/s 是以 50 MHz 为假设的推导量，不是直接测量值。'
        'V 吞吐与同窗口 A 吞吐共用这一窗口换算。',
        '真实 V 工作量固定为 transfers × 4096 B；同窗口 A 工作量 = w3 × 4096 B；'
        '脚本按此计算，不重新测数据量。',
        'A 速度只用同一窗口的 A 吞吐评价；不用不同窗口的 A 完成数直接比较 A 速度。',
        '短组只用做验收，不与长组混合；长组每条件三轮值 + 中位/min/max 均为描述性，'
        '不做显著性检验，不能据此外推。',
        '失败 / 残缺数据逐项保留 raw 并分类，只作诊断；同一比较条件三轮技术上未全部'
        '合格时，不发布该条件的性能汇总与两段对照，也不把残缺数值与有效样本混合，'
        '更不剔除不利但合法的合格样本。fast=0 是合法观察，不产生 issue、不剔除。',
        '每个 label 只对应一次运行、一次新鲜启动；跨 label 的差异同时包含运行间波动，'
        '本脚本不做方差分析。S 来源受限于旧 legacy3 与 S/N 512 缓冲差异，'
        '不得声称因果。',
        'banked-hit 相对 banked-serial 的差值只是描述性 `N/S变化`；'
        'original-serial 与 banked-hit 的直接差值不用于绕过归因。'
        '本轮 fast 计数全 0 时，`N/S变化` 不构成绕过效果证据。',
        'helper 窗口语义为 (open, close]；序号一律取 `words[4]`（reader emit_json 的 '
        '`windowseq` 亦等于 `words[4]`，仅在存在时做交叉核对）；脚本只校验 ABI 冻结位、'
        '计数守恒、与 P4M 相同的 cycles、Mode1 快速项全 0、Mode2 可大于 0、序号严格递增。',
        '`--diagnostics` 打开时，第二 16 字诊断 bank 从 `helper.json` 的 `diagnostic` '
        '对象读取（`diag-window.json` 仅在存在时交叉核对）；关闭时行为与旧版一致。',
        '诊断 bank 的字段语义由 RTL/runner 契约决定；本脚本只做只读交叉核对，不实现硬件。',
        '只按冻结计划的 15 个 case id 打开确切路径，未枚举目录；'
        '因此计划外的额外 case 目录不会被发现。',
        '技术资格（qualified）已汇合该 case 的全部质量 issue 与该 label 的 '
        'plan/boot/flash/caps/summary 必要质量问题；合法偏低或波动样本与 fast=0 '
        '不产生 issue，仍保留并参与汇总，但任一真实 issue 都会阻止两段对照发布。',
        '不做哈希校验、不从设备读回、不重跑、不修改任何原始文件。',
        '旧 HPM / COST / E3 / miss / PTW 数据不是本设计的机制证据；'
        'helper 快速路径事件数不是命中率，也不是全部 miss 数；不宣称 novelty。',
    ]


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description='离线汇总冻结的 N 工程 pilot 三个 runner 输出目录：'
                    '只读输入，只写 --out，输出唯一 JSON 汇总 + RESULTS.md。')
    parser.add_argument('--original', required=True, metavar='DIR',
                        help='original-serial 的 runner 输出目录')
    parser.add_argument('--serial', required=True, metavar='DIR',
                        help='banked-serial 的 runner 输出目录')
    parser.add_argument('--hit', required=True, metavar='DIR',
                        help='banked-hit 的 runner 输出目录')
    parser.add_argument('--out', required=True, metavar='DIR',
                        help='汇总输出目录；不得与任何输入目录相同或互相包含')
    parser.add_argument('--diagnostics', action='store_true',
                        help='额外要求 banked-serial / banked-hit 每个 case 的 '
                             'helper.json 带第二 16 字只读诊断 bank（diagnostic.words）'
                             '并逐项校验；关闭时行为与旧版一致')
    return parser


def write_atomic(path: pathlib.Path, text: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + '.tmp')
    tmp.write_text(text, encoding='utf-8')
    tmp.replace(path)


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = build_arg_parser().parse_args(argv)

    inputs = {
        'original-serial': pathlib.Path(args.original),
        'banked-serial': pathlib.Path(args.serial),
        'banked-hit': pathlib.Path(args.hit),
    }
    out = pathlib.Path(args.out)

    seen: Dict[str, str] = {}
    for label, path in inputs.items():
        key = str(path.resolve())
        if key in seen and seen[key] != label:
            raise SystemExit('输入目录重复：%s 与 %s 都指向 %s'
                             % (seen[key], label, key))
        seen[key] = label

    for label, path in inputs.items():
        if same_or_within(out, path) or same_or_within(path, out):
            raise SystemExit('--out 不得与输入目录重叠（%s: %s）'
                             % (label, path.as_posix()))

    try:
        out.mkdir(parents=True, exist_ok=True)
    except OSError as exc:
        raise SystemExit('无法创建 --out %s: %r' % (out, exc))

    an = Analysis()
    plan = build_frozen_plan()

    labels: Dict[str, Dict[str, Any]] = {}
    for label in LABELS:
        labels[label] = summarize_label(an, label, inputs[label], plan,
                                        diagnostics=args.diagnostics)

    completeness = compute_completeness(labels, plan)
    short_summary = compute_short(labels)
    long_summary = compute_long(labels)
    effects = compute_effects(long_summary)
    fast_diagnostics = compute_fast_diagnostics(labels)
    fast_diagnostics['enabled'] = bool(args.diagnostics)
    fast_diagnostics['notes'] = build_diagnostic_notes()

    quality_ok = not an.quality
    exit_code = 0 if (completeness['all_cases_complete'] and quality_ok) else 1

    analysis_doc: Dict[str, Any] = {
        'schema': ANALYSIS_SCHEMA,
        'generated_at': utc_now(),
        'analyzer': {
            'script': 'scripts/analyze_nonblock_pilot.py',
            'status': 'executed; see exit_code and quality_issues',
            'diagnostics_enabled': bool(args.diagnostics),
            'clock_hz': CLOCK_HZ,
            'chunk_bytes': CHUNK_BYTES,
            'cycle_unit': 'raw_window[1]，1 字 = 1 cycle',
            'raw_window_field_map': {
                'pinned_by_runner': {
                    'w0': 'bank magic',
                    'w1': 'window cycles (== raw_observation[0])',
                    'w3': 'A done in window (== p4m.a_completed_jobs_window)',
                    'w13': 'V issued (== transfers)',
                    'w14': 'V done (== transfers)',
                },
                'a_jobs_index': RAW_WINDOW_A_JOBS_INDEX,
                'a_jobs_note': 'index 3 == a_done_window == p4m.a_completed_jobs_window '
                               '(bench/idma_experiment.c:487); A bytes = w3 * 4096',
            },
            'diagnostic_bank': {
                'magic': DIAG_MAGIC,
                'version': DIAG_VERSION,
                'words': DIAG_WORDS,
                'index_range': '16..31 (NB_INDEX_BASE + 16)',
                'source': 'helper.json 的 diagnostic 对象（可选 core_fast_sum 作核对）',
            },
        },
        'frozen_plan': plan,
        'inputs': {label: labels[label] for label in LABELS},
        'cases': {label: labels[label]['cases'] for label in LABELS},
        'completeness': completeness,
        'short_acceptance': short_summary,
        'long_per_condition': long_summary,
        'effects': effects,
        'diagnostics': fast_diagnostics,
        'quality_issues': an.quality,
        'observations': an.observations,
        'uncertainties': build_uncertainties(),
        'exit_code': exit_code,
        'boundary_notes': [
            '只读三个 runner 输出目录，只写 --out；原始文件不修改。',
            '不做目录全扫描、不做哈希、不从设备读回。',
            '直接按 runner validate_record() 源逐字段复核 record（schema/success/'
            'result.status/victim|aggressor.data_errors/quarantined）与 p4m '
            'raw_window/raw_observation 一致性；不导入 runner、不写设备。',
            'qualified 汇合该 case 全部 quality issue 与该 label 的 plan/boot/'
            'flash/caps/summary 必要问题；有真实技术问题时 compute_effects 不发布。',
            'fast 全 0 是科学上未行使，不是技术失败，不产生 quality issue。',
            '不剔除异常、不自动重跑、不做显著性统计。',
            '旧 HPM / COST / E3 / miss / PTW 不作 N 机制证据，也不作有效 metric。',
            '快速路径事件数不是命中率，也不是全部 miss 数。',
            '两段对照列按弱因果措辞重命名：S/原版结构+缓冲变化、N/S变化。',
            'S 来源受限于旧 legacy3 vs S/N 512 缓冲；跨 boot/布局+缓冲差异不得作因果外推。',
        ],
    }

    write_atomic(out / RESULT_JSON_NAME,
                 json.dumps(analysis_doc, ensure_ascii=False, indent=2,
                            sort_keys=False) + '\n')
    write_atomic(out / RESULT_MD_NAME, render_markdown(analysis_doc))

    print('written: %s' % (out / RESULT_JSON_NAME).as_posix())
    print('written: %s' % (out / RESULT_MD_NAME).as_posix())
    print('complete=%s quality_issues=%d diagnostics=%s exit=%d'
          % (completeness['all_cases_complete'], len(an.quality),
             bool(args.diagnostics), exit_code))
    return exit_code


if __name__ == '__main__':
    sys.exit(main())
````

---

FILE: tests/test_analyze_nonblock_diagnostics.py

````python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Focused tests for the optional v4 diagnostic bank in scripts/analyze_nonblock_pilot.py.

Covers, without mirroring the whole analyzer:
  * diagnostic validity (a clean bank passes),
  * boundary checks (flags overflow bit, uint64 / bool range),
  * mismatches (magic / mode / seq / cycles / fast_accept / core_fast_sum),
  * scientific semantics: core fast counts all zero must NOT fail a case.

No hardware, no runner import, no device writes. These tests build a synthetic
single-case directory tree in a temp dir and call analyzer functions directly.
"""

from __future__ import annotations

import importlib.util
import json
import pathlib
import sys
import tempfile
import unittest

_HERE = pathlib.Path(__file__).resolve()
_ANALYZER_PATH = _HERE.parent.parent / 'scripts' / 'analyze_nonblock_pilot.py'

_spec = importlib.util.spec_from_file_location(
    'analyze_nonblock_pilot', _ANALYZER_PATH)
assert _spec is not None and _spec.loader is not None
an_mod = importlib.util.module_from_spec(_spec)
sys.modules.setdefault('analyze_nonblock_pilot', an_mod)
_spec.loader.exec_module(an_mod)

P4M_BANK_MAGIC = an_mod.P4M_BANK_MAGIC
DIAG_MAGIC = an_mod.DIAG_MAGIC
HELPER_MAGIC = an_mod.HELPER_MAGIC

# A real banked-serial long-r1-a1 core window (fast counts all zero).
CORE_WORDS = [
    5279429606066244609, 1, 1, 4, 5, 5581750,
    0, 0, 0, 0, 0,          # fast 6..10 (all zero)
    16384, 16383, 0,        # normal 11..13
    32767,                  # accepted 14
    0,                      # errors 15
]

RAW_WINDOW = [
    5279429606098881793, 5581750, 4096, 4096, 1, 1, 1, 0,
    51, 4147, 51, 4146, 0, 4096, 4096, 7,
]

RAW_OBSERVATION = [
    5581750, 5577655, 0, 4095, 1406, 0, 1, 4096, 4096, 1, 0, 8192, 8191,
    5565367, 8192, 8192, 8192, 8192, 0, 0, 8192, 8192, 8192, 8192, 0, 0,
    16384, 16384, 0, 5384641, 0, 131074, 0, 131074, 16530,
]


def good_diag_words():
    """seen bits 0..3 set, one DC MSI reject, fast_accepts == 0 == Σcore fast."""
    seen = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)
    return [
        DIAG_MAGIC, 1, 1, 4, 5, 5581750,
        seen,
        1,   # probe_starts
        1,   # dc_evaluations
        0,   # dc_good
        0,   # leaf_evaluations
        0,   # leaf_good
        1,   # dc_msi_rejects
        0,   # leaf_msi_rejects
        0,   # leaf_lookup_misses
        0,   # fast_accepts
    ]


def diag_object(words=None, core_fast_sum=0, with_core_fast_sum=True):
    obj = {'words': list(good_diag_words() if words is None else words)}
    if with_core_fast_sum:
        obj['core_fast_sum'] = core_fast_sum
    return obj


def _write_json(path, obj):
    path.write_text(json.dumps(obj), encoding='utf-8')


def _make_case(root, spec, diag_words=None, include_diagnostic=True,
               helper_words=None, core_fast_sum=0):
    case_dir = pathlib.Path(root) / 'cases' / spec['id']
    case_dir.mkdir(parents=True, exist_ok=True)
    core = list(helper_words if helper_words is not None else CORE_WORDS)

    _write_json(case_dir / 'case.json', {
        'exit': 0, 'framed_exit': 0, 'taint_before': '0', 'taint_after': '0',
        'problems': [],
    })
    (case_dir / 'exit.txt').write_text('0\n', encoding='utf-8')
    (case_dir / 'helper.exit.txt').write_text('0\n', encoding='utf-8')

    _write_json(case_dir / 'metrics.json', {
        'a_completed_jobs_window': 4096, 'condition': spec['condition'],
        'group': spec['group'], 'round': spec['round'],
        'transfers': spec['transfers'], 'warmup': spec['warmup'],
        'v_window_cycles': 5581750,
    })

    _write_json(case_dir / 'record.json', {
        'schema': 'idma.p4m.v1', 'success': True, 'error': None,
        'result': {
            'status': 0,
            'victim': {'data_errors': 0, 'quarantined': 0},
            'aggressor': {'data_errors': 0, 'quarantined': 0},
        },
        'p4m': {
            'bank_magic': P4M_BANK_MAGIC,
            'observer_status': 4,
            'raw_window': list(RAW_WINDOW),
            'raw_observation': list(RAW_OBSERVATION),
            'cycles': 5581750,
            'closed': True,
            'has_a': True,
            'a_completed_jobs_window': 4096,
        },
    })

    helper = {
        'abi': 'idma-nonblock-window-v1',
        'words': core,
        'mode': 1,
        'cycles': 5581750,
        'windowseq': 5,
        'fastcounts': [0, 0, 0, 0, 0],
        'normalcounts': [16384, 16383, 0],
    }
    if include_diagnostic:
        helper['diagnostic'] = diag_object(diag_words, core_fast_sum=core_fast_sum)
    _write_json(case_dir / 'helper.json', helper)

    _write_json(case_dir / 'helper-window.json', {
        'schema': 'nonblock-pilot-helper-window-v1',
        'window': core,
    })
    return case_dir


LONG_A1_SPEC = {
    'id': 'long-r1-a1', 'order': 5, 'group': 'long', 'condition': 'A1',
    'round': 1, 'transfers': 4096, 'warmup': 256,
}


class DiagnosticValidationTests(unittest.TestCase):
    def test_valid_diagnostic_passes(self):
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', diag_object(), CORE_WORDS)
        self.assertEqual(problems, [])

    def test_magic_and_mode_mismatch(self):
        words = good_diag_words()
        words[0] = 0
        words[2] = 2  # wrong mode for banked-serial
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': words}, CORE_WORDS)
        self.assertTrue(any('magic' in p for p in problems))
        self.assertTrue(any('mode' in p for p in problems))

    def test_uint64_range_and_bool_rejected(self):
        neg = good_diag_words()
        neg[5] = -1
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': neg}, CORE_WORDS)
        self.assertTrue(any('uint64' in p for p in problems))

        boolean = good_diag_words()
        boolean[5] = True
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': boolean}, CORE_WORDS)
        self.assertTrue(any('bool' in p or 'uint64' in p for p in problems))

    def test_flags_overflow_boundary(self):
        words = good_diag_words()
        words[3] = 4 | (1 << 63)  # frozen low bits, overflow bit set
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': words}, CORE_WORDS)
        self.assertTrue(any('overflow' in p for p in problems))
        self.assertFalse(any('frozen 低位' in p for p in problems))

    def test_seq_and_cycles_mismatch(self):
        words = good_diag_words()
        words[4] = 6       # core seq is 5
        words[5] = 1       # core cycles is 5581750
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': words}, CORE_WORDS)
        self.assertTrue(any('seq' in p for p in problems))
        self.assertTrue(any('cycles' in p for p in problems))

    def test_fast_accept_must_equal_core_fast_sum(self):
        words = good_diag_words()
        words[15] = 3  # Σ core fast 6..10 == 0
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': words}, CORE_WORDS)
        self.assertTrue(any('fast_accept' in p for p in problems))

    def test_core_fast_sum_optional_verifier_if_present(self):
        # Absent -> no core_fast_sum problem, still valid.
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', diag_object(with_core_fast_sum=False), CORE_WORDS)
        self.assertEqual(problems, [])
        # Present but wrong -> problem.
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', diag_object(core_fast_sum=5), CORE_WORDS)
        self.assertTrue(any('core_fast_sum' in p for p in problems))

    def test_nonzero_core_fast_matches(self):
        core = list(CORE_WORDS)
        core[6] = 2
        core[9] = 3           # A write
        core[14] = 32767 + 5  # keep accept conservation sensible
        words = good_diag_words()
        words[15] = 5
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': words, 'core_fast_sum': 5}, core)
        self.assertEqual(problems, [])


class FastZeroScientificTests(unittest.TestCase):
    def test_fast_zero_is_not_a_technical_failure(self):
        with tempfile.TemporaryDirectory() as tmp:
            case_dir = _make_case(tmp, LONG_A1_SPEC)
            an = an_mod.Analysis()
            entry = an_mod.analyze_case(
                an, 'banked-serial', LONG_A1_SPEC, case_dir, diagnostics=True)

            self.assertEqual(entry['state'], 'complete')
            self.assertIsNotNone(entry['fast'])
            self.assertTrue(entry['fast']['all_zero'])
            self.assertEqual(entry['fast']['total'], 0)
            self.assertIsNotNone(entry['diagnostic'])
            self.assertTrue(entry['diagnostic']['ok'])
            # No quality issue from the diagnostic bank, and the case qualifies.
            self.assertEqual([q for q in an.quality if q['code'] == 'helper_diagnostic'], [])
            self.assertTrue(entry['qualified'], entry['qualified_reasons'])
            self.assertEqual(entry['qualified_reasons'], [])

    def test_missing_diagnostic_fails_only_when_requested(self):
        with tempfile.TemporaryDirectory() as tmp:
            case_dir = _make_case(tmp, LONG_A1_SPEC, include_diagnostic=False)

            an_off = an_mod.Analysis()
            entry_off = an_mod.analyze_case(
                an_off, 'banked-serial', LONG_A1_SPEC, case_dir, diagnostics=False)
            self.assertTrue(entry_off['qualified'], entry_off['qualified_reasons'])

            an_on = an_mod.Analysis()
            entry_on = an_mod.analyze_case(
                an_on, 'banked-serial', LONG_A1_SPEC, case_dir, diagnostics=True)
            self.assertFalse(entry_on['qualified'])
            self.assertTrue(any('诊断' in r for r in entry_on['qualified_reasons']))
            self.assertTrue(any(q['code'] == 'helper_diagnostic' for q in an_on.quality))


if __name__ == '__main__':
    unittest.main()
````

---

## Acceptance checks (concise; Codex should run and verify)

```bash
# 1) Syntax / import only
python3 -m py_compile scripts/analyze_nonblock_pilot.py
python3 -m py_compile tests/test_analyze_nonblock_diagnostics.py

# 2) Focused unit tests (no hardware)
python3 -m pytest tests/test_analyze_nonblock_diagnostics.py -q

# 3) Default path must remain v3-compatible (same CLI, same exit semantics)
python3 scripts/analyze_nonblock_pilot.py \
    --original <DIR-original> --serial <DIR-serial> --hit <DIR-hit> --out <OUT>
# -> JSON: no `diagnostics.enabled: true`, exit code unchanged vs v3
# -> RESULTS.md: contrast columns now read "S/原版结构+缓冲变化" / "N/S变化"

# 4) Optional diagnostics path
python3 scripts/analyze_nonblock_pilot.py \
    --original <DIR-original> --serial <DIR-serial> --hit <DIR-hit> --out <OUT> --diagnostics
# -> banked-serial/banked-hit every case must carry helper.json diagnostic.words[16]
#    valid; a malformed bank makes the case technically unqualified (exit 1)
```

## Unresolved / for Codex to confirm

1. Diagnostic source location: I read `helper.json -> diagnostic` (per the runner diff you supplied) and treat `diag-window.json` as an optional cross-check. If the runner instead emits the bank only in `diag-window.json`, flip the primary/optional source (one place: `analyze_case`).
2. JSON key naming: `layout_original_to_serial` / `bypass_serial_to_hit` are kept for backward compatibility; the requested rename is applied to the RESULTS.md columns and to `effects.definition` / `effects.column_labels`. Confirm if you want the JSON keys renamed too.
3. `diag_entry['ok']` semantics for `original-serial` is `None` (no helper, so diagnostic not applicable) — consistent with `helper_ok=None`.
4. I did not execute anything; no runtime or hardware result is claimed.
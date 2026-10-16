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
``STOPPED.txt``，以及 ``cases/<case-id>/`` 下的 ``case.json`` /
``metrics.json`` / ``record.json`` / ``exit.txt`` / ``helper.json`` /
``helper.exit.txt`` / ``helper-window.json``。本脚本只按**冻结计划里的 15 个
case id**逐个打开这些确切路径，不做目录枚举、不做哈希、不从设备读回。

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
* 布局效应 = banked-serial − original-serial；
  绕过效应 = banked-hit − banked-serial；
  original 与 banked-hit 的直接差值不用于归因绕过。
* 发布前的技术资格会汇合**该 case 的全部质量 issue**（含 a_jobs / exit_txt /
  metrics / helper 交叉核对）以及该 label 的 plan / boot / flash / caps /
  summary 必要质量问题；任一存在即该 case（及受影响 label）技术上不合格，
  compute_effects 不发布。合法偏低或波动样本不记为 quality failure，不剔除。
* 直接按 runner validate_record() 的源逐实际字段复核 record.schema /
  success / error / result.status / victim|aggressor.data_errors /
  quarantined，以及 p4m.bank_magic / observer_status / raw_window 与
  raw_observation 的一致性；不导入 runner、不写设备、不重算哈希。
* helper 窗口是 ``idma-nonblock-window-v1``，16 字，计数区间为 (open, close]；
  序号一律取 ``words[4]``；快速路径事件数不是命中率，也不是全部 miss 数。
* 旧 HPM / COST / E3 数据不是 N 机制证据。

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
                          expected_cycles: Optional[int]) -> List[str]:
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

    if expected_mode == 1:
        fast_words = [words[i] for i in HELPER_FAST_SLICE]
        if any(value != 0 for value in fast_words):
            problems.append('Mode1 (banked-serial) 快速计数必须全 0，实际 %r' % (fast_words,))
    return problems


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
                 case_dir: pathlib.Path) -> Dict[str, Any]:
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
                helper_entry['problems'].extend(validate_helper_words(
                    label, w, entry['v_window_cycles']))

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

    helper_ok = None
    if helper_entry['words'] is not None:
        helper_ok = not helper_entry['problems']
    elif helper_expected:
        helper_ok = False
    entry['helper_ok'] = helper_ok

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
    entry['qualified'] = not reasons
    entry['qualified_reasons'] = reasons

    return entry


# ---------------------------------------------------------------------------
# 单个 label
# ---------------------------------------------------------------------------

def finalize_label_qualification(an: Analysis, label: str,
                                 info: Dict[str, Any]) -> None:
    '''把该 label 的全部质量 issue 折进每个 case 的技术资格与 label 汇总。

    该 case 的任何质量 issue（含 a_jobs/exit_txt/metrics/helper 交叉核对）或
    该 label 的 plan/boot/flash/caps/summary 等必要问题，都使受影响 case 技术上
    不合格；合法偏低/波动样本不产生 issue，仍保留并参与汇总。
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
                    plan: List[Dict[str, Any]]) -> Dict[str, Any]:
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
                                              directory / 'cases' / spec['id']))
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
            an, label, spec, directory / 'cases' / spec['id']))

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
        'note': '短组只做验收（3 例），不与长组混合，不参与布局/绕过效应比较，不做显著性。'
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
                'V 吞吐与同窗口 A 吞吐分列，不用不同窗口的 A 完成数比较 A 速度。',
        'series_keys': list(series_keys),
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
        'layout_original_to_serial': _effect_pair(None, None),
        'bypass_serial_to_hit': _effect_pair(None, None),
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
                section['layout_original_to_serial'] = _effect_pair(
                    medians['original-serial'], medians['banked-serial'])
                section['bypass_serial_to_hit'] = _effect_pair(
                    medians['banked-serial'], medians['banked-hit'])
        per_condition[condition] = {
            'published': not reasons,
            'reasons': reasons,
            'v_throughput_mib_s': v_section,
            'a_throughput_mib_s': a_section,
        }
    return {
        'definition': {
            'layout': 'banked-serial − original-serial（同串行行为、不同银行布局）：'
                      'original -> bankedSerial',
            'bypass': 'banked-hit − banked-serial（同布局，仅运行时使能绕过）：'
                      'bankedSerial -> bankedHit',
        },
        'attribution_warning': (
            '不允许用 original-serial 与 banked-hit 的直接差值归因绕过效应；'
            '绕过效应必须以 banked-serial 为中介。'),
        'metric_note': (
            'V 吞吐与同窗口 A 吞吐均以 raw_window[1] 为窗口、50 MHz 换算（推导量）；'
            'A 速度只用同窗口 A 吞吐评价，不用不同窗口 A 完成数直接比较。'),
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
    add('- 分析脚本：`scripts/analyze_nonblock_pilot.py`（离线汇总 proposal，未在真实数据上验证）')
    add('- 汇总 JSON：`%s`' % RESULT_JSON_NAME)
    add('- 数据状态：**%s**（脚本退出码 = %d）' % (state_text, doc['exit_code']))
    add('- 时钟换算假设：%.0f MHz（1 cycle = %.1f ns，仅用于时间/吞吐推导）'
        % (CLOCK_HZ / 1e6, 1e9 / CLOCK_HZ))
    add('- 真实 V 固定工作量：short = %d B，long = %d B（= transfers × %d）'
        % (SHORT_TRANSFERS * CHUNK_BYTES, LONG_TRANSFERS * CHUNK_BYTES, CHUNK_BYTES))
    add()
    add('> 本文件由脚本按固定模板生成，是描述性汇总，不是实验结论声明；')
    add('> 不含显著性统计，不含 novelty 判断，不对未运行/失败项做推测或补值，')
    add('> 不修改任何原始文件。')
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
            detail = str(item['detail']).replace('|', '\\|')
            add('| %d | %s | %s | %s | %s |' % (
                index, item['scope'] or '—', item['case'] or '—',
                item['code'], detail[:400]))
    add()
    if doc['observations']:
        add('### 3.1 观察（不改变退出码）')
        add()
        add('| # | scope | case | code | detail |')
        add('|---|---|---|---|---|')
        for index, item in enumerate(doc['observations'], start=1):
            detail = str(item['detail']).replace('|', '\\|')
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

    # ---- 6. 效应分解 -----------------------------------------------------
    add('## 6. 布局效应与绕过效应分解（仅 long 组）')
    add()
    add('- 布局效应：`%s`' % doc['effects']['definition']['layout'])
    add('- 绕过效应：`%s`' % doc['effects']['definition']['bypass'])
    add('- **%s**' % doc['effects']['attribution_warning'])
    add('- %s' % doc['effects']['metric_note'])
    add('- 发布规则：%s' % doc['effects']['publish_rule'])
    add('- %s' % doc['effects']['statistics'])
    add()

    def effect_table(kind: str, heading: str) -> None:
        add('### %s' % heading)
        add()
        add('| condition | published | orig 中位 | serial 中位 | hit 中位 |'
            ' 布局 Δ | 布局 Δ% | 绕过 Δ | 绕过 Δ% |')
        add('|---|---|---|---|---|---|---|---|---|')
        for condition in LONG_CONDITIONS:
            item = doc['effects']['per_condition'][condition]
            section = item[kind]
            layout = section['layout_original_to_serial']
            bypass = section['bypass_serial_to_hit']
            add('| %s | %s | %s | %s | %s | %s | %s | %s | %s |' % (
                condition, '是' if item['published'] else '否',
                fmt_num(section['original_median']),
                fmt_num(section['serial_median']),
                fmt_num(section['hit_median']),
                fmt_num(layout['delta']), fmt_pct(layout['delta_pct']),
                fmt_num(bypass['delta']), fmt_pct(bypass['delta_pct']),
            ))
        add()
        for condition in LONG_CONDITIONS:
            item = doc['effects']['per_condition'][condition]
            if not item['published']:
                add('- 条件 %s 未发布：%s'
                    % (condition, '；'.join(item['reasons']) or '数据不合格'))
        add()

    effect_table('v_throughput_mib_s', '6.1 V 吞吐（MiB/s）收益 / 代价')
    effect_table('a_throughput_mib_s', '6.2 同窗口 A 吞吐（MiB/s）收益 / 代价')

    # ---- 7. 原始字段证据 -------------------------------------------------
    add('## 7. 原始字段证据')
    add()
    add('### 7.1 p4m.raw_window 逐 case（16 字，原样，未做任何解释性替换）')
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
    add('### 7.2 A 窗口内完成数的取值来源（逐 case）')
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
    add('### 7.3 helper 窗口摘要（完整 16 字见 JSON `cases[*].helper.words`）')
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
    add('### 7.4 p4m.raw_observation')
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
    add('> 旧 HPM / COST / E3 数据 **不作为 N 机制证据**；helper 的快速路径事件数')
    add('> **不是命中率，也不是全部 miss 数**。')
    add()

    # ---- 8. 不确定性 -----------------------------------------------------
    add('## 8. 不确定性、未运行项与未做的事')
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
        '合格时，不发布该条件的性能汇总与两段效应，也不把残缺数值与有效样本混合，'
        '更不剔除不利但合法的合格样本。',
        '每个 label 只对应一次运行、一次新鲜启动；跨 label 的差异同时包含运行间波动，'
        '本脚本不做方差分析。',
        'banked-hit 相对 banked-serial 的差值才是绕过效应；'
        'original-serial 与 banked-hit 的直接差值不用于绕过归因。',
        'helper 窗口语义为 (open, close]；序号一律取 `words[4]`（reader emit_json 的 '
        '`windowseq` 亦等于 `words[4]`，仅在存在时做交叉核对）；脚本只校验 ABI 冻结位、'
        '计数守恒、与 P4M 相同的 cycles、Mode1 快速项全 0、Mode2 可大于 0、序号严格递增。',
        '只按冻结计划的 15 个 case id 打开确切路径，未枚举目录；'
        '因此计划外的额外 case 目录不会被发现。',
        '技术资格（qualified）已汇合该 case 的全部质量 issue 与该 label 的 '
        'plan/boot/flash/caps/summary 必要质量问题；合法偏低或波动样本不产生 '
        'issue，仍保留并参与汇总，但任一真实 issue 都会阻止效应发布。',
        '不做哈希校验、不从设备读回、不重跑、不修改任何原始文件。',
        '旧 HPM / COST / E3 数据不是本设计的机制证据；'
        'helper 快速路径事件数不是命中率，也不是全部 miss 数。',
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
        labels[label] = summarize_label(an, label, inputs[label], plan)

    completeness = compute_completeness(labels, plan)
    short_summary = compute_short(labels)
    long_summary = compute_long(labels)
    effects = compute_effects(long_summary)

    quality_ok = not an.quality
    exit_code = 0 if (completeness['all_cases_complete'] and quality_ok) else 1

    analysis_doc: Dict[str, Any] = {
        'schema': ANALYSIS_SCHEMA,
        'generated_at': utc_now(),
        'analyzer': {
            'script': 'scripts/analyze_nonblock_pilot.py',
            'status': 'proposal, 未在真实数据上执行',
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
        },
        'frozen_plan': plan,
        'inputs': {label: labels[label] for label in LABELS},
        'cases': {label: labels[label]['cases'] for label in LABELS},
        'completeness': completeness,
        'short_acceptance': short_summary,
        'long_per_condition': long_summary,
        'effects': effects,
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
            '不剔除异常、不自动重跑、不做显著性统计。',
            '旧 HPM / COST / E3 不作 N 机制证据。',
            '快速路径事件数不是命中率，也不是全部 miss 数。',
        ],
    }

    write_atomic(out / RESULT_JSON_NAME,
                 json.dumps(analysis_doc, ensure_ascii=False, indent=2,
                            sort_keys=False) + '\n')
    write_atomic(out / RESULT_MD_NAME, render_markdown(analysis_doc))

    print('written: %s' % (out / RESULT_JSON_NAME).as_posix())
    print('written: %s' % (out / RESULT_MD_NAME).as_posix())
    print('complete=%s quality_issues=%d exit=%d'
          % (completeness['all_cases_complete'], len(an.quality), exit_code))
    return exit_code


if __name__ == '__main__':
    sys.exit(main())

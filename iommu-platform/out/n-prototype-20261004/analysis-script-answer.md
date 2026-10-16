下面是三个交付物：完整脚本 proposal、脚本会生成的 `RESULTS.md` 中文模板、以及 `docs/nonblock-pilot-plan-20261004.md` 的简短中文补充。脚本未执行过，也没有假设实验成功。

---

# 1. `scripts/analyze_nonblock_pilot.py`（完整代码）

```python
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
* 50 MHz 只用于把周期换算成时间和吞吐（推导量，不是直接测量）。
* A 的窗口内完成数只用记录里窗口内的字段，绝不用整轮 A 计数代替。
* 短组只做验收，不与长组混合；长组每条件列出每次值 + 中位/min/max，不做显著性。
* 布局效应 = banked-serial − original-serial；
  绕过效应 = banked-hit − banked-serial；
  original 与 banked-hit 的直接差值不用于归因绕过。
* helper 窗口是 ``idma-nonblock-window-v1``，16 字，计数区间为 (open, close]；
  快速路径事件数不是命中率，也不是全部 miss 数。
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
# 其它下标（包括"窗口内 A 完成数"）在给定的 runner 里没有被钉死。因此本脚本：
#   * 一律把 raw_window 16 字原样存进 JSON 并列进 RESULTS.md，作为可人工定位的证据；
#   * A 的窗口内完成数优先取 p4m.raw_window[RAW_WINDOW_A_JOBS_INDEX]，
#     该常量为 None 时退回 runner 自己命名的 p4m["a_completed_jobs_window"]，
#     并在每条记录里标出取值来源，绝不使用整轮 A 计数代替。
P4M_BANK_MAGIC = 0x49444D4150344D01
P4M_WINDOW_WORDS = 16
RAW_WINDOW_MAGIC_INDEX = 0
RAW_WINDOW_CYCLES_INDEX = 1
RAW_WINDOW_V_ISSUED_INDEX = 13
RAW_WINDOW_V_DONE_INDEX = 14
RAW_WINDOW_A_JOBS_INDEX: Optional[int] = None   # 待 record 写入方确认后再填 int

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
        'raw_window': None,
        'raw_window_ok': None,
        'raw_observation_len': None,
        'raw_observation': None,
        'p4m_has_a': None,
        'helper': None,
        'helper_ok': None,
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
        candidate = record_doc.get('p4m')
        if isinstance(candidate, dict):
            p4m = candidate
        else:
            an.q(label, case_id, 'record_p4m_missing', 'record.p4m 不是对象')

    if p4m is not None:
        entry['p4m_has_a'] = p4m.get('has_a')

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
            seconds = cycles / float(CLOCK_HZ)
            entry['v_window_ns'] = seconds * 1e9
            entry['v_throughput_mib_s'] = (
                (spec['transfers'] * CHUNK_BYTES / (1024.0 * 1024.0)) / seconds)

        # --- A 的窗口内完成数 ---------------------------------------------
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
                 'record.json 里没有窗口内 A 完成数；raw_window 下标亦未确认，'
                 '不退回整轮 A 计数')
        elif spec['condition'] == 'Solo' and a_value != 0:
            an.o(label, case_id, 'solo_a_jobs_nonzero',
                 'Solo 用例的窗口内 A 完成数 = %d（字段语义未独立确认，仅作观察）'
                 % a_value)

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
        'windowseq': None,
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
            helper_entry['windowseq'] = int_or_none(helper_doc.get('windowseq'))
            helper_entry['cycles'] = int_or_none(helper_doc.get('cycles'))
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

    return entry


# ---------------------------------------------------------------------------
# 单个 label
# ---------------------------------------------------------------------------

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
    }

    if not info['exists']:
        an.q(label, None, 'input_dir_missing',
             'runner 输出目录不存在: ' + directory.as_posix())
        for spec in plan:
            info['cases'].append(analyze_case(an, label, spec,
                                              directory / 'cases' / spec['id']))
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
    if err is None and isinstance(boot, dict):
        info['boot'] = {
            'uname_r': boot.get('uname_r'),
            'boot_id': boot.get('boot_id'),
            'taint': boot.get('taint'),
            'boot_id_before': boot.get('boot_id_before'),
        }

    flash, err = load_json_file(directory / 'flash.json')
    if err is None and isinstance(flash, dict):
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

    caps, err = load_json_file(directory / 'caps.json')
    info['caps_present'] = isinstance(caps, dict)

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

    # --- helper 序号在计划顺序上必须严格递增（冻结 ABI 的 (open, close] 证据）---
    sequences: List[Tuple[int, str, int]] = []
    for case_entry in info['cases']:
        helper = case_entry.get('helper') or {}
        seq = helper.get('windowseq')
        if isinstance(seq, int) and not isinstance(seq, bool):
            sequences.append((case_entry['order'], case_entry['id'], seq))
    sequences.sort()
    previous: Optional[Tuple[int, str, int]] = None
    for current in sequences:
        if previous is not None and current[2] <= previous[2]:
            an.q(label, current[1], 'helper_sequence',
                 'helper windowseq %d 未超过前一有效窗口 %s 的 %d'
                 % (current[2], previous[1], previous[2]))
        previous = current

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
                'a_jobs_field': case_entry['a_jobs_field'],
                'raw_window_ok': case_entry['raw_window_ok'],
                'helper_ok': case_entry['helper_ok'],
            })
        rows.sort(key=lambda item: item['id'])
        per_label[label] = rows
    return {
        'note': '短组只做验收（3 例），不与长组混合，不参与布局/绕过效应比较，不做显著性。',
        'per_label': per_label,
    }


def compute_long(labels: Dict[str, Dict[str, Any]]) -> Dict[str, Any]:
    per_condition: Dict[str, Any] = {}
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
                    'cycles': case_entry['v_window_cycles'],
                    'a_jobs': case_entry['a_jobs_in_window'],
                })
            rounds.sort(key=lambda item: (item['round'] is None, item['round']))
            cycles = [r['cycles'] for r in rounds if isinstance(r['cycles'], int)]
            a_jobs = [r['a_jobs'] for r in rounds if isinstance(r['a_jobs'], int)]
            by_label[label] = {
                'rounds': rounds,
                'cycles_values': cycles,
                'cycles_median': median_or_none(cycles),
                'cycles_min': min(cycles) if cycles else None,
                'cycles_max': max(cycles) if cycles else None,
                'a_jobs_values': a_jobs,
                'a_jobs_median': median_or_none(a_jobs),
                'a_jobs_min': min(a_jobs) if a_jobs else None,
                'a_jobs_max': max(a_jobs) if a_jobs else None,
            }
        per_condition[condition] = by_label
    return {
        'note': 'long 组每条件给出三轮每次值 + 中位/min/max（描述性，不做显著性）。'
                '第 2 轮顺序为反向，请按轮次看，不要只看中位。',
        'per_condition': per_condition,
    }


def compute_effects(long_summary: Dict[str, Any]) -> Dict[str, Any]:
    per_condition: Dict[str, Any] = {}
    for condition, by_label in long_summary['per_condition'].items():
        original = by_label['original-serial']
        serial = by_label['banked-serial']
        hit = by_label['banked-hit']
        per_condition[condition] = {
            'cycles': {
                'original_median': original['cycles_median'],
                'serial_median': serial['cycles_median'],
                'hit_median': hit['cycles_median'],
                'layout_serial_minus_original':
                    delta(original['cycles_median'], serial['cycles_median']),
                'layout_pct': delta_pct(original['cycles_median'], serial['cycles_median']),
                'bypass_hit_minus_serial':
                    delta(serial['cycles_median'], hit['cycles_median']),
                'bypass_pct': delta_pct(serial['cycles_median'], hit['cycles_median']),
            },
            'a_jobs': {
                'original_median': original['a_jobs_median'],
                'serial_median': serial['a_jobs_median'],
                'hit_median': hit['a_jobs_median'],
                'layout_serial_minus_original':
                    delta(original['a_jobs_median'], serial['a_jobs_median']),
                'layout_pct': delta_pct(original['a_jobs_median'], serial['a_jobs_median']),
                'bypass_hit_minus_serial':
                    delta(serial['a_jobs_median'], hit['a_jobs_median']),
                'bypass_pct': delta_pct(serial['a_jobs_median'], hit['a_jobs_median']),
            },
        }
    return {
        'definition': {
            'layout': 'banked-serial − original-serial（同一串行行为、不同银行布局）',
            'bypass': 'banked-hit − banked-serial（同布局，仅运行时使能绕过）',
        },
        'attribution_warning': (
            '不允许用 original-serial 与 banked-hit 的直接差值归因绕过效应；'
            '绕过效应必须以 banked-serial 为中介。'),
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
    add('| label | case | condition | state | cycles | ns | MiB/s | A jobs | A 字段来源 | raw_window 一致 | helper 一致 |')
    add('|---|---|---|---|---|---|---|---|---|---|---|')
    for label in LABELS:
        for row in doc['short_acceptance']['per_label'][label]:
            add('| %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s |' % (
                label, row['id'], row['condition'], row['state'],
                fmt_num(row['cycles']), fmt_float(row['ns'], 1),
                fmt_float(row['mib_s'], 2), fmt_num(row['a_jobs']),
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
    for condition in LONG_CONDITIONS:
        by_label = doc['long_per_condition']['per_condition'][condition]
        add('### 5.%s 条件 %s — 窗口周期数（raw_window[1]）'
            % (LONG_CONDITIONS.index(condition) + 1, condition))
        add()
        add('| label | %s | %s | %s | 中位 | min | max | n |'
            % tuple('r%d' % i for i in (1, 2, 3)))
        add('|---|---|---|---|---|---|---|---|')
        for label in LABELS:
            item = by_label[label]
            by_round = {r['round']: r['cycles'] for r in item['rounds']}
            add('| %s | %s | %s | %s | %s | %s | %s | %d |' % (
                label,
                fmt_num(by_round.get(1)), fmt_num(by_round.get(2)),
                fmt_num(by_round.get(3)),
                fmt_num(item['cycles_median']), fmt_num(item['cycles_min']),
                fmt_num(item['cycles_max']), len(item['cycles_values'])))
        add()
        add('| label | %s | %s | %s | 中位 | min | max | n |'
            % tuple('r%d' % i for i in (1, 2, 3)))
        add('|---|---|---|---|---|---|---|---|')
        for label in LABELS:
            item = by_label[label]
            by_round = {r['round']: r['a_jobs'] for r in item['rounds']}
            add('| %s | %s | %s | %s | %s | %s | %s | %d |' % (
                label,
                fmt_num(by_round.get(1)), fmt_num(by_round.get(2)),
                fmt_num(by_round.get(3)),
                fmt_num(item['a_jobs_median']), fmt_num(item['a_jobs_min']),
                fmt_num(item['a_jobs_max']), len(item['a_jobs_values'])))
        add()

    # ---- 6. 效应分解 -----------------------------------------------------
    add('## 6. 布局效应与绕过效应分解（仅 long 组）')
    add()
    add('- 布局效应：`%s`' % doc['effects']['definition']['layout'])
    add('- 绕过效应：`%s`' % doc['effects']['definition']['bypass'])
    add('- **%s**' % doc['effects']['attribution_warning'])
    add('- %s' % doc['effects']['statistics'])
    add()
    add('| condition | original 中位 | serial 中位 | hit 中位 | 布局 Δ | 布局 Δ% | 绕过 Δ | 绕过 Δ% |')
    add('|---|---|---|---|---|---|---|---|')
    for condition in LONG_CONDITIONS:
        item = doc['effects']['per_condition'][condition]['cycles']
        add('| %s | %s | %s | %s | %s | %s | %s | %s |' % (
            condition,
            fmt_num(item['original_median']), fmt_num(item['serial_median']),
            fmt_num(item['hit_median']),
            fmt_num(item['layout_serial_minus_original']),
            fmt_pct(item['layout_pct']),
            fmt_num(item['bypass_hit_minus_serial']),
            fmt_pct(item['bypass_pct']),
        ))
    add()
    add('上表为窗口周期数。窗口内 A 完成数的同一分解：')
    add()
    add('| condition | original 中位 | serial 中位 | hit 中位 | 布局 Δ | 布局 Δ% | 绕过 Δ | 绕过 Δ% |')
    add('|---|---|---|---|---|---|---|---|')
    for condition in LONG_CONDITIONS:
        item = doc['effects']['per_condition'][condition]['a_jobs']
        add('| %s | %s | %s | %s | %s | %s | %s | %s |' % (
            condition,
            fmt_num(item['original_median']), fmt_num(item['serial_median']),
            fmt_num(item['hit_median']),
            fmt_num(item['layout_serial_minus_original']),
            fmt_pct(item['layout_pct']),
            fmt_num(item['bypass_hit_minus_serial']),
            fmt_pct(item['bypass_pct']),
        ))
    add()

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
    add('字段定位（哪些下标由 runner 固定，哪些未确认）：')
    add()
    add('- 由 `scripts/mac_nonblock_pilot.py` 的 `validate_record()` 钉死：'
        '`w0` = bank magic，`w1` = 窗口周期数（1 字 = 1 cycle，== `raw_observation[0]`），'
        '`w13` = V issued，`w14` = V done（后三者应等于该 case 的 transfers）。')
    add('- `w2..w12`、`w15` 未由 runner 固定；"窗口内 A 完成数" 落在哪一位需要'
        '与 record 写入方确认，本脚本不做猜测：')
    add('  - 若已确认，请把 `scripts/analyze_nonblock_pilot.py` 的 '
        '`RAW_WINDOW_A_JOBS_INDEX` 从 `None` 改为该下标；')
    add('  - 未确认时，A 列取值来自 runner 自己命名的 '
        '`record.p4m.a_completed_jobs_window`，并在每条记录里标注来源，'
        '**不使用整轮 A 计数代替**。')
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
    add('| label | case | mode | windowseq | cycles | flags | errors | Σfast | Σnormal | accepted | 来源 |')
    add('|---|---|---|---|---|---|---|---|---|---|---|')
    for label in LABELS:
        for case_entry in doc['cases'][label]:
            helper = case_entry['helper'] or {}
            if not helper.get('present'):
                continue
            add('| %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s |' % (
                label, case_entry['id'],
                fmt_num(helper.get('mode')), fmt_num(helper.get('windowseq')),
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
        '`p4m.raw_window` 中"窗口内 A 完成数"的下标没有被 '
        '`scripts/mac_nonblock_pilot.py` 的 `validate_record()` 固定；'
        '本脚本在 `RAW_WINDOW_A_JOBS_INDEX = None` 时改用 runner 命名的 '
        '`record.p4m.a_completed_jobs_window`，并在每条记录里标注来源。',
        '窗口周期数取自 `raw_window[1]`，单位按 1 字 = 1 cycle 处理；'
        'ns 与 MiB/s 是以 50 MHz 为假设的推导量，不是直接测量值。',
        '真实 V 工作量固定为 transfers × 4096 B；脚本按此计算，不重新测数据量。',
        '短组只用做验收，不与长组混合；长组每条件三轮值 + 中位/min/max 均为描述性，'
        '不做显著性检验，不能据此外推。',
        '每个 label 只对应一次运行、一次新鲜启动；跨 label 的差异同时包含运行间波动，'
        '本脚本不做方差分析。',
        'banked-hit 相对 banked-serial 的差值才是绕过效应；'
        'original-serial 与 banked-hit 的直接差值不用于绕过归因。',
        'helper 窗口语义为 (open, close]；脚本只校验 ABI 冻结位、计数守恒、'
        '与 P4M 相同的 cycles、Mode1 快速项全 0、Mode2 可大于 0、序号在计划顺序上严格递增。',
        '只按冻结计划的 15 个 case id 打开确切路径，未枚举目录；'
        '因此计划外的额外 case 目录不会被发现。',
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
                    'w13': 'V issued (== transfers)',
                    'w14': 'V done (== transfers)',
                },
                'a_jobs_index': RAW_WINDOW_A_JOBS_INDEX,
                'a_jobs_note': 'NULL 表示未确认；此时 A 值来自 '
                               'record.p4m.a_completed_jobs_window，绝不退回整轮 A 计数',
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
```

---

# 2. 有数据后使用的 `RESULTS.md` 中文模板

脚本会按下面的固定骨架生成 `RESULTS.md`；下面是同一模板（`{…}` 为脚本填入处）。人工写报告时可直接沿用这些小节与措辞边界。

```markdown
# N 非阻塞工程 pilot 离线汇总（RESULTS.md）

- 生成时间：{UTC 时间}
- 分析脚本：`scripts/analyze_nonblock_pilot.py`（离线汇总 proposal，未在真实数据上验证）
- 汇总 JSON：`nonblock-pilot-analysis.json`
- 数据状态：**{完整 | 不完整 / 存在问题}**（脚本退出码 = {0 | 1}）
- 时钟换算假设：50 MHz（1 cycle = 20.0 ns，仅用于时间/吞吐推导）
- 真实 V 固定工作量：short = 262144 B，long = 16777216 B（= transfers × 4096）

> 本文件由脚本按固定模板生成，是描述性汇总，不是实验结论声明；
> 不含显著性统计，不含 novelty 判断，不对未运行/失败项做推测或补值，
> 不修改任何原始文件。

## 1. 数据来源与运行身份
| label | 目录 | 存在 | run_id | BIT | device | summary.status | STOPPED |
|---|---|---|---|---|---|---|---|
| original-serial | `{路径}` | {是/否} | {run_id} | `{bit}` | {device} | {complete/…} | {有/无} |
| banked-serial   | `{路径}` | … | … | … | … | … | … |
| banked-hit      | `{路径}` | … | … | … | … | … | … |

## 2. 完整度矩阵（每 label 冻结 15 例，三 label 共 45 例）
| label | 完成 | 失败 | 不完整 | 未运行 | 合计 |
|---|---|---|---|---|---|
| original-serial | {n} | {n} | {n} | {n} | 15 |
| banked-serial   | … | … | … | … | 15 |
| banked-hit      | … | … | … | … | 15 |

| order | case | group | condition | round | state | cycles | A jobs |
|---|---|---|---|---|---|---|---|
| 1 | original-serial / short-solo | short | Solo | — | {state} | {n} | {n} |
| … | （共 45 行） | | | | | | |

## 3. 质量问题清单（不剔除、不修补、不重跑）
| # | scope | case | code | detail |
|---|---|---|---|---|
| 1 | {label} | {case 或 —} | {code} | {detail} |

### 3.1 观察（不改变退出码）
（如 Solo 用例的窗口内 A 完成数非 0；仅作观察，字段语义未独立确认）

### 3.2 runner 记录的 case problems（原样保留）
- `{label}/{case}`: {problems}

## 4. 短组验收（3 例，只做验收，不与长组混合）
> 短组只做验收（3 例），不与长组混合，不参与布局/绕过效应比较，不做显著性。

| label | case | condition | state | cycles | ns | MiB/s | A jobs | A 字段来源 | raw_window 一致 | helper 一致 |
|---|---|---|---|---|---|---|---|---|---|---|
| {label} | {id} | {Solo/A1/A8} | {state} | {n} | {n} | {n} | {n} | {字段来源} | {是/否/—} | {是/否/不适用} |

## 5. 长组：每条件每次值 + 中位/min/max
> long 组每条件给出三轮每次值 + 中位/min/max（描述性，不做显著性）。
> 第 2 轮顺序为反向，请按轮次看，不要只看中位。

### 5.1 条件 Solo — 窗口周期数（raw_window[1]）
| label | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|
| {label} | {n} | {n} | {n} | {n} | {n} | {n} | {0–3} |

（同表再列一次"窗口内 A 完成数"；A1 / A8 / A16 同样各两张表）

## 6. 布局效应与绕过效应分解（仅 long 组）
- 布局效应：`banked-serial − original-serial`（同一串行行为、不同银行布局）
- 绕过效应：`banked-hit − banked-serial`（同布局，仅运行时使能绕过）
- **不允许用 original-serial 与 banked-hit 的直接差值归因绕过效应；绕过效应必须以 banked-serial 为中介。**
- 仅描述性中位/min/max，不做显著性检验。

| condition | original 中位 | serial 中位 | hit 中位 | 布局 Δ | 布局 Δ% | 绕过 Δ | 绕过 Δ% |
|---|---|---|---|---|---|---|---|
| Solo | {n} | {n} | {n} | {n} | {%} | {n} | {%} |
| A1 / A8 / A16 | … | … | … | … | … | … | … |

（同表再列一次"窗口内 A 完成数"的分解）

## 7. 原始字段证据
### 7.1 p4m.raw_window 逐 case（16 字，原样）
| label | case | w0 | w1 | w2 | … | w15 |
|---|---|---|---|---|---|---|
| {label} | {case} | … | … | … | … | … |

字段定位：
- 由 runner 的 `validate_record()` 钉死：`w0` = bank magic，`w1` = 窗口周期数（1 字 = 1 cycle），`w13` = V issued，`w14` = V done。
- `w2..w12`、`w15` 未固定；"窗口内 A 完成数"落在哪一位需与 record 写入方确认。
- 已确认时把 `RAW_WINDOW_A_JOBS_INDEX` 从 `None` 改为该下标；未确认时 A 值来自 `record.p4m.a_completed_jobs_window` 并标注来源，**不使用整轮 A 计数代替**。

### 7.2 A 窗口内完成数的取值来源（逐 case，仅 long）
### 7.3 helper 窗口摘要（完整 16 字见 JSON `cases[*].helper.words`）
| label | case | mode | windowseq | cycles | flags | errors | Σfast | Σnormal | accepted |
|---|---|---|---|---|---|---|---|---|---|
| {label} | {case} | {1/2} | {n} | {n} | 4 | 0 | {n} | {n} | {n} |

### 7.4 p4m.raw_observation
> 旧 HPM / COST / E3 数据 **不作为 N 机制证据**；helper 的快速路径事件数
> **不是命中率，也不是全部 miss 数**。

## 8. 不确定性、未运行项与未做的事
- （逐条列出脚本 `uncertainties` 字段的内容）
- 未运行 / 失败 / 不完整的用例已逐条列在第 2 节与第 3 节，不做补值、不做剔除、不自动重跑。
```

---

# 3. `docs/nonblock-pilot-plan-20261004.md` 的简短中文补充

建议追加在该文件末尾（或第 8 节之后）：

```markdown
## 离线汇总与报告约定（2026-10-04 补充，方案；非实验结果）

- 汇总脚本：`scripts/analyze_nonblock_pilot.py`（本稿为 proposal，尚未对真实数据运行）。
  调用形式：`--original DIR --serial DIR --hit DIR --out DIR`；只读三个 runner 输出目录，
  只写 `--out`，输出唯一 JSON 汇总 `nonblock-pilot-analysis.json` 与中文 `RESULTS.md`。
- 每个 label 15 例（短组 3 + 长组 12），三 label 合计 45 例。
- 数据不完整时仍照常出汇总，但退出码明确非 0；未运行 / 失败 / 不完整的 case 逐条列出，
  不剔除、不修补、不自动重跑、不修改任何原始文件。
- 只按冻结计划里的 15 个 case id 打开确切路径，不做目录全扫描、不重新计算哈希、不从设备读回。
- 指标只取 `record.json` 的 `p4m.raw_window`：`w1` 为窗口周期数（1 字 = 1 cycle）；
  真实 V 固定工作量 = transfers × 4096 B；50 MHz 仅用于把周期换算成时间/吞吐，属推导量。
- A 的"窗口内完成数"必须用记录里窗口内的字段（脚本会标注取值来源与本字段下标是否已确认），
  **不用整轮 A 计数代替**。
- 短组只做验收，不与长组混合、不参与比较；长组每条件列出三轮每次值 + 中位/min/max，
  仅描述性，不做显著性检验。
- 效应分解只做两段：`banked-serial − original-serial` 记为布局效应，
  `banked-hit − banked-serial` 记为绕过效应；
  `original-serial` 与 `banked-hit` 的直接差值不用于归因绕过。
- helper 窗口（ABI `idma-nonblock-window-v1`，16 字）校验：Mode1 快速项全 0、Mode2 可大于 0、
  `all = Σfast + Σnormal`、`cycles` 与 P4M 相同、flags 冻结 4 且 errors 0、
  计数区间为 (open, close]。快速路径事件数不是命中率，也不是全部 miss 数。
- 旧 HPM / COST / E3 数据不作为 N 机制证据。所有数值只来自一次性、每 label 一次新鲜启动的运行，
  不做外推。

当前状态（不冒充结果）：仿真通过；两个 BIT 正在编译；原版 15 例正在上板执行；
两个新版本尚未板测。因此本补充不含任何性能或机制结论，也不含 novelty 判断。
```

---

# 假设、边界与待 Codex 确认项

1. **`RAW_WINDOW_A_JOBS_INDEX = None`**：给定 runner 的 `validate_record()`（`mac_nonblock_pilot.py:362-373`）只钉死 `w0/w1/w13/w14`，没有钉死 "窗口内 A 完成数" 的下标。脚本因此在 `None` 时退回 runner 自己命名的 `record.p4m.a_completed_jobs_window`（`mac_nonblock_pilot.py:392`），逐条标注来源，并把 16 字原样列入报告便于人工定位。**请 Codex 用 record 写入方源码确认下标后改为 int**，那时代码会自动改用 `raw_window[i]` 并与 runner 字段交叉核对。
2. **helper 计数守恒写法**：按 `bench/idma_nonblock_read.c:149-166` 与 runner 的 `validate_helper_window`（`mac_nonblock_pilot.py:426-430`），守恒为 `words[14] == Σwords[6..10] + Σwords[11..13]`；脚本按此校验，并额外要求 Mode1 的 `words[6..10]` 全 0。
3. **`helper.exit.txt` / `exit.txt` 交叉核对**：这两个文件由 runner 导出（`mac_nonblock_pilot.py:868-869`、`:1006-1007`），脚本只读不写；缺失时记为质量问题而不会中止汇总。
4. **不做的事**：不枚举目录（计划外 case 目录不会被发现，已在不确定性中写明）、不哈希、不读回设备、不合并短长、不做统计检验、不写镜像/JTAG/串口逻辑。
5. **建议验收检查**（需有数据后由 Codex 执行）：
   - `--out` 与三个输入目录重叠时脚本必须拒绝退出（码 2）；
   - 三个目录都缺失时脚本仍写出 JSON + `RESULTS.md` 且退出码 1，45 条 case 全部标为 `not_run`；
   - 只放 `original-serial` 一个目录时，另外两 label 判为 `not_run`，退出码 1；
   - 人为把某个 `record.json` 的 `raw_window` 截短，脚本报 `raw_window_shape` 且退出码 1，但其余 case 仍照常汇总；
   - 运行后核对三个输入目录的 mtime/内容未变（脚本只做只读打开）。
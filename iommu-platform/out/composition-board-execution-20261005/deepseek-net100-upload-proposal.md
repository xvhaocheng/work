以下为可直接保存运行的单文件脚本（Mac 主机侧，复用你提供的 `SerialConsole`）。先给出关键说明，随后是唯一一个 python 代码块。

说明与假设

- 复用方式：按路径 `importlib` 加载 `root/tools/composition-v1-20261004/mac_research2.py`，只用其中的 `SerialConsole`（含其随机 marker 分帧、raw/events 日志），不引入任何新框架、不新增登录/密码逻辑。
- 前置：串口在 Mac 端由 `lsof /dev/cu.usbserial-AV0KRCHT` 判空（有行即占用则停止）；本地先校验 3 个输入存在（`mac_research2.py`、`.ko` 存在且大小 ==31944，可用 `EXPECTED_KO_SIZE=None` 关闭该本地尺寸检查，这属于本机预检，不是板上回读/哈希）。
- 上传：`base64` 文本分块 `printf %s '<chunk>' >> /tmp/net100.b64`，块长 960，最大命令长度 991 < 1024（`shell()` 内对每条 `run_shell` 命令强制断言 ≤1024）；最后 `base64 -d /tmp/net100.b64 > /tmp/lowrisc_net100.ko`。不做板上哈希/回读/独立测试。
- 顺序：`uname -r` 必须等于 `6.5.0-rc2-cpn2-net1-00093-gc0f5e13ea81f-dirty` → 身份检查（`/sys/bus/platform/devices/30000000.lowrisc-eth/driver` 为 `lowrisc-eth`，eth0 存在且 owner 为该平台设备）→ 上传 → `insmod`（此时旧 driver 仍绑定）→ 新 driver 目录存在检查 → `ip link set eth0 down` → 写 `lowrisc-eth/unbind` → 身份检查（无 driver、eth0 消失）→ 写 `lowrisc-eth100-bootstrap/bind` → 身份检查（新 driver、eth0 回来）→ `platform-net-up eth0 --addr 192.168.77.2/24 --apply` → 最多 10 s 轮询 `/sys/class/net/eth0/carrier`（每秒 1 次，记录每次退出码）→ `ip -o addr show eth0` 与 `dmesg | grep -i -E 'lowrisc|eth100|eth0|carrier|link' | tail -n 20` 作为证据。不触碰 MMC/IOMMU/DMA，无 JTAG/无 SD 写/无实验。
- 恢复：只在“解绑成功之后”的 bind 或 `platform-net-up` 失败时执行一次恢复（若新 driver 在绑则先写新 driver 的 `unbind`，再写旧 driver 的 `bind`，再 `platform-net-up` 恢复 IP），不重试；恢复结果单独记录。
- 退出码：`0` 最终 carrier==1；`1` 失败且未进入恢复路径；`2` 失败但已执行一次性恢复。carrier 超时默认不触发恢复（`RECOVER_ON_CARRIER_TIMEOUT=False`），这属于需要 Codex 确认的策略点。
- 输出目录：`root/results/ethernet-update-20261005-net100-bootstrap/`，内含 `results.json`（含 `marks`：上传/insmod/解绑/绑定/配置/carrier 退出码）、`run.log`、`lsof-serial.txt`、`console.raw.log`、`console.events.log`；`finally` 关闭串口；目录非空则拒跑。
- 需 Codex 把关/确认的点：新 driver 是否需要 eth0 以外名字、`platform-net-up` 的确切语义、是否要 `rmmod` 旧模块、以及解绑并发审查与实际执行验收。

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""net100_bootstrap.py - one-shot Mac-side bootstrap of the lowrisc_net100.ko
platform driver over the already-authenticated Linux root serial console.

Scope (deliberately narrow):
  * reuse SerialConsole (+ its random-marker framing, raw/events logs) from
    tools/composition-v1-20261004/mac_research2.py -- no new framework
  * lsof occupancy check, uname gate, base64/text-chunk upload
  * insmod -> ip link down -> sysfs unbind (built-in) -> sysfs bind (new)
    -> platform-net-up -> <=10 s carrier wait + short dmesg link lines
  * fail fast on every real command; exactly one recovery attempt to the
    built-in driver if bind/config fails after the unbind
  * no login, no password handling, no module hash/read-back, no on-board test,
    no JTAG reload, no SD write, no experiment, no automatic retry

Exit codes: 0 = carrier == 1 at the end, 1 = failed without running the
recovery path, 2 = failed after the one-shot recovery to the built-in driver
was run, 130 = KeyboardInterrupt.
"""
from __future__ import annotations

import base64
import importlib.util
import json
import pathlib
import subprocess
import sys
import time
from typing import Any, Dict, List, Optional

# --------------------------------------------------------------- fixed inputs
ROOT = pathlib.Path('/Users/jojo/Desktop/在mac链接pfga(codex)')
SERIAL_SRC = ROOT / 'tools' / 'composition-v1-20261004' / 'mac_research2.py'
KO_SRC = ROOT / 'results' / 'ethernet-update-20261005-oldbit-02' / 'lowrisc_net100.ko'
OUT_DIR = ROOT / 'results' / 'ethernet-update-20261005-net100-bootstrap'
SERIAL_PORT = '/dev/cu.usbserial-AV0KRCHT'

KERNEL_RELEASE = '6.5.0-rc2-cpn2-net1-00093-gc0f5e13ea81f-dirty'
EXPECTED_KO_SIZE: Optional[int] = 31944  # local pre-check only; None disables it

PLATFORM_DEVICE = '30000000.lowrisc-eth'
OLD_DRIVER = 'lowrisc-eth'
NEW_DRIVER = 'lowrisc-eth100-bootstrap'
NETDEV = 'eth0'
IP_CIDR = '192.168.77.2/24'

B64_TMP = '/tmp/net100.b64'
KO_TMP = '/tmp/lowrisc_net100.ko'

MAX_CMD_CHARS = 1024
B64_CHUNK_CHARS = 960
PROBE_TIMEOUT = 15.0
CMD_TIMEOUT = 30.0
UPLOAD_TIMEOUT = 60.0
CARRIER_TIMEOUT = 10.0

# The task only requires recovery for a bind/config failure.  Flip to True if a
# carrier timeout should also trigger the single recovery attempt.
RECOVER_ON_CARRIER_TIMEOUT = False

SCHEMA = 'net100-bootstrap-v1'


class BootstrapError(Exception):
    pass


class RecoveredFailure(BootstrapError):
    pass


def utc_now() -> str:
    return time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime())


def _text(data: Any) -> str:
    if isinstance(data, bytes):
        return data.decode('utf-8', errors='replace')
    return str(data)


def identity_probe_cmd() -> str:
    """One command, always exit 0, prints: driver=<name|NONE> netdev=<absent|present> owner=<name|none>."""
    dev = '/sys/bus/platform/devices/' + PLATFORM_DEVICE
    net = '/sys/class/net/' + NETDEV
    return ('p=' + dev + '; drv=NONE; '
            'if [ -e "$p/driver" ]; then drv=$(basename "$(readlink "$p/driver")"); fi; '
            'nd=absent; own=none; '
            'if [ -e ' + net + ' ]; then nd=present; '
            'if [ -e ' + net + '/device ]; then own=$(basename "$(readlink ' + net + '/device)"); fi; fi; '
            'printf "driver=%s netdev=%s owner=%s\\n" "$drv" "$nd" "$own"')


def preflight_local() -> None:
    missing = [str(p) for p in (SERIAL_SRC, KO_SRC) if not p.is_file()]
    if missing:
        raise BootstrapError('missing required local file(s): ' + ', '.join(missing))
    if EXPECTED_KO_SIZE is not None:
        size = KO_SRC.stat().st_size
        if size != EXPECTED_KO_SIZE:
            raise BootstrapError('unexpected ko size: ' + str(size) +
                                 ' != ' + str(EXPECTED_KO_SIZE))
    if not pathlib.Path(SERIAL_PORT).exists():
        raise BootstrapError('serial device missing: ' + SERIAL_PORT)


def prepare_out_dir() -> None:
    if OUT_DIR.exists():
        leftovers = [p.name for p in OUT_DIR.iterdir()]
        if leftovers:
            raise BootstrapError('output directory is not empty: ' + str(OUT_DIR) +
                                 ' -> ' + repr(leftovers[:5]))
    else:
        OUT_DIR.mkdir(parents=True)


def lsof_serial(port: str, out_path: pathlib.Path) -> None:
    proc = subprocess.run(['lsof', port], stdout=subprocess.PIPE,
                          stderr=subprocess.PIPE, check=False)
    out = proc.stdout.decode('utf-8', errors='replace')
    err = proc.stderr.decode('utf-8', errors='replace')
    out_path.write_text('exit=' + str(proc.returncode) + chr(10) + out + err,
                        encoding='utf-8')
    rows = [r for r in out.splitlines() if r.strip()]
    if rows and rows[0].startswith('COMMAND'):
        rows = rows[1:]
    if rows:
        raise BootstrapError('serial port is in use (lsof): ' + repr(rows[:3]))
    if proc.returncode not in (0, 1):
        raise BootstrapError('lsof failed exit=' + str(proc.returncode) + ': ' +
                             err.strip()[:200])


def load_mac_research2(path: pathlib.Path):
    spec = importlib.util.spec_from_file_location('mac_research2_reuse', str(path))
    if spec is None or spec.loader is None:
        raise BootstrapError('cannot import ' + str(path))
    module = importlib.util.module_from_spec(spec)
    sys.modules['mac_research2_reuse'] = module
    spec.loader.exec_module(module)
    if not hasattr(module, 'SerialConsole'):
        raise BootstrapError('SerialConsole not found in ' + str(path))
    return module


class BootstrapRunner:
    def __init__(self, console, out_dir: pathlib.Path):
        self.console = console
        self.out_dir = out_dir
        self.steps: List[Dict[str, Any]] = []
        self.marks: Dict[str, Any] = {}
        self.upload_chunks = 0
        self.recovered = False
        self.recovery_ok = False

    # -- plumbing ---------------------------------------------------------
    def _log(self, text: str) -> None:
        line = utc_now() + ' ' + text
        try:
            with open(self.out_dir / 'run.log', 'a', encoding='utf-8') as f:
                f.write(line + chr(10))
        except OSError:
            pass
        print(line, file=sys.stderr)

    def add_step(self, step: str, cmd=None, res=None, note=None, **extra) -> Dict[str, Any]:
        item: Dict[str, Any] = {'step': step, 'at': utc_now()}
        if cmd is not None:
            item['cmd'] = cmd
        if res is not None:
            item['exit'] = res.exit_code
            item['stdout'] = _text(res.stdout).strip()
        if note:
            item['note'] = note
        item.update(extra)
        self.steps.append(item)
        return item

    def shell(self, step: str, cmd: str, timeout: float = CMD_TIMEOUT,
              allow_fail: bool = False, note: Optional[str] = None,
              record_cmd: bool = True):
        if len(cmd) > MAX_CMD_CHARS:
            raise BootstrapError(step + ': run_shell command is ' + str(len(cmd)) +
                                 ' chars (> ' + str(MAX_CMD_CHARS) + ')')
        self._log(step + ': ' + (cmd if record_cmd else (note or cmd[:64])))
        try:
            res = self.console.run_shell(cmd, timeout=timeout)
        except Exception as exc:
            self.add_step(step, cmd if record_cmd else None, None,
                          note='run_shell raised ' + repr(exc))
            raise BootstrapError(step + ': run_shell failed: ' + repr(exc))
        self.add_step(step, cmd if record_cmd else None, res, note=note)
        if res.exit_code != 0 and not allow_fail:
            raise BootstrapError(step + ': exit=' + str(res.exit_code) +
                                 ' stdout=' + repr(_text(res.stdout)[-200:]))
        return res

    def probe(self, step: str, cmd: str, note: Optional[str] = None) -> str:
        return _text(self.shell(step, cmd, PROBE_TIMEOUT, allow_fail=True,
                                note=note).stdout).strip()

    # -- identity checks ---------------------------------------------------
    def identity(self, step: str, note: Optional[str] = None) -> Dict[str, str]:
        raw = self.probe(step, identity_probe_cmd(), note=note)
        state = {'driver': 'NONE', 'netdev': 'absent', 'owner': 'none', 'raw': raw}
        for token in raw.replace(chr(10), ' ').split():
            if '=' not in token:
                continue
            key, value = token.split('=', 1)
            if key in ('driver', 'netdev', 'owner'):
                state[key] = value
        return state

    def require_identity(self, step: str, driver: Optional[str],
                         expect_netdev: Optional[str] = None) -> Dict[str, str]:
        state = self.identity(step)
        problems: List[str] = []
        if driver is None:
            if state['driver'] != 'NONE':
                problems.append('expected no bound driver, got ' + state['driver'])
        elif state['driver'] != driver:
            problems.append('expected driver ' + driver + ', got ' + state['driver'])
        if expect_netdev is not None and state['netdev'] != expect_netdev:
            problems.append('expected netdev ' + expect_netdev + ', got ' + state['netdev'])
        if state['owner'] not in ('none', PLATFORM_DEVICE):
            problems.append('netdev owner mismatch: ' + state['owner'])
        if problems:
            self.add_step(step + '-mismatch', note='; '.join(problems))
            raise BootstrapError(step + ': ' + '; '.join(problems))
        return state

    # -- upload ------------------------------------------------------------
    def upload_module(self) -> None:
        data = KO_SRC.read_bytes()
        payload = base64.b64encode(data).decode('ascii')
        self.shell('upload-truncate', ': > ' + B64_TMP, CMD_TIMEOUT)
        chunks = [payload[i:i + B64_CHUNK_CHARS]
                  for i in range(0, len(payload), B64_CHUNK_CHARS)]
        for index, chunk in enumerate(chunks):
            cmd = "printf %s '" + chunk + "' >> " + B64_TMP
            self.shell('upload-%03d' % index, cmd, UPLOAD_TIMEOUT,
                       note='chars=' + str(len(chunk)), record_cmd=False)
        self.upload_chunks = len(chunks)
        self.add_step('upload-summary',
                      note='chunks=' + str(len(chunks)) +
                           ' b64_chars=' + str(len(payload)) +
                           ' ko_bytes=' + str(len(data)))
        self.shell('upload-decode', 'base64 -d ' + B64_TMP + ' > ' + KO_TMP,
                   UPLOAD_TIMEOUT)

    # -- carrier -----------------------------------------------------------
    def wait_for_carrier(self):
        deadline = time.monotonic() + CARRIER_TIMEOUT
        attempt = 0
        last_value = ''
        last_exit: Optional[int] = None
        while True:
            attempt += 1
            res = self.shell('carrier-read-%02d' % attempt,
                             'cat /sys/class/net/' + NETDEV + '/carrier',
                             PROBE_TIMEOUT, allow_fail=True)
            last_value = _text(res.stdout).strip()
            last_exit = res.exit_code
            if res.exit_code == 0 and last_value == '1':
                self.add_step('carrier-result',
                              note='carrier=1 attempts=' + str(attempt))
                break
            if time.monotonic() >= deadline:
                self.add_step('carrier-result',
                              note='carrier=' + repr(last_value) +
                                   ' attempts=' + str(attempt) +
                                   ' timeout=' + str(CARRIER_TIMEOUT))
                break
            time.sleep(1.0)
        return last_value, last_exit

    # -- one-shot recovery -------------------------------------------------
    def recover(self) -> None:
        self.recovered = True
        self.add_step('recover-start',
                      note='one-shot restore to the built-in driver; no retry')
        try:
            state = self.identity('recover-probe')
            if state['driver'] == NEW_DRIVER:
                res = self.shell('recover-unbind-new',
                                 'echo ' + PLATFORM_DEVICE +
                                 ' > /sys/bus/platform/drivers/' + NEW_DRIVER + '/unbind',
                                 CMD_TIMEOUT, allow_fail=True)
                self.marks['recover_unbind_new'] = res.exit_code
            if state['driver'] != OLD_DRIVER:
                res = self.shell('recover-bind-old',
                                 'echo ' + PLATFORM_DEVICE +
                                 ' > /sys/bus/platform/drivers/' + OLD_DRIVER + '/bind',
                                 CMD_TIMEOUT, allow_fail=True)
                self.marks['recover_bind_old'] = res.exit_code
            res = self.shell('recover-net-up',
                             'platform-net-up ' + NETDEV + ' --addr ' + IP_CIDR + ' --apply',
                             CMD_TIMEOUT, allow_fail=True)
            self.marks['recover_net_up'] = res.exit_code
            self.recovery_ok = True
        except Exception as exc:
            self.recovery_ok = False
            self.add_step('recover-error', note=repr(exc))
        try:
            self.require_identity('recover-final', OLD_DRIVER, 'present')
        except Exception as exc:
            self.add_step('recover-final-failed', note=repr(exc))

    # -- main flow ---------------------------------------------------------
    def run_flow(self) -> None:
        res = self.shell('uname-r', 'uname -r', CMD_TIMEOUT)
        kernel = _text(res.stdout).strip()
        self.marks['uname'] = res.exit_code
        if kernel != KERNEL_RELEASE:
            self.add_step('kernel-mismatch',
                          note='got=' + repr(kernel) + ' expected=' + repr(KERNEL_RELEASE))
            raise BootstrapError('kernel release mismatch: ' + repr(kernel))
        self.add_step('kernel-ok', note=repr(kernel))

        self.require_identity('identity-before', OLD_DRIVER, 'present')

        self.upload_module()
        self.marks['upload'] = 0

        res = self.shell('insmod', 'insmod ' + KO_TMP, CMD_TIMEOUT)
        self.marks['insmod'] = res.exit_code

        res = self.shell('new-driver-dir',
                         'test -d /sys/bus/platform/drivers/' + NEW_DRIVER, CMD_TIMEOUT)
        self.marks['new_driver_dir'] = res.exit_code

        self.require_identity('identity-after-insmod', OLD_DRIVER, 'present')

        res = self.shell('ip-link-down', 'ip link set ' + NETDEV + ' down', CMD_TIMEOUT)
        self.marks['ip_link_down'] = res.exit_code

        res = self.shell('unbind-old',
                         'echo ' + PLATFORM_DEVICE + ' > /sys/bus/platform/drivers/' +
                         OLD_DRIVER + '/unbind', CMD_TIMEOUT)
        self.marks['unbind'] = res.exit_code
        self.require_identity('identity-after-unbind', None, 'absent')

        try:
            res = self.shell('bind-new',
                             'echo ' + PLATFORM_DEVICE + ' > /sys/bus/platform/drivers/' +
                             NEW_DRIVER + '/bind', CMD_TIMEOUT)
            self.marks['bind'] = res.exit_code
            self.require_identity('identity-after-bind', NEW_DRIVER, 'present')
            res = self.shell('platform-net-up',
                             'platform-net-up ' + NETDEV + ' --addr ' + IP_CIDR + ' --apply',
                             CMD_TIMEOUT)
            self.marks['platform_net_up'] = res.exit_code
        except BootstrapError as exc:
            self.add_step('bind-config-failed', note=str(exc))
            self.recover()
            raise RecoveredFailure(str(exc))

        carrier, carrier_exit = self.wait_for_carrier()
        self.marks['carrier'] = carrier
        self.marks['carrier_exit'] = carrier_exit

        try:
            self.identity('identity-final')
        except BootstrapError as exc:
            self.add_step('identity-final-failed', note=str(exc))

        self.shell('final-ip', 'ip -o addr show ' + NETDEV, CMD_TIMEOUT, allow_fail=True)
        self.shell('dmesg-link',
                   "dmesg | grep -i -E 'lowrisc|eth100|" + NETDEV +
                   "|carrier|link' | tail -n 20",
                   CMD_TIMEOUT, allow_fail=True)

        if carrier != '1':
            if RECOVER_ON_CARRIER_TIMEOUT:
                self.recover()
                raise RecoveredFailure('carrier not up within ' + str(CARRIER_TIMEOUT) + 's')
            raise BootstrapError('carrier not up within ' + str(CARRIER_TIMEOUT) +
                                 's (last carrier value ' + repr(carrier) + ')')


def main() -> int:
    results: Dict[str, Any] = {
        'schema': SCHEMA,
        'started_at': utc_now(),
        'serial_port': SERIAL_PORT,
        'serial_source': str(SERIAL_SRC),
        'kernel_expected': KERNEL_RELEASE,
        'ko_source': str(KO_SRC),
        'platform_device': PLATFORM_DEVICE,
        'old_driver': OLD_DRIVER,
        'new_driver': NEW_DRIVER,
        'netdev': NETDEV,
        'ip_cidr': IP_CIDR,
        'steps': [],
        'marks': {},
        'upload_chunks': 0,
        'recovered': False,
        'error': None,
        'exit_code': None,
    }
    console = None
    runner = None
    code = 1
    try:
        preflight_local()
        prepare_out_dir()
        lsof_serial(SERIAL_PORT, OUT_DIR / 'lsof-serial.txt')
        results['lsof_ok'] = True
        module = load_mac_research2(SERIAL_SRC)
        console = module.SerialConsole(str(SERIAL_PORT),
                                       OUT_DIR / 'console.raw.log',
                                       OUT_DIR / 'console.events.log')
        runner = BootstrapRunner(console, OUT_DIR)
        console.start()
        console.disable_echo()
        runner.run_flow()
        code = 0
    except RecoveredFailure as exc:
        code = 2
        results['error'] = 'recovered failure: ' + str(exc)
    except BootstrapError as exc:
        code = 1
        results['error'] = str(exc)
    except KeyboardInterrupt:
        code = 130
        results['error'] = 'KeyboardInterrupt'
    except Exception as exc:
        code = 1
        results['error'] = type(exc).__name__ + ': ' + str(exc)
    finally:
        if runner is not None:
            results['steps'] = runner.steps
            results['marks'] = runner.marks
            results['upload_chunks'] = runner.upload_chunks
            results['recovered'] = runner.recovered
            results['recovery_ok'] = runner.recovery_ok
        results['finished_at'] = utc_now()
        results['exit_code'] = code
        try:
            OUT_DIR.mkdir(parents=True, exist_ok=True)
            with open(OUT_DIR / 'results.json', 'w', encoding='utf-8') as f:
                json.dump(results, f, ensure_ascii=False, indent=2, sort_keys=True)
                f.write(chr(10))
        except OSError as exc:
            print('cannot write results.json: ' + repr(exc), file=sys.stderr)
        if console is not None:
            try:
                console.close()
            except Exception:
                pass
    return code


if __name__ == '__main__':
    sys.exit(main())
```

建议的验收检查（由 Codex 实际执行验证）
1. 运行后 `results.json` 中 `marks` 同时含 `upload=0`、`insmod`、`unbind`、`bind`、`platform_net_up`、`carrier`、`carrier_exit`；上传相关 45 个 `upload-0xx` 步骤退出码均为 0，且 `upload_chunks` 与 `b64_chars=42592`、`ko_bytes=31944` 一致。
2. `identity-before`/`identity-after-insmod` 为 `lowrisc-eth`，`identity-after-unbind` 为 `NONE/absent`，`identity-after-bind` 为 `lowrisc-eth100-bootstrap/present`。
3. 退出码语义、`recovered`/`recovery_ok` 与 `recover-*` 步骤一致；任何 `upload-*`、`insmod`、`unbind-old`、`bind-new`、`platform-net-up` 非零退出都必须停止并留有记录。
4. `console.raw.log`、`console.events.log`、`lsof-serial.txt`、`run.log` 均存在；目录非空时脚本拒绝运行。
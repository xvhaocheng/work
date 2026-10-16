#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""net100_bootstrap.py - one-shot Mac-side bootstrap of lowrisc_net100.ko over the
already-authenticated Linux root serial console.  No login/password handling here: the
console is already a root shell and this script only types commands into it.

Reuses SerialConsole from mac_research2.py (no new framework).  lsof check, uname gate,
initial driver/netdev/owner match, base64 chunked upload (< 1024 chars per command),
insmod -> ip link down -> sysfs unbind (built-in) -> sysfs bind (new) -> platform-net-up
-> <= 10 s carrier wait -> results.json + console raw/events logs.  Every real command
must exit 0 and identity mismatches stop the run; at most one recovery attempt to the
built-in driver if the failure is at/after the unbind.  No module hash/read-back, no
on-board test, no JTAG reload, no SD write, no retry.

Exit codes: 0 = carrier == 1 at the end, 1 = failed without any recovery attempt,
2 = failed after the one-shot recovery was attempted, 130 = KeyboardInterrupt.
"""
from __future__ import annotations
import base64, importlib.util, json, pathlib, subprocess, sys, time
from typing import Any, Dict

ROOT = pathlib.Path('/Users/jojo/Desktop/在mac链接pfga(codex)')
SERIAL_SRC = ROOT / 'tools' / 'composition-v1-20261004' / 'mac_research2.py'
KO_SRC = ROOT / 'results' / 'ethernet-update-20261005-oldbit-02' / 'lowrisc_net100.ko'
OUT_DIR = ROOT / 'results' / 'ethernet-update-20261005-net100-bootstrap'
SERIAL_PORT, LSOF = '/dev/cu.usbserial-AV0KRCHT', '/usr/sbin/lsof'
PLATFORM_DEVICE, OLD_DRIVER, NEW_DRIVER = ('30000000.lowrisc-eth', 'lowrisc-eth',
                                           'lowrisc-eth100-bootstrap')
NETDEV, IP_CIDR = 'eth0', '192.168.77.2/24'
B64_TMP, KO_TMP = '/tmp/net100.b64', '/tmp/lowrisc_net100.ko'
KERNEL_RELEASE = '6.5.0-rc2-cpn2-net1-00093-gc0f5e13ea81f-dirty'
KO_SIZE = 31944                       # existing local size pre-check, kept unchanged
MAX_CMD_CHARS, B64_CHUNK_CHARS = 1024, 960
CMD_TIMEOUT, UPLOAD_TIMEOUT = 30.0, 60.0
CARRIER_TIMEOUT, CARRIER_MIN = 10.0, 0.5
RECOVER_ON_CARRIER_TIMEOUT = False    # recovery is for a bind/config failure
SCHEMA = 'net100-bootstrap-v1'
NET_UP = 'platform-net-up %s --addr %s --apply' % (NETDEV, IP_CIDR)
PROBE = ('p=/sys/bus/platform/devices/' + PLATFORM_DEVICE + '; drv=NONE; n=/sys/class/net/'
         + NETDEV + '; if [ -e "$p/driver" ]; then drv=$(basename "$(readlink "$p/driver")"); '
         'fi; nd=absent; own=none; if [ -e "$n" ]; then nd=present; if [ -e "$n/device" ]; '
         'then own=$(basename "$(readlink "$n/device")"); fi; fi; '
         'printf "driver=%s netdev=%s owner=%s\\n" "$drv" "$nd" "$own"')
STATE: Dict[str, Any] = {'console': None, 'out_dir': None, 'steps': [], 'marks': {},
                         'upload_chunks': 0, 'recovered': False, 'recovery_ok': False}


class BootstrapError(Exception):
    pass


def utc_now() -> str:
    return time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime())


def _text(data) -> str:
    return data.decode('utf-8', errors='replace') if isinstance(data, bytes) else str(data)


def add_step(step, cmd=None, res=None, note=None, **extra):
    item = {'step': step, 'at': utc_now(), **extra}
    if cmd is not None: item['cmd'] = cmd
    if res is not None: item['exit'], item['stdout'] = res.exit_code, _text(res.stdout).strip()
    if note: item['note'] = note
    STATE['steps'].append(item)
    return item


def sh(step, cmd, timeout=CMD_TIMEOUT, allow_fail=False, note=None, record=True):
    """Run one real command; a non-zero exit stops the run unless allow_fail=True."""
    if len(cmd) > MAX_CMD_CHARS:
        raise BootstrapError('%s: command is %d chars (> %d)' % (step, len(cmd), MAX_CMD_CHARS))
    print('%s %s: %s' % (utc_now(), step, cmd if record else (note or cmd[:64])), file=sys.stderr)
    try:
        res = STATE['console'].run_shell(cmd, timeout=timeout)
    except Exception as exc:
        add_step(step, note='run_shell raised ' + repr(exc))
        raise BootstrapError('%s: run_shell failed: %s' % (step, repr(exc)))
    add_step(step, cmd=(cmd if record else None), res=res, note=note)
    if res.exit_code != 0 and not allow_fail:
        raise BootstrapError('%s: exit=%d %r' % (step, res.exit_code, _text(res.stdout)[-200:]))
    return res


def dev_cmd(action, driver) -> str:
    return 'echo %s > /sys/bus/platform/drivers/%s/%s' % (PLATFORM_DEVICE, driver, action)


def run_all(pairs, allow_fail=False) -> bool:
    """Run (name, cmd) in order, record marks, stop at the first non-zero exit."""
    for name, cmd in pairs:
        res = sh(name, cmd, CMD_TIMEOUT, allow_fail=allow_fail)
        STATE['marks'][name] = res.exit_code
        if res.exit_code != 0:
            return False
    return True


def probe(step):
    """Parse the fixed probe line -> (driver, netdev, owner); an unseen key gives None."""
    out = _text(sh(step, PROBE, CMD_TIMEOUT).stdout)
    vals = dict(tok.split('=', 1) for tok in out.replace('\n', ' ').split() if '=' in tok)
    return vals.get('driver'), vals.get('netdev'), vals.get('owner')


def require_identity(step, driver, netdev=None):
    """driver=None means strictly no bound driver (NONE); any mismatch stops the run."""
    drv, nd, own = probe(step)
    bad = []
    if drv != (driver or 'NONE'): bad.append('driver=%r != %r' % (drv, driver or 'NONE'))
    if netdev is not None and nd != netdev: bad.append('netdev=%r != %r' % (nd, netdev))
    if own != (PLATFORM_DEVICE if nd == 'present' else 'none'): bad.append('owner=%r' % (own,))
    if bad:
        add_step(step + '-mismatch', note='; '.join(bad))
        raise BootstrapError(step + ': ' + '; '.join(bad))


def upload_module() -> None:
    data = KO_SRC.read_bytes()
    payload = base64.b64encode(data).decode('ascii')
    sh('upload-truncate', ': > ' + B64_TMP, CMD_TIMEOUT)
    chunks = [payload[i:i + B64_CHUNK_CHARS] for i in range(0, len(payload), B64_CHUNK_CHARS)]
    for i, chunk in enumerate(chunks):
        sh('upload-%03d' % i, "printf %s '" + chunk + "' >> " + B64_TMP, UPLOAD_TIMEOUT,
           note='chars=%d' % len(chunk), record=False)
    STATE['upload_chunks'] = len(chunks)
    add_step('upload-summary', note='chunks=%d ko_bytes=%d' % (len(chunks), len(data)))
    sh('upload-decode', 'base64 -d ' + B64_TMP + ' > ' + KO_TMP, UPLOAD_TIMEOUT)


def wait_for_carrier():
    """Poll for <= CARRIER_TIMEOUT; every read only gets the time still left (>= 0.5 s)."""
    deadline, value, attempt = time.monotonic() + CARRIER_TIMEOUT, None, 0
    while time.monotonic() < deadline:
        attempt += 1
        res = sh('carrier-read-%02d' % attempt, 'cat /sys/class/net/' + NETDEV + '/carrier',
                 max(CARRIER_MIN, deadline - time.monotonic()))
        value = _text(res.stdout).strip()
        if value == '1': break
        time.sleep(CARRIER_MIN)
    add_step('carrier-result', note='carrier=%r attempts=%d' % (value, attempt))
    return value


def recover() -> None:
    """One attempt only; recovery_ok stays False on any non-zero exit or final mismatch."""
    STATE['recovered'], STATE['recovery_ok'] = True, False
    add_step('recover-start', note='one-shot restore to built-in driver; no retry')
    try:
        drv = probe('recover-probe')[0]
        if drv not in (NEW_DRIVER, OLD_DRIVER, 'NONE'):
            add_step('recover-abort', note='foreign driver %r; nothing is unbound' % (drv,))
            return
        todo = [('recover-unbind-new', dev_cmd('unbind', NEW_DRIVER))] if drv == NEW_DRIVER else []
        if drv != OLD_DRIVER:
            todo.append(('recover-bind-old', dev_cmd('bind', OLD_DRIVER)))
        todo.append(('recover-net-up', NET_UP))
        if not run_all(todo, allow_fail=True):
            add_step('recover-abort', note='first non-zero exit stopped the recovery')
            return
        require_identity('recover-final', OLD_DRIVER, 'present')
    except Exception as exc:
        add_step('recover-error', note=repr(exc))
        return
    STATE['recovery_ok'] = True


def run_flow() -> None:
    res = sh('uname-r', 'uname -r', CMD_TIMEOUT)
    STATE['marks']['uname'] = res.exit_code
    kernel = _text(res.stdout).strip()
    if kernel != KERNEL_RELEASE:
        add_step('kernel-mismatch', note='got=%r expected=%r' % (kernel, KERNEL_RELEASE))
        raise BootstrapError('kernel release mismatch: %r' % (kernel,))
    require_identity('identity-before', OLD_DRIVER, 'present')
    upload_module()
    STATE['marks']['upload'] = 0
    run_all([('insmod', 'insmod ' + KO_TMP),
             ('new-driver-dir', 'test -d /sys/bus/platform/drivers/' + NEW_DRIVER)])
    require_identity('identity-after-insmod', OLD_DRIVER, 'present')
    run_all([('ip-link-down', 'ip link set ' + NETDEV + ' down'),
             ('unbind-old', dev_cmd('unbind', OLD_DRIVER))])
    try:
        require_identity('identity-after-unbind', None, 'absent')      # strict: driver == NONE
        STATE['marks']['bind-new'] = sh('bind-new', dev_cmd('bind', NEW_DRIVER),
                                        CMD_TIMEOUT).exit_code
        require_identity('identity-after-bind', NEW_DRIVER, 'present')
        STATE['marks']['platform-net-up'] = sh('platform-net-up', NET_UP, CMD_TIMEOUT).exit_code
    except BootstrapError as exc:
        add_step('bind-config-failed', note=str(exc))
        recover()
        raise
    carrier = wait_for_carrier()
    STATE['marks']['carrier'] = carrier
    require_identity('identity-final', NEW_DRIVER, 'present')          # mismatch stops the run
    sh('final-ip', 'ip -o addr show ' + NETDEV, CMD_TIMEOUT)           # non-zero stops the run
    sh('dmesg-link', "dmesg | grep -i -E 'lowrisc|eth100|" + NETDEV + "|carrier|link'"
       + ' | tail -n 20', CMD_TIMEOUT, allow_fail=True)
    if carrier != '1':
        if RECOVER_ON_CARRIER_TIMEOUT:
            recover()
        raise BootstrapError('carrier not up within %ss (last %r)' % (CARRIER_TIMEOUT, carrier))


def main() -> int:
    result = {'schema': SCHEMA, 'started_at': utc_now(), 'serial_port': SERIAL_PORT,
              'serial_source': str(SERIAL_SRC), 'ko_source': str(KO_SRC), 'error': None,
              'kernel_expected': KERNEL_RELEASE, 'platform_device': PLATFORM_DEVICE,
              'netdev': NETDEV, 'old_driver': OLD_DRIVER, 'new_driver': NEW_DRIVER,
              'ip_cidr': IP_CIDR, 'lsof_ok': False}
    console, code = None, 1
    try:
        if not SERIAL_SRC.is_file() or not KO_SRC.is_file():
            raise BootstrapError('missing ko or mac_research2 source file')
        if KO_SRC.stat().st_size != KO_SIZE:
            raise BootstrapError('unexpected ko size: %d' % KO_SRC.stat().st_size)
        if not pathlib.Path(SERIAL_PORT).exists():
            raise BootstrapError('serial device missing: ' + SERIAL_PORT)
        if OUT_DIR.exists() and any(OUT_DIR.iterdir()):
            raise BootstrapError('output directory is not empty: ' + str(OUT_DIR))
        OUT_DIR.mkdir(parents=True, exist_ok=True)
        STATE['out_dir'] = OUT_DIR
        proc = subprocess.run([LSOF, SERIAL_PORT], stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE, check=False)
        (OUT_DIR / 'lsof-serial.txt').write_text(
            'exit=%d\n%s%s' % (proc.returncode, _text(proc.stdout), _text(proc.stderr)),
            encoding='utf-8')
        rows = [r for r in _text(proc.stdout).splitlines()
                if r.strip() and not r.startswith('COMMAND')]
        if rows or proc.returncode not in (0, 1):
            raise BootstrapError('serial port in use / lsof exit=%d: %r'
                                 % (proc.returncode, rows[:3]))
        result['lsof_ok'] = True
        spec = importlib.util.spec_from_file_location('mac_research2_reuse', str(SERIAL_SRC))
        module = importlib.util.module_from_spec(spec)
        sys.modules['mac_research2_reuse'] = module
        spec.loader.exec_module(module)
        console = module.SerialConsole(SERIAL_PORT, OUT_DIR / 'console.raw.log',
                                       OUT_DIR / 'console.events.log')
        STATE['console'] = console
        console.start()
        console.disable_echo()
        run_flow()
        code = 0
    except BootstrapError as exc:
        code, result['error'] = (2 if STATE['recovered'] else 1), str(exc)
    except KeyboardInterrupt:
        code, result['error'] = 130, 'KeyboardInterrupt'
    except Exception as exc:
        code, result['error'] = 1, '%s: %s' % (type(exc).__name__, exc)
    finally:
        result.update(steps=STATE['steps'], marks=STATE['marks'],
                      upload_chunks=STATE['upload_chunks'], recovered=STATE['recovered'],
                      recovery_ok=STATE['recovery_ok'], finished_at=utc_now(), exit_code=code)
        if STATE['out_dir'] is not None:   # only after the out dir passed the empty check
            try:
                with open(OUT_DIR / 'results.json', 'w', encoding='utf-8') as f:
                    json.dump(result, f, ensure_ascii=False, indent=2, sort_keys=True)
                    f.write('\n')
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

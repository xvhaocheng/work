"""Use an existing user-supplied credential helper without logging login bytes."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import select
import sys
import time


def authenticate_password(console, helper, timeout=20):
    """Called only after the root account's Password prompt was observed.

    The trusted local helper supplies the existing credential in memory.
    Private receive bypasses Console.read's raw logger, including unexpected
    terminal echo. No password, password hash, or received login bytes is saved.
    One failed attempt stops; the caller does not retry.
    """
    helper = Path(helper).resolve()
    spec = importlib.util.spec_from_file_location('ispass_existing_board_credential', helper)
    module = importlib.util.module_from_spec(spec)
    sys.path.insert(0, str(helper.parent))
    try:
        spec.loader.exec_module(module)
        username, secret = module.supplied_board_credential()
    except Exception:
        raise RuntimeError('existing credential helper unavailable') from None
    finally:
        sys.path.pop(0)
    if username != 'root' or not isinstance(secret, str) or not secret or '\n' in secret or '\r' in secret:
        raise RuntimeError('credential helper does not supply the expected account')
    receipt = {'helper': str(helper), 'helper_sha256': hashlib.sha256(helper.read_bytes()).hexdigest(),
               'credential_transmissions': 1, 'automatic_retry': False,
               'login_bytes_saved': False, 'status': 'attempted'}
    private = b''
    try:
        console.send(secret.encode()+b'\n')
        deadline = time.monotonic()+timeout
        while time.monotonic() < deadline:
            ready, _, _ = select.select([console.fd], [], [], .1)
            if not ready:
                continue
            private += os.read(console.fd, 4096)
            text = private[-8192:].decode('utf8', 'replace').replace('\r', '')
            if re.search(r'Login incorrect|Authentication failed|Password:\s*$', text):
                raise RuntimeError('board authentication rejected; no retry')
            if re.search(r'(?:^|\n)[^\n]*#\s*$', text):
                receipt['status'] = 'shell_prompt_observed_identity_still_required'
                return
        raise TimeoutError('board authentication response timeout; no retry')
    finally:
        private = b''
        secret = None
        (console.output/'authentication.json').write_text(json.dumps(receipt, indent=2)+'\n')

Below is a complete, self-contained Python 3 standard-library-only proposal for `scripts/read_hang_snapshot.py`. It implements a bounded XVC diagnostic reader for the verified Kintex-7 `xc7k325t` TAP described in the task. No execution has been performed by the author; this is a proposal for Codex to review, adapt, and execute.

```python
#!/usr/bin/env python3
"""
read_hang_snapshot.py

Bounded XVC diagnostic reader for a single verified Kintex-7 xc7k325t TAP.
Standard library only. Proposal only; no execution performed by author.

References:
- AMD UG470 Table 10-2 for documented JTAG primitives.
- XVC protocol (Xilinx Virtual Cable) raw commands.

This script does NOT program the FPGA, perform JPROGRAM, or write memory.
It only issues JTAG IR/DR shifts via XVC.
"""

import argparse
import json
import socket
import struct
import sys
import time
from datetime import datetime, timezone

# ----------------------------------------------------------------------
# Constants
# ----------------------------------------------------------------------
DEFAULT_HOST = "127.0.0.1"
DEFAULT_PORT = 13721
DEFAULT_SAMPLES = 3
MIN_SAMPLES = 1
MAX_SAMPLES = 10
DEFAULT_PERIOD_NS = 1000
SOCKET_TIMEOUT = 5.0

IR_LEN = 6
IDCODE_IR = 0x09
IDCODE_EXPECTED = 0x03651093
IDCODE_MASK = 0x0FFFFFFF  # mask revision nibble (bits 31:28)
BYPASS_IR = 0x3F
USER4_IR = 0x23

FRAME_BITS = 1088
HEADER_BITS = 64
PAYLOAD_BITS = 1024
PAYLOAD_WORDS = 32
MAGIC = 0x48414E47
VERSION = 1

# ----------------------------------------------------------------------
# Low-level utilities
# ----------------------------------------------------------------------
def read_exact(sock, n):
    """Read exactly n bytes, raise EOFError if connection closes early."""
    buf = bytearray()
    while len(buf) < n:
        chunk = sock.recv(n - len(buf))
        if not chunk:
            raise EOFError(f"connection closed after {len(buf)}/{n} bytes")
        buf.extend(chunk)
    return bytes(buf)

def read_line_bounded(sock, max_bytes=128):
    """Read a newline-terminated line, bounded by max_bytes."""
    buf = bytearray()
    while len(buf) < max_bytes:
        b = sock.recv(1)
        if not b:
            raise EOFError("connection closed while reading line")
        if b == b'\n':
            return bytes(buf)
        buf.extend(b)
    raise ValueError(f"response line exceeded {max_bytes} bytes")

def pack_bits(bits):
    """Pack list of 0/1 ints into bytes, LSB first."""
    n = len(bits)
    nbytes = (n + 7) // 8
    out = bytearray(nbytes)
    for i, b in enumerate(bits):
        if b:
            out[i // 8] |= (1 << (i % 8))
    return bytes(out)

def unpack_bits(data, nbits):
    """Unpack nbits from bytes, LSB first, return list of 0/1 ints."""
    bits = []
    for i in range(nbits):
        byte = data[i // 8]
        bit = (byte >> (i % 8)) & 1
        bits.append(bit)
    return bits

# ----------------------------------------------------------------------
# XVC client
# ----------------------------------------------------------------------
class XVCClient:
    def __init__(self, host, port, timeout=SOCKET_TIMEOUT):
        self.sock = socket.create_connection((host, port), timeout=timeout)
        self.sock.settimeout(timeout)
        self.max_bits = None

    def close(self):
        try:
            self.sock.close()
        except Exception:
            pass

    def _send(self, data):
        self.sock.sendall(data)

    def getinfo(self):
        self._send(b"getinfo:")
        line = read_line_bounded(self.sock, 128)
        s = line.decode('ascii', errors='strict')
        if not s.startswith("xvcServer_v1.0:"):
            raise ValueError(f"unexpected getinfo: {s!r}")
        parts = s.split(':')
        if len(parts) != 2:
            raise ValueError(f"malformed getinfo: {s!r}")
        self.max_bits = int(parts[1])
        if self.max_bits <= 0:
            raise ValueError(f"invalid max bits: {self.max_bits}")
        return self.max_bits

    def settck(self, period_ns=DEFAULT_PERIOD_NS):
        self._send(b"settck:" + struct.pack('<I', period_ns))
        resp = read_exact(self.sock, 4)
        actual = struct.unpack('<I', resp)[0]
        return actual

    def shift(self, tms_bytes, tdi_bytes, bitcount):
        """Perform one XVC shift. bitcount must be <= max_bits."""
        if self.max_bits is None:
            raise RuntimeError("getinfo must be called before shift")
        if bitcount > self.max_bits:
            raise ValueError(f"shift bitcount {bitcount} exceeds max {self.max_bits}")
        nbytes = (bitcount + 7) // 8
        if len(tms_bytes) != nbytes or len(tdi_bytes) != nbytes:
            raise ValueError("TMS/TDI byte lengths do not match bitcount")
        payload = struct.pack('<I', bitcount) + tms_bytes + tdi_bytes
        self._send(payload)
        resp = read_exact(self.sock, nbytes)
        return resp

# ----------------------------------------------------------------------
# JTAG helper sequences (TAP state machine)
# ----------------------------------------------------------------------
def xvc_shift(client, tms_bits, tdi_bits):
    """Shift a full TMS/TDI bit sequence, chunking to respect max_bits.
    Returns full TDO bit list."""
    if len(tms_bits) != len(tdi_bits):
        raise ValueError("TMS and TDI lengths differ")
    total = len(tms_bits)
    tdo_bits = []
    offset = 0
    while offset < total:
        chunk_len = min(client.max_bits, total - offset)
        tms_chunk = tms_bits[offset:offset+chunk_len]
        tdi_chunk = tdi_bits[offset:offset+chunk_len]
        tms_bytes = pack_bits(tms_chunk)
        tdi_bytes = pack_bits(tdi_chunk)
        resp_bytes = client.shift(tms_bytes, tdi_bytes, chunk_len)
        tdo_chunk = unpack_bits(resp_bytes, chunk_len)
        tdo_bits.extend(tdo_chunk)
        offset += chunk_len
    return tdo_bits

def reset_tap(client):
    """Reset TAP: >=5 TMS=1 then TMS=0. Leaves TAP in IDLE."""
    tms = [1] * 5 + [0]
    tdi = [0] * len(tms)
    xvc_shift(client, tms, tdi)

def shift_ir(client, ir_value, ir_len=IR_LEN, idle_clocks=16):
    """Assume TAP is in IDLE. Shift IR, end in IDLE.
    Returns TDO bits (unused)."""
    entry_tms = [1, 1, 0, 0]       # Select-DR, Select-IR, Capture-IR, Shift-IR
    entry_tdi = [0, 0, 0, 0]
    data_tms = [0] * ir_len
    if ir_len > 0:
        data_tms[-1] = 1            # exit1
    data_tdi = [(ir_value >> i) & 1 for i in range(ir_len)]
    exit_tms = [1, 0]               # Update-IR, Idle
    exit_tdi = [0, 0]
    idle_tms = [0] * idle_clocks
    idle_tdi = [0] * idle_clocks
    tms = entry_tms + data_tms + exit_tms + idle_tms
    tdi = entry_tdi + data_tdi + exit_tdi + idle_tdi
    return xvc_shift(client, tms, tdi)

def shift_dr(client, data_bits, data_len, idle_clocks=16):
    """Assume TAP is in IDLE. Shift DR, end in IDLE.
    Returns TDO bits for the data_len data bits."""
    if len(data_bits) != data_len:
        raise ValueError("data_bits length mismatch")
    entry_tms = [1, 0, 0]           # Select-DR, Capture-DR, Shift-DR
    entry_tdi = [0, 0, 0]
    data_tms = [0] * data_len
    if data_len > 0:
        data_tms[-1] = 1            # exit1
    data_tdi = data_bits[:]
    exit_tms = [1, 0]               # Update-DR, Idle
    exit_tdi = [0, 0]
    idle_tms = [0] * idle_clocks
    idle_tdi = [0] * idle_clocks
    tms = entry_tms + data_tms + exit_tms + idle_tms
    tdi = entry_tdi + data_tdi + exit_tdi + idle_tdi
    tdo = xvc_shift(client, tms, tdi)
    # Extract data TDO bits
    data_start = len(entry_tms)
    data_tdo = tdo[data_start:data_start+data_len]
    return data_tdo

# ----------------------------------------------------------------------
# High-level operations
# ----------------------------------------------------------------------
def read_idcode(client):
    """Reset TAP, load IDCODE instruction, shift DR 32 bits, return IDCODE."""
    reset_tap(client)
    shift_ir(client, IDCODE_IR)
    data_bits = [0] * 32
    tdo_bits = shift_dr(client, data_bits, 32)
    idcode = 0
    for i, b in enumerate(tdo_bits):
        if b:
            idcode |= (1 << i)
    return idcode

def request_and_read_sample(client, index):
    """Perform one USER4 sample request and read. Returns dict with frame data."""
    # Load USER4 instruction
    shift_ir(client, USER4_IR)
    # Request shift: 1088 bits, TDI bit0=1, rest 0. Ignore TDO.
    request_tdi = [1] + [0] * (FRAME_BITS - 1)
    shift_dr(client, request_tdi, FRAME_BITS)
    # Wait 0.01 sec plus 16 idle clocks (idle clocks are included in shift_dr).
    time.sleep(0.01)
    # Read shift: 1088 bits, TDI all 0. Capture TDO.
    read_tdi = [0] * FRAME_BITS
    tdo_bits = shift_dr(client, read_tdi, FRAME_BITS)
    # Assemble frame bytes (LSB first)
    frame_bytes = pack_bits(tdo_bits)  # 136 bytes
    # Parse header (first 64 bits = first 8 bytes)
    header_bytes = frame_bytes[:8]
    header_int = int.from_bytes(header_bytes, byteorder='little')
    magic = header_int & 0xFFFFFFFF
    version = (header_int >> 32) & 0xFF
    valid = (header_int >> 40) & 1
    ready = (header_int >> 41) & 1
    reqtoggle = (header_int >> 42) & 1
    acksync = (header_int >> 43) & 1
    sequence = (header_int >> 44) & 0xFFFFF  # 20 bits
    # Validate
    if magic != MAGIC:
        raise ValueError(f"sample {index}: bad magic 0x{magic:08X}, expected 0x{MAGIC:08X}")
    if version != VERSION:
        raise ValueError(f"sample {index}: bad version {version}, expected {VERSION}")
    if not valid:
        raise ValueError(f"sample {index}: valid bit not set")
    if not ready:
        raise ValueError(f"sample {index}: ready bit not set")
    # Payload: next 128 bytes (1024 bits)
    payload_bytes = frame_bytes[8:8+128]
    payload_words = []
    for i in range(PAYLOAD_WORDS):
        word = int.from_bytes(payload_bytes[i*4:(i+1)*4], byteorder='little')
        payload_words.append(f"0x{word:08X}")
    timestamp = time.time()
    timestamp_iso = datetime.fromtimestamp(timestamp, tz=timezone.utc).isoformat()
    return {
        "index": index,
        "timestamp": timestamp,
        "timestamp_iso": timestamp_iso,
        "header": {
            "raw": f"0x{header_int:016X}",
            "magic": f"0x{magic:08X}",
            "version": version,
            "valid": bool(valid),
            "ready": bool(ready),
            "reqtoggle": reqtoggle,
            "acksync": acksync,
            "sequence": sequence
        },
        "payload_words": payload_words,
        "rawframehex": frame_bytes.hex(),
        "payload_hex": payload_bytes.hex()
    }

# ----------------------------------------------------------------------
# Main
# ----------------------------------------------------------------------
def main():
    parser = argparse.ArgumentParser(description="Bounded XVC diagnostic reader for Kintex-7 xc7k325t")
    parser.add_argument("--idcode-only", action="store_true", help="Only read IDCODE, no USER4 writes.")
    parser.add_argument("--host", default=DEFAULT_HOST, help=f"XVC host (default {DEFAULT_HOST})")
    parser.add_argument("--port", type=int, default=DEFAULT_PORT, help=f"XVC port (default {DEFAULT_PORT})")
    parser.add_argument("--output", required=True, help="Output JSON file path")
    parser.add_argument("--samples", type=int, default=DEFAULT_SAMPLES,
                        help=f"Number of samples (default {DEFAULT_SAMPLES}, min {MIN_SAMPLES}, max {MAX_SAMPLES})")
    args = parser.parse_args()

    if args.samples < MIN_SAMPLES or args.samples > MAX_SAMPLES:
        print(f"error: --samples must be between {MIN_SAMPLES} and {MAX_SAMPLES}", file=sys.stderr)
        sys.exit(2)

    result = {
        "host": args.host,
        "port": args.port,
        "idcode_only": args.idcode_only,
        "max_bits": None,
        "actual_period_ns": None,
        "idcode": None,
        "idcode_masked_match": None,
        "samples": [],
        "errors": []
    }

    client = None
    healthy = False
    try:
        client = XVCClient(args.host, args.port)
        max_bits = client.getinfo()
        result["max_bits"] = max_bits
        actual_period = client.settck(DEFAULT_PERIOD_NS)
        result["actual_period_ns"] = actual_period
        healthy = True

        idcode = read_idcode(client)
        result["idcode"] = f"0x{idcode:08X}"
        masked = idcode & IDCODE_MASK
        expected_masked = IDCODE_EXPECTED & IDCODE_MASK
        result["idcode_masked_match"] = (masked == expected_masked)
        if not result["idcode_masked_match"]:
            raise ValueError(f"IDCODE mismatch: read 0x{idcode:08X}, expected 0x{IDCODE_EXPECTED:08X} (masked)")

        if not args.idcode_only:
            for i in range(args.samples):
                sample = request_and_read_sample(client, i)
                result["samples"].append(sample)

    except Exception as e:
        result["errors"].append(str(e))
        # Do not retry or hide; just record error.
    finally:
        # Attempt to set BYPASS and go to idle if connection healthy
        if client is not None and healthy:
            try:
                shift_ir(client, BYPASS_IR)
                # shift_ir ends in IDLE
            except Exception as e:
                result["errors"].append(f"cleanup BYPASS failed: {e}")
        if client is not None:
            client.close()

    # Write output
    try:
        with open(args.output, 'w') as f:
            json.dump(result, f, indent=2)
    except Exception as e:
        print(f"error writing output: {e}", file=sys.stderr)
        sys.exit(1)

    if result["errors"]:
        print("completed with errors:", file=sys.stderr)
        for err in result["errors"]:
            print(f"  {err}", file=sys.stderr)
        sys.exit(1)
    sys.exit(0)

if __name__ == "__main__":
    main()
```

## Assumptions

1. The XVC server maintains TAP state across separate `shift` commands, as per the XVC protocol. Chunking long shifts is therefore safe as long as the bitstream order is preserved.
2. The USER4 data register is exactly 1088 bits: 64-bit header followed by 1024-bit payload (32 little-endian 32-bit words).
3. The “low bit” that requests a sample is the first TDI bit shifted in (LSB first), i.e. `TDI[0] = 1` of the 1088-bit frame.
4. The 16 idle clocks after UPDATE are sufficient for any CDC synchronisation required by the hardware.
5. IDCODE revision nibble (bits 31:28) is allowed to vary; the mask `0x0FFFFFFF` is correct.
6. The XVC `getinfo` response never exceeds 128 bytes and is newline-terminated.
7. The socket timeout of 5 s is adequate for the diagnostic operations used here.
8. The script does not perform any FPGA programming, JPROGRAM, or memory writes. The TAP reset is a JTAG state reset only.

## Suggested Acceptance Checks (minimal)

- **IDCODE on existing BIT**  
  With the FPGA already configured, run:  
  `python3 scripts/read_hang_snapshot.py --idcode-only --host 127.0.0.1 --port 13721 --output idcode.json`  
  Verify `idcode_masked_match` is `true` and `idcode` matches `0x03651093` (with any revision nibble).

- **Snapshot after diagnostic BIT**  
  After loading a BIT that exposes the USER4 snapshot interface, run:  
  `python3 scripts/read_hang_snapshot.py --host 127.0.0.1 --port 13721 --output snapshot.json --samples 3`  
  Verify each sample in `samples` has a valid header (`magic == 0x48414E47`, `version == 1`, `valid == true`, `ready == true`) and non-empty `payload_hex`/`rawframehex`. Verify `errors` is empty.

- **Truncation / EOF mock (if needed)**  
  Point the script at a mock server that returns fewer bytes than requested or closes the connection early. Verify the script records an `EOFError` in `errors` and does not retry or hide the failure. The output JSON should still be written and the exit code should be non-zero.

- **Cleanup**  
  On a healthy connection, verify the script attempts a BYPASS IR shift before closing (can be checked with a mock server that logs commands). No BYPASS shift is attempted if the connection is already broken.
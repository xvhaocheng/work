#!/usr/bin/env python3
"""
read_hang_snapshot.py

Bounded XVC diagnostic reader for a single verified Kintex-7 xc7k325t TAP.
Standard library only. Drafted by a worker; the packet-split behavior below
was executed and verified on hardware by Codex.

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

SEQ_MASK = 0xFFFFF  # 20-bit sequence field
MIN_ADVERTISED_BYTES = 16
MAX_ADVERTISED_BYTES = 16 * 1024 * 1024
MAX_TRANSACTION_BITS = 1024

# ----------------------------------------------------------------------
# Exceptions
# ----------------------------------------------------------------------
class XVCError(Exception):
    """Base class for XVC-related errors."""

class XVCTransportError(XVCError):
    """Socket/transport-level failure."""

class XVCProtocolError(XVCError):
    """XVC protocol framing or command failure."""

class ValidationError(XVCError):
    """Semantic validation failure after a completed shift."""

# ----------------------------------------------------------------------
# Low-level utilities
# ----------------------------------------------------------------------
def read_exact(sock, n):
    """Read exactly n bytes, raise XVCTransportError if connection closes early."""
    buf = bytearray()
    while len(buf) < n:
        try:
            chunk = sock.recv(n - len(buf))
        except OSError as e:
            raise XVCTransportError(f"recv failed: {e}") from e
        if not chunk:
            raise XVCTransportError(f"connection closed after {len(buf)}/{n} bytes")
        buf.extend(chunk)
    return bytes(buf)

def read_line_bounded(sock, max_bytes=128):
    """Read a newline-terminated line, bounded by max_bytes."""
    buf = bytearray()
    while len(buf) < max_bytes:
        try:
            b = sock.recv(1)
        except OSError as e:
            raise XVCTransportError(f"recv failed while reading line: {e}") from e
        if not b:
            raise XVCTransportError("connection closed while reading line")
        if b == b'\n':
            return bytes(buf)
        buf.extend(b)
    raise XVCProtocolError(f"response line exceeded {max_bytes} bytes")

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
        try:
            self.sock = socket.create_connection((host, port), timeout=timeout)
        except OSError as e:
            raise XVCTransportError(f"connection to {host}:{port} failed: {e}") from e
        self.sock.settimeout(timeout)
        self.advertised_bytes = None
        self.max_bits = None

    def close(self):
        try:
            self.sock.close()
        except Exception:
            pass

    def _send(self, data):
        try:
            self.sock.sendall(data)
        except OSError as e:
            raise XVCTransportError(f"send failed: {e}") from e

    def getinfo(self):
        self._send(b"getinfo:")
        line = read_line_bounded(self.sock, 128)
        try:
            s = line.decode('ascii', errors='strict').strip()
        except UnicodeDecodeError as e:
            raise XVCProtocolError(f"getinfo not ASCII: {e}") from e
        if not s.startswith("xvcServer_v1.0:"):
            raise XVCProtocolError(f"unexpected getinfo: {s!r}")
        parts = s.split(':')
        if len(parts) != 2:
            raise XVCProtocolError(f"malformed getinfo: {s!r}")
        try:
            advertised_bytes = int(parts[1])
        except ValueError as e:
            raise XVCProtocolError(f"non-integer getinfo size: {parts[1]!r}") from e
        if advertised_bytes < MIN_ADVERTISED_BYTES:
            raise XVCProtocolError(
                f"advertised buffer too small: {advertised_bytes} bytes "
                f"(minimum {MIN_ADVERTISED_BYTES})"
            )
        if advertised_bytes > MAX_ADVERTISED_BYTES:
            raise XVCProtocolError(
                f"advertised buffer too large: {advertised_bytes} bytes "
                f"(maximum {MAX_ADVERTISED_BYTES})"
            )
        self.advertised_bytes = advertised_bytes
        # getinfo advertises server buffer BYTES, not bits.
        # Choose a conservative transaction cap. Each shift sends
        # b"shift:" + 4-byte bitcount + TMS bytes + TDI bytes. With
        # max_bits <= advertised_bytes, the combined TMS+TDI payload is
        # at most 2*ceil(max_bits/8) bytes, which fits inside the
        # advertised byte buffer for the accepted range.
        self.max_bits = min(advertised_bytes, MAX_TRANSACTION_BITS)
        return self.max_bits

    def settck(self, period_ns=DEFAULT_PERIOD_NS):
        self._send(b"settck:" + struct.pack('<I', period_ns))
        resp = read_exact(self.sock, 4)
        actual = struct.unpack('<I', resp)[0]
        return actual

    def shift(self, tms_bytes, tdi_bytes, bitcount):
        """Perform one XVC shift. bitcount must be <= max_bits."""
        if self.max_bits is None:
            raise XVCProtocolError("getinfo must be called before shift")
        if bitcount > self.max_bits:
            raise XVCProtocolError(
                f"shift bitcount {bitcount} exceeds max {self.max_bits}"
            )
        nbytes = (bitcount + 7) // 8
        if len(tms_bytes) != nbytes or len(tdi_bytes) != nbytes:
            raise XVCProtocolError("TMS/TDI byte lengths do not match bitcount")
        payload = b"shift:" + struct.pack('<I', bitcount) + tms_bytes + tdi_bytes
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
        raise XVCProtocolError("TMS and TDI lengths differ")
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
    """Reset TAP: 6 TMS=1 then TMS=0. Leaves TAP in IDLE.

    6 ones (instead of the JTAG minimum 5) matches the measured
    openFPGALoader behavior verified on hardware by Codex.
    """
    tms = [1] * 6 + [0]
    tdi = [0] * len(tms)
    xvc_shift(client, tms, tdi)

def idle_clocks(client, count=16):
    """Issue idle clocks (TMS=0, TDI=0) while in IDLE state."""
    tms = [0] * count
    tdi = [0] * count
    return xvc_shift(client, tms, tdi)

def shift_ir(client, ir_value, ir_len=IR_LEN, idle_clocks=16):
    """Assume TAP is in IDLE. Shift IR, end in IDLE.

    Each TAP phase is issued as its own XVC shift transaction. The
    openFPGALoader XVC server mis-samples TDO when the state entry,
    data, exit and idle clocks are packed into a single shift, so they
    are split here (verified on hardware by Codex). Returns the TDO
    bits of the IR data packet (unused)."""
    # Entry: Select-DR, Select-IR, Capture-IR, Shift-IR
    xvc_shift(client, [1, 1, 0, 0], [0, 0, 0, 0])
    # Data: ir_len IR bits LSB-first; final bit carries TMS=1 to leave Shift-IR
    data_tms = [0] * ir_len
    if ir_len > 0:
        data_tms[-1] = 1            # Exit1-IR
    data_tdi = [(ir_value >> i) & 1 for i in range(ir_len)]
    tdo = xvc_shift(client, data_tms, data_tdi)
    # Exit: Update-IR, Idle
    xvc_shift(client, [1, 0], [0, 0])
    # Idle clocks (TMS=0, TDI=0)
    xvc_shift(client, [0] * idle_clocks, [0] * idle_clocks)
    return tdo

def shift_dr(client, data_bits, data_len, idle_clocks=16):
    """Assume TAP is in IDLE. Shift DR, end in IDLE.

    Each TAP phase is issued as its own XVC shift transaction (see
    shift_ir for rationale). The data packet keeps TMS=0 for every bit
    except the last (Exit1-DR); xvc_shift still chunks it automatically
    to respect the advertised XVC buffer size. Returns the full TDO of
    the data packet, so the result is exactly data_len bits with no
    entry offset to strip."""
    if len(data_bits) != data_len:
        raise XVCProtocolError("data_bits length mismatch")
    # Entry: Select-DR, Capture-DR, Shift-DR
    xvc_shift(client, [1, 0, 0], [0, 0, 0])
    # Data: data_len bits; final bit carries TMS=1 to leave Shift-DR
    data_tms = [0] * data_len
    if data_len > 0:
        data_tms[-1] = 1            # Exit1-DR
    data_tdi = data_bits[:]
    data_tdo = xvc_shift(client, data_tms, data_tdi)
    # Exit: Update-DR, Idle
    xvc_shift(client, [1, 0], [0, 0])
    # Idle clocks (TMS=0, TDI=0)
    xvc_shift(client, [0] * idle_clocks, [0] * idle_clocks)
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
    """Perform one USER4 sample request and read. Returns dict with frame data
    and a list of validation errors. Does not raise on validation errors;
    caller records the sample and then fails."""
    # Load USER4 instruction
    shift_ir(client, USER4_IR)
    # Request shift: 1088 bits, TDI bit0=1, rest 0. Ignore TDO.
    request_tdi = [1] + [0] * (FRAME_BITS - 1)
    shift_dr(client, request_tdi, FRAME_BITS)
    # Wait 0.01 sec plus idle clocks. The shift_dr above already ended in
    # IDLE with its own idle clocks. Add more idle clocks after the sleep
    # so acksync can settle if the SoC is slow at request time.
    time.sleep(0.01)
    idle_clocks(client, 16)
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
    sequence = (header_int >> 44) & SEQ_MASK  # 20 bits
    # Validate; collect all errors, do not raise here.
    errors = []
    if magic != MAGIC:
        errors.append(f"bad magic 0x{magic:08X}, expected 0x{MAGIC:08X}")
    if version != VERSION:
        errors.append(f"bad version {version}, expected {VERSION}")
    if not valid:
        errors.append("valid bit not set")
    if not ready:
        errors.append("ready bit not set")
    if reqtoggle != acksync:
        errors.append(f"reqtoggle {reqtoggle} != acksync {acksync}")
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
        "payload_hex": payload_bytes.hex(),
        "validation_errors": errors
    }

# ----------------------------------------------------------------------
# Main
# ----------------------------------------------------------------------
def main():
    parser = argparse.ArgumentParser(description="Bounded XVC diagnostic reader for Kintex-7 xc7k325t")
    parser.add_argument("--idcode-only", action="store_true", help="Only read IDCODE, no USER4 sample requests.")
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
        "advertised_bytes": None,
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
        result["advertised_bytes"] = client.advertised_bytes
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
            raise ValidationError(
                f"IDCODE mismatch: read 0x{idcode:08X}, "
                f"expected 0x{IDCODE_EXPECTED:08X} (masked)"
            )

        if not args.idcode_only:
            prev_seq = None
            for i in range(args.samples):
                sample = request_and_read_sample(client, i)
                result["samples"].append(sample)
                if sample["validation_errors"]:
                    raise ValidationError(
                        f"sample {i}: " + "; ".join(sample["validation_errors"])
                    )
                seq = sample["header"]["sequence"]
                if prev_seq is not None:
                    expected = (prev_seq + 1) & SEQ_MASK
                    if seq != expected:
                        msg = (
                            f"sample {i}: sequence {seq} != expected {expected} "
                            f"(previous {prev_seq})"
                        )
                        sample["validation_errors"].append(msg)
                        raise ValidationError(msg)
                prev_seq = seq

    except Exception as e:
        result["errors"].append(str(e))
        # Any non-validation failure means transport/protocol state is not
        # trusted, so do not attempt a cleanup BYPASS from an unknown TAP state.
        if not isinstance(e, ValidationError):
            healthy = False
    finally:
        # Normal cleanup BYPASS only if we still have a known-healthy connection
        # and the TAP was left in a known state (IDLE) by the prior shift.
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

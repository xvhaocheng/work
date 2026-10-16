#!/usr/bin/env python3
"""Task-owned transfer endpoint on the dedicated board Ethernet link."""
import base64
import hashlib
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
from pathlib import Path
import time
from urllib.parse import urlsplit

ROOT = Path(__file__).resolve().parent.parent
DELIVERY = ROOT / 'delivery'
LIVE = ROOT / 'live'


class Handler(BaseHTTPRequestHandler):
    def log_message(self, fmt, *args):
        with (LIVE / 'task-http-access.log').open('a') as f:
            f.write(json.dumps({'at_ns': time.monotonic_ns(), 'peer': self.client_address[0],
                                'message': fmt % args}) + '\n')

    def do_GET(self):
        name = urlsplit(self.path).path.removeprefix('/')
        if self.client_address[0] not in ('192.168.77.1', '192.168.77.2') or '/' in name or not name:
            self.send_error(403)
            return
        p = DELIVERY / name
        if not p.is_file() or p.is_symlink():
            self.send_error(404)
            return
        self.send_response(200)
        self.send_header('Content-Length', str(p.stat().st_size))
        self.end_headers()
        with p.open('rb') as f:
            for block in iter(lambda: f.read(262144), b''):
                self.wfile.write(block)

    def do_POST(self):
        if self.client_address[0] != '192.168.77.2' or self.path not in ('/backup', '/backup-base64'):
            self.send_error(403)
            return
        encoded = self.path == '/backup-base64'
        length = self.headers.get('Content-Length')
        if (not length or not length.isdigit() or
                (not encoded and length != '33554432') or
                (encoded and not 44739244 <= int(length) <= 46000000)):
            (LIVE / 'backup-HTTP-rejected-header.json').write_text(json.dumps({
                'content_length': length, 'encoding': self.headers.get('Transfer-Encoding'),
                'path': self.path, 'expected_raw_bytes': 33554432}) + '\n')
            self.send_error(400)
            return
        start = time.monotonic_ns()
        partial = LIVE / 'original-mmcblk0p1.bin.partial'
        final = LIVE / 'original-mmcblk0p1.bin'
        if partial.exists() or final.exists():
            self.send_error(409)
            return
        got = 0
        h = hashlib.sha256()
        self.connection.settimeout(60)
        try:
            if encoded:
                data = self.rfile.read(int(length))
                if len(data) != int(length):
                    raise RuntimeError('short encoded backup upload')
                raw = base64.b64decode(b''.join(data.split()), validate=True)
                if len(raw) != 33554432:
                    raise RuntimeError('decoded backup length mismatch')
                with partial.open('xb') as f:
                    f.write(raw)
                got = len(raw)
                h.update(raw)
            else:
                with partial.open('xb') as f:
                    while got < 33554432:
                        block = self.rfile.read(min(262144, 33554432 - got))
                        if not block:
                            raise RuntimeError('short SD backup upload')
                        got += len(block)
                        f.write(block)
                        h.update(block)
            partial.rename(final)
            record = {'received_bytes': got, 'sha256': h.hexdigest(), 'file': str(final),
                      'dell_wall_seconds': (time.monotonic_ns() - start) / 1e9,
                      'peer': self.client_address[0], 'SD_read_or_write_by_server': False,
                      'transport_encoding': 'base64 text' if encoded else 'raw binary',
                      'wire_bytes': int(length)}
            (LIVE / 'SD-backup-host-transfer.json').write_text(json.dumps(record, indent=2) + '\n')
            answer = (json.dumps(record) + '\n').encode()
            self.send_response(200)
            self.send_header('Content-Length', str(len(answer)))
            self.end_headers()
            self.wfile.write(answer)
        except Exception as exc:
            (LIVE / 'SD-backup-transfer-failure.json').write_text(json.dumps({
                'received_bytes': got, 'error_type': type(exc).__name__,
                'dell_wall_seconds': (time.monotonic_ns() - start) / 1e9}) + '\n')
            self.send_error(500)


if __name__ == '__main__':
    server = ThreadingHTTPServer(('192.168.77.1', 18080), Handler)
    (LIVE / 'task-http-listen.json').write_text(json.dumps({
        'bind': '192.168.77.1', 'port': 18080, 'directory': str(DELIVERY),
        'backup_target': str(LIVE / 'original-mmcblk0p1.bin'),
        'scope': 'task artifacts and exact 32MiB original partition backup only',
        'started_monotonic_ns': time.monotonic_ns()}) + '\n')
    server.serve_forever()

"""Bounded local app-server JSON-RPC; no model inference or daemon restart."""
import json
import os
import socket
import struct
import base64
import hashlib
import time


class Client:
    def __init__(self):
        self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.sock.settimeout(15)
        self.sock.connect('/home/xvhaocheng/.codex/app-server-control/app-server-control.sock')
        self.buffer = b''
        self.messages = []
        self.serial = 0
        key = base64.b64encode(os.urandom(16)).decode()
        request = ('GET / HTTP/1.1\r\nHost: localhost\r\nUpgrade: websocket\r\n'
                   'Connection: Upgrade\r\nSec-WebSocket-Version: 13\r\n'
                   f'Sec-WebSocket-Key: {key}\r\n\r\n')
        self.sock.sendall(request.encode())
        while b'\r\n\r\n' not in self.buffer:
            self.buffer += self.sock.recv(4096)
        headers, self.buffer = self.buffer.split(b'\r\n\r\n', 1)
        if not headers.startswith(b'HTTP/1.1 101 '):
            self.sock.close()
            raise RuntimeError('Local WebSocket handshake refused')
        expected = base64.b64encode(hashlib.sha1(
            (key + '258EAFA5-E914-47DA-95CA-C5AB0DC85B11').encode()).digest())
        if expected.lower() not in headers.lower():
            self.sock.close()
            raise RuntimeError('Invalid local WebSocket accept')
        response = self.call('initialize', {
            'clientInfo': {'name': 'model-context-policy', 'version': '1.0'},
            'capabilities': {'experimentalApi': True},
        })
        if 'error' in response:
            raise RuntimeError(response['error'])
        self.send({'method': 'initialized'})

    def send(self, message):
        self.frame(1, json.dumps(message).encode())

    def frame(self, opcode, payload):
        size = len(payload)
        prefix = bytes([0x80 | opcode, 0x80 | size]) if size < 126 else (
            bytes([0x80 | opcode, 0x80 | 126]) + struct.pack('!H', size)
            if size < 65536 else bytes([0x80 | opcode, 0x80 | 127]) + struct.pack('!Q', size))
        mask = os.urandom(4)
        self.sock.sendall(prefix + mask + bytes(x ^ mask[i % 4] for i, x in enumerate(payload)))

    def read(self, size):
        while len(self.buffer) < size:
            chunk = self.sock.recv(65536)
            if not chunk:
                raise RuntimeError('Local control socket closed')
            self.buffer += chunk
        value, self.buffer = self.buffer[:size], self.buffer[size:]
        return value

    def recv(self):
        fragments = []
        while True:
            first, second = self.read(2)
            opcode, size = first & 15, second & 127
            if size == 126:
                size = struct.unpack('!H', self.read(2))[0]
            elif size == 127:
                size = struct.unpack('!Q', self.read(8))[0]
            if size > 32 * 1024 * 1024:
                raise RuntimeError('Unexpected control frame size')
            mask = self.read(4) if second & 128 else b''
            data = self.read(size)
            if mask:
                data = bytes(x ^ mask[i % 4] for i, x in enumerate(data))
            if opcode == 9:
                self.frame(10, data)
                continue
            if opcode == 10:
                continue
            if opcode == 8:
                raise RuntimeError('Control connection closed')
            fragments.append(data)
            if first & 128:
                return json.loads(b''.join(fragments))

    def call(self, method, params, timeout=25):
        self.serial += 1
        ident = self.serial
        self.send({'id': ident, 'method': method, 'params': params})
        deadline = time.monotonic() + timeout
        while time.monotonic() < deadline:
            self.sock.settimeout(max(.1, deadline - time.monotonic()))
            message = self.recv()
            if message.get('id') == ident:
                return message
            self.messages.append(message)
        raise TimeoutError(method)

    def close(self):
        self.sock.close()


def safe_config_result(response):
    if 'error' in response:
        return response
    result = response['result']
    config = result.get('config', {})
    return {
        'config': {key: val for key, val in config.items()
                   if key.startswith(('model', 'agents', 'profile'))
                   and key != 'model_providers'},
        'origins': {key: val for key, val in result.get('origins', {}).items()
                    if key.startswith(('model', 'agents', 'profile'))},
        'layer_count': len(result.get('layers') or []),
    }


def safe_thread_result(response):
    if 'error' in response:
        return response
    result = response['result']
    thread = result.get('thread', result)
    fields = ['id', 'parentThreadId', 'path', 'model', 'reasoningEffort',
              'status', 'source', 'threadSource', 'cwd', 'historyMode']
    return {key: thread.get(key) for key in fields if key in thread}

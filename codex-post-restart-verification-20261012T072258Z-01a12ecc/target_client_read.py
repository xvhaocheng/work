import os, socket, json, base64, hashlib, struct, time, re
from pathlib import Path
from datetime import datetime, timezone

OUT = Path(__file__).parent
SHARED = Path('/home/xvhaocheng/.codex')
transcript = []
def now(): return datetime.now(timezone.utc).isoformat()
def scrub(x, key=''):
    if re.search(r'token|password|secret|authorization|cookie|api.?key', key, re.I):
        return x if key in ['model_context_window', 'model_auto_compact_token_limit'] else '[sensitive field omitted]'
    if isinstance(x, dict): return {k: scrub(v, k) for k, v in x.items()}
    if isinstance(x, list): return [scrub(v, key) for v in x]
    return x
def catalog():
    x = json.loads((SHARED / 'models_cache.json').read_text())
    sol = next(z for z in x['models'] if z['slug'] == 'gpt-6.1-sol')
    return {'fetched_at': x.get('fetched_at'), 'etag': x.get('etag'), 'client_version': x.get('client_version'),
            'sol': {k: sol.get(k) for k in ['slug', 'context_window', 'max_context_window', 'effective_context_window_percent']}}

s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM); s.settimeout(30)
result = {'started_at_utc': now(), 'transport': 'documented control socket WebSocket',
          'client_name': 'codex-backend (non-originating per versioned initialize source)', 'catalog_before': catalog()}
buf = bytearray()
def take(n):
    while len(buf) < n:
        block = s.recv(max(4096, n-len(buf)))
        if not block: raise EOFError('WebSocket closed')
        buf.extend(block)
    value = bytes(buf[:n]); del buf[:n]; return value
def send_frame(data, op=1):
    mask = os.urandom(4); n = len(data)
    header = bytes([0x80 | op, 0x80 | (n if n < 126 else 126 if n < 65536 else 127)])
    if n >= 65536: header += struct.pack('!Q', n)
    elif n >= 126: header += struct.pack('!H', n)
    s.sendall(header + mask + bytes(b ^ mask[i % 4] for i, b in enumerate(data)))
def receive():
    chunks = []
    while True:
        a,b = take(2); op=a & 15; n=b & 127
        if n == 126: n=struct.unpack('!H', take(2))[0]
        elif n == 127: n=struct.unpack('!Q', take(8))[0]
        if n > 16*1024*1024: raise ValueError('Unexpected oversized WebSocket frame')
        mask=take(4) if b & 128 else None; payload=take(n)
        if mask: payload=bytes(v ^ mask[i%4] for i,v in enumerate(payload))
        if op == 9: send_frame(payload,10); continue
        if op == 10: continue
        if op == 8: raise EOFError('WebSocket close frame')
        chunks.append(payload)
        if a & 128: return json.loads(b''.join(chunks))
def rpc(rid, method, params):
    msg={'id':rid,'method':method,'params':params}
    transcript.append({'at_utc':now(),'direction':'request','message':scrub(msg)})
    send_frame(json.dumps(msg).encode())
    deadline=time.monotonic()+30
    while time.monotonic()<deadline:
        reply=receive()
        if reply.get('id') == rid and ('result' in reply or 'error' in reply): return reply
    raise TimeoutError(method)
try:
    s.connect(str(SHARED / 'app-server-control/app-server-control.sock'))
    key=base64.b64encode(os.urandom(16)).decode()
    s.sendall(('GET / HTTP/1.1\r\nHost: localhost\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: '+key+'\r\nSec-WebSocket-Version: 13\r\n\r\n').encode())
    while b'\r\n\r\n' not in buf: buf.extend(s.recv(4096))
    raw_headers, rest=bytes(buf).split(b'\r\n\r\n',1); buf[:]=rest
    headers=raw_headers.decode().split('\r\n'); assert ' 101 ' in headers[0],headers[0]
    expected=base64.b64encode(hashlib.sha1((key+'258EAFA5-E914-47DA-95CA-C5AB0DC85B11').encode()).digest()).decode()
    actual=next(x.split(':',1)[1].strip() for x in headers[1:] if x.lower().startswith('sec-websocket-accept:'))
    assert actual==expected
    result['handshake_status']=headers[0]
    response=rpc(1,'initialize',{'clientInfo':{'name':'codex-backend','version':'0.162.1'},'capabilities':{'experimentalApi':True}})
    result['initialize']=scrub(response); transcript.append({'at_utc':now(),'direction':'response','message':scrub(response)})
    assert 'result' in response,response.get('error')
    send_frame(json.dumps({'method':'initialized','params':{}}).encode())
    response=rpc(2,'config/read',{'cwd':'/home/xvhaocheng/work','includeLayers':True})
    result['config_read']=scrub(response); transcript.append({'at_utc':now(),'direction':'response','message':scrub(response)})
    response=rpc(3,'account/read',{'refreshToken':False}); account=response.get('result',{}).get('account')
    result['account_read']={'error':scrub(response.get('error')),'account':{k:account.get(k) for k in ['type','planType']} if isinstance(account,dict) else None}
    transcript.append({'at_utc':now(),'direction':'response','method':'account/read','message':result['account_read']})
    result['catalog_after']=catalog(); result['finished_at_utc']=now()
except Exception as exc:
    result['error']=type(exc).__name__+': '+str(exc)
finally:
    try: send_frame(b'',8)
    except Exception: pass
    s.close()
    (OUT/'target-client-read.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
    (OUT/'target-client-read-transcript.jsonl').write_text(''.join(json.dumps(x,ensure_ascii=False)+'\n' for x in transcript))
    print(json.dumps({k:result.get(k) for k in ['error','started_at_utc','initialize','account_read','catalog_before','catalog_after','finished_at_utc']},ensure_ascii=False,indent=2))

#!/usr/bin/env python3
"""Selected task HTTP server plus bounded, lossless finite result reception.

Prepared offline. It must replace only our task-owned listener after approval;
no unrelated service is stopped. Existing payload/asset GET behavior is reused.
"""
import json
import sys
import time
sys.dont_write_bytecode = True
from http.server import ThreadingHTTPServer
from pathlib import Path
from finite_result_bundle import MAX_WIRE, accept_transport
from task_http import Handler, LIVE


class ResultHandler(Handler):
    def do_POST(self):
        if self.path != '/finite-results':
            return super().do_POST()
        length = self.headers.get('Content-Length', '')
        if (self.client_address[0] != '192.168.77.2' or not length.isdigit()
                or not 0 < int(length) <= MAX_WIRE):
            self.send_error(400)
            return
        start = time.monotonic_ns()
        self.connection.settimeout(60)
        try:
            body = self.rfile.read(int(length))
            if len(body) != int(length):
                raise ValueError('short finite result transfer')
            result = accept_transport(body, LIVE / 'finite-pilot-result')
            result['dell_receive_wall_seconds'] = (time.monotonic_ns() - start) / 1e9
            answer = (json.dumps(result) + '\n').encode()
            self.send_response(200)
            self.send_header('Content-Length', str(len(answer)))
            self.end_headers()
            self.wfile.write(answer)
            (LIVE / 'finite-pilot-transport-result.json').write_text(json.dumps(result, indent=2) + '\n')
        except Exception as exc:
            (LIVE / 'finite-pilot-transport-failure.json').write_text(json.dumps({
                'reason_type': type(exc).__name__, 'reason': str(exc),
                'semantic_success': 'unverified',
                'dell_wall_seconds': (time.monotonic_ns() - start) / 1e9}) + '\n')
            self.send_error(500)


if __name__ == '__main__':
    ThreadingHTTPServer(('192.168.77.1', 18081), ResultHandler).serve_forever()

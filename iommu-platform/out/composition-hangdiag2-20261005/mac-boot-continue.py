import sys,pathlib,threading,json
sys.path.insert(0,'scripts')
import mac_update_reboot as m
sys.path.insert(0,'tools/composition-v1-20261004')
import mac_composition as c
out=pathlib.Path('results/composition-hangdiag2-20261005-boot01-continuation')
uartlog=pathlib.Path('results/composition-hangdiag2-20261005-boot01-continuation-uart.raw.log')
assert not out.exists() and not uartlog.exists()
c.check_uart_ownership('/dev/cu.usbserial-AV0KRCHT')
port=m.default_serial_factory('/dev/cu.usbserial-AV0KRCHT')
stop=threading.Event()
def capture():
    with uartlog.open('wb') as f:
        while not stop.is_set():
            b=port.read(4096)
            if b: f.write(b); f.flush()
thread=threading.Thread(target=capture,daemon=True);thread.start()

orig_capture_identity=m.SshRebootRunner._capture_identity
def rescue_identity(self, label, timeout=None):
    if label == 'before':
        source=pathlib.Path('results/composition-hangdiag-20261005-boot02/result.json')
        before=json.loads(source.read_text())['after']
        assert before['boot_id']=='2458d16b-87e3-4cab-af96-427a01671f9f'
        self.result['before_identity_source']={'file':str(source),'kind':'last verified boot before observed hang; no fresh board read'}
        return before['uname_r'],before['boot_id']
    return orig_capture_identity(self,label,timeout=timeout)
def rescue_sync(self):
    self.result['sync_skipped_reason']='continuation verifies already started boot; no SD writing or second JTAG'
m.SshRebootRunner._capture_identity=rescue_identity
m.SshRebootRunner._sync=rescue_sync

def already_loaded(self):
    source=pathlib.Path('results/composition-hangdiag2-20261005-boot01/result.json')
    previous=json.loads(source.read_text())
    assert previous['fpga']['returncode']==0 and previous['bit']==str(self.args.bit)
    self.result['continued_from']=str(source)
    self.result['fpga']={'load_skipped':True,'reason':'already loaded by prior invocation; only continue startup verification','previous_returncode':0}
m.SshRebootRunner._load_bit=already_loaded


try:
    rc=m.main(['--run','--transport','ssh','--board-host','192.168.77.2','--host-ip','192.168.77.1','--credential-script','scripts/run_research2_mac.py','--bit','tools/composition-hangdiag2-20261005/ariane_xilinx.bit','--expected-kernel','6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty','--out',str(out),'--boot-timeout','900','--command-timeout','20'])
    print('existing helper exit',rc,flush=True)
    r=json.loads((out/'result.json').read_text());print(json.dumps({k:r.get(k) for k in ('status','before','after','fpga')}),flush=True)
finally:
    stop.set();thread.join(2);port.close()
    if out.exists() and uartlog.exists(): uartlog.replace(out/'uart.raw.log')

sys.exit(rc)


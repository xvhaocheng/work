import sys,pathlib,threading,json
sys.path.insert(0,'scripts')
import mac_update_reboot as m
sys.path.insert(0,'tools/composition-v1-20261004')
import mac_composition as c
p=c.load_plan_module()
plan=p.build_plan()
case=next(x for x in plan['cases'] if x['id']=='explore-b1-g1000-a8-mpn')
diag=dict(plan,cases=[case],count=1,phases={'short':0,'noise':0,'explore':1},diagnostic_only=True,formal172=False)
p.build_plan=lambda:diag
c.load_plan_module=lambda:p
original_attach=c.AttachRunner._wait_for_shell
def attach_trace(self):
    original_attach(self)
    r=self.console.run_shell('echo 8 > /proc/sys/kernel/printk; echo Y > /sys/module/idma/parameters/composition_trace',timeout=10)
    if r.exit_code: raise RuntimeError('trace setup failed')
c.AttachRunner._wait_for_shell=attach_trace
out=pathlib.Path('results/composition-ptwfix-20261005-pn01')
uartlog=pathlib.Path('results/composition-ptwfix-20261005-pn01-uart.raw.log')
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
try:
    print('diagnostic single case',json.dumps(case),flush=True)
    rc=c.main(['--run','--transport','ssh','--board-host','192.168.77.2','--credential-script','scripts/run_research2_mac.py','--ssh-helper','scripts/mac_update_reboot.py','--expected-kernel','6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty','--out',str(out),'--through-phase','explore','--attach-timeout','30','--command-timeout','10','--case-timeout','30'])
    print('diagnostic runner exit',rc,flush=True)
finally:
    stop.set();thread.join(2);port.close()
    if out.exists() and uartlog.exists():uartlog.replace(out/'uart.raw.log')

sys.exit(rc)


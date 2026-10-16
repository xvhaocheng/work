import sys,pathlib,threading,json
sys.path.insert(0,'scripts')
import mac_update_reboot as m
sys.path.insert(0,'tools/composition-v1-20261004')
import mac_composition as c
original_attach=c.AttachRunner._wait_for_shell
def attach_trace(self):
    original_attach(self)
    r=self.console.run_shell('echo N > /sys/module/idma/parameters/composition_trace',timeout=10)
    if r.exit_code: raise RuntimeError('trace setup failed')
c.AttachRunner._wait_for_shell=attach_trace
out=pathlib.Path('results/composition-dsfix-20261005-formal01')
uartlog=pathlib.Path('results/composition-dsfix-20261005-formal01-uart.raw.log')
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
    print('full frozen 172-case plan; same boot follows successful single N4096 diagnostic',flush=True)
    rc=c.main(['--run','--transport','ssh','--board-host','192.168.77.2','--credential-script','scripts/run_research2_mac.py','--ssh-helper','scripts/mac_update_reboot.py','--expected-kernel','6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty','--out',str(out),'--through-phase','explore','--attach-timeout','30','--command-timeout','10','--case-timeout','300'])
    print('full runner exit',rc,flush=True)
finally:
    stop.set();thread.join(2);port.close()
    if out.exists() and uartlog.exists():uartlog.replace(out/'uart.raw.log')

sys.exit(rc)


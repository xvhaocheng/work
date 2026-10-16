import sys,pathlib,threading,json
sys.path.insert(0,'scripts')
import mac_update_reboot as m
sys.path.insert(0,'tools/composition-v1-20261004')
import mac_composition as c
out=pathlib.Path('results/composition-ptwfix-20261005-boot01')
uartlog=pathlib.Path('results/composition-ptwfix-20261005-boot01-uart.raw.log')
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
    rc=m.main(['--run','--transport','ssh','--board-host','192.168.77.2','--host-ip','192.168.77.1','--credential-script','scripts/run_research2_mac.py','--bit','tools/composition-ptwfix-20261005/ariane_xilinx.bit','--expected-kernel','6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty','--out',str(out),'--boot-timeout','900','--command-timeout','20'])
    print('existing helper exit',rc,flush=True)
    r=json.loads((out/'result.json').read_text());print(json.dumps({k:r.get(k) for k in ('status','before','after','fpga')}),flush=True)
finally:
    stop.set();thread.join(2);port.close()
    if out.exists() and uartlog.exists(): uartlog.replace(out/'uart.raw.log')

sys.exit(rc)


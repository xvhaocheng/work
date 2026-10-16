
import os,sys,pathlib,shlex
os.environ['PATH']='/opt/homebrew/bin:'+os.environ.get('PATH','')
sys.path.insert(0,str(pathlib.Path('tools/composition-v1-20261004').resolve()))
import mac_composition as mc
m=mc._load_serial_module();h=mc.load_ssh_helper('scripts/mac_update_reboot.py')
o=pathlib.Path('results/composition-fix-20261005-net4trace-gzip-update');o.mkdir(exist_ok=False)
u=m.SerialConsole('/dev/cu.usbserial-AV0KRCHT',o/'uart.raw.log',o/'uart.events.log')
original=h.SshRebootRunner._run_ssh
def compressed_fetch(self,name,command,timeout):
 if name=='board-fetch-update':
  parts=shlex.split(command)
  if len(parts)!=3 or parts[0]!='platform-fetch-update' or parts[2]!='--write' or not parts[1].startswith('http://192.168.77.1:'):
   raise RuntimeError('unexpected existing fetch command')
  command='wget -q -O /tmp/fw_payload.bin.gz.partial '+shlex.quote(parts[1])+' && gzip -dc /tmp/fw_payload.bin.gz.partial > /tmp/fw_payload.bin.partial && mv /tmp/fw_payload.bin.partial /tmp/fw_payload.bin && echo decompressed_payload_ready && platform-sd-update --device /dev/mmcblk0p1 --image /tmp/fw_payload.bin'
 return original(self,name,command,timeout)
h.SshRebootRunner._run_ssh=compressed_fetch
u.start()
try:
 args=['--run','--transport','ssh','--bit','tools/composition-trim-20261005/route-opt/ariane_xilinx.bit','--payload','tools/composition-fix-20261005/net4trace/fw_payload.bin.gz','--host-ip','192.168.77.1','--board-host','192.168.77.2','--credential-script','scripts/run_research2_mac.py','--expected-kernel','6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty','--out',str(o/'workflow'),'--boot-timeout','900','--command-timeout','30','--update-timeout','3600']
 rc=h.main(args)
 print('gzip_update_workflow_exit='+str(rc),flush=True)
 raise SystemExit(rc)
finally:u.close()


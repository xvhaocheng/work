
import os,sys,pathlib,json,time
os.environ['PATH']='/opt/homebrew/bin:'+os.environ.get('PATH','')
sys.path.insert(0,str(pathlib.Path('tools/composition-v1-20261004').resolve()))
import mac_composition as mc
m=mc._load_serial_module();h=mc.load_ssh_helper('scripts/mac_update_reboot.py')
o=pathlib.Path('results/composition-fix-20261005-net4trace-n01');o.mkdir(exist_ok=False)
mc.check_uart_ownership('/dev/cu.usbserial-AV0KRCHT')
u=m.SerialConsole('/dev/cu.usbserial-AV0KRCHT',o/'uart.raw.log',o/'uart.events.log')
C=mc.build_ssh_console_class(m,h)
c=C(o/'console.raw.log',o/'console.events.log','192.168.77.2',h.load_credential_response('scripts/run_research2_mac.py'))
args=mc.build_arg_parser().parse_args(['--run','--transport','ssh','--board-host','192.168.77.2','--credential-script','scripts/run_research2_mac.py','--ssh-helper','scripts/mac_update_reboot.py','--expected-kernel','6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty','--out',str(o),'--through-phase','explore','--case-timeout','45','--command-timeout','10'])
p=mc.load_plan_module();plan=p.build_plan();case=next(x for x in plan['cases'] if x['id']=='explore-b1-g1000-a8-mn')
r=mc.AttachRunner(args,c,o,p);r.plan_total=plan['count'];r.current_phase='explore'
mc.write_json(o/'diagnostic-plan.json',{'diagnostic_only':True,'trace_enabled':True,'case':case})
u.start();c.start()
try:
 r.identity=r.collect_identity();mc.write_json(o/'boot.json',r.identity)
 x=c.run_shell('echo 8 > /proc/sys/kernel/printk; echo Y > /sys/module/idma/parameters/composition_trace; cat /sys/module/idma/parameters/composition_trace',timeout=10)
 mc.write_json(o/'trace-enable.json',{'exit_code':x.exit_code,'stdout':mc.text_of(x.stdout)})
 if x.exit_code!=0 or mc.text_of(x.stdout).strip()!='Y':raise RuntimeError('trace enable failed')
 r.run_case(case);r.write_summary('complete')
except Exception as e:
 r._stop(type(e).__name__+': '+str(e));r.write_summary('stopped')
 mc.write_json(o/'diagnostic-error.json',{'type':type(e).__name__,'message':str(e)})
 print(type(e).__name__+': '+str(e),flush=True)
 raise
finally:
 time.sleep(1);c.close();u.close()

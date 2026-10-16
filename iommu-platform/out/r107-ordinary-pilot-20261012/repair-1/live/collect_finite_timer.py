#!/usr/bin/env python3
"""After accepted new boot only: three read-only CAPS probes, no DMA/reset.

The event-time brackets include UART and process overhead. No board nominal
clock is used to construct Dell time, and no frequency point is fabricated.
"""
import argparse,json,subprocess,sys,time
from pathlib import Path
sys.dont_write_bytecode=True
from parse_caps_probe import parse_probe
ROOT=Path(__file__).resolve().parent.parent
LIVE=ROOT/'live'

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--describe',action='store_true');a=p.parse_args()
 if a.describe:
  print(json.dumps({'sample_count':3,'minimum_between_command_calls_seconds':10,'read_only_CAPS_only':True,'actual_UART_commands':0,'same_boot_required':True,'SI_error_ppm':'unspecified; no SI accuracy claim'}));return 0
 approval=json.loads((LIVE/'new-SD-user-approval.json').read_text());identity=json.loads((LIVE/'new-kernel-identity-acceptance.json').read_text());proposal=json.loads((ROOT/'deployment-proposal.json').read_text())
 if approval.get('approved') is not True or approval.get('source')!='persistent_user_task_authorization' or approval.get('payload_sha256')!=proposal['payload_sha256'] or identity.get('accepted') is not True:raise RuntimeError('explicit approval / actual new boot identity missing')
 state=LIVE/'timer-capture-attempt.json'
 with state.open('x') as f:json.dump({'state':'collecting actual CAPS, not a DMA RUN','same_boot_ID':identity['boot_ID'],'automatic_retry':False},f,indent=2)
 samples=[]
 for i in range(3):
  prefix='timer-caps-'+str(i)
  result=subprocess.run([sys.executable,'-B',str(LIVE/'board_query.py'),'--command-file',str(LIVE/'pre-pilot-caps.command'),'--output-prefix',prefix,'--timeout','20'],capture_output=True,text=True)
  (LIVE/(prefix+'-host.stdout')).write_text(result.stdout);(LIVE/(prefix+'-host.stderr')).write_text(result.stderr)
  record=json.loads((LIVE/(prefix+'-identity-result.json')).read_text())
  if result.returncode:raise RuntimeError('actual CAPS observation failed; stop, no retry')
  sample=parse_probe(record)
  if sample['boot_ID']!=identity['boot_ID'] or sample['kernel_release']!=proposal['new_release']:raise RuntimeError('new chosen boot changed between actual samples')
  samples.append(sample)
  (LIVE/'actual-timer-samples-partial.json').write_text(json.dumps({'schema':'r107-finite-timer-samples-v1','samples':samples},indent=2)+'\n')
  if i<2:time.sleep(10)
 output=LIVE/'actual-timer-samples.json';output.write_text(json.dumps({'schema':'r107-finite-timer-samples-v1','samples':samples},indent=2)+'\n')
 analysis=subprocess.run([sys.executable,'-B',str(ROOT/'host/calibrate_finite_timer.py'),'--input',str(output),'--output',str(LIVE/'actual-timer-bounds.json')],capture_output=True,text=True)
 (LIVE/'actual-timer-analysis.stdout').write_text(analysis.stdout);(LIVE/'actual-timer-analysis.stderr').write_text(analysis.stderr)
 state.write_text(json.dumps({'state':'all three actual CAPS collected; analysis returned','analysis_returncode':analysis.returncode,'same_boot_ID':identity['boot_ID'],'actual_DMA_RUNs':0,'actual_SI_accuracy_evidence':'not supplied'},indent=2)+'\n')
 return analysis.returncode

if __name__=='__main__':
 try:raise SystemExit(main())
 except (OSError,KeyError,ValueError,RuntimeError) as exc:
  print('timer collection refused/stopped: '+str(exc),file=sys.stderr);raise SystemExit(2)

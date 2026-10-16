from pathlib import Path
import json,hashlib,re,datetime
W=Path('/home/xvhaocheng/work'); R=W/'iommu-platform/out/d-persistent-source-20261015';D=R/'documentation'; H=lambda b:hashlib.sha256(b).hexdigest()
receipt=D/'receipt.json'; archived=D/'receipt-before-focused-revision.json';assert not archived.exists(); archived.write_bytes(receipt.read_bytes());j=json.loads(receipt.read_text())
paths=[m['path'] for m in j['files']];current={p:(W/p).read_bytes() for p in paths}
base='iommu-platform/out/d-persistent-source-20261015'
short='SSD覆写→FTL/GC与DRAM行/刷新来源均成立，优先SSD。MQSim两次构建、4对8次冻结运行＋1次调试重放；覆写2048/4096/8192各缺1/1/2完成，原/观察XML一致。完成门未过（NOT_ESTABLISHED），不否定GC假设；暂不接P/PN。下一步只在固定2048输入核请求→child事务→CQE唯一身份链，不调参或扩矩阵，先正确性再建模；CS1本版局部停止保持。'
s=current['CURRENT_STATUS.md'].decode(); start=s.index('- SSD正常');end=s.index('\n- I有条件',start)
s=s[:start]+'- '+short+f'[来源/裁决]({base}/root/decision-analysis.md)／[接受]({base}/root/root-result-acceptance.json)／[下一合同]({base}/root/next-minimal-contract.json)。'+s[end:]
s=s.replace('实际2次构建、8次冻结运行＋1次只读调试重放，root仅独立只读核账。','实际2次构建；root审查补丁和运行器后亲自工具执行8次冻结运行＋1次GDB只读调试重放，并独立只读核账。')
nav=current['研究导航.md'].decode();start=nav.index('SSD正常覆写');end=nav.index('\n\n## 已有成果',start)
nav=nav[:start]+f'两来源已核且优先SSD，但MQSim三个覆写case缺完成，尚不能接P/PN；来源、观测信息边界、条件源码路径及费用未知项见[root裁决]({base}/root/decision-analysis.md)。下一关仅在固定2048输入取得请求→child事务→CQE唯一身份账本，先正确性再建模，不调参或扩矩阵；D主线、CS1局部停止、I/A保留及10月15—28日窗口不变，见[下一最小合同]({base}/root/next-minimal-contract.json)。'+nav[end:]
for p,b in current.items():assert (W/p).read_bytes()==b
(W/'CURRENT_STATUS.md').write_text(s);(W/'研究导航.md').write_text(nav)
# Verify protected bytes against unchanged original before-write snapshots.
old={p:(D/'before-write'/p).read_bytes() for p in paths};after={p:(W/p).read_bytes() for p in paths}
assert after['HANDOFF.md'].endswith(old['HANDOFF.md']);assert after['iommu-platform/docs/validation.md'].endswith(old['iommu-platform/docs/validation.md'])
assert old['CURRENT_STATUS.md'].split('## 各分支最新进展'.encode())[1].split('## 硬件与执行状态'.encode())[0] in after['CURRENT_STATUS.md']
assert after['CURRENT_STATUS.md'].endswith(old['CURRENT_STATUS.md'].split('## 独立工程事项'.encode())[1])
assert after['研究计划.md']==current['研究计划.md'];assert after['HANDOFF.md']==current['HANDOFF.md'];assert after['iommu-platform/docs/validation.md']==current['iommu-platform/docs/validation.md']
assert after['研究导航.md'].split('## 已有成果'.encode())[1]==old['研究导航.md'].split('## 已有成果'.encode())[1]
checks=[]
for p in paths+[str((R/'README.md').relative_to(W))]:
 text=(W/p).read_text()
 if p in ['HANDOFF.md','iommu-platform/docs/validation.md']:text=text[:-len(old[p].decode())]
 for match in re.finditer(r'\[[^\]]*\]\(([^)]+)\)',text):
  target=match.group(1).strip('<>')
  if target.startswith(('http:','https:','app:','codex:','#')):continue
  target=target.split('#')[0];q=Path(target) if target.startswith('/') else (W/p).parent/target
  checks.append({'document':p,'target':target,'exists':q.exists()})
assert all(c['exists'] for c in checks)
for row in j['files']:row['after_sha256']=H(after[row['path']])
j['actual_changes']['CURRENT_STATUS.md']='replace D summary with concise source/completion/next gate summary; focused revision compresses D to 190 characters before links and corrects root tool execution attribution; protected branch/engineering bytes retained'
j['actual_changes']['研究导航.md']='update D row; focused revision compresses next gate to two sentences plus source/root/next-contract links; remaining asset/code/history text retained'
j['focused_revision']={'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'prior_receipt':'documentation/receipt-before-focused-revision.json','before_hashes':{p:H(current[p]) for p in ['CURRENT_STATUS.md','研究导航.md']},'short_D_characters_before_links':len(short),'original_before_write_snapshots_untouched':True,'unchanged_other_three_canonical':True,'model_build_simulation_reruns':0,'changes':'compress status/nav and correct root execution attribution only'}
j['local_link_count']=len(checks);j['local_links']=checks
j['supporting_documentation_files'].append({'path':str(D/'focused_revision.py'),'before_sha256':None,'after_sha256':H((D/'focused_revision.py').read_bytes())})
receipt.write_text(json.dumps(j,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'short_D_chars':len(short),'links':len(checks),'protected_old_bytes':True,'other_three_canonical_unchanged':True},ensure_ascii=False))

#!/usr/bin/env python3
"""Package and statically verify the completed CVA6 source series."""
from pathlib import Path
import subprocess, json, os, hashlib, re, datetime
import organize as o

def sha256(data):
    return hashlib.sha256(data).hexdigest()

def main():
    frozen = json.loads((o.OUT / 'frozen-manifest.json').read_text())
    summary = json.loads((o.OUT / 'final-summary.json').read_text())
    steps = json.loads((o.OUT / 'steps.json').read_text())
    semantic = {
      'CVA6-DTI-COMPAT':['CVA6-UPSTREAM','IOMMU-A1'],
      'CVA6-INTEGRATION':['CVA6-DTI-COMPAT'],
      'CVA6-SC-FIX':['CVA6-UPSTREAM'],
      'CVA6-IOMMU-FIXES':['CVA6-INTEGRATION','IOMMU-F5'],
      'CVA6-DS-DELAY':['CVA6-DTI-COMPAT'],
      'CVA6-ACTIVE-CYCLES':['CVA6-UPSTREAM'],
      'CVA6-BUILD-CLEANUP':['CVA6-UPSTREAM'],
      'CVA6-LIFECYCLE':['CVA6-INTEGRATION'],
      'CVA6-BATCH':['CVA6-LIFECYCLE'],
      'CVA6-PATH':['CVA6-LIFECYCLE'],
      'CVA6-E3-TRACE':['CVA6-LIFECYCLE','CVA6-PATH','IOMMU-O1'],
      'CVA6-E3-ACTIVE':['CVA6-E3-TRACE','IOMMU-E1'],
      'CVA6-E3-OPPORTUNITIES':['CVA6-E3-TRACE','IOMMU-E1'],
      'CVA6-HELD-LAUNCH':['CVA6-BATCH','CVA6-E3-TRACE'],
      'CVA6-P4M-GAP':['CVA6-BATCH','CVA6-HELD-LAUNCH'],
      'CVA6-E2-COST':['CVA6-LIFECYCLE','CVA6-P4M-GAP','IOMMU-O1'],
      'CVA6-LOG-RAM':['CVA6-E2-COST'],
      'CVA6-LEGACY-E3-SELECT':['CVA6-E3-TRACE','CVA6-E2-COST'],
      'CVA6-COST-AGGREGATE':['CVA6-E2-COST'],
      'CVA6-RESEARCH-POLICY':['CVA6-BATCH','CVA6-E2-COST','IOMMU-O1'],
      'CVA6-QUIET-TAPS':['CVA6-BATCH'],
      'CVA6-FULLBUFFER-CERT':['CVA6-INTEGRATION'],
      'CVA6-FULLBUFFER-RAM':['CVA6-UPSTREAM'],
      'CVA6-NONBLOCK-OBS':['IOMMU-N1'],
      'CVA6-COMPOSITION-CONTROL':['CVA6-QUIET-TAPS','IOMMU-P2'],
      'CVA6-OBSERVER-V9':['CVA6-LIFECYCLE','CVA6-COMPOSITION-CONTROL'],
      'CVA6-BANKED-PN-WIRING':['CVA6-FULLBUFFER-CERT','CVA6-FULLBUFFER-RAM',
                               'CVA6-NONBLOCK-OBS','CVA6-COMPOSITION-CONTROL','CVA6-OBSERVER-V9','IOMMU-P2'],
    }
    effects = {
      'CVA6-DTI-COMPAT':'required interface compatibility; ATS remains disabled',
      'CVA6-INTEGRATION':'required four-DMA address/ID wiring; no research controls',
      'CVA6-SC-FIX':'functional correctness, delayed store-conditional response byte lane',
      'CVA6-IOMMU-FIXES':'child-source binding to independently separated correctness/service-order patches',
      'CVA6-DS-DELAY':'ACTIVE service timing fixture, DS input/output default 50/50',
      'CVA6-ACTIVE-CYCLES':'passive backend cycle counter',
      'CVA6-BUILD-CLEANUP':'build-tool housekeeping source; command not executed',
      'CVA6-LIFECYCLE':'passive lifecycle/AXI observation',
      'CVA6-BATCH':'ACTIVE autonomous job supply and completion-gated accounting',
      'CVA6-PATH':'passive per-ID path observation',
      'CVA6-E3-TRACE':'passive tracing with child O1; no PTW delay actuator',
      'CVA6-E3-ACTIVE':'ACTIVE optional PTW-delay enable/arm/trigger interface',
      'CVA6-E3-OPPORTUNITIES':'passive trigger-opportunity identity accounting',
      'CVA6-HELD-LAUNCH':'ACTIVE held-job launch/cancel coordination',
      'CVA6-P4M-GAP':'ACTIVE inserted batch gaps plus passive progress',
      'CVA6-E2-COST':'passive recorder with arm/window/configuration registers',
      'CVA6-LOG-RAM':'recorder memory implementation; resource/timing not measured here',
      'CVA6-LEGACY-E3-SELECT':'profile-selected observer elaboration and associated cost',
      'CVA6-COST-AGGREGATE':'passive aggregate recording',
      'CVA6-RESEARCH-POLICY':'ACTIVE admission/probe policy and accepted-command fixes',
      'CVA6-QUIET-TAPS':'passive gap and complete-idle signals',
      'CVA6-FULLBUFFER-CERT':'new research permission metadata; no direct handshake changes',
      'CVA6-FULLBUFFER-RAM':'research buffer storage selection in tracked generic FIFO vendor code',
      'CVA6-NONBLOCK-OBS':'passive read-only accepted-request/diagnostic observation module',
      'CVA6-COMPOSITION-CONTROL':'ACTIVE quiet-gated mode/clear configuration plus passive counters',
      'CVA6-OBSERVER-V9':'register ABI version distinguishes trimmed composition observer',
      'CVA6-BANKED-PN-WIRING':'ACTIVE research integration and atomic child ABI binding',
    }
    previous = 'CVA6-UPSTREAM'
    for row in steps:
        row.pop('dependencies', None)
        row['semantic_requires'] = semantic[row['feature_id']]
        row['stack_prerequisites'] = [previous]
        row['effect'] = effects[row['feature_id']]
        row['original_commits'] = [o.git('rev-parse', x, cwd=o.REPO).decode().strip() for x in row['original_commits']]
        row['source_layer'] = 'frozen-original-working-source' if not row['original_commits'] else 'existing-commit-source'
        row['author'] = o.git('show','-s','--format=%an <%ae>',row['commit'],cwd=o.REPO).decode().strip()
        row['author_date'] = o.git('show','-s','--format=%aI',row['commit'],cwd=o.REPO).decode().strip()
        previous = row['feature_id']
    (o.OUT / 'steps.json').write_text(json.dumps(steps,indent=2)+'\n')
    patches = o.OUT / 'patches'
    patches.mkdir(exist_ok=True)
    patch_names = o.git('format-patch','--no-signature','--full-index','--binary',
                        '--output-directory',str(patches),o.UP+'..'+summary['final'],cwd=o.REPO).decode().splitlines()
    assert len(patch_names) == len(steps)
    index = o.OUT / 'replay.index'
    env = dict(os.environ, GIT_INDEX_FILE=str(index))
    o.git('read-tree',o.UP,cwd=o.REPO,env=env)
    replay = []
    for row,name in zip(steps,patch_names):
        patch = Path(name)
        o.git('apply','--cached','--check',str(patch),cwd=o.REPO,env=env)
        o.git('apply','--cached',str(patch),cwd=o.REPO,env=env)
        tree = o.git('write-tree',cwd=o.REPO,env=env).decode().strip()
        assert tree == row['tree'], (row['feature_id'],tree,row['tree'])
        row['patch'] = str(patch.relative_to(o.OUT))
        row['patch_sha256'] = sha256(patch.read_bytes())
        replay.append(dict(feature_id=row['feature_id'],parent=row['parent'],commit=row['commit'],
                           patch=row['patch'],patch_sha256=row['patch_sha256'],
                           applies_to_recorded_parent=True,replayed_tree=tree,expected_tree=row['tree'],tree_matches=True))
    index.unlink()
    (o.OUT/'steps.json').write_text(json.dumps(steps,indent=2)+'\n')
    capture_patch = o.git('format-patch','--stdout','--no-signature','--full-index','--binary','-1',summary['capture'],cwd=o.REPO)
    (o.OUT/'capture.patch').write_bytes(capture_patch)
    bundles = []
    for name,ref,prereq in [
      ('cva6-chain.bundle','refs/heads/codex/organize-20261017-cva6-final',o.UP),
      ('cva6-capture.bundle','refs/heads/codex/organize-20261017-cva6-capture',o.OLD)]:
        bundle = o.OUT / name
        o.git('bundle','create',str(bundle),ref,'^'+prereq,cwd=o.REPO)
        verified = subprocess.run(['git','-C',str(o.REPO),'bundle','verify',str(bundle)],capture_output=True,check=True)
        bundle_head = o.git('bundle','list-heads',str(bundle),cwd=o.REPO).decode().strip()
        bundles.append(dict(path=name,sha256=sha256(bundle.read_bytes()),size=bundle.stat().st_size,
                            exported_ref=ref,prerequisite=prereq,list_heads=bundle_head,
                            verification_stdout=verified.stdout.decode(),verification_stderr=verified.stderr.decode()))
    recovery = o.ROOT / 'out/source-organization-20261017/bundle-check/cva6.git'
    subprocess.check_output(['git','init','--bare',str(recovery)],stderr=subprocess.STDOUT)
    for prereq in [o.UP,o.OLD]:
        subprocess.check_output(['git','-C',str(recovery),'fetch','--no-tags',str(o.REPO),prereq],stderr=subprocess.STDOUT)
    for item in bundles:
        subprocess.check_output(['git','-C',str(recovery),'fetch','--no-tags',str(o.OUT/item['path']),
                                 item['exported_ref']+':'+item['exported_ref']],stderr=subprocess.STDOUT)
    recovered_final=o.git('rev-parse','refs/heads/codex/organize-20261017-cva6-final',cwd=recovery).decode().strip()
    recovered_capture=o.git('rev-parse','refs/heads/codex/organize-20261017-cva6-capture',cwd=recovery).decode().strip()
    assert recovered_final==summary['final'] and recovered_capture==summary['capture']
    file_checks=[]
    for entry in frozen['source_entries']:
        if entry['kind']=='commit':continue
        path=entry['path']
        final_blob=o.git('show',summary['final']+':'+path,cwd=o.REPO)
        current=(o.REPO/path).read_bytes() if entry['mode']!='120000' else os.readlink(o.REPO/path).encode()
        final_hash=sha256(final_blob)
        current_hash=sha256(current)
        assert final_hash==entry['sha256']==current_hash, path
        file_checks.append(dict(path=path,mode=entry['mode'],size=entry['size'],frozen_sha256=entry['sha256'],
                               final_sha256=final_hash,original_checkout_current_sha256=current_hash,matches=True))
    (o.OUT/'file-identity-check.json').write_text(json.dumps(file_checks,indent=2)+'\n')
    old_refs={line.split()[1]:line.split()[0] for line in (o.OUT/'original-refs.txt').read_text().splitlines()}
    refs_now={line.split()[1]:line.split()[0] for line in o.git('show-ref',cwd=o.REPO).decode().splitlines()}
    assert all(refs_now.get(k)==v for k,v in old_refs.items())
    assert o.git('rev-parse','HEAD',cwd=o.REPO).decode().strip()==o.OLD
    assert o.git('symbolic-ref','--short','HEAD',cwd=o.REPO).decode().strip()=='platform'
    assert o.git('ls-files','--stage',cwd=o.REPO)==(o.OUT/'original-index.txt').read_bytes()
    assert o.git('diff','--binary',o.OLD,cwd=o.REPO)==(o.OUT/'original-dirty.patch').read_bytes()
    assert o.git('status','--porcelain=v1','--untracked-files=all',cwd=o.REPO)==(o.OUT/'original-status.txt').read_bytes()
    assert o.git('diff','--name-only',summary['capture'],summary['final'],cwd=o.REPO).decode().splitlines()==[o.LINK]
    baseline={}
    for label,sha in [('adapted',summary['adapted']),('fixed',summary['fixed'])]:
        periph=o.source(sha,o.P).decode();top=o.source(sha,o.TOP).decode();front=o.source(sha,o.FRONT).decode()
        banned=['IOMMUDSFixedDelay','i_iommu_ds_delayer','E3Enable','e3_ctl','IOMMUResearchRole','IOMMUNonblockMode','obs_issue','batch_active','active_cycles']
        found=[word for word in banned if word in periph or word in top or word in front]
        assert not found,(label,found)
        assert 'NumDMADevices = 4' in periph and 'dma_cfg.aw_addr[13:12]' in periph
        assert '`AXI_ASSIGN_FROM_REQ(iommu_ds, axi_iommu_ds_req)' in periph
        baseline[label]=dict(commit=sha,no_research_boundary_tokens=True,forbidden_tokens_checked=banned,
                             four_dma_and_address_decode=True,direct_ds_path=True,
                             iommu_gitlink=o.git('ls-tree',sha,o.LINK,cwd=o.REPO).decode().split()[2])
    verification=dict(timestamp=datetime.datetime.now(datetime.timezone.utc).isoformat(),scope='STATIC SOURCE/GIT ONLY',
       builds_experiments_simulation_synthesis_board_runs=0,patch_replay=replay,
       all_patch_trees_match=True,source_file_count=len(file_checks),all_frozen_regular_source_bytes_match=True,
       final_capture_only_difference=o.LINK,baseline_boundary_checks=baseline,
       original_checkout_head_branch_index_status_diff_and_old_refs_unchanged=True,
       original_ref_count=len(old_refs),new_refs={k:v for k,v in refs_now.items() if k not in old_refs},
       bundle_recovery=dict(directory=str(recovery),prerequisites_imported_from_fixed_local_history=[o.UP,o.OLD],
                            recovered_final=recovered_final,recovered_capture=recovered_capture,exact_commit_sha_recovered=True),
       bundles=bundles,not_verified=['HDL elaboration','synthesis','timing','runtime correctness','performance','arbitrary feature combinations'])
    (o.OUT/'verification.json').write_text(json.dumps(verification,indent=2)+'\n')
    (o.OUT/'manifest.json').write_text(json.dumps(dict(schema=1,date='2026-10-17',component='CVA6 including tracked iDMA and generic FIFO vendor deltas',
       model_attribution='Codex only; no DeepSeek; native model metadata unavailable in worker tools; requested inherited Sol/ultra.',
       anchors=summary,upstream=dict(url='https://github.com/zero-day-labs/cva6.git',ref='refs/heads/feat/iommu-refact',commit=o.UP,
                                   proof='upstream-ls-remote.txt',original_iommu_gitlink='f8f918aa251209a8b7fafde89ba46a01cf0f7c68',
                                   original_child_object_exists=True),
       dependency_contract='stack_prerequisites records the actual patch order/context. semantic_requires records feature/data/ABI needs; a missing ancestor is not a claim that the function needs that ancestor. No arbitrary combinations have been built.',
       iommu_bindings=dict(A1='7bea3f202903279c18df99a2284d6f56ccdacdc6',F5='85b157ebe71a623e326e51fff7b7126fe77e8eb9',
                           O1='5304477888b627546fa5f46ef36735f78f0ee504',E1='027583cd564af8ccb0cc05228756afa8a322d017',P2=summary['final_iommu_gitlink']),
       steps=steps,frozen_source='frozen-manifest.json',verification='verification.json',
       provisional_replay=dict(ref='refs/heads/codex/organize-20261017-cva6-chain',purpose='Initial regular-source-only replay retained for audit; lacks per-layer child research bindings. Not an endorsed build profile.'),
       source_boundary='CVA6 tracked ordinary files plus three necessary untracked RTL modules. Child gitlinks retain separate ownership; no vendor/build tree was copied for padding.',
       bundles=bundles),indent=2)+'\n')
    print(json.dumps(dict(steps=len(steps),source_files=len(file_checks),final=summary['final'],
                          all_static_checks_passed=True,bundles=[x['path'] for x in bundles]),indent=2))

if __name__=='__main__':main()

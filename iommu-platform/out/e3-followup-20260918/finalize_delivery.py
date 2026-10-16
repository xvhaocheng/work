#!/usr/bin/env python3
"""Finalize only after the foreground FPGA command exits successfully."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib, json, shutil
root=Path(__file__).resolve().parents[2]
out=root/'out/e3-followup-20260918'
dest=out/'delivery'
profile='migration-4dma-e3-queue'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
manifest={k:json.loads((root/f'out/manifests/{k}-{profile}.json').read_text()) for k in ['software','fpga']}
for action,m in manifest.items():
    assert m['status']=='passed', (action,m['status'])
    assert m['sources_lock_sha256']==sha(root/'sources.lock.json')
    assert all((root/f).is_file() and (root/f).stat().st_size and sha(root/f)==h for f,h in m['artifacts'].items())
bit=root/f'out/{profile}/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit'
mcs=bit.with_suffix('.mcs')
payload=root/'out/opensbi/platform/fpga/ariane/firmware/fw_payload.bin'
assert str(bit.relative_to(root)) in manifest['fpga']['artifacts']
assert mcs.is_file() and mcs.stat().st_size, 'Missing or empty MCS (not listed by the standard manifest)'
validation=json.loads((out/'validation.json').read_text())
assert validation['status']=='passed'
for f,h in validation['sources_sha256'].items():
    assert sha(root/f)==h, ('post-test source change',f)
    if f.startswith('cva6/'):
        assert sha(root/f'out/{profile}/fpga-src'/f)==h, ('staged source mismatch',f)
lock=json.loads((root/'sources.lock.json').read_text())
versions={x['path']:x['commit'] for x in lock['components'] if x['path'] in ['cva6','cva6/corev_apu/riscv-iommu','linux']}
status=json.loads((out/'build-status.json').read_text())
status.update({'fpga_status':'passed','fpga_exit_code':0,'fpga_manifest_status':'passed','fpga_completed_at':manifest['fpga']['timestamp'],'bit_sha256':sha(bit),'mcs_sha256':sha(mcs),'payload_sha256':sha(payload),'critical_staged_sources_match_tested_sources':True,'artifact_hash_verification':'passed','updated_at':datetime.now(timezone.utc).isoformat()})
(out/'build-status.json').write_text(json.dumps(status,indent=2,ensure_ascii=False)+'\n')
dest.mkdir(exist_ok=True)
files={
    'ariane_xilinx.bit':bit,'fw_payload.bin':payload,
    'idma_e3_pair':root/'out/bench/idma_e3_pair',
    'idma_experiment':root/'out/bench/idma_experiment',
    'platform-timeout':root/'out/bench/platform-timeout',
    'idma.h':root/'linux/include/uapi/linux/idma.h',
    'E3_QUEUE_PAIR.md':root/'E3_QUEUE_PAIR.md',
    'E3_DIAGNOSTIC.md':root/'E3_DIAGNOSTIC.md',
    'E3_SERVICE.md':root/'E3_SERVICE.md',
    'e3-diag-results-review.md':root/'docs/e3-diag-results-review.md',
    'sources.lock.json':root/'sources.lock.json',
    'profile.json':root/f'configs/{profile}.json',
    'software-manifest.json':root/f'out/manifests/software-{profile}.json',
    'fpga-manifest.json':root/f'out/manifests/fpga-{profile}.json',
    'validation.json':out/'validation.json','build-status.json':out/'build-status.json',
    'controller-test-result.json':out/'controller-test/result.json',
    'diag-server-review.json':root/'out/mac-e3-diag-main-20260918-081411-run-1z6a407q/server-review.json',
}
for name,p in files.items(): shutil.copy2(p,dest/name)
# Keep documentation links usable when this directory is copied alone.
for name in ['E3_QUEUE_PAIR.md','E3_DIAGNOSTIC.md','E3_SERVICE.md']:
    p=dest/name;s=p.read_text()
    s=s.replace('docs/e3-diag-results-review.md','e3-diag-results-review.md')
    s=s.replace('out/e3-followup-20260918/delivery/README.md','README.md')
    p.write_text(s)
p=dest/'e3-diag-results-review.md'
p.write_text(p.read_text().replace('../E3_QUEUE_PAIR.md','E3_QUEUE_PAIR.md').replace('../E3_DIAGNOSTIC.md','E3_DIAGNOSTIC.md'))
delivery={'profile':profile,'software_exit_code':0,'fpga_exit_code':0,'kernel_release':status['kernel_release'],'build_start_platform_commit':manifest['software']['build_start']['platform_commit'],'fpga_build_start_platform_commit':manifest['fpga']['build_start']['platform_commit'],'fpga_manifest_platform_commit':manifest['fpga']['platform_commit'],'versions':versions,'bit_sha256':sha(bit),'payload_sha256':sha(payload),'mcs_source_path':str(mcs),'mcs_manifest_note':'Standard platform manifest records BIT but not MCS; MCS is nonempty and its SHA256 is recorded separately here.','mcs_sha256':sha(mcs),'artifact_hash_verification':'passed','successful_build_logs_read':False,'board_tested':False,'timing_report_reviewed':False,'source_note':'Documentation-only commits may follow build start; component commits, lock and tested critical sources match. Existing documentation dirty changes retained.','recorded_at':datetime.now(timezone.utc).isoformat()}
(dest/'delivery.json').write_text(json.dumps(delivery,indent=2,ensure_ascii=False)+'\n')
readme=f'''# E3 v5 等待识别与带诊断配对：完整交付

2026-09-18：软件和 FPGA 完整构建均退出 0、manifest passed，BIT/MCS 非空，清单哈希及关键导出源码与已测试源码一致。新版尚未上板。成功日志只保存，未审查时序报告，原时钟/CDC/IO 审查边界保留。

必须同时使用本目录的新 BIT 和 SD payload。保持 DS50/50、V0/A1、4 KiB 硬件接续及 CPU 整批轮询。

- `ariane_xilinx.bit`：observer v5 / E3 bank v2，修正 V 排在 A 读之后时的触发识别，加入门槛、前史和两条插入请求记录。
- `fw_payload.bin`：按已有 SD 原始分区流程更新；内核 `{status['kernel_release']}`。新程序已内置 rootfs，不能只在旧系统替换 CLI。
- [E3_QUEUE_PAIR.md](E3_QUEUE_PAIR.md)：本轮逻辑、完整字段、三对短验收与十对正式矩阵。新配对 `--diagnostic 2`；D0/D0 对照仍用 `--diagnostic 1`。采集器需接受新 schema 和 bank v2。
- [结果分析](e3-diag-results-review.md)：已完成的 00085 D0/D0 数据及解释，和新版实板结果区分。
- `idma_e3_pair`、`idma_experiment`、`platform-timeout`、`idma.h`：程序和配套接口；其余 JSON 与 `SHA256SUMS` 保存版本、验证和校验记录。

BIT SHA256：`{sha(bit)}`。
payload SHA256：`{sha(payload)}`。
构建起点：`{delivery['build_start_platform_commit']}`；CVA6 `{versions['cva6']}`，IOMMU `{versions['cva6/corev_apu/riscv-iommu']}`，Linux `{versions['linux']}`。文档后续提交不改变被编译组件。原文档脏改动保留在 manifest 的 dirty 标志中。

主机验证：真实控制器/FIFO/demux/等待计数/延迟开关各15场景；E3及旧observer/path RTL回归；驱动76控制+3缓存+96小chunk；E3 CLI49、旧CLI69、rootfs5。模型与编译成功不能代替上板验收。新增触发前摘要/队头需在结果回收时继续核对语义，不能只凭 success 判断因果成立。

先三对短验收，通过后仅十对正式：2对A8 D0/D0、6对A8 D0/D128交替首轮D、2对A1控制，顺序见协议。保存26子轮的全部JSON/diagnostic/256个E3字段、按sequence收齐494条观测、UART/退出码/taint和版本来源。真实错误停，无触发或不可比不补采。若可比性仍不足，再设计有限请求或共同启动，本轮不扩展。

历史原始数据保留在 Dell 工程中，分析文中的原始数据链接未随交付复制。软件日志在上一级 `software.log`，FPGA 成功日志在 `fpga.log`；首次许可证连接失败的日志及构建树另存，未覆盖旧实验产物。MCS 保存在 delivery.json 的 `mcs_source_path`，上板常用本目录 BIT。没有本机刷卡、JTAG 或板卡操作。

DeepSeek 整理原始证据并提供 CLI/桩/CLI 测试提案；Codex 独立复核原始数据与关键代码，负责 RTL/驱动、应用审查提案、运行验证与编译。
'''
(dest/'README.md').write_text(readme)
rows=[f'{sha(p)}  {p.name}' for p in sorted(dest.iterdir()) if p.is_file() and p.name!='SHA256SUMS']
(dest/'SHA256SUMS').write_text('\n'.join(rows)+'\n')
for row in rows:
    h,name=row.split('  ',1);assert sha(dest/name)==h
print(json.dumps({'delivery':str(dest),'files':len(rows),'kernel':status['kernel_release'],'bit_sha256':sha(bit),'payload_sha256':sha(payload),'fpga_completed_at':manifest['fpga']['timestamp']},ensure_ascii=False,indent=2))

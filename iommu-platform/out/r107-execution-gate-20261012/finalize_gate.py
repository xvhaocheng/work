#!/usr/bin/env python3
"""Save the root prerequisite decision before synchronizing project records."""
import hashlib
import json
from pathlib import Path
from datetime import datetime, timezone

BASE = Path(__file__).resolve().parent
PLATFORM = BASE.parent.parent
WORK = PLATFORM.parent
NOW = datetime.now(timezone.utc).isoformat()


def save(name, value):
    (BASE / name).write_text(json.dumps(value, ensure_ascii=False, indent=2) + '\n')


live = json.loads((BASE / 'identity/current-live-evidence.json').read_text())
decision = {
    'stage': 109, 'recorded_at_utc': NOW,
    'thread_id': '01a12eff-c971-7c62-8071-a08b646a4b61',
    'decision': 'conditional_retain_R107_contract_asset_execution_prerequisites_not_closed',
    'bounded_question_answered': True,
    'overall_plan_stage': 'phase1 problem and method selection; no new G0/G1',
    'ordinary_pilot_entry_passed': False,
    'new_G0_registered': False, 'new_G1_designed': False,
    'method_paper_complete': False,
    'basis': 'stage108 accepted result and exact unchanged next-goal-contract.json',
    'accepted_positive_results': [
        'Fresh board Linux authenticated over Dell UART; new boot ID and riscv64 kernel observed.',
        'Live DTB hash equals local recorded DTB; live 84-byte kernel-note hash equals local ELF notes embedded in Image.',
        'Four idma nodes and eth0 192.168.77.2/24 UP/LOWER_UP observed; no blind board SSH test or Dell IP change.',
        'Optimized route-opt bit f9fa9434... matches preserved delivery package; 24 staged overlay files reconcile.',
        'Optimized raw constrained timing +0.022ns WNS/+0.051ns WHS, 50MHz static CPU clock, failing endpoints0.',
        'USB adapter discovery and exact JTAG target access established; wildcard double-interface selector error resolved.',
        'One necessary finite ordinary adapter scope reviewed, without implementation or new RTL.',
    ],
    'rejected_evidence_substitutions': [
        'Original f15f8a72... bit has -0.219ns WNS and1 failing setup endpoint; cannot borrow optimized timing.',
        'Historical22/22 and old boot ID cannot prove this boot, loaded bit or current mode.',
        'JTAG client exit0 with Unknown_Device IDCODE0x00000001 does not identify xc7k325t or bit.',
        'Build mode3, reset S0, DT50MHz and kernel release are not current live mode, physical clock or source freeze.',
        'Missing ordinary adapter, old correctness defects or unverified prerequisites are not scientific translation loss.',
    ],
    'actual_live_boot_id': live['actual_boot_id'],
    'actual_live_kernel_release': live['actual_kernel_release'],
    'current_live_bit_sha256': None, 'current_runtime_mode': None,
    'current_physical_CPU_frequency_Hz': None,
    'current_DT_declared_CPU_frequency_Hz': 50000000,
    'current_DT_declared_timebase_frequency_Hz': 25000000,
    'current_JTAG_IDCODE': '0x00000001', 'JTAG_unknown_device_cause': 'unknown',
    'kernel_exact_dirty_source_and_config_freeze': None,
    'IO_CDC_full_signoff': 'unverified; raw reports retain6 clock critical warnings,21/23 missing input/output delays',
    'ordinary_full_prepare_restore_costs': None,
    'ordinary_translation_specific_residual_loss': None,
    'specific_new_facts_required': [
        'Resolve the currently unknown JTAG chain/device result with evidence; identify this boot actual bit/source/profile and runtime ordinary S0 without treating a reset constant as an observation.',
        'Freeze exact kernel sources/configuration matching a chosen reproducible ordinary adapter build; current blob/build note identity does not provide dirty source freeze.',
        'Give a specific justified disposition of relevant clock/IO/CDC constraint gaps before corresponding FPGA performance claims; retain the bounded internal timing result.',
        'After identity/interface gates close, implement only reviewed ordinary stable-pool/manifest/locked-snapshot/cache/fence/completion glue; account known old private-only credit correctness fix or lawful segmentation, without treating it as an innovation.',
        'Capture full finite ordinary preparation/completion/teardown costs and actual mapping/leaves/cache/fence/drain; only a reachable avoidable translation-related residual can enter a new minimal G0.',
    ],
    'why_conditional_not_archive_now': 'New current board software evidence and a reusable optimized artifact exist. The remaining facts are concrete execution identity/interface questions; no ordinary-uncovered loss is established.',
    'archive_condition': 'If the identified platform cannot provide a lawful finite interface, or complete ordinary controls/costs close the desired gap, archive this R107 card; do not repeat unchanged source/CPU/static identity work.',
    'route_after_archive': 'Return to phase1 using retained literature and accepted E2/E3 boundaries to formulate a new explicit demand/ordinary-failure contract. No automatic revival of closed row/HERO/cold-weight/copy candidates or third scene.',
    'next_goal_registered': False,
    'evidence': [
        'identity/current-live-evidence.json', 'identity/composition-current-gate-result.json',
        'identity/composition-current-evidence.json', 'route/jtag-route-diagnosis.json',
        'route/live-authenticated-UART.raw.log', 'route/live-kernel-notes-UART.raw.log',
        'route/live-JTAG-enumeration.raw.log', 'route/live-JTAG-exact.raw.log',
        'critical/ordinary-adapter-scope.json', 'validation/protected-inputs-check.json',
    ],
}
save('root-result-acceptance.json', decision)
save('next-goal-contract.json', {
    'kind': 'inactive new-fact-gated continuation; not registered goal or G0',
    'scope': 'Same R107 identity/interface closure only, then one ordinary pilot if all prerequisites close',
    'new_facts': decision['specific_new_facts_required'],
    'pilot_contract': '../r107-bounded-decision-20261012/next-goal-contract.json',
    'bounds': {'workflows': 1, 'new_scenes': 0, 'stage106_or_stage108_rerun': False,
               'new_RTL_before_gap': False, 'pressure_compute_deadline_search': False},
    'stop': decision['archive_condition'], 'route_after_archive': decision['route_after_archive'],
    'G0': 'Concrete reachable avoidable ordinary-uncovered translation loss with complete costs first',
    'G1': 'Only after accepted G0 and original prior-work/full-cost review',
    'successor_goal_registered': False, 'paper_complete': False,
})
save('execution-ledger.json', {
    'stage': 109, 'recorded_at_utc': NOW, 'real_goal_registered': True, 'extra_token_budget': None,
    'authorship_and_execution': 'Codex root plus2 exclusive Sol/ultra seats; no DeepSeek/Claude/Astra',
    'child_ledgers': ['identity/execution-ledger.json', 'identity/composition-current-execution-ledger.json',
                      'route/execution-ledger.json', 'route/jtag-route-diagnosis-ledger.json'],
    'root_board_actions': [
        {'action': 'initial one framed UART identity attempt at login', 'accepted': False, 'receipt': 'route/live-identity-result.json'},
        {'action': 'one board login after explicit user reply; credential not logged', 'accepted': True, 'receipt': 'route/board-login-receipt.json'},
        {'action': 'fresh framed Linux/DTB/idma identity query', 'accepted': True, 'receipt': 'route/live-authenticated-identity-result.json'},
        {'action': 'same-boot build notes/architecture/DT clocks/network query', 'accepted': True, 'receipt': 'route/live-kernel-notes-identity-result.json'},
        {'action': 'initial JTAG selector query', 'accepted': False, 'receipt': 'route/live-jtag-result.json'},
        {'action': 'one targeted enumeration exposing2 matches', 'accepted': True, 'receipt': 'route/live-jtag-enumeration-result.json'},
        {'action': 'exact main target1MHz status query; unknown ID retained', 'accepted': 'route only; FPGA identity not accepted', 'receipt': 'route/live-jtag-exact-result.json'},
    ],
    'host_authorized_changes': {
        'temporary_exact_device_ACLs': 2, 'sudoers_group_udev_or_driver_install_changes': 0,
        'Dell_credential': 'user-requested SecretService storage and nonsecret persistent reference; no value in artifacts',
        'model_settings_cache_or_historical_record_changes': 0,
        'task_owned_hw_server_starts_and_stops': 3, 'existing_service_restarts': 0,
        'network_address_changes': 0,
    },
    'Mac_access': {'authorized_SSH_reference_reads': True, 'task_files_copied': 4,
                   'receipt': 'route/mac-reference-fetch-record.json', 'remote_mutations_or_experiments': 0},
    'experimental_counts': {'R107_hardware_jobs': 0, 'FPGA_builds_or_flashes': 0,
                            'GPU_runs': 0, 'DUT_runs': 0, 'physical_pool_allocations_or_maps': 0,
                            'ordinary_adapter_implementations': 0, 'stage106_stage108_replays': 0},
    'protected_inputs': {'count': 129, 'unchanged': 129, 'receipt': 'validation/protected-inputs-check.json'},
    'root_critical_review': 'Original raw timing, staged top/buffers/mode reset, current Linux ioctl/caps boundary, raw UART/JTAG evidence and exact local ELF notes; child summaries are not sole proof',
    'diagnostic_limits': 'Malformed/path-missing host read attempts and failed initial selector/query are diagnostics, never scientific negatives; original evidence preserved. No unchanged repeated matrix/check/build.',
    'cost_status': 'Only bounded host/query receipts have actual intervals; application prepare/restore/cache/leaf/fence/drain/teardown costs remain unknown.',
    'goal_completion_receipt': 'goal-completion.json after deliveries and checks pass',
})

report = '''# R107执行前提有界裁决（stage109，2026-10-12）

**条件保留R107有限合同资产。本轮执行前提没有全部通过，不进入普通恢复试验、新G0或G1。** 已核当前Dell可访问的板端Linux；历史构建中确有可复用的优化bit。当前载入bit、运行模式和完整内核源码冻结仍未验实，JTAG未知器件的原因未知。缺实现或身份不明不作为普通方法的科学失败。本关口完成不代表方法论文完成。

本轮具体问题是：同一R107有限回放是否已经具备可辨认、可访问的实际执行环境，以及一个必要且有限的普通adapter范围。依据是stage108稳定裁决及其未改的next-goal-contract；进入条件为实际镜像／source／mode／clock／timing／route与接口闭合，结束条件为推进一次普通采证、列明新事实的条件保留，或有理由地归档。实际goal工具登记，无额外token预算。

| 项目 | 本轮实际证据 | 裁决与限制 |
|---|---|---|
| 当前Linux | UART成功登录；riscv64；6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty；taint0 | 两次只读查询同一新boot ID：db0664f0-2d67-4b0d-91fb-c67d7557abe6；非历史boot |
| 当前DTB／内核构建身份 | live DTB SHA256 aa9cdf4c…与本地3510B完全一致；live84B kernel notes SHA256 9dc6815e…与本地ELF一致 | notes精确包含在Image中，GNU build ID 1e019b55…；证明构建标记对应，不是全运行内存哈希或dirty源码冻结 |
| 当前访问 | /dev/idma0–3；eth0 UP/LOWER_UP、192.168.77.2/24 | Dell UART访问成立；未改Dell网络地址、未把旧Mac网段当本次SSH成功 |
| 可复用优化bit | route-opt/ariane_xilinx.bit，SHA256 f9fa9434…，11,443,722B；与Mac交付包相同 | 静态冻结overlay24项可对齐；不能推出当前运行此bit |
| 两份原始时序 | 原manifest f15f8a72…：WNS−0.219ns，setup失败1；优化版：WNS+0.022ns／WHS+0.051ns，失败0 | 原版拒绝借用优化结果；优化内部约束50MHz通过，但保留6 clock critical warnings、21/23缺输入／输出delay与IO/CDC未完成边界 |
| 当前JTAG | 旧wildcard同时选2目标，已用完整末段修正；主目标1MHz打开成功 | 实际PART=Unknown_Device、IDCODE=0x00000001；原因未知。用户确认电源开、有亮灯，仍不能填入xc7k325t或载入bit |
| 运行模式与时钟 | 冻结build mode3，reset S0；缓冲512/512/3/3。当前DT声明CPU50MHz、timebase25MHz | live S/N/P/PN及物理频率未测；build mode3不是运行PN3，reset常量不是本次观测 |
| 内核与普通接口 | Image／DTB及build note身份明确；完整dirty源码／config冻结未知 | 已核caps与RUN_COMPOSITION边界；caps2不提供当前composition mode，RUN会配模式并执行任务，不为读身份跑旧实验 |

原始入口：[本次live证据与完整hash](../out/r107-execution-gate-20261012/identity/current-live-evidence.json)、[认证后UART原日志](../out/r107-execution-gate-20261012/route/live-authenticated-UART.raw.log)、[同boot构建与网络原日志](../out/r107-execution-gate-20261012/route/live-kernel-notes-UART.raw.log)、[JTAG完整枚举](../out/r107-execution-gate-20261012/route/live-JTAG-enumeration.raw.log)、[实际未知器件读取](../out/r107-execution-gate-20261012/route/live-JTAG-exact.raw.log)。[静态构建证据及精确原文行号](../out/r107-execution-gate-20261012/identity/composition-current-evidence.md)分列优化前后bit；root亲核优化原报告159/174行、原版159行、模式／缓冲源码及当前Linux ioctl调用链。

普通adapter范围已成立且有必要：stage108仅有来源／CPU全字节回放，只有同实例普通路径实测才能补实际准备和完成成本。范围是一个可信owner、四稳定池、不可变manifest、完整144笔或合法合并96笔／1,769,472B恢复、24 worker-layer和12共同数据就绪标记。663,552B仍只是CPU checkpoint代理标记，不是native attention必读量；没有原生GPU计算／到达／期限。

保留四完整4MiB/order10或八2MiB/order9、16MiB backing、目标8叶及合法4KiB回退，允许预映射／驻留／批量和授权singleDID合并。Linux大小上可行不等于此次实际分配成功，8目标叶小于16项不证明零miss。实现须提供内核所有的池／边界校验、映射与真实叶、可靠fence/cache handoff、NEXT/DONE完整完成、层依赖、drain/reacquire及全字节／guard验证；普通安全snapshot需受同一锁保护。旧stage106私有credit修复未包含在此bit，不能把其已验结果冒称当前生产正确性；必要修复或合法分段属于普通实现成本。

全部准备／初始化／此前swapout／map／leaf／fence／cache／issue／done／CPU代理／drain／reacquire／verify／unmap／free成本目前未知；首次、稳态、销毁以及失败／回退／隔离费用必须分列。应用逻辑四池9MiB、普通backing16MiB等为既已核的资源合同，不是本轮已分配资源。此轮未实现adapter、分配／映射池、运行恢复或取得翻译损失。[明确接口范围](../out/r107-execution-gate-20261012/critical/ordinary-adapter-scope.json)、[已接受完整成本清单](../out/r107-bounded-decision-20261012/platform/cost-inventory.json)。

需要的新事实依次为：当前有效FPGA链／载入bit的可信身份与普通S0观测；匹配必要adapter的精确内核源码／config冻结；对相应性能结论涉及的clock／IO／CDC缺口给出具体处置。当前Linux已经可读，不再把sudo、串口登录、线缆未接或无driver作为待办。现有Linux保持运行，UART已释放；未为证明历史bit而自动刷写、复位或重启。

这些事实齐备后，才必要实现并执行同一有限普通准备／完成采证，复用已验校验器。普通强对照未覆盖的可达、可避免翻译相关残余成立，才登记最小G0；G0支持后才G1。若平台不能提供合法接口，或完整普通对照／成本已覆盖所需目标，归档这张R107卡，回到整体阶段一以保留文献及E2/E3边界形成新的明确需求合同；不会自动重开行预取、HERO、冷权重或copy等关闭分支，也不会自动增加第三场景。[未激活后继合同](../out/r107-execution-gate-20261012/next-goal-contract.json)。

实际执行包括Mac授权只读参考、当前UART登录／两次成功身份查询（最初未登录查询失败保留）、3次有新证据理由的JTAG步骤。主机仅执行用户授权的两个设备临时ACL和本机系统凭据库保存／非秘密复用引用；未安装driver、改sudoers／用户组、改网络、重启既有服务。密码未进入项目、报告或串口日志。全部工作由Codex root＋两独占Sol／ultra席完成，零DeepSeek／Claude／Astra、旧矩阵重跑、新DUT／FPGA构建／flash／GPU运行。129个stage108及旧输入文件保持不变。[实际goal登记](../out/r107-execution-gate-20261012/goal-registration.json)、[稳定裁决](../out/r107-execution-gate-20261012/root-result-acceptance.json)、[执行账本](../out/r107-execution-gate-20261012/execution-ledger.json)、[核验记录](../out/r107-execution-gate-20261012/validation/root-evidence-check.json)。
'''
(PLATFORM / 'docs/r107-execution-gate-20261012.md').write_text(report)

paths = [WORK / 'CURRENT_STATUS.md', WORK / 'HANDOFF.md', PLATFORM / 'docs/validation.md',
         PLATFORM / 'docs/research-execution-plan-20261011.md', WORK / '研究计划.md']
snapshots = {}
for path in paths:
    data = path.read_text()
    dest = BASE / 'before' / ('before-final-' + path.name)
    if dest.exists():
        raise RuntimeError('Final document snapshot already exists; do not repeat unchanged finalization')
    dest.write_text(data)
    snapshots[str(path)] = hashlib.sha256(data.encode()).hexdigest()
save('before/final-document-inputs.json', snapshots)

status_path = WORK / 'CURRENT_STATUS.md'
old_status = status_path.read_text()
tail_start = old_status.index('\n- 独立Linux贡献：')
tail = old_status[tail_start:]
status_path.write_text('''# 当前状态 stage109 2026年10月12日 R107执行前提有界裁决完成

- 整体仍为第一阶段“问题与方法立题”。实际goal已登记、无额外token预算；本轮条件保留R107合同资产，执行前提未全过，无普通恢复试验／新G0／G1或论文完成声明。[稳定裁决](iommu-platform/out/r107-execution-gate-20261012/root-result-acceptance.json)。
- 已在Dell串口取得当前riscv64／cpn3-clean Linux，新boot ID db0664f0-2d67-4b0d-91fb-c67d7557abe6，taint0、四idma节点。live DTB与本地hash完全一致；live kernel notes与本地ELF且Image内84B notes一致。eth0为192.168.77.2/24、UP/LOWER_UP；没有改Dell网络或把旧SSH当本次成功。[当前原始证据](iommu-platform/out/r107-execution-gate-20261012/identity/current-live-evidence.json)。
- 可复用候选仅为composition route-opt bit f9fa9434…，冻结24项overlay可对齐，内部50MHz时序WNS+0.022／WHS+0.051ns、失败0。原manifest f15f8a72…为−0.219ns／setup1失败，不能混用；6 clock critical warnings及IO/CDC缺口仍在。[构建证据](iommu-platform/out/r107-execution-gate-20261012/identity/composition-current-evidence.md)。
- JTAG旧wildcard选中两个接口的错误已修；精确主目标1MHz访问成功但PART=Unknown_Device、IDCODE0x00000001，原因未知。当前载入bit、live S/N/P/PN、物理时钟及精确dirty内核源码/config冻结未验实。DT声明CPU50MHz/timebase25MHz，不代替实测；buildmode3/reset S0不代替运行模式。[JTAG原始诊断](iommu-platform/out/r107-execution-gate-20261012/route/jtag-route-diagnosis.md)。
- stage108完整OPT125m有限合同保留：3唯一槽、144完整恢复／1,769,472B、此前换出同量；只source/CPU回放，无native trace/GPU计算。663,552B只是proxy标记。普通96笔合并、同owner单DID、4×4MiB或8×2MiB、合法4KiB/premap/residency/batch均保留；未重跑旧矩阵或CPU回放。[有限合同与成本](iommu-platform/docs/r107-bounded-decision-20261012.md)。
- 必要ordinary adapter范围已核，未实现／分配／映射／恢复；实际leaf/fence/cache/全成本及翻译残余仍未知。下一只补有效FPGA链／本次bit和S0、精确内核冻结、相关时序处置等新事实，再执行同实例普通采证。若接口不可达或普通覆盖则归档此卡，回到阶段一新需求合同，不自动恢复关闭候选／扩场景。未登记后继goal。[下一合同](iommu-platform/out/r107-execution-gate-20261012/next-goal-contract.json)。
- 本轮Codex root＋两Sol／ultra独占席；零DeepSeek／Claude／Astra、新build/DUT/flash/GPU/恢复性能试验。实际有UART登录／只读身份与JTAG采证，两个授权临时设备ACL；现有Linux保持运行、UART释放。129个旧输入不变。[本轮报告](iommu-platform/docs/r107-execution-gate-20261012.md)／[账本](iommu-platform/out/r107-execution-gate-20261012/execution-ledger.json)。
- 本新主会话和独占核验子席实际gpt-6.1-sol／ultra、原生500,000／有效475,000已验实，最大支持872,000分列，未改模型配置；旧258,400历史保留。[本次模型核验](codex-sol-runtime-verification-20261012T072912Z-01a12eff/REPORT.md)。
''' + tail)

handoff = '''## stage109 R107执行前提裁决（2026-10-12）

本轮真实goal登记且无额外预算；结论条件保留R107合同资产，actual execution前提未全过，无ordinary恢复／新G0／G1，论文未完成。当前Dell UART实际登录成功，riscv64/cpn3-clean、boot db0664f0-2d67-4b0d-91fb-c67d7557abe6、taint0、四idma；live DTB及kernel notes与本地构建一致，eth0 192.168.77.2/24 UP/LOWER_UP。当前bit/runtime mode/source freeze仍未验实，不能用历史boot填入。

静态候选仅route-opt f9fa9434…，24 overlay对齐，原内部50MHz时序+0.022/+0.051ns、失败0；原manifest f15f8a72…为−0.219ns/1失败，拒混用。IO/CDC与6 clock warnings未完成。JTAG双接口wildcard选择错误已解，精确主目标1MHz只读状态却为Unknown_Device/0x00000001，原因未知；用户确认开机亮灯。没有以此推断bit损坏或自动重刷。

普通adapter范围/必要性已核，保留原完整144或合并96笔/1,769,472B以及强普通大页/稳定池/预映射/驻留/批量/singleDID；没有实现、池分配/map或恢复性能实验。仅补当前bit/S0、精确kernel freeze、相关时序处置等新事实后做同实例采证；loss成立才G0。无新输入重复静态工作到此停止，归档条件与整体阶段一后续路线已登记，不恢复关闭场景。全Codex，129旧输入不变；两个用户授权临时设备ACL、系统凭据库/非秘密引用，密码无项目日志。现有Linux保持运行、UART已释放。

[报告](iommu-platform/docs/r107-execution-gate-20261012.md)／[稳定裁决](iommu-platform/out/r107-execution-gate-20261012/root-result-acceptance.json)／[本次live](iommu-platform/out/r107-execution-gate-20261012/identity/current-live-evidence.json)／[下一合同](iommu-platform/out/r107-execution-gate-20261012/next-goal-contract.json)／[账本](iommu-platform/out/r107-execution-gate-20261012/execution-ledger.json)。原CURRENT_STATUS/五文档快照在out/r107-execution-gate-20261012/before，stage108和stage106原始输入均未改。下列条目保留原样，旧“当前/下一”只指当轮。

'''
p = WORK / 'HANDOFF.md'; p.write_text(handoff + p.read_text())
validation = '''## stage109 R107执行前提有界采证（2026-10-12）

条件保留，未进入ordinary恢复/G0/G1。实际Dell UART登录/两次成功只读身份查询闭合新boot、riscv64/cpn3-clean、DTB/ELF notes构建标记、四idma及eth0；最初未登录查询失败保留。JTAG完整枚举2目标解释旧selector失败，精确主目标1MHz读到Unknown_Device/IDCODE1，原因未知，current bit/mode仍未验实。全部raw日志和退出码保留；没有自动flash/reset/服务重启或旧矩阵重跑。

优化bit f9fa9434…与交付包同hash，24冻结overlay对齐；原始优化报告+0.022/+0.051ns/失败0与原版−0.219ns/1失败分列。CPU50MHz是约束/DT声明，完整IO/CDC及dirty kernel source/config freeze未知；不因历史22/22而验当前身份。ordinary adapter scope具体但未实现，完整恢复/prepare/leaf/cache/fence/drain/成本与翻译残余未测。129旧输入hash保护通过。本轮的结构/原始证据关联校验不替代或重跑已验stage106/108模型。

[root核验](../out/r107-execution-gate-20261012/validation/root-evidence-check.json)／[旧输入保护](../out/r107-execution-gate-20261012/validation/protected-inputs-check.json)／[当前raw入口](../out/r107-execution-gate-20261012/identity/current-live-evidence.json)／[报告](r107-execution-gate-20261012.md)／[账本](../out/r107-execution-gate-20261012/execution-ledger.json)。

'''
p = PLATFORM / 'docs/validation.md'; p.write_text(validation + p.read_text())

execution = '''## stage109执行前提裁决（当前入口，2026-10-12）

真实goal已登记，无额外token预算。R107条件保留有限合同；当前Dell实际Linux访问、DTB/内核notes构建身份、四idma及eth0已核，但载入bit、runtime S0与dirty kernel source/config freeze仍未闭合。JTAG精确目标可开，实际Unknown_Device/IDCODE1，原因未知。可复用静态候选为route-opt f9fa9434…，内部50MHz时序+0.022/+0.051；原manifest f15f8a72…−0.219/1失败不能混用，IO/CDC缺口分列。

ordinary adapter必要范围已核，未实现或运行恢复。只补具体新身份/冻结/时序事实，齐备才同实例一次完整准备与完成采证；保持大页/预映射/驻留/批量/同owner单DID合并与全部成本。无普通未覆盖翻译loss，故无G0/G1。不可达或普通覆盖则归档卡，回整体阶段一新需求合同，不自动恢复旧场景/加压力/重跑stage106或108。129旧输入不变，全Codex；当前Linux保持运行，UART释放。

[报告](r107-execution-gate-20261012.md)／[稳定裁决](../out/r107-execution-gate-20261012/root-result-acceptance.json)／[下一合同](../out/r107-execution-gate-20261012/next-goal-contract.json)／[账本](../out/r107-execution-gate-20261012/execution-ledger.json)。本关口完成不代表方法论文完成。以下stage108及更早内容为历史记录。

'''
p = PLATFORM / 'docs/research-execution-plan-20261011.md'
s = p.read_text(); marker = '## stage108有限KV合同与有界裁决（当前入口）'
assert marker in s
s = s.replace(marker, execution + '## stage108有限KV合同与有界裁决（历史入口）', 1)
p.write_text(s)

p = WORK / '研究计划.md'; s = p.read_text()
start = s.index('当前stage108完成R107')
end = s.index('\n', s.index('[下一合同](iommu-platform/out/r107-bounded-decision-20261012/next-goal-contract.json)。', start))
s = s[:start] + '''当前stage109完成R107执行前提有界裁决，条件保留合同资产，无ordinary恢复试验/新G0/G1。当前Dell Linux/DTB/内核build notes及访问成立；current bit/runtime mode/kernel dirty source freeze仍未验实，JTAG Unknown_Device/IDCODE1原因未知。仅优化route-opt f9fa9434…静态候选内部50MHz+0.022/+0.051通过；原manifest f15f8a72…−0.219/1失败拒混用，IO/CDC边界保留。[本轮报告与原始入口](iommu-platform/docs/r107-execution-gate-20261012.md)。stage108完整OPT125m有限144笔/1,769,472B合同和强普通96笔合并、4×4MiB或8×2MiB、4KiB/premap/residency/batch/singleDMA继续保留，未重跑。

下一只补当前有效FPGA链/bit/S0、精确内核冻结及相关时序处置这些新事实，齐备才实现必要ordinary adapter并同实例一次全成本采证。缺口具体不以更多静态核查代替方法；平台不可达或普通覆盖则归档R107卡，回到阶段一以保留文献/E2/E3边界形成新需求合同，不自动恢复关闭场景或增加第三场景/压力/猜计算。可达、可避免的翻译残余成立才最小G0，G0支持后才最多两G1方向。后继goal未登记，论文未完成。[下一合同](iommu-platform/out/r107-execution-gate-20261012/next-goal-contract.json)。''' + s[end:]
p.write_text(s)
save('validation/document-write-receipt.json', {'stable_decision_written_first': True,
     'updated_documents': [str(p) for p in paths], 'concurrent_independent_status_tail_preserved_exactly': True,
     'HANDOFF_old_body_prepended_intact': True, 'validation_old_body_prepended_intact': True,
     'before_final_snapshots': snapshots, 'ordinary_finalization_repeated': False})
print(json.dumps({'decision': decision['decision'], 'documents_updated': len(paths), 'report': str(PLATFORM / 'docs/r107-execution-gate-20261012.md')}, ensure_ascii=False))

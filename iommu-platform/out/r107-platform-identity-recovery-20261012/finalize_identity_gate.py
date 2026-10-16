#!/usr/bin/env python3
"""Accept original stage110 evidence and publish the bounded conditional decision."""
from pathlib import Path
import json, re, hashlib
from datetime import datetime, timezone
ROOT=Path('/home/xvhaocheng/work')
P=ROOT/'iommu-platform'
B=P/'out/r107-platform-identity-recovery-20261012'
def read(name):return json.loads((B/name).read_text())
def save(name,value):(B/name).write_text(json.dumps(value,ensure_ascii=False,indent=2)+'\n')
def line(file,pattern):
    for i,s in enumerate(Path(file).read_text(errors='replace').splitlines(),1):
        if re.search(pattern,s):return i
    raise AssertionError((str(file),pattern))
def field(receipt,name):
    match=re.search(r'^'+re.escape(name)+r'=([^\r\n]+)',receipt['board_stdout'],re.M)
    if not match:raise AssertionError(name)
    return match.group(1).strip()
software=read('live/known-bit-new-boot-software-identity-result.json')
mode=read('live/known-bit-new-boot-mode-identity-result.json')
repro=read('kernel/reproduction/reproduction-result.json')
binaries=read('kernel/root-direct-binary-check.json')
program=read('jtag/known-bit-program-and-verify-result.json')
recovery=read('jtag/known-bit-recovery-no-readback-result.json')
wait=read('live/known-bit-boot-extension-result.json')
preservation=read('validation/final-preservation-and-UART-release.json')
raw=(B/'jtag/known-bit-program-and-verify.raw.log').read_text()
raw2=(B/'jtag/known-bit-recovery-no-readback.raw.log').read_text()
boot=software['parsed_boot_id']
words={i:int(field(mode,'R110_WORD_'+str(3872+i)),16) for i in range(6)}
checks={
 'explicit_specific_reload_approval':read('jtag/known-bit-reload-approval.json')['permission']=='approved',
 'known_bit_hash_both_loads':program['bit_sha256']==recovery['bit_sha256']=='f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca',
 'both_actual_program_commands_returned':all('R110_PROGRAM_COMMAND_RETURNED\n' in x for x in [raw,raw2]),
 'known_file_positive_masked_verify':program['client_exit_code']==0 and '\nR110_POST_PROGRAM_VERIFY_RC=0\n' in raw,
 'second_load_without_readback':recovery['configuration_readback_during_SD_boot'] is False and 'verify_hw_devices' not in raw2,
 'second_load_normal_config_status':all('\nR110_AFTER_PROGRAM '+key+'='+value+'\n' in raw2 for key,value in [('REGISTER.CONFIG_STATUS.BIT00_CRC_ERROR','0'),('REGISTER.CONFIG_STATUS.BIT04_END_OF_STARTUP_(EOS)_STATUS','1'),('REGISTER.CONFIG_STATUS.BIT06_GWE_STATUS','1'),('REGISTER.CONFIG_STATUS.BIT13_DONE_INTERNAL_SIGNAL_STATUS','1'),('REGISTER.CONFIG_STATUS.BIT15_IDCODE_ERROR','0')]),
 'new_boot_same_software_and_mode_query':software['board_query_succeeded'] and mode['board_query_succeeded'] and boot==mode['parsed_boot_id'] and boot!='db0664f0-2d67-4b0d-91fb-c67d7557abe6',
 'current_software_fingerprints':field(software,'R110_CONFIG_SHA').split()[0]=='b9af38b79a9f0d7c5348bb70ea4641ac0f7e21a4e44119c63a1e653639c2933d' and field(software,'R110_DTB_SHA').split()[0]=='aa9cdf4cb28700c83374205caca5ac0085a527f03aaeae49f7e19e8d2d3d90af' and field(software,'R110_NOTES_SHA').split()[0]=='9dc6815e908b46c1dbd7819ba2df6f010b1f9f8f671100d7c1d4d1bfdababa56',
 'current_kernel_taint0':field(software,'R110_TAINT')=='0',
 'current_observer_v9_and_composition_ABI1':int(field(mode,'R110_MAGIC'),16)==0x49444d414f425309 and words[0]==0x49444d4143504e01 and words[1]==1,
 'actual_S0_DID10_idle_no_status_error':words[2]==0xA00 and words[3]==0 and words[4]==0 and words[5]==1,
 'observer_index_restored':field(mode,'R110_OLD_INDEX')==field(mode,'R110_RESTORED_INDEX'),
 'critical_binary_inputs_parent_checked':all(x['equal'] for x in binaries['objects'] if x['path'].startswith('drivers/')),
 'full_Image_mismatch_honestly_retained':repro['final_Images']['Image_equal'] is False and repro['final_Images']['ELF_equal'] is False,
 'linked_inputs_partial_correspondence':repro['code_correspondence']['archive_members']==2079 and repro['code_correspondence']['exact_objects']==2078 and repro['code_correspondence']['only_unmatched_object']=='init/version.o',
 'one_clean_build_one_bounded_metadata_recovery':repro['actual_builds']['clean_Image_attempts']==1 and repro['actual_builds']['metadata_recovery_attempts']==1,
 'old_inputs_preserved_and_UART_released':preservation['passed'],
 'actual_passive_wait_ended_at_login':wait['status']=='login_prompt_observed',
}
save('validation/root-evidence-check.json',{'checked_at_utc':datetime.now(timezone.utc).isoformat(),'checks':checks,'passed':all(checks.values()),'scope':'Evidence consistency and original input preservation; not R107 execution or scientific replay'})
if not all(checks.values()):raise RuntimeError('Root evidence checks failed; shared documentation not touched')
refs={
 'FPGA_part_IDCODE':{'path':str(B/'jtag/B-FPGA-status.raw.log'),'line':line(B/'jtag/B-FPGA-status.raw.log',r'^R110_PROP .* PART=')},
 'positive_readback':{'path':str(B/'jtag/known-bit-program-and-verify.raw.log'),'line':line(B/'jtag/known-bit-program-and-verify.raw.log',r'^R110_POST_PROGRAM_VERIFY_RC=0')},
 'failed_SD':{'path':str(B/'live/known-bit-reload-boot-UART.raw.log'),'line':line(B/'live/known-bit-reload-boot-UART.raw.log','SD card failed!')},
 'new_boot':{'path':str(B/'live/known-bit-new-boot-software-UART.raw.log'),'line':line(B/'live/known-bit-new-boot-software-UART.raw.log',r'^R110_BOOT_ID=')},
 'actual_mode':{'path':str(B/'live/known-bit-new-boot-mode-UART.raw.log'),'line':line(B/'live/known-bit-new-boot-mode-UART.raw.log',r'^R110_WORD_3874=')},
 'kernel_objects':{'path':str(B/'kernel/reproduction/reproduction-result.json'),'line':line(B/'kernel/reproduction/reproduction-result.json',r'"code_correspondence"')},
 'kernel_full_mismatch':{'path':str(B/'kernel/reproduction/reproduction-result.json'),'line':line(B/'kernel/reproduction/reproduction-result.json',r'"final_Images"')},
}
next_contract={
 'kind':'inactive successor contract; not a registered goal or G0',
 'decision':'Conditionally retain this same R107 finite contract. Platform native JTAG, selected-bit load, new Linux boot and ordinary S0 are now established. Do not repeat these unchanged checks or old source replay.',
 'specific_question':'Can one source-frozen ordinary execution kernel and minimal additive adapter legally prepare and completely restore the accepted finite KV instance, with actual mapping/cache/fence/completion and full costs?',
 'remaining_new_facts':[
  'Use the accepted full c0f5e13 source freeze plus recorded overlays as a base for a new, uniquely identified ordinary execution kernel; retain its exact config/ELF/Image/notes/DTB/payload and match the actual new boot. Do not spend another round trying to match the old temporary UTS banner.',
  'Root review the necessary additive stable-pool/finite-manifest/ownership/layer-dependency/cache/fence/locked-snapshot/hardware-completion glue before deployment; retain old ABI and existing private credit-fix/segmentation correctness boundary.',
  'Resolve deployment method against a concrete built payload. Current approval covers selected volatile bit and restart, not a new SD image write.',
  'For physical seconds/microseconds or throughput, calibrate the current timer against an independent reference with a stated error. Raw cycles and independently measured host phases remain separate.',
  'Only one same-instance full-cost ordinary pilot after the selected kernel and interface are accepted; technical error stops that attempt and is not translation-loss evidence.'
 ],
 'finite_instance':{'restoration_bytes':1769472,'full_copies':144,'legal_coalesced_copies':96,'logical_pools_bytes':9437184,'ordinary_backing_bytes':16777216,'native_GPU_trace_or_compute_time':False},
 'required_controls':['lawful large pages, including full order10/order9 backing','4KiB fallback','pre-mapping and residency','batching and legal coalescing','authorized same-owner single-DMA access to all four pools'],
 'costs_not_yet_measured':['allocation and initialization','actual leaf sizes and IOVA mapping','cache maintenance','fence completion','supply and per-layer dependencies','issue and full hardware completion/drain','all-byte/guard validation','unmap/free and failure cleanup','build, transfer and deployment costs'],
 'G0_entry':'A concrete reachable avoidable translation-related loss outside these ordinary controls, with all costs counted; none established in stage110',
 'G1_entry':'Only after accepted G0 and original prior-work/full-cost review',
 'archive_rule':'If the lawful finite interface is unavailable, or the complete ordinary control closes the proposed residual, archive this R107 card. A technical startup/metadata failure is not that scientific conclusion.',
 'route_after_archive':'Return to phase1 for a new explicit demand/ordinary-failure contract using retained literature and accepted E2/E3 boundaries; do not revive closed row/HERO/cold-weight/copy branches or broaden pressure/scenarios.',
 'bounds':{'workflows':1,'new_scenes':0,'old_stage106_or108_replay':False,'new_RTL_before_gap':False,'invented_compute_arrivals_deadlines':False},
 'successor_goal_registered':False,'paper_complete':False}
save('next-goal-contract.json',next_contract)
result={
 'stage':110,'accepted_at_utc':datetime.now(timezone.utc).isoformat(),'decision':'conditional_retain_R107; native JTAG and chosen-bit/live-S0 identity closed; complete old kernel Image correspondence not accepted; no ordinary recovery pilot',
 'goal_registration':'goal-registration.json','no_extra_token_budget':True,'current_goal_delivery_status':'evidence accepted; publishing documentation before actual goal completion',
 'JTAG':{'old_IDCODE':'0x00000001','established_cause':'FTDI channel0 CPU DMI TAP default IDCODE; FPGA is channel1 B target','actual_FPGA':'xc7k325t','actual_FPGA_IDCODE':'0x43651093','frequency_Hz':1000000,'primary_source':'jtag/history-interface-evidence.json'},
 'prior_loaded_bit':{'status':'unidentified','user_attestation_preserved':'live/user-current-bit-confirmation.json','candidate_masked_readback_differences':{'optimized_f9fa9434':15848937,'default_cd49d4ab':14894878},'unregistered_latest_artifact':'unknown; limited registered/default-path search only','cause_of_prior_different_configuration':'unknown'},
 'chosen_bit':{'path':'out/composition-isolated-20261007/route-opt/ariane_xilinx.bit','sha256':program['bit_sha256'],'source':'frozen October7 composition; IOMMU238a39e, not independent57eb562 maintenance','actual_loads':2,'first_load_masked_verify_rc':0,'second_load_masked_readback':False,'second_load_status':'DONE/EOS/GWE1; CRC/IDerror0; fresh Linux and ABI/mode observed','new_synthesis_place_route_or_RTL':False},
 'startup':{'first_attempt':'SD multi-block CRC16 error -2 before Linux','one_recovery_change':'same approved bit; no overlapping configuration readback','second_attempt':'OpenSBI/Linux/login and subsequent framed queries succeeded','root_cause':'unknown; one successful changed startup is not complete physical attribution','Flash_or_SD_image_writes':0,'raw_capture_gap':'separate passive extension records the uncaptured interval; no fabricated continuity'},
 'current_live':{'boot_id':boot,'release':software['parsed_uname'],'taint':0,'config_sha256':field(software,'R110_CONFIG_SHA').split()[0],'DTB_sha256':field(software,'R110_DTB_SHA').split()[0],'kernel_notes_sha256':field(software,'R110_NOTES_SHA').split()[0],'observer_magic':'0x49444d414f425309','composition_magic':'0x49444d4143504e01','ABI':words[1],'config':words[2],'mode':'S0','protected_DID':10,'generation':words[3],'status':words[4],'idle':words[5],'query_effects':'only bank selector writes then restore; no mode/ARM/START/NEXT write or DMA issue'},
 'kernel_correspondence':{'source_freeze':'kernel/reproduction/frozen-source-manifest.json','base':'c0f5e13ea81fa30d78f3bb1d6389e2397321e040','frozen_items':81122,'linked_inputs_byte_equal':'2078/2079; same order','critical_idma_iommu_iopgtable_ethernet_objects':'all byte equal, independently parent hashed','config_rootfs_final_metadata':'exact equal','full_Image_ELF':'not equal; not accepted as complete historical binary correspondence','old_Image_sha256':repro['final_Images']['old_Image_sha256'],'new_Image_sha256':repro['final_Images']['new_Image_sha256'],'specific_difference':'one temporary init/version.o: 32B generated banner growth, not a found driver source change','recovery_failure_cause':'outer make --old-file did not stop recursive Kbuild from regenerating the temporary header/object','no_more_metadata_recovery':True},
 'timing':{'internal_constraint_CPU_MHz':50,'WNS_ns':0.022,'WHS_ns':0.051,'actual_physical_CPU_frequency':'unverified','mtime_frequency':'unverified; same CPU-derived2:1 is not an independent reference','finite_functional_capture':'conditionally possible after kernel/interface gate, all bytes/completion/errors and full phase costs required','global_IO_CDC_signoff':'not accepted; actual active-path disposition in jtag/timing-gate-disposition.json'},
 'R107_execution':{'adapter_implemented':False,'pool_allocation_mapping_or_restore':False,'ordinary_residual_translation_loss':'unmeasured','new_G0':False,'G1':False,'reason_pilot_not_entered':'Complete current historical kernel Image/source correspondence acceptance is still unmet. The next route is a chosen new source-frozen ordinary kernel, not another old-banner recovery loop.'},
 'preservation':{'stage109_files_unchanged':133,'kernel_original_inputs_unchanged':46,'production_source_config_and_old_history':'unchanged','UART':'released; Linux left running'},
 'agents':'root plus two exclusive gpt-6.1-sol/ultra children; no DeepSeek/Claude/Astra',
 'original_evidence':refs,'next_contract':'next-goal-contract.json','paper_complete':False,
 'receipt_interpretation':{'pre_reload_receipt':'live/pre-known-bit-reload-identity-result.json; program framework stored an incomplete filename, corrected here without editing original receipt','second_load_status_label':'program_and_verify_returned is an inherited generic framework label. Actual Tcl and configuration_readback_during_SD_boot=false establish that the second load did not verify.'}}
save('root-result-acceptance.json',result)
ledger={'stage':110,'source':'Actual tool receipts, saved commands and raw records; diagnostic failures retained','goal':'actual create_goal, no added budget; complete only after delivery','author_execution':'Codex root; two exclusive Sol/ultra source/timing workers','device_calls':[
 {'action':'B FPGA native identity/status','receipt':'jtag/B-FPGA-status-result.json','program':False},
 {'action':'optimized current-config masked comparison','receipt':'jtag/current-bit-verify-result.json','exit':2,'differences':15848937},
 {'action':'one ABI-informed default current-config comparison','receipt':'jtag/current-default-verify-result.json','exit':2,'differences':14894878},
 {'action':'pre-load software/sync query','receipt':'live/pre-known-bit-reload-identity-result.json','status':'no begin marker; sync not confirmed'},
 {'action':'explicitly approved volatile load and one positive masked verify','receipt':'jtag/known-bit-program-and-verify-result.json','elapsed_seconds':program['elapsed_seconds'],'exit':0,'startup':'SD CRC error'},
 {'action':'one purposeful same-file recovery without readback','receipt':'jtag/known-bit-recovery-no-readback-result.json','elapsed_seconds':recovery['elapsed_seconds'],'exit':0},
 {'action':'one progress-based passive wait extension','receipt':'live/known-bit-boot-extension-result.json','elapsed_seconds':wait['elapsed_seconds'],'status':wait['status']},
 {'action':'one private board login','receipt':'live/board-login-receipt.json','password_logging':False},
 {'action':'current new-boot software/config/DTB/notes query','receipt':'live/known-bit-new-boot-software-identity-result.json'},
 {'action':'magic-guarded composition mode read; selector restored','receipt':'live/known-bit-new-boot-mode-identity-result.json','DMA_or_mode_change':False}],
 'host_builds':{'kernel_clean_Image':1,'kernel_clean_Image_seconds':repro['actual_builds']['clean_Image_seconds'],'one_targeted_metadata_recovery_seconds':repro['actual_builds']['targeted_version_compile_seconds']+repro['actual_builds']['relink_seconds'],'new_synthesis_place_route':0,'saved_DCP_mask_reference_generations':2,'firmware_adapter_builds':0},
 'worker_ledgers':['jtag/execution-ledger.json','jtag/current-workspace-bit-ledger.json','jtag/timing-gate-disposition.json','kernel/command-ledger.json','kernel/reproduction/reproduction-result.json','kernel/sd-boot-failure-evidence.json'],
 'scientific_counts':{'stage106_rerun':0,'stage108_replay':0,'R107_restore':0,'GPU_execution':0,'new_G0_or_G1':0},
 'host_program_device_side_effects':'two approved volatile loads/restarts; no Flash/eFuse/SD image write, production source or model configuration change',
 'cost_scope':'These are actual diagnosis/build/boot costs. R107 allocation/map/cache/fence/restore/cleanup costs remain unknown.',
 'preservation_receipt':'validation/final-preservation-and-UART-release.json'}
save('execution-ledger.json',ledger)
def link(label,key):
 x=refs[key];return f"[{label}]({x['path']}:{x['line']})"
report=f'''# stage110 R107平台身份与恢复进入关（2026-10-12）

条件保留R107有限合同。JTAG原因、指定bit载入、新Linux启动及实际S0已核实；旧内核全Image/ELF重现仍未通过，本轮没有普通恢复采证、新G0/G1或论文完成声明。

实际goal登记，无额外token预算，依据stage109有效next-goal-contract。原问题是：当前平台究竟运行什么、必要普通恢复接口应绑定哪份源码。进入恢复的条件是对应的平台/内核/接口成立并有可解释的计时与完整成本；结束条件是这些事实的稳定裁决及明确后续，不扩大场景或重跑旧矩阵。

## 已接受的实际身份

| 项目 | 原始实测与接受范围 |
|---|---|
| JTAG `0x00000001` | CPU DMI TAP默认IDCODE；旧无后缀target误当FPGA。FPGA在B后缀/channel1，实际xc7k325t、`0x43651093`，1MHz |
| 本轮指定bit | `f9fa94346035943bca1c7a3e5d89667c2c6fc351c7984bf2e8d6cdb23a1155ca`，10月7日冻结优化bit；不含独立57eb562维护 |
| 实际载入 | 用户明确批准；两次同文件易失载入，第一次同文件掩码回读rc0；第二次不并行回读、DONE/EOS/GWE1、CRC/IDerror0并成功启动 |
| 新boot | `{boot}`；cpn3-clean/riscv64、taint0、四idma节点 |
| 配置／DTB／notes | config `b9af38b7…`、DTB `aa9cdf4c…`、notes `9dc6815e…`与原本地构建一致 |
| 实际模式 | observer V9、composition magic正确、ABI1；config `0xA00`=S0/DID10，generation0/status0/idle1；不是拿reset常量当观测 |

{link('原始FPGA身份','FPGA_part_IDCODE')}、{link('同文件回读成功','positive_readback')}、{link('新boot原始UART','new_boot')}、{link('实际模式原始UART','actual_mode')}。模式读取仅改变银行索引并还原，未写mode、ARM、START或NEXT，未执行DMA。第二次载入后没有再次回读；当前判断合用同文件实际加载回执、正常配置状态和新boot/接口值，不伪造第二次回读。

载入前优化与默认两个候选的掩码比较分别报15,848,937／14,894,878差异。两份mask均由保存的routed DCP生成，与原bit配置packet完全相同；没有重新综合或布局布线。此前配置仍无已识别文件，未登记自定义产物未知，用户现场确认原样保留，不把“最近／当前工作区”推成确定hash。根本原因未知，也不能推成bit损坏。[原结果]({B}/jtag/current-bit-verify-result.json)／[默认对照]({B}/jtag/current-default-verify-result.json)／[有限候选清单]({B}/jtag/current-workspace-bit-candidates.md)。

掩码回读的操作依据为AMD 2023.1 [UG908](https://docs.amd.com/r/2023.1-English/ug908-vivado-programming-debugging/Bitstream-Verify-and-Readback-for-FPGAs-and-MPSoCs)，mask指定跳过和比较的配置位；工具实际退出码与原日志决定本轮结果。

## 失败与一次有理由的恢复

第一次已知bit载入后，SD初始化/GPT读取成功，boot image复制中出现`FFFFFFFFFFFFFFFE`，尚未到Linux。冻结C及生成SV反解机器码都对应512B块CRC16不一致；不是超时、容量失效或翻译残余。{link('失败原UART','failed_SD')}／[原源码及ROM指令]({B}/kernel/sd-boot-failure-evidence.md)。

root只追加一次同已批准bit恢复，移除与SD启动重叠的configuration readback。初始120秒结束时仍有复制进度，依据同bit历史整流程496秒，另作一次最多360秒的被动串口等待，实际190.954秒到登录。两段捕获间的空档明确保留，未补造日志。第二次进入OpenSBI/Linux并通过随后身份与模式查询；这证明恢复可用，未证明前次CRC的完整物理因果。[恢复合同]({B}/jtag/known-bit-recovery-contract.json)／[载入回执]({B}/jtag/known-bit-recovery-no-readback-result.json)／[被动等待回执]({B}/live/known-bit-boot-extension-result.json)。

原清单bit不是“历史Dell程序直接复现”：旧实际链为Dell构建、Mac openFPGALoader1MHz载入，本次采用Dell现有Vivado2023.1和精确B接口；复用bit、冻结源、SD软件与UART协议。未改Flash、eFuse或SD镜像，未安装驱动、解绑USB或重启主机服务。当前Linux保持运行，UART已释放。

## 内核对应：正证据与未通过项

冻结base `c0f5e13ea81fa30d78f3bb1d6389e2397321e040`、5项tracked overlay、3项lowrisc源码及81,122项完整清单；新空O一次干净构建56.886秒。2079个实际链接输入顺序相同，2078个完整字节相同；root另直接哈希核对idma、IOMMU、io_pgtable及Ethernet四关键对象全同。config、rootfs和最终版本对象也完全相同。[root直接字节核验]({B}/kernel/root-direct-binary-check.json)／{link('链接输入原结果','kernel_objects')}。

完整旧Image SHA `8c19da27…`，本次Image `70ac6480…`，均24,131,072B；ELF也不同，未接受全二进制对应。唯一对象差异为`init/version.o`临时版本banner增加32B，造成链接布局与构建ID等变化，没有发现关键驱动代码对象变化。一次定向metadata恢复曾生成匹配的临时对象，但没有独立副本；外层`make --old-file`未阻止递归Kbuild重生header/object，最终仍不匹配。保留全部失败和实际命令，停止追加恢复。{link('Image与ELF原结果','kernel_full_mismatch')}／[详细重现]({B}/kernel/reproduction/reproduction-result.md)。

因此接受“当前配置与关键编译执行路径有强字节对应”，不接受“完整旧Image已从冻结源精确重现”。下一走新的明确源码冻结普通执行内核与唯一release/notes，并匹配其真实boot；不继续追旧临时横幅或复用旧增量对象冒充来源。

## 普通路径、成本和失效边界

R107仍是已接受的source方法／有限研究回放：3唯一槽、完整144笔/1,769,472B，合法合并96笔同量；此前换出另计同量。四逻辑池9MiB、普通16MiB backing、合法order10/order9大页、4KiB回退、预映射、驻留、批量及同可信owner单DMA访问四池全部保留。663,552B仅proxy标记；本轮无native GPU、计算/到达/期限数据，也无stage106/108回放。[原有限合同与成本]({P}/docs/r107-bounded-decision-20261012.md)。

必要ordinary adapter仍未实现，真实分配/map/leaf、cache/fence、供给/层依赖、恢复/硬件完成/排空、全字节/guard核验及unmap/free/失败清理成本未知；本轮构建与启动账不能代替这些成本。内核完整对应项未全过，因此未进入普通恢复采证，不把接口实现缺口作为创新或科学阴性。

时序按路径处置：内部20ns约束WNS+0.022/WHS+0.051、失败0；实际DDR→CPU为异步FIFO Gray指针5ns max-delay、slack+4.238，反向最差涉及reset。已有IP结构不支持按六条警告直接判硬件坏，也不支持整体豁免。有限功能原始采证可条件进入，但须所有字节、硬件完成、错误/复位、完整阶段成本；物理秒/微秒与吞吐需独立计时校准。CPU50/mtime25MHz是报告/DT值，CPU派生的2:1比例不是独立实测；全面IO/CDC和更强翻译因果仍未接受。[原路径与准入裁决]({B}/jtag/timing-gate-disposition.md)。

## 稳定决定及下一关口

条件保留同一R107合同。JTAG/已知bit/新boot/S0这些问题已收束；新关口只绑定源码冻结的ordinary内核与必要接口，匹配实际运行身份后做同实例一次完整普通恢复与成本记录。部署先形成具体产物，当前批准不包含新SD镜像写入；没有当前独立计时事实时，保留cycles与host阶段时长分列，不能换算出未经核验的物理耗时。

具体可达且普通未覆盖的翻译相关残余成立后才最小G0；满足G0与前作/全成本审查后才G1。若合法接口不可达或普通控制覆盖目标残余，归档这张R107卡、回阶段一新的需求合同；不重启已关闭候选或扩场景。启动/metadata技术失败不等于这项科学归档条件。

[稳定JSON]({B}/root-result-acceptance.json)／[未激活下一合同]({B}/next-goal-contract.json)／[实际执行账本]({B}/execution-ledger.json)。root＋两独占Sol/ultra；未调用DeepSeek、Claude或Astra。133个stage109文件和46项内核原输入均未变，[保护及UART释放]({B}/validation/final-preservation-and-UART-release.json)。实goal仅在这些交付与文档同步完成后complete；本轮不代表方法论文完成。
'''
(P/'docs/r107-platform-identity-recovery-20261012.md').write_text(report)
(B/'README.md').write_text(f'''# stage110证据入口\n\n[完整报告]({P}/docs/r107-platform-identity-recovery-20261012.md)／[稳定裁决]({B}/root-result-acceptance.json)／[实际账本]({B}/execution-ledger.json)／[下一合同]({B}/next-goal-contract.json)。\n\n各原证据与行号见root-result-acceptance.json.original_evidence。完整冻结源码和新构建在kernel/reproduction；它们不替代本次board原记录。\n''')
# Snapshot the fresh shared documents, so unrelated concurrent contribution entries survive.
before=B/'before-final-doc-sync';before.mkdir(exist_ok=False)
docpaths=[ROOT/'CURRENT_STATUS.md',ROOT/'HANDOFF.md',P/'docs/validation.md',P/'docs/research-execution-plan-20261011.md',ROOT/'研究计划.md']
for path in docpaths:(before/path.name).write_bytes(path.read_bytes())
common=f'''stage110完成JTAG／指定bit／新boot与S0有界核验，R107条件保留。CPU DMI默认IDCODE1被误当FPGA；B接口实xc7k325t/0x43651093。用户批准载入冻结优化f9fa9434…，第一次mask verify通过但SD块CRC失败，第二次同文件不并行readback后启动成功；CRC物理原因未知。新boot {boot}、config/DTB/notes一致、taint0，observerV9/ABI1/config0xA00=S0/DID10、idle1；选择器还原，未执行DMA。\n\n一次干净内核构建56.886秒，81122项源冻结、2078/2079链接对象与四关键驱动逐字同；临时version元数据32B造成完整Image/ELF未匹配，一次metadata恢复未保留成功中间对象且递归覆盖，停止，不声称完整旧Image来源闭合。下一走新源码冻结ordinary执行内核/接口并匹配其真实boot，避免旧横幅循环。尚无普通分配/map/恢复/全成本或translation残余，不进G0/G1。\n\n相关clock/IO/CDC按真实路径处置，允许有限功能采证条件，绝对秒/吞吐需独立计时校准，不能用50/25MHz声明或2:1比例冒充实测。保留同实例144或合法96笔/1769472B、合法大页/4KiB/premap/residency/batch/singleDMA；不重复stage106/108、扩场景或加压力。133旧stage109文件及46内核输入不变，生产代码/config/原历史保留；两次授权易失载入、无Flash/SD镜像写入，Linux运行、UART释放。全Codex root＋两独占Sol/ultra，零DeepSeek/Claude/Astra。\n'''
current=(ROOT/'CURRENT_STATUS.md').read_text();tail=current[current.index('\n- 独立Linux贡献'):]
(ROOT/'CURRENT_STATUS.md').write_text(f'''# 当前状态 stage110 2026年10月12日 R107平台身份恢复有界裁决完成\n\n- 整体仍为阶段一立题；实goal登记、无额外预算。R107条件保留，JTAG/指定bit/新boot/S0已核；完整旧内核Image对应仍未接受，无普通恢复/G0/G1。[稳定裁决](iommu-platform/out/r107-platform-identity-recovery-20261012/root-result-acceptance.json)。\n- 旧IDCODE1为CPU DMI TAP，FPGA为B接口xc7k325t/0x43651093。用户批准f9fa9434…冻结优化bit，两次实际易失载入；第一次掩码回读成功，SD CRC失败；第二次不并行readback后成功。前次物理原因未知。当前不含独立57eb562修复。[bit与启动](iommu-platform/docs/r107-platform-identity-recovery-20261012.md)。\n- 当前boot {boot}、cpn3-clean/riscv64、taint0；config/DTB/notes同本地，四idma。实际observerV9/ABI1、config0xA00=S0/DID10、generation/status0、idle1。索引已还原，Linux运行、UART释放。[本次原始记录](iommu-platform/out/r107-platform-identity-recovery-20261012/live/known-bit-new-boot-mode-identity-result.json)。\n- 源冻结81122项，一次干净构建2078/2079链接对象全同，四关键驱动及config/rootfs匹配；临时版本metadata32B差造成全Image/ELF不匹配，一次有界恢复仍失败且停止。[内核字节证据](iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/reproduction-result.md)。\n- 时序按活跃路径处置；内部20ns+0.022/+0.051ns保留，物理50/25MHz未验实，绝对耗时/吞吐需独立计时，完整IO/CDC不宣称通过。[准入边界](iommu-platform/out/r107-platform-identity-recovery-20261012/jtag/timing-gate-disposition.md)。\n- R107合同不变：144或合法96笔/1769472B，3唯一槽、普通大页/4KiB/premap/residency/batch/授权singleDMA均保留；未分配/map/恢复，全部实际成本和翻译残余未知。[有限合同](iommu-platform/docs/r107-bounded-decision-20261012.md)。\n- 下一只建源码冻结ordinary执行内核与必要接口，核新boot对应后同实例一次普通恢复；不再重复已闭合身份或追旧banner。具体loss成立才G0；普通覆盖/接口不可达则归档R107并回阶段一新合同，不恢复关闭候选。[未激活下一合同](iommu-platform/out/r107-platform-identity-recovery-20261012/next-goal-contract.json)。\n- 133旧stage109文件与46内核输入不变，全Codex两Sol/ultra独占席，零旧回放、新RTL/firmware/ordinary实验；两次已批准易失配置载入、零Flash/SD镜像写入。[账本](iommu-platform/out/r107-platform-identity-recovery-20261012/execution-ledger.json)。论文未完成。\n- 本新主会话及独占模型核验席Sol/ultra，原生500000/有效475000、最大支持872000分列已验，模型配置与旧历史未改。[模型验收](codex-sol-runtime-verification-20261012T072912Z-01a12eff/REPORT.md)。\n'''+tail)
(ROOT/'HANDOFF.md').write_text('## stage110 R107平台身份恢复与普通恢复进入关（2026-10-12）\n\n'+common+'\n[报告](iommu-platform/docs/r107-platform-identity-recovery-20261012.md)／[稳定裁决](iommu-platform/out/r107-platform-identity-recovery-20261012/root-result-acceptance.json)／[下一合同](iommu-platform/out/r107-platform-identity-recovery-20261012/next-goal-contract.json)／[账本](iommu-platform/out/r107-platform-identity-recovery-20261012/execution-ledger.json)。仅本关口交付后complete；论文未完成。下方原条目保留为历史。\n\n'+(before/'HANDOFF.md').read_text())
(P/'docs/validation.md').write_text('## stage110 R107平台身份恢复原证据核验（2026-10-12）\n\n'+common+'\n[root证据检查](../out/r107-platform-identity-recovery-20261012/validation/root-evidence-check.json)／[原输入保护](../out/r107-platform-identity-recovery-20261012/validation/final-preservation-and-UART-release.json)／[报告](r107-platform-identity-recovery-20261012.md)／[账本](../out/r107-platform-identity-recovery-20261012/execution-ledger.json)。这些是身份/字节/来源检查，不是R107性能测试。\n\n'+(before/'validation.md').read_text())
plan=(before/'research-execution-plan-20261011.md').read_text().replace('## stage109执行前提裁决（当前入口，2026-10-12）','## stage109执行前提裁决（历史入口，2026-10-12）',1)
pos=plan.index('## stage109')
entry='## stage110平台身份恢复裁决（当前入口，2026-10-12）\n\n'+common+'\n当前下一关口仅新source-frozen ordinary内核/接口与同实例一次完整成本；新部署先有具体可审产物，未登记后继goal。归档条件与阶段一后续路线继续按本次合同。\n\n[报告](r107-platform-identity-recovery-20261012.md)／[稳定裁决](../out/r107-platform-identity-recovery-20261012/root-result-acceptance.json)／[下一合同](../out/r107-platform-identity-recovery-20261012/next-goal-contract.json)。\n\n'
(P/'docs/research-execution-plan-20261011.md').write_text(plan[:pos]+entry+plan[pos:])
plan=(before/'研究计划.md').read_text();pos=plan.index('## 1.')
entry='当前有界进展stage110：JTAG/已批准优化bit/新boot/S0已核实；完整旧内核Image重现仍未接受，R107条件保留、无普通恢复或新G0/G1。下一走新的源码冻结ordinary执行内核与必要接口，再做同实例一次完整成本，不重复旧身份/回放或追旧临时banner。具体翻译残余成立才G0；普通覆盖则结束R107卡并回阶段一新合同，论文未完成。[本轮报告](iommu-platform/docs/r107-platform-identity-recovery-20261012.md)／[有效下一合同](iommu-platform/out/r107-platform-identity-recovery-20261012/next-goal-contract.json)。\n\n'
(ROOT/'研究计划.md').write_text(plan[:pos]+entry+plan[pos:])
# Only explicit local report links are validated, not historical proposal filenames.
missing=[]
for path in [P/'docs/r107-platform-identity-recovery-20261012.md',B/'README.md']:
 for target in re.findall(r'\]\((/[^)]+)\)',path.read_text()):
  actual=re.sub(r':\d+$','',target)
  if not Path(actual).exists():missing.append({'document':str(path),'target':target})
save('validation/delivery-links-check.json',{'checked_at_utc':datetime.now(timezone.utc).isoformat(),'missing':missing,'passed':not missing,'shared_documents_synchronized':[str(x) for x in docpaths]})
if missing:raise RuntimeError(missing)
result['current_goal_delivery_status']='bounded deliveries and shared documentation completed; actual goal completion tool pending'
save('root-result-acceptance.json',result)
print(json.dumps({'checks_passed':sum(checks.values()),'checks_total':len(checks),'new_boot':boot,'actual_mode':'S0','decision':'conditional_retain','R107_recovery_runs':0,'shared_docs_updated':len(docpaths),'report':str(P/'docs/r107-platform-identity-recovery-20261012.md')},ensure_ascii=False,indent=2))

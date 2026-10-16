#!/usr/bin/env python3
"""Read scoped local historical evidence and write only this artifact folder."""
from pathlib import Path
import datetime
import hashlib
import json
import sys

ROOT = Path('/home/xvhaocheng/work/iommu-platform')
OUT = Path(__file__).resolve().parent
STAGE = 'out/composition-isolated-20261007/fpga-src/'
READS = []
EXTRACTS = []


def evidence(relative, ranges):
    path = ROOT / relative
    raw = path.read_bytes()
    lines = raw.decode('utf-8').splitlines()
    record = {'path': str(path), 'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest(),
              'ranges': ranges, 'operation': 'local_file_read', 'result': 'success'}
    READS.append(record)
    items = []
    for first, last in ranges:
        items.extend({'line': line, 'text': lines[line - 1]}
                     for line in range(first, min(last, len(lines)) + 1))
    EXTRACTS.append({**record, 'lines': items})
    return record


def dump(name, data):
    (OUT / name).write_text(json.dumps(data, ensure_ascii=False, indent=2) + '\n')


def main():
    now = datetime.datetime.now(datetime.timezone.utc).isoformat()
    paths = {
        'cpu_adapter': evidence(STAGE + 'cva6/corev_apu/fpga/ariane.cfg', [(1,24), (40,44)]),
        'cpu_dtm': evidence(STAGE + 'cva6/corev_apu/riscv-dbg/src/dmi_jtag.sv', [(19,33), (224,244)]),
        'cpu_tap': evidence(STAGE + 'cva6/corev_apu/riscv-dbg/src/dmi_jtag_tap.sv',
                            [(19,30), (65,71), (109,124), (144,175), (198,211), (335,344)]),
        'cpu_wiring': evidence(STAGE + 'cva6/corev_apu/fpga/src/ariane_xilinx.sv', [(325,342)]),
        'cpu_pins': evidence(STAGE + 'cva6/corev_apu/fpga/constraints/genesys-2.xdc', [(1,9)]),
        'board_interface_description': evidence('cva6/README.md', [(255,275)]),
        'mac_first_board': evidence('out/r107-execution-gate-20261012/route/mac-reference/FIRST_BOARD.md', [(1,10),(24,38)]),
        'mac_current_handoff': evidence('out/r107-execution-gate-20261012/route/mac-reference/tools--composition-isolated-20261007--HANDOFF.md', [(1,9)]),
        'mac_programmer': evidence('out/r107-execution-gate-20261012/route/mac-reference/tools--composition-isolated-20261007--mac_research2.py', [(521,545)]),
        'mac_original_wrapper': evidence('out/deployment-workflow-review-20261005/mac-snapshot/scripts/fpga.py', [(246,261),(280,294)]),
        'mac_actual_program': evidence('out/composition-isolated-20261007/mac-records/composition-isolated-20261007-boot01/03-fpga-load.stdout', [(1,5),(107,111)]),
        'make_entry': evidence('Makefile', [(1,15)]),
        'build_entry': evidence('scripts/platform.py', [(228,249),(285,297)]),
        'dell_target_enumeration': evidence('out/r107-execution-gate-20261012/route/live-JTAG-enumeration.raw.log', [(59,70)]),
        'dell_main_target_actual': evidence('out/r107-execution-gate-20261012/route/live-JTAG-exact.raw.log', [(27,55)]),
    }
    result_path = ROOT / 'out/composition-isolated-20261007/mac-records/composition-isolated-20261007-boot01/result.json'
    raw = result_path.read_bytes()
    result = json.loads(raw)
    READS.append({'path': str(result_path), 'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest(),
                  'operation': 'local_json_read_and_select_fpga_fields', 'result': 'success'})
    selected = {key: result[key] for key in ['bit','fpga','jtag_hz','status','boot_id','uname_r','started_at','finished_at']}
    selected['commands'] = [item for item in result['commands'] if item.get('name') == 'fpga-load']
    dump('historical-actual-load-selected.json', {'source': str(result_path), 'source_sha256': hashlib.sha256(raw).hexdigest(), 'selected': selected})
    comparisons = []
    for name in ['cva6/corev_apu/fpga/ariane.cfg','cva6/corev_apu/riscv-dbg/src/dmi_jtag.sv',
                 'cva6/corev_apu/riscv-dbg/src/dmi_jtag_tap.sv','cva6/corev_apu/fpga/constraints/genesys-2.xdc']:
        current = (ROOT / name).read_bytes()
        frozen = (ROOT / STAGE / name).read_bytes()
        comparisons.append({'current': str(ROOT / name), 'frozen': str(ROOT / STAGE / name),
                            'byte_equal': current == frozen, 'sha256': hashlib.sha256(frozen).hexdigest()})
        READS.extend([{'path': str(ROOT / name), 'operation':'local_file_read_for_byte_comparison','result':'success'},
                      {'path': str(ROOT / STAGE / name), 'operation':'local_file_read_for_byte_comparison','result':'success'}])
    dump('key-source-extracts.json', EXTRACTS)
    (OUT / 'key-source-extracts.txt').write_text('\n\n'.join(
        item['path'] + '\nSHA256=' + item['sha256'] + '\n' +
        '\n'.join(str(line['line']) + ': ' + line['text'] for line in item['lines'])
        for item in EXTRACTS) + '\n')
    findings = {
        'schema':'r110-jtag-history-interface-evidence-v1','created_utc':now,
        'author':'Codex gpt-6.1-sol / ultra','scope':'critical local source/log review; no device operations',
        'verdict':'Current main-target IDCODE 0x1 exactly matches the platform CPU soft JTAG TAP. Native configuration-target identity requires root B-target observation.',
        'historical_chain':{
            'dell_role':'Vivado FPGA build and artifact delivery; this selected historical execution was on Mac',
            'mac_role':'native openFPGALoader SRAM load',
            'actual_loader':selected['fpga']['loader'],'actual_argv':selected['commands'][0]['argv'],
            'requested_and_reported_real_hz':1000000,'load_returncode':selected['fpga']['returncode'],
            'post_load_ir':{'isc_done':1,'isc_ena':0,'init':1,'done':1},
            'actual_record':str(result_path),
            'historic_idcode_reference':'0x03651093 / xc7k325t in FIRST_BOARD.md; original detect directory not imported among the four selected Mac files',
            'six_mhz_boundary':'FIRST_BOARD.md reports rc0 with DONE0 and no boot at default 6MHz; successful 1MHz retry. Cause of 6MHz failure remains unknown.',
        },
        'interface_separation':{
            'source':paths['cpu_adapter']['path'],'source_lines':[1,4,6,7,13,14,15,16,17,19,21,24],
            'cpu_debug_ftdi_channel':0,'configuration_ftdi_channel_documented':1,
            'cpu_debug_ir_bits':5,'cpu_debug_idcode_default':'0x00000001',
            'host_low_byte_initial_data':'0x18','host_low_byte_direction_mask':'0x1b',
            'cpu_debug_signal_map':[
                {'ftdi':'ADBUS0','signal':'TCK','host_direction':'output','fpga_pin':'AD27'},
                {'ftdi':'ADBUS1','signal':'TDI','host_direction':'output','fpga_pin':'W27'},
                {'ftdi':'ADBUS2','signal':'TDO','host_direction':'input','fpga_pin':'W28'},
                {'ftdi':'ADBUS3','signal':'TMS','host_direction':'output','fpga_pin':'W29'},
                {'ftdi':'ADBUS4','signal':'nTRST','host_direction':'output per mask bit4; original comment In/Out','fpga_pin':'Y29'},
            ],
            'initial_data_inference':'TMS and nTRST bits are high in 0x18; nTRST is defined using -ndata0x10. This is local config evidence, not an instruction to apply that layout.',
            'cpu_tap_call_chain':[
                {'path':paths['cpu_wiring']['path'],'lines':[325,336,337,338,339,340],'finding':'unparameterized dmi_jtag instance; external CPU debug pins connected directly'},
                {'path':paths['cpu_dtm']['path'],'lines':[20,224,225,226],'finding':'default IdcodeValue1 passed to TAP with IrLength5'},
                {'path':paths['cpu_tap']['path'],'lines':[22,67,116,123,150,173,208,338],'finding':'IDCODE selected after TAP reset; capture/reset load1; TDO shifts stored IDCODE'},
            ],
            'native_ftdi_pin_layout':'Not established from an available local openFPGALoader source/binary. CPU GPIO XDC pins must not be used as the native Kintex configuration JTAG pin map.',
            'vivado_suffix_mapping':'Serial without B versus B must be established by actual device ID observation, not assumed from the suffix alone.',
            'current_frozen_byte_comparisons':comparisons,
        },
        'stage109_actual':{'selected_target_suffix':'200300BD5148','jtag_hz':1000000,
                           'device_count':1,'part':'Unknown_Device','idcode':'0x00000001',
                           'b_target_enumerated':True,'b_target_identity_in_this_subtask':'not queried'},
        'minimal_root_action':{
            'kind':'one bounded native-configuration-target identification query using existing Vivado/hw_server',
            'target_suffix_exact':'200300BD5148B','jtag_hz':1000000,
            'steps':['Ensure the task-owned earlier hw_server has been released or exclusively reuse one root-owned instance.',
                     'Select one get_hw_targets item whose final path component equals 200300BD5148B.',
                     'Set PARAM.FREQUENCY1000000, open_hw_target, get_hw_devices; refresh and report PART/IDCODE/configuration registers if available.',
                     'Close target, manager/server connections; clean up only root-owned processes.'],
            'not_included':['program','flash','DMIACCESS','CPU halt','CPU reset','OpenOCD ariane.cfg execution','driver/USB binding or installation','Linux restart'],
            'accept':['Exact B target is unique and opens at 1MHz.',
                      'Observed FPGA is xc7k325t and IDCODE low28 bits equal0x03651093 (revision nibble may differ).',
                      'Keep raw query output and pair it with the existing main-target1 log and frozen-source call chain.'],
            'stop':['B target missing or ambiguous; open/query fails; device unknown or wrong FPGA ID.',
                    'No blind requery, installation, driver change, frequency matrix or reprogramming follows from this subtask.'],
            'accepted_claim_if_passed':'This distinguishes soft CPU-debug chain from native FPGA configuration chain and explains stage109 Unknown_Device1 as the CPU TAP identity.',
            'still_not_proven_if_passed':['exact current BIT bytes/hash','runtime S/N/P/PN','physical CPU clock','full timing/CDC/IO acceptance','ordinary R107 restore outcome'],
        },
        'local_tool_search':{'PATH_openFPGALoader':None,'PATH_openocd':None,
                             'bounded_locations_checked':['~/.local/bin top level','/usr/local/bin top level','/opt top level','/tmp top-level fpga/jtag/ftdi names','work/tools top level'],
                             'claim':'No executable/source entry was located in those bounded paths; not an exhaustive filesystem absence claim.',
                             'help_executed':False,'detect_executed':False,'search_stopped_when_cpu_tap_source_found':True},
        'observed_uncertainties':['No actual B-target hardware result belongs to this read-only subtask.',
                                  'The historic FIRST_BOARD detect result is currently supported by the imported record, not its unimported original detect output.',
                                  'A source/log match explains the0x1 observation but by itself cannot authenticate which current BIT is loaded.'],
        'devices_opened':0,'servers_started':0,'sudo_invocations':0,'installations':0,
        'shared_documents_modified':False,'external_models_called':[],
    }
    b_log = OUT / 'B-FPGA-status.raw.log'
    b_result = OUT / 'B-FPGA-status-result.json'
    if b_log.is_file() and b_result.is_file():
        lines = b_log.read_text().splitlines()
        props = {}
        for line in lines:
            if line.startswith('R110_PROP ') and ' xc7k325t_0 ' in line:
                key, value = line.split(' ', 2)[2].split('=', 1)
                if key in ['PART','IDCODE','REGISTER.CONFIG_STATUS','REGISTER.USERCODE','REGISTER.USR_ACCESS',
                           'REGISTER.CONFIG_STATUS.BIT00_CRC_ERROR',
                           'REGISTER.CONFIG_STATUS.BIT04_END_OF_STARTUP_(EOS)_STATUS',
                           'REGISTER.CONFIG_STATUS.BIT06_GWE_STATUS',
                           'REGISTER.CONFIG_STATUS.BIT14_DONE_PIN',
                           'REGISTER.CONFIG_STATUS.BIT15_IDCODE_ERROR']:
                    props[key] = value
        receipt = json.loads(b_result.read_text())
        observed_id = int(props['IDCODE'],2)
        passed = (receipt['client_exit_code'] == 0 and props['PART'] == 'xc7k325t'
                  and observed_id & 0x0fffffff == 0x03651093
                  and 'R110_TARGET=127.0.0.1:3123/xilinx_tcf/Digilent/200300BD5148B' in lines
                  and 'R110_JTAG_HZ=1000000' in lines and 'R110_DEVICE_COUNT=1' in lines)
        findings['root_later_B_query'] = {
            'executed_by':'root','reviewed_by':'this read-only Codex subagent',
            'raw_log':str(b_log),'result':str(b_result),'client_exit_code':receipt['client_exit_code'],
            'accepted_interface_discriminator':passed,'idcode_hex':f'0x{observed_id:08x}',
            'config_status_hex':f"0x{int(props['REGISTER.CONFIG_STATUS'],2):08x}",
            'selected_properties':props,
            'scope_limit':'Current native Kintex chain and configuration-complete state proved; current BIT hash/source/mode/clock not authenticated.'}
        findings['stage109_actual']['b_target_identity_in_this_subtask'] = 'root query subsequently reviewed; xc7k325t /0x43651093 at1MHz'
        findings['minimal_root_action']['execution_status'] = 'executed_by_root; accepted' if passed else 'executed_by_root; not_accepted'
        if passed:
            findings['verdict']='Root B-target xc7k325t/0x43651093 at1MHz plus frozen CPU TAP IDCODE1 resolves stage109 Unknown_Device1 as CPU-debug versus native-configuration interface selection.'
            findings['observed_uncertainties']=[
                'Native-chain query does not authenticate exact current BIT bytes/source, runtime mode, physical clock or ordinary R107 recovery.',
                'Historic FIRST_BOARD detect directory remains unimported among the four selected Mac reference files.',
                'Native-channel custom FTDI layout was not established from local programmer source; no custom driver is required for the accepted existing-tool query.']
        evidence(str(b_log.relative_to(ROOT)),[(27,56),(75,91),(126,138)])
        READS.append({'path':str(b_result),'operation':'local_json_receipt_read','result':'success'})
        dump('key-source-extracts.json',EXTRACTS)
        (OUT/'key-source-extracts.txt').write_text('\n\n'.join(
            item['path']+'\nSHA256='+item['sha256']+'\n'+
            '\n'.join(str(line['line'])+': '+line['text'] for line in item['lines'])
            for item in EXTRACTS)+'\n')
    dump('history-interface-evidence.json',findings)
    ledger={
        'schema':'r110-jtag-history-interface-ledger-v1','created_utc':now,
        'artifact_extraction_command':[sys.executable,str(Path(__file__).resolve())],
        'artifact_extraction_argv_actual':sys.argv,
        'actual_file_reads':READS,
        'preceding_read_only_discovery_transcript':[
            {'description':'initial filename discovery was broader than needed; transcript is authoritative for the exact command. Subsequent reads were narrowed to named sources and artifact directories.'},
            {'chunk_id':'4ae157','exit_code':0,'command':'sed -n 1,260p AGENTS.md','cwd':'/home/xvhaocheng/work'},
            {'chunk_id':'1a0d89','exit_code':0,'command':'cat AGENTS.md','cwd':'/home/xvhaocheng/work/iommu-platform'},
            {'chunk_id':'cd32d8','exit_code':0,'command':'cat CURRENT_STATUS.md','cwd':'/home/xvhaocheng/work'},
            {'chunk_id':'4f0c7b','exit_code':0,'command_summary':'numbered stage109 diagnosis and actual exact/enumeration logs'},
            {'chunk_id':'3120ee','exit_code':0,'command':'rg --files out/r107-execution-gate-20261012/route/mac-reference'},
            {'chunk_id':'4c15b6','exit_code':0,'command_summary':'directed jtag/openFPGALoader search in Makefile/README/scripts and named docs/HANDOFF','output_truncated':True},
            {'chunk_id':'91e4c6','exit_code':0,'command_summary':'Python shutil.which and bounded top-level executable/source directory names'},
            {'chunk_id':'b38822','exit_code':0,'command_summary':'rg -n -C5 JTAG/tool identity fields in four imported Mac reference files'},
            {'chunk_id':'95ee91','exit_code':0,'command_summary':'Makefile and exact mac_research2 programmer interfaces'},
            {'chunk_id':'29723d','exit_code':0,'command':'cat out/r107-execution-gate-20261012/route/mac-reference/AGENTS.md'},
            {'chunk_id':'a9e8e5','exit_code':0,'command_summary':'initial tool/source-snapshot JSON read plus historical boot-result read','output_truncated':True,
             'acceptance':'snapshot was unexpectedly embedded full source; the truncated broad output was not used as proof. Subsequent reads used exact fpga.py and selected JSON fields only.'},
            {'chunk_id':'9e241e','exit_code':2,'command_summary':'CVA6 README selected lines and globbed cfg/XDC search','failure':'two cfg glob paths absent; relevant ariane.cfg results were present'},
            {'chunk_id':'b09a02','exit_code':0,'command_summary':'docs/build and selected platform.py build entry'},
            {'chunk_id':'b9f483','exit_code':0,'command_summary':'numbered original dmi_jtag/tap/top source call chain'},
            {'chunk_id':'f75436','exit_code':0,'command_summary':'existing composition evidence and exact frozen-stage path checks'},
            {'chunk_id':'2a45c9','exit_code':0,'command_summary':'historical fpga.py command and existing TAP-only XVC helper source'},
            {'chunk_id':'d6d562','exit_code':0,'command_summary':'read root-produced B status local log and receipt; this subagent did not perform the query'},
        ],
        'timestamp_caveat':'Earlier discovery timestamps were not collected; tool transcript IDs identify actual calls. No invented start/end times are supplied.',
        'execution_boundary':{'only_local_reads_and_artifact_writes':True,'no_device_or_server_operations':True,
                              'no_sudo_install_binding_or_restart':True,'no_Secret_Service_or_key_reads':True},
    }
    dump('execution-ledger.json',ledger)
    print(json.dumps({'status':'written','output':str(OUT),'input_reads':len(READS),
                      'decisive_source':paths['cpu_dtm']['path'],'device_actions':0},ensure_ascii=False))


if __name__ == '__main__':
    main()

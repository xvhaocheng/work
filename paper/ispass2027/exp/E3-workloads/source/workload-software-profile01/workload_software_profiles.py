"""Exactly two accepted software profiles; hardware CVA6 remains independently1d."""
from workload_records import require

LEGACY_LINUX='65b427946152e3aca27babab0a437821729805d4'
LEGACY_PLATFORM='ec99648e3c9a1e48c1e9721e3662bb632eb439bd'
HARDWARE_CVA6='1d587125eee690e623e6ffae13e70f9bbe989adc'
LEGACY_RELEASE='6.5.0-rc2-ispass2027-full-e3-v1'
G_MANIFEST_SHA='4bee1eadd8d8102e652129833f171ba869e5edd8a1a039973d13559a3a0b2f6c'
G_LINUX='69d8b9d4549c473477bcdc1df7c09ee1bb55528c'
G_PLATFORM='6491dfd76fc92dc020d17166b327da93605f58b1'
G_REFERENCE='cfb9a0a8bb70b32be6036bc112a5189d6e2aa0ab'
G_RELEASE='6.5.0-rc2-ispass2027-g-v1'
UAPI_SHA='f7b17880f29d2fe2f785952919281a2f66eadbbde9288d20684ba7ae0d03713f'
PRESERVED={
 'usr/bin/idma_experiment':'c25edc4b3cac1a01a3ef8a81670f73c38a6f8c3d0633da17cc601663fc4a5d18',
 'usr/bin/idma_p0':'f949e6b0d00444f837820e34018e4189a8dfa0d6f46b4b3509e3f7c3b41e6c63',
 'usr/bin/idma_scaling':'afbfbc76893837d300067e8cc5ac70cb5dd42f061a61836cfc09c33bf0695d5b',
 'usr/bin/idma_pages':'1fed2b3525de25bd5b8125eb6761c3c84030a043f7e74e8eda25b66ebf1507c2',
 'usr/bin/idma_workload':'7cd7ae4075404ec53bc3ef20437ac72a5341d8ae49ecb46350a1de6bf11a2119',
 'usr/bin/platform-timeout':'2a2fe20789d0087364ad1afacff854e659ac2477fbb3eb28b80ac2d9236bf733',
 'usr/bin/platform-sd-update':'6fbd639e07117312b7cda5abd445ca21d4e5e205bdf3dcdc0161cd67bde61a04'}


def select(software,manifest_sha256):
    s=software
    require(s['status']=='passed' and s['selected_cva6_reference_only'] is True,'ABI5 passed reference-only software required')
    if (s['selected_linux'],s['selected_platform'],s['selected_cva6'])==(LEGACY_LINUX,LEGACY_PLATFORM,HARDWARE_CVA6):
        # Original65b checks and accepted fixture behavior remain unchanged.
        require(s['kernel_critical_sources']['drivers/misc/idma.c']=='38316b66af557509653d11c9035c7c9d0e093aa68b632449710c88f6adab13e5'
                and s['kernel_critical_sources']['include/uapi/linux/idma_workload.h']==UAPI_SHA,'ABI5 software critical sources')
        name,release,reference='full_e3_65b_v1',LEGACY_RELEASE,HARDWARE_CVA6
    else:
        require(s['schema']=='ispass2027-g-software-v1' and manifest_sha256==G_MANIFEST_SHA,'unknown ABI5 software profile/manifest')
        require((s['selected_linux'],s['selected_platform'],s['selected_cva6'],s['kernel_release'])==(G_LINUX,G_PLATFORM,G_REFERENCE,G_RELEASE)
                and s['selected_cva6_role']=='source_reference_not_loaded_bit','ABI5 exact G software tuple/reference required')
        require(s['kernel_critical_sources']['drivers/misc/idma.c']=='dfdad8be6cc6ff1db7232c32694a57be30d7cbcdd4e7e794d2170bfcbae39158'
                and s['kernel_critical_sources']['include/uapi/linux/idma_workload.h']==UAPI_SHA,'ABI5 G driver/UAPI bytes')
        require(s['common_software_manifest_sha256']=='7415dcc4d076fa434bc22294f4309582af84086964f1b47bb05c695555f0bcc4'
                and s['preserved_rootfs_entries']==414 and all(s['preserved_executables'][k]==v for k,v in PRESERVED.items()),'ABI5 G original CLI/rootfs bytes changed')
        require(s['artifacts']['linux-build/arch/riscv/boot/Image']=='762418037e5c163a26b34a8656279803e6727804f1b452f2ac6da2a6759de36b','ABI5 G actual Image identity')
        name,release,reference='g_shared_69d_v1',G_RELEASE,G_REFERENCE
    return {'name':name,'kernel_release':release,'software_cva6_reference_only':True,
            'software_cva6_reference':reference,'required_actual_fpga_cva6':HARDWARE_CVA6,
            'scope':'unchanged ABI5 workload protocol; software reference is not loaded BIT identity'}

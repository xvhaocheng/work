#!/usr/bin/env python3
"""Finite E2 software tuples; the baseline source remains hardware provenance."""
import hashlib
from pathlib import Path

if not __debug__:
    raise RuntimeError('Run without Python -O: E2 identity checks require assertions')

CLI_SHA256='b1433570dc7f7f4971b0926cc64478d6ca872f4275cf6d1b8274d735870a5523'
DS0_BIT_SHA256='e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6'
BASELINE_COMMITS={
    'platform':'885ed0a7ea90ed42dae2b7a2dadfdd0acba15273',
    'cva6':'8aac3fcd345af7686a55af1867783fd6b91a0069',
    'iommu':'85b157ebe71a623e326e51fff7b7126fe77e8eb9',
    'linux':'45bf76b15e36cf608dff95013d91f8bedb7b1b8e',
    'opensbi':'e8d21bfdc2dd753314627968e496c3a1188f6dfb',
}
PROFILES={
    'p0-multi-v1':{
        'selected_linux':'acece3a3ab7d72ba1f936c1984b82a4a55f28d46',
        'selected_platform':'52c5fdd028bbf3b07deaf1d0f7e13e3a271b29d0',
        'selected_cva6':'8aeb9ac9b3aea82a975cc789cb47633fb29bf5c8',
        'selected_opensbi':'e8d21bfdc2dd753314627968e496c3a1188f6dfb',
        'kernel_release':'6.5.0-rc2-ispass2027-p0-multi-v1',
    },
    'full-e3-v1':{
        'selected_linux':'65b427946152e3aca27babab0a437821729805d4',
        'selected_platform':'ec99648e3c9a1e48c1e9721e3662bb632eb439bd',
        'selected_cva6':'1d587125eee690e623e6ffae13e70f9bbe989adc',
        'selected_opensbi':'e8d21bfdc2dd753314627968e496c3a1188f6dfb',
        'kernel_release':'6.5.0-rc2-ispass2027-full-e3-v1',
    },
}


def helper_sha256():return hashlib.sha256(Path(__file__).read_bytes()).hexdigest()


def validate_software(software,expected_kernel):
    matches=[name for name,profile in PROFILES.items() if all(software.get(k)==v for k,v in profile.items())]
    assert len(matches)==1,'unknown E2 software profile: exact accepted tuple required'
    assert software['status']=='passed','completed existing software build manifest required'
    assert software['kernel_release']==expected_kernel,'software kernel identity'
    assert software['handwritten_inputs']['bench/idma_scaling.c']==CLI_SHA256,'frozen CLI input'
    assert 'bin/idma_scaling' in software['artifacts'],'CLI included in completed build'
    return matches[0]


def bind_identity(software,expected_kernel,source,loaded):
    name=validate_software(software,expected_kernel)
    assert loaded['status']=='boot_identity_verified' and loaded['kernel_release']==expected_kernel,'load/kernel identity'
    assert loaded['bit_sha256']==DS0_BIT_SHA256,'E2 requires the existing original DS0 BIT'
    assert source['actual_commits']==BASELINE_COMMITS,'immutable original DS0 hardware source required'
    runtime={'platform':software['selected_platform'],'linux':software['selected_linux'],
             'opensbi':software['selected_opensbi'],'cva6':BASELINE_COMMITS['cva6'],'iommu':BASELINE_COMMITS['iommu']}
    return {'software_profile':name,'software_profile_helper_sha256':helper_sha256(),
            'runtime_commits':runtime,'software_build_reference':{k:software[k] for k in PROFILES[name]},
            'source_roles':{
                'selected_source':'original DS0 hardware provenance; its Linux/platform remain historical build references',
                'software_build_reference.selected_cva6':'software build reference only; loaded hardware is runtime_commits.cva6',
                'runtime_commits':'actual running software sources and actual loaded original DS0 hardware sources'}}


def validate_recorded(manifest,software,source,loaded,*,software_sha256,source_sha256,collector_sha256):
    identity=bind_identity(software,manifest['kernel_release'],source,loaded)
    assert all(manifest.get(k)==v for k,v in identity.items()),'recorded software/runtime profile differs'
    assert manifest['software_manifest_sha256']==software_sha256,'software receipt bytes changed'
    assert manifest['source_record_sha256']==source_sha256,'hardware source receipt bytes changed'
    assert manifest['script_sha256']==collector_sha256,'actual accepted collector source changed'
    return identity

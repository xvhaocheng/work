#!/usr/bin/env python3
"""Finite, accepted E4 software identities and their loaded-hardware roles.

Only a complete, reviewed tuple belongs here. An ABI5 Linux commit alone does
not authorize a platform, kernel release, or software CVA6 build reference.
"""
if not __debug__:
    raise RuntimeError('Run without Python -O: software profile checks require assertions')
HARDWARE_CVA6 = '6c5c666d45979d3a03fb98cff89ba7ad095f6125'
HARDWARE_IOMMU = '85b157ebe71a623e326e51fff7b7126fe77e8eb9'
BIT_SHA256 = 'd934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c'
CLI_SHA256 = '264e75c6dc20d597e97831038c15b87b9d8274440661dbfb75505c9cedbe5314'
PROFILE_FIELDS = ('selected_linux', 'selected_platform', 'selected_cva6', 'kernel_release')
SOFTWARE_CVA6_ROLE = 'software build reference only; not loaded hardware'
FPGA_LINUX_ROLE = 'Linux in the original FPGA source manifest is a build reference only; running Linux is actual_commits.linux'
PROFILES = {
    'e4-e5-v1': {
        'selected_linux': 'da6b69fc007ad5e671dc288dd8038d02e3ebe43d',
        'selected_platform': 'a68603f0fc668771bc0c9e3a86fcefe289bfa17b',
        'selected_cva6': HARDWARE_CVA6,
        'kernel_release': '6.5.0-rc2-ispass2027-e4-e5-v1',
    },
}


def validate_software(software, expected_kernel):
    matches = [name for name, profile in PROFILES.items()
               if all(software.get(key) == profile[key] for key in PROFILE_FIELDS)]
    assert len(matches) == 1, 'unknown E4 software profile: exact accepted tuple required'
    name = matches[0]
    assert software['status'] == 'passed' and software['kernel_release'] == expected_kernel, 'software status/kernel binding'
    assert software['handwritten_inputs']['bench/idma_pages.c'] == CLI_SHA256, 'unchanged E4 CLI required'
    assert all('bin/' + binary in software['artifacts'] for binary in
               ['idma_experiment', 'platform-timeout', 'platform-sd-update', 'idma_p0', 'idma_scaling', 'idma_pages'])
    assert software['recovery']['kernel_release'] == expected_kernel, 'SPI recovery kernel binding'
    return name


def validate_bound_software(bound, software, source, fpga):
    """Bind actual Linux/software sources separately from the loaded E5 FPGA."""
    name = validate_software(software, bound['kernel_release'])
    expected = {'linux': software['selected_linux'], 'platform': software['selected_platform'],
                'opensbi': software['selected_opensbi'], 'cva6': HARDWARE_CVA6, 'iommu': HARDWARE_IOMMU}
    assert source['actual_commits'] == bound['actual_commits'], 'source/bound component binding'
    assert all(bound['actual_commits'].get(key) == value for key, value in expected.items()), 'software/loaded-hardware component binding'
    assert bound['bit_sha256'] == BIT_SHA256, 'fixed accepted E5 BIT required'
    assert fpga['selected_cva6'] == HARDWARE_CVA6 and fpga['selected_iommu'] == HARDWARE_IOMMU
    # The old immutable package predates these role annotations and stays valid.
    if name != 'e4-e5-v1':
        assert source['software_profile'] == name, 'source software profile binding'
        reference = source['software_build_reference']
        assert all(reference.get(key) == software[key] for key in PROFILE_FIELDS + ('selected_opensbi',)), 'software build reference binding'
        assert reference['cva6_role'] == SOFTWARE_CVA6_ROLE, 'software CVA6 is only a build reference'
        reference = source['fpga_build_reference']
        keys = ('selected_platform', 'selected_cva6', 'selected_iommu', 'source_manifest', 'source_manifest_sha256')
        assert all(reference.get(key) == fpga[key] for key in keys), 'original FPGA build reference binding'
        assert reference['linux_role'] == FPGA_LINUX_ROLE, 'FPGA Linux reference is not the running Linux'
    return name

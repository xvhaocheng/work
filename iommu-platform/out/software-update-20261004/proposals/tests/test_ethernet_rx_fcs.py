import json
import os
import shutil
import struct
import subprocess
import tempfile
import unittest
import zlib
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
RTL_DIR = REPO_ROOT / 'cva6' / 'corev_apu' / 'fpga' / 'src' / 'ariane-ethernet'
AXIS_SV = RTL_DIR / 'axis_gmii_rx.sv'
LFSR_SV = RTL_DIR / 'rgmii_lfsr.sv'

HARNESS_CPP = r'''
#include "verilated.h"
#include "Vaxis_gmii_rx.h"

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

static void tick(Vaxis_gmii_rx& dut) {
    dut.clk = 0;
    dut.eval();
    dut.clk = 1;
    dut.eval();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: harness <frame_file>" << std::endl;
        return 2;
    }

    std::ifstream in(argv[1], std::ios::binary);
    if (!in) {
        std::cerr << "cannot open frame file" << std::endl;
        return 2;
    }
    std::vector<uint8_t> frame((std::istreambuf_iterator<char>(in)),
                               std::istreambuf_iterator<char>());

#if defined(VM_VERSION_MAJOR) && (VM_VERSION_MAJOR >= 5)
    VerilatedContext context;
    context.commandArgs(argc, argv);
    Vaxis_gmii_rx dut(&context);
#else
    Verilated::commandArgs(argc, argv);
    Vaxis_gmii_rx dut;
#endif

    dut.rst = 1;
    dut.clk_enable = 1;
    dut.mii_select = 0;
    dut.gmii_rxd = 0;
    dut.gmii_rx_dv = 0;
    dut.gmii_rx_er = 0;

    for (int i = 0; i < 8; i++) {
        tick(dut);
    }
    dut.rst = 0;
    for (int i = 0; i < 4; i++) {
        tick(dut);
    }

    std::vector<uint8_t> captured;
    bool tlast_seen = false;
    bool tuser_seen = false;
    bool error_bad_fcs_seen = false;
    unsigned int extra_after_tlast = 0;
    unsigned int cycles = 0;

    auto sample = [&]() {
        cycles++;
        if (dut.m_axis_tvalid) {
            if (tlast_seen) {
                extra_after_tlast++;
            } else {
                captured.push_back(static_cast<uint8_t>(dut.m_axis_tdata));
                if (dut.m_axis_tuser) {
                    tuser_seen = true;
                }
                if (dut.m_axis_tlast) {
                    tlast_seen = true;
                }
            }
        }
        if (dut.error_bad_fcs) {
            error_bad_fcs_seen = true;
        }
    };

    std::vector<uint8_t> wire;
    for (int i = 0; i < 7; i++) {
        wire.push_back(0x55);
    }
    wire.push_back(0xD5);
    wire.insert(wire.end(), frame.begin(), frame.end());

    for (size_t i = 0; i < wire.size(); i++) {
        dut.gmii_rxd = wire[i];
        dut.gmii_rx_dv = 1;
        dut.gmii_rx_er = 0;
        tick(dut);
        sample();
    }

    dut.gmii_rx_dv = 0;
    dut.gmii_rxd = 0;
    dut.gmii_rx_er = 0;
    for (int i = 0; i < 32; i++) {
        tick(dut);
        sample();
    }

    std::ostringstream hex;
    hex << std::hex << std::setfill('0');
    for (size_t i = 0; i < captured.size(); i++) {
        hex << std::setw(2) << static_cast<unsigned int>(captured[i]);
    }

    const char quote = static_cast<char>(34);
    std::cout << '{'
              << quote << "captured_hex" << quote << ':' << quote << hex.str() << quote << ','
              << quote << "captured_len" << quote << ':' << captured.size() << ','
              << quote << "tlast_seen" << quote << ':' << (tlast_seen ? "true" : "false") << ','
              << quote << "tuser_seen" << quote << ':' << (tuser_seen ? "true" : "false") << ','
              << quote << "error_bad_fcs_seen" << quote << ':' << (error_bad_fcs_seen ? "true" : "false") << ','
              << quote << "extra_after_tlast" << quote << ':' << extra_after_tlast << ','
              << quote << "cycles" << quote << ':' << cycles
              << '}' << std::endl;

    dut.final();
    return 0;
}
'''

class EthernetRxFcsTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if shutil.which('verilator') is None:
            raise unittest.SkipTest('verilator not installed')
        if not AXIS_SV.is_file() or not LFSR_SV.is_file():
            raise unittest.SkipTest('axis_gmii_rx.sv/rgmii_lfsr.sv not found')
        cls.tmp = tempfile.TemporaryDirectory(prefix='axis-gmii-rx-fcs-')
        cls.build_dir = Path(cls.tmp.name)
        cpp_path = cls.build_dir / 'harness.cpp'
        cpp_path.write_text(HARNESS_CPP, encoding='utf-8')
        obj_dir = cls.build_dir / 'obj_dir'
        cmd = [
            'verilator',
            '--cc',
            '--exe',
            '--build',
            '-Wno-fatal',
            '-Wno-WIDTH',
            '-Wno-CASEINCOMPLETE',
            '-Wno-UNOPTFLAT',
            '-Wno-DECLFILENAME',
            '-Wno-SYNCASYNCNET',
            '-Wno-PINCONNECTEMPTY',
            '-Wno-EOFNEWLINE',
            '-Wno-TIMESCALEMOD',
            '--top-module',
            'axis_gmii_rx',
            '-Mdir',
            str(obj_dir),
            '-o',
            'harness',
            str(AXIS_SV),
            str(LFSR_SV),
            str(cpp_path),
        ]
        proc = subprocess.run(
            cmd,
            cwd=str(cls.build_dir),
            capture_output=True,
            text=True,
            timeout=600,
        )
        if proc.returncode != 0:
            cls.tmp.cleanup()
            raise AssertionError(
                'verilator build failed:' + chr(10) + 'STDOUT:' + chr(10) + proc.stdout + chr(10) + 'STDERR:' + chr(10) + proc.stderr
            )
        candidates = sorted(cls.build_dir.rglob('harness'))
        if not candidates:
            candidates = sorted(cls.build_dir.rglob('Vaxis_gmii_rx'))
        if not candidates:
            cls.tmp.cleanup()
            raise AssertionError('verilator did not produce a harness executable')
        cls.harness = candidates[0]

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    @staticmethod
    def _payload(length):
        return bytes(((i * 37 + 11) & 0xff) for i in range(length))

    def _run_harness(self, frame_bytes, timeout=120):
        with tempfile.TemporaryDirectory(prefix='frame-') as td:
            path = Path(td) / 'frame.bin'
            path.write_bytes(frame_bytes)
            proc = subprocess.run(
                [str(self.harness), str(path)],
                cwd=str(self.build_dir),
                capture_output=True,
                text=True,
                timeout=timeout,
            )
        self.assertEqual(proc.returncode, 0, proc.stderr)
        return json.loads(proc.stdout)

    def _assert_good_frame(self, payload):
        fcs = struct.pack('<I', zlib.crc32(payload) & 0xffffffff)
        frame = payload + fcs
        result = self._run_harness(frame)
        captured = bytes.fromhex(result['captured_hex'])
        self.assertEqual(len(captured), len(frame))
        self.assertEqual(captured, frame)
        self.assertTrue(result['tlast_seen'])
        self.assertEqual(result['extra_after_tlast'], 0)
        self.assertFalse(result['error_bad_fcs_seen'])
        self.assertEqual(
            zlib.crc32(captured[:-4]) & 0xffffffff,
            struct.unpack('<I', captured[-4:])[0],
        )

    def test_60_byte_payload_good_fcs(self):
        self._assert_good_frame(self._payload(60))

    def test_1514_byte_payload_good_fcs(self):
        self._assert_good_frame(self._payload(1514))

    def test_corrupted_fcs_is_captured_and_detected_from_bytes(self):
        payload = self._payload(60)
        good_fcs = zlib.crc32(payload) & 0xffffffff
        bad_fcs = good_fcs ^ 0xffffffff
        frame = payload + struct.pack('<I', bad_fcs)
        result = self._run_harness(frame)
        captured = bytes.fromhex(result['captured_hex'])
        self.assertEqual(len(captured), len(payload) + 4)
        self.assertEqual(captured, frame)
        self.assertTrue(result['tlast_seen'])
        self.assertEqual(result['extra_after_tlast'], 0)
        self.assertNotEqual(
            zlib.crc32(captured[:-4]) & 0xffffffff,
            struct.unpack('<I', captured[-4:])[0],
        )
        self.assertTrue(result['error_bad_fcs_seen'])


if __name__ == '__main__':
    unittest.main()

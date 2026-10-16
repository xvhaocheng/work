## Summary

Scope honored: **tests only**, no production edits. Three artifacts change/create:

| Path | Change |
|---|---|
| `tests/test_dma_p4m_observer.py` | add a 4th Verilator variant `research-path` (PathEnable=1 + `dma_path_observer.sv`) |
| `tests/dma_p4m_observer_test.cpp` | 3-way magic expectation (V9/V8/V7), `path_research` mode, `arm_p4m(...,path=)`, PathEnable-aware path-bit check, two new focused blocks |
| `tests/idma_observation_harness.c` | **new** host harness (mocked MMIO instance lookup, module param) |
| `tests/test_idma_observation.py` | **new** extraction driver (mirrors `tests/test_idma_batch_start.py` style) |

Key comprehension facts used (all from the supplied sources):

* `dma_observer.sv:42-46` magic priority; research=1 && P4M=1 && PathEnable=0 && E3=0 && Controlled=0 is **exactly** the existing `research` variant, so after root's edit it must read **V9**, while the new `PathEnable=1` variant falls through the old priority to **V8** (root: "all other combinations preserve old magic priority").
* ARM path-bit rule is `(!cfg_wdata_i[26] || (PathEnable && cfg_wdata_i[25]))` (`dma_observer.sv:486`) and the index rule is `(PathEnable && 64..192)` (`dma_observer.sv:452`). **Root says these stay unchanged**, so the existing assertion at `dma_p4m_observer_test.cpp:264` ("path bit needs PathEnable") is only valid when `PathEnable=0` — it must be made conditional, otherwise the new variant fails on a *correct* design.
* `dma_observer.sv:504` sets `p4m_n = bit30`; `:1029` (Linux) sets `command |= BIT_ULL(30)` for `o->p4m`. So "ARM bit30+25, bit26 clear" for V9 requires an observation with `p4m=1 && batch=1`.

## Assumptions (please confirm against root's patch)

1. **V9 literal** = `0x49444d414f425309ULL` (taken verbatim from the task text; mirrors `…5308`). Pinned via `#ifndef IDMA_OBS_MAGIC_V9` in the harness so it compiles before and after root adds the driver define.
2. **V9 is accepted by *two* magic guards**: the `o->p4m` guard and the final general guard. This is required by the composition flow (`idma_run_composition` → `idma_run_session2(..., p4m=&p->p4m)` → `.p4m = true`); if root extends only the general guard, `case_v9_arm` will fail and that is a **real interface gap**, not a test bug. (`o->path` forced to `false` is required for the ARM-command assertion.)
3. V9 is **not** added to the E3 / controlled / queue-pair guards (task: "rejects E3/controlled/queue_pair via existing guards").
4. `research-path` = `ResearchEnable=1, P4MEnable=1, PathEnable=1, E3Enable=0, ControlledEnable=0` → magic V8, and it is the variant that needs `dma_path_observer.sv`.
5. No production MMIO arm/check/count, P4M bank, UAPI, or method logic changes.

## 1. `tests/test_dma_p4m_observer.py`

```diff
--- a/tests/test_dma_p4m_observer.py
+++ b/tests/test_dma_p4m_observer.py
@@
-"""Compile and execute the P4M observer contract (P4MEnable=1, PathEnable=0)."""
+"""Compile and execute the P4M observer contract (P4MEnable=1, PathEnable=0/1)."""
 from pathlib import Path
 import subprocess
 
 ROOT = Path(__file__).resolve().parents[1]
-for variant in ('rtl', 'rtl-small', 'research'):
+FPGA = ROOT/'cva6/corev_apu/fpga/src'
+for variant in ('rtl', 'rtl-small', 'research', 'research-path'):
     small = variant == 'rtl-small'
+    research = variant in ('research', 'research-path')
+    with_path = variant == 'research-path'
     out = ROOT/'out/p4m-20260920'/variant
     out.mkdir(parents=True, exist_ok=True)
     args = ['verilator', '--cc', '--exe', '--build', '-Wall', '-Wno-fatal',
             '--top-module', 'dma_observer', '--Mdir', str(out),
             '-GP4MEnable=1',
-            str(ROOT/'cva6/corev_apu/fpga/src/dma_observer.sv'),
-            str(ROOT/'tests/dma_p4m_observer_test.cpp')]
+            str(FPGA/'dma_observer.sv')]
+    if with_path:
+        args += ['-GPathEnable=1', str(FPGA/'dma_path_observer.sv')]
+    args.append(str(ROOT/'tests/dma_p4m_observer_test.cpp'))
     if small:
         args += ['-GCounterWidth=8', '-GOutstandingWidth=3']
-    if variant == 'research':
+    if research:
         args += ['-GResearchEnable=1', '-GE3Enable=0', '-GControlledEnable=0']
     with (out/'build.log').open('w') as log:
         subprocess.run(args, stdout=log, stderr=subprocess.STDOUT, check=True)
-    subprocess.run([str(out/'Vdma_observer')] +
-                   (['small'] if small else ['research'] if variant == 'research' else []), check=True)
+    run_arg = 'small' if small else ('' if variant == 'rtl' else variant)
+    subprocess.run([str(out/'Vdma_observer')] + ([run_arg] if run_arg else []),
+                   check=True)
```

## 2. `tests/dma_p4m_observer_test.cpp`

```diff
--- a/tests/dma_p4m_observer_test.cpp
+++ b/tests/dma_p4m_observer_test.cpp
@@ -1,5 +1,7 @@
-// P4M observer contract regression: P4MEnable=1, PathEnable=0.
+// P4M observer contract regression: P4MEnable=1.
+// Variants: default P4M (V7), small counter model (V7), trimmed research (V9, no path
+// observer) and legacy research that keeps the path observer (V8, PathEnable=1).
 // Independently driven clock and event schedule; no board or formal claim.
@@ -41,12 +43,14 @@
     // P4M arm: bit30 set, batch required, victim must be 0, optional aggressor must be 1.
-    void arm_p4m(unsigned victim,int aggressor,unsigned target){
+    // path=true adds the v3 path bit, which only a PathEnable build accepts.
+    void arm_p4m(unsigned victim,int aggressor,unsigned target,bool path=false){
         d.batch_active_i=3;
         write(0x68,target);
         uint64_t w=1ULL|(1ULL<<30)|(1ULL<<25)|(uint64_t(victim)<<8);
         if(aggressor>=0)w|=(uint64_t(aggressor)<<16)|(1ULL<<24);
+        if(path)w|=(1ULL<<26);
         write(0x60,w);
         check(status()==1,"P4M arm");
     }
@@ -61,10 +65,16 @@
 int main(int argc,char**argv){Verilated::commandArgs(argc,argv);try{
     Bench b;
-    const bool research = argc > 1 && std::string(argv[1]) == "research";
-    check(b.read(0x58)==(research ? 0x49444d414f425308ULL : 0x49444d414f425307ULL),"P4M version magic");
+    const std::string mode = argc > 1 ? std::string(argv[1]) : std::string();
+    // V9 trims the path observer out of the research composition; legacy research keeps
+    // the existing V8 priority and therefore the path observer.
+    const bool path_research = mode == "research-path";
+    const bool research = mode == "research" || path_research;
+    const uint64_t magic = mode == "research" ? 0x49444d414f425309ULL :
+        (path_research ? 0x49444d414f425308ULL : 0x49444d414f425307ULL);
+    check(b.read(0x58)==magic,"P4M version magic");
     check(b.bank(0)==0x49444d4150344d01ULL,"P4M bank capability");
     cases++;
-    if(argc>1 && std::string(argv[1])=="small") {
+    if(mode=="small") {
@@ -264,1 +274,1 @@
-        b.reset(); b.write(0x68,1); b.write(0x60,1ULL|(1ULL<<30)|(1ULL<<25)|(1ULL<<26)); check(b.status()==(1ULL<<32),"path bit needs PathEnable");
+        b.reset(); b.write(0x68,1); b.write(0x60,1ULL|(1ULL<<30)|(1ULL<<25)|(1ULL<<26)); check(b.status()==(path_research ? 1ULL : (1ULL<<32)),"path bit follows PathEnable");
@@ -462,1 +472,33 @@
         cases+=6;
+
+        // V9 trimmed research: the path observer is not instantiated, so its index bank is
+        // unreachable while the P4M bank and the legacy counter bank stay usable.
+        if (mode == "research") {
+            b.reset();
+            check(b.read(0x58)==0x49444d414f425309ULL,"V9 trimmed research magic");
+            check(b.bank(0)==0x49444d4150344d01ULL,"V9 keeps the P4M bank");
+            b.write(0x70,64);
+            check(((b.status()>>32)&1)!=0,"V9 denies the first path index");
+            b.write(0x70,191);
+            check(((b.status()>>32)&1)!=0,"V9 denies the last path index");
+            check(b.count(0)==0,"V9 keeps the legacy counter bank readable");
+            cases++;
+        }
+
+        // V8 legacy research keeps the path observer: the path index bank is reachable and a
+        // path-armed P4M window still completes and validates.
+        if (path_research) {
+            b.reset();
+            check(b.read(0x58)==0x49444d414f425308ULL,"V8 legacy research magic");
+            b.write(0x70,64);
+            check(((b.status()>>32)&1)==0,"V8 exposes the path index bank");
+            check(b.read(0x78)==0,"V8 path bank reads zero before ARM");
+            b.reset();
+            b.arm_p4m(0,-1,1,true);
+            b.issue(0,1);
+            b.tick(3);
+            b.done(0,1);
+            b.freeze(0,1);
+            check(b.bank(13)==1&&b.bank(14)==1,"V8 path-armed P4M window completes");
+            check(b.bank(12)==0,"V8 path-armed P4M window is clean");
+            cases++;
+        }
     }
```

Note: `freeze()` already asserts `status()==4`, which includes `cfg_rdata_o[38]` (path error, `dma_observer.sv:153`), so the path-armed run implicitly proves `path_error==0`; no separate assertion needed.

## 3. `tests/idma_observation_harness.c` (new, complete)

```c
/*
 * Host harness for the production idma_observation_prepare()/idma_observation_arm()
 * helpers.  tests/test_idma_observation.py splices the production #defines, structs and
 * the two functions in at the markers below.  MMIO, the observer status handshake, the
 * instance lookup and the path_observe module parameter are mocked; the decisions under
 * test are the production ones.  Software control flow only: no FPGA or timing claim.
 */
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define __iomem
#define BIT_ULL(nr) (1ULL << (nr))
#define READ_ONCE(x) (*(const volatile __typeof__(x) *)&(x))

/* Kernel types referenced by the extracted struct idma_device. */
struct device;
struct miscdevice { struct device *parent; };

/* PRODUCTION_TYPES */

/* V9 is the trimmed-research composition magic; the literal is pinned here so the harness
 * builds before and after the driver adds its own define. */
#ifndef IDMA_OBS_MAGIC_V9
#define IDMA_OBS_MAGIC_V9 0x49444d414f425309ULL
#endif

/* ---- mocked register file ------------------------------------------------ */
#define REG_BYTES 0x1000
static u8 regs[REG_BYTES];
#define REG_BASE ((void __iomem *)regs)
#define MMIO(off) (*(volatile u64 *)(void *)(regs + (off)))

static unsigned mmio_writes;
static unsigned mmio_write_off[8];
static u64 mmio_write_val[8];

/* Test hook: a register that refuses the CLEAR handshake. */
static bool mock_clear_sticky;

static u64 readq(const void __iomem *addr)
{
    return *(const volatile u64 *)(const void *)addr;
}

static void writeq(u64 value, void __iomem *addr)
{
    unsigned off = (unsigned)((const u8 *)(const void *)addr - regs);

    if (mmio_writes < sizeof(mmio_write_off) / sizeof(mmio_write_off[0])) {
        mmio_write_off[mmio_writes] = off;
        mmio_write_val[mmio_writes] = value;
    }
    mmio_writes++;
    /* Minimal status handshake: CLEAR reads back idle, an ARM command reads back ARMED,
     * ABORT reads back FROZEN. */
    if (off == IDMA_OBS_CONTROL_OFFSET) {
        if (value == 4)
            value = mock_clear_sticky ? 5 : 0;
        else if (value == 2)
            value = 4;
        else if (value & 1)
            value = 1;
    }
    *(volatile u64 *)(void *)addr = value;
}

/* ---- mocked driver state ------------------------------------------------- */
static bool idma_path_observe = true;   /* production module-parameter default */
static u64 idma_observation_sequence;

static struct idma_device mock_control;
static bool mock_present = true;

static struct idma_device *idma_get_instance(u32 id)
{
    return (id == 0 && mock_present) ? &mock_control : NULL;
}

/* PRODUCTION_FUNCTIONS */

/* ---- test scaffolding ---------------------------------------------------- */
static void check(bool condition, const char *message)
{
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        exit(1);
    }
}

static void setup(u64 magic, bool present)
{
    memset(regs, 0, sizeof(regs));
    memset(&mock_control, 0, sizeof(mock_control));
    mock_control.id = 0;
    mock_control.did = 10;
    mock_control.reg = REG_BASE;
    mmio_writes = 0;
    mock_clear_sticky = false;
    mock_present = present;
    idma_path_observe = true;
    idma_observation_sequence = 0;
    MMIO(IDMA_OBS_MAGIC_OFFSET) = magic;
    MMIO(IDMA_OBS_CONTROL_OFFSET) = 0;
}

static void case_v9_prepare(void)
{
    struct idma_observation o = { .batch = true };

    setup(IDMA_OBS_MAGIC_V9, true);
    check(idma_observation_prepare(&o, true) == 0, "V9 prepare accepted");
    check(o.sequence == 1, "V9 prepare takes the first sequence");
    check(o.control == &mock_control, "V9 prepare binds the control instance");
    check(o.path == false, "V9 forces path off although path_observe is true");
    check(o.armed == false, "prepare alone does not arm");
    check(mmio_writes == 1, "V9 prepare performs exactly one MMIO write");
    check(mmio_write_off[0] == IDMA_OBS_CONTROL_OFFSET && mmio_write_val[0] == 4,
          "V9 prepare writes only the CLEAR command");
}

static void case_v9_arm(void)
{
    struct idma_observation o = { .batch = true, .p4m = true };
    struct idma_device victim;
    u64 command;

    memset(&victim, 0, sizeof(victim));
    victim.id = 0;
    victim.did = 10;

    setup(IDMA_OBS_MAGIC_V9, true);
    check(idma_observation_prepare(&o, true) == 0, "V9/P4M prepare accepted");
    check(o.path == false, "V9/P4M prepare keeps path off");

    mmio_writes = 0;
    check(idma_observation_arm(&o, &victim, NULL, 1) == 0, "V9/P4M ARM accepted");
    check(mmio_writes == 2, "V9/P4M ARM performs target and command writes");
    check(mmio_write_off[0] == IDMA_OBS_TARGET_OFFSET && mmio_write_val[0] == 1,
          "V9/P4M ARM writes the transfer count");
    check(mmio_write_off[1] == IDMA_OBS_CONTROL_OFFSET,
          "V9/P4M ARM writes the command word");
    command = mmio_write_val[1];
    check(command == (1ULL | BIT_ULL(25) | BIT_ULL(30)),
          "V9/P4M ARM command is exactly bit0+bit25+bit30");
    check((command & BIT_ULL(26)) == 0, "V9/P4M ARM leaves the path bit clear");
    check(o.armed == true, "V9/P4M ARM reports armed");
    check(MMIO(IDMA_OBS_CONTROL_OFFSET) == 1, "V9/P4M ARM observes the ARMED status");
}

static void case_v8_path(void)
{
    struct idma_observation o = { .batch = true, .p4m = true };
    struct idma_device victim;
    u64 command;

    memset(&victim, 0, sizeof(victim));
    victim.id = 0;
    victim.did = 10;

    setup(IDMA_OBS_MAGIC_V8, true);
    check(idma_observation_prepare(&o, true) == 0, "V8 prepare accepted");
    check(o.path == true, "V8 keeps the path observer");

    mmio_writes = 0;
    check(idma_observation_arm(&o, &victim, NULL, 1) == 0, "V8 ARM accepted");
    check(mmio_writes == 2, "V8 ARM performs target and command writes");
    command = mmio_write_val[1];
    check((command & BIT_ULL(26)) != 0, "V8 ARM keeps the path bit");
    check((command & BIT_ULL(30)) != 0, "V8 ARM keeps the P4M bit");
}

static void case_v9_denials(void)
{
    struct idma_observation o;

    setup(IDMA_OBS_MAGIC_V9, true);
    o = (struct idma_observation){ .batch = true, .e3 = true };
    check(idma_observation_prepare(&o, true) == -EOPNOTSUPP, "V9 rejects E3");
    check(mmio_writes == 0, "V9 E3 denial precedes every MMIO write");

    setup(IDMA_OBS_MAGIC_V9, true);
    o = (struct idma_observation){ .batch = true, .controlled = true };
    check(idma_observation_prepare(&o, true) == -EOPNOTSUPP, "V9 rejects controlled");
    check(mmio_writes == 0, "V9 controlled denial precedes every MMIO write");

    setup(IDMA_OBS_MAGIC_V9, true);
    o = (struct idma_observation){ .batch = true, .queue_pair = true };
    check(idma_observation_prepare(&o, true) == -EOPNOTSUPP, "V9 rejects queue pair");
    check(mmio_writes == 0, "V9 queue-pair denial precedes every MMIO write");
}

static void case_unknown_magic(void)
{
    struct idma_observation o;

    setup(0x1111111111111111ULL, true);
    o = (struct idma_observation){ .batch = true };
    check(idma_observation_prepare(&o, true) == -EOPNOTSUPP,
          "unknown magic with batch is denied");
    check(mmio_writes == 0, "unknown-magic denial precedes every MMIO write");

    setup(0x1111111111111111ULL, true);
    o = (struct idma_observation){ 0 };
    check(idma_observation_prepare(&o, true) == -EOPNOTSUPP,
          "unknown magic without batch is denied");
    check(mmio_writes == 0, "unknown-magic denial precedes every MMIO write");
}

static void case_busy_and_clear(void)
{
    struct idma_observation o;

    setup(IDMA_OBS_MAGIC_V9, true);
    MMIO(IDMA_OBS_CONTROL_OFFSET) = 1;
    o = (struct idma_observation){ .batch = true };
    check(idma_observation_prepare(&o, true) == -EBUSY, "an armed session is refused");
    check(mmio_writes == 0, "armed-session refusal precedes every MMIO write");

    setup(IDMA_OBS_MAGIC_V9, true);
    MMIO(IDMA_OBS_CONTROL_OFFSET) = 2;
    o = (struct idma_observation){ .batch = true };
    check(idma_observation_prepare(&o, true) == -EBUSY, "a frozen session is refused");
    check(mmio_writes == 0, "frozen-session refusal precedes every MMIO write");

    setup(IDMA_OBS_MAGIC_V9, true);
    mock_clear_sticky = true;
    o = (struct idma_observation){ .batch = true };
    check(idma_observation_prepare(&o, true) == -EIO, "a failed CLEAR readback is reported");
    check(mmio_writes == 1 && mmio_write_off[0] == IDMA_OBS_CONTROL_OFFSET &&
          mmio_write_val[0] == 4, "the failed CLEAR still writes exactly once");
}

static void case_no_instance(void)
{
    struct idma_observation o = { .batch = true };

    setup(IDMA_OBS_MAGIC_V9, false);
    check(idma_observation_prepare(&o, true) == -ENODEV, "a missing observer is reported");
    check(mmio_writes == 0, "missing-observer refusal precedes every MMIO write");
}

int main(int argc, char **argv)
{
    static const struct { const char *name; void (*run)(void); } cases[] = {
        { "v9_prepare",    case_v9_prepare },
        { "v9_arm",        case_v9_arm },
        { "v8_path",       case_v8_path },
        { "v9_denials",    case_v9_denials },
        { "unknown_magic", case_unknown_magic },
        { "busy_and_clear", case_busy_and_clear },
        { "no_instance",   case_no_instance },
    };
    unsigned i;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <case>\n", argv[0]);
        return 2;
    }
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        if (!strcmp(cases[i].name, argv[1])) {
            cases[i].run();
            printf("PASS %s\n", argv[1]);
            return 0;
        }
    }
    fprintf(stderr, "unknown case: %s\n", argv[1]);
    return 2;
}
```

## 4. `tests/test_idma_observation.py` (new, complete)

```python
#!/usr/bin/env python3
"""Compile and run the production idma observation prepare/arm helpers on a host model.

This establishes software control flow only, not FPGA timing or hardware acceptance.
Only the production idma_observation_prepare/idma_observation_arm helpers and their
immediate dependencies are compiled; kernel MMIO, the instance lookup and the
path_observe module parameter are stubbed.
"""
import os
from pathlib import Path
import re
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
STRUCTURES = ('idma_device', 'idma_observation')
FUNCTIONS = ('idma_observation_prepare', 'idma_observation_arm')
CASES = (
    'v9_prepare',
    'v9_arm',
    'v8_path',
    'v9_denials',
    'unknown_magic',
    'busy_and_clear',
    'no_instance',
)


def extract(source, pattern, label):
    matches = re.findall(pattern, source, re.MULTILINE | re.DOTALL)
    if len(matches) != 1:
        raise RuntimeError(f'Expected exactly one production {label}, got {len(matches)}')
    return matches[0]


class IdmaObservationTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        build = Path(cls.temp.name)
        source = Path(os.environ.get('IDMA_SOURCE',
                      ROOT / 'linux/drivers/misc/idma.c')).read_text()
        definitions = '\n'.join(re.findall(r'^#define IDMA_.*$', source, re.MULTILINE))
        structures = '\n'.join(
            extract(source, rf'^struct {name} \{{.*?^\}};', name)
            for name in STRUCTURES
        )
        functions = '\n'.join(
            extract(source, rf'^static [^\n]*\b{name}\(.*?^\}}', name)
            for name in FUNCTIONS
        )
        harness = (ROOT / 'tests/idma_observation_harness.c').read_text()
        harness = harness.replace('/* PRODUCTION_TYPES */', definitions + '\n' + structures)
        harness = harness.replace('/* PRODUCTION_FUNCTIONS */', functions)
        (build / 'observation.c').write_text(harness)
        cls.binary = build / 'observation'
        subprocess.run([
            os.environ.get('HOSTCC', 'cc'), '-std=gnu11', '-O0', '-g', '-Wall',
            '-Wextra', '-Werror', str(build / 'observation.c'), '-o', str(cls.binary),
        ], check=True)

    def test_production_observation(self):
        for case in CASES:
            with self.subTest(case=case):
                result = subprocess.run([str(self.binary), case], capture_output=True,
                                        text=True, timeout=5)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertIn('PASS ' + case, result.stdout)


if __name__ == '__main__':
    unittest.main()
```

## Acceptance checks

1. `python3 tests/test_dma_p4m_observer.py` — 4 variants. Against the **pre-change** `dma_observer.sv` the `research` variant must fail on "P4M version magic" (V9 vs V8) and on the V9 path-index denials; `rtl`, `rtl-small`, `research-path` must pass unchanged behavior (`research-path` should pass even before root's edit, since V8 is the untouched priority).
2. `python3 tests/test_idma_observation.py` — all 7 cases. Before root's Linux edit: `v9_prepare`, `v9_arm`, `v9_denials`, `unknown_magic` fail; `v8_path`, `busy_and_clear`, `no_instance` pass. After: all pass.
3. `case_v9_arm` is the tripwire for assumption (2): if the p4m guard is not extended to accept V9, it fails on "V9/P4M prepare accepted" — report to Codex rather than relax the test.
4. Boundary checks worth a second look from Codex: V9 magic literal (`0x49444d414f425309`) in both `dma_observer.sv` and `IDMA_OBS_MAGIC_V9`; and that `research-path` really has `E3Enable=0/ControlledEnable=0` (otherwise the V9 condition could re-trigger or the arm-pulse block expectations shift).

## Uncertainty / missing files

* I could not see `tests/idma_batch_start_harness.c`, so the mock layout (register file, `readq`/`writeq`, `idma_get_instance` stub, `check()` helper) is my reconstruction of "the existing extraction-test style" from `tests/test_idma_batch_start.py`; align names/macros if the real harness convention differs.
* `tests/dma_path_observer`-specific C++ test files were not supplied; I did not touch them. `tests/test_dma_observer_finite.py` already compiles `dma_path_observer.sv` with `PathEnable=1`, so the file/parameter combination is known to compile.
* No test was executed; nothing here is claimed as passing.
* `cases` counters in the C++ file are scenario counters, not assertion counts; the `cases+=8` block at old line 266 now counts 8 scenarios of which one (path bit) expects a legal ARM under `research-path`.

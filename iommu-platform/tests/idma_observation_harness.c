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
_Alignas(u64) static u8 regs[REG_BYTES];
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
    check(idma_observation_prepare(&o, true) == -EBUSY, "a busy session is refused");
    check(mmio_writes == 0, "busy-session refusal precedes every MMIO write");

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

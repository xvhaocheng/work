/* Actual pn_snapshot/pn_json, fake MMIO only. Never opens /dev/mem. */
#define PN_CONTROL_HOST_TEST
#include "pn_control.c"
#include <assert.h>
#include <stdlib.h>

struct fake_mmio {
    uint64_t raw[PN_WORDS], gap, gap_cap, before, after, index;
    unsigned int calls, loads, stores, controls, fail_call, mismatch_index;
    int fail_error;
};

static void fake_init(struct fake_mmio *f)
{
    memset(f, 0, sizeof(*f));
    f->raw[0] = PN_CAP;
    /* Unlocked words194/199 intentionally contain arbitrary bits. */
    f->raw[2] = UINT64_C(0xffffffffffffffff);
    f->raw[3] = 37;
    f->raw[4] = 91;
    f->raw[5] = 4097;
    f->raw[6] = 8199;
    f->raw[7] = UINT64_C(0x8000000000001234);
    f->gap_cap = GAP_CAP;
    f->before = 4; /* FROZEN is allowed; armed/busy are not. */
    f->after = 4;
    f->fail_error = EACCES;
}

static int fake_load(void *context, unsigned int offset, uint64_t *value)
{
    struct fake_mmio *f = context;
    f->calls++;
    f->loads++;
    if (f->calls == f->fail_call)
        return f->fail_error;
    switch (offset) {
    case OBS_CONTROL:
        assert(f->controls < 2);
        assert(f->calls == (f->controls ? 28U : 1U));
        *value = f->controls++ ? f->after : f->before;
        break;
    case OBS_INDEX:
        assert(f->stores >= 1 && f->stores <= PN_WORDS);
        assert(f->calls == 3U + 3U * (f->stores - 1U));
        *value = f->index + (f->index == f->mismatch_index ? 1U : 0U);
        break;
    case OBS_DATA:
        assert(f->stores >= 1 && f->stores <= PN_WORDS);
        assert(f->calls == 4U + 3U * (f->stores - 1U));
        assert(f->index == PN_FIRST + f->stores - 1U);
        *value = f->raw[f->index - PN_FIRST];
        break;
    case V_GAP:
        assert(f->calls == 26);
        *value = f->gap;
        break;
    case V_GAP_CAP:
        assert(f->calls == 27);
        *value = f->gap_cap;
        break;
    default:
        assert(!"unexpected MMIO load");
    }
    return 0;
}

static int fake_select(void *context, unsigned int index)
{
    struct fake_mmio *f = context;
    f->calls++;
    f->stores++;
    assert(index >= PN_FIRST && index < PN_FIRST + PN_WORDS);
    assert(index == PN_FIRST + f->stores - 1U);
    assert(f->calls == 2U + 3U * (f->stores - 1U));
    if (f->calls == f->fail_call)
        return f->fail_error;
    f->index = index;
    return 0;
}

static void run(const char *name, struct fake_mmio *f, enum pn_operation operation,
                int expected_error, const char *expected_stage)
{
    struct pn_io io = { f, fake_load, fake_select };
    struct pn_result result;
    unsigned int i;

    assert(pn_snapshot(&io, operation, &result) == expected_error);
    assert(result.error == expected_error);
    assert(!strcmp(result.stage, expected_stage));
    assert(result.index_write_attempts == f->stores);
    for (i = 0; i < PN_WORDS; i++)
        if (result.raw_valid & (1U << i))
            assert(result.raw[i] == f->raw[i]);
    if (!expected_error) {
        assert(f->calls == 28 && f->loads == 20 && f->stores == 8);
        assert(result.raw_valid == 255 && result.scalar_valid == 15);
        assert(result.gap == f->gap && result.gap_cap == f->gap_cap);
    }
    if (!strcmp(expected_stage, "index_readback"))
        assert(!(result.raw_valid & (1U << (f->stores - 1U))));
    printf("{\"test_case\":\"%s\",\"result\":", name);
    pn_json(stdout, &result);
    puts("}");
}

int main(void)
{
    struct fake_mmio f;
    struct pn_result result;
    struct pn_io bad_io = {0};
    unsigned int i;
    char name[64];
    char *valid[] = { "pn-control", "read", NULL };
    char *caps[] = { "pn-control", "capabilities", NULL };
    char *write[] = { "pn-control", "mode", "1", NULL };
    char *gap[] = { "pn-control", "gap", "1000", NULL };
    char *unknown[] = { "pn-control", "write", NULL };
    char *extra[] = { "pn-control", "read", "0x50001000", NULL };

    assert(pn_arguments(2, valid) == PN_READ);
    assert(pn_arguments(2, caps) == PN_CAPABILITIES);
    assert(pn_arguments(1, valid) == PN_INVALID);
    assert(pn_arguments(3, write) == PN_INVALID);
    assert(pn_arguments(3, gap) == PN_INVALID);
    assert(pn_arguments(2, unknown) == PN_INVALID);
    assert(pn_arguments(3, extra) == PN_INVALID);
    fake_init(&f);
    run("invalid-operation-no-io", &f, PN_INVALID, EINVAL, "arguments");
    assert(!f.calls);
    assert(pn_snapshot(NULL, PN_READ, &result) == EINVAL);
    assert(!strcmp(result.stage, "interface"));
    assert(pn_snapshot(&bad_io, PN_READ, &result) == EINVAL);
    puts("{\"test_case\":\"arguments-and-interface\",\"fixture_only\":true,"
         "\"source_board_cases\":0,\"checks\":10}");

    for (i = 0; i < 4; i++) {
        fake_init(&f);
        f.raw[1] = i;
        snprintf(name, sizeof(name), "valid-mode-%u", i);
        run(name, &f, PN_READ, 0, "complete");
    }
    fake_init(&f);
    run("capabilities-full-snapshot", &f, PN_CAPABILITIES, 0, "complete");
    for (i = 0; i < 3; i++) {
        const uint64_t values[] = {1000, 4000, UINT32_MAX};
        fake_init(&f);
        f.gap = values[i];
        snprintf(name, sizeof(name), "valid-gap-%" PRIu64, f.gap);
        run(name, &f, PN_READ, 0, "complete");
    }
    fake_init(&f); f.raw[0] ^= 1;
    run("pn-cap-mismatch-no-fallback", &f, PN_READ, ENODEV, "pn_capability");
    fake_init(&f); f.gap_cap ^= 1;
    run("gap-cap-mismatch-no-fallback", &f, PN_READ, ENODEV, "gap_capability");
    fake_init(&f); f.raw[1] = 4;
    run("invalid-mode", &f, PN_READ, EPROTO, "mode");
    fake_init(&f); f.gap = UINT64_C(1) << 32;
    run("invalid-gap-high-bits", &f, PN_READ, EPROTO, "gap_high_bits");
    for (i = 1; i <= 3; i++) {
        fake_init(&f); f.before = i;
        snprintf(name, sizeof(name), "armed-busy-before-%u", i);
        run(name, &f, PN_READ, EBUSY, "observer_before");
        assert(f.calls == 1 && !f.stores);
        fake_init(&f); f.after = i;
        snprintf(name, sizeof(name), "armed-busy-after-%u", i);
        run(name, &f, PN_READ, EBUSY, "observer_after");
    }
    for (i = PN_FIRST; i < PN_FIRST + PN_WORDS; i++) {
        fake_init(&f); f.mismatch_index = i;
        snprintf(name, sizeof(name), "index-mismatch-%u", i);
        run(name, &f, PN_READ, EIO, "index_readback");
        assert(f.calls == 3U + 3U * (i - PN_FIRST));
    }
    for (i = 1; i <= 28; i++) {
        const char *stage = i == 1 ? "observer_before" :
            i == 26 ? "gap_read" : i == 27 ? "gap_cap_read" :
            i == 28 ? "observer_after" :
            (i - 2) % 3 == 0 ? "index_write" :
            (i - 2) % 3 == 1 ? "index_readback" : "bank_read";
        fake_init(&f); f.fail_call = i;
        snprintf(name, sizeof(name), "access-error-at-%u", i);
        run(name, &f, PN_READ, EACCES, stage);
        assert(f.calls == i); /* No access after first failure. */
    }
    fake_init(&f); f.fail_call = 4; f.fail_error = EIO;
    run("mmio-fault-reported-eio", &f, PN_READ, EIO, "bank_read");
    /* Cleanup errors retain the first error and already read values. */
    assert(pn_snapshot(&bad_io, PN_READ, &result) == EINVAL);
    assert(pn_fail(&result, EIO, "cleanup") == EINVAL);
    assert(!strcmp(result.stage, "interface"));
    memset(&result, 0, sizeof(result));
    assert(pn_fail(&result, EACCES, "open_devmem") == EACCES);
    assert(pn_fail(&result, EIO, "mmap64") == EACCES);
    assert(!strcmp(result.stage, "open_devmem"));
    puts("{\"test_case\":\"first-error-preserved\",\"fixture_only\":true,"
         "\"source_board_cases\":0,\"checks\":3}");
    return 0;
}

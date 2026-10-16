#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/idma.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define IDMA_RESEARCH_MAGIC 0x49444d4152455331ULL
#define RESEARCH_OBSERVER_MAGIC 0x49444d414f425308ULL
#define RESEARCH_COST_MAGIC 0x49444d4152454331ULL
#define P4M_WINDOW_MAGIC 0x49444d4150344d01ULL
#define IDMA_FULLSTATS_MAGIC 0x49444d4141474731ULL
#define IDMA_FULLSTATS_VERSION 1
#define IDMA_FULLSTATS_GROUPS 6
#define IDMA_FULLSTATS_STRIDE 16

#define IDMA_RESEARCH2_MIN_WINDOW   1024U
#define IDMA_RESEARCH2_MAX_WINDOW   1048576U
#define IDMA_RESEARCH2_MAX_TARGET   16777216U
#define IDMA_RESEARCH2_MAX_BUDGET   16777216U
#define IDMA_RESEARCH2_MAX_CAP      67108864U
#define IDMA_RESEARCH2_MODE6_MAX    200000U
#define IDMA_RESEARCH2_DEF_WINDOW   16384U
#define IDMA_RESEARCH2_DEF_TARGET   51540U
#define IDMA_RESEARCH2_DEF_DEADBAND 4096U
#define IDMA_RESEARCH2_DEF_INITIAL  8000U
#define IDMA_RESEARCH2_DEF_MIN      1U
#define IDMA_RESEARCH2_DEF_MAX      32768U
#define IDMA_RESEARCH2_DEF_MODE6_MIN 0U
#define IDMA_RESEARCH2_DEF_MODE6_MAX 200000U
#define IDMA_RESEARCH2_DEF_STEP     512U
#define IDMA_RESEARCH2_DEF_CAP      65536U
#define IDMA_RESEARCH2_DEF_MODE6_CAP 200000U
#define IDMA_RESEARCH2_DEF_HIT      1U
#define IDMA_RESEARCH2_DEF_MISS     100U
#define IDMA_RESEARCH2_DEF_PTW      100U

struct options {
	const char *device;
	int mode;
	int help;
	int v2;
	int v2_role;
	int caps2;
	uint32_t a_pages, a_transfers, window_cycles, warmup, transfers, gap_cycles, timeout_ms;
	int a_pages_set, a_transfers_set, window_cycles_set, warmup_set, transfers_set, gap_cycles_set, timeout_ms_set;
	int require_fullstats;
	uint32_t reverse, policy_mode, policy_window, target_bytes, deadband_bytes;
	uint32_t initial_budget, min_budget, max_budget, budget_step, credit_cap;
	uint32_t hit_weight, miss_weight, ptw_weight;
	int reverse_set, policy_mode_set, policy_window_set, target_bytes_set, deadband_bytes_set;
	int initial_budget_set, min_budget_set, max_budget_set, budget_step_set, credit_cap_set;
	int hit_weight_set, miss_weight_set, ptw_weight_set;
};

static void usage(void)
{
	puts("Usage: idma_research --e2|--cost|--e2-ext|--method|--caps2 [--device PATH] [options]\n"
	     "  --e2                    Passive E2 entry-lifecycle research run\n"
	     "  --cost                  Natural-cost research run\n"
	     "  --e2-ext                Research2 role 1 extended E2 run (ioctl 0x0f)\n"
	     "  --method                Research2 role 2 policy-method run (ioctl 0x0f)\n"
	     "  --caps2                 Standalone read-only research2 capability query (0x10)\n"
	     "  --device PATH           Device node (default /dev/idma0)\n"
	     "  --a-pages N             Aggressor pages: E2 0/1/4/7/8/16; cost/method 0/1/8/16\n"
	     "  --a-transfers N         Aggressor transfers (frozen by mode)\n"
	     "  --window-cycles N       E2 window cycles (frozen 5000000; cost/method 0)\n"
	     "  --warmup N              Warmup transfers (frozen by mode)\n"
	     "  --transfers N           Victim transfers (frozen by mode)\n"
	     "  --a-gap-cycles N        Gap (cost 0/1000/2000; method 0/500/1000/2000/4000)\n"
	     "  --timeout-ms N          Run timeout (frozen by mode)\n"
	     "  --require-fullstats     Require full-aggregation raw fields (cost/method)\n"
	     "  --reverse N             A access order 0 forward / 1 reverse (research2)\n"
	     "  --policy-mode N         Method policy 0..7 (research2 role 2)\n"
	     "  --policy-window N       Policy feedback window cycles (research2 role 2)\n"
	     "  --target-bytes N        Policy target bytes (research2 role 2)\n"
	     "  --deadband-bytes N      Policy deadband bytes (research2 role 2)\n"
	     "  --initial-budget N      Policy initial budget (research2 role 2)\n"
	     "  --min-budget N          Policy minimum budget (research2 role 2)\n"
	     "  --max-budget N          Policy maximum budget (research2 role 2)\n"
	     "  --budget-step N         Policy budget step (research2 role 2)\n"
	     "  --credit-cap N          Policy credit cap (research2 role 2)\n"
	     "  --hit-weight N          Policy hit weight (research2 role 2)\n"
	     "  --miss-weight N         Policy miss weight (research2 role 2)\n"
	     "  --ptw-weight N          Policy PTW weight (research2 role 2)\n"
	     "  --help                  Show this help (text, not JSON)\n"
	     "All other arguments/values are rejected. One JSON record per run.");
}

static int parse_u32(const char *s, uint32_t *value)
{
	const unsigned char *p = (const unsigned char *)s;
	uint64_t n = 0;

	if (!*p)
		return -1;
	for (; *p; p++) {
		unsigned int digit;

		if (*p < '0' || *p > '9')
			return -1;
		digit = *p - '0';
		if (n > (UINT32_MAX - digit) / 10)
			return -1;
		n = n * 10 + digit;
	}
	*value = (uint32_t)n;
	return 0;
}

static int parse_args(int argc, char **argv, struct options *o, char *err, size_t errlen)
{
	static const char *const names[] = {
		"--device", "--a-pages", "--a-transfers", "--window-cycles",
		"--warmup", "--transfers", "--a-gap-cycles", "--timeout-ms",
		"--e2", "--cost", "--help", "--require-fullstats",
		"--e2-ext", "--method", "--caps2", "--reverse",
		"--policy-mode", "--policy-window", "--target-bytes", "--deadband-bytes",
		"--initial-budget", "--min-budget", "--max-budget", "--budget-step",
		"--credit-cap", "--hit-weight", "--miss-weight", "--ptw-weight",
	};
	unsigned int seen[sizeof(names) / sizeof(names[0])] = {0};
	int i;

	for (i = 1; i < argc; i++) {
		unsigned int opt;

		for (opt = 0; opt < sizeof(names) / sizeof(names[0]); opt++)
			if (!strcmp(argv[i], names[opt]))
				break;
		if (opt == sizeof(names) / sizeof(names[0])) {
			snprintf(err, errlen, "unknown argument: %s", argv[i]);
			return -1;
		}
		if (seen[opt]) {
			snprintf(err, errlen, "duplicate option: %s", names[opt]);
			return -1;
		}
		seen[opt] = 1;
		if (opt == 8 || opt == 9 || opt == 12 || opt == 13) {
			static const unsigned int mode_opt[4] = { 8, 9, 12, 13 };
			unsigned int k;

			for (k = 0; k < 4; k++)
				if (mode_opt[k] != opt && seen[mode_opt[k]]) {
					snprintf(err, errlen, "specify exactly one of --e2, --cost, --e2-ext or --method");
					return -1;
				}
			if (opt == 8) {
				o->mode = IDMA_RESEARCH_E2;
			} else if (opt == 9) {
				o->mode = IDMA_RESEARCH_COST;
			} else if (opt == 12) {
				o->mode = IDMA_RESEARCH_E2;
				o->v2 = 1;
				o->v2_role = IDMA_RESEARCH2_E2_ROLE;
			} else {
				o->mode = IDMA_RESEARCH_COST;
				o->v2 = 1;
				o->v2_role = IDMA_RESEARCH2_METHOD_ROLE;
			}
			continue;
		}
		if (opt == 10) {
			o->help = 1;
			continue;
		}
		if (opt == 11) {
			o->require_fullstats = 1;
			continue;
		}
		if (opt == 14) {
			o->caps2 = 1;
			continue;
		}
		if (++i == argc) {
			snprintf(err, errlen, "missing value for %s", names[opt]);
			return -1;
		}
		if (opt == 0) {
			if (!*argv[i]) {
				snprintf(err, errlen, "device path is empty");
				return -1;
			}
			o->device = argv[i];
			continue;
		}
		{
			uint32_t num;

			if (parse_u32(argv[i], &num)) {
				snprintf(err, errlen, "invalid numeric value for %s: %s", names[opt], argv[i]);
				return -1;
			}
			switch (opt) {
			case 1: o->a_pages = num; o->a_pages_set = 1; break;
			case 2: o->a_transfers = num; o->a_transfers_set = 1; break;
			case 3: o->window_cycles = num; o->window_cycles_set = 1; break;
			case 4: o->warmup = num; o->warmup_set = 1; break;
			case 5: o->transfers = num; o->transfers_set = 1; break;
			case 6: o->gap_cycles = num; o->gap_cycles_set = 1; break;
			case 7: o->timeout_ms = num; o->timeout_ms_set = 1; break;
			case 15: o->reverse = num; o->reverse_set = 1; break;
			case 16: o->policy_mode = num; o->policy_mode_set = 1; break;
			case 17: o->policy_window = num; o->policy_window_set = 1; break;
			case 18: o->target_bytes = num; o->target_bytes_set = 1; break;
			case 19: o->deadband_bytes = num; o->deadband_bytes_set = 1; break;
			case 20: o->initial_budget = num; o->initial_budget_set = 1; break;
			case 21: o->min_budget = num; o->min_budget_set = 1; break;
			case 22: o->max_budget = num; o->max_budget_set = 1; break;
			case 23: o->budget_step = num; o->budget_step_set = 1; break;
			case 24: o->credit_cap = num; o->credit_cap_set = 1; break;
			case 25: o->hit_weight = num; o->hit_weight_set = 1; break;
			case 26: o->miss_weight = num; o->miss_weight_set = 1; break;
			case 27: o->ptw_weight = num; o->ptw_weight_set = 1; break;
			}
		}
	}
	return 0;
}

static void fill_request2(struct idma_research2 *r2, const struct options *o,
			  const uint64_t *raw)
{
	memset(r2, 0, sizeof(*r2));
	r2->base.abi_version = IDMA_RESEARCH_ABI;
	r2->base.struct_size = sizeof(struct idma_research);
	r2->base.mode = o->mode;
	r2->base.a_pages = o->a_pages;
	r2->base.a_transfers = o->a_transfers;
	r2->base.window_cycles = o->window_cycles;
	r2->base.warmup = o->warmup;
	r2->base.transfers = o->transfers;
	r2->base.gap_cycles = o->gap_cycles;
	r2->base.timeout_ms = o->timeout_ms;
	r2->base.raw_words = IDMA_RESEARCH_RAW_WORDS;
	r2->base.raw_user = (uint64_t)(uintptr_t)raw;
	r2->config.abi_version = IDMA_RESEARCH2_ABI;
	r2->config.struct_size = sizeof(struct idma_research2_config);
	r2->config.required_role = o->v2_role;
	r2->config.reverse = o->reverse;
	r2->config.policy_mode = o->policy_mode;
	r2->config.window_cycles = o->policy_window;
	r2->config.target_bytes = o->target_bytes;
	r2->config.deadband_bytes = o->deadband_bytes;
	r2->config.initial_budget = o->initial_budget;
	r2->config.min_budget = o->min_budget;
	r2->config.max_budget = o->max_budget;
	r2->config.budget_step = o->budget_step;
	r2->config.credit_cap = o->credit_cap;
	r2->config.hit_weight = o->hit_weight;
	r2->config.miss_weight = o->miss_weight;
	r2->config.ptw_weight = o->ptw_weight;
}

static int validate_caps2_options(const struct options *o, char *err, size_t errlen)
{
	if (o->mode || o->require_fullstats ||
	    o->a_pages_set || o->a_transfers_set || o->window_cycles_set ||
	    o->warmup_set || o->transfers_set || o->gap_cycles_set ||
	    o->timeout_ms_set || o->reverse_set || o->policy_mode_set ||
	    o->policy_window_set || o->target_bytes_set || o->deadband_bytes_set ||
	    o->initial_budget_set || o->min_budget_set || o->max_budget_set ||
	    o->budget_step_set || o->credit_cap_set || o->hit_weight_set ||
	    o->miss_weight_set || o->ptw_weight_set) {
		snprintf(err, errlen, "--caps2 is a standalone query and cannot be combined with run options");
		return -1;
	}
	return 0;
}

static int validate_e2_ext_options(struct options *o, char *err, size_t errlen)
{
	if (!o->a_pages_set) o->a_pages = 8;
	if (!o->a_transfers_set) o->a_transfers = o->a_pages ? 112 : 0;
	if (!o->window_cycles_set) o->window_cycles = 5000000;
	if (!o->warmup_set) o->warmup = 2;
	if (!o->transfers_set) o->transfers = 1;
	if (!o->gap_cycles_set) o->gap_cycles = 0;
	if (!o->timeout_ms_set) o->timeout_ms = 2000;
	if (o->a_pages != 0 && o->a_pages != 1 && o->a_pages != 4 &&
	    o->a_pages != 7 && o->a_pages != 8 && o->a_pages != 16) {
		snprintf(err, errlen, "invalid E2 extension --a-pages");
		return -1;
	}
	if (o->a_pages == 0) {
		if (o->a_transfers != 0) {
			snprintf(err, errlen, "invalid E2 extension --a-transfers for W0");
			return -1;
		}
	} else if (o->a_transfers != 1 && o->a_transfers != 8 &&
		   o->a_transfers != 16 && o->a_transfers != 112) {
		snprintf(err, errlen, "invalid E2 extension --a-transfers");
		return -1;
	}
	if (o->window_cycles != 5000000) {
		snprintf(err, errlen, "invalid E2 extension --window-cycles");
		return -1;
	}
	if (o->warmup != 2) {
		snprintf(err, errlen, "invalid E2 extension --warmup");
		return -1;
	}
	if (o->transfers != 1) {
		snprintf(err, errlen, "invalid E2 extension --transfers");
		return -1;
	}
	if (o->gap_cycles != 0) {
		snprintf(err, errlen, "invalid E2 extension --a-gap-cycles");
		return -1;
	}
	if (o->timeout_ms != 2000) {
		snprintf(err, errlen, "invalid E2 extension --timeout-ms");
		return -1;
	}
	if (!o->reverse_set) o->reverse = 0;
	if (o->reverse > 1) {
		snprintf(err, errlen, "invalid --reverse (0 or 1)");
		return -1;
	}
	if (o->a_pages == 0 && o->reverse) {
		snprintf(err, errlen, "--reverse requires aggressor pages");
		return -1;
	}
	if (o->policy_mode_set || o->policy_window_set || o->target_bytes_set ||
	    o->deadband_bytes_set || o->initial_budget_set || o->min_budget_set ||
	    o->max_budget_set || o->budget_step_set || o->credit_cap_set ||
	    o->hit_weight_set || o->miss_weight_set || o->ptw_weight_set) {
		snprintf(err, errlen, "--e2-ext does not accept policy configuration");
		return -1;
	}
	o->policy_mode = 0;
	o->policy_window = 0;
	o->target_bytes = 0;
	o->deadband_bytes = 0;
	o->initial_budget = 0;
	o->min_budget = 0;
	o->max_budget = 0;
	o->budget_step = 0;
	o->credit_cap = 0;
	o->hit_weight = 0;
	o->miss_weight = 0;
	o->ptw_weight = 0;
	return 0;
}

static int validate_method_options(struct options *o, char *err, size_t errlen)
{
	uint32_t def_min, def_max, def_cap;

	if (!o->policy_mode_set) o->policy_mode = 0;
	if (o->policy_mode > 7) {
		snprintf(err, errlen, "invalid --policy-mode (0..7)");
		return -1;
	}
	if (!o->a_pages_set) o->a_pages = 8;
	if (!o->a_transfers_set) o->a_transfers = 0;
	if (!o->window_cycles_set) o->window_cycles = 0;
	if (!o->gap_cycles_set) o->gap_cycles = 0;
	if (!o->transfers_set) o->transfers = 4096;
	if (!o->warmup_set) o->warmup = (o->transfers == 64) ? 8 : 256;
	if (!o->timeout_ms_set) o->timeout_ms = 10000;
	if (o->a_pages != 0 && o->a_pages != 1 && o->a_pages != 8 && o->a_pages != 16) {
		snprintf(err, errlen, "invalid method --a-pages");
		return -1;
	}
	if (o->a_transfers != 0) {
		snprintf(err, errlen, "invalid method --a-transfers");
		return -1;
	}
	if (o->window_cycles != 0) {
		snprintf(err, errlen, "invalid method --window-cycles");
		return -1;
	}
	if (!((o->transfers == 4096 && o->warmup == 256) ||
	      (o->transfers == 64 && o->warmup == 8))) {
		snprintf(err, errlen, "invalid method --transfers/--warmup pair");
		return -1;
	}
	if (o->timeout_ms != 10000) {
		snprintf(err, errlen, "invalid method --timeout-ms");
		return -1;
	}
	if (o->gap_cycles != 0 && o->gap_cycles != 500 && o->gap_cycles != 1000 &&
	    o->gap_cycles != 2000 && o->gap_cycles != 4000) {
		snprintf(err, errlen, "invalid method --a-gap-cycles");
		return -1;
	}
	if (o->gap_cycles && o->policy_mode != 0) {
		snprintf(err, errlen, "nonzero --a-gap-cycles requires --policy-mode 0");
		return -1;
	}
	if (o->gap_cycles && o->a_pages == 0) {
		snprintf(err, errlen, "nonzero --a-gap-cycles requires aggressor pages");
		return -1;
	}
	if (o->policy_mode != 0 && o->a_pages == 0) {
		snprintf(err, errlen, "nonzero --policy-mode requires aggressor pages");
		return -1;
	}
	if (!o->reverse_set) o->reverse = 0;
	if (o->reverse > 1) {
		snprintf(err, errlen, "invalid --reverse (0 or 1)");
		return -1;
	}
	if (o->a_pages == 0 && o->reverse) {
		snprintf(err, errlen, "--reverse requires aggressor pages");
		return -1;
	}
	if (o->policy_mode == 6) {
		def_min = IDMA_RESEARCH2_DEF_MODE6_MIN;
		def_max = IDMA_RESEARCH2_DEF_MODE6_MAX;
		def_cap = IDMA_RESEARCH2_DEF_MODE6_CAP;
	} else {
		def_min = IDMA_RESEARCH2_DEF_MIN;
		def_max = IDMA_RESEARCH2_DEF_MAX;
		def_cap = IDMA_RESEARCH2_DEF_CAP;
	}
	if (!o->policy_window_set) o->policy_window = IDMA_RESEARCH2_DEF_WINDOW;
	if (!o->target_bytes_set) o->target_bytes = IDMA_RESEARCH2_DEF_TARGET;
	if (!o->deadband_bytes_set) o->deadband_bytes = IDMA_RESEARCH2_DEF_DEADBAND;
	if (!o->initial_budget_set) o->initial_budget = IDMA_RESEARCH2_DEF_INITIAL;
	if (!o->min_budget_set) o->min_budget = def_min;
	if (!o->max_budget_set) o->max_budget = def_max;
	if (!o->credit_cap_set) o->credit_cap = def_cap;
	if (!o->budget_step_set) o->budget_step = IDMA_RESEARCH2_DEF_STEP;
	if (!o->hit_weight_set) o->hit_weight = IDMA_RESEARCH2_DEF_HIT;
	if (!o->miss_weight_set) o->miss_weight = IDMA_RESEARCH2_DEF_MISS;
	if (!o->ptw_weight_set) o->ptw_weight = IDMA_RESEARCH2_DEF_PTW;
	if (o->policy_window < IDMA_RESEARCH2_MIN_WINDOW ||
	    o->policy_window > IDMA_RESEARCH2_MAX_WINDOW) {
		snprintf(err, errlen, "invalid --policy-window (1024..1048576)");
		return -1;
	}
	if (o->target_bytes < 1 || o->target_bytes > IDMA_RESEARCH2_MAX_TARGET) {
		snprintf(err, errlen, "invalid --target-bytes (1..16777216)");
		return -1;
	}
	if (o->deadband_bytes > o->target_bytes) {
		snprintf(err, errlen, "--deadband-bytes exceeds --target-bytes");
		return -1;
	}
	if (o->min_budget > o->initial_budget || o->initial_budget > o->max_budget ||
	    o->max_budget > IDMA_RESEARCH2_MAX_BUDGET) {
		snprintf(err, errlen, "invalid --min/--initial/--max budget ordering");
		return -1;
	}
	if (o->policy_mode == 6) {
		if (o->max_budget > IDMA_RESEARCH2_MODE6_MAX) {
			snprintf(err, errlen, "mode 6 --max-budget exceeds 200000");
			return -1;
		}
	} else if (o->min_budget < 1) {
		snprintf(err, errlen, "billing modes require --min-budget >= 1");
		return -1;
	}
	if (o->budget_step < 1 || o->budget_step > o->max_budget) {
		snprintf(err, errlen, "invalid --budget-step (1..--max-budget)");
		return -1;
	}
	if (o->credit_cap < o->max_budget || o->credit_cap > IDMA_RESEARCH2_MAX_CAP) {
		snprintf(err, errlen, "invalid --credit-cap (--max-budget..67108864)");
		return -1;
	}
	return 0;
}

static int validate_v2_options(struct options *o, char *err, size_t errlen)
{
	if (o->v2_role == IDMA_RESEARCH2_E2_ROLE)
		return validate_e2_ext_options(o, err, errlen);
	return validate_method_options(o, err, errlen);
}

static int validate_options(struct options *o, char *err, size_t errlen)
{
	if (o->caps2)
		return validate_caps2_options(o, err, errlen);
	if (!o->mode) {
		snprintf(err, errlen, "specify exactly one of --e2, --cost, --e2-ext or --method");
		return -1;
	}
	if (o->require_fullstats && o->mode != IDMA_RESEARCH_COST) {
		snprintf(err, errlen, "--require-fullstats is only valid with --cost or --method");
		return -1;
	}
	if (o->v2)
		return validate_v2_options(o, err, errlen);
	if (o->mode == IDMA_RESEARCH_E2) {
		if (!o->a_pages_set) o->a_pages = 8;
		if (!o->a_transfers_set) o->a_transfers = o->a_pages ? 112 : 0;
		if (!o->window_cycles_set) o->window_cycles = 5000000;
		if (!o->warmup_set) o->warmup = 2;
		if (!o->transfers_set) o->transfers = 1;
		if (!o->gap_cycles_set) o->gap_cycles = 0;
		if (!o->timeout_ms_set) o->timeout_ms = 2000;
		if (o->a_pages != 0 && o->a_pages != 1 && o->a_pages != 4 &&
		    o->a_pages != 7 && o->a_pages != 8 && o->a_pages != 16) {
			snprintf(err, errlen, "invalid E2 --a-pages");
			return -1;
		}
		if (o->a_transfers != (o->a_pages ? 112 : 0)) {
			snprintf(err, errlen, "invalid E2 --a-transfers");
			return -1;
		}
		if (o->window_cycles != 5000000) {
			snprintf(err, errlen, "invalid E2 --window-cycles");
			return -1;
		}
		if (o->warmup != 2) {
			snprintf(err, errlen, "invalid E2 --warmup");
			return -1;
		}
		if (o->transfers != 1) {
			snprintf(err, errlen, "invalid E2 --transfers");
			return -1;
		}
		if (o->gap_cycles != 0) {
			snprintf(err, errlen, "invalid E2 --a-gap-cycles");
			return -1;
		}
		if (o->timeout_ms != 2000) {
			snprintf(err, errlen, "invalid E2 --timeout-ms");
			return -1;
		}
	} else {
		if (!o->a_pages_set) o->a_pages = 8;
		if (!o->a_transfers_set) o->a_transfers = 0;
		if (!o->window_cycles_set) o->window_cycles = 0;
		if (!o->gap_cycles_set) o->gap_cycles = 0;
		if (!o->transfers_set) o->transfers = 4096;
		if (!o->warmup_set) o->warmup = (o->transfers == 64) ? 8 : 256;
		if (!o->timeout_ms_set) o->timeout_ms = 10000;
		if (o->a_pages != 0 && o->a_pages != 1 && o->a_pages != 8 && o->a_pages != 16) {
			snprintf(err, errlen, "invalid cost --a-pages");
			return -1;
		}
		if (o->a_transfers != 0) {
			snprintf(err, errlen, "invalid cost --a-transfers");
			return -1;
		}
		if (o->window_cycles != 0) {
			snprintf(err, errlen, "invalid cost --window-cycles");
			return -1;
		}
		if (o->gap_cycles && (o->a_pages != 8 ||
		    (o->gap_cycles != 1000 && o->gap_cycles != 2000))) {
			snprintf(err, errlen, "invalid cost --a-gap-cycles");
			return -1;
		}
		if (!((o->transfers == 4096 && o->warmup == 256) ||
		      (o->transfers == 64 && o->warmup == 8))) {
			snprintf(err, errlen, "invalid cost --transfers/--warmup pair");
			return -1;
		}
		if (o->timeout_ms != 10000) {
			snprintf(err, errlen, "invalid cost --timeout-ms");
			return -1;
		}
	}
	return 0;
}

static int validate_caps(const struct idma_experiment_caps *c, char *err, size_t errlen)
{
	if (c->abi_version != IDMA_EXPERIMENT_ABI || c->struct_size != sizeof(*c)) {
		snprintf(err, errlen, "capabilities ABI/size mismatch");
		return -1;
	}
	if (!(c->capabilities & IDMA_EXPERIMENT_CAP_RESEARCH)) {
		snprintf(err, errlen, "driver lacks CAP_RESEARCH");
		return -1;
	}
	if (c->device_id != 0 || c->did != 10 || c->page_bytes != 4096) {
		snprintf(err, errlen, "capabilities context mismatch");
		return -1;
	}
	return 0;
}

static void json_string(const char *s)
{
	const unsigned char *p = (const unsigned char *)s;

	putchar('"');
	for (; *p; p++) {
		if (*p == '"' || *p == '\\')
			printf("\\%c", *p);
		else if (*p < 0x20)
			printf("\\u%04x", *p);
		else
			putchar(*p);
	}
	putchar('"');
}

static void json_hex64(uint64_t v)
{
	printf("\"0x%016" PRIx64 "\"", v);
}

static void print_stats(const struct idma_experiment_stats *s)
{
	printf("{\"submitted\":%" PRIu64 ",\"completed\":%" PRIu64
	       ",\"completed_bytes\":%" PRIu64 ",\"active_cycles\":%" PRIu64
	       ",\"iotlb_miss\":%" PRIu64 ",\"s1_ptw\":%" PRIu64
	       ",\"s2_ptw\":%" PRIu64 ",\"refill_gap_ns_sum\":%" PRIu64
	       ",\"refill_gap_ns_max\":%" PRIu64 ",\"refill_gap_samples\":%" PRIu64
	       ",\"in_flight_at_start\":%u,\"in_flight_at_end\":%u,\"data_errors\":%u,\"quarantined\":%u}",
	       (uint64_t)s->submitted, (uint64_t)s->completed, (uint64_t)s->completed_bytes,
	       (uint64_t)s->active_cycles, (uint64_t)s->iotlb_miss, (uint64_t)s->s1_ptw,
	       (uint64_t)s->s2_ptw, (uint64_t)s->refill_gap_ns_sum,
	       (uint64_t)s->refill_gap_ns_max, (uint64_t)s->refill_gap_samples,
	       s->in_flight_at_start, s->in_flight_at_end, s->data_errors, s->quarantined);
}

static void print_experiment(const struct idma_experiment *e)
{
	unsigned int i;

	printf("{\"abi_version\":%u,\"struct_size\":%u,\"flags\":%u,\"aggressor_id\":%u,"
	       "\"victim_pages\":%u,\"aggressor_pages\":%u,\"chunk_bytes\":%u,"
	       "\"run_timeout_ms\":%u,\"victim_transfers\":%" PRIu64
	       ",\"warmup_transfers\":%" PRIu64 ",\"reserved\":[",
	       e->abi_version, e->struct_size, e->flags, e->aggressor_id,
	       e->victim_pages, e->aggressor_pages, e->chunk_bytes, e->run_timeout_ms,
	       (uint64_t)e->victim_transfers, (uint64_t)e->warmup_transfers);
	for (i = 0; i < 8; i++)
		printf("%s%u", i ? "," : "", e->reserved[i]);
	printf("],\"status\":%d,\"victim_did\":%u,\"aggressor_did\":%u,"
	       "\"hpm_configured\":%u,\"measurement_started\":%u,\"measurement_complete\":%u,"
	       "\"victim_elapsed_ns\":%" PRIu64 ",\"snapshot_before_ns\":%" PRIu64
	       ",\"snapshot_after_ns\":%" PRIu64 ",\"drain_ns\":%" PRIu64
	       ",\"victim\":",
	       e->status, e->victim_did, e->aggressor_did, e->hpm_configured,
	       e->measurement_started, e->measurement_complete,
	       (uint64_t)e->victim_elapsed_ns, (uint64_t)e->snapshot_before_ns,
	       (uint64_t)e->snapshot_after_ns, (uint64_t)e->drain_ns);
	print_stats(&e->victim);
	printf(",\"aggressor\":");
	print_stats(&e->aggressor);
	printf("}");
}

static void print_rates(const struct idma_research *r, const uint64_t *raw, int valid)
{
	double v_rate = 0.0, a_rate = 0.0, a_jobs = 0.0;
	int v_ok = 0, a_ok = 0;

	if (valid && r->mode == IDMA_RESEARCH_COST) {
		uint64_t v_done = raw[0xc12];
		uint64_t window = raw[0xc19];
		uint64_t a_done = r->p4m.window[3];

		/* V and A rates share the hardware common window raw[0xc19].  The
		 * whole-session controller counters raw[0xc04..0xc07] are not the
		 * window length. */
		if (window && v_done) {
			v_rate = (double)v_done * 4096.0 * 50000000.0 /
				 (double)window / (1024.0 * 1024.0);
			v_ok = 1;
		}
		if (window) {
			a_rate = (double)a_done * 4096.0 * 50000000.0 /
				 (double)window / (1024.0 * 1024.0);
			a_jobs = (double)a_done * 50000000.0 / (double)window;
			a_ok = 1;
		}
	}
	printf("{\"platform_hz\":50000000,\"cycle_ns\":20,\"v_mib_per_s\":");
	if (v_ok)
		printf("%.6f", v_rate);
	else
		printf("null");
	printf(",\"a_common_mib_per_s\":");
	if (a_ok)
		printf("%.6f", a_rate);
	else
		printf("null");
	printf(",\"a_common_jobs_per_s\":");
	if (a_ok)
		printf("%.6f", a_jobs);
	else
		printf("null");
	printf("}");
}

static void output_json(const struct options *o, const struct idma_research *r,
			const uint64_t *raw, const struct idma_research2 *r2,
			int success, int validated,
			int syscall_rc, int saved_errno, const char *stage_name,
			const char *err)
{
	const char *mode = r->mode == IDMA_RESEARCH_E2 ? "e2" :
			   (r->mode == IDMA_RESEARCH_COST ? "cost" : "none");
	unsigned int i;

	printf("{\"schema\":\"idma-research-v1\",\"success\":%s,\"validated\":%s,"
	       "\"mode\":\"%s\",\"device\":",
	       success ? "true" : "false", validated ? "true" : "false", mode);
	json_string(o->device);
	printf(",\"syscall_rc\":%d,\"errno\":%d,\"status\":%d,\"flush_status\":%d,"
	       "\"stage\":%u,\"stage_name\":",
	       syscall_rc, saved_errno, (int)r->status, (int)r->flush_status, r->stage);
	json_string(stage_name);
	printf(",\"raw_captured\":%u,\"observer_magic\":", r->raw_captured);
	json_hex64((uint64_t)r->observer_magic);
	printf(",\"mapping\":[");
	for (i = 0; i < 4; i++) {
		if (i)
			putchar(',');
		json_hex64((uint64_t)r->mapping[i]);
	}
	printf("],\"input\":{");
	printf("\"abi_version\":%u,\"struct_size\":%u,\"mode\":%u,\"a_pages\":%u,"
	       "\"a_transfers\":%u,\"window_cycles\":%u,\"warmup\":%u,"
	       "\"transfers\":%u,\"gap_cycles\":%u,\"timeout_ms\":%u,\"raw_words\":%u",
	       r->abi_version, r->struct_size, r->mode, r->a_pages, r->a_transfers,
	       r->window_cycles, r->warmup, r->transfers, r->gap_cycles,
	       r->timeout_ms, r->raw_words);
	printf("},\"phase_stats\":[");
	for (i = 0; i < 3; i++) {
		if (i)
			putchar(',');
		print_stats(&r->phase_stats[i]);
	}
	printf("],\"p4m\":{\"aggressor_gap_cycles\":%u,\"sequence\":",
	       r->p4m.aggressor_gap_cycles);
	json_hex64((uint64_t)r->p4m.sequence);
	printf(",\"observer_status\":");
	json_hex64((uint64_t)r->p4m.observer_status);
	printf(",\"observation\":[");
	for (i = 0; i < IDMA_P4M_OBS_WORDS; i++) {
		if (i)
			putchar(',');
		json_hex64((uint64_t)r->p4m.observation[i]);
	}
	printf("],\"window\":[");
	for (i = 0; i < IDMA_P4M_WORDS; i++) {
		if (i)
			putchar(',');
		json_hex64((uint64_t)r->p4m.window[i]);
	}
	printf("],\"experiment\":");
	print_experiment(&r->p4m.experiment);
	printf("},\"raw\":[");
	for (i = 0; i < IDMA_RESEARCH_RAW_WORDS; i++) {
		if (i)
			putchar(',');
		json_hex64(raw[i]);
	}
	printf("],\"rates\":");
	print_rates(r, raw, success);
	if (r2) {
		const struct idma_research2_config *c = &r2->config;

		printf(",\"research2\":{\"api_version\":2,\"role\":%u,\"config\":{",
		       c->required_role);
		printf("\"abi_version\":%u,\"struct_size\":%u,\"required_role\":%u,"
		       "\"reverse\":%u,\"policy_mode\":%u,\"window_cycles\":%u,"
		       "\"target_bytes\":%u,\"deadband_bytes\":%u,\"initial_budget\":%u,"
		       "\"min_budget\":%u,\"max_budget\":%u,\"budget_step\":%u,"
		       "\"credit_cap\":%u,\"hit_weight\":%u,\"miss_weight\":%u,"
		       "\"ptw_weight\":%u,\"reserved\":[",
		       c->abi_version, c->struct_size, c->required_role, c->reverse,
		       c->policy_mode, c->window_cycles, c->target_bytes,
		       c->deadband_bytes, c->initial_budget, c->min_budget,
		       c->max_budget, c->budget_step, c->credit_cap, c->hit_weight,
		       c->miss_weight, c->ptw_weight);
		for (i = 0; i < 4; i++)
			printf("%s%u", i ? "," : "", c->reserved[i]);
		printf("]},\"caps\":{\"magic\":");
		json_hex64(raw[0xf00]);
		printf(",\"version\":%" PRIu64 ",\"role\":%" PRIu64 ",\"features\":%" PRIu64,
		       raw[0xf01], raw[0xf02], raw[0xf03]);
		printf("}}");
	}
	printf(",\"errors\":[");
	if (*err)
		json_string(err);
	printf("]}");
}

static void output_caps2_json(int success, const struct idma_research2_caps *c,
			      const char *err)
{
	printf("{\"schema\":\"idma-research2-caps-v1\",\"success\":%s,\"magic\":",
	       success ? "true" : "false");
	json_hex64(c->magic);
	printf(",\"version\":%" PRIu64 ",\"role\":%" PRIu64 ",\"features\":%" PRIu64 ",\"observer_magic\":",
	       (uint64_t)c->version, (uint64_t)c->role, (uint64_t)c->features);
	json_hex64(c->observer_magic);
	printf(",\"error\":");
	json_string(err);
	printf("}");
}

static int run_caps2(const struct options *o)
{
	struct idma_research2_caps c;
	char err[256] = {0};
	int fd, rc, success = 0, saved_errno = 0;

	memset(&c, 0, sizeof(c));
	c.abi_version = IDMA_RESEARCH2_ABI;
	c.struct_size = sizeof(c);
	fd = open(o->device, O_RDWR | O_CLOEXEC);
	if (fd < 0) {
		saved_errno = errno;
		snprintf(err, sizeof(err), "open: %s", strerror(saved_errno));
		output_caps2_json(0, &c, err);
		fprintf(stderr, "idma_research: %s\n", err);
		return EXIT_FAILURE;
	}
	rc = ioctl(fd, IDMA_IOC_RESEARCH2_CAPS, &c);
	if (rc < 0) {
		saved_errno = errno;
		snprintf(err, sizeof(err), "caps2: %s", strerror(saved_errno));
	} else if (c.magic != IDMA_RESEARCH2_CAP_MAGIC || c.version != 1) {
		snprintf(err, sizeof(err), "caps2 magic/version mismatch");
	} else {
		success = 1;
	}
	if (close(fd) < 0 && success) {
		saved_errno = errno;
		snprintf(err, sizeof(err), "close: %s", strerror(saved_errno));
		success = 0;
	}
	if (*err)
		fprintf(stderr, "idma_research: %s\n", err);
	output_caps2_json(success, &c, err);
	if (fflush(stdout) == EOF)
		return EXIT_FAILURE;
	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int entry_is_ordinary(uint64_t lo, uint64_t hi)
{
	(void)hi;
	if (!(lo & (1ULL << 51)))
		return 0;
	if (!(lo & (1ULL << 49)))
		return 0;
	if (lo & (1ULL << 50))
		return 0;
	if (lo & ((1ULL << 52) | (1ULL << 53) | (1ULL << 54) | (1ULL << 55) | (1ULL << 56)))
		return 0;
	return 1;
}

static uint64_t entry_vpn(uint64_t lo)
{
	return lo & ((1ULL << 29) - 1);
}

static uint64_t entry_pscid(uint64_t lo)
{
	return (lo >> 29) & ((1ULL << 20) - 1);
}

static uint64_t entry_gscid(uint64_t hi)
{
	return hi & 0xFFFF;
}

static int validate_e2_raw(const struct options *o, const struct idma_research *r,
			   const uint64_t *raw, char *err, size_t errlen)
{
	uint64_t T = r->window_cycles;
	uint64_t astart, deadline, a_counts = r->a_transfers;
	unsigned int i, events;

	(void)o;
	if (raw[0] != IDMA_RESEARCH_MAGIC) {
		snprintf(err, errlen, "E2 raw[0] magic mismatch");
		return -1;
	}
	if (raw[1] != RESEARCH_OBSERVER_MAGIC) {
		snprintf(err, errlen, "E2 raw[1] observer magic mismatch");
		return -1;
	}
	if (raw[4] != IDMA_RESEARCH_E2) {
		snprintf(err, errlen, "E2 raw[4] mode mismatch");
		return -1;
	}
	if (raw[6] != 4) {
		snprintf(err, errlen, "E2 phase is not 4");
		return -1;
	}
	if (raw[3] || raw[0xd] || raw[0xc] || raw[0xb]) {
		snprintf(err, errlen, "E2 error/overflow/inval counters nonzero");
		return -1;
	}
	if (!(raw[0x2] & (1ULL << 21)) || !(raw[0x2] & (1ULL << 22))) {
		snprintf(err, errlen, "E2 observer not quiet/supported");
		return -1;
	}
	if (raw[0x21] != 7) {
		snprintf(err, errlen, "E2 snapshot_valid is not 7");
		return -1;
	}
	if (raw[0x1c] != 3) {
		snprintf(err, errlen, "E2 target_valid is not 3");
		return -1;
	}
	if (raw[0x1d] != 3) {
		snprintf(err, errlen, "E2 firstprobe_done is not 3");
		return -1;
	}
	if (raw[0x25] != a_counts || raw[0x26] != a_counts) {
		snprintf(err, errlen, "E2 A issue/done count mismatch");
		return -1;
	}
	astart = raw[7];
	deadline = raw[0x1e];
	if (raw[8] != T) {
		snprintf(err, errlen, "E2 A_ELAPSED mismatch");
		return -1;
	}
	if (deadline != astart + T) {
		snprintf(err, errlen, "E2 deadline/astart mismatch");
		return -1;
	}
	if (raw[0x1f] < deadline) {
		snprintf(err, errlen, "E2 PROBESTART before deadline");
		return -1;
	}
	if (raw[0x20] < raw[0x14] || raw[0x20] < raw[0x18]) {
		snprintf(err, errlen, "E2 FREEZE before first probe");
		return -1;
	}
	if (!entry_is_ordinary(raw[0x10], raw[0x11]) ||
	    !entry_is_ordinary(raw[0x12], raw[0x13])) {
		snprintf(err, errlen, "E2 target not ordinary S1-only 4K");
		return -1;
	}
	if (entry_pscid(raw[0x10]) != entry_pscid(raw[0x12]) ||
	    entry_gscid(raw[0x11]) != entry_gscid(raw[0x13])) {
		snprintf(err, errlen, "E2 targets differ in PSCID/GSCID");
		return -1;
	}
	if (entry_vpn(raw[0x10]) != ((r->mapping[0] >> 12) & ((1ULL << 29) - 1)) ||
	    entry_vpn(raw[0x12]) != ((r->mapping[1] >> 12) & ((1ULL << 29) - 1))) {
		snprintf(err, errlen, "E2 target VPN does not match mapping");
		return -1;
	}
	/* RTL records the first-probe key as cap_key on a hit, or the
	 * synthesized S1-only lookup context on a miss (ctx_key_128 forces the
	 * valid bit), so the key is not necessarily an ordinary entry and a
	 * first-probe miss is a valid research outcome.  Only identity against
	 * the captured target is required. */
	if (entry_vpn(raw[0x16]) != entry_vpn(raw[0x10]) ||
	    entry_pscid(raw[0x16]) != entry_pscid(raw[0x10]) ||
	    entry_gscid(raw[0x17]) != entry_gscid(raw[0x11]) ||
	    entry_vpn(raw[0x1a]) != entry_vpn(raw[0x12]) ||
	    entry_pscid(raw[0x1a]) != entry_pscid(raw[0x12]) ||
	    entry_gscid(raw[0x1b]) != entry_gscid(raw[0x13])) {
		snprintf(err, errlen, "E2 firstprobe key identity mismatch");
		return -1;
	}
	if (!!(raw[0x15] & (1ULL << 32)) != !!(raw[0x15] & 0xFFFF)) {
		snprintf(err, errlen, "E2 FP0 hit/presence mismatch");
		return -1;
	}
	if (!!(raw[0x19] & (1ULL << 32)) != !!(raw[0x19] & 0xFFFF)) {
		snprintf(err, errlen, "E2 FP1 hit/presence mismatch");
		return -1;
	}
	if (r->phase_stats[0].submitted != 2 || r->phase_stats[0].completed != 2) {
		snprintf(err, errlen, "E2 warm stats mismatch");
		return -1;
	}
	if (r->phase_stats[1].submitted != a_counts || r->phase_stats[1].completed != a_counts) {
		snprintf(err, errlen, "E2 A stats mismatch");
		return -1;
	}
	if (r->phase_stats[2].submitted != 1 || r->phase_stats[2].completed != 1) {
		snprintf(err, errlen, "E2 probe stats mismatch");
		return -1;
	}
	for (i = 0; i < 3; i++) {
		if (r->phase_stats[i].completed_bytes != r->phase_stats[i].completed * 4096ULL) {
			snprintf(err, errlen, "E2 stats bytes mismatch");
			return -1;
		}
		if (r->phase_stats[i].data_errors || r->phase_stats[i].quarantined) {
			snprintf(err, errlen, "E2 stats errors/quarantine");
			return -1;
		}
	}
	events = (unsigned int)(raw[0xa] & 0xFFFF);
	if (events > 256) {
		snprintf(err, errlen, "E2 event count overflow");
		return -1;
	}
	if ((raw[0xe] & 0xFFFFFFFFULL) > 256) {
		snprintf(err, errlen, "E2 update count overflow");
		return -1;
	}
	for (i = 0; i < events; i++) {
		uint64_t meta = raw[0x200 + i * 6 + 1];
		uint64_t mask = meta & 0xFFFF;

		if (!mask || (mask & (mask - 1))) {
			snprintf(err, errlen, "E2 event slot mask not one-hot");
			return -1;
		}
	}
	return 0;
}

static int validate_fullstats_raw(const struct options *o, const uint64_t *raw,
				  char *err, size_t errlen)
{
	unsigned __int128 total_count = 0, total_main = 0, total_complete = 0;
	unsigned __int128 total_duration = 0, total_req = 0, total_miss = 0;
	unsigned int g, i;

	if (!raw[0xd00]) {
		if (o->require_fullstats) {
			snprintf(err, errlen, "fullstats magic missing");
			return -1;
		}
		return 0;
	}
	if (raw[0xd00] != IDMA_FULLSTATS_MAGIC) {
		snprintf(err, errlen, "fullstats magic unknown");
		return -1;
	}
	if (raw[0xd01] != IDMA_FULLSTATS_VERSION) {
		snprintf(err, errlen, "fullstats version mismatch");
		return -1;
	}
	if (raw[0xd02] != IDMA_FULLSTATS_GROUPS) {
		snprintf(err, errlen, "fullstats group count mismatch");
		return -1;
	}
	if (raw[0xd03] != IDMA_FULLSTATS_STRIDE) {
		snprintf(err, errlen, "fullstats group stride mismatch");
		return -1;
	}
	if (raw[0xd04]) {
		snprintf(err, errlen, "fullstats sticky error nonzero");
		return -1;
	}
	for (i = 0xd06; i <= 0xd0f; i++) {
		if (raw[i]) {
			snprintf(err, errlen, "fullstats header reserved nonzero");
			return -1;
		}
	}
	for (g = 0; g < IDMA_FULLSTATS_GROUPS; g++) {
		unsigned int base = 0xd10 + g * IDMA_FULLSTATS_STRIDE;
		uint64_t count = raw[base + 0];
		uint64_t sum_main = raw[base + 1];
		uint64_t sum_complete = raw[base + 2];
		uint64_t sum_duration = raw[base + 3];
		uint64_t sum_req = raw[base + 4];
		uint64_t sum_miss = raw[base + 5];
		uint64_t mm = raw[base + 6];
		uint64_t mc = raw[base + 7];
		uint64_t md = raw[base + 8];
		uint64_t min_main = mm & 0xffffffffULL;
		uint64_t max_main = mm >> 32;
		uint64_t min_complete = mc & 0xffffffffULL;
		uint64_t max_complete = mc >> 32;
		uint64_t min_duration = md & 0xffffffffULL;
		uint64_t max_duration = md >> 32;

		for (i = 9; i <= 15; i++) {
			if (raw[base + i]) {
				snprintf(err, errlen, "fullstats group reserved nonzero");
				return -1;
			}
		}
		if (!count) {
			if (sum_main || sum_complete || sum_duration || sum_req ||
			    sum_miss || mm || mc || md) {
				snprintf(err, errlen, "fullstats empty group nonzero");
				return -1;
			}
			continue;
		}
		if (min_main > max_main || min_complete > max_complete ||
		    min_duration > max_duration) {
			snprintf(err, errlen, "fullstats group min/max order");
			return -1;
		}
		if ((unsigned __int128)sum_main < (unsigned __int128)min_main * count ||
		    (unsigned __int128)sum_main > (unsigned __int128)max_main * count ||
		    (unsigned __int128)sum_complete < (unsigned __int128)min_complete * count ||
		    (unsigned __int128)sum_complete > (unsigned __int128)max_complete * count ||
		    (unsigned __int128)sum_duration < (unsigned __int128)min_duration * count ||
		    (unsigned __int128)sum_duration > (unsigned __int128)max_duration * count) {
			snprintf(err, errlen, "fullstats group mean out of range");
			return -1;
		}
		if ((unsigned __int128)sum_main + sum_complete > sum_duration) {
			snprintf(err, errlen, "fullstats group main+complete exceeds duration");
			return -1;
		}
		if (sum_miss > sum_req) {
			snprintf(err, errlen, "fullstats group miss exceeds requests");
			return -1;
		}
		if (g < 5) {
			if ((unsigned __int128)sum_req != (unsigned __int128)4 * count) {
				snprintf(err, errlen, "fullstats group requests mismatch");
				return -1;
			}
			if ((unsigned __int128)sum_miss != (unsigned __int128)g * count) {
				snprintf(err, errlen, "fullstats group misses mismatch");
				return -1;
			}
		}
		total_count += count;
		total_main += sum_main;
		total_complete += sum_complete;
		total_duration += sum_duration;
		total_req += sum_req;
		total_miss += sum_miss;
	}
	if (total_count != raw[0xd05]) {
		snprintf(err, errlen, "fullstats total submits mismatch");
		return -1;
	}
	if (raw[0xd05] != raw[0xc14]) {
		snprintf(err, errlen, "fullstats C14 eligible mismatch");
		return -1;
	}
	if (o->a_pages == 0) {
		if (total_count || total_main || total_complete || total_duration ||
		    total_req || total_miss) {
			snprintf(err, errlen, "fullstats Solo nonzero A");
			return -1;
		}
	}
	/* Common-window A counters use the A (not V) registers:
	 * 0xc1b win_main_A, 0xc1d win_complete_A, 0xc1f win_select_A and
	 * 0xc23 win_miss_A.  0xc1a/0xc1c are V-side window counters and must
	 * not bound the A aggregation.  Every bound fails on its own field. */
	if (total_main > (unsigned __int128)raw[0xc1b]) {
		snprintf(err, errlen, "fullstats total main exceeds common window main");
		return -1;
	}
	if (total_complete > (unsigned __int128)raw[0xc1d]) {
		snprintf(err, errlen, "fullstats total complete exceeds common window complete");
		return -1;
	}
	if (total_req > (unsigned __int128)raw[0xc1f]) {
		snprintf(err, errlen, "fullstats total requests exceed common window select");
		return -1;
	}
	if (total_miss > (unsigned __int128)raw[0xc23]) {
		snprintf(err, errlen, "fullstats total misses exceed common window miss");
		return -1;
	}
	if (total_duration > (unsigned __int128)raw[0xc19]) {
		snprintf(err, errlen, "fullstats duration exceeds window");
		return -1;
	}
	return 0;
}

static int validate_cost_raw(const struct options *o, const struct idma_research *r,
			     const uint64_t *raw, char *err, size_t errlen)
{
	const struct idma_p4m *p = &r->p4m;
	uint64_t sum;
	unsigned int i;

	if (raw[0] != IDMA_RESEARCH_MAGIC) {
		snprintf(err, errlen, "cost raw[0] magic mismatch");
		return -1;
	}
	if (raw[1] != RESEARCH_OBSERVER_MAGIC) {
		snprintf(err, errlen, "cost raw[1] observer magic mismatch");
		return -1;
	}
	if (raw[3]) {
		snprintf(err, errlen, "cost error counter nonzero");
		return -1;
	}
	if (raw[4] != IDMA_RESEARCH_COST) {
		snprintf(err, errlen, "cost raw[4] mode mismatch");
		return -1;
	}
	if (raw[0xc00] != RESEARCH_COST_MAGIC) {
		snprintf(err, errlen, "cost raw[0xc00] magic mismatch");
		return -1;
	}
	if (raw[0xc01] != 6) {
		snprintf(err, errlen, "cost status is not 6");
		return -1;
	}
	if (raw[0xc02] != 0) {
		snprintf(err, errlen, "cost error nonzero");
		return -1;
	}
	if (raw[0xc16] != 0) {
		snprintf(err, errlen, "cost sample overflow");
		return -1;
	}
	if (!raw[0xc19] || raw[0xc19] != p->window[1] || raw[0xc19] != p->observation[0]) {
		snprintf(err, errlen, "cost window/cycle mismatch");
		return -1;
	}
	if (raw[0xc11] != r->transfers || raw[0xc12] != r->transfers) {
		snprintf(err, errlen, "cost V issue/done mismatch");
		return -1;
	}
	if (raw[0xc15] != raw[0xc14] / 16 || raw[0xc15] > 256) {
		snprintf(err, errlen, "cost sample count mismatch");
		return -1;
	}
	if (p->observer_status != 4) {
		snprintf(err, errlen, "P4M observer status is not 4");
		return -1;
	}
	if (!p->sequence) {
		snprintf(err, errlen, "P4M sequence is zero");
		return -1;
	}
	if (p->window[0] != P4M_WINDOW_MAGIC) {
		snprintf(err, errlen, "P4M window magic mismatch");
		return -1;
	}
	if (!(p->window[15] & 0x2)) {
		snprintf(err, errlen, "P4M window not closed");
		return -1;
	}
	if (p->window[15] & ~0x7ULL) {
		snprintf(err, errlen, "P4M window flags invalid");
		return -1;
	}
	if (p->experiment.status != 0) {
		snprintf(err, errlen, "P4M experiment status nonzero");
		return -1;
	}
	if (p->window[15] & 0x1) {
		if (p->window[4] + p->window[2] != p->window[3] + p->window[5]) {
			snprintf(err, errlen, "P4M conservation mismatch");
			return -1;
		}
	} else {
		for (i = 2; i <= 12; i++) {
			if (p->window[i]) {
				snprintf(err, errlen, "P4M Solo nonzero field");
				return -1;
			}
		}
	}
	/* Whole-session controller counters (raw[0xc04..0xc07]) may exceed the
	 * common window; only the window-scoped occupancy counters
	 * raw[0xc1a..0xc1d] are bounded by raw[0xc19].  At most one of the four
	 * advances per window cycle (one ctl lane in one of {main, comp}). */
	if (raw[0xc1a] > raw[0xc19] || raw[0xc1b] > raw[0xc19] ||
	    raw[0xc1c] > raw[0xc19] || raw[0xc1d] > raw[0xc19]) {
		snprintf(err, errlen, "cost window main/complete exceeds window");
		return -1;
	}
	sum = raw[0xc1a] + raw[0xc1b] + raw[0xc1c] + raw[0xc1d];
	if (sum < raw[0xc1a] || sum > raw[0xc19]) {
		snprintf(err, errlen, "cost window main/complete sum exceeds window");
		return -1;
	}
	if (p->experiment.victim.data_errors || p->experiment.victim.quarantined ||
	    (o->a_pages && (p->experiment.aggressor.data_errors ||
			    p->experiment.aggressor.quarantined))) {
		snprintf(err, errlen, "P4M kernel data errors/quarantine");
		return -1;
	}
	if (validate_fullstats_raw(o, raw, err, errlen)) {
		return -1;
	}
	return 0;
}

static int validate_caps2(const struct options *o,
			  const struct idma_research2_caps *c,
			  char *err, size_t errlen)
{
	uint64_t features = (o->v2_role == IDMA_RESEARCH2_E2_ROLE) ? 0x7ULL : 0xfULL;

	if (c->abi_version != IDMA_RESEARCH2_ABI || c->struct_size != sizeof(*c)) {
		snprintf(err, errlen, "research2 caps ABI/size mismatch");
		return -1;
	}
	if (c->magic != IDMA_RESEARCH2_CAP_MAGIC) {
		snprintf(err, errlen, "research2 caps F00 magic mismatch");
		return -1;
	}
	if (c->version != 1) {
		snprintf(err, errlen, "research2 caps F01 version mismatch");
		return -1;
	}
	if (c->role != (uint64_t)o->v2_role) {
		snprintf(err, errlen, "research2 caps F02 role mismatch");
		return -1;
	}
	if ((c->features & features) != features || (c->features & ~0xFULL)) {
		snprintf(err, errlen, "research2 caps F03 features mismatch");
		return -1;
	}
	if (c->observer_magic != RESEARCH_OBSERVER_MAGIC) {
		snprintf(err, errlen, "research2 caps observer magic mismatch");
		return -1;
	}
	return 0;
}

static int validate_research2_config(const struct options *o,
				     const struct idma_research2 *r2,
				     char *err, size_t errlen)
{
	const struct idma_research2_config *c = &r2->config;

	if (c->abi_version != IDMA_RESEARCH2_ABI ||
	    c->struct_size != sizeof(*c) ||
	    c->required_role != (uint32_t)o->v2_role ||
	    c->reverse != o->reverse ||
	    c->policy_mode != o->policy_mode ||
	    c->window_cycles != o->policy_window ||
	    c->target_bytes != o->target_bytes ||
	    c->deadband_bytes != o->deadband_bytes ||
	    c->initial_budget != o->initial_budget ||
	    c->min_budget != o->min_budget ||
	    c->max_budget != o->max_budget ||
	    c->budget_step != o->budget_step ||
	    c->credit_cap != o->credit_cap ||
	    c->hit_weight != o->hit_weight ||
	    c->miss_weight != o->miss_weight ||
	    c->ptw_weight != o->ptw_weight ||
	    c->reserved[0] || c->reserved[1] || c->reserved[2] || c->reserved[3]) {
		snprintf(err, errlen, "research2 config changed");
		return -1;
	}
	return 0;
}

static int validate_research2_raw(const struct options *o,
				  const uint64_t *raw,
				  char *err, size_t errlen)
{
	uint64_t features = (o->v2_role == IDMA_RESEARCH2_E2_ROLE) ? 0x7ULL : 0xfULL;

	if (raw[0xf00] != IDMA_RESEARCH2_CAP_MAGIC) {
		snprintf(err, errlen, "research2 raw F00 magic mismatch");
		return -1;
	}
	if (raw[0xf01] != 1) {
		snprintf(err, errlen, "research2 raw F01 version mismatch");
		return -1;
	}
	if (raw[0xf02] != (uint64_t)o->v2_role) {
		snprintf(err, errlen, "research2 raw F02 role mismatch");
		return -1;
	}
	if ((raw[0xf03] & features) != features || (raw[0xf03] & ~0xFULL)) {
		snprintf(err, errlen, "research2 raw F03 features mismatch");
		return -1;
	}
	if ((raw[0xf10] & 1ULL) != (o->reverse & 1ULL) || (raw[0xf10] & ~1ULL)) {
		snprintf(err, errlen, "research2 raw F10 reverse mismatch");
		return -1;
	}
	return 0;
}

static int validate_e2_ext_raw(const uint64_t *raw, char *err, size_t errlen)
{
	uint64_t issue, select, done;

	if (raw[0xd80] != IDMA_RESEARCH2_E2_TIMING_MAGIC) {
		snprintf(err, errlen, "E2 timing D80 magic mismatch");
		return -1;
	}
	if (raw[0xd81] != 1) {
		snprintf(err, errlen, "E2 timing D81 version mismatch");
		return -1;
	}
	if (raw[0xd82] != 0) {
		snprintf(err, errlen, "E2 timing D82 error nonzero");
		return -1;
	}
	if (raw[0xd83] != 7) {
		snprintf(err, errlen, "E2 timing D83 flags not 7");
		return -1;
	}
	issue = raw[0xd84];
	select = raw[0xd85];
	done = raw[0xd86];
	if (issue > select || select > done) {
		snprintf(err, errlen, "E2 timing D84..D86 epoch order");
		return -1;
	}
	if (raw[0xd87] != done - issue) {
		snprintf(err, errlen, "E2 timing D87 done-issue mismatch");
		return -1;
	}
	if (raw[0xd88] != select - issue) {
		snprintf(err, errlen, "E2 timing D88 first_select-issue mismatch");
		return -1;
	}
	if (raw[0xd8e] || raw[0xd8f]) {
		snprintf(err, errlen, "E2 timing D8E/D8F reserved nonzero");
		return -1;
	}
	return 0;
}

static int validate_method_raw(const struct idma_research2 *r2,
			       const uint64_t *raw,
			       char *err, size_t errlen)
{
	const struct idma_research2_config *c = &r2->config;
	uint64_t status = raw[0xe03];

	if (raw[0xe00] != IDMA_RESEARCH2_POLICY_MAGIC) {
		snprintf(err, errlen, "method E00 magic mismatch");
		return -1;
	}
	if (raw[0xe01] != 1) {
		snprintf(err, errlen, "method E01 version mismatch");
		return -1;
	}
	if (raw[0xe02] != 0) {
		snprintf(err, errlen, "method E02 error nonzero");
		return -1;
	}
	if (!(status & 0x2) || (status & 0x1) || (status & 0x4) || (status & ~0x7ULL)) {
		snprintf(err, errlen, "method E03 not frozen/quiet");
		return -1;
	}
	if (raw[0xe05] != c->policy_mode ||
	    raw[0xe06] != c->window_cycles ||
	    raw[0xe07] != c->target_bytes ||
	    raw[0xe08] != c->deadband_bytes ||
	    raw[0xe09] != c->initial_budget ||
	    raw[0xe0a] != c->min_budget ||
	    raw[0xe0b] != c->max_budget ||
	    raw[0xe0c] != c->budget_step ||
	    raw[0xe0d] != c->credit_cap ||
	    raw[0xe0e] != c->hit_weight ||
	    raw[0xe0f] != c->miss_weight ||
	    raw[0xe10] != c->ptw_weight) {
		snprintf(err, errlen, "method E05..E10 config echo mismatch");
		return -1;
	}
	return 0;
}

int main(int argc, char **argv)
{
	struct options o = { .device = "/dev/idma0" };
	struct idma_research req;
	struct idma_research2 req2;
	struct idma_research2 *r2 = NULL;
	struct idma_research *reqp = &req;
	uint64_t raw[IDMA_RESEARCH_RAW_WORDS] = {0};
	struct idma_experiment_caps caps;
	struct idma_research2_caps caps2;
	char err[256] = {0};
	const char *stage_name = "arguments";
	int fd = -1, rc = 0, saved_errno = 0;
	int success = 0, validated = 0;
	int parse_rc;

	memset(&req, 0, sizeof(req));
	req.abi_version = IDMA_RESEARCH_ABI;
	req.struct_size = sizeof(req);
	req.raw_words = IDMA_RESEARCH_RAW_WORDS;
	memset(&req2, 0, sizeof(req2));
	memset(&caps, 0, sizeof(caps));
	memset(&caps2, 0, sizeof(caps2));

	parse_rc = parse_args(argc, argv, &o, err, sizeof(err));
	if (parse_rc < 0) {
		rc = -1;
		output_json(&o, &req, raw, NULL, 0, 0, rc, 0, stage_name, err);
		fprintf(stderr, "idma_research: %s\n", err);
		return EXIT_FAILURE;
	}
	if (o.help) {
		usage();
		return EXIT_SUCCESS;
	}
	if (validate_options(&o, err, sizeof(err))) {
		rc = -1;
		output_json(&o, &req, raw, NULL, 0, 0, rc, 0, stage_name, err);
		fprintf(stderr, "idma_research: %s\n", err);
		return EXIT_FAILURE;
	}
	if (o.caps2)
		return run_caps2(&o);

	if (o.v2) {
		fill_request2(&req2, &o, raw);
		reqp = &req2.base;
		r2 = &req2;
	} else {
		req.mode = o.mode;
		req.a_pages = o.a_pages;
		req.a_transfers = o.a_transfers;
		req.window_cycles = o.window_cycles;
		req.warmup = o.warmup;
		req.transfers = o.transfers;
		req.gap_cycles = o.gap_cycles;
		req.timeout_ms = o.timeout_ms;
		req.raw_words = IDMA_RESEARCH_RAW_WORDS;
		req.raw_user = (uint64_t)(uintptr_t)raw;
		reqp = &req;
		r2 = NULL;
	}

	stage_name = "open";
	fd = open(o.device, O_RDWR | O_CLOEXEC);
	if (fd < 0) {
		saved_errno = errno;
		snprintf(err, sizeof(err), "open: %s", strerror(saved_errno));
		rc = -1;
		goto done;
	}
	if (o.v2) {
		stage_name = "caps2";
		caps2.abi_version = IDMA_RESEARCH2_ABI;
		caps2.struct_size = sizeof(caps2);
		rc = ioctl(fd, IDMA_IOC_RESEARCH2_CAPS, &caps2);
		if (rc < 0) {
			saved_errno = errno;
			snprintf(err, sizeof(err), "caps2: %s", strerror(saved_errno));
			goto done;
		}
		if (validate_caps2(&o, &caps2, err, sizeof(err))) {
			rc = 0;
			goto done;
		}
		validated = 1;
		stage_name = "run";
		rc = ioctl(fd, IDMA_IOC_RUN_RESEARCH2, &req2);
	} else {
		stage_name = "capabilities";
		rc = ioctl(fd, IDMA_IOC_EXPERIMENT_CAPS, &caps);
		if (rc < 0) {
			saved_errno = errno;
			snprintf(err, sizeof(err), "capabilities: %s", strerror(saved_errno));
			goto done;
		}
		if (validate_caps(&caps, err, sizeof(err))) {
			rc = 0;
			goto done;
		}
		validated = 1;
		stage_name = "run";
		rc = ioctl(fd, IDMA_IOC_RUN_RESEARCH, reqp);
	}
	saved_errno = rc < 0 ? errno : 0;
	if (rc < 0) {
		if (-(int)reqp->status != saved_errno) {
			snprintf(err, sizeof(err), "ioctl errno %d does not match status %d",
				 saved_errno, (int)reqp->status);
		} else {
			snprintf(err, sizeof(err), "run: %s", strerror(saved_errno));
		}
		goto done;
	}
	if (reqp->abi_version != IDMA_RESEARCH_ABI || reqp->struct_size != sizeof(*reqp) ||
	    reqp->raw_words != IDMA_RESEARCH_RAW_WORDS) {
		snprintf(err, sizeof(err), "run ABI mismatch");
		goto done;
	}
	if (reqp->status != 0) {
		snprintf(err, sizeof(err), "run status %d", (int)reqp->status);
		goto done;
	}
	if (reqp->flush_status != 0 &&
	    !(reqp->mode == IDMA_RESEARCH_COST && reqp->flush_status == -EINPROGRESS)) {
		snprintf(err, sizeof(err), "flush status %d", (int)reqp->flush_status);
		goto done;
	}
	if (reqp->stage != 5) {
		snprintf(err, sizeof(err), "stage %u", reqp->stage);
		goto done;
	}
	if (reqp->raw_captured != IDMA_RESEARCH_RAW_WORDS) {
		snprintf(err, sizeof(err), "raw captured %u", reqp->raw_captured);
		goto done;
	}
	if (reqp->observer_magic != RESEARCH_OBSERVER_MAGIC) {
		snprintf(err, sizeof(err), "observer magic mismatch");
		goto done;
	}
	if (reqp->mode != (uint32_t)o.mode || reqp->a_pages != o.a_pages ||
	    reqp->a_transfers != o.a_transfers ||
	    reqp->window_cycles != o.window_cycles || reqp->warmup != o.warmup ||
	    reqp->transfers != o.transfers || reqp->gap_cycles != o.gap_cycles ||
	    reqp->timeout_ms != o.timeout_ms ||
	    reqp->raw_user != (uint64_t)(uintptr_t)raw) {
		snprintf(err, sizeof(err), "run request fields changed");
		goto done;
	}
	if (o.v2) {
		if (validate_research2_config(&o, &req2, err, sizeof(err)))
			goto done;
		if (validate_research2_raw(&o, raw, err, sizeof(err)))
			goto done;
		if (o.v2_role == IDMA_RESEARCH2_E2_ROLE) {
			if (validate_e2_raw(&o, reqp, raw, err, sizeof(err)))
				goto done;
			if (validate_e2_ext_raw(raw, err, sizeof(err)))
				goto done;
		} else {
			if (validate_cost_raw(&o, reqp, raw, err, sizeof(err)))
				goto done;
			if (validate_method_raw(&req2, raw, err, sizeof(err)))
				goto done;
		}
	} else if (reqp->mode == IDMA_RESEARCH_E2) {
		if (validate_e2_raw(&o, reqp, raw, err, sizeof(err))) {
			success = 0;
			goto done;
		}
	} else {
		if (validate_cost_raw(&o, reqp, raw, err, sizeof(err))) {
			success = 0;
			goto done;
		}
	}
	success = 1;
	stage_name = "complete";
done:
	if (fd >= 0 && close(fd) < 0 && success) {
		saved_errno = errno;
		snprintf(err, sizeof(err), "close: %s", strerror(saved_errno));
		success = 0;
		stage_name = "close";
	}
	if (*err)
		fprintf(stderr, "idma_research: %s\n", err);
	output_json(&o, reqp, raw, r2, success, validated, rc, saved_errno, stage_name, err);
	if (fflush(stdout) == EOF)
		return EXIT_FAILURE;
	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

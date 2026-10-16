// SPDX-License-Identifier: GPL-2.0
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define IDMA_IOC_MAGIC		'i'

#define IDMA_BENCH_F_HUGE	(1U << 0)
#define IDMA_BENCH_F_VERIFY	(1U << 1)
#define IDMA_BENCH_F_DMA_API	(1U << 2)
#define IDMA_BENCH_F_RANDOM	(1U << 3)

#define IDMA_PAIR_AGGRESSOR_NONE UINT32_MAX
#define MAX_CHUNKS 8

struct idma_bench_args {
	uint64_t size;
	uint32_t iterations;
	uint32_t flags;

	uint64_t bytes;
	uint64_t cycles;
	uint64_t iotlb_miss;
	uint64_t s1_ptw;
	uint64_t s2_ptw;
	uint64_t dma_active_cycles;
	uint32_t errors;
	uint32_t reserved;
};

struct idma_pair_bench_args {
	uint32_t aggressor_id;
	uint32_t xfer_chunk;

	struct idma_bench_args victim;
	struct idma_bench_args aggressor;
};

#define IDMA_IOC_RUN_PAIR_BENCH \
	_IOWR(IDMA_IOC_MAGIC, 0x02, struct idma_pair_bench_args)
#define IDMA_IOC_RUN_PAIR_BENCH_INDEPENDENT \
	_IOWR(IDMA_IOC_MAGIC, 0x05, struct idma_pair_bench_args)

enum pair_schedule {
	SCHEDULE_LOCKSTEP,
	SCHEDULE_INDEPENDENT,
};

enum baseline_kind {
	BASE_NONE,
	BASE_2M_LINEAR,
	BASE_4K_LINEAR,
	BASE_4K_RANDOM,
	BASE_COUNT,
};

struct suite_case {
	const char *name;
	uint32_t victim_flags;
	uint32_t aggressor_flags;
	int has_aggressor;
	enum baseline_kind baseline;
};

struct options {
	const char *victim0_dev;
	const char *victim1_dev;
	uint32_t victim0_aggressor_id;
	uint32_t victim1_aggressor_id;
	uint64_t size;
	uint32_t iterations;
	uint32_t repeats;
	uint32_t chunks[MAX_CHUNKS];
	uint32_t nr_chunks;
	int both_directions;
	int verify;
	int csv_header;
	int single;
	int victim_size_set;
	int aggressor_size_set;
	int victim_iters_set;
	int aggressor_iters_set;
	enum pair_schedule schedule;
	struct idma_pair_bench_args single_args;
};

static const struct suite_case suite_cases[] = {
	{
		.name = "solo_2m_linear",
		.victim_flags = IDMA_BENCH_F_HUGE,
		.has_aggressor = 0,
		.baseline = BASE_2M_LINEAR,
	},
	{
		.name = "solo_4k_linear",
		.victim_flags = 0,
		.has_aggressor = 0,
		.baseline = BASE_4K_LINEAR,
	},
	{
		.name = "solo_4k_random",
		.victim_flags = IDMA_BENCH_F_RANDOM,
		.has_aggressor = 0,
		.baseline = BASE_4K_RANDOM,
	},
	{
		.name = "pair_v2m_a2m_linear",
		.victim_flags = IDMA_BENCH_F_HUGE,
		.aggressor_flags = IDMA_BENCH_F_HUGE,
		.has_aggressor = 1,
		.baseline = BASE_2M_LINEAR,
	},
	{
		.name = "pair_v2m_a4k_linear",
		.victim_flags = IDMA_BENCH_F_HUGE,
		.aggressor_flags = 0,
		.has_aggressor = 1,
		.baseline = BASE_2M_LINEAR,
	},
	{
		.name = "pair_v2m_a4k_random",
		.victim_flags = IDMA_BENCH_F_HUGE,
		.aggressor_flags = IDMA_BENCH_F_RANDOM,
		.has_aggressor = 1,
		.baseline = BASE_2M_LINEAR,
	},
	{
		.name = "pair_v4krandom_a4krandom",
		.victim_flags = IDMA_BENCH_F_RANDOM,
		.aggressor_flags = IDMA_BENCH_F_RANDOM,
		.has_aggressor = 1,
		.baseline = BASE_4K_RANDOM,
	},
};

static void usage(const char *prog)
{
	fprintf(stderr,
		"Usage: %s [suite options]\n"
		"       %s --single [single-case options]\n"
		"\n"
		"Default suite:\n"
		"  Runs victim-alone baselines and paired victim/aggressor cases.\n"
		"  Directions: idma0 victim/idma1 aggressor and reverse.\n"
		"  Defaults: size=2m, iters=20, repeats=3, chunks=256,1k,4k,\n"
		"            schedule=independent, verify=on.\n"
		"\n"
		"Suite options:\n"
		"  --victim0-dev PATH          default /dev/idma0\n"
		"  --victim1-dev PATH          default /dev/idma1\n"
		"  --victim0-aggressor-id N    default 1\n"
		"  --victim1-aggressor-id N    default 0\n"
		"  -s, --size SIZE             default 2m\n"
		"  -i, --iters N               default 20\n"
		"  -r, --repeats N             default 3\n"
		"  --chunks LIST               comma list, default 256,1k,4k\n"
		"  --schedule SCHEDULE         independent (default) or lockstep\n"
		"  --one-direction             only run victim0/aggressor-id direction\n"
		"  --verify                    enable verification\n"
		"  --noverify                  disable verification\n"
		"  --no-header                 omit CSV header\n"
		"\n"
		"Single-case options:\n"
		"  -v, --victim-dev PATH       victim device path, default /dev/idma0\n"
		"  -a, --aggressor-id N        aggressor id, or none\n"
		"  --schedule SCHEDULE         independent (default) or lockstep\n"
		"  --chunk SIZE                default 4k\n"
		"  --victim-size SIZE          override victim size\n"
		"  --aggressor-size SIZE       override aggressor size\n"
		"  --victim-iters N            override victim iterations\n"
		"  --aggressor-iters N         override aggressor iterations\n"
		"  --victim-mode MODE          4k, 2m, or dma\n"
		"  --aggressor-mode MODE       4k, 2m, or dma\n"
		"  --victim-random|--victim-linear\n"
		"  --aggressor-random|--aggressor-linear\n",
		prog, prog);
}

static uint64_t parse_size(const char *s)
{
	char *end;
	uint64_t value = strtoull(s, &end, 0);

	if (!*end)
		return value;
	if (!strcmp(end, "k") || !strcmp(end, "K"))
		return value << 10;
	if (!strcmp(end, "m") || !strcmp(end, "M"))
		return value << 20;

	fprintf(stderr, "invalid size: %s\n", s);
	exit(1);
}

static uint32_t parse_u32(const char *s, const char *name)
{
	char *end;
	unsigned long value = strtoul(s, &end, 0);

	if (*end || value > UINT32_MAX) {
		fprintf(stderr, "invalid %s: %s\n", name, s);
		exit(1);
	}

	return (uint32_t)value;
}

static void add_chunk(struct options *opts, uint64_t chunk)
{
	if (!chunk || chunk > UINT32_MAX || opts->nr_chunks >= MAX_CHUNKS) {
		fprintf(stderr, "invalid chunk: %" PRIu64 "\n", chunk);
		exit(1);
	}

	opts->chunks[opts->nr_chunks++] = (uint32_t)chunk;
}

static void parse_chunks(struct options *opts, const char *arg)
{
	char *copy = malloc(strlen(arg) + 1);
	char *tok;

	if (!copy) {
		perror("malloc");
		exit(1);
	}

	strcpy(copy, arg);
	opts->nr_chunks = 0;
	for (tok = strtok(copy, ","); tok; tok = strtok(NULL, ","))
		add_chunk(opts, parse_size(tok));

	free(copy);

	if (!opts->nr_chunks) {
		fprintf(stderr, "empty chunk list\n");
		exit(1);
	}
}

static void set_mode(uint32_t *flags, const char *mode)
{
	*flags &= ~(IDMA_BENCH_F_HUGE | IDMA_BENCH_F_DMA_API);

	if (!strcmp(mode, "4k"))
		return;
	if (!strcmp(mode, "2m")) {
		*flags |= IDMA_BENCH_F_HUGE;
		return;
	}
	if (!strcmp(mode, "dma")) {
		*flags |= IDMA_BENCH_F_DMA_API;
		return;
	}

	fprintf(stderr, "invalid mode: %s\n", mode);
	exit(1);
}

static const char *mode_name(uint32_t flags)
{
	if (flags & IDMA_BENCH_F_DMA_API)
		return "dma";
	if (flags & IDMA_BENCH_F_HUGE)
		return "2m";
	return "4k";
}

static const char *pattern_name(uint32_t flags)
{
	return (flags & IDMA_BENCH_F_RANDOM) ? "random" : "linear";
}

static const char *schedule_name(enum pair_schedule schedule)
{
	return schedule == SCHEDULE_INDEPENDENT ? "independent" : "lockstep";
}

static double active_per_byte(const struct idma_bench_args *args)
{
	return args->bytes ?
		(double)args->dma_active_cycles / (double)args->bytes : 0.0;
}

static double cycles_per_byte(const struct idma_bench_args *args)
{
	return args->bytes ? (double)args->cycles / (double)args->bytes : 0.0;
}

static double active_utilization(const struct idma_bench_args *args)
{
	return args->cycles ?
		(double)args->dma_active_cycles / (double)args->cycles : 0.0;
}

static void init_options(struct options *opts)
{
	memset(opts, 0, sizeof(*opts));
	opts->victim0_dev = "/dev/idma0";
	opts->victim1_dev = "/dev/idma1";
	opts->victim0_aggressor_id = 1;
	opts->victim1_aggressor_id = 0;
	opts->size = 2ULL << 20;
	opts->iterations = 20;
	opts->repeats = 3;
	opts->both_directions = 1;
	opts->verify = 1;
	opts->csv_header = 1;
	opts->schedule = SCHEDULE_INDEPENDENT;
	add_chunk(opts, 256);
	add_chunk(opts, 1ULL << 10);
	add_chunk(opts, 4ULL << 10);

	opts->single_args.aggressor_id = 1;
	opts->single_args.xfer_chunk = 4ULL << 10;
	opts->single_args.victim.size = opts->size;
	opts->single_args.victim.iterations = opts->iterations;
	opts->single_args.victim.flags = IDMA_BENCH_F_HUGE | IDMA_BENCH_F_VERIFY;
	opts->single_args.aggressor.size = opts->size;
	opts->single_args.aggressor.iterations = opts->iterations;
	opts->single_args.aggressor.flags = IDMA_BENCH_F_RANDOM | IDMA_BENCH_F_VERIFY;
}

static void apply_verify(struct idma_pair_bench_args *args, int verify)
{
	if (verify) {
		args->victim.flags |= IDMA_BENCH_F_VERIFY;
		args->aggressor.flags |= IDMA_BENCH_F_VERIFY;
	} else {
		args->victim.flags &= ~IDMA_BENCH_F_VERIFY;
		args->aggressor.flags &= ~IDMA_BENCH_F_VERIFY;
	}
}

static void print_header(void)
{
	puts("repeat,direction,case,schedule,role,victim_dev,aggressor_id,chunk,size,"
	     "mode,pattern,aggressor_mode,aggressor_pattern,iterations,bytes,"
	     "shared_cycles,shared_cycles_per_role_byte,dma_active_cycles,"
	     "dma_active_cycles_per_byte,dma_active_utilization,"
	     "shared_iotlb_miss,shared_s1_ptw,shared_s2_ptw,errors,"
	     "victim_active_slowdown");
}

static void print_row(uint32_t repeat, const char *direction,
		      const char *case_name, const char *schedule,
		      const char *role,
		      const char *victim_dev, uint32_t aggressor_id,
		      uint32_t chunk, const struct idma_bench_args *args,
		      const struct idma_bench_args *aggressor_args,
		      double slowdown)
{
	const char *aggr_id = aggressor_id == IDMA_PAIR_AGGRESSOR_NONE ?
		"none" : "";

	printf("%u,%s,%s,%s,%s,%s,", repeat, direction, case_name, schedule,
	       role, victim_dev);
	if (*aggr_id)
		printf("%s,", aggr_id);
	else
		printf("%u,", aggressor_id);

	printf("%u,%" PRIu64 ",%s,%s,%s,%s,%u,%" PRIu64
	       ",%" PRIu64 ",%.6f,%" PRIu64 ",%.6f,%.6f,%" PRIu64
	       ",%" PRIu64 ",%" PRIu64 ",%u,%.6f\n",
	       chunk, args->size, mode_name(args->flags), pattern_name(args->flags),
	       aggressor_args ? mode_name(aggressor_args->flags) : "none",
	       aggressor_args ? pattern_name(aggressor_args->flags) : "none",
	       args->iterations, args->bytes, args->cycles, cycles_per_byte(args),
	       args->dma_active_cycles, active_per_byte(args),
	       active_utilization(args),
	       args->iotlb_miss, args->s1_ptw, args->s2_ptw, args->errors,
	       slowdown);
}

static int run_ioctl(const char *victim_dev, struct idma_pair_bench_args *args,
		     enum pair_schedule schedule)
{
	unsigned long request = schedule == SCHEDULE_INDEPENDENT ?
		IDMA_IOC_RUN_PAIR_BENCH_INDEPENDENT :
		IDMA_IOC_RUN_PAIR_BENCH;
	int fd;
	int ret;

	fd = open(victim_dev, O_RDWR);
	if (fd < 0) {
		perror(victim_dev);
		return -1;
	}

	ret = ioctl(fd, request, args);
	if (ret < 0)
		fprintf(stderr, "%s pair ioctl failed on %s: %s\n",
			schedule_name(schedule), victim_dev, strerror(errno));

	close(fd);
	return ret;
}

static struct idma_pair_bench_args make_case_args(const struct options *opts,
						  const struct suite_case *tc,
						  uint32_t aggressor_id,
						  uint32_t chunk)
{
	struct idma_pair_bench_args args;

	memset(&args, 0, sizeof(args));
	args.aggressor_id = tc->has_aggressor ? aggressor_id :
			    IDMA_PAIR_AGGRESSOR_NONE;
	args.xfer_chunk = chunk;
	args.victim.size = opts->size;
	args.victim.iterations = opts->iterations;
	args.victim.flags = tc->victim_flags;
	args.aggressor.size = opts->size;
	args.aggressor.iterations = opts->iterations;
	args.aggressor.flags = tc->aggressor_flags;
	apply_verify(&args, opts->verify);

	return args;
}

static int run_suite_direction(const struct options *opts, const char *direction,
			       const char *victim_dev, uint32_t aggressor_id)
{
	double baselines[BASE_COUNT];
	size_t nr_cases = sizeof(suite_cases) / sizeof(suite_cases[0]);
	int failures = 0;
	uint32_t r, c;

	for (c = 0; c < opts->nr_chunks; c++) {
		uint32_t chunk = opts->chunks[c];

		for (r = 1; r <= opts->repeats; r++) {
			size_t i;

			memset(baselines, 0, sizeof(baselines));
			for (i = 0; i < nr_cases; i++) {
				const struct suite_case *tc = &suite_cases[i];
				struct idma_pair_bench_args args;
				const struct idma_bench_args *aggr = NULL;
				const char *schedule = "solo";
				double baseline = 0.0;
				double slowdown = 0.0;

				args = make_case_args(opts, tc, aggressor_id, chunk);
				if (tc->has_aggressor)
					schedule = schedule_name(opts->schedule);
				if (run_ioctl(victim_dev, &args,
					      tc->has_aggressor ? opts->schedule :
					      SCHEDULE_LOCKSTEP) < 0) {
					failures++;
					continue;
				}

				if (!tc->has_aggressor && tc->baseline != BASE_NONE)
					baselines[tc->baseline] =
						active_per_byte(&args.victim);

				if (tc->baseline != BASE_NONE)
					baseline = baselines[tc->baseline];
				if (baseline > 0.0)
					slowdown = active_per_byte(&args.victim) /
						   baseline;

				if (tc->has_aggressor)
					aggr = &args.aggressor;

				print_row(r, direction, tc->name, schedule,
					  "victim",
					  victim_dev, args.aggressor_id, chunk,
					  &args.victim, aggr, slowdown);
				if (tc->has_aggressor)
					print_row(r, direction, tc->name,
						  schedule, "aggressor",
						  victim_dev,
						  args.aggressor_id, chunk,
						  &args.aggressor, aggr, 0.0);
			}
		}
	}

	return failures ? 1 : 0;
}

static int run_suite(const struct options *opts)
{
	int ret = 0;

	if (opts->csv_header)
		print_header();

	ret |= run_suite_direction(opts, "idma0_to_idma1", opts->victim0_dev,
				   opts->victim0_aggressor_id);
	if (opts->both_directions)
		ret |= run_suite_direction(opts, "idma1_to_idma0",
					   opts->victim1_dev,
					   opts->victim1_aggressor_id);

	return ret;
}

static int run_single(const struct options *opts, const char *victim_dev)
{
	struct idma_pair_bench_args args = opts->single_args;
	const struct idma_bench_args *aggr = NULL;
	enum pair_schedule schedule = args.aggressor_id ==
		IDMA_PAIR_AGGRESSOR_NONE ? SCHEDULE_LOCKSTEP : opts->schedule;
	const char *schedule_label = args.aggressor_id ==
		IDMA_PAIR_AGGRESSOR_NONE ? "solo" : schedule_name(schedule);

	apply_verify(&args, opts->verify);
	if (run_ioctl(victim_dev, &args, schedule) < 0)
		return 1;

	if (opts->csv_header)
		print_header();
	if (args.aggressor_id != IDMA_PAIR_AGGRESSOR_NONE)
		aggr = &args.aggressor;
	print_row(1, "single", "single", schedule_label, "victim", victim_dev,
		  args.aggressor_id, args.xfer_chunk, &args.victim, aggr, 0.0);
	if (aggr)
		print_row(1, "single", "single", schedule_label, "aggressor",
			  victim_dev, args.aggressor_id, args.xfer_chunk,
			  &args.aggressor, aggr, 0.0);

	return args.victim.errors || args.aggressor.errors;
}

int main(int argc, char **argv)
{
	struct options opts;
	const char *single_victim_dev = "/dev/idma0";
	int i;

	init_options(&opts);

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			usage(argv[0]);
			return 0;
		} else if (!strcmp(argv[i], "--single")) {
			opts.single = 1;
		} else if (!strcmp(argv[i], "--victim0-dev")) {
			if (++i >= argc)
				goto missing_arg;
			opts.victim0_dev = argv[i];
		} else if (!strcmp(argv[i], "--victim1-dev")) {
			if (++i >= argc)
				goto missing_arg;
			opts.victim1_dev = argv[i];
		} else if (!strcmp(argv[i], "--victim0-aggressor-id")) {
			if (++i >= argc)
				goto missing_arg;
			opts.victim0_aggressor_id = parse_u32(argv[i], argv[i - 1]);
		} else if (!strcmp(argv[i], "--victim1-aggressor-id")) {
			if (++i >= argc)
				goto missing_arg;
			opts.victim1_aggressor_id = parse_u32(argv[i], argv[i - 1]);
		} else if (!strcmp(argv[i], "-v") ||
			   !strcmp(argv[i], "--victim-dev")) {
			if (++i >= argc)
				goto missing_arg;
			single_victim_dev = argv[i];
		} else if (!strcmp(argv[i], "-a") ||
			   !strcmp(argv[i], "--aggressor-id")) {
			if (++i >= argc)
				goto missing_arg;
			if (!strcmp(argv[i], "none"))
				opts.single_args.aggressor_id =
					IDMA_PAIR_AGGRESSOR_NONE;
			else
				opts.single_args.aggressor_id =
					parse_u32(argv[i], "aggressor id");
		} else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--size")) {
			if (++i >= argc)
				goto missing_arg;
			opts.size = parse_size(argv[i]);
			if (!opts.victim_size_set)
				opts.single_args.victim.size = opts.size;
			if (!opts.aggressor_size_set)
				opts.single_args.aggressor.size = opts.size;
		} else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--iters")) {
			if (++i >= argc)
				goto missing_arg;
			opts.iterations = parse_u32(argv[i], "iterations");
			if (!opts.victim_iters_set)
				opts.single_args.victim.iterations = opts.iterations;
			if (!opts.aggressor_iters_set)
				opts.single_args.aggressor.iterations = opts.iterations;
		} else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--repeats")) {
			if (++i >= argc)
				goto missing_arg;
			opts.repeats = parse_u32(argv[i], "repeats");
		} else if (!strcmp(argv[i], "--chunks")) {
			if (++i >= argc)
				goto missing_arg;
			parse_chunks(&opts, argv[i]);
		} else if (!strcmp(argv[i], "--schedule")) {
			if (++i >= argc)
				goto missing_arg;
			if (!strcmp(argv[i], "independent"))
				opts.schedule = SCHEDULE_INDEPENDENT;
			else if (!strcmp(argv[i], "lockstep"))
				opts.schedule = SCHEDULE_LOCKSTEP;
			else {
				fprintf(stderr, "invalid schedule: %s\n", argv[i]);
				return 1;
			}
		} else if (!strcmp(argv[i], "--chunk")) {
			uint64_t chunk;

			if (++i >= argc)
				goto missing_arg;
			chunk = parse_size(argv[i]);
			if (chunk > UINT32_MAX) {
				fprintf(stderr, "invalid chunk: %s\n", argv[i]);
				return 1;
			}
			opts.single_args.xfer_chunk = (uint32_t)chunk;
		} else if (!strcmp(argv[i], "--victim-size")) {
			if (++i >= argc)
				goto missing_arg;
			opts.single_args.victim.size = parse_size(argv[i]);
			opts.victim_size_set = 1;
		} else if (!strcmp(argv[i], "--aggressor-size")) {
			if (++i >= argc)
				goto missing_arg;
			opts.single_args.aggressor.size = parse_size(argv[i]);
			opts.aggressor_size_set = 1;
		} else if (!strcmp(argv[i], "--victim-iters")) {
			if (++i >= argc)
				goto missing_arg;
			opts.single_args.victim.iterations =
				parse_u32(argv[i], "victim iterations");
			opts.victim_iters_set = 1;
		} else if (!strcmp(argv[i], "--aggressor-iters")) {
			if (++i >= argc)
				goto missing_arg;
			opts.single_args.aggressor.iterations =
				parse_u32(argv[i], "aggressor iterations");
			opts.aggressor_iters_set = 1;
		} else if (!strcmp(argv[i], "--one-direction")) {
			opts.both_directions = 0;
		} else if (!strcmp(argv[i], "--verify")) {
			opts.verify = 1;
		} else if (!strcmp(argv[i], "--noverify")) {
			opts.verify = 0;
		} else if (!strcmp(argv[i], "--no-header")) {
			opts.csv_header = 0;
		} else if (!strcmp(argv[i], "--victim-mode")) {
			if (++i >= argc)
				goto missing_arg;
			set_mode(&opts.single_args.victim.flags, argv[i]);
		} else if (!strcmp(argv[i], "--aggressor-mode")) {
			if (++i >= argc)
				goto missing_arg;
			set_mode(&opts.single_args.aggressor.flags, argv[i]);
		} else if (!strcmp(argv[i], "--victim-random")) {
			opts.single_args.victim.flags |= IDMA_BENCH_F_RANDOM;
		} else if (!strcmp(argv[i], "--victim-linear")) {
			opts.single_args.victim.flags &= ~IDMA_BENCH_F_RANDOM;
		} else if (!strcmp(argv[i], "--aggressor-random")) {
			opts.single_args.aggressor.flags |= IDMA_BENCH_F_RANDOM;
		} else if (!strcmp(argv[i], "--aggressor-linear")) {
			opts.single_args.aggressor.flags &= ~IDMA_BENCH_F_RANDOM;
		} else {
			fprintf(stderr, "unknown option: %s\n", argv[i]);
			usage(argv[0]);
			return 1;
		}
	}

	if (!opts.repeats) {
		fprintf(stderr, "repeats must be non-zero\n");
		return 1;
	}

	return opts.single ? run_single(&opts, single_victim_dev) :
			     run_suite(&opts);

missing_arg:
	fprintf(stderr, "%s requires an argument\n", argv[i]);
	return 1;
}

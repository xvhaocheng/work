/* Host fixture only: real extracted driver functions, mocked frozen MMIO. */
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/linux/idma.h"
#include "include/linux/idma_multi.h"
typedef unsigned long long u64;
typedef unsigned int u32;
#define __iomem
#define __user
#define noinline __attribute__((noinline))
#define READ_ONCE(value) (value)
#define U32_MAX UINT32_MAX
#define PAGE_SIZE 4096U
#define SZ_4K 4096U
struct idma_device { unsigned id, did; void *reg; struct { void *parent; } miscdev; };
static unsigned char registers[256];
static struct idma_device device={.id=0,.did=10,.reg=registers};
static u64 magic, control, index_value, values[35], paths[16][8], histogram[16][32], hist_info;
static unsigned hist_reads, info_reads, log_lines, hist_lines, invalid_batch, assertions;
static char log_buffer[128*1024];
static size_t log_used;
static struct idma_device *idma_get_instance(unsigned id) { return id==0 ? &device : NULL; }
static bool idma_batch_supported(struct idma_device *d,bool unused) { (void)d;(void)unused;return true; }
static bool riscv_iommu_hpm_supported(void *unused) { (void)unused;return true; }
static int copy_to_user(void *to,const void *from,size_t n) { memcpy(to,from,n);return 0; }
static int scnprintf(char *buffer,size_t size,const char *fmt,...) {
    va_list ap;va_start(ap,fmt);int result=vsnprintf(buffer,size,fmt,ap);va_end(ap);
    return size ? (result<(int)size ? result : (int)size-1) : 0;
}
static void host_log(const char *fmt,...) {
    char text[1024];va_list ap;va_start(ap,fmt);int n=vsnprintf(text,sizeof(text),fmt,ap);va_end(ap);
    assert(n>=0 && n<(int)sizeof(text));assert(log_used+(size_t)n<sizeof(log_buffer));
    memcpy(log_buffer+log_used,text,n);log_used+=n;log_buffer[log_used]=0;++log_lines;
    if(strstr(text,"idma_path_hist "))++hist_lines;
    if(strstr(text,"part=time valid=0"))++invalid_batch;
}
#define dev_info(device_pointer,...) host_log(__VA_ARGS__)
static u64 readq(const void *address) {
    uintptr_t offset=(uintptr_t)address-(uintptr_t)registers;
    if(offset==0x58)return magic;
    if(offset==0x60)return control;
    assert(offset==0x78);
    if(index_value<35)return values[index_value];
    if(index_value>=64 && index_value<192)return paths[(index_value-64)/8][(index_value-64)%8];
    if(index_value==192){++info_reads;return hist_info;}
    if(index_value>=256 && index_value<768){++hist_reads;return histogram[(index_value-256)/32][(index_value-256)%32];}
    abort();
}
static void writeq(u64 value,void *address) {
    uintptr_t offset=(uintptr_t)address-(uintptr_t)registers;
    if(offset==0x70){index_value=value;return;}
    if(offset==0x60){if(value==4)control=0;else if(value==2)control=4;else control=1;return;}
    assert(offset==0x68);
}
#define IDMA_OBS_MAGIC_OFFSET 0x58
#define IDMA_OBS_CONTROL_OFFSET 0x60
#define IDMA_OBS_TARGET_OFFSET 0x68
#define IDMA_OBS_INDEX_OFFSET 0x70
#define IDMA_OBS_DATA_OFFSET 0x78
#define IDMA_OBS_MAGIC 0x49444d414f425301ULL
#define IDMA_OBS_MAGIC_V2 0x49444d414f425302ULL
#define IDMA_OBS_MAGIC_V3 0x49444d414f425303ULL
#define IDMA_OBS_MAGIC_V4 0x49444d414f425304ULL
#define IDMA_OBS_VALUES 34
#define IDMA_MAX_RUN_TIMEOUT_MS 300000U
struct idma_observation {
	struct idma_device *control;
	u64 sequence;
	bool armed, batch, path, hist;
};
static u64 idma_observation_sequence;
static bool idma_path_observe=true;
static bool idma_observer_has_path(u64 magic)
{
	return magic == IDMA_OBS_MAGIC_V3 || magic == IDMA_OBS_MAGIC_V4;
}

static int idma_observation_prepare(struct idma_observation *o, bool enabled)
{
	u64 magic;

	if (!enabled)
		return 0;
	o->sequence = ++idma_observation_sequence;
	o->control = idma_get_instance(0);
	if (!o->control)
		return -ENODEV;
	magic = readq(o->control->reg + IDMA_OBS_MAGIC_OFFSET);
	o->path = o->batch && READ_ONCE(idma_path_observe);
	o->hist = o->path && magic == IDMA_OBS_MAGIC_V4;
	if (o->control->did != 10 ||
	    (!idma_observer_has_path(magic) &&
	     (o->path || (magic != IDMA_OBS_MAGIC_V2 && (o->batch || magic != IDMA_OBS_MAGIC)))))
		return -EOPNOTSUPP;
	/* Clear previous frozen results; never take ownership of an active session. */
	if (readq(o->control->reg + IDMA_OBS_CONTROL_OFFSET) & 3)
		return -EBUSY;
	writeq(4, o->control->reg + IDMA_OBS_CONTROL_OFFSET);
	return readq(o->control->reg + IDMA_OBS_CONTROL_OFFSET) ? -EIO : 0;
}

static noinline bool idma_observation_hist_finish(struct idma_observation *o,
						void __iomem *base,
						const u64 path[16][8], bool valid)
{
	char counts[32 * 21]; /* Also retain corrupt 64-bit MMIO values in invalid logs. */
	u64 info;
	u32 overflow;
	int i, bin;

	writeq(192, base + IDMA_OBS_INDEX_OFFSET);
	info = readq(base + IDMA_OBS_DATA_OFFSET);
	overflow = (info >> 32) & 0xffff;
	valid = valid && (info & 0xffffffffULL) == 0x20202001ULL &&
		!(info >> 48) && !overflow;
	dev_info(o->control->miscdev.parent,
		 "idma_hist_info seq=%llu valid=%u version=4 schema=%u bins=%u count_bits=%u time_bits=%u overflow=%#x raw=%#llx\n",
		 o->sequence, valid, (u32)(info & 0xff), (u32)((info >> 8) & 0xff),
		 (u32)((info >> 16) & 0xff), (u32)((info >> 24) & 0xff), overflow, info);
	for (i = 0; i < 16; i++) {
		u64 total = 0;
		size_t used = 0;
		bool stage_valid = valid;

		for (bin = 0; bin < 32; bin++) {
			u64 value;

			writeq(256 + i * 32 + bin, base + IDMA_OBS_INDEX_OFFSET);
			value = readq(base + IDMA_OBS_DATA_OFFSET);
			if (value > U32_MAX)
				stage_valid = false;
			else
				total += value;
			used += scnprintf(counts + used, sizeof(counts) - used,
					  "%s%llu", bin ? "," : "", value);
		}
		stage_valid = stage_valid && total == path[i][1];
		dev_info(o->control->miscdev.parent,
			 "idma_path_hist seq=%llu role=%c channel=%c stage=%u valid=%u schema=1 bins=32 full=%llu total=%llu overflow=%u counts=%s\n",
			 o->sequence, i < 8 ? 'v' : 'a', (i / 4) % 2 ? 'w' : 'r',
			 i % 4, stage_valid, path[i][1], total, (overflow >> i) & 1, counts);
		valid = valid && stage_valid;
	}
	return valid;
}

static int idma_observation_finish(struct idma_observation *o,
				   const struct idma_experiment *e, int run_error)
{
	u64 values[IDMA_OBS_VALUES], path[16][8], status;
	void __iomem *base;
	bool valid;
	int i;

	if (!o->control || !o->armed)
		return 0;
	base = o->control->reg;
	status = readq(base + IDMA_OBS_CONTROL_OFFSET);
	if (status & 3) {
		writeq(2, base + IDMA_OBS_CONTROL_OFFSET);
		status = readq(base + IDMA_OBS_CONTROL_OFFSET);
	}
	for (i = 0; i < IDMA_OBS_VALUES; i++) {
		writeq(i, base + IDMA_OBS_INDEX_OFFSET);
		values[i] = readq(base + IDMA_OBS_DATA_OFFSET);
	}
	valid = !run_error && status == 4 &&
		values[7] == e->victim_transfers && values[8] == values[7] &&
		values[9] == (o->batch ? 1 : values[7]) &&
		values[0] == values[1] + values[2] + values[3] &&
		values[0] == values[10] + values[11] + values[12] + values[13];
	/* Opening stock + accepted requests must equal returns + closing stock. */
	for (i = 0; i < 2; i++) {
		u64 opening = values[30 + i], closing = values[32 + i];

		if ((opening >> 16) + values[14 + 6 * i] !=
		    (closing >> 16) + values[16 + 6 * i] ||
		    (opening & 0xffff) + values[15 + 6 * i] !=
		    (closing & 0xffff) + values[17 + 6 * i])
			valid = false;
	}
	if (o->path) {
		int lane, stage, field;

		for (i = 0; i < 16; i++)
			for (field = 0; field < 8; field++) {
				writeq(64 + i * 8 + field, base + IDMA_OBS_INDEX_OFFSET);
				path[i][field] = readq(base + IDMA_OBS_DATA_OFFSET);
			}
		for (lane = 0; lane < 4; lane++) {
			for (stage = 0; stage < 4; stage++) {
				u64 *p = path[lane * 4 + stage];

				/* Opening + entered = completed + closing, including carry-in. */
				if (p[5] + p[0] != p[1] + p[2] + p[6] ||
				    p[4] > p[3] || (!p[1] && (p[3] || p[4])) ||
				    (stage < 3 && p[1] + p[2] != path[lane * 4 + stage + 1][0]))
					valid = false;
				/* All V transactions must be wholly inside the batch window. */
				if (lane < 2 && (p[5] || p[6] || p[2] ||
				    p[1] != values[14 + lane] || p[7] != p[3]))
					valid = false;
				if (lane >= 2 && e->aggressor_did == IDMA_EXPERIMENT_NO_AGGRESSOR)
					for (field = 0; field < 8; field++)
						if (p[field])
							valid = false;
			}
		}
		if (!values[14] || !values[15])
			valid = false;
		if (o->hist)
			valid = idma_observation_hist_finish(o, base, path, valid);
	}
	if (o->batch) {
		u64 read_delay;

		writeq(34, base + IDMA_OBS_INDEX_OFFSET);
		read_delay = readq(base + IDMA_OBS_DATA_OFFSET);
		valid = valid && values[2] == 0;
		dev_info(o->control->miscdev.parent,
			 "idma_batch_obs seq=%llu part=time valid=%u status=%#llx v_did=%u a_did=%u cycles=%llu h=%llu gap=%llu hmax=%llu gapmax=%llu issued=%llu done=%llu batch_polled=%llu done_to_read=%llu\n",
			 o->sequence, valid, status, e->victim_did, e->aggressor_did,
			 values[0], values[1], values[3], values[4], values[6],
			 values[7], values[8], values[9], read_delay);
	} else {
		dev_info(o->control->miscdev.parent,
			 "idma_obs seq=%llu part=time valid=%u status=%#llx v_did=%u a_did=%u cycles=%llu h=%llu p=%llu r=%llu hmax=%llu pmax=%llu rmax=%llu issued=%llu done=%llu polled=%llu\n",
			 o->sequence, valid, status, e->victim_did, e->aggressor_did,
			 values[0], values[1], values[2], values[3], values[4], values[5],
			 values[6], values[7], values[8], values[9]);
	}
	dev_info(o->control->miscdev.parent,
		 "%s seq=%llu part=axi_v neither=%llu v_only=%llu a_only=%llu both=%llu ar=%llu aw=%llu rlast=%llu b=%llu ar_wait=%llu aw_wait=%llu start=%#llx end=%#llx\n",
		 (o->batch ? "idma_batch_obs" : "idma_obs"), o->sequence,
		 values[10], values[11], values[12], values[13],
		 values[14], values[15], values[16], values[17], values[18],
		 values[19], values[30], values[32]);
	dev_info(o->control->miscdev.parent,
		 "%s seq=%llu part=axi_a ar=%llu aw=%llu rlast=%llu b=%llu ar_wait=%llu aw_wait=%llu start=%#llx end=%#llx shared_ar=%llu shared_aw=%llu shared_ar_wait=%llu shared_aw_wait=%llu\n",
		 (o->batch ? "idma_batch_obs" : "idma_obs"), o->sequence,
		 values[20], values[21], values[22], values[23],
		 values[24], values[25], values[31], values[33], values[26],
		 values[27], values[28], values[29]);
	if (o->path)
		for (i = 0; i < 16; i++)
			dev_info(o->control->miscdev.parent,
				 "idma_path_obs seq=%llu role=%c channel=%c stage=%u valid=%u entered=%llu full=%llu carry=%llu sum=%llu max=%llu start=%llu end=%llu area=%llu\n",
				 o->sequence, i < 8 ? 'v' : 'a', (i / 4) % 2 ? 'w' : 'r',
				 i % 4, valid, path[i][0], path[i][1], path[i][2],
				 path[i][3], path[i][4], path[i][5], path[i][6], path[i][7]);
	return valid ? 0 : -EIO;
}

static long idma_caps_multi(struct idma_device *victim, void __user *uarg)
{
	struct idma_multi_caps caps = {
		.abi_version = IDMA_MULTI_ABI,
		.struct_size = sizeof(caps),
		.device_id = victim->id,
		.did = victim->did,
		.max_devices = IDMA_MULTI_MAX_DEVICES,
		.max_pages = IDMA_MULTI_MAX_PAGES,
		.page_bytes = PAGE_SIZE,
		.max_run_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS,
	};

	if (victim->id != 0 || victim->did != 10 || PAGE_SIZE != SZ_4K)
		return -EOPNOTSUPP;
	if (idma_batch_supported(victim, false))
		caps.capabilities |= IDMA_MULTI_CAP_HW_BATCH;
	if (riscv_iommu_hpm_supported(victim->miscdev.parent))
		caps.capabilities |= IDMA_MULTI_CAP_ALL_DID_HPM;
	if (idma_observer_has_path(readq(victim->reg + IDMA_OBS_MAGIC_OFFSET)))
		caps.capabilities |= IDMA_MULTI_CAP_SELECTED_PATH;
	return copy_to_user(uarg, &caps, sizeof(caps)) ? -EFAULT : 0;
}
static void fixture(u64 version) {
    magic=version;control=4;index_value=0;device.id=0;device.did=10;idma_path_observe=true;
    hist_reads=info_reads=log_lines=hist_lines=invalid_batch=0;log_used=0;log_buffer[0]=0;
    memset(values,0,sizeof(values));memset(paths,0,sizeof(paths));memset(histogram,0,sizeof(histogram));
    values[0]=values[1]=values[11]=10;values[7]=values[8]=values[9]=1;
    values[14]=values[15]=values[16]=values[17]=2;
    for(unsigned lane=0;lane<2;lane++)for(unsigned stage=0;stage<4;stage++) {
        u64 *p=paths[lane*4+stage];p[0]=p[1]=2;p[3]=p[7]=4;p[4]=2;
        histogram[lane*4+stage][2]=2;
    }
    hist_info=0x20202001ULL;
}
static void check(bool condition,const char *label) {
    ++assertions;if(!condition){fprintf(stderr,"FAIL check=%u %s\n",assertions,label);abort();}
    printf("CHECK %u %s\n",assertions,label);
}
static struct idma_observation observation(bool batch) {
    struct idma_observation o={.batch=batch};
    return o;
}
static int finish(struct idma_observation *o,int error) {
    struct idma_experiment e={.victim_transfers=1,.victim_did=10,.aggressor_did=IDMA_EXPERIMENT_NO_AGGRESSOR};
    o->control=&device;o->armed=true;control=4;return idma_observation_finish(o,&e,error);
}
int main(void) {
    puts("FIXTURE_ONLY: actual driver functions with mocked frozen MMIO; no board measurements");
    for(unsigned version=1;version<=4;version++) {
        fixture(IDMA_OBS_MAGIC+version-1);struct idma_observation o=observation(true);
        int rc=idma_observation_prepare(&o,true);
        check(rc==(version>=3?0:-EOPNOTSUPP),"batch requires path-capable v3 or v4");
        if(version>=3) {
            check(o.path && o.hist==(version==4),"prepare records actual histogram capability");
            check(finish(&o,0)==0,"valid frozen batch remains valid");
            check(hist_reads==(version==4?512:0) && info_reads==(version==4?1:0),"bounded one-pass MMIO reads; v3 reads no histogram");
            check(hist_lines==(version==4?16:0),"all 16 histogram lines or legacy no lines");
            if(version==4) { puts("BEGIN_VALID_V4_FIXTURE_LOG");fputs(log_buffer,stdout);puts("END_VALID_V4_FIXTURE_LOG"); }
        }
        struct idma_multi_caps caps={};control=4;
        check(!idma_caps_multi(&device,&caps),"multi capability query succeeds");
        check(!!(caps.capabilities&IDMA_MULTI_CAP_SELECTED_PATH)==(version>=3),"multi path capability accepts v3 and v4 consistently");
    }
    fixture(IDMA_OBS_MAGIC_V2);idma_path_observe=false;struct idma_observation o=observation(true);
    check(!idma_observation_prepare(&o,true) && !o.path && !o.hist,"old v2 batch without path remains supported");
    fixture(IDMA_OBS_MAGIC);o=observation(false);
    check(!idma_observation_prepare(&o,true) && !o.path && !o.hist,"old v1 nonbatch remains supported");
    fixture(0);o=observation(true);check(idma_observation_prepare(&o,true)==-EOPNOTSUPP,"unknown magic rejects");
    fixture(IDMA_OBS_MAGIC_V4);device.did=11;o=observation(true);
    check(idma_observation_prepare(&o,true)==-EOPNOTSUPP,"wrong control DID rejects");
    fixture(IDMA_OBS_MAGIC_V4);control=1;o=observation(true);
    check(idma_observation_prepare(&o,true)==-EBUSY,"active observation cannot be acquired");
    fixture(IDMA_OBS_MAGIC_V4);o=observation(true);check(!idma_observation_prepare(&o,false)&&!o.control,"disabled prepare does no MMIO ownership");
    for(unsigned fault=0;fault<7;fault++) {
        fixture(IDMA_OBS_MAGIC_V4);o=observation(true);check(!idma_observation_prepare(&o,true),"prepare negative finish fixture");
        switch(fault) {
        case 0:histogram[15][3]=1;break; /* absent A must have zero full/bins */
        case 1:histogram[3][2]=1;break; /* missing count */
        case 2:hist_info|=1ULL<<32;break; /* overflow */
        case 3:hist_info^=3;break; /* schema mismatch */
        case 4:hist_info|=1ULL<<63;break; /* reserved field */
        case 5:histogram[0][0]=~0ULL;break; /* retain out-of-range original value */
        case 6:break; /* pre-existing run error invalidates every observation */
        }
        check(finish(&o,fault==6?-EILSEQ:0)==-EIO,"invalid histogram or run returns EIO");
        check(invalid_batch==1 && hist_lines==16 && hist_reads==512,"overall batch validity rejects partial or corrupt histogram");
        if(fault==5)check(strstr(log_buffer,"18446744073709551615")!=NULL,"invalid 64-bit MMIO value preserved in raw log");
    }
    printf("PASS actual driver source: %u host fixture assertions\n",assertions);
    return 0;
}

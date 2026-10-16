#ifndef NETWORK_INTERRUPTS_H
#define NETWORK_INTERRUPTS_H

#define SIMICS30
#include <stdint.h>
#include <map>
#include <set>
#include <queue>
#include <vector>
//#include <simics/api.h>
//#include <simics/alloc.h>
//#include <simics/utils.h>
#define SIM_ENHANCE
#define SIM_NET_PORTS
#include "../../mem/ruby/system/SimicsNetworkPortInterface.hh"
#include "../Common/BaseCallbacks.hh"
#include "../../cpu/thread_context.hh"
#include "cpu/translation.hh"
#include "../Common/mf_api.hh"
#include "arch/x86/pagetable.hh"
#include "base/trie.hh"
#include <cstdlib>  // For srand() and rand()
#include <ctime>    // For time()

typedef struct NetworkInterruptHandle_t {
  SimicsNetworkPortInterface* snpi;
  int portID;
  int deviceID;
  ThreadContext *attachedCPU;
  int procID;
} NetworkInterruptHandle;

#define PAGE_BYTES 4096 // Example value, replace with actual value
#define CHUNK_SIZE 64   // SHA-256 chunk size
#define TOTAL_LEN_LEN 8

// Define constants and state structure for SHA-256
static const uint32_t k[] = {
    // SHA-256 constants (first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311)
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};


struct buffer_state {
    const uint8_t* p;
    size_t len;
    size_t total_len;
    int single_one_delivered; /* bool */
    int total_len_delivered;  /* bool */
};

// SHA-256 helper functions
static inline uint32_t right_rot(uint32_t value, unsigned int count)
{
    return value >> count | value << (32 - count);
}

static void init_buf_state(struct buffer_state * state, const void * input, size_t len)
{
	state->p = (const uint8_t*)input;
	state->len = len;
	state->total_len = len;
	state->single_one_delivered = 0;
	state->total_len_delivered = 0;
}

/* Return value: bool */
static int calc_chunk(uint8_t chunk[CHUNK_SIZE], struct buffer_state * state)
{
	size_t space_in_chunk;

	if (state->total_len_delivered) {
		return 0;
	}

	if (state->len >= CHUNK_SIZE) {
		memcpy(chunk, state->p, CHUNK_SIZE);
		state->p += CHUNK_SIZE;
		state->len -= CHUNK_SIZE;
		return 1;
	}

	memcpy(chunk, state->p, state->len);
	chunk += state->len;
	space_in_chunk = CHUNK_SIZE - state->len;
	state->p += state->len;
	state->len = 0;

	/* If we are here, space_in_chunk is one at minimum. */
	if (!state->single_one_delivered) {
		*chunk++ = 0x80;
		space_in_chunk -= 1;
		state->single_one_delivered = 1;
	}

	/*
	 * Now:
	 * - either there is enough space left for the total length, and we can conclude,
	 * - or there is too little space left, and we have to pad the rest of this chunk with zeroes.
	 * In the latter case, we will conclude at the next invokation of this function.
	 */
	if (space_in_chunk >= TOTAL_LEN_LEN) {
		const size_t left = space_in_chunk - TOTAL_LEN_LEN;
		size_t len = state->total_len;
		int i;
		memset(chunk, 0x00, left);
		chunk += left;

		/* Storing of len * 8 as a big endian 64-bit without overflow. */
		chunk[7] = (uint8_t) (len << 3);
		len >>= 5;
		for (i = 6; i >= 0; i--) {
			chunk[i] = (uint8_t) len;
			len >>= 8;
		}
		state->total_len_delivered = 1;
	} else {
		memset(chunk, 0x00, space_in_chunk);
	}

	return 1;
}
// SHA-256 calculation function
static void calc_sha_256(uint8_t hash[32], const void* input, size_t len)
{
    // Implementation of SHA-256 (as in the original code)
    // Initialize hash values (first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19)
    uint32_t h[] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
    int i, j;

    uint8_t chunk[64];
    struct buffer_state state;

    init_buf_state(&state, input, len);

    while (calc_chunk(chunk, &state)) {
        uint32_t ah[8];
        uint32_t w[64];
        const uint8_t* p = chunk;

        memset(w, 0x00, sizeof w);
        for (i = 0; i < 16; i++) {
            w[i] = (uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8 | (uint32_t)p[3];
            p += 4;
        }

        for (i = 16; i < 64; i++) {
            const uint32_t s0 = right_rot(w[i - 15], 7) ^ right_rot(w[i - 15], 18) ^ (w[i - 15] >> 3);
            const uint32_t s1 = right_rot(w[i - 2], 17) ^ right_rot(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }

        for (i = 0; i < 8; i++) {
            ah[i] = h[i];
        }

        for (i = 0; i < 64; i++) {
            const uint32_t s1 = right_rot(ah[4], 6) ^ right_rot(ah[4], 11) ^ right_rot(ah[4], 25);
            const uint32_t ch = (ah[4] & ah[5]) ^ (~ah[4] & ah[6]);
            const uint32_t temp1 = ah[7] + s1 + ch + k[i] + w[i];
            const uint32_t s0 = right_rot(ah[0], 2) ^ right_rot(ah[0], 13) ^ right_rot(ah[0], 22);
            const uint32_t maj = (ah[0] & ah[1]) ^ (ah[0] & ah[2]) ^ (ah[1] & ah[2]);
            const uint32_t temp2 = s0 + maj;

            ah[7] = ah[6];
            ah[6] = ah[5];
            ah[5] = ah[4];
            ah[4] = ah[3] + temp1;
            ah[3] = ah[2];
            ah[2] = ah[1];
            ah[1] = ah[0];
            ah[0] = temp1 + temp2;
        }

        for (i = 0; i < 8; i++) {
            h[i] += ah[i];
        }
    }

    for (i = 0, j = 0; i < 8; i++) {
        hash[j++] = (uint8_t)(h[i] >> 24);
        hash[j++] = (uint8_t)(h[i] >> 16);
        hash[j++] = (uint8_t)(h[i] >> 8);
        hash[j++] = (uint8_t)h[i];
    }
}

class L2tlbEntry
{
public:
  uint64_t vpBase;
  uint64_t ppBase;
  uint64_t device_id;
  bool free;
  uint64_t mruTick;
  L2tlbEntry() : vpBase(0), ppBase(0), free(true), device_id(1000), mruTick(0) {}
  void setMRU()
  {
    mruTick = GetSystemTime();
  };
};

class L2TLB
{
  int numEntries;
  int assoc;
  int sets;

  L2tlbEntry **entries;

protected:
  L2TLB() {}

public:
  L2TLB(int _numEntries, int associativity) :
    numEntries(_numEntries), assoc(associativity)
  {
    
    assert(numEntries % assoc == 0);
    sets = numEntries / assoc;
    std::cout << "class: sets" << sets << std::endl;
    std::cout << "class: assoc" << assoc << std::endl;
    entries = new L2tlbEntry*[sets];

    for (int i = 0; i < sets; i++) {
      entries[i] = new L2tlbEntry[assoc];
    }
  }
  virtual ~L2TLB()
  {
    for (int i = 0; i < assoc; i++) {
      delete [] entries[i];
    }

    delete [] entries;
  }
  bool lookup(uint64_t vp_base,  uint64_t device_id, uint64_t& pp_base, bool set_mru = true);
  void insert(uint64_t vp_base, uint64_t device_id, uint64_t pp_base);
  void flushAll();
  void flushentry(uint64_t vp_base);
};

class L2TLB_Scatter
{
  int numEntries;
  int assoc;
  int sets;

  L2tlbEntry **entries;

protected:
  L2TLB_Scatter() {}

public:
  L2TLB_Scatter(int _numEntries, int associativity) :
    numEntries(_numEntries), assoc(associativity)
  {
    
    assert(numEntries % assoc == 0);
    sets = numEntries / assoc;
    //std::cout << "class: sets" << sets << std::endl;
    //std::cout << "class: assoc" << assoc << std::endl;
    entries = new L2tlbEntry*[sets];

    for (int i = 0; i < sets; i++) {
      entries[i] = new L2tlbEntry[assoc];
    }
  }
  virtual ~L2TLB_Scatter()
  {
    for (int i = 0; i < assoc; i++) {
      delete [] entries[i];
    }

    delete [] entries;
  }
  bool lookup(uint64_t vp_base,  uint64_t& device_id, uint64_t& pp_base, bool set_mru = true);
  void insert(uint64_t vp_base, uint64_t device_id, uint64_t pp_base);
  void flushAll();
  uint64_t hashAddress(uint64_t vp_base);
  int getScatterSet(uint64_t hash);
  void flushentry(uint64_t vp_base);
};




class PTEntry
{
public:
  uint64_t ppBase;
  uint64_t device_id;
  bool free;
  uint64_t mruTick;
  PTEntry() : ppBase(0), device_id(0), free(true), mruTick(0) {}
  void setMRU()
  {
    mruTick = GetSystemTime();
  };
};

class PTCache
{
  int numEntries;
  int assoc;
  int sets;

  PTEntry **entries;

protected:
  PTCache() {}

public:
  PTCache(int _numEntries, int associativity) :
    numEntries(_numEntries), assoc(associativity)
  {
    
    assert(numEntries % assoc == 0);
    sets = numEntries / assoc;
    std::cout << "class: sets" << sets << std::endl;
    std::cout << "class: assoc" << assoc << std::endl;
    entries = new PTEntry*[sets];

    for (int i = 0; i < sets; i++) {
      entries[i] = new PTEntry[assoc];
    }
  }
  virtual ~PTCache()
  {
    for (int i = 0; i < assoc; i++) {
      delete [] entries[i];
    }

    delete [] entries;
  }
  bool lookup(uint64_t pp_base, uint64_t& device_id, bool set_mru = true);
  void insert(uint64_t pp_base, uint64_t device_id);
  std::unordered_map<uint64_t, std::array<int, 16>> evictedPPNs;  // Map to track evicted PPNs

  void flushAll();
};


class PTrack {
private:
    struct CacheEntry {
        uint64_t pp_base;
        bool valid;
        std::array<int, 16> deviceAccessStatus;
        uint64_t lruTick;
        uint64_t lenderID = 0;
        int shareCounter = 0; 
    };

    int numSets; // Number of sets
    int numWays; // Number of ways (associativity)
    std::vector<std::vector<CacheEntry>> permissionCache; // 2D vector: [set][way]
    std::unordered_map<uint64_t, std::array<int, 16>> evictedEntries; // Stores evicted entries
    std::unordered_map<uint64_t, uint32_t> pageSharerMasks;
    uint64_t PT_hits = 0, PT_misses = 0;

    int findOrEvict(int setIndex, uint64_t pp_base, uint64_t device_id);

public:
    PTrack(int totalEntries, int associativity) {
        numWays = associativity;
        numSets = totalEntries / numWays;
        permissionCache.resize(numSets, std::vector<CacheEntry>(numWays));
    }

    void access(uint64_t pp_base, uint64_t device_id);
    bool lookup(uint64_t pp_base, uint64_t device_id);
    void evictEntry(uint64_t pp_base, const std::array<int, 16>& deviceAccessStatus);
    uint64_t getHits() const { return PT_hits; }
    uint64_t getMisses() const { return PT_misses; } // back-invalidation count
    uint64_t getAccesses() const { return PT_hits + PT_misses; }
    uint64_t getPagesWithOneAccessor() const;
    uint64_t getPagesWithTwoToFiveAccessors() const;
    uint64_t getPagesWithMoreThanFiveAccessors() const;
};



class NetworkInterrupts
{
public:
  class Msg
  {
  public:
    std::vector<uint8_t> packet;
    int thread;
    int lcacc;
    int priority;
    inline bool operator <(const Msg& m) const
    {
      return priority < m.priority;
    }
  };

  class BiNPerformancePoint
  {
  public:
    uint32_t bufferSize;
    uint32_t performance;
    uint32_t cacheImpact;
  };

  class AcceleratorDeclaration
  {
  public:
    int type;
    int count;
  };
private:
  static std::map<int, NetworkInterrupts*> cpuMap; //key cpu simics object
  NetworkInterruptHandle* nih;
  std::map<int, std::priority_queue<Msg> > pendingMsgs; //key thread
  std::map<int, std::queue<int> > pendingSignals;
  void TryRaise(int thread);
  int CalcPriority(int source, int threadID, const std::vector<uint8_t>& packet);

  uint32_t hostPTWLatency;

  uint64_t hostPTWalks;
  uint64_t hostPTWalkTick;
  uint64_t hostPTWalkTime;
  uint64_t pagewalkcounter=0;
  uint64_t key_counter=0;
  std::list<RequestPtr> pendingTranslations;
  

  enum State {
    Ready,
    Waiting
  };

  State currState;

  int interval;

protected:
  uint32_t tlbSize;

  typedef std::list<X86ISA::TlbEntry *> EntryList;

  X86ISA::TlbEntry * tlb;

  EntryList freeList;

  TlbEntryTrie trie;

  uint64_t lruSeq;

public:
  static std::map<int, std::vector<BiNPerformancePoint> > BiNCurveInfo; //key thread, value bin point
  static std::map<int, std::vector<AcceleratorDeclaration> > accDeclInfo; //key thread, value declaration
  static std::map<int, std::vector<int> > pendingReservation; //key threadID, vector of lcaccID's
  static NetworkInterrupts* LookupNIByCpu(int cpu);

  std::vector<int> queueLen;
  struct MACstruct{
          uint32_t buffer[12];
        };
  typedef MACstruct* MACptr;
  std::list<MACstruct* > MAC_verf;
  // typedef NetworkInterruptsParams Params;
  // NetworkInterrupts(const Params *p);

  NetworkInterrupts(NetworkInterruptHandle* x);
  ~NetworkInterrupts();

  NetworkInterruptHandle* GetHandle()
  {
    return nih;
  }
  int GetSignal(int thread);
  void RaiseInterrupt(int source, int threadID, const void* buffer, int bufferSize);
  void PutOffInterrupt(int source, int threadID, const void* buffer, int bufferSize, int delay);
  void RecvMessage(int source, const char* buffer, int size);
  void HostPTWalk(RequestPtr req);
  void startverification( MACptr buffer);
  void setupverification();
  void sampleQueueLen();
  typedef MemberCallback4<NetworkInterrupts, int, int, const void*, int, &NetworkInterrupts::RaiseInterrupt> RaiseInterruptCB;
  typedef Arg3MemberCallback<NetworkInterrupts, int, const char*, int, &NetworkInterrupts::RecvMessage> RecvMessageCB;
  typedef MemberCallback1<NetworkInterrupts, int, &NetworkInterrupts::TryRaise> TryRaiseCB;
  typedef MemberCallback1<NetworkInterrupts, RequestPtr, &NetworkInterrupts::HostPTWalk> HostPTWalkCB;
  typedef MemberCallback0<NetworkInterrupts, &NetworkInterrupts::sampleQueueLen> sampleQueueLenCB;
  typedef MemberCallback1<NetworkInterrupts, MACptr, &NetworkInterrupts::startverification> startverificationCB;

  inline std::string GetDeviceName()
  {
    char s[20];
    sprintf(s, "netinterrupts.%02d", nih->portID);
    return s;
  }
  L2TLB* l2tlb = new L2TLB(1024,8); //IOTLB(entries, assoc)
  //L2TLB_Scatter* l2tlb = new L2TLB_Scatter(1024,8);
  L2TLB* PML4 = new L2TLB(8,8); //IOTLB(entries, assoc)
  L2TLB* PDP = new L2TLB(8,8); //IOTLB(entries, assoc)
  L2TLB* PD = new L2TLB(8,8); //IOTLB(entries, assoc)
  L2TLB* PTE = new L2TLB(8,8); //IOTLB(entries, assoc)
  //PTCache* PT = new PTCache(256,256);
  PTrack* PT = nullptr; // allocated in ctor: entries = 16 * num_acc, assoc = --ptrack_assoc
  int l2tlb_hits=0;
  int l2tlb_access=0;
  int l2tlb_misses=0;
  int PT_access=0;
  uint64_t mshr_entries =0;
  uint64_t mshr_size=0;
  const uint64_t OFFSET_MASK = 0xFFF;           // 12 bits for offset
  const uint64_t PTE_MASK     = 0x1FF << 12;     // 9 bits for PT index
  const uint64_t PD_MASK     = 0x1FF << 21;     // 9 bits for PD index
  const uint64_t PDP_MASK   = 0x1FF << 30;     // 9 bits for PDPT index
  const uint64_t PML4_MASK   = 0x1FF << 39;     // 9 bits for PML4 index
  uint64_t PTE_Latency     = 5;     // 9 bits for PT index
  uint64_t PD_Latency     = 5;     // 9 bits for PD index
  uint64_t PDP_Latency   = 5;     // 9 bits for PDPT index
  uint64_t PML4_Latency   = 5;     // 9 bits for PML4 index
uint64_t generatePTE() {
    return (static_cast<uint64_t>(rand()) << 32) | rand();
}
uint64_t pml4PTE, pdpPTE, pdPTE, ptPTE;
  void finishTranslation(WholeTranslationState *state);
  void startWalk();
  void walkerstate();
  void setupWalk(int thread, uint64_t vAddr, uint64_t device_id, uint64_t pAddr, uint64_t ver_req);

  /** This function is used by the page table walker to determin if it could
  * translate a pending request or if the underlying request has been
  * squashed. This always returns false for the accelerater as it never
  * executes any instructions speculatively.
  */
  bool isSquashed() const
  {
    return false;
  }

  uint64_t getHostPTWalks()
  {
    return hostPTWalks;
  }

  uint64_t getHostPTWalkTime()
  {
    return hostPTWalkTime;
  }

  uint64_t getBackInvalidation()
  {
    return PT ? PT->getMisses() : 0;
  }

  uint64_t getPTrackHits()
  {
    return PT ? PT->getHits() : 0;
  }

  uint64_t getPTrackAccesses()
  {
    return PT ? PT->getAccesses() : 0;
  }

  uint64_t getPagesWithOneAccessor() const
  {
    return PT ? PT->getPagesWithOneAccessor() : 0;
  }

  uint64_t getPagesWithTwoToFiveAccessors() const
  {
    return PT ? PT->getPagesWithTwoToFiveAccessors() : 0;
  }

  uint64_t getPagesWithMoreThanFiveAccessors() const
  {
    return PT ? PT->getPagesWithMoreThanFiveAccessors() : 0;
  }

  uint64_t getPermissionChanges() const
  {
    return key_counter;
  }

  X86ISA::TlbEntry *lookup(uint64_t va, bool update_lru = true);

  void flushAll();

  void evictLRU();

  uint64_t nextSeq()
  {
    return ++lruSeq;
  }

  X86ISA::TlbEntry * insert(uint64_t vpn, X86ISA::TlbEntry &entry);
};

void HandleNetworkMessage(void*, int, int, const char*, int);
//void HandleEvent(conf_object_t* obj, void* arg);
void HandleEvent(ThreadContext* obj, void* arg);
//void EnqueueEvent(conf_object_t* obj, void* arg, int dt);
void EnqueueEvent(void* arg, int dt);

class CBContainer
{
  void (*cb)(void*);
  void* args;
public:
  CBContainer(void (*cb)(void*), void* args)
  {
    this->cb = cb;
    this->args = args;
  }
  void call()
  {
    cb(args);
  }
};

//X86ISA::IntReg LCAccMagicIntercept(void*, ThreadContext* cpu, int32_t op);
uint64_t LCAccMagicIntercept(void*, ThreadContext* cpu, int32_t op, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6, uint64_t arg7);
NetworkInterruptHandle* createNetworkInterruptHandle(int portID, int deviceID, int procID);
void HandleEvent(void* arg);

extern std::vector<std::queue<CBContainer> > cycleCBRing;
bool localCBsForCycle(Cycles current_cycle);
std::queue<CBContainer>& getCurrentCycleQueue(Cycles current_cycle);
void retireCBsForCycle(Cycles current_cycle);

#endif

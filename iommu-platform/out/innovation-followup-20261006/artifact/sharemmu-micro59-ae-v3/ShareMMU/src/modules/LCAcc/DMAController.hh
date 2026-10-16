#ifndef DMACONTROLLER_H
#define DMACONTROLLER_H

#include <stdint.h>
#include <map>
#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "SPMInterface.hh"
#include "NetworkInterface.hh"
#include "modules/Common/BaseCallbacks.hh"
#include "modules/linked-prefetch-tile/prefetcher-tile.hh"
#include "modules/MsgLogger/MsgLogger.hh"

namespace LCAcc
{

class TLBEntry
{
public:
  uint64_t vpBase;
  uint64_t ppBase;
  bool free;
  uint64_t mruTick;
  TLBEntry() : vpBase(0), ppBase(0), free(true), mruTick(0) {}
  void setMRU()
  {
    mruTick = GetSystemTime();
  };
};

class BaseTLBMemory
{
public:
  virtual bool lookup(uint64_t vp_base, uint64_t& pp_base, bool set_mru = true) = 0;
  virtual void insert(uint64_t vp_base, uint64_t pp_base) = 0;
  virtual void flushAll() = 0;
  virtual void flushentry( uint64_t vp_base)=0;
};

class TLBMemory : public BaseTLBMemory
{
  int numEntries;
  int sets;
  int ways;

  TLBEntry **entries;

protected:
  TLBMemory() {}

public:
  TLBMemory(int _numEntries, int associativity) :
    numEntries(_numEntries), sets(associativity)
  {
    if (sets == 0) {
      sets = numEntries;
    }

    assert(numEntries % sets == 0);
    ways = numEntries / sets;
    entries = new TLBEntry*[ways];

    for (int i = 0; i < ways; i++) {
      entries[i] = new TLBEntry[sets];
    }
  }
  virtual ~TLBMemory()
  {
    for (int i = 0; i < sets; i++) {
      delete [] entries[i];
    }

    delete [] entries;
  }

  virtual bool lookup(uint64_t vp_base, uint64_t& pp_base, bool set_mru = true);
  virtual void insert(uint64_t vp_base, uint64_t pp_base);
  virtual void flushAll();
  virtual void flushentry( uint64_t vp_base);
};

class InfiniteTLBMemory : public BaseTLBMemory
{
  std::map<uint64_t, uint64_t> entries;
public:
  InfiniteTLBMemory() {}
  ~InfiniteTLBMemory() {}

  bool lookup(uint64_t vp_base, uint64_t& pp_base, bool set_mru = true)
  {
    auto it = entries.find(vp_base);

    if (it != entries.end()) {
      pp_base = it->second;
      return true;
    } else {
      pp_base = 0;
      return false;
    }
  }
  void insert(uint64_t vp_base, uint64_t pp_base)
  {
    entries[vp_base] = pp_base;
  }
  void flushAll() {}
  void flushentry( uint64_t vp_base) {}
};

class DMAController
{
public:
  class AccessType
  {
  public:
    static const int Read;
    static const int Write;
    static const int ReadLock;
    static const int WriteLock;
    static const int WriteUnlock;
    static const int Unlock;
  };
private:
  class SignalEntry
  {
  public:
    int ID;
    uint64_t dstAddr;
    uint64_t requestedAddr;
    unsigned int size;
    CallbackBase* onFinish;
    bool Matches(const SignalEntry& s)
    {
      return s.ID == ID && s.dstAddr == dstAddr && s.requestedAddr == requestedAddr && s.size == size;
    }
  };
  uint64_t GetPageAddr(uint64_t addr) const;
  std::vector<SignalEntry> remoteSignals;
  std::vector<SignalEntry> localSignals;

  std::map<uint64_t, std::list<TransferData*> > MSHRs;


    struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2; // Combine the two hashes
    }
};

  /*
  std::unordered_map<std::pair<uint64_t, uint64_t>, std::string, pair_hash> last_operations;
  std::unordered_map<std::pair<uint64_t, uint64_t>, int, pair_hash> transition_counts;
  std::unordered_set<uint64_t> unique_addresses;
  */
  std::unordered_map<uint64_t, std::string> last_operations;
  std::unordered_set<uint64_t> unique_addresses;
  std::unordered_map<uint64_t, std::string> last_access; // key = acc_id<<52 | vp_base



  std::string R_W;
  std::string operation;
  uint64_t key_change = 0;
  uint64_t transitions=0;
  uint64_t node_id;
  uint64_t num_ctxsw=0;
  uint64_t lastTrigger=0; 


  DMAEngineHandle *dmaDevice;
  prftch_direct_interface_t* dmaInterface;
  SPMInterface* spm;
  NetworkInterface* network;
  Arg1CallbackBase<uint64_t>* onAccViolation;
  Arg4CallbackBase<uint64_t, uint64_t, uint64_t, uint64_t>* onTLBMiss;
  // std::map<uint64_t, uint64_t> tlbMap;
  int buffer;
  void OnAccessError(uint64_t logicalAddr);
  void OnNetworkMsg(int src, const void* buffer, unsigned int bufSize);

  typedef Arg1MemberCallback<DMAController, uint64_t, &DMAController::OnAccessError> OnAccessErrorCB;
  typedef Arg3MemberCallback<DMAController, int, const void*, unsigned int, &DMAController::OnNetworkMsg> OnNetworkMsgCB;
  bool isHookedToMemory;
public:
  DMAController(NetworkInterface* ni, SPMInterface* spmInterface, Arg4CallbackBase<uint64_t, uint64_t, uint64_t, uint64_t>* TLBMiss, Arg1CallbackBase<uint64_t>* accessViolation);
  ~DMAController();

  void BeginTransfer(int srcSpm, uint64_t srcAddr, const std::vector<unsigned int>& srcSize, const std::vector<int>& srcStride, int dstSpm, uint64_t dstAddr, const std::vector<unsigned int>& dstSize, const std::vector<int>& dstStride, size_t elementSize, CallbackBase* finishedCB);
  void BeginTransfer(int srcSpm, uint64_t srcAddr, const std::vector<unsigned int>& srcSize, const std::vector<int>& srcStride, int dstSpm, uint64_t dstAddr, const std::vector<unsigned int>& dstSize, const std::vector<int>& dstStride, size_t elementSize, int priority, CallbackBase* finishedCB);
  void PrefetchMemory(uint64_t baseAddr, const std::vector<unsigned int>& size, const std::vector<int>& stride, size_t elementSize);

  void BeginSingleElementTransfer(int mySPM, uint64_t src, uint64_t dst, uint32_t size, int type, CallbackBase* finishedCB);
  void SetBuffer(int buf);
  void FlushTLB();
  void AddTLBEntry(uint64_t vAddr, uint64_t pAddr);
  void HookToMemoryController(const std::string& deviceName);

  enum TransferStatus {
    Running,
    TlbWait
  };

  typedef MemberCallback2<DMAController, uint64_t, uint64_t, &DMAController::AddTLBEntry> AddTLBEntryCB;

protected:

  inline std::string GetDeviceName()
  {
    char s[100];
    sprintf(s, "dma%02d", network->GetNetworkPort());
    return s;
  }

  // Access Stats
  uint64_t hits;
  uint64_t mshrhits;
  uint64_t misses;
  uint64_t flushTlb;
  uint64_t tlbCycles;
  uint64_t mshr_entries = 8;
  uint64_t mshr_size =8;
  uint64_t cpu_perm =0;


  TransferStatus transferStatus;
  uint64_t timeStamp;

public:
  // private TLB entries
  int numEntries;

  int associativity;
  int TLB_shootdown=0;
  int hitLatency;

  BaseTLBMemory *tlbMemory;

  void beginTranslateTiming(TransferData* td);

  void translateTiming(TransferData* td);

  void finishTranslation(uint64_t vp_base, uint64_t pp_base, uint64_t Mac_return);

  void flushAll();

  void flushentry( uint64_t vp_base);

  typedef Arg1MemberCallback<DMAController, TransferData*, &DMAController::beginTranslateTiming> beginTranslateTimingCB;
  typedef MemberCallback1<DMAController, TransferData*, &DMAController::translateTiming> translateTimingCB;

  uint64_t getTlbHits()
  {
    return hits + mshrhits;
  }
  uint64_t getTlbMisses()
  {
    return misses;
  }
  uint64_t getTlbFlush()
  {
    return flushTlb;
  }
  uint64_t getTlbCycles()
  {
    return tlbCycles;
  }
};

}

#endif
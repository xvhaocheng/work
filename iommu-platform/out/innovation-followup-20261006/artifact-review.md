# ShareMMU artifact v3 只读证据整理

范围：仅依据本轮提供的 7 个源文件与 paper/docs 文本。行号按你给出的编号；formfeed 可能造成物理行偏移，因此下面同时给函数/邻近代码名。未执行、未构建、未运行测试。关键并发与最终创新判断仍需 Codex 直接核。

---

## 1. 相关调用链与 file:line

### 1.1 DMA 翻译入口、延迟翻译、完成

`DMAController` 构造时把 TLB 翻译回调挂到 `dmaInterface`，并注册网络消息接收：

- `DMAController::DMAController(...)`：`DMAController.cc:34-71`
  - `dmaInterface->Configure(... beginTranslateTimingCB..., OnAccessErrorCB...)`：`DMAController.cc:46-48`
  - `ni->RegisterRecvHandler(OnNetworkMsgCB::Create(this))`：`DMAController.cc:50`
  - TLB 组织：`numEntries = RubySystem::getLCAccTLBSize()`，`hitLatency = getLCAccTLBLatency()`，`associativity = getLCAccTLBAssoc()`：`DMAController.cc:54-62`

翻译启动：

```cpp
// DMAController.cc:112-137
void DMAController::beginTranslateTiming(TransferData* td)
{
  if (RubySystem::idealMMU()) {
    ...
    if (g_TLBHack_interface->PageKnown(1, vp_base)) {
      uint64_t pp_base = g_TLBHack_interface->Lookup(1, vp_base);
      td->setPaddr(pp_base + offset);
      dmaInterface->finishTranslation(dmaDevice, td);
    } else {
      ...
      uint64_t pp_base = TheISA::vtophys(cpu, vp_base);
      td->setPaddr(pp_base + offset);
      dmaInterface->finishTranslation(dmaDevice, td);
    }
  } else {
    SimicsInterface::RegisterCallback(
      translateTimingCB::Create(this, td), hitLatency);
  }
}
```

结论：非 ideal 路径下，翻译不是立即完成，而是经 `SimicsInterface::RegisterCallback(... hitLatency)` 延迟到 `translateTiming`。

`translateTiming` 是核心分支：

```cpp
// DMAController.cc:139-244 摘要
void DMAController::translateTiming(TransferData* td)
{
  uint64_t vaddr = td->getVaddr();
  uint64_t vp_base = GetPageAddr(vaddr);
  uint64_t offset = vaddr - vp_base;
  uint64_t pp_base;
  uint64_t lcacc_tlb_mshr = RubySystem::getLCAccTLBMSHR();
  uint64_t MAC_dma;
  std::string R_W = td->isRead() ? "Read" : "Write";
  key_change = 0;

  if (job != lastJob) {
    job = lastJob;
    cpu_perm++;
    key_change = 1;
    tlbMemory->flushentry(vp_base);
    ...
  }

  uint64_t simFreq = 1000000000000ULL;
  uint64_t interval = simFreq / 200; // 5e9 ticks
  if (curTick() - lastTrigger >= interval) {
    lastTrigger = curTick();
    num_ctxsw++;
    FlushTLB();
    ...
  }

  uint64_t key = vp_base;
  auto it = last_access.find(key);
  if (it == last_access.end()) {
    last_access[key] = R_W;
  } else {
    if ((it->second == "Read" && R_W == "Write") ||
        (it->second == "Write" && R_W == "Read")) {
      transitions++;
      key_change = 1;
      tlbMemory->flushentry(vp_base);
      ...
    }
    it->second = R_W;
  }

  if (key_change) {
    misses++;
    if (MSHRs.empty()) {
      transferStatus = TlbWait;
      timeStamp = GetSystemTime();
    }
    td->MAC_ver = 0;
    MAC_dma = 0;
    pp_base = 0;
    MSHRs[vp_base].push_back(td);
    onTLBMiss->Call(vp_base, MAC_dma, pp_base, key_change);
  } else {
    if (tlbMemory->lookup(vp_base, pp_base)) {
      hits++;
      td->setPaddr(pp_base + offset);
      td->MAC_ver = 1;
      MSHRs[vp_base].push_back(td);
      MAC_dma = 1;
      onTLBMiss->Call(vp_base, MAC_dma, pp_base, 0);
    } else {
      misses++;
      if (MSHRs.empty()) {
        transferStatus = TlbWait;
        timeStamp = GetSystemTime();
      }
      td->MAC_ver = 0;
      MAC_dma = 0;
      pp_base = 0;
      MSHRs[vp_base].push_back(td);
      onTLBMiss->Call(vp_base, MAC_dma, pp_base, 0);
    }
  }
}
```

关键事实：

- 命中时也 `MSHRs[vp_base].push_back(td)`，并调用 `onTLBMiss->Call(vp_base, MAC_dma=1, pp_base, 0)`：`DMAController.cc:219-226`。也就是说，本地 TLB 命中仍要经过 `onTLBMiss` 回调，不像理想 TLB 直接 `finishTranslation`。
- miss/key_change 时 `MAC_dma=0, pp_base=0`：`DMAController.cc:204-214`、`230-239`。
- 本地权限变化触发 `tlbMemory->flushentry(vp_base)`：`DMAController.cc:187`。
- 周期性 flush：`curTick()-lastTrigger >= 5e9` 时 `FlushTLB()`：`DMAController.cc:165-174`。
- `last_access` 的键是 `vp_base`，不是头文件注释说的 `acc_id<<52 | vp_base`：头文件 `DMAController.hh:162` vs 实现 `DMAController.cc:176`。

完成翻译：

```cpp
// DMAController.cc:88-110
void DMAController::finishTranslation(uint64_t vp_base, uint64_t pp_base, uint64_t MAC_return)
{
  std::list<TransferData*> &tds = MSHRs[vp_base];
  for (it = tds.begin(); it != tds.end(); it++) {
    TransferData* td = (*it);
    uint64_t offset = td->getVaddr() % TheISA::PageBytes;
    td->setPaddr(pp_base + offset);
    dmaInterface->finishTranslation(dmaDevice, td);
  }
  tlbMemory->insert(vp_base, pp_base);
  MSHRs.erase(vp_base);
  if (MSHRs.empty()) {
    transferStatus = Running;
    tlbCycles += GetSystemTime() - timeStamp;
  }
}
```

注意：形参 `MAC_return` 未被使用：`DMAController.cc:89, 110`。在提供的文件里也没有 `DMAController::finishTranslation(...)` 的调用点；调用点应在未提供的回调/接口文件中。

---

### 1.2 TLB miss 回调 → NetworkInterrupts 处理链

`onTLBMiss` 的类型是 `Arg4CallbackBase<uint64_t,uint64_t,uint64_t,uint64_t>*`：`DMAController.hh:180`。调用点：

- key_change：`onTLBMiss->Call(vp_base, MAC_dma, pp_base, key_change)`：`DMAController.cc:214`
- hit：`onTLBMiss->Call(vp_base, MAC_dma, pp_base, 0)`，其中 `MAC_dma=1`，`pp_base` 为物理基址：`DMAController.cc:225`
- miss：`onTLBMiss->Call(vp_base, MAC_dma, pp_base, 0)`，其中 `MAC_dma=0, pp_base=0`：`DMAController.cc:239`

`NetworkInterrupts::RecvMessage` 明确处理 `LCACC_CMD_TLB_MISS`：

```cpp
// NetworkInterrupts.cc:277-301
} else if (args[0] == LCACC_CMD_TLB_MISS) {
  assert(size >= 10);
  int thread = args[1];
  BitConverter bc_vAddr;
  bc_vAddr.u32[0] = args[2];
  bc_vAddr.u32[1] = args[3];
  uint64_t vAddr = bc_vAddr.u64[0];
  ...
  bc_pAddr.u32[0] = args[4];
  bc_pAddr.u32[1] = args[5];
  uint64_t pAddr = bc_pAddr.u64[0];
  ...
  bmc.u32[0] = args[6];
  bmc.u32[1] = args[7];
  uint64_t MAC = bmc.u64[0];
  ...
  bno.u32[0] = args[8];
  bno.u32[1] = args[9];
  uint64_t device_id = bno.u64[0];
  ...
  bnk.u32[0] = args[10];
  bnk.u32[1] = args[11];
  uint64_t key_change = bnk.u64[0];
```

这里有需要 Codex 核验的次序问题：`DMAController` 传入顺序是 `(vp_base, MAC_dma, pp_base, key_change)`，而 `NetworkInterrupts` 解析为 `vAddr, pAddr, MAC, device_id, key_change`。回调打包实现不在提供的文件中，因此不能断定实际错位；但这是最小下一文件要核的点。

`MAC == 1` 分支：

```cpp
// NetworkInterrupts.cc:307-335
if (MAC == 1) {
  MACptr macstruct = new MACstruct;
  macstruct->buffer[0] = LCACC_CMD_TLB_SERVICE;
  macstruct->buffer[1] = thread;
  ... vAddr, pAddr, MAC, device_id, key_change ...
  MAC_verf.push_back(macstruct);
  setupverification();
}
```

`MAC != 1` 分支：

```cpp
// NetworkInterrupts.cc:337-391 摘要
else {
  if (key_change == 1) {
    l2tlb_misses++;
    uint64_t pAddr = 0;
    uint64_t permission_update = 1;
    l2tlb->flushentry(vAddr);
    key_counter++;
    setupWalk(thread, vAddr, device_id, pAddr, permission_update);
  } else {
    if (l2tlb->lookup(vAddr, device_id, pAddr)) {
      l2tlb_hits++;
      ...
      nih->snpi->SendMessageOnDevice(
          nih->deviceID, 0, buffer, sizeof(buffer));
    } else {
      l2tlb_misses++;
      uint64_t pAddr = 0;
      uint64_t ver_req = 0;
      setupWalk(thread, vAddr, device_id, pAddr, ver_req);
    }
  }
}
```

`setupverification` / `startverification`：

- `setupverification`：`NetworkInterrupts.cc:401-519`
  - `PT->access(pAddr, device_id); PT_access++;`：`NetworkInterrupts.cc:429-431`
  - `if (PT->lookup(pAddr, device_id))`：`NetworkInterrupts.cc:432`
  - PTrack 命中/未命中后模拟 `PML4/PDP/PD/PTE` 延迟：`NetworkInterrupts.cc:442-504`
  - 最后 `EnqueueEvent(startverificationCB::Create(this, buffer), latency)`：`NetworkInterrupts.cc:436-437, 504, 513-514`
- `startverification`：`NetworkInterrupts.cc:521-562`
  - 构造 `LCACC_CMD_TLB_SERVICE`
  - `nih->snpi->SendMessageOnDevice(nih->deviceID, 0, msg, sizeof(msg));`：`NetworkInterrupts.cc:561`
  - 这里发送的是翻译服务响应，不是显式 back-invalidation。

`setupWalk` → `startWalk` → `HostPTWalk` → `finishTranslation`：

- `setupWalk`：`NetworkInterrupts.cc:564-580`
  - `req->setVirt(...)`，`req->SetdeviceID(device_id)`，`req->SetVerficationRequest(permission_update)`，`req->SetPaddr_Ver(pAddr)`
  - `pendingTranslations.push_back(req); walkerstate();`
- `walkerstate` / `startWalk`：`NetworkInterrupts.cc:582-673`
  - `startWalk` 模拟四级页表缓存，并设置 `hostPTWLatency`：`NetworkInterrupts.cc:609-661`
  - `if (permission_update) hostPTWLatency = 480; req->SetVerficationRequest(0);`：`NetworkInterrupts.cc:665-670`
  - `EnqueueEvent(HostPTWalkCB::Create(this, req), hostPTWLatency + RubySystem::getPTrackLatency());`：`NetworkInterrupts.cc:671-672`
- `HostPTWalk`：`NetworkInterrupts.cc:675-695`
  - 调用真实 `X86ISA::Walker::start(...)`
- `finishTranslation`：`NetworkInterrupts.cc:697-747`
  - `l2tlb->insert(logicalPage, device_id, physicalPage);`：`NetworkInterrupts.cc:716`
  - `PT_access++;`：`NetworkInterrupts.cc:719`
  - 发送 `LCACC_CMD_TLB_SERVICE` 到 `nih->deviceID`：`NetworkInterrupts.cc:724-739`
  - 若还有 `pendingTranslations`，继续 `startWalk()`：`NetworkInterrupts.cc:744-746`

结论：提供的文件中，TLB miss 链是“DMA 侧回调 → 网络消息 → NetworkInterrupts 处理 → 模拟/真实页表 walk → 发回 `LCACC_CMD_TLB_SERVICE`”。`DMAController::OnNetworkMsg` 没有处理 `LCACC_CMD_TLB_SERVICE`，因此 accelerator 侧接收该响应并最终调用 `DMAController::finishTranslation` 的代码在未提供文件中。

---

### 1.3 DMA 数据搬运网络路径

`DMAController::OnNetworkMsg` 只处理两个 opcode：

```cpp
// DMAController.cc:253-332 摘要
if (opcode == DMA_MEMORY_REQUEST) {
  ...
  for (i = localSignals.begin(); i != localSignals.end(); i++) {
    if (i->Matches(target)) {
      msg.push_back(DMA_MEMORY_RESPONSE);
      ...
      spm->Read(i->requestedAddr, i->size, &(msg[6]));
      network->SendMessage(i->ID, &(msg[0]), msg.size() * sizeof(int32_t));
      assert(i->onFinish);
      SimicsInterface::RegisterCallback(i->onFinish, 0);
      localSignals.erase(i);
      return;
    }
  }
  remoteSignals.push_back(target);
} else if (opcode == DMA_MEMORY_RESPONSE) {
  ...
  for (i = localSignals.begin(); i != localSignals.end(); i++) {
    if (i->Matches(r)) {
      SimicsInterface::RegisterCallback(i->onFinish, 0);
      spm->Write(r.dstAddr, r.size, &(args[5]));
      localSignals.erase(i);
      return;
    }
  }
  std::cerr << "DMA Memory Response came unsolicited" << std::endl;
  assert(0);
}
```

这条路径是 SPM→SPM 的远端搬运：`spm->Read` / `spm->Write` + 网络消息，没有调用 `beginTranslateTiming` / `translateTiming`。

`BeginTransfer` 的分支：

```cpp
// DMAController.cc:347-442 摘要
assert(srcSpm != -1 || dstSpm != -1);
assert(srcSpm == spm->GetID() || dstSpm == spm->GetID());

if (srcSpm == -1 || dstSpm == -1) {
  dmaInterface->StartTransferPrio(dmaDevice,
      srcSpm, srcAddr, srcSize.size(), &(srcSize[0]), &(srcStride[0]),
      dstSpm, dstAddr, dstSize.size(), &(dstSize[0]), &(dstStride[0]),
      elementSize, priority, buffer, finishCB);
} else if (dstSpm == spm->GetID()) {
  uint32_t msg[6];
  msg[0] = DMA_MEMORY_REQUEST;
  ...
  network->SendMessage(srcSpm, msg, 6 * sizeof(uint32_t));
  SignalEntry target;
  target.ID = srcSpm;
  target.dstAddr = dstAddr;
  target.requestedAddr = srcAddr;
  target.size = elementSize;
  ...
  localSignals.push_back(target);
} else {
  assert(srcSpm == spm->GetID());
  SignalEntry target;
  target.ID = dstSpm;
  ...
  for (i = remoteSignals.begin(); i != remoteSignals.end(); i++) {
    if (i->Matches(target)) {
      msg.push_back(DMA_MEMORY_RESPONSE);
      ...
      spm->Read(i->requestedAddr, i->size, &(msg[6]));
      network->SendMessage(i->ID, &(msg[0]), msg.size() * sizeof(int32_t));
      SimicsInterface::RegisterCallback(target.onFinish, 0);
      assert(i->onFinish == NULL);
      remoteSignals.erase(i);
      return;
    }
  }
  localSignals.push_back(target);
}
```

---

### 1.4 失效/刷新相关路径（提供文件内）

- 本地 TLB 全/条目失效：
  - `FlushTLB()`：`DMAController.cc:461-465`
  - `TLBMemory::flushAll()`：`DMAController.cc:509-516`
  - `TLBMemory::flushentry(vp_base)`：`DMAController.cc:519-530`
- L2TLB 条目失效：
  - `L2TLB::flushentry(vp_base)`：`NetworkInterrupts.cc:813-824`
- 权限变化时 NetworkInterrupts 侧：
  - `l2tlb->flushentry(vAddr); key_counter++; setupWalk(..., permission_update=1);`：`NetworkInterrupts.cc:346-349`
- PTrack 容量淘汰：
  - `PTrack::findOrEvict`：`NetworkInterrupts.cc:887-940`
  - 淘汰时只写 `evictedEntries`、置 `valid=false`：`NetworkInterrupts.cc:931-936`
- `PTrack::lookup`：`NetworkInterrupts.cc:967-977`

提供的文件中，没有在这些路径里找到显式“发送 back-invalidation 给 private TLB”的代码，也没有找到 flush ack / drain wait / global lock。

---

## 2. 实际 DMA 地址模式：一端 memory，另一端 scratchpad；不是 memory-to-memory 两端独立翻译

### 已证明

1. `BeginTransfer` 明确禁止两端都是 `-1`：

```cpp
// DMAController.cc:361-362
assert(srcSpm != -1 || dstSpm != -1);
assert(srcSpm == spm->GetID() || dstSpm == spm->GetID());
```

因此在这套 API 里，不存在 `srcSpm == -1 && dstSpm == -1` 的 memory-to-memory 调用。

2. 只要有一端是 `-1`，就交给 `dmaInterface->StartTransferPrio(...)`：

```cpp
// DMAController.cc:364-368
if (srcSpm == -1 || dstSpm == -1) {
  dmaInterface->StartTransferPrio(dmaDevice,
      srcSpm, srcAddr, srcSize.size(), &(srcSize[0]), &(srcStride[0]),
      dstSpm, dstAddr, dstSize.size(), &(dstSize[0]), &(dstStride[0]),
      elementSize, priority, buffer, finishCB);
}
```

而 `DMAController` 的 TLB 回调就是通过 `dmaInterface->Configure(...)` 挂进去的：`DMAController.cc:46-48`。因此翻译是针对 `dmaInterface` 管理的 memory 侧地址发生的。

3. 单元素访问更直接：读时 memory→SPM，写时 SPM→memory：

```cpp
// DMAController.cc:496-505
bool isRead = (type == AccessType::Read || type == AccessType::ReadLock);
...
dmaInterface->StartSingleTransferPrio(dmaDevice, isRead ? -1 : mySPM,
                                      isRead ? memAddr : spmAddr,
                                      isRead ? mySPM : -1,
                                      isRead ? spmAddr : memAddr,
                                      size, 0, buffer, finishedCB);
```

读：`srcSpm=-1, srcAddr=memAddr, dstSpm=mySPM, dstAddr=spmAddr`。写：`srcSpm=mySPM, srcAddr=spmAddr, dstSpm=-1, dstAddr=memAddr`。都是一端 memory、一端 SPM。

4. 两端都是 SPM 时，走 `DMA_MEMORY_REQUEST/RESPONSE` 网络消息，不经过 TLB 翻译：

- `BeginTransfer` 的 `dstSpm == spm->GetID()` 分支：`DMAController.cc:369-399`
- `BeginTransfer` 的 `srcSpm == spm->GetID()` 分支：`DMAController.cc:400-442`
- `OnNetworkMsg` 的 `DMA_MEMORY_REQUEST/RESPONSE`：`DMAController.cc:260-331`

这些分支只操作 `spm->Read` / `spm->Write`、`network->SendMessage`，没有调用 `beginTranslateTiming` 或 `translateTiming`。

### 支持字段/分支

| 字段/分支 | 位置 | 含义 |
|---|---|---|
| `srcSpm == -1 || dstSpm == -1` | `DMAController.cc:364` | 进入 `dmaInterface` 路径，由 DMA 接口处理 memory 侧 |
| `isRead ? -1 : mySPM` | `DMAController.cc:503-504` | 单元素访问中 memory 端用 `-1` 表示 |
| `dstSpm == spm->GetID()` | `DMAController.cc:369` | 本地 SPM 作为目的，发 `DMA_MEMORY_REQUEST` 给源 SPM |
| `else { assert(srcSpm == spm->GetID()); ... }` | `DMAController.cc:400-401` | 本地 SPM 作为源，回 `DMA_MEMORY_RESPONSE` |
| `assert(srcSpm != -1 || dstSpm != -1)` | `DMAController.cc:361` | 禁止双 memory 端 |

### 未知/需下一文件

- `prftch_direct_interface_t::StartTransferPrio` / `StartSingleTransferPrio` 内部是否只在 memory 侧翻译，还是对两端都翻译。
- `TransferData` 的 `getVaddr()` 来源：是 memory 侧虚拟地址，还是可能同时包含源/目的两个虚拟地址。
- 若某些调用绕过 `DMAController::BeginTransfer` 直接调 `dmaInterface`，是否可能构造双 memory 端。
- `buffer` 字段：`DMAController::SetBuffer(int buf)`：`DMAController.cc:454-458`；实际用途不在提供文件内。

结论：在当前 7 个文件的 API 层面，实际模式是 **memory↔scratchpad 一端翻译**，外加 **SPM↔SPM 网络搬运、无翻译**。没有看到 memory-to-memory 两端独立翻译的证据。

---

## 3. PT 容量淘汰：victim 选择、mask/recency、失效传送/确认/等待

### 3.1 PTrack 结构

`NetworkInterrupts.hh:351-387`：

```cpp
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

    int numSets;
    int numWays;
    std::vector<std::vector<CacheEntry>> permissionCache;
    std::unordered_map<uint64_t, std::array<int, 16>> evictedEntries;
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
    void evictEntry(...);
    ...
};
```

注意：

- `CacheEntry` 有 `deviceAccessStatus[16]`，没有 paper 所说的 16-bit Access Mask 字段名。
- 没有 VPN 字段，没有 alias presence/counter/pointer。
- 有 `lenderID` 和 `shareCounter`，这是 paper 文本没有展开的实现字段。

### 3.2 victim 选择实际代码

```cpp
// NetworkInterrupts.cc:887-940
int PTrack::findOrEvict(int setIndex, uint64_t pp_base, uint64_t device_id) {
    std::vector<CacheEntry>& set = permissionCache[setIndex];

    // Search for hit
    for (int i = 0; i < numWays; i++) {
        if (set[i].valid && set[i].pp_base == pp_base) {
            PT_hits++;
            return i;
        }
    }

    // Look for invalid slot
    for (int i = 0; i < numWays; i++) {
        if (!set[i].valid) {
            PT_hits++;
            return i;
        }
    }

    // AA-LRU Victim Selection
    int victimIndex = -1;
    double worstScore = -1e9;

    for (int i = 0; i < numWays; i++) {
        CacheEntry& e = set[i];

        // Preferential eviction for unshared entries by the same lender
        /*
        if (e.lenderID == device_id && e.shareCounter == 0) {
            victimIndex = i;
            break;
        }
        */
        // Compute Accelerator-Aware LRU Score
        double age = GetSystemTime() - e.lruTick;
        double score = age - 5.0 * e.shareCounter;  // tune weight = 5.0

        if (score > worstScore) {
            worstScore = score;
            victimIndex = i;
        }
    }

    // Evict chosen entry
    if (victimIndex != -1) {
        CacheEntry& evicted = set[victimIndex];
        PT_misses++;
        evictedEntries[evicted.pp_base] = evicted.deviceAccessStatus;
        evicted.valid = false;
    }
    /*PT miss is back invalidation */
    std::cout << "PT hits: " << PT_hits << "  PT misses: " << PT_misses << std::endl;
    return victimIndex;
}
```

已证明：

- 命中查找只看 `valid && pp_base`，不看 `device_id`：`NetworkInterrupts.cc:891-896`。
- 有空位时返回空位，并执行 `PT_hits++`：`NetworkInterrupts.cc:899-903`。这不是通常意义的 hit。
- 无空位时，victim 分数是 `age - 5.0 * shareCounter`，选择 **score 最大** 者：`NetworkInterrupts.cc:920-928`。
- `age = GetSystemTime() - e.lruTick`，所以越久未用 age 越大，越倾向于被淘汰。
- 权重是固定 `5.0`：`NetworkInterrupts.cc:922`。
- 注释掉的“同 lender 且 shareCounter==0 优先淘汰”没有启用：`NetworkInterrupts.cc:913-919`。
- `deviceAccessStatus` 和 `pageSharerMasks` **没有**参与 victim 分数。victim 只用 `lruTick` 和 `shareCounter`。
- 淘汰时只写 `evictedEntries[evicted.pp_base] = evicted.deviceAccessStatus;`，然后 `valid=false`：`NetworkInterrupts.cc:931-936`。
- 没有发送 back-invalidation，没有等待 ack，没有锁。

### 3.3 mask/recency 实际使用

`PTrack::access`：

```cpp
// NetworkInterrupts.cc:943-965
void PTrack::access(uint64_t pp_base, uint64_t device_id) {
    if (device_id >= 2 && device_id < 34) {
        const uint64_t physicalPage = pp_base >> 12;
        pageSharerMasks[physicalPage] |=
            static_cast<uint32_t>(1U << (device_id - 2));
    }

    int setIndex = pp_base % numSets;
    int index = findOrEvict(setIndex, pp_base, device_id);

    CacheEntry& entry = permissionCache[setIndex][index];
    entry.pp_base = pp_base;
    entry.valid = true;
    if (entry.lenderID == 0) {
        entry.lenderID = device_id;  // First device becomes lender
    } else if (entry.lenderID != device_id) {
        entry.shareCounter++;        // Another accelerator reused this entry
    }
    entry.deviceAccessStatus[device_id - 2] = 1;
    entry.lruTick = GetSystemTime();
}
```

已证明：

- `pageSharerMasks` 是单独的 `unordered_map<uint64_t,uint32_t>`，按 physical page 累积 OR 位：`NetworkInterrupts.cc:944-947`。
- `deviceAccessStatus[device_id - 2] = 1` 被写，但 **victim 选择不读它**：`NetworkInterrupts.cc:963`，`NetworkInterrupts.cc:920-928`。
- `shareCounter` 的语义不是 “Access Mask 中 set bit 数”，而是：
  - 第一次访问时 `lenderID = device_id`；
  - 之后只要 `entry.lenderID != device_id`，每次访问都 `shareCounter++`：`NetworkInterrupts.cc:958-962`。
  - 因此同一非 lender 设备重复访问会多次增加 `shareCounter`，不是不同 sharer 的数量。
- 淘汰时 **没有重置** `lenderID`、`shareCounter`、`deviceAccessStatus`：`NetworkInterrupts.cc:931-936`。随后 `access` 复用同一个 `CacheEntry`，所以新条目可能继承旧条目的 `shareCounter/lenderID`：`NetworkInterrupts.cc:955-964`。
- `pageSharerMasks` 在提供文件中也没有清理路径；它只增长/OR，并用于统计 `getPagesWithOneAccessor` / `getPagesWithTwoToFiveAccessors` / `getPagesWithMoreThanFiveAccessors`：`NetworkInterrupts.cc:979-1005`。

`PTrack::lookup`：

```cpp
// NetworkInterrupts.cc:967-977
bool PTrack::lookup(uint64_t pp_base, uint64_t device_id) {
    int index = device_id - 2;
    auto it = evictedEntries.find(pp_base);

    if (it == evictedEntries.end()) {
        return false;
    } else {
        evictedEntries.erase(it);
        return true;
    }
}
```

已证明：

- `lookup` 不查 `permissionCache`，不检查权限，不检查 `device_id`。
- 它只查 `evictedEntries`，找到就删除并返回 true。
- 在 `setupverification` 中调用顺序是 `PT->access(...)` 然后 `PT->lookup(...)`：`NetworkInterrupts.cc:429-432`。这意味着“PT 命中/未命中”的实际语义由 `evictedEntries` 决定，而不是由 Permission Tracker 当前有效条目决定。

### 3.4 失效如何传送/确认/等待

在提供的文件中：

- `PTrack::findOrEvict` 淘汰时没有发送任何网络消息，只写本地 `evictedEntries`：`NetworkInterrupts.cc:931-936`。
- `PTrack::lookup` 只是消费 `evictedEntries`：`NetworkInterrupts.cc:967-977`。
- `NetworkInterrupts::RecvMessage` 处理 `key_change==1` 时，只做 `l2tlb->flushentry(vAddr)` 和 `setupWalk(... permission_update=1)`：`NetworkInterrupts.cc:340-349`。
- `startverification` / `finishTranslation` 发送的是 `LCACC_CMD_TLB_SERVICE`，不是 back-invalidation：`NetworkInterrupts.cc:521-562`，`NetworkInterrupts.cc:724-739`。
- `DMAController::OnNetworkMsg` 只识别 `DMA_MEMORY_REQUEST` / `DMA_MEMORY_RESPONSE`：`DMAController.cc:260-331`；没有 `LCACC_CMD_TLB_SERVICE` 或 invalidate opcode 的处理。

因此，在已读范围内：

- 没有找到 PT 淘汰触发的显式 back-invalidation 发送。
- 没有找到 invalidation ack 的接收、等待或状态机。
- 没有找到 `S_BCKInV` / `S_INFMem` / `S_INVALID` 之类的 transient state。
- 没有找到 global lock、shared TLB lock、drain wait。
- 只有 counter / latency / event 抽象：
  - `PT_hits` / `PT_misses`：`NetworkInterrupts.hh:367`
  - `PT_access`、`key_counter`、`l2tlb_hits/misses`：`NetworkInterrupts.hh:512-515`
  - `hostPTWLatency`、`PTrackLatency`：`NetworkInterrupts.cc:499-504, 661, 671-672`
  - `EnqueueEvent` / `SimicsInterface::RegisterCallback`：`NetworkInterrupts.cc:1124-1127`，`DMAController.cc:134-135`
  - `transferStatus = TlbWait/Running`：`DMAController.cc:206-207, 232-233, 107`；在提供文件里只写不读。

---

## 4. 已读范围内能证明的正面事实 vs 未找到

### 4.1 正面事实（提供文件内可证明）

1. 翻译有 ideal 即时路径和非 ideal 延迟路径：`DMAController.cc:112-137`。
2. 非 ideal 路径下 `translateTiming` 经 `hitLatency` 延迟：`DMAController.cc:134-135`。
3. 本地 TLB 命中仍走 `onTLBMiss` 回调，并进入 `MSHRs`：`DMAController.cc:219-226`。
4. 本地 TLB 在 job 变化、R/W transition、周期 5e9 tick 时 flush：`DMAController.cc:151-174, 176-195`。
5. `finishTranslation` 按 `vp_base` 完成所有 MSHR，插入 TLB，清零/置 Running，累计 `tlbCycles`：`DMAController.cc:88-110`。
6. `finishTranslation` 的 `MAC_return` 参数未使用：`DMAController.cc:89`。
7. `BeginTransfer` 禁止双 memory 端：`DMAController.cc:361`。
8. 一端 `-1` 走 `dmaInterface->StartTransferPrio`；双 SPM 走网络消息：`DMAController.cc:364-442`。
9. 单元素访问读/写各有一端 memory：`DMAController.cc:503-505`。
10. `PTrack::findOrEvict` 的 victim 公式是 `age - 5.0 * shareCounter`，选最大：`NetworkInterrupts.cc:920-928`。
11. `PTrack` victim 选择不使用 `deviceAccessStatus` / `pageSharerMasks`，只使用 `lruTick` 和 `shareCounter`：`NetworkInterrupts.cc:920-928`。
12. `shareCounter` 不是不同 sharer 数，而是非 lender 访问次数，且淘汰时不重置：`NetworkInterrupts.cc:931-936, 958-962`。
13. `PTrack::lookup` 只查 `evictedEntries`，不查 `permissionCache` 或权限：`NetworkInterrupts.cc:967-977`。
14. `PTrack` 淘汰时没有发送 invalidation，只记录 `evictedEntries`：`NetworkInterrupts.cc:931-936`。
15. `L2TLB::lookup` 忽略 `device_id`，尽管 `insert` 存储它：`NetworkInterrupts.cc:759-780` vs `783-811`。
16. `L2TLB_Scatter::insert` 有 `rand()%8` 随机替换，但 active 路径注释掉了 `L2TLB_Scatter`，实际使用 `L2TLB`：`NetworkInterrupts.hh:504-505`，`NetworkInterrupts.cc:1058-1061`。
17. `L2TLB_Scatter::flushentry` 使用 `(vp_base / PageBytes) % sets`，而 lookup/insert 使用 `hashAddress`/`getScatterSet`，索引不一致：`NetworkInterrupts.cc:1075-1086` vs `1007-1073`。
18. `ProtectionMemobj` 是简单的 blocking MemObject，只有 `blocked` 布尔流控，没有地址翻译或 Permission Tracker 逻辑：`ProtectionMemobj.cc:35-42, 165-209`；`Protectiontable.py:34-40`。

### 4.2 在提供文件中未找到（不能外推为整个 artifact 不存在）

以下均指“在这 7 个源文件中未找到”，可能在未提供的 `SimicsInterface`、`LCAccCommandListing`、`prefetcher-tile`、`TLBHack`、`System` 等文件中：

- `onTLBMiss` 回调的实际发送实现、打包顺序、`LCACC_CMD_TLB_MISS` 的 sender。
- `LCACC_CMD_TLB_SERVICE` 在 accelerator/DMA 侧的 handler，以及它如何调用 `DMAController::finishTranslation`。
- `TransferData` 的定义与 `getVaddr` / `setPaddr` / `isRead` / `MAC_ver` 的来源。
- `prftch_direct_interface_t::StartTransferPrio`、`StartSingleTransferPrio`、`Configure`、`finishTranslation`、`Prefetch` 内部行为。
- PT 淘汰触发的 back-invalidation 发送、flush ack 接收、等待队列、global lock、drain wait。
- paper 的 `S_VALID` / `S_BCKInV` / `S_INFMem` / `S_INVALID` 状态机。
- `PTrack::evictEntry` 定义（头文件声明于 `NetworkInterrupts.hh:380`，提供的 `.cc` 中没有定义/调用）。
- `L2TLB::flushAll`、`PTCache::flushAll`、`NetworkInterrupts::lookup/flushAll/evictLRU/insert` 的定义（头文件声明于 `NetworkInterrupts.hh:246, 347, 591-602`，提供 `.cc` 中未出现）。
- `mshr_entries` / `mshr_size` 的实际使用；在 `DMAController.hh:225-226` 和 `NetworkInterrupts.hh:516-517` 声明，但提供的 `.cc` 中未见使用。
- `transferStatus` 的读取；`DMAController.cc` 只写不读。

---

## 5. 与 paper C1 / AA-LRU 的可核对比

### 5.1 paper 中的 C1 公式

paper 文本：

- Access Mask、LRU、AA-LRU 说明：`sharemmu2026.txt:443-465`
  - “The Access Mask is a bitmap with one bit per accelerator...”
  - “On a miss, if no free entry is available... a victim score is computed...”
  - “The victim score equals the number of set bits in the Access Mask... combined with the entry’s LRU priority.”
  - “Additional weighting between the LRU component and the Access Mask count can be applied...”
  - “victim-way selection can be occasionally randomized...”
- Algorithm 1：`sharemmu2026.txt:469-483`
  ```text
  1: if PPN is present in Permission Tracker then  ▷ PT hit
  2:     update LRUbits;
  3:     AccessMask[deviceID] = 1;
  4: else                                           ▷ PT miss
  5:     for each entry x in the set do
  6:         VictimScore = (Way − LRUbits) + count(AccessMask);
  7:     end for
  8:     victim = argmin(VictimScore);
  9:     victim = random(way)                        ▷ at timestamp
  10:    replace victim;
  11: end if
  ```

paper 的 Secure Vector 字段：`sharemmu2026.txt:350-363`，包括 PPN、Access Mask、LRU、R/W、alias presence、alias counter、pointer to Alias Table。

### 5.2 artifact 中实际 PTrack 公式

`NetworkInterrupts.cc:920-928`：

```cpp
double age = GetSystemTime() - e.lruTick;
double score = age - 5.0 * e.shareCounter;  // tune weight = 5.0
if (score > worstScore) {
    worstScore = score;
    victimIndex = i;
}
```

即：

```text
code_score = age - 5.0 * shareCounter
victim = argmax(code_score)
age = now - lruTick
```

### 5.3 能核的差异

| 维度 | paper Algorithm 1 | artifact PTrack | 状态 |
|---|---|---|---|
| 分数方向 | `argmin(VictimScore)` | `argmax(score)` | 可核差异 |
| 分数组成 | `(Way − LRUbits) + count(AccessMask)` | `age - 5.0 * shareCounter` | 可核差异 |
| recency | `LRUbits` | `lruTick` / `age` | 有 recency，但编码不同 |
| mask 使用 | `count(AccessMask)` | 未使用 `deviceAccessStatus` / `pageSharerMasks` | 可核差异 |
| 共享计数 | Access Mask set bits | `shareCounter++` on every non-lender access | 可核差异 |
| 权重 | 正文说可调 | 固定 5.0 | 可核 |
| 随机化 | `victim = random(way)` at timestamp | PTrack 无随机 | 可核差异 |
| 淘汰后状态 | paper 未展开 | `lenderID/shareCounter/deviceAccessStatus` 不重置 | 可核正面事实 |
| PT lookup | 验证 PPN + permissions | 只查 `evictedEntries`，忽略权限 | 可核差异 |
| Secure Vector 字段 | PPN, AccessMask, LRU, R/W, alias, VPN | `pp_base, valid, deviceAccessStatus, lruTick, lenderID, shareCounter` | 缺少 VPN/alias/R-W 字段 |

### 5.4 随机化

- paper 允许 AA-LRU 偶尔随机 victim：`sharemmu2026.txt:463-467`，Algorithm 1 第 9 行：`sharemmu2026.txt:481`。
- artifact 中 PTrack 的 active 路径没有随机：`NetworkInterrupts.cc:907-928`。
- 提供文件中唯一明确的随机替换是 `L2TLB_Scatter::insert`：

```cpp
// NetworkInterrupts.cc:1053-1062
for (int i = 0; i < assoc; i++) {
    if (entries[set][i].free) {
        entry = &entries[set][i];
        break;
    } else {
        std::srand(static_cast<unsigned>(std::time(0)));
        int randomNumber = std::rand() % 8; // Random Replacement policy
        std::cout << "Evicted Ways : " << randomNumber << std::endl;
        entry = &entries[set][randomNumber];
        break;
    }
}
```

但 `L2TLB_Scatter` 在 active 配置中被注释掉：

```cpp
// NetworkInterrupts.hh:504-505
L2TLB* l2tlb = new L2TLB(1024,8); //IOTLB(entries, assoc)
//L2TLB_Scatter* l2tlb = new L2TLB_Scatter(1024,8);
```

因此这个随机替换不等于 paper 的 AA-LRU 随机化，也不作用于 Permission Tracker。

### 5.5 哪些定义在未提供文件

- `RubySystem::getPTrackAssoc()`、`RubySystem::numberOfAccInstances()`、`RubySystem::getPTrackLatency()`：决定 `PT` 的 `numSets` / `numWays` 和延迟；`NetworkInterrupts.cc:180-181` 调用了它们。
- `PTrack::evictEntry` 定义：`NetworkInterrupts.hh:380` 声明，提供 `.cc` 中未定义。
- `AccessMask` 的正式位宽/编码：paper 说 16-bit；artifact 用 `std::array<int,16> deviceAccessStatus` 和 `uint32_t pageSharerMasks`，但 victim 不用。
- `LRUbits` / `Way` 在 paper 中未展开为具体位编码；artifact 用连续 tick `lruTick`。
- `onTLBMiss` 回调如何把 `(vp_base, MAC_dma, pp_base, key_change)` 映射到 `NetworkInterrupts` 的 `(vAddr, pAddr, MAC, device_id, key_change)`。
- `device_id` 的实际取值范围与 `deviceAccessStatus[device_id-2]` 的边界检查；`pageSharerMasks` 只在 `device_id >= 2 && device_id < 34` 时写 `1U << (device_id-2)`，但 `deviceAccessStatus` 下标没有同样检查：`NetworkInterrupts.cc:944-947, 963`。

---

## 6. 最少下一文件清单（按优先级）

### P0：补全 TLB miss/service 回调链

1. `ShareMMU/src/modules/LCAcc/SimicsInterface.cc` / `.hh`
   - `SimicsInterface::RegisterCallback`
   - `LCACC_CMD_TLB_MISS`、`LCACC_CMD_TLB_SERVICE`
   - `onTLBMiss` 的实际 sender / 打包顺序
   - `DMAController::finishTranslation` 的调用点
   - `LCAccCommandListing`
2. `ShareMMU/src/modules/LCAcc/LCAccCommandListing.cc` / `.hh`
   - `LCACC_CMD_TLB_MISS`、`LCACC_CMD_TLB_SERVICE` 常量
   - `new DMAController(...)` 的调用点和传入的 TLBMiss 回调
   - 任何 `onTLBMiss` 包装器

### P1：确认 DMA 地址模式与翻译端

3. `ShareMMU/src/modules/linked-prefetch-tile/prefetcher-tile.hh` / `.cc`
   - `prftch_direct_interface_t`
   - `DMAEngineHandle`
   - `Configure`
   - `StartTransferPrio`
   - `StartSingleTransferPrio`
   - `finishTranslation`
   - `Prefetch`
   - `HookToMemoryPort` / `UnhookMemoryPort`
   - 目的：确认 memory↔SPM 是否只翻译 memory 端，是否可能双 memory 端。
4. `TransferData` 定义文件（搜索 `class TransferData`）
   - `TransferData::getVaddr`
   - `TransferData::setPaddr`
   - `TransferData::isRead`
   - `TransferData::MAC_ver`
   - 目的：确认是否有源/目的两个虚拟地址字段。

### P2：确认 PT/AA-LRU 配置与遗漏定义

5. `ShareMMU/src/mem/ruby/system/System.cc` / `.hh` 或配置脚本
   - `RubySystem::getPTrackAssoc`
   - `RubySystem::getPTrackLatency`
   - `RubySystem::numberOfAccInstances`
   - `RubySystem::getLCAccTLBSize`
   - `RubySystem::getLCAccTLBAssoc`
   - `RubySystem::getLCAccTLBLatency`
   - `RubySystem::getLCAccTLBMSHR`
   - `RubySystem::idealMMU`
   - `RubySystem::TLBHack`
   - `RubySystem::getHostPTWLatency`
6. `ShareMMU/src/modules/NetworkInterrupt/NetworkInterrupts.cc` 中搜索缺失定义：
   - `PTrack::evictEntry`
   - `L2TLB::flushAll`
   - `PTCache::flushAll`
   - `NetworkInterrupts::lookup`
   - `NetworkInterrupts::flushAll`
   - `NetworkInterrupts::evictLRU`
   - `NetworkInterrupts::insert`
   - 目的：确认是否有未展示的 invalidation/lock/drain 路径。

### P3：辅助

7. `ShareMMU/src/modules/TLBHack/TLBHack.hh` / `.cc`
   - `TLBHackInterface::PageKnown`
   - `TLBHackInterface::Lookup`
   - `g_TLBHack_interface`
8. `ShareMMU/src/arch/vtophys.cc` / `.hh`
   - `TheISA::vtophys`
9. `ShareMMU/src/modules/LCAcc/SPMInterface.hh` / `.cc`
   - `SPMInterface::Read`、`Write`、`GetID`

---

## 7. 不确定性与给 Codex 的直接核验点

1. **回调参数次序**：`DMAController` 调 `onTLBMiss(vp_base, MAC_dma, pp_base, key_change)`，`NetworkInterrupts` 解析为 `vAddr, pAddr, MAC, device_id, key_change`。需要看 `onTLBMiss` 包装器确认是否重排。若未重排，`pAddr` 与 `MAC` 语义会错位；但本文不能据此断言 bug。
2. **`PTrack::lookup` 的真实用途**：它只查 `evictedEntries`，不查 `permissionCache`。需要确认 `setupverification` 中 `PT->access` + `PT->lookup` 的语义是否就是 paper 的 PT hit/miss，还是局部实验逻辑。
3. **`shareCounter` 淘汰不重置**：提供的 active 路径 `findOrEvict` 不重置 `lenderID/shareCounter/deviceAccessStatus`。若 `PTrack::evictEntry` 或其他未提供路径会重置，则结论需修正；但当前提供的调用链没有调用 `evictEntry`。
4. **`deviceAccessStatus` 边界**：`pageSharerMasks` 只处理 `device_id < 34`，但 `deviceAccessStatus[device_id-2]` 对 16 元素数组没有范围保护。需要确认实际 accelerator 数量。
5. **无 global lock / drain wait**：这是“提供文件中未找到”，不是 artifact 全局不存在。`prefetcher-tile`、`SimicsInterface`、`TLBHack`、`System` 可能含有实际锁或等待。
6. **`LCACC_CMD_TLB_SERVICE` 处理**：`DMAController::OnNetworkMsg` 未处理该 opcode；需要下一文件确认 accelerator 侧谁接收并调用 `finishTranslation`。
7. **`MAC_return` 未使用**：`DMAController::finishTranslation` 形参 `MAC_return` 未使用。需要确认调用方是否只在成功时调用，还是 artifact 中验证结果被忽略。
8. **L2TLB device_id**：`L2TLB::lookup` 忽略 `device_id`，尽管 `insert` 存储。需要确认这是否影响跨设备共享/隔离判定。
9. **`L2TLB_Scatter` 非 active**：随机替换代码存在但被注释；若论文的随机化对应此结构，则实际评测配置未启用它。
10. **`ProtectionMemobj`**：它只是 generic blocking memobj，不承载 ShareMMU PT/AA-LRU 语义；不应把它当作 Permission Tracker 实现。

以上为只读证据整理；最终并发/创新裁定请 Codex 直接核。

本轮仅基于所给来源做只读证据补全；未执行、未构建、未上板。行号按你提供的文件编号。结论中“已证明”只指所给文件内可核；“未找到”不推出整个 artifact 不存在。

---

## 一、闭环与边界补全

### 1. onTLBMiss → pack → NetworkInterrupts → TLB_SERVICE → finishTranslation 实际闭环

**DMA 侧翻译入口与 miss 回调：**

- `DMAController` 构造时把 `beginTranslateTiming` 挂到 `dmaInterface`，并注册网络消息接收：`DMAController.cc:46-50`。
- 非 ideal 路径：`SimicsInterface::RegisterCallback(translateTimingCB::Create(this, td), hitLatency)`，即翻译不是立即完成，而是延迟到 `translateTiming`：`DMAController.cc:112-136`。
- `translateTiming` 三个分支都进 MSHR 并调 `onTLBMiss`：
  - key_change：`MSHRs[vp_base].push_back(td); onTLBMiss->Call(vp_base, MAC_dma, pp_base, key_change);`：`DMAController.cc:202-214`
  - 本地 TLB hit：`td->setPaddr(pp_base + offset); td->MAC_ver = 1; MSHRs[vp_base].push_back(td); MAC_dma = 1; onTLBMiss->Call(vp_base, MAC_dma, pp_base, 0);`：`DMAController.cc:219-226`
  - miss：`td->MAC_ver = 0; MAC_dma = 0; pp_base = 0; MSHRs[vp_base].push_back(td); onTLBMiss->Call(vp_base, MAC_dma, pp_base, 0);`：`DMAController.cc:229-240`

**LCAccDevice 侧处理并打包：**

- `LCAccDevice::Initialize` 创建 `DMAController` 时传入 `HandleTLBMissCB::Create(this)`：`LCAccDevice.cc:1164`。
- `HandleTLBMiss` 签名是 `(uint64_t addr, uint64_t MAC_code, uint64_t paddr, uint64_t key_change)`：`LCAccDevice.hh:226`，定义在 `LCAccDevice.cc:1027`。
- 打包为 12 个 `uint32_t`：
  - `msg[0] = LCACC_CMD_TLB_MISS; msg[1] = currentUser;`
  - `addr` 放 `msg[2..3]`，`paddr` 放 `msg[4..5]`，`MAC_code` 放 `msg[6..7]`，`netPort->GetNodeID()` 放 `msg[8..9]`，`key_change` 放 `msg[10..11]`
  - `netPort->SendMessage(currentUserProc, msg, sizeof(msg));`
  - 见 `LCAccDevice.cc:1030-1050`。

**NetworkInterrupts 侧解析并分支：**

- `RecvMessage` 处理 `LCACC_CMD_TLB_MISS`，按同一顺序解析：
  - `vAddr = args[2..3]`
  - `pAddr = args[4..5]`
  - `MAC = args[6..7]`
  - `device_id = args[8..9]`
  - `key_change = args[10..11]`
  - 见 `NetworkInterrupts.cc:277-301`。
- `MAC == 1` 分支：构造 `MACstruct`，`buffer[0] = LCACC_CMD_TLB_SERVICE`，填入 vAddr/pAddr/MAC/device_id/key_change，`MAC_verf.push_back(macstruct); setupverification();`：`NetworkInterrupts.cc:307-335`。
- `setupverification`：`PT->access(pAddr, device_id); PT_access++;`，然后 `if(PT->lookup(pAddr, device_id))`，并按 PT miss/PTrack latency 调度 `startverification`：`NetworkInterrupts.cc:401-519`。
- `startverification`：构造 `LCACC_CMD_TLB_SERVICE` 的 10-word `msg`，最终 `nih->snpi->SendMessageOnDevice(nih->deviceID, 0, msg, sizeof(msg));`：`NetworkInterrupts.cc:521-562`。
- `MAC != 1` 且 `key_change == 1`：`l2tlb->flushentry(vAddr); key_counter++; setupWalk(thread, vAddr, device_id, pAddr, permission_update);`：`NetworkInterrupts.cc:340-349`。
- `MAC != 1`、非 key_change、L2TLB hit：构造 9-word `buffer`，`buffer[0] = LCACC_CMD_TLB_SERVICE`，填 vAddr/pAddr，`buffer[6..8] = 0`，然后 `SendMessageOnDevice`：`NetworkInterrupts.cc:356-374`。
- `MAC != 1`、L2TLB miss：`setupWalk(thread, vAddr, device_id, pAddr, ver_req)`：`NetworkInterrupts.cc:379-383`。

**页表 walk 与回发 TLB_SERVICE：**

- `setupWalk`：`req->setVirt(...); req->SetdeviceID(device_id); req->SetVerficationRequest(permission_update); req->SetPaddr_Ver(pAddr); pendingTranslations.push_back(req); walkerstate();`：`NetworkInterrupts.cc:564-580`。
- `walkerstate` / `startWalk`：单 PTW 状态机，`currState == Ready` 才 `startWalk()`，`startWalk` 里 `currState = Waiting;`，取 `pendingTranslations.front()`，最后 `EnqueueEvent(HostPTWalkCB::Create(this, req), hostPTWLatency + RubySystem::getPTrackLatency());`：`NetworkInterrupts.cc:582-673`。
- `HostPTWalk` 调真实 `X86ISA::Walker::start(...)`：`NetworkInterrupts.cc:675-695`。
- `finishTranslation`：`l2tlb->insert(logicalPage, device_id, physicalPage);`，`PT_access++;`，构造 `LCACC_CMD_TLB_SERVICE` 并 `SendMessageOnDevice`：`NetworkInterrupts.cc:697-739`。若 `pendingTranslations` 非空，继续 `startWalk()`：`NetworkInterrupts.cc:744-746`。

**回到 accelerator，完成翻译：**

- `LCAccDevice::MsgHandler` 处理 `LCACC_CMD_TLB_SERVICE`：
  - 解析 `vAddr = msg[2..3]`
  - `pAddr = msg[4..5]`
  - `MAC_return = msg[6..7]`
  - `node_return = msg[8..9]`
  - 调用 `dma->finishTranslation(vAddr, pAddr, MAC_return);`
  - 见 `LCAccDevice.cc:559-574`。
- `DMAController::finishTranslation`：
  - 取 `MSHRs[vp_base]`，对每个 `td` 算 `offset = td->getVaddr() % TheISA::PageBytes; td->setPaddr(pp_base + offset); dmaInterface->finishTranslation(dmaDevice, td);`
  - `tlbMemory->insert(vp_base, pp_base); MSHRs.erase(vp_base);`
  - 若 `MSHRs.empty()`，`transferStatus = Running; tlbCycles += ...`
  - 见 `DMAController.cc:88-110`。

**闭环结论：** 在所给文件内，完整路径是 `DMAController::translateTiming → onTLBMiss → LCAccDevice::HandleTLBMiss → 网络消息 LCACC_CMD_TLB_MISS → NetworkInterrupts::RecvMessage → setupverification/startverification 或 setupWalk/finishTranslation → 回发 LCACC_CMD_TLB_SERVICE → LCAccDevice::MsgHandler → DMAController::finishTranslation → MSHR 完成 + TLB insert`。闭环成立。

---

### 2. 参数错位问题：撤销

之前担心 `pAddr` 与 `MAC` 错位。按所给来源，**未错位**，此前不确定性可撤销。

对齐关系：

- `DMAController` 传入顺序：`onTLBMiss->Call(vp_base, MAC_dma, pp_base, key_change)`：`DMAController.cc:214, 225, 239`。
- `LCAccDevice::HandleTLBMiss` 形参：`(uint64_t addr, uint64_t MAC_code, uint64_t paddr, uint64_t key_change)`：`LCAccDevice.hh:226`，`LCAccDevice.cc:1027`。
  - 因此 `addr = vp_base`，`MAC_code = MAC_dma`，`paddr = pp_base`。
- `HandleTLBMiss` 打包顺序：
  - `msg[2..3] = addr`，即 vp_base
  - `msg[4..5] = paddr`，即 pp_base
  - `msg[6..7] = MAC_code`，即 MAC_dma
  - `msg[8..9] = netPort->GetNodeID()`
  - `msg[10..11] = key_change`
  - 见 `LCAccDevice.cc:1030-1050`。
- `NetworkInterrupts` 解析顺序：
  - `vAddr = args[2..3]`
  - `pAddr = args[4..5]`
  - `MAC = args[6..7]`
  - `device_id = args[8..9]`
  - `key_change = args[10..11]`
  - 见 `NetworkInterrupts.cc:277-301`。

所以 `pAddr` 与 `MAC` 在打包和解析中均对齐。包装器 `HandleTLBMissCB` 只是成员函数绑定，没有看到重排。

补充：`DMAController::finishTranslation` 的形参 `MAC_return` 未被使用：`DMAController.cc:89, 110`；`LCAccDevice::MsgHandler` 的 `node_return` 也未被使用：`LCAccDevice.cc:568-574`。因此即使某些路径回发 `MAC=0`，对 `finishTranslation` 当前实现无影响。

---

### 3. memory↔scratchpad 一端翻译；可能的底层 memory-memory 支持

**TransferDescription 的表示：**

- `TransferDescription::MemoryDevice = -1`：`TransferDescription.hh:11`。
- 字段包括 `srcDevice/dstDevice`、`srcBaseAddress/dstBaseAddress`、`srcStride/srcSize`、`dstStride/dstSize`、`srcSplit/dstSplit`、`elementSize`：`TransferDescription.hh:13-23`。
- `ReadIn` 读入 `srcDevice`、`srcBaseAddress`、维度、`dstDevice`、`dstBaseAddress` 等：`TransferDescription.hh:82-119`。

**LCAccDevice 如何形成 TransferOrder：**

- `ParseTaskSignature` 中：
  - `src = td.srcDevice; srcBaseAddr = td.srcBaseAddress;`
  - `srcDimensionalityLower = td.srcSplit; srcDimensionalityUpper = td.srcSize.size() - td.srcSplit;`
  - `dst = td.dstDevice; dstBaseAddr = td.dstBaseAddress;`
  - 见 `LCAccDevice.cc:697-706`。
- 计算 `srcMoveSize` / `dstMoveSize`，并 `assert(srcMoveSize == dstMoveSize);`：`LCAccDevice.cc:740-750`。
- 填充 `transferSrcSPM.push_back(src); transferDstSPM.push_back(dst);`：`LCAccDevice.cc:751-760`。
- 每个 task 分类：
  - `if (t.dst.spm == spm->GetID()) reads.push_back(t); else writes.push_back(t);`
  - 见 `LCAccDevice.cc:895-911`。
- `EndNodeTransferData::IsSPM()` 返回 `spm != -1`，`IsMemory()` 返回 `!IsSPM()`：`LCAccDevice.hh:99-106`。

**DMAController 的硬约束：**

- `BeginTransfer` 内有：
  - `assert(srcSpm != -1 || dstSpm != -1);`
  - `assert(srcSpm == spm->GetID() || dstSpm == spm->GetID());`
  - 见 `DMAController.cc:361-362`。
- 因此通过 `DMAController` 不可能构造双 memory 端 `(-1, -1)`。
- 一端为 `-1` 时，走 `dmaInterface->StartTransferPrio(...)`：`DMAController.cc:364-368`。
- 两端都是 SPM 时：
  - `dstSpm == spm->GetID()` 分支发 `DMA_MEMORY_REQUEST`，本地登记 `localSignals`：`DMAController.cc:369-399`。
  - 否则 `assert(srcSpm == spm->GetID())`，尝试匹配 `remoteSignals`，否则登记 `localSignals`：`DMAController.cc:400-442`。
  - `OnNetworkMsg` 只处理 `DMA_MEMORY_REQUEST` / `DMA_MEMORY_RESPONSE`，用 `spm->Read` / `spm->Write`，不调用 `beginTranslateTiming` / `translateTiming`：`DMAController.cc:253-332`。
- 单元素访问：
  - `isRead ? -1 : mySPM` 作为 src，`isRead ? mySPM : -1` 作为 dst。
  - 见 `DMAController.cc:503-505`。
  - 读 = memory→SPM，写 = SPM→memory。

**DMAEngine / TransferData 端：**

- `TransferData::isRead()` 返回 `srcSpmID == NO_SPM_ID`：`DMAEngine.hh:55-58`。
- `getVaddr()` 返回 `(srcSpmID == NO_SPM_ID) ? srcLAddr : dstLAddr`：`DMAEngine.hh:70-73`。
- `DMAEngine::finishTranslation` 用 `td->isRead()` 选择 `ReadBlock` / `WriteBlock`：`DMAEngine.cc:381-452`。
- `ReadBlock` 是 `ReadMemory(...)` 后 `spmInterface->write(spm, spmAddr, ...)`；`WriteBlock` 是 `spmInterface->read(spm, spmAddr, ...)` 后 `WriteMemory(...)`：`DMAEngine.cc:269-287`。
- 因此翻译针对的是 memory 侧地址，数据一侧落在 SPM。

**memory-memory 支持判断：**

- `DMAController` 明确禁止双 memory 端：`DMAController.cc:361`。
- `DMAEngine::AddTransferSet` 本身没有 `-1/-1` 断言，但 `ReadBlock` / `WriteBlock` 都假定有一端是 SPM：
  - `ReadBlock` 总要 `spmInterface->write(spm, spmAddr, ...)`：`DMAEngine.cc:279-287`。
  - `WriteBlock` 总要 `spmInterface->read(spm, spmAddr, ...)`：`DMAEngine.cc:269-276`。
- 若直接绕过 `DMAController` 向 `dmaInterface` 传 `(-1, -1)`，`isRead()` 为 true，`finishTranslation` 会走 `ReadBlock`，并把 `td->dstLAddr` 当 SPM 地址写，而不是写另一段 memory。
- 因此：**在所给来源内没有建立 memory-to-memory 支持；DMAController 层禁止，DMAEngine 辅助函数假定单端 SPM。** 不能从“未出现某词”推出所有 variant 都不存在；但当前源码不构成 memory-memory 实现证据。

**包装器链路已补齐：**

- `prefetcher-tile.cc` 中：
  - `StartTransferPrio` 调 `x->ptd->AddTransferSet(...)`：`prefetcher-tile.cc:66-80`
  - `StartSingleTransferPrio` 调 `x->ptd->AddSingleTransfer(...)`：`prefetcher-tile.cc:100-110`
  - `Configure` 调 `x->ptd->Configure(...)`：`prefetcher-tile.cc:40-46`
  - `finishTranslation` 调 `x->ptd->finishTranslation(td)`：`prefetcher-tile.cc:48-52`
- 接口声明：`prefetcher-tile.hh:18-54`。

---

### 4. 读返回等待 / 缓冲 / 并发限制

**DMAEngine 的并发限制是请求数，不是字节数：**

- `issueWidth = RubySystem::getDMAIssueWidth();`：`DMAEngine.cc:104`。
- `TryTransfers`：`if (inflight >= issueWidth) { scheduled = false; return; }`：`DMAEngine.cc:458-461`。
- 每次发出翻译前 `inflight++`：`DMAEngine.cc:486-488`。
- `OnMemoryResponse` 中 `inflight--`：`DMAEngine.cc:502-508`。
- `System.hh:129` 提供 `getDMAIssueWidth()`；`System.cc:133` 从 `p->dma_issue_width` 赋值。实际值不在所给配置文件中，不能断言具体并发数。
- `TransferSetDesc::NextTransfer` 可能把同一 cacheline 内连续地址聚合成一个 `TransferData`，`aggSize` 可大于 `elementSize`：`DMAEngine.cc:55-89`。所以 **inflight 计数不等于未返回字节数**。
- `TransferSetDesc::pendingSize` 只用于该 transfer set 完成判断：`DMAEngine.hh:102, 119-123`；`DMAEngine.cc:44-53`。它不是准入缓冲。

**读写返回等待与同块串行：**

- `pendingReads` / `pendingWrites` 是以 `pBlockAddr` 为键的 map：`DMAEngine.hh:169-172`。
- 同方向同 `pBlockAddr` 已 pending：把回调 splice 到已有回调：`DMAEngine.cc:393-403`。
- 相反方向同 `pBlockAddr` 已 pending：把 `reFinishTranslation(td)` 追加到 opposite 回调，然后返回：`DMAEngine.cc:405-409`。
  - 这意味着同物理块的读和写不会同时下发，后到者要等先到者的 memory response 回调完成。
  - 这是同块总线下发顺序约束，不是“等整个 DMA job”或“等另一方向翻译”。
- `emitTime` 对每个 `pBlockAddr` 有 `assert(emitTime.find(pBlockAddr) == emitTime.end());`：`DMAEngine.cc:438-445`。这保证同一块只有一个 outstanding memory 请求。
- `IsReady`：
  - 非 redirect 到 memory：`seq->isReady(pAddr)`：`DMAEngine.cc:166-169`
  - redirect：`memInterface->IsReadReady/IsWriteReady`：`DMAEngine.cc:170-179`
  - 不 ready 时 `waitingTransfers.push(td); return;`：`DMAEngine.cc:426-428`

**缓冲能力：**

- `DMAEngine::IsBufferReady` 直接 `return true;`：`DMAEngine.cc:183-188`。
- `DMAEngine::MakeBufferCopy` 空实现：`DMAEngine.cc:246-251`。
- `td->buffer` 只用于分叉：
  - `if (td->buffer == -1)` 直接 memory 路径
  - `else` 走 shared buffer 分支，但实际 `MakeBufferCopy` 为空
  - 见 `DMAEngine.cc:416-425, 436-449`。
- `DMAController::SetBuffer` 只是 `buffer = buf;`：`DMAController.cc:455-458`。
- `buffer` 被传给 `StartTransferPrio` / `StartSingleTransferPrio`：`DMAController.cc:365-368, 503-505`。
- 结论：**在所给 DMAEngine 范围内没有真实的缓冲容量、信用、释放模型；`IsBufferReady` 恒真，`MakeBufferCopy` 空操作。**

**LCAccDevice 的 action 级读并发：**

- `StartTask` 仅在 `awaitingRetireActionSet.size() < spmWindowCount && !pendingActionSet.empty() && pendingReadSet.empty()` 时启动下一个 action：`LCAccDevice.cc:441-452`。
- `EmitActionReads` 把 action 推入 `pendingReadSet`，设置 `a->readsInFlight = a->readSet.size();`，然后为该 action 的所有 read 调 `dma->BeginTransfer(...)`：`LCAccDevice.cc:60-84`。
- `FinishedActionRead` 递减 `readsInFlight`；到 0 时 `pendingReadSet.pop(); StartTask(); EmitActionCompute(a);`：`LCAccDevice.cc:92-117`。
- 因此：**同一时刻只有一个 action 的 read set 在飞；单个 action 内可有多个 read transfer 并发；但没有按字节的 buffer 准入。**

---

### 5. 因缺文件造成的不确定性更新

**已因此轮文件解决：**

- `TransferData` 定义已见：`DMAEngine.hh:30-84`，包括 `isRead()`、`getVaddr()`、`setPaddr()`、`getPaddr()`、`MAC_ver`、`buffer`。
- `prftch_direct_interface` / `DMAEngineHandle` 包装器已见：`prefetcher-tile.hh:14-54`，`prefetcher-tile.cc:40-164`。
- `DMAController::finishTranslation` 的调用点已见：`LCAccDevice.cc:574`。
- `LCACC_CMD_TLB_SERVICE` receiver 已见：`LCAccDevice.cc:559-574`；`DMAController::OnNetworkMsg` 不处理该 opcode：`DMAController.cc:253-332`。
- callback 参数错位问题：已按打包/解析对齐，撤销。
- memory↔SPM 单端翻译：已见 `DMAController.cc:361-368, 503-505`；`DMAEngine.hh:55-73`；`DMAEngine.cc:269-287, 381-452`。
- SPM↔SPM 无翻译：已见 `DMAController.cc:369-442, 253-332`。
- memory-memory：所给 `DMAController` 禁止；`DMAEngine` helper 假定 SPM 端。未看到实现。

**仍缺 / 仍需下一文件或 Codex 核：**

- `SimicsInterface` 实现：`RegisterCallback`、`GetSystemTime` 等。
- `TLBHack`、`SPMInterface`、`RubySystem::getPTrackLatency`、`getLCAccTLB*` 的实际配置。
- `PTrack::evictEntry` 定义；`L2TLB::flushAll`、`PTCache::flushAll`、`NetworkInterrupts::lookup/flushAll/evictLRU/insert` 定义。
- `DMAEngine::protectionTableAccess` 只有声明：`DMAEngine.hh:214`，所给 `.cc` 未见定义。
- 实际 `dma_issue_width`、`buffer` 模式、`memObject` / `memInterface` 是否启用。

**两个 source-level 包长/边界风险，建议 Codex 核：**

1. `NetworkInterrupts::RecvMessage` 对 `LCACC_CMD_TLB_MISS` 只写 `assert(size >= 10);`，但随后读 `args[10]`、`args[11]`：`NetworkInterrupts.cc:279, 299-301`。当前 `LCAccDevice::HandleTLBMiss` 发 12 个 word（48B），所以此发送者下够用；但断言过弱。
2. `LCAccDevice::MsgHandler` 的 `LCACC_CMD_TLB_SERVICE` 读 `msg[8]`、`msg[9]`：`LCAccDevice.cc:568-574`。`NetworkInterrupts::startverification` 发 `uint32_t msg[10]`：`NetworkInterrupts.cc:542-561`；而 L2TLB hit 分支和 `NetworkInterrupts::finishTranslation` 发 `uint32_t buffer[9]`：`NetworkInterrupts.cc:356-374, 724-739`。若这些 9-word 包也送到 `LCAccDevice`，`msg[9]` 读越界。需核 `SendMessageOnDevice` 是否有补充/padding。

---

## 二、更强普通基线审计

你提出的规则：

> 发出读 R 前，保证空闲缓冲至少覆盖所有已发尚未返回读的总字节；停止发新读后，所有 R 可在不等待后续写 W 翻译的情况下被接收。

### 1. 来源是否已覆盖该规则

**未覆盖“按字节空闲缓冲覆盖未返回读”的部分。**

- `DMAEngine` 只有 `inflight` 计数与 `issueWidth`：`DMAEngine.cc:104, 152-153, 458-461, 486-488, 502-508`。
- 没有 `freeBufferBytes`、`outstandingReadBytes`、per-read byte credit。
- `pendingReads` / `pendingWrites` 只按 `pBlockAddr` 存回调，不按字节计费：`DMAEngine.hh:169-172`。
- `TransferSetDesc::pendingSize` 是该 transfer set 完成计数，不是空闲缓冲准入：`DMAEngine.hh:102, 119-123`；`DMAEngine.cc:44-53`。
- `IsBufferReady` 恒真；`MakeBufferCopy` 空操作：`DMAEngine.cc:183-188, 246-251`。
- `LCAccDevice` 按 action `readSet.size()` 并发，不是按字节：`LCAccDevice.cc:60-84`。

因此，该规则在来源中不是已有准入条件，而是更强/新增条件。

**“停止发新读后所有 R 不等待后续 W 翻译”部分：部分成立，但有同页依赖。**

- 不同 `vp_base`：`DMAController` 的 `MSHRs` 是按 `vp_base` 的 map；`finishTranslation(vp_base)` 只完成该 `vp_base` 下的 list：`DMAController.cc:90-110`。不同页的 R/W 翻译互不等待。
- 同一 `vp_base`：R 和 W 都进入 `MSHRs[vp_base]` 同一 list；一次 `finishTranslation(vp_base)` 遍历并完成 list 中所有 `td`：`DMAController.cc:90-100`。所以 R/W 同页时仍要一起等该页翻译。
- `DMAEngine` 层还有同 `pBlockAddr` opposite-direction 串行：后到者被 `reFinishTranslation` 挂到先到者回调之后：`DMAEngine.cc:405-409`。这不是等 W 翻译，而是同块 memory 访问顺序。
- 没有全局“停止读直到写翻译完”的门。`TryTransfers` 从 read/write 共用优先队列取，排序只按 `priority` 和 `timeStamp`：`DMAEngine.hh:75-82, 174-175`；`DMAEngine.cc:465-479`。

### 2. 是否已覆盖先前 C2 条件环

所给来源没有 C2 的定义或标识符，无法直接审计“C2 条件环”是否被覆盖。

若 C2 指 `DMAEngine::finishTranslation` 中 `pendingOpposite` 的条件环：

- 来源已有该环：`pendingOpposite[pBlockAddr] = SpliceCB::Create(this, pendingOpposite[pBlockAddr], reFinishTranslationCB::Create(this, td)); return;`：`DMAEngine.cc:405-409`。
- 但它实现的是同物理块反向访问串行，不是“空闲缓冲覆盖未返回读字节”的准入。
- 因此不能据此说更强规则已被覆盖。若 C2 另有所指，需要提供 C2 原文/代码。

### 3. 真实代价

- 需要新增按字节的 outstanding read 统计和 free buffer 统计；来源没有。
- 来源的 `issueWidth` 是请求数限制，而 `TransferData` 可聚合多个 element 成更大 `aggSize`：`DMAEngine.cc:55-89`。因此“请求数够”不等于“字节缓冲够”；按字节规则会改变准入行为。
- 若把“完整 copy buffer”当唯一基线，成本是容量至少覆盖最大未返回读字节；但这不是唯一基线。2018 工作用 metadata retirement buffer 64B 对比 16KiB data buffer：`socdma2018.txt:550-569`。因此 full copy buffer 只是其中一种代价选择。
- 若缓冲释放不被建模，规则可能永远不阻塞或错误阻塞；当前 `IsBufferReady` 恒真，无法提供释放事件。
- 不要把应用完成当总线完成：
  - DMAEngine 的 `TransferSetDesc::DecrementPending` 在 `!MoreTransfers() && pendingSize == 0` 时 `ScheduleCB(0, onFinish)`：`DMAEngine.cc:44-53`。这是该 transfer set 的 DMA 完成。
  - `LCAccDevice` 的 action retire 和 `LCACC_CMD_TASK_COMPLETED` 在 `RetireAction`：`LCAccDevice.cc:405-433`。这是更晚的应用/任务完成。
  - 二者不能混为同一“总线完成”。

---

## 三、收窄候选卡（仅此一张）

**候选名：buffer-release-aware translation scheduling**

| 项目 | 内容 |
|---|---|
| 目标 | 在翻译服务完成顺序上，优先完成那些完成后能释放受阻 DMA 缓冲或跨通道返还信用的翻译；从 memory-to-memory paired read/write 场景出发。 |
| 与 A1 的区别 | A1 是应用消费者关键路径/消费者 slack；本候选在翻译服务完成选择，利用“完成哪个翻译能解开哪个缓冲/信用”的依赖。 |
| 与普通限流的区别 | 普通限流在源端准入/发请求节奏；本候选不改变请求数，只改翻译完成顺序。 |
| 当前裁定 | **来源不足，只标候选，不绿色通过。** 不假定收益或新颖性。 |

### 具体判据

1. **至少 2 个待选 miss，且有调度自由度。**
   - 来源：`NetworkInterrupts` 是单 PTW、FIFO 状态机：`pendingTranslations` 是 `std::list`，`currState` 只有 `Ready` / `Waiting`，`startWalk` 一次处理一个，`finishTranslation` 后才继续下一个：`NetworkInterrupts.cc:582-599, 697-747`。
   - 若只有 1 个 pending miss，非抢占单 PTW 没有选择自由度。

2. **被优先翻译的 miss 必须能释放可识别的受阻缓冲/信用。**
   - 来源没有 buffer/credit 模型：`IsBufferReady` 恒真：`DMAEngine.cc:183-188`；`MakeBufferCopy` 空：`DMAEngine.cc:246-251`；只有 `inflight` 计数：`DMAEngine.cc:152-153, 458-461`。
   - 若没有外部 buffer state，规则不可判定。

3. **决策必须用“完成哪个翻译释放谁”的依赖，而非仅 R/W 方向或 occupancy。**
   - 若只看读/写方向、criticality 或占用率，可能只是普通 write priority / criticality / occupancy priority 的重命名。
   - 来源没有翻译优先级队列；`MSHRs` 是按 `vp_base` 的 map，`finishTranslation` 按服务到达完成整个 `vp_base` list：`DMAController.hh:143`；`DMAController.cc:90-100`。这不能直接提供“按释放收益排序”。

4. **当前 4KiB full buffer 单 copy 是否排除收益。**
   - 来源：当前 batch 每 lane 至多一个逻辑 4 KiB copy 在途：`platform-feasibility-20261006.md:31`；主翻译服务是单 PTW：`platform-feasibility-20261006.md:36`；顶层配 `JOB_FIFO_DEPTH=8, NUM_AX_IN_FLIGHT=8, BUFFER_DEPTH=512`：`platform-feasibility-20261006.md:32`。
   - 若单个 4KiB copy 只产生一个翻译 miss，则没有至少 2 个待选 miss，收益被排除。
   - 若 src/dst 成对产生两个 miss，则可能有顺序自由度；但来源未建立 buffer/credit 释放关系，不能假定收益。需以实际 miss 数和 buffer/credit 语义为准。

5. **与 2018 metadata replay / 预锁 TLB 前作的关系。**
   - 2018 hybrid IOMMU：miss 时回错误并 drop transaction，DMA engine 记录 failed burst，之后 reissue；retirement buffer 存 metadata，不存数据；64B 对 16KiB data buffer：`socdma2018.txt:376-417, 550-569`。
   - 旧 SoA 需要预锁 TLB 条目，限制并行 DMA：`socdma2018.txt:383-392, 483-488, 495-499`。
   - 若本候选只是 failed burst replay、drain 后重发或普通 issue control，已被近邻覆盖。
   - 只有“数据缓冲/信用释放依赖”才是可能差异；但本来源的 DMAEngine 没有数据缓冲/信用模型。故不能宣称与 2018 有明确差异。

### 反事实

构造一个**同请求数、同写读比例、同 FIFO 占用**，但依赖关系不同、可能需要不同选择的来源级反事实：

- 设 2 个 pending miss：A 和 B。请求数相同，W:R 比例相同，FIFO 占用相同（都是 2 个 pending）。
- Case X：A 和 B 的 `vp_base` 相同。`DMAController::finishTranslation(vp_base)` 会遍历 `MSHRs[vp_base]` 中所有 `td` 并完成：`DMAController.cc:90-100`。一次翻译完成可释放两个等待者。
- Case Y：A 和 B 的 `vp_base` 不同。一次 `finishTranslation` 只完成一个页的等待者：`DMAController.cc:90-110`。
- 若候选目标是“释放受阻 buffer/credit”，则 Case X 与 Case Y 可能有不同优先选择。此反事实只证明依赖关系不同，不证明 buffer-release-aware 一定有正收益。

### 停止条件

- 只有 R/W 方向或 occupancy 可依据，不能区分依赖 → 视为优先级重命名，停止。
- 只有 1 个 pending miss → 无调度自由度，停止。
- 没有可释放 buffer/credit 模型 → 停止。
- 2018 metadata replay / 普通 criticality / occupancy priority 已覆盖相同决策 → 停止。
- 构造不出“同请求数/同 W:R/同 FIFO 占用但不同依赖”的反事实 → 停止。
- 若 current 4KiB full buffer single copy 只有单 miss → 当前配置下排除收益。

---

总体：第一部分闭环和参数对齐已补齐，memory↔SPM 单端翻译成立，memory-memory 在所给来源内无实现证据；第二部分更强普通基线未被现有字节缓冲准入覆盖，且不能把 full copy buffer 当唯一基线，也不能混淆 DMA 完成与应用完成；第三部分只形成一张候选卡，判据指向“至少 2 个 pending miss + 可释放 buffer/credit + 依赖而非方向/占用”，来源不足以绿色通过。最终架构/接受归 root/Codex。

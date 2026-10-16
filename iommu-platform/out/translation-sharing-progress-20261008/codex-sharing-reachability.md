# §46 root：共享结果与续走保护的真实性

root Codex亲自设计下面的合法构造、直接核源及官方规范；DeepSeek仅补接口证据，两名Codex席独立质疑。只读/纸面构造，未运行模型或DUT、未改生产。

## 1. 当前平台并非统一存储池

`rv_iommu_tl_wrap.sv:657–685`实例化IOTLB，`:759–804`实例化独立PTW。`rv_iommu_iotlb.sv:58–71,464–473`的tags/content/PLRU和`rv_iommu_ptw.sv:661–696`的执行state/levels/iova/gpa/tablepointer/leaf寄存器分开。最终PTW update写入IOTLB，不把活跃进度停放到IOTLB entry。正常PTW是一条walk，没有共享PWC/MSHR waiter数组或续走帧池；N路径另接缓存命中，不接第二笔miss去共享PTE读取。

这些模块可能争lookup服务、TL入口和外部内存，但不能据此改称同一缓存槽的保护/复用竞争。若以后合并存储，那是新组织下的约束，不是旧板已测出的瓶颈。

## 2. 最新实际fixture确实不共享

root直接读`out/dual-write-prefix-20261008/multi_test.cpp:913–948`最新natural初始化：V的PSCID10/root0x20000→0x40000→0x50000；A的PSCID11/root0x30000→0x60000→0x70000；G-stage Bare。V root PTE0x20000、下一PTE0x40400，A为0x30000/0x60800；leaf arrays也分开。`tests/composition_tl_test.cpp:342–371`和旧actual DMA fixture同类。旧实验不是nested15步、跨设备共享前缀或shared-progress池实验。

## 3. 一个合法共享构造（未执行）

沿用composition TL测试的V页表内容，让DID11的DC也引用PSCID10、相同S-root0x20000，G-stage同为Bare；DDT的DID条目仍分别位于0x10280/0x102C0。相对于原fixture，仅构造中DID11的ta=0xA000、fsc=0x8000000000000020，原test不修改。

两设备分别读取IOVA0x10000000与0x10001000：

| 步 | DID10/P0 | DID11/P1 |
|---|---|---|
| S-root PTE | 0x20000，值0x10001→下表0x40000 | 相同 |
| 中间PTE | 0x40400，值0x14001→下表0x50000 | 相同 |
| 4KiB leaf PTE | 0x50000，值0x200000DF | 0x50008，值0x200004DF |
| 最终SPA | 0x80000000 | 0x80001000 |

索引是Sv39 VPN2=0、VPN1=128、VPN0=0/1；8字节PTE得到0x40400和不同leaf。这里可以共享两笔上层PTE结果，不能把两个最终翻译合成一个leaf verdict；普通partial cache/coalescer+各请求原IOVA/身份即可保留差异。若两设备都读P0，当前IOTLB本身就能按相同PSCID/VPN命中同一条目，DID不在其tag中。

共享前提是静态、同一合法地址空间和相同解释/权限语义；不同context仅物理地址相同不自动许可共享最终结果。root实读[RISC-V IOMMU v20260222简介](https://docs.riscv.org/reference/iommu/v20260222/iommu_intro.html)的context共享说明与[数据结构](https://docs.riscv.org/reference/iommu/v20260222/iommu_data_structures.html)的DC.ta.PSCID、DC无PC时stage roots和cache tag表。PSCID/GSCID标识地址空间以支持复用；不能把不同页表配成同一ID制造错误‘共享’。此处配置同root同page tables且无PC，没有利用错误配置。

## 4. nested共享仍须保留每请求差异

两席构造T_A/T_V的GPPN高11bit相同、低18bit不同：它们的G-root PTE地址/返回Q可以相同，下一G2/G3目标却不同，且T来自已经读取的S nonleaf。合并Q不代表原IOVA/C可以重建各T；suffix/phase/attrs/fault route仍需分别保留。原始请求、共享key/value/ref保护及各自Info都计费，状态移动到message/MSHR不等于消失。

root实际读NB-Walker作者固定commit `8837da7dda5591837877d20d4ad51b042c988aad`的`walkermshr.go:17–78,106–120`、`walkerStage.go:118–143,194–210`、`directory.go:173–179`、`respondstage.go:47–61`、`NBWalkerMMU.go:558–580`：同PID/PTEBlock共享fetch、多个独立transaction/Info返回、data/PTW同MSHR容量及PTW预约已经具体存在。因此共享数据＋各请求状态＋普通预约不能称独立机制。它不是Sv39双阶段fault/面积或每DMA严格公平性的定理，不能偷换成那些保证。

## 5. 判定

存在合法共享场景，但当前平台没有所假设的进度与缓存统一槽，普通one-result/per-request-state和分槽/保护已能实现列明能力。本合同没有发现这些普通方法做不到的成本点，停止以此组合立机制，不构建新统一池来制造阻塞。

额外真实边界是共享IOTLB命中下的requester history，见`codex-requester-history-boundary.md`；这个边界也须与直接前作比较，而不是转安全或独立创新命名。

# E6-PN：独立 N 核心有限验证

2026-10-18 JST；Codex-only。生产组件 **`755cd723b20457418734700fae9e8d6f91d988c8`**，直接父提交已接受的 P `d2a813d67bcfd0295d8934c3cc027d24d3195d63`。组件树 `/home/xvhaocheng/work-e6-pn-iommu`、分支 `ispass/e6-pn`；未接 CVA6/backend/top、未构建 FPGA、未访问板卡。

本批仅 7 文件：`Bender.yml`、banked_ctl、nonblock_hit、原 trans_ctl 的可选 read-admission、DDTC/IOTLB 真 accept touch，以及共享 TL 必要候选/响应接口。两个新模块来源固定 donor **`0b7939445298a23ea3e131ffd4e003cebb15ac59`**；[donor 原件及逐行差异](../component-source/n-core01/donor-delta.patch)保留原资格条件、W-owner/WLAST 与 fullbuffer DID 条件，只移除 E3/诊断，增加显式受限私有 bank 配置。没有整栈 cherry-pick。已接受 P 的 2/14 mask/local-history 不变，尚未加入后来要求的 P_shared 接口。

[恢复目录](../component-source/n-core01/)保存七份实际字节、完整 patch、增量 Git bundle 和 verify 原输出。bundle 唯一 prerequisite 为父 P commit；未创建远端或推送第三方 upstream。12 份 package、PTW/DDTW/MSIPTW/MRIFC、DS IF、top/wrap 与 `85b157ebe71a623e326e51fff7b7126fe77e8eb9` **逐字节相同**，见 [unchanged-baseline.json](../component-source/n-core01/unchanged-baseline.json)。

## 实际生产调用边界

- TL `rv_iommu_tl_wrap.sv:279–337`：compile-default `NonblockEnable=0`；runtime N=0 直接恢复原 request normal gate、spill 预取和 slow response ready/data/valid。RR 只在 N=1 使用；slow request 只在 slow handshake 退休，fast response 不误退休 walker。
- TL `:298–319`：N 窗口必须原 PT 状态、S1 walker 活跃、S2 Bare、另一 bank、非 implicit/ATS/debug；真实候选进入 helper，真实 cache lookup 复用、真正 accept/touch 分离。`:363` 的 DDTW init 保留原 DDT/ATS 来源，不能因 N DDTC probe 启动 walk。
- helper `rv_iommu_nonblock_hit.sv:62–107`：原正常 FLAT context 可以存在；候选输入和 cached leaf GPA 均排除实际 MSI 范围。cache DC 和 leaf 必须真 hit、Sv39/4KiB、权限及 PID/priv/debug/规范地址等检查成立；candidate/DDTP/DC 不变且无 invalidation 才 accept。probe 不 touch，accept 同拍恰一次 touch 两真实 cache。
- TL `:761、1398–1405`：仅 `NonblockEnable && nonblock_enable_i && restricted_bank_i` 的 MSI 状态禁止 MSI PTW init，按原 DTF/ERROR/FQ 路径发 error260、is_mrif0、ignore0；S/P 与 global MSI capability 不降级。私有 bank 仅 `RestrictedNormalOnly=1` 时从 global MRIF cfg 派生 BT_ONLY；原默认参数仍拒绝没有该显式约束的 MRIF bank。
- banked `rv_iommu_banked_ctl.sv:244–280、332–371`：immutable 原 AXI ID bit 选 bank；原 W-owner signed balance/read-admit、真实 FullBufferDID 例外接口不改。当前 harness 驱动许可位，**不是 DMA full-copy certificate**。真正 frontend→jobFIFO fire/retire 证书与双 backend 留到后批。

这些是有限模块证据，不是整栈 S 等价、性能复现或论文板点。TL 和 banked **分别**验证，尚未证明已连接的 TL error→banked SLVERR 全链。

## 有限验证与原始输入

最终 TL run08 为 4773 周期、173 条验证记录、32577 项检查、4 次真 N accept、32 probe；banked run03 为 2137周期、11情境、2220项检查。实际计数与 source-commit 字节绑定在 [summary.json](summary.json)。原 manifest 保留执行当时 HEAD/dirty 差异与实际输入哈希；没有把运行时 HEAD 事后重写成归档 commit。

- TL 最终检查使用真实 TL/DDTC/DDTW/PTW/IOTLB、原 MSI_BT_MRIF/PC 和扩展 DC。gold 是原 85b DDTC、IOTLB、TL，除了 module 标识和 TL 的两个 cache 实例绑定重命名外无改动。每拍核 ready、完整响应、fault/HPM/四组 walker AXI，且核 DDTC/IOTLB tags/content/PLRU。N=0 的 48 正常请求加 2 笔 PTW 忙时 spill 预取逐拍对照，防止 donor 编译 N 后无条件 serialize 原 S。
- 真缓存 N：hot V 在 A 冷 walk 期间接受；探测不改两 cache PLRU。held fast response 在 slow response 到达后仍稳定；连续两真命中各只计一次。13 个真实缓存排除情境见下表。另核 miss/RO write/2MiB/RX 候选持续 held VALID 后真正回到 normal TL，不把拒绝后撤回当 fallback 通过。
- 受限 MSI：实际 DC 是 S1 Bare + S2 Sv39x4、FLAT、MSI 输入匹配，实际 TL 进入 MSI，DTF0 保持真实 FQ record 后回复 error260，DTF1 无 FQ 直接 error260。零 PTW AR、零 MSI init/AR、零 IOTLB fill、is_mrif0。此处 S2 只为有限负测进入真实 MSI 状态，不扩大 N 支持域。
- banked 最终用目标全局 8 outstanding / W FIFO32，分别分成 4/16；11 情境覆盖两 bank、ID 恢复、错误 SLVERR、响应重排、同 ID 四槽容量、completion 背压、W-before-AW、单/多拍读、owner/read-admission、许可 DID 匹配。AXI completion 和 TL response 是明确的有限 test models。最后 admission 负例故意保留 A 写未完成，不能称全样例均 drain；其他 drain 断言按真实情况检查。

| 排除 scenario | 实际变化/被拒绝原因 |
| --- | --- |
| 0 | 真 cache RO leaf 上 write |
| 1 | 真 cache 2MiB leaf |
| 2 | 未缓存的 4KiB 候选 |
| 3/4/5 | PID valid / privilege / is_debug |
| 6/7 | probe 期间 candidate 更换 / DDTC invalidation |
| 8/9 | 真 cached leaf GPA 在 MSI 范围 / 输入在 MSI 范围 |
| 10/11/12 | probe 期间 DDTP 变化 / 非规范地址 / candidate 撤回 |

C++ 原始输入及 cycle CSV 明确记录所有上述字段；没有注入 cache 内容或改写 walker/RTL 来制造 hit。C++ memory model 只返回所列真实 DDT/PTE bytes、有限 AXI backpressure；`cases.csv` 是验证记录，不能作为论文 trial n。

## 失败保留与局限

[evidence](evidence/)保存每次实际 source closure、原命令 manifest、原 build/run log、逐拍 CSV 的无损 gzip。gzip 解压后哈希须仍等于原 manifest；debug 目录沿用 defaultoff01 source，另存真实 command/backtrace 与明确 derived 的 tree excerpt。

- `n-defaultoff01`：Verilator 5.040 内部 ATTROF dtype 故障。准确 gdb 定位到新增默认值 `touch_i` 内联 placeholder。改内部 cache 端口为显式连接、由默认关闭的 `TouchEnable` 保持旧行为后，`n-defaultoff02` 原 TL 回归通过。未把内部故障称时序/功能失败。
- `n-defaultoff01-debug` 曾因安装缺 5.040 debug binary 误选 PATH 中 4.038；`debug5` 首次直接 gdb 又缺实际 VERILATOR_ROOT。两份错误工具环境原日志保留。`debug5-root` 明确环境后才是准确 5.040 backtrace；没有升级/篡改工具缓存。
- `n-tl01/02`：test-only hierarchical MSI init tap 的 generate 路径名不正确，原编译诊断保留；改为实际 `gen_msi_support_tl.msi_ptw_init`。
- `n-tl03/04`：负测前提错误：S1 Sv39/S2 Bare 不会按所假设直接进入原 MSI 状态；03 输入缺普通 S1 PTE，04 S1-only leaf 实际完成普通翻译，未产生预期 FQ。原 PTW `:438–449` 仅在 en2 分支上发 GPA-MSI 事件，原件未改。修正为上述实际可达的 S2 负测后通过。**不声称 N 透明支持任意 MSI/MRIF，更不能以这些负测代替真实 payload leaf 预检。** S1-only 实验域仍须既定 prepare/readback 排除输入/leaf MSI；helper 的实际缓存 input/leaf 保护另有场景8/9通过。
- `n-tl05/06/07` 先前已过的有限范围均保留；后续仅为新增响应竞争、13排除、完整输入 trace、缓存状态 gold 和 normal fallback 增覆盖，未修改生产 RTL。`n-banked01/02` 的更小容量(4/8)旧检查保留；最终 03 改目标8/32并扩为四槽测试。

编译统一 finite build180s/run30s、`-j2 --assert -Wall -Wno-fatal`，拒绝覆盖输出。原 upstream width/unused、原 DDTW `en_msi_check` UNOPTFLAT 等告警完整保留；N=0 gold 使用 bank-extended test type 时未驱动 bank bit 的告警是 test metadata，实际该序列 bank=0。当前无综合/资源/timing 合格声明，后续仍必须既定 DS0 routed 硬门和 S15 实板准入。

## 复查入口

```sh
python3 paper/ispass2027/exp/E6-pn/n-validation/check_evidence.py
```

该检查只验归档哈希、源码/原 gold 绑定与 trace 合同，不重新仿真。需要新有限执行时，`run_n_tl.py` / `run_n_banked.py` 都接受 `--iommu-root --deps-root --verilator --out`；完整实际命令在原 manifest，须用全新输出目录。

没有 Linux、CVA6、backend、top、mode/quiet/flush 控制、gap、被动新 observer 的改动；这些各有独立后批/独占席。本批不改后来要求的 history 开关。实际模型/原生上下文元数据接口未暴露，未修改模型缓存或窗口设置；DeepSeek 未参与。

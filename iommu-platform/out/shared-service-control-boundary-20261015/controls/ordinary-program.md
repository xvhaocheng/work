# 普通 current-VALID DS 入口优先：有界硬件设计与服务边界

结论：可以在现有共享 DDR AR 仲裁所有者处构造一份可综合的普通逻辑：在该所有者首次 offer 前，只优先已经 current-VALID 的 DS，四次有竞争的 DS 接受后给非 DS 一次 RR 逃逸；已 offer 的 held 请求及所有下游债务继续正常 drain。这是具体的新硬件构造，**不是当前 bit 已有的 DS 优先，也不是已接受读的返回提升**。设计的合法接入位置已找到；没有实施、综合、时序或板测，没有完整任务独有收益。

本交付只审查源码与合同。原参考、旧 pack、生产代码和共享文档均未修改。所有新测量为零。`evidence.json` 保存原文、行号和 SHA-256；`cost.json` 把设计可数的量与未测量费用分开。

## 1. 实例、owner 与输入

唯一硬件参考仍是 2026-10-07 Genesys2 composition 目录，绑定限度沿用 `out/ddr-service-boundary-20261014/root/profile-binding.json:254–294`。它不代表当前 Dell 板的加载身份。原 `O-RR-DSAR` 合同见 `out/pte-residual-feedback-20261014/root/four-path-proof.md:7–13`：NormalTL RR/NativeN、入口 O-DS-AR 和条件 memory-law 的返回 DS 优先是三个不同部件，不能把最后一项当成真实 MIG 能力。

**实际选中的组件副本。** `fpga/scripts/add_sources.tcl:3–4` 指定 CVA6 `vendor/pulp-platform/common_cells/src/rr_arb_tree.sv` 和 `vendor/pulp-platform/axi/src/axi_mux.sv`。因此本设计主入口是该 CVA6 `axi_mux.sv:333–350`，不是 IOMMU vendor 副本的 `:409–426`。后者的结构相近，但不作 Genesys2 选中模块的代证。`rr_i='0` 也不是已经启用静态优先；实际 `ExtPrio` 默认 0，使用内部 RR 状态（选中 `rr_arb_tree.sv:54–60,145–148`）。

接入层次是：

```text
ariane_xilinx.i_axi_xbar
  -> gen_mst_port_mux[ariane_soc::DRAM=0].i_axi_mux
     -> [用下述普通选择器替换 i_ar_arbiter]
     -> 原 i_ar_spill_reg
  -> 原 axi_riscv_atomics_wrap -> 原 asynchronous AXI clock converter
  -> 原 Genesys2 MIG AXI64 -> 原 64→256 upsizer / 顺序读返回
```

`axi_xbar.sv:131–163` 已为每个目的端生成 mux。仅 DDR 的 AR 选择器替换，其余目的端、AW/W/B/R 保留原有逻辑。建议以两个静态集成参数（此 DDR 实例启用策略、DS source index=3）经 xbar/mux 层次传递，不能声称现有 RTL 已有这个参数或运行时开关。

选择器接口直接复用 ID prepend 后的 `slv_ar_valids[3:0]`、`slv_ar_chans[3:0]`、`slv_ar_readies[3:0]`、`ar_valid`、`mst_ar_chan`、`ar_ready`（选中 `axi_mux.sv:79–89,135–181,331–364`）。source index：CPU=0、debug=1、completion/payload=2、DS=3；当地 ID4 位，prepend 后 fullID6 位（`ariane_soc_pkg.sv:20–32`）。DS 指聚合 source3，包含当前合法 PTW/DDTW/PDTW/CQ 等 DS 读头；不是只给尚未出生的未来 PTE 一个名字（`rv_iommu_ds_if.sv:78–109`）。

输入严格限于该 arbitration owner 当拍拥有的合法 AR 头和原下游 spill READY。没有未来 job、未来 PTE 地址、未完成 walk 的翻译结果、hidden MIG 状态、R 队内位置、预测残余服务或免费第二 walker 输入。

## 2. 一份完整的可综合选择逻辑

这是设计说明中的 SystemVerilog 逻辑，不是生产补丁或已编译原型。四源固定为 0/1/2/3。非 DS 间采用循环 RR，游标只在非 DS 接受时前进，避免每个 DS grant 把逃逸游标重新打回 CPU 而饿死 debug/payload。该具体 RR tie-break 在结果之前固定；不声称与原 `rr_arb_tree` 每一任意四源状态的 tie-break 逐周期相同。在旧有限域只有 source2/3，非 DS tie-break 没有歧义。

状态是 `nd_rr_q[1:0]`、`ds_streak_q[2:0]`、`held_q`、`held_idx_q[1:0]`，绝对编码共 8 bit。计数沿用冻结合同：计的是原 output spill 的 **input accepts**；只在 DS 接受且同时有 non-DS current-VALID 时加一，非 DS 接受或没有 non-DS pending 时清零，饱和至 4（`ar-bound.md:73,106`）。不是四个时钟，也不是四个已完成 R burst。

```systemverilog
module ordinary_ds_ar4_design #(
  parameter type ar_t = logic
) (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic [3:0]  req_i,
  input  ar_t  [3:0]  data_i,
  output logic [3:0]  gnt_o,
  input  logic        gnt_i,
  output logic        req_o,
  output ar_t         data_o
);
  logic [1:0] nd_rr_q, held_idx_q;
  logic [2:0] ds_streak_q;
  logic held_q;
  logic nd_pending, fresh_valid, fire;
  logic [1:0] fresh_idx, chosen_idx;

  function automatic logic [1:0] nd_pick(
    input logic [2:0] active,
    input logic [1:0] cursor
  );
    // Called only if at least one non-DS head is current-VALID.
    case (cursor)
      2'd1: nd_pick = active[1] ? 2'd1 :
                       (active[2] ? 2'd2 : 2'd0);
      2'd2: nd_pick = active[2] ? 2'd2 :
                       (active[0] ? 2'd0 : 2'd1);
      default: nd_pick = active[0] ? 2'd0 :
                         (active[1] ? 2'd1 : 2'd2);
    endcase
  endfunction

  always_comb begin
    nd_pending = |req_i[2:0];
    fresh_valid = |req_i;
    fresh_idx = 2'd0;
    if (req_i[3] && !(ds_streak_q == 3'd4 && nd_pending))
      fresh_idx = 2'd3;
    else if (nd_pending)
      fresh_idx = nd_pick(req_i[2:0], nd_rr_q);
    // No input VALID depends on READY. A previous offer owns its index.
    chosen_idx = held_q ? held_idx_q : fresh_idx;
    req_o = held_q || fresh_valid;
    data_o = '0;
    gnt_o = '0;
    if (req_o) begin
      data_o = data_i[chosen_idx];
      gnt_o[chosen_idx] = gnt_i;
    end
    fire = req_o && gnt_i;
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      nd_rr_q <= 2'd0;
      ds_streak_q <= 3'd0;
      held_q <= 1'b0;
      held_idx_q <= 2'd0;
    end else begin
      held_q <= req_o && !gnt_i;
      if (!held_q && req_o && !gnt_i)
        held_idx_q <= chosen_idx;
      if (fire && chosen_idx != 2'd3)
        nd_rr_q <= chosen_idx == 2'd2 ? 2'd0 : chosen_idx + 2'd1;
      if (!nd_pending)
        ds_streak_q <= 3'd0;
      else if (fire) begin
        if (chosen_idx != 2'd3)
          ds_streak_q <= 3'd0;
        else if (ds_streak_q != 3'd4)
          ds_streak_q <= ds_streak_q + 3'd1;
      end
    end
  end
endmodule
```

实际拟接线是 `req_i=slv_ar_valids`、`data_i=slv_ar_chans`、`gnt_o=slv_ar_readies`、`gnt_i=ar_ready`、`req_o=ar_valid`、`data_o=mst_ar_chan`。选择器不新增请求 payload 槽，依赖现有合法 AXI 上游 producer 保持未接受的 VALID 及整个 AR struct；这与原仲裁器 `LockIn` 的源稳定前提相同（选中 `rr_arb_tree.sv:151–184`）。若要容忍非法 upstream 撤回，必须另存整个 AR 并重新设计握手，不能把该费用藏起来。

**选择承诺。** 一个时钟周期的采样点首次 `req_o=1` 时就已 offer；若本拍没有 `gnt_i`，下拍 `held_q/held_idx_q` 强制保持同一头。之后即使 DS 到来、streak 清零或另一个输入变 VALID，也不能替换该 AR 的 ID、ADDR、LEN、SIZE、BURST、LOCK、CACHE、PROT、QOS、REGION、USER。新选择只在没有旧 held 时发生。

**公平。** 持续存在 non-DS contender 时，每四次有竞争的 DS input accept 后下一次新的选择为 non-DS RR。三个持续非 DS 头间游标轮转，最多三次 non-DS 逃逸覆盖一轮；晚到 contender 最多另遇到一笔此前已 offer 的 DS。该界以成功 selector/spill input accept 数计，依赖上游保持和下游最终 READY，不是墙钟界，也不保证整个读写任务吞吐。没有因本域不触发 streak4 而删掉计数器。

**同 ID。** 每个输入只见最前面的 AR；该程序不跳过同输入的旧头。fullID 保留 `{source_index, localID}`；相同 fullID 的请求来自同一输入流，因此不越过同 ID。不同源相同 localID 因前缀不同仍是不同 fullID。原 R route 从 fullID 高位选 owner，不能把 ID、RLAST、错误响应或 retirement 换给另一任务（选中 `axi_mux.sv:369–386`；`axi_id_prepend.sv:80–110`）。

**已有债务。** 原 output AR spill、frontend demux、route multicut、atomics 和 CDC 保留其原顺序、容量、握手和 reset 约束。选择器已握手进入 spill 的 payload 即便还未到 MIG，也已由该下游 stage 拥有，程序没有 pop/reorder/flush 端口。下一笔 DS 只写入合法空位，不能顶掉旧 A/B token。spill 的 `ready_o=!a_full_q||!b_full_q`、B 先输出等见 `spill_register_flushable.sv:75–95`；CDC 的实际连接见 `ariane_xilinx.sv:1006–1047`。该设计不跨到 UI clock 读取 FIFO 内部，不额外引入 CDC 同步器，也不把 CDC 暂存误记为“尚未承诺”。

**W/B 与错误。** 新程序只处理 AR；原 AW 选择、W owner FIFO、正常 W/B 服务、R 全字段和错误响应正常 drain。不以 PTE 返回提前退休 payload、不改 RREADY 选择另一个返回、不清掉既有 debt。局部策略状态 reset 只在原系统 reset 合同内清零；没有新增可在运行中清 queue 的 flush。单方面 reset 选择器而保留其他 stage 的生命周期尚未定义，不能当普通控制许可。

## 3. MIG 已接受读为什么不能被它改变

沿用已验 `ip-semantics/verdict.json` 的 F01–F07，以及 `primary-sources.md:29–73`：生成 MIG 只有 active/head read 的顺序展开、读写 command 仲裁和顺序数据/metadata 返回；没有 read-vs-read priority、旧 AR 字段更新、取消或可选返回队列。这里不重跑这个证明。

必须区分承诺位置：

| 位置 | 该普通程序还能做的动作 |
|---|---|
| mux 尚未首次 offer，新合法 current-VALID 头 | DS-first 或 fairness/RR 选一个。 |
| mux 首次 offer 后 held | 保持原头直到原 output spill input accept；晚到 DS 等待。 |
| spill/route/atomics/CDC 已接受，但 MIG 尚未接收 | 继续原 token；不能在它们内部免费提升 DS。是否何时成为 MIG 前序债务要看真实 MIG AR handshake。 |
| MIG 已接受 L，L 的 R 未完 | 正常 drain；后到 PTE P 不能靠入口 priority 或 QOS 越过该已接受读。 |
| 已 held RVALID | 保持同 RID/DATA/RESP/LAST 直到 R handshake。 |

若相同共同前缀中 L 先被 MIG AR handshake 接受，P 后接受时 L 还欠 `k>0` 个真正 MIG R handshake，则这份参考的顺序输出要求 `P.firstR` 晚于 `L.RLAST` handshake。`k` 只用已接受 `ARLEN+1` 减真实已握手的 L R beats 得到，不用隐藏 bank 状态、native FIFO深度、DDR-model queue16 或“满了”的假设。该共同前缀在候选和普通两方都要支付；不能重新初始化它让候选一方免费消债。

相邻反条件为 `k=0`、P 先接受、L 根本尚未 offer 而两头 current-VALID 可选择，或 L 未进入本 MIG。前三个分别改变前序关系/可控制位置，最后一个改变证明对象；都不能被混成当前板的 measured benefit。

## 4. 最强普通控制允许什么，但必须收费

**合法 burst 分段。** 此实际 frontend 已有 `CONF.deburst`，同时设置源/目的 reduce_len=1 与 max_llen=0，意味着读写都完全拆成单拍；已有 DEFAULT INCR/4KiB 边界合法化。没有“只任意挑读 burst 长度 8/16/32”的现有寄存器旋钮（`idma_reg64_frontend.sv:341–373`，`idma_legalizer.sv:139–175`）。正常 deburst 会改变将来产生的 AR/AW 数、同 ID/credit 压力和 B 完成数，不能修改已经接受的 ARLEN，也不保证 PTE 更快。对对齐 4096B/64bit copy，长 burst 的两笔 256-beat AR/AW 与完全 deburst 的 512 笔单拍 AR/AW 是逻辑数量；512 个 B 仍须完成，控制/仲裁/元信息/总线占用成本要计，净完成效应 unknown。现有上下游可综合参数/接口支持不等于这份参考已板测 deburst。

更细分段可用本来合法的 descriptor 子作业或扩展 frontend 控制，但前者须定义完整 4096B parent 的共同就绪、所有子读写/B 和 CPU/提交成本；后者必须收新增参数、寄存器、状态和验证费用。不能把原 4096B 任务偷换成一个 64B copy，或使用只接收 64B、全零配置的原 launcher 当作原任务已拥有的能力（`idma_reg64_frontend.sv:253–266`）。buffer512 的整作业资格不因 AR 拆小而自动获得新权限；当前参考前两 DMA 是512、后两是3（`ariane_peripherals_xilinx.sv:1262–1277`）。不引用“原生64B fullbuffer例外”放松原4096资格。

**合法提前准备。** 可以事先准备属于本任务的 descriptor、page table/context、已经许可的普通物理数据或已有正常缓存状态，并把 CPU/cache/flush/CQ、额外存储、所有权和共同起点费用计入完整任务。不能把 current-VALID 尚不存在的 PTE 免费交给共享 AR 选择器。原 NativeN 是另 bank 的命中探测旁路，接口没有启动第二个 walk 的 prepare/raw-adopt 动作；normal gate 和 PTW init 见 `rv_iommu_tl_wrap.sv:284–329,375`。若以新 translation-only、prefetch 或 adapter 提前生产 PTE，必须证明接口和结果许可并收费，不能称现有 controller 的免费输入。先验 warmup/改变共同空起点也须显式更新观察合同；不能拿它反驳固定空起点域。

**公开 QoS。** 可以在首次 offer 前给将来的合法 AR 携带公开 QoS 字段，原 ID prepend/CDC/MIG ARQOS 接线传线；当前 DMA register frontend 源/目的 QOS均硬置0，因而设置非零还需明确事务属性注入位置或新增配置接口。整 struct 必须 held 稳定。该 MIG `RD_PRI_REG` 只把非零 QoS 用于读/写切换，不比较两个 read 的数值级别；ARQOS=15 不构成越过已接受 payload read 的控制。效果、面积、时序、写服务和 CPU费用均未验。

**普通许可与创新判定。** 普通 baseline 应允许已知合法的 segmentation、提前准备、QoS，以及上述 current-VALID AR-first 硬件构造；每项选择必须预先固定并完整收费。不能因 ordinary 可额外造任意 candidate-equivalent 模块，就宣称该模块已发表或免费覆盖创新。若双方必须新增同一种 source/service adapter，需说明 adapter 赋予的信息/动作、接口、所有权和全成本，再比较双方；名称“普通”不会使新增动作消失。

## 5. 四种对象不能互换

| 对象 | 证据/本轮地位 | 可用来支持的结论 |
|---|---|---|
| 公开/现存组件 | 选中 PULP RR+LockIn、ID prepend、spill/route，现有 iDMA legalizer/deburst 与生成 MIG；本轮不再穷尽查新 | 具体合法接口与已有组件行为；不证明本文 exact DS-first/F4 已发表。 |
| 新普通构造 | 本文 current-VALID DS-first/F4 选择器 | 合法接入方案和可数设计状态；没有实现时序、净收益或已部署能力。 |
| 候选等价 adapter | 新服务/预测/prepare/raw-adopt/accepted-read reorder 接口，视候选具体要求 | 须另外构造及收费；不能作“ordinary 已实现”的自动证据，也不能仅因能编程就否定创新。 |
| 理想/条件 memory law | `tests/nonblock_actual_dma_test.cpp:681–697` 对已接受 mem_q 使用 `find_if/rotate`；旧完整DS-window合同另见 four-path-proof:13 | AXI不同ID许可范围内的私有返回调度合同；不是 mux 入口动作，不是该 MIG 原生接口。 |

`tests` 的 held/same-ID 保护不使 `mem_q.rotate` 变成可直接接入生成 MIG 的公开动作；它已经在 memory owner 内操作 accepted requests。本文没有免费添加这一返回选择器。

**CO1min 与 LR1 phased 的来源另列。** CO1 对照原件实际名为 `OC1-min-frozen-v1`：`out/completion-obligation-design-20261014/control/min-rule.md:3–13` 明说读取 CO1 卡之后按普通模块构造，并使用共同新增付费登记/阶段退休/许可接口；`root/mechanism-card.md:51–53` 把它当该文字合同的唯一具体实现，明确映射不等于发表前作。这是候选驱动的普通重构，不能当独立已有完整 baseline。

LR1 `phased` 是独立编写的普通 policy，但读取的是共同 adapter 已完成的 paid primitive 事实；它本身不发现 reader、不测真实R、不封口、不执行PTE/cache/CQ（`out/local-read-reclaim-20261015/controls/ordinary_policy.py:1–8,84–118`）。共同 `Engine` 的 `snapshot/execute` 是候选与普通的相同接口（`prototype/check_functional.py:48–58,181–186`）。已验事件映射可支持该有限逻辑合同中 ordinary 复现 LR1；不能将共同 adapter 的 native 原语归零、证明其已发表，或推广成本实测等价。`root/full-cost.json:63–77` 明确 aggregate bits 和 native费用为null、`native_adapter_cost_identity_proven=false`。本轮只核这几段来源，不重跑其功能检查，也不改既有LR1裁决。

## 6. 正条件、相邻反条件与本轮停点

**入口动作的正条件。** 同一 owner 在首次 offer 前已同时拥有 DS source3 与至少一个非 DS 的合法 current-VALID 头；无旧 held，streak<4，且本轮冻结的普通程序与原 RR 会选不同头。DS-first 可改变随后新请求的 offer/input-accept 次序。如果还要据此改变实际 MIG AR 次序，必须有沿原 spill/atomics/CDC 的顺序保持、相同公共前债务及正常服务的证据；若有并行原子读注入，不能忽略其前债务。即使这些成立，也只是入口/前序债务动作差异，**不是全任务更快的证据**。

**相邻反条件。** DS 尚未 current-VALID；payload 已在该 selector held；payload 已在 downstream storage 被接受；streak已4且 non-DS pending；原 RR本来也先选DS。前三种没有这个新选择自由，第四种按公平合同逃逸，第五种没有入口 action delta。MIG已接受的公共 L debt 及 held R 对所有合法入口程序继续保留。

**论文推进需要的具体事实。** 要有一个已证真实的承诺前控制机会，或一个合法新执行动作，能改变属于真实原任务的目标债务，并与结果之前固定的普通程序在动作或完整实现成本上有明确差异；性能接受仍需双方完整 R/W/B、错误/所有权退休和全费用。是否需要两个合法 NormalTL 头由将来具体机制决定，本轮不把双头设成必要条件；观察本身可以判定原工作流能否达到该控制机会。若差异仅是 current-VALID DS 的 RR 位移，则因本文可执行普通构造已经拥有该动作，不能把该动作当候选独有能力；若差异必须新 accepted-read reorder/未来PTE/服务反馈 adapter 才有，则应单独提出接口与完整成本，不能默认普通覆盖，也不能默认候选已有。新接口保留同一共同前缀和全部原债务，不能只给候选清空历史。

当前源证据没有提供该真实全任务差。因此本轮停在**观察合同**：owner 首次 offer、每一 storage 的实际 accept、MIG AR/fullID/ARLEN、MIG R握手/RLAST、PTE真实producer/consumer及双方完整任务退休；新增观测、关联、CDC与采样延迟的实现费用 unknown。没有新事实，不恢复旧分支、不改返回律、不追加实验/构建/旧 validator/trace重解析。

## 7. 接受检查与未解费用

本设计需要接受以下检查，但本轮没有运行：全 AR 字段 held稳定；唯一gnt且source/data/fullID一致；streak单位与清零/饱和正确；非DS公平；同ID次序；spill/CDC debt守恒；原AW/W/B/R/error/reset不提前退休；完整4096B任务资格不偷换；DS尚未VALID时不等待未来输入。集成后才可验证编译/时序、真实选择周期和面积/Fmax，不能把本文组合设计的0新增注册stage当成实测零选择费用。

8bit是本文这一可选控制替换编码的绝对数，不是全部新bits、微架构面积或成本闭合；净增量寄存器/逻辑因替换旧selector和综合优化仍unknown。AR数据4选1覆盖整个 `99+U` bit struct（U为实际AXI_USER_WIDTH），所有参数/层次修改、选择/READY扇出、held锁、公平、观察关联、既有spill/CDC存储及延迟、合法强普通选项的CPU与全完成成本均在 `cost.json` 列项。总面积、Fmax、额外真实周期、CPU、应用收益、完整成本等价全部null。

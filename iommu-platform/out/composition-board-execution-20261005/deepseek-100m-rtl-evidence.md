# 定向证据报告：RTL 链（rgmii_core → MAC/FIFO/PHY IF → AXIS RX/TX）对 10/100 的静态支持

范围声明：仅基于本次提供的 6 个文件正文（rgmii_core.sv、eth_mac_1g_rgmii_fifo.sv、eth_mac_1g_rgmii.sv、rgmii_phy_if.sv、axis_gmii_rx.sv、axis_gmii_tx.sv、framing_top.sv，共 7 个）。未执行任何命令、未做板测/回读/hash，不声称任何测试通过。lowrisc_ethernet.c 的结论按你方已确认前置，不重复审查。以下为可导航证据，未决定是否接受 100M 方案。

---

## 0. 链路映射（本批文件可见部分）

| 层 | 文件 | 关键实例/端口 |
|---|---|---|
| 顶层帧接口/缓冲 | framing_top.sv | `rgmii_soc rgmii_soc1` (312–343)，RX/TX 双口 RAM (119–149) |
| 未提供 | rgmii_soc.sv | **缺失**（见 §5） |
| 以太网核 | rgmii_core.sv | `eth_mac_1g_rgmii_fifo eth_mac_inst` (88–140) |
| MAC+FIFO 包装 | eth_mac_1g_rgmii_fifo.sv | `eth_mac_1g_rgmii eth_mac_1g_rgmii_inst` (162–200) |
| MAC+RGMII 适配+速率检测 | eth_mac_1g_rgmii.sv | `rgmii_phy_if` (187–218)、`eth_mac_1g` (220–253) |
| PHY 侧 RGMII↔GMII + 发送节拍 | rgmii_phy_if.sv | RX DDR 采样 (84–103)、TX 时钟/节拍 (107–235) |
| MAC 内 RX/TX 字节引擎 | axis_gmii_rx.sv / axis_gmii_tx.sv | MII 半字节处理分别见 §3 |

时钟域（用于后面判断“是否被千兆锁死”）：
- RX 域时钟来自 PHY：`rgmii_phy_if.sv:97` `.output_clk(mac_gmii_rx_clk)` → `eth_mac_1g_rgmii.sv:198` → `eth_mac_1g_rgmii_fifo.sv:176` → `rgmii_core.sv:71/106` → `framing_top.sv:77/318`。即 **RX 域频率随 PHY 链路速率自动变（125/25/2.5MHz），RTL 无需显式分频**。
- TX 域时钟固定 125MHz：`rgmii_phy_if.sv:233` `assign mac_gmii_tx_clk = clk;`，节拍靠 `mac_gmii_tx_clk_en` 抽稀（§2）。

---

## 1. 真实 10/100 速度检测：**保留，且是“真检测”，不是固定值**

证据（eth_mac_1g_rgmii.sv）：

- 检测算法本体，130–185：
  - 131–135 用 `rx_clk` 做 3bit 预分频：
    ```verilog
    131: reg [2:0] rx_prescale;
    133: always @(posedge rx_clk) begin
    134:     rx_prescale <= rx_prescale + 3'd1;
    ```
  - 141–145 把 `rx_prescale[2]` 同步进 `gtx_clk`（125MHz 参考域）：
    ```verilog
    142: rx_prescale_sync_1 <= rx_prescale[2];
    ```
  - 150–182 在 `gtx_clk` 域统计参考计数，区分三档，含明确注释：
    ```verilog
    154:         speed_reg <= 2'b10;
    162:         if (&rx_speed_count_1) begin
    163:             // reference count overflow - 10M
    166:             speed_reg <= 2'b00;
    169:         if (&rx_speed_count_2) begin
    170:             // prescaled count overflow - 100M or 1000M
    173:             if (rx_speed_count_1[6:5]) begin
    174:                 // large reference count - 100M
    175:                 speed_reg <= 2'b01;
    176:             end else begin
    177:                 // small reference count - 1000M
    178:                 speed_reg <= 2'b10;
    ```
- 184–185 对外与选择信号：
  ```verilog
  184: assign speed = speed_reg;
  185: assign mii_select = speed != 2'b10;
  ```
- 复位默认 2'b10（1000M），154 行；也就是说 **上电默认千兆，但检测持续运行并会改写**，并非“锁死千兆”。

检测结果的可见性：
- eth_mac_1g_rgmii_fifo.sv:150–160 把 `speed_int` 同步到 `logic_clk` 并输出（`155: assign speed = speed_sync_reg_2;`，198：`.speed(speed_int)`）。
- **但 rgmii_core.sv 把检测结果丢掉了**：`137: .speed(),`（未连接）。所以经这条链，软件层拿不到已检速率（除非缺失的 rgmii_soc.sv 另有连接，见 §5）。

发送侧对速率的响应（10M/100M 分支实际存在）——rgmii_phy_if.sv:
```verilog
124:         if (speed == 2'b00) begin
125:             // 10M
129:             if (count_reg == 24) begin ... 
133:             end else if (count_reg >= 49) begin ...
139:         end else if (speed == 2'b01) begin
140:             // 100M
144:             if (count_reg == 2) begin ...
148:             end else if (count_reg >= 4) begin ...
153:         end else begin
154:             // 1000M
```
100M 周期≈5 clk（25MHz），10M 周期≈50 clk（2.5MHz），与 RGMII 10/100 的 TXC 频率要求量级一致。端口连接：`rgmii_phy_if.sv:79 input wire [1:0] speed;` → `eth_mac_1g_rgmii.sv:217 .speed(speed)`。

---

## 2. 发送节拍（TX clock enable / pacing）：**保留**

- rgmii_phy_if.sv 产生字节节拍：
  ```verilog
  168: reg gmii_clk_en;
  182:         gmii_clk_en = rgmii_tx_clk_fall;   // 10M
  194:         gmii_clk_en = rgmii_tx_clk_fall;   // 100M
  201:         gmii_clk_en = 1;                   // 1000M
  235: assign mac_gmii_tx_clk_en = gmii_clk_en;
  ```
- 送到 MAC：`eth_mac_1g_rgmii.sv:103 wire mac_gmii_tx_clk_en;` → `245: .tx_clk_enable(mac_gmii_tx_clk_en)`。
- 节拍周期与 MII 语义自洽：MII 每字节需要 2 个使能步（LSN/MSN，见 §3），100M 下每 5 clk 一脉冲 → 每字节 10 clk ≈ 12.5MB/s = 100Mbit/s；10M 下每 50 clk 一脉冲 → 每字节 100 clk。
- TX 时钟 125MHz 恒定、靠 `tx_clk_enable` 抽稀，这是该 IP 的标准做法，非千兆专用。

注意一处不对称（仅记录，不下结论）：RX 侧 `eth_mac_1g_rgmii.sv:244: .rx_clk_enable(1'b1)` 恒 1，RX 的“半速率”不是靠 clk enable 抽稀，而是靠 `mii_select`+奇偶周期处理（§3），且 RX 域时钟本身由 PHY 减速。

---

## 3. 接收 MII 半字节处理：**保留**

axis_gmii_rx.sv：
```verilog
62:     input  wire        mii_select,
167:     end else if (mii_select && !mii_odd) begin
168:         // MII even cycle - hold state
288:             if (mii_select) begin
289:                 mii_odd <= !mii_odd;
291:                 if (mii_locked) begin
292:                     mii_locked <= gmii_rx_dv;
293:                 end else if (gmii_rx_dv && {gmii_rxd[3:0], gmii_rxd_d0[7:4]} == ETH_SFD) begin
294:                     mii_locked <= 1'b1;
295:                     mii_odd <= 1'b1;
...
324:             gmii_rxd_d0 <= {gmii_rxd[3:0], gmii_rxd_d0[7:4]};
326:             if (mii_odd) begin
327:                 gmii_rxd_d1 <= gmii_rxd_d0;
```
即：低半字节每拍进入、隔拍凑成一个字节（324 行），SFD 检测在凑齐后的字节上做（293 行）。这是 MII 半字节重组，非千兆 8bit/拍。

发送 MII 半字节（axis_gmii_tx.sv）：
```verilog
163:     end else if (mii_select && mii_odd_reg) begin
164:         // MII odd cycle - hold state, output MSN
166:         gmii_txd_next = {4'd0, mii_msn_reg};
...
347:         if (mii_select) begin
348:             mii_msn_next = gmii_txd_next[7:4];
349:             gmii_txd_next[7:4] = 4'd0;
```
`mii_select` 的 3 拍跨域同步：`eth_mac_1g_rgmii.sv:110–128`（tx/rx 各一套），使用 `.rx_mii_select(rx_mii_select_3)`/`.tx_mii_select(tx_mii_select_3)`（246–247）。

结论：**RX 半字节重组、TX 半字节拆分在提供的 AXIS 源中都真实存在，不是千兆裁剪版**。

---

## 4. framing_top 寄存器/FIFO 边界是否限制为千兆

边界事实：
- framing_top 不接任何“速度/速率”寄存器。寄存器译码见 `230–242`，写寄存器见 `181–191`，只有 MAC 地址、irq/promiscuous/loopback、tx 长度、mdio 位操作、first/last buf，**无 speed 字段**。
- RX 侧逻辑跑在 PHY 域时钟：`88: always @(posedge rx_clk)`、`291: always @(posedge rx_clk)`；RAM A 端口也是 `120: .clka(rx_clk)`、`139` 对应 TX 用 `clka(~clk_int)`。即 **缓冲器天然跟随 rx_clk 频率，不假设 125MHz**。
- 字节写通道 `114: wire [1:0] rx_wr = rx_axis_tvalid << rx_addr_axis[2];` 与 `127` 的 32bit 读口无关速率。
- `rgmii_soc` 实例（312–343）只连 RGMII、AXIS、FCS，**没有 speed / clock-enable / 速率配置端口**；`rgmii_core.sv:88–140` 也是固定参数、无速率端口。
- FCS 交叉/回读、`rx_length_axis`、`tx_frame_addr`/`tx_axis_tlast` 生成（264–289）只依赖 AXIS 握手，不依赖线速。

因此：**从所提供的 framing_top 与 rgmii_core 代码看，边界本身没有“千兆写死”的证据**；唯一硬编码的千兆相关常量是 `rgmii_core.sv:139 ifg_delay(12)` 和 `94 MIN_FRAME_LENGTH(64)`，这些都是合法且与 10/100 兼容的量。

但有一处必须提请 Codex 注意（非速率本身，属接口实现差异）：`eth_mac_1g_rgmii_fifo.sv` 虽名为 “_fifo” 并声明了 `TX_FIFO_ADDR_WIDTH/RX_FIFO_ADDR_WIDTH/TX_FRAME_FIFO/RX_FRAME_FIFO`（47–54），但正文 113–201 **只实例化了 `eth_mac_1g_rgmii`，没有任何 FIFO 实例、参数也未被引用**。也就是说该包装在本批文件里是直通（tready 直接来自 MAC），真正的收发缓冲在 framing_top 的双口 RAM（119–149）。这会影响“低速下背压/突发”行为，但**不是千兆限制**；请确认实际文件是否与提供正文一致（若上游版本含 FIFO，则需补该文件）。

---

## 5. 尚缺、无法在本批文件内证明的项

必须补齐才能对“仅放开软件速度约束是否足够”下结论：

1. **eth_mac_1g.sv**（`eth_mac_1g_rgmii.sv:220–253` 实例）——最关键缺口。无法确认它如何消费 `tx_clk_enable`、`rx_mii_select`、`tx_mii_select`、`rx_clk_enable`，以及 `s_axis_tready` 在低速节拍下的背压行为（framing_top 的 TX 握手见 279–289 依赖 tready）。
2. **rgmii_soc.sv**（`framing_top.sv:312`）——无法确认它是否把 `rgmii_core.sv:137` 被丢弃的 `.speed()` 接出、是否另加 FIFO/CDC、`rx_clk` 是否原样透传。
3. 若实际工程存在 `eth_mac_1g_fifo.sv` / `axis_async_fifo` 等，需要正文以核对 §4 的“无 FIFO”判断。
4. **RGMII 时序约束文件（.xdc/.sdc）**——是否只按 125MHz 约束 rx_clk/IDELAY；10/100 下 rx_clk 变 25/2.5MHz 的时序裕量无证据。
5. **顶层引脚/PHY 配置**：RTL8211E 的自协商/strap、RGMII 延时（IDELAY/内部延时）设置。framing_top 只暴露软件位操作 MDIO（`187`、`235`、`117: assign phy_mdc = phy_mdclk;`），说明 PHY 寄存器由软件位敲；实际 MDIO 是否接到 PHY 需顶层证实（framing_top 的 `rgmii_soc` 实例未含 mdio 端口）。
6. **旧 BIT 对应源码**：你提到“旧已板测通过 N v5 BIT 跑 cpn2-net1 Linux”，要比较必须给出该 BIT 的源码/版本（尤其是当时 rgmii_core 是否接线不同、speed 是否接出）。本批文件不足以证明与旧 BIT 等价。

---

## 6. “仅放开软件速度约束”的静态支持证据与未验证风险

静态支持证据（本批文件内，肯定项）：
- 存在真实三档速度检测（eth_mac_1g_rgmii.sv:130–185）。
- 存在 10M/100M TX 时钟生成与节拍（rgmii_phy_if.sv:124–159、182/194/201、235）。
- 存在 MII 半字节 TX 拆分/RX 重组（axis_gmii_tx.sv:163–169、347–350；axis_gmii_rx.sv:167–170、288–339）。
- RX 域时钟由 PHY 提供，频率随线速自动变化（rgmii_phy_if.sv:97 → … → framing_top.sv:77/318），无需额外分频。
- framing_top 缓冲/寄存器边界不含速度写死项（§4）。

未验证风险（不能由本批文件排除）：
- R-1：`eth_mac_1g.sv` 不可见，MAC 是否正确使用上述节拍/`mii_select`/`rx_clk_enable` 未证。
- R-2：`rgmii_soc.sv` 不可见，被丢弃的 `.speed()` 是否可恢复、FIFO/CDC 是否引入 125MHz 假设未证。
- R-3：`speed_reg` 复位默认 2'b10（eth_mac_1g_rgmii.sv:154），而 100M 时 rx_clk 只有 25MHz；检测收敛前存在 `mii_select=0` 的启动窗口，其行为未证（复位期间 rx_clk 是否已稳定、检测收敛时间）。
- R-4：无时序约束文件，rx_clk 在 25/2.5MHz 下的 IDELAY/RGMII 延时裕量未证。
- R-5：PHY（RTL8211E）strap/自协商与对端 Realtek 8152 的 100M 共同速率，本批 RTL 无法证明；需 PHY 配置或板级证据。
- R-6：`eth_mac_1g_rgmii_fifo.sv` 的 FIFO 参数未被使用（§4），低速下背压直接打到 framing_top 的 TX 握手，行为未证。
- R-7：`lowrisc_ethernet.c` 侧仅为软件广告/`adjust_link` 约束（按你方已确认），本报告不重审；软件放开后 RTL 是否真正按 100M 收发仍取决于 R-1…R-6。

结论性表述（不做接受判定）：本批文件**支持“该 RTL 链内含可用的 10/100 数据通路与节拍/半字节处理”这一静态主张**；但**不足以单独证明“只放开软件速率约束即可在板级建立 100M 链路”**，缺口集中在 §5 的 eth_mac_1g.sv、rgmii_soc.sv、时序约束与 PHY 配置。是否接受该方案及后续架构/时序判断由 Codex 保留。

---

## 7. 引用索引（file:line 速查）

- 速率检测：eth_mac_1g_rgmii.sv:130–185（131/133/142/154/162–166/169–178/184/185）
- 速率上报与丢弃：eth_mac_1g_rgmii_fifo.sv:150–160、198；rgmii_core.sv:137
- 速率→PHY：eth_mac_1g_rgmii.sv:217；rgmii_phy_if.sv:79、124–159、182/194/201、235
- TX 节拍：eth_mac_1g_rgmii.sv:103、245；rgmii_phy_if.sv:168、235；恒定 TX 时钟 rgmii_phy_if.sv:233
- RX clk 由 PHY 提供：rgmii_phy_if.sv:97；eth_mac_1g_rgmii.sv:198；eth_mac_1g_rgmii_fifo.sv:176；rgmii_core.sv:71/106；framing_top.sv:77/318
- MII 半字节：axis_gmii_rx.sv:62、167–170、288–339；axis_gmii_tx.sv:163–169、347–350；eth_mac_1g_rgmii.sv:110–128、246–247
- framing_top 边界：88、114、119–133、139、151、181–191、230–242、264–289、312–343
- 固定参数：rgmii_core.sv:88–99、139；rgmii_phy_if.sv:233；eth_mac_1g_rgmii_fifo.sv:47–54（未使用）

以上为只读证据整理，未修改任何文件、未执行命令、未做板测/回读/hash。
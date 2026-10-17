# G 选定合同与锁定接口

Root已选择G先行，M只保留设计；V/A各固定4096×4096字节自然完成。
Root已锁定e0/e8、两个cap magic、32word语义布局与只读硬件配置tap。
observer/Linux尚未实施；error mask/clear/abort/ARM细节仍需下一批前另锁。

## 本批frontend

- 冻结CVA6父提交`6c5c666d45979d3a03fb98cff89ba7ad095f6125`，独立worktree
  `/home/xvhaocheng/work-a1-e6-cva6`、分支`ispass/e6-mitigation`。
- 新参数`parameter bit BatchGapEnable = 1'b0`，只有与`BatchPort=1`同时有效。
  所有生产实例本批不改，默认关闭保持原行为；启用实例gap0在旧合法接口上逐拍等价。
- `0xe0`：gap cycles，读为32bit零扩展；仅IDLE、full wstrb、wdata[63:32]=0时可写。
  复位及batch disable/clear置0，active写拒绝并保留旧gap。旧BTC2 magic不改。
- `0xe8`：只读capability `0x49444d4147415001`（ASCII IDMAGAP，version1），沿用历史gap能力语义，
  此地址独立于BTC3 d0。写拒绝。参数关闭时e0/e8保持原reserved地址的read error/write sticky error。
- 新只读tap `output logic batch_gap_active_o`，恰为有效batch的GAP状态占用；不反馈握手。
  同时增加`output logic [31:0] batch_gap_cycles_o`和`output logic [32:0] batch_target_o`，
  均取实际硬件寄存器；Gap关闭时gap tap为0，BatchPort关闭时target tap为0。
  后续ARM必须锁存这些实际配置，不得仅使用软件期望。
  wrapper/顶层接线留下一小批，本批原生产实例可以不连新输出。
- 状态仍2bit，原IDLE/COUNT/WAIT编码不动，新增GAP=2'b11。只有非末task完成且gap!=0进GAP。
  倒计数恰占gap个GAP周期；首task和最后task无gap；gap0保留WAIT→COUNT原路径。
  STOP在GAP/到期同拍优先，不产生额外task done；已offer的COUNT请求保留直到握手后完成。

## 后续observer已锁最小32word布局（768..799）

全局OBS4 magic、INFO192、旧0..34/PATH64..191/hist256..767原字节保留。
只在G观测参数启用时新增以下bank；关闭时保持当前不支持的index写行为，不扩全局允许集合。
bank capability建议`0x49444d4150344d02`（IDMAP4M，version2），明确区别旧P4M1的16word布局和STOP合同。
本批不分配新全局OBS版本，不增加身份体系；包仍记录实际source/BIT/kernel。

| 索引 | 64bit读值 |
| --- | --- |
| 768 | 独立P4M2 capability magic |
| 769 | reserved0；现有sequence只在软件，硬件不回显 |
| 770 | flags：bit0 armed，1 V-window active，2 V-window closed，3 V whole done，4 A whole done，5 has-A；其余0 |
| 771 | sticky error mask；具体bit分配在observer小批前另锁，至少含非法issue/done/ID、非4096预算、溢出、abort |
| 772/773 | V/A目标，固定4096；Solo A目标0 |
| 774/775 | ARM锁存V/A gap，V必须0；A为采集计划0/1000/2000 |
| 776/777/778 | V whole issued / completed / elapsed cycles |
| 779/780/781 | A whole issued / completed / elapsed cycles |
| 782/783 | V/A whole区间内实际GAP占用cycles |
| 784 | V窗口cycles，必须等于既有batch time cycles |
| 785/786 | `(open,close]`内A issue / completed |
| 787/788 | V开窗/关窗的A在途（0/1） |
| 789/790 | carried-in A在窗内完成（0/1） / 开窗同拍A issue（0/1） |
| 791/792 | 开窗/关窗时A在途task ID（无则0） |
| 793/794 | 窗内首/末A done ID（无则0） |
| 795/796 | 从ARM起算的V首issue/最后done时间戳 |
| 797/798 | 从ARM起算的A首issue/最后done时间戳 |
| 799 | reserved0 |

所有数据只读，角色复用既有ARM选lane，Root本轮固定V0/A1且DID10/11。
每lane whole计数与timer由同一次测量ARM后首真实issue启动，完成目标第4096 done后各自冻结，
无任务间重启；warmup必须先完成再ARM。elapsed按(first issue,last done]定义，零起点不作为“未启动”哨兵，另有state。
窗口在真实首V issue开、最后V done关；开边沿不计issue/done，闭边沿计入，carry用连续lane跟踪。
A whole允许在V窗前/窗内/窗后完成，必须继续保留到两lane完成后读快照；不在V关窗强制STOP。
全部whole/时间累计采用64bit饱和+sticky error，溢出不绕回；原hist32bit/Depth4及错误门原样保留。

新旁路模块端口只读输入：clk/reset、同ARM/clear/abort/selected roles/has-A、
各lane真实issue/done和ID、`batch_active`、新`batch_gap_active`、gap配置回读快照及V open/close；
输出只读bank data、sticky error，不参与AXI/DMA握手。若采用独立模块需在ARM锁存配置，禁止运行中变化。
Root已修订锁定769为reserved0：由既有experiment_lock下单一prepare→ARM→finish的software sequence绑定日志，
不增加ARM generation或新身份计数，不声称硬件回显sequence。

### 已发现的有限A兼容点

当前E5 `dma_observer.sv:173` 要求V窗口内A batch一直active，这是旧background-A合同的实际检查。
新的固定A自然完成允许A先于V完成，不能盲目复用该检查或整体移除它。
后续observer小批仅在G capability启用且新tracker证明A issued=done=4096、无在途且无错误后，
允许这一合法A inactive；否则仍报原错。G关闭时原条件逐字节保留。
Root要求本批只做frontend，因此此处只记录所需有界适配，尚未改observer。

Linux/UAPI仍不改。旧P4M0x0d冲突，不复制旧header；下一小批前Root再锁新接口编号及结构。

# 完整流式写入与B终点：条件边界及正向设计

actualgoal1791949728。本页是root亲自手推，不是运行模型或实验。**完整R/W/B加入后，R-only门限不能直接保持最优；然而下面的正反方向仍依赖未由实际共享DDR建立的服务假设，不准入microbenchmark。** 五个预先冻结普通规则见frozen-controls.json；其端点语义澄清见frozen-controls-endpoints.json。

## 1. 实际路径必须保留的完成关系

对策略s和已拥有job j，定义D[j,k,s]为第k份写数据经原R接收、byte buffer与W FIFO后真正对原W路径可用的时刻；H[j,k,s]为原input/output W owner、AW tracker和credit允许此拍继续前进的时刻。它不是“同bank所以可写”，也不要求免费增加MIG AW-before-W限制：AXI的AW/W可能解耦，原生授权与endpoint必须分开。

令Phi_W[s](t)给出在真实该策略共享服务下、所有原约束满足且数据从t起可用时，这份W实际接受/服务完成的时刻。保持原已经承诺的W顺序：

`T_W[j,k,s] = Phi_W[s](max(D[j,k,s], H[j,k,s], T_W[previous native W,s]))`。

Phi包含已有packet/owner、通道pipeline、反压及共享服务机会；它不是可免费输入的内部DDR状态，也不先假定与s无关。B另记真实对应写burst的终结函数Psi_B[s]，输入至少有该burst的AW拥有/接受、真实WLAST与原错误路径。job正常完成C[j,s]来自最后B与backend last-burst metadata的真实匹配；quiet还包括其他已拥有R/W/B/错误/队列及原生后续供给。

这样保留了R/W streaming：每份D可以在R-last之前出现，W不能被人为拖到全部R完成后。W owner通常在原WLAST事件推进，不必等B；B仍是完成义务，不能用释放W owner代替它。

## 2. 用于手算的条件流式写包络

以下仅用于说明“写入终点能改变选择”，不是实际MIG服务模型。附加条件全部显式：固定正常两当前job与共用前缀；原可证明W顺序为B job后A job；AW/credit已能合法支持各自数据；每job剩余N_j份数据不超过原buffer容量；R数据依次可用、无额外queue/backpressure改变样本；W可用服务以常数w_j推进且跨策略不变；两job末B相对最后W的正常终结间隔为同一个固定z。实际DDR没有建立这些读写独立/固定机会及B law，本轮不给它们数值。

给定R数据可用时间向量d_j(k)，在这个条件域中：

`U_j = max_k [d_j(k) + (N_j-k+1)*w_j]`。

第一job W结束为`max(common_W_end+N_B*w_B,U_B)`；第二job还必须等原第一job W顺序完成：

`W_A_end = max(W_B_end+N_A*w_A, U_A)`。

所有AW授权/原pipeline下界若不能由已证data路径覆盖，要另放入max，不能删除。若d_j(k)=t_j+k*r_j+sigma_j，sigma是原native传播延迟而非人为新增尾部，则

`U_j = t_j + sigma_j + max(r_j+N_j*w_j, N_j*r_j+w_j)`。

这是真正的流式包络：慢写时首份数据与持续W负载占主导，快写时最后读数据占主导。它不是“R-last加固定写尾巴”。第一、最后B仍各自保留C_B=W_B_end+z与C_A=W_A_end+z；由于原W顺序与N_A*w_A>0，本条件域C_A>C_B，max=C_A，sum=C_A+C_B。

## 3. 一个保持相同工作量的条件正反方向

仍取上轮的L/P/Q读交换符号，但明确仅在current P最终为正常叶、其后实际已拥有A数据Q的合法释放gap h跨两策略不变的条件子域使用。当前P是否最终完成、是否fault只能真实返回后确认；调度只能用历史估计，不能预读未来PTE。一般完整walk必须回到第1节，后续PTE会使h内生，不能套本式。

移去相同已接受前缀；本节另要求共同W前缀已结束、B原W owner与相关授权合法保留，初始DS当前VALID未出现、L新下游尚未VALID。B的L剩余读服务a>0，当前P准备gap g>0/P服务b>0，A数据Q在P服务后h释放。ADMIT读顺序L→P→Q，WAIT为P→L→Q，始终不免费把Q提到已承诺L之前。取a>g且h>a，则B数据流在WAIT整体后移g+b；A的Q读开始分别为a+b+h和g+b+h。

令U为ADMIT下B剩余流式W结束；A从其Q读开始到流式W需求结束的包络为H；定义

`D = U + N_A*w_A; E = b+h+H`。

完整A job终点为：

`C_A(admit) = max(D,a+E)+z`；

`C_A(wait) = max(D+g+b,g+E)+z`。

B终点分别U+z和U+g+b+z，原任务字节和全部W/B义务未变。

- **读依赖主导条件：D+b<=E。** 两max中数据释放项主导，ADMIT最晚完成比WAIT晚a-g>0，应WAIT。
- **写排空主导相邻边界：D>=a+E。** 原B写入顺序主导，ADMIT最晚完成比WAIT早g+b>0，应ADMIT。

两条件固定a/g/h及原任务工作，只在已有W服务包络/可用性上区分；没有运行任何延迟变体或增加W压力。它们是符号条件，不是两个已发现的合法平台prefix，更不是面积/物理完成数据。

普通选择比较另以必要历史有效、各估计在同一声明端点恰好匹配这些条件参数为前提；当前来源并没有取得这组历史，不免费注入它们。在两条件中R-only双门限因min(a,h)>g都WAIT；earlyPause与cheapDebt也WAIT。若L真实直接供应原B W owner，owner/current规则都ADMIT。五冻结普通没有一个作为统一在线规则精确复现两个条件。但这仅是上述假设域的选择比较，不证明真实共享DDR域的独有收益。

## 4. 一个明确的便宜实现边界，不拼接对照

完整条件式本身可由常数个加法/max实现：比较

`F_admit=max(D,a+E)`与`F_wait=max(D+g+b,g+E)`，小者优先；相等时B完成更早的ADMIT使sum更小。

这是本轮推导出的**O-envelope-derived**，不是五个预先冻结在线对照之一，不加入它们逐场景选最优。它在同一个声明域内是一条完整、统一、可执行的闭式规则，准确复现本条件candidate选择；公共paid输入相同，选择逻辑只需要上述两score，未包含两份逐义务rollout。因此“这对条件需要完整预测器才可选择”被这个具体实现否决；未证明真实物理周期或工程数字成本等价。

对真实路径，N、current W顺序与剩余可用data必须来自原packet/owner；a/g/h/H/U只能由同端点有效服务历史和已拥有依赖形成估计，未知/overflow回退，held与安全先行。部署时还要冻结完整R/W/B service law、history class/width与snapshot，不能凭该纸面公式直接给新microbenchmark准入。

## 5. 正向产物及停止范围

留下的正向机制问题是：**用原W顺序和逐拍数据可用性构造完整任务完成包络，能否用小成本准入逻辑正确区分“继续排空当前写owner”与“提前服务翻译释放另一任务”？** 本页已有有界闭式选择和明确失效条件，比单纯PTE等待更接近完整任务；独立创新/物理收益均未接受。

接受本页：native终点关系；流式W包络的条件定理；五冻结规则在该条件对子上的非统一覆盖；O-envelope-derived对此闭式candidate的统一选择覆盖。

不接受：条件假设在实际共享MIG成立、自然accepted prefix/有效在线历史、整个原V/A窗口固定双方工作、source-bound full-RWB positive、净收益或新颖性。不能据此关闭完整准入家族/F1全族。下一轮必须有新原始证据让服务/来源假设成立或有明确新机制修订；不自动再次延长本纸面模型。

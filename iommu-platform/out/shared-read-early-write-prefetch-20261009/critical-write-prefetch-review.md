# §58：强普通 next-WR 预取的授权、生命周期与 W 前沿

Codex 直接核实际 root patch；未构建、运行或修改 SV。核查文件为本目录 `multi_actual_dma_top.sv`、`shared_pretl_read.sv`、`q_write_splitter.sv`、`hit_bypass_parent_translation.sv`、`multi_banked_top.sv`、`multi_system_top.sv`，以及真实 frontend/legalizer/TL 的关键授权路径。以下是架构/源码接受范围，不是 worker 或本代理执行的 copy 结果。

**结论：指定单 job、静态正常、mode2/common clip、keep-q1、无handover/pause合同下，当前 patch 的独立 WRITE 授权、两页无 hole 前沿、跨 B 的固定 recipe 生命周期与共同 W 截齐逻辑成立。未发现本任务范围内的阻断项。** 下一页提前成功并不允许改变 actual raw WRITE parent 的页边界，当前代码明确保留该边界。这是强普通控制，应与 §57 完整计费比较，不能命名为新机制。

## 1. WRITE key 真实来源与检查链

- top `:1050–1059` 只有 actual qwriter 曾捕获 raw AW 后，`total_q!=0` 才让 WRITE slots 成为有效 parent。qwriter `:72–81` 在真实 `s_aw_valid && s_aw_ready` 时捕获完整原 raw AW 与总长；先发了假的 descriptor/header 或仅收到 source READ 都不会使 WRITE slot有效。
- top `:1072–1075` 从 qwriter 当前保留的 **真实 raw WRITE header** 取 DID、PID、PID-valid、priv。slot地址独立由jobdst/nextpage产生（`:1069–1071`）。这里没有额外锁存“首WRITE header”；后续仍从 current/retained raw AW取字段，依靠static-job context不变使其与首WRITE key一致。这一点应按实际代码描述。
- 当前实际 DMA 合同确实支持 context不变：`idma_reg64_frontend.sv:333–360` 生成固定DEVICE_ID、job PV/PROCESS_ID、src/dst privilege0；`idma_legalizer.sv:284–300` 在新job时保存options，`:328–363` 的AR/AW分别从同一保存options生成DID/PV/PID及各自PROT。top `:1033–1040` 又每拍检查当前真实WRITE DID/PID/PV/priv等于原source header的对应context。该 source header仅是context稳定性参照，**不是WRITE权限证书**。
- helper `parent_key :71–78` 明确使用完整 DID/IOVA/PID/PV/priv/bank，WRITE slot生成 `UNTRANSLATED_W`。`multi_banked_top.sv:813–825` 把其prep请求接入真实TL；不是直接写valid位或把cache中某个r位当w位。
- `rv_iommu_tl_wrap.sv:415–419` 根据type识别store；`:1374–1400` 检查两阶段WRITE权限和privilege。helper fill `:222–230` 要求仍有效、generation相同、完整parent_key等于原slow_req、响应正常且offset匹配。cache-valid `:103–106` 再要求无error/ignore/MRIF，且WRITE w位或合法bypass。故slot2成功是独立真实WRITE翻译结果。

本次 assertion主动限制src/dst translation context相同；这符合实际iDMA，但不是一般DMA预取的必要条件。若未来允许独立source/destination context，应保留真实WRITE key，不能用该assertion替代独立授权。普通预取也必须承担这套key/权限成本。

## 2. 固定 page key、跨 B 生命周期、generation

top `:1069–1071` 的slot1固定 `jobdst`（保留首page offset），A slot2固定 `(jobdst & ~4095)+4096`。后续 actual raw AW变到第二页，slot1不会随它迁移；这修掉了“同seq0但地址变了，旧valid响应暂时被视为新地址响应”的风险。

有效期由 `active_q && total_q!=0` 保持（`:1054–1059`），不使用当前 `parent_live` 来撤销预取slot。qwriter即使原first parent B已回复，仍保留其header/total，直到下一 actual raw AW capture。于是第一页证书不因首 B消失，第二页已成功的预取也不在B→AW2空档被清掉。后续AW2的context字段由实际static-job contract和fatal检查约束为同一key；地址固定，因此整个有效期内key没有改变。

helper当前seq0可以安全用于这个 **reset起单job、static固定key** 合同；它不是一般多job generation方案。`helper :174–177` 在parent失效/flush/seq变化时清valid/prepared；`:223–225` 又拒绝对失效或不同key的late fill。jobactive清掉后，迟到响应仍被消费但不会重新让失效slot成功。

未来多job、动态context、更换页表/权限均不由本稿保证；source owner本身目前也未提供可反复复用的多job游标复位协议。slot2准备失败在top `:1029` fatal，而非fault恢复；不能将该实验当作无副作用的容错prefetch。

offset0不跨页：slot2的有效条件明确要求A且jobdst低12位非0，top `:1058–1059`；所以没有对job范围外的下一页做无用准备，也不会因其未分配而使本job失败。允许的非零offset是8B对齐2056..4088，实际两页F1..255，`shared_pretl_read.sv:173–178` 检查该范围。

## 3. 不允许 slot2 越过 slot1 的 hole

top `:1044–1048`：V或不跨页A的recipe end为256；跨页A只有 `cache1 && cache2` 时才给256，否则只给第一页末端

`F=(4096-jobdst[11:0])/8`。

更关键的是 source owner `shared_pretl_read.sv:170–171` 只在 **slot1 cache-valid** 时把该end采入certified_end。因而四种状态是：

| 页1成功 | 页2成功 | 可发布前沿 |
|---|---|---|
| 0 | 0 | 不推进 |
| 0 | 1 | 不推进，不能越过缺失第一页 |
| 1 | 0 | F |
| 1 | 1 | 256 |

`source_recipe_valid` 仍保留真实slot1 gate（top `:1013–1018`），没有用sticky debug的future-ready代替授权。top `:1022–1025` 的1bit debug只是记录曾经真实成功，不参与policy。

页2预取若在第一页物理AR offer前成功，mode2可offer完整长source AR256；若已经offer了前沿F的child，则该child len保持，等其真实RLAST和skid退出后才发下一child。未来PTE响应不改变held AR/RVALID。

## 4. 实际 WRITE child 选哪一页权限

qwriter的新child gate已由“永远slot1有效”改为 `current_write_recipe_valid`，top `:723–727`。top `:1013–1018` 根据当前 **actual raw AW 页号** 为A选择slot1或slot2；V始终首page。每个actual rawparent是合法页内WRITE，后续qchild保持该parent的context与页。

不能仅靠这个page选择授权：actualTC仍把每个child的完整请求送helper。helper `:121–129` 使用 `normalized==parent_key`（只规范IOVA）及同4KiB页检查；`:130` 对多匹配fatal；`:182–184` 用相应slot的PA加实际child相对地址offset。slot1/2在不同页，另与source slot的READ type不同，所以实际WRITE child不会双匹配，也不会借sourceREAD结果的w属性。

首page slot1的PA是jobdst对应PA，child offset非负；次page slot2的PA是页首PA，child offset在本页。actual monotonic job和qwriter的8B/within-page检查保证相应范围。这里仅支持4KiB同页偏移复用与当前normal访问，不扩展到任意ATOP/ATS/指令/MMIO。

## 5. WRITE2 已预取成功时仍必须共同裁到 first F

若FA1而提前cert=256，只有R的共同frontier变256、V仍qW2，A native firstparent却qW1，便再次落回 §56 旧环/错位问题。因此 top `:698–707` 在mode2下无条件对 **两流W** 保留first F边界，即使成功common_end已经256也不允许child从F前越过去。

令q2网格先由0开始。到F最后一个child若需要就裁q1；V/A两边都结束于F。A native firstparent恰在该点结束，V original parent仅结束一个内部child。两边prefix共有 `ceil(F/2)` 个child B；现有groups相等门在下一组前等待对方旧child的真实B。A raw B→next raw AW存在正常捕获间隔，但WRITE2证书已按job保持，actualW metadata/R长度独立（先前§54/56直接核过），不等整source logical RLAST。

从F继续双方相同q2网格，到256最后child也同点结束。这时一个flow即使先收到B，也不能多offer一组堵住peer的旧W；每一组的数据经shared skid依次真实交入双方FIFO。最多两拍借槽足以容纳一个q2，旧W的完成不依赖下一组R。因此一方真实B/原parent capture较晚只是等待，不形成“W头等下一R、下一R又等这个B”的结构环。该证明依赖当前same-bank正常TC/DDR最终服务与mode2不detach；没有cycle期限。

F偶数时每流128个qW child；F奇数时 `ceil(F/2)+ceil((256-F)/2)=129`。所以普通预取并非忽略native WRITE parent或强迫它改len；它只让V共同裁一个页尾、正确合成各自original B。

若预取迟到，source第一次共同child停在F；两路W仍完整消耗这段已有数据并返回首parent B，physical R通道在该短child真实RLAST后可服务next-WR PTE，common前沿成功后才继续。若预取提前成功，后续所有WRITE child真实key都命中各自已成功recipe，无未知WR miss排在已held长source R后。不能把未来recipe计划、已发prep或TLB中碰巧有某权限当成这些成功事件。

## 6. 成本与接受范围

Depth4中的A slot2此前未激活，不等于该新增功能零状态成本。新增活动槽的现有布局包含正常响应record、seq32、valid/prepared两bit，布局计数为 `$bits(normal_resp_t)+32+2`，另计保持/匹配/页选择/前沿组合逻辑和1bit观察器。这不是通用必要位数：本合同seq0及固定响应字段可能被综合剪除。现out二者都声明整个array，也不代表未用常量槽不可剪除；本稿不报面积，不把20bit handover与一个prefetch slot直接作未经综合的等面积结论。

prefetch激活限定mode2、keep-q1、无cutover，top `:1026–1029` 有实际fatal检查；source模块还排外部pause。参数和general-offset driver验收由根负责，本稿没有运行新trace或声称next-WR确已先于rawAW2成功。

因为它是同job信息、真实WRITE授权、普通ahead translation与共同合法子块裁剪的强控制，本次应检验 §57 的性能收益是否只是缺少该能力的弱baseline造成。当前源码架构门接受；任何性能/正确性实验结论必须待实际完整copy、目标/未写区bytechecks、TL/PTE事件和raw/physical拍数验收后给出。

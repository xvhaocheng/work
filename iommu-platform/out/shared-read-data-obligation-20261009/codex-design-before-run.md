# §56 运行前：读membership与写响应分开退休

根Codex设计，out-only。A dst offset4088、2KiB copy，actual A第一WRITE parent仅1beat，第二255beat；V原父256beat，q2 W。全部权限正常成功，V全长WRITE sponsor。普通串行独立读为可进展强控制；也要给共同pagecut普通策略切V W边界的能力。

旧规则反例：V firstqW2位于A firstqW1前；只广播R0后A达到首成功WRITE前沿1，R1不得给A；A旧W/B等待V首qW2完成，V首qW2缺R1，detach等待A B，故形成真实依赖环。不是生产通用错误，§55窄F128/q2合同没有包括它。

新原则：future R membership可在A已接logicalR覆盖**全部已firstVALID的qW数据需求**后清除，不等旧W/B退休。qwriter本地offered child也算义务，不能只看global mux接受覆盖。旧qAW/W/B从不撤销，payload保持，V继续满足自己的qW。A未承诺的短W若全数据已经留在其实际FIFO，可在读退出期间继续提交，使原父B与次页header/preparation有进展；任何需要未来A R的新W仍禁止，直到V oldphysical stream退休后A fill可运行。

promised_end、next_child_end均由现有qwriter parent地址/offset/count计算，没有新payload。返回计数必须是实际DMA接受的logicalR，raw_written是job全局已交W；不能以计划R或internal offer充当已有payload。所有firstVALID held稳定、原父ID/RLAST/B，完整target+未写区与actual拍数继续检查。

普通共同pagecut对照可把两路W quantum同时对齐最小成功WRITE end，避免用固定q2对照制造自锁；它可能增加V一个W子请求。长R模式仍须V sponsor全覆盖，且仅A可读退出。故障/动态失效/多作业与all-member pause不在合同。因为如果ordinary同成本同决策，新规则不能独立命名算法；需要最强普通对照及原文边界支持贡献。

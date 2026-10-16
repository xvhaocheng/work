# LR1 frozen contract, 2026-10-15

目标：在source page旧数据已读取且全部旧授权/请求可安全退休时，归还真实物理页并撤销旧访问；目的W/B仍独立完成，无关DMA不因page receipt被全局停止。只比较源授权需要撤销的任务；单纯buffer reuse另列普通方案。

原资源：DMA0/1各512beat=4096B；aligned4096B DEFAULT copy、readonly源授权、physical source != destination；1PTW/cache16、bank各AR/AW4、TC W总32、不改原资源。page/job/epoch/source/allreaders/alias及真正队列/结果义务是收费输入。64B没有native fullbuffer例外。

执行对象：可信注册的源物理page和所有正式映射/读者，用(sourceID,job,context,PA object,mapping generation)识别。新封口只阻止尚未发布的旧代读；任一VALID已经发布就保持原header/value至accept，不以取消消除债务。登记不完整、物理alias/writer未解决、不完整pageR、RRESP错误、old raw/walk/OUT/held/accepted义务不为0均不成功release。

归还步骤：seal所有旧读端→完整4096B R receipt/error及old owner账本join→实际PTE撤销→必要CPU/cache同步→精确IOTLB失效且防oldwalk晚fill→target访问排序证明→ACK_RELEASE。ACK是page定向的新增私有回执，不是标准global IOFENCE.C完成。目的W/B/consumer最终完成独立核对。错误仅返回失败收据/隔离源页，不能冒成功ACK。

私有prototype为untimed可执行ownership/value参考。它带明确正常memory/ordering证书事件，缓存/translation义务抽象而非真实DDTC/PTW/RTL/DDR仿真；指明native尚缺接口，不给周期/FPGA值。以已读取oldbytes写完目的，ACK后producer覆盖源page检验目的仍oldbytes；无额外payload buffer。

固定普通程序接口 snapshot keys：request_epoch,current_epoch,readonly,nonoverlap,registry_complete,alias_safe,all_sources_sealed,all_readers_complete,read_error,old_obligations,authorization_revoked,cache_synced,invalidation_committed,target_ordering_proven,whole_copy_complete,request_active。step返回wait/seal/revoke/sync/invalidate/order/ack/error。各bool显式且strict；wholecopy先等实际所有reader B完成，phased允许合法target证明支持的轻同步，targeted-drain按准确原作scope。原语不是免费新证明。

创新门：对实际完整动作序列/所有权，与独立ordinary phased/targeted程序做具体映射，并完整计入登记、封口、端口/所有owner副本、来源数据收据、alias/epoch、PTE/cache/inval/order、错误清理、CPU/MMIO通知和所有完成工作。actions同且同具体实现成本没有优势，则本LR1方法主张关闭；不是全领域否定、发表全文覆盖或最小面积定理。源页更早可用不是全task收益。

同goal交付所有源码边界、primary、三ordinary程序、私有功能原型/原事件、成本与稳定裁决。差异通过后才最多四性能条件；没有差异不构造参数/时相/服务延迟/压力来救收益。最多两个工作日，无额外token预算；five-page sync+新checks后actual complete。root+两个新Sol6.1/ultra/forknone，Codex-only，原生context/no manual overrides；0old rerun/restored branch/seats/production修改/FPGA，本轮不代表论文完成。

Root implementation clarification: seal is an accepted-parent/authorization frontier. A registered4096B parent already owns its two declared read intervals; required unissued intervals, existing heldVALID and acceptedR remain counted and are drained normally. New old-generation parents or undeclared intervals are forbidden. Otherwise sealing after firstburst could prevent secondburst forever. This is not new queue admission or free extra headers: the original accepted job/remaining legalizer length is owned, and its observation/phase bookkeeping is charged to both LR1 and ordinary. Source receipt does not arrive until all declared intervals truly return.

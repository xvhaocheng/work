# MQ-IDENTITY-1 私有观察器交付

实现已完成；本席实际模拟次数为0。共享IOMMU任务Codex-only，未使用DeepSeek/Claude。请求配置Sol/medium；实际模型/强度运行元数据不可取得，标为unknown。

## 交付入口

- `source/`：从旧 `d-persistent-source-20261015/offline/instrumented/` 复制，排除build/及MQSim，保留旧boundary observer；没有写旧run或canonical源码。
- `identity-observer.patch`：相对旧instrumented的完整源码补丁。
- `pre-build-manifest.json`：最终构建前旧/新源码、全部实现脚本、完整patch及contract hash；`build-status.json`含最终binary SHA256。
- `runner.py`：仅交root审核，未执行。唯一输入2048；复制旧ssdconfig/workload字节；trace仍指向旧冻结绝对路径。stdin DEVNULL，60s timeout；新run目录拒绝覆盖。启动前保存所有源码、binary、contract、脚本和实际input SHA256，结束保存stdout/stderr/XML/status。

## 身份和hook语义

新增事件全部以identity_开头，输出stderr NDJSON；旧boundary源码和日志事件保留。Observer只写独立状态，不写模型字段、不添加模拟事件、不改变调度、分配策略或完成动作。

1. host生成：Trace生成计数递增后分配全进程monotonic host_uid，记录host pointer、flow、arrival、LBA、sectors、type。每次generation都覆盖pointer索引，避免pointer复用冒充旧身份。
2. SQE fetch：完整SQE构造后，以host pointer查host_uid并绑定SQE pointer/CID；device REQUEST_INFO完整初始化后、Handle_new_arrived_request前绑定User_Request pointer/native ID。user_uid等于其已绑定的唯一host_uid，绝不以recycled CID/native ID/pointer单独作为身份。
3. child lifetime：两个NVM_Transaction_Flash基类ctor末尾分配独立monotonic child_uid，保存source/type/stream/LPA/PPA与owner uid。仅读两ctor已经初始化的标量；不读Address/Physical_address_determined/派生未初始化字段。
4. AMU：barrier进入、read/write局部handler前release、delete前retire均记录child/保存owner/source/type/创建LPA；live stage另发identity_child_position记录当时LPA/PPA，不读取owner。AMU callback入口记录，可明确判断局部callback是否仅接受MAPPING source。另含mapping等待进入/退出、overfull等待进入/退出、MVPN临时child delete及unmapped cleanup delete。
5. Set_barrier成功insert后记录identity_lpa_lock(time,stream,lpa)；Remove_barrier原erase后记录identity_lpa_unlock。成功lock以原有find结果判断，未额外调用模型动作。
6. TSU_Base与FLIN submit记录；PHY原handler遍历前记录，handler遍历后delete前仅查observer快照并retire，不解引用owner或transaction字段。
7. cache user serviced广播前记录owner complete；Advanced cache原child detach/service前记录。TURNED_OFF冻结输入不走cache命中/驱逐策略；未改缓存逻辑。
8. CQE发送前绑定cqe pointer/user_uid/CID；host consume原取出Host_IO_Request后且delete前记录host_uid及cqe_user_uid并交叉核对。

lookup缺失或身份不一致只输出identity_error，不abort、不发模拟事件、不伪造completion。child retire后移除pointer索引，下一次ctor同pointer获得新child_uid。user/SQE索引在下一绑定覆盖；后续child事件只读已保存owner uid，永不读取已delete UserIORequest。

## 专用内存与限制

root审核指出observer std::map节点可能扰动模型通用heap。最终版用32MiB alignas(max_align_t)静态BSS bump arena作为所有observer map专用allocator；deallocate no-op，不改模型allocator/container。arena不足时捕获bad_alloc，输出identity_error并禁用后续identity记录，模型继续。若出现此错误，本次身份验收必须失败，不能据不完整日志下结论。

该改动排除了新增map节点与模型malloc的直接竞争；仍需root实际比对旧XML和过滤identity事件后的boundary全事件序列，静态审查不代替此验收。Creation owner归属是创建时归属，cache内部后来改变owner字段不覆盖该身份。cache detach表示模型从owner list移除，不冒充delete。程序退出仍存活/泄漏child以日志最后一动作呈现，不人为补delete或CQE。

## 构建账本

全部是新副本clean完整make -j2，单次180s上限；0依赖安装，0sim执行。

- build1：初版身份hook，成功；`build1-build-status.json`及stdout/stderr、pre-build manifest保留。
- build2：补齐MVPN临时child和unmapped cleanup直接delete，成功；`build2-*`保留。
- build3：root明确要求observer节点移至专用BSS arena，成功；`build3-*`保留。
- build4（最终）：root后续明确要求两个锁历史hook；结果见`build-status.json`，当前源码与最终manifest对应。

`static-review.json`记录原176份源码hash未变、旧代码行序列忽略缩进完整保留；最终接受仍由root直接审patch/runner并亲自执行唯一初始2048模拟。

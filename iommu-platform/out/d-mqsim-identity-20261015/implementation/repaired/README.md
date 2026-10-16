# MQ-LPA-REQUEUE-1 私有实施交付

本目录仅实施已确认根因的release-only修复候选；修复模拟实际0次。原identity `../source/`、`../run-2048/`及旧run没有改动。共享研究Codex-only；请求Sol/medium，实际runtime元数据未知。

## 源码变更

`repair.patch`相对已验identity源码，只有四文件改变：

- `Address_Mapping_Unit_Page_Level.cpp`：唯一模型语义变化在 `Remove_barrier_for_accessing_lpa`。原read/write multimap各用equal_range取指定LPA的原child指针，借用至局部list；两图全部detach后只erase本LPA锁，保留unlock事件；原child发resume观察后非空批次仅调用一次正常Translate。没有delete、retire、假handler、重建、owner list更改或child字段重置，未改overfull恢复等函数。
- 同AMU正常用户分配函数末尾仅新增只读identity_user_allocate，包含child_uid、owner_uid/user_uid、stream、LPA、old/new PPA、Locked_LPAs真实当前状态。沿用已存在old_ppa，不额外读取mutating CMT方法。
- `NVM_PHY_ONFI_NVDDR2.cpp`：Send_command_to_chip入口只读遍历借用list，为每child发identity_phy_issue及identity_child_position，没有新增调度调用。
- `GC_and_WL_Unit_Page_Level.cpp`：通过现有候选检查后、原GC state machine动作前，记录候选坐标/stream/Ongoing_user_program_count。
- `identity_log.h`：只增加两个观察辅助函数；现有map仍使用32MiB独立BSS arena，未改变模型allocator/container。

普通分配/Stop/GC movement与erase/TSU/PHY/user owner/CQE路径保持原函数负责。本版范围是四冻结输入的整页USERIO写、cache off；不宣称read/partial/cache的通用修复。若运行出现这些路径或locked allocation等，root按合同单列违反，不补fake write。

## 构建与证据

- `pre-build-manifest.json`：第五次（本修复第一次）构建前保存完整baseline/new source、全部脚本、patch、两份root合同、设计说明和全部四输入hash。
- `build5-status.json`：clean成功、完整make -j2成功、RC0、12.355秒，180秒上限。
- `build5-stdout.log`、`build5-stderr.log`：完整工具输出；原四次构建收据仍在上一级。
- binary SHA256：`7423e10efecf0129e240c249c0ccec9c9da10e80d89fe43528cd38aa1304016b`。
- `static-review.json`：baseline identity源码hash未变、修复源码与prebuild逐文件一致、4文件允许范围、全部脚本语法通过。编译成功不是动态修复正确性证明。

## 未执行的runner

`runner.py`仅供root审核后亲自调用。四个固定旧输入512/2048/4096/8192各一次，不自动重试；逐字复制原ssdconfig/workload，保留旧trace绝对路径；每次60秒、stdin DEVNULL。任何非零进程返回、timeout或进程启动错误都会保存result并停止后续case。`runs/`拒绝覆盖。

启动前核正式repair合同hash、源码与最终binary绑定；每次run前保存实际UTC/cmd/cwd、所有源码/binary、脚本/patch/合同/设计与实际输入SHA256；保存stdout/stderr/XML/result及总账。runner不把进程RC0冒充合同验收，不修改root成功规则。实际正确性、旧512边界/完整事件比较、身份逐UID因果链、锁/overfull/GC outstanding与负mutation检查由root负责。

以下 old/new 均针对 patch 文件自身文本；新增行保留 `+` 前缀。按文件分组。

## 文件 1：`iommu-platform/out/composition-ptwfix-20261005/workflow-revision-0.patch`

### R1（hunk 头；Codex 可自行 recount，建议同步）
OLD:
```text
@@ -1,7 +1,94 @@
```
NEW:
```text
@@ -1,7 +1,90 @@
```

### R2（需求 1：正常 sync+JTAG 命令前先结束 XVC/其他 JTAG 占用）
OLD:
````text
+### 日常：同步并重载当前 BIT（不带 `--payload` = 不写 SD）
+
+```sh
````
NEW:
````text
+### 日常：同步并重载当前 BIT（不带 `--payload` = 不写 SD）
+
+- 执行前先确认并结束已有 XVC / 其他 JTAG 占用，确保同一时间只有一个 JTAG 使用者。
+
+```sh
````

### R3（需求 3：`--boot-timeout 600` 改 `900`）
OLD:
```text
+  --boot-timeout 600 \
```
NEW:
```text
+  --boot-timeout 900 \
```

### R4（需求 2：仅 Linux 变更加 `--payload` 时，`--expected-kernel` 换实际交付版本）
OLD:
```text
+- 固定流程：正常工作走以太网、SD 留板卡；仅 RTL 改变时 sync + JTAG 重载指定 BIT、不写 SD；只有 Linux 改变时才加 `--payload <fw_payload.bin>`，网传后只写 32 MiB 第一原始分区并 sync，再用 JTAG 重载已验收 BIT，不覆盖整卡。
```
NEW:
```text
+- 固定流程：正常工作走以太网、SD 留板卡；仅 RTL 改变时 sync + JTAG 重载指定 BIT、不写 SD；只有 Linux 改变时才加 `--payload <fw_payload.bin>`，此时 `--expected-kernel` 必须换成该软件实际交付的 `uname -r` 版本，不沿用旧 net4trace；网传后只写 32 MiB 第一原始分区并 sync，再用 JTAG 重载已验收 BIT，不覆盖整卡。
```

### R5（需求 4：异常卡死标题，落实 DS 免 sync 历史例外，不称脚本 rescue CLI 开关）
OLD:
```text
+### 异常卡死：先保存证据，再用已有明确 rescue 模式（故障例外，不是默认流程）
```
NEW:
```text
+### 异常卡死：先保存证据，再释放 JTAG 后手动恢复（此前已确认完全卡死的 DS 恢复例外，不是默认流程）
```

### R6（需求 4：保留先存现场再释放 JTAG 后恢复，且 openFPGALoader 不是脚本现有 rescue CLI 开关）
OLD:
```text
+先保存可取得的故障输出；UART 诊断保留且只能有一个采集者。确认既有 JTAG XVC 已正常退出后，直接加载同一已验收 BIT：
```
NEW:
```text
+先保存可取得的故障输出；UART 诊断保留且只能有一个采集者。确认既有 JTAG XVC 已正常退出后，使用下方 openFPGALoader 手动加载同一已验收 BIT；这不是脚本现有 rescue CLI 开关：
```

### R7（需求 4：免 sync 只对应 DS 完全卡死历史例外，本次 PTW 是正常 sync）
OLD:
```text
+这一步不写 SD；恢复后用 SSH 读取实际内核、boot_id 与网络状态。恢复成功不代表失败实验已通过；无法先 `sync` 只是本次授权例外，不能把免 sync 固化成正常流程。
```
NEW:
```text
+这一步不写 SD；恢复后用 SSH 读取实际内核、boot_id 与网络状态。恢复成功不代表失败实验已通过；免 sync 仅适用于此前已确认完全卡死的 DS 恢复历史例外（本次 PTW 上板走正常 sync 流程），不能固化为默认流程。
```

### R8（需求 5：关闭 trace 是当前内核要求；单次 PN 仅本轮排错，不作为每次 172 常规预跑）
OLD:
```text
+- 正式 172 前关闭 trace（`echo N > /sys/module/idma/parameters/composition_trace`）；单 PN4096 只是前置诊断，不是 172 的一部分。
```
NEW:
```text
+- 进入正式 172 前关闭 trace 是当前内核要求（`echo N > /sys/module/idma/parameters/composition_trace`）；单次 PN4096 仅属本轮排错，不是 172 的一部分，也不作为每次 172 的常规额外预跑。
```

### R9（需求 6：summary.json 存在不算完成；必须 status/planned/attempted/completed/failed 与 case problems 达标；失败保留 STOPPED 与原始结果；不预填当前结果）
OLD:
```text
+- 全量 172 运行中，目录 `results/composition-ptwfix-20261005-formal01`；待真实出现 `summary.json` 或 `STOPPED.txt` 后，在此填 planned/attempted/completed/failed 与结论。不得预填通过。
```
NEW:
```text
+- 全量 172 运行中，目录 `results/composition-ptwfix-20261005-formal01`；不得把 `summary.json` 存在当作完整完成，必须 `status=complete`、`planned=attempted=completed=172`、`failed=0`，且各 case 检查无 problems，方可在此填结论；失败保留 `STOPPED.txt` 与原始结果。当前不预填通过或任何运行结果。
```

### R10（需求 7：删除重复“安全与失败边界”小节）
OLD:
```text
+### 安全与失败边界
+
+- 不夸大板端完整 IO/CDC 签核，也不写成机制创新或性能收益。
+- 不做 payload hash、SD 回读、冗余测试或新框架；不做串口抢读。
+
```
NEW:
```text
（空，删除以上 5 行）
```

---

## 文件 2：`iommu-platform/out/composition-ptwfix-20261005/workflow-revision-1.patch`

### R1（需求 5：PN4096 仅本轮排错；关闭 trace 是当前内核要求）
OLD:
```text
+- PN4096：此前失败，本次实际 1/1 实板通过，V/A data errors 0、taint 0；boot_id `2695d848-8589-491e-bf7f-1de7af5ea9cb`。单 PN4096 是前置诊断，不是 172 的一部分；正式 172 前关闭 `/sys/module/idma/parameters/composition_trace`。
```
NEW:
```text
+- PN4096：此前失败，本次实际 1/1 实板通过，V/A data errors 0、taint 0；boot_id `2695d848-8589-491e-bf7f-1de7af5ea9cb`。单次 PN4096 仅属本轮排错，不是 172 的一部分，也不作为每次 172 的常规额外预跑；进入正式 172 前关闭 `/sys/module/idma/parameters/composition_trace` 是当前内核要求。
```

### R2（需求 6：summary.json 存在不算完成；给完整完成条件与失败保留）
OLD:
```text
+- 完整冻结 172：当前在 `results/composition-ptwfix-20261005-formal01` 运行，尚无完整结果，未完成/未全部通过；不得预填通过或宣称研究收益。旧 net3 记录 52 通过/1 失败/119 未运行保留，未验收，不自动重跑。
```
NEW:
```text
+- 完整冻结 172：当前在 `results/composition-ptwfix-20261005-formal01` 运行，尚无完整结果，未完成/未全部通过；不得把 `summary.json` 存在当作完整完成，必须 `status=complete`、`planned=attempted=completed=172`、`failed=0` 且各 case 无 problems 方可判定完成；失败保留 `STOPPED` 和原始结果。不得预填当前运行结果或宣称研究收益。旧 net3 记录 52 通过/1 失败/119 未运行保留，未验收，不自动重跑。
```

### R3（需求 1、2、4：正常 sync+JTAG 前结束 JTAG 占用；Linux 变更 `--payload` 时换 `--expected-kernel`；免 sync 只属历史 DS 卡死例外，本次 PTW 正常 sync，openFPGALoader 不是脚本 rescue CLI 开关）
OLD:
```text
+- 固定流程：正常工作走以太网、SD 留板卡；仅 RTL 改变时 sync + JTAG 重载指定 BIT、不写 SD；只有 Linux 改变时才网传 `fw_payload` 后只写 32 MiB 第一原始分区并 sync，再 JTAG 重载已验收 BIT。异常卡死先保存证据，再用已有明确 rescue 模式；免 sync 只是本次例外，不固化为默认流程。
```
NEW:
```text
+- 固定流程：正常工作走以太网、SD 留板卡；仅 RTL 改变时，先确认并结束已有 XVC / 其他 JTAG 占用，再 sync + JTAG 重载指定 BIT、不写 SD；只有 Linux 改变时才网传 `fw_payload`（`--payload`），此时 `--expected-kernel` 必须换成该软件实际交付的 `uname -r` 版本，不沿用旧 net4trace，网传后只写 32 MiB 第一原始分区并 sync，再 JTAG 重载已验收 BIT。异常卡死先保存证据，再释放 JTAG 后手动恢复；此前已确认完全卡死的 DS 恢复曾使用免 sync 例外，本次 PTW 上板是正常 sync 模式，不能把免 sync 或手动 openFPGALoader 称为脚本现有 rescue CLI 开关，也不固化为默认流程。
```

未决一句话：`workflow-revision-0.patch` 的 hunk 头行数已按“+1 行新增、-5 行删除”预估为 `+1,90`，最终仍应以 Codex 机械替换后实际 recount 为准。
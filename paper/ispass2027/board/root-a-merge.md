# Root A 分支的后续合并

Root 只推送 `origin/ispass/root-a`，没有合并到 main。所有实验入口、验证回执、
真实数据和失败记录均保留在本分支；当前进度以 [队列](queue.md) 和
[状态](../status/root-board.md) 为准。BIT、Image、payload、PDF/PNG 本机保存，
各包 manifest 给出路径；Git 中保留结果 CSV、SVG、源码和可追溯元数据。

## 在独立集成 worktree 中审查

以下供后续获授权的集成者执行，本任务未执行这些合并步骤：

```sh
git -C /home/xvhaocheng/work fetch origin
git -C /home/xvhaocheng/work worktree add ../work-root-a-integration \
  -b ispass/root-a-integration origin/main
git -C /home/xvhaocheng/work-root-a-integration merge --no-ff --no-commit origin/ispass/root-a
```

先处理下面的重叠和数据身份，再完成集成提交。主工作目录保持原分支；不强推、
不重置或覆盖并发工作。内部组件的实验 commit 使用各包的 bundle/patch 恢复，
沿用独立实验分支，不把本地实验分支推向第三方上游，也不把内部源码误作顶层 gitlink。

## 已知重叠

[只读比较](root-a-merge-overlap.json) 对比 main `7123bbfe79cca298c36a880206ab85a2d68e6f67`，
共同祖先 `01f981a9c603fa909fdb2fb22450aa7c5197271f`：两侧改过56个相同路径，
其中17个当前 blob 不同。这是潜在重叠清单，尚未执行 merge，不代表17个实际冲突。
main 后续变化需重新核对。

- DS 构建/采集入口：`build_baseline.py`、`collect.py`、`run_baseline100.py`。
  保留 DS25/100、现有通过行为和来源角色；不可用一侧文件整体覆盖另一侧的新修订。
- `packages/ds-sweep-20261017/` 的 manifest/source/host验证及 `ds-sweep.md`。
  区分构建原件、准备快照与最终运行来源。
- DS campaign 和 `ds25-all-load/{load.json,program.tcl}` 同路径而不同字节。
  原始身份不能按“较新文件”直接覆盖；核实独立 DS 会话的实际成功记录与 Root
  在 UART 锁前失败的重复启动，分别保留真实来源，不把失败启动计作新试次。
- E3 的 `analyze.py`、`capability.md`、`change-request.md`、`run-package.md`、
  `subset-results.md`：合并 main 的并发内容与本分支已完成 P0 / 后续完整 E3 的版本区分。

## 共享汇总待补

本任务没有修改共享 `README.md`、`CURRENT_STATUS.md` 或 `data/board_results.csv`。
集成者需补论文包导航、当前已完成/未完成状态，并按共享表的原 schema 摘录结果；
不要直接把17列长表的统计行当作额外板测行追加。

- E1b 最终包200试次含历史 DS0/50 的100和 DS25/100 新100。main 已有同一 DS
  会话的部分或全部数据，按实际 BIT / boot / observer sequence / condition / DS 去重。
  中途累计包与最终包不能叠加。
- P0最终45、角色互换30、E4最终20各自为独立 cohort；E5桶数据复用同一 E4 的20次，
  新增板测数为0。G及完整E3以各自最终真实包为准，不与旧协议混池。
- E2 两个软件版本各5个已接受点仍是独立 partial，失败的第6点不计为性能样本；
  默认 off 探针的后续运行只用于诊断，不并入这些性能重复数。
- 每个汇总数字保留实际软件/硬件 manifest、原 raw 和长表 source pointer。
  DS0作为主结果；DS25/50/100注明数据结构访存附加延迟的敏感性分析。

完成文字/代码冲突处理后，只运行涉及改动的现有主机检查。已经完成的实际板测不因
合并重复运行；需要重建或再测的实质变化另立新身份、新目录并由唯一板队列安排。

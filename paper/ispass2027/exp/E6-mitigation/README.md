# E6 历史缓解的有界移植设计

来源设计已由Root接受，选择G(P4M)先行，M(S/N/P/PN)只保留设计。
第一小批frontend已经独立实现并有限编译/验证，见`frontend-validation/component.json`和原diff；
组件commit `2c7c68f59190b9ce8739efd7b67d92262b0021f0`，39场景通过。
observer/wrapper已由Root直接审阅接受，组件tip `0f0403f56aff3f1ee10e18dccea7a9fd6f2f26a4`，
实际diff/原件入口`observer-validation/`。上层仅暴露默认off参数，新增CVA6 `cfb9a0a8`；
独立构建export显式启用G，平台 `d5cc448`，见`build/README.md`。
Root审builder后，唯一G DS0 BIT构建于2026-10-17 15:08:57Z启动，尚未通过全SoC/时序/面积验收。
G Linux首批UAPI/只读CAPS/纯decode已被Root接受；后继RUN tip`69d8b9d4`已由Root直接审实际源码并复跑有限检查接受。
唯一正式G软件已构建passed，见`software-source/README.md`；未做新G板测。实际G BIT已通过合成结构门，仍在原place/route流程，尚未最终时序/面积验收。
当前BIT构建的Linux65b仍仅作reference，不因后续caps首批修改而追认成G软件已完成。
分支 `ispass/e6-mitigation` 从main `7123bbfe79cca298c36a880206ab85a2d68e6f67` 创建，
本工作树只写本目录；组件独占工作树为`/home/xvhaocheng/work-a1-e6-cva6`，同名分支，从冻结6c5起。
Root审实际frontend diff后才进入下一小批；E4、完整E3优先。

- `design.md`：原版锚、固定容量与服务合同、P4M gap及完整工作代价、最小移植/验收/板点上限。
- `g-interface-proposal.md`：Root已锁定的G cap/地址、32word observer布局及有限A兼容点；observer尚未实现。
- `frontend-validation/README.md`：本批实际frontend范围、原始失败/通过、bundle恢复与有限命令。
- `observer-validation/README.md`：实际observer/wrapper小批，G-on19、G-off17+hist18、真实配置整链69项，明确未验证的完整系统边界。
- `build/README.md`：Root已审最小顶层diff、精确source/lock、复用builder及恢复包；一次50MHz构建实际dispatch，不含板操作。
- `linux-validation/README.md`：独立ABI6/CAPS0x11/RUN保留0x12、60s输入边界、真实函数329checks及RV64对象/异常表原件；未接RUN。
- `linux-design/README.md`：原锁与STOP调用链原文、已锁定G独立256+4096自然完成合同、待审下一批RUN设计。
- `linux-validation/g-run/README.md`：G RUN候选actual全diff、1176项实际函数夹具、旧38/57/97/416+6及精确MMIOtrace、最终RV64对象。
- `cli-validation/README.md`：独立ABI6静态CLI四固定条件、507项实际main假syscall/UBSan、原始错误保留及RV64 ELF。
- `software-source/README.md`：新platform6491/Linux69d绑定、唯一正式软件builder、9项rootfs/拒绝门检查，以及已passed Image/payload/匹配SPI与transport。
- `source-map.md`：可导航的实际组件commit、文件行号、历史执行输入与新组织commit的区别。
- `evidence/index.json`：34份只读Git源码/原报告的路径、blob/SHA256、来源角色。
- `evidence/isolated-source-comparison.json`：7份IOMMU关键源码与历史isolated实际构建副本逐字节相同。
- `evidence/historical-identity-rows.json`：原历史身份表的三行完整选摘，保留dirty/overlay与未知项。

`source-audit.json`的设计文件hash对应最初来源设计批`4f1ed24`中的字节；
源码/报告原件未改。本次组件实际测试源码hash另由`frontend-validation/component.json`绑定。

历史P/N/PN的负结果、P4M启动混杂/采集失败与跨boot追认均保留。
本批不恢复旧机制探索、普通对照判别门或停止审查，不将任何新设计写成已验证功能。
研究与本批设计由Codex完成；证据中旧报告的DeepSeek等归属仅指当年的历史工作。

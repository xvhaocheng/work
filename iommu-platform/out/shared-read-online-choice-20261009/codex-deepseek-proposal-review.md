# Codex 对 DeepSeek 三个独立提案的关键复核

原完整输出见 deepseek-positive-proposal-answer.md 与 raw.json；模型flash/high，总usage99801，未截断。Worker没有执行或浏览，本稿由Codex直接核源码/来源。

M1共同授权许可：不能要求不同device的generation数相等。真正条件是每方自己的完整key/seq仍匹配自身live parent；helper hit_bypass_parent_translation.sv:103–107 已检查success/error/ignore/MRIF和真实r/w/bypass，不是仅信任请求attrs。WR前沿来自实际normal成功translation。一个永久sticky“gen不同永不加入”也不能代替IOFENCE/IOATC invalidation合同。当前§73重新核own grants+PA/attrs是正确保守接纳，但普通permission validation不能单独作为创新。

M2只在R完全退休后退组：不能解仍在途的长R背压问题。若退组判定需要skid blocked，又规定必须无skid/无busy才能退，两条件不相容；N端点R退休已接近job末尾。要有中途边界需short children或额外可排空规则，并计算buffer/metadata/AR成本。Worker没有给真实DUT见证。

M3错误扇出：不能仅删source $fatal后复制RRESP就声称DMA恢复。平台iDMA NO_ERROR_HANDLING，qwritererror来自B；closed receiver会使heldR退休条件无法成立，必须先定义abort/drain/完成状态和错误归属。正常static复制PASS不覆盖fault/remap。

真正复核产物：§72新公共prefix把原已完成V授权错误地继续作为A独立suffix前提，8个真实watchdog；source在independent分支改为只检查ownA后全部24PASS。§73取消A全许可对V closure/continuation的交叉栏杆，但目的数据仍不借许可；真实30copyPASS。以上是Codex亲自诊断/设计/执行，不是声称Worker实施过。

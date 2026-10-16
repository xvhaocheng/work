# §53 根验收：已有私有FIFO的有限先读

out-only真实RTL：1次build成功，12次完整copy全部PASS。根席另从12份CSV核source AR声明拍数与目标实际W退休拍数：每个目标256拍，logical R各2048B，issue/done各1；完整payload/全部周边未写区由driver检查。SHA与逐次配置见execution-ledger.json。

不增加payload队列；复用B3字节FIFO允许最多2拍logical R相对已接raw W领先，冻结skid仍逐member交付。最后logical R必须已有最后W覆盖，保持V尾部AW先于A补读AW。singleton读严格按已有W债务截断；暂停不能阻挡已承诺W所需R。

mode1与普通mode2/q256无暂停CSV完全相同，V/A5657/5648、sourceAR1/R256。q2为8579/8572、AR128/R256。单长暂停mode1与q256同5161/19847、AR125/R504，q2为9928/19893、AR252/R504。两长暂停同5161/29792，对照q2为9928/29838。

早恢复mode3 V/A13770/13761，相对§52同配置13855/13848仅约0.6%变化；单/两次早恢复均成功，但10cycle窗口明确没有quiet要求。sticky旧策略13274/28072，需要R504；这是普通保留cohort策略的代价区别，不是新算法。

因为同信息普通q256仍逐字同轨迹，有限先读只得到小实现收益，这一项不能作为独立论文贡献。不把FIFO手算当面积，不宣称已测最大occupancy；没有综合、BIT或板卡动作。下一用真实跨页WRITE父与成功授权前沿判断recipient各自等待是否能产生不同完整成本点。

Codex设计/实现/诊断/执行与验收，DeepSeek此前贡献基础夹具；本阶段未新增DeepSeek调用或Claude调用。

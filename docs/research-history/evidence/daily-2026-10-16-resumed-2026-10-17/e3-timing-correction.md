## E3证据等级更正（2026-10-17恢复核查）

本目录此前无条件称E3 v6为“实板受控因果”、或称负时序未绑定实际映像的表述，须由本节限定。**127/129/129/130周期是同一时序未收敛E3构成下的配对观测，不能表述为完成时序验证后的定量因果结论。**原始数据和原报告不改写。

- 冻结E3报告全局/50MHz域WNS均−6.470ns，TNS−8455.683ns，5496个setup违例端点；最差十条setup路径含observer error与data寄存器。动态激活、约束例外及可达性未审，不能降格为无关告警。
- 最终buildlog串联routed报告→同DCP→写BIT→work-fpga导出；交付、Mac现存加载文件和采集预期BIT SHA均为`b55f5afbba667a7540ed003dd724d078e0e6e4bff5d30fe7e796abba5b7f2ecc`。这是存档来源链；当时加载记录仍`sha256=null/hash_verified=false`，无板内配置回读hash或唯一BIT ID。
- 不能据STA失败宣称四组数据错误或推定误差；也不能用数据/UART一致代替时序闭合。50MHz是配置/STA值，独立实测未知，不能只据WNS推导安全频率。
- batch和E2 role1各自报告WNS+0.177ns、setup违例0；不能把E3失败扩散给它们。通过范围仍仅为现有约束，非完整CDC/外部IO签核。
- 对应原件、SHA及实际抽读行段见本轮`timing-dell.json`、`timing-mac.json`和reading-ledger。E3原有64B/人工延迟/一次启动、仅case6触发前时刻一致等限制同时保留。


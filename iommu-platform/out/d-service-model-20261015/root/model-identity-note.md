## Material Passport

2026-10-15，root对已冻结D-CS1投影的身份与结论范围说明；不改参数、动作或结果选择规则。

两个owner是两个研究翻译上下文/入口的抽象身份，task、buffer及缓存分区按owner区分。它们不是从SPDK的QID、CID、ctx或LBA推导的真实DID/PSCID，也没有声称同一NVMe控制器的两个qpair天然拥有不同Requester。共同外部device-work server是明确的研究服务假设，不证明某个NIC/SSD部署的共享关系。未来原生版本必须另外绑定实际Requester→bank/context/页表/共享结构；不能用本模型owner标签补证。

P_M/PN_M是本次根席合同里的付费旁路条件抽象，保留普通P正常重叠和有限N探测/响应。缓存分区、LRU、聚合写服务、CPU poll与服务单位都不构成RTL逐周期、PLRU或AXI协议等价。各项setup/warm/清理费用完整，只指已列出的投影生命周期；真实控制队列翻译、PCIe/一致性、面积/时序和部署成本依然未知。

对相同reference状态克隆目标动作，只用于离线反事实分析。它不是原机可以免费复制隐状态的付费试选接口。精确O0后验也是信息上界，不能称为已实现的低成本在线策略；对应模型若无信息增量，只停止这个有限服务族的新信息主张。

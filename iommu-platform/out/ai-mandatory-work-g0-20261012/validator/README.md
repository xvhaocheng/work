# 独立验收器

`validate_g0_records.py` 不导入 root/compiler。它从固定 OPT-125m 形状和 G0 合同独立重建：每侧六笔完整张量、身份/实际通道、共同 ready0、四个10MiB槽、4KiB/2MiB/单DID布局、完整有效字节与 rank-distinct fixture。

验收原始 `events.csv`、`summary.json`、两个 `rankN-output.bin` 和 `metadata.bin`。逐笔核真实 issue/DONE 与实际 polled NEXT/DONE，NEXT从2递增；拒绝重复/漏完成、角色/通道错绑、提前重用通道、隐藏ready等待、DDR地址覆盖缺口、完整输出/holes/inactive guard破坏及最终未排空。真实DDT/PT dump逐叶检查PA/权限，按每个DID独立枚举可达页，拒绝跨角色别名和额外guard映射；单DID合法拥有四槽。

`--static` 只验冻结输入与八案命令/两份源fixture；不能验收RTL。`--run` 验一案，`--suite` 验八案，`--negative-cases --run` 在验收通过的记录副本上做五种必要破坏，原始记录哈希必须保持。

```bash
python3 out/ai-mandatory-work-g0-20261012/validator/validate_g0_records.py \
  --static --report out/ai-mandatory-work-g0-20261012/validator/static-acceptance.json
python3 out/ai-mandatory-work-g0-20261012/validator/validate_g0_records.py \
  --run out/ai-mandatory-work-g0-20261012/runs/4k-parallel \
  --report out/ai-mandatory-work-g0-20261012/validator/4k-parallel-acceptance.json
python3 out/ai-mandatory-work-g0-20261012/validator/validate_g0_records.py \
  --suite out/ai-mandatory-work-g0-20261012/runs \
  --report out/ai-mandatory-work-g0-20261012/validator/suite-acceptance.json
python3 out/ai-mandatory-work-g0-20261012/validator/validate_g0_records.py \
  --run out/ai-mandatory-work-g0-20261012/runs/4k-parallel --negative-cases \
  --report out/ai-mandatory-work-g0-20261012/validator/negative-acceptance.json
```

边界：两个raw输出文件只有完整张量extent=9,457,664B，extent中的holes与inactive role完全独立核查；其余10MiB槽尾、48MiB backing guard与源保持使用driver逐字节assert。DDR AR/AW/B和TL握手逐条独立核，R/W逐beat统计和协议稳定性使用真实runtime observer。硬件自然quiet可早于软件随后读到DONE；两者分别保留，validation_end必须晚于软件完成并含64周期quiet确认。observer job IDs只绑定证据，不能用于driver admission。

目前 `static-acceptance.json` 已通过；八案实际结果与负例尚待root提供。`execution-ledger.json`记录脚本冻结哈希、接口核读、边界和实际执行。

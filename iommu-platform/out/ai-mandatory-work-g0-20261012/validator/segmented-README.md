# 分段运输独立验收

`validate_segmented_records.py` 是新增文件；已冻结的 whole validator 和首案/负例报告保持不动。旧 whole 八案只有首个4k-parallel完整验收，其余未完成，不能形成配对G0或普通方法的性能负结论。

新运输将同一完整张量按张量内≤4096B分段，两个4718592B矩阵各1152段、6144B偏置两段、三个1536B小张量各一段；每rank2309描述符、六完整张量、9447936B。两rank共4618描述符、18895872B。所有逻辑ready仍为0；角色、物理通道、源/目的、40MiB映射、48MiBbacking及fixture不变。4096B来自功能失败后的实际FIFO容量依据，是安全运输修订，不能说它是最快长度。

新验收器独立重建全部segment区间和 `job_id=role*2309+index`，不导入root compiler。每tensor只有所有segment真实DONE后才完成；role须六完整tensor和2309描述符都完成，才可产生完整角色就绪。实际NEXT/DONE从2递增，单设备全配对为2–4619；双设备各2–2310。ready0到issue、实际DONE到软件polled DONE的等待全部保留。原先raw DDT/PT权限/角色隔离、DDR完整覆盖、两个完整输出/holes/inactive guard和自然排空检查保留。

```bash
python3 out/ai-mandatory-work-g0-20261012/validator/validate_segmented_records.py \
  --static --report out/ai-mandatory-work-g0-20261012/validator/segmented-static-acceptance.json
python3 out/ai-mandatory-work-g0-20261012/validator/validate_segmented_records.py \
  --run out/ai-mandatory-work-g0-20261012/segmented-runs/4k-solo0 \
  --report out/ai-mandatory-work-g0-20261012/validator/segmented-minimal-acceptance.json
python3 out/ai-mandatory-work-g0-20261012/validator/validate_segmented_records.py \
  --suite out/ai-mandatory-work-g0-20261012/segmented-runs \
  --report out/ai-mandatory-work-g0-20261012/validator/segmented-suite-acceptance.json
```

静态合同、八TSV与root生成的全部segmented job列表已通过。实际分段最小功能、副本负例和八案结果尚待root执行；验收器本身没有跑DUT。原始数据与失败尝试保留，冻结哈希及后续实际检查登记在 `segmented-execution-ledger.json`。独立raw输出extent之外的槽尾/8MiBbacking guard和逐beat R/W协议证据仍用driver运行时断言，不能当Linux/cache/MIG或AI推理测量。

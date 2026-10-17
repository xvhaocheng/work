# OBS5／Depth64：被动容量批

2026-10-17，Codex-only、ARS experiment-agent/run。Root已直接审查并接受四文件diff与有限结果。
实际CVA6组件 `1d587125eee690e623e6ffae13e70f9bbe989adc`，parent为已接受BTC3 frontend
`2db78c0927d6e70b819b46651df86e82ab139710`。没有新BIT、没有板测，也没有ABI5 driver。
完整身份／四个源码SHA在 [component](source/path64-component.json)，
[原始diff](source/path64-review.patch)、[commit patch](source/cva6-path64.patch)、
[增量bundle](source/cva6-path64.bundle)已保存，bundle实际verify通过、前提2db78c。
组件没有用户现有可写remote，不向上游推送；恢复包随顶层任务分支交付。

参数从`ariane_xilinx.DMAPathQueueDepth`经peripheral、observer到全部16个实际stage，所有默认值4。
完整E3专用builder将显式选择top generic64；当前未建该BIT，不能把默认4描述成64。
observer仅允许默认4，或PathEnable+HistEnable的64；其他配置magic0且initial fatal。
默认4保持OBS4／INFO1；HistEnable0仍是OBS3。64使用OBS5 `0x49444d414f425305`、
HIST_INFO格式2，bits55:48为64、bits63:56保留0、bits47:32仍为原直方图overflow mask。
其余INFO字段、计数、开关窗、carry、overflow失效和饱和行为不改。
非法容量16不会被标成OBS4或E5原Depth4。

硬界仍是[已接受设计](full-bounded-design.md)的同lane单COPY、同S/D offset、
CONF.deburst=decouple0、≤64KiB：每COPY≤33burst，stage0保守≤34，故选64。
本批未改变`dma_path_stage`的事件、计时或错误算法，只将既有Depth参数传入；容量增加不是删除overflow检查。
所有observer输出仍只到观测MMIO，参数不连接DMA ready、AXI或translation控制。

## 实际验证与原始失败

[联合资格回执](test-results/path64-qualification03.json)明确组合原02的四项成功执行和独立03的非法容量续验，
没有重写原执行manifest。

| 实际执行 | 结果 |
| --- | --- |
| default4、Hist1与冻结E5 6c5c666逐周期gold | 18集成情境通过；两次clock eval均比较MMIO、35计数、128 PATH字段、512桶与overflowmask |
| default4、Hist0与同一E5冻结源逐周期gold | 17集成情境通过，OBS3原行为保持 |
| 显式64实际observer | 34情境通过；含四物理port×两role×两channel的33burst三波，不清窗就跨过64深head/tail回绕 |
| 每个实际stage的64条结果行 | full99，area=sum；所有16 stage均覆盖，两role的映射独立检查 |
| 实际stage Depth64 | 98个32-bit年龄边界、413个独立模型步；满64同时pop/push、第65条overflow、carry、clear、饱和、timer与abort失效保持 |
| 非法depth16探针 | 独立03编译成功，运行按预期fatal，不产生有效旧版本 |

02原suite最后的非法探针缺少C++ `sc_time_stamp()`定义而链接失败；前四项真实执行已经成功。
只修复该测试程序后，在`path64-invalid03`重编/运行这一项；生产source两次逐字节一致。
02原manifest仍为failed，原链接log保存；联合资格回执检查相同source与每条实际exit code。
01更早的wrapper遗漏NumPorts/AxiIdWidth参数，在编译阶段失败，也原样保存。
这些是测试程序修复，不修改生产RTL，不把链接失败当成预期参数拒绝。

原始[02 manifest](test-results/path64-02-manifest.json)、[03 manifest](test-results/path64-invalid03-manifest.json)、
[非法实际fatal输出](test-results/path64-invalid03-invalid16-run.log)、
[64实际集成输出](test-results/path64-02-capacity64-run.log)、
[stage输出](test-results/path64-02-stage64-run.log)均可直接核读。
全部实际编译source与各版本test保存在`source/path64-02/`、`source/path64-invalid03/`；
原01快照也保留。新runner的`--only`明确记录选择，单项passed不冒称整套已重跑。

复现默认整套或单项（新output路径，host finite RTL，不访问硬件）：

```sh
python3 paper/ispass2027/exp/E3-workloads/verify_path64.py \
  --cva6 /home/xvhaocheng/work-a2-p0-cva6 \
  --output paper/ispass2027/exp/E3-workloads/artifacts/path64-NEW
```

这是实际observer／stage RTL的有限测试；受控事件波不是backend同时库存的实测上界。
backend长COPY既有真实证据在long-backend04；本批没有重新执行backend／完整IOMMU，
也未编译完整SoC的top/peripheral依赖。后者当前只是实际源级参数连接，须由后续Vivado完整实现验证。
没有删减数据、PATH、排空或时序检查；未生成新Linux，也未改变A1 E5已冻结build archive。

## 后续构建边界

Root允许ABI5软件之前独立准备／构建full-E3 DS0硬件，先审实际builder和固定platform pins再启动。
新out限定本worktree的E3/artifacts，top generic64，manifest与routed边界明确BTC3／OBS5／Depth64。
50MHz原约束、WNS≥0、失败端点0和routing0原样，失败即停、不改约束。
硬件source archive可记录已审Linux `da6b69fc007ad5e671dc288dd8038d02e3ebe43d`，
它没有ABI5，未来正式软件另从同一基座完成driver后独立冻结，不把当前Linux标签伪称新软件能力。

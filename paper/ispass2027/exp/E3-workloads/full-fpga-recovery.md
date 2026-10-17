# Full E3 实现中断与检查点续编

原full-e3-ds0-build01未得到最终实现结论。原exec session60739返回143，原manifest保留stale running，
原日志/manifest一字不改。最后输出是2026-10-17 15:29:13Z的route overlaps74742，
没有routed/routed_error checkpoint、route end/error marker或最终时序/布线资格。
发信号者与原因未知，不能把143冒充Vivado时序失败、资源失败、OOM或工具寿命已确诊。

独立回执 `test-results/full-fpga01-execution-terminated.json` 绑定原manifest/log、原生成TCL、generic64和五个DCP清单。
`source/full-fpga01-execution-terminated/` 保留原impl TCL/日志/placed util、两个原XDC、clock-gen TCL，
以及physopt DCP内四份完整early/late/main/board XDC和dcp.xml的原字节。
原placed资源LUT180238、regs175733、Slice50950/50950（100%）、BRAM50/DSP27仅为布线前资源，不等于routing通过。

最近route前检查点为原 `ariane_xilinx_physopt.dcp`：123540943B，2026-10-17T14:40:13.342182Z，
SHA `0f1fe07fed706f9461efeb1b84294ef60bf20394d2392a3b36e20fc91d69340f`。
原manifest SHA `ebeeb7dee87b57591c01ef11ea656e2dc38c0cac8c1c0ab1b8c5e18e05b8d597`。
两个顶层XDC匹配原CVA源码；clock-gen仅保留原baseline exporter已有board.repoPaths前缀，实际SHA仍匹配原manifest.sources。
未改变50MHz配置。DCP声明目标xc7k325tffg900-2、Genesys2，嵌入全部约束。

## 待Root源码验收的小恢复入口

`resume_full_fpga.py`先prepare、再显式launch。prepare仅host复制身份/原日志与source，hardlink已锁定physopt DCP；
不会调用Vivado。hardlink只读使用，不chmod共享inode；实际TCL从不write到原DCP路径，结束再核原DCP和manifest SHA。
新out限定本E3 artifacts，不覆盖旧out，也不重新综合、布局或phys_opt。

`resume_full_route.tcl`保留原生成TCL:129–133的set_param值、XPM/IP cache权限，
只把可写cache和parent-project快照路径改到新out；原manifest.environment复用于Vivado子进程。
从DCP直接打开完整已放置设计，另核CPU20ns与SPIx4，执行原:256命令 `route_design -directive RuntimeOptimized`。
后续保留route/timing/DRC/功耗/资源报告、默认write_bitstream DRC与原SPIx4 MCS命令，
复用逐字相同的verify_routed_full.tcl。没有新seed、directive、read_xdc或约束更改。
原验收门仍是WNS≥0、setup/hold/pulse失败端点0、routing errors0及原DS0/50MHz routed boundary。
原full源CVA1d/Linuxda6 reference-only标签保持，不宣称硬件archive包含ABI5软件。

新manifest保留现collector要求字段，并明确parent_interrupted_build与continuation阶段/检查点/attempt1。
即便产生BIT，status也只有所有原验收通过才passed；原stale manifest不被伪造为未中断完成。
原Linux reference与当前正式65软件交付独立绑定，旧P0/E5 BIT不受影响。

launch使用start_new_session的detached worker，不依赖长exec session存活。
worker最长21600秒，超时终止自身Vivado进程组（先TERM、最多30秒后KILL），独立记录failed，绝不自动再试。
任何真实route/timing/DRC失败均停止，不改设置或启动第二次续编。

以下只在Root直接审查实际runner/TCL并放行后运行一次：

```bash
python3 /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/resume_full_fpga.py \
  --prepare --out /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/full-e3-ds0-resume01
python3 /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/resume_full_fpga.py \
  --launch --out /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/full-e3-ds0-resume01
```

## 有限host检查

`verify_full_resume.py`的actual Tcl stub执行检查唯一原route命令、仅新checkpoint输出、无综合/布局/physopt/read_xdc、
原set_param值及原boundary helper调用；实际timing parser分别检查WNS/三类端点/routing门和缺失计数拒绝。
回执为synthetic/0板测，不证明真实DRC或routing。
01的jobs Tcl list空格规范化预期错误原样保存；改断言后02通过，后续03补绑定原子进程环境后同样9项通过。
生产TCL未因该测试空格问题修改。
截至本文件小批准备时，未prepare或launch实际续编。

## 已接受的唯一续编启动

Root直接审查接受5f7a306后，2026-10-17T15:46:07.579994Z启动唯一resume01，
detached worker PID2083297，worker_start=15:46:07.690845Z。
实际out为本E3 `artifacts/full-e3-ds0-resume01/`，dispatch.json / build-manifest.json / build.log为持续状态入口。
`test-results/full-resume01-dispatch.json`与`full-resume01-manifest.as-launched.json`保存启动时原件。
执行源commit固定5f7a30611d6d84e513a781cc42b8951570a37c3d，后续归档commit不冒充该执行源。
启动时确认checkpoint hardlink与原physopt同inode；原manifest不改。当前尚无最终timing/routing/BIT资格。

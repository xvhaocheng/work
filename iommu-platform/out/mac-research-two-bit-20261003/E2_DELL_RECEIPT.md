# E2 64项提前回传

2026-10-03：E2 64/64已执行并保存，交付runner的逐项验证及整段保存核对通过。完整结果已回传Dell：
`/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/e2/`。

rsync内容核对无差异。每项runs/<ID>/包含record.json、archive.tar.gz、metadata.json、退出码、taint、dmesg、argv及哈希；stage-done.json cases64。

e2-console-prefix.raw.log是原始UART的完整前缀快照，含E2全部过程及方法启动后的部分输出，未修改原始字节。DELIVERY.json记录使用的软件/BIT及原服务器哈希。

首次第1项CLI exit0后因BusyBox tar不支持-z取回停止。原停止证据仍在本地20261003-research-two-bit目录；续进只将归档改为tar -cf后gzip -c，取回原第1项、检查通过后跑其余63项，第1项未重跑。所有实验参数、数据校验及taint判据保留。boot/kernel/caps通过，Linux00093。

方法阶段462项尚在运行，未宣称完成。任务继续，暂不关闭Linux。完整方法结果结束后另行同步本目录。

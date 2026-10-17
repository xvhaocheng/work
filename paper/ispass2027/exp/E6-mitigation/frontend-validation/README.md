# G frontend首批

实际CVA6 `2c7c68f59190b9ce8739efd7b67d92262b0021f0`，直接父提交为已审E5
`6c5c666d45979d3a03fb98cff89ba7ad095f6125`，独立组件工作树`/home/xvhaocheng/work-a1-e6-cva6`。
只改`idma_reg64_frontend.sv`一个文件，52add/2del，完整diff为`cva6-g-frontend.patch`。
没有修改既有E5/P0树、observer、wrapper、Linux、约束或顶层实例；新参数默认关闭，没有生产实例启用G。

新增BatchGapEnable参数、e0 gap32、e8只读IDMAGAP1 cap、GAP状态与三个只读tap。
原BTC2 magic、合法batch长度/页数/地址、旧状态编码和已有MMIO保留。
disable/clear复位gap0；active/bad-width/partial-strobe写拒绝，不改实际gap。
新GAP只在非末任务done后插入，STOP在GAP到期同拍优先；功能batch ID退休不会伪造一个task done。

`runs/g-frontend02/manifest.json`绑定原运行命令、完整dirty差异、输入hash、实际候选/冻结旧源和finite budget。
候选复制字节SHA256 `13c33cf32a1961f9f71b7433a9b85629c533eddacb1978dea31e65bf25902ba0`
与最终组件commit文件完全相同。
原生产模块三实例：精确6c5旧P0、候选默认关闭、候选启用。前两者在全部刺激每个前/后沿逐字段比较，
启用gap0也在旧接口合法刺激上逐拍比较，覆盖rsp、完整burst、VALID、type/data、batch_active、
issue/done/poll及ID、observer配置转发全部旧输出。

实际结果：39场景、943985检查、207332时钟周期，启用gap0比较395990个前/后沿。
包括64/1024/4096长度、1/8页、1/17/4096任务、非batch64/1500/65536；
gap1/2/1000/2000下1/4任务的精确GAP占用及首/尾不插gap；STOP到期同拍、GAP中STOP、
COUNT已offer+背压不撤回、active COUNT/GAP配置拒绝、宽度/strobe/cap写拒绝，
最大32bit gap实际装载/递减/STOP和33bit target端点。
32bit最大间隔没有执行完整2^32等待；这是明确有界端点测试，不声称全范围穷举。
真实frontend只收到合成backend完成，未验证backend/AXI/数据/IOMMU或完整系统。

2026-10-17T14:27:39.059163Z至14:27:41.245987Z完成编译和执行，预算编译180s、仿真20s。
候选、冻结gold和wrapper没有warning，已有vendor/生成寄存器23条width warning保留在原compile.log，未压掉或改vendor。
`runs/g-frontend01/`原编译失败完整保留：runner漏把registers.svh放在RTL前，导致FF未定义。
修复只改runner文件顺序，生产RTL在01/02完全相同。

可复用命令（独立新输出，不覆盖历史）：

```sh
python3 paper/ispass2027/exp/E6-mitigation/frontend-validation/run_frontend.py \
  --cva6 /home/xvhaocheng/work-a1-e6-cva6 \
  --output paper/ispass2027/exp/E6-mitigation/artifacts/NEW_FINITE_RUN
```

组件最小incremental bundle为`cva6-g-frontend.bundle`，唯一prerequisite为6c5，
已在真实组件仓库`git bundle verify`通过（原回执`cva6-bundle-verify.txt`）。
恢复先verify再fetch到新恢复ref；不向第三方upstream创建remote或推送，不修改既有冻结分支。
完整源、patch、bundle及原件hash入口为`component.json`。

Root审此首批实际diff之后，才做observer小批；尚无Linux/完整BIT构建授权排程或硬件操作。

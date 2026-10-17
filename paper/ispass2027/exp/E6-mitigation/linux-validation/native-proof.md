# RV64新cap探测：实际目标对象证据

最终driver：Linux889f75b0，`object02/idma-disassembly.log`；
`idma.o` SHA256 `2302dbfa008937bb744e687b089a150313d1b5374a06a63893d72712b30b7a50`。
完整编译参数、config、输入hash和退出状态见`object02/manifest.json`，compile.log无warning/error。

在`idma_caps_g`内，两个lane的e8探测实际编译为：

| caller地址 | 实际指令/relocation |
| --- | --- |
| 0x32d0、0x32d6 | `li a2,8`；`addi a1,a1,232`，第三参数固定8B、地址+0xe8 |
| 0x32de | `R_RISCV_CALL copy_from_kernel_nofault`，函数调用后0x32e6是`fence i,r` |
| 0x32f6、0x32f8、0x3308 | 第二lane同样size8/地址+0xe8/nofault调用；0x3312是`fence i,r` |
| 0x32ea、0x3316 | 非零errno跳向-EOPNOTSUPP退出，后续index选择不执行 |

RV64 readq的前置`__io_br()`是空宏，后置`__io_ar()`为`fence i,r`，上述对象保留这一排序。
ELF为`elf64-littleriscv`，原readq little-endian转换在此配置为恒等，不改端序。
stack上的raw为8-byte对齐，源e8亦8-byte对齐。

nofault被调用实现复用`object01`中实际`mm/maccess.o`，SHA256
`ec569d2a37d4e8aed16a2077c0b29c77c8e3328ad539b6d65c806fa78e11e150`。
最终源文件和相关RISC-V uaccess/mmio/traps/extable hash与object01逐一相同，object02 manifest绑定该原件。
不重跑未改的maccess编译。`object01/maccess-disassembly.log`的0x146函数检查对齐后，
0x196是实际`ld a4,0(s1)`；8B请求只走这一轮u64读取，不退到byte循环。

`object01/maccess-sections-relocations.log`记录真实`__ex_table`段108B及其relocations。
首entry的相对指令地址指向0x196（`.L1^B2`），fixup指向0x198（`.L2^B2`）；
`object01/maccess-ex-table.log`中首entry type/data为`03 00 cf 01`，type3=`EX_TYPE_UACCESS_ERR_ZERO`。
data0x1cf表示err寄存器x15/a5、zero寄存器x14/a4，匹配实际ld输出和errno分支。
`arch/riscv/mm/extable.c`处理该type时置err=-EFAULT、zero=0并跳fixup；
`arch/riscv/kernel/traps.c:112`的内核load fault路径先尝试fixup_exception，无匹配才Oops。

源码、实际对象和异常表共同支持该helper处理CPU load fault；host stub只验证errno传播/无index副作用。
**没有测试或证明旧BIT SLVERR经过完整CVA6/互联一定到达此异常路径。**
正式Root运行仍只在已验G BIT上，不因此扩展RTLcap或身份体系。

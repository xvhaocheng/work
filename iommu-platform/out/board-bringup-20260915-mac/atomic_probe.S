/* Diagnostic only. Link at 0x801f0000, within saved/restored firmware gap.
 * Output 8 x 64-bit fields per case at 0x801fe000:
 * width/offset tag, LR value, first SC status, first memory value,
 * second SC (without LR) status, final memory value, iterations, failures.
 */
.section .text
.globl _start
_start:
    la t0, trap
    csrw mtvec, t0
    li s0, 0x801fe000
    li s1, 0x801ff000
    .macro CASE width, offset, tag
    li s2, 1000
    li s3, 0
    addi s4, s1, \offset
1:
    li t0, 1
    s\width t0, 0(s4)
    fence rw,rw
    lr.\width t1, (s4)
    addi t2, t1, 1
    sc.\width\().rl t2, t2, (s4)
    fence rw,rw
    l\width t3, 0(s4)
    li t4, 3
    sc.\width t4, t4, (s4)
    fence rw,rw
    l\width t5, 0(s4)
    li t0, 1
    bne t1, t0, 2f
    bnez t2, 2f
    li t0, 2
    bne t3, t0, 2f
    beqz t4, 2f
    bne t5, t0, 2f
    j 3f
2:
    addi s3, s3, 1
3:
    addi s2, s2, -1
    bnez s2, 1b
    li t0, \tag
    sd t0, 0(s0)
    sd t1, 8(s0)
    sd t2, 16(s0)
    sd t3, 24(s0)
    sd t4, 32(s0)
    sd t5, 40(s0)
    li t0, 1000
    sd t0, 48(s0)
    sd s3, 56(s0)
    addi s0, s0, 64
    .endm
    CASE w, 0, 32
    CASE w, 4, 36
    CASE d, 0, 64
    li a0, 0x600d
    fence rw,rw
    ebreak
    j .
.balign 4
trap:
    csrr a0, mcause
    csrr a1, mepc
    csrr a2, mtval
    ebreak
    j .

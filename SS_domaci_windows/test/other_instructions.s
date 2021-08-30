.section data
    int r4
    iret
    ret
    push r2
    pop r3
    xchg r2, r4
    add r1, r1
    sub r1, pc
    mul r4, r4
    div r1, r1
    cmp pc, psw
    not r1
    and r2, r2
    or r3, r4
    xor r1, r1
    test r4, r5
    shl r6, r1
    shr r6, r1

    halt

.end
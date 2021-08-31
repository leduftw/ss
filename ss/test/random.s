.section SekcijaPrva
.global a, b, c
a:			.global d
    .global e
.extern f
 .extern g, h
   b: .extern g1, h2
.section SekcijaDruga
	.word b
.skip 2
.skip 0x0002
.equ a1, 0xFF00
.equ a2, 0xFF02
.equ a3, 84
halt
c: halt
int r0
int sp
iret
  iret
call *a
call %a
call 10
call 0x00EE
call *r1
call *[r1]
call *[r2 + 3]
call *[r2 + 0x0003]
call *[r2 + b]
push r0
push r1
pop r6
e: push r0
pop r1
d: pop r1
push r1 
pop r0 
add r0, r1
 add r0, r1
xchg r0, r1
xchg r0, r1
not r0
not r4
jmp %a
jmp *a
ldr r1, 10
str r5, $10
str r5, $b
str r5, %c
ldr r7, [r5 + 22]
ldr r5, [r4 + b]

.end

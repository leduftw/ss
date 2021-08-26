.equ a, 0x10

.global a, b

.equ b, 10

.section .text
	add r0, r6
	xor r1, r3
	
.section .bss
	.skip 0x20
	
.section .data
	.word a, 11, b
	
.end

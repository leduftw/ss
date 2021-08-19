.section .text
	push r1
	pop r1
	add r1, r1
	xor r1, r2
	lab1: xor r1, r1

	main:
		call foo
		
	foo:
		push r3
		ret

.section .data
	.word r2, r4, r5
	.skip 5
	
.section .text
	xchg r3, r4
	
.section .bss
	.skip 11
	
.end
	
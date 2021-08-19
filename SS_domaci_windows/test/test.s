.section .text
	ldr r1, $100
	ldr r1, $pca  #comm
	#ldr r1, $pc  # should report error, it does
	ldr r1, 100
	ldr r1, a
	ldr r1, sp  # not symbol, but register, it works as expected
	ldr r1, %ac
	#ldr r1, %r6  # should report error, it does
	ldr r1, r2
	ldr r2, [ r4 ]
	ldr r2, [ r4 + 870 ]
	#ldr r2, [ r4 + $870 ]  # should report error, it does
	ldr r2, [ r4  + a ]
	#ldr r2, [ r4  + r5 ]  # should report error, it does
	
	#########################################
	jmp 400
	jmp a
	#jmp r4  # should report error, it does
	call %a
	#call %r1  # should report error, it does
	jeq *100
	jne *q
	jne *r2
	# all jgt below work
	#jgt *[  r1    ]
	#jgt *[  r1]
	jgt *[r1]
	#jgt *[r1    ]
	jgt *[  r1  + 100   ]
	jgt *[r1  + g   ]
	#jgt *[r1  + psw  ]  # should report error, it does
	call *[  r1  +    4   ]
	push r1
	pop r1
	#add r1, *[r1 + 5]  # should report error, it does
	xor r1, r2
	lab1: xor r1, r1

	main:
		call foo
		
	foo:
		push r3
		ret

.section .data
	.word a, b, 100  # comment
	.skip 5
	
.section .text
	xchg r3, r4
	
.section .bss
	.skip 11
	
.end
	
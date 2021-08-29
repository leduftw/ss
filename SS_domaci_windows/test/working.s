.equ local_equ_symbol, 65
.equ global_equ_symbol, 55

.global global_equ_symbol
.global global_label
.global data

.extern ex_symbol

.section text
    jmp *[r1 + local_equ_symbol]  # no relocation
    local_label: jmp *[r1 + global_equ_symbol]  # no relocation

    global_label: jmp %local_equ_symbol  # has relocation
    jmp %global_equ_symbol  # has relocation

    jmp %local_label  # has relocation
	
	jmp *[r1 + text]  # has relocation
	jmp *[r1 + data]  # has relocation

.section data
	.skip 12
	.word local_equ_symbol, global_equ_symbol, local_label, global_label, ex_symbol, text, data

.end
###########################

.equ a, 0x10

.section .text
	jmp 100
	jmp a
	jmp *0x100
	jmp *a
	jmp %a
	jgt *r1
	jne *[psw]
	jeq *[r4 + 0X100]
	call *[r3 + a]
	
.end
###########################

.section .text
	ldr r1, $100
	ldr r1, $a
	ldr r1, 0x100
	ldr r1, a
	ldr r1, %a
	ldr r1, r2
	ldr r1, [sp]
	ldr r1, [psw]
	ldr r1, [r2]
	ldr r1, [r2 + 100]
	ldr r1, [r2 + a]	
.end
###########################

.equ a, 0x10

.global a, b

.equ b, 10

.section .text
	ldr r1, $100
	ldr r1, $a
	ldr r1, 0x100
	ldr r1, a
	ldr r1, %a
	ldr r1, r2
	ldr r1, [sp]
	ldr r1, [psw]
	ldr r1, [r2]
	ldr r1, [r2 + 100]
	ldr r1, [r2 + a]
	add r0, r6
	xor r1, r3
	push r2
	pop r2
	
.section .bss
	.skip 0x20
	
.section .data
	.word a, 11, b
	
.end
###########################

neposredno => 5 bajtova
memorijsko direktno => 5 bajtova
registarsko direktno => 3 bajta
registarsko indirektno => 3 bajta
registarsko indirektno sa pomerajem => 5 bajtova
ono šesto bi bilo 5 bajtova ako se negde uopste javlja
 
ldr r1, $100 => neposredno
ldr r1, $a => neposredno
ldr r1, 100 => memorijsko direktno
ldr r1, a => memorijsko direktno
ldr r1, %a => registarsko indirektno sa pomerajem c6???
ldr r1, r2 => registarsko direktno
ldr r1, [r2] => registarsko indirektno
ldr r1, [r2 + 100] => registarsko indirektno sa pomerajem
ldr r1, [r2 + a] => registarsko indirektno sa pomerajem

Isto i za str

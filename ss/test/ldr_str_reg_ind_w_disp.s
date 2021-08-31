.equ local_equ_symbol, 65
.equ global_equ_symbol, 55

.global global_equ_symbol
.global global_label
.global data

.extern ex_symbol

.section text
    ldr r1, [r2 + local_equ_symbol]
    local_label: ldr r1, [r3 + global_equ_symbol]
    global_label: ldr r1, [pc + local_label]
    ldr r1, [sp + global_label]

    ldr r1, [psw + ex_symbol]
	
	ldr r1, [r4 + text]
	ldr r1, [r5 + data]

    ldr r1, [r6 + 101]
    ldr r1, [r7 + 0x10f]
    ldr r1, [r7 + 0X10F]

.section data
	.skip 12
    
.end
#################################################
.equ local_equ_symbol, 65
.equ global_equ_symbol, 55

.global global_equ_symbol
.global global_label
.global data

.extern ex_symbol

.section text
    str r1, [r2 + local_equ_symbol]
    local_label: str r1, [r3 + global_equ_symbol]
    global_label: str r1, [pc + local_label]
    str r1, [sp + global_label]

    str r1, [psw + ex_symbol]
	
	str r1, [r4 + text]
	str r1, [r5 + data]

    str r1, [r6 + 101]
    str r1, [r7 + 0x10f]
    str r1, [r7 + 0X10F]

.section data
	.skip 12
    
.end

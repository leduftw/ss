.equ local_equ_symbol, 65
.equ global_equ_symbol, 55

.global global_equ_symbol
.global global_label
.global data

.extern ex_symbol

.section text
    ldr r1, r2
    local_label: ldr r1, r3
    global_label: ldr r1, pc
    ldr r1, sp

    ldr r1, psw
	
	ldr r1, r4
	ldr r1, r5

    ldr r1, r6
    ldr r1, r7

.section data
	.skip 12
    
.end
############################################
.equ local_equ_symbol, 65
.equ global_equ_symbol, 55

.global global_equ_symbol
.global global_label
.global data

.extern ex_symbol

.section text
    str r1, r2
    local_label: str r1, r3
    global_label: str r1, pc
    str r1, sp

    str r1, psw
	
	str r1, r4
	str r1, r5

    str r1, r6
    str r1, r7

.section data
	.skip 12
    
.end

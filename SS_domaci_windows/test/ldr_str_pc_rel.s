.equ local_equ_symbol, 65
.equ global_equ_symbol, 55

.global global_equ_symbol
.global global_label
.global data

.extern ex_symbol

.section text
    ldr r1, %local_equ_symbol
    local_label: ldr r1, %global_equ_symbol
    global_label: ldr r1, %local_label
    ldr r1, %global_label

    ldr r1, %ex_symbol
	
	ldr r1, %text
	ldr r1, %data

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
    str r1, %local_equ_symbol
    local_label: str r1, %global_equ_symbol
    global_label: str r1, %local_label
    str r1, %global_label

    str r1, %ex_symbol
	
	str r1, %text
	str r1, %data

.section data
	.skip 12
    
.end

.equ local_equ_symbol, 65
.equ global_equ_symbol, 55

.global global_equ_symbol
.global global_label
.global data

.extern ex_symbol

.section text
    jmp *local_equ_symbol
    local_label: jmp *global_equ_symbol
    global_label: jmp *local_label
    jmp *global_label

    jmp *ex_symbol
	
	jmp *text
	jmp *data

    jmp *100
    jmp *0x10f
    jmp *0X10F
    
    jne *r2
    jne *[r4]

.section data
	.skip 12
    
.end

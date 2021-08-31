.equ local_equ_symbol, 65
.equ global_equ_symbol, 55

.global global_equ_symbol
.global global_label
.global data

.extern ex_symbol

.section text
    jmp *[r1 + local_equ_symbol]
    local_label: jmp *[r1 + global_equ_symbol]
    global_label: jmp *[r1 + local_label]
    jmp *[r1 + global_label]

    jmp *[r1 + ex_symbol]
	
	jmp *[r1 + text]
	jmp *[r1 + data]

    jmp *[r1 + 100]
    jmp *[r1 + 0x10f]

.section data
	.skip 12
    
.end

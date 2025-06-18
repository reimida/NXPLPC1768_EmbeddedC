

		;EOR R3, R5, R6 scrive in R3 tutti i bit che sono differenti tra R5 e R6

		;str r1,[r0,r3,lsl#2]

		;The instruction above basically says: r0 + (r3 << 2) = r1, or if we manually "expand" the bit-shift: r0 + (r3 * 4) = r1

		;The notation is <instruction> <src> [<dst>, <offset>, <shift>].

		;            Operand1:     0101
		;			 Operand2:     0011
		;----------------------------
		;After ORR -> Operand1:    0111
		
		;             Operand1:     0101
        ;    Operand2:     0011
		;----------------------------
		;After XOR -> Operand1:    0110
		
		;orr=or, eor=xor, and=and
		;NEG R2,R3 ---> R3 (00100100 --> 11011011)

		AREA asm_functions, CODE, READONLY
		EXPORT totale_pressioni_con_filtro
			

totale_pressioni_con_filtro
		MOV r12, sp
		PUSH {r4-r8,r10-r11, LR}
			
	
		;r0 vett
		;r1 num letture
		;r2 VAL MAX FILTER
		;r3 VAL MIN FILTER
		
		MOV r5,#0 ; counter
		MOV r6,#0 ;result
		
loop	LDRB r4, [r0,r5] ;vett[i]
		
		CMP  r4,r2
		BHI	 no_add
		CMP  r4,r3
		BLO  no_add
		ADD  r6,r4
no_add	ADD  r5,#1
		CMP  r1,r5
		BNE  loop  
		
		MOV r0,r6 ;set result
			
		POP {r4-r8,r10-r11, PC}
		END
section .text

bits 64
default rel

global stencil_ASM
stencil_ASM:
	sub RCX, 6	;ignore edges/halos

	block:
		push R8			;input pointer
		push RCX		;counter (n)

	
		xor RAX, RAX	;reset accumulator

		;main sum operation
		mov RCX, 7	;perform sum on 7 inputs
		sum:
	
			mov R9, [R8]		;get input
			add RAX, R9

			add R8, 4			;move to next input
		LOOP sum
		
		mov [RDX], RAX	;store result in output

		;retrieve stored values
		pop RCX
		pop R8

		ADD R8, 4		;shift 1 input forward
		ADD RDX, 4		;move to next output

	LOOP block

	ret
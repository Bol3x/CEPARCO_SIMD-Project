;written by Jan Carlo Roleda
;Section: S11

section .text

bits 64
default rel

global stencil_ASM
stencil_ASM:
	sub RCX, 6	;ignore edges/halos

	block:
		push R8			;input pointer

		xor RAX, RAX	;clear accumulator

		; -- main sum operation --
		;unroll add 7x
		mov R9, [R8]		;get input
		add RAX, R9

		mov R9, [R8+8]		;get input
		add RAX, R9

		mov R9, [R8+16]		;get input
		add RAX, R9

		mov R9, [R8+24]		;get input
		add RAX, R9

		mov R9, [R8+32]		;get input
		add RAX, R9

		mov R9, [R8+40]		;get input
		add RAX, R9

		mov R9, [R8+48]		;get input
		add RAX, R9
		
		mov [RDX], RAX	;store result in output

		;retrieve stored values
		pop R8

		;move to next element
		ADD R8, 8		;shift 1 input forward
		ADD RDX, 8		;move to next output

	LOOP block

	ret
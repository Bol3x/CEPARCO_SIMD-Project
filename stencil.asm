;written by Jan Carlo Roleda
;Section: S11

section .text

bits 64
default rel

global stencil_ASM
stencil_ASM:
	sub RCX, 6	;ignore edges/halos

	block:

		xor RAX, RAX	;clear accumulator

		; -- main sum operation --
		;unroll add 7x
		mov RAX, qword [R8]
		add RAX, qword [R8+8]
		add RAX, qword [R8+16]
		add RAX, qword [R8+24]
		add RAX, qword [R8+32]
		add RAX, qword [R8+40]
		add RAX, qword [R8+48]
		
		mov [RDX], RAX	;store result in output

		;move to next element
		ADD R8, 8		;shift 1 input forward
		ADD RDX, 8		;move to next output

	LOOP block

	ret
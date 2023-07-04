section .text

bits 64
default rel

global stencil_SIMD_ASM
stencil_SIMD_ASM:
	shr RCX, 3	;ymm reg = 256bit/32bit (int) = 8 elem = 2^3

	;init RCX
	xor RBX, RBX

	block:
		;initialization
		vxorps YMM1, YMM1

		push RCX
		mov RCX, 7
		sum:
			;add (packed) on each output elem
			vmovdqu YMM0, [R8]
			vaddps YMM1, YMM1, YMM0

			;move input window forward
			add R8, 4

		LOOP sum

		;copy result to output
		vmovdqu [RDX], YMM1

		;move to next set of outputs
		add R8, 4
		add RDX, 32

	pop RCX
	LOOP block

	ret
	
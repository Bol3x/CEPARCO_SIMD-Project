section .text

bits 64
default rel

global stencil_SIMD_ASM
stencil_SIMD_ASM:
	shr RCX, 2	;ymm reg = 256bit/64bit (long long) = 4 elem = 2^2

	xor RBX, RBX

	block:
		;initialization
		vxorps YMM1, YMM1

		push R8

		; -- main sum operation --
		;unroll add 7x
		vmovdqu YMM0, [R8]
		vaddpd YMM1, YMM1, YMM0

		vmovdqu YMM0, [R8+8]
		vaddpd YMM1, YMM1, YMM0

		vmovdqu YMM0, [R8+16]
		vaddpd YMM1, YMM1, YMM0

		vmovdqu YMM0, [R8+24]
		vaddpd YMM1, YMM1, YMM0

		vmovdqu YMM0, [R8+32]
		vaddpd YMM1, YMM1, YMM0

		vmovdqu YMM0, [R8+40]
		vaddpd YMM1, YMM1, YMM0

		vmovdqu YMM0, [R8+48]
		vaddpd YMM1, YMM1, YMM0

		;copy result to output
		vmovdqu [RDX], YMM1

		;adjust input window
		pop R8
		ADD R8, 32

		;move to next set of outputs
		add RDX, 32	;move to next set of elements

	LOOP block

	ret
	
void stencil(long n, float* h_out, float* h_in) {
    //assume vector is large enough (n >= 7)
    for (int i = 3; i < n - 3; i++) {
        h_out[i - 3] = h_in[i - 3] + h_in[i - 2] + h_in[i - 1] + h_in[i] + h_in[i + 1] + h_in[i + 2] + h_in[i + 3];
    }
}


//RCX, RDX, R8
extern void stencil_SIMD_ASM(long n, float* out, float* in);
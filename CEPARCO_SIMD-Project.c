#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>


void stencil(int n, long long* h_out, long long* h_in) {
    //assume vector is large enough (n >= 7)
    for (int i = 0; i < n - 6; i++) {
        h_out[i] = h_in[i] + h_in[i+1] + h_in[i+2] + h_in[i+3] + h_in[i+4] + h_in[i+5] + h_in[i+6];
    }
}

//RCX, RDX, R8
extern void stencil_SIMD_ASM(int n, long long* out, long long* in);

//RCX, RDX, R8
extern void stencil_ASM(int n, long long* out, long long* in);

int main() {
    //note: 2^30 crashes (overloaded memory)
    const unsigned int VEC_SIZE = 1 << 24;
    const unsigned long long ARRAY_BYTES = VEC_SIZE * sizeof(long long);

    printf("Array size: %d\n\n", VEC_SIZE);

    // allocate memory
    long long* in, * out;
    in = (long long*)malloc(ARRAY_BYTES);
    out = (long long*)malloc(ARRAY_BYTES);

    // timer variables
    clock_t start, end;
    double times[30];

    // initialize data
    for (int i = 0; i < VEC_SIZE; i++)
        in[i] = (i + 1) % 1024;

    // flush-in cache
    stencil(VEC_SIZE, out, in);

    // time here
    double sum = 0;
    for (int i = 0; i < 30; i++) {

        start = clock();
        stencil(VEC_SIZE, out, in);
        end = clock();

        times[i] = ((double)(end - start)) * 1e6 / CLOCKS_PER_SEC; // microseconds
        sum = sum + times[i];
    }

    double ave_time_C = sum / 30;

    printf("C function took an average of %fus.\n", ave_time_C);
    
    printf("%lld\t%lld\t%lld\t%lld\t%lld\n", out[0], out[1], out[2], out[3], out[4]);

    // check for errors
    unsigned long err_count = 0;
    for (int i = 3; i < VEC_SIZE-3; i++) {
        int temp = in[i - 3] + in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] + in[i + 3];
        if (temp != out[i-3])
            ++err_count;
    }
    printf("Error count: %ld \n", err_count);
    


    printf("\n\n");


    /*Standard x86-64*/
    long long* out_asm;
    out_asm = (long long*)malloc(ARRAY_BYTES);

    // flush-in cache
    stencil_ASM(VEC_SIZE, out_asm, in);

    // time here
    sum = 0;
    for (int i = 0; i < 30; i++) {

        start = clock();
        stencil_ASM(VEC_SIZE, out_asm, in);
        end = clock();
        times[i] = ((double)(end - start)) * 1e6 / CLOCKS_PER_SEC; // microseconds
        sum = sum + times[i];
    }
    double ave_time_ASM = sum / 30;

    printf("ASM function took an average of %fus.\n", ave_time_ASM);

    printf("%lld\t%lld\t%lld\t%lld\t%lld\n", out_asm[0], out_asm[1], out_asm[2], out_asm[3], out_asm[4]);

    err_count = 0;
    for (int i = 0; i < VEC_SIZE - 6; i++)
        if (out_asm[i] != out[i]) {
            ++err_count;
            //printf("%d\t%d\t", i, out_asm[i]);
            //printf("c: %d\n", out[i]);
        }
    printf("Error count: %ld \n", err_count);

    //free memory
    free(out_asm);


    printf("\n\n");


    /*SIMD Implementation*/
    long long* out_simd;
    out_simd = (long long*)malloc(ARRAY_BYTES);

    // flush-in cache
    stencil_SIMD_ASM(VEC_SIZE, out_simd, in);

    // time here
    sum = 0;
    for (int i = 0; i < 30; i++) {

        start = clock();
        stencil_SIMD_ASM(VEC_SIZE, out_simd, in);
        end = clock();
        times[i] = ((double)(end - start)) * 1e6 / CLOCKS_PER_SEC; // microseconds
        sum = sum + times[i];
    }
    double ave_time_SIMD = sum / 30;

    printf("SIMD ASM function took an average of %fus.\n", ave_time_SIMD);

    printf("%lld\t%lld\t%lld\t%lld\t%lld\n", out_simd[0], out_simd[1], out_simd[2], out_simd[3], out_simd[4]);

    err_count = 0;
    for (int i = 0; i < VEC_SIZE - 6; i++)
        if (out_simd[i] != out[i]) {
            ++err_count;
            //printf("%d\t%d\t", i, out_simd[i]);
            //printf("c: %d\n", out[i]);
        }
    printf("Error count: %ld \n", err_count);

    // free memory
    free(in);
    free(out);
    free(out_simd);

    return 0;
}
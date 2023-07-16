# CEPARCO_SIMD-Project
Assembly to C program that calculates a 64-bit integer 1D stencil of width 7
Created by Jan Carlo Roleda - S11

Project Specifications:
![Screenshot 2023-07-01 140438](https://github.com/Bol3x/CEPARCO_SIMD-Project/assets/59347516/5fe9b7a8-ad38-4f46-8f79-1c7049f02d95)


### Sample output of program (C, ASM, SIMD-AVX2) Output of the program at 2^20 elements.
![image](https://github.com/Bol3x/CEPARCO_SIMD-Project/assets/59347516/c28d46ef-19aa-4b57-9c15-853addbbeb74)

Given that the specifications require a 7-length stencil window, the programs were written accordingly. 
Although the code could be modified to vary its size, its performance would also be affected to accommodate for the added loop controls. 
Additionally, the borders of the output are ignored as the window does not fully capture the input. 
As such the bounds for calculating do not account for the first and last 3 outputs
which assumes that the window specifications do not change and that these values are irrelevant for this case.

The program runs all 3 versions of the stencil calculation within a single calling function so that the direct C output can be used for error checking the assembly code. 
Using the output from the C function as basis, all 3 variations resulted in no errors. 

---
The following data were taken from my personal computer, with a `Ryzen 5 7600x` CPU and `16GB 5600MT/s DDR5` RAM.

It is worth noting that these results were tested in debug mode, as although release mode highly optimizes the C function, it also causes issues when timing the SIMD program, resulting in supposed incorrect measurements.
The resulting optimized C function when inspected in the disassembly is near identical to the implemented ASM program, with slightly lower measured time, likely due to less overhead in switching from C to an ASM function, as well as less-than-ideal implementation compared to the compiler's precise jumps and memory addressing.

I will be comparing the debug mode as to see how the unoptimized C version compares to the optimized assembly functions due to this timing limitation.

| **    Array Size   ** | C        | ASM      | **    Speedup   ** |
|-----------------------|----------|----------|--------------------|
|     2^20              | 2466.667 | 766.6667 | 3.22x              |
|     2^24              | 41266.67 | 11733.33 | 3.52x              |
|     2^29              | 1263433  | 418266.7 | 3.02x              |

The assembly function results in an increased process speed over the C function as it optimizes the number of instructions needed per element, removing unnecessary instructions such as extra MOVs and memory loads. This results in a consistent increase in performance of about 3x-3.5x.

| **    Array Size   ** | C        | SIMD-ASM | Speedup |
|-----------------------|----------|----------|---------|
|     2^20              | 2466.667 | 366.6667 | 6.73x   |
|     2^24              | 41266.67 | 12066.67 | 3.42x   |
|     2^29              | 1263433  | 404233.3 | 3.13x   |

Using SIMD instructions, it appears that it only improves at lower array sizes. At 2^20 elements, it is able to achieve a 6.73x speedup compared to the unoptimized C function. this is twice as fast in comparison to the optimized assembly function. However, at 2^24 and 2^29 it manages to be just as fast as the optimized assembly function. This could be a result of the SIMD instructions taking longer from the additional loop control overhead, as well as the SIMD instructions taking more clock cycles over the regular x64 instructions.

---
The following data were taken from a Google Colab execution. This can be found in https://colab.research.google.com/drive/1x7EVRyA3RiATd_H9Jc12dpCViPR-9fFi?usp=sharing.

The C program used in the notebook is identical to the one used in the local test.

| **    Array Size   ** | **    C Function   ** | **    CUDA   **   | **    Speedup   ** |
|-----------------------|-----------------------|-------------------|--------------------|
|     2^20              |     6251.27ms         |     1434.61ms     |     4.36x          |
|     2^24              |     134678.7ms        |     22926.87ms    |     5.87x          |
|     2^29              |     3478581.97ms      |     716796.6ms    |     4.85x          |

With only using a simple CUDA program, the speedup vs the C function is around 5x, with the recorded times including the overhead of data transer from device-to-host and vice versa. 
Although faster than the SIMD implementation, it may be preferable to still use the former as the CUDA implementation is hardware-dependent on the system having a dedicated Nvidia GPU, whereas AVX2 instructions are widely available in modern systems. If that is also not availalble, the 64-bit assembly program will still work just as well at larger array sizes.

### Sample output of program at 2^20 elements

C execution

![image](https://github.com/Bol3x/CEPARCO_SIMD-Project/assets/59347516/174d9d58-9846-43e7-bc10-71c1c9e3aeac)


CUDA NVprof execution profile

![image](https://github.com/Bol3x/CEPARCO_SIMD-Project/assets/59347516/e58f2903-a60f-4c1b-87d2-a2449afd39f1)





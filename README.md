# CEPARCO_SIMD-Project
Assembly to C program that calculates a 64-bit integer 1D stencil of width 7

## Sample Image of Results (C, ASM, SIMD-AVX2) Output of the program at 2^20 elements.
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

| **    Array Size   ** | **    C Function   ** | **    ASM Call   ** | **    Speedup   ** |
|-----------------------|-----------------------|---------------------|--------------------|
|     2^20              |     2466.67ms         |     766.67ms        |     3.22x          |
|     2^24              |     44300ms           |     18000ms         |     2.46x          |
|     2^29              |     1303700ms         |     524433.33ms     |     2.49x          |

There is a noiceable improvement in using direct x86-64 assembly to calculate each element’s value. 
Inspecting the disassembly of the C function shows that there’s extra overhead in loading the data from memory and converting register values to x64 registers. 
The custom assembly function optimizes the accumulation of values and removes the unnecessary instructions for calculating the stencil that was generated by the compiler. 
As such, it is 2-3x faster in performance over the standard C function implementation.

| **    Array Size   ** | **    C Function   ** | **    ASM SIMD (AVX2)   ** | **    Speedup   ** |
|-----------------------|-----------------------|----------------------------|--------------------|
|     2^20              |     2466.67ms         |     366.67ms               |     6.73x          |
|     2^24              |     44300ms           |     13500ms                |     3.28x          |
|     2^29              |     1303700ms         |     407933.33ms            |     3.20x          |

Using SIMD methods, the calculation of the stencil is improved, especially for smaller ranges. 
Reducing the number of addition operations using an SIMD instruction from the AVX2 instruction set results in considerably faster performance, even over the optimized x86-64 assembly function. 
Because of this parallelization, 4 outputs can be completed for each stencil calculation, and thus a speedup of 3x to 6x over the C function implementation. 
Note that as the array size increases, the speedup of the SIMD program decreases, with a speedup of only 3.2x at 2^29 elements.

---
The following data were taken from a Google Colab execution. This can be found in https://colab.research.google.com/drive/1x7EVRyA3RiATd_H9Jc12dpCViPR-9fFi?usp=sharing.

The C program used in the notebook is identical to the one used in the local test.

| **    Array Size   ** | **    C Function   ** | **    CUDA   **   | **    Speedup   ** |
|-----------------------|-----------------------|-------------------|--------------------|
|     2^20              |     6251.27ms         |     1434.61ms     |     4.36x          |
|     2^24              |     134678.7ms        |     22926.87ms    |     5.87x          |
|     2^29              |     3478581.97ms      |     716796.6ms    |     4.85x          |

With only using a simple CUDA program, the speedup vs the C function is around 5x, with the times including the overhead of data transer from device-to-host and vice versa. 
Although faster than the SIMD implementation, it may be preferable to still use the former as the CUDA implementation is hardware-dependent on the system having a dedicated Nvidia GPU.

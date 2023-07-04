# CEPARCO_SIMD-Project
Assembly to C program that calculates a 1D stencil of width 7

## Sample Image of Results (C, ASM, SIMD-AVX2) Output of the program at 2^20 elements.
![image](https://github.com/Bol3x/CEPARCO_SIMD-Project/assets/59347516/a54e4f82-c2f5-4200-99b1-d015fc29ba87)

Given that the specifications require a 7-length stencil window, the programs were written accordingly. 
Although the code could be modified to vary its size, its performance would also be affected to accommodate for the added loop controls. 
Additionally, the borders of the output are ignored as the window does not fully capture the input, 
and as such the bounds for calculating do not account for the first and last 3 outputs, 
which assumes that the window specifications do not change and that these values are irrelevant.

The program runs all 3 versions of the stencil calculation within a single main function so that the C function output can be used for error checking. 
Using the output from the C function as basis, all 3 variations resulted in no errors. 


---
The following data were taken from my personal computer, with a `Ryzen 5 7600x` and `16GB 5600MT/s DDR5 RAM`.

| **    Array Size   ** | **    C Function   ** | **    ASM Call   ** | **    Speedup   ** |
|-----------------------|-----------------------|---------------------|--------------------|
|     2^20              |     2333.333ms        |     1766.666ms      |     1.32x          |
|     2^24              |     40233.33ms        |     27633.33ms      |     1.46x          |
|     2^29              |     1169700ms         |     938733.3ms      |     1.25x          |

There is a slight improvement in using direct assembly to calculate each element’s value. 
Inspecting the disassembly of the C function shows that there’s extra overhead in loading the data from memory and converting register values to x64 registers. 
The custom assembly function optimizes the memory reads and removes the unnecessary steps for calculating the stencil that was done by the compiler. 
As such, there is a 25% to 46% increase in performance over the standard C function.

| **    Array Size   ** | **    C Function   ** | **    ASM SIMD (AVX2)   ** | **    Speedup   ** |
|-----------------------|-----------------------|----------------------------|--------------------|
|     2^20              |     2333.333ms        |     1766.666ms             |     11.67x         |
|     2^24              |     40233.33ms        |     27633.33ms             |     5.64x          |
|     2^29              |     1169700ms         |     938733.3ms             |     5.39x          |

Using SIMD methods, the calculation of the stencil is significantly improved. 
This is due to having less overall work by reducing the amount of ADD operations by 8, as each YMM register can hold 256 bits of data, or 8 integers. 
Because of this, 8 outputs can be completed in parallel for each stencil calculation, and thus a speedup of 5x to 11x over the C function implementation. 
Note that as the array size increases, the speedup of the SIMD program decreases, with a speedup of only 5.39x at 2^29 elements.

---
The following data were taken from a Google Colab execution. This can be found in https://colab.research.google.com/drive/1x7EVRyA3RiATd_H9Jc12dpCViPR-9fFi?usp=sharing.

The C program used in the notebook is identical to the one used in the local test.

| **    Array Size   ** | **    C Function   ** | **    CUDA   **   | **    Speedup   ** |
|-----------------------|-----------------------|-------------------|--------------------|
|     2^20              |     6708.8ms          |     746.638ms     |     8.99x          |
|     2^24              |     106941.3ms        |     11546.7ms     |     9.26x          |
|     2^29              |     7132625ms         |     362028.1ms    |     19.70x         |

Despite the improvement of using SIMD instructions to the performance of the function, 
using a simple CUDA kernel with a grid-stride loop that parallelizes the C function to GPU processors results in even higher speedup, 
with up to 19.70x at the highest array size of 2^29, where the SIMD program had its weakest performance in. 

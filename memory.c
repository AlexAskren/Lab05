//============================================================================
// Name        : memory.c
// Author      : Saavedra-Barrera [1992] copied from textbook Computer Architecture
// 		 A Quantitative Approach by Hennessy and Patterson
// Version     :
// Copyright   : Your copyright notice
// Description : measure cache 
// Compile     : gcc -o memory memory.c
// Usage       : ./memory
//============================================================================
#include <stdio.h>
#include <windows.h>

#define CACHE_MIN (1024)
#define CACHE_MAX (1024 * 1024 * 16)
#define SAMPLE 10

int x[CACHE_MAX];
double PCFreq = 0.0;
__int64 CounterStart = 0;

// High-resolution timer functions
void StartCounter() {
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li)) {
        printf("QueryPerformanceFrequency failed!\n");
    }
    PCFreq = (double)(li.QuadPart) / 1000.0; // convert to ms
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return (double)(li.QuadPart - CounterStart) / PCFreq; // ms
}

int main() {
    FILE *fp = fopen("memory_log.txt", "w");
    if (!fp) {
        perror("Unable to open log file");
        return 1;
    }

    // More detailed stride list
    int strides[] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192, 256,
                     384, 512, 768, 1024, 2048, 4096, 8192, 16384, 32768};
    int num_strides = sizeof(strides) / sizeof(strides[0]);

    int i, index, stride, limit, temp = 0;
    int steps, tsteps, csize;
    double sec0, sec;

    // 1D int array benchmark
    for (csize = CACHE_MIN; csize <= CACHE_MAX; csize *= 2) {
        for (int s = 0; s < num_strides; s++) {
            stride = strides[s];
            if (stride > csize / 2)
                continue;

            sec = 0;
            limit = csize - stride + 1;
            steps = 0;
            do {
                StartCounter();
                for (i = SAMPLE * stride; i != 0; i--)
                    for (index = 0; index < limit; index += stride)
                        x[index] = x[index] + 1;
                steps++;
                sec += GetCounter();
            } while (sec < 1000.0); // run for at least 1s

            // Empty loop timing (to subtract overhead)
            tsteps = 0;
            do {
                StartCounter();
                for (i = SAMPLE * stride; i != 0; i--)
                    for (index = 0; index < limit; index += stride)
                        temp += index;
                tsteps++;
                sec -= GetCounter();
            } while (tsteps < steps);

            fprintf(fp, "[int 1D] Size: %7d Stride: %7d Time: %14.1f ns\n",
                    csize * (int)sizeof(int), stride * (int)sizeof(int),
                    (sec * 1e6) / (steps * SAMPLE * stride * ((limit - 1) / stride + 1))); // convert ms → ns
        }
    }

    fclose(fp);
    return 0;
}

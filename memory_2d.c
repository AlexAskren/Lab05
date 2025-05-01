#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define BASE_NROWS 1024
#define BASE_NCOLS (1024 * 16 / BASE_NROWS)  // initial 64K total ints
#define SAMPLE 10

double PCFreq = 0.0;
__int64 CounterStart = 0;

// High-resolution timer functions
void StartCounter() {
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li)) {
        printf("QueryPerformanceFrequency failed!\n");
    }
    PCFreq = (double)(li.QuadPart) / 1000.0;
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return (double)(li.QuadPart - CounterStart) / PCFreq;
}

int main() {
    FILE *fp = fopen("memory_log_2d_multi.txt", "w");
    if (!fp) {
        perror("Unable to open output file");
        return 1;
    }

    int strides[] = {1, 2, 4, 8, 16, 32, 64, 96, 128, 192, 256, 384, 512};
    int num_strides = sizeof(strides) / sizeof(strides[0]);

    int temp = 0;

    // Vary the array scale (increase size each time)
    for (int scale = 1; scale <= 8; scale *= 2) {
        int nrows = BASE_NROWS * scale;
        int ncols = BASE_NCOLS / scale;

        int** x = malloc(nrows * sizeof(int*));
        for (int i = 0; i < nrows; i++)
            x[i] = calloc(ncols, sizeof(int));

        int total_bytes = nrows * ncols * sizeof(int);
        printf("\n--- Testing array size: %d bytes (%d × %d) ---\n", total_bytes, nrows, ncols);
        fprintf(fp, "\n--- 2D int array: %d × %d = %d bytes ---\n", nrows, ncols, total_bytes);

        for (int s = 0; s < num_strides; s++) {
            int stride = strides[s];
            if (stride >= nrows) continue;

            double sec = 0;
            int steps = 0;
            do {
                StartCounter();
                for (int i = SAMPLE * stride; i != 0; i--)
                    for (int col = 0; col < ncols; col++)
                        for (int row = 0; row < nrows; row += stride)
                            x[row][col]++;
                steps++;
                sec += GetCounter();
            } while (sec < 1000.0);

            int tsteps = 0;
            do {
                StartCounter();
                for (int i = SAMPLE * stride; i != 0; i--)
                    for (int col = 0; col < ncols; col++)
                        for (int row = 0; row < nrows; row += stride)
                            temp += col;
                tsteps++;
                sec -= GetCounter();
            } while (tsteps < steps);

            fprintf(fp, "[int 2D Y-stride] Size: %8d Stride: %6d Time: %10.2f ns\n",
                    total_bytes, stride * sizeof(int),
                    (sec * 1e6) / (steps * SAMPLE * (nrows / stride) * ncols));
        }

        // Cleanup
        for (int i = 0; i < nrows; i++)
            free(x[i]);
        free(x);
    }

    fclose(fp);
    printf("\nAll tests complete. Output written to memory_log_2d_multi.txt\n");
    return 0;
}

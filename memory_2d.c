#include <stdio.h>
#include <windows.h>

#define NROWS 1024
#define NCOLS (1024 * 16 / NROWS)  // keep total size same as 1D version
#define SAMPLE 10

int x[NROWS][NCOLS];
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
    FILE *fp = fopen("memory_log_2d.txt", "w");
    if (!fp) {
        perror("Unable to open log file");
        return 1;
    }

    // Strides across Y-dimension (rows)
    int strides[] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192, 256, 384, 512};
    int num_strides = sizeof(strides) / sizeof(strides[0]);

    int i, row, col, stride, temp = 0;
    int steps, tsteps;
    double sec;

    fprintf(fp, "--- 2D int array: Y-dimension stride access ---\n");

    for (int s = 0; s < num_strides; s++) {
        stride = strides[s];
        if (stride >= NROWS) continue;

        printf("Testing stride = %d rows\n", stride);

        sec = 0;
        steps = 0;
        do {
            StartCounter();
            for (i = SAMPLE * stride; i != 0; i--)
                for (col = 0; col < NCOLS; col++)
                    for (row = 0; row < NROWS; row += stride)
                        x[row][col]++;
            steps++;
            sec += GetCounter();
        } while (sec < 1000.0);

        // Empty loop for subtraction
        tsteps = 0;
        do {
            StartCounter();
            for (i = SAMPLE * stride; i != 0; i--)
                for (col = 0; col < NCOLS; col++)
                    for (row = 0; row < NROWS; row += stride)
                        temp += col;
            tsteps++;
            sec -= GetCounter();
        } while (tsteps < steps);

        int total_bytes = NROWS * NCOLS * sizeof(int);
        fprintf(fp, "[int 2D Y-stride] Size: %8d Stride: %6d Time: %10.2f ns\n",
                total_bytes, stride * sizeof(int),
                (sec * 1e6) / (steps * SAMPLE * (NROWS / stride) * NCOLS));
    }

    fclose(fp);
    printf("Benchmark complete. Results written to memory_log_2d.txt\n");
    return 0;
}

#include <stdio.h>
#include <windows.h>

#define CACHE_MIN (1024)
#define CACHE_MAX (1024 * 1024 * 16)
#define SAMPLE 10

double xd[CACHE_MAX];
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
    FILE *fp = fopen("memory_log_double.txt", "w");
    if (!fp) {
        perror("Unable to open log file");
        return 1;
    }

    // Stride list
    int strides[] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192, 256,
                     384, 512, 768, 1024, 2048, 4096, 8192, 16384, 32768};
    int num_strides = sizeof(strides) / sizeof(strides[0]);

    int i, index, stride, limit, temp = 0;
    int steps, tsteps, csize;
    double sec;

    fprintf(fp, "--- Double 1D Array ---\n");

    for (csize = CACHE_MIN; csize <= CACHE_MAX; csize *= 2) {
        printf("Testing array size: %d bytes (%.1f KB)\n", csize * sizeof(double), (double)csize * sizeof(double) / 1024.0);

        for (int s = 0; s < num_strides; s++) {
            stride = strides[s];
            if (stride > csize / 2) continue;

            printf("  Stride: %5d bytes\n", stride * (int)sizeof(double));

            sec = 0;
            limit = csize - stride + 1;
            steps = 0;
            do {
                StartCounter();
                for (i = SAMPLE * stride; i != 0; i--)
                    for (index = 0; index < limit; index += stride)
                        xd[index] = xd[index] + 1.0;
                steps++;
                sec += GetCounter();
            } while (sec < 1000.0);

            tsteps = 0;
            do {
                StartCounter();
                for (i = SAMPLE * stride; i != 0; i--)
                    for (index = 0; index < limit; index += stride)
                        temp += index;
                tsteps++;
                sec -= GetCounter();
            } while (tsteps < steps);

            fprintf(fp, "[double 1D] Size: %7d Stride: %7d Time: %14.1f ns\n",
                    csize * (int)sizeof(double), stride * (int)sizeof(double),
                    (sec * 1e6) / (steps * SAMPLE * stride * ((limit - 1) / stride + 1)));
        }
    }

    printf("\nBenchmark complete. Results written to memory_log_double.txt\n");
    fclose(fp);
    return 0;
}

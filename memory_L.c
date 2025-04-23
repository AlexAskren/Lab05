#include <stdio.h>
#include <windows.h>

#define CACHE_MIN (1024)
#define CACHE_MAX (32768)
#define SAMPLE 10

int x[CACHE_MAX];
double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter() {
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
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
    FILE *fp = fopen("memory_log_2.txt", "w");
    if (!fp) {
        perror("Unable to open file");
        return 1;
    }

    int strides[] = {
        1, 2, 4, 8, 16, 32, 64, 96, 128, 192, 256, 384,
        512, 768, 1024, 2048, 4096, 8192, 16384, 32768
    };
    int num_strides = sizeof(strides) / sizeof(strides[0]);

    int i, index, stride, limit, temp = 0;
    int steps, tsteps, csize;
    double sec0, sec;

    for (csize = CACHE_MIN; csize <= CACHE_MAX; csize *= 2) {
        fprintf(fp, "\n### Size = %d bytes ###\n", csize * (int)sizeof(int));

        for (int s = 0; s < num_strides; s++) {
            stride = strides[s];
            if (stride > csize / 2) continue;

            sec = 0;
            limit = csize - stride + 1;
            steps = 0;
            do {
                StartCounter();
                for (i = SAMPLE * stride; i != 0; i--)
                    for (index = 0; index < limit; index += stride)
                        x[index]++;
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

            fprintf(fp, "Size: %8d Stride: %6d Time: %10.2f ns\n",
                    csize * sizeof(int), stride * sizeof(int),
                    (sec * 1e6) / (steps * SAMPLE * stride * ((limit - 1) / stride + 1)));
        }
    }

    fclose(fp);
    return 0;
}

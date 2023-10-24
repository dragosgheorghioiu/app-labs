#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NO_THREADS 4

// scriere in fisier
void write_file(char *filename, char *type, double time, long sum) {
    FILE *file = fopen(filename, "w+");
    fprintf(file, "Type: %s\nTime: %lf\nSum: %ld", type, time, sum);
    fclose(file);
}

int main() {
    FILE *file = fopen("input.txt", "r");
    int size, i;
    int *numbers;

    long sum1 = 0, sum2 = 0, sum3 = 0;
    double t1, t2;
    double time1, time2, time3;
    omp_set_num_threads(NO_THREADS);

    // de paralelizat toata bucata de mai jos

    // fiti atenti la partea de citire din fisier

    #pragma omp parallel
    {

        #pragma omp master
        {
            fscanf(file, "%d", &size);
            numbers = malloc(size * sizeof(int));

            for (int i = 0; i < size; i++) {
                fscanf(file, "%d", &numbers[i]);
            }
            fclose(file);
            t1 = omp_get_wtime();
        }

        #pragma omp barrier


        

        #pragma omp sections private(i)
        {
            #pragma omp section
            {
                #pragma omp parallel for reduction(+:sum1)
                for (i = 0; i < size; i++) {
                    sum1 += numbers[i];
                }
                time1 = omp_get_wtime() - t1;
            }
    
            #pragma omp section
            {
                for (i = 0; i < size; i++) {
                    #pragma omp atomic
                    sum2 += numbers[i];
                }
                time2 = omp_get_wtime() - t1;
            }
    
            #pragma omp section
            {
                for (i = 0; i < size; i++) {
                    #pragma omp critical
                    sum3 += numbers[i];
                }
                time3 = omp_get_wtime() - t1;
            }           
        }

        #pragma omp sections
        {
            #pragma omp section
            {
                write_file("output1.txt", "reduction", time1, sum1);
            }
    
            #pragma omp section
            {
                write_file("output2.txt", "atomic", time2, sum2);
            }
    
            #pragma omp section
            {
                write_file("output3.txt", "critical", time3, sum3);
            }
        }
        
        

        // de paralelizat in 3 moduri - atomic, critical si reduction, de masurat timpii de executie
    }

    // TODO: de scris timpii de executie in 3 fisiere folosind sections (fiecare scriere intr-un section)

    return 0;
}
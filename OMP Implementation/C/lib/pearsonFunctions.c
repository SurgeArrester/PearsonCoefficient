#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void generateSinArray(double *inputArray,
                      int offset,
                      double sizeOfArray) {

    for (int i = 0; i < sizeOfArray; i++) {
        inputArray[i] = sin(i + offset);
    }
}

void generateSinArrayParallel(double *inputArray,
                              int offset,
                              int sizeOfArray,
                              int thread_count) {

#   pragma omp parallel for num_threads(thread_count)   \
        schedule(runtime)
    for (int i = 0; i < sizeOfArray; i++) {
        inputArray[i] = sin(i + offset);
    }
}

double calcAveArray(double *inputArray,
                    int sizeOfArray) {
    double sum = 0.0;

    for (int i =0; i < sizeOfArray; i++) {
        sum += inputArray[i];
    }
    return sum / sizeOfArray;
}

double calcAveArrayParallel(double *inputArray,
                            int sizeOfArray,
                            int thread_count) {
    double sum = 0.0;

#   pragma omp parallel for num_threads(thread_count)   \
        reduction(+: sum)                               \
        schedule(runtime)
    for (int i =0; i < sizeOfArray; i++) {
        sum += inputArray[i];
    }
    return sum / sizeOfArray;
}

double calcStdDev(double *inputArray,
                  double average,
                  int sizeOfArray) {

    double stdDev, deviationFromMean, variation;
    double sum = 0.0;

    for (int i = 0; i < sizeOfArray; i++) {
        deviationFromMean = inputArray[i] - average;
        sum += deviationFromMean * deviationFromMean;
    }

    variation = sum / sizeOfArray;
    stdDev = sqrt(variation);

    return stdDev;
}

double calcStdDevParallel(double *inputArray,
                  double average,
                  int sizeOfArray,
                  int thread_count) {

    double stdDev, deviationFromMean, variation;
    double sum = 0.0;

#   pragma omp parallel for num_threads(thread_count)   \
        reduction(+: sum)                               \
        private(deviationFromMean)                      \
        schedule(runtime)

    for (int i = 0; i < sizeOfArray; i++) {
        deviationFromMean = inputArray[i] - average;
        sum += deviationFromMean * deviationFromMean;
    }

    variation = sum / sizeOfArray;
    stdDev = sqrt(variation);

    return stdDev;
}

double calcPearson(double *xArray,
                   double *yArray,
                   double xAverage,
                   double yAverage,
                   double xStdDev,
                   double yStdDev,
                   double sizeOfArray) {

    double sum = 0, pearson;

    for (int i = 0; i < sizeOfArray; i++) {
        sum += (xArray[i] - xAverage) * (yArray[i] - yAverage);
    }

    pearson = sum / (sizeOfArray * xStdDev * yStdDev);
    return pearson;
}

double calcPearsonParallel(double *xArray,
                   double *yArray,
                   double xAverage,
                   double yAverage,
                   double xStdDev,
                   double yStdDev,
                   int sizeOfArray,
                   int thread_count) {

    double sum = 0, pearson;

#   pragma omp parallel for num_threads(thread_count)   \
        reduction(+: sum)                               \
        schedule(runtime)
    for (int i = 0; i < sizeOfArray; i++) {
        sum += (xArray[i] - xAverage) * (yArray[i] - yAverage);
    }

    pearson = sum / (sizeOfArray * xStdDev * yStdDev);
    return pearson;
}

void displayResults(char flag[3],
                    int thread_count,
                    int num_elements,
                    double x_average,
                    double y_average,
                    double x_std_dev,
                    double y_std_dev,
                    double pearson_coeff,
                    int num_trials,
                    double time_average) {

    if (strcmp(flag, "-p") == 0) {
        printf("{'elements': %i, ", num_elements);
        printf("'averageX': %1.10f, 'averageY': %1.10f, ", x_average, y_average);
        printf("'StdDevX': %1.10f, 'StdDevY': %1.10f, ", x_std_dev, y_std_dev);
        printf("'pearson': %1.10f, ", pearson_coeff);
        printf("'time': %1.10lf, ", time_average);
        printf("'threads': %i, ", thread_count);
        printf("'loopSchedule': '%s'}\n", getenv("OMP_SCHEDULE"));

    } else if (strcmp(flag, "-v") == 0) {
        printf("Number of elements in array: %i\n", num_elements);
        printf("Average of array X: %1.10f, Average of array Y: %1.10f\n", x_average, y_average);
        printf("Std Dev of X: %1.10f, Std Dev of Y: %1.10f\n", x_std_dev, y_std_dev);
        printf("Pearson correlation coefficient: %1.10f\n", pearson_coeff);
        printf("Average time taken across %i runs: %1.10lfs\n", num_trials, time_average);
        if (thread_count != 1){
            printf("Number of threads used: %i\n", thread_count);
            printf("Loop scheduling used: %s\n", getenv("OMP_SCHEDULE"));
        }
    } else if (strcmp(flag, "-c") == 0) {
        printf("%i,", num_elements);
        printf(" %1.10f, %1.10f, ", x_average, y_average);
        printf(" %1.10f, %1.10f,", x_std_dev, y_std_dev);
        printf(" %1.10f,", pearson_coeff);
        printf(" %i, %1.10lf, ", num_trials, time_average);
        printf(" %i, ", thread_count);
        printf(" '%s'\n", getenv("OMP_SCHEDULE"));

    } else {
        printf("Number of elements in array: %i\n", num_elements);
        printf("Average of array X: %1.10f, Average of array Y: %1.10f\n", x_average, y_average);
        printf("Std Dev of X: %1.10f, Std Dev of Y: %1.10f\n", x_std_dev, y_std_dev);
        printf("Pearson correlation coefficient: %1.10f\n", pearson_coeff);
        printf("Average time taken across %i runs: %1.10lfs\n", num_trials,time_average);
        if (thread_count != 1){
            printf("Number of threads used: %i\n", thread_count);
            printf("Loop scheduling used: %s\n", getenv("OMP_SCHEDULE"));
        }
    }
}

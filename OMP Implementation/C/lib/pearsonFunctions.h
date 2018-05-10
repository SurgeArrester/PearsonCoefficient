#ifndef PEARSON_HEADER
#define PEARSON_HEADER

#include <math.h>
#include <stdio.h>

void generateSinArray(double *inputArray,
                      int offset,
                      double sizeOfArray);

void generateSinArrayParallel(double *inputArray,
                              int offset,
                              int sizeOfArray,
                              int thread_count);

double calcAveArray(double *inputArray,
                    int sizeOfArray);

double calcAveArrayParallel(double *inputArray,
                            int sizeOfArray,
                            int thread_count);

double calcStdDev(double *inputArray,
                  double average,
                  int sizeOfArray);

double calcStdDevParallel(double *inputArray,
                double average,
                int sizeOfArray,
                int thread_count);

double calcPearson(double *xArray,
                   double *yArray,
                   double xAverage,
                   double yAverage,
                   double xStdDev,
                   double yStdDev,
                   double sizeOfArray);

double calcPearsonParallel(double *xArray,
                  double *yArray,
                  double xAverage,
                  double yAverage,
                  double xStdDev,
                  double yStdDev,
                  int sizeOfArray,
                  int thread_count);


void displayResults(char flag[3],
                    int thread_count,
                    int num_elements,
                    double x_average,
                    double y_average,
                    double x_std_dev,
                    double y_std_dev,
                    double pearson_coeff,
                    int num_trials,
                    double time_average);
#endif

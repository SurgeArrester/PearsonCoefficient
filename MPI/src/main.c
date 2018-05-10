/*
    Main file to process the data from two arrays in parallel
*/

// Standard Library Functions
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// User defined functions
#include "../../lib/pearsonFunctions.h"

int main(int argc, char *argv[]) {
    int comm_sz, myRank, num_elements;

    double timeStart, timeEnd,
           *myXInput, *myYInput,
           mySumX = 0, mySumY = 0,
           globalSumX = 0, globalSumY = 0,
           xAverage = 0, yAverage = 0,
           myXSumMeanDiffSquared = 0, myYSumMeanDiffSquared = 0,
           globalXSumMeanDiffSquare = 0, globalYSumMeanDiffSquare = 0,
           mySumMeanDiffXY = 0,
           globalSumMeanDiffXY = 0,
           pearsonCoeff = 0;

    if (argc > 1) {
        sscanf(argv[1],"%i",&num_elements);
    } else {
        num_elements = 2000000;
    }

    MPI_Init(NULL, NULL);

    timeStart = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    int remainderIndex = 0;
    int arrayLength = num_elements / comm_sz;
    int remainingElements = num_elements % comm_sz;

    // If we have a remaining number of elements to process, assign extra memory and give index rank to remainder
    if (myRank < remainingElements) {
        remainderIndex = arrayLength + myRank;

        myXInput = malloc(sizeof(double) * arrayLength + 1);
        myYInput = malloc(sizeof(double) * arrayLength + 1);
    } else {
        myXInput = malloc(sizeof(double) * arrayLength);
        myYInput = malloc(sizeof(double) * arrayLength);
    }

    generateSinArray(myXInput, arrayLength * myRank, remainderIndex, arrayLength);
    generateSinArray(myYInput, arrayLength * myRank + 5, remainderIndex, arrayLength);

    for(int i = 0; i < arrayLength; i++) {
        mySumX += myXInput[i];
        mySumY += myYInput[i];
    }

    // Global sum/average of the arrays (for large enough inputs we expect to be 0)
    MPI_Allreduce (&mySumX, &globalSumX, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce (&mySumY, &globalSumY, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    xAverage = globalSumX / num_elements;
    yAverage = globalSumY / num_elements;

    // globalXSumMeanDiffSquare for final standard deviation calculation
    myXSumMeanDiffSquared = calcSumMeanDiffSquared(myXInput, xAverage, arrayLength);
    myYSumMeanDiffSquared = calcSumMeanDiffSquared(myYInput, yAverage, arrayLength);

    MPI_Allreduce (&myXSumMeanDiffSquared, &globalXSumMeanDiffSquare, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce (&myYSumMeanDiffSquared, &globalYSumMeanDiffSquare, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    // globalSumMeanDiffXY to be used for final pearson calculation
    for (int i = 0; i < arrayLength; i++) {
        mySumMeanDiffXY += (myXInput[i] - xAverage) * (myYInput[i] - yAverage);
    }

    MPI_Allreduce (&mySumMeanDiffXY, &globalSumMeanDiffXY, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    if (myRank == 0) {
        double globalStdDevX, globalStdDevY;

        globalStdDevX = combineStdDev(globalXSumMeanDiffSquare, num_elements);
        globalStdDevY = combineStdDev(globalYSumMeanDiffSquare, num_elements);

        pearsonCoeff = calcParallelPearson (globalSumMeanDiffXY, globalStdDevX, globalStdDevY, num_elements);

        timeEnd = MPI_Wtime();
        printf("Num of elements is %i\n", num_elements);
        printf("Average of array X: %f and Y: %f\n", xAverage, yAverage);
        printf("Std Deviation of array X: %f and Y: %f\n", globalStdDevX, globalStdDevY);
        printf("Pearson coefficient is: %f\n", pearsonCoeff);
        printf("Time elapsed is %lf\n", timeEnd - timeStart);
    }

    free(myXInput);
    free(myYInput);
    
    MPI_Finalize();
    return 0;
}

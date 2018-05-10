/*
    Main file to process the data from two arrays serially
    readArray
    calculateMean
    calculateStdDev
    calculatepearson_coeff
*/

// Standard Library Functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

// User defined functions
#include "../../lib/pearsonFunctions.h"

int main(int argc, char *argv[]) {
    // Ensure the correct number of arguments are given when running the program TODO add flag for writing to file
    if (argc < 4) {
        printf("Missing an argument, arguments: [NUM OF ELEMENTS] [NUM OF TRIALS] [OUTPUT FORMAT (-p for pythonic, -v for verbose)]\n");
    }

    // Initialise all the variables used
    int num_elements, num_trials;
    char output_flag[3];
    double time_start, time_end,
           *time_trials, time_average,
           *x_input, *y_input,
           x_average, y_average,
           x_std_dev, y_std_dev,
           pearson_coeff;

    // Read in the command line arguments and save to the correct variables, if none are given set some default values TODO add error checking to ensure correct format
    if (argc == 4) {
       sscanf(argv[1],"%i",&num_elements);
       sscanf(argv[2], "%i", &num_trials);
       strcpy(output_flag, argv[3]);
    } else {
       num_elements = 5000000;
       num_trials = 5;
       strcpy(output_flag, "-v");
    }

    // Assign memory for each of the arrays used
    time_trials = malloc(sizeof(double) * num_trials);
    x_input = malloc(sizeof(double) * num_elements);
    y_input = malloc(sizeof(double) * num_elements);

    // Start a loop running through each of the time trials
    for (int i = 0; i < num_trials; i++) {
        time_start = omp_get_wtime();

        generateSinArray(x_input, 0, num_elements); // Generate our sin waves
        generateSinArray(y_input, 5, num_elements);

        x_average = calcAveArray(x_input, num_elements); // Return average of each array
        y_average = calcAveArray(y_input, num_elements);

        x_std_dev = calcStdDev(x_input, x_average, num_elements); // Return std dev of each array
        y_std_dev = calcStdDev(y_input, y_average, num_elements);

        pearson_coeff = calcPearson(x_input, y_input,           // Return the pearson coeff between the arrays
                                    x_average, y_average,
                                    x_std_dev, y_std_dev,
                                    num_elements);

        time_end = omp_get_wtime();                      // Add time to time array
        time_trials[i] = time_end - time_start;
    }

    time_average = calcAveArray(time_trials, num_trials);  // Calculate average of time taken

    // Display results to console
    displayResults(output_flag, 1, num_elements, x_average, y_average, x_std_dev, y_std_dev, pearson_coeff, num_trials, time_average);

    free(time_trials); // Free the memory
    free(x_input);
    free(y_input);
    return 0;
}

/*
    Main file to process the data from two arrays in parallel
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
    if (argc < 5) {
        printf("Missing an argument, arguments: [NUM OF ELEMENTS] [NUM OF TRIALS] [NUM OF THREADS] [OUTPUT FORMAT (-p for pythonic, -v for verbose)]\n");
    }

    // Some error checking to ensure the environment variable is set for the openMP loops
    if (getenv("OMP_SCHEDULE")) {
        // OMP_SCHEDULE is set, assumed to be a valid value
    } else {
        printf("OMP_SCHEDULE is not set as an environment variable, this program will have undefined behaviour\n");
    }

    // Initialise all the variables used
    int num_elements, thread_count, num_trials;

    char output_flag[3];

    double time_start, time_end,
           *time_trials, time_average,
           *x_input, *y_input,
           x_average = 0, y_average = 0,
           x_std_dev, y_std_dev,
           pearson_coeff = 0;

    // Read in the command line arguments and save to the correct variables, if none are given set some default values TODO add error checking to ensure correct format
    if (argc == 5) {
        sscanf(argv[1],"%i",&num_elements);
        sscanf(argv[2], "%i", &num_trials);
        sscanf(argv[3], "%i", &thread_count);
        strcpy(output_flag, argv[4]);
    } else {
        num_elements = 5000000;
        num_trials = 5;
        thread_count = 4;
        strcpy(output_flag, "-v");
    }

    // Assign memory for each of the arrays used
    time_trials = malloc(sizeof(double) * num_trials);
    x_input = malloc(sizeof(double) * num_elements);
    y_input = malloc(sizeof(double) * num_elements);

    // Start a loop running through each of the time trials
    for (int i = 0; i < num_trials; i++) {
        time_start = omp_get_wtime();   // Get start time

        generateSinArrayParallel(x_input, 0, num_elements, thread_count); // Generate our sin waves
        generateSinArrayParallel(y_input, 5, num_elements, thread_count);

        x_average = calcAveArrayParallel(x_input, num_elements, thread_count); // Return average of each array
        y_average = calcAveArrayParallel(y_input, num_elements, thread_count);

        x_std_dev = calcStdDevParallel(x_input, x_average, num_elements, thread_count); // Return std dev of each array
        y_std_dev = calcStdDevParallel(y_input, y_average, num_elements, thread_count);

        pearson_coeff = calcPearsonParallel(x_input, y_input,               // Return the pearson coeff between the arrays
                                            x_average, y_average,
                                            x_std_dev, y_std_dev,
                                            num_elements, thread_count);

        time_end = omp_get_wtime();                                         // Get the final time

        time_trials[i] = time_end - time_start;                             // Add time to time array
    }

    time_average = calcAveArrayParallel(time_trials, num_trials, thread_count); // Calculate average of time taken

    // Display results to console
    displayResults(output_flag, thread_count, num_elements, x_average, y_average, x_std_dev, y_std_dev, pearson_coeff, num_trials, time_average);

    free(time_trials); // Free the memory
    free(x_input);
    free(y_input);

    return 0;
}

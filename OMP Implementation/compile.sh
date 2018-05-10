#!/bin/bash

# for arraySize in {0, 100, 1000, 100000, 1000000, 5000000}
# do
#     ~/Dropbox/University/Liverpool/ParallelProgramming/Assignment2/C/serial_v1/bin/serial_pearson $arraySize 5 -p
# done

    # for schedulingType in "guided" "dynamic" "guided" "auto"
    # do
    #     export OMP_SCHEDULE=$schedulingType
    #     for arraySize in {0, 100, 1000, 100000, 1000000, 5000000}
    #     do
    #         for numThreads in {1..7..1}
    #         do
    #             ~/Dropbox/University/Liverpool/ParallelProgramming/Assignment2/C/parallel_v1/bin/parallel_pearson $arraySize 5 $numThreads -c
    #         done
    #
    #         for numThreads in {0..128..8}
    #         do
    #             ~/Dropbox/University/Liverpool/ParallelProgramming/Assignment2/C/parallel_v1/bin/parallel_pearson $arraySize 5 $numThreads -c
    #         done
    #     done
    # done

for schedulingType in "guided, 1" "guided, 2" "guided, 4" "guided, 8" "guided, 16" "guided, 32" "guided, 64" "guided, 128" "guided, 256" "guided, 512" "guided, 1024" "guided, 2048" "guided, 4096" "guided, 8192" "guided, 16384" "guided, 32768" "guided, 65536" "guided, 131072" "guided, 262144" "guided, 524288"  "guided, 1048576" "guided, 2097152" "guided, 4194304" "guided, 5000000"
do
    export OMP_SCHEDULE=$schedulingType
    for numThreads in {1..7..1}
    do
        ../C/parallel_v1/bin/parallel_pearson 5000000 5 $numThreads -c
    done

    for numThreads in {0..128..8}
    do
        ../C/parallel_v1/bin/parallel_pearson 5000000 5 $numThreads -c
    done

done

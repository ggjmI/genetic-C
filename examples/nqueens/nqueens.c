#include "../../GA_int/GA_int.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#define NQUEENS 20

float objective_function(int *arr, int length);

float objective_function(int *arr, int length)
{
    int i, j;
    int fo = 0;

    /* Calculating attacks for positive and negative diagonals. */
    for (i = 0; i < length; i++)
    {
        for (j = i + 1; j < length; j++)
        {
            if (arr[j] == arr[i] + j - i ||
                arr[j] == arr[i] - j + i)
                fo++;
        }
    }
    return (float) fo;
}

int main()
{
    /* solution data. */
    int length = NQUEENS;
    int min_value = 0;
    int max_value = NQUEENS - 1;
    int non_repeatable = NO_REPEAT;
    /* population data. */
    int n_population = 100;
    struct IntPopulation *pop;
    char init_mode[] = "random";
    /* tournament data. */
    int tournament_size = 3;
    /* crossover data. */
    char cross_mode[] = "2kpoints";
    float n_parents_percentage = 0.8;
    int n_parents = (int) (n_parents_percentage * (float) n_population);
    if (n_parents % 2)
        /* Only even n_parents accepted for 2kpoints mode. */
        n_parents--;
    int n_childs = n_parents;
    /* mutation data. */
    char mutate_mode[] = "swap";
    float mutate_rate = 0.01;

    /* iteration data. */
    int print_mode = PRINT_INDV;
    int max_iter = 5000;
    int k = 0;
    float best_fo_alltime;
    float END_FO = 0.0;
    float epsilon = 0.000001;
    /* results data. */
    FILE *results;
    results = fopen("RESULTS/RESULTS_nqueens50.txt", "w");
    /* time structs to get total algorithm time. */
    struct timeval stop, start;

    srand((unsigned int) time(NULL));
    gettimeofday(&start, NULL);

    /* ===1st population creation, evalution and print.=== */
    pop = int_init_population(init_mode, n_population, length,
                              min_value, max_value, non_repeatable);
    int_evaluate_population(pop, objective_function);
    best_fo_alltime = pop->best_fo_alltime;
    print_results(results, print_mode, pop, k);

    /* ===Main loop.=== */
    /* Updating pop until best fo found or max_iter. */
    while(k < max_iter && (pop->best_fo_alltime - END_FO) > epsilon)
    {
        int_ga_one_iter(pop,
                objective_function, tournament_size,
                cross_mode, n_parents, n_childs,
                mutate_mode, mutate_rate);
        k++;
        /* Updating best_fo_alltime and printing. */
        if (pop->best_fo_alltime < best_fo_alltime)
        {
            best_fo_alltime = pop->best_fo_alltime;
            print_results(results, print_mode, pop, k);
        }
    }
    gettimeofday(&stop, NULL);

    /* Printing final results and time taken. */
    print_end_results(results, pop, k);
    fprintf(results, "Time taken for the algorithm to complete:\n"
            "%f seconds", stop.tv_sec - start.tv_sec
            + (stop.tv_usec - start.tv_usec) / 1000000.0);

    /* ===Freeing population and closing results.=== */
    int_free_population(pop);
    fclose(results);

    return 0;
}

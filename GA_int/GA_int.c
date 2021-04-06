/* This is the source file for 'GA_int.h'.

   Written by Gabriel Gil <2021> */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "GA_int.h"

/*==========================*/
/* externals from GA_int.h and globals. */
extern int **ext_parents, **ext_childs, **ext_new_individuals, ext_ptrs_state;
int **competitors, *winner, glb_compet_win_state;

/*==========================*/
/* Local source functions prototypes. */
int
compare (const void *a, const void *b);

void
int_init_competitors_winner(struct IntPopulation *pop);

void
int_free_competitors_winner(struct IntPopulation *pop);

/*==========================*/
/* Miscellanous functions. */
int
compare (const void *a, const void *b)
{
    /* To use in qsort. */
    return ( *(int*)a - *(int*)b );
}

/*==========================*/
/* Print functions. */
void
print_results(FILE *ptr, int print_mode,
                struct IntPopulation *pop, int k)
{
/* Simple function to print results of the current population
   'pop' to an external file.
   =ARGUMENTS=
   - '*ptr' : A FILE pointer.
   - 'print_mode': An integer that says how the results should be printed:
                   - '1' : Only the best individual and the best fo
                           of the current pop should be printed.
                   - '2' : The best individual and all the population
                           is printed.
   - '*pop' : An already evaluated IntPopulation struct.
   - 'k' : The current iteration. */
    int i, j;

    /* Basic function to print results to ptr file. */
    fprintf(ptr, "\n===POPULATION GEN %d===\n", k);
    if (print_mode == PRINT_COMPLETE)
    {
        for (i = 0; i < pop->n_population; i++)
        {
            fprintf(ptr, "[ ");
            for (j = 0; j < pop->length; j++)
            {
                fprintf(ptr, "%d ", pop->individuals[i][j]);
            }
            fprintf(ptr, "]\n");
        }
    }
    if (print_mode == PRINT_INDV || print_mode == PRINT_COMPLETE)
    {
        fprintf(ptr, "Best individual(s) found in the population:\n");
        for (i = 0; i < pop->n_best_individuals; i++)
        {
            fprintf(ptr, "[ ");
            for (j = 0; j < pop->length; j++)
            {
                fprintf(ptr, "%d ", pop->individuals[
                                        pop->best_indexes[i]][j]);
            }
            fprintf(ptr, "]\n");
        }
        fprintf(ptr, "The best fo for this population is: %.8f\n",
                pop->best_fo);
    }
}

void
print_end_results(FILE *ptr, struct IntPopulation *pop, int k)
{
/* Simple function to print end results of the GA to an external file.
   =ARGUMENTS=
   - '*ptr' : A FILE pointer.
   - '*pop' : An already evaluated IntPopulation struct.
   - 'k' : The last iteration. */
    int i, j;

    fprintf(ptr,
        "\n\nAfter %d iterations this is the best fo found!!"
        " Good for you!\n", k);
    fprintf(ptr,
        "Best individual(s) found through all iters:\n");
    for (i = 0; i < pop->n_best_indv_alltime; i++)
    {
        fprintf(ptr, "[ ");
        for (j = 0; j < pop->length; j++)
        {
            fprintf(ptr, "%d ", pop->best_indv_alltime[i][j]);
        }
        fprintf(ptr, "]\n");
    }
    fprintf(ptr,
            "The best fo value found through all iters: %.8f\n",
            pop->best_fo_alltime);
}

/*==========================*/
/* Functions for allocation of useful pointers. */
void
int_init_ext_ptrs(struct IntPopulation *pop)
{
/* Function to alloc the external pointers:
   - 'ext_parents'
   - 'ext_childs'
   - 'ext_individuals'
   These pointers are used in 'ga_one_iter' function, but can
   also be used by the user in his own GA steps'
   =ARGUMENTS=
   - '*pop' : A pointer to an IntPopulation struct. */
    int i;
    /* By default ext_ptrs_state will be 0 since it's a global. */
    if (ext_ptrs_state == PTR_NOT_ALLOCD)
    {
        ext_parents = ec_calloc(pop->n_population_orig, sizeof(int*),
            __LINE__, __FILE__);
        ext_childs = ec_calloc(pop->n_population_orig, sizeof(int*),
            __LINE__, __FILE__);
        ext_new_individuals = ec_calloc(pop->n_population_orig, sizeof(int*),
            __LINE__, __FILE__);
        for (i = 0; i < pop->n_population_orig; i++)
        {
            ext_new_individuals[i] = ec_calloc(pop->length, sizeof(int),
                __LINE__, __FILE__);
            ext_parents[i] = ec_calloc(pop->length, sizeof(int),
                __LINE__, __FILE__);
            ext_childs[i] = ec_calloc(pop->length, sizeof(int),
                __LINE__, __FILE__);
        }
        ext_ptrs_state = PTR_ALLOCD;
    }
}

void
int_init_competitors_winner(struct IntPopulation *pop)
{
/* Function to alloc the global pointers:
   - 'competitors'
   - 'winner'
   These pointers are used in 'int_tournament_selection' function
   to keep number of allocations constant.
   =ARGUMENTS=
   - '*pop' : A pointer to an IntPopulation struct. */
    int i;

    if (glb_compet_win_state == PTR_NOT_ALLOCD)
    {
        competitors = ec_calloc(pop->n_population_orig, sizeof(int*),
                          __LINE__, __FILE__);
        winner = ec_calloc(pop->length, sizeof(int),
                     __LINE__, __FILE__);
        for (i = 0; i < pop->n_population_orig; i++)
        {
            competitors[i] = ec_calloc(pop->length, sizeof(int),
                                 __LINE__, __FILE__);
        }
        glb_compet_win_state = PTR_ALLOCD;
    }
}

/*==========================*/
/* Functions to free the useful pointers and population. */
void
int_free_ext_ptrs(struct IntPopulation *pop)
{
/* Function to free alloc'd external pointers:
   - 'ext_parents'
   - 'ext_childs'
   - 'ext_individuals'
   These pointers are used in 'ga_one_iter' function, but can
   also be used by the user in his own GA steps'
   =ARGUMENTS=
   - '*pop' : A pointer to an IntPopulation struct. */
    int i;
    if (ext_ptrs_state == PTR_ALLOCD)
    {
        for (i = 0; i < pop->n_population_orig; i++)
        {
            free(ext_new_individuals[i]);
            free(ext_childs[i]);
            free(ext_parents[i]);
        }
        free(ext_new_individuals);
        free(ext_childs);
        free(ext_parents);
        ext_ptrs_state = PTR_FREE; /* No more alloc or free. */
    }
}

void
int_free_competitors_winner(struct IntPopulation *pop)
{
/* Function to free the global pointers:
   - 'competitors'
   - 'winner'
   These pointers are used in 'int_tournament_selection' function
   to keep number of allocations constant.
   =ARGUMENTS=
   - '*pop' : A pointer to an IntPopulation struct. */
    int i;

    if (glb_compet_win_state == PTR_ALLOCD)
    {
        free(winner);
        for (i = 0; i < pop->n_population_orig; i++)
        {
            free(competitors[i]);
        }
        free(competitors);
        glb_compet_win_state = PTR_FREE; /* No more alloc or free. */
    }
}

void
int_free_population(struct IntPopulation *pop)
{
/* This function frees an IntPopulation struct and all it's members.
   It MUST be called in the end of your program to free heap memory.
   =ARGUMENTS=
   - '*pop' : A IntPopulation struct already initialized AND evaluated. */
    int i;

    int_free_competitors_winner(pop);
    int_free_ext_ptrs(pop);

    if (pop->non_repeatable == NO_REPEAT)
        free(pop->reference_arr);

    if (strcmp(pop->init_mode, "unalloc") != 0)
    {
        /* No allocation for these members in this init_mode. */
        for (i = 0; i < pop->n_population_orig; i++)
        {
            free(pop->individuals[i]);
        }
        free(pop->individuals);
    }
    if (pop->first_population == POP_EVALUATED)
    {
        /* Only if the population was evaluated. */
        for (i = 0; i < pop->n_population_orig; i++)
        {
            free(pop->best_indv_alltime[i]);
        }
        free(pop->best_indv_alltime);
        free(pop->best_indexes);
        free(pop->fos);
        free(pop->sorted_fos_indexes);
        free(pop->sorted_fos);
    }
    free(pop);
}

/*==========================*/
/* GA operators and related functions definition. */
void
int_init_solution(struct IntPopulation *pop, int *arr)
{
/* This functions fullfills an array with random values
   within the pop boundaries.
   =ARGUMENTS=
   - '*pop' : A IntPopulation struct pointer with variables
              length, min_value, max_value, non-repeatable,
              range and reference array already defined.
   - '*arr' : The array to be fullfilled with size pop->length. */

    int j, tmp_value;
    int shuff_reference_arr[pop->range];

    if (pop->non_repeatable == NO_REPEAT)
    {
        /* We create a random array from selecting values
           from a shuffled 'reference_arr' array. */
        memcpy(shuff_reference_arr, pop->reference_arr,
               sizeof(shuff_reference_arr[0]) * pop->range);
        shuffle_arr(shuff_reference_arr, pop->range,
                    sizeof(shuff_reference_arr[0]));
        memcpy(arr, shuff_reference_arr,
               sizeof(shuff_reference_arr[0]) * pop->length);
    }
    else
    {
        /* If repeated values are accepted, the creation of
           the solution array is very simple. */
        for (j = 0; j < pop->length; j++)
        {
            tmp_value = rand() % pop->range;
            tmp_value += pop->min_value;
            arr[j] = tmp_value;
        }
    }
}

struct IntPopulation
*int_init_population(char *init_mode, int n_population, int length,
                     int min_value, int max_value, int non_repeatable)
{
/* This function returns an initalized IntPopulation struct. It's the
   first call in your program, defining the initial population. After this
   you MUST evaluate the population with 'int_evaluate_population' function.
   =ARGUMENTS=
                    - 'random'  : Init a random pop according to
                                  solution boundaries.
                    - 'empty'   : An empty pop (arrays of 0s) - In case
                                  a non-random start method (as heuristic one)
                                  could be used.
                    - 'unalloc' : The pop will begin with 'pop->individuals'
                                  unallocated. Useful for generation > 0.
   - 'n_population' : The number of individuals inside population.
   - 'length' : Length of the solution presentation (len of individual array).
   - 'min_value' : The minimum value contained in the solution presentation.
   - 'max_value' : The maximum value contained in the solution presentation.
   =RETURNS=
   - 'pop' : A pointer to an IntPopulation struct. This struct has the
             the following variables initialized:
             - 'length'
             - 'min_value'
             - 'max_value'
             - 'non_repeatable'
             - 'n_population'
             - 'individuals'
             - 'init_mode' */

    /* Sanity check. */
    check_null(init_mode, __LINE__, __FILE__);
    if (non_repeatable == NO_REPEAT)
    {
        if ((max_value + 1 - min_value) < length)
        {
            fprintf(stderr, "Impossible to build non-repeatable Solution\n"
                   "for this population.\n"
                   "====> (max_value-min_value) < length\n");
            exit(EXIT_FAILURE);
        }
    }

    int i, j = 0;
    struct IntPopulation *pop;

    pop = ec_malloc(sizeof(struct IntPopulation), __LINE__, __FILE__);
    pop->n_population = n_population;
    pop->n_population_orig = n_population;
    pop->init_mode = init_mode;
    pop->length = length;
    pop->min_value = min_value;
    pop->max_value = max_value;
    pop->range = max_value + 1 - min_value;
    pop->first_population = POP_NOT_EVAL;
    pop->non_repeatable = non_repeatable;

    if (non_repeatable == NO_REPEAT)
    {
        pop->reference_arr = ec_malloc(sizeof(int) * pop->range,
                                 __LINE__, __FILE__);
        for (i = min_value; i < max_value + 1; i++)
        {
            pop->reference_arr[j++] = i;
        }
    }

    if (strcmp(init_mode, "random") == 0)
    {
        pop->individuals = ec_calloc(n_population, sizeof(int*),
                               __LINE__, __FILE__);
        for (i = 0; i < pop->n_population; i++)
        {
            pop->individuals[i] = ec_calloc(pop->length, sizeof(int),
                                       __LINE__, __FILE__);
            int_init_solution(pop, pop->individuals[i]);
        }
    }
    else if (strcmp(init_mode, "empty") == 0)
    {
        pop->individuals = ec_calloc(n_population, sizeof(int*),
                               __LINE__, __FILE__);
        /* Just a bunch of 0s. */
        for (i = 0; i < n_population; i++)
        {
             pop->individuals[i] = ec_calloc(length, sizeof(int),
                                       __LINE__, __FILE__);
        }
    }
    else if (strcmp(init_mode, "unalloc") == 0)
    {
        /* pop->individuals not allocated because it will
           be assigned to an already alloc'd pointer of pointers
           with [n_population][length] size. */
    }
    else
    {
        fprintf(stderr, "===ARGUMENT ERROR===\n"
               "Wrong 'init_mode' ('%s') argument passed\nto "
               "'int_init_population' function.\nThe supported"
               " arguments are (so far):\n"
               "-'random'  :    randomly initialize population according"
               "\n\t\tto solution boundaries.\n"
               "-'empty'   :    individuals inside population start as"
               "\n\t\tempty (0s) arrays.\n"
               "-'unalloc' :    pop->individuals not alloc'd.\n"
               "====================\n", init_mode);
        free(pop);
        exit(EXIT_FAILURE);
    }
    return pop;
}

void
int_evaluate_population(struct IntPopulation *pop,
                        float (*objective_function)())
{
/* Evaluate a int population defining the struct variables:
   - 'fos'
   - 'best_fo'
   - 'n_best_individuals'
   - 'best_indexes'
   - 'sorted_fos'
   - 'sorted_fos_indexes'
   - 'best_fo_alltime'
   - 'n_best_indv_alltime'
   - 'best_indv_alltime'
   It should be called after the generation of a population.
   =ARGUMENTS=
   - '*pop' : An IntPopulation struct already initialized.
   - '(*objective_function)()' : A fo calculation with inputs:
                                 - 'int *arr'   : int solution array.
                                 - 'int length' : length of int solution arr.
*/
    int i, j, k;
    int aux = 0;
    float best_fo = 0.0;

    if (pop->first_population == POP_NOT_EVAL)
    {
        /* After evaluation 'first_population' goes to POP_EVALUATED
           to avoid multiple callocs. */
        pop->best_indexes = ec_malloc(pop->n_population_orig * sizeof(int),
                                __LINE__, __FILE__);
        pop->fos = ec_malloc(pop->n_population * sizeof(float),
                       __LINE__, __FILE__);
        pop->sorted_fos_indexes = ec_malloc(
                                      pop->n_population_orig * sizeof(int),
                                      __LINE__, __FILE__);
        pop->sorted_fos = ec_malloc(pop->n_population_orig * sizeof(float),
                              __LINE__, __FILE__);
        pop->best_indv_alltime = ec_calloc(
                                     pop->n_population_orig, sizeof(int*),
                                     __LINE__, __FILE__);
        for (i = 0; i < pop->n_population_orig; i++)
        {
            pop->best_indv_alltime[i] = ec_calloc(pop->length, sizeof(int),
                                            __LINE__, __FILE__);
        }
        pop->n_best_indv_alltime = 0;
        pop->best_fo_alltime = 0.0;
    }
    pop->n_best_individuals = 0;

    /* Getting fo for each individual and the best_fo. */
    for (i = 0; i < pop->n_population; i++)
    {
        pop->fos[i] = objective_function(pop->individuals[i],
                                         pop->length);
        /* Assigning sorted_fos before sorting with qsort. */
        pop->sorted_fos[i] = pop->fos[i];
        /* Getting best_fo. */
        if (pop->fos[i] < best_fo || i == 0)
            best_fo = pop->fos[i];
    }
    pop->best_fo = best_fo;
    /* Looking for individuals with fo == best_fo. */
    for (i = 0; i < pop->n_population; i++)
    {
        if (pop->fos[i] == pop->best_fo)
            pop->best_indexes[pop->n_best_individuals++] = i;
    }
    /* Sorting fos. */
    qsort(pop->sorted_fos, pop->n_population, sizeof(float), compare);
    /* Getting index for sorted fos.
       aux == 0 if index is already inside 'sorted_fos_indexes'. */
    for (i = 0; i < pop->n_population; i++)
    {
        for (j = 0; j < pop->n_population; j++)
        {
            for (k = 0; k < i; k++)
            {
                if (pop->sorted_fos_indexes[k] == j)
                {
                    aux = 0;
                    break;
                }
                else
                    aux = 1;
            }
            if ((aux || i == 0) && (pop->fos[j] == pop->sorted_fos[i]))
            {
                pop->sorted_fos_indexes[i] = j;
                break;
            }
        }
    }
    /* Updating all time best individuals and fo. */
    if (pop->best_fo < pop->best_fo_alltime ||
        pop->first_population == POP_NOT_EVAL)
    {
        pop->best_fo_alltime = pop->best_fo;
        pop->n_best_indv_alltime = pop->n_best_individuals;
        for (i = 0; i < pop->n_best_individuals; i++)
        {
            memcpy(pop->best_indv_alltime[i],
                   pop->individuals[pop->best_indexes[i]],
                   sizeof(int) * pop->length);
        }
    }
    pop->first_population = POP_EVALUATED;
}

int
*int_tournament_selection(struct IntPopulation *pop, int tournament_size)
{
/* This function return the winner (int solution array) pointer
   after the ocorrence of a tournament inside the population.
   =ARGUMENTS=
   - '*pop' : A IntPopulation struct already initialized AND evaluated.
   - 'tournament_size' : The number of simultaneous competitors.
   =RETURNS=
   - 'winner' : A pointer to a int solution array with the minor fo
                from the tournament. */

    int i, j, repeated_index;
    int random_indexes[tournament_size];
    float winner_fo, fos[tournament_size];

    /* Initializing competitors and winner. */
    int_init_competitors_winner(pop);

    for (i = 0; i < tournament_size; i++)
    {
        /* Collecting competitors data. */
        random_indexes[i] = rand() % pop->n_population;
        repeated_index = 1;
        while (repeated_index && i > 0)
        {
            /* This loop checks for non-repeated index,
               i.e., a competitor can't compete with itself. */
            for (j = 0; j < i; j++)
            {
                if (random_indexes[j] == random_indexes[i])
                {
                    repeated_index = 1;
                    random_indexes[i] = rand() % pop->n_population;
                    break;
                }
                else
                    repeated_index = 0;
            }
        }
        memcpy(competitors[i],
               pop->individuals[random_indexes[i]],
               sizeof(int) * pop->length);
        fos[i] = pop->fos[random_indexes[i]];

        /* Best competitor evaluation. */
        if (i == 0 || fos[i] <= winner_fo)
        {
            memcpy(winner, competitors[i],
                   sizeof(int) * pop->length);
            winner_fo = fos[i];
        }
    }
    /* We don't care too much with tied competitors,
       as if a tie happens, the winner index is already
       randomly selected from the population. */
    return winner;
}

void
int_replace_repeated(struct IntPopulation *pop, int **childs,
                     int n_childs)
/* This function replaces repeated values from each pointer
   in **childs - useful after crossover in non-repeatable solutions. */
{
    int aux, i, j, k, l;
    int n_repeated_indexes;
    int repeated_indexes[pop->length];
    int n_missing_val;
    int missing_val[pop->range];

    for (i = 0; i < n_childs; i++)
    {
        /* Here we check where are the repeated
           values (if any). */
        n_repeated_indexes = 0;
        for (j = 0; j < pop->length; j++)
        {
            for (k = j; k < pop->length; k++)
            {
                aux = 0;
                for (l = 0; l < n_repeated_indexes; l++)
                {
                    if (repeated_indexes[l] == k)
                    {
                        aux = 1;
                        break;
                    }
                }
                if (childs[i][j] == childs[i][k] &&
                    j != k && aux == 0)
                {
                    repeated_indexes[
                        n_repeated_indexes++] = k;
                }
            }
        }
        /* If crossover did not repeat values. */
        if (n_repeated_indexes == 0)
            continue;
        /* Identifying values that
           are not included in this solution. */
        n_missing_val = 0;
        for (j = 0; j < pop->range; j++)
        {
            for (k = 0; k < pop->length; k++)
            {
                if (pop->reference_arr[j] ==
                    childs[i][k])
                    break;
                else if (k == pop->length-1)
                    missing_val[
                        n_missing_val++] = pop->reference_arr[j];
            }
        }
        /* Correct the kids removing repeated values. */
        shuffle_arr(missing_val, n_missing_val,
                    sizeof(missing_val[0]));
        for (j = 0; j <  n_repeated_indexes; j++)
        {
            childs[i][repeated_indexes[j]] = missing_val[j];
        }
    }
}

void
int_crossover(char *cross_mode, struct IntPopulation *pop,
              int **parents, int **childs, int n_parents)
{
/* This function defines the pointers from '**childs' using a crossover
   method defined by '*cross_mode' applied to the parents inside
   '**parents'. Shape of '**parents' is [n_parents][pop->length].
   The shape of parents depends on the '**cross_mode'.
   More info on some methods:
   https://en.wikipedia.org/wiki/Genetic_algorithm
   =ARGUMENTS=
   - '*cross_mode' : Method for crossover between '**parents'. Know-methods:
                        - '1kpoint'  : Select random k1 point and exchange
                                       around it within 2 parents.
                                       '**childs' shape:
                                       [n_parents][pop->length].
                        - '2kpoints' : Select two random k1, k2 points and
                                       exchange between them in 2 parents.
                                       '**childs' shape:
                                       [n_parents][pop->length].
                        - 'uniform' :  Choose one value from two parents with
                                       equal probability.
                                       '**childs' shape:
                                       [n_parents][pop->length].
   - '*pop' : IntPopulation struct already initialized and evaluated.
   - '**parents' : Pointer of parents (int arrays) with shape:
                   [n_parents][pop->length]
                   Usually defined using selection method.
   - '**childs'  : An initialized via calloc/malloc pointer of pointers
                   with shape selected from method:
                   [n_childs][pop->length]
                   e.g. for '1kpoint':
                   [n_parents][pop->length]; n_parents = n_childs.
                   But there are a few crossover methods in which
                   n_parents != n_childs, and the correct shape within
                   **childs must be provided.
                   Each child values will be defined via crossover method.
   - 'n_parents' : Number of parents (pointers inside '**parents'). */

    /* Sanity check. */
    check_null(cross_mode, __LINE__, __FILE__);

    int i, j;

    if (strcmp(cross_mode, "1kpoint") == 0)
    {
        if (n_parents % 2 != 0)
        {
            fprintf(stderr, "For %s crossover the number of\n"
                   "parents must be even!.\n", cross_mode);
            int_free_population(pop);
            exit(EXIT_FAILURE);
        }
        else
        {
            /* k1_record is so far kept only for
               non-repeatable solutions. */
            int k1;

            for (i = 0; i < n_parents; i+=2)
            {
                /* k1 must be > 0 and < pop->length */
                k1 = rand() % pop->length;;
                while (k1 == 0)
                {
                    k1 = rand() % pop->length;
                }
                for (j = 0; j < pop->length; j++)
                {
                    /* Performing crossover before k1. */
                    if (j < k1)
                    {
                        childs[i][j] = parents[i+1][j];
                        childs[i+1][j] = parents[i][j];
                    }
                    else
                    {
                        childs[i][j] = parents[i][j];
                        childs[i+1][j] = parents[i+1][j];
                    }
                }
            }
            if (pop->non_repeatable == NO_REPEAT)
            {
                /* Replace the repeated values after crossover. */
                int_replace_repeated(pop, childs, n_parents);
            }
        }
    }
    else if (strcmp(cross_mode, "2kpoints") == 0)
    {
        if (n_parents % 2 != 0)
        {
            fprintf(stderr, "For %s crossover the number of\n"
                   "parents must be even!.\n", cross_mode);
            int_free_population(pop);
            exit(EXIT_FAILURE);
        }
        else
        {
            int k1, k2;

            for (i = 0; i < n_parents; i+=2)
            {
                /* k1 must be >= 0 and < k2
                   k2 must be > k1 and <= pop->length. */
                k1 = rand() % pop->length;
                k2 = 1 + (rand() % pop->length);
                while (k1 >= k2)
                {
                    k1 = rand() % pop->length;
                    k2 = 1 + (rand() % pop->length);
                }

                for (j = 0; j < pop->length; j++)
                {
                    /* Performing crossover around k1 and k2. */
                    if (j >= k1 && j < k2)
                    {
                        childs[i][j] = parents[i+1][j];
                        childs[i+1][j] = parents[i][j];
                    }
                    else
                    {
                        childs[i][j] = parents[i][j];
                        childs[i+1][j] = parents[i+1][j];
                    }
                }
            }
            if (pop->non_repeatable == NO_REPEAT)
            {
                /* Replace the repeated values after crossover. */
                int_replace_repeated(pop, childs, n_parents);
            }
        }
    }
    else if (strcmp(cross_mode, "uniform") == 0)
    {
        if (n_parents % 2 != 0)
        {
            fprintf(stderr, "For %s crossover the number of\n"
                   "parents must be even!.\n", cross_mode);
            int_free_population(pop);
            exit(EXIT_FAILURE);
        }
        else
        {
            for (i = 0; i < n_parents; i+=2)
            {
                for (j = 0; j < pop->length; j++)
                {
                    /* Performing uniform crossover:
                       selecting from parents with equal
                       probability. */
                    if ((float) ((double) rand() / (double) RAND_MAX)
                         > 0.5)
                    {
                        childs[i][j] = parents[i+1][j];
                        childs[i+1][j] = parents[i][j];
                    }
                    else
                    {
                        childs[i][j] = parents[i][j];
                        childs[i+1][j] = parents[i+1][j];
                    }
                }
            }
            if (pop->non_repeatable == NO_REPEAT)
            {
                /* Replace the repeated values after crossover. */
                int_replace_repeated(pop, childs, n_parents);
            }
        }
    }
    else
    {
        fprintf(stderr, "===ARGUMENT ERROR===\n"
               "Wrong 'cross_mode' ('%s') argument passed to\n"
               "'int_crossover' function.\nThe supported"
               " arguments are (so far):\n"
               "-'1kpoint'  :   swap content of parent 1 with parent"
               " 2\n\t\tfrom index 0 to random index k1.\n"
               "-'2kpoints' :   swap content of parent 1 with parent"
               "2\n\t\tfrom random index k1 to random index k2"
               "\n\t\t(k2 > k1).\n"
               "-'uniform'  :   For each gene, there's a 0.5 probability"
               " that\n\t\ta gene will be selected from one parent or"
               "\n\t\tthe other.\n"
               "====================\n", cross_mode);
        int_free_population(pop);
        exit(EXIT_FAILURE);
    }
}

void
int_mutation(char *mutate_mode, struct IntPopulation *pop,
             int **new_individuals, int n_new_individuals,
             float mutate_rate)
{
/* This function performs the mutation operation inside
   a pointer of individuals ('**new_individuals').
   This operator is an absolut must, as generates diversification
   across future generations. Without it, the chances that the GA
   algorithm will convert are very low. A few mutation types are accepted,
   more info on these, check:
   https://en.wikipedia.org/wiki/Mutation_(genetic_algorithm)
   =ARGUMENTS=
   - '*mutate_mode': A string containing the method used for mutation.:
                     - 'swap' : For the mutated individual, one of it's
                                gene will be swapped with another of it's
                                own gene. Useful in non-repeatable solutions.
                     - 'uniform' : The mutated individual will have one of
                                   it's gene randomly replaced with another
                                   possible value for the solution. Can't be
                                   applied to non-repeatable solutions. For
                                   binary-coded solutions (i.e. pop->max_value
                                   =1 and pop->min_value=0) it's the same as
                                   'flip bit' mutation type.
   - '**new_individuals' : A pointer to int pointers of the problem solution.
                           In the most general case, can be pop->individuals
                           or ext_childs if n_parents < n_population.
   - 'n_new_individuals' : Number of pointers inside '**new_individuals'.
   - '*pop' : A pointer to the current already initialized/defined
              IntPopulation structure.
   - '*mutate_rate' : The probability that an gene will be mutate. */

    /* Sanity check. */
    check_null(mutate_mode, __LINE__, __FILE__);

    int i, j, p1;

    if (strcmp(mutate_mode, "swap") == 0)
    {
        /* Does not depend on 'non-repeatable' value.
           Swap item individual[p1] with item individual[p2]. */
        int tmp;

        for (i = 0; i < n_new_individuals; i++)
        {
            for (j = 0; j < pop->length; j++)
            {
                /* If condition true do not mutate. */
                if ((float) ((double)rand() / (double)RAND_MAX) > mutate_rate)
                    continue;
                p1 = rand() % pop->length;
                while (j == p1)
                {
                    p1 = rand() % pop->length;
                }
                /* Swapping. */
                tmp = new_individuals[i][p1];
                new_individuals[i][p1] = new_individuals[i][j];
                new_individuals[i][j] = tmp;
            }
        }
    }
    else if ((strcmp(mutate_mode, "uniform") == 0) &&
            (pop->non_repeatable == REPEATABLE))
    {
        /* Can't be applied to non-repeatable solutions. */
        for (i = 0; i < n_new_individuals; i++)
        {
            /* If condition true do not mutate. */
            for (j = 0; j < pop->length; j++)
            {
                if ((float) ((double)rand() / (double)RAND_MAX) > mutate_rate)
                    continue;
                p1 = rand() % (pop->max_value + 1) + pop->min_value;
                while (p1 == new_individuals[i][j])
                {
                    p1 = rand() % (pop->max_value + 1) + pop->min_value;
                }
                /* Switching by another random value within
                   solution min_value and max_value. */
                new_individuals[i][j] = p1;
            }
        }
    }
    else
    {
        fprintf(stderr, "===ARGUMENT ERROR===\n"
               "Wrong 'mutate_mode' ('%s') argument passed\nto"
               "'int_mutation' function.\nThe supported"
               " arguments are (so far):\n"
               "-'swap'    :    Probability of 'mutate_rate' that one gene"
               "\n\t\twill be swapped with another random gene\n\t\tfrom the"
               " same individual.\n"
               "-'uniform' :    Probability of 'mutate_rate' that one gene"
               "\n\t\twill be replaced with a different random\n\t\t"
               "possible gene from the solution boundaries.\n"
               "====================\n", mutate_mode);
        int_free_population(pop);
        exit(EXIT_FAILURE);
    }
}

void
int_ga_one_iter(struct IntPopulation *pop,
                float (*objective_function)(), int tournament_size,
                char *cross_mode, int n_parents, int n_childs,
                char *mutate_mode, float mutate_rate)
/* The purpose of this function is to apply a complete iteration
   of the GA algorithm. This means:
   -> Apply crossover using '*cross_mode' with 'n_parents' to
      'n_childs'. Parents are defined after 'n_parents' tournaments
      with 'tournament_size' competitors.
   -> Apply mutation to the childs with '*mutate_mode' and
      'mutate_rate'.
   -> If n_childs < n_population, the rest of the individuals of the
      next generation will be selected by elitism.
   -> After having a full population, evaluate it.
   The parents, childs and future individuals used in this function are
   the external pointers 'ext_parents', 'ext_childs', 'ext_new_individuals'.
   =ARGUMENTS=
   - '*pop' : pointer to alread initialized IntPopulation struct.
   - '(*objective_function)()' : pointer to the objective function (fo)'.
   - 'tournament_size' : How many competitors within each tournament.
   - 'cross_mode' : See 'int_crossover' function for details on accepted modes.
   - 'n_parents' : Number of parents to be combined.
   - 'n_childs' : Number of childs to be formed after crossover.
   - 'mutate_mode' : See 'int_mutation' function for details on accepted modes.
   - 'mutate_rate' : probability of one gene to suffer mutation. */
{
    int i, j = 0;

    /* Alloc'ng the parents, childs and new_individuals
       for new populations. */
    int_init_ext_ptrs(pop);

    /* Defining parents by tournaments. */
    for (i = 0; i < n_parents; i++)
    {
        memcpy(ext_parents[i],
               int_tournament_selection(pop, tournament_size),
               sizeof(int) * pop->length);
    }
    /* Defining childs from crossover. */
    int_crossover(cross_mode, pop, ext_parents, ext_childs, n_parents);
    /* Applying mutation to the childs. */
    int_mutation(mutate_mode, pop, ext_childs, n_childs, mutate_rate);

    /* Defining 'new_individuals'. */
    /* If n_childs < n_population, the remaining individuals are
       selected by elitism. */
    for (i = 0; i < pop->n_population; i++)
    {
        if (i < n_childs)
            memcpy(ext_new_individuals[i], ext_childs[i],
                   sizeof(int)*pop->length);
        else
        {
            /* Elitism. */
            memcpy(ext_new_individuals[i],
                   pop->individuals[pop->sorted_fos_indexes[j]],
                   sizeof(int)*pop->length);
            j++;
        }
    }
    /* Upgrading individuals inside pop. */
    for (i = 0; i < pop->n_population; i++)
    {
        memcpy(pop->individuals[i], ext_new_individuals[i],
               sizeof(int)*pop->length);
    }
    /* Evaluating new individuals inside pop. */
    int_evaluate_population(pop, objective_function);
}

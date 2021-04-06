/* This header defines a struct and functions to make easy
   work with integer based genetic algorithms. For this, it declares
   the struct 'IntPopulation' with the main variables related
   to the algorithm.

   There is not much to talk on the algorithm itself. The wiki
   page has great info regarding it:
   https://en.wikipedia.org/wiki/Genetic_algorithm

   Written by Gabriel Gil <2020-2021> */

#ifndef GA_INT_H
#define GA_INT_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../generals/generals.h"

/* For checks on the external and global pointers. */
#define PTR_NOT_ALLOCD 0
#define PTR_ALLOCD 1
#define PTR_FREE 2
/* For checks on 'pop->first_population' */
#define POP_NOT_EVAL 1
#define POP_EVALUATED 0
/* For 'print_mode' of 'print_results' function. */
#define PRINT_COMPLETE 2
#define PRINT_INDV 1
/* For 'non-repeatable' checks. */
#define NO_REPEAT 1
#define REPEATABLE 0

/*==========================*/
/* The main struct for solving GA. */
struct IntPopulation
{
/* IntPopulation compose of 'n_population' individuals
   (solutions), with each individual being a 1-D array
   (**individuals pointer is an array of pointers).

   The individuals are representations of the problem solution
   with boundaries 'length', 'min_value', 'max_value' and 'non_repeatable'.
   The solution boundaries, population size and individuals
   are first defined via 'int_init_population' call
   ('length', 'min_value', 'max_value', 'n_population', 'individuals').
   The variable '*init_mode' contains a string declaring how the
   population was initialized.

   The evaluation related and 'all time best' variables
   are computed via 'int_evaluate_population'
   function. All these contain information regarding the quality
   of the population and it's best individual(s),
   along with the best fo and individual(s) found through all
   iterations with this population. */

    /* Solution boundaries.
       - if 'non-repeatable' != 0, then the solution can't
         admit repeated values (e.g N-queens problem). */
    int length, non_repeatable;
    int min_value, max_value, range;
    /* Reference array for non-repeatable indv. */
    int *reference_arr;
    /* Population data. */
    char *init_mode;
    int n_population;
    int n_population_orig;  /* In cases n_population is reduced. */
    int **individuals;
    /* Evaluation related.
       - 'best_fo' : best of all fo from this pop (minor).
       - 'n_best_individuals' : Number of individuals with 'best_fo'.
       - 'best_indexes' : The index (inside pop) of the best individuals.
       - 'fos' : fo value for each individual.
       - 'sorted_fos' : fo value sorted from best (minor) to worst.
       - 'sorted_fos_indexes' : indexes for the fos in 'sorted_fos'. */
    int n_best_individuals, *best_indexes;
    float *fos, best_fo;
    float *sorted_fos;
    int *sorted_fos_indexes;
    /* All time best indiv. and fo found through all iters in this pop. */
    float best_fo_alltime;
    int n_best_indv_alltime;
    int **best_indv_alltime;
    /* To make sure a few variables are calloc'd only on 1st init call. */
    int first_population;
};

/*==========================*/
/* Externals to make life easy. */
int **ext_parents, **ext_childs, **ext_new_individuals, ext_ptrs_state;
/* These three pointers are used inside 'ga_one_iter' function but can
   also be used by the end user for whatever means they want.
   - 'ext_parents' : a pointer of pointers to be generally used in
                     'int_crossover' function (each pointer assigned after
                     selection.). After alloc, has shape:
                     [pop->n_population][pop->length] but only
                     [pop->n_parents][pop->length] has to be defined.
   - 'ext_childs' : a pointer of pointers to contain the individuals generated
                    after crossover with the parents (used for 'int_crossover)';
                    Has shape:
                    [pop->n_population][pop->length] but only
                    [pop->n_childs][pop->length] has to be defined.
                    n_childs always depend on the crossover method.
   - 'ext_new_individuals' : a pointer of pointers with shape:
                             [pop->n_population][pop->length]
                             It's main purpose is to be the new individuals
                             of the population k + 1.
   - 'ext_ptrs_state' : a variable that's used by alloc and free functions
                        to avoid incorrect alloc and free calls.
   These three pointers can be alloc'd by 'int_init_ext_ptrs' function and
   are free by 'int_free_ext_ptrs' function. */

/*==========================*/
/* Print functions. */
void
print_results(FILE *ptr, int print_mode,
                struct IntPopulation *pop, int k);
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

void
print_end_results(FILE *ptr, struct IntPopulation *pop, int k);
/* Simple function to print end results of the GA to an external file.
   =ARGUMENTS=
   - '*ptr' : A FILE pointer.
   - '*pop' : An already evaluated IntPopulation struct.
   - 'k' : The last iteration. */

/*==========================*/
/* Functions for allocation of useful pointers. */
void
int_init_ext_ptrs(struct IntPopulation *pop);
/* Function to alloc the external pointers:
   - 'ext_parents'
   - 'ext_childs'
   - 'ext_individuals'
   These pointers are used in 'ga_one_iter' function, but can
   also be used by the user in his own GA steps'
   =ARGUMENTS=
   - '*pop' : A pointer to an IntPopulation struct. */

/*==========================*/
/* Functions to free the useful pointers and population. */
void
int_free_ext_ptrs(struct IntPopulation *pop);
/* Function to free alloc'd external pointers:
   - 'ext_parents'
   - 'ext_childs'
   - 'ext_individuals'
   These pointers are used in 'ga_one_iter' function, but can
   also be used by the user in his own GA steps'
   =ARGUMENTS=
   - '*pop' : A pointer to an IntPopulation struct. */

void
int_free_population(struct IntPopulation *pop);
/* This function frees a IntPopulation struct and all it's members.
   It MUST be called in the end of your program to free heap memory.
   =ARGUMENTS=
   - '*pop' : A IntPopulation struct already initialized AND evaluated. */

/*==========================*/
/* GA operators and related functions declarations. */
void
int_init_solution(struct IntPopulation *pop, int *arr);
/* This functions fullfills an array with random values
   within the pop boundaries.
   =ARGUMENTS=
   - '*pop' : A IntPopulation struct pointer with variables
              length, min_value, max_value, non-repeatable,
              range and reference array already defined.
   - '*arr' : The array to be fullfilled with size pop->length. */

struct IntPopulation
*int_init_population(char *init_mode, int n_population, int length,
                     int min_value, int max_value, int non_repeatable);
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
             - 'individuals' */

void
int_evaluate_population(struct IntPopulation *pop,
                        float (*objective_function)());
/* Evaluate a int population defining the struct variables:
   - 'fos'
   - 'best_fo'
   - 'n_best_individuals'
   - 'best_indexes'
   - 'sorted_fos'
   - 'sorted_fos_indexes'
   It should be called after the generation of a population.
   =ARGUMENTS=
   - '*pop' : An IntPopulation struct already initialized.
   - '(*objective_function)()' : A fo calculation with inputs:
                                 - 'int *arr'   : int solution array.
                                 - 'int length' : length of int solution arr.
*/

int
*int_tournament_selection(struct IntPopulation *pop, int tournament_size);
/* This function return the winner (int solution array) pointer
   after the ocorrence of a tournament inside the population.
   =ARGUMENTS=
   - '*pop' : A IntPopulation struct already initialized AND evaluated.
   - 'tournament_size' : The number of simultaneous competitors.
   =RETURNS=
   - 'winner' : A pointer to a int solution array with the minor fo
                from the tournament. */

void
int_replace_repeated(struct IntPopulation *pop, int **childs,
                     int n_childs);
/* This function replaces repeated values from each pointer
   in **childs - useful after crossover in non-repeatable solutions. */

void
int_crossover(char *cross_mode, struct IntPopulation *pop,
              int **parents, int **childs, int n_parents);
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

void
int_mutation(char *mutate_mode, struct IntPopulation *pop,
             int **new_individuals, int n_new_individuals,
             float mutate_rate);
/* This function performs the mutation operation inside
   a pointer of individuals ('**new_individuals').
   This operator is an absolut must, as generates diversification
   across future generations. Without it, the chances that the GA
   algorithm will convert are very low. A few mutation types are accepted,
   more info on these, check:
   https://en.wikipedia.org/wiki/Mutation_(genetic_algorithm)
   =ARGUMENTS=
   - '*mutate_mode': A string containing the method used for mutation.:
                     - 'swap' : One of its gene gets swapped with another of it's
                                own  individual. Useful in non-repeatable solutions.
                     - 'uniform' : For each gene, according to mutate_rate,
                                   it can be randomly replaced with another
                                   possible value for the solution. Can't be
                                   applied to non-repeatable solutions. For
                                   binary-coded solutions (i.e. pop->max_value
                                   =1 and pop->min_value=0) it's the same as
                                   'flip bit' mutation type.
   - '**new_individuals' : A pointer to int pointers of the problem solution.
                           In the most general case, can be pop->individuals.
   - 'n_new_individuals' : Number of pointers inside '**new_individuals'.
   - '*pop' : A pointer to the current already initialized/defined
              IntPopulation structure.
   - '*mutate_rate' : The probability that a gene will mutate. */

void
int_ga_one_iter(struct IntPopulation *pop,
                float (*objective_function)(), int tournament_size,
                char *cross_mode, int n_parents, int n_childs,
                char *mutate_mode, float mutate_rate);
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

#endif /* GA_INT_H */

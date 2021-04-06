# genetic-C

## Introduction
This project aims to create a general Genetic Algorithm implementation for both integer (binary included) and float problems (float yet to be released) written in pure C. The goal is to provide a powerful, efficient and simple implementation, that can be used for many different problems.

If you don't know anything about genetic algorithms, the links bellow should provide a good starting point:
- [Wikipedia](https://en.wikipedia.org/wiki/Genetic_algorithm)
- [Tutorials Point](https://www.tutorialspoint.com/genetic_algorithms/genetic_algorithms_introduction.htm)

The approach in this project is to create a struct with the population and solution data and work with this struct through the GA operators (which are functions declared in the header file and defined in it's respective source).

The structure of this project is:

- **[GA\_int](GA_int/)**   : header and source for the GA int implementation.
- **GA\_float** : header and source for the GA float implementation (not released yet).
- **[generals](generals/)** : header and source for a few useful generic functions.

To compile the code, it's only necessary to compile the GA\_(int|float)/GA\_(int|float).c source, your main code, generals/generals.c and then link the three object files to generate the output.
Bellow is a compilation of [nqueens.c example](examples/nqueens/) with gcc (executing from [examples/nqueens/](examples/nqueens/) folder):

```
gcc -c -Wall ../../GA_int/GA_int.c
gcc -c -Wall nqueens.c
gcc -c -Wall ../../generals/generals.c
gcc -o nqueens.out GA_int.o nqueens.o generals.o && \
```

I usually use an alias (inside .bashrc) to compile all together
from a single command (Linux):
```
alias gcc_int_nqueens='gcc -c -Wall ../../GA_int/GA_int.c && \
gcc -c -Wall nqueens.c && \
gcc -c -Wall ../../generals/generals.c && \
gcc -o nqueens.out GA_int.o nqueens.o generals.o && \
rm GA_int.o nqueens.o generals.o'
```

## Integer implementation (GA\_int)
GA\_int is the GA integer implementation. It consists of:

- **GA\_int.h** : header with the main [struct IntPopulation](###-struct-intpopulation), defines and function declarations.
- **GA\_int.c** : source with function definitions.

### Struct IntPopulation
The struct IntPopulation contains the following data:
- Solution (individual) related data:
  - **length** : The size of the solution array (individual).
  - **repeatable**: This tells us if values can be repeated or not inside the solution.
  - **min\_value** : The minimum value accepted inside the solution.
  - **max\_value** : The maximum value accepted inside the solution.
  - **range** : The difference **(max\_value + 1) - min\_value**.
  - **\*reference\_arr** : An array containing all the possible values for the solution (useful for non-repetable solutions).
- Population related data:
  - **\*init\_mode** : Char containing how the population was initialized.
  - **n\_population** : Number of individuals inside the population.
  - **n\_population\_orig** : To store the starting population (useful when population shrinking is used).
  - **\*\*individuals**: Contains all the individuals from the population (shape _\[n\_population\]\[length\]_).
  - **first\_population** : This is used internally for allocation calls.
- Data related to the population evaluation (objective function, a.k.a fo).
   - **best_fo** : best fo found int the current population (minor).
   - **n\_best\_individuals** : Number of individuals with **best\_fo**.
   - **\*best\_indexes** : Array with the indexes of the best individuals (shape _\[n\_best\_individuals\]_) so they can be found in the population.
   - **\*fos** : fo array with the fo value for each individual (shape _\[n\_population\]_).
   - **\*sorted\_fos** : array with fo values sorted from best (index 0) to worst (shape _\[n\_population\]_) using qsort().
   - **\*sorted\_fos\_indexes** : array with indexes for the fos in 'sorted\_fos' (shape _\[n\_population\]_) so they can be found in the population.
- Best individuals and fo found through all iterations in this population.
   - **best\_fo_alltime** : Best fo found through all iterations in this population.
   - **n\_best\_indv\_alltime** : Number of individuals with the best fo of all time.
   - **\*\*best\_indv\_alltime** : Individuals with the best fo of all time (shape _\[n\_best\_indv\_alltime\]\[length\]_).

### Population init and eval

The struct IntPopulation is partially initialize calling the function:
```
struct IntPopulation
*int_init_population(char *init_mode, int n_population, int length,
                     int min_value, int max_value, int non_repeatable)
```
With this call, all the solution and population data described above are defined (exception for _\*\*individuals_ if _\*init_mode == 'unalloc'_). The next step (if _\*\*individuals_ is defined) is to evaluate the population, defining the remaining struct data:
```
void
int_evaluate_population(struct IntPopulation *pop,
                        float (*objective_function)())
```
In the evaluation process, the _fo_ must be inserted. The code works with a float objective function with arguments (_int \*arr, int length_), with _\*arr_ being the individual itself and _length_ being it's number of elements. I know this apply a few restrictions on the _fo_ being used, but they can be easy to work around (like using a _va\_list_). If there is a better suggestion on how to work with the user objective function, please let me know.

Once evaluated, if we did not found our end solution (the ones which solve the problem), the next steps is to apply the GA operators in a loop until our global/local solutions is found or max number of iterations is reached (or obviously any other conditions the user may like).

### GA Operators and loop
Here we describe the main GA operators defined within the header.

#### Tournament selection:
```
int
*int_tournament_selection(struct IntPopulation *pop,
                          int tournament_size)
```
This performs one tournament inside the population and returns with the winner.

#### Crossover:
```
void
int_crossover(char *cross_mode, struct IntPopulation *pop,
              int **parents, int **childs, int n_parents)
```
The crossover is applied to the pointer of int pointers _parents_ and the result of the crossover (children) is stored in the pointer of int pointers _childs_. _n\_parents_ is the number of elements in _parents_. If all individuals in the current population will have crossover applied, then _parents_ can be members of _pop->individuals_ selected through tournaments. If not, e.g. elitism is used, then _parents_ can be any pointer of pointers you want to temporarily use to later integrate to the next generation of _pop->individuals_. A variable like _n\_childs_ is not used because the number of children generated by crossover is deterministic having the number of parents and the crossover method.

#### Mutation:
```
void
int_mutation(char *mutate_mode, struct IntPopulation *pop,
             int **new_individuals, int n_new_individuals,
             float mutate_rate)
```
This will perform mutation to each gene with a rate of _mutate\_rate_. The individuals that mutation will be applied are the ones within the pointer of int pointers _new_individuals_. If there is no elitism, this can be _pop->individuals_. If there is elitism, it can be the pointer of int pointers _childs_ used in the crossover method above.

#### Linking the operators:

After calling those three functions (operators), which is basically one iteration of GA, one [new evaluation](###-population-init-and-eval) must occur. This is basically repeated in your main loop until the end condition is met.
To work with elitism cases or any other in which _parents_, _childs_ and _new\_individuals_ must be manually created, the external pointers bellow can be used:
```
int **ext_parents, **ext_childs, **ext_new_individuals, ext_ptrs_state;
```
And those are init/free by the functions:
```
void
int_init_ext_ptrs(struct IntPopulation *pop);
```
```
void
int_free_ext_ptrs(struct IntPopulation *pop);
```
#### Ending the algorithm
Once your GA reaches to it's end, it's time to free the population:
```
void
int_free_population(struct IntPopulation *pop);
```
This will free all alloc'd pointers used through the algorithm.

#### Out-of-the-box alternative!
Instead of individually calling the three main operator functions ([tournament](####-tournament-selection), [crossover](####-crossover) and [mutation](####-mutation)), the user can call the function _int\_ga\_one\_iter_ which does it automatically (greatly facilitating things); it will basically perform one iteration of the genetic algorithm (including evaluation). With this, the user do not need to use the external pointers or any other GA operator, everything is handled by this function. The only think the user must do is [start the population](###-population-init-and-free) and free it once the algorithm ends.
```
void
int_ga_one_iter(struct IntPopulation *pop,
                float (*objective_function)(), int tournament_size,
                char *cross_mode, int n_parents, int n_childs,
                char *mutate_mode, float mutate_rate);
```

### End considerations
Description of the accepted arguments in the operators and detail on it's principles are described within the own source code. I hope to slowly implement a separate documentation to contain those descriptions, but I left lots of comments in both GA\_int.h and GA\_int.c which shall certainly help the user to understand how to properly use the functions. The [nqueens](examples/nqueens) examples should be a good starting point to understand how to use the code.

## GA_float
Analogous to GA\_int. Hope to release soon.

## Next steps

- [ ] Review code and test for bugs.
- [ ] Provide more and better documentation.
- [ ] Add more selection, crossover and mutation methods.
- [ ] Provide more complex examples (Restoration problem is a good one).
- [ ] Release GA\_float.

# N-queens example

## The problem
The N-queens problem is a classical one for metaheuristic algorithms.
It is very easy to understand for anyone who knows the basic rules of chess:
You have a N x N size chess table and want to place N Queens inside it
having a total of 0 attacks inside the table, i.e., there is no more than one Queen in every row, column and diagonal of the chess table.

From this description, it's simply to observe a few things:
- You can only have one Queen per row (row goes from 0 to N-1).
- You can only have one Queen per column (column goes from 0 to N-1).
- The attacks (objective function) will be calculated from the Queens diagonals.

The solution array can be build in the following manner:

\[**Q0\_column**, **Q1\_column**, **Q2\_column**, ..., **QN-1\_column**\]

We assume Queen Q0 is in row 0 (i.e. index 0 of array) with column **Q0\_column**.

For example, with N = 5, we could have a solution array like:

\[ 3 2 0 1 4\]

Graphically, it would be:

\[------\]\[------\]\[------\]\[ Q0 \]\[------\]

\[------\]\[------\]\[ Q1 \]\[------\]\[------\]

\[ Q2 \]\[------\]\[------\]\[------\]\[------\]

\[------\][ Q3 \]\[------\]\[------\]\[------\]

\[------\]\[------\]\[------\]\[------\]\[ Q4 \]

Obviously, this is not a individual (solution) which solves the N-queens problem. But it is a strong candidate: If we swap the queen in row 0 with the queen in row 2 and then swap row 2 with row 4, we would have the following solution:

\[ 0 2 4 1 3\]

Graphically:

\[ Q0 \]\[------\]\[------\]\[------\]\[------\]

\[------\]\[------\]\[ Q1 \]\[------\]\[------\]

\[------\]\[------\]\[------\]\[------\]\[ Q2 \]

\[------\]\[ Q3 \]\[------\]\[------\]\[------\]

\[------\]\[------\]\[------\]\[ Q4 \]\[------\]

This is a correct solution to the N-queens problem with N = 5, as there are no attacks in the chess table.

To use genetic algorithm to solve the N-queens problem with a small value of N is very trivial. However, as N increases, the complexity of the problem increases exponentially (actually, with N*(N-1)/2).

In this example we solve the N-queens problem for N = 50 with pure GA without any optimization method (like mutation reduction) and for N = 200 using population reduction.

## N = 50 (nqueens.c)
In this example ([nqueens.c](nqueens.c)) we use the _int\_ga\_one\_iter_ function in a loop to solve the N queens problem with N = 50. The basic parameters used are:

- _n\_population = 100_ and randomly initialize the pop (_init\_mode = "random"_).
- Use tournament selection with _tournament\_size = 3_ to get the parents selected for crossover.
- Use _2kpoints_ as crossover method and fulfill _n\_parents\_percentage_ (80%) of the next generation with children from crossover, the remaining (_n\_population * (1 - n\_parents\_percentage_)) (20%) are selected by elitism (best individuals from current generation without mutation).
- Use swap mutation method with constant _mutate\_rate_ of 0.01 (i.e. 1% of the genes will get mutated) to the children of crossover. In this case, for every two solutions (total of 100 genes), 1 gene will be mutated (swap applied one time).
- Maximum of _max\_iter_ (5000) iteration and every time the best fo found gets better (minor), we write changes to file _results_ using _print\_mode = PRINT\_INDV_ (resolves to integer 1), meaning only the best individuals in the population in which a change in fo occurs will be printed, along with the fo value.

The file ([RESULTS\_nqueens50.txt](RESULTS/RESULTS_nqueens50.txt)) contains the results of the GA for this particular scenario. It's possible to see that after 11 iterations and 0.009954 seconds the solution was found.

## N = 200 (nqueens_popreduction.c)
In this example ([nqueens_popreduction.c](nqueens_popreduction.c)) we use the exact same structure as the previous example (with _int_ga_one_iter_ but N = 200 instead of 50)) with a population reduction applied every time the fo gets betters (minor) until a minimum population is kept to reduce computation time.

In the early iterations we have a large population thus exploring several possible individuals (diversity) and as the population refines, we reduce it's number as most of the individuals in the population eventually becomes very similar to each other due to selection and crossover process. So having a small population in this situation would be actually better as we can have more (and faster) iterations.

The basic parameters used are:
- Initial _n\_population_ = 400 and randomly initialize the pop (_init\_mode_ = "random"), reducing the population by _pop\_decrease\_factor_ (0.88) every time the fo gets better, until a minimum population of _min\_pop_ (10) is reached. The decrease factor was selected based on how many times the fo must get better until _min\_pop_ is reached, which is determined by: _log (min\_pop \% n\_population\_orig )_ with basis _(pop\_decrease\_factor)_. With a 0.88 _pop\_decrease\_factor_, 10  _min\_pop_ and 400 _n\_population\_orig_, it would take approx. 28.85 improvements on the fo to the _pop->n\_population_ reaches the minimum of 10. This is a good value, as empirically after this number of improvements we've already reached a high iteration number, i.e., diversity in early iterations was not affected.
- Use tournament selection with _tournament_size = 3_ to get the parents selected for crossover.
- Use _2kpoints_ as crossover method and fulfill _n\_parents\_percentage_ (80%) of the next generation with children from crossover, the remaining (_n\_population * (1 - n\_parents\_percentage_)) (20%) are selected by elitism (best individuals from current generation without mutation).
- Use swap mutation method with constant _mutate\_rate_ of 0.01 (i.e. 1% of the genes will get mutated) to the children of crossover. In this case, for each solution (total of 200 genes), 2 genes will be mutated (swap applied two times).
- Maximum of _max\_iter_ (100000) iteration and every time the best fo found gets better (minor), we write changes to file _results_ using _print\_mode = PRINT\_INDV_ (resolves to integer 1), meaning only the best individuals in the population in which a change in fo occurs will be printed, along with the fo value.

The file [RESULTS\_nqueens200.txt](RESULTS/RESULTS_nqueens50.txt) contains the results of the GA for this particular scenario. It's possible to see that after 12005 iterations and 20.717 seconds the solution was found (which seems pretty good!).

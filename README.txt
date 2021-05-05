TSP problem part 2

Niels Christoffersen




Chromosome:

I added no members to the chromosome class.
get_fitness was the most difficult, as determining the right fitness was important
to the parents that were chosen.

Deme:

the only design choices I made were for both ##mutate()## and 
##compute_next_generation()##, I made sure they had different values,
so they were always changing. 

Building:

there is a makefile included. Simply type 
\\
make tsp

\\
to compile the program, and it will take inputs and output tsv files.

Results:

I didnt have much success getting a very low value for the final distance,
which probably had something to do with the parent selection being too random
and not choosing the fittest parents enough.
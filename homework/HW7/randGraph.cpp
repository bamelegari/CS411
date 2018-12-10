// randGraph.hpp
// core logic by StackOverflow user ArjunShankar
// https://stackoverflow.com/questions/12790337/generating-a-random-dag
// Modified by Bryce Melegari to work as a utility function and use the graph object
// generates a random directed acyclic graph following the given parameters.
// graph is initially stored as an adjacency list.

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dagraph.hpp"

#define MIN_PER_RANK 1 /* Nodes/Rank: How 'fat' the DAG should be.  */
#define MAX_PER_RANK 5
#define MIN_RANKS 3    /* Ranks: How 'tall' the DAG should be.  */
#define MAX_RANKS 5
#define PERCENT 30     /* Chance of having an Edge.  */








int main (void)
{
  int i, j, k,nodes = 0;
  srand (time (NULL));

  int ranks = MIN_RANKS + (rand () % (MAX_RANKS - MIN_RANKS + 1));

  printf ("digraph {\n");
  for (i = 0; i < ranks; i++)
    {
      /* New nodes of 'higher' rank than all nodes generated till now.  */
      int new_nodes = MIN_PER_RANK + (rand () % (MAX_PER_RANK - MIN_PER_RANK + 1));

      /* Edges from old nodes ('nodes') to new ones ('new_nodes').  */
      for (j = 0; j < nodes; j++)
        for (k = 0; k < new_nodes; k++)
          if ( (rand () % 100) < PERCENT)
            printf ("  %d -> %d;\n", j, k + nodes); /* An Edge.  */

      nodes += new_nodes; /* Accumulate into old node set.  */
    }
  printf ("}\n");
  return 0;
}
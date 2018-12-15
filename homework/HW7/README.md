#maxFlow.py
##Bryce Melegari, UAF CS411 Fall 2018
##Assignment 7

######Implements the Edmonds-Karp algorithm (often considered a specialization of the Ford-Fulkerson Algorithm/Method) for finding the max flow configuration of a flow network using iterative improvement.

##setup:
Clone the repo, navigate to the directory, and run `pip install .`. This will only install all of the dependencies, it will not add the script to bin or anything like that. Once this is done, the script maxFlow.py can be run using Python 2 or 3. 

##notes:
Upon running, the script will create a directory for each hard-coded test graph, into which it will dump images of the initial flow network with its capacities, the maximized network with its flows, as well as each intermediate augmenting path. Note that the drawing framework doesn't handle multiple edges between the same nodes well -- only one of the edges will be drawn. The maximum flow for each graph is printed to console.
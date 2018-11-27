// build.cpp
// complete description in comments of build.hpp.
// Last modified: 
#include "build.hpp"
#include <iostream>

int builder::recursiveBuild(int currentW, int currentE)
{
	// if we have already calculated this sub-problem, just return what we know.
	if(solutionMatrix[currentW][currentE] != -1)
			return solutionMatrix[currentW][currentE];

	int max = 0;

	for(Bridge bridge : bridges)
	{
		// checks if bridge is legal with the previous ones and within our current sub-problem.
		if(bridge[0] < currentW && bridge[1] < currentE)
		{
			// recurse with new bridge now considered, stepping backward.
			int newMax = recursiveBuild(bridge[0], bridge[1]) + bridge[2];

			// set max to newMax if newMax is larger.
			max = (max > newMax ? max:newMax);
		}
	}

	// update solutionMatrix with this solution and return.
	solutionMatrix[currentW][currentE] = max;
	return max;
}


int build(int w, int e, const vector<Bridge> & bridges)
{
	builder b(w, e, bridges);

	// start recursive function with all cities, consider last bridge first.
	return b.recursiveBuild(w, e);
}
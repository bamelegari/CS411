// build.cpp
// Bryce Melegari, UAF CS411 Asssignment 2
// See build.hpp for description.
#include "build.hpp"
#include <iostream>

// checks if two bridges cross or have a common point
bool builder::pairIsBad(const Bridge & a, const Bridge & b)
{
	// check for bridges with common cities
	if (a[0] == b[0] || a[1] == b[1])
		return true;

	// check for overlapping bridges
	if((a[0] < b[0] && a[1] > b[1]) || (a[0] > b[0] && a[1] < b[1]))
		return true;

	return false;
}

bool builder::checkNewBridge(vector<Bridge> & bs, const Bridge & b)
{
	for(Bridge i : bs)
	{
		if(pairIsBad(i, b))
		{
			return false;
		}
	}
	return true;
}


// Recursive backtracking
int builder::build(vector<Bridge> & workingSet, int index)
{	

	if(index < bridges.size())
	{
		// check if current solution is feasible by checking new bridge
		// with all existing bridges.

		bool feasible = checkNewBridge(workingSet, bridges[index]);

		if(feasible)
		{

			workingSet.push_back(bridges[index]);
			int tollWithCurrentBridge = build(workingSet, index + 1);
			workingSet.pop_back();
			int tollWithoutCurrentBridge = build(workingSet, index + 1);
			
			// return whichever is greater
			return (tollWithCurrentBridge > tollWithoutCurrentBridge ? 
						tollWithCurrentBridge:tollWithoutCurrentBridge);
		}
		else
		{
			return build(workingSet, index + 1);
		}
	}

	// base case -- we've reached the bottom of a branch.
	int temp = 0;
	for(Bridge i : workingSet)
	{
		temp += i[2];
	}

	return temp;
}

// w = # of cities on West bank, e = # of cities on East bank
// given these and a vector of bridges with tolls, and return
// the maximum toll possible without overlapping bridges.
// w and e are actually superfluous.
int build(int w, int e, const vector<Bridge> & bridges)
{
	builder b(w, e, bridges);

	vector<Bridge> empty;
	return b.build(empty, 0);
}
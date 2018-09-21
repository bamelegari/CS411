// build.cpp
// Bryce Melegari, UAF CS411 Asssignment 2
// See build.hpp for description.
#include "build.hpp"

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


// checks if given set of bridges has any pairs that cross or share a city,
// both of which violate our constraints.
bool builder::badBridges(vector<Bridge> bridges)
{
	// base case
	if (bridges.size() == 1)
	{
		return false;
	}

	// Check all bridge pairs (a, b) where a = the last bridge in the vector.
	// If those pairs cross or have any common start/end points, stop.
	int last = bridges.size() - 1;
	for(int i = last - 1; i >= 0; --i)
	{
		if(pairIsBad(bridges[last], bridges[i]))
		{
			return true;
		} 
	}

	// if no pairs are bad, do same without the last bridge
	bridges.pop_back();
	return badBridges(bridges);
}

void builder::checkNewToll()
{
	// check if toll is greater than the greatest so far. If so, 
	// make it the greatest so far.
	int tempToll = 0;
	for(auto i : workingSet)
	{
		tempToll += i[2];
	}
	if(tempToll > maxToll)
		maxToll = tempToll;
}

// Recursive backtracking
int builder::build()
{	
	// If we're out of places to go, base case.
	if(workingSet.size() == bridges.size())
	{
		return 0;
	}

	int startPoint = bridges.size() - 1 - workingSet.size();
	for(int i = startPoint; i >= 0; --i)
	{
		workingSet.push_back(bridges[i]);

		if(!badBridges(workingSet))
		{
			checkNewToll();
			build();
		}

		workingSet.pop_back();
	}

	return maxToll;
	
}

// w = # of cities on West bank, e = # of cities on East bank
// given these and a vector of bridges with tolls, and return
// the maximum toll possible without overlapping bridges.
int build(int w, int e, const vector<Bridge> & bridges)
{
	builder b(w, e, bridges);
	return b.build();
}
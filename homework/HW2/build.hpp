// build.hpp
// Bryce Melegari, UAF CS411 Asssignment 2
// Implements an exhaustive search solution to the specifications described
// in HW2.pdf.

#pragma once

#include <vector>

using std::vector;
using Bridge = vector<int>;


// Holds data for the recursive algorithm to work with, as well
// as the algorithm and its helper functions
struct builder
{
	builder(int w, int e, const vector<Bridge> & bridges):
		w(w), e(e), bridges(bridges) {

			//start working set with first bridge
			//workingSet.push_back(bridges[0]);
		}

	// recursive solution finder. called from client code
	int build(int maxToll);

	int w;
	int e;
	const vector<Bridge> & bridges;
	vector<Bridge> workingSet;

private:
	// checks if two bridges cross or have a common point
	bool pairIsBad(const Bridge & a, const Bridge & b);

	// checks if given set of bridges has any pairs that cross or share a city,
	// both of which violate our constraints.
	bool badBridges(vector<Bridge> bridges);

	int newToll(int maxToll);

};


// w = # of cities on West bank, e = # of cities on East bank
// given these and a vector of bridges with tolls, return
// the maximum toll possible within constraints.
int build(int w, int e, const vector<Bridge> & bridges);




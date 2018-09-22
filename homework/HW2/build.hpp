// build.hpp
// Bryce Melegari, UAF CS411 Asssignment 2
// last modified: 9/21/2018
// Implements an exhaustive recursive backtracking solution to the problem described in
// in HW2.pdf.

// note: in the context of this program, a feasible set of bridges is a set of bridges that
// could be built without violating the constraints of the problem, making no statement as to
// whether or not it has the highest toll of all possibilities.
#pragma once

#include <vector>

using std::vector;
using Bridge = vector<int>;


// Holds data for the recursive algorithm to work with to eliminate extra parameters, 
// Also has the algorithm itself and its helper functions.
struct builder
{
	builder(const vector<Bridge> & bridges): bridges(bridges) {}

	// recursive solution finder. called from client code
	// pre: workingSet is an empty vector<vector<int>>,
	// index is 0. These are the ONLY parameter values with which this
	// function should ever be called in client code.
	int build(vector<Bridge> & workingSet, int index);

	// set of all possible bridges given on construction.
	const vector<Bridge> & bridges;

private:
	// returns true if two bridges cross or have a common point. Helper for checkNewBridge.
	bool pairIsBad(const Bridge & a, const Bridge & b);

	// returns true if the addition of a new bridge to a set of bridges results in
	// a feasible set of bridges.
	// pre: The vector of Bridges is assumed to be feasible without the single bridge.
	bool checkNewBridge(const vector<Bridge> & bs, const Bridge & b);

};

// outward-facing build function for test program.
// w and e are superfluous in solving this problem, so they are taken as
// parameters purely to satisfy the problem requirements, and are not used.
int build(int w, int e, const vector<Bridge> & bridges);




/* build.hpp
Bryce Melegari, UAF CS 411 HW 5
Last modified: 
implement a solution to the same problem as HW 2, this time using
dynamic programming.

The problem will be divided into sub-problems consisting of all legal bridges
up to the last one, which we are deciding on. The optimum for each sub-problem will be stored in a matrix
indexed by w and e, the combination of which tell which bridge was being considered.

*/

#pragma once

#include <vector>
using std::vector;

using Bridge = vector<int>;

struct builder
{
	builder(int w, int e, const vector<Bridge> & bridges):
		w(w), e(e), bridges(bridges), solutionMatrix(vector<vector<int>>(w+1, vector<int>(e+1, -1))) {}

	const vector<Bridge> & bridges;

	//memoization matrix, to be indexed by [w][e] coordinates of current bridge.
	vector<vector<int>> solutionMatrix;

	const int w, e;

	int recursiveBuild(int currentW, int currentE);

};

// outward-facing build function for test program.
int build(int w, int e, const vector<Bridge> & bridges);
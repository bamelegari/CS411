// build.hpp
// Bryce Melegari, UAF CS411 Asssignment 2
// Implements an exhaustive search solution to the specifications described
// in HW2.pdf.


//TODO add guards


#include <vector>

using std::vector;
using Bridge = vector<int>;

// w = # of cities on West bank, e = # of cities on East bank
// given these and a vector of bridges with tolls, and return
// the maximum toll possible without overlapping bridges.
int build(int w, int e, const vector<Bridge> & bridges);
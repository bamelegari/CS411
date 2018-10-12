//contigsum.hpp
//Bryce Melegari

// UAF Fall 2018 CS411 Assignment 3, part A.

#pragma once
#include <algorithm>
#include <vector>
#include <iostream>

// vector values:
// A = GCS of sequence [0]
// B = GCS that includes first value (0 if < 0) [1]
// C = GCS that includes last value (0 if < 0) [2]
// D = sum of entire sequence [3]
template<typename RAIter>
std::vector<int> contigSumHelper(RAIter first, RAIter last)
{
	int A, B, C, D;

	//base case
	if(last - first == 1)
	{
		int currentVal = *first;
		D = currentVal;
		A = B = C = std::max(currentVal, 0);
	}

	else
	{
		// split set in two
		RAIter middle = (last - first) / 2 + first;
		// RAIter middle = first;
		// std::advance(middle, std::distance(first, last) / 2);
		std::vector<int> firstHalf = contigSumHelper(first, middle);
		std::vector<int> secondHalf = contigSumHelper(middle, last);

		// candidates for A are the greatest of first half's A, 
		// second half's A, and first's C + second's B:
		int A_1 = firstHalf[0];
		int A_2 = secondHalf[0];
		int A_3 = firstHalf[2] + secondHalf[1];
		A = std::max({A_1, A_2, A_3});

		// candidates for B are either the first half's B, or
		// the entire first half plus the second's B:
		int B_1 = firstHalf[1];
		int B_2 = firstHalf[3] + secondHalf[1];
		B = std::max(B_1, B_2);

		// candidates for C are either the second half's C, or
		// the entire second half plus the first's C:
		int C_1 = secondHalf[2];
		int C_2 = secondHalf[3] + firstHalf[2];
		C = std::max(C_1, C_2);

		D = firstHalf[3] + secondHalf[3];
	}
	return {A, B, C, D};
}



template<typename RAIter>
int contigSum(RAIter first, RAIter last)
{
	if(last - first == 1)
		return std::max(*first, 0);
	else if(last == first)
		return 0;

	return contigSumHelper(first, last)[0];
}

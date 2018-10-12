//inversions.hpp
// Bryce Melegari
// last modified 10/11/2018

// UAF Fall 2018 CS411 Assignment 3, part B.
// See HW3.pdf for problem statement. Counts the number of
// inversions in a given list by sorting it. The sort functionality
// is written by Glenn Chappell, and exists between the "[begin/end] copied code"
// comment blocks. Any code added within the copied code by myself is commented "-BAM"

#pragma once

// begin copied code ----------------------------------------------------------------
// Glenn G. Chappell
// 9 Oct 2015
//
// For CS 411/611 Fall 2015
// Merge Sort using Iterators

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <vector>
using std::vector;
#include <cstddef>
using std::size_t;
#include <algorithm>
using std::copy;
#include <iterator>
using std::distance;
using std::advance;
#include <type_traits>
using std::remove_reference;


// stableMerge
// Merge two halves of a sequence, each sorted, into a single sorted
// sequence in the same location. Merge is done in a stable manner.
// Requirements on Types:
//     FDIter is a forward iterator type.
//     The value type of FDIter has default ctor, dctor, copy=,
//      operator<.
//     operator< is a total order on the value type of FDIter.
// Pre:
//     [first, middle) and [middle, last) are valid ranges, each sorted
//      by <.
// Post:
//     [first, last) contains the same items as it did initially, but
//      now sorted by < (in a stable manner).

// changed to size_t -BAM
template <typename FDIter>
size_t stableMerge(FDIter first, FDIter middle, FDIter last)
{
    int count = 0; // -BAM
    // ** C++03:
    //typedef typename iterator_traits<FDIter>::value_type Value;
    // ** C++11:
    using Value = typename remove_reference<decltype(*first)>::type;
    // ** Is this really better?

    vector<Value> buffer(distance(first, last));
                                // Buffer for temporary copy of data
    FDIter in1 = first;         // Read location in 1st half
    FDIter in2 = middle;        // Read location in 2nd half
    auto out = buffer.begin();  // Write location in buffer
    // ** auto! That *is* better than vector<Value>::iterator

    // Merge two sorted lists into a single list in buff.
    while (in1 != middle && in2 != last)
    {
        if (*in2 < *in1)  // Must do comparison this way, to be stable.
        {
            *out++ = *in2++;
        	count += middle - in1; // items skipped = items remaining in 1st half. -BAM
 		}
        else
            *out++ = *in1++;
    }

    // Copy remainder of original sequence to buffer.
    // Only one of the following two copy calls will do anything, since
    //  the other is given an empty source range.
    copy(in1, middle, out);
    copy(in2, last, out);

    // Copy buffer contents back to original sequence location.
    copy(buffer.begin(), buffer.end(), first);

    return count; // -BAM
}


// mergeSort
// Sorts a sequence, using Merge Sort.
// Recursive.
// Requirements on Types:
//     FDIter is a forward iterator type.
//     The value type of FDIter has default ctor, dctor, copy=,
//      operator<.
//     operator< is a total order on the value type of FDIter.
// Pre:
//     [first, last) is a valid range.
// Post:
//     [first, last) contains the same items as it did initially,
//      but now sorted by < (in a stable manner).

// Modified to size_t, returns number of inversions
template <typename FDIter>
size_t mergeSort(FDIter first, FDIter last)
{
    // Compute size of sequence
    size_t size = distance(first, last);

    // BASE CASE
    if (size <= 1)
        return 0; // made 0 -BAM

    // RECURSIVE CASE
    FDIter middle = first;
    advance(middle, size/2);  // middle is iterator to middle of range

    // recursively sort the two lists and merge them, adding together their inversion return values. -BAM
    return mergeSort(first, middle) + mergeSort(middle, last) + stableMerge(first, middle, last);
}

// End Copied code -----------------------------------------------------------------

template<typename RAIter>
size_t inversions(RAIter first, RAIter last)
{
	return mergeSort(first, last);
}

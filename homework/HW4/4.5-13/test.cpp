//Bryce Melegari

//design a O(n) algorithm for searching for a specific number in
//an nxn matrix which has both its rows and columns sorted in increasing order.

#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <algorithm>
using std::sort;
#include <time.h>
#include <stdlib.h>


using nMat = vector<vector<int>>;


void sortMatrix(nMat & mat)
{
	int n = mat.size();

	// temporary matrix of size n^2 
    int temp[n * n]; 
    int k = 0; 
  
    // copy the elements of matrix one by one 
    // into temp[] 
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < n; j++) 
            temp[k++] = mat[i][j]; 
  
    // sort temp[] 
    sort(temp, temp + k); 
      
    // copy the elements of temp[] one by one 
    // in mat[][] 
    k = 0; 
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < n; j++) 
            mat[i][j] = temp[k++]; 
} 

nMat testMatrix(int n)
{
	nMat mat;
	srand(time(NULL));

	for(int i = 0; i < n; ++i)
	{
		vector<int> temp;
		for(int j = 0; j < n; ++j)
		{
			temp.push_back(rand() % 100);
		}
		mat.push_back(temp);		
	}


	return mat;
}

void printMatrix(nMat mat)
{
	for(int i=0; i < mat.size(); ++i)
	{
		for(int j=0; j < mat.size(); ++j)
		{
			if(mat[i][j] < 10)
			cout << mat[i][j] << " , ";
			else
			cout << mat[i][j] << ", ";
		}
		cout << endl;
	}
}

// searches matrix for target. If found, returns
// index of target in matrix, else returns {-1, -1}.
// Note that matrix is indexed with (0,0) in top left,
// as (row #, column #)
vector<int> searchMatrix(nMat & mat, int target)
{
	int n = mat.size();
	for(int i = 0; i < n; ++i)
	{	
		if(mat[i][n-1] == target)
			return {i, n-1};
		if(mat[i][n-1] > target) // walk back through current row
		{
			for(int j = n-2; j >= 0; --j)
			{
				if(mat[i][j] == target)
					return {i, j};
			}
			
		}
	}
	//if not found
	return {-1, -1};
}


int main(int argc, char *argv[])
{
	int n = std::strtol(argv[1], &argv[1], 10);
	nMat mat = testMatrix(n);
  
    cout << "Original Matrix:\n"; 
    printMatrix(mat); 
  
    sortMatrix(mat); 
  
    cout << "\nMatrix After Sorting:\n"; 
    printMatrix(mat); 

    int target;
    cout << "\ninput search target:\n";
    cin >> target;

    vector<int> result = searchMatrix(mat, target);
    cout << result[0] << ", " << result[1] << endl;

  
	return 0;
}
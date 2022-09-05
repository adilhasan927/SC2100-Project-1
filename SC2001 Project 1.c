// SC2001 Project 1.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
	srand(time(NULL));   // Initialization, should only be called once.
	printf("Hello world!");
}

int* gen_input_data(int count) {
	int* array = malloc(sizeof(int) * count);
	for (int i = 0; i < count; i++)
	{
		array[i] = rand();
	}
}

void InsertionSort(int* arr_start, int n) {
	for (int i = 1; i < n; i++) {
		for(int j=i; j>0; j--) {
			if(arr_start[j] < arr_start[j-1])
				swap(arr_start[j],arr_start[j-1]);
			else break;
		}
	}
}

void mergeInsert(int first, int last, int S, int* arr_start) { // Hybrid Algorithm
        if (last - first <= 0) {
            return;
        } else if (last - first > S) {
            int mid = (first + last) / 2;
            mergeInsert(first, mid, S);
            mergeInsert(mid + 1, last, S);
            merge(first, last);
        } else if (last-first <= S) {
            insertionSort(first, last);
        }
    }



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

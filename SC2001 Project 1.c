// SC2001 Project 1.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int CMP_CNT;

int main()
{
	srand(time(NULL));   // Initialization, should only be called once.
	printf("Hello world!");

}

typedef void *(SORT_FN_PTR) (int, int, int*);

typedef struct timing_info {
	double cpu_time;
	int cmp_cnt;
} TIMING_INFO;

 test_alg_on_rand(int count, SORT_FN_PTR fn_ptr, TIMING_INFO* timing_info) {
	CMP_CNT = 0;

	clock_t start, end;
	double cpu_time_used;

	start = clock();

	int array = gen_input_data(count);

	fn_ptr(0, count - 1, array);

	if (!test_is_sorted(array, count)) {
		return 0;
	};

	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

	timing_info->cpu_time = cpu_time_used;
	timing_info->cmp_cnt = CMP_CNT;
	
	return 1;
};

int test_is_sorted(int* array, int count) {
	for (int i = 0; i < count-1; i++)
	{
		if (array[i + 1] < array[i])
		{
			return 0;
		}
	}
	return 1;
}

int* gen_input_data(int count) {
	int* array = malloc(sizeof(int) * count);
	for (int i = 0; i < count; i++)
	{
		array[i] = rand();
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

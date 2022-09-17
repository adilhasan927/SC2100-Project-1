// SC2001 Project 1.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int CMP_CNT;

typedef void (SORT_FN_PTR)(int, int, int, int*);

int* BACKING_ARRAY = NULL;

typedef struct timing_info {
	double cpu_time;
	int cmp_cnt;
} TIMING_INFO;

int cmp_lt(int a, int b) {
	CMP_CNT++;
	return a < b;
}

int cmp_gt(int a, int b) {
	CMP_CNT++;
	return a > b;
}

int cmp_lte(int a, int b) {
	CMP_CNT++;
	return a <= b;
}

int cmp_gte(int a, int b) {
	CMP_CNT++;
	return a >= b;
}

void InsertionSort(int start_idx, int end_idx, int unused, int* arr);

int test_alg_on_array(int count, int* array, SORT_FN_PTR fn_ptr,
	int size_for_mergeinsert, TIMING_INFO* timing_info);

int* gen_input_data(int count);

void MergeInsert(int first, int last, int S, int* arr_start);

void MergeSort(int first, int last, int unused, int* arr_start);

int test_is_sorted(int* array, int count);

int main() {
	for (int i = 0; i < 1; i++)
	{
		inner_main();
	}
}

int inner_main()
{
	srand(time(NULL));   // Initialization, should only be called once

	TIMING_INFO timing_info;

	//Creating CSV file
	int s = 0; //threshold
	int n = 20'000; //size of array
	FILE *fpt;
	
	errno_t errno;

	BACKING_ARRAY = malloc(sizeof(int) * 10'000'000);

	//Open File
	if (errno = fopen_s(&fpt, "C:\\Users\\Adil\ Hasan\\Desktop\\Data.csv","w")) exit(errno);

	fseek(fpt, 0L, SEEK_END);

	int sz = ftell(fpt);
	if (sz == 0)
		fprintf(fpt,"Algorithm,Threshold,Arr_Size,Time,Comparisons\n"); //Header of CSV rows

	//Generate datasets
	int num_datasets = 0;
	int last_marker = 1000;
	for (int i = 1000; i <= 10'000'000; i += last_marker)
	{
		if (i == last_marker * 10)
		{
			last_marker = i;
		}
		num_datasets++;
	}

	int** datasets = malloc(sizeof(int*) * num_datasets);
	int* sizes = malloc(sizeof(int) * num_datasets);

	last_marker = 1000;
	for (int i = 1000, j = 0; i <= 10'000'000; i += last_marker, j++)
	{
		if (i == last_marker * 10)
		{
			last_marker = i;
		}
		datasets[j] = gen_input_data(i);
		sizes[j] = i;
		printf("Generating: (Dataset #: %d. Dataset size: %d)\n", j+1, i);
	}

	printf("\nRunning the hybrid algorithm on our datasets, with the\n\
			insertion sort threshold size fixed at 32 bytes.\n\n");

	for (int i = 0; i < num_datasets; i++)
	{
		test_alg_on_array(sizes[i], datasets[i], &MergeInsert, 32, &timing_info);
		printf("\tRunning on: (Dataset #: %d. Dataset size: %d)\n", i + 1, sizes[i]);
		fprintf(fpt, "Hybrid,32,%d,%f,%d\n", sizes[i], timing_info.cpu_time, timing_info.cmp_cnt);
	}

	printf("\nRunning the hybrid algorithm on our datasets, with the\n\
			insertion sort threshold size varying from 0-100.\n\n");

	for (int i = 0; i < num_datasets; i++)
	{
		printf("\tRunning on: (Dataset #: %d. Dataset size: %d)\n\n", i+1, sizes[i]);

		for (int j = 0; j <= 100; j++)
		{
			printf("\t\tRunning with: (Threshold: %d)\n", j);

			test_alg_on_array(sizes[i], datasets[i], &MergeInsert, j, &timing_info);
			fprintf(fpt, "Hybrid,%d,%d,%f,%d\n", j, sizes[i], timing_info.cpu_time, timing_info.cmp_cnt);
		}

		printf("\n");
	}

	printf("\nRunning the pure mergesort algorithm on our datasets, with the\n\
			insertion sort threshold size N/A.\n\n");

	for (int i = 0; i < num_datasets; i++)
	{
		test_alg_on_array(sizes[i], datasets[i], &MergeSort, 32, &timing_info);
		printf("\tRunning on: (Dataset #: %d. Dataset size: %d)\n", i + 1, sizes[i]);
		fprintf(fpt, "MergeSort,0,%d,%f,%d\n", sizes[i], timing_info.cpu_time, timing_info.cmp_cnt);
	}

	fclose(fpt);

	return;
	
}

void swap(int* arr, int a, int b)
{
	int temp = arr[b];
	arr[b] = arr[a];
	arr[a] = temp;
}

void merge(int start, int end, int* arr_start) {
	int mid = (start + end) / 2;

	int first_bucket = start;
	int second_bucket = mid + 1;
	int backing_idx = 0;

	if (end - start <= 0) {
		return;
	}

	while (first_bucket <= mid && second_bucket <= end) {
		int fst_bucket_gt_snd_bucket = cmp_gt(arr_start[first_bucket], arr_start[second_bucket]);

		if (fst_bucket_gt_snd_bucket) {
			BACKING_ARRAY[backing_idx] = arr_start[second_bucket];
			second_bucket++;
			backing_idx++;
		}

		else if (!fst_bucket_gt_snd_bucket) {
			BACKING_ARRAY[backing_idx] = arr_start[first_bucket];
			first_bucket++;
			backing_idx++;
		}
	}

	while (first_bucket <= mid)
	{
		BACKING_ARRAY[backing_idx] = arr_start[first_bucket];
		first_bucket++;
		backing_idx++;
	}

	while (second_bucket <= mid)
	{
		BACKING_ARRAY[backing_idx] = arr_start[second_bucket];
		second_bucket++;
		backing_idx++;
	}

	backing_idx--;
	for (backing_idx; backing_idx >= 0; backing_idx--)
	{
		arr_start[start + backing_idx] = BACKING_ARRAY[backing_idx];
	}
}

/*
void merge(int start, int end, int* arr_start) {
	int mid = (start + end) / 2;

	int first_bucket = start;
	int second_bucket = mid + 1;
	
	if (end - start <= 0) {
		return;
	}

	while (first_bucket <= mid && second_bucket <= end) {
		int fst_bucket_gt_snd_bucket = cmp_gt(arr_start[first_bucket], arr_start[second_bucket]);

		if (fst_bucket_gt_snd_bucket) {
			//set tmp to the first element in the second bucket
			int tmp = arr_start[second_bucket++];

			for (int i = ++mid; i > first_bucket; i--) {
				arr_start[i] = arr_start[i - 1];
			}
			arr_start[first_bucket++] = tmp;
		}

		else if (!fst_bucket_gt_snd_bucket) {
			first_bucket++;
		}
	}
}
*/

void InsertionSort(int start_idx, int end_idx, int unused, int* arr) {
	int* arr_start = &(arr[start_idx]);
	int n = (end_idx - start_idx) + 1;
	for (int i = 1; i < n; i++) {
		for (int j = i; j > 0; j--) {
			if (cmp_lt(arr_start[j], arr_start[j - 1]))
				swap(arr_start, j, j - 1);
			else break;
		}
	}
}

void MergeSort(int first, int last, int unused, int* arr_start) { // Non-Hybrid Algorithm
	if (last - first <= 0) {
		return;
	}
	int mid = (first + last) / 2;
	MergeSort(first, mid, unused, arr_start);
	MergeSort(mid + 1, last, unused, arr_start);
	merge(first, last, arr_start);
}

void MergeInsert(int first, int last, int S, int* arr_start) { // Hybrid Algorithm
	if (last - first <= 0) {
		return;
	}
	else if (last - first > S) {
		int mid = (first + last) / 2;
		MergeInsert(first, mid, S, arr_start);
		MergeInsert(mid + 1, last, S, arr_start);
		merge(first, last, arr_start);
	}
	else if (last - first <= S) {
		InsertionSort(first, last, 0, arr_start);
	}
}

int test_alg_on_array(int count, int* array, SORT_FN_PTR fn_ptr, int size_for_mergeinsert, TIMING_INFO* timing_info) {
	CMP_CNT = 0;

	clock_t start, end;
	double cpu_time_used;

	start = clock();

	fn_ptr(0, count - 1, size_for_mergeinsert, array);

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
			exit(-1);
			return 0;
		}
	}
	return 1;
}

int* gen_input_data(int count) {
	int* array = (int*)malloc(sizeof(int) * count);

	if (array == NULL)
	{
		exit(1);
	}

	for (int i = 0; i < count; i++)
	{
		int randval = 0;
		while(randval <= 0)
		{
			randval = rand();
		}
		array[i] = randval;
	}

	return array;
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

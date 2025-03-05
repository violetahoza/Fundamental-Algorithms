/*
	Hoza Violeta Maria, Group 30424
	I've implemented the selection sort algorithm in both iterative and recursive manner. The complexity is O(n^2).
	As expected, the nr of operations performed by the 2 algorithms is exactly the same. Regarding the execution time, we can see from the chart that the iterative version is better than the recursive one.
*/

#include <iostream>
#include "Profiler.h"

Profiler p("selectionSort");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

using namespace std;

void printArray(int* array, int size);
void iterativeSelectionSort(int* array, int n);
void recursiveSelectionSort(int* array, int n, int i);
void demo();
void perf_op(int order);
void perf_opAll();
void perf_time();

void printArray(int* array, int size)
{
	for (int i = 0; i < size; i++)
		cout << array[i] << " ";
	cout << endl;
}

void iterativeSelectionSort(int* array, int n)//not stable:( 
{
	//easy put - look for the minimum in the unsorted part of the array and place it on its correct position in the final version of the array
	//Operation opComp = p.createOperation("iterativeSelection-comp", n); 
	//Operation opAssig = p.createOperation("iterativeSelection-assig", n); 

	for (int i = 0; i < n - 1; i++)//I start with the 1st element of the unsorted part of the array
	{
		int minIndex = i;
		for (int j = i + 1; j < n; j++)//I look for the minimum value in the unsorted array
		{
			if (array[j] < array[minIndex])
			{
				minIndex = j;//I update the position of the min element
			}
			//opComp.count(1);//I count the comparison outside the if, because even if the condition is false, the comparison is still evaluated  
		}
		if (minIndex != i)
		{
			//swap the minimum with the first element of the unsorted part of the array => the minimum is placed in its correct position
			//opAssig.count(3); 
			//int aux = array[minIndex];
			//array[minIndex] = array[i];
			//array[i] = aux;
			swap(array[minIndex], array[i]); 
		}
	}
	
}


void recursiveSelectionSort(int* array, int n, int i = 0)
{
	//The idea of the recursive solution is to increment the sorted part one by one and recursively call for the remaining (yet to be sorted) part.

	//Operation opComp = p.createOperation("recursiveSelection-comp", n);
	//Operation opAssig = p.createOperation("recursiveSelection-assig", n);

	// Return when the starting index and the size are same
	if (i == n) 
		return;

	int minIndex = i;
	for (int j = i + 1; j < n; j++)//I look for the minimum value in the unsorted subarray
	{
		if (array[j] < array[minIndex])
		{
			minIndex = j;//I update the position of the min element
		}
		//opComp.count(1);//I count the comparison outside the if, because even if the condition is false, the comparison is still evaluated 
	}
	if (minIndex != i)
	{
		swap(array[minIndex], array[i]);
		//opAssig.count(3);
	}
	
	if (i + 1 < n)
		recursiveSelectionSort(array, n, i + 1); // Recursively calling the selection sort function
}

void demo()
{
	//demo function for checking if the algorithms work
	int v[] = { 4, 5, 12, 9, 7, 2, 10, 5, 15, 7, 6 };
	int u[] = { 4, 5, 12, 9, 7, 2, 10, 5, 15, 7, 6 };
	int n = sizeof(v) / sizeof(v[0]);

	iterativeSelectionSort(v, n);
	cout << "The array sorted using the iterative selection sort: ";
	printArray(v, n); 

	recursiveSelectionSort(u, n);
	cout << "The array sorted using the recursive selection sort: ";
	printArray(u, n);

}


void perf_op(int order)
{
	int v[MAX_SIZE], u[MAX_SIZE];
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)//vary the size of the input array
		for (int test = 0; test < NR_TESTS; test++) //I repeat the measurements NR_TESTS times
		{
			FillRandomArray(v, n, 10, 50000, false, order);
			if (order == 2)
				v[n - 1] = v[0] + 1;
			memcpy(u, v, sizeof(v));// I make a copy of the generated array, so I don't sort an already sorted array 
			iterativeSelectionSort(v, n);  
			recursiveSelectionSort(u, n); 
		}

	p.divideValues("iterativeSelection-comp", NR_TESTS);
	p.divideValues("iterativeSelection-assig", NR_TESTS);
	p.addSeries("iterativeSelection-op", "iterativeSelection-comp", "iterativeSelection-assig");

	p.divideValues("recursiveSelection-comp", NR_TESTS);
	p.divideValues("recursiveSelection-assig", NR_TESTS);
	p.addSeries("recursiveSelection-op", "recursiveSelection-comp", "recursiveSelection-assig");

	p.createGroup("operations", "iterativeSelection-op", "recursiveSelection-op");
	p.createGroup("comparations", "iterativeSelection-comp", "recursiveSelection-comp");
	p.createGroup("assignments", "iterativeSelection-assig", "recursiveSelection-assig");

	//p.showReport();
}

void perf_opAll()
{
	perf_op(UNSORTED);//the generated array is unordered
	p.reset("best");
	perf_op(ASCENDING);//already sorted
	p.reset("worst");
	perf_op(DESCENDING);//in decreasing order
	p.showReport();
}


/*
	profiler.startTimer("your_function", current_size);
	for(int test=0; test<nr_tests; ++test) {
		your_function(array, current_size);
	}
	profiler.stopTimer("your_function", current_size);
*/


void perf_time()
{
	int v[MAX_SIZE], u[MAX_SIZE];
	
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)//vary the size of the input array
	{
		FillRandomArray(v, n, 10, 50000, false, UNSORTED);
		//memcpy(u, v, n);

		p.startTimer("iterativeSelectionSort", n);
		for (int test = 0; test < 100; test++)
		{
			memcpy(u, v, n);//making a copy because I don't want to sort an array already sorted
			iterativeSelectionSort(u, n);
		}
		p.stopTimer("iterativeSelectionSort", n);

		p.startTimer("recursiveSelectionSort", n);
		for (int test = 0; test < 100; test++)
		{
			memcpy(u, v, n);
			recursiveSelectionSort(u, n);
		}
		p.stopTimer("recursiveSelectionSort", n);

	}
	
	p.divideValues("iterativeSelectionSort", 100);
	p.divideValues("recursiveSelectionSort", 100);
	p.createGroup("TimeComparison", "iterativeSelectionSort", "recursiveSelectionSort");
	p.showReport();  
}


int main()
{
	demo();
	//perf_opAll();
	//perf_time();   
	return 0;
}
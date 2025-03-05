/*
	Hoza Violeta Maria, Group 30424
	I have implemented 3 sorting algorithms: Insertion Sort, Selection Sort and Bubble Sort.
	Remarks:
		- only the algorithms for Insertion Sort and Bubble Sort are stable, because they preserve the order of the equal elements from the beginning to the end of the algorithm
		- for all 3 algorithms, in the average case and in the worst case we have a O(n^2) complexity
		- for all 3 algorithms, the best case is when the array is already sorted
	For Insertion Sort and Bubble Sort, in the best case the complexity is O(n), while for Selection Sort it is O(n^2).
	In the best case:
		-for bubble sort: there are no swaps made, so the nr of assignments is 0 ( O(1) complexity); the array is traversed 
		only once, so the nr. of comparisons is n-1 (n - the nr of elements) => the total nr of operations = n-1 ( O(n) complexity)
		-insertion sort: nr of assignments = nr of comparisons = n-1, because we start traversing the array from the 2nd element => 2*(n-1) operations => O(n) complexity
		-selection sort: regarding the nr of assignments, the complexity is O(1) (we don't have any assignments to do); however, there are 
		a lot of comparisons made, so the overall complexity is O(n^2)
		-given a best case scenario and regarding the total nr of operations, the most efficient algorithm is bubble sort (the nr of 
		operations = n-1), followed by insertion sort (the nr of op = (n-1)*2)
	In the average case:
		-selection sort performs a considerably smaller nr of assignments (from this point of view the complexity is O(n), ~ 3*(n-1) assig.) than the other 2
		algorithms, while insertion sort performs less comparisons than the others
		-overall, in terms of efficiency, we can easily observe that bubble sort comes last, while insertion sort and selection sort are
		close to each other, being far more efficient than bubble sort
	In the worst case:
		-for insertion sort and bubble sort the worst case scenario is when the elements are in descreasing order; in this case we can see that
		insertion sort is way better than bubble sort (the nr of operations performed by the 1st is at least half of the nr of operations
		performed by the former one)
		-I've considered an array with the elements in decreasing order as the worst case for the selection sort alg (however, a case that 
		can be just as bad as this is the following: 2,3,...,n-1,1); in this case, the most efficient algorithm is selection sort, performing 
		only half the nr of operations compared to the insertion sort algorithm.
	I've noticed that for selection sort the nr of comparisons is constant (4950 in all 3 cases), and the nr of assignments is very small compared to the other algorithms 
	(constant in the best case, linear function for the others).
	Also, for insertion sort, I've noticed that in the average case and in the worst case the nr. of assignments is very close to the nr of comparisons performed.

*/

#include <stdio.h>
#include "Profiler.h"

Profiler p("sortingAlg");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

void printArray(int* array, int size)
{
	printf("The sorted array is: ");
	for (int i = 0; i < size; i++)
		printf("%d ", array[i]);
	printf("\n");
}

void insertionSort(int* array, int n)//stable:)
{
	//easy take - take the 1st element from the unsorted part and look for its position in the ordered part
	Operation opComp = p.createOperation("insertion-comp", n);
	Operation opAssig = p.createOperation("insertion-assig", n);

	for (int i = 1; i < n; i++)//start from the second element (the array is indexed from 0)
	{
		int buffer = array[i];//I make an assignment,
		opAssig.count();//so I count it
		int j = i - 1;
		while (j >= 0 && buffer < array[j])//the operator '<' ensures the stability of the algorithm, the order of the equal elements in the sorted array being the same as in  the initial array
		{
			opComp.count();
			//the elements greater than the current element are shifted with one position to the right
			array[j + 1] = array[j];
			opAssig.count();
			j--;
		}
		opComp.count();//if the result of the comparison is false, we don't enter the while, but the comparison was evaluated so we have to count it
		if (i - 1 != j) //if the condition is false, it means that the current element was already in its correct position
		{
			array[j + 1] = buffer;
			opAssig.count();
		}
	}
}

void selectionSort(int* array, int n)//not stable:(
{
	//easy put - look for the minimum in the unsorted part of the array and place it on its correct position in the final version of the array
	Operation opComp = p.createOperation("selection-comp", n);
	Operation opAssig = p.createOperation("selection-assig", n);

	for (int i = 0; i < n - 1; i++)//I start with the 1st element of the unsorted part of the array
	{
		int minIndex = i;
		for (int j = i + 1; j < n; j++)//I look for the minimum value in the unsorted array
		{
			if (array[j] < array[minIndex])
			{
				minIndex = j;//I update the position of the min element
			}
			opComp.count(1);//I count the comparison outside the if, because even if the condition is false, the comparison is still evaluated
		}
		if (minIndex != i)
		{
			//swap the minimum with the first element of the unsorted part of the array => the minimum is placed in its correct position
			opAssig.count(3);
			int aux = array[minIndex];
			array[minIndex] = array[i];
			array[i] = aux;
		}
	}
}

void bubbleSort(int* array, int n)//stable:)
{
	//one pass
	//at each pass the max value is moved to the right6;
	Operation opComp = p.createOperation("bubble-comp", n);
	Operation opAssig = p.createOperation("bubble-assig", n);

	int swaps;
	do
	{
		swaps = 0;
		for (int i = 0; i < n - 1; i++)
		{
			if (array[i] > array[i + 1])// '>' gives the stability of the algorithm
			{
				swaps = 1;
				int aux = array[i];
				array[i] = array[i + 1];
				array[i + 1] = aux;
				opAssig.count(3);
			}
			opComp.count();
		}
	} while (swaps != 0);//if no swap was made, it means that the array was already sorted
}

void demo()
{
	//demo function for checking if the algorithms work
	int array[] = { 4, 5, 12, 9, 7, 2, 10, 5, 15, 7, 6 };
	int n = sizeof(array) / sizeof(array[0]);

	//insertionSort(array, n);
	//selectionSort(array, n);
	bubbleSort(array, n);

	printArray(array, n);
}

void perf(int order)
{
	int v[MAX_SIZE], u[MAX_SIZE];
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)//vary the size of the input array
		for (int test = 0; test < NR_TESTS; test++) //I repeat the measurements NR_TESTS times
		{
			FillRandomArray(v, n, 10, 50000, false, order);
			memcpy(u, v, sizeof(v));// I make a copy of the generated array, so I don't sort an already sorted array multiple times
			insertionSort(u, n);
			memcpy(u, v, sizeof(v));
			selectionSort(u, n);
			memcpy(u, v, sizeof(v));
			bubbleSort(u, n);
		}
	
	
	p.divideValues("insertion-comp", NR_TESTS);
	p.divideValues("insertion-assig", NR_TESTS);
	p.addSeries("insertion-op", "insertion-comp", "insertion-assig");
	
	
	p.divideValues("selection-comp", NR_TESTS);
	p.divideValues("selection-assig", NR_TESTS);
	p.addSeries("selection-op", "selection-comp", "selection-assig");
	
	
	p.divideValues("bubble-comp", NR_TESTS);
	p.divideValues("bubble-assig", NR_TESTS);
	p.addSeries("bubble-op", "bubble-comp", "bubble-assig");

	
	p.createGroup("assignments", "insertion-assig", "selection-assig", "bubble-assig");
	p.createGroup("comparisons", "insertion-comp", "selection-comp", "bubble-comp");
	p.createGroup("operations", "insertion-op", "selection-op", "bubble-op");

	//for point 3
	if(order == UNSORTED) //for the average case, I represent the nr of assig for sel sort on a separate chart, because we can't observe it properly along the others
		p.createGroup("assignments-selection", "selection-assig");
	else if (order == ASCENDING) //for the best case
	{
		p.createGroup("assignments-selection", "selection-assig");
		p.createGroup("assignments-bubble", "bubble-assig");
		p.createGroup("comparisons-bubble", "bubble-comp");
		p.createGroup("comparisons-insertion", "insertion-comp");
		p.createGroup("operations-insertion", "insertion-op");
		p.createGroup("operations-bubble", "bubble-op");
	}
	else //for the worst case
	{
		p.createGroup("assignments-selection", "selection-assig");
		//I place the nr of assignments for sel sort on another chart, because it cannot be visualised correctly along the others (the others have a larger growth rate)
	}
	
	//p.showReport();

}

void perf_all()
{
	perf(UNSORTED);//the generated array is unordered
	p.reset("best");
	perf(ASCENDING);//already sorted
	p.reset("worst");
	perf(DESCENDING);//in decreasing order
	p.showReport();
}

int main()
{
	demo();
	//perf_all();
	return 0;
}





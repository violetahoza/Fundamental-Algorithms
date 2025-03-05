/*
	Hoza Violeta Maria, Group 30424
	I've implemented 2 methods that can be used for building a max heap: bottom-up and top-down strategy + a sorting algorithm based on heaps (heapsort).
	Remarks:
	- average case:
		- bottom-up approach: O(n) complexity (we can build a max heap from an unordered array in linear time)
		- top-down: O(n * log(n)) complexity 
		- the nr. of comparisons made by the bottom-up implementation is close to the one made by the top-down implementation, while the nr of assignments made by the 1st is smaller than 
		in the case of the former
	- worst case:
		- the worst case scenario for building a max heap is when the elements of the given array are in ascending order (because every child is larger 
		than its parent, so it needs to be moved up to the root => we get the maximum nr of comparisons and assignments for each element)
		- the bottom-up approach performs well even in the worst case scenario, still having a linear time complexity (O(n))
		- however, the top-down approach has a O(n * log(n)) complexity (because of the great nr of assignments)(log(n) comes from the height of the binary heap)
		- the nr of comparisons made by the top-down implementation is ~twice the nr of comp made by the other one
	Conclusion: The bottom-up approach is more efficient.
	About heapsort: O(n * log(n)) complexity
*/

#include <iostream>
#include "Profiler.h"

Profiler p("buildHeap");

#define MAX_SIZE 10000 //the maximum size of the input
#define STEP_SIZE 100 
#define NR_TESTS 5 //how many times we repeat the measurements

using namespace std;

void printArray(int *array, int size)
{
	for (int i = 0; i < size; i++)
		cout << array[i] << " ";
	cout << endl;
}

void heapify(int* array, int size, int i)//the running time of heapify for a single node is of O(logn) 
{
	//sink the root in its right place in the heap 

	Operation opComp = p.createOperation("bottomUp-comp", size);
	Operation opAssig = p.createOperation("bottomUp-assig", size);

	int largest = i;//i is the root of the tree / subtree on which we are applying the recursive function
	int leftChild = 2 * i + 1; //because the array is indexed from 0
	int rightChild = 2 * i + 2;

	opComp.count(2);
	if (leftChild < size && array[leftChild] > array[largest])//if the left child is larger than the root
		largest = leftChild;//update the index of the largest element
	if (rightChild < size && array[rightChild] > array[largest])//if the right child is larger than the largest so far
		largest = rightChild;
	if (largest != i)//if the largest element is not the root (it means that the subtree doesn't respect the max heap property)
	{
		swap(array[i], array[largest]);
		opAssig.count(3);//count 3 because the swap makes 3 assignments
		heapify(array, size, largest);//heapify the affected subtree (in order to maintain the max heap property)
	}
}

void buildHeapBottomUp(int* array, int size)//building a max heap from a given array
{
	//use heapify in a bottom-up manner
	for (int i = size / 2 - 1; i >= 0; i--)//I start from the index of the last non-leaf node (because the leaves already respect the heap property) and go up to the root
		heapify(array, size, i);//heapify to restore the max heap property
}


void buildHeapTopDown(int* array, int size)//swim
{
	//top-down strategy
	//start with an empty heap, insert the current element into the heap, then heapify (maintain the heap property)
	
	Operation opComp = p.createOperation("topDown-comp", size);
	Operation opAssig = p.createOperation("topDown-assig", size);
	
	for (int i = 0; i < size; i++)
	{
		int j = i;//the position of the current element
		int parent = (j - 1) / 2;//the position of its parent
		while (j > 0 && array[j] > array[parent])//while the parent is smaller than its child
		{
			opComp.count();
			swap(array[j], array[parent]);//swap the current element with its parent
			opAssig.count(3);
			j = parent;//update the position of the current element
			parent = (j - 1) / 2;//and of its parent
		}
		//after this, the current element is placed in its right position in the heap
		opComp.count();//for the case when the condition is false and we don't enter the while
	}
}

void heapSort(int* array, int size)//not stable
{
	buildHeapBottomUp(array, size);//initially I rearrange the array by constructing a max-heap => the max is at the top of the heap 
	for (int i = size - 1; i > 0; i--)//I go from the last element to the second one
	{
		//at each step I extract an element from the heap (this element is actually the max and it will be placed at the end of the array)
		swap(array[0], array[i]);//swap the root with the last element in the current heap 
		heapify(array, i, 0);//"repair" the new heap (it's a smaller heap, because the last element in the array is no longer part of the heap, so we reduce the size of the heap by 1)
	}
}

void demo()
{
	int v[] = { 11, 7, 15, 4, 6, 3, 10, 9, 2, 15, 17 };
	int u[] = { 11, 7, 15, 4, 6, 3, 10, 9, 2, 15, 17 };
	int n = sizeof(v) / sizeof(v[0]);

	cout << "The initial array is: ";
	printArray(v, n);

	buildHeapBottomUp(v, n);
	cout << "The array representation after buildHeap (bottom-up strategy) is: ";
	printArray(v, n);

	buildHeapTopDown(u, n);
	cout << "The array representation after buildHeap (top-down strategy) is: ";
	printArray(u, n);

	heapSort(v, n);
	cout << "The array after being sorted using heapsort is: ";
	printArray(v, n);
}

void perf(int order)
{
	int v[MAX_SIZE], u[MAX_SIZE];
	for(int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)//vary the size of the input
		for (int test = 0; test < NR_TESTS; test++)//repeat the measurements by NR_TIMES times
		{
			FillRandomArray(v, n, 10, 50000, false, order);
			memcpy(u, v, sizeof(v));//make a copy of the generated array so I can use the same input for both algorithms
			buildHeapBottomUp(v, n);
			buildHeapTopDown(u, n);
		}

	p.divideValues("bottomUp-comp", NR_TESTS);
	p.divideValues("bottomUp-assig", NR_TESTS);
	p.addSeries("bottomUp-op", "bottomUp-comp", "bottomUp-assig");

	p.divideValues("topDown-comp", NR_TESTS);
	p.divideValues("topDown-assig", NR_TESTS);
	p.addSeries("topDown-op", "topDown-comp", "topDown-assig");

	p.createGroup("assignments", "bottomUp-assig", "topDown-assig");
	p.createGroup("comparisons", "bottomUp-comp", "topDown-comp");
	p.createGroup("operations", "bottomUp-op", "topDown-op");

}

void perf_all()
{
	perf(UNSORTED);//the generated array is unordered
	p.reset("worst");
	perf(ASCENDING);//the elements of the array are in ascending order
	p.showReport();
}


int main()
{
	demo();
	//perf_all();
	return 0;
}
/*
	Hoza Violeta Maria, Group 30424
	I've implemented the following algorithms: quickSelect, quicksort and heapsort, comparing and analysing the 2 sorting algorithms (the last 2 alg).
	
	-The quickSelect algorithm finds the kth smallest element in an unsorted array (obviously, without sorting the array first). This is how it works: first it chooses a random pivot from the
	array, then it partitions the array in 2 parts - one containing the elements smaller than the pivot, the other one containing the elements greater than the pivot. Next it determines the 
	position of the pivot in the array. If the position is k, then it has found the kth smallest element in the array and it return its value. If the position of the pivot is greater than k, 
	then it calls the recursive function for the left subarray (the one containing the elements smaller than the pivot), otherwise, if position(pivot) < k, it looks for the wanted element in the 
	subarray containing the elements larger than our pivot (the right one). It repeats these steps until it finds the kth smallest element in the array.

	Quicksort vs Heapsort:
	- average case:
		- quicksort:  O(n * log(n)) time complexity
		- heapsort: O(n * log(n)) time complexity (building the heap takes O(n) and each of the n-1 calls to heapify takes O(log(n)); log(n) comes from the height of the binary tree)
	- best case:
		- quicksort: O(n * log(n)) time complexity; the best case for quicksort is when the pivot chosen at each step divides the array into almost equal halves
		- heapsort: O(n * log(n)) time complexity 
	- worst case:
		- quicksort: O(n^2) time complexity, due to uneven partitioning; the worst case for quicksort is when the pivot chosen at each step divides the array into highly unbalanced parts 
		(example: when the array is already sorted and the chosen pivot is always the smallest or the largest element)
		- heapsort: O(n * log(n)) time complexity

	Remarks:
	- Heapsort has a constant time complexity ( O(n * log(n)) in all 3 cases), which makes it efficient for sorting large datasets.
	- In the average case quicksort is more efficient than heapsort. Even though in the worst case quicksort has O(n^2) time complexity, in real life it doesn't happen too often to have
	such case, so it is preferred over heapsort.
	- Heapsort is optimal, but quicksort is faster in practice.

*/

#include <iostream>
#include "Profiler.h"

Profiler p("quickSort");

#define MAX_SIZE 10000 //the maximum size of the input
#define STEP_SIZE 100 
#define NR_TESTS 5 //how many times we repeat the measurements

using namespace std;

//int heapifyAssig = 0, heapifyComp = 0;

void printArray(int* array, int size)
{
	for (int i = 0; i < size; i++)
		cout << array[i] << " ";
	cout << endl;
}

void heapify(int* array, int size, int i, Operation *comp, Operation *assig)//the running time of heapify for a single node is of O(logn) 
{
	//sink the root in its right place in the heap 

	//Operation opComp = p.createOperation("heapify-comp", size);
	//Operation opAssig = p.createOperation("heapify-assig", size);

	int largest = i;//i is the root of the tree / subtree on which we are applying the recursive function
	int leftChild = 2 * i + 1; //because the array is indexed from 0
	int rightChild = 2 * i + 2;

	(*comp).count(2);
	
	if (leftChild < size && array[leftChild] > array[largest])//if the left child is larger than the root
		largest = leftChild;//update the index of the largest element
	if (rightChild < size && array[rightChild] > array[largest])//if the right child is larger than the largest so far
		largest = rightChild;
	if (largest != i)//if the largest element is not the root (it means that the subtree doesn't respect the max heap property)
	{
		swap(array[i], array[largest]);
		(*assig).count(3);//count 3 because the swap makes 3 assignments
		//heapifyAssig += 3;
		heapify(array, size, largest, comp, assig);//heapify the affected subtree (in order to maintain the max heap property)
	}
}

void heapSort(int* array, int size)//not stable; optimal
{
	Operation opComp = p.createOperation("heapSort-comp", size);
	Operation opAssig = p.createOperation("heapSort-assig", size);

	//building the heap
	//initially I rearrange the array by constructing a max-heap => the max is at the top of the heap 
	//use heapify in a bottom-up manner
	for (int i = size / 2 - 1; i >= 0; i--)//I start from the index of the last non-leaf node (because the leaves already respect the heap property) and go up to the root
		heapify(array, size, i, &opComp, &opAssig);//heapify to restore the max heap property

	for (int i = size - 1; i > 0; i--)//I go from the last element to the second one
	{
		//at each step I extract an element from the heap (this element is actually the max and it will be placed at the end of the array)
		swap(array[0], array[i]);//swap the root with the last element in the current heap 
		opAssig.count(3);
		heapify(array, i, 0, &opComp, &opAssig);//"repair" the new heap (it's a smaller heap, because the last element in the array is no longer part of the heap, so we reduce the size of the heap by 1)
	}
	//opAssig.count(heapifyAssig);
	//opComp.count(heapifyComp);
}

int partition(int* array, int size, int left, int right)//Lomuto partition; O(n)
{
	//this function takes the last element as pivot and places all the elements smaller than the pivot to its left, while the elemnts greater than the pivot are placed to its right
	Operation opComp = p.createOperation("quickSort-comp", size);
	Operation opAssig = p.createOperation("quickSort-assig", size);


	int i = left - 1;//index of the smaller element
	for (int j = left; j <= right; j++)
	{
		opComp.count();
		if (array[j] < array[right])//if the current element is smaller than the pivot
		{
			i = i + 1;//increase the index of the smaller element
			swap(array[i], array[j]);
			opAssig.count(3);
		}
	}
	opAssig.count(3);
	swap(array[i + 1], array[right]);
	return (i + 1);
}

int partitionRandom(int* array, int size, int left, int right)//generates a random pivot and swaps it with the rightmost element of the array
{ 
	//used to avoid entering the worst case
	// Generate a random number in between left and right
	srand(time(NULL));
	int index = left + rand() % (right - left);
	swap(array[index], array[right]);
	return partition(array, size, left, right);
}

int medianOfThree(int* array, int left, int right)//return the middle element between these 3
{
	//used to enter the best case
	int middle = (left + right) / 2;
	if ((array[right] > array[left]) ^ (array[right] > array[middle]))//use XOR, not OR! (because only a single condition is allowed to be true)
		return right;
	else if ((array[left] < array[right]) ^ (array[left] < array[middle]))
		return left;
	else
		return middle;
}

int midPartition(int* array, int size, int left, int right)//by median partition, quicksort enters the best case always
{
	//approach: use the median-of-3 method (the chosen pivot is the median of a set of 3 elements randomly selected from the subarray)
	//int pivot = medianOfThree(array, left, right);
	int pivot = (left + right) / 2;
	swap(array[pivot], array[right]);
	return partition(array, size, left, right); 
}

void quickSort(int* array, int size, int left, int right, int type)//Divide et Impera (Divide and Conquer) algorithm; not stable; not optimal
{
	if (left < right)//if there are at least 2 elements
	{
		int p;
		if (type == 0)//for average case
			p = partitionRandom(array, size, left, right); //p is the pivot; array[p] is now at the right place
		else if(type == 1)//for worst case
			p = partition(array, size, left, right); 
		else p = midPartition(array, size, left, right); //for best case
		quickSort(array, size, left, p - 1, type);//sort the elements before the pivot
		quickSort(array, size, p + 1, right, type);//sort the elements after the pivot
	}
}

int quickSelect(int* array, int size, int left, int right, int i)//not optimal; O(n^2) for worst case, O(n) for best (element found after a single partition pass) and average
{
	//returns the ith smallest element in the array (without sorting the array)
	if (left == right)//if there is a single element 
		return array[left];
	int index = partitionRandom(array, size, left, right);//get the position of the pivot in the sorted array (the pivot being the last element)
	int k = index - left + 1;
	if (i == k)//if the pivot value is the answer
		return array[index];
	else if (i < k)//if the position of the pivot is greater than i
		return quickSelect(array, size, left, index - 1, i);//recursive call for the left subarray
	else return quickSelect(array, size, index + 1, right, i - k); //recursive call for the right subarray
}

void demo()
{
	int v[] = { 11, 7, 15, 4, 6, 3, 10, 9, 2, 15, 17 };
	int u[] = { 11, 7, 15, 4, 6, 3, 10, 9, 2, 15, 17 };
	int n = sizeof(v) / sizeof(v[0]);

	cout << "The initial array is: ";
	printArray(v, n);

	int i = 2;
	cout << "The " << i << "th smallest element of the array is: ";
	cout << quickSelect(v, n, 0, n - 1, i) << endl; 

	quickSort(v, n, 0, n - 1, 2);
	cout << "The array sorted using quicksort is: ";
	printArray(v, n);

	heapSort(u, n);
	cout << "The array sorted using heapsort is: ";
	printArray(u, n);
}

void perf(int order, int type)
{
	int v[MAX_SIZE], u[MAX_SIZE];

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)//vary the size of the input
		for (int test = 0; test < NR_TESTS; test++)//repeat the measurements by NR_TIMES times
		{
			FillRandomArray(v, n, 10, 50000, false, order);
			memcpy(u, v, sizeof(v));//make a copy of the generated array so I can use the same input for both algorithms 
			heapSort(u, n); 
			quickSort(v, n, 0, n - 1, type); 
		}

	p.divideValues("quickSort-comp", NR_TESTS);
	p.divideValues("quickSort-assig", NR_TESTS);
	p.addSeries("quickSort-op", "quickSort-comp", "quickSort-assig");

	/*p.divideValues("heapify-comp", NR_TESTS);
	p.divideValues("heapify-assig", NR_TESTS);
	p.addSeries("heapify-op", "heapify-comp", "heapify-assig");*/

	
	p.divideValues("heapSort-comp", NR_TESTS); 
	p.divideValues("heapSort-assig", NR_TESTS); 
	p.addSeries("heapSort-op", "heapSort-comp", "heapSort-assig"); 

	/*
	p.addSeries("heapsort-assig", "heapify-assig", "heapSort-assig");
	p.addSeries("heapsort-comp", "heapify-comp", "heapSort-comp");
	p.addSeries("heapsort-op", "heapify-op", "heapSort-op");*/

	p.createGroup("assignments", "heapSort-assig", "quickSort-assig");
	p.createGroup("comparisons", "heapSort-comp", "quickSort-comp");
	p.createGroup("operations", "heapSort-op", "quickSort-op");
	
	if (type == 1)
	{
		p.createGroup("heapSort-assignments","heapSort-assig");
		p.createGroup("heapSort-comparisons", "heapSort-comp");
		p.createGroup("heapSort-operations", "heapSort-op");
	}
}

void perf_all()
{
	perf(UNSORTED, 0);//the generated array is unordered and using partitionRandom 
	p.reset("worst"); 
	//heapifyAssig = 0;  
	//heapifyComp = 0; 
	perf(ASCENDING, 1);//already sorted, using partition 
	p.reset("best"); 
	//heapifyAssig = 0; 
	//heapifyComp = 0;
	perf(ASCENDING, 2);//already sorted, but using midPartition
	p.showReport();
}

int main()
{
	demo();
	//perf_all(); 
	return 0;
}
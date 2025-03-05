/*
	Hoza Violeta Maria, Group 30424
	- I have implemented 2 merging algorithms, one for merging 2 random lists and one for merging k random lists. 
	- For generating a list, I generate a random array with elements in ascending order (using FillRandomArray) and then I put these elements in the list.
	- For merging 2 lists I use the lists from C++. I generate 2 sorted lists with random elements (between 0 and 5000), then at each step I compare the elements at the front of each list, I choose the minimum between 
	these 2 and add it to the solution, then I remove it from the respective list.
	- The algorithm for merging k sorted sequences has an O(n * log(k)) complexity (log(k) from the height of the complete binary tree - the heap, n the nr of elements we need to put in the final list) and it works in the following way: 
	it generates k sorted lists with random elements and merges them using a min heap (I create a heap of lists, so at each step the minimum is at the top and I can easily extract it and insert into the solution list).
	For this algorithm I also define some structures (Node, List), so I can create a list, insert a new element in the list.
	- From the charts, we can observe that the nr of operations increases when the value of k (the nr of lists) is also increased. For example, when k is fixed the nr operations performed for k1 = 5 is smaller than the nr. of op for k2 = 10 < the nr of op for k3 = 100
	*/

#include <iostream>
#include <list>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100 
//#define N 33
//#define k 5

using namespace std;

Profiler p("mergeK"); 

//define the structure of an element from my list
typedef struct node{
	int key;
	struct node* next; 
}Node;

//define the structure of the list
typedef struct {
	Node* first, *last;//I always have access at the first and the last element of the list
}List; 

Node* createNode(int key)//creates a new node 
{
	Node* el = (Node*)malloc(sizeof(Node));
	el->next = NULL;
	el->key = key; 
	return el;
}

void insertLast(List* list, int key)//inserts a new node at the end of the list
{
	Node* el = createNode(key);//create the node
	if (list->first == NULL)//if the list is empty
		list->first = list->last = el;
	else
	{
		list->last->next = el;//insert the new element
		list->last = el;//update the last element
	}
}

void createList(List* list, int array[], int size)//creates a list from a given array (knowing that the elements of the array are in ascending order, the elements of the list will also be in the same order)
{
	list->first = list->last = NULL;//initialize the list
	for (int i = 0; i < size; i++)
		insertLast(list, array[i]);
}

void printList(list<int> l)//print a list using the lists from C++
{
	while (l.empty() != 1)
	{
		cout << l.front() << " ";
		l.pop_front();
	}
	cout << endl;
}

list<int> merge2(list<int> list1, list<int> list2)//merge 2 lists 
{
	list<int> list;//the list in which I store the merged list 
	int el1 = list1.front();//the smallest element of the 1st list
	int el2 = list2.front();//the smallest element of the 2nd list

	while (list1.empty() == 0 && list2.empty() == 0)//while there are elements in both lists
	{
		//I insert the smaller value at the end of the final list
		if (el1 < el2)
		{
			list.push_back(el1);
			list1.pop_front();
			el1 = list1.front();//update the smallest element of the 1st list
		}
		else
		{
			list.push_back(el2); 
			list2.pop_front(); 
			el2 = list2.front(); //update the smallest element of the 2nd list
		}
	}
	//if one list is empty and the other one is not, then I can simply insert at the end of the list the remaining elements
	while (list1.empty() != 1)
	{
		list.push_back(el1); 
		list1.pop_front(); 
		el1 = list1.front();  
	}
	while (list2.empty() != 1)
	{
		list.push_back(el2);
		list2.pop_front();
		el2 = list2.front();
	}

	return list;
}

void demo2lists()//generate 2 sorted listed and merge them
{
	int k = 2, n = 17;//k represents the nr of lists, n the total nr of elements
	int size1 = n / k;//the size of the 1st list
	int size2 = n / k + (n % k);//the size of the 2nd list
	int array[500];

	list<int> lists[2];

	FillRandomArray(array, size1, 0, 5000, false, ASCENDING);//generate a random array and fill the list with the elements of the array
	for (int i = 0; i < size1; i++)
		lists[0].push_back(array[i]);

	cout << "The first list: ";
	printList(lists[0]);

	FillRandomArray(array, size2, 0, 5000, false, ASCENDING); 
	for (int i = 0; i < size2; i++)
		lists[1].push_back(array[i]);

	cout << "The second list: ";
	printList(lists[1]);

	list<int> l = merge2(lists[0], lists[1]);

	cout << "The list obtained after merging these 2 lists: ";
	printList(l);
	cout << endl; 
}


void heapify(Node* heap[], int size, int i, Operation* op)//the running time of heapify for a single node is of O(logn) 
{
	//sink the root in its right place in the heap 
	int smallest = i;//i is the root of the tree / subtree on which we are applying the recursive function
	int leftChild = 2 * i + 1; //because the array is indexed from 0
	int rightChild = 2 * i + 2;

	(*op).count(2);//I count the 2 comparisons

	if (leftChild < size && heap[leftChild]->key < heap[smallest]->key)//if the left child is smaller than the root
		smallest = leftChild;//update the smallest 
	if (rightChild < size && heap[rightChild]->key < heap[smallest]->key)//if the right child is larger than the smallest so far
		smallest = rightChild;
	if (smallest != i) //if the largest element is not the root (it means that the subtree doesn't respect the min heap property)
	{
		swap(heap[i], heap[smallest]); 
		(*op).count(3);//count 3 because the swap makes 3 assignments
		//heap[i].swap(heap[smallest]);
		heapify(heap, size, smallest, op);//heapify the affected subtree (in order to maintain the min heap property)
	}
}

void listPrint(List list)
{
	for (Node* i = list.first; i != NULL; i = i->next)
		cout << i->key << " ";
	cout << endl; 
}

List mergeK(List lists[], int k, Operation* op)
{
	//Operation ops = p.createOperation("mergek-operations", k);
	List sol;//the list in which I store the final result
	int size = k;//the size of the heap (we have k lists => k nodes)
	Node* heap[500];
	sol.first = sol.last = NULL;//initialize the list in which I store the final result

	for (int i = 0; i < size; i++)//make a heap of lists
	{
		heap[i] = lists[i].first;//the ith element from the heap is a pointer to the first element of the ith list
		(*op).count();
	}
	
	//building a min heap from a given array (use heapify in a bottom-up manner)
	for (int i = size / 2 - 1; i >= 0; i--)//I start from the index of the last non-leaf node (because the leaves already respect the heap property) and go up to the root
		heapify(heap, size, i, op);//heapify to restore the min heap property

	while (size > 0)//while the heap is not empty
	{
		int el = heap[0]->key;//get the smallest element
		(*op).count();
		insertLast(&sol, el);//makes 2 assignments and one comparison => I count 3 operations
		(*op).count(3);
		heap[0] = heap[0]->next;//remove the smallest element (because I already have it in the final list)
		(*op).count();
		if (heap[0] == NULL)//if there are no more elements in the list at the top of the heap
		{
			(*op).count();
			heap[0] = heap[size - 1];//move the last list in the heap at the top, in the place of the empty one which will be removed 
			size--;//decrease the size of the heap
		}
		(*op).count();
		if (size > 1)//if there is more than one list in the heap of lists
			heapify(heap, size, 0, op);//mantain the min heap property
	}
	
	return sol;
}


void generateKMergeK(int n, int k, Operation* op)//generate k random lists and merge them 
{
	//int n = N;
	//for n - even all the lists have the same nr of elements, otherwise the 1st list will contain the additional elements
	int size1 = (n / k) + (n % k);//the size of the 1st list
	int size2 = n / k;//the sizes of the rest of the lists
	int array[MAX_SIZE];
	List lists[500];//the list in which I store the result

	FillRandomArray(array, size1, 0, 5000, false, ASCENDING); 
	createList(&lists[0], array, size1);//generate the first list

	cout << "The first list is: ";
	listPrint(lists[0]);

	for (int i = 1; i < k; i++)//generate the rest of the lists
	{
		FillRandomArray(array, size2, 0, 5000, false, ASCENDING);
		createList(&lists[i], array, size2);
		cout << "The " << i + 1 << "th list is : ";
		listPrint(lists[i]);
	}

	List list = mergeK(lists, k, op);//merge the generated lists
	cout << "The list obtained after merging " << k << " lists with " << n << " elements is: ";
	listPrint(list);
}

void demo()
{
	//demo function to test the algorithms 
	Operation op = p.createOperation(" ", 0);
	demo2lists();//merge 2 lists
	generateKMergeK(31, 6, &op);//merge k lists with n elements in total
}

void perf_n()//when k has constant values and n varies
{
	int k1 = 5, k2 = 10, k3 = 100;
	for (int n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		Operation op1 = p.createOperation("k1=5", n);
		Operation op2 = p.createOperation("k2=10", n);
		Operation op3 = p.createOperation("k3=100", n);
		
		generateKMergeK(n, k1, &op1);  
		generateKMergeK(n, k2, &op2);  
		generateKMergeK(n, k3, &op3);  
	}
	p.createGroup("variableN-operations", "k1=5", "k2=10", "k3=100");
	//p.showReport();
}


void perf_k()//when n has constant value and k varies
{
	int n = MAX_SIZE;
	for (int k = 10; k <= 500; k += 10)
	{
		Operation ops = p.createOperation("variableK-operations", k);
		generateKMergeK(n, k, &ops); 
	}
	//p.showReport();
}

void perf_all()
{
	perf_n();
	perf_k(); 
	p.showReport(); 
}

int main()
{
	demo();
	//perf_all(); 
	return 0;
}


/*
void heapify(list<int> heap[], int size, int i)//the running time of heapify for a single node is of O(logn)
{
	//sink the root in its right place in the heap
	int smallest = i;//i is the root of the tree / subtree on which we are applying the recursive function
	int leftChild = 2 * i + 1; //because the array is indexed from 0
	int rightChild = 2 * i + 2;

	if (leftChild < size && heap[leftChild].front() < heap[smallest].front())//if the left child is smaller than the root
		smallest = leftChild;
	if (rightChild < size && heap[rightChild].front() < heap[smallest].front())//if the right child is larger than the smallest so far
		smallest = rightChild;
	if (smallest != i) //if the largest element is not the root (it means that the subtree doesn't respect the min heap property)
	{
		//swap(heap[i], heap[smallest]);
		heap[i].swap(heap[smallest]);
		heapify(heap, size, smallest);//heapify the affected subtree (in order to maintain the min heap property)
	}
}

void buildHeap(list<int> heap[], int size)//building a min heap from a given array
{
	//use heapify in a bottom-up manner
	for (int i = size / 2 - 1; i >= 0; i--)//I start from the index of the last non-leaf node (because the leaves already respect the heap property) and go up to the root
		heapify(heap, size, i);//heapify to restore the min heap property
}


list<int> mergeK(list<int> lists[])
{
	list<int> sol;
	int heapSize = k;//the size of the heap (we have k lists => k nodes)
	list<int> heap[100];

	for (int i = 0; i < heapSize; i++)
	{
		heap[i].push_back(lists[i].front());
		lists[i].pop_front();
	}
	buildHeap(heap, heapSize);

	while (heapSize > 0)
	{
		int el = heap[0].front();
		sol.push_back(el);
		heap[0].pop_front();
		if (heap[0].empty())
		{
			//swap(heap[0], heap[heapSize - 1]);
			heap[0].swap(heap[heapSize - 1]);
			heapSize--;
		}
		if (heapSize > 1)
			heapify(heap, heapSize, 0);
	}

	return sol;
}

void generateKMergeK()
{
	//the nr of elements from the lists is equal (if n is odd, the first list has one element in plus)
	int n = MAX_SIZE;
	int size1 = n / k + (n % k);
	int size2 = n / k;
	int array[MAX_SIZE];

	list<int> listK[k];

	FillRandomArray(array, size1, 0, 5000, false, ASCENDING);
	//cout << "The first list is: ";
	for (int i = 0; i < size1; i)
	{
		listK[0].push_back(array[i]);
		cout << array[i] << " ";
	}
	cout << endl;
	for (int i = 1; i < k; i++)
	{
		cout << "The " << i << "th list is: ";
		FillRandomArray(array, size2, 0, 5000, false, ASCENDING);
		for (int j = 0; j < size2; j++)
		{
			listK[i].push_back(array[j]);
			cout << array[i] << " ";
		}
		cout << endl;
	}

	list<int> mergedList = mergeK(listK);
	cout << "The list obtained after merging " << k << " lists with " << n << " elements is: ";
	printList(mergedList);
}
*/
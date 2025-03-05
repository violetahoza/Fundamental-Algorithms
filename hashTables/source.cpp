/*
	Hoza Violeta Maria, Group 30424

	-I have implemented some basic operations in a hash table, such as: insertion, deletion and search, using open adressing and quadratic probing + a function that analyzes
	the performance of the hash table under different fill factors. It measures the average (for 1500 elements) and maximum effort required to find and not find elements in the hash table.
	-The hash function uses quadratic probing in order to avoid clustering (beacuse of that N and the parameters C1 and C2 should be prime). It takes the id of the element and the 
	nr of attempts to find an index.
	-The search function searches an element after its id. If there exists in the table an element with the given id, the function returns its index, otherwise, if the element
	does not exist or it has been deleted, it returns the value -1. It continues searching until all the the slots of the table have been verified. It uses the hash function
	for computing the possible position of the element in the table.
	-The insert function inserts a new element in the table using the hash function for finding an appropriate index. If a position hasn't been found after searching through the
	whole table, then it means the table is full.
	-The delete function does not actually free the memory allocated for the Entry structure, it just marks the element as deleted.
	-Regarding the performance, we can observe that the fuller the table, the larger the effort. After deleting 19% of the elements, the effort is close to the effort
	for the case when we have the filling factor = 0.99.
*/
#ifdef _MSC_VER 
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <string.h>
#include "Profiler.h"

#define C1 3 //C1, C2 - parameters used in the quadratic probing hash function
#define C2 5
#define N 10007
#define deleted -2

Profiler p("hashTables");

using namespace std;

typedef struct {
	int id; 
	char name[30]; 
}Entry;


//define the hash table
typedef struct {
	Entry** elements; //an array of pointers to entry 
	int size; //the capacity of the table 
	int nrEl; //the nr of elements that exist in the table 
}HashTable;

int hashF(int id, int i)//the hash function (calculates the hash index using quadratic probing)
{
	//i is the nr of the attempt to find the index
	int index = (id + C1 * i + C2 * i * i) % N; 
	return index;
}

Entry* createItem(int id, char* name)//creates a new entry
{
	//Entry* item = (Entry*)malloc(sizeof(Entry));//allocates memory for a new element of the hash table
	Entry* item = new Entry;
	item->id = id;  
	strcpy(item->name, name); 
	return item;
}

HashTable* createTable(int size)//create the hash table
{
	//HashTable* T = (HashTable*)malloc(sizeof(HashTable)); 
	HashTable* T = new HashTable;
	T->size = size; 
	T->nrEl = 0; 
	//T->elements = (Entry**)malloc(sizeof(Entry*) * size); 
	T->elements = new Entry*[size];  
	
	for (int i = 0; i < size; i++)
	{
		T->elements[i] = NULL; // initialize each entry to NULL  
	}

	return T;
}

void insert(HashTable* Table, Entry* el)//inserting a new element in the table
{
	int i = 0, index;//i is the nr of attempts to find an index for the element
	do {
		index = hashF(el->id, i); 
		if (Table->elements[index] == NULL || Table->elements[index]->id == deleted)
		{
			//Table[index] = new Entry;  
			//Table->elements[index]->id  = el->id;   
			//strcpy(Table->elements[index]->name, el->name);   
			Table->elements[index] = createItem(el->id, el->name);
			Table->nrEl++; 
			//Table[index + 1] = NULL; 
			//return index;
			return;
		}
		else i++;
	} while (i < Table->size);//continue until the whole hash table is searched
	//return -1;//the table is full
	cout << "The table is full\n";
}

int search(HashTable* Table, int id, int* count)//search an element after its id
{
	int i = 0;//the nr of attempts to find the element
	do {
		(*count)++;
		int index = hashF(id, i);
		if (Table->elements[index] == NULL)
			return -1;//element not found
		else if (/*Table->elements[index] != NULL && */ Table->elements[index]->id != deleted && Table->elements[index]->id == id)
			return index;//I found the element
		//else if (Table->elements[index]->id == deleted)
			//return -2;
		i++;
	} while (i < Table->size /*&& Table->elements[i] != NULL*/);
	return -1; //element not found
}

void deleteEl(HashTable* Table, int id)//deleting an element from the table
{
	int x;
	int index = search(Table, id, &x);
	if (index != -1) 
	{
		delete(Table->elements[index]);
		Table->elements[index]->id = deleted;
		strcpy(Table->elements[index]->name, ""); 
		Table->nrEl--;
	}
}

void printTable(HashTable* T)
{
	for (int i = 0; i < T->size; i++)
	{
		cout << i << ": ";
		if (T->elements[i] != NULL)
		{
			//if (T->elements[i]->id != deleted)
				cout << T->elements[i]->id << " " << T->elements[i]->name;
			//else cout << T->elements[i]->id;
		}	
		cout << endl;
	}
	cout << endl; 
}

void printSearch(HashTable* T, int id)
{
	int x;
	int value = search(T, id, &x); 
	if (value == -1)
		cout << "element with id = " << id << " was not found\n";
	else
		cout << "the element with id = " << id << " is found at the position " << value  << " and it is " << T->elements[value]->name << endl;
}

void demo()
{
	HashTable* T = createTable(N);//create the table

	//insert some elements
	insert(T, createItem(1, (char*)"apple"));
	insert(T, createItem(3, (char*)"cake"));
	insert(T, createItem(4, (char*)"coffee"));
	insert(T, createItem(6, (char*)"pasta"));
	insert(T, createItem(13, (char*)"pizza"));
	insert(T, createItem(5, (char*)"chocolate"));
	insert(T, createItem(10, (char*)"bread"));
	insert(T, createItem(14, (char*)"mango"));

	printTable(T); 

	//search some elements
	printSearch(T, 1);
	printSearch(T, 5);
	printSearch(T, 2);
	printSearch(T, 3); 
	printSearch(T, 13);

	deleteEl(T, 3);//delete an element
	cout << "\nThe table after deleting the element with id = 3: \n";
	printTable(T);

	printSearch(T, 3);//search one of the deleted elements

	deleteEl(T, 1);//delete an element
	cout << "\nThe table after deleting the element with id = 1: \n";
	printTable(T);

	printSearch(T, 14);//search one of the deleted elements
}

void evaluation() 
{
	cout << "Filling\t Avg effort\tMax effort\t Avg effort\t   Max effort\n";
	cout << "factor \t   found \t found\t   \t not-found\t    not-found \n";

	float avgF = 0, avgNF = 0;//variables in which I store the average effort
	int maxF = 0, maxNF = 0;//variables in which I store the maximum number of accesses performed by one search operation 
	float alpha[5] = {0.80, 0.85, 0.90, 0.95, 0.99};//fill factor
	int array[N + 1500];//the array in which I store the elements generated random

	for (int i = 0; i < 5; i++)//make 5 tests
	{
		HashTable* T = createTable(N);//create the hash table

		int nrEl = (int)(N * alpha[i]);//how much we fill the hash table
		FillRandomArray(array, nrEl + 1500, 1, 1000000, true);//+1500 because we need to search 1500 elements that are not in the table
		//FillRandomArray(array, nrEl, 1, 1000000, true); 
		//FillRandomArray(array + nrEl, 1500, 1000000, 2000000, true);
		
		for (int j = 0; j < nrEl; j++)
		{
			insert(T, createItem(array[j], (char*)""));//insert the generated elements in the table
		}
		//printTable(T);

		int index[1500];
		FillRandomArray(index, 1500, 0, nrEl - 1, true);//generate an array of indexes for the elements that are in the table

		for (int k = 0; k < 1500; k++)//search 1500 elements that are in the table and 1500 elements that are not in the table
		{
			int cntFound = 0, cntNotFound = 0; 

			search(T, array[index[k]], &cntFound);//look for elements that are in the table
			avgF += cntFound;
			int result = search(T, array[nrEl + k], &cntNotFound);//look for elements that are not in the table; we take the index (nrEl + k) because this is not in the table (we only have nrEl elements in the table)
			if(result == -1 ) //if the result of the search is -1, than we are sure that the element is not in the table
				avgNF += cntNotFound;

			//update the max effort
			if (cntFound > maxF) 
				maxF = cntFound;
			if (cntNotFound > maxNF) 
				maxNF = cntNotFound;
		}

		//compute the average effort
		avgF = (float)(avgF / 1500);
		avgNF = (float)(avgNF / 1500);  

		cout << "\n" << alpha[i] << "   \t  " << avgF << "  \t " << maxF << "  \t  \t  " << avgNF << " \t\t" << maxNF << endl;

		avgF = 0, avgNF = 0;//reinitialize these variables with 0 for the next test
		maxF = 0, maxNF = 0;

		//the evaluation of the search operation after deletion
		//go from the fill factor of 99% to 80% => delete 19% of the elements 
		if (i == 4)//when I'm done with the first 5 tests
		{
			avgF = 0, avgNF = 0;  
		    maxF = 0, maxNF = 0; 

			//generate an array of indexes for the elements that have to be deleted
			int index2[N];
			int nrToDelete = 0.19 * N;//how many numbers we have to delete
			FillRandomArray(index2, nrToDelete + 1500, 0, N - 1, true);//+1500 that are not deleted
			
			for (int j = 0; j < nrToDelete; j++)
				deleteEl(T, array[index2[j]]);//delete the randomly selected elements

			for (int k = 0; k < 1500; k++)//search 1500 elements that are in the table and 1500 elements that are not in the table
			{
				int cntFound = 0, cntNotFound = 0; 

				search(T, array[index2[nrToDelete + k]], &cntFound);//look for elements that are in the table 
				avgF += cntFound; 
				int result = search(T, array[index2[k]], &cntNotFound);//look for elements that are not in the table (these elements have been deleted)  
				if (result == -1)
					avgNF += cntNotFound;

				//update the max effort
				if (cntFound > maxF) 
					maxF = cntFound; 
				if (cntNotFound > maxNF) 
					maxNF = cntNotFound; 
			}

			//compute the average effort
			avgF = (float)(avgF / 1500);  
			avgNF = (float)(avgNF / 1500); 

			cout << "\nAfter deleting 19% of the elements:\n";
			cout << "\n" << alpha[0] << "   \t  " << avgF << "  \t " << maxF << "  \t  \t  " << avgNF << " \t\t" << maxNF << endl;
		}

		delete(T->elements);
		delete(T);//free the memory allocated for the hash table
	}
}

int main()
{
	//for demo: give to N a smaller value
	//demo();
	evaluation();
	return 0;
}
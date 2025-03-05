/*
	Hoza Violeta Maria, Group 30424

	1. I have implemented the inorder / preorder / postorder traversal (visit the left child, node, then the right child) in a binary tree in an iterative manner (provided in the lecture 6) and in a recursive one. The complexity for both of them is 
	O(n) and they don't require additional space. Regarding the number of operations, both algorithms perform the same number of print key operations.
	- For testing the algorithms and measuring their performance, I have also implemented an algorithm that builds a binary tree from a given array. The first element of the array will be the root, 
	then the rest of the elements will be inserted recursively as the left and right children of the root.

	2. I have implemented a hybridization for quicksort using iterative insertion sort. At each recursive call I find the size of the array and if the size is smaller than the threshold value (30), then
	the insertion function is called for that portion of the array. Otherwise, the quicksort function is the one called.
	   Regarding the nr of operations, the hybrid quicksort performs more operations than the basic one. However, regarding the running time the hybrid version is more efficient than the other one.
*/

#include <iostream>
#include "Profiler.h"

Profiler p("traversal");

#define MAX_SIZE 10000 //the maximum size of the input
#define STEP_SIZE 100 
#define NR_TESTS 15 //how many times we repeat the measurements 

using namespace std;

enum traversal {
	inorder = 1,
	preorder = 2,
	postorder = 3
};

typedef struct Node {
	int value;
	Node* left, * right, * parent; //pointers to the left child, to the right child and to the parent of the node 
};

Node* createNode(int value) //creates a new node 
{
	Node* node = new Node;//allocate memory for the node
	//initialize the node
	node->value = value;
	node->left = node->right = node->parent = NULL;
	return node;
}

Node* createBinaryTree(int array[], Node* root, int index, int nrNodes) //creates a binary tree from a given array
{
	if (index >= nrNodes) //all the elements of the array are already in the tree
		return NULL; 

	//the root will be array[0], but I want it to be chosen random
	if (index == 0)
	{
		srand(time(NULL));
		int x = rand() % nrNodes;//take a random index 
		swap(array[index], array[x]); //then swap it with the first element, so I can have a random root
		root->parent = NULL;
	}

	if (index < nrNodes) //as long as there are elements in the array
	{
		Node* node = createNode(array[index]);//create a node with the current array element
		root = node;
		
		//recursively insert the left and right children
		root->left = createBinaryTree(array, root->left, 2 * index + 1 , nrNodes);
		if (root->left != NULL) //create the link between the child and its parent
			root->left->parent = root;

		root->right = createBinaryTree(array, root->right, 2 * index + 2 , nrNodes);
		if (root->right != NULL)
			root->right->parent = root;
	}

	return root;
}

void inorderRec(Node* root, Operation* op) //inorder traversal (the recursive version)
{
	if (root != NULL)
	{
		inorderRec(root->left, op); //left
		cout << root->value << " "; //node
		//(*op).count();
		inorderRec(root->right, op); //right
	}
}

void preorderRec(Node* root, Operation* op)
{
	if (root != NULL)
	{
		cout << root->value << " "; //node
		//(*op).count(); 
		preorderRec(root->left, op); //left
		preorderRec(root->right, op); //right
	}
}

void postorderRec(Node* root, Operation* op)
{
	if (root != NULL)
	{
		postorderRec(root->left, op); //left 
		postorderRec(root->right, op); //right 
		cout << root->value << " "; //node
		//(*op).count(); 
	}
}

void iterativeTraversal(Node* root, Operation* op, traversal t) //inorder traversal (iterative version)   
{
	int direction = 1; 

	//direction = 1 : top -> down
	//direction = 2 : left -> root
	//direction = 3 : right -> root

	if (root == NULL) //the tree is empty => nothing to traverse 
		return; 

	Node* currentNode = root;

	do {
		if (direction == 1) //we are at the root
		{
			if (t == preorder)
			{
				cout << currentNode->value << " "; //print in preorder 
				//(*op).count(); 
			}

			if (currentNode->left != NULL) //advance to the left with direction still 1 (it's the first time I see the node)
				currentNode = currentNode->left;
			else
				direction = 2; //set direction to 2 as I meet the node for the second time and advance to the right
		}
		if (direction == 2)
		{
			if (t == inorder)
			{
				cout << currentNode->value << " "; //print in inorder 
				//(*op).count(); 
			}

			if (currentNode->right != NULL) //advance to the right for the first time
			{
				currentNode = currentNode->right;
				direction = 1;
			}
			else
				direction = 3; //set direction to 3 as I meet the node for the 3rd time and advance to the parent
		}
		if (direction == 3)
		{
			if (t == postorder)
			{
				cout << currentNode->value << " "; //print in postorder 
				//(*op).count(); 
			}

			if (currentNode->parent != NULL) //check the direction from where we are coming from
			{
				if (currentNode == currentNode->parent->left)
					direction = 2; //otherwise it remains 3
				currentNode = currentNode->parent; //advance to the parent and continue checking
			}
		}
	} while (currentNode != root || direction != 3); //we printed all the nodes

	if (t == postorder)
	{
		cout << currentNode->value << " "; //print in postorder 
		//(*op).count();
	}
}

void printBST(Node* node, int level = 0)//pretty print the binary representation of a tree
{
	if (node == NULL)
		return;

	for (int i = 0; i < level; i++)
		cout << "   ";
	cout << node->value << endl;

	printBST(node->left, level + 1);//first go through the left branch
	printBST(node->right, level + 1);//then continue with the right branch
}

void demoTraversal()
{
	Operation op = p.createOperation("-", 0);
	//int v[] = { 12, 6, 2, 9, 1, 4, 15, 7, 9 };

	int v[9];
	FillRandomArray(v, 9, 1, 23);
	int n = sizeof(v) / sizeof(v[0]);

	Node* tree = new Node;
	tree = createBinaryTree(v, tree, 0, n);

	cout << "Binary representation : \n";
	printBST(tree);
	cout << endl;

	cout << "Inorder traversal (recursive): ";
	inorderRec(tree, &op);
	cout << endl;

	cout << "Inorder traversal (iterative): ";
	iterativeTraversal(tree, &op, inorder); 
	cout << endl;

	cout << endl;

	cout << "Preorder traversal (recursive): ";
	preorderRec(tree, &op);
	cout << endl;

	cout << "Preorder traversal (iterative): ";
	iterativeTraversal(tree, &op, preorder); 
	cout << endl;

	cout << endl;

	cout << "Postorder traversal (recursive): ";
	postorderRec(tree, &op);
	cout << endl;

	cout << "Postorder traversal (iterative): ";
	iterativeTraversal(tree, &op, postorder); 
	cout << endl;

	cout << endl;
}

void perfTraversal()
{
	int v[MAX_SIZE]; 
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)//vary the size of the input array
		for (int test = 0; test < NR_TESTS; test++) //I repeat the measurements NR_TESTS times
		{
			Operation op1 = p.createOperation("inorder-recursive", n);
			Operation op2 = p.createOperation("inorder-iterative", n);
			Operation op3 = p.createOperation("preorder-recursive", n);
			Operation op4 = p.createOperation("preorder-iterative", n);
			Operation op5 = p.createOperation("postorder-recursive", n);
			Operation op6 = p.createOperation("postorder-iterative", n);

			FillRandomArray(v, n, 1, 10000);
			Node* tree = new Node;
			tree = createBinaryTree(v, tree, 0, n);
			//Node* tree2 = new Node;
			//tree2 = createBinaryTree(v, tree2, 0, n);

			inorderRec(tree, &op1);
			iterativeTraversal(tree, &op2, inorder); 

			preorderRec(tree, &op3);
			iterativeTraversal(tree, &op4, preorder); 

			postorderRec(tree, &op5);
			iterativeTraversal(tree, &op6, postorder);
		}

	p.divideValues("inorder-recursive", NR_TESTS);
	p.divideValues("inorder-iterative", NR_TESTS);
	p.createGroup("inorder-traversal", "inorder-recursive", "inorder-iterative");


	p.divideValues("preorder-recursive", NR_TESTS);
	p.divideValues("preorder-iterative", NR_TESTS);
	p.createGroup("preorder-traversal", "preorder-recursive", "preorder-iterative");

	p.divideValues("postorder-recursive", NR_TESTS);
	p.divideValues("postorder-iterative", NR_TESTS);
	p.createGroup("postorder-traversal", "postorder-recursive", "postorder-iterative");

	p.showReport();
}

int partition(int* array, int size, int left, int right, Operation* op)//Lomuto partition; O(n)
{
	//this function takes the last element as pivot and places all the elements smaller than the pivot to its left, while the elemnts greater than the pivot are placed to its right
	//Operation opComp = p.createOperation("quickSort-comp", size);
	//Operation opAssig = p.createOperation("quickSort-assig", size);

	int i = left - 1;//index of the smaller element
	for (int j = left; j <= right; j++)
	{
		//opComp.count();
		//(*op).count();
		if (array[j] < array[right])//if the current element is smaller than the pivot
		{
			i = i + 1;//increase the index of the smaller element
			swap(array[i], array[j]);
			//opAssig.count(3);
			(*op).count(3);
		}
	}
	//opAssig.count(3);
	//(*op).count(3);
	swap(array[i + 1], array[right]);
	return (i + 1);
}

void quickSort(int* array, int size, int left, int right)//Divide et Impera (Divide and Conquer) algorithm; not stable; not optimal
{
	Operation op = p.createOperation("quickSort", size);

	if (left < right)//if there are at least 2 elements
	{
		int p;
		//if (type == 0)//for average case
			//p = partitionRandom(array, size, left, right); //p is the pivot; array[p] is now at the right place
		//else if (type == 1)//for worst case
		p = partition(array, size, left, right, &op);
		//else p = midPartition(array, size, left, right); //for best case
		quickSort(array, size, left, p - 1);//sort the elements before the pivot
		quickSort(array, size, p + 1, right);//sort the elements after the pivot
	}
}


void insertionSort(int* array, int left, int n, Operation* op)//stable:)
{
	//easy take - take the 1st element from the unsorted part and look for its position in the ordered part
	//Operation opComp = p.createOperation("insertion-comp", n);
	//Operation opAssig = p.createOperation("insertion-assig", n);

	for (int i = left + 1; i < n; i++)//start from the second element (the array is indexed from 0)
	{
		int buffer = array[i];//I make an assignment,
		//opAssig.count();//so I count it
		//(*op).count();
		int j = i - 1;
		while (j >= left && buffer < array[j])//the operator '<' ensures the stability of the algorithm, the order of the equal elements in the sorted array being the same as in  the initial array
		{
			//opComp.count();
			//(*op).count();
			//the elements greater than the current element are shifted with one position to the right
			array[j + 1] = array[j];
			//(*op).count();
			//opAssig.count();
			j--;
		}
		//opComp.count();//if the result of the comparison is false, we don't enter the while, but the comparison was evaluated so we have to count it
		//(*op).count();
		if (i - 1 != j) //if the condition is false, it means that the current element was already in its correct position
		{
			array[j + 1] = buffer;
			//(*op).count();
			//opAssig.count();
		}
	}
}

void hybrid_quickSort(int* array, int size, int left, int right, int threshold, Operation* op)
{
	if (left < right) //as long as there are elements
	{
		if (right - left + 1 < threshold) //if the size of the array is smaller than the threshold use insertion sort and stop the recursion
		{
			insertionSort(array, left, right + 1, op);
			//break;
		}
		else //otherwise continue with the quicksort
		{
			int pivot = partition(array, size, left, right, op);

			//if (pivot - left < right - pivot)
			//{
				hybrid_quickSort(array, size, left, pivot - 1, threshold, op);
				//left = pivot + 1;
			//}
			//else
			//{
				hybrid_quickSort(array, size, pivot + 1, right, threshold, op);
				//right = pivot - 1;
			//}
		}
	}
}

void demo_quickSort()
{
	//int v[] = { 12, 67, 9, 2, 7, 1, 33, 78, 8, 9, 12, 43, 13, 6, 2, 5, 4, 10, 3, 13, 15 };
	//int u[] = { 12, 67, 9, 2, 7, 1, 33, 78, 8, 9, 12, 43, 13, 6, 2, 5, 4, 10, 3, 13, 15 };

	int v[50], u[50];
	FillRandomArray(v, 50, 1, 100);
	memcpy(u, v, sizeof(v)); 

	int n = sizeof(v) / sizeof(v[0]);
	Operation op = p.createOperation("-", 0);

	cout << "The sorted array using the basic quicksort: ";
	quickSort(v, n, 0, n - 1);
	for (int i = 0; i < n; i++)
		cout << v[i] << " ";
	cout << endl;

	cout << "\nThe sorted array using the hybrid quicksort: ";
	hybrid_quickSort(u, n, 0, n - 1, 30, &op);
	for (int i = 0; i < n; i++)
		cout << u[i] << " ";
	cout << endl;
}

void perf_op()
{
	int v[MAX_SIZE], u[MAX_SIZE];

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)//vary the size of the input array
		for (int test = 0; test < NR_TESTS; test++) //I repeat the measurements NR_TESTS times
		{
			Operation op = p.createOperation("hybridQuickSort", n);
			//Operation op2 = p.createOperation("quickSort", n);
			FillRandomArray(v, n, 1, 9999);
			memcpy(u, v, sizeof(v));// I make a copy of the generated array, so I don't sort an already sorted array 
			quickSort(v, n, 0, n - 1);
			hybrid_quickSort(u, n, 0, n - 1, 30, &op);
		}

	p.divideValues("quickSort", NR_TESTS);
	p.divideValues("hybridQuickSort", NR_TESTS);
	p.createGroup("operations", "hybridQuickSort", "quickSort");

	p.showReport();
}


void perf_time()
{
	int v[MAX_SIZE], u[MAX_SIZE];
	Operation op = p.createOperation("-", 0);

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)//vary the size of the input array
	{
		FillRandomArray(v, n, 1, 9999);
		//memcpy(u, v, n);

		p.startTimer("quickSort", n);
		for (int test = 0; test < 200; test++)
		{
			memcpy(u, v, n);//making a copy because I don't want to sort an array already sorted
			quickSort(u, n, 0, n - 1);
		}
		p.stopTimer("quickSort", n);

		p.startTimer("hybridQuickSort", n);
		for (int test = 0; test < 200; test++)
		{
			memcpy(u, v, n);
			hybrid_quickSort(u, n, 0, n - 1, 30, &op);
		}
		p.stopTimer("hybridQuickSort", n);
	}

	p.divideValues("quickSort", 200);
	p.divideValues("hybridQuickSort", 200);
	p.createGroup("TimeComparison", "hybridQuickSort", "quickSort");
	p.showReport();
}

void thresholdTime()
{
	int v[MAX_SIZE];
	FillRandomArray(v, MAX_SIZE, 1, 9999, false, UNSORTED);
	Operation op = p.createOperation("-", 0);

	for (int threshold = 1; threshold <= 100; threshold++)//vary the value of the threshold
		for(int test = 0; test < 15; test++)
		{
			int** matrix = (int**)malloc(300 * sizeof(int*)); //make a matrix with 300 rows 
			for (int i = 0; i < 300; i++)
			{
				matrix[i] = (int*)malloc(10000 * sizeof(int)); //10000 columns
				memcpy(matrix[i], v, sizeof(int) * 10000); //copy the generated vector on each row of the matrix
			}

			p.startTimer("threshold", threshold);
			for (int test1 = 0; test1 < 300; test1++)
			{
				hybrid_quickSort(matrix[test1], 10000, 0, 10000 - 1, threshold, &op);
			}
			p.stopTimer("threshold", threshold);
		}
	
	p.divideValues("threshold", 15);
	p.showReport();
}


int main()
{
	demoTraversal();
	demo_quickSort();
	
	//perfTraversal();
	//perf_op();
	//perf_time();
	//thresholdTime();
	
	return 0;
}

//void inorderIterative(Node* root, Operation* op) //inorder traversal (iterative version) 
//{
//	if (root == NULL) //the tree is empty => nothing to traverse
//		return; 
//
//	Node* currentNode = root; //start from the root
//
//	//left - node - right
//	while (currentNode != NULL) 
//	{
//		if (currentNode->left == NULL) //if the current node doesn't have a left subtree
//		{
//			cout << currentNode->value << " ";//print the value of the node
//			(*op).count();
//			currentNode = currentNode->right;//then go to the right subtree
//		}
//		else //if there is a left subtree
//		{
//			Node* predecessor = currentNode->left;
//			//find the inorder predecessor of the current node (the rightmost node in the left subtree) or the node whose right child is the current node
//			while (predecessor->right && predecessor->right != currentNode)
//				predecessor = predecessor->right;
//
//			if (predecessor->right == currentNode)  
//			{
//				predecessor->right = NULL; //update the predecessor's right child as NULL 
//				cout << currentNode->value << " "; //print the value of the current node 
//				(*op).count(); 
//				currentNode = currentNode->right; //go to the right child of the current node  
//			}
//			else if(predecessor->right == NULL)//the inorder predecessor's right child points to the current node => the left subtree has been visited
//			{
//				predecessor->right = currentNode; //make the current node the right child of the rightmost node from the left subtree 
//				currentNode = currentNode->left; //go to the left child of the current node 
//			}
//		}
//	}
//}
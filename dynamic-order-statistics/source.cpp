/*
	Hoza Violeta Maria, Group 30424
	- I have implemented the management operations of an ordered statistics tree: buildTree, OS_Select and OS_Delete, using an augmented BST structure.
	- For building the tree I use a divide and conquer strategy. I find the middle element from the given range, then I make it the root of the subtree I'm dealing with. I continue recursively
	with the left branch and the right one.
	- The OS_Select function selects the element with the i-th smallest key.
	- For deleting a node, I look for it using the OS_Select function, then delete the node like in a BST. After deleting a node, I don't need to rebalance the tree because the balance property
	is maintained during the deletion process (because I replace the deleted node with the smallest node from the nodes in the right subtree).
	- In the function demo, I build a tree with n elements (with n a small number), I pretty print it, then I search for some random nodes and delete them.
*/

#include <iostream>
#include <cstdlib> 
#include "Profiler.h"

Profiler p("dynamicOS");

#define MAX_SIZE 10000 //the maximum size of the input
#define STEP_SIZE 100 
#define NR_TESTS 5 //how many times we repeat the measurements

using namespace std;

typedef struct Node {
	int value;
	int size;//the size of the sub-tree rooted at the node
	Node* left, * right, * parent; //pointers to the left child, to the right child and to the parent of the node
};

Node* divideEtImpera(int left, int right, Node* parent, Operation* op)
{
	if (left > right)//the range is not valid
		return NULL;

	//Node* currentNode = (Node*)malloc(sizeof(Node));//create the node
	Node* currentNode = new Node;

	//initialize the node
	int middle = (left + right) / 2;//find the middle element in the given range
	currentNode->value = middle;
	currentNode->size = right - left + 1;//compute the size of the node
	(*op).count();//count the assignment
	currentNode->parent = parent;

	currentNode->left = divideEtImpera(left, middle - 1, currentNode, op); //continue recursively on the left half of the current range with the elements smaller than the current node
	currentNode->right = divideEtImpera(middle + 1, right, currentNode, op);  //continue recursively on the right half of the current range with the elements greater than the current node
	//middle - 1 and middle + 1 because the middle element is already in its right place

	return currentNode;
}

Node* buildTree(int n)//builds a balanced BST containing the keys 1,2,...n
{
	//Node* tree = (Node*)malloc(sizeof(Node));
	Operation op = p.createOperation("buildTree", n);
	return divideEtImpera(1, n, NULL, &op); //the parent of the root is NULL  
	//return tree;
}

/*
void inorder(Node* root) //inorder traversal
{
	if (root != NULL)
	{
		inorder(root->left);
		cout << "Value: " << root->value << ", Size: " << root->size << endl;
		inorder(root->right);
	}
}
*/

void printBST(Node* node, int level = 0)//pretty print the binary representation of a tree
{
	if (node == NULL)
		return;

	for (int i = 0; i < level; i++)
		cout << "\t";
	cout << node->value << "/" << node->size << endl;

	printBST(node->left, level + 1);//first go through the left branch
	printBST(node->right, level + 1);//then continue with the right branch

}

Node* OS_Select(Node* root, int i, Operation* op)//selects the element with the i-th smallest key
{
	//int n = root->size;
	//Operation op = p.createOperation("select", n);

	if (root == NULL) //no elements in the tree
		return NULL;

	int rank = 1;//the number of nodes preceding the root in an inorder tree walk, plus 1 for the root itself
	if (root->left != NULL)
	{
		rank = root->left->size + 1;//we know that the elements on the left of the current node are smaller than it (they precede it in an inorder tree walk)
		(*op).count();
	}

	if (i == rank) //the current node has the desired rank => we found the wanted element
	{
		//if(rank != 1)
			//(*op).count();
		return root;
	}
	else if (i < rank) //we look for the node in the left subtree
	{
		//if (rank != 1)
			//(*op).count();
		return OS_Select(root->left, i, op);
	}
	else //we look for the node in the right subtree
	{
		//if (rank != 1)
			//(*op).count();
		return OS_Select(root->right, i - rank, op); // the i-th smallest element in the right subtree corresponds to the (i - rank)-th smallest element in the entire tree
	}
}

//int getSize(Node* node)//returns the size of the given node
//{
//	if (node != NULL)
//	{
//		if (node->left == NULL && node->right == NULL)
//			return 1;
//		else
//			return (1 + getSize(node->left) + getSize(node->right));
//	}
//	else
//		return 0;
//}

void updateSize(Node* node, Operation* op)//update the size of the nodes from the current node up to the root
{
	while (node != NULL)
	{
		node->size--;// -1 because we delete only one node at a time
		(*op).count();//count an assignment
		node = node->parent;
	}
}

int getSize(Node* node, Operation* op)//returns the size of the given node
{
	if (node == NULL)
		return 0;

	int leftSize = getSize(node->left, op);
	int rightSize = getSize(node->right, op);

	/*int leftSize = 0;
	if (node->left)
		leftSize = node->left->size;

	int rightSize = 0;
	if (node->right)
		leftSize = node->right->size;*/

	node->size = 1 + leftSize + rightSize;
	(*op).count();

	return node->size;
}

Node* OS_Delete(Node* root, int i, Operation* op)
{
	if (root == NULL)//the tree is empty
		return NULL;

	if (i < root->value) //if the node to be deleted is smaller than the root we look for it in the left subtree 
		root->left = OS_Delete(root->left, i, op);
	else if (i > root->value)//if the node to be deleted is greater than the root we look for it in the right subtree 
		root->right = OS_Delete(root->right, i, op);
	else if (i == root->value) //we found the node that we need to delete
	{
		//deleting a leaf
		if (root->left == NULL && root->right == NULL)
		{
			if (root->parent != NULL)
			{
				updateSize(root->parent, op); //update the size of the nodes affected by the deletion  

				if (root->parent->left == root)
					root->parent->left = NULL;
				else if (root->parent->right == root)
					root->parent->right = NULL;
			}
			delete root;
			return NULL;
		}

		//delete a node with  a single child
		if (root->left == NULL)//if it doesn't have a left child
		{
			Node* aux = root->right;//we replace the node with its right child
			if (aux != NULL)//update the links between the node and its parent
			{
				aux->parent = root->parent;
				if (aux->parent != NULL)
				{
					if (root->parent->left == root)
						root->parent->left = aux;
					else
						root->parent->right = aux;
					updateSize(aux->parent, op);//update the size of the nodes affected by the deletion  
				}
			}
			delete root;
			return aux;
		}
		else if (root->right == NULL)//if it doesn't have a right child
		{
			Node* aux = root->left;//we replace the node with its leftt child
			if (aux != NULL)
			{
				aux->parent = root->parent;
				if (aux->parent != NULL)
				{
					if (root->parent->left == root)
						root->parent->left = aux;
					else
						root->parent->right = aux;
				}
				updateSize(aux->parent, op);//update the size of the nodes affected by the deletion  
			}
			delete root;
			return aux;
		}
		else
		{
			//both children exist
			//find the succesor (the smallest (leftmost) node from the right sub-tree)
			Node* succesor = root->right;
			while (succesor->left != NULL)
			{
				succesor = succesor->left;
			}
			//replace the link between the succesor and its parent with a link between the parent and the succesor's right child
			if (succesor->parent != root)
			{
				succesor->parent->left = succesor->right;
				//succesor->right->parent = succesor->parent;
			}
			else
			{
				succesor->parent->right = succesor->right;
				//succesor->right->parent = succesor->parent;  
			}
			root->value = succesor->value;//update the node with the value of the succesor
			updateSize(succesor->parent, op);//update the size of the nodes affected by the deletion  
			root->right = OS_Delete(root->right, succesor->value, op);//delete the succesor
		}
	}

	//update the size
	//if(root != NULL) 
		//root->size = getSize(root, op); 

	/*if (root != NULL)//recalculate the size of the current node
	{
		int leftSize = 0, rightSize = 0;

		if (root->right)
			rightSize = root->right->size;
		if (root->left)
			leftSize = root->left->size;

		root->size = 1 + leftSize + rightSize;
		(*op).count();
	}*/

	return root;
}

void demo()
{
	Operation op = p.createOperation("-", 0);
	int n = 11;
	Node* tree = buildTree(n);//create a tree 

	//inorder(tree);
	//cout << endl;
	cout << "Binary representation:\n";
	printBST(tree);//pretty print
	cout << endl;

	/*
	cout << getSize(tree) << " ";
	cout << getSize(tree->left) << " ";
	cout << getSize(tree->right) << " ";
	cout << getSize(tree->left->right) << " ";
	cout << getSize(tree->left->right->left) << " ";
	cout << getSize(tree->left->right->right) << " ";
	cout << endl;
	*/


	/*OS_Delete(tree, 3, &op);
	printBST(tree);
	OS_Delete(tree, 2, &op);
	printBST(tree);
	OS_Delete(tree, 5, &op);
	printBST(tree)*/;


	int nr = 5, cnt = 0;
	for (int i = 1; i <= nr; i++)
	{
		srand(time(NULL));
		int j = 1 + (rand() % (n - cnt));
		Node* node = OS_Select(tree, j, &op);
		if (node != NULL)
		{
			cout << "\nThe element with the " << j << "-th smallest key has the value: " << node->value << " and the size: " << node->size << endl;
			//cout << "The tree after deleting the node with the key " << node->value << ": \n";
			cout << "The tree after deleting the node with the " << j << "-th smallest key: \n";
			OS_Delete(tree, node->value, &op);
			//tree = OS_Delete(tree, node->value, &op);
			cnt++;//how many numbers I have deleted
			printBST(tree);
			cout << endl;
		}
		else
			cout << "\nThe element with the " << j << "-th smallest key was not found, so it couldn't be deleted\n";
	}
}

void perf()
{
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		Operation op1 = p.createOperation("delete", n);
		Operation op2 = p.createOperation("select", n);
		for (int test = 0; test < NR_TESTS; test++)
		{
			Node* tree = buildTree(n);
			int size = n;
			for (int i = 0; i < size; i++)
			{
				int j = 1 + (rand() % (size));
				Node* node = OS_Select(tree, j, &op2);
				OS_Delete(tree, node->value, &op1);
				size--;
			}
		}
	}

	p.divideValues("buildTree", NR_TESTS);
	p.divideValues("delete", NR_TESTS);
	p.divideValues("select", NR_TESTS);
	p.createGroup("operations", "buildTree", "select");
	p.createGroup("operations", "buildTree", "select", "delete");
	p.showReport();
}

int main()
{
	demo();
	//perf();
	return 0;
}


//Node* BST_Delete(Node* node, Node* root, Operation* op)  
//{
//	if (root == NULL) //the tree is empty
//		return NULL; 
//
//	if (node == NULL) //the node to be deleted is not in the tree, so we don't have to delete anything
//		return root;
//
//	bool isRoot = false; //use this to know if the node that has to be deleted is the root
//	if (node == root)
//		isRoot = true;
//
//	//deleting a leaf
//	if (node->left == NULL && node->right == NULL)
//	{
//		//Node* parent = node->parent;
//		if (node->parent->left == node) 
//			node->parent->left = NULL;    
//		else if(node->parent->right == node)
//			node->parent->right = NULL; 
//		updateSize(node->parent, op);   
//
//		//node->value = 0;
//		node = NULL;
//		//free(node);
//		delete node;
//
//		if (isRoot)
//			return NULL;
//		return root;
//	}
//		
//	//node with only one child
//	if (node->left == NULL) //if it doesn't have a left child
//	{
//		Node* aux = node->right; //we replace the node with its right child
//		if (aux != NULL)//update the links between the node and its parent
//		{
//			aux->parent = node->parent; 
//			if (node->parent->left == node)
//				node->parent->left = aux;
//			else
//				node->parent->right = aux; 
//		}
//		//free(node);
//		delete node;
//		//if (!node)
//			//cout << "deleted\n";
//		updateSize(aux->parent, op); //update the size of the nodes affected by the deletion  
//		if (isRoot) //if the deleted node was the root
//		{
//			aux->parent = NULL;
//			return aux;//return the new root
//		}
//		//node = node->right; 
//		//updateSize(node); 
//		//free(node->right); 
//		//if (isRoot) 
//			//return node; 
//		return root;  
//	}
//	else if (node->right == NULL) //if it doesn't have a right child
//	{
//		Node* aux = node->left; //we replace the node with its left child
//		if (aux != NULL)
//		{
//			aux->parent = node->parent;  
//			if (node->parent->left == node) 
//				node->parent->left = aux; 
//			else
//				node->parent->right = aux; 
//		}
//		//free(node);  
//		delete node;
//		updateSize(aux->parent, op); 
//		if (isRoot) //if the deleted node was the root
//		{	
//			aux->parent = NULL;
//			return aux;//return the new root
//		}
//		//node = node->left;
//		//updateSize(node);
//		//free(node->left);
//		//if (isRoot)
//			//return node; 
//		return root;
//	}
//	else
//	{
//		//both children exist
//		//find the succesor (the smallest (leftmost) node from the right sub-tree)
//		Node* succesor = node->right;
//		while (succesor->left != NULL)
//		{
//			succesor = succesor->left;
//		}
//		//replace the link between the succesor and its parent with a link between the parent and the succesor's right child
//		if (succesor->parent != node)
//		{
//			succesor->parent->left = succesor->right;
//			//succesor->right->parent = succesor->parent;
//		}
//		else
//		{
//			succesor->parent->right = succesor->right;
//			//succesor->right->parent = succesor->parent;  
//		}
//		node->value = succesor->value;//update the node with the value of the succesor
//		if (isRoot)
//			node->parent = NULL; 
//		updateSize(succesor->parent, op);
//		//delete the succesor
//		succesor = NULL;
//		//free(succesor);
//		delete succesor;
//		if (isRoot)//if the deleted node was the root
//		{
//			//node->parent = NULL;
//			return node;//return the new root
//		}
//		return root;
//	}
//}
//
//
//Node* OSDelete(Node* tree, int i, Operation* op)
//{
//	//int n = tree->size;
//
//	Node* node = OS_Select(tree, i, op);//first I find the element 
//
//	/*
//	Node* node = NULL;
//	Node* root = tree;
//
//	//find the element to delete
//	while (root)
//	{
//		if (i < root->value)
//			root = root->left;
//		else if (i > root->value)
//			root = root->right;
//		else
//		{
//			node = root;
//			break;
//		}
//	}*/
//	
//	tree = BST_Delete(node, tree, op); //then delete it like in a BST tree
//	//if (tree != NULL) 
//	//{
//		//tree->parent = NULL; // Update the root's parent to NULL 
//	//} 
//
//
//	return tree;
//}
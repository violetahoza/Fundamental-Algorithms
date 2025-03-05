/*
	Hoza Violeta Maria, Group 30424
	- I have implemented 3 representation for a multi-way tree and a Pretty Print for each of them.
	- First I have the parent representation. This is a simple vector, in which each element is the parent of its corresponding index node (considering the indexes start from 1).
    - For the multi-way tree representation I use a structure in which I store the key of the node and a vector with its children.
	- For the 1st transformation (from the parent representation to the multi-way tree representation), I create an array of R2 nodes and initializes them based on the parent representation.
	After that, I go through the parent vector, find the root, and for each node that is a parent I add its child to its vector of children. I return the resulted tree.
	  This transformation has O(n) complexity, because it iterates through each element of the parent representation only once.
	  Additional space is required for the vector of children of each node. The space complexity is O(n) for the array of pointers.
	- For the binary representation I use a structure in which I store the key a of the node, a pointer to its first child and a pointer to its right sibling.
	- For the 2nd transformation (from the multi - way tree representation to the binary representation), start with the root of the tree represented as a
	multi-way tree and create recursively the left branch, then the right branch. The maximum depth of the recursion is the height of the multi-way tree.
	  This function traverses recursively each node in the multi-way tree only once, so it has O(n) complexity.
	  Additional space is required for each level of recursion. The space complexity is O(h).
*/

#include <iostream>
#include <vector>

using namespace std;

typedef struct R2 {
	int value;
	//int nrChildren;
	//R2** children;
	vector<R2*> children; //a vector of child nodes
};

typedef struct R3 {
	int value;
	R3* firstChild;//pointer to the first child
	R3* nextSibling;//pointer to the right children
};

void printR1(int parent[], int nrNodes, int root, int level = 0)//pretty print for the parent representation
{
	for(int i = 0 ; i < nrNodes; i++)
		if (parent[i] == root)
		{
			for (int j = 0; j < level; j++)
				cout << "  ";
			cout << i + 1 << endl;//i + 1 because the array is indexed from 0; for example, the first element in the parent representation is the father of 1, not 0
			printR1(parent, nrNodes, i + 1, level + 1);//continue with the nodes at the next level
		}
}

//R2* addChild(R2* node[], int p[], int i)

R2* createNode(int value, int nrChildren)//create a new R2* node and allocate memory for it
{
	//R2* node = (R2*)malloc(sizeof(R2));
	R2* node = new R2; 
	node->value = value;
	node->children = vector<R2*>();  // Initialize the children vector 
	//node->nrChildren = nrChildren;
	//node->children = NULL; 
	return node;
}

R2* transform1(int R1[], int nrNodes)//transformation from the parent representation to the multi-way tree representation
{
	//R2** node = (R2**)malloc(nrNodes * sizeof(R2*));
	//R2* root = (R2*)malloc(sizeof(R2));

	R2** node = new R2*[nrNodes];

	for (int i = 0; i < nrNodes; i++)//create and initialize the nodes of the tree
		node[i] = createNode(i + 1, 0);// i + 1 because the array containing the parent representation is indexed from 0, but I don't actually have the node 0;
										//(the first element is the parent of the node 1, not node 0)

	//for (int i = 0; i < nrNodes; i++)
		//cout << node[i]->value << " ";

	R2* root = NULL;//initialize the root
	
	/*
	for (int i = 0; i < nrNodes; i++)
		if (R1[i] != -1)
			node[R1[i] - 1]->nrChildren++;//count the nr of children of every node
		//else root = node[i];
	
	for (int i = 0; i < nrNodes; i++)
		if(node[i]->nrChildren != 0)
		{
			node[i]->children = (R2**)malloc(sizeof(R2*) * node[i]->nrChildren);
			node[i]->nrChildren = 0;
		}
	*/

	for (int i = 0; i < nrNodes; i++)//go through the parent representation vector
	{
		if (R1[i] == -1)//if the element is the root
			root = node[i];//we update the root
		else
		{
			//add child
			int parent = R1[i] - 1;
			node[parent]->children.push_back(node[i]);//add the child to the vector of children
			//node[parent]->children[node[parent]->nrChildren] = node[i]; 
			//node[parent]->nrChildren++;
			//cout << node[parent]->nrChildren << " ";
		}
	}

	//cout<<root->nrChildren;

	return root;

	for (int i = 0; i < nrNodes; i++)
		delete node[i];
	delete[] node;
}

void printR2(R2* node, int nrNodes, int level = 0)//pretty print for the multi-way tree representation
{
	if (node != NULL)
	{
		for (int i = 0; i < level; i++)
			cout << "  ";
		cout << node->value << endl;
		//if (node->children.size() != 0)
		//{
			for (int i = 0; i < node->children.size(); i++)//recursively call the function for each child of the current node
				printR2(node->children[i], nrNodes, level + 1);//the children of the current node are situated at the next level
		//}
		/*else
		{
			printR2(node + 1, level);
			for (int i = 0; i < level + 1; i++)
				cout << "  ";
			//cout << endl;
		}*/
	}
	//cout << endl;
}

R3* transform2(R2* root)//transformation from the multi - way tree representation to the binary representation
{
	if (root == NULL)
		return NULL;

	//R3* node = (R3*)malloc(sizeof(R3));
	R3* node = new R3;//create the node
	
	//initialize the node
	node->value = root->value;  
	node->firstChild = NULL; 
	node->nextSibling = NULL; 

	//R3* current_child = (R3*)malloc(sizeof(R3));
	R3* current_child = NULL;  

	if (root->children.size() != 0)//if the current node has children
	{
		node->firstChild = transform2(root->children[0]);//complete the left branch; add the first child of the current node
		current_child = node->firstChild;  
		for (int i = 1; i < root->children.size(); i++)//complete the right branch; continue with the right siblings
		{
			current_child->nextSibling = transform2(root->children[i]);    
			current_child = current_child->nextSibling; 
		}
	}

	//delete current_child; 

	return node;
}


void printR3(R3* node, int level = 0)//pretty print for the binary representation
{
	if (node == NULL)
		return;

	for (int i = 0; i < level; i++)
		cout << "  ";
	cout << node->value << endl;

	printR3(node->firstChild, level + 1);//first go through the left branch
	printR3(node->nextSibling, level);//then continue with the right branch

	//cout << endl;
}

int main()
{
	int p[] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };//representation 1 (parent representation)
	int n = sizeof(p) / sizeof(p[0]);

	cout << "Parent representation:\n";
	printR1(p, n, -1, 0);
	cout << endl;

	R2* tree1 = transform1(p, n);
	cout << "Multi-way tree representation:\n";
	
	//cout<< tree1->value;
	//cout << tree1->nrChildren;
	
	/*
	for (int i = 0; i < tree1->nrChildren; i++) {
	
		cout << tree1->children[i]->value;
		if (tree1->children[i]->nrChildren != 0) {

			for (int j = 0; j < tree1->children[i]->nrChildren; j++) {
				cout << tree1->children[i]->children[j]->value;
			}
		}
	}*/
	
	printR2(tree1, n);
	cout << endl;

	R3* tree2 = transform2(tree1); 
	cout << "Binary representation:\n"; 
	printR3(tree2);  
	cout << endl;

	return 0;
}
/*
	Hoza Violeta Maria, Group 30424
	- DFS: - start from a vertex, print it, and then recursively call DFS for its adjacent nodes
		   - O(V + E)
	- Topological sort: - DFS with an extra stack. Start with a vertex, then recursively call topological sort for its adjacent nodes, then push it to the stack. Print the contents of the stack.
	                    A node is pushed to the stack only when all its adjacent nodes are already on the stack. O(V + E)
	                    - not possible if the graph is not a DAG; in a valid topological ordering, every vertex comes before its successors, and the ordering respects the direction of the edges
    - Tarjan: a linear-time algorithm for finding strongly connected components            
*/

#include <iostream>
#include <vector>
#include <stack>
#include "Profiler.h"

using namespace std;

Profiler p("dfs");

enum Color {
	white = 0,//unvisited
	gray, //visited
	black //processed
};

typedef struct Node {
	int val;
	vector<Node*> adj; //adjacency list
	Color color;
	int index; //the order of the discovered node
	int lowLink; //the smallest (lowest) index of any node reachable from the current node
};

typedef struct Graph {
	int n; //the nr of nodes
	Node** nodes;
};

Node* generateNode(int i) //create a node
{
	Node* node = (Node*)calloc(1, sizeof(Node)); //allocate memory for a single node
	//Node* node = new Node;

	//initialize the node
	node->color = white;
	node->val = i;

	return node;
}

Graph* createGraph(int n) //create a graph with n vertices
{
	//Graph* graph = new Graph;
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->n = n;

	graph->nodes = (Node**)malloc(n * sizeof(Node*));

	for (int i = 0; i < n; i++)
		graph->nodes[i] = generateNode(i);

	return graph;
}

void addEdge(Graph* graph, int src, int dest) //function to add an edge to the graph (edhe from src to dest)
{
	graph->nodes[src]->adj.push_back(graph->nodes[dest]);
}


void printGraph(Graph* graph)
{
	for (int i = 0; i < graph->n; i++)
	{
		cout << graph->nodes[i]->val << ": ";
		for (int j = 0; j < graph->nodes[i]->adj.size(); j++)
			cout << graph->nodes[i]->adj[j]->val << " ";
		cout << endl;
	}
	cout << endl;
}

void DFS_visit(Node* node, bool print = false, Operation* op = nullptr, int level = 0)
{
	node->color = gray; //mark the node as visited
	if (op != nullptr)
		(*op).count(); //count the assignment

	if (print == true) //print the node
	{
		for (int i = 0; i < level; i++) 
			cout << "   ";
		cout << node->val << endl;
	}

	for (int i = 0; i < node->adj.size(); i++) //iterate through the nodes adjacent with the current node
	{
		if (op != nullptr)
			(*op).count(); //count the comparison
		if (node->adj[i]->color == white) 
			DFS_visit(node->adj[i], print, op, level + 1);
	}

	node->color = black;
	if (op != nullptr)
		(*op).count(); //count the assignment
	return;
}

void DFS_Init(Graph* graph, bool print = false, Operation* op = nullptr)
{
	//DFS(graph->nodes[start], print, op);

	for (int i = 0; i < graph->n; i++)
	{
		if (op != nullptr)
			(*op).count(); //count the comparison
		if (graph->nodes[i]->color == white)
			DFS_visit(graph->nodes[i], print, op);
	}
}

void topoSortUtil(Node* node, stack<Node*>& stk)
{
	node->color = gray; //mark the node as visited

	for (int i = 0; i < node->adj.size(); i++) //iterate through its adjacent nodes
		if (node->adj[i]->color == white) //if the adjacent node is not visited
			topoSortUtil(node->adj[i], stk); //recursively call the topological sort on the node

	node->color = black; //mark the node as processed
	stk.push(node); //push the node on the stack

	return;
}

void topologicalSort(Graph* graph)
{
	stack<Node*> stk; //create a stack to store the nodes

	for (int i = 0; i < graph->n; i++)
		if (graph->nodes[i]->color == white) //if the node is not visited
			topoSortUtil(graph->nodes[i], stk); //call the recursive function for topological sort


	while (!stk.empty()) //print the contents of the stack
	{
		cout << stk.top()->val;
		stk.pop();
		if (!stk.empty())
			cout << " -> ";
	}
	cout << endl;
}

void Tarjan(Node* node, int index, stack<Node*>& stck)
{
	node->color = gray; //mark the node as visited

	//set the index and lowLink values for the current node
	node->index = index;
	node->lowLink = index;
	index++;

	stck.push(node); //push the curreny node on the stack

	for (int i = 0; i < node->adj.size(); i++) //process the nodes adjacent with the current node
	{
		if (node->adj[i]->color == white) //if the adjacent node is unvisited
		{
			Tarjan(node->adj[i], index, stck); //recursively call Tarjan on the adjacent node
			
			if (node->lowLink > node->adj[i]->lowLink) //update the lowLink of the current node 
				node->lowLink = node->adj[i]->lowLink;
		}
		else if (node->adj[i]->color == gray) //if the node is visited 
		{
			if (node->lowLink > node->adj[i]->index) //update the lowLink of the current node
				node->lowLink = node->adj[i]->index;
		}
	}

	node->color = black; 

	if (node->index == node->lowLink) //if the current node is the root of a strongly connected component
	{
		cout << "\n- the connected component rooted at " << node->val << ": ";

		while (!stck.empty() && node != stck.top()) //print the nodes in the strongly connected component
		{
			cout << stck.top()->val << " ";
			stck.pop();
		}
		
		cout << stck.top()->val;
		stck.pop();
	}

	return;
}

void Tarjan_init(Graph*graph)
{
	cout << "The strongly connected components of the graph are:\n";

	stack<Node*> stck; 

	for (int i = 0; i < graph->n; i++)
		if (graph->nodes[i]->color == white) //perform Tarjan on the unvisited nodes of the graph
			Tarjan(graph->nodes[i], graph->nodes[i]->val, stck);
}

void generateEdges(Graph* graph, int nrEdges)
{
	int nrNodes = graph->n;


	bool** adjacencyMAtrix = (bool**)malloc(nrNodes * sizeof(bool*)); 

	for (int i = 0; i < nrNodes; i++) 
	{
		adjacencyMAtrix[i] = (bool*)malloc(nrNodes * sizeof(bool)); 
		for (int j = 0; j < nrNodes; j++) 
			adjacencyMAtrix[i][j] = false; 
	}

	srand(time(NULL));

	for (int i = 0; i < nrEdges; i++)
	{
		int x, y;
		do {
			x = rand() % nrNodes;
			y = rand() % nrNodes;
		} while (x == y || adjacencyMAtrix[x][y] != 0);

		if (adjacencyMAtrix[x][y] == 0)
		{
			adjacencyMAtrix[x][y] = 1;
			addEdge(graph, x, y);
		}
	}

	for (int i = 0; i < nrNodes; i++)
		free(adjacencyMAtrix[i]);
	free(adjacencyMAtrix);
}


void perf()
{
	for (int e = 1000; e <= 4500; e += 100) //vary the nr of edges
		for (int test = 0; test < 10; test++)
		{
			Operation op = p.createOperation("dfs-edges", e);
			Graph* graph = createGraph(100);
			generateEdges(graph, e);
			DFS_Init(graph, false, &op);
		}

	for (int n = 100; n <= 200; n += 10) //vary the nr of nodes
		for (int test = 0; test < 10; test++)
		{
			Operation op = p.createOperation("dfs-nodes", n);
			Graph* graph = createGraph(n);
			generateEdges(graph, 4500);
			DFS_Init(graph, false, &op);
		}

	p.divideValues("dfs-edges", 10);
	p.divideValues("dfs-nodes", 10);
	p.showReport();
}

void demo() 
{
	Graph* graph = createGraph(9);

	addEdge(graph, 0, 1);
	addEdge(graph, 2, 0);
	addEdge(graph, 1, 2);
	addEdge(graph, 1, 3);
	addEdge(graph, 2, 3);
	addEdge(graph, 1, 4);
	addEdge(graph, 3, 5);
	addEdge(graph, 5, 3);
	addEdge(graph, 5, 7);
	addEdge(graph, 6, 7);
	addEdge(graph, 6, 4);
	addEdge(graph, 4, 6);

	cout << "The initial graph is:\n";
	printGraph(graph);

	cout << "The tree resulted from DFS:\n";
	DFS_Init(graph, true);
	cout << endl;

	for (int i = 0; i < graph->n; i++)
		graph->nodes[i]->color = white;

	Tarjan_init(graph);
	cout << endl;

	Graph* topoGr = createGraph(11);
	addEdge(topoGr, 0, 3);
	addEdge(topoGr, 0, 4);
	addEdge(topoGr, 1, 2);
	addEdge(topoGr, 1, 3);
	addEdge(topoGr, 3, 4);
	addEdge(topoGr, 8, 4);
	addEdge(topoGr, 3, 7);
	addEdge(topoGr, 5, 2);
	addEdge(topoGr, 8, 7);
	addEdge(topoGr, 6, 9);
	addEdge(topoGr, 9, 5);
	addEdge(topoGr, 10, 6);
	addEdge(topoGr, 8, 10);

	cout << "\nA DAG for performing topological sort:\n";
	printGraph(topoGr);
	cout << "The list of nodes sorted topologically: ";
	topologicalSort(topoGr); //the list is non-empty because the graph is a DAG
	cout << endl;

}

int main() 
{
	demo();
	//perf();
	return 0;
}
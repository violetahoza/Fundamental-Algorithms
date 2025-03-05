/*
	Hoza Violeta Maria, Group 30424

	- I have implemented the base operations on disjoint sets (makeSet, union and findSet) and the Kruskal's algorithm using disjoint sets.
	- For representing a disjoint set I use a tree representation, having a link between the node and its parent and holding the value and the rank (the height of the tree, 
	which is relevant only for the root) of the node.
	- The makeSet function creates a new set with a single element (O(1)). The function findSet finds the representative (the root) of the set to which the given node belongs, and uses 
	path compression for optimization. The function Union performs the union of 2 sets, with the help of the function link (O(1)), which links 2 sets based on their ranks.
	- I have also defined a structure for representing an edge and implemented a function that creates an edge between 2 nodes with a given weight; they are used in the implementation 
	of the Kruskal's algorithm.
	- Implementation of the Kruskal's algorithm: 
		- create a disjoint set for each node of the graph, and store these sets in a vector of sets
		- sort the edges of the graph in ascending order, based on their weights (for that I use the sort function from C++)
		- iterate through the edges, and for each edge find the roots of the sets to which the edge's vertices belong; if the roots are different, it means that the current edge
		won't close a cycle in the MST, so it is added to the solution
		- it returns a vector of edges (the edges of the MST)
		- O(E * log(V))
*/

#include <iostream>
#include <vector>
#include "Profiler.h"

Profiler p("disjointSets");

#define MAX_SIZE 10000 //the maximum size of the input
#define STEP_SIZE 100 
#define NR_TESTS 15 //how many times we repeat the measurements 

using namespace std;

typedef struct Node {
	Node* parent;
	int value;
	int rank; //represents the height of the tree(it matters only for the root)
};

typedef struct Edge {
	int x, y, weight; 
};

/*
typedef struct Graph {
	int nrEdges, nrNodes;
	vector<Edge> edges;
};*/

Node* makeSet(int value, Operation* op) //creates a set with the element x
{
	Node* x = (Node*)malloc(sizeof(Node));//allocate memory for the element of the set

	//initialize the element
	x->parent = NULL;
	x->rank = 0;
	x->value = value; 

	(*op).count(3);//count 3 assignments

	return x;
}

Edge createEdge(int x, int y, int weight)//create an edge from the node x to node y, with the given weight 
{
	Edge edge;

	edge.x = x;
	edge.y = y;
	edge.weight = weight;

	return edge;
}

/*
Graph createGraph(int V)
{
	Graph graph;

	graph.nrNodes = V;
	graph.nrEdges = 0;
	graph.edges.clear();

	return graph;
}

void addEdge(Graph graph, int x, int y, int w)
{
	Edge edge = createEdge(x, y, w); 
	graph.edges.push_back(edge);
	graph.nrEdges++;
}*/

Node* findSet(Node* x, Operation* op)//searches for the set that contains the element x
{
	if (x->parent == NULL) //if the element is the root 
	{
		op->count();//count the comparison
		return x;
		//path.push_back(x);
	}
	op->count();//count outside the if for the case when the condition is false

	(*op).count();//count the assignment
	x->parent = findSet(x->parent, op); //path compression (links all the nodes that were found on the path to x to the root node)
	
	//path.push_back(x); 
	//Node* repr = findSet(x->parent);
	//return repr;

	return x->parent; 
}

void link(Node* x, Node* y, Operation* op)
{
	//make the tree with smaller height a subtree of the other tree (heuristic union by rank)
	if (x->rank > y->rank) 
	{
		op->count(2); //count a comparison and an assignment
		y->parent = x; 
	}
	else if (x->rank < y->rank) 
	{
		op->count(3); //count 2 comparisons and an assignment
		x->parent = y;
	}
	else //if the ranks are equal, I make one root the parent of the other and increase its rank
	{
		op->count(3); //count a comparison and 2 assignments
		x->parent = y;
		y->rank++;
	}
}

void Union(Node* x, Node* y, Operation* op)
{
	//vector<Node*> path; 
	link(findSet(x, op), findSet(y, op), op);
}

//int compareEdges(const void* edge1, const void* edge2)
//{
//	return ((Edge*)edge1)->weight < ((Edge*)edge2)->weight;
//}

int compareEdges(const Edge& edge1, const Edge& edge2)
{
	return edge1.weight < edge2.weight; //returns the edge with the smaller weight
}

vector<Edge> Kruskal(vector<Edge>& edges, int nrNodes) // Kruskal's algorithm for finding the minimum spanning tree
{
	Operation op = p.createOperation("kruskal", nrNodes);

	vector<Edge> sol;
	vector<Node*> sets;

	for (int i = 0; i < nrNodes; i++)
	{
		sets.push_back(makeSet(i, &op));//create disjoint sets for each vertex
	}

	sort(edges.begin(), edges.end(), compareEdges);//sort edges based on weight

	//qsort(edges, nrEdges, sizeof(Edge), compareEdges); 

	for (int i = 0; i < edges.size(); i++) //go through the sorted edges and if the current edge doesn't close a cycle, then add it to the solution 
	{
		Node* rootX = findSet(sets[edges[i].x], &op);
		Node* rootY = findSet(sets[edges[i].y], &op);
		if (rootX != rootY)
		{
			sol.push_back(edges[i]);
			Union(rootX, rootY, &op);
		}
	}

	return sol;
}

vector<Edge> buildGraph(int nrNodes, int nrEdges)//build an undirected, connected, and random graph with random weights on edges (n nodes, n * 4 edges)
{
	std::vector<Edge> edges; 

	int** adjacencyMatrix = (int**)malloc(nrNodes * sizeof(int*)); 
	/*if (adjacencyMatrix == NULL)
	{ 
		fprintf(stderr, "Memory allocation failed for matrix rows.\n");
		exit(EXIT_FAILURE);
	}*/
	for (int i = 0; i < nrNodes; i++)
	{
		adjacencyMatrix[i] = (int*)calloc(nrNodes, sizeof(int));
		//for (int j = 0; j < nrNodes; j++)
			//adjacencyMatrix[i][j] = 0;
		/*if (adjacencyMatrix[i] == NULL) 
		{
			fprintf(stderr, "Memory allocation failed for matrix columns.\n"); 
			exit(EXIT_FAILURE);
		}*/
	}

	srand(time(NULL)); 
	
	for (int i = 1; i < nrNodes; i++)
	{
		int j = rand() % i; //choose random a previously generated node
		int weight = rand() % 99 + 1; //generate a random weight
		Edge edge = createEdge(i, j, weight);
		edges.push_back(edge);
		adjacencyMatrix[i][j] = adjacencyMatrix[j][i] = weight;//mark the new edge 

		//cout << i << " " << j << " " << weight << endl;
	}

	//int rest = 0;
	//if (edges.size() < nrEdges)
		//rest = nrEdges - edges.size();

	//edges.resize(nrEdges);
	//cout << edges.size() << " " << nrEdges;

	//for (int i = nrNodes; i < nrEdges; i++)
	while(edges.size() < nrEdges) //while there are still edges to add
	{
		//cout << i << " ";
		int x = 0, y = 0, weight;
		do {
			x = rand() % nrNodes;
			y = rand() % nrNodes;
			weight = rand() % 99 + 1;
		} while (x == y || adjacencyMatrix[x][y] == weight); //ensure no self loops or duplicate edges
		
		Edge edge = createEdge(x, y, weight);
		edges.push_back(edge); 
		adjacencyMatrix[x][y] = adjacencyMatrix[y][x] = weight;
	}


	//for (int i = 0; i < edges.size(); i++)
		//cout << edges[i].x << " " << edges[i].y << " " << edges[i].weight << endl;

	for (int i = 0; i < nrNodes; i++) {
		free(adjacencyMatrix[i]);
	}
	free(adjacencyMatrix);

	return edges;
}

void demo()
{
	Operation op = p.createOperation("-", 0);
	vector<Node*> sets;
	int n = 10;

	for (int i = 0; i < n; i++)
	{
		sets.push_back(makeSet(i, &op));
		//cout << "{" << sets[i]->value << "} ";
	}

	cout << "The initial sets are: ";
	for (int i = 0; i < n; i++)
	{
		cout << "{" << findSet(sets[i], &op)->value << "}";
		if (i != (n - 1))
			cout << ", ";
	}
	cout << endl;

	Union(sets[0], sets[1], &op);
	Union(sets[2], sets[1], &op);
	Union(sets[3], sets[2], &op);
	Union(sets[4], sets[7], &op);
	Union(sets[8], sets[9], &op);

	cout << "\nAfter the unions:\n";
	for (int i = 0; i < n; i++)
	{
		cout << "\nThe element " << sets[i]->value << " belongs to the set with the root: " << findSet(sets[i], &op)->value;
	}
	cout << endl;

	int nrNodes = 4;
	vector<Edge> graph = { 
	   {0, 1, 12}, {0, 2, 5}, {0, 3, 15}, {1, 2, 1},
	   {2, 3, 5}, {0, 1, 9}, {0, 2, 6}, {0, 3, 9},
	   {1, 2, 22}, {1, 3, 14}, {2, 3, 31}, {1, 3, 3},
	   {0, 1, 5}, {0, 2, 3}, {0, 3, 21}, {1, 2, 3}
	};
	
	cout << "\nGraph edges:\n";
	for (int i = 0; i < graph.size(); i++)
		cout << "\n- edge from " << graph[i].x << " to " << graph[i].y << " with weight w = " << graph[i].weight;
	cout << endl;

	vector<Edge> MST = Kruskal(graph, nrNodes);
	
	cout << "\nThe edges of the minimum spanning tree obtained after applying Kruskal:\n";
	for (int i = 0; i < MST.size(); i++)
		cout << "\n- edge from " << MST[i].x << " to " << MST[i].y << " with weight w = " << MST[i].weight;
	cout << endl;

	int nrNodes1 = 5;
	vector<Edge> graph1 = buildGraph(nrNodes1, 9); 

	cout << "\nRandom graph edges:\n";
	for (int i = 0; i < graph1.size(); i++)
		cout << "\n- edge from " << graph1[i].x << " to " << graph1[i].y << " with weight w = " << graph1[i].weight;
	cout << endl;

	vector<Edge> MST1 = Kruskal(graph1, nrNodes1);

	cout << "\nThe edges of the minimum spanning tree obtained after applying Kruskal:\n";
	for (int i = 0; i < MST1.size(); i++)
		cout << "\n- edge from " << MST1[i].x << " to " << MST1[i].y << " with weight w = " << MST1[i].weight;
	cout << endl;

}

void perf()
{
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)//vary the size of the input array
		for (int test = 0; test < NR_TESTS; test++) //I repeat the measurements NR_TESTS times
		{
			vector<Edge> graph = buildGraph(n, n * 4);//generate an undirected, connected, and random graph with random weights on edges (n nodes, n * 4 edges)
			vector<Edge> MST = Kruskal(graph, n);//apply Kruskal's algorithm on the generated graph
		}

	p.divideValues("kruskal", NR_TESTS);
	p.showReport();
}

int main()
{
	demo();
	//perf();
	return 0; 
}

/*
vector<Edge> buildGraph(int nrNodes)
{
	vector<Edge> edges;
	vector<int> nodes;

	srand(time(NULL));

	for (int i = 0; i < nrNodes; i++)
		nodes.push_back(i);
	random_shuffle(nodes.begin(), nodes.end());

	for (int i = 1; i < nrNodes; i++)
	{
		int weight = rand() % 99;
		Edge edge = createEdge(nodes[i - 1], nodes[i], weight);
		edges.push_back(edge);
	}

	int rest = nrNodes * 4 - nrNodes + 1;
	for (int i = 0; i < rest; i++)
	{
		int x = rand() % nrNodes;
		int y = rand() % nrNodes;
		int weight = rand() % 99;
		Edge edge = createEdge(x, y, weight);
		edges.push_back(edge);
	}

	return edges;
}*/
/*
    Hoza Violeta Maria, Group 30424
    BFS complexity: O(V + E), where V is the nr of nodes and E the nr of edges
*/

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <queue>
#include <vector>
#include "bfs.h"

using namespace std;

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4

    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};
    int nr = 0;

    for (int i = 0; i < 4; i++)
    {
        Point newNeighb;
        newNeighb.row = p.row + dx[i];
        newNeighb.col = p.col + dy[i];
        
        if (grid->mat[newNeighb.row][newNeighb.col] == 0) 
        {
            neighb[nr++] = newNeighb;
        }
    }

    return nr;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();

    queue<Node*> q;  
    q.push(s); //add the node to the queue
    if (op != NULL) 
        (*op).count(4); //count 2 assignments
    s->color = COLOR_GRAY; //mark it as visited
    s->parent = NULL;
    s->dist = 0; //the first node is the root

    while (!q.empty())
    {
        //if (op != NULL)
            //(*op).count(4);

        Node* node = q.front(); //get the current node
         
        //Point p = node->position;
        //int nrNeighb = node->adjSize;  
        
        for (int i = 0; i < node->adjSize; i++) //process each neighbor of the current node
        {
            //push the unvisited neighbors in the queue
            Node* neighbor = node->adj[i]; 
            if (op != NULL)
                (*op).count(); //count the comparison
            if (neighbor->color == COLOR_WHITE) //if the neighbor is unvisited
            {
                q.push(neighbor); //add it to the queue
                if (op != NULL)
                    (*op).count(4);

                neighbor->color = COLOR_GRAY; // and mark it as visited
                neighbor->dist = node->dist + 1; //compute the distance from the current node to the root
                neighbor->parent = node; //update its parent
            }
        }

        q.pop(); 
        if (op != NULL)
            (*op).count(2);
        node->color = COLOR_BLACK; 
    }
}

void prettyPrint(int parent[], Point repr[], int nrNodes, int root, int level = 0)//pretty print for the parent representation
{
    for (int i = 0; i < nrNodes; i++)
        if (parent[i] == root)
        {
            for (int j = 0; j < level; j++)
                cout << "\t";
            cout << "(" << repr[i].row << ", " << repr[i].col << ")" << endl;
            prettyPrint(parent, repr, nrNodes, i, level + 1);//continue with the nodes at the next level
        }
}


void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        prettyPrint(p, repr, n, -1);
    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000

    bfs(graph, start, NULL); //perform BFS starting from the node start

    if (end->color == COLOR_WHITE) //if after the BFS the node is not black
        return -1; //it means the node is not reacheable (because it doesn't belong to the same connected component as the node start)

    //reconstruct the shortest path by backtracking from end to start
    vector<Node*> shortestPath; //here I will store the shortest part, but in reverse order
    Node* currentNode = end; //start from the end
    while (currentNode != NULL)
    {
        shortestPath.push_back(currentNode);
        currentNode = currentNode->parent;
    }

    int nrNodes = shortestPath.size();
    for (int i = 0; i < nrNodes; i++) //copy the shortest path to the path array
        path[i] = shortestPath[nrNodes - i - 1];

    return nrNodes;
   // return -1;
}

int isAdjacent(Node* node1, Node* node2) //verify if there is a edge between 2 given nodes
{
    for (int i = 0; i < node1->adjSize; i++)
        if (node1->adj[i] == node2)
            return 1;
    return 0;
}

void addEdge(Node* x, Node* y) 
{
    x->adj = (Node**)realloc(x->adj, (x->adjSize + 1) * sizeof(Node*)); //allocate memory for a new node in the adjacency list
    y->adj = (Node**)realloc(y->adj, (y->adjSize + 1) * sizeof(Node*));

    x->position.row = rand() % 10;
    x->position.col = rand() % 18;

    do {
        y->position.row = rand() % 10;
        y->position.col = rand() % 18;
    } while (x->position.col == y->position.col && x->position.row == y->position.row);

    if (x != y && isAdjacent(x, y) == 0)
    {
        x->adj[x->adjSize++] = y; //update the adjacency list of the node 
        y->adj[y->adjSize++] = x;
    }
}

Graph* generate(Graph* graph, int nrEdges) //generate an undirected, connected, random graph
{
    int nrNodes = graph->nrNodes;

    // cout << nrNodes << " ";
    // int countEdges = 0;
     //vector<Node*> nodes;

     /*
     bool** adjacencyMAtrix = (bool**)malloc(nrNodes * sizeof(bool*));

     for (int i = 0; i < nrNodes; i++)
     {
         adjacencyMAtrix[i] = (bool*)malloc(nrNodes * sizeof(bool));
         for (int j = 0; j < nrNodes; j++)
             adjacencyMAtrix[i][j] = false;
     }*/

    srand(time(NULL));

    for (int i = 1; i < nrNodes; i++) //generate the first nrNodes - 1 edges, such that the graph is connected
    {
        int j = rand() % i;//choose random a previously generated node
        //cout << i << " " << j << endl;
        addEdge(graph->v[i], graph->v[j]); //create an edge
        // adjacencyMAtrix[i][j] = adjacencyMAtrix[j][i] = true;//mark the new edge
        // countEdges++;
    }

    int remainingEdges = nrEdges - nrNodes + 1; 

    while (remainingEdges > 0) //as long as there are edges left to add
    {
        int x = 0, y = 0;
        do {
            x = rand() % nrNodes;
            y = rand() % nrNodes;
        } while (x == y || isAdjacent(graph->v[x], graph->v[y]) != 0);

        //cout << x << " " << y << endl;
        addEdge(graph->v[x], graph->v[y]);
        remainingEdges--;
        // adjacencyMAtrix[x][y] = adjacencyMAtrix[y][x] = true;
    }

    // free(adjacencyMAtrix);

    /*
    for (int i = 0; i < nrNodes; i++)
    {
        printf("Node %d (%d, %d): ", i, graph->v[i]->position.row, graph->v[i]->position.col);
        for (int j = 0; j < graph->v[i]->adjSize; j++) {
            printf("(%d, %d) ", graph->v[i]->adj[j]->position.row, graph->v[i]->adj[j]->position.col);
        }
        printf("\n");
    }*/

    return graph;
}

void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        generate(&graph, n); 

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        generate(&graph, 4500);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}

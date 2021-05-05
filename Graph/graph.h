#include <set>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <random>
#include <sys/time.h>
#include <iomanip> 

class Graph
{
public:
    Graph(int numOfVertices): numOfVertices_(numOfVertices) 
    {
        adjacentVertices.resize(numOfVertices_);
    }

    ~Graph() = default;
 
    void AddEdge(int v, int w);
    std::vector<int> GreedAlgorithm();
    void PrintGraph();
    void PrintColors(std::vector<int> color);

private:
    int numOfVertices_;
    std::vector<std::set<int>> adjacentVertices;
};

Graph CreateGraph();
Graph CreateRandomGraph(int vertices);

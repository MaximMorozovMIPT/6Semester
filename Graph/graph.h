#include <set>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

class Graph
{
public:
    Graph(int numOfVertices): numOfVertices_(numOfVertices) 
    {
        adjacentVertices.resize(numOfVertices_);
    }

    ~Graph() = default;
 
    void AddEdge(int v, int w);
    void GreedAlgorithm();
    void PrintGraph();

private:
    int numOfVertices_;
    std::vector<std::set<int>> adjacentVertices;
};

Graph CreateGraph();

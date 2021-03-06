#include "graph.h"

void Graph::AddEdge(int v, int adj)
{
    adjacentVertices[v].insert(adj);
    adjacentVertices[adj].insert(v);
}

std::vector<int> Graph::GreedAlgorithm()
{
    // Vector of int where every number equal to color
    // -1 - not colored yet, 0 - first color, 1 - second, etc.
    std::vector<int> color(numOfVertices_);
    color[0] = 0;
    for (int i = 1; i < numOfVertices_; ++i)
    {
        color[i] = -1;
    }
 
    // Vector of unavailable colors for every vertex
    // 0 - color available, 1 - adjacent vertex color and unavailable
    std::vector<int> colorCheck(numOfVertices_);

    // Coloring other vertices
    for (int vertex = 1; vertex < numOfVertices_; vertex++)
    {
        // Mark all colors as available
        for (int i = 0; i < numOfVertices_; ++i)
        {
            colorCheck[i] = 0;
        }

        // Find unavailable colors for current vertex
        for (auto i: adjacentVertices[vertex])
        {
            if (color[i] != -1)
            {
                colorCheck[color[i]] = 1;
            }
        }
 
        // Find the first available color
        // and coloring the vertex
        for (int clr = 0; clr < numOfVertices_; clr++)
        {
            if (colorCheck[clr] == 0)
            {
                color[vertex] = clr;
                break;
            }
        }
    }
    return color;
}
void Graph::PrintColors(std::vector<int> color)
{
    std::cout << "\nVertex   Color " << std::endl;
    // Print colors
    for (int u = 0; u < numOfVertices_; u++)
    {
        std::cout << u + 1 << "     |  " << color[u] << std::endl;
    }

    std::cout << "\nNumber of used colors: " << *std::max_element(color.begin(), color.end()) << std::endl;
}

void Graph::PrintGraph()
{
    for(int i = 0; i < adjacentVertices.size(); ++i)
    {
        std::cout << i + 1 << ": ";
        for (auto v: adjacentVertices[i])
        {
            std::cout << v + 1 << " ";
        }
        std::cout << std::endl;
    }
}

Graph CreateGraph()
{
    std::string graph;
    std::ifstream file("input.txt");
    std::getline(file, graph);
    std::vector<std::string> vertex;
    std::string::size_type pos_begin = 3, pos_end = 0;
    
    // Define the number of vertices as a number of ";" simbols
    // and parse string to strings with adjacent vertices for specific vertex
    for (;;)
    {
        pos_end = graph.find(";", pos_begin);
        if (pos_end == std::string::npos)
        {
            break;
        }
        vertex.push_back(std::string(graph, pos_begin, pos_end - pos_begin));
        pos_begin = pos_end + 5;
    }

    // Create graph
    int sizeOfGraph = vertex.size();
    Graph gf(sizeOfGraph);

    // Read all adjacent vertices for specific one
    for(int i = 0; i < vertex.size(); ++i)
    {
        std::stringstream buf(vertex[i]);
        int adjVertex, emptynum;
        char brace;
        while (buf >> adjVertex) 
        {
            // Read unnecessary edge weight
            buf >> brace >> emptynum >> brace;

            // Add read vertex, subtract one because of vector logic
            gf.AddEdge(i, adjVertex - 1);
        }
    }
    return gf;
}

Graph CreateRandomGraph(int vertices)
{
    Graph gf(vertices);
    for (int i = 0; i < vertices; ++i)
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, vertices - 1); // distribution in range [0, vertices - 1]
        // Number of POSSIBLE edges because newAdjVertex can repeat
        int numOfPossibleEdges = dist(rng);
        // For every possible edge generating second vertex (newAdjVertex)
        for (int j = 0; j < numOfPossibleEdges; ++j)
        {
            int newAdjVertex = dist(rng);
            // Check that current vertex not equal new vertex
            if (newAdjVertex != i)
            {
                gf.AddEdge(i, newAdjVertex);
            }
        }
    }
    return gf;
}
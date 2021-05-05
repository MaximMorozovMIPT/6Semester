#include "graph.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Enter work mode\n \"./color certain\" for read graph from input\n \"./color random\" for random graph\n";
        return 0;
    }

    if (std::string(argv[1]) == "certain")
    {
        auto g1 = CreateGraph();
        g1.PrintGraph();
        auto color = g1.GreedAlgorithm();
        g1.PrintColors(color);
    }
    if (std::string(argv[1]) == "random")
    {
        if (argc < 3)
        {
            std::cout << "Enter number of vertices\n";
            return 0;
        }
        auto numOfVertices = atoi(argv[2]);
        auto g2 = CreateRandomGraph(numOfVertices);
        g2.PrintGraph();
        auto color = g2.GreedAlgorithm();
        g2.PrintColors(color);
    }
    if (std::string(argv[1]) == "test")
    {
        std::ofstream myfile;
        myfile.open ("example.csv");
        for(int i = 10; i < 250; ++i)
        {
            struct timeval begin, end;
            gettimeofday(&begin, 0);
            for(int j = 0; j < 100; ++j)
            {
                auto g = CreateRandomGraph(i);
                auto color = g.GreedAlgorithm();
            }
            gettimeofday(&end, 0);
            long seconds = end.tv_sec - begin.tv_sec;
            long microseconds = end.tv_usec - begin.tv_usec;
            double elapsed = seconds + microseconds*1e-6;
            myfile << i << "," << std::setprecision(5) << std::fixed << elapsed << "\n";
        }
        myfile.close();
    }
    return 0;
}
#include <openipML/DijkstraClosedPath.h>
#include <openipDS/Stopper.h>

namespace openip
{
    DijkstraClosedPath::DijkstraClosedPath()
    {
    }

    DijkstraClosedPath::DijkstraClosedPath(const DijkstraClosedPath& /*d*/)
    {
    }

    DijkstraClosedPath::~DijkstraClosedPath()
    {
    }

    Vector<int> DijkstraClosedPath::extractPath(SparseAdjacencyMatrix& s, int startNode)
    {
        int targetNode= startNode;

        tprintf("startNode: %d\n", startNode);

        Vector<int> parents;
        Vector<int> closed;
        Vector<int> result;
        parents.resize(s.size());
        parents(startNode)= -1;

        PriorityQueue<float, int> open;
        open.push(0, startNode);

        while ( open.size() > 0 )
        {
            int node= open.top();
            float weight= open.topP();
            open.pop();

            closed.push_back(node);

            tprintf("opening: %f, %d\n", weight, node);
            for ( unsigned int i= 0; i < s(node).size(); ++i )
                printf("%d ", s(node)(i).vertex);
            printf("\n");

            for ( unsigned int i= 0; i < s(node).size(); ++i )
            {

                if ( s(node)(i).vertex == targetNode )
                {
                    while ( parents(node) != -1 )
                    {
                        result.push_back(node);
                        node= parents(node);
                        printf("%d ", node);
                    }
                    break;
                }

                if ( closed.contains(s(node)(i).vertex) )
                    continue;
                parents(s(node)(i).vertex)= node;
                open.push(s(node)(i).weight, s(node)(i).vertex);
            }
            if ( result.size() > 0 )
                break;
        }
        return result;
    }

    Vector<int> DijkstraClosedPath::extractPath(SparseAdjacencyMatrix& s)
    {
        Vector<int> result;
        Vector<int> testedNodes;

        while ( 1 )
        {
            /*float weight= -1;*/
            int startNode= -1;
            float maxValue= 0.0;

            for ( unsigned int i= 0; i < s.size(); ++i )
            {
                if ( testedNodes.contains(i) )
                    continue;
                for ( unsigned int j= 0; j < s(i).size(); ++j )
                {
                    //printf("%f %f\n", s(i)(j).weight, maxValue);
                    if ( s(i)(j).weight > maxValue )
                    {
                        startNode= i;
                        maxValue= s(i)(j).weight;
                    }
                }
            }

            result= extractPath(s, startNode);
            testedNodes.push_back(startNode);
            if ( result.size() > 0 )
                break;
        }


        tprintf("dijkstra finished\n");

        return result;
    }
}

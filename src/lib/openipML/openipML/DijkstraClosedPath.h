#ifndef DIJKSTRACLOSEDPATH_H
#define DIJKSTRACLOSEDPATH_H

#include <openipDS/Matrix.h>
#include <openipDS/PriorityQueue.h>

#include <openipML/SparseAdjacencyMatrix.h>

namespace openip
{
    class DijkstraClosedPath
    {
    public:
        DijkstraClosedPath();

        DijkstraClosedPath(const DijkstraClosedPath& d);

        ~DijkstraClosedPath();

        Vector<int> extractPath(SparseAdjacencyMatrix& s);

        Vector<int> extractPath(SparseAdjacencyMatrix& s, int startNode);
    };


}

#endif // DIJKSTRACLOSEDPATH_H

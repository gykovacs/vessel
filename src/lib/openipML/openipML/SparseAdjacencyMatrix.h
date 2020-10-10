#ifndef SPARSEADJACENCYMATRIX_H
#define SPARSEADJACENCYMATRIX_H

#include <openipDS/Vector.h>

namespace openip
{
    class VertexWeightPair
    {
    public:
        VertexWeightPair(int vertex, float weight);

        VertexWeightPair(const VertexWeightPair& v);

        ~VertexWeightPair();

        VertexWeightPair& operator=(const VertexWeightPair& v);

        int vertex;
        float weight;
    };

    class SparseAdjacencyMatrix: public Vector<Vector<VertexWeightPair> >
    {
    public:
        SparseAdjacencyMatrix();

        SparseAdjacencyMatrix(int n);

        SparseAdjacencyMatrix(const SparseAdjacencyMatrix& s);

        ~SparseAdjacencyMatrix();
    };
}

#endif // SPARSEADJACENCYMATRIX_H

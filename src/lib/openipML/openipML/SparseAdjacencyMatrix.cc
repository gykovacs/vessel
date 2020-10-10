#include <openipML/SparseAdjacencyMatrix.h>

namespace openip
{
    VertexWeightPair::VertexWeightPair(int vertex, float weight)
    {
        this->vertex= vertex;
        this->weight= weight;
    }

    VertexWeightPair::VertexWeightPair(const VertexWeightPair& v)
    {
        this->vertex= v.vertex;
        this->weight= v.weight;
    }

    VertexWeightPair::~VertexWeightPair()
    {
    }

    VertexWeightPair& VertexWeightPair::operator=(const VertexWeightPair& v)
    {
        this->vertex= v.vertex;
        this->weight= v.weight;
        return (*this);
    }

    SparseAdjacencyMatrix::SparseAdjacencyMatrix(int n)
        : Vector<Vector<VertexWeightPair> >(n)
    {
    }

    SparseAdjacencyMatrix::SparseAdjacencyMatrix()
        : Vector<Vector<VertexWeightPair> >()
    {
    }

    SparseAdjacencyMatrix::SparseAdjacencyMatrix(const SparseAdjacencyMatrix& s)
        : Vector<Vector<VertexWeightPair> >(s)
    {
    }

    SparseAdjacencyMatrix::~SparseAdjacencyMatrix()
    {
    }
}

#include <openipML/Combinations.h>
#include <openipDS/mathFunctions.h>

namespace openip
{
    Combinations::Combinations(int n, int k)
    {
        this->n= n;
        this->k= k;
        //total= comb(n,k);
        total= 0;
        idx= 0;
        indices.resize(k);
        indices[0]= 1;
    }

    Combinations::Combinations(const Combinations& c)
    : n(c.n), k(c.k), idx(c.idx), total(c.total), indices(c.indices)
    {
    }

    Combinations::~Combinations()
    {
    }

    int Combinations::hasNextCombination()
    {
        if ( idx < total )
            return 1;
        return 0;
    }

    void Combinations::nextCombination(std::vector<int>& /*c*/)
    {
        
    }
}

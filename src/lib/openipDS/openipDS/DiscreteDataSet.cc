#include <openipDS/DiscreteDataSet.h>

#include "DiscreteDataSet.h"

namespace openip
{
    DiscreteDataSet::DiscreteDataSet()
    : DataSet<int>()
    {
    }

    DiscreteDataSet::DiscreteDataSet(const DiscreteDataSet& c)
    : DataSet<int>(c)
    {
    }

    DiscreteDataSet::~DiscreteDataSet()
    {
    }
}

#include <openipDS/ContinuousDataSet.h>

namespace openip
{
    ContinuousDataSet::ContinuousDataSet()
    : DataSet<float>()
    {
    }

    ContinuousDataSet::ContinuousDataSet(const ContinuousDataSet& c)
    : DataSet<float>(c)
    {
    }

    ContinuousDataSet::~ContinuousDataSet()
    {
    }
}

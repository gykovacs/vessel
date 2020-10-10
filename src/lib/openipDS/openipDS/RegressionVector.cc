#include <openipDS/RegressionVector.h>

namespace openip
{
    RegressionVector::RegressionVector()
    : Vector<float>()
    {
    }

    RegressionVector::RegressionVector(const RegressionVector& r)
    : Vector<float>(r)
    {
        this->value= r.value;
    }

    RegressionVector::~RegressionVector()
    {
    }
}

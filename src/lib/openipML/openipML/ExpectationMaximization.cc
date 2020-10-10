#include <openipML/ExpectationMaximization.h>

namespace openip
{
    ExpectationMaximization::ExpectationMaximization()
    : iteration(0)
    {
    }

    ExpectationMaximization::ExpectationMaximization(const ExpectationMaximization& em)
    {
        iteration= em.iteration;
    }

    ExpectationMaximization::~ExpectationMaximization()
    {
    }

    void ExpectationMaximization::doEM()
    {
        init();
        while ( ! stopCondition() )
        {
            expect();
            maximize();
            ++iteration;
        }
    }
}
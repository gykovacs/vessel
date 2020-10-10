#include <openipSC/distributions.h>

#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>

namespace openip
{
    double poissonDistribution(double mu, int k)
    {
        return gsl_ran_poisson_pdf(k, mu);
    }
}

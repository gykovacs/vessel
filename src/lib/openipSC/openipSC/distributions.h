#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

#include <gsl/gsl_math.h>

namespace openip
{
    double poissonDistribution(double mu, int k);
}

#endif // DISTRIBUTIONS_H

#ifndef NONCENTEREDGAUSSIANFILTER2_H
#define NONCENTEREDGAUSSIANFILTER2_H

#include <openipDS/Filter2s.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class NonCenteredGaussianFilter2: public GaussianFilter2<INPUT, OUTPUT>
    {
    public:
        NonCenteredGaussianFilter2(double angle, double dist, double sigma, int kernel, double alpha, double beta, double delta, double gamma, int stride);

        NonCenteredGaussianFilter2(const NonCenteredGaussianFilter2<INPUT, OUTPUT>& ncgf);

        ~NonCenteredGaussianFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    NonCenteredGaussianFilter2<INPUT, OUTPUT>::NonCenteredGaussianFilter2(double angle, double dist, double sigma, int kernel, double alpha, double beta, double delta, double gamma, int stride)
        : GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, alpha, beta, delta, gamma, stride)
    {

        for ( int i= 0; i < this->size(); ++i )
        {

        }
    }

    template<typename INPUT, typename OUTPUT>
    NonCenteredGaussianFilter2<INPUT, OUTPUT>::NonCenteredGaussianFilter2(const NonCenteredGaussianFilter2<INPUT, OUTPUT>& ncgf)
        : GaussianFilter2<INPUT, OUTPUT>(ncgf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    NonCenteredGaussianFilter2<INPUT, OUTPUT>::~NonCenteredGaussianFilter2()
    {
    }
}

#endif // NONCENTEREDGAUSSIANFILTER2_H

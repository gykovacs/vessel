#include <openipSC/FourierTransformer1.h>

namespace openip
{
    FourierTransformer1::FourierTransformer1()
    {
    }

    FourierTransformer1::FourierTransformer1(int n)
    {
        wt= gsl_fft_complex_wavetable_alloc(n);
        ws= gsl_fft_complex_workspace_alloc(n);
        this->n= n;
    }

    FourierTransformer1::FourierTransformer1(const FourierTransformer1& m)
    {
        this->wt= m.wt;
        this->ws= m.ws;
        this->n= m.n;
    }

    FourierTransformer1::~FourierTransformer1()
    {
    }
}
#include <openipSC/RandomNumberGenerator.h>
#include <gsl/gsl_randist.h>

#include <stdio.h>

#include "RandomNumberGenerator.h"

namespace openip
{
    UniformRandomIntegerGenerator::UniformRandomIntegerGenerator(int)
    {
        r= gsl_rng_alloc(gsl_rng_taus);
    }

    unsigned long int UniformRandomIntegerGenerator::generate(int n)
    {
        if ( n == -1 )
            return gsl_rng_uniform_int(r, this->n);
        else
            return gsl_rng_uniform_int(r, n);
    };

    UniformRandomFloatGenerator::UniformRandomFloatGenerator(float a, float b)
    {
        this->a= a;
        this->b= b;
        r= gsl_rng_alloc(gsl_rng_taus);
    }

    float UniformRandomFloatGenerator::generate()
    {
        return (float)(gsl_rng_uniform_pos(r) * (b - a) + a);
    }

    GaussianRandomFloatGenerator::GaussianRandomFloatGenerator(float m, float d)
    {
        mean= m;
        dev= d;
        r= gsl_rng_alloc (gsl_rng_taus);
    }
    
    GaussianRandomFloatGenerator::~GaussianRandomFloatGenerator()
    {
    }

    float GaussianRandomFloatGenerator::generate()
    {
        //printf("%f\n", gsl_ran_gaussian(this->r, this->dev)); fflush(stdout);
        return (float)(this->mean + gsl_ran_gaussian(this->r, this->dev));
    }
    
    PoissonRandomFloatGenerator::PoissonRandomFloatGenerator(float m)
    {
        mean= m;
        r= gsl_rng_alloc (gsl_rng_taus);
    }

    float PoissonRandomFloatGenerator::generate()
    {
        return (float)(gsl_ran_poisson(this->r, this->mean));
    }
}

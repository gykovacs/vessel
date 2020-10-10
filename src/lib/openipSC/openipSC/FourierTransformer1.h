/**
 * @file FourierTransformer1.h
 * @author Gyorgy Kovacs <gyuriofkovacs@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * FourierTransformer1 class contains allocated wavetable and workspace for
 * 1 dimensional Fourier-transform
 */
#ifndef _VECTORFOURIERTRANSFORMER_H
#define	_VECTORFOURIERTRANSFORMER_H

#include <openipSC/fourierTransform.h>

/**
 * namespace of all openip objects and functions
 */
namespace openip
{
    /**
     * FourierTransformer1 class contains allocated wavetable and workspace for
     * 1 dimensional Fourier-transform of given size n
     */
    class FourierTransformer1
    {
    public:
        /**
         * default constructor
         */
        FourierTransformer1();

        /**
         * constructor to create FourierTransformator1 with size n
         * @param n size of the vectors that can be efficiently transformed
         * with this object
         */
        FourierTransformer1(int n);

        /**
         * copy constructor
         * @param m instance to copy
         */
        FourierTransformer1(const FourierTransformer1& m);

        /**
         * destructor
         */
        ~FourierTransformer1();

        /**
         * transforms the input vector of size n into the output vector
         * @param input vector pointer to transform, contains n allocated elements
         * @param output resulting vector, the pointer points to n allocated elements
         */
        template<typename INPUT>
        void transform(INPUT* input, std::complex<double>* output);

        /**
         * inverse transform of the input transformed vector into the output vector
         * @param input vector pointer to inverse transform, contains n allocated elements
         * @param output resulting vector, the pointer points to n allocated elements
         */
        template<typename OUTPUT>
        void inverseTransform(std::complex<double>* input, OUTPUT* output);

        /**
         * wavetable of the transform
         */
        gsl_fft_complex_wavetable* wt;

        /**
         * workspace of the transform
         */
        gsl_fft_complex_workspace* ws;

        /**
         * size of the vectors that can be efficiently transformed with this object
         */
        int n;
    };

    

    template<typename INPUT>
    void FourierTransformer1::transform(INPUT* input, std::complex<double>* output)
    {
        fourierTransform1D(input, n, output, this->wt, this->ws);
    }

    template<typename OUTPUT>
    void FourierTransformer1::inverseTransform(std::complex<double>* input, OUTPUT* output)
    {
        inverseFourierTransform1D(input, n, output, this->wt, this->ws);
    }
}

#endif	/* _VECTORFOURIERTRANSFORMER_H */


/**
 * @file FourierTransformer2.h
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
 * FourierTransformer2 class contains allocated wavetable and workspace for
 * 2 dimensional Fourier-transform
 */

#ifndef _MATRIXFOURIERTRANSFORMER_H
#define	_MATRIXFOURIERTRANSFORMER_H

#include <complex>

#include <openipSC/fourierTransform.h>

namespace openip
{
    /**
     * FourierTransformer2 class contains allocated wavetable and workspace for
     * 2 dimensional Fourier-transform
     */
    template<typename INPUT, typename OUTPUT>
    class FourierTransformer2
    {
    public:
        /**
         * default constructor
         */
        FourierTransformer2();

        /**
         * constructor to allocate wavetable and workspace
         * @param rows number of rows in the matrix to transform
         * @param columns number of columns in the matrix to transform
         */
        FourierTransformer2(int rows, int columns);

        /**
         * copy constructor
         * @param m instance to copy
         */
        FourierTransformer2(const FourierTransformer2& m);

        /**
         * destructor
         */
        ~FourierTransformer2();

        /**
         * Fourier-transform of the input matrix with rows and columns into
         * the output matrix
         * @param input the matrix to transform, contains rows*columns elements
         * in row-continuous representation
         * @param output the Fourier-transform of input
         */
        void transform(INPUT* input, std::complex<double>* output);

        void transform(std::complex<double>* input, std::complex<double>* output);

        /**
         * inverse Fourier-transform of the input frequency space object into
         * the output area
         * @param input frequency space object, contains rows*columns elements
         * @param output inverse transformed object
         */
        void inverseTransform(std::complex<double>* input, OUTPUT* output);

        void updateSize(int rows, int columns);

        /**
         * gsl wave table for the rows
         */
        gsl_fft_complex_wavetable* wtr;

        /**
         * gsl workspace for the rows
         */
        gsl_fft_complex_workspace* wsr;

        /**
         * gsl wave table for the columns
         */
        gsl_fft_complex_wavetable* wtc;

        /**
         * gsl workspace for the columns
         */
        gsl_fft_complex_workspace* wsc;

        /**
         * number of rows in the matrices that can be transformed with
         * this FourierTransformer
         */
        int rows;

        /**
         * number of columns in the matrices that can be transformed with
         * this FourierTransformer
         */
        int columns;
    };

    template<typename INPUT, typename OUTPUT>
    FourierTransformer2<INPUT, OUTPUT>::FourierTransformer2()
    {
        wtr= NULL;
        wsr= NULL;
        wtc= NULL;
        wsc= NULL;
        rows= 0;
        columns= 0;
    }

    template<typename INPUT, typename OUTPUT>
    FourierTransformer2<INPUT, OUTPUT>::FourierTransformer2(int rows, int columns)
    {
        updateSize(rows, columns);
    }

    template<typename INPUT, typename OUTPUT>
    FourierTransformer2<INPUT, OUTPUT>::FourierTransformer2(const FourierTransformer2& m)
    {
        this->wtr= m.wtr;
        this->wsr= m.wsr;
        this->wtc= m.wtc;
        this->wsc= m.wsc;
        this->columns= m.columns;
        this->rows= m.rows;
    }

    template<typename INPUT, typename OUTPUT>
    FourierTransformer2<INPUT, OUTPUT>::~FourierTransformer2()
    {
        if ( wtr != NULL )
        {
            gsl_fft_complex_wavetable_free(wtr);
            gsl_fft_complex_workspace_free(wsr);
            gsl_fft_complex_wavetable_free(wtc);
            gsl_fft_complex_workspace_free(wsc);
        }
    }

    template<typename INPUT, typename OUTPUT>
    void FourierTransformer2<INPUT, OUTPUT>::transform(INPUT* input, std::complex<double>* output)
    {
        fourierTransform2D(input, rows, columns, output, this->wtr, this->wsr, this->wtc, this->wsc);
    }

    template<typename INPUT, typename OUTPUT>
    void FourierTransformer2<INPUT, OUTPUT>::transform(std::complex<double>* input, std::complex<double>* output)
    {
        fourierTransform2Dc(input, rows, columns, output, this->wtr, this->wsr, this->wtc, this->wsc);
    }

    template<typename INPUT, typename OUTPUT>
    void FourierTransformer2<INPUT, OUTPUT>::inverseTransform(std::complex<double>* input, OUTPUT* output)
    {
        inverseFourierTransform2D(input, rows, columns, output, this->wtr, this->wsr, this->wtc, this->wsc);
    }

    template<typename INPUT, typename OUTPUT>
    void FourierTransformer2<INPUT, OUTPUT>::updateSize(int rows, int columns)
    {
        if ( this->rows != rows || this->columns != columns )
        {
            if ( wtr != NULL )
            {
                gsl_fft_complex_wavetable_free(wtr);
                gsl_fft_complex_workspace_free(wsr);
                gsl_fft_complex_wavetable_free(wtc);
                gsl_fft_complex_workspace_free(wsc);
            }
            wtr= gsl_fft_complex_wavetable_alloc(columns);
            wsr= gsl_fft_complex_workspace_alloc(columns);
            wtc= gsl_fft_complex_wavetable_alloc(rows);
            wsc= gsl_fft_complex_workspace_alloc(rows);
            this->rows= rows;
            this->columns= columns;
        }
    }
}

#endif	/* _FOURIERTRANSFORMER_H */


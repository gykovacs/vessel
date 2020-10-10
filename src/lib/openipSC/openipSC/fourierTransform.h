/**
 * @file fourierTransform.h
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
 * The methods in this header are wrappers on the GSL Fast Fourier Transform
 * methods.
 */

#ifndef _FOURIER_TRANSFORM_
#define _FOURIER_TRANSFORM_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <complex>
#include <omp.h>

#include <gsl/gsl_fft_complex.h>

/**
 * namespace of all openip objects and functions
 */
namespace openip
{
    /**
     * computes 1 dimensional Fourier-transform of a vector
     * @param vector the vector to transform, contains length element of type INPUT
     * @param length of the vector to transform
     * @param ft Fourier-transformed vector, containing complex elements
     * @param wt wavetable
     * @param ws workspace
     */
    template<typename INPUT>
    void fourierTransform1D(INPUT* vector, unsigned int length, std::complex<double>* ft, gsl_fft_complex_wavetable* wt= NULL, gsl_fft_complex_workspace* ws= NULL);

    /**
     * computes inverse Fourier-transform of the input vector, containing complex<double>
     * elements
     * @param ft complex<double> vector of length length
     * @param length length of the first parameter
     * @param image output vector
     * @param wt wavetable
     * @param ws workspace
     */
    template<typename OUTPUT>
    void inverseFourierTransform1D(std::complex<double>* ft, unsigned int length, OUTPUT* image, gsl_fft_complex_wavetable* wt= NULL, gsl_fft_complex_workspace* ws= NULL);

    /**
     * computes 2 dimensional Fourier-transform of a matrix
     * @param matrix matrix to transform, in row-continuous representation
     * @param rows number of rows in the matrix
     * @param columns number of columns in the matrix
     * @param t Fourier-transformed matrix in row-continuous representation
     * @param wtr wavetable for rows
     * @param wsr workspace for rows
     * @param wtc wavetable for columns
     * @param wsc workspace for columns
     */
    template<typename INPUT>
    void fourierTransform2D(INPUT* matrix, int rows, int columns, std::complex<double>* t, gsl_fft_complex_wavetable* wtr= NULL, gsl_fft_complex_workspace* wsr= NULL, gsl_fft_complex_wavetable* wtc= NULL, gsl_fft_complex_workspace* wsc= NULL);

    void fourierTransform2Dc(std::complex<double>* matrix, int rows, int columns, std::complex<double>* t, gsl_fft_complex_wavetable* wtr= NULL, gsl_fft_complex_workspace* wsr= NULL, gsl_fft_complex_wavetable* wtc= NULL, gsl_fft_complex_workspace* wsc= NULL);

    /**
     * computes the inverse Fourier-transform of the input matrix containing comples<double>
     * elements
     * @param ft complex<double> matrix of rows x columns size, in row-continuous
     * representation
     * @param rows number of rows in ft
     * @param columns number of columns in ft
     * @param matrix output matrix
     * @param wtr wavetable for rows
     * @param wsr workspace for rows
     * @param wtc wavetable for columns
     * @param wsc workspace for columns
     */
    template<typename OUTPUT>
    void inverseFourierTransform2D(std::complex<double>* ft, int rows, int columns, OUTPUT* matrix, gsl_fft_complex_wavetable* wtr= NULL, gsl_fft_complex_workspace* wsr= NULL, gsl_fft_complex_wavetable* wtc= NULL, gsl_fft_complex_workspace* wsc= NULL);
}

template<typename INPUT>
void openip::fourierTransform1D(INPUT* data, unsigned int length, std::complex<double>* ft, gsl_fft_complex_wavetable* wt, gsl_fft_complex_workspace* ws)
{
    double* tmp= (double*)ft;
    for ( unsigned int i= 0; i < length; ++i )
    {
        tmp[i*2]= data[i];
        tmp[i*2+1]= 0;
    }

    bool clearWT= false;
    bool clearWS= false;

    if ( wt == NULL )
    {
        wt= gsl_fft_complex_wavetable_alloc(length);
        clearWT= true;
    }
    if ( ws == NULL )
    {
        ws= gsl_fft_complex_workspace_alloc(length);
        clearWS= true;
    }

    gsl_fft_complex_forward(tmp, 1, length, wt, ws);

    if ( clearWT )
        gsl_fft_complex_wavetable_free(wt);
    if ( clearWS )
        gsl_fft_complex_workspace_free(ws);
}

template<typename OUTPUT>
void openip::inverseFourierTransform1D(std::complex<double>* ft, unsigned int length, OUTPUT* image, gsl_fft_complex_wavetable* wt, gsl_fft_complex_workspace* ws)
{
    double* tmp= (double*)&(ft[0]);

    bool clearWT= false;
    bool clearWS= false;

    if ( wt == NULL )
    {
        wt= gsl_fft_complex_wavetable_alloc(length);
        clearWT= true;
    }
    if ( ws == NULL )
    {
        ws= gsl_fft_complex_workspace_alloc(length);
        clearWS= true;
    }

    gsl_fft_complex_inverse(tmp, 1, length, wt, ws);

    if ( clearWT )
        gsl_fft_complex_wavetable_free(wt);
    if ( clearWS )
        gsl_fft_complex_workspace_free(ws);

    for ( unsigned int i= 0; i < length; ++i )
    {
        image[i]= tmp[i*2];
    }
}

template<typename INPUT>
void openip::fourierTransform2D(INPUT* image, int rows, int columns, std::complex<double>* ft, gsl_fft_complex_wavetable* wtr, gsl_fft_complex_workspace* wsr, gsl_fft_complex_wavetable* wtc, gsl_fft_complex_workspace* wsc)
{
    double* tmp= (double*)(&(ft[0]));
    /*double* tmpi= (double*)malloc(sizeof(double)*rows*columns);
    for ( int i= 0; i < rows*columns; ++i )
        tmpi[i]= image[i];
    double* tmp= (double*)(&(ft[0]));*/

    int n= rows*columns;

    for ( int j= 0; j < n; ++j )
    {
        tmp[j*2]= image[j];
        tmp[j*2+1]= 0;
    }

    /*fftw_plan plan;
    plan= fftw_plan_dft_r2c_2d(rows, columns, (double*)tmpi, (fftw_complex*)tmp, FFTW_FORWARD);
    fftw_execute(plan);

    free(tmpi);*/
    bool clearWTR= false, clearWSR= false, clearWTC= false, clearWSC= false;

    if ( wtr == NULL )
    {
        wtr= gsl_fft_complex_wavetable_alloc(columns);
        clearWTR= true;
    }
    if ( wsr == NULL )
    {
        wsr= gsl_fft_complex_workspace_alloc(columns);
        clearWSR= true;
    }
    if ( wtc == NULL )
    {
        wtc= gsl_fft_complex_wavetable_alloc(rows);
        clearWTC= true;
    }
    if ( wsc == NULL )
    {
        wsc= gsl_fft_complex_workspace_alloc(rows);
        clearWSC= true;
    }

    int num= 4;

    omp_set_num_threads(num);

    gsl_fft_complex_workspace* ws_rows[num];
    for ( int i= 0; i < num; ++i )
        ws_rows[i]= gsl_fft_complex_workspace_alloc(rows);
    gsl_fft_complex_workspace* ws_columns[num];
    for ( int i= 0; i < num; ++i )
        ws_columns[i]= gsl_fft_complex_workspace_alloc(columns);

    
    #pragma omp parallel for
    for ( int i= 0; i < columns; ++i )
        //gsl_fft_complex_forward(tmp + i*2, columns, rows, wtc, wsc);
        gsl_fft_complex_forward(tmp + i*2, columns, rows, wtc, ws_rows[omp_get_thread_num()]);

    #pragma omp parallel for
    for ( int i= 0; i < rows; ++i )
        //gsl_fft_complex_forward(tmp + i*2*columns, 1, columns, wtr, wsr);
        gsl_fft_complex_forward(tmp + i*2*columns, 1, columns, wtr, ws_columns[omp_get_thread_num()]);

    for ( int i= 0; i < num; ++i )
    {
        gsl_fft_complex_workspace_free(ws_rows[i]);
        gsl_fft_complex_workspace_free(ws_columns[i]);
    }

    if ( clearWTR == true )
        gsl_fft_complex_wavetable_free(wtr);
    if ( clearWSR == true )
        gsl_fft_complex_workspace_free(wsr);
    if ( clearWTC == true )
        gsl_fft_complex_wavetable_free(wtc);
    if ( clearWSC == true )
        gsl_fft_complex_workspace_free(wsc);

}

template<typename OUTPUT>
void openip::inverseFourierTransform2D(std::complex<double>* t, int rows, int columns, OUTPUT* image, gsl_fft_complex_wavetable* wtr, gsl_fft_complex_workspace* wsr, gsl_fft_complex_wavetable* wtc, gsl_fft_complex_workspace* wsc)
{
    double* tmp= (double*)t;

    /*double* tmpi= (double*)malloc(sizeof(double)*rows*columns);

    fftw_plan plan;
    plan= fftw_plan_dft_c2r_2d(rows, columns, (fftw_complex*)tmp, (double*)tmpi, FFTW_BACKWARD);
    fftw_execute(plan);

    for ( int i= 0; i < rows*columns; ++i )
        image[i]= tmpi[i];
    free(tmpi);*/
    bool clearWTR= false, clearWSR= false, clearWTC= false, clearWSC= false;

    if ( wtr == NULL )
    {
        wtr= gsl_fft_complex_wavetable_alloc(columns);
        clearWTR= true;
    }
    if ( wsr == NULL )
    {
        wsr= gsl_fft_complex_workspace_alloc(columns);
        clearWSR= true;
    }
    if ( wtc == NULL )
    {
        wtc= gsl_fft_complex_wavetable_alloc(rows);
        clearWTC= true;
    }
    if ( wsc == NULL )
    {
        wsc= gsl_fft_complex_workspace_alloc(rows);
        clearWSC= true;
    }

    int i;
    int num= 4;
    omp_set_num_threads(num);

    gsl_fft_complex_workspace* ws_rows[num];
    for ( int i= 0; i < num; ++i )
        ws_rows[i]= gsl_fft_complex_workspace_alloc(rows);
    gsl_fft_complex_workspace* ws_columns[num];
    for ( int i= 0; i < num; ++i )
        ws_columns[i]= gsl_fft_complex_workspace_alloc(columns);
    #pragma omp parallel for
    for ( i= 0; i < rows; ++i )
    {
        gsl_fft_complex_inverse(tmp + i*2*columns, 1, columns, wtr, ws_columns[omp_get_thread_num()]);
    }

    #pragma omp parallel for
    for ( i= 0; i < columns; i++ )
    {
        gsl_fft_complex_inverse(tmp + 2*i, columns, rows, wtc, ws_rows[omp_get_thread_num()]);
    }

    for ( int i= 0; i < num; ++i )
    {
        gsl_fft_complex_workspace_free(ws_rows[i]);
        gsl_fft_complex_workspace_free(ws_columns[i]);
    }

    if ( clearWTR == true )
        gsl_fft_complex_wavetable_free(wtr);
    if ( clearWSR == true )
        gsl_fft_complex_workspace_free(wsr);
    if ( clearWTC == true )
        gsl_fft_complex_wavetable_free(wtc);
    if ( clearWSC == true )
        gsl_fft_complex_workspace_free(wsc);

    int n= rows*columns;

    for ( int i= 0; i < n; ++i )
        image[i]= tmp[i*2];
}

#endif

/**
 * @file matrixOperations.h
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
 * This file contains functions for matrix operations, based on GNU GSL
 */

#ifndef _MATRIXOPERATIONS_H
#define	_MATRIXOPERATIONS_H

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_eigen.h>

namespace openip
{
    /**
    * invert square matrix
    * @param data data pointer of the matrix, stored in row-major representation
    * @param n size of the matrix
    * @param output output data pointer
    */
    template<typename T>
    void invertSquareMatrix(T* data, int n, T* output);

    /**
    * determine the determinant of a matrix
    * @param data data pointer of the matrix, stored in row-major representation
    * @param n size of the matrix
    * @return the determinant of the matrix
    */
    template<typename T>
    float determinantSquareMatrix(T* data, int n);

    /**
    * compute eigenvectors and eigenvalues of the square matrix
    * @param data data pointer of the matrix, stored in row-major representation
    * @param n size of the matrix
    * @param evect output matrix data pointer, the columns contain the eigenvectors
    * @param eval output vector data pointer, the eigenvalues of the appropriate eigenvectors
    */ 
    template<typename T>
    void computeEigenvectorsAndEigenvalues(T* data, int n, T* evect, T* eval);

    template<typename T>
    void computeEigenvectorsAndEigenvalues(T* data, int n, T* evectors, T* evalues)
    {
        gsl_matrix_view m= gsl_matrix_view_array(data, n, n);
        gsl_matrix_view evec= gsl_matrix_view_array(evectors, n, n);
        gsl_vector_view eval= gsl_vector_view_array(evalues, n);

        gsl_eigen_symmv_workspace* w= gsl_eigen_symmv_alloc(n);

        gsl_eigen_symmv(&m.matrix, &eval.vector, &evec.matrix, w);

        gsl_eigen_symmv_free(w);
    }

    template<typename T>
    void invertSquareMatrix(T* data, int n, T* output)
    {
        double* d1, *d2;
        d1= (double*)malloc(sizeof(double)*n*n);
        d2= (double*)malloc(sizeof(double)*n*n);

        for ( int i= 0; i < n*n; ++i )
            d1[i]= data[i];

        gsl_matrix* in= gsl_matrix_alloc(n, n);
        for ( int i= 0; i < n; ++i )
            for ( int j= 0; j < n; ++j )
            {
                gsl_matrix_set(in, i, j, d1[i*n + j]);
            }
        
        //gsl_matrix_view out= gsl_matrix_view_array(d2, n, n);
        gsl_matrix* out= gsl_matrix_alloc(n, n);

        gsl_permutation *p= gsl_permutation_alloc(n);
        gsl_permutation_init(p);
        int sign;

        int err= gsl_linalg_LU_decomp(in, p, &sign);
        //gsl_linalg_LU_invert(in, p, &(out.matrix));
        if ( err )
        {
            printf("gsl_linalg_LU_decomp failed: %d\n", err);
            fflush(stdout);
        }
        err= gsl_linalg_LU_invert(in, p, out);
        if ( err )
        {
            printf("gsl_linalg_LU_invert failed: %d\n", err);
            fflush(stdout);
        }

        gsl_matrix_free(in);
        gsl_permutation_free(p);

        //for ( int i= 0; i < n*n; ++i )
            //output[i]= d2[i];
        for ( int i= 0; i < n; ++i )
            for ( int j= 0; j < n; ++j )
                output[i*n + j]= gsl_matrix_get(out, i, j);
        gsl_matrix_free(out);
        free(d1);
        free(d2);
    }

    template<typename T>
    float determinantSquareMatrix(T* data, int n)
    {
        gsl_matrix* in= gsl_matrix_alloc(n, n);
        for ( int i= 0; i < n; ++i )
            for ( int j= 0; j < n; ++j )
                gsl_matrix_set(in, i, j, data[i*n + j]);

        gsl_permutation *p= gsl_permutation_alloc(n);
        gsl_permutation_init(p);
        int sign;

        gsl_linalg_LU_decomp(in, p, &sign);
        double det= gsl_linalg_LU_det(in, sign);

        gsl_matrix_free(in);
        gsl_permutation_free(p);

        return det;
    }
    
    template<typename T>
    void solveLinearEquationSystem(T* matrix, int rows, int columns, T* vector, T* output)
    {
        double *dmatrix, *dvector, *doutput;
        dmatrix= (double*)malloc(sizeof(double)*rows*columns);
        dvector= (double*)malloc(sizeof(double)*rows);
	doutput= (double*)malloc(sizeof(double)*columns);

        for ( int i= 0; i < rows*columns; ++i )
            dmatrix[i]= matrix[i];
	for ( int i= 0; i < rows; ++i )
	  dvector[i]= vector[i];

        gsl_matrix* in= gsl_matrix_alloc(rows, columns);
        for ( int i= 0; i < rows; ++i )
            for ( int j= 0; j < columns; ++j )
                gsl_matrix_set(in, i, j, dmatrix[i*columns + j]);
        
	gsl_vector* vect= gsl_vector_alloc(rows);
	for ( int i= 0; i < rows; ++i )
	  gsl_vector_set(vect, i, dvector[i]);
	
	gsl_vector* out= gsl_vector_alloc(columns);

        gsl_permutation *p= gsl_permutation_alloc(columns);
        gsl_permutation_init(p);
        int sign;

        gsl_linalg_LU_decomp(in, p, &sign);
        gsl_linalg_LU_solve(in, p, vect, out);

        gsl_matrix_free(in);
        gsl_permutation_free(p);
	gsl_vector_free(vect);

        for ( int i= 0; i < columns; ++i )
            output[i]= gsl_vector_get(out, i);
        free(dmatrix);
        free(dvector);
	free(doutput);
    }
}

#endif	/* _MATRIXOPERATIONS_H */


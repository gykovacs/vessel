/**
 * @file Interpolation.h
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
 * Interpolation methods are defined in this file.
 */

#ifndef _INTERPOLATION_H
#define	_INTERPOLATION_H

#include <openipDS/Image.h>
#include <openipDS/Transform2.h>
#include <openipLL/imageIO.h>
#include <openipDS/PixelSet1.h>
#include <limits.h>
#include <omp.h>

template<typename T>
T nearestNeighborInterpolate (T* p, double x)
{
    if ( x < 0.5 )
        return p[0];
    return p[1];
}

template<typename T>
T binearestNeighborInterpolate(T* p, double x, double y)
{
    if ( x <= 0.5 && y <= 0.5 )
        return p[0];
    if ( x <= 0.5 && y > 0.5 )
        return p[1];
    if ( x > 0.5 && y <= 0.5 )
        return p[2];
    return p[3];
}

template<typename T>
T cubicInterpolate (T* p, double x) 
{
    return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

template<typename T>
T bicubicInterpolate (T* p, double x, double y) 
{
    T arr[4];
    arr[0] = cubicInterpolate(p, y);
    arr[1] = cubicInterpolate(p + 4, y);
    arr[2] = cubicInterpolate(p + 8, y);
    arr[3] = cubicInterpolate(p + 12, y);
    return cubicInterpolate(arr, x);
}

template<typename T>
T linearInterpolate(T* p, double x)
{
    return p[0] * (1-x) + p[1] * (x);
}

template<typename T>
T bilinearInterpolate(T* p, double x, double y)
{
    T arr[2];
    arr[0]= linearInterpolate(p, y);
    arr[1]= linearInterpolate(p + 2, y);
    
    return linearInterpolate(arr, x);
    //return p[0]*(1-x)*(1-y) + (1-x)*y*p[2] + (1-y)*x*p[1] + x*y*p[3];
}

template<typename T>
T bilinearInterpolate2(T p0, T p1, T p2, T p3, double x, double y)
{
    return p0*(1-x)*(1-y) + (1-x)*y*p1 + (1-y)*x*p2 + x*y*p3;
}


template<typename T>
T tricubicInterpolate (T p[4][4][4], double x, double y, double z) 
{
    T arr[4];
    arr[0] = bicubicInterpolate(p[0], y, z);
    arr[1] = bicubicInterpolate(p[1], y, z);
    arr[2] = bicubicInterpolate(p[2], y, z);
    arr[3] = bicubicInterpolate(p[3], y, z);
    return cubicInterpolate(arr, x);
}

template<typename T>
T nCubicInterpolate (int n, T* p, double coordinates[]) 
{
    if (n == 1) {
        return cubicInterpolate(p, *coordinates);
    }
    else {
        T arr[4];
        int skip = 1 << (n - 1) * 2;
        arr[0] = nCubicInterpolate(n - 1, p, coordinates + 1);
        arr[1] = nCubicInterpolate(n - 1, p + skip, coordinates + 1);
        arr[2] = nCubicInterpolate(n - 1, p + 2*skip, coordinates + 1);
        arr[3] = nCubicInterpolate(n - 1, p + 3*skip, coordinates + 1);
        return cubicInterpolate(arr, *coordinates);
    }
}

/**
* Laplacian interpolation
* @param input input image
* @param base base points
* @param beta method parameter
* @param eps convergence limit
* @param mask the operation is performed in the foreground (non 0) region of the mask
*/
template<typename T>
void interpolationLaplace(Image<T>& input, Image<unsigned char>& base, float beta= 1, float eps= 1000, Image<unsigned char>* mask= NULL);

/**
* weighted interpolation
* @param input input/output image
* @param base base points
*/
template<typename T>
void interpolationWeighted(Image<T>& input, PixelSet1& base);

template<typename T>
void interpolationLaplace(Image<T>& input, Image<unsigned char>& base, float beta, float eps, Image<unsigned char>* mask)
{
    Image<float> tmp(input.columns, input.rows);
    Image<float> tmp2(input.columns, input.rows);

    tmp= input;
    tmp2= tmp;

    float diff= eps + 1;
    float d;

    int k= 0;
    if ( mask == NULL )
    {
        while( fabs(diff) > eps )
        {
            diff= 0;

            #pragma omp parallel for
            for ( int i= tmp.columns; i < input.n - tmp.columns-1; ++i )
                    if ( !base(i) )
                    {
                        d= tmp(i - tmp.columns) + tmp(i+1) + tmp(i-1) + tmp(i + tmp.columns) - 4*tmp(i);
                        //d*= 1.1;
                        d/= 4.0;
                        tmp2(i)+= d;
                        diff+= fabs(d);
                    }

            tmp= tmp2;
            printf("%f ", diff); fflush(stdout);
            ++k;
            if ( (k % 10) == 0 )
                writeImage("tmp.jpg", tmp, tmp, tmp);
        }
        for ( int i= 0; i < input.n; ++i )
            if ( base(i) == 0 )
                input(i)= tmp(i);
    }
    else
    {
        while( fabs(diff) > eps )
        {
            diff= 0;

            #pragma omp parallel for
            for ( int i= tmp.columns; i < input.n - tmp.columns-1; ++i )
                    if ( base(i) == 0 && (*mask)(i) > 0 )
                    {
                        d= tmp(i - tmp.columns) + tmp(i+1) + tmp(i-1) + tmp(i + tmp.columns) - 4*tmp(i);
                        //d*= 1.1;
                        d/= 4.0;
                        tmp2(i)+= d;
                        diff+= fabs(d);
                    }

            tmp= tmp2;
            printf("%f ", diff); fflush(stdout);
            ++k;
            if ( (k % 10) == 0 )
                writeImage("tmp.jpg", tmp, tmp, tmp);
        }
        for ( int i= 0; i < input.n; ++i )
            if ( base(i) == 0 && (*mask)(i) > 0 )
                input(i)= tmp(i);
    }


}

template<typename T>
void interpolationWeighted(Image<T>& input, PixelSet1& base)
{
    Image<float> tmp(input.columns, input.rows);

    int x, y;
    for ( int i= 0; i < input.columns; ++i )
        for ( int j= 0; j < input.rows; ++j )
            tmp(i,j)= sqrt(i*i + j*j);

    float res;
    float sum;
    float div;

    int x1, y1;
    for ( int i= 0; i < input.columns; ++i )
    {
        for ( int j= 0; j < input.rows; ++j )
        {
            if ( base.find(j*input.columns + i) == base.end() )
            {
               res= 0;
                sum= 0;

                for ( PixelSet1::iterator psit= base.begin(); psit != base.end(); ++psit )
                {
                    x1= *psit % input.columns;
                    y1= *psit / input.columns;

                    div= 1.0/tmp(abs(i - x1), abs(j - y1));
                    div*= div;
                    res+= div * input(*psit);
                    sum+= div;
                }
                input(i,j)= (T)(res / sum);
            }
        }
    }
}

#endif	/* _INTERPOLATION_H */


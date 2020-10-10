/**
 * @file mathFunctions.h
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
 * Some usual mathematical functions are defined in this file.
 */

#ifndef _MATH_FUNCTIONS_H_
#define _MATH_FUNCTIONS_H_

#include <math.h>
#include <string>
#include <typeinfo>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y) )
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y) )

namespace openip
{
    double sech(double d);
    
    std::string type(const std::type_info& ti);
    
    /**
     * computes the values of a 1D Gaussian density function
     * @param x distance from the mean
     * @param sd standard deviation
     * @param alpha alpha parameter
     * @param beta beta parameter
     * @param delta delta parameter
     * @param gamma gamma parameter
     * @return the density value
     */
    double gauss1(float x, float sd, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

    /**
     * computes the values of a 2D Gaussian density function
     * @param x x coordinate of the point in which the density is computed
     * @param y y coordiante of the point in which the density is computed
     * @param sd standard deviation
     * @param alpha alpha parameter
     * @param beta beta parameter
     * @param delta delta parameter
     * @param gamma gamma parameter
     * @return the density value
     */
    double gauss2(float x, float y, float sd, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

    /**
    * computes the first derivative of the gaussian comulative probability function along x
    * @param x x coordinate
    * @param y y coordinate
    * @param sd deviation
    * @param alpha alpha parameter
    * @param beta beta parameter
    * @param delta delta parameter
    * @param gamma gamma parameter
    */
    double gauss2_dx(float x, float y, float sd, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

    /**
    * computes the first derivative of the gaussian comulative probability function along y
    * @param x x coordinate
    * @param y y coordiante
    * @param sd deviance
    * @param alpha alpha parameter
    * @param beta bate parameter
    * @param delta delta parameter
    * @param gamma gamma paraneter
    */
    double gauss2_dy(float x, float y, float sd, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

    /**
    * computes the second partial derivative of the gaussian comulative probability function along x and y
    * @param x x coordinate
    * @param y y coordinate
    * @param sd deviation
    * @param alpha alpha parameter
    * @param beta beta parameter
    * @param delta delta parameter
    * @param gamma gamma parameter
    */
    double gauss2_dxy(float x, float y, float sd, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

    /**
    * computes the second partial derivative of the gaussian comulative probability function along x and x
    * @param x x coordinate
    * @param y y coordinate
    * @param sd deviation
    * @param alpha alpha parameter
    * @param beta beta parameter
    * @param delta delta parameter
    * @param gamma gamma parameter
    */
    double gauss2_dxx(float x, float y, float sd, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

    /**
    * computes the second partial derivative of the gaussian comulative probability function along y and y
    * @param x x coordinate
    * @param y y coordinate
    * @param sd deviation
    * @param alpha alpha parameter
    * @param beta beta parameter
    * @param delta delta parameter
    * @param gamma gamma parameter
    */
    double gauss2_dyy(float x, float y, float sd, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

    double logfilter(float x, float y, float sigma);

    float fakt(int n);

    float combination(int n, int k);
    
    float log2(float x);

    template<typename T> int sgn(T val)
    {
        return (T(0) < val) - (val < T(0));
    }
}

#endif

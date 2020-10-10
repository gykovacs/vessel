/**
 * @file GaussianScaleSpace.h
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
 * The GaussianScaleSpace represents a scale space object generated with
 * Gaussian filters.
 */

#ifndef _GAUSSIANSCALESPACE_H
#define	_GAUSSIANSCALESPACE_H

#include <openipDS/ScaleSpace.h>
#include <openipDS/Filter2s.h>
#include <string.h>

namespace openip
{
    /**
    * GaussianScaleSpace represents a Gaussian scale space object
    */
    template<typename T>
    class GaussianScaleSpace: public ScaleSpace<T>
    {
    public:
	/**
	* constructor
	* @param size kernel size of the Gaussian filter
	* @param sigma standard deviation of the Gaussian filter
	* @param step step of kernel increase
	* @param scales number of scales
	*/
        GaussianScaleSpace(int size, float sigma, float step, int scales);

	/**
	* copy constructor
	* @param gss instance to copy
	*/
        GaussianScaleSpace(const GaussianScaleSpace& gss);

	/**
	* destructor
	*/
        ~GaussianScaleSpace();

	/**
	* initializes the scale space
	* @param input input image (lowes level)
	* @param c Cache2 object containing the Image objects from whom the scale space is built up
	*/
        //void init(Image<T>& input, Cache2* c);

	/**
	* kernel size of the used filter
	*/
        int size;
        
        /**
        * standard deviation of the used filter
        */
        float sigma;
        
        /**
        * step of increasing the standard deviation
        */
        float step;
        
        /**
        * number of scales in the scale space
        */
        int scales;
    };

    template<typename T>
    GaussianScaleSpace<T>::GaussianScaleSpace(int size, float sigma, float step, int scales)
    : ScaleSpace<T>()
    {
        this->size= size;
        this->sigma= sigma;
        this->step= step;
        this->scales= scales;
    }

    template<typename T>
    GaussianScaleSpace<T>::GaussianScaleSpace(const GaussianScaleSpace& gss)
    {
        this->size= gss.size;
        this->sigma= gss.sigma;
        this->step= gss.step;
        this->scales= gss.scales;
    }

    template<typename T>
    GaussianScaleSpace<T>::~GaussianScaleSpace()
    {
    }

    /*template<typename T>
    void GaussianScaleSpace<T>::init(Image<T>& input, Cache2* c)
    {
        this->ScaleSpace<T>::init(input, c, scales);

        int sigma= this->sigma;
        for ( int i= 0; i < scales; ++i )
        {
            GaussianFilter2<T, T> gf(sigma);
            gf.apply(input,*(*this)[i], *c);
            if ( sigma == 0 )
                sigma= 1;
            else
                sigma*= step;
        }

        char filename[10];

        for ( int i= 0; i < scales; ++i )
        {
            sprintf(filename, "gf%d.jpg", i);
            writeImage(filename, (*this)[i], (*this)[i], (*this)[i]);
        }
    }*/
}

#endif	/* _GAUSSIANSCALESPACE_H */


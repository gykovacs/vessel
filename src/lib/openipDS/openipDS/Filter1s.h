/**
 * @file Filter1s.h
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
 * This file contains some common filters.
 */

#ifndef _FILTER1S_H
#define	_FILTER1S_H

#include <openipDS/Filter1.h>
#include <openipDS/Vector.h>

namespace openip
{
    /**
    * GaussianFilter1_3 represents an optimized 1d Gaussian Filter with kernel size 3
    */
    template<typename INPUT, typename OUTPUT>
    class GaussianFilter1_3: public Filter1<INPUT, OUTPUT, float>
    {
    public:
        using Filter1<INPUT, OUTPUT, float>::apply;

	/**
	* GaussianFilter1_3 constructor
	* @param sigma standard deviation
	* @param alpha distribution transformation parameter
	* @param beta distribution transformation parameter
	* @param delta distribution transformation parameter
	* @param gamma distribution transformation parameter
	*/
        GaussianFilter1_3(float sigma, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

        /**
         * applies the filter in the position n
         * @param in input image
         * @param n position in row-continuous representation
         * @return the filter response in n
         */
        virtual OUTPUT apply(Vector<INPUT>& in, Pixel1 n, Vector<unsigned char>* support= NULL);

        /**
         * computes min max values
         */
        void computeMinMax();

	/**
	* weight of position 0
	*/
        float w1;
        
        /**
        * weight of position -1 and 1
        */
        float w2;
        
        /**
        * step of positions
        */
        int pos;
    };

    template<typename INPUT, typename OUTPUT>
    GaussianFilter1_3<INPUT, OUTPUT>::GaussianFilter1_3(float sigma, float alpha, float beta, float delta, float gamma)
    {
        w1= gauss1(0, sigma, alpha, beta, delta, gamma);
        w2= gauss1(1, sigma, alpha, beta, delta, gamma);
        float sum= w1 + 2*w2;
        w1/=sum;
        w2/=sum;
        pos= 1;
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT GaussianFilter1_3<INPUT, OUTPUT>::apply(Vector<INPUT>& input, Pixel1 n, Vector<unsigned char>* support)
    {
        return w1 * input(n) + w2 * ((int)(input(n - pos)) + input(n + pos));
    }

    template<typename INPUT, typename OUTPUT>
    void GaussianFilter1_3<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= -pos;
        this->max= pos;
    }

    /**
    * GaussianFilter1_5 represents an optimized 1D Gaussian filter of kernel size 5
    */
    template<typename INPUT, typename OUTPUT>
    class GaussianFilter1_5: public Filter1<INPUT, OUTPUT, float>
    {
    public:
        using Filter1<INPUT, OUTPUT, float>::apply;
	
	/**
	* GaussianFilter1_5 constructor
	* @param sigma standard deviation
	* @param alpha distribution transformation parameter
	* @param beta distribution transformation parameter
	* @param delta distribution transformation parameter
	* @param gamma distribution transformation parameter
	*/
        GaussianFilter1_5(float sigma, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

        /**
         * applies the filter in the position n
         * @param in input image
         * @param n position in row-continuous representation
         * @return the filter response in n
         */
        virtual OUTPUT apply(Vector<INPUT>& in, Pixel1 n, Vector<unsigned char>* support= NULL);

        /**
         * computes min max values
         */
        void computeMinMax();

	/**
	* weight of position 0
	*/
        float w1;
        
        /**
        * weight of position -1 and 1
        */
        float w2;
        
        /**
        * wieght of position -2 and 2
        */
        float w3;
        
        /**
        * step to positions -1 and 1
        */
        int pos1;
        
        /**
        * step to positions -2 and 2
        */
        int pos2;
    };

    template<typename INPUT, typename OUTPUT>
    GaussianFilter1_5<INPUT, OUTPUT>::GaussianFilter1_5(float sigma, float alpha, float beta, float delta, float gamma)
    {
        w1= gauss1(0, sigma, alpha, beta, delta, gamma);
        w2= gauss1(1, sigma, alpha, beta, delta, gamma);
        w3= gauss1(2, sigma);
        float sum= w1 + 2*w2 + 2*w3;
        w1/=sum;
        w2/=sum;
        w3/=sum;
        pos1= 1;
        pos2= 2;
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT GaussianFilter1_5<INPUT, OUTPUT>::apply(Vector<INPUT>& input, Pixel1 n, Vector<unsigned char>* support)
    {
        return w1 * input(n) + w2 * ((int)(input(n - pos1)) + input(n + pos1)) + w3 * ((int)(input(n - pos2)) + input(n + pos2));
    }

    template<typename INPUT, typename OUTPUT>
    void GaussianFilter1_5<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= -pos2;
        this->max= pos2;
    }

    /**
     * GaussianFilter1 represents a Gaussian filter applicable to Vectors
     */
    template<typename INPUT, typename OUTPUT>
    class GaussianFilter1: public Filter1<INPUT, OUTPUT, float>
    {
    public:
        using Filter1<INPUT, OUTPUT, float>::apply;
	using Filter1<INPUT, OUTPUT, float>::normalize;

        /**
         * GaussianFilter1 constructor
         * @param size size of the filter
         * @param sigma standard deviation of the Gaussian distribution
         * @param alpha distribution transformation parameter
         * @param beta distribution transformation parameter
         * @param delta distribution transformation parameter
         * @param gamma distribution transformation parameter
         */
        GaussianFilter1(float sigma, int size= -1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0);

	virtual ~GaussianFilter1();
	
        /**
         * applies the filter in the position n
         * @param in input image
         * @param n position in row-continuous representation
         * @return the filter response in n
         */
        virtual OUTPUT apply(Vector<INPUT>& in, Pixel1 n, Vector<unsigned char>* support= NULL);

        /**
         * computes min max values
         */
        void computeMinMax();
	
	int stride;
    };

    template<typename INPUT, typename OUTPUT>
    GaussianFilter1<INPUT, OUTPUT>::GaussianFilter1(float sigma, int size, float alpha, float beta, float delta, float gamma)
    : Filter1<INPUT, OUTPUT, float>()
    {
        float sum= 0, g= 0;

        if ( size == -1 )
        {
            int i= 0;
            while( gauss1(i, sigma, alpha, beta, delta, gamma) > 0.00001 )
                ++i;
            size= i-1;
        }

        for ( int i= -size/2; i <= 0; ++i )
        {
            g= gauss1(i, sigma, alpha, beta, delta, gamma);
            if ( i != 0 )
            {
                sum+= 2*g;
            }
            else
            {
                sum+= g;
            }
        }

        for ( int i= -size/2; i <= 0; ++i )
        {
            g= gauss1(i, sigma, alpha, beta, delta, gamma);

            if ( i != 0 )
                this->insert(PositionWeightPair<float>(i, g/sum));
            else if ( i == 0 )
                this->insert(PositionWeightPair<float>(i, g/2.0/sum));
        }

        this->normalize();
        this->computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    GaussianFilter1<INPUT, OUTPUT>::~GaussianFilter1()
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    OUTPUT GaussianFilter1<INPUT, OUTPUT>::apply(Vector<INPUT>& in, Pixel1 n, Vector<unsigned char>* support)
    {
        float f= 0;
        float sum= 0;

        if ( support == NULL )
        {
            for( typename Filter1<INPUT, OUTPUT, float>::iterator mit= this->begin(); mit != this->end(); ++mit)
            {
                f+= (float(in(n + (*mit).first)) + float(in(n - (*mit).first))) * ((*mit).second);
                sum+= 2*(*mit).second;
            }
        }
        else
        {
            for( typename Filter1<INPUT, OUTPUT, float>::iterator mit= this->begin(); mit != this->end(); ++mit)
            {
                if ( (*support)(n + (*mit).first) > 0 )
                {
                    f+= (float(in(n + (*mit).first)) + float(in(n - (*mit).first))) * ((*mit).second);
                    sum+= (*mit).second;
                }
            }
        }

        return (OUTPUT)f;
    }

    template<typename INPUT, typename OUTPUT>
    void GaussianFilter1<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= -(this->size()+1);
        this->max= -this->min;
    }

    /**
     * prints the description of the parameter GaussianFilter1 object to the parameter output stream
     * @param o parameter output stream
     * @param v GaussianFilter1 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT>
    std::ostream& operator<<(std::ostream& o, const GaussianFilter1<INPUT, OUTPUT>& g)
    {
        o << "[GaussianFilter1 - size: ";
        o << g.size();
        o << " elements: ";
        for ( typename Filter1<INPUT, OUTPUT, float>::const_iterator it = g.begin(); it != g.end(); ++it )
        {
            o << *it;
            o << ", ";
        }
        o << "]";

        return o;
    }
}

#endif	/* _FILTER1S_H */


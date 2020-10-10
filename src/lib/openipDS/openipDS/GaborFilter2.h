/**
 * @file GaborFilter2.h
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
 * The GaborFilter2 and GaborFilter2I represent the 2d Gabor filters.
 */

#ifndef _GABORFILTER2_H
#define	_GABORFILTER2_H

#include <float.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#include <openipDS/Filter2.h>
#include <openipDS/mathFunctions.h>
#include <openipLL/imageIO.h>
#include <openipDS/FilterSet2.h>
#include <openipDS/ShiftedFeatureFilter2.h>

#define CUT_FILTER 1
#define HALF_FILTER 0
#define MINUS_ONE 0
#define EXPONENT 0
#define _EXPONENT 0.2
#define ZERO_MEAN 1
#define NORMALIZE 0
#define FIX_SIZE 0
#define NSIGMA 1

namespace openip
{
    /**************************************************************************************************/
    /****************************** GaborFilterR2                    **********************************/
    /**************************************************************************************************/

    /**
    * GaborFilter2 represents a 2D Gabor filter
    */
    template<typename INPUT, typename OUTPUT>
    class GaborFilterR2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
	/**
	* GaborFilter2 default constructor
	*/
        GaborFilterR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        GaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma);
        
        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        GaborFilterR2(const GaborFilterR2& gf);
        
        /**
        * destructor
        */
        ~GaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    GaborFilterR2<INPUT, OUTPUT>::GaborFilterR2()
        : Filter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    GaborFilterR2<INPUT, OUTPUT>::GaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma)
        : Filter2<INPUT, OUTPUT, float>()
    {
        stride= 4000;
        float sigma_x= sigma;
        float sigma_y= sigma/gamma;

        int nstds= 3;
        float xmax= max(fabs(nstds*sigma_x*cos(theta)), fabs(nstds*sigma_y*sin(theta)));
        xmax= ceil(max(1.0f, xmax));
        float ymax= max(fabs(nstds*sigma_x*sin(theta)), fabs(nstds*sigma_y*cos(theta)));
        ymax= ceil(max(1.0f, ymax));

#if FIX_SIZE
        xmax= FIX_SIZE/2;
        ymax= xmax;
#endif

        float xmin= -xmax;
        float ymin= -ymax;
#if HALF_FILTER
        ymin= 0;
#endif

        float weight;
        float x_theta;
        float y_theta;

        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                weight= cos(2*M_PI/lambda*x_theta + psi);
#if MINUS_ONE
                weight-= 1;
#endif
#if EXPONENT
                weight= weight > 0 ? pow(fabs(weight), _EXPONENT) : -pow(fabs(weight), _EXPONENT);
#endif
                weight*= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y)));
#if CUT_FILTER
                /*if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )*/
                if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                    continue;
#endif

                this->addElement(i, j, weight);
            }
        this->angle= theta;
        this->computeMinMax();
#if NORMALIZE
        this->normalize();
#endif
#if ZERO_MEAN
        this->zeroMean();
#endif

        std::stringstream ss;
        ss << "GaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GaborFilterR2<INPUT, OUTPUT>::GaborFilterR2(const GaborFilterR2& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    GaborFilterR2<INPUT, OUTPUT>::~GaborFilterR2()
    {
    }

    /**
    * GaborFilter2I represents the imaginary part of the filter
    */
    template<typename INPUT, typename OUTPUT>
    class GaborFilterI2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;


    /**
    * GaborFilter2I default constructor
    */
        GaborFilterI2();

        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        GaborFilterI2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        GaborFilterI2(const GaborFilterI2& gf);

        /**
        * destructor
        */
        ~GaborFilterI2();
    };

    template<typename INPUT, typename OUTPUT>
    GaborFilterI2<INPUT, OUTPUT>::GaborFilterI2()
        : Filter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    GaborFilterI2<INPUT, OUTPUT>::GaborFilterI2(float sigma, float theta, float lambda, float psi, float gamma)
        : Filter2<INPUT, OUTPUT, float>()
    {
        stride= 4000;
        float sigma_x= sigma;
        float sigma_y= sigma/gamma;

        int nstds= 3;
        float xmax= max(fabs(nstds*sigma_x*cos(theta)), fabs(nstds*sigma_y*sin(theta)));
        xmax= ceil(max(1.0f, xmax));
        float ymax= max(fabs(nstds*sigma_x*sin(theta)), fabs(nstds*sigma_y*cos(theta)));
        ymax= ceil(max(1.0f, ymax));

#if FIX_SIZE
        xmax= FIX_SIZE/2;
        ymax= xmax;
#endif

        float xmin= -xmax;
        float ymin= -ymax;

#if HALF_FILTER
        ymin= 0;
#endif

        float weight;
        float x_theta;
        float y_theta;

        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                weight= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y))) * sin(2*M_PI/lambda*x_theta + psi);
                //weight= -1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                  //      + y_theta*y_theta/(sigma_y*sigma_y))) * exp(-x_theta*x_theta/(lambda*lambda) + psi);
#if CUT_FILTER
                /*if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )*/
                if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                    continue;
#endif

                this->addElement(i, j, weight);
            }
        this->computeMinMax();
        std::stringstream ss;
        ss << "GaborFilter2I" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GaborFilterI2<INPUT, OUTPUT>::GaborFilterI2(const GaborFilterI2& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    GaborFilterI2<INPUT, OUTPUT>::~GaborFilterI2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class FixedSizeGaborFilterR2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
    /**
    * GaborFilter2 default constructor
    */
        FixedSizeGaborFilterR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        FixedSizeGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        FixedSizeGaborFilterR2(const FixedSizeGaborFilterR2& gf);

        /**
        * destructor
        */
        ~FixedSizeGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    FixedSizeGaborFilterR2<INPUT, OUTPUT>::FixedSizeGaborFilterR2()
        : Filter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeGaborFilterR2<INPUT, OUTPUT>::FixedSizeGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma)
        : Filter2<INPUT, OUTPUT, float>()
    {
        stride= 4000;
        float sigma_x= sigma;
        float sigma_y= sigma/gamma;

        int nstds= 3;
        float xmax= max(fabs(nstds*sigma_x), fabs(nstds*sigma_y));
        xmax= ceil(max(1.0f, xmax));
        float ymax= max(fabs(nstds*sigma_x), fabs(nstds*sigma_y));
        ymax= ceil(max(1.0f, ymax));

#if FIX_SIZE
        xmax= FIX_SIZE/2;
        ymax= xmax;
#endif

        float xmin= -xmax;
        float ymin= -ymax;
#if HALF_FILTER
        ymin= 0;
#endif

        float weight;
        float x_theta;
        float y_theta;

        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                weight= cos(2*M_PI/lambda*x_theta + psi);
#if MINUS_ONE
                weight-= 1;
#endif
#if EXPONENT
                weight= weight > 0 ? pow(fabs(weight), _EXPONENT) : -pow(fabs(weight), _EXPONENT);
#endif
                weight*= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y)));
#if CUT_FILTER
                /*if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )*/
                if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                    continue;
#endif

                this->addElement(i, j, weight);
            }
        this->angle= theta;
        this->computeMinMax();
#if NORMALIZE
        this->normalize();
#endif
#if ZERO_MEAN
        this->zeroMean();
#endif

        std::stringstream ss;
        ss << "FixedSizeGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeGaborFilterR2<INPUT, OUTPUT>::FixedSizeGaborFilterR2(const FixedSizeGaborFilterR2& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeGaborFilterR2<INPUT, OUTPUT>::~FixedSizeGaborFilterR2()
    {
    }

    /**************************************************************************************************/
    /****************************** End of GaborFilterR2             **********************************/
    /**************************************************************************************************/

    /**************************************************************************************************/
    /****************************** CorrelationGaborFilterR2             ******************************/
    /**************************************************************************************************/

    template<typename INPUT, typename OUTPUT>
    class CorrelationGaborFilterR2: public CorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
    /**
    * GaborFilter2 default constructor
    */
        CorrelationGaborFilterR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        CorrelationGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        CorrelationGaborFilterR2(const CorrelationGaborFilterR2& gf);

        /**
        * destructor
        */
        ~CorrelationGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    CorrelationGaborFilterR2<INPUT, OUTPUT>::CorrelationGaborFilterR2()
        : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationGaborFilterR2<INPUT, OUTPUT>::CorrelationGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma)
        : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
        stride= 4000;
        float sigma_x= sigma;
        float sigma_y= sigma/gamma;

        int nstds= 3;
        float xmax= max(fabs(nstds*sigma_x*cos(theta)), fabs(nstds*sigma_y*sin(theta)));
        xmax= ceil(max(1.0f, xmax));
        float ymax= max(fabs(nstds*sigma_x*sin(theta)), fabs(nstds*sigma_y*cos(theta)));
        ymax= ceil(max(1.0f, ymax));

#if FIX_SIZE
        xmax= FIX_SIZE/2;
        ymax= xmax;
#endif

        float xmin= -xmax;
        float ymin= -ymax;

#if HALF_FILTER
        ymin= 0;
#endif

        float weight;
        float x_theta;
        float y_theta;

        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                weight= cos(2*M_PI/lambda*x_theta + psi);
#if MINUS_ONE
                weight-= 1;
#endif
#if EXPONENT
                weight= weight > 0 ? pow(fabs(weight), _EXPONENT) : -pow(fabs(weight), _EXPONENT);
#endif
                weight*= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y)));
#if CUT_FILTER
                /*if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.00001 )*/
                if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                    continue;
#endif

                this->addElement(i, j, weight);
            }
        this->angle= theta;
        this->computeMinMax();
#if NORMALIZE
        this->normalize();
#endif
#if ZERO_MEAN
        this->zeroMean();
#endif
        std::stringstream ss;
        ss << "CorrelationGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationGaborFilterR2<INPUT, OUTPUT>::CorrelationGaborFilterR2(const CorrelationGaborFilterR2& gf)
    : CorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationGaborFilterR2<INPUT, OUTPUT>::~CorrelationGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class FixedSizeCorrelationGaborFilterR2: public CorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
    /**
    * GaborFilter2 default constructor
    */
        FixedSizeCorrelationGaborFilterR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        FixedSizeCorrelationGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        FixedSizeCorrelationGaborFilterR2(const FixedSizeCorrelationGaborFilterR2& gf);

        /**
        * destructor
        */
        ~FixedSizeCorrelationGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>::FixedSizeCorrelationGaborFilterR2()
        : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>::FixedSizeCorrelationGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma)
        : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
        stride= 4000;
        float sigma_x= sigma;
        float sigma_y= sigma/gamma;

        int nstds= 3;
        float xmax= max(fabs(nstds*sigma_x), fabs(nstds*sigma_y));
        xmax= ceil(max(1.0f, xmax));
        float ymax= max(fabs(nstds*sigma_x), fabs(nstds*sigma_y));
        ymax= ceil(max(1.0f, ymax));

#if FIX_SIZE
        xmax= FIX_SIZE/2;
        ymax= xmax;
#endif

        float xmin= -xmax;
        float ymin= -ymax;

#if HALF_FILTER
        ymin= 0;
#endif

        float weight;
        float x_theta;
        float y_theta;

        //float minWeight= FLT_MAX;
        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                weight= cos(2*M_PI/lambda*x_theta + psi);
#if MINUS_ONE
                weight-= 1;
#endif
#if EXPONENT
                weight= weight > 0 ? pow(fabs(weight), _EXPONENT) : -pow(fabs(weight), _EXPONENT);
#endif
                weight*= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y)));
#if CUT_FILTER
                /*if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )*/
                if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                    continue;
#endif

                this->addElement(i, j, weight);
            }
        this->angle= theta;
        this->computeMinMax();
#if NORMALIZE
        this->normalize();
#endif
#if ZERO_MEAN
        this->zeroMean();
#endif

        std::stringstream ss;
        ss << "FixedSizeCorrelationGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>::FixedSizeCorrelationGaborFilterR2(const FixedSizeCorrelationGaborFilterR2& gf)
    : CorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>::~FixedSizeCorrelationGaborFilterR2()
    {
    }

    /**************************************************************************************************/
    /****************************** End of CorrelationGaborFilterR2               *********************/
    /**************************************************************************************************/

    /**************************************************************************************************/
    /****************************** CCorrelationGaborFilterR2               *********************/
    /**************************************************************************************************/

    template<typename INPUT, typename OUTPUT>
    class CCorrelationGaborFilterR2: public CCorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
        using CCorrelationFilter2<INPUT, OUTPUT, float>::meanF;
        using CCorrelationFilter2<INPUT, OUTPUT, float>::devF;
    /**
    * GaborFilter2 default constructor
    */
        CCorrelationGaborFilterR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        CCorrelationGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        CCorrelationGaborFilterR2(const CCorrelationGaborFilterR2& gf);

        /**
        * destructor
        */
        ~CCorrelationGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    CCorrelationGaborFilterR2<INPUT, OUTPUT>::CCorrelationGaborFilterR2()
        : CCorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationGaborFilterR2<INPUT, OUTPUT>::CCorrelationGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma)
        : CCorrelationFilter2<INPUT, OUTPUT, float>()
    {
        stride= 4000;
        float sigma_x= sigma;
        float sigma_y= sigma/gamma;

        int nstds= 3;
        float xmax= max(fabs(nstds*sigma_x*cos(theta)), fabs(nstds*sigma_y*sin(theta)));
        xmax= ceil(max(1.0f, xmax));
        float ymax= max(fabs(nstds*sigma_x*sin(theta)), fabs(nstds*sigma_y*cos(theta)));
        ymax= ceil(max(1.0f, ymax));

#if FIX_SIZE
        xmax= FIX_SIZE/2;
        ymax= xmax;
#endif
        xmax = xmax > ymax ? xmax : ymax;
        ymax= ymax > xmax ? ymax : xmax;

        float xmin= -xmax;
        float ymin= -ymax;

#if HALF_FILTER
        ymin= 0;
#endif

        float weight;
        float x_theta;
        float y_theta;

        //printf("%f %f\n", xmax, ymax);
        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                weight= cos(2*M_PI/lambda*x_theta + psi);
#if MINUS_ONE
                weight-= 1;
#endif
#if EXPONENT
                weight= weight > 0 ? pow(fabs(weight), _EXPONENT) : -pow(fabs(weight), _EXPONENT);
#endif
                weight*= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y)));
#if CUT_FILTER
                /*if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )*/
                if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                    continue;
#endif

                this->addElement(i, j, weight);
            }
        this->angle= theta;
        this->computeMinMax();
#if NORMALIZE
        this->normalize();
#endif
#if ZERO_MEAN
        this->zeroMean();
#endif
        std::stringstream ss;
        ss << "CCorrelationGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        for ( typename CCorrelationFilter2<INPUT, OUTPUT, float>::fIt fit= this->begin(); fit != this->end(); ++fit )
            meanF+= fit->second;

        meanF/= this->size();

        for ( typename CCorrelationFilter2<INPUT, OUTPUT, float>::fIt fit= this->begin(); fit != this->end(); ++fit )
            devF+= (fit->second - meanF) * (fit->second - meanF);

        devF= sqrt(devF);
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationGaborFilterR2<INPUT, OUTPUT>::CCorrelationGaborFilterR2(const CCorrelationGaborFilterR2& gf)
    : CCorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationGaborFilterR2<INPUT, OUTPUT>::~CCorrelationGaborFilterR2()
    {
    }


    template<typename INPUT, typename OUTPUT>
    class FixedSizeCCorrelationGaborFilterR2: public CCorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
    /**
    * GaborFilter2 default constructor
    */
        FixedSizeCCorrelationGaborFilterR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        FixedSizeCCorrelationGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        FixedSizeCCorrelationGaborFilterR2(const FixedSizeCCorrelationGaborFilterR2& gf);

        /**
        * destructor
        */
        ~FixedSizeCCorrelationGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>::FixedSizeCCorrelationGaborFilterR2()
        : CCorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>::FixedSizeCCorrelationGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma)
        : CCorrelationFilter2<INPUT, OUTPUT, float>()
    {
        stride= 4000;
        float sigma_x= sigma;
        float sigma_y= sigma/gamma;

        int nstds= 3;
        float xmax= max(fabs(nstds*sigma_x), fabs(nstds*sigma_y));
        xmax= ceil(max(1.0f, xmax));
        float ymax= max(fabs(nstds*sigma_x), fabs(nstds*sigma_y));
        ymax= ceil(max(1.0f, ymax));

#if FIX_SIZE
        xmax= FIX_SIZE/2;
        ymax= xmax;
#endif

        float xmin= -xmax;
        float ymin= -ymax;

#if HALF_FILTER
        ymin= 0;
#endif

        float weight;
        float x_theta;
        float y_theta;

        //float minWeight= FLT_MAX;
        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                weight= cos(2*M_PI/lambda*x_theta + psi);
#if MINUS_ONE
                weight-= 1;
#endif
#if EXPONENT
                weight= weight > 0 ? pow(fabs(weight), _EXPONENT) : -pow(fabs(weight), _EXPONENT);
#endif
                weight*= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y)));
#if CUT_FILTER
                /*if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )*/
                if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                    continue;
#endif

                this->addElement(i, j, weight);
            }
        this->angle= theta;
        this->computeMinMax();
#if NORMALIZE
        this->normalize();
#endif
#if ZERO_MEAN
        this->zeroMean();
#endif

        std::stringstream ss;
        ss << "FixedSizeCCorrelationGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>::FixedSizeCCorrelationGaborFilterR2(const FixedSizeCCorrelationGaborFilterR2& gf)
    : CCorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>::~FixedSizeCCorrelationGaborFilterR2()
    {
    }

    /**************************************************************************************************/
    /****************************** End of CCorrelationGaborFilterR2               ********************/
    /**************************************************************************************************/

    /**
     * GaborFilter2Magnitude feature
     */
    template<typename INPUT, typename OUTPUT>
    class GaborFilter2Magnitude: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::min;
        using Filter2<INPUT, OUTPUT, float>::max;


	/**
	* GaborFilter2I default constructor
	*/
        GaborFilter2Magnitude();

        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        GaborFilter2Magnitude(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        GaborFilter2Magnitude(const GaborFilter2Magnitude& gf);

        /**
        * destructor
        */
        ~GaborFilter2Magnitude();

        /**
         * updates the stride
         * @param stride new stride
         * @return non-zero if the stride is changed, zero otherwise
         */
        int updateStride(int stride);

        /**
         * computes the minimum and maximum relative positions
         */
        void computeMinMax();

        /**
         * applies the filter in the nth row-major position
         * @param input input image
         * @param n row-major position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        /**
         * the real part of the Gabor filter
         */
        GaborFilterR2<INPUT, OUTPUT>* real;

        /**
         * the imaginary part of the Gabor filter
         */
        GaborFilterI2<INPUT, OUTPUT>* imag;
    };

    template<typename INPUT, typename OUTPUT>
    GaborFilter2Magnitude<INPUT, OUTPUT>::GaborFilter2Magnitude()
        : Filter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    GaborFilter2Magnitude<INPUT, OUTPUT>::GaborFilter2Magnitude(float sigma, float theta, float lambda, float psi, float gamma)
        : Filter2<INPUT, OUTPUT, float>()
    {
        stride= 4000;

        this->real= new GaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma);
        this->imag= new GaborFilterI2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma);

        std::stringstream ss;
        ss << "GaborFilter2Magnitude" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GaborFilter2Magnitude<INPUT, OUTPUT>::GaborFilter2Magnitude(const GaborFilter2Magnitude& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
        this->real= gf.real;
        this->imag= gf.imag;
    }

    template<typename INPUT, typename OUTPUT>
    GaborFilter2Magnitude<INPUT, OUTPUT>::~GaborFilter2Magnitude()
    {
        delete real;
        delete imag;
    }

    template<typename INPUT, typename OUTPUT>
    int GaborFilter2Magnitude<INPUT, OUTPUT>::updateStride(int stride)
    {
        real->updateStride(stride);
        imag->updateStride(stride);
        this->computeMinMax();
        return 1;
    }

    template<typename INPUT, typename OUTPUT>
    void GaborFilter2Magnitude<INPUT, OUTPUT>::computeMinMax()
    {
        real->computeMinMax();
        imag->computeMinMax();
        this->min= real->min < imag->min ? real->min : imag->min;
        this->max= real->max > imag->max ? real->max : imag->max;
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT GaborFilter2Magnitude<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>*)
    {
        OUTPUT tmp1, tmp2;
        tmp1= real->apply(input, n);
        tmp2= imag->apply(input, n);

        return sqrt(tmp1*tmp1 + tmp2*tmp2);
    }


    /****************************************************************************/
    /******************** Junction Gabor Filter *********************************/
    /****************************************************************************/

    /**
    * JunctionGaborFilterR2 represents a 2D Gabor filter
    */
    template<typename INPUT, typename OUTPUT>
    class JunctionGaborFilterR2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;
        using Filter2<INPUT, OUTPUT, float>::getMin;
        using Filter2<INPUT, OUTPUT, float>::getMax;
    /**
    * JunctionGaborFilterR2 default constructor
    */
        JunctionGaborFilterR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        JunctionGaborFilterR2(float sigma, float theta, float dtheta, float lambda, float psi, float gamma);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        JunctionGaborFilterR2(const JunctionGaborFilterR2& gf);

        /**
        * destructor
        */
        ~JunctionGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    JunctionGaborFilterR2<INPUT, OUTPUT>::JunctionGaborFilterR2()
        : Filter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    JunctionGaborFilterR2<INPUT, OUTPUT>::JunctionGaborFilterR2(float sigma, float theta, float dtheta, float lambda, float psi, float gamma)
        : Filter2<INPUT, OUTPUT, float>()
    {
        GaborFilterR2<INPUT, OUTPUT> gf1(sigma, theta, lambda, psi, gamma);
        GaborFilterR2<INPUT, OUTPUT> gf2(sigma, theta + dtheta, lambda, psi, gamma);

        int p;
        float w;
        unsigned int j;
        for ( unsigned int i= 0; i < gf1.size(); ++i )
        {
            p= gf1(i).first;
            w= gf1(i).second;
            for ( j= 0; j < gf2.size(); ++j )
                if ( gf2(j).first == p )
                {
                    this->push_back(PositionWeightPair<float>(p, w < gf2(j).second ? w : gf2(j).second));
                    break;
                }
            if ( j == gf2.size() )
                this->push_back(PositionWeightPair<float>(p, w));
            //this->addElement(i, j, weight);
        }
        for ( unsigned int i= 0; i < gf2.size(); ++i )
        {
            p= gf2(i).first;
            w= gf2(i).second;
            for ( j= 0; j < gf1.size(); ++j )
                if ( gf1(j).first == p )
                    break;
            if ( j == gf1.size() )
                this->push_back(PositionWeightPair<float>(p, w));
        }
        this->angle= theta;
        this->computeMinMax();
#if NORMALIZE
        this->normalize();
#endif
#if ZERO_MEAN
        this->zeroMean();
#endif

        std::stringstream ss;
        ss << "JunctionGaborFilterR2" << " " << sigma << " " << theta << " " << dtheta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    JunctionGaborFilterR2<INPUT, OUTPUT>::JunctionGaborFilterR2(const JunctionGaborFilterR2& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    JunctionGaborFilterR2<INPUT, OUTPUT>::~JunctionGaborFilterR2()
    {
    }

    /**
    * JunctionGaborFilterR2 represents a 2D Gabor filter
    */
    template<typename INPUT, typename OUTPUT>
    class CorrelationJunctionGaborFilterR2: public CorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using CorrelationFilter2<INPUT, OUTPUT, float>::apply;
        using CorrelationFilter2<INPUT, OUTPUT, float>::descriptor;
        using CorrelationFilter2<INPUT, OUTPUT, float>::stride;
        using CorrelationFilter2<INPUT, OUTPUT, float>::updateStride;
        using CorrelationFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using CorrelationFilter2<INPUT, OUTPUT, float>::getMin;
        using CorrelationFilter2<INPUT, OUTPUT, float>::getMax;
    /**
    * JunctionGaborFilterR2 default constructor
    */
        CorrelationJunctionGaborFilterR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        CorrelationJunctionGaborFilterR2(float sigma, float theta, float dtheta, float lambda, float psi, float gamma);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        CorrelationJunctionGaborFilterR2(const CorrelationJunctionGaborFilterR2& gf);

        /**
        * destructor
        */
        ~CorrelationJunctionGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    CorrelationJunctionGaborFilterR2<INPUT, OUTPUT>::CorrelationJunctionGaborFilterR2()
        : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationJunctionGaborFilterR2<INPUT, OUTPUT>::CorrelationJunctionGaborFilterR2(float sigma, float theta, float dtheta, float lambda, float psi, float gamma)
        : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
        GaborFilterR2<INPUT, OUTPUT> gf1(sigma, theta, lambda, psi, gamma);
        GaborFilterR2<INPUT, OUTPUT> gf2(sigma, theta + dtheta, lambda, psi, gamma);

        int p;
        float w;
        int j;
        for ( int i= 0; i < gf1.size(); ++i )
        {
            p= gf1(i).first;
            w= gf1(i).second;
            for ( j= 0; j < gf2.size(); ++j )
                if ( gf2(j).first == p )
                {
                    this->push_back(PositionWeightPair<float>(p, w < gf2(j).second ? w : gf2(j).second));
                    break;
                }
            if ( j == gf2.size() )
                this->push_back(PositionWeightPair<float>(p, w));
            //this->addElement(i, j, weight);
        }
        for ( int i= 0; i < gf2.size(); ++i )
        {
            p= gf2(i).first;
            w= gf2(i).second;
            for ( j= 0; j < gf1.size(); ++j )
                if ( gf1(j).first == p )
                    break;
            if ( j == gf1.size() )
                this->push_back(PositionWeightPair<float>(p, w));
        }
        this->angle= theta;
        this->computeMinMax();
#if NORMALIZE
        this->normalize();
#endif
#if ZERO_MEAN
        this->zeroMean();
#endif

        std::stringstream ss;
        ss << "CorrelationJunctionGaborFilterR2" << " " << sigma << " " << theta << " " << dtheta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationJunctionGaborFilterR2<INPUT, OUTPUT>::CorrelationJunctionGaborFilterR2(const CorrelationJunctionGaborFilterR2& gf)
    : CorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationJunctionGaborFilterR2<INPUT, OUTPUT>::~CorrelationJunctionGaborFilterR2()
    {
    }

    /**
    * JunctionGaborFilterR2 represents a 2D Gabor filter
    */
    template<typename INPUT, typename OUTPUT>
    class CCorrelationJunctionGaborFilterR2: public CCorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using CCorrelationFilter2<INPUT, OUTPUT, float>::apply;
        using CCorrelationFilter2<INPUT, OUTPUT, float>::descriptor;
        using CCorrelationFilter2<INPUT, OUTPUT, float>::stride;
        using CCorrelationFilter2<INPUT, OUTPUT, float>::updateStride;
        using CCorrelationFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using CCorrelationFilter2<INPUT, OUTPUT, float>::getMin;
        using CCorrelationFilter2<INPUT, OUTPUT, float>::getMax;
    /**
    * JunctionGaborFilterR2 default constructor
    */
        CCorrelationJunctionGaborFilterR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        CCorrelationJunctionGaborFilterR2(float sigma, float theta, float dtheta, float lambda, float psi, float gamma);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        CCorrelationJunctionGaborFilterR2(const CCorrelationJunctionGaborFilterR2& gf);

        /**
        * destructor
        */
        ~CCorrelationJunctionGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    CCorrelationJunctionGaborFilterR2<INPUT, OUTPUT>::CCorrelationJunctionGaborFilterR2()
        : CCorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationJunctionGaborFilterR2<INPUT, OUTPUT>::CCorrelationJunctionGaborFilterR2(float sigma, float theta, float dtheta, float lambda, float psi, float gamma)
        : CCorrelationFilter2<INPUT, OUTPUT, float>()
    {
        GaborFilterR2<INPUT, OUTPUT> gf1(sigma, theta, lambda, psi, gamma);
        GaborFilterR2<INPUT, OUTPUT> gf2(sigma, theta + dtheta, lambda, psi, gamma);

        int p;
        float w;
        unsigned int j;
        for ( unsigned int i= 0; i < gf1.size(); ++i )
        {
            p= gf1(i).first;
            w= gf1(i).second;
            for ( j= 0; j < gf2.size(); ++j )
                if ( gf2(j).first == p )
                {
                    this->push_back(PositionWeightPair<float>(p, w < gf2(j).second ? w : gf2(j).second));
                    break;
                }
            if ( j == gf2.size() )
                this->push_back(PositionWeightPair<float>(p, w));
            //this->addElement(i, j, weight);
        }
        for ( unsigned int i= 0; i < gf2.size(); ++i )
        {
            p= gf2(i).first;
            w= gf2(i).second;
            for ( j= 0; j < gf1.size(); ++j )
                if ( gf1(j).first == p )
                    break;
            if ( j == gf1.size() )
                this->push_back(PositionWeightPair<float>(p, w));
        }
        this->angle= theta;
        this->computeMinMax();
#if NORMALIZE
        this->normalize();
#endif
#if ZERO_MEAN
        this->zeroMean();
#endif

        std::stringstream ss;
        ss << "CCorrelationJunctionGaborFilterR2" << " " << sigma << " " << theta << " " << dtheta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationJunctionGaborFilterR2<INPUT, OUTPUT>::CCorrelationJunctionGaborFilterR2(const CCorrelationJunctionGaborFilterR2& gf)
    : CCorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationJunctionGaborFilterR2<INPUT, OUTPUT>::~CCorrelationJunctionGaborFilterR2()
    {
    }


    /****************************************************************************/
    /******************** Shifted Filters ***************************************/
    /****************************************************************************/

    template<typename INPUT, typename OUTPUT>
    class ShiftedGaborFilterR2: public ShiftedFilter2<INPUT, OUTPUT, float>
    {
    public:
        using ShiftedFilter2<INPUT, OUTPUT, float>::updateStride;
        using ShiftedFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using ShiftedFilter2<INPUT, OUTPUT, float>::apply;
        using ShiftedFilter2<INPUT, OUTPUT, float>::descriptor;

        ShiftedGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma);

        ShiftedGaborFilterR2(const ShiftedGaborFilterR2<INPUT, OUTPUT>& s);

        ~ShiftedGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterR2<INPUT, OUTPUT>::ShiftedGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma)
        : ShiftedFilter2<INPUT, OUTPUT, float>(angle + M_PI/2, dist, new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma))
    {
        stringstream ss;
        ss << "ShiftedGaborFilterR2 " << angle << " " << dist << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        //this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterR2<INPUT, OUTPUT>::ShiftedGaborFilterR2(const ShiftedGaborFilterR2<INPUT, OUTPUT>& f)
        : ShiftedFilter2<INPUT, OUTPUT, float>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedGaborFilterR2<INPUT, OUTPUT>::~ShiftedGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedCorrelationGaborFilterR2: public ShiftedCorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::updateStride;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::apply;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::descriptor;

        ShiftedCorrelationGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma);

        ShiftedCorrelationGaborFilterR2(const ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>& s);

        ~ShiftedCorrelationGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>::ShiftedCorrelationGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma)
        : ShiftedCorrelationFilter2<INPUT, OUTPUT, float>(angle + M_PI/2, dist, new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma))
    {
        stringstream ss;
        ss << "ShiftedCorrelationGaborFilterR2 " << angle << " " << dist << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        //this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>::ShiftedCorrelationGaborFilterR2(const ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>& f)
        : ShiftedCorrelationFilter2<INPUT, OUTPUT, float>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>::~ShiftedCorrelationGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedCCorrelationGaborFilterR2: public ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>::updateStride;
        using ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>::apply;
        using ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>::descriptor;

        ShiftedCCorrelationGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma);

        ShiftedCCorrelationGaborFilterR2(const ShiftedCCorrelationGaborFilterR2<INPUT, OUTPUT>& s);

        ~ShiftedCCorrelationGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedCCorrelationGaborFilterR2<INPUT, OUTPUT>::ShiftedCCorrelationGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma)
        : ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>(angle + M_PI/2, dist, new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma))
    {
        stringstream ss;
        ss << "ShiftedCorrelationGaborFilterR2 " << angle << " " << dist << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        //this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCCorrelationGaborFilterR2<INPUT, OUTPUT>::ShiftedCCorrelationGaborFilterR2(const ShiftedCCorrelationGaborFilterR2<INPUT, OUTPUT>& f)
        : ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCCorrelationGaborFilterR2<INPUT, OUTPUT>::~ShiftedCCorrelationGaborFilterR2()
    {
    }

    /****************************************************************************/
    /******************** End of Shifted Filters ********************************/
    /****************************************************************************/

    /****************************************************************************/
    /******************** FilterSets ********************************************/
    /****************************************************************************/

    template<typename INPUT, typename OUTPUT>
    class GaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;
        using FilterSet2<INPUT, OUTPUT, float>::apply;

        GaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    GaborFilterSet2<INPUT, OUTPUT>::GaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->push_back(new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "GaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class CorrelationGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;
        using FilterSet2<INPUT, OUTPUT, float>::apply;

        CorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    CorrelationGaborFilterSet2<INPUT, OUTPUT>::CorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "CorrelationGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class CCorrelationGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;
        using FilterSet2<INPUT, OUTPUT, float>::apply;

        CCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    CCorrelationGaborFilterSet2<INPUT, OUTPUT>::CCorrelationGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->push_back(new FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "CCorrelationGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class PhaseShiftedGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;
        using FilterSet2<INPUT, OUTPUT, float>::apply;

        PhaseShiftedGaborFilterSet2(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    PhaseShiftedGaborFilterSet2<INPUT, OUTPUT>::PhaseShiftedGaborFilterSet2(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float p= psi0; p < psi1; p+= psis )
            this->push_back(new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, p, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "PhaseShiftedGaborFilterSet2 " << sigma << " " << theta << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class PhaseShiftedCorrelationGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;
        using FilterSet2<INPUT, OUTPUT, float>::apply;

        PhaseShiftedCorrelationGaborFilterSet2(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    PhaseShiftedCorrelationGaborFilterSet2<INPUT, OUTPUT>::PhaseShiftedCorrelationGaborFilterSet2(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float p= psi0; p < psi1; p+= psis )
        {
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, p, gamma));
            //cout << (*this)[this->size()-1]->getDescriptor() << endl;
        }
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "PhaseShiftedCorrelationGaborFilterSet2 " << sigma << " " << theta << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class PhaseShiftedCCorrelationGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;
        using FilterSet2<INPUT, OUTPUT, float>::apply;

        PhaseShiftedCCorrelationGaborFilterSet2(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    PhaseShiftedCCorrelationGaborFilterSet2<INPUT, OUTPUT>::PhaseShiftedCCorrelationGaborFilterSet2(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float p= psi0; p < psi1; p+= psis )
        {
            this->push_back(new FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, p, gamma));
            //cout << (*this)[this->size()-1]->getDescriptor() << endl;
        }
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "PhaseShiftedCCorrelationGaborFilterSet2 " << sigma << " " << theta << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class LambdaSpaceGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;
        using FilterSet2<INPUT, OUTPUT, float>::apply;

        LambdaSpaceGaborFilterSet2(float sigma, float theta, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    LambdaSpaceGaborFilterSet2<INPUT, OUTPUT>::LambdaSpaceGaborFilterSet2(float sigma, float theta, float lambda0, float lambdas, float lambda1, float psi, float gamma)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
            this->push_back(new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, theta, l, psi, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "LambdaSpaceGaborFilterSet2 " << sigma << " " << theta << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class LambdaSpaceCorrelationGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;
        using FilterSet2<INPUT, OUTPUT, float>::apply;

        LambdaSpaceCorrelationGaborFilterSet2(float sigma, float theta, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    LambdaSpaceCorrelationGaborFilterSet2<INPUT, OUTPUT>::LambdaSpaceCorrelationGaborFilterSet2(float sigma, float theta, float lambda0, float lambdas, float lambda1, float psi, float gamma)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
            this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta, l, psi, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "LambdaSpaceCorrelationGaborFilterSet2 " << sigma << " " << theta << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class LambdaSpaceCCorrelationGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;
        using FilterSet2<INPUT, OUTPUT, float>::apply;

        LambdaSpaceCCorrelationGaborFilterSet2(float sigma, float theta, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    LambdaSpaceCCorrelationGaborFilterSet2<INPUT, OUTPUT>::LambdaSpaceCCorrelationGaborFilterSet2(float sigma, float theta, float lambda0, float lambdas, float lambda1, float psi, float gamma)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
            this->push_back(new FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, theta, l, psi, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "LambdaSpaceCCorrelationGaborFilterSet2 " << sigma << " " << theta << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class TranslatedCorrelationGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::apply;

        TranslatedCorrelationGaborFilterSet2(int n, float step, float sigma, float theta, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    TranslatedCorrelationGaborFilterSet2<INPUT, OUTPUT>::TranslatedCorrelationGaborFilterSet2(int n, float step, float sigma, float theta, float lambda, float psi, float gamma)
        : FilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float t= -n*step; t < n*step; t+= step )
            this->push_back(new ShiftedCorrelationGaborFilterR2<INPUT, OUTPUT>(theta + M_PI/2, t, sigma, t, lambda, psi, gamma));
        computeMinMax();
        stringstream ss;
        ss << "TranslatedCorrelationGaborFilterSet2 " << n << " " << step << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    /*************************************************************************************/
    /***************************** End of FilterSets *************************************/
    /*************************************************************************************/
}


#endif	/* _GABORFILTER2_H */


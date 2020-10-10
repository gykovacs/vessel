/**
 * @file GaussGaussGaborFilter2.h
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
 * The GaussGaussGaborFilter2 and GaussGaussGaborFilter2I represent the 2d GaussGaussGabor filters.
 */

#ifndef _GaussGaborFILTER2_H
#define	_GaussGaborFILTER2_H

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

namespace openip
{

    /**************************************************************************************************/
    /****************************** GaussGaussGaborFilterR2                    **********************************/
    /**************************************************************************************************/

    /**
    * GaussGaborFilter2 represents a 2D GaussGabor filter
    */
    template<typename INPUT, typename OUTPUT>
    class GaussGaborFilterR2: public Filter2<INPUT, OUTPUT, float>
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
    * GaussGaborFilter2 default constructor
    */
        GaussGaborFilterR2();
        /**
         * 2D GaussGabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        GaussGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaussGaborFilter2 copy constructor
        * @param gf instance to copy
        */
        GaussGaborFilterR2(const GaussGaborFilterR2& gf);

        /**
        * destructor
        */
        ~GaussGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilterR2<INPUT, OUTPUT>::GaussGaborFilterR2()
        : Filter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilterR2<INPUT, OUTPUT>::GaussGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma)
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
        float xmin= -xmax;
        float ymin= -ymax;

        float weight;
        float x_theta;
        float y_theta;

        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                //weight= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                  //      + y_theta*y_theta/(sigma_y*sigma_y))) * cos(2*M_PI/lambda*x_theta + psi);
                weight= -1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y))) * exp(-x_theta*x_theta/(lambda*lambda) + psi);
                if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )
                    continue;

                this->addElement(i, j, weight);
            }
        this->angle= theta;
        this->computeMinMax();
        //this->add(2);
        //this->zeroMean();

        std::stringstream ss;
        ss << "GaussGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilterR2<INPUT, OUTPUT>::GaussGaborFilterR2(const GaussGaborFilterR2& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilterR2<INPUT, OUTPUT>::~GaussGaborFilterR2()
    {
    }

    /**
    * GaussGaborFilter2I represents the imaginary part of the filter
    */
    template<typename INPUT, typename OUTPUT>
    class GaussGaborFilterI2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::computeMinMax;


    /**
    * GaussGaborFilter2I default constructor
    */
        GaussGaborFilterI2();

        /**
         * 2D GaussGabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        GaussGaborFilterI2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        GaussGaborFilterI2(const GaussGaborFilterI2& gf);

        /**
        * destructor
        */
        ~GaussGaborFilterI2();
    };

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilterI2<INPUT, OUTPUT>::GaussGaborFilterI2()
        : Filter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilterI2<INPUT, OUTPUT>::GaussGaborFilterI2(float sigma, float theta, float lambda, float psi, float gamma)
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
        float xmin= -xmax;
        float ymin= -ymax;

        float weight;
        float x_theta;
        float y_theta;

        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                //weight= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                  //      + y_theta*y_theta/(sigma_y*sigma_y))) * sin(2*M_PI/lambda*x_theta + psi);
                weight= -1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y))) * exp(-x_theta*x_theta/(lambda*lambda) + psi);
                if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )
                    continue;

                this->addElement(i, j, weight);
            }
        this->computeMinMax();
        std::stringstream ss;
        ss << "GaussGaborFilter2I" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilterI2<INPUT, OUTPUT>::GaussGaborFilterI2(const GaussGaborFilterI2& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilterI2<INPUT, OUTPUT>::~GaussGaborFilterI2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class FixedSizeGaussGaborFilterR2: public Filter2<INPUT, OUTPUT, float>
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
    * GaussGaborFilter2 default constructor
    */
        FixedSizeGaussGaborFilterR2();
        /**
         * 2D GaussGabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        FixedSizeGaussGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaussGaborFilter2 copy constructor
        * @param gf instance to copy
        */
        FixedSizeGaussGaborFilterR2(const FixedSizeGaussGaborFilterR2& gf);

        /**
        * destructor
        */
        ~FixedSizeGaussGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    FixedSizeGaussGaborFilterR2<INPUT, OUTPUT>::FixedSizeGaussGaborFilterR2()
        : Filter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeGaussGaborFilterR2<INPUT, OUTPUT>::FixedSizeGaussGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma)
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
        float xmin= -xmax;
        float ymin= -ymax;

        float weight;
        float x_theta;
        float y_theta;

        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                //weight= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                  //      + y_theta*y_theta/(sigma_y*sigma_y))) * cos(2*M_PI/lambda*x_theta + psi);
                weight= -1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y))) * exp(-x_theta*x_theta/(lambda*lambda) + psi);
                if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )
                    continue;

                this->addElement(i, j, weight);
            }
        this->angle= theta;
        this->computeMinMax();
        //this->add(2);
        //this->zeroMean();

        std::stringstream ss;
        ss << "FixedSizeGaussGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeGaussGaborFilterR2<INPUT, OUTPUT>::FixedSizeGaussGaborFilterR2(const FixedSizeGaussGaborFilterR2& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeGaussGaborFilterR2<INPUT, OUTPUT>::~FixedSizeGaussGaborFilterR2()
    {
    }

    /**************************************************************************************************/
    /****************************** End of GaussGaborFilterR2             **********************************/
    /**************************************************************************************************/

    /**************************************************************************************************/
    /****************************** CorrelationGaussGaborFilterR2             ******************************/
    /**************************************************************************************************/

    template<typename INPUT, typename OUTPUT>
    class CorrelationGaussGaborFilterR2: public CorrelationFilter2<INPUT, OUTPUT, float>
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
    * GaussGaborFilter2 default constructor
    */
        CorrelationGaussGaborFilterR2();
        /**
         * 2D GaussGabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        CorrelationGaussGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaussGaborFilter2 copy constructor
        * @param gf instance to copy
        */
        CorrelationGaussGaborFilterR2(const CorrelationGaussGaborFilterR2& gf);

        /**
        * destructor
        */
        ~CorrelationGaussGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    CorrelationGaussGaborFilterR2<INPUT, OUTPUT>::CorrelationGaussGaborFilterR2()
        : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationGaussGaborFilterR2<INPUT, OUTPUT>::CorrelationGaussGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma)
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
        float xmin= -xmax;
        float ymin= -ymax;

        float weight;
        float x_theta;
        float y_theta;

        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                //weight= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                  //      + y_theta*y_theta/(sigma_y*sigma_y))) * cos(2*M_PI/lambda*x_theta + psi);
                weight= -1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y))) * exp(-x_theta*x_theta/(lambda*lambda) + psi);
                if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )
                    continue;

                this->addElement(i, j, weight);
            }
        this->angle= theta;
        this->computeMinMax();
        //this->add(2);
        //this->zeroMean();
        std::stringstream ss;
        ss << "CorrelationGaussGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationGaussGaborFilterR2<INPUT, OUTPUT>::CorrelationGaussGaborFilterR2(const CorrelationGaussGaborFilterR2& gf)
    : CorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationGaussGaborFilterR2<INPUT, OUTPUT>::~CorrelationGaussGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class FixedSizeCorrelationGaussGaborFilterR2: public CorrelationFilter2<INPUT, OUTPUT, float>
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
    * GaussGaborFilter2 default constructor
    */
        FixedSizeCorrelationGaussGaborFilterR2();
        /**
         * 2D GaussGabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        FixedSizeCorrelationGaussGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaussGaborFilter2 copy constructor
        * @param gf instance to copy
        */
        FixedSizeCorrelationGaussGaborFilterR2(const FixedSizeCorrelationGaussGaborFilterR2& gf);

        /**
        * destructor
        */
        ~FixedSizeCorrelationGaussGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    FixedSizeCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::FixedSizeCorrelationGaussGaborFilterR2()
        : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::FixedSizeCorrelationGaussGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma)
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
        float xmin= -xmax;
        float ymin= -ymax;

        float weight;
        float x_theta;
        float y_theta;

        //float minWeight= FLT_MAX;
        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                //weight= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                  //      + y_theta*y_theta/(sigma_y*sigma_y))) * cos(2*M_PI/lambda*x_theta + psi);
                weight= -1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y))) * exp(-x_theta*x_theta/(lambda*lambda) + psi);
                if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )
                    continue;

                this->addElement(i, j, weight);
            }
        this->angle= theta;
        this->computeMinMax();
        //this->add(2);
        //this->zeroMean();

        std::stringstream ss;
        ss << "FixedSizeCorrelationGaussGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::FixedSizeCorrelationGaussGaborFilterR2(const FixedSizeCorrelationGaussGaborFilterR2& gf)
    : CorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::~FixedSizeCorrelationGaussGaborFilterR2()
    {
    }

    /**************************************************************************************************/
    /****************************** End of CorrelationGaussGaborFilterR2               *********************/
    /**************************************************************************************************/

    /**************************************************************************************************/
    /****************************** CCorrelationGaussGaborFilterR2               *********************/
    /**************************************************************************************************/

    template<typename INPUT, typename OUTPUT>
    class CCorrelationGaussGaborFilterR2: public CCorrelationFilter2<INPUT, OUTPUT, float>
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
    * GaussGaborFilter2 default constructor
    */
        CCorrelationGaussGaborFilterR2();
        /**
         * 2D GaussGabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        CCorrelationGaussGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaussGaborFilter2 copy constructor
        * @param gf instance to copy
        */
        CCorrelationGaussGaborFilterR2(const CCorrelationGaussGaborFilterR2& gf);

        /**
        * destructor
        */
        ~CCorrelationGaussGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    CCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::CCorrelationGaussGaborFilterR2()
        : CCorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::CCorrelationGaussGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma)
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
        float xmin= -xmax;
        float ymin= -ymax;

        float weight;
        float x_theta;
        float y_theta;

        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                //weight= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                  //      + y_theta*y_theta/(sigma_y*sigma_y))) * cos(2*M_PI/lambda*x_theta + psi);
                weight= -1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y))) * exp(-x_theta*x_theta/(lambda*lambda) + psi);
                if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )
                    continue;

                this->addElement(i, j, weight);
            }
        this->angle= theta;
        this->computeMinMax();
        //this->add(2);
        //this->zeroMean();
        std::stringstream ss;
        ss << "CCorrelationGaussGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::CCorrelationGaussGaborFilterR2(const CCorrelationGaussGaborFilterR2& gf)
    : CCorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::~CCorrelationGaussGaborFilterR2()
    {
    }


    template<typename INPUT, typename OUTPUT>
    class FixedSizeCCorrelationGaussGaborFilterR2: public CCorrelationFilter2<INPUT, OUTPUT, float>
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
    * GaussGaborFilter2 default constructor
    */
        FixedSizeCCorrelationGaussGaborFilterR2();
        /**
         * 2D GaussGabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        FixedSizeCCorrelationGaussGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * GaussGaborFilter2 copy constructor
        * @param gf instance to copy
        */
        FixedSizeCCorrelationGaussGaborFilterR2(const FixedSizeCCorrelationGaussGaborFilterR2& gf);

        /**
        * destructor
        */
        ~FixedSizeCCorrelationGaussGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    FixedSizeCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::FixedSizeCCorrelationGaussGaborFilterR2()
        : CCorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::FixedSizeCCorrelationGaussGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma)
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
        float xmin= -xmax;
        float ymin= -ymax;

        float weight;
        float x_theta;
        float y_theta;

        //float minWeight= FLT_MAX;
        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                y_theta= i*sin(theta) + j*cos(theta);

                //weight= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                  //      + y_theta*y_theta/(sigma_y*sigma_y))) * cos(2*M_PI/lambda*x_theta + psi);
                weight= -1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                        + y_theta*y_theta/(sigma_y*sigma_y))) * exp(-x_theta*x_theta/(lambda*lambda) + psi);
                if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )
                    continue;

                this->addElement(i, j, weight);
            }
        this->angle= theta;
        this->computeMinMax();
        //this->add(2);
        //this->zeroMean();

        std::stringstream ss;
        ss << "FixedSizeCCorrelationGaussGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::FixedSizeCCorrelationGaussGaborFilterR2(const FixedSizeCCorrelationGaussGaborFilterR2& gf)
    : CCorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    FixedSizeCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::~FixedSizeCCorrelationGaussGaborFilterR2()
    {
    }

    /**************************************************************************************************/
    /****************************** End of CCorrelationGaussGaborFilterR2               ********************/
    /**************************************************************************************************/

    /**
     * GaussGaborFilter2Magnitude feature
     */
    template<typename INPUT, typename OUTPUT>
    class GaussGaborFilter2Magnitude: public Filter2<INPUT, OUTPUT, float>
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
    * GaussGaborFilter2I default constructor
    */
        GaussGaborFilter2Magnitude();

        /**
         * 2D GaussGabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        GaussGaborFilter2Magnitude(float sigma, float theta, float lambda, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        GaussGaborFilter2Magnitude(const GaussGaborFilter2Magnitude& gf);

        /**
        * destructor
        */
        ~GaussGaborFilter2Magnitude();

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
         * the real part of the GaussGabor filter
         */
        GaussGaborFilterR2<INPUT, OUTPUT>* real;

        /**
         * the imaginary part of the GaussGabor filter
         */
        GaussGaborFilterI2<INPUT, OUTPUT>* imag;
    };

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilter2Magnitude<INPUT, OUTPUT>::GaussGaborFilter2Magnitude()
        : Filter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilter2Magnitude<INPUT, OUTPUT>::GaussGaborFilter2Magnitude(float sigma, float theta, float lambda, float psi, float gamma)
        : Filter2<INPUT, OUTPUT, float>()
    {
        stride= 4000;

        this->real= new GaussGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma);
        this->imag= new GaussGaborFilterI2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma);

        std::stringstream ss;
        ss << "GaussGaborFilter2Magnitude" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilter2Magnitude<INPUT, OUTPUT>::GaussGaborFilter2Magnitude(const GaussGaborFilter2Magnitude& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
        this->real= gf.real;
        this->imag= gf.imag;
    }

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilter2Magnitude<INPUT, OUTPUT>::~GaussGaborFilter2Magnitude()
    {
        delete real;
        delete imag;
    }

    template<typename INPUT, typename OUTPUT>
    int GaussGaborFilter2Magnitude<INPUT, OUTPUT>::updateStride(int stride)
    {
        real->updateStride(stride);
        imag->updateStride(stride);
        this->computeMinMax();
        return 1;
    }

    template<typename INPUT, typename OUTPUT>
    void GaussGaborFilter2Magnitude<INPUT, OUTPUT>::computeMinMax()
    {
        real->computeMinMax();
        imag->computeMinMax();
        this->min= real->min < imag->min ? real->min : imag->min;
        this->max= real->max > imag->max ? real->max : imag->max;
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT GaussGaborFilter2Magnitude<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>*)
    {
        OUTPUT tmp1, tmp2;
        tmp1= real->apply(input, n);
        tmp2= imag->apply(input, n);

        return sqrt(tmp1*tmp1 + tmp2*tmp2);
    }



    /****************************************************************************/
    /******************** Shifted Filters ***************************************/
    /****************************************************************************/

    template<typename INPUT, typename OUTPUT>
    class ShiftedGaussGaborFilterR2: public ShiftedFilter2<INPUT, OUTPUT, float>
    {
    public:
        using ShiftedFilter2<INPUT, OUTPUT, float>::updateStride;
        using ShiftedFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using ShiftedFilter2<INPUT, OUTPUT, float>::apply;
        using ShiftedFilter2<INPUT, OUTPUT, float>::descriptor;

        ShiftedGaussGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma);

        ShiftedGaussGaborFilterR2(const ShiftedGaussGaborFilterR2<INPUT, OUTPUT>& s);

        ~ShiftedGaussGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedGaussGaborFilterR2<INPUT, OUTPUT>::ShiftedGaussGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma)
        : ShiftedFilter2<INPUT, OUTPUT, float>(angle + M_PI/2, dist, new FixedSizeGaussGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma))
    {
        stringstream ss;
        ss << "ShiftedGaussGaborFilterR2 " << angle << " " << dist << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        //this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedGaussGaborFilterR2<INPUT, OUTPUT>::ShiftedGaussGaborFilterR2(const ShiftedGaussGaborFilterR2<INPUT, OUTPUT>& f)
        : ShiftedFilter2<INPUT, OUTPUT, float>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedGaussGaborFilterR2<INPUT, OUTPUT>::~ShiftedGaussGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedCorrelationGaussGaborFilterR2: public ShiftedCorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::updateStride;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::apply;
        using ShiftedCorrelationFilter2<INPUT, OUTPUT, float>::descriptor;

        ShiftedCorrelationGaussGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma);

        ShiftedCorrelationGaussGaborFilterR2(const ShiftedCorrelationGaussGaborFilterR2<INPUT, OUTPUT>& s);

        ~ShiftedCorrelationGaussGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::ShiftedCorrelationGaussGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma)
        : ShiftedCorrelationFilter2<INPUT, OUTPUT, float>(angle + M_PI/2, dist, new FixedSizeGaussGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma))
    {
        stringstream ss;
        ss << "ShiftedCorrelationGaussGaborFilterR2 " << angle << " " << dist << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        //this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::ShiftedCorrelationGaussGaborFilterR2(const ShiftedCorrelationGaussGaborFilterR2<INPUT, OUTPUT>& f)
        : ShiftedCorrelationFilter2<INPUT, OUTPUT, float>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::~ShiftedCorrelationGaussGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class ShiftedCCorrelationGaussGaborFilterR2: public ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>
    {
    public:
        using ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>::updateStride;
        using ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>::computeMinMax;
        using ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>::apply;
        using ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>::descriptor;

        ShiftedCCorrelationGaussGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma);

        ShiftedCCorrelationGaussGaborFilterR2(const ShiftedCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>& s);

        ~ShiftedCCorrelationGaussGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    ShiftedCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::ShiftedCCorrelationGaussGaborFilterR2(float angle, float dist, float sigma, float theta, float lambda, float psi, float gamma)
        : ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>(angle + M_PI/2, dist, new FixedSizeGaussGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, psi, gamma))
    {
        stringstream ss;
        ss << "ShiftedCorrelationGaussGaborFilterR2 " << angle << " " << dist << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
        //this->zeroMean();
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::ShiftedCCorrelationGaussGaborFilterR2(const ShiftedCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>& f)
        : ShiftedCCorrelationFilter2<INPUT, OUTPUT, float>(f)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ShiftedCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>::~ShiftedCCorrelationGaussGaborFilterR2()
    {
    }

    /****************************************************************************/
    /******************** End of Shifted Filters ********************************/
    /****************************************************************************/

    /****************************************************************************/
    /******************** FilterSets ********************************************/
    /****************************************************************************/

    template<typename INPUT, typename OUTPUT>
    class GaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;

        GaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    GaussGaborFilterSet2<INPUT, OUTPUT>::GaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->push_back(new FixedSizeGaussGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "GaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class CorrelationGaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;

        CorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    CorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::CorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->push_back(new FixedSizeCorrelationGaussGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "CorrelationGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class CCorrelationGaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;

        CCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    CCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::CCorrelationGaussGaborFilterSet2(float sigma, float theta0, float thetas, float theta1, float lambda, float psi, float gamma)
    {
        for ( float t= theta0; t < theta1; t+= thetas )
            this->push_back(new FixedSizeCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>(sigma, t, lambda, psi, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "CCorrelationGaussGaborFilterSet2 " << sigma << " " << theta0 << " " << thetas << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class PhaseShiftedGaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;

        PhaseShiftedGaussGaborFilterSet2(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    PhaseShiftedGaussGaborFilterSet2<INPUT, OUTPUT>::PhaseShiftedGaussGaborFilterSet2(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma)
    {
        for ( float p= psi0; p < psi1; p+= psis )
            this->push_back(new FixedSizeGaussGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, p, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "PhaseShiftedGaussGaborFilterSet2 " << sigma << " " << theta << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class PhaseShiftedCorrelationGaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;

        PhaseShiftedCorrelationGaussGaborFilterSet2(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    PhaseShiftedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::PhaseShiftedCorrelationGaussGaborFilterSet2(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma)
    {
        for ( float p= psi0; p < psi1; p+= psis )
        {
            this->push_back(new FixedSizeCorrelationGaussGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, p, gamma));
            //cout << (*this)[this->size()-1]->getDescriptor() << endl;
        }
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "PhaseShiftedCorrelationGaussGaborFilterSet2 " << sigma << " " << theta << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class PhaseShiftedCCorrelationGaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;

        PhaseShiftedCCorrelationGaussGaborFilterSet2(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    PhaseShiftedCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::PhaseShiftedCCorrelationGaussGaborFilterSet2(float sigma, float theta, float lambda, float psi0, float psis, float psi1, float gamma)
    {
        for ( float p= psi0; p < psi1; p+= psis )
        {
            this->push_back(new FixedSizeCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>(sigma, theta, lambda, p, gamma));
            //cout << (*this)[this->size()-1]->getDescriptor() << endl;
        }
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "PhaseShiftedCCorrelationGaussGaborFilterSet2 " << sigma << " " << theta << " " << lambda << " " << psi0 << " " << psis << " " << psi1 << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class LambdaSpaceGaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;

        LambdaSpaceGaussGaborFilterSet2(float sigma, float theta, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    LambdaSpaceGaussGaborFilterSet2<INPUT, OUTPUT>::LambdaSpaceGaussGaborFilterSet2(float sigma, float theta, float lambda0, float lambdas, float lambda1, float psi, float gamma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
            this->push_back(new FixedSizeGaussGaborFilterR2<INPUT, OUTPUT>(sigma, theta, l, psi, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "LambdaSpaceGaussGaborFilterSet2 " << sigma << " " << theta << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class LambdaSpaceCorrelationGaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;

        LambdaSpaceCorrelationGaussGaborFilterSet2(float sigma, float theta, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    LambdaSpaceCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::LambdaSpaceCorrelationGaussGaborFilterSet2(float sigma, float theta, float lambda0, float lambdas, float lambda1, float psi, float gamma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
            this->push_back(new FixedSizeCorrelationGaussGaborFilterR2<INPUT, OUTPUT>(sigma, theta, l, psi, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "LambdaSpaceCorrelationGaussGaborFilterSet2 " << sigma << " " << theta << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class LambdaSpaceCCorrelationGaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::orthogonalize;

        LambdaSpaceCCorrelationGaussGaborFilterSet2(float sigma, float theta, float lambda0, float lambdas, float lambda1, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    LambdaSpaceCCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::LambdaSpaceCCorrelationGaussGaborFilterSet2(float sigma, float theta, float lambda0, float lambdas, float lambda1, float psi, float gamma)
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
            this->push_back(new FixedSizeCCorrelationGaussGaborFilterR2<INPUT, OUTPUT>(sigma, theta, l, psi, gamma));
        computeMinMax();
        //orthogonalize();
        stringstream ss;
        ss << "LambdaSpaceCCorrelationGaussGaborFilterSet2 " << sigma << " " << theta << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    class TranslatedCorrelationGaussGaborFilterSet2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::push_back;
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;

        TranslatedCorrelationGaussGaborFilterSet2(int n, float step, float sigma, float theta, float lambda, float psi, float gamma);
    };

    template<typename INPUT, typename OUTPUT>
    TranslatedCorrelationGaussGaborFilterSet2<INPUT, OUTPUT>::TranslatedCorrelationGaussGaborFilterSet2(int n, float step, float sigma, float theta, float lambda, float psi, float gamma)
    {
        for ( float t= -n*step; t < n*step; t+= step )
            this->push_back(new ShiftedCorrelationGaussGaborFilterR2<INPUT, OUTPUT>(theta + M_PI/2, t, sigma, t, lambda, psi, gamma));
        computeMinMax();
        stringstream ss;
        ss << "TranslatedCorrelationGaussGaborFilterSet2 " << n << " " << step << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    /*************************************************************************************/
    /***************************** End of FilterSets *************************************/
    /*************************************************************************************/
}


#endif	/* _GaussGaussGaborFILTER2_H */


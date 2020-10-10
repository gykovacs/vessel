#ifndef POWEREDGABORFILTER_H
#define POWEREDGABORFILTER_H

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
#include <openipDS/MatchedGaborFilter2.h>

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
    /**
    * GaborFilter2 represents a 2D Gabor filter
    */
    template<typename INPUT, typename OUTPUT>
    class PowerGaborFilterR2: public Filter2<INPUT, OUTPUT, float>
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
        PowerGaborFilterR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        PowerGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma, float powerFactor);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        PowerGaborFilterR2(const PowerGaborFilterR2& gf);

        /**
        * destructor
        */
        ~PowerGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    PowerGaborFilterR2<INPUT, OUTPUT>::PowerGaborFilterR2()
        : Filter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    PowerGaborFilterR2<INPUT, OUTPUT>::PowerGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma, float powerFactor)
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
        //float y_theta;

        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                //y_theta= i*sin(theta) + j*cos(theta);

                weight= cos(2*M_PI/lambda*x_theta + psi);
                //weight*= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x) + y_theta*y_theta/(sigma_y*sigma_y)));
                weight*= 1.0/(2*M_PI*sigma_x) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)));
    #if CUT_FILTER
                /*if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )*/
                //if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                if ( abs(x_theta) > NSIGMA * sigma_x )
                    continue;
    #endif
                weight= weight > 0 ? pow(fabs(weight), powerFactor) : -pow(fabs(weight), powerFactor);
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
        ss << "PowerGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    PowerGaborFilterR2<INPUT, OUTPUT>::PowerGaborFilterR2(const PowerGaborFilterR2& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    PowerGaborFilterR2<INPUT, OUTPUT>::~PowerGaborFilterR2()
    {
    }



    template<typename INPUT, typename OUTPUT>
    class CorrelationPowerGaborFilterR2: public CorrelationFilter2<INPUT, OUTPUT, float>
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
        CorrelationPowerGaborFilterR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        CorrelationPowerGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma, float powerFactor);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        CorrelationPowerGaborFilterR2(const CorrelationPowerGaborFilterR2& gf);

        /**
        * destructor
        */
        ~CorrelationPowerGaborFilterR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    CorrelationPowerGaborFilterR2<INPUT, OUTPUT>::CorrelationPowerGaborFilterR2()
        : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationPowerGaborFilterR2<INPUT, OUTPUT>::CorrelationPowerGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma, float powerFactor)
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
        //float y_theta;

        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i*cos(theta) - j*sin(theta);
                //y_theta= i*sin(theta) + j*cos(theta);

                weight= cos(2*M_PI/lambda*x_theta + psi);
                //weight*= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x) + y_theta*y_theta/(sigma_y*sigma_y)));
                weight*= 1.0/(2*M_PI*sigma_x) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)));
#if CUT_FILTER
                /*if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.00001 )*/
                //if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                if ( abs(x_theta) > NSIGMA * sigma_x )
                    continue;
#endif
                weight= weight > 0 ? pow(fabs(weight), powerFactor) : -pow(fabs(weight), powerFactor);
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
        ss << "CorrelationPowerGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationPowerGaborFilterR2<INPUT, OUTPUT>::CorrelationPowerGaborFilterR2(const CorrelationPowerGaborFilterR2& gf)
    : CorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationPowerGaborFilterR2<INPUT, OUTPUT>::~CorrelationPowerGaborFilterR2()
    {
    }



    template<typename INPUT, typename OUTPUT>
    class CCorrelationPowerGaborFilterR2: public CCorrelationFilter2<INPUT, OUTPUT, float>
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
        CCorrelationPowerGaborFilterR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        CCorrelationPowerGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma, float powerFactor, float scale= 1, float middle= 0);
        
        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        CCorrelationPowerGaborFilterR2(const CCorrelationPowerGaborFilterR2& gf);

        /**
        * destructor
        */
        ~CCorrelationPowerGaborFilterR2();
        
        CCorrelationPowerGaborFilterR2 operator=(const CCorrelationPowerGaborFilterR2& c);

        /** angle of the filter */
        float angle;
        float lambda;
        float sigma;
        float gamma;
        float psi;
        float middle;
    };

    
    template<typename INPUT, typename OUTPUT>
    CCorrelationPowerGaborFilterR2<INPUT, OUTPUT> CCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::operator=(const CCorrelationPowerGaborFilterR2<INPUT, OUTPUT>& c)
    {
        this->Filter2<INPUT, OUTPUT, float>::operator=(c);
        this->lambda= c.lambda;
        this->sigma= c.sigma;
        this->gamma= c.gamma;
        this->psi= c.psi;
        this->middle= c.middle;
        
        return *this;
    }
    
    template<typename INPUT, typename OUTPUT>
    CCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::CCorrelationPowerGaborFilterR2()
        : CCorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    CCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::CCorrelationPowerGaborFilterR2(const CCorrelationPowerGaborFilterR2& c)
        : CCorrelationFilter2<INPUT, OUTPUT, float>(c)
    {
        this->lambda= c.lambda;
        this->sigma= c.sigma;
        this->gamma= c.gamma;
        this->psi= c.psi;
        this->middle= c.middle;
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::CCorrelationPowerGaborFilterR2(float sigma, float theta, float lambda, float psi, float gamma, float powerFactor, float scale, float middle)
        : CCorrelationFilter2<INPUT, OUTPUT, float>()
    {
        this->lambda= lambda;
        this->sigma= sigma;
        this->gamma= gamma;
        this->psi= psi;
        this->middle= middle;
        
        /*float offsetlength= (psi - M_PI)/(2*M_PI)*lambda;
        float offsetx= sin(theta - M_PI/2)*offsetlength;
        float offsety= cos(theta - M_PI/2)*offsetlength;*/
        float offsetlength= 0;
        float offsetx= 0;
        float offsety= 0;
        //printf("offsets: %f %f %f\n", offsetlength, offsetx, offsety);
        
        //printf("constructing: %f %f %f\n", this->lambda, this->sigma, this->gamma); fflush(stdout);
        
        stride= 4000;
        float sigma_x= sigma;
        float sigma_y= sigma/gamma;

        int nstds= 10;
        float xmax= max(fabs(nstds*sigma_x*cos(theta)*scale), fabs(nstds*sigma_y*sin(theta)*scale));
        xmax= ceil(max(1.0f, xmax));
        float ymax= max(fabs(nstds*sigma_x*sin(theta)*scale), fabs(nstds*sigma_y*cos(theta)*scale));
        ymax= ceil(max(1.0f, ymax));

        xmax = xmax > ymax ? xmax : ymax;
        ymax= ymax > xmax ? ymax : xmax;

        float xmin= -xmax;
        float ymin= -ymax;

        float weight;
        float x_theta;
        float y_theta;

        //printf("%f %f\n", xmax, ymax);
        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= (i - offsetx)/scale*cos(theta) - (j - offsety)/scale*sin(theta);
                y_theta= (i - offsetx)/scale*sin(theta) + (j - offsety)/scale*cos(theta);

                float x_theta_mod= (x_theta + offsetlength);
                
                weight= cos(2*M_PI/lambda*x_theta + psi);
                //if ( middle != 0 && abs(x_theta_mod) < /*0.5*/ (NSIGMA*sigma_x / 8.0 < 0.5 ? 0.5 : NSIGMA*sigma_x / 8.0) )
                    //weight= middle;
                if ( middle > 0 )
                {
                    if ( y_theta < 0 && (abs(x_theta_mod) > /*0.5*/ (NSIGMA*sigma_x / 4.0 < 0.5 ? 0.5 : NSIGMA*sigma_x / 4.0)) )
                        continue;
                }
                
                
                //weight*= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x) + y_theta*y_theta/(sigma_y*sigma_y)));
                weight*= 1.0/(2*M_PI*sigma_x*sigma_x) * exp(-0.5*(x_theta_mod*x_theta_mod/(sigma_x*sigma_x)));
#if CUT_FILTER
                /*if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )*/
                //if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                if ( abs(x_theta_mod /* + offsetlength - offsetx*/) > NSIGMA * sigma_x || abs(y_theta/* - offsety*/) > NSIGMA * sigma_y  )
                    continue;
#endif
                weight= weight > 0 ? pow(fabs(weight), powerFactor) : -pow(fabs(weight), powerFactor);
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
        ss << "CCorrelationPowerGaborFilterR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        for ( typename CCorrelationFilter2<INPUT, OUTPUT, float>::fIt fit= this->begin(); fit != this->end(); ++fit )
            meanF+= fit->second;

        meanF/= this->size();

        for ( typename CCorrelationFilter2<INPUT, OUTPUT, float>::fIt fit= this->begin(); fit != this->end(); ++fit )
            devF+= (fit->second - meanF) * (fit->second - meanF);

        devF= sqrt(devF);
    }

    

    template<typename INPUT, typename OUTPUT>
    CCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::~CCorrelationPowerGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedPowerGaborFilterR2: public MaxFilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using MaxFilterSet2<INPUT, OUTPUT, float>::apply;
        using MaxFilterSet2<INPUT, OUTPUT, float>::descriptor;
        using MaxFilterSet2<INPUT, OUTPUT, float>::updateStride;
        using MaxFilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using MaxFilterSet2<INPUT, OUTPUT, float>::push_back;

    /**
    * GaborFilter2I default constructor
    */
        MatchedPowerGaborFilterR2();

        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta0 smallest angle
         * @param step angle step
         * @param theta1 largest angle
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        MatchedPowerGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedPowerGaborFilterR2(const MatchedPowerGaborFilterR2& gf);

        /**
        * destructor
        */
        ~MatchedPowerGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedPowerGaborFilterR2<INPUT, OUTPUT>::MatchedPowerGaborFilterR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedPowerGaborFilterR2<INPUT, OUTPUT>::MatchedPowerGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= theta0;
        while ( tmp < theta1 )
        {
            this->push_back(new PowerGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma, powerFactor));
            tmp+= step;
        }

        std::stringstream ss;
        ss << "MatchedPowerGaborFilterR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma << " " << powerFactor;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedPowerGaborFilterR2<INPUT, OUTPUT>::MatchedPowerGaborFilterR2(const MatchedPowerGaborFilterR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedPowerGaborFilterR2<INPUT, OUTPUT>::~MatchedPowerGaborFilterR2()
    {
    }


    template<typename INPUT, typename OUTPUT>
    class MatchedCorrelationPowerGaborFilterR2: public MaxFilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using MaxFilterSet2<INPUT, OUTPUT, float>::apply;
        using MaxFilterSet2<INPUT, OUTPUT, float>::descriptor;
        using MaxFilterSet2<INPUT, OUTPUT, float>::updateStride;
        using MaxFilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using MaxFilterSet2<INPUT, OUTPUT, float>::push_back;

    /**
    * GaborFilter2I default constructor
    */
        MatchedCorrelationPowerGaborFilterR2();

        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta0 smallest angle
         * @param step angle step
         * @param theta1 largest angle
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        MatchedCorrelationPowerGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedCorrelationPowerGaborFilterR2(const MatchedCorrelationPowerGaborFilterR2& gf);

        /**
        * destructor
        */
        ~MatchedCorrelationPowerGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::MatchedCorrelationPowerGaborFilterR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::MatchedCorrelationPowerGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= theta0;
        while ( tmp < theta1 )
        {
            this->push_back(new CorrelationPowerGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma, powerFactor));
            tmp+= step;
        }

        std::stringstream ss;
        ss << "MatchedCorrelationPowerGaborFilterR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma << " " << powerFactor;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::MatchedCorrelationPowerGaborFilterR2(const MatchedCorrelationPowerGaborFilterR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::~MatchedCorrelationPowerGaborFilterR2()
    {
    }



    template<typename INPUT, typename OUTPUT>
    class MatchedCCorrelationPowerGaborFilterR2: public MaxFilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using MaxFilterSet2<INPUT, OUTPUT, float>::apply;
        using MaxFilterSet2<INPUT, OUTPUT, float>::descriptor;
        using MaxFilterSet2<INPUT, OUTPUT, float>::updateStride;
        using MaxFilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using MaxFilterSet2<INPUT, OUTPUT, float>::push_back;

    /**
    * GaborFilter2I default constructor
    */
        MatchedCCorrelationPowerGaborFilterR2();

        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta0 smallest angle
         * @param step angle step
         * @param theta1 largest angle
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        MatchedCCorrelationPowerGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor, float scale= 1);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedCCorrelationPowerGaborFilterR2(const MatchedCCorrelationPowerGaborFilterR2& gf);

        /**
        * destructor
        */
        ~MatchedCCorrelationPowerGaborFilterR2();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<int>& direction, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::MatchedCCorrelationPowerGaborFilterR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::MatchedCCorrelationPowerGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor, float scale)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= theta0;
        while ( tmp < theta1 )
        {
            this->push_back(new CCorrelationPowerGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma, powerFactor, scale));
            tmp+= step;
        }

        std::stringstream ss;
        ss << "MatchedCCorrelationPowerGaborFilterR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma << " " << powerFactor;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::MatchedCCorrelationPowerGaborFilterR2(const MatchedCCorrelationPowerGaborFilterR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::~MatchedCCorrelationPowerGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void MatchedCCorrelationPowerGaborFilterR2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<int>& direction, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->MaxFilterSet2<INPUT, OUTPUT, float>::updateStride(input.columns);

        //#pragma omp parallel for
        for ( int i= -this->getMin(); i < int(input.n) - this->getMax(); ++i )
        {
            float max;
            float tmp;
            int maxIdx;
            if ( !roi || (*roi)(i) )
            {
                maxIdx= -1;
                max= -FLT_MAX;
                for ( unsigned int j= 0; j < this->size(); ++j )
                {
                    tmp= (*this)[j]->apply(input, i, support);
                    if ( tmp > max )
                    {
                        max= tmp;
                        maxIdx= j;
                    }
                }

                output(i)= max;
                direction(i)= maxIdx;
            }
        }

        return;
    }





    /**
    * GaborFilter2 represents a 2D Gabor filter
    */
    template<typename INPUT, typename OUTPUT>
    class PowerGaborFilterSimpleR2: public Filter2<INPUT, OUTPUT, float>
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
        PowerGaborFilterSimpleR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        PowerGaborFilterSimpleR2(float sigma, float theta, float lambda, float psi, float gamma, float powerFactor);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        PowerGaborFilterSimpleR2(const PowerGaborFilterSimpleR2& gf);

        /**
        * destructor
        */
        ~PowerGaborFilterSimpleR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    PowerGaborFilterSimpleR2<INPUT, OUTPUT>::PowerGaborFilterSimpleR2()
        : Filter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    PowerGaborFilterSimpleR2<INPUT, OUTPUT>::PowerGaborFilterSimpleR2(float sigma, float theta, float lambda, float psi, float gamma, float powerFactor)
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

                weight= cos(2*M_PI/lambda*x_theta + psi);
                weight*= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x) + y_theta*y_theta/(sigma_y*sigma_y)));
                //weight*= 1.0/(2*M_PI*sigma_x) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)));
    #if CUT_FILTER
                /*if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.001 )*/
                if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                //if ( abs(x_theta) > NSIGMA * sigma_x )
                    continue;
    #endif
                weight= weight > 0 ? pow(fabs(weight), powerFactor) : -pow(fabs(weight), powerFactor);
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
        ss << "PowerGaborFilterSimpleR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    PowerGaborFilterSimpleR2<INPUT, OUTPUT>::PowerGaborFilterSimpleR2(const PowerGaborFilterSimpleR2& gf)
    : Filter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    PowerGaborFilterSimpleR2<INPUT, OUTPUT>::~PowerGaborFilterSimpleR2()
    {
    }



    template<typename INPUT, typename OUTPUT>
    class CorrelationPowerGaborFilterSimpleR2: public CorrelationFilter2<INPUT, OUTPUT, float>
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
        CorrelationPowerGaborFilterSimpleR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        CorrelationPowerGaborFilterSimpleR2(float sigma, float theta, float lambda, float psi, float gamma, float powerFactor);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        CorrelationPowerGaborFilterSimpleR2(const CorrelationPowerGaborFilterSimpleR2& gf);

        /**
        * destructor
        */
        ~CorrelationPowerGaborFilterSimpleR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    CorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::CorrelationPowerGaborFilterSimpleR2()
        : CorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::CorrelationPowerGaborFilterSimpleR2(float sigma, float theta, float lambda, float psi, float gamma, float powerFactor)
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

                weight= cos(2*M_PI/lambda*x_theta + psi);
                weight*= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x) + y_theta*y_theta/(sigma_y*sigma_y)));
                //weight*= 1.0/(2*M_PI*sigma_x) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)));
#if CUT_FILTER
                /*if ( 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x)
                                                              + y_theta*y_theta/(sigma_y*sigma_y))) < 0.00001 )*/
                if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                //if ( abs(x_theta) > NSIGMA * sigma_x )
                    continue;
#endif
                weight= weight > 0 ? pow(fabs(weight), powerFactor) : -pow(fabs(weight), powerFactor);
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
        ss << "CorrelationPowerGaborFilterSimpleR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::CorrelationPowerGaborFilterSimpleR2(const CorrelationPowerGaborFilterSimpleR2& gf)
    : CorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    CorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::~CorrelationPowerGaborFilterSimpleR2()
    {
    }



    template<typename INPUT, typename OUTPUT>
    class CCorrelationPowerGaborFilterSimpleR2: public CCorrelationFilter2<INPUT, OUTPUT, float>
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
        CCorrelationPowerGaborFilterSimpleR2();
        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta orientation
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        CCorrelationPowerGaborFilterSimpleR2(float sigma, float theta, float lambda, float psi, float gamma, float powerFactor, float scale= 1);

        /**
        * GaborFilter2 copy constructor
        * @param gf instance to copy
        */
        CCorrelationPowerGaborFilterSimpleR2(const CCorrelationPowerGaborFilterSimpleR2& gf);

        /**
        * destructor
        */
        ~CCorrelationPowerGaborFilterSimpleR2();

        /** angle of the filter */
        float angle;
    };

    template<typename INPUT, typename OUTPUT>
    CCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::CCorrelationPowerGaborFilterSimpleR2()
        : CCorrelationFilter2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::CCorrelationPowerGaborFilterSimpleR2(float sigma, float theta, float lambda, float psi, float gamma, float powerFactor, float scale)
        : CCorrelationFilter2<INPUT, OUTPUT, float>()
    {
      //printf("ccpgfsr2 scale: %f\n", scale);
        stride= 4000;
        float sigma_x= sigma;
        float sigma_y= sigma/gamma;

        int nstds= 3;
        float xmax= max(fabs(nstds*sigma_x*cos(theta)*scale), fabs(nstds*sigma_y*sin(theta))*scale);
        xmax= ceil(max(1.0f, xmax));
        float ymax= max(fabs(nstds*sigma_x*sin(theta)*scale), fabs(nstds*sigma_y*cos(theta)*scale));
        ymax= ceil(max(1.0f, ymax));
	//printf("ccpgfsr2 xmax, ymax: %f %f\n", xmax, ymax);

        xmax = xmax > ymax ? xmax : ymax;
        ymax= ymax > xmax ? ymax : xmax;

        float xmin= -xmax;
        float ymin= -ymax;

        float weight;
        float x_theta;
        float y_theta;

        //printf("%f %f\n", xmax, ymax);
        for ( int i= xmin; i <= xmax; ++i )
            for ( int j= ymin; j <= ymax; ++j )
            {
                x_theta= i/scale*cos(theta) - j/scale*sin(theta);
                y_theta= i/scale*sin(theta) + j/scale*cos(theta);

                weight= cos(2*M_PI/lambda*x_theta + psi);

                weight*= 1.0/(2*M_PI*sigma_x*sigma_y) * exp(-0.5*(x_theta*x_theta/(sigma_x*sigma_x) + y_theta*y_theta/(sigma_y*sigma_y)));
#if CUT_FILTER
                if ( abs(x_theta) > NSIGMA * sigma_x || abs(y_theta) > NSIGMA * sigma_y )
                    continue;
#endif
                weight= weight > 0 ? pow(fabs(weight), powerFactor) : -pow(fabs(weight), powerFactor);
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
        ss << "CCorrelationPowerGaborFilterSimpleR2" << " " << sigma << " " << theta << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();

        for ( typename CCorrelationFilter2<INPUT, OUTPUT, float>::fIt fit= this->begin(); fit != this->end(); ++fit )
            meanF+= fit->second;

        meanF/= this->size();

        for ( typename CCorrelationFilter2<INPUT, OUTPUT, float>::fIt fit= this->begin(); fit != this->end(); ++fit )
            devF+= (fit->second - meanF) * (fit->second - meanF);

        devF= sqrt(devF);
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::CCorrelationPowerGaborFilterSimpleR2(const CCorrelationPowerGaborFilterSimpleR2& gf)
    : CCorrelationFilter2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    CCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::~CCorrelationPowerGaborFilterSimpleR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedPowerGaborFilterSimpleR2: public MaxFilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using MaxFilterSet2<INPUT, OUTPUT, float>::apply;
        using MaxFilterSet2<INPUT, OUTPUT, float>::descriptor;
        using MaxFilterSet2<INPUT, OUTPUT, float>::updateStride;
        using MaxFilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using MaxFilterSet2<INPUT, OUTPUT, float>::push_back;

    /**
    * GaborFilter2I default constructor
    */
        MatchedPowerGaborFilterSimpleR2();

        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta0 smallest angle
         * @param step angle step
         * @param theta1 largest angle
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        MatchedPowerGaborFilterSimpleR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedPowerGaborFilterSimpleR2(const MatchedPowerGaborFilterSimpleR2& gf);

        /**
        * destructor
        */
        ~MatchedPowerGaborFilterSimpleR2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedPowerGaborFilterSimpleR2<INPUT, OUTPUT>::MatchedPowerGaborFilterSimpleR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedPowerGaborFilterSimpleR2<INPUT, OUTPUT>::MatchedPowerGaborFilterSimpleR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= theta0;
        while ( tmp < theta1 )
        {
            this->push_back(new PowerGaborFilterSimpleR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma, powerFactor));
            tmp+= step;
        }

        std::stringstream ss;
        ss << "MatchedPowerGaborFilterSimpleR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma << " " << powerFactor;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedPowerGaborFilterSimpleR2<INPUT, OUTPUT>::MatchedPowerGaborFilterSimpleR2(const MatchedPowerGaborFilterSimpleR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedPowerGaborFilterSimpleR2<INPUT, OUTPUT>::~MatchedPowerGaborFilterSimpleR2()
    {
    }


    template<typename INPUT, typename OUTPUT>
    class MatchedCorrelationPowerGaborFilterSimpleR2: public MaxFilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using MaxFilterSet2<INPUT, OUTPUT, float>::apply;
        using MaxFilterSet2<INPUT, OUTPUT, float>::descriptor;
        using MaxFilterSet2<INPUT, OUTPUT, float>::updateStride;
        using MaxFilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using MaxFilterSet2<INPUT, OUTPUT, float>::push_back;

    /**
    * GaborFilter2I default constructor
    */
        MatchedCorrelationPowerGaborFilterSimpleR2();

        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta0 smallest angle
         * @param step angle step
         * @param theta1 largest angle
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        MatchedCorrelationPowerGaborFilterSimpleR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedCorrelationPowerGaborFilterSimpleR2(const MatchedCorrelationPowerGaborFilterSimpleR2& gf);

        /**
        * destructor
        */
        ~MatchedCorrelationPowerGaborFilterSimpleR2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::MatchedCorrelationPowerGaborFilterSimpleR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::MatchedCorrelationPowerGaborFilterSimpleR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= theta0;
        while ( tmp < theta1 )
        {
            this->push_back(new CorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma, powerFactor));
            tmp+= step;
        }

        std::stringstream ss;
        ss << "MatchedCorrelationPowerGaborFilterSimpleR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma << " " << powerFactor;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::MatchedCorrelationPowerGaborFilterSimpleR2(const MatchedCorrelationPowerGaborFilterSimpleR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::~MatchedCorrelationPowerGaborFilterSimpleR2()
    {
    }



    template<typename INPUT, typename OUTPUT>
    class MatchedCCorrelationPowerGaborFilterSimpleR2: public MaxFilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using MaxFilterSet2<INPUT, OUTPUT, float>::apply;
        using MaxFilterSet2<INPUT, OUTPUT, float>::descriptor;
        using MaxFilterSet2<INPUT, OUTPUT, float>::updateStride;
        using MaxFilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using MaxFilterSet2<INPUT, OUTPUT, float>::push_back;

    /**
    * GaborFilter2I default constructor
    */
        MatchedCCorrelationPowerGaborFilterSimpleR2();

        /**
         * 2D Gabor Filter constructor
         * @param sigma variance
         * @param theta0 smallest angle
         * @param step angle step
         * @param theta1 largest angle
         * @param lambda wavelength
         * @param psi phase offset
         * @param gamma aspect ration
         */
        MatchedCCorrelationPowerGaborFilterSimpleR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor, float scale= 1);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedCCorrelationPowerGaborFilterSimpleR2(const MatchedCCorrelationPowerGaborFilterSimpleR2& gf);

        /**
        * destructor
        */
        ~MatchedCCorrelationPowerGaborFilterSimpleR2();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<int>& direction, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::MatchedCCorrelationPowerGaborFilterSimpleR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::MatchedCCorrelationPowerGaborFilterSimpleR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma, float powerFactor, float scale)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
      //printf("mccpgfsr2 scale: %f\n", scale);
        float tmp= theta0;
        while ( tmp < theta1 )
        {
            this->push_back(new CCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma, powerFactor, scale));
            tmp+= step;
        }

        std::stringstream ss;
        ss << "MatchedCCorrelationPowerGaborFilterSimpleR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma << " " << powerFactor;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::MatchedCCorrelationPowerGaborFilterSimpleR2(const MatchedCCorrelationPowerGaborFilterSimpleR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::~MatchedCCorrelationPowerGaborFilterSimpleR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void MatchedCCorrelationPowerGaborFilterSimpleR2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<int>& direction, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->MaxFilterSet2<INPUT, OUTPUT, float>::updateStride(input.columns);

        //#pragma omp parallel for
        for ( int i= -this->getMin(); i < input.n - this->getMax(); ++i )
        {
            float max;
            float tmp;
            int maxIdx;
            if ( !roi || (*roi)(i) )
            {
                maxIdx= -1;
                max= -FLT_MAX;
                for ( int j= 0; j < this->size(); ++j )
                {
                    tmp= (*this)[j]->apply(input, i, support);
                    if ( tmp > max )
                    {
                        max= tmp;
                        maxIdx= j;
                    }
                }

                output(i)= max;
                direction(i)= maxIdx;
            }
        }

        return;
    }
}

#endif // POWEREDGABORFILTER_H

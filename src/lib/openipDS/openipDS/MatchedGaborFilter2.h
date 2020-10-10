#ifndef MATCHEDGABORFILTER2_H
#define MATCHEDGABORFILTER2_H

#include <openipDS/MatchedFilter2s.h>
#include <openipDS/GaborFilter2.h>

namespace openip
{
    /**
     * Matched Gabor filter magnitude
     */
    template<typename INPUT, typename OUTPUT>
    class MatchedGaborFilter2Magnitude: public MaxFilterSet2<INPUT, OUTPUT, float>
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
        MatchedGaborFilter2Magnitude();

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
        MatchedGaborFilter2Magnitude(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedGaborFilter2Magnitude(const MatchedGaborFilter2Magnitude& gf);

        /**
        * destructor
        */
        ~MatchedGaborFilter2Magnitude();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedGaborFilter2Magnitude<INPUT, OUTPUT>::MatchedGaborFilter2Magnitude()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaborFilter2Magnitude<INPUT, OUTPUT>::MatchedGaborFilter2Magnitude(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= theta0;
        while ( tmp < theta1 )
        {
            this->push_back(new GaborFilter2Magnitude<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma));
            tmp+= step;
        }

        std::stringstream ss;
        ss << "MatchedGaborFilter2Magnitude" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaborFilter2Magnitude<INPUT, OUTPUT>::MatchedGaborFilter2Magnitude(const MatchedGaborFilter2Magnitude& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaborFilter2Magnitude<INPUT, OUTPUT>::~MatchedGaborFilter2Magnitude()
    {
    }

    /**
     * Matched Gabor filter magnitude (only the real part)
     */
    template<typename INPUT, typename OUTPUT>
    class MatchedGaborFilterR2: public MaxFilterSet2<INPUT, OUTPUT, float>
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
        MatchedGaborFilterR2();

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
        MatchedGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedGaborFilterR2(const MatchedGaborFilterR2& gf);

        /**
        * destructor
        */
        ~MatchedGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedGaborFilterR2<INPUT, OUTPUT>::MatchedGaborFilterR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaborFilterR2<INPUT, OUTPUT>::MatchedGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= theta0;
        while ( tmp < theta1 )
        {
            this->push_back(new FixedSizeGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma));
            tmp+= step;
        }

        std::stringstream ss;
        ss << "MatchedGaborFilterR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaborFilterR2<INPUT, OUTPUT>::MatchedGaborFilterR2(const MatchedGaborFilterR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaborFilterR2<INPUT, OUTPUT>::~MatchedGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedCorrelationGaborFilterR2: public MaxFilterSet2<INPUT, OUTPUT, float>
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
        MatchedCorrelationGaborFilterR2();

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
        MatchedCorrelationGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedCorrelationGaborFilterR2(const MatchedCorrelationGaborFilterR2& gf);

        /**
        * destructor
        */
        ~MatchedCorrelationGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationGaborFilterR2<INPUT, OUTPUT>::MatchedCorrelationGaborFilterR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationGaborFilterR2<INPUT, OUTPUT>::MatchedCorrelationGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= theta0;
        while ( tmp < theta1 )
        {
            this->push_back(new CorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma));
            tmp+= step;
        }

        std::stringstream ss;
        ss << "MatchedCorrelationGaborFilterR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationGaborFilterR2<INPUT, OUTPUT>::MatchedCorrelationGaborFilterR2(const MatchedCorrelationGaborFilterR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationGaborFilterR2<INPUT, OUTPUT>::~MatchedCorrelationGaborFilterR2()
    {
    }


    template<typename INPUT, typename OUTPUT>
    class MatchedCCorrelationGaborFilterR2: public MaxFilterSet2<INPUT, OUTPUT, float>
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
        MatchedCCorrelationGaborFilterR2();

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
        MatchedCCorrelationGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedCCorrelationGaborFilterR2(const MatchedCCorrelationGaborFilterR2& gf);

        /**
        * destructor
        */
        ~MatchedCCorrelationGaborFilterR2();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<int>& direction, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationGaborFilterR2<INPUT, OUTPUT>::MatchedCCorrelationGaborFilterR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationGaborFilterR2<INPUT, OUTPUT>::MatchedCCorrelationGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= theta0;
        while ( tmp < theta1 )
        {
            this->push_back(new CCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma));
            tmp+= step;
        }

        std::stringstream ss;
        ss << "MatchedCCorrelationGaborFilterR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationGaborFilterR2<INPUT, OUTPUT>::MatchedCCorrelationGaborFilterR2(const MatchedCCorrelationGaborFilterR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationGaborFilterR2<INPUT, OUTPUT>::~MatchedCCorrelationGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void MatchedCCorrelationGaborFilterR2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<int>& direction, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->MaxFilterSet2<INPUT, OUTPUT, float>::updateStride(input.columns);

        #pragma omp parallel for
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

    template<typename INPUT, typename OUTPUT>
    class MatchedPWCMTMCCorrGaborFilterR2: public MaxFilterSet2<INPUT, OUTPUT, float>
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
        MatchedPWCMTMCCorrGaborFilterR2();

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
        MatchedPWCMTMCCorrGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedPWCMTMCCorrGaborFilterR2(const MatchedPWCMTMCCorrGaborFilterR2& gf);

        /**
        * destructor
        */
        ~MatchedPWCMTMCCorrGaborFilterR2();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<int>& direction, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
	
	MatchedCCorrelationGaborFilterR2<float, float> directions;
    };

    template<typename INPUT, typename OUTPUT>
    MatchedPWCMTMCCorrGaborFilterR2<INPUT, OUTPUT>::MatchedPWCMTMCCorrGaborFilterR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedPWCMTMCCorrGaborFilterR2<INPUT, OUTPUT>::MatchedPWCMTMCCorrGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= theta0;
        while ( tmp < theta1 )
        {
	    directions.push_back(new CCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma));
            this->push_back(new PWCMTMCCorrFilter2<INPUT, OUTPUT, float>(*(new CCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma))));
            tmp+= step;
        }

        std::stringstream ss;
        ss << "MatchedPWCMTMCCorrGaborFilterR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedPWCMTMCCorrGaborFilterR2<INPUT, OUTPUT>::MatchedPWCMTMCCorrGaborFilterR2(const MatchedPWCMTMCCorrGaborFilterR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedPWCMTMCCorrGaborFilterR2<INPUT, OUTPUT>::~MatchedPWCMTMCCorrGaborFilterR2()
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void MatchedPWCMTMCCorrGaborFilterR2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<int>& /*direction*/, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
      updateStride(input.columns);
      for ( unsigned int i= 0; i < directions.size(); ++i )
      {
	directions[i]->updateStride(input.columns);
      }
      
      float max= -FLT_MAX, tmp;
      int maxIdx= 0;
      for ( int i= -this->getMin(); i < int(input.n) - this->getMax(); ++i )
      {
	max= -FLT_MAX;
	if ( !roi || (*roi)(i) )
	{
	  for ( unsigned int j= 0; j < this->size(); ++j )
	  {
	    tmp= directions[j]->apply(input, i, support);
	    if ( tmp > max )
	    {
	      max= tmp;
	      maxIdx= j;
	    }
	  }
	  output(i)= (*this)[maxIdx]->apply(input, i, support);
	}
      }
      
      float corrmin= 1;
      for ( unsigned int i= 0; i < output.n; ++i )
	if ( output(i) > 0 && output(i) < corrmin )
	  corrmin= output(i);
      for ( unsigned int i= 0; i < output.n; ++i )
	if ( output(i) == 0 )
	  output(i)= corrmin;
      output.normalize(0, 1);
    }
    
    /**
     * Matched Gabor filter orientation
     */
    template<typename INPUT, typename OUTPUT>
    class MatchedGaborFilterR2Orientation: public MaxFilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using MaxFilterSet2<INPUT, OUTPUT, float>::descriptor;
        using MaxFilterSet2<INPUT, OUTPUT, float>::apply;
        using MaxFilterSet2<INPUT, OUTPUT, float>::updateStride;
        using MaxFilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using MaxFilterSet2<INPUT, OUTPUT, float>::push_back;

        /**
         * constructor
         */
        MatchedGaborFilterR2Orientation();

        /**
         * constructor
         * @param sigma sigma parameter
         * @param theta0 smallest angle parameter
         * @param step angle step
         * @param theta1 largest angle paramter
         * @param lambda labmda parameter
         * @param psi psi paramter
         * @param gamma gamma parameter
         */
        MatchedGaborFilterR2Orientation(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
         * copy constructor
         * @param gf instance to copy
         */
        MatchedGaborFilterR2Orientation(const MatchedGaborFilterR2Orientation& gf);

        /**
         * destructor
         */
        ~MatchedGaborFilterR2Orientation();

        /**
         * computes the filter response in the nth row-major position of image input
         * @param input input image
         * @param n row-major position
         * @return computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n);
    };

    template<typename INPUT, typename OUTPUT>
    OUTPUT MatchedGaborFilterR2Orientation<INPUT, OUTPUT>::apply(Image<INPUT>& in, int n)
    {
        OUTPUT tmp;
        OUTPUT max= -FLT_MAX;
        OUTPUT angle= 0;

        for ( typename MatchedGaborFilterR2Orientation<INPUT, OUTPUT>::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            tmp= (*fit)->apply(in, n);
            if ( tmp > max )
            {
                max= tmp;
                angle= ((GaborFilterR2<INPUT, OUTPUT>*)(*fit))->angle;
            }
        }

        return angle;
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaborFilterR2Orientation<INPUT, OUTPUT>::MatchedGaborFilterR2Orientation()
        : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaborFilterR2Orientation<INPUT, OUTPUT>::MatchedGaborFilterR2Orientation(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
        : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= theta0;
        while ( tmp < theta1 )
        {
            this->push_back(new GaborFilterR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma));
            tmp+= step;
        }

        std::stringstream ss;
        ss << "MatchedGaborFilterR2Orientation" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaborFilterR2Orientation<INPUT, OUTPUT>::MatchedGaborFilterR2Orientation(const MatchedGaborFilterR2Orientation& gf)
        : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaborFilterR2Orientation<INPUT, OUTPUT>::~MatchedGaborFilterR2Orientation()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMultiScaleGaborFilterR2: public MaxFilterSet2<INPUT, OUTPUT, float>
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
        MatchedMultiScaleGaborFilterR2();

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
        MatchedMultiScaleGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedMultiScaleGaborFilterR2(const MatchedMultiScaleGaborFilterR2& gf);

        /**
        * destructor
        */
        ~MatchedMultiScaleGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMultiScaleGaborFilterR2<INPUT, OUTPUT>::MatchedMultiScaleGaborFilterR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMultiScaleGaborFilterR2<INPUT, OUTPUT>::MatchedMultiScaleGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        {
            float tmp= theta0;
            while ( tmp < theta1 )
            {
                this->push_back(new GaborFilterR2<INPUT, OUTPUT>(sigma, tmp, l, psi, gamma));
                tmp+= step;
            }
        }

        std::stringstream ss;
        ss << "MatchedMultiScaleGaborFilterR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMultiScaleGaborFilterR2<INPUT, OUTPUT>::MatchedMultiScaleGaborFilterR2(const MatchedMultiScaleGaborFilterR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMultiScaleGaborFilterR2<INPUT, OUTPUT>::~MatchedMultiScaleGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMultiScaleCorrelationGaborFilterR2: public MaxFilterSet2<INPUT, OUTPUT, float>
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
        MatchedMultiScaleCorrelationGaborFilterR2();

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
        MatchedMultiScaleCorrelationGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedMultiScaleCorrelationGaborFilterR2(const MatchedMultiScaleCorrelationGaborFilterR2& gf);

        /**
        * destructor
        */
        ~MatchedMultiScaleCorrelationGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMultiScaleCorrelationGaborFilterR2<INPUT, OUTPUT>::MatchedMultiScaleCorrelationGaborFilterR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMultiScaleCorrelationGaborFilterR2<INPUT, OUTPUT>::MatchedMultiScaleCorrelationGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        {
            float tmp= theta0;
            while ( tmp < theta1 )
            {
                this->push_back(new FixedSizeCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, l, psi, gamma));
                tmp+= step;
            }
        }

        std::stringstream ss;
        ss << "MatchedMultiScaleCorrelationGaborFilterR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMultiScaleCorrelationGaborFilterR2<INPUT, OUTPUT>::MatchedMultiScaleCorrelationGaborFilterR2(const MatchedMultiScaleCorrelationGaborFilterR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMultiScaleCorrelationGaborFilterR2<INPUT, OUTPUT>::~MatchedMultiScaleCorrelationGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMultiScaleCCorrelationGaborFilterR2: public MaxFilterSet2<INPUT, OUTPUT, float>
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
        MatchedMultiScaleCCorrelationGaborFilterR2();

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
        MatchedMultiScaleCCorrelationGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedMultiScaleCCorrelationGaborFilterR2(const MatchedMultiScaleCCorrelationGaborFilterR2& gf);

        /**
        * destructor
        */
        ~MatchedMultiScaleCCorrelationGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMultiScaleCCorrelationGaborFilterR2<INPUT, OUTPUT>::MatchedMultiScaleCCorrelationGaborFilterR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMultiScaleCCorrelationGaborFilterR2<INPUT, OUTPUT>::MatchedMultiScaleCCorrelationGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda0, float lambdas, float lambda1, float psi, float gamma)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        for ( float l= lambda0; l < lambda1; l+= lambdas )
        {
            float tmp= theta0;
            while ( tmp < theta1 )
            {
                this->push_back(new FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, l, psi, gamma));
                tmp+= step;
            }
        }

        std::stringstream ss;
        ss << "MatchedMultiScaleCCorrelationGaborFilterR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda0 << " " << lambdas << " " << lambda1 << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMultiScaleCCorrelationGaborFilterR2<INPUT, OUTPUT>::MatchedMultiScaleCCorrelationGaborFilterR2(const MatchedMultiScaleCCorrelationGaborFilterR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMultiScaleCCorrelationGaborFilterR2<INPUT, OUTPUT>::~MatchedMultiScaleCCorrelationGaborFilterR2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedCCorrelationJunctionGaborFilterR2: public MaxFilterSet2<INPUT, OUTPUT, float>
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
        MatchedCCorrelationJunctionGaborFilterR2();

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
        MatchedCCorrelationJunctionGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
        * copy constructor
        * @param gf instance to copy
        */
        MatchedCCorrelationJunctionGaborFilterR2(const MatchedCCorrelationJunctionGaborFilterR2& gf);

        /**
        * destructor
        */
        ~MatchedCCorrelationJunctionGaborFilterR2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationJunctionGaborFilterR2<INPUT, OUTPUT>::MatchedCCorrelationJunctionGaborFilterR2()
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationJunctionGaborFilterR2<INPUT, OUTPUT>::MatchedCCorrelationJunctionGaborFilterR2(float sigma, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    : MaxFilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= theta0;
        /*while ( tmp < theta1 )
        {
            this->push_back(new FixedSizeCCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma));
            tmp+= step;
        }*/

        while ( tmp < theta1 )
        {
            //this->push_back(new CCorrelationGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, lambda, psi, gamma));
            tmp+= step;

            //float tmp2= tmp;
            //while ( tmp2 < tmp + 2*M_PI )
            {
                this->push_back(new CCorrelationJunctionGaborFilterR2<INPUT, OUTPUT>(sigma, tmp, M_PI/2, lambda, psi, gamma));
                //tmp2+= step*3;
            }
        }

        std::stringstream ss;
        ss << "MatchedCCorrelationJunctionGaborFilterR2" << " " << sigma << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationJunctionGaborFilterR2<INPUT, OUTPUT>::MatchedCCorrelationJunctionGaborFilterR2(const MatchedCCorrelationJunctionGaborFilterR2& gf)
    : MaxFilterSet2<INPUT, OUTPUT, float>(gf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationJunctionGaborFilterR2<INPUT, OUTPUT>::~MatchedCCorrelationJunctionGaborFilterR2()
    {
    }
}

#endif // MATCHEDGABORFILTER2_H

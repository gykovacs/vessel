/* 
 * File:   MatchedFilter2s.h
 * Author: gykovacs
 *
 * Created on February 10, 2011, 2:08 PM
 */

#ifndef MATCHEDFILTER2S_H
#define	MATCHEDFILTER2S_H

#include <openipDS/Filter2s.h>
#include <openipDS/GaborFilter2.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class MatchedMinFilter2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;
        using FilterSet2<INPUT, OUTPUT, float>::operator[];

        MatchedMinFilter2();

        MatchedMinFilter2(const MatchedMinFilter2& m);

        ~MatchedMinFilter2();

        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMinFilter2<INPUT, OUTPUT>::MatchedMinFilter2()
        : FilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinFilter2<INPUT, OUTPUT>::MatchedMinFilter2(const MatchedMinFilter2 &m)
        : FilterSet2<INPUT, OUTPUT, float>(m)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinFilter2<INPUT, OUTPUT>::~MatchedMinFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MatchedMinFilter2<INPUT, OUTPUT>::apply(Image<INPUT> &input, int n, Image<unsigned char> *support)
    {
        float min= FLT_MAX;
        float tmp;
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            tmp= (*this)[i]->apply(input, n, support);
            if ( tmp < min )
                min= tmp;
        }

        return min;
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMaxFilter2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::updateStride;
        using FilterSet2<INPUT, OUTPUT, float>::computeMinMax;

        MatchedMaxFilter2();

        MatchedMaxFilter2(const MatchedMaxFilter2& m);

        ~MatchedMaxFilter2();

        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMaxFilter2<INPUT, OUTPUT>::MatchedMaxFilter2()
        : FilterSet2<INPUT, OUTPUT, float>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxFilter2<INPUT, OUTPUT>::MatchedMaxFilter2(const MatchedMaxFilter2 &m)
        : FilterSet2<INPUT, OUTPUT, float>(m)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxFilter2<INPUT, OUTPUT>::~MatchedMaxFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT MatchedMaxFilter2<INPUT, OUTPUT>::apply(Image<INPUT> &input, int n, Image<unsigned char> *support)
    {
        float max= -FLT_MAX;
        float tmp;
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            tmp= (*this)[i]->apply(input, n, support);
            if ( tmp > max )
                max= tmp;
        }

        return max;
    }

    /**
     * enumeration used to describe the matched filter mode
     */
    enum MATCHED_GAUSSIAN_FILTER_TYPE
    {
        MATCHED_GAUSSIAN_FILTER_KERNEL= 1,
        MATCHED_GAUSSIAN_FILTER_SIGMA,
        MATCHED_GAUSSIAN_FILTER_ALPHA,
        MATCHED_GAUSSIAN_FILTER_BETA,
        MATCHED_GAUSSIAN_FILTER_DELTA,
        MATCHED_GAUSSIAN_FILTER_GAMMA
    };

    /**
     * Matched filter from Gaussian filters with correlation kernel response
     */
    template<typename INPUT, typename OUTPUT>
    class MatchedCorrelationGaussianFilter2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma variance
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param delta delta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param stride stride of the images to apply to
         */
        MatchedCorrelationGaussianFilter2(int kernel, float sigma, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param m instance to copy
         */
        MatchedCorrelationGaussianFilter2(const MatchedCorrelationGaussianFilter2& m);

        /**
         * set filter match type
         * @param a lower border
         * @param step step
         * @param b upper border
         * @param type matcs type
         */
        void setMatchType(float a, float step, float b, enum MATCHED_GAUSSIAN_FILTER_TYPE type);

        /** stride */
        int stride;
        /** kernel size */
        int kernel;
        /** variance */
        float sigma;
        /** alpha */
        float alpha;
        /** beta */
        float beta;
        /** delta */
        float delta;
        /** gamma */
        float gamma;
    };


    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationGaussianFilter2<INPUT, OUTPUT>::MatchedCorrelationGaussianFilter2(int k, float s, float a, float b, float d, float g, int str)
    : FilterSet2<INPUT, OUTPUT, float>(), stride(str), kernel(k), sigma(s), alpha(a), beta(b), delta(d), gamma(g)
    {
        stringstream ss;
        ss << "MatchedCorrelationGaussianFilter2 " << k << " " << s << " " << a << " " << b << " " << d << " " << g;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationGaussianFilter2<INPUT, OUTPUT>::MatchedCorrelationGaussianFilter2(const MatchedCorrelationGaussianFilter2& m)
    : FilterSet2<INPUT, OUTPUT, float>(m), stride(m.stride), kernel(m.kernel), sigma(m.sigma), alpha(m.alpha), beta(m.beta), delta(m.delta), gamma(m.gamma)
    {
    }

    template<typename INPUT, typename OUTPUT>
    void MatchedCorrelationGaussianFilter2<INPUT, OUTPUT>::setMatchType(float a, float step, float b, enum MATCHED_GAUSSIAN_FILTER_TYPE type)
    {
        this->clear();
        float tmp= a;
        CorrelationFilter2<INPUT, OUTPUT, float>* pf;

        stringstream ss;
        ss << descriptor << " " << a << " " << step << " " << b << " " << type;
        descriptor= ss.str();

        switch(type)
        {
            case MATCHED_GAUSSIAN_FILTER_KERNEL:
                while ( tmp <= b )
                {
                    pf= new CorrelationFilter2<INPUT, OUTPUT, float>(GaussianFilter2<INPUT, OUTPUT>(sigma, tmp, alpha, beta, delta, gamma, stride));
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_SIGMA:
                while ( tmp <= b )
                {
                    pf= new CorrelationFilter2<INPUT, OUTPUT, float>(GaussianFilter2<INPUT, OUTPUT>(tmp, kernel, alpha, beta, delta, gamma, stride));
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_ALPHA:
                while ( tmp <= b )
                {
                    pf= new CorrelationFilter2<INPUT, OUTPUT, float>(GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, tmp, beta, delta, gamma, stride));
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_BETA:
                while ( tmp <= b )
                {
                    pf= new CorrelationFilter2<INPUT, OUTPUT, float>(GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, alpha, tmp, delta, gamma, stride));
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_DELTA:
                while ( tmp <= b )
                {
                    pf= new CorrelationFilter2<INPUT, OUTPUT, float>(GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, alpha, beta, tmp, gamma, stride));
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_GAMMA:
                while ( tmp <= b )
                {
                    pf= new CorrelationFilter2<INPUT, OUTPUT, float>(GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, alpha, beta, delta, tmp, stride));
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
        }
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedCCorrelationGaussianFilter2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::getProposedBorder;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma variance
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param delta delta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param stride stride of the images to apply to
         */
        MatchedCCorrelationGaussianFilter2(int kernel, float sigma, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param m instance to copy
         */
        MatchedCCorrelationGaussianFilter2(const MatchedCCorrelationGaussianFilter2& m);

        /**
         * set filter match type
         * @param a lower border
         * @param step step
         * @param b upper border
         * @param type matcs type
         */
        void setMatchType(float a, float step, float b, enum MATCHED_GAUSSIAN_FILTER_TYPE type);

        /** stride */
        int stride;
        /** kernel size */
        int kernel;
        /** variance */
        float sigma;
        /** alpha */
        float alpha;
        /** beta */
        float beta;
        /** delta */
        float delta;
        /** gamma */
        float gamma;
    };


    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationGaussianFilter2<INPUT, OUTPUT>::MatchedCCorrelationGaussianFilter2(int k, float s, float a, float b, float d, float g, int str)
    : FilterSet2<INPUT, OUTPUT, float>(), stride(str), kernel(k), sigma(s), alpha(a), beta(b), delta(d), gamma(g)
    {
        stringstream ss;
        ss << "MatchedCCorrelationGaussianFilter2 " << k << " " << s << " " << a << " " << b << " " << d << " " << g;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCCorrelationGaussianFilter2<INPUT, OUTPUT>::MatchedCCorrelationGaussianFilter2(const MatchedCCorrelationGaussianFilter2& m)
    : FilterSet2<INPUT, OUTPUT, float>(m), stride(m.stride), kernel(m.kernel), sigma(m.sigma), alpha(m.alpha), beta(m.beta), delta(m.delta), gamma(m.gamma)
    {
    }

    template<typename INPUT, typename OUTPUT>
    void MatchedCCorrelationGaussianFilter2<INPUT, OUTPUT>::setMatchType(float a, float step, float b, enum MATCHED_GAUSSIAN_FILTER_TYPE type)
    {
        this->clear();
        float tmp= a;
        CCorrelationFilter2<INPUT, OUTPUT, float>* pf;

        stringstream ss;
        ss << descriptor << " " << a << " " << step << " " << b << " " << type;
        descriptor= ss.str();

        switch(type)
        {
            case MATCHED_GAUSSIAN_FILTER_KERNEL:
                while ( tmp <= b )
                {
                    pf= new CCorrelationFilter2<INPUT, OUTPUT, float>(GaussianFilter2<INPUT, OUTPUT>(sigma, tmp, alpha, beta, delta, gamma, stride));
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_SIGMA:
                while ( tmp <= b )
                {
                    pf= new CCorrelationFilter2<INPUT, OUTPUT, float>(GaussianFilter2<INPUT, OUTPUT>(tmp, kernel, alpha, beta, delta, gamma, stride));
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_ALPHA:
                while ( tmp <= b )
                {
                    pf= new CCorrelationFilter2<INPUT, OUTPUT, float>(GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, tmp, beta, delta, gamma, stride));
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_BETA:
                while ( tmp <= b )
                {
                    pf= new CCorrelationFilter2<INPUT, OUTPUT, float>(GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, alpha, tmp, delta, gamma, stride));
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_DELTA:
                while ( tmp <= b )
                {
                    pf= new CCorrelationFilter2<INPUT, OUTPUT, float>(GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, alpha, beta, tmp, gamma, stride));
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_GAMMA:
                while ( tmp <= b )
                {
                    pf= new CCorrelationFilter2<INPUT, OUTPUT, float>(GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, alpha, beta, delta, tmp, stride));
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
        }
    }

    /**
     * Matched filter from Gaussian filters with convolution response
     */
    template<typename INPUT, typename OUTPUT>
    class MatchedGaussianFilter2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma variance
         * @param alpha alpha parameter
         * @param beta beta parameter
         * @param delta delta parameter
         * @param gamma gamma parameter
         * @param stride stride of the images to apply to
         */
        MatchedGaussianFilter2(int kernel, float sigma, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param m instance to copy
         */
        MatchedGaussianFilter2(const MatchedGaussianFilter2& m);

        /**
         * set filter type
         * @param a lower border
         * @param step step
         * @param b upper order
         * @param type filter type
         */
        void setMatchType(float a, float step, float b, enum MATCHED_GAUSSIAN_FILTER_TYPE type);

        /** stride */
        int stride;
        /** kernel size */
        int kernel;
        /** variance */
        float sigma;
        /** alpha Gaussian parameter */
        float alpha;
        /** beta Gaussian parameter */
        float beta;
        /** delta Gaussian parameter */
        float delta;
        /** gamma Gaussian parameter */
        float gamma;
    };

    template<typename INPUT, typename OUTPUT>
    MatchedGaussianFilter2<INPUT, OUTPUT>::MatchedGaussianFilter2(int k, float s, float a, float b, float d, float g, int str)
    : FilterSet2<INPUT, OUTPUT, float>(), stride(str), kernel(k), sigma(s), alpha(a), beta(b), delta(d), gamma(g)
    {
        stringstream ss;
        ss << "MatchedGaussianFilter2 " << k << " " << s << " " << a << " " << b << " " << d << " " << g;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaussianFilter2<INPUT, OUTPUT>::MatchedGaussianFilter2(const MatchedGaussianFilter2& m)
    : FilterSet2<INPUT, OUTPUT, float>(m), stride(m.stride), kernel(m.kernel), sigma(m.sigma), alpha(m.alpha), beta(m.beta), delta(m.delta), gamma(m.gamma)
    {
    }

    template<typename INPUT, typename OUTPUT>
    void MatchedGaussianFilter2<INPUT, OUTPUT>::setMatchType(float a, float step, float b, enum MATCHED_GAUSSIAN_FILTER_TYPE type)
    {
        this->clear();
        float tmp= a;
        GaussianFilter2<INPUT, OUTPUT>* pf;
        stringstream ss;
        ss << descriptor << " " << a << " " << step << " " << b << " " << type;
        descriptor= ss.str();

        switch(type)
        {
            case MATCHED_GAUSSIAN_FILTER_KERNEL:
                while ( tmp <= b )
                {
                    pf= new GaussianFilter2<INPUT, OUTPUT>(sigma, tmp, alpha, beta, delta, gamma, stride);
                    pf->normalize();
                    pf->zeroMean();
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_SIGMA:
                while ( tmp <= b )
                {
                    pf= new GaussianFilter2<INPUT, OUTPUT>(tmp, kernel, alpha, beta, delta, gamma, stride);
                    pf->normalize();
                    pf->zeroMean();
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_ALPHA:
                while ( tmp <= b )
                {
                    pf= new GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, tmp, beta, delta, gamma, stride);
                    pf->normalize();
                    pf->zeroMean();
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_BETA:
                while ( tmp <= b )
                {
                    pf= new GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, alpha, tmp, delta, gamma, stride);
                    pf->normalize();
                    pf->zeroMean();
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_DELTA:
                while ( tmp <= b )
                {
                    pf= new GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, alpha, beta, tmp, gamma, stride);
                    pf->normalize();
                    pf->zeroMean();
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
            case MATCHED_GAUSSIAN_FILTER_GAMMA:
                while ( tmp <= b )
                {
                    pf= new GaussianFilter2<INPUT, OUTPUT>(sigma, kernel, alpha, beta, delta, tmp, stride);
                    pf->normalize();
                    pf->zeroMean();
                    for ( unsigned int i= 0; i < pf->size(); ++i )
                        (*pf)[i].second= -(*pf)[i].second;
                    this->push_back(pf);
                    tmp+= step;
                }
                break;
        }
    }

    /**
     * Matched line segment filter with correlation responses
     */
    template<typename INPUT, typename OUTPUT>
    class MatchedCorrelationLineSegmentFilter2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;

        /**
         * constructor
         * @param rows numer of rows
         * @param columns number of columns
         * @param sigma variance
         * @param angle0 smallest angle
         * @param angleStep angle step
         * @param angle1 largest angle
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param delta delta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param stride stride of the images to apply on
         */
        MatchedCorrelationLineSegmentFilter2(int rows, int columns, float sigma, float angle0, float angleStep, float angle1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param m instance to copy
         */
        MatchedCorrelationLineSegmentFilter2(const MatchedCorrelationLineSegmentFilter2& m);

        /**
         * destructor
         */
        ~MatchedCorrelationLineSegmentFilter2();

        /**
         * updates the stride
         * @param stride new stride
         * @return non-zero if the stride is changed, zero otherwise
         */
        virtual int updateStride(int stride);

        /**
         * returns the minimum position of row-major representation in which the filter can be applied
         * @return the minimum position
         */
        virtual int getMin();

        /**
         * returns the maximum position of row-major representation in which the filter can be applied
         * @return the maximum position
         */
        virtual int getMax();
    };

    template<typename INPUT, typename OUTPUT>
    int MatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::updateStride(int stride)
    {
        return this->FilterSet2<INPUT, OUTPUT, float>::updateStride(stride);
    }

    template<typename INPUT, typename OUTPUT>
    int MatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::getMin()
    {
        return this->FilterSet2<INPUT, OUTPUT, float>::min;
    }

    template<typename INPUT, typename OUTPUT>
    int MatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::getMax()
    {
        return this->FilterSet2<INPUT, OUTPUT, float>::max;
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::MatchedCorrelationLineSegmentFilter2(int rows, int columns, float sigma, float angle0, float angleStep, float angle1, float alpha, float beta, float delta, float gamma, int stride)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= angle0;
        while( tmp <= angle1 )
        {
            this->push_back(new CorrelationFilter2<INPUT, OUTPUT, float>(LineSegmentFilter<INPUT, OUTPUT>(rows, columns, sigma, tmp, alpha, beta, delta, gamma, stride)));
            tmp+= angleStep;
        }
        this->computeMinMax();
        stringstream ss;
        ss << "MatchedCorrelationLineSegmentFilter2 " << rows << " " << columns << " " << sigma << " " << angle0 << " " << angleStep << " " << angle1 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::MatchedCorrelationLineSegmentFilter2(const MatchedCorrelationLineSegmentFilter2& m)
    : FilterSet2<INPUT, OUTPUT, float>(m)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>::~MatchedCorrelationLineSegmentFilter2()
    {
    }

    /**
     * Matched line segment filter with convolution response
     */
    template<typename INPUT, typename OUTPUT>
    class MatchedLineSegmentFilter2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::apply;

        /**
         * constructor
         * @param rows number of rows
         * @param columns number of columns
         * @param sigma variance
         * @param angle0 smallest angle
         * @param angleStep angle step
         * @param angle1 largest angle
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param delta delta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param stride stride of the images to apply to
         */
        MatchedLineSegmentFilter2(int rows, int columns, float sigma, float angle0, float angleStep, float angle1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param m instance to copy
         */
        MatchedLineSegmentFilter2(const MatchedLineSegmentFilter2& m);

        /**
         * destructor
         */
        ~MatchedLineSegmentFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedLineSegmentFilter2<INPUT, OUTPUT>::MatchedLineSegmentFilter2(int rows, int columns, float sigma, float angle0, float angleStep, float angle1, float alpha, float beta, float delta, float gamma, int stride)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= angle0;
        while ( tmp <= angle1 )
        {
            this->push_back(new LineSegmentFilter<INPUT, OUTPUT>(columns, rows, sigma, tmp, alpha, beta, delta, gamma, stride));
            tmp+= angleStep;
        }

        this->FilterSet2<INPUT, OUTPUT, float>::computeMinMax();

        stringstream ss;
        ss << "MatchedLineSegmentFilter2 " << rows << " " << columns << " " << sigma << " " << angle0 << " " << angleStep << " " << angle1 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedLineSegmentFilter2<INPUT, OUTPUT>::MatchedLineSegmentFilter2(const MatchedLineSegmentFilter2& m)
    : FilterSet2<INPUT, OUTPUT, float>(m)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedLineSegmentFilter2<INPUT, OUTPUT>::~MatchedLineSegmentFilter2()
    {
    }

    
    template<typename INPUT, typename OUTPUT>
    class MatchedTJunctionFilter2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::apply;
        using FilterSet2<INPUT, OUTPUT, float>::stride;

        /**
         * constructor
         * @param rows number of rows
         * @param columns number of columns
         * @param sigma variance
         * @param angle0 smallest angle
         * @param angleStep angle step
         * @param angle1 largest angle
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param delta delta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param stride stride of the images to apply to
         */
        MatchedTJunctionFilter2(int kernel, float a1, float as, float a2, float b1, float bs, float b2, float c1, float cs, float c2, int r1, int r2, int r3, int stride= 4000);

        /**
         * copy constructor
         * @param m instance to copy
         */
        MatchedTJunctionFilter2(const MatchedTJunctionFilter2& m);

        /**
         * destructor
         */
        ~MatchedTJunctionFilter2();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedTJunctionFilter2<INPUT, OUTPUT>::MatchedTJunctionFilter2(int kernel, float a1, float as, float a2, float b1, float bs, float b2, float c1, float cs, float c2, int r1, int r2, int r3, int stride)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        float ta= a1;
        float tb, tc;

        while ( ta <= a2 )
        {
            tb= b1;
            while ( tb <= b2 )
            {
                tc= c1;
                while ( tc <= c2 )
                {
                    this->push_back(new CorrelationFilter2<INPUT, OUTPUT, float>(TJunctionFilter2<INPUT, OUTPUT>(kernel, ta, tb, tc, r1, r2, r3, stride)));
                    tc+= cs;
                }
                tb+= bs;
            }
            ta+= as;
        }

        this->FilterSet2<INPUT, OUTPUT, float>::computeMinMax();

        stringstream ss;
        ss << "MatchedTJunctionFilter2 " << kernel << " " << a1 << " " << as << " " << a2 << " " << b1 << " " << bs << " " << b2 << " " << c1 << " " << cs << " " << c2 << " " << r1 << " " << r2 << " " << r3 << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedTJunctionFilter2<INPUT, OUTPUT>::MatchedTJunctionFilter2(const MatchedTJunctionFilter2& m)
    : FilterSet2<INPUT, OUTPUT, float>(m)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedTJunctionFilter2<INPUT, OUTPUT>::~MatchedTJunctionFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedGaussianDerivedMagnitude: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;
        using FilterSet2<INPUT, OUTPUT, float>::apply;

        /**
         * constructor
         * @param rows number of rows
         * @param columns number of columns
         * @param sigma variance
         * @param angle0 smallest angle
         * @param angleStep angle step
         * @param angle1 largest angle
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param delta delta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param stride stride of the images to apply to
         */
        MatchedGaussianDerivedMagnitude(int rows, float sigma0, float sigmaStep, float sigma1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param m instance to copy
         */
        MatchedGaussianDerivedMagnitude(const MatchedGaussianDerivedMagnitude& m);

        /**
         * destructor
         */
        ~MatchedGaussianDerivedMagnitude();
    };

    template<typename INPUT, typename OUTPUT>
    MatchedGaussianDerivedMagnitude<INPUT, OUTPUT>::MatchedGaussianDerivedMagnitude(int kernel, float sigma0, float sigmaStep, float sigma1, float alpha, float beta, float delta, float gamma, int stride)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= sigma0;
        while ( tmp <= sigma1 )
        {
            this->push_back(new GaussianDerivedMagnitude<INPUT, OUTPUT>(tmp, kernel, alpha, beta, delta, gamma, stride));
            tmp+= sigmaStep;
        }

        this->FilterSet2<INPUT, OUTPUT, float>::computeMinMax();

        stringstream ss;
        ss << "MatchedGaussianDerivedMagnitude " << kernel << " " << sigma0 << " " << sigmaStep << " " << sigma1 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaussianDerivedMagnitude<INPUT, OUTPUT>::MatchedGaussianDerivedMagnitude(const MatchedGaussianDerivedMagnitude& m)
    : FilterSet2<INPUT, OUTPUT, float>(m)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedGaussianDerivedMagnitude<INPUT, OUTPUT>::~MatchedGaussianDerivedMagnitude()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedLoGFilter2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma variance
         * @param alpha alpha parameter
         * @param beta beta parameter
         * @param delta delta parameter
         * @param gamma gamma parameter
         * @param stride stride of the images to apply to
         */
        MatchedLoGFilter2(int kernel, float sigma0, float step, float sigma1);

        /**
         * copy constructor
         * @param m instance to copy
         */
        MatchedLoGFilter2(const MatchedLoGFilter2& m);

        ~MatchedLoGFilter2();

        /** stride */
        int stride;
        /** kernel size */
        int kernel;
        /** variance */
        float sigma;
    };

    template<typename INPUT, typename OUTPUT>
    MatchedLoGFilter2<INPUT, OUTPUT>::MatchedLoGFilter2(int kernel, float sigma0, float step, float sigma1)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= sigma0;
        while( tmp <= sigma1 )
        {
            this->push_back(new LoGFilter2<INPUT, OUTPUT>(tmp, kernel));
            tmp+= step;
        }
        this->computeMinMax();
        stringstream ss;
        ss << "MatchedLoGFilter2 " << kernel << " " << sigma0 << " " << step << " " << sigma1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedLoGFilter2<INPUT, OUTPUT>::MatchedLoGFilter2(const MatchedLoGFilter2& m)
    : FilterSet2<INPUT, OUTPUT, float>(m)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedLoGFilter2<INPUT, OUTPUT>::~MatchedLoGFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedCorrelationLoGFilter2: public FilterSet2<INPUT, OUTPUT, float>
    {
    public:
        using FilterSet2<INPUT, OUTPUT, float>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma variance
         * @param alpha alpha parameter
         * @param beta beta parameter
         * @param delta delta parameter
         * @param gamma gamma parameter
         * @param stride stride of the images to apply to
         */
        MatchedCorrelationLoGFilter2(int kernel, float sigma0, float step, float sigma1);

        /**
         * copy constructor
         * @param m instance to copy
         */
        MatchedCorrelationLoGFilter2(const MatchedCorrelationLoGFilter2& m);

        ~MatchedCorrelationLoGFilter2();

        /** stride */
        int stride;
        /** kernel size */
        int kernel;
        /** variance */
        float sigma;
    };

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationLoGFilter2<INPUT, OUTPUT>::MatchedCorrelationLoGFilter2(int kernel, float sigma0, float step, float sigma1)
    : FilterSet2<INPUT, OUTPUT, float>()
    {
        float tmp= sigma0;
        while( tmp <= sigma1 )
        {
            this->push_back(new CorrelationFilter2<INPUT, OUTPUT, float>(LoGFilter2<INPUT, OUTPUT>(tmp, kernel)));
            tmp+= step;
        }
        this->computeMinMax();
        stringstream ss;
        ss << "MatchedCorrelationLoGFilter2 " << kernel << " " << sigma0 << " " << step << " " << sigma1;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationLoGFilter2<INPUT, OUTPUT>::MatchedCorrelationLoGFilter2(const MatchedCorrelationLoGFilter2& m)
    : FilterSet2<INPUT, OUTPUT, float>(m)
    {
    }

    template<typename INPUT, typename OUTPUT>
    MatchedCorrelationLoGFilter2<INPUT, OUTPUT>::~MatchedCorrelationLoGFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMinGaussianFilter2: public MatchedMinFilter2<INPUT, OUTPUT>
    {
    public:
        using MatchedMinFilter2<INPUT, OUTPUT>::descriptor;
        using MatchedMinFilter2<INPUT, OUTPUT>::apply;
        using MatchedMinFilter2<INPUT, OUTPUT>::updateStride;
        using MatchedMinFilter2<INPUT, OUTPUT>::computeMinMax;
        using MatchedMinFilter2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma variance
         * @param alpha alpha parameter
         * @param beta beta parameter
         * @param delta delta parameter
         * @param gamma gamma parameter
         * @param stride stride of the images to apply to
         */
        MatchedMinGaussianFilter2(int kernel, float sigma0, float sigmas, float sigma1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param m instance to copy
         */
        MatchedMinGaussianFilter2(const MatchedMinGaussianFilter2& m);

        /** stride */
        int stride;
        /** kernel size */
        int kernel;
        /** variance */
        float sigma0;
        float sigmas;
        float sigma1;
        /** alpha Gaussian parameter */
        float alpha;
        /** beta Gaussian parameter */
        float beta;
        /** delta Gaussian parameter */
        float delta;
        /** gamma Gaussian parameter */
        float gamma;
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMinGaussianFilter2<INPUT, OUTPUT>::MatchedMinGaussianFilter2(int kernel, float sigma0, float sigmas, float sigma1, float alpha, float beta, float delta, float gamma, int stride)
        : MatchedMinFilter2<INPUT, OUTPUT>()
    {
        this->clear();
        float tmp= sigma0;
        GaussianFilter2<INPUT, OUTPUT>* pf;
        stringstream ss;
        ss << "MatchedMinGaussianFilter2 " << kernel << " " << sigma0 << " " << sigmas << " " << sigma1 << " " << alpha << " " << beta << " " << delta << " " << gamma << " " << stride;
        descriptor= ss.str();

        while ( tmp <= sigma1 )
        {
            pf= new GaussianFilter2<INPUT, OUTPUT>(tmp, kernel, alpha, beta, delta, gamma, stride);
            pf->normalize();
            this->push_back(pf);
            tmp+= sigmas;
        }
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMinGaussianFilter2<INPUT, OUTPUT>::MatchedMinGaussianFilter2(const MatchedMinGaussianFilter2& m)
        : MatchedMinFilter2<INPUT, OUTPUT>(m)
    {
    }

    template<typename INPUT, typename OUTPUT>
    class MatchedMaxGaussianFilter2: public MatchedMaxFilter2<INPUT, OUTPUT>
    {
    public:
        using MatchedMaxFilter2<INPUT, OUTPUT>::descriptor;
        using MatchedMaxFilter2<INPUT, OUTPUT>::apply;
        using MatchedMaxFilter2<INPUT, OUTPUT>::updateStride;
        using MatchedMaxFilter2<INPUT, OUTPUT>::computeMinMax;
        using MatchedMaxFilter2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma variance
         * @param alpha alpha parameter
         * @param beta beta parameter
         * @param delta delta parameter
         * @param gamma gamma parameter
         * @param stride stride of the images to apply to
         */
        MatchedMaxGaussianFilter2(int kernel, float sigma0, float sigmas, float sigma1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param m instance to copy
         */
        MatchedMaxGaussianFilter2(const MatchedMaxGaussianFilter2& m);

        /** stride */
        int stride;
        /** kernel size */
        int kernel;
        /** variance */
        float sigma0;
        float sigmas;
        float sigma1;
        /** alpha Gaussian parameter */
        float alpha;
        /** beta Gaussian parameter */
        float beta;
        /** delta Gaussian parameter */
        float delta;
        /** gamma Gaussian parameter */
        float gamma;
    };

    template<typename INPUT, typename OUTPUT>
    MatchedMaxGaussianFilter2<INPUT, OUTPUT>::MatchedMaxGaussianFilter2(int kernel, float sigma0, float sigmas, float sigma1, float alpha, float beta, float delta, float gamma, int stride)
        : MatchedMaxFilter2<INPUT, OUTPUT>()
    {
        this->clear();
        float tmp= sigma0;
        GaussianFilter2<INPUT, OUTPUT>* pf;
        stringstream ss;
        ss << "MatchedMaxGaussianFilter2 " << kernel << " " << sigma0 << " " << sigmas << " " << sigma1 << " " << alpha << " " << beta << " " << delta << " " << gamma << " " << stride;
        descriptor= ss.str();

        while ( tmp <= sigma1 )
        {
            pf= new GaussianFilter2<INPUT, OUTPUT>(tmp, kernel, alpha, beta, delta, gamma, stride);
            pf->normalize();
            this->push_back(pf);
            tmp+= sigmas;
        }
    }

    template<typename INPUT, typename OUTPUT>
    MatchedMaxGaussianFilter2<INPUT, OUTPUT>::MatchedMaxGaussianFilter2(const MatchedMaxGaussianFilter2& m)
        : MatchedMaxFilter2<INPUT, OUTPUT>(m)
    {
    }
    
    
}

#endif	/* MATCHEDFILTER2S_H */


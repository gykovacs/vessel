/**
 * @file RatioFeature2s.h
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
 * This file contains some ratio features.
 */

#ifndef _RATIOFEATURE2S_H
#define	_RATIOFEATURE2S_H

#include <openipDS/Filter.h>
#include <openipDS/Feature2.h>
#include <openipDS/StatisticalFeature2s.h>
#include <openipDS/Filter2s.h>

namespace openip
{
    /**
     * RatioFeature2 is the base class of all ratio features
     */
    template<typename INPUT, typename OUTPUT>
    class RatioFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::descriptor;

        /**
         * default constructor
         */
        RatioFeature2();

        /**
         * constructor setting the features: f1/f2
         * @param f1 feature 1
         * @param f2 feature 2
         */
        RatioFeature2(Feature2<INPUT, OUTPUT>* f1, Feature2<INPUT, OUTPUT>* f2);

        /**
         * copy constructor
         * @param df
         */
        RatioFeature2(const RatioFeature2<INPUT, OUTPUT>& df);

        /**
         * destructor
         */
        ~RatioFeature2();

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position in row-major representation
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        /**
         * applies the ratio feature to the input image, in the foreground (non-zero) region of the mask
         * @param input input image
         * @param output output image
         * @param mask the operation is performed in the foreground region of the mask
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * updates the stride
         * @param stride new stride
         * @return non-zero if the stride is changed, zero otherwise
         */
        virtual int updateStride(int stride);

        /**
         * computes the minimum and maximum positions, in which the feature can be computed
         */
        virtual void computeMinMax();

        /**
         * returns the minimum position of row-major representation, in which the feature can be computed
         * @return the minimum position
         */
        virtual int getMin();

        /**
         * returns the maximum position of row-major representation, in which the feature can be computed
         * @return the maximum position
         */
        virtual int getMax();

        /**
         * using the proposed border, the feature can be computed in each position of the original image
         * @return the proposed border
         */
        virtual Border2 getProposedBorder();

        /**
         * the minimum position of row-major representation, in which the feature can be computed
         */
        int min;

        /**
         * the maximum position of row-major representation, in which the feature can be computed
         * @return
         */
        int max;

        /** feature 1 */
        Feature2<INPUT, OUTPUT>* f1;
        /** feature 2 */
        Feature2<INPUT, OUTPUT>* f2;
    };

    template<typename INPUT, typename OUTPUT>
    Border2 RatioFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        Border2 b1= f1->getProposedBorder();
        Border2 b2= f2->getProposedBorder();

        return b1.topBorder > b2.topBorder ? b1.topBorder : b2.topBorder;
    }

    template<typename INPUT, typename OUTPUT>
    int RatioFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        int tmp= this->f1->updateStride(stride);
        tmp|= this->f2->updateStride(stride);
        if ( tmp )
        {
            this->f1->computeMinMax();
            this->f2->computeMinMax();
        }

        return tmp;
    }

    template<typename INPUT, typename OUTPUT>
    int RatioFeature2<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int RatioFeature2<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    void RatioFeature2<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= f1->getMin() > f2->getMin() ? f1->getMin() : f2->getMin();
        this->max= f1->getMax() < f2->getMax() ? f1->getMax() : f2->getMax();
    }

    /**
     * prints the description of the parameter RatioFeature2 object to the parameter output stream
     * @param o parameter output stream
     * @param v RatioFeature2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT>
    std::ostream& operator<<(std::ostream& o, const RatioFeature2<INPUT, OUTPUT>& v)
    {
        o << "RatioFeature2 " << *(v.f1) << " - " << *(v.f2) << "";
        return o;
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT RatioFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float ff1= f1->apply(input, n, support);
        float ff2= f2->apply(input, n, support);
        if ( fabs(ff2) > 0 )
            return ff1 / ff2;
        else
            return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void RatioFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( updateStride(input.columns) )
            computeMinMax();

        if ( roi == NULL )
        {
            for ( int i= -this->getMin(); i < (int)(input.n) - this->getMax(); ++i )
            {
                output(i)= this->apply(input, i, support);
            }
        }
        else
        {
            for ( int i= -this->getMin(); i < (int)(input.n) - this->getMax(); ++i )
            {
                if ( (*roi)(i) > 0 )
                    output(i)= this->apply(input, i, support);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    RatioFeature2<INPUT, OUTPUT>::RatioFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioFeature2<INPUT, OUTPUT>::RatioFeature2(Feature2<INPUT, OUTPUT>* f1, Feature2<INPUT, OUTPUT>* f2)
    {
        this->f1= new Feature2<INPUT, OUTPUT>(*f1);
        this->f2= new Feature2<INPUT, OUTPUT>(*f2);
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    RatioFeature2<INPUT, OUTPUT>::RatioFeature2(const RatioFeature2<INPUT, OUTPUT>& df)
    {
        this->f1= new Feature2<INPUT, OUTPUT>(*(df.f1));
        this->f2= new Feature2<INPUT, OUTPUT>(*(df.f2));
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    RatioFeature2<INPUT, OUTPUT>::~RatioFeature2()
    {
        delete this->f1;
        delete this->f2;
    }

    /**
     * Ratio of SNR features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfSNRFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply on
         */
        RatioOfSNRFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        RatioOfSNRFeature2(const RatioOfSNRFeature2& d);

        /**
         * destructor
         */
        ~RatioOfSNRFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfSNRFeature2<INPUT, OUTPUT>::RatioOfSNRFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new SNRFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new SNRFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfSNRFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfSNRFeature2<INPUT, OUTPUT>::RatioOfSNRFeature2(const RatioOfSNRFeature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfSNRFeature2<INPUT, OUTPUT>::~RatioOfSNRFeature2()
    {
    }

    /**
     * Ratio of d/(dydy)Gaussian features with different variance
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfGaussianDyyFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma1 variance of feature 1
         * @param sigma2 variance of feature 2
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param delta delta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param stride stride of the images to apply on
         */
        RatioOfGaussianDyyFeature2(int kernel, float sigma1, float sigma2, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        RatioOfGaussianDyyFeature2(const RatioOfGaussianDyyFeature2& dgf);

        /**
         * destructor
         */
        ~RatioOfGaussianDyyFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDyyFeature2<INPUT, OUTPUT>::RatioOfGaussianDyyFeature2(int kernel, float sigma1, float sigma2, float alpha, float beta, float delta, float gamma, int stride)
    : RatioFeature2<INPUT, OUTPUT>()
    {
        this->f1= new GaussianDyyFilter2<INPUT, OUTPUT>(sigma1, kernel, alpha, beta, delta, gamma, stride);
        this->f2= new GaussianDyyFilter2<INPUT, OUTPUT>(sigma2, kernel, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfGaussianDyyFeature2 " << kernel << " " << sigma1 << " " << sigma2 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDyyFeature2<INPUT, OUTPUT>::RatioOfGaussianDyyFeature2(const RatioOfGaussianDyyFeature2& dgf)
    : RatioFeature2<INPUT, OUTPUT>(dgf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDyyFeature2<INPUT, OUTPUT>::~RatioOfGaussianDyyFeature2()
    {
    }

    /**
     * Ratio of d/(dxdy)Gaussian features with different variance
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfGaussianDxyFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma1 variance of feature 1
         * @param sigma2 variance of feature 2
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param delta delta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param stride stride of the images to apply on
         */
        RatioOfGaussianDxyFeature2(int kernel, float sigma1, float sigma2, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        RatioOfGaussianDxyFeature2(const RatioOfGaussianDxyFeature2& dgf);

        /**
         * destructor
         */
        ~RatioOfGaussianDxyFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDxyFeature2<INPUT, OUTPUT>::RatioOfGaussianDxyFeature2(int kernel, float sigma1, float sigma2, float alpha, float beta, float delta, float gamma, int stride)
    : RatioFeature2<INPUT, OUTPUT>()
    {
        this->f1= new GaussianDxyFilter2<INPUT, OUTPUT>(sigma1, kernel, alpha, beta, delta, gamma, stride);
        this->f2= new GaussianDxyFilter2<INPUT, OUTPUT>(sigma2, kernel, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfGaussianDxyFeature2 " << kernel << " " << sigma1 << " " << sigma2 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDxyFeature2<INPUT, OUTPUT>::RatioOfGaussianDxyFeature2(const RatioOfGaussianDxyFeature2& dgf)
    : RatioFeature2<INPUT, OUTPUT>(dgf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDxyFeature2<INPUT, OUTPUT>::~RatioOfGaussianDxyFeature2()
    {
    }

    /**
     * Ratio of d/(dxdx)Gaussian features with different variance
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfGaussianDxxFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma1 variance of feature 1
         * @param sigma2 variance of feature 2
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param delta delta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param stride stride of the images to apply on
         */
        RatioOfGaussianDxxFeature2(int kernel, float sigma1, float sigma2, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        RatioOfGaussianDxxFeature2(const RatioOfGaussianDxxFeature2& dgf);

        /**
         * destructor
         */
        ~RatioOfGaussianDxxFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDxxFeature2<INPUT, OUTPUT>::RatioOfGaussianDxxFeature2(int kernel, float sigma1, float sigma2, float alpha, float beta, float delta, float gamma, int stride)
    : RatioFeature2<INPUT, OUTPUT>()
    {
        this->f1= new GaussianDxxFilter2<INPUT, OUTPUT>(sigma1, kernel, alpha, beta, delta, gamma, stride);
        this->f2= new GaussianDxxFilter2<INPUT, OUTPUT>(sigma2, kernel, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfGaussianDxxFeature2 " << kernel << " " << sigma1 << " " << sigma2 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDxxFeature2<INPUT, OUTPUT>::RatioOfGaussianDxxFeature2(const RatioOfGaussianDxxFeature2& dgf)
    : RatioFeature2<INPUT, OUTPUT>(dgf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDxxFeature2<INPUT, OUTPUT>::~RatioOfGaussianDxxFeature2()
    {
    }

    /**
     * Ratio of d/(dy)Gaussian features with different variance
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfGaussianDyFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma1 variance of feature 1
         * @param sigma2 variance of feature 2
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param delta delta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param stride stride of the images to apply on
         */
        RatioOfGaussianDyFeature2(int kernel, float sigma1, float sigma2, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        RatioOfGaussianDyFeature2(const RatioOfGaussianDyFeature2& dgf);

        /**
         * destructor
         */
        ~RatioOfGaussianDyFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDyFeature2<INPUT, OUTPUT>::RatioOfGaussianDyFeature2(int kernel, float sigma1, float sigma2, float alpha, float beta, float delta, float gamma, int stride)
    : RatioFeature2<INPUT, OUTPUT>()
    {
        this->f1= new GaussianDyFilter2<INPUT, OUTPUT>(sigma1, kernel, alpha, beta, delta, gamma, stride);
        this->f2= new GaussianDyFilter2<INPUT, OUTPUT>(sigma2, kernel, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfGaussianDyFeature2 " << kernel << " " << sigma1 << " " << sigma2 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDyFeature2<INPUT, OUTPUT>::RatioOfGaussianDyFeature2(const RatioOfGaussianDyFeature2& dgf)
    : RatioFeature2<INPUT, OUTPUT>(dgf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDyFeature2<INPUT, OUTPUT>::~RatioOfGaussianDyFeature2()
    {
    }

    /**
     * Ratio of d/(dx)Gaussian features with different variance
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfGaussianDxFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma1 variance of feature 1
         * @param sigma2 variance of feature 2
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param delta delta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param stride stride of the images to apply on
         */
        RatioOfGaussianDxFeature2(int kernel, float sigma1, float sigma2, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        RatioOfGaussianDxFeature2(const RatioOfGaussianDxFeature2& dgf);

        /**
         * destructor
         */
        ~RatioOfGaussianDxFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDxFeature2<INPUT, OUTPUT>::RatioOfGaussianDxFeature2(int kernel, float sigma1, float sigma2, float alpha, float beta, float delta, float gamma, int stride)
    : RatioFeature2<INPUT, OUTPUT>()
    {
        this->f1= new GaussianDxFilter2<INPUT, OUTPUT>(sigma1, kernel, alpha, beta, delta, gamma, stride);
        this->f2= new GaussianDxFilter2<INPUT, OUTPUT>(sigma2, kernel, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfGaussianDxFeature2 " << kernel << " " << sigma1 << " " << sigma2 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDxFeature2<INPUT, OUTPUT>::RatioOfGaussianDxFeature2(const RatioOfGaussianDxFeature2& dgf)
    : RatioFeature2<INPUT, OUTPUT>(dgf)
    {
        this->f1= dgf.f1;
        this->f2= dgf.f2;
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianDxFeature2<INPUT, OUTPUT>::~RatioOfGaussianDxFeature2()
    {
    }

    /**
     * Ratio of Gaussian features with different variance
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfGaussianFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma1 variance of feature 1
         * @param sigma2 variance of feature 2
         * @param alpha alpha Gaussian parameter
         * @param beta beta Gaussian parameter
         * @param delta delta Gaussian parameter
         * @param gamma gamma Gaussian parameter
         * @param stride stride of the images to apply on
         */
        RatioOfGaussianFeature2(int kernel, float sigma1, float sigma2, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        RatioOfGaussianFeature2(const RatioOfGaussianFeature2& dgf);

        /**
         * destructor
         */
        ~RatioOfGaussianFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianFeature2<INPUT, OUTPUT>::RatioOfGaussianFeature2(int kernel, float sigma1, float sigma2, float alpha, float beta, float delta, float gamma, int stride)
    : RatioFeature2<INPUT, OUTPUT>()
    {
        this->f1= new GaussianFilter2<INPUT, OUTPUT>(sigma1, kernel, alpha, beta, delta, gamma, stride);
        this->f2= new GaussianFilter2<INPUT, OUTPUT>(sigma2, kernel, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfGaussianFeature2 " << kernel << " " << sigma1 << " " << sigma2 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianFeature2<INPUT, OUTPUT>::RatioOfGaussianFeature2(const RatioOfGaussianFeature2& dgf)
    : RatioFeature2<INPUT, OUTPUT>(dgf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfGaussianFeature2<INPUT, OUTPUT>::~RatioOfGaussianFeature2()
    {
    }

    /**
     * Ratio of Min features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfMinFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfMinFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        RatioOfMinFeature2(const RatioOfMinFeature2& dgf);

        /**
         * destructor
         */
        ~RatioOfMinFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfMinFeature2<INPUT, OUTPUT>::RatioOfMinFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new MinFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new MinFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfMinFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfMinFeature2<INPUT, OUTPUT>::RatioOfMinFeature2(const RatioOfMinFeature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfMinFeature2<INPUT, OUTPUT>::~RatioOfMinFeature2()
    {
    }

    /**
     * Ratio of Max features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfMaxFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfMaxFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        RatioOfMaxFeature2(const RatioOfMaxFeature2& dgf);

        /**
         * destructor
         */
        ~RatioOfMaxFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfMaxFeature2<INPUT, OUTPUT>::RatioOfMaxFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new MaxFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new MaxFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfMaxFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfMaxFeature2<INPUT, OUTPUT>::RatioOfMaxFeature2(const RatioOfMaxFeature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfMaxFeature2<INPUT, OUTPUT>::~RatioOfMaxFeature2()
    {
    }

    /**
     * Ratio of Max features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfMeanFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfMeanFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        RatioOfMeanFeature2(const RatioOfMeanFeature2& dgf);

        /**
         * destructor
         */
        ~RatioOfMeanFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfMeanFeature2<INPUT, OUTPUT>::RatioOfMeanFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new MeanFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new MeanFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfMeanFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfMeanFeature2<INPUT, OUTPUT>::RatioOfMeanFeature2(const RatioOfMeanFeature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfMeanFeature2<INPUT, OUTPUT>::~RatioOfMeanFeature2()
    {
    }

    /**
     * Ratio of Variance features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfVarianceFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfVarianceFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        RatioOfVarianceFeature2(const RatioOfVarianceFeature2& dgf);

        /**
         * destructor
         */
        ~RatioOfVarianceFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfVarianceFeature2<INPUT, OUTPUT>::RatioOfVarianceFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new VarianceFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new VarianceFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfVarianceFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfVarianceFeature2<INPUT, OUTPUT>::RatioOfVarianceFeature2(const RatioOfVarianceFeature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfVarianceFeature2<INPUT, OUTPUT>::~RatioOfVarianceFeature2()
    {
    }

    /**
     * Ratio of StandardDeviation features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfStandardDeviationFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfStandardDeviationFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        RatioOfStandardDeviationFeature2(const RatioOfStandardDeviationFeature2& dgf);

        /**
         * destructor
         */
        ~RatioOfStandardDeviationFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfStandardDeviationFeature2<INPUT, OUTPUT>::RatioOfStandardDeviationFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new StandardDeviationFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new StandardDeviationFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfStandardDeviationFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfStandardDeviationFeature2<INPUT, OUTPUT>::RatioOfStandardDeviationFeature2(const RatioOfStandardDeviationFeature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfStandardDeviationFeature2<INPUT, OUTPUT>::~RatioOfStandardDeviationFeature2()
    {
    }

    /**
     * Ratio of CentralMoment features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfCentralMomentFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param p1 exponent of x components in feature 1
         * @param q1 exponent of y components in feature 1
         * @param radius2 radius of feature 2
         * @param p2 exponent of x components in feature 2
         * @param q2 exponent of y components in feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfCentralMomentFeature2( int radius1, float p1, float q1, int radius2, float p2, float q2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        RatioOfCentralMomentFeature2(const RatioOfCentralMomentFeature2& d);

        /**
         * destructor
         */
        ~RatioOfCentralMomentFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfCentralMomentFeature2<INPUT, OUTPUT>::RatioOfCentralMomentFeature2(int radius1, float p1, float q1, int radius2, float p2, float q2, int stride)
    {
        this->f1= new CentralMomentFeature2<INPUT, OUTPUT>(radius1, stride, p1, q1);
        this->f2= new CentralMomentFeature2<INPUT, OUTPUT>(radius2, stride, p2, q2);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfCentralMomentFeature2 " << radius1 << " " << p1 << " " << q1 << " " << radius2 << " " << p2 << " " << q2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfCentralMomentFeature2<INPUT, OUTPUT>::RatioOfCentralMomentFeature2(const RatioOfCentralMomentFeature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfCentralMomentFeature2<INPUT, OUTPUT>::~RatioOfCentralMomentFeature2()
    {
    }

    /**
     * Ratio of invariant Hu's moment 1 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfInvariantHuMoment1Feature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfInvariantHuMoment1Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        RatioOfInvariantHuMoment1Feature2(const RatioOfInvariantHuMoment1Feature2& d);

        /**
         * destructor
         */
        ~RatioOfInvariantHuMoment1Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment1Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment1Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment1Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment1Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfInvariantHuMoment1Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment1Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment1Feature2(const RatioOfInvariantHuMoment1Feature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment1Feature2<INPUT, OUTPUT>::~RatioOfInvariantHuMoment1Feature2()
    {
    }

    /**
     * Ratio of invariant Hu's moment 2 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfInvariantHuMoment2Feature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfInvariantHuMoment2Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d isntance to copy
         */
        RatioOfInvariantHuMoment2Feature2(const RatioOfInvariantHuMoment2Feature2& d);

        /**
         * destructor
         */
        ~RatioOfInvariantHuMoment2Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment2Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment2Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment2Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment2Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfInvariantHuMoment2Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment2Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment2Feature2(const RatioOfInvariantHuMoment2Feature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment2Feature2<INPUT, OUTPUT>::~RatioOfInvariantHuMoment2Feature2()
    {
    }

    /**
     * Ratio of invariant Hu's moment 3 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfInvariantHuMoment3Feature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfInvariantHuMoment3Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        RatioOfInvariantHuMoment3Feature2(const RatioOfInvariantHuMoment3Feature2& d);

        /**
         * destructor
         */
        ~RatioOfInvariantHuMoment3Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment3Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment3Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment3Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment3Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfInvariantHuMoment3Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment3Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment3Feature2(const RatioOfInvariantHuMoment3Feature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment3Feature2<INPUT, OUTPUT>::~RatioOfInvariantHuMoment3Feature2()
    {
    }

    /**
     * Ratio of invariant Hu's moment 4 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfInvariantHuMoment4Feature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfInvariantHuMoment4Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        RatioOfInvariantHuMoment4Feature2(const RatioOfInvariantHuMoment4Feature2& d);

        /**
         * destructor
         */
        ~RatioOfInvariantHuMoment4Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment4Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment4Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment4Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment4Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfInvariantHuMoment4Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment4Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment4Feature2(const RatioOfInvariantHuMoment4Feature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment4Feature2<INPUT, OUTPUT>::~RatioOfInvariantHuMoment4Feature2()
    {
    }

    /**
     * Ratio of invariant Hu's moment 5 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfInvariantHuMoment5Feature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfInvariantHuMoment5Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        RatioOfInvariantHuMoment5Feature2(const RatioOfInvariantHuMoment5Feature2& d);

        /**
         * destructor
         */
        ~RatioOfInvariantHuMoment5Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment5Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment5Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment5Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment5Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfInvariantHuMoment5Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment5Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment5Feature2(const RatioOfInvariantHuMoment5Feature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment5Feature2<INPUT, OUTPUT>::~RatioOfInvariantHuMoment5Feature2()
    {
    }

    /**
     * Ratio of invariant Hu's moment 6 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfInvariantHuMoment6Feature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfInvariantHuMoment6Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        RatioOfInvariantHuMoment6Feature2(const RatioOfInvariantHuMoment6Feature2& d);

        /**
         * destructor
         */
        ~RatioOfInvariantHuMoment6Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment6Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment6Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment6Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment6Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfInvariantHuMoment6Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment6Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment6Feature2(const RatioOfInvariantHuMoment6Feature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment6Feature2<INPUT, OUTPUT>::~RatioOfInvariantHuMoment6Feature2()
    {
    }

    /**
     * Ratio of invariant Hu's moment 7 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfInvariantHuMoment7Feature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfInvariantHuMoment7Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        RatioOfInvariantHuMoment7Feature2(const RatioOfInvariantHuMoment7Feature2& d);

        /**
         * destructor
         */
        ~RatioOfInvariantHuMoment7Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment7Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment7Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment7Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment7Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfInvariantHuMoment7Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment7Feature2<INPUT, OUTPUT>::RatioOfInvariantHuMoment7Feature2(const RatioOfInvariantHuMoment7Feature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfInvariantHuMoment7Feature2<INPUT, OUTPUT>::~RatioOfInvariantHuMoment7Feature2()
    {
    }

    /**
     * Ratio of eccentricity features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfEccentricityFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfEccentricityFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        RatioOfEccentricityFeature2(const RatioOfEccentricityFeature2& d);

        /**
         * destructor
         */
        ~RatioOfEccentricityFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfEccentricityFeature2<INPUT, OUTPUT>::RatioOfEccentricityFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new EccentricityFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new EccentricityFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfEccentricityFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfEccentricityFeature2<INPUT, OUTPUT>::RatioOfEccentricityFeature2(const RatioOfEccentricityFeature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfEccentricityFeature2<INPUT, OUTPUT>::~RatioOfEccentricityFeature2()
    {
    }

    /**
     * Ratio of orientation features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class RatioOfOrientationFeature2: public RatioFeature2<INPUT, OUTPUT>
    {
    public:
        using RatioFeature2<INPUT, OUTPUT>::f1;
        using RatioFeature2<INPUT, OUTPUT>::f2;
        using RatioFeature2<INPUT, OUTPUT>::updateStride;
        using RatioFeature2<INPUT, OUTPUT>::computeMinMax;
        using RatioFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature 1
         * @param radius2 radius of feature 2
         * @param stride stride of the images to apply to
         */
        RatioOfOrientationFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        RatioOfOrientationFeature2(const RatioOfOrientationFeature2& d);

        /**
         * destructor
         */
        ~RatioOfOrientationFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    RatioOfOrientationFeature2<INPUT, OUTPUT>::RatioOfOrientationFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new OrientationFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new OrientationFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "RatioOfOrientationFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfOrientationFeature2<INPUT, OUTPUT>::RatioOfOrientationFeature2(const RatioOfOrientationFeature2& d)
    : RatioFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RatioOfOrientationFeature2<INPUT, OUTPUT>::~RatioOfOrientationFeature2()
    {
    }
}

#endif	/* _RATIOFEATURE2S_H */


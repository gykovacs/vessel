/**
 * @file DifferenceFeature2s.h
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
 * This file contains some difference features.
 */

#ifndef _DIFFERENCEFILTER2S_H
#define	_DIFFERENCEFILTER2S_H

#include <openipDS/Filter2s.h>
#include <openipDS/StatisticalFeature2s.h>
#include <openipDS/Feature2.h>
#include <openipDS/Image.h>
#include <openipDS/GaborFilter2.h>
#include <openipDS/MatchedFilter2s.h>
#include <openipDS/MatchedGaborFilter2.h>

#include <sstream>

namespace openip
{
    /**
     * DifferenceFeature2 is the base class of all difference features
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::descriptor;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * default constructor
         */
        DifferenceFeature2();

        /**
         * constructor, to set the features whose difference will be computed at
         * a given pixel
         * @param f1 feature 1
         * @param f2 feature 2
         */
        DifferenceFeature2(Feature2<INPUT, OUTPUT>* f1, Feature2<INPUT, OUTPUT>* f2);

        /**
         * copy constructor
         * @param df feature to copy
         */
        DifferenceFeature2(const DifferenceFeature2<INPUT, OUTPUT>& df);

        /**
         * destructor
         */
        ~DifferenceFeature2();

        /**
         * computes the difference of features f1 and f2 in the image input in the position n
         * @param input input image
         * @param n position in row-major representation
         * @return the difference of features f1 and f2
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support);

        /**
         * computes the difference of features f1 and f2 in the image input where the mask contains
         * non-zero elements, if no mask is set, the features are computed in the whole image
         * @param input input image
         * @param output output image containing the result
         * @param mask the operation is performed in the foreground (non-zero) region
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * updates the stride in features f1 and f2
         * @param stride the new stride
         * @return non-zero if the stride had to be modified, zero if no changes were performed
         */
        virtual int updateStride(int stride);

        /**
         * computes the min and max positions in row-major representetation where the feature can be computed
         */
        virtual void computeMinMax();

        /**
         * returns the minimum position in row-major representation where the feature can be computed
         * @return the minimum position
         */
        virtual int getMin();

        /**
         * returns the maximum position in row-major representation where the feature can be computed
         * @return the maximum position
         */
        virtual int getMax();

        /**
         * returns the proposed border, having this border, the feature can be computed in each position of the original image
         * @return
         */
        virtual Border2 getProposedBorder();

        /**
         * contains the minimum position in row-major representation where the feature can be computed
         */
        int min;

        /**
         * contains the maximum position in row-major representation where the feature can be computed
         */
        int max;

        /**
         * f1-f2 is computed
         */
        Feature2<INPUT, OUTPUT>* f1;

        /**
         * f1-f2 is computed
         */
        Feature2<INPUT, OUTPUT>* f2;
    };

    /**
     * prints the description of the parameter DifferenceFeature2 object to the parameter output stream
     * @param o parameter output stream
     * @param v DifferenceFeature2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT>
    std::ostream& operator<<(std::ostream& o, const DifferenceFeature2<INPUT, OUTPUT>& v);


    template<typename INPUT, typename OUTPUT>
    int DifferenceFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        int tmp= this->f1->updateStride(stride);
        tmp|= this->f2->updateStride(stride);
        if ( tmp )
        {
            this->f1->computeMinMax();
            this->f2->computeMinMax();
        }
        this->computeMinMax();

        return tmp;
    }

    template<typename INPUT, typename OUTPUT>
    void DifferenceFeature2<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= f1->getMin() < f2->getMin() ? f1->getMin() : f2->getMin();
        this->max= f1->getMax() > f2->getMax() ? f1->getMax() : f2->getMax();
    }

    template<typename INPUT, typename OUTPUT>
    int DifferenceFeature2<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int DifferenceFeature2<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 DifferenceFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        Border2 b1= f1->getProposedBorder();
        Border2 b2= f2->getProposedBorder();

        return b1.topBorder > b2.topBorder ? b1.topBorder : b2.topBorder;
    }

    template<typename INPUT, typename OUTPUT>
    std::ostream& operator<<(std::ostream& o, const DifferenceFeature2<INPUT, OUTPUT>& v)
    {
        o << v.descriptor;
        return o;
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT DifferenceFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float ff1= f1->apply(input, n, support);
        float ff2= f2->apply(input, n, support);
        return ff1 - ff2;
    }

    template<typename INPUT, typename OUTPUT>
    void DifferenceFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        if ( updateStride(input.columns) )
            computeMinMax();

        if ( roi == NULL )
        {
            #pragma omp parallel for
            for ( int i= -this->getMin(); i < (int)(input.n) - this->getMax(); ++i )
            {
                output(i)= this->apply(input, i, support);
            }
        }
        else
        {
            #pragma omp parallel for
            for ( int i= -this->getMin(); i < (int)(input.n) - this->getMax(); ++i )
            {
                if ( (*roi)(i) > 0 )
                    output(i)= this->apply(input, i, support);
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFeature2<INPUT, OUTPUT>::DifferenceFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFeature2<INPUT, OUTPUT>::DifferenceFeature2(Feature2<INPUT, OUTPUT>* f1, Feature2<INPUT, OUTPUT>* f2)
    {
        this->f1= new Feature2<INPUT, OUTPUT>(*f1);
        this->f2= new Feature2<INPUT, OUTPUT>(*f2);
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFeature2<INPUT, OUTPUT>::DifferenceFeature2(const DifferenceFeature2<INPUT, OUTPUT>& df)
    {
        this->f1= new Feature2<INPUT, OUTPUT>(*(df.f1));
        this->f2= new Feature2<INPUT, OUTPUT>(*(df.f2));
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceFeature2<INPUT, OUTPUT>::~DifferenceFeature2()
    {
        delete this->f1;
        delete this->f2;
    }

    /**
     * Difference of d/(dydy)Gaussian features with different variance
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfGaussianDyyFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma1 variance of f1
         * @param sigma2 variance of f2
         * @param alpha Gaussian transformation parameter
         * @param beta Gaussian transformation parameter
         * @param delta Gaussian transformation parameter
         * @param gamma Gaussian transformation parameter
         * @param stride stride of the images to apply on
         */
        DifferenceOfGaussianDyyFeature2(int kernel, float sigma1, float sigma2, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DifferenceOfGaussianDyyFeature2(const DifferenceOfGaussianDyyFeature2& dgf);

        /**
         * destructor
         */
        ~DifferenceOfGaussianDyyFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDyyFeature2<INPUT, OUTPUT>::DifferenceOfGaussianDyyFeature2(int kernel, float sigma1, float sigma2, float alpha, float beta, float delta, float gamma, int stride)
    : DifferenceFeature2<INPUT, OUTPUT>()
    {
        this->f1= new GaussianDyyFilter2<INPUT, OUTPUT>(sigma1, kernel, alpha, beta, delta, gamma, stride);
        this->f2= new GaussianDyyFilter2<INPUT, OUTPUT>(sigma2, kernel, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfGaussianDyyFeature2 " << stride << " " << kernel << " " << sigma1 << " " << sigma2 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDyyFeature2<INPUT, OUTPUT>::DifferenceOfGaussianDyyFeature2(const DifferenceOfGaussianDyyFeature2& dgf)
    : DifferenceFeature2<INPUT, OUTPUT>(dgf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDyyFeature2<INPUT, OUTPUT>::~DifferenceOfGaussianDyyFeature2()
    {
    }

    /**
     * Difference of d/(dxdy)Gaussian features with different variance
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfGaussianDxyFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma1 variance of f1
         * @param sigma2 variance of f2
         * @param alpha Gaussian transformation parameter
         * @param beta Gaussian transformation parameter
         * @param delta Gaussian transformation parameter
         * @param gamma Gaussian transformation parameter
         * @param stride stride of the images to apply on
         */
        DifferenceOfGaussianDxyFeature2(int kernel, float sigma1, float sigma2, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DifferenceOfGaussianDxyFeature2(const DifferenceOfGaussianDxyFeature2& dgf);

        /**
         * destructor
         */
        ~DifferenceOfGaussianDxyFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDxyFeature2<INPUT, OUTPUT>::DifferenceOfGaussianDxyFeature2(int kernel, float sigma1, float sigma2, float alpha, float beta, float delta, float gamma, int stride)
    : DifferenceFeature2<INPUT, OUTPUT>()
    {
        this->f1= new GaussianDxyFilter2<INPUT, OUTPUT>(sigma1, kernel, alpha, beta, delta, gamma, stride);
        this->f2= new GaussianDxyFilter2<INPUT, OUTPUT>(sigma2, kernel, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfGaussianDxyFeature2 " << kernel << " " << sigma1 << " " << sigma2 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDxyFeature2<INPUT, OUTPUT>::DifferenceOfGaussianDxyFeature2(const DifferenceOfGaussianDxyFeature2& dgf)
    : DifferenceFeature2<INPUT, OUTPUT>(dgf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDxyFeature2<INPUT, OUTPUT>::~DifferenceOfGaussianDxyFeature2()
    {
    }

    /**
     * Difference of d/(dxdx)Gaussian features with different variance
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfGaussianDxxFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma1 variance of f1
         * @param sigma2 variance of f2
         * @param alpha Gaussian transformation parameter
         * @param beta Gaussian transformation parameter
         * @param delta Gaussian transformation parameter
         * @param gamma Gaussian transformation parameter
         * @param stride stride of the images to apply on
         */
        DifferenceOfGaussianDxxFeature2(int kernel, float sigma1, float sigma2, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DifferenceOfGaussianDxxFeature2(const DifferenceOfGaussianDxxFeature2& dgf);

        /**
         * destructor
         */
        ~DifferenceOfGaussianDxxFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDxxFeature2<INPUT, OUTPUT>::DifferenceOfGaussianDxxFeature2(int kernel, float sigma1, float sigma2, float alpha, float beta, float delta, float gamma, int stride)
    : DifferenceFeature2<INPUT, OUTPUT>()
    {
        this->f1= new GaussianDxxFilter2<INPUT, OUTPUT>(sigma1, kernel, alpha, beta, delta, gamma, stride);
        this->f2= new GaussianDxxFilter2<INPUT, OUTPUT>(sigma2, kernel, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfGaussianDxxFeature2 " << kernel << " " << sigma1 << " " << sigma2 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDxxFeature2<INPUT, OUTPUT>::DifferenceOfGaussianDxxFeature2(const DifferenceOfGaussianDxxFeature2& dgf)
    : DifferenceFeature2<INPUT, OUTPUT>(dgf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDxxFeature2<INPUT, OUTPUT>::~DifferenceOfGaussianDxxFeature2()
    {
    }

    /**
     * Difference of d/(dy)Gaussian features with different variance
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfGaussianDyFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma1 variance of f1
         * @param sigma2 variance of f2
         * @param alpha Gaussian transformation parameter
         * @param beta Gaussian transformation parameter
         * @param delta Gaussian transformation parameter
         * @param gamma Gaussian transformation parameter
         * @param stride stride of the images to apply on
         */
        DifferenceOfGaussianDyFeature2(int kernel, float sigma1, float sigma2, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DifferenceOfGaussianDyFeature2(const DifferenceOfGaussianDyFeature2& dgf);

        /**
         * destructor
         */
        ~DifferenceOfGaussianDyFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDyFeature2<INPUT, OUTPUT>::DifferenceOfGaussianDyFeature2(int kernel, float sigma1, float sigma2, float alpha, float beta, float delta, float gamma, int stride)
    : DifferenceFeature2<INPUT, OUTPUT>()
    {
        this->f1= new GaussianDyFilter2<INPUT, OUTPUT>(sigma1, kernel, alpha, beta, delta, gamma, stride);
        this->f2= new GaussianDyFilter2<INPUT, OUTPUT>(sigma2, kernel, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfGaussianDyFeature2 " << kernel << " " << sigma1 << " " << sigma2 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDyFeature2<INPUT, OUTPUT>::DifferenceOfGaussianDyFeature2(const DifferenceOfGaussianDyFeature2& dgf)
    : DifferenceFeature2<INPUT, OUTPUT>(dgf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDyFeature2<INPUT, OUTPUT>::~DifferenceOfGaussianDyFeature2()
    {
    }

    /**
     * Difference of d/(dx)Gaussian features with different variance
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfGaussianDxFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma1 variance of f1
         * @param sigma2 variance of f2
         * @param alpha Gaussian transformation parameter
         * @param beta Gaussian transformation parameter
         * @param delta Gaussian transformation parameter
         * @param gamma Gaussian transformation parameter
         * @param stride stride of the images to apply on
         */
        DifferenceOfGaussianDxFeature2(int kernel, float sigma1, float sigma2, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DifferenceOfGaussianDxFeature2(const DifferenceOfGaussianDxFeature2& dgf);

        /**
         * destructor
         */
        ~DifferenceOfGaussianDxFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDxFeature2<INPUT, OUTPUT>::DifferenceOfGaussianDxFeature2(int kernel, float sigma1, float sigma2, float alpha, float beta, float delta, float gamma, int stride)
    : DifferenceFeature2<INPUT, OUTPUT>()
    {
        this->f1= new GaussianDxFilter2<INPUT, OUTPUT>(sigma1, kernel, alpha, beta, delta, gamma, stride);
        this->f2= new GaussianDxFilter2<INPUT, OUTPUT>(sigma2, kernel, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfGaussianDxFeature2 " << kernel << " " << sigma1 << " " << sigma2 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDxFeature2<INPUT, OUTPUT>::DifferenceOfGaussianDxFeature2(const DifferenceOfGaussianDxFeature2& dgf)
    : DifferenceFeature2<INPUT, OUTPUT>(dgf)
    {
        this->f1= dgf.f1;
        this->f2= dgf.f2;
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianDxFeature2<INPUT, OUTPUT>::~DifferenceOfGaussianDxFeature2()
    {
    }

    /**
     * Difference of Gaussian features with different variance
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfGaussianFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma1 variance of f1
         * @param sigma2 variance of f2
         * @param alpha Gaussian transformation parameter
         * @param beta Gaussian transformation parameter
         * @param delta Gaussian transformation parameter
         * @param gamma Gaussian transformation parameter
         * @param stride stride of the images to apply on
         */
        DifferenceOfGaussianFeature2(int kernel, float sigma1, float sigma2, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DifferenceOfGaussianFeature2(const DifferenceOfGaussianFeature2& dgf);

        /**
         * destructor
         */
        ~DifferenceOfGaussianFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianFeature2<INPUT, OUTPUT>::DifferenceOfGaussianFeature2(int kernel, float sigma1, float sigma2, float alpha, float beta, float delta, float gamma, int stride)
    : DifferenceFeature2<INPUT, OUTPUT>()
    {
        this->f1= new GaussianFilter2<INPUT, OUTPUT>(sigma1, kernel, alpha, beta, delta, gamma, stride);
        this->f2= new GaussianFilter2<INPUT, OUTPUT>(sigma2, kernel, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfGaussianFeature2 " << kernel << " " << sigma1 << " " << sigma2 << " " << alpha << " " << beta << " " << delta << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianFeature2<INPUT, OUTPUT>::DifferenceOfGaussianFeature2(const DifferenceOfGaussianFeature2& dgf)
    : DifferenceFeature2<INPUT, OUTPUT>(dgf)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfGaussianFeature2<INPUT, OUTPUT>::~DifferenceOfGaussianFeature2()
    {
    }

    /**
     * Difference of Min features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfMinFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfMinFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DifferenceOfMinFeature2(const DifferenceOfMinFeature2& dgf);

        /**
         * destructor
         */
        ~DifferenceOfMinFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfMinFeature2<INPUT, OUTPUT>::DifferenceOfMinFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new MinFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new MinFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfMinFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfMinFeature2<INPUT, OUTPUT>::DifferenceOfMinFeature2(const DifferenceOfMinFeature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfMinFeature2<INPUT, OUTPUT>::~DifferenceOfMinFeature2()
    {
    }

    /**
     * Difference of Max features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfMaxFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfMaxFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DifferenceOfMaxFeature2(const DifferenceOfMaxFeature2& dgf);

        /**
         * destructor
         */
        ~DifferenceOfMaxFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfMaxFeature2<INPUT, OUTPUT>::DifferenceOfMaxFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new MaxFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new MaxFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfMaxFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfMaxFeature2<INPUT, OUTPUT>::DifferenceOfMaxFeature2(const DifferenceOfMaxFeature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfMaxFeature2<INPUT, OUTPUT>::~DifferenceOfMaxFeature2()
    {
    }

    /**
     * Difference of Mean features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfMeanFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfMeanFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DifferenceOfMeanFeature2(const DifferenceOfMeanFeature2& dgf);

        /**
         * destructor
         */
        ~DifferenceOfMeanFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfMeanFeature2<INPUT, OUTPUT>::DifferenceOfMeanFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new MeanFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new MeanFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfMeanFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfMeanFeature2<INPUT, OUTPUT>::DifferenceOfMeanFeature2(const DifferenceOfMeanFeature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfMeanFeature2<INPUT, OUTPUT>::~DifferenceOfMeanFeature2()
    {
    }

    /**
     * Difference of Variance features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfVarianceFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfVarianceFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DifferenceOfVarianceFeature2(const DifferenceOfVarianceFeature2& dgf);

        /**
         * destructor
         */
        ~DifferenceOfVarianceFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfVarianceFeature2<INPUT, OUTPUT>::DifferenceOfVarianceFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new VarianceFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new VarianceFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfVarianceFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfVarianceFeature2<INPUT, OUTPUT>::DifferenceOfVarianceFeature2(const DifferenceOfVarianceFeature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfVarianceFeature2<INPUT, OUTPUT>::~DifferenceOfVarianceFeature2()
    {
    }

    /**
     * Difference of StandardDeviation features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfStandardDeviationFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfStandardDeviationFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DifferenceOfStandardDeviationFeature2(const DifferenceOfStandardDeviationFeature2& dgf);

        /**
         * destructor
         */
        ~DifferenceOfStandardDeviationFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfStandardDeviationFeature2<INPUT, OUTPUT>::DifferenceOfStandardDeviationFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new StandardDeviationFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new StandardDeviationFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfStandardDeviationFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfStandardDeviationFeature2<INPUT, OUTPUT>::DifferenceOfStandardDeviationFeature2(const DifferenceOfStandardDeviationFeature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfStandardDeviationFeature2<INPUT, OUTPUT>::~DifferenceOfStandardDeviationFeature2()
    {
    }

    /**
     * Difference of CentralMoment features with different parameters
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfCentralMomentFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param p1 exponent of x components in feature1
         * @param q1 exponent of y components in feature1
         * @param radius2 radius of feature2
         * @param p2 exponent of x components in feature2
         * @param q2 exponent of y components in feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfCentralMomentFeature2(int radius1, float p1, float q1, int radius2, float p2, float q2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceOfCentralMomentFeature2(const DifferenceOfCentralMomentFeature2& d);

        /**
         * destructor
         */
        ~DifferenceOfCentralMomentFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfCentralMomentFeature2<INPUT, OUTPUT>::DifferenceOfCentralMomentFeature2(int radius1, float p1, float q1, int radius2, float p2, float q2, int stride)
    {
        this->f1= new CentralMomentFeature2<INPUT, OUTPUT>(radius1, p1, q1, stride);
        this->f2= new CentralMomentFeature2<INPUT, OUTPUT>(radius2, p2, q2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfCentralMomentFeature2 " << radius1 << " " << p1 << " " << q1 << " " << radius2 << " " << p2 << " " << q2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfCentralMomentFeature2<INPUT, OUTPUT>::DifferenceOfCentralMomentFeature2(const DifferenceOfCentralMomentFeature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfCentralMomentFeature2<INPUT, OUTPUT>::~DifferenceOfCentralMomentFeature2()
    {
    }

    /**
     * Difference of Invariant Hu Moment 1 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfInvariantHuMoment1Feature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfInvariantHuMoment1Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceOfInvariantHuMoment1Feature2(const DifferenceOfInvariantHuMoment1Feature2& d);

        /**
         * destructor
         */
        ~DifferenceOfInvariantHuMoment1Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment1Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment1Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment1Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment1Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfInvariantHuMoment1Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment1Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment1Feature2(const DifferenceOfInvariantHuMoment1Feature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment1Feature2<INPUT, OUTPUT>::~DifferenceOfInvariantHuMoment1Feature2()
    {
    }

    /**
     * Difference of Invariant Hu Moment 2 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfInvariantHuMoment2Feature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfInvariantHuMoment2Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceOfInvariantHuMoment2Feature2(const DifferenceOfInvariantHuMoment2Feature2& d);

        /**
         * destructor
         */
        ~DifferenceOfInvariantHuMoment2Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment2Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment2Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment2Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment2Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfInvariantHuMoment2Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment2Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment2Feature2(const DifferenceOfInvariantHuMoment2Feature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment2Feature2<INPUT, OUTPUT>::~DifferenceOfInvariantHuMoment2Feature2()
    {
    }

    /**
     * Difference of Invariant Hu Moment 3 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfInvariantHuMoment3Feature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfInvariantHuMoment3Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceOfInvariantHuMoment3Feature2(const DifferenceOfInvariantHuMoment3Feature2& d);

        /**
         * destructor
         */
        ~DifferenceOfInvariantHuMoment3Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment3Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment3Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment3Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment3Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfInvariantHuMoment3Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment3Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment3Feature2(const DifferenceOfInvariantHuMoment3Feature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment3Feature2<INPUT, OUTPUT>::~DifferenceOfInvariantHuMoment3Feature2()
    {
    }

    /**
     * Difference of Invariant Hu Moment 4 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfInvariantHuMoment4Feature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfInvariantHuMoment4Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceOfInvariantHuMoment4Feature2(const DifferenceOfInvariantHuMoment4Feature2& d);

        /**
         * destructor
         */
        ~DifferenceOfInvariantHuMoment4Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment4Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment4Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment4Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment4Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfInvariantHuMoment4Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment4Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment4Feature2(const DifferenceOfInvariantHuMoment4Feature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment4Feature2<INPUT, OUTPUT>::~DifferenceOfInvariantHuMoment4Feature2()
    {
    }

    /**
     * Difference of Invariant Hu Moment 5 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfInvariantHuMoment5Feature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfInvariantHuMoment5Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceOfInvariantHuMoment5Feature2(const DifferenceOfInvariantHuMoment5Feature2& d);

        /**
         * destructor
         */
        ~DifferenceOfInvariantHuMoment5Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment5Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment5Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment5Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment5Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfInvariantHuMoment5Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment5Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment5Feature2(const DifferenceOfInvariantHuMoment5Feature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment5Feature2<INPUT, OUTPUT>::~DifferenceOfInvariantHuMoment5Feature2()
    {
    }

    /**
     * Difference of Invariant Hu Moment 6 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfInvariantHuMoment6Feature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfInvariantHuMoment6Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceOfInvariantHuMoment6Feature2(const DifferenceOfInvariantHuMoment6Feature2& d);

        /**
         * destructor
         */
        ~DifferenceOfInvariantHuMoment6Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment6Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment6Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment6Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment6Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfInvariantHuMoment6Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment6Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment6Feature2(const DifferenceOfInvariantHuMoment6Feature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment6Feature2<INPUT, OUTPUT>::~DifferenceOfInvariantHuMoment6Feature2()
    {
    }

    /**
     * Difference of Invariant Hu Moment 7 features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfInvariantHuMoment7Feature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfInvariantHuMoment7Feature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceOfInvariantHuMoment7Feature2(const DifferenceOfInvariantHuMoment7Feature2& d);

        /**
         * destructor
         */
        ~DifferenceOfInvariantHuMoment7Feature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment7Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment7Feature2(int radius1, int radius2, int stride)
    {
        this->f1= new InvariantHuMoment7Feature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new InvariantHuMoment7Feature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfInvariantHuMoment7Feature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment7Feature2<INPUT, OUTPUT>::DifferenceOfInvariantHuMoment7Feature2(const DifferenceOfInvariantHuMoment7Feature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfInvariantHuMoment7Feature2<INPUT, OUTPUT>::~DifferenceOfInvariantHuMoment7Feature2()
    {
    }

    /**
     * Difference of Eccentricity features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfEccentricityFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfEccentricityFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceOfEccentricityFeature2(const DifferenceOfEccentricityFeature2& d);

        /**
         * destructor
         */
        ~DifferenceOfEccentricityFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfEccentricityFeature2<INPUT, OUTPUT>::DifferenceOfEccentricityFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new EccentricityFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new EccentricityFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfEccentricityFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfEccentricityFeature2<INPUT, OUTPUT>::DifferenceOfEccentricityFeature2(const DifferenceOfEccentricityFeature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfEccentricityFeature2<INPUT, OUTPUT>::~DifferenceOfEccentricityFeature2()
    {
    }

    /**
     * Difference of Orientation features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfOrientationFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfOrientationFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceOfOrientationFeature2(const DifferenceOfOrientationFeature2& d);

        /**
         * destructor
         */
        ~DifferenceOfOrientationFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfOrientationFeature2<INPUT, OUTPUT>::DifferenceOfOrientationFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new OrientationFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new OrientationFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfOrientationFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfOrientationFeature2<INPUT, OUTPUT>::DifferenceOfOrientationFeature2(const DifferenceOfOrientationFeature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfOrientationFeature2<INPUT, OUTPUT>::~DifferenceOfOrientationFeature2()
    {
    }

    /**
     * Difference of SNR features with different radius
     */
    template<typename INPUT, typename OUTPUT>
    class DifferenceOfSNRFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceOfSNRFeature2(int radius1, int radius2, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceOfSNRFeature2(const DifferenceOfSNRFeature2& d);

        /**
         * destructor
         */
        ~DifferenceOfSNRFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceOfSNRFeature2<INPUT, OUTPUT>::DifferenceOfSNRFeature2(int radius1, int radius2, int stride)
    {
        this->f1= new SNRFeature2<INPUT, OUTPUT>(radius1, stride);
        this->f2= new SNRFeature2<INPUT, OUTPUT>(radius2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceOfSNRFeature2 " << radius1 << " " << radius2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfSNRFeature2<INPUT, OUTPUT>::DifferenceOfSNRFeature2(const DifferenceOfSNRFeature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceOfSNRFeature2<INPUT, OUTPUT>::~DifferenceOfSNRFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class DifferenceMatchedCorrelationLineSegmentFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceMatchedCorrelationLineSegmentFeature2(int rows, int columns, float sigma1, float sigma2, float angle0, float angleStep, float angle1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceMatchedCorrelationLineSegmentFeature2(const DifferenceMatchedCorrelationLineSegmentFeature2& d);

        /**
         * destructor
         */
        ~DifferenceMatchedCorrelationLineSegmentFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceMatchedCorrelationLineSegmentFeature2<INPUT, OUTPUT>::DifferenceMatchedCorrelationLineSegmentFeature2(int rows, int columns, float sigma1, float sigma2, float angle0, float angleStep, float angle1, float alpha, float beta, float delta, float gamma, int stride)
    {
        this->f1= new MatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>(rows, columns, sigma1, angle0, angleStep, angle1, alpha, beta, delta, gamma, stride);
        this->f2= new MatchedCorrelationLineSegmentFilter2<INPUT, OUTPUT>(rows, columns, sigma2, angle0, angleStep, angle1, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceMatchedCorrelationLineSegmentFeature2 " << rows << " " << columns << " " << sigma1 << " " << sigma2 << " " << angle0 << " " << angleStep << " " << angle1 << " " << alpha << " " << beta << " " << delta << " " << gamma << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceMatchedCorrelationLineSegmentFeature2<INPUT, OUTPUT>::DifferenceMatchedCorrelationLineSegmentFeature2(const DifferenceMatchedCorrelationLineSegmentFeature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceMatchedCorrelationLineSegmentFeature2<INPUT, OUTPUT>::~DifferenceMatchedCorrelationLineSegmentFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class DifferenceMatchedLineSegmentFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceMatchedLineSegmentFeature2(int rows, int columns, float sigma1, float sigma2, float angle0, float angleStep, float angle1, float alpha= 1, float beta= 1, float delta= 1, float gamma= 0, int stride= 4000);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceMatchedLineSegmentFeature2(const DifferenceMatchedLineSegmentFeature2& d);

        /**
         * destructor
         */
        ~DifferenceMatchedLineSegmentFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceMatchedLineSegmentFeature2<INPUT, OUTPUT>::DifferenceMatchedLineSegmentFeature2(int rows, int columns, float sigma1, float sigma2, float angle0, float angleStep, float angle1, float alpha, float beta, float delta, float gamma, int stride)
    {
        this->f1= new MatchedLineSegmentFilter2<INPUT, OUTPUT>(rows, columns, sigma1, angle0, angleStep, angle1, alpha, beta, delta, gamma, stride);
        this->f2= new MatchedLineSegmentFilter2<INPUT, OUTPUT>(rows, columns, sigma2, angle0, angleStep, angle1, alpha, beta, delta, gamma, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceMatchedLineSegmentFeature2 " << rows << " " << columns << " " << sigma1 << " " << sigma2 << " " << angle0 << " " << angleStep << " " << angle1 << " " << alpha << " " << beta << " " << delta << " " << gamma << " " << stride;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceMatchedLineSegmentFeature2<INPUT, OUTPUT>::DifferenceMatchedLineSegmentFeature2(const DifferenceMatchedLineSegmentFeature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceMatchedLineSegmentFeature2<INPUT, OUTPUT>::~DifferenceMatchedLineSegmentFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    class DifferenceMatchedGaborRFeature2: public DifferenceFeature2<INPUT, OUTPUT>
    {
    public:
        using DifferenceFeature2<INPUT, OUTPUT>::f1;
        using DifferenceFeature2<INPUT, OUTPUT>::f2;
        using DifferenceFeature2<INPUT, OUTPUT>::updateStride;
        using DifferenceFeature2<INPUT, OUTPUT>::computeMinMax;
        using DifferenceFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param radius1 radius of feature1
         * @param radius2 radius of feature2
         * @param stride stride of the images to apply on
         */
        DifferenceMatchedGaborRFeature2(float sigma1, float sigma2, float theta0, float step, float theta1, float lambda, float psi, float gamma);

        /**
         * copy constructor
         * @param d instance to copy
         */
        DifferenceMatchedGaborRFeature2(const DifferenceMatchedGaborRFeature2& d);

        /**
         * destructor
         */
        ~DifferenceMatchedGaborRFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DifferenceMatchedGaborRFeature2<INPUT, OUTPUT>::DifferenceMatchedGaborRFeature2(float sigma1, float sigma2, float theta0, float step, float theta1, float lambda, float psi, float gamma)
    {
        this->f1= new MatchedGaborFilterR2<INPUT, OUTPUT>(sigma1, theta0, step, theta1, lambda, psi, gamma);
        this->f2= new MatchedGaborFilterR2<INPUT, OUTPUT>(sigma2, theta0, step, theta1, lambda, psi, gamma);
        computeMinMax();
        std::stringstream ss;
        ss << "DifferenceMatchedGaborRFeature2 " << sigma1 << " " << sigma2 << " " << theta0 << " " << step << " " << theta1 << " " << lambda << " " << psi << " " << gamma;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceMatchedGaborRFeature2<INPUT, OUTPUT>::DifferenceMatchedGaborRFeature2(const DifferenceMatchedGaborRFeature2& d)
    : DifferenceFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DifferenceMatchedGaborRFeature2<INPUT, OUTPUT>::~DifferenceMatchedGaborRFeature2()
    {
    }


}


#endif	/* _DIFFERENCEFILTER2S_H */


/**
 * @file DoubleRingFeature2s.h
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
 * This file contains some double ring features.
 */

#ifndef DOUBLERingFeature2S_H
#define	DOUBLERingFeature2S_H

#include <openipDS/Filter2s.h>
#include <openipDS/StatisticalFeature2s.h>
#include <openipDS/Feature2.h>
#include <openipDS/Image.h>
#include <openipDS/RingFeature2s.h>

#include <sstream>

namespace openip
{
    /**
     * DoubleRingFeature2 is the base class of all double ring features
     */
    template<typename INPUT, typename OUTPUT>
    class DoubleRingFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using Feature2<INPUT, OUTPUT>::descriptor;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * default constructor
         */
        DoubleRingFeature2();

        /**
         * constructor to set the features computed in the rings
         * @param f1 feature1
         * @param f2 feature2
         */
        DoubleRingFeature2(Feature2<INPUT, OUTPUT>* f1, Feature2<INPUT, OUTPUT>* f2);

        /**
         * copy constructor
         * @param df instance to copy
         */
        DoubleRingFeature2(const DoubleRingFeature2<INPUT, OUTPUT>& df);

        /**
         * destructor
         */
        ~DoubleRingFeature2();

        /**
         * computes the double ring feature in image input, in position n, using the non-zero regions of roi
         * @param input input image
         * @param n position in row-major representation
         * @param roi the operation uses the foreground (non-zero) region of the roi
         * @return the computed feature
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        /**
         * computes the double ring feature in image input, in all non-zero positions of mask, using the non-zero regions of roi
         * @param input input image
         * @param output output image
         * @param mask the feature is computed in the non-zero regions of the mask
         * @param roi the operation uses the non-zero regions of roi
         */
        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        /**
         * updates the stride
         * @param stride stride of the images to apply on
         * @return non-zero is the stride is changed, 0 otherwise
         */
        virtual int updateStride(int stride);

        /**
         * computes the minimum and maximum positions in row-major representation, in which the
         * feature can be computed
         */
        virtual void computeMinMax();

        /**
         * returns the minimum position in row-major representation in which the feature can be computed
         * @return the minimum position
         */
        virtual int getMin();

        /**
         * returns the maximum position in row-major representation in which the feature can be computed
         * @return the maximum position
         */
        virtual int getMax();

        /**
         * returns the proposed border
         * @return using the returned border, the feature can be computed in each position of the original image
         */
        virtual Border2 getProposedBorder();

        /**
         * contains the minimum position
         */
        int min;

        /**
         * contains the maximum position
         */
        int max;

        /**
         * feature1
         */
        Feature2<INPUT, OUTPUT>* f1;

        /**
         * feature2
         */
        Feature2<INPUT, OUTPUT>* f2;
    };

    /**
     * prints the description of the parameter DoubleRingFeature2 object to the parameter output stream
     * @param o parameter output stream
     * @param v DoubleRingFeature2 object to describe
     * @return reference to the parameter output stream
     */
    template<typename INPUT, typename OUTPUT>
    std::ostream& operator<<(std::ostream& o, const DoubleRingFeature2<INPUT, OUTPUT>& v);

    template<typename INPUT, typename OUTPUT>
    int DoubleRingFeature2<INPUT, OUTPUT>::updateStride(int stride)
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
    void DoubleRingFeature2<INPUT, OUTPUT>::computeMinMax()
    {
        this->min= f1->getMin() < f2->getMin() ? f1->getMin() : f2->getMin();
        this->max= f1->getMax() > f2->getMax() ? f1->getMax() : f2->getMax();
    }

    template<typename INPUT, typename OUTPUT>
    int DoubleRingFeature2<INPUT, OUTPUT>::getMin()
    {
        return this->min;
    }

    template<typename INPUT, typename OUTPUT>
    int DoubleRingFeature2<INPUT, OUTPUT>::getMax()
    {
        return this->max;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 DoubleRingFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        Border2 b1= f1->getProposedBorder();
        Border2 b2= f2->getProposedBorder();

        return b1.topBorder > b2.topBorder ? b1.topBorder : b2.topBorder;
    }

    template<typename INPUT, typename OUTPUT>
    std::ostream& operator<<(std::ostream& o, const DoubleRingFeature2<INPUT, OUTPUT>& v)
    {
        o << v.descriptor;
        return o;
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT DoubleRingFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        float ff1= f1->apply(input, n, support);
        float ff2= f2->apply(input, n, support);
        return ff1 - ff2;
    }

    template<typename INPUT, typename OUTPUT>
    void DoubleRingFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
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
    DoubleRingFeature2<INPUT, OUTPUT>::DoubleRingFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    DoubleRingFeature2<INPUT, OUTPUT>::DoubleRingFeature2(Feature2<INPUT, OUTPUT>* f1, Feature2<INPUT, OUTPUT>* f2)
    {
        this->f1= f1;
        this->f2= f2;
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    DoubleRingFeature2<INPUT, OUTPUT>::DoubleRingFeature2(const DoubleRingFeature2<INPUT, OUTPUT>& df)
    {
        this->f1= new Feature2<INPUT, OUTPUT>(*(df.f1));
        this->f2= new Feature2<INPUT, OUTPUT>(*(df.f2));
        computeMinMax();
    }

    template<typename INPUT, typename OUTPUT>
    DoubleRingFeature2<INPUT, OUTPUT>::~DoubleRingFeature2()
    {
        delete this->f1;
        delete this->f2;
    }

    /**
     * double ring feature of Mean features
     */
    template<typename INPUT, typename OUTPUT>
    class DoubleRingMeanFeature2: public DoubleRingFeature2<INPUT, OUTPUT>
    {
    public:
        using DoubleRingFeature2<INPUT, OUTPUT>::f1;
        using DoubleRingFeature2<INPUT, OUTPUT>::f2;
        using DoubleRingFeature2<INPUT, OUTPUT>::updateStride;
        using DoubleRingFeature2<INPUT, OUTPUT>::computeMinMax;
        using DoubleRingFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param i1 inner radius of feature1
         * @param o1 outer radius of feature1
         * @param i2 inner radius of feature2
         * @param o2 outer radius of feature2
         * @param stride stride of the images to apply on
         */
        DoubleRingMeanFeature2(int i1, int o1, int i2, int o2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DoubleRingMeanFeature2(const DoubleRingMeanFeature2& dgf);

        /**
         * destructor
         */
        ~DoubleRingMeanFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DoubleRingMeanFeature2<INPUT, OUTPUT>::DoubleRingMeanFeature2(int i1, int o1, int i2, int o2, int stride)
    {
        this->f1= new RingMeanFeature2<INPUT, OUTPUT>(i1, o1, stride);
        this->f2= new RingMeanFeature2<INPUT, OUTPUT>(i2, o2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DoubleRingMeanFeature2 " << i1 << " " << o1 << " " << i2 << " " << o2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DoubleRingMeanFeature2<INPUT, OUTPUT>::DoubleRingMeanFeature2(const DoubleRingMeanFeature2& d)
    : DoubleRingFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DoubleRingMeanFeature2<INPUT, OUTPUT>::~DoubleRingMeanFeature2()
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    class DoubleRingMeanRatioFeature2: public DoubleRingFeature2<INPUT, OUTPUT>
    {
    public:
        using DoubleRingFeature2<INPUT, OUTPUT>::f1;
        using DoubleRingFeature2<INPUT, OUTPUT>::f2;
        using DoubleRingFeature2<INPUT, OUTPUT>::updateStride;
        using DoubleRingFeature2<INPUT, OUTPUT>::computeMinMax;
        using DoubleRingFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param i1 inner radius of feature1
         * @param o1 outer radius of feature1
         * @param i2 inner radius of feature2
         * @param o2 outer radius of feature2
         * @param stride stride of the images to apply on
         */
        DoubleRingMeanRatioFeature2(int i1, int o1, int i2, int o2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DoubleRingMeanRatioFeature2(const DoubleRingMeanRatioFeature2& dgf);

        /**
         * destructor
         */
        ~DoubleRingMeanRatioFeature2();
        
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    DoubleRingMeanRatioFeature2<INPUT, OUTPUT>::DoubleRingMeanRatioFeature2(int i1, int o1, int i2, int o2, int stride)
    {
        this->f1= new RingMeanFeature2<INPUT, OUTPUT>(i1, o1, stride);
        this->f2= new RingMeanFeature2<INPUT, OUTPUT>(i2, o2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DoubleRingMeanRatioFeature2 " << i1 << " " << o1 << " " << i2 << " " << o2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DoubleRingMeanRatioFeature2<INPUT, OUTPUT>::DoubleRingMeanRatioFeature2(const DoubleRingMeanRatioFeature2& d)
    : DoubleRingFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DoubleRingMeanRatioFeature2<INPUT, OUTPUT>::~DoubleRingMeanRatioFeature2()
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    OUTPUT DoubleRingMeanRatioFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        OUTPUT tmpa= f1->apply(input, n, support);
        OUTPUT tmpb= f2->apply(input, n, support);
        return (tmpa - tmpb)/tmpa;
    }

    /**
     * double ring feature of Max features
     */
    template<typename INPUT, typename OUTPUT>
    class DoubleRingMaxFeature2: public DoubleRingFeature2<INPUT, OUTPUT>
    {
    public:
        using DoubleRingFeature2<INPUT, OUTPUT>::f1;
        using DoubleRingFeature2<INPUT, OUTPUT>::f2;
        using DoubleRingFeature2<INPUT, OUTPUT>::updateStride;
        using DoubleRingFeature2<INPUT, OUTPUT>::computeMinMax;
        using DoubleRingFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param i1 inner radius of feature1
         * @param o1 outer radius of feature1
         * @param i2 inner radius of feature2
         * @param o2 outer radius of feature2
         * @param stride stride of the images to apply on
         */
        DoubleRingMaxFeature2(int i1, int o1, int i2, int o2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DoubleRingMaxFeature2(const DoubleRingMaxFeature2& dgf);

        /**
         * destructor
         */
        ~DoubleRingMaxFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DoubleRingMaxFeature2<INPUT, OUTPUT>::DoubleRingMaxFeature2(int i1, int o1, int i2, int o2, int stride)
    {
        this->f1= new RingMaxFeature2<INPUT, OUTPUT>(i1, o1, stride);
        this->f2= new RingMaxFeature2<INPUT, OUTPUT>(i2, o2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DoubleRingMaxFeature2 " << i1 << " " << o1 << " " << i2 << " " << o2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DoubleRingMaxFeature2<INPUT, OUTPUT>::DoubleRingMaxFeature2(const DoubleRingMaxFeature2& d)
    : DoubleRingFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DoubleRingMaxFeature2<INPUT, OUTPUT>::~DoubleRingMaxFeature2()
    {
    }

    /**
     * double ring feature of Min features
     */
    template<typename INPUT, typename OUTPUT>
    class DoubleRingMinFeature2: public DoubleRingFeature2<INPUT, OUTPUT>
    {
    public:
        using DoubleRingFeature2<INPUT, OUTPUT>::f1;
        using DoubleRingFeature2<INPUT, OUTPUT>::f2;
        using DoubleRingFeature2<INPUT, OUTPUT>::updateStride;
        using DoubleRingFeature2<INPUT, OUTPUT>::computeMinMax;
        using DoubleRingFeature2<INPUT, OUTPUT>::descriptor;

        /**
         * constructor
         * @param i1 inner radius of feature1
         * @param o1 outer radius of feature1
         * @param i2 inner radius of feature2
         * @param o2 outer radius of feature2
         * @param stride stride of the images to apply on
         */
        DoubleRingMinFeature2(int i1, int o1, int i2, int o2, int stride= 4000);

        /**
         * copy constructor
         * @param dgf instance to copy
         */
        DoubleRingMinFeature2(const DoubleRingMinFeature2& dgf);

        /**
         * destructor
         */
        ~DoubleRingMinFeature2();
    };

    template<typename INPUT, typename OUTPUT>
    DoubleRingMinFeature2<INPUT, OUTPUT>::DoubleRingMinFeature2(int i1, int o1, int i2, int o2, int stride)
    {
        this->f1= new RingMinFeature2<INPUT, OUTPUT>(i1, o1, stride);
        this->f2= new RingMinFeature2<INPUT, OUTPUT>(i2, o2, stride);
        computeMinMax();
        std::stringstream ss;
        ss << "DoubleRingMinFeature2 " << i1 << " " << o1 << " " << i2 << " " << o2;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    DoubleRingMinFeature2<INPUT, OUTPUT>::DoubleRingMinFeature2(const DoubleRingMinFeature2& d)
    : DoubleRingFeature2<INPUT, OUTPUT>(d)
    {
    }

    template<typename INPUT, typename OUTPUT>
    DoubleRingMinFeature2<INPUT, OUTPUT>::~DoubleRingMinFeature2()
    {
    }
}

#endif	/* DOUBLERingFeature2S_H */


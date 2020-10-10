/**
 * @file RingFeature2s.h
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
 * Ring features are statistical descriptors computed in rings around a pixel.
 */

#ifndef RingFeature2S_H
#define	RingFeature2S_H

#include <openipDS/Filter2.h>
#include <openipDS/FilterSet2.h>
#include <openipDS/StructuringElement2s.h>

#include <sstream>
#include <cmath>

namespace openip
{
    /**
     * RingFeature2 is the base class for all ring features.
     */
    template<typename INPUT, typename OUTPUT>
    class RingFeature2: public Feature2<INPUT, OUTPUT>, public StructuringElementRing
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using StructuringElementRing::stride;
        using StructuringElementRing::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using StructuringElementRing::getMin;
        using StructuringElementRing::getMax;
        using StructuringElementRing::getMean;
        using StructuringElementRing::getMedian;
        using StructuringElementRing::getVariance;
        using StructuringElementRing::getMeanAndVariance;
        using StructuringElementRing::getStandardDeviation;
        using StructuringElementRing::computeMinMax;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param inner radius of the inner border of the ring
         * @param outer radius of the outer border of the ring
         * @param stride stride of the images to apply to
         */
        RingFeature2(int inner, int outer, int stride= 4000);

        /**
         * copy constructor
         * @param s instance to copy
         */
        RingFeature2(const RingFeature2& s);

        /**
         * updates the stride
         * @param stride new stride
         * @return non-zero if the stride is changed, zero otherwise
         */
        virtual int updateStride(int stride);

        /**
         * returns the minimum relative position in the feature object
         * @return the minimum relative position
         */
        virtual int getMin();

        /**
         * returns the maximum relative position in the feature object
         * @return the maximum relative position
         */
        virtual int getMax();

        /**
         * using the proposed border, the feature can be computed in each pixel
         * of the original image
         * @return the proposed border
         */
        virtual Border2 getProposedBorder();
    };

    template<typename INPUT, typename OUTPUT>
    int RingFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        return this->StructuringElementRing::updateStride(stride);
    }

    template<typename INPUT, typename OUTPUT>
    Border2 RingFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        computeMinMax();
        int w= getMax() / stride + 1;
        return Border2(w, w, w, w);
    }

    template<typename INPUT, typename OUTPUT>
    RingFeature2<INPUT, OUTPUT>::RingFeature2(int inner, int outer, int stride)
    : StructuringElementRing(inner, outer, stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    RingFeature2<INPUT, OUTPUT>::RingFeature2(const RingFeature2& s)
    : StructuringElementRing(s), Feature2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int RingFeature2<INPUT, OUTPUT>::getMin()
    {
        return this->StructuringElementRing::getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int RingFeature2<INPUT, OUTPUT>::getMax()
    {
        return this->StructuringElementRing::getMax();
    }

    /**
     * Ring feature of mean descriptors.
     */
    template<typename INPUT, typename OUTPUT>
    class RingMeanFeature2: public RingFeature2<INPUT, OUTPUT>
    {
    public:
        using RingFeature2<INPUT, OUTPUT>::descriptor;
        using RingFeature2<INPUT, OUTPUT>::getMin;
        using RingFeature2<INPUT, OUTPUT>::getMax;
        using RingFeature2<INPUT, OUTPUT>::stride;
        using RingFeature2<INPUT, OUTPUT>::updateStride;
        using RingFeature2<INPUT, OUTPUT>::getMean;
        using RingFeature2<INPUT, OUTPUT>::apply;
        using RingFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param inner inner radius of the ring
         * @param outer outer radius of the ring
         * @param stride stride of the images to apply to
         */
        RingMeanFeature2(int inner, int outer, int stride= 4000);

        /**
         * applies the feature to position n of the input image
         * @param input input image
         * @param n position
         * @param roi the mean is computed only in the foreground (non-zero) region of the roi
         * @return
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        /** mean value */
        float mean;
        /** variance */
        float variance;
    };

    template<typename INPUT, typename OUTPUT>
    RingMeanFeature2<INPUT, OUTPUT>::RingMeanFeature2(int inner, int outer, int stride)
    : RingFeature2<INPUT, OUTPUT>(inner, outer, stride)
    {
        std::stringstream ss;
        ss << "RingMeanFeature2 " << inner << " " << outer;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT RingMeanFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return (OUTPUT)(this->getMean(input, n, support));
    }

    /**
     * Ring feature of max descriptors.
     */
    template<typename INPUT, typename OUTPUT>
    class RingMaxFeature2: public RingFeature2<INPUT, OUTPUT>
    {
    public:
        using RingFeature2<INPUT, OUTPUT>::descriptor;
        using RingFeature2<INPUT, OUTPUT>::getMin;
        using RingFeature2<INPUT, OUTPUT>::getMax;
        using RingFeature2<INPUT, OUTPUT>::stride;
        using RingFeature2<INPUT, OUTPUT>::updateStride;
        using RingFeature2<INPUT, OUTPUT>::getMeanAndVariance;
        using RingFeature2<INPUT, OUTPUT>::apply;
        using RingFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param inner inner radius of the ring
         * @param outer outer radius of the ring
         * @param stride stride of the images to apply to
         */
        RingMaxFeature2(int inner, int outer, int stride= 4000);

        /**
         * applies the feature to position n of the input image
         * @param input input image
         * @param n position
         * @param roi the max is computed only in the foreground (non-zero) region of the roi
         * @return
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    RingMaxFeature2<INPUT, OUTPUT>::RingMaxFeature2(int inner, int outer, int stride)
    : RingFeature2<INPUT, OUTPUT>(inner, outer, stride)
    {
        std::stringstream ss;
        ss << "RingMaxFeature2 " << inner << " " << outer;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT RingMaxFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return (OUTPUT)(this->getMax(input, n, support));
    }

    /**
     * Ring feature of min descriptors.
     */
    template<typename INPUT, typename OUTPUT>
    class RingMinFeature2: public RingFeature2<INPUT, OUTPUT>
    {
    public:
        using RingFeature2<INPUT, OUTPUT>::descriptor;
        using RingFeature2<INPUT, OUTPUT>::getMin;
        using RingFeature2<INPUT, OUTPUT>::getMax;
        using RingFeature2<INPUT, OUTPUT>::stride;
        using RingFeature2<INPUT, OUTPUT>::updateStride;
        using RingFeature2<INPUT, OUTPUT>::getMeanAndVariance;
        using RingFeature2<INPUT, OUTPUT>::apply;
        using RingFeature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param inner inner radius of the ring
         * @param outer outer radius of the ring
         * @param stride stride of the images to apply to
         */
        RingMinFeature2(int inner, int outer, int stride= 4000);

        /**
         * applies the feature to position n of the input image
         * @param input input image
         * @param n position
         * @param roi the min is computed only in the foreground (non-zero) region of the roi
         * @return
         */
        virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    RingMinFeature2<INPUT, OUTPUT>::RingMinFeature2(int inner, int outer, int stride)
    : RingFeature2<INPUT, OUTPUT>(inner, outer, stride)
    {
        std::stringstream ss;
        ss << "RingMinFeature2 " << inner << " " << outer;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT RingMinFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
        return (OUTPUT)(this->getMin(input, n, support));
    }
}

#endif	/* RingFeature2S_H */


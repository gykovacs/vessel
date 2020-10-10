#ifndef GRADIENTFEATURE2S_H
#define GRADIENTFEATURE2S_H

#include <openipDS/Feature2.h>
#include <openipDS/StructuringElement2s.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class GradientFeature2: public Feature2<INPUT, OUTPUT>, public StructuringElementDisk
    {
    public:
        using Feature2<INPUT, OUTPUT>::descriptor;
        using StructuringElementDisk::stride;
        using StructuringElementDisk::updateStride;
        using Feature2<INPUT, OUTPUT>::apply;
        using Feature2<INPUT, OUTPUT>::getMin;
        using Feature2<INPUT, OUTPUT>::getMax;
        using StructuringElementDisk::getMin;
        using StructuringElementDisk::getMax;
        using StructuringElementDisk::getMean;
        using StructuringElementDisk::getMedian;
        using StructuringElementDisk::getVariance;
        using StructuringElementDisk::getSkewness;
        using StructuringElementDisk::getKurtosis;
        using StructuringElementDisk::getMeanAndVariance;
        using StructuringElementDisk::getStandardDeviation;
        using StructuringElementDisk::computeMinMax;
        using Feature2<INPUT, OUTPUT>::getProposedBorder;

        /**
         * constructor
         * @param r radius of the descriptor
         * @param stride stride of the images to apply to
         */
        GradientFeature2(int r, int stride);

        /**
         * copy constructor
         * @param s instance to copy
         */
        GradientFeature2(const GradientFeature2& s);

        /**
         * updates the stride
         * @param stride the new stride
         * @return non-zero if the stride is changed, zero otherwise
         */
        virtual int updateStride(int stride);

        /**
         * returns the minimum relative position of row-major representation in the descriptor
         * @return the minimum position
         */
        virtual int getMin();

        /**
         * returns the maximum relative position of row-major representation in the descriptor
         * @return the maximum position
         */
        virtual int getMax();

        /**
         * using the proposed border, the feature can be computed in each position of the original image
         * @return the proposed border
         */
        virtual Border2 getProposedBorder();

        void getCoordinate2D(int n, int& rows, int& columns);
    };

    template<typename INPUT, typename OUTPUT>
    void GradientFeature2<INPUT, OUTPUT>::getCoordinate2D(int n, int& rows, int& columns)
    {
        rows= n / this->stride;
        if ( abs(n % this->stride) > this->stride/2 )
            rows= n > 0 ? rows + 1 : rows - 1;
        columns= n % this->stride;
        if ( abs(columns) > this->stride/2 )
            columns+= columns > 0 ? -this->stride : this->stride;
    }


    template<typename INPUT, typename OUTPUT>
    int GradientFeature2<INPUT, OUTPUT>::updateStride(int stride)
    {
        return this->StructuringElementDisk::updateStride(stride);
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GradientFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        computeMinMax();
        int w= getMax() / stride + 1;
        return Border2(w, w, w, w);
    }

    template<typename INPUT, typename OUTPUT>
    GradientFeature2<INPUT, OUTPUT>::GradientFeature2(int r, int stride)
    : StructuringElementDisk(r, stride)
    {
    }

    template<typename INPUT, typename OUTPUT>
    GradientFeature2<INPUT, OUTPUT>::GradientFeature2(const GradientFeature2& s)
    : StructuringElementDisk(s), Feature2<INPUT, OUTPUT>(s)
    {
    }

    template<typename INPUT, typename OUTPUT>
    int GradientFeature2<INPUT, OUTPUT>::getMin()
    {
        return this->StructuringElementDisk::getMin();
    }

    template<typename INPUT, typename OUTPUT>
    int GradientFeature2<INPUT, OUTPUT>::getMax()
    {
        return this->StructuringElementDisk::getMax();
    }
}

#endif // GRADIENTFEATURE2S_H

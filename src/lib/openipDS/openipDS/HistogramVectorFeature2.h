/* 
 * File:   HistogramVectorFeature2.h
 * Author: gykovacs
 *
 * Created on 2011. április 11., 9:41
 */

#ifndef HISTOGRAMVECTORFEATURE2_H
#define	HISTOGRAMVECTORFEATURE2_H

#include <openipDS/VectorTransform2.h>
#include <openipDS/Histogram.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class HistogramVectorFeature2: public VectorTransform2<INPUT, OUTPUT>, public StructuringElementDisk
    {
    public:
        using VectorTransform2<INPUT, OUTPUT>::descriptor;
        using StructuringElementDisk::getMinMax;
        using StructuringElementDisk::getMin;
        using StructuringElementDisk::getMax;
        /**
         * constructor
         * @param radius radius of the feature
         */
        HistogramVectorFeature2(int radius, int bins);

        /**
         * copy constructor
         * @param i instance to copy
         */
        HistogramVectorFeature2(const HistogramVectorFeature2& i);

        /**
         * destructor
         */
        ~HistogramVectorFeature2();

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n row-major position
         * @param output output vector
         */
        virtual void apply(Image<INPUT>& input, int n, Vector<OUTPUT>& output, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual Border2 getProposedBorder();

        virtual int dimensions();

        int bins;
    };

    template<typename INPUT, typename OUTPUT>
    HistogramVectorFeature2<INPUT, OUTPUT>::HistogramVectorFeature2(int radius, int bins)
    : VectorTransform2<INPUT, OUTPUT>(), StructuringElementDisk(radius)
    {
        std::stringstream ss;
        ss << "HistogramVectorFeature2 " << radius;
        this->descriptor= ss.str();
        this->bins= bins;
    }

    template<typename INPUT, typename OUTPUT>
    HistogramVectorFeature2<INPUT, OUTPUT>::HistogramVectorFeature2(const HistogramVectorFeature2& i)
    : VectorTransform2<INPUT, OUTPUT>(i), StructuringElementDisk(i)
    {
    }

    template<typename INPUT, typename OUTPUT>
    HistogramVectorFeature2<INPUT, OUTPUT>::~HistogramVectorFeature2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void HistogramVectorFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Vector<OUTPUT>& output, Image<unsigned char>* support)
    {
        this->updateStride(input.columns);

        INPUT min, max;
        float d;
        this->getMinMax(input, n, min, max, support);
        d= float(max - min)/bins + 0.1;

        output= 0;
        for ( StructuringElement2::iterator it= this->begin(); it != this->end(); ++it )
        {
            output(floor((input(n + *it) - min)/d))++;
        }
    }

    template<typename INPUT, typename OUTPUT>
    void HistogramVectorFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int n, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        this->updateStride(input.columns);
        Vector<OUTPUT> tmp(bins);

        //printf("%s %d %d %d %d\n", this->descriptor.c_str(), this->getMin(), input.n, this->getMax(), this->size()); fflush(stdout);

        int k;
        for ( int i= -int(this->getMin()); i < int(input.n) - int(this->getMax()); ++i )
        {
            if ( !roi || (*roi)(i) > 0 )
            {
                apply(input, i, tmp, support);
                for ( k= 0; k < bins; ++k )
                {
                    output(k+n)(i)= tmp(k);
                }
            }
        }
    }

    template<typename INPUT, typename OUTPUT>
    Border2 HistogramVectorFeature2<INPUT, OUTPUT>::getProposedBorder()
    {
        int w= this->getRows() / 2 + 1;
        return Border2(w, w, w, w);
    }

    template<typename INPUT, typename OUTPUT>
    int HistogramVectorFeature2<INPUT, OUTPUT>::dimensions()
    {
        return bins;
    }
}

#endif	/* HISTOGRAMVECTORFEATURE2_H */


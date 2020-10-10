/* 
 * File:   ImageVectorFeatures.h
 * Author: gykovacs
 *
 * Created on 2011. április 11., 13:10
 */

#ifndef IMAGEVECTORFEATURES_H
#define	IMAGEVECTORFEATURES_H

#include <openipDS/ImageVectorFeature.h>
#include <openipDS/Histogram.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class HistogramVectorFeature: public ImageVectorFeature<INPUT, OUTPUT>
    {
    public:
        using ImageVectorFeature<INPUT, OUTPUT>::descriptor;
        using ImageVectorFeature<INPUT, OUTPUT>::apply;

        HistogramVectorFeature(int bins);

        HistogramVectorFeature(const HistogramVectorFeature& h);

        ~HistogramVectorFeature();

        virtual void apply(Image<INPUT>& input, Vector<OUTPUT>& output, int begin= 0, Image<unsigned char>* roi= NULL);

        virtual int dimensions();

        virtual Border2 getProposedBorder();

        Histogram h;
        int bins;
    };

    template<typename INPUT, typename OUTPUT>
    HistogramVectorFeature<INPUT, OUTPUT>::HistogramVectorFeature(int bins)
    : ImageVectorFeature<INPUT, OUTPUT>()
    {
        this->bins= bins;
        stringstream ss;
        ss << "HistogramVectorFeature " << bins;
    }

    template<typename INPUT, typename OUTPUT>
    HistogramVectorFeature<INPUT, OUTPUT>::HistogramVectorFeature(const HistogramVectorFeature& h)
    : ImageVectorFeature<INPUT, OUTPUT>(h)
    {
        this->bins= h.bins;
    }

    template<typename INPUT, typename OUTPUT>
    HistogramVectorFeature<INPUT, OUTPUT>::~HistogramVectorFeature()
    {

    }

    template<typename INPUT, typename OUTPUT>
    void HistogramVectorFeature<INPUT, OUTPUT>::apply(Image<INPUT>& input, Vector<OUTPUT>& output, int begin, Image<unsigned char>* roi)
    {
        h.computeHistogram(&input, bins, input.getMin(roi), input.getMax(roi), roi);
        h.normalize();
        for ( int i= 0; i < h.size(); ++i )
            output(begin + i)= h(i);
    }

    template<typename INPUT, typename OUTPUT>
    int HistogramVectorFeature<INPUT, OUTPUT>::dimensions()
    {
        return bins;
    }

    template<typename INPUT, typename OUTPUT>
    Border2 HistogramVectorFeature<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(0,0,0,0);
    }

}

#endif	/* IMAGEVECTORFEATURES_H */


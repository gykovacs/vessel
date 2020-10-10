/* 
 * File:   volumeCorrection.h
 * Author: gykovacs
 *
 * Created on July 21, 2011, 7:21 AM
 */

#ifndef VOLUMECORRECTION_H
#define	VOLUMECORRECTION_H

#include <openipDS/Volume.h>
#include <openipDS/Histogram.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class HistogramEqualization3: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::apply;
        using Transform3<INPUT, OUTPUT>::descriptor;

        /**
         * default constructor
         */
        HistogramEqualization3();

        /**
         * copy constuctor
         * @param b instance to copy
         */
        HistogramEqualization3(const HistogramEqualization3& b);

        /**
         * destructor
         */
        ~HistogramEqualization3();

        /**
         * returns the proposed image border
         * @return the proposed image border object
         */
        virtual Border3 getProposedBorder();
        
        /**
         * apply function
         * @param input input image
         * @param output output image
         * @param roi the operation is performed only in the foreground (non 0) positions
         * @param support only the foreground (non 0) pixels are considered
         */
        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* mask= NULL, Volume<unsigned char>* support= NULL);
    };
    
    template<typename INPUT, typename OUTPUT>
    HistogramEqualization3<INPUT, OUTPUT>::HistogramEqualization3()
    : Transform3<INPUT, OUTPUT>()
    {
        std::stringstream ss;
        ss << "HistogramEqualization3";
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    HistogramEqualization3<INPUT, OUTPUT>::HistogramEqualization3(const HistogramEqualization3& b)
    : Transform3<INPUT, OUTPUT>(b)
    {
    }

    template<typename INPUT, typename OUTPUT>
    HistogramEqualization3<INPUT, OUTPUT>::~HistogramEqualization3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 HistogramEqualization3<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(0, 0, 0, 0, 0, 0, BORDER_MODE_ZERO);
    }

    template<typename INPUT, typename OUTPUT>
    void HistogramEqualization3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* mask, Volume<unsigned char>*)
    {
        Histogram h;

        INPUT min, max;
        input.getMinMax(min, max, mask);
        h.computeHistogram(&input, 10000, (INPUT)min, (INPUT)max, mask);

        h.accumulate();

        for ( unsigned int i= 0; i < input.n; ++i )
        {
            (output)(i)= (float)(h.get((input)(i)))/(float)(input.n) * 255;
        }
        //output.normalize(0, 255);
    }
}
#endif	/* VOLUMECORRECTION_H */


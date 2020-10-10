/**
 * @file attributeSimilarityThresholding.h
 * @author Janos Ivan Istvan <janos.istvan.ivan@gmail.com>
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
 * Attribute similarity based thresholding algorithms.
 */


#ifndef _ATTRIBUTESIMILARITYTHRESHOLDING_H_
#define _ATTRIBUTESIMILARITYTHRESHOLDING_H_

#include <openipDS/Image.h>
#include <openipDS/Histogram.h>
#include <openipDS/Filter2s.h>

#include <openipLL/Threshold2.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class TsaiThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        TsaiThresholding2(int resolution);

        TsaiThresholding2(TsaiThresholding2& o);

        ~TsaiThresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    TsaiThresholding2<INPUT, OUTPUT>::TsaiThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    TsaiThresholding2<INPUT, OUTPUT>::TsaiThresholding2(TsaiThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    TsaiThresholding2<INPUT, OUTPUT>::~TsaiThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void TsaiThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void TsaiThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float m1, m2, m3;
            m1= m2= m3= 0;

            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                m1+= h(i)*pow(h.indexToValue(i), 1);
                m2+= h(i)*pow(h.indexToValue(i), 2);
                m3+= h(i)*pow(h.indexToValue(i), 3);
            }

            float b1, b2, b3;
            float minValue= FLT_MAX;
            float value;
            int minIdx= 0;
            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                b1= b2= b3= 0;
                b1= h.pIndex(i)*h.mbIndex(i) + (1.0 - h.pIndex(i))*h.mfIndex(i);
                b2= h.pIndex(i)*pow(h.mbIndex(i), 2) + (1.0 - h.pIndex(i))*pow(h.mfIndex(i), 2);
                b3= h.pIndex(i)*pow(h.mbIndex(i), 3) + (1.0 - h.pIndex(i))*pow(h.mfIndex(i), 3);

                value= fabs(b1 - m1) + fabs(b2 - m2) + fabs(b3 - m3);
                if ( value < minValue )
                {
                    minValue= value;
                    minIdx= i;
                }
            }

            float t= h.indexToValue(minIdx);
            th= t;
        }
        else
            th= min;

        //printf("%f\n", th);

        for ( unsigned int i= 0; i < input.size(); ++i )
            if ( !roi || (*roi)(i) )
            {
                if ( input(i) < th )
                    output(i)= 0;
                else
                    output(i)= 1;
            }
        if ( th1 )
            *th1= th;
    }

    template<typename INPUT, typename OUTPUT>
    class HerzThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        HerzThresholding2(int resolution);

        HerzThresholding2(HerzThresholding2& o);

        ~HerzThresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    HerzThresholding2<INPUT, OUTPUT>::HerzThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    HerzThresholding2<INPUT, OUTPUT>::HerzThresholding2(HerzThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    HerzThresholding2<INPUT, OUTPUT>::~HerzThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void HerzThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void HerzThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th= 0;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            SobelFilter<INPUT, INPUT> filter;
            Image<INPUT> gedge;
            gedge.resizeImage(input);

            filter.updateStride(gedge.columns);
            filter.apply(input, gedge, roi, support);

            INPUT gemin, gemax;
            gedge.getMinMax(gemin, gemax, roi);
            INPUT get= gedge.getMean(roi);

            for ( unsigned int i= 0; i < gedge.n; ++i )
                if ( gedge(i) > get )
                    gedge(i)= 1;
                else
                    gedge(i)= 0;

            float maxValue= -FLT_MAX;
            float value;
            float th;
            int maxIdx= 0;
            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                value= 0;
                th= h.indexToValue(i);
                for ( unsigned int j= 0; j < gedge.n; ++j )
                {
                    if ( !roi || (*roi)(j) > 0 )
                    {
                        if ( gedge(j) > 0 && input(j) > th )
                            ++value;
                        else if ( gedge(j) > 0 && input(j) < th )
                            --value;
                        else if ( gedge(j) == 0 && input(j) > th )
                            --value;
                    }
                }

                if ( value > maxValue )
                {
                    maxValue= value;
                    maxIdx= i;
                }
            }

            float t= h.indexToValue(maxIdx);
            th= t;
        }
        else
            th= min;

        printf("%f\n", th);

        for ( unsigned int i= 0; i < input.size(); ++i )
            if ( !roi || (*roi)(i) )
            {
                if ( input(i) < th )
                    output(i)= 0;
                else
                    output(i)= 1;
            }
        if ( th1 )
            *th1= th;
    }
}

#endif

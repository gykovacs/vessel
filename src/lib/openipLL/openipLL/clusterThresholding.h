/**
 * @file clusterThresholding.h
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
 * Clustering based thresholding methods.
 */

#ifndef _THRESHOLDING_H_
#define _THRESHOLDING_H_

#include <openipDS/Image.h>
#include <openipDS/Histogram.h>
#include <openipLL/Threshold2.h>

#include <math.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class OtsuThresholding: public Feature3<INPUT, OUTPUT>
    {
    public:
        using Feature3<INPUT, OUTPUT>::descriptor;
        using Feature3<INPUT, OUTPUT>::getMin;
        using Feature3<INPUT, OUTPUT>::getMax;
        using Feature3<INPUT, OUTPUT>::computeMinMax;
        using Feature3<INPUT, OUTPUT>::getProposedBorder;

        OtsuThresholding(int resolution);

        OtsuThresholding(OtsuThresholding& o);

        ~OtsuThresholding();

        void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    OtsuThresholding<INPUT, OUTPUT>::OtsuThresholding(int resolution)
        : Feature3<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    OtsuThresholding<INPUT, OUTPUT>::OtsuThresholding(OtsuThresholding& o)
        : Feature3<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    OtsuThresholding<INPUT, OUTPUT>::~OtsuThresholding()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void OtsuThresholding<INPUT, OUTPUT>::apply(Volume<INPUT> &input, Volume<OUTPUT> &output, Volume<unsigned char> *roi, Volume<unsigned char> *support)
    {
        INPUT min, max;
        INPUT threshold;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, roi);
            h.normalize();

            float o0, o1, m0, m1, maxIdx, maxValue;
            maxIdx= maxValue= 0;
            //printf("aaa\n");
            for ( int i= 0; i < resolution; ++i )
            {
                o0= o1= m0= m1= 0;
                for ( int j= 0; j < i; ++j )
                {
                    o0+= h(j);
                    m0+= h(j)*h.indexToValue(j);
                }
                for ( int j= i; j < resolution; ++j )
                {
                    o1+= h(j);
                    m1+= h(j)*h.indexToValue(j);
                }

                float tmp= o0*o1*(m0-m1)*(m0-m1);
                if ( tmp > maxValue )
                {
                    maxValue= tmp;
                    maxIdx= i;
                }
                //printf("ccc: %f %f %f %f %f %f\n", o0, o1, m0, m1, maxIdx, maxValue);
            }
            //printf("bbb\n");
            //printf("threshold: %f\n", h.indexToValue(maxIdx));

            INPUT threshold= h.indexToValue(maxIdx);
        }
        else
            threshold= min;

        for ( int i= 0; i < input.size(); ++i )
            if ( !roi || (*roi)(i) )
            {
                if ( input(i) < threshold )
                    output(i)= 0;
                else
                    output(i)= 1;
            }
    }

    template<typename INPUT, typename OUTPUT>
    class OtsuThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        OtsuThresholding2(int resolution);

        OtsuThresholding2(OtsuThresholding2& o);

        ~OtsuThresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    OtsuThresholding2<INPUT, OUTPUT>::OtsuThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    OtsuThresholding2<INPUT, OUTPUT>::OtsuThresholding2(OtsuThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    OtsuThresholding2<INPUT, OUTPUT>::~OtsuThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void OtsuThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;

        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float o0, o1, m0, m1, maxIdx, maxValue;
            maxIdx= maxValue= 0;
            for ( int i= 0; i < resolution; ++i )
            {
                o0= o1= m0= m1= 0;
                for ( int j= 0; j < i; ++j )
                {
                    o0+= h(j);
                    m0+= h(j)*h.indexToValue(j);
                }
                for ( int j= i; j < resolution; ++j )
                {
                    o1+= h(j);
                    m1+= h(j)*h.indexToValue(j);
                }

                float tmp= o0*o1*(m0-m1)*(m0-m1);
                if ( tmp > maxValue )
                {
                    maxValue= tmp;
                    maxIdx= i;
                }
            }

            th= h.indexToValue(maxIdx);
        }
        else
            th= min;

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
    void OtsuThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    class RiddlerThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        RiddlerThresholding2(int resolution);

        RiddlerThresholding2(RiddlerThresholding2& o);

        ~RiddlerThresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    RiddlerThresholding2<INPUT, OUTPUT>::RiddlerThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    RiddlerThresholding2<INPUT, OUTPUT>::RiddlerThresholding2(RiddlerThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    RiddlerThresholding2<INPUT, OUTPUT>::~RiddlerThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void RiddlerThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void RiddlerThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float mf, mb;
            float tn, tnm1;
            tn= h.indexToValue(h.size()/2+1);
            tnm1= -FLT_MAX;

            float nf, nb;
            while ( fabs(tn - tnm1) > h.d )
            {
                int i= h.valueToIndex(tn);

                mf= mb= 0;
                nf= nb= 0;
                for ( int j= 0; j < i; ++j )
                {
                    mb+= h(j)*h.indexToValue(j);
                    nb+= h(j);
                }
                for ( unsigned int j= i; j < h.size(); ++j )
                {
                    mf+= h(j)*h.indexToValue(j);
                    nf+= h(j);
                }

                mf/= nf;
                mb/= nb;

                tnm1= tn;
                tn= (mb + mf)/2.0;
            }

            th= tn;
        }
        else
            th= min;

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
    class YanniThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        YanniThresholding2(int resolution);

        YanniThresholding2(YanniThresholding2& o);

        ~YanniThresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    YanniThresholding2<INPUT, OUTPUT>::YanniThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    YanniThresholding2<INPUT, OUTPUT>::YanniThresholding2(YanniThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    YanniThresholding2<INPUT, OUTPUT>::~YanniThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void YanniThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void YanniThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float mf, mb;
            float nf, nb;
            float tn, tnm1;
            float tmin= h.indexToValue(0);
            float tmax= h.indexToValue(h.size()-1);
            tn= (tmax + tmin)/2.0;
            tnm1= -FLT_MAX;

            while ( fabs(tn - tnm1) > h.d )
            {
                int i= h.valueToIndex(tn);

                mf= mb= nf= nb= 0;
                for ( int j= 0; j < i; ++j )
                {
                    mb+= h(j)*h.indexToValue(j);
                    nb+= h(j);
                }
                for ( unsigned int j= i; j < h.size(); ++j )
                {
                    mf+= h(j)*h.indexToValue(j);
                    nf+= h(j);
                }

                mb/= nb;
                mf/= nf;

                tnm1= tn;
                tn= (mb + mf)/2.0;
                //printf("%f\n", tn);
            }

            int i= h.valueToIndex(tn);
            mb= 0;
            for ( int j= 0; j < i; ++j )
                mb+= h(j);
            tn= (tmax - tmin)*mb;

            //printf("threshold: %f\n", tn);

            th= tn;
        }
        else
            th= min;

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
    class LloydThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        LloydThresholding2(int resolution);

        LloydThresholding2(LloydThresholding2& o);

        ~LloydThresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    LloydThresholding2<INPUT, OUTPUT>::LloydThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    LloydThresholding2<INPUT, OUTPUT>::LloydThresholding2(LloydThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    LloydThresholding2<INPUT, OUTPUT>::~LloydThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void LloydThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void LloydThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float mf, mb;
            float nf, nb;
            float pt;
            float minValue= FLT_MAX;
            float t= 0;
            float value;
            float sigma= input.getVariance(support);

            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                pt= mf= mb= nf= nb= 0;
                for ( unsigned int j= 0; j < i; ++j )
                {
                    mb+= h(j)*h.indexToValue(j);
                    nb+= h(j);
                    pt+= h(j);
                }
                for ( unsigned int j= i; j < h.size(); ++j )
                {
                    mf+= h(j)*h.indexToValue(j);
                    nf+= h(j);
                }

                mb/= nb;
                mf/= nf;

                value= (mf + mb)/2.0 + sigma/(mf - mb)*log((1.0 - pt)/pt);
                if ( value < minValue )
                {
                    minValue= value;
                    t= h.indexToValue(i);
                }
            }

            //printf("threshold: %f\n", t);

            th= t;
        }
        else
            th= min;

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
    class KittlerThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        KittlerThresholding2(int resolution);

        KittlerThresholding2(KittlerThresholding2& o);

        ~KittlerThresholding2();

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    KittlerThresholding2<INPUT, OUTPUT>::KittlerThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    KittlerThresholding2<INPUT, OUTPUT>::KittlerThresholding2(KittlerThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    KittlerThresholding2<INPUT, OUTPUT>::~KittlerThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void KittlerThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void KittlerThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float sf, sb, p;
            float maxValue= -FLT_MAX;
            float value;
            float t= 0;

            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                sf= h.sfIndex(i);
                sb= h.sbIndex(i);
                p= h.pIndex(i);

                if ( p == 0 || sb == 0 || sf == 0 || p == 1)
                    continue;

                value= p*log(sf) + (1.0 - p)*log(sb) - p*log(p) - (1.0 - p)*log(1.0  - p);
                if ( value > maxValue )
                {
                    maxValue= value;
                    t= h.indexToValue(i);
                }

                //printf("%f %f %f %f\n", value, sf, sb, p);
            }

            //printf("threshold: %f\n", t);

            th= t;
        }
        else
            th= min;

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
    class Jawahar1Thresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        Jawahar1Thresholding2(float tau, int resolution);

        Jawahar1Thresholding2(Jawahar1Thresholding2& o);

        ~Jawahar1Thresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
        float tau;
    };

    template<typename INPUT, typename OUTPUT>
    Jawahar1Thresholding2<INPUT, OUTPUT>::Jawahar1Thresholding2(float tau, int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
        this->tau= tau;
    }

    template<typename INPUT, typename OUTPUT>
    Jawahar1Thresholding2<INPUT, OUTPUT>::Jawahar1Thresholding2(Jawahar1Thresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
        this->tau= o.tau;
    }

    template<typename INPUT, typename OUTPUT>
    Jawahar1Thresholding2<INPUT, OUTPUT>::~Jawahar1Thresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void Jawahar1Thresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void Jawahar1Thresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            Vector<float> muf(h.size());
            Vector<float> mub(h.size());

            for ( unsigned int i= 0; i < muf.size(); ++i )
            {
                muf(i)= float(rand()%1000)/1000.0;
                mub(i)= float(rand()%1000)/1000.0;
            }
            float sumf= 0, sumb= 0;
            for ( unsigned int i= 0; i < muf.size(); ++i )
            {
                sumf+= muf(i);
                sumb+= mub(i);
            }
            muf/= sumf;
            mub/= sumb;

            //float dgmf;
            //float dgmb;
            //float g;
            float mf;
            float mb;
            float nf, nb;
            float tmp;
            float diff= 0;

            while ( 1 )
            {
                mf= mb= nf= nb= 0;
                for ( unsigned int j= 0; j < h.size(); ++j )
                {
                    mb+= h.indexToValue(j)*h(j)*mub(j);
                    nb+= h(j)*mub(j);
                }
                mb/= nb;

                for ( unsigned int j= 0; j < h.size(); ++j )
                {
                    mf+= h.indexToValue(j)*h(j)*muf(j);
                    nf+= h(j)*muf(j);
                }
                mf/= nf;

                diff= 0;
                for ( unsigned int i= 0; i < mub.size(); ++i )
                {
                    tmp= h.indexToValue(i);
                    muf(i)= 1.0/(1.0 + pow(fabs(tmp - mf)/fabs(tmp - mb), 2.0/(tau - 1.0)));
                    diff+= fabs(mub(i) - 1.0 + muf(i));
                    mub(i)= 1.0 - muf(i);
                }

                //printf("%f\n", diff);
                if ( diff < 0.1 )
                    break;
            }

            /*for ( int i= 0; i < muf.size(); ++i )
                printf("%f %f\n", mub(i), muf(i));*/

            float minDiff= FLT_MAX;
            int minIdx= -1;
            for ( unsigned int i= 0; i < muf.size(); ++i )
                if ( fabs(muf(i) - mub(i)) < minDiff )
                {
                    minDiff= fabs(muf(i) - mub(i));
                    minIdx= i;
                }

            th= h.indexToValue(minIdx);
        }
        else
            th= min;

        //printf("threshold: %f\n", threshold);

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
    class Jawahar2Thresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        Jawahar2Thresholding2(float tau, int resolution);

        Jawahar2Thresholding2(Jawahar2Thresholding2& o);

        ~Jawahar2Thresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
        float tau;
    };

    template<typename INPUT, typename OUTPUT>
    Jawahar2Thresholding2<INPUT, OUTPUT>::Jawahar2Thresholding2(float tau, int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
        this->tau= tau;
    }

    template<typename INPUT, typename OUTPUT>
    Jawahar2Thresholding2<INPUT, OUTPUT>::Jawahar2Thresholding2(Jawahar2Thresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
        this->tau= o.tau;
    }

    template<typename INPUT, typename OUTPUT>
    Jawahar2Thresholding2<INPUT, OUTPUT>::~Jawahar2Thresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void Jawahar2Thresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void Jawahar2Thresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            Vector<float> muf(h.size());
            Vector<float> mub(h.size());

            for ( unsigned int i= 0; i < muf.size(); ++i )
            {
                muf(i)= float(rand()%1000)/1000.0;
                mub(i)= float(rand()%1000)/1000.0;
            }
            float sumf= 0, sumb= 0;
            for ( unsigned int i= 0; i < muf.size(); ++i )
            {
                sumf+= muf(i);
                sumb+= mub(i);
            }
            muf/= sumf;
            mub/= sumb;

            //float dgmf;
            //float dgmb;
            //float g;
            float mf;
            float mb;
            float nf, nb;
            float tmp;
            float diff= 0;
            float betaf, betab, sigmaf, sigmab;
            float tmp1, tmp2;
            float dist;

            while ( 1 )
            {
                mf= mb= nf= nb= 0;
                for ( unsigned int j= 0; j < h.size(); ++j )
                {
                    mb+= h.indexToValue(j)*h(j)*mub(j);
                    nb+= h(j)*mub(j);
                }
                mb/= nb;

                for ( unsigned int j= 0; j < h.size(); ++j )
                {
                    mf+= h.indexToValue(j)*h(j)*muf(j);
                    nf+= h(j)*muf(j);
                }
                mf/= nf;

                //printf("%f %f\n", mb, mf);

                diff= 0;
                for ( unsigned int i= 0; i < mub.size(); ++i )
                {
                    tmp= h.indexToValue(i);

                    betaf= betab= 0;
                    tmp1= tmp2= 0;
                    for ( unsigned int j= 0; j < h.size(); ++j )
                    {
                        tmp1+= h(j)*mub(j);
                        tmp2+= h(j)*(mub(j) + muf(j));
                    }
                    betab= tmp1/tmp2;
                    tmp1= tmp2= 0;
                    for ( unsigned int j= 0; j < h.size(); ++j )
                    {
                        tmp1+= h(j)*muf(j);
                        tmp2+= h(j)*(mub(j) + muf(j));
                    }
                    betaf= tmp1/tmp2;

                    sigmab= sigmaf= 0;
                    tmp1= tmp2= 0;
                    for ( unsigned int j= 0; j < h.size(); ++j )
                    {
                        tmp= h.indexToValue(j);
                        tmp1+= h(j)*mub(j)*(tmp - mb)*(tmp - mb);
                        tmp2+= h(j)*mub(j);
                    }
                    sigmab= tmp1/tmp2;

                    tmp1= tmp2= 0;
                    for ( unsigned int j= 0; j < h.size(); ++j )
                    {
                        tmp= h.indexToValue(j);
                        tmp1+= h(j)*muf(j)*(tmp - mf)*(tmp - mf);
                        tmp2+= h(j)*muf(j);
                    }
                    sigmaf= tmp1/tmp2;

                    dist= 0.5*(tmp - mb)/sigmab*(tmp - mb) + log(sqrt(sigmab)) - log(betab);
                    dist/= 0.5*(tmp - mf)/sigmaf*(tmp - mf) + log(sqrt(sigmaf)) - log(betaf);
                    //printf("d %f\n", dist);

                    muf(i)= 1.0/(1.0 + pow(dist, 2.0/(tau - 1.0)));
                    diff+= fabs(mub(i) - 1.0 + muf(i));
                    mub(i)= 1.0 - muf(i);
                }



                //printf("%f\n", diff);
                if ( diff < 0.1 )
                    break;
            }

            /*for ( int i= 0; i < muf.size(); ++i )
                printf("%f %f\n", mub(i), muf(i));*/

            float minDiff= FLT_MAX;
            int minIdx= -1;
            for ( unsigned int i= 0; i < muf.size(); ++i )
                if ( fabs(muf(i) - mub(i)) < minDiff )
                {
                    minDiff= fabs(muf(i) - mub(i));
                    minIdx= i;
                }

            th= h.indexToValue(minIdx);
        }
        else
            th= min;

        //printf("threshold: %f\n", threshold);

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

/**
 * @file entropyThresholding.h
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
 * Entropy based thresholding methods.
 */

#ifndef _ENTROPYTHRESHOLDING_H_
#define _ENTROPYTHRESHOLDING_H_

#include <openipDS/Image.h>
#include <openipDS/Histogram.h>

#include <openipSC/distributions.h>

#include <openipLL/Threshold2.h>

#include <math.h>
#include <algorithm>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class KapurThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        KapurThresholding2(int resolution);

        KapurThresholding2(KapurThresholding2& o);

        ~KapurThresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    KapurThresholding2<INPUT, OUTPUT>::KapurThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    KapurThresholding2<INPUT, OUTPUT>::KapurThresholding2(KapurThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    KapurThresholding2<INPUT, OUTPUT>::~KapurThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void KapurThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void KapurThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float hf, hb;
            float maxValue= -FLT_MAX;
            float value;
            int maxIdx= 0;

            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                hf= hb= 0;
                for ( unsigned int j= 0; j < i; ++j )
                {
                    if ( h(j) == 0 || h.pIndex(j) == 0 )
                        continue;
                    hb+= -h(j)/h.pIndex(j)*log(h(j)/h.pIndex(j));
                }
                for ( unsigned int j= i; j < h.size(); ++j )
                {
                    if ( h(j) == 0 || h.pIndex(j) == 0 )
                        continue;
                    hf+= -h(j)/h.pIndex(j)*log(h(j)/h.pIndex(j));
                }
                value= hb + hf;
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

        //printf("%f\n", threshold);

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
    class YenThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        YenThresholding2(int resolution);

        YenThresholding2(YenThresholding2& o);

        ~YenThresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    YenThresholding2<INPUT, OUTPUT>::YenThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    YenThresholding2<INPUT, OUTPUT>::YenThresholding2(YenThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    YenThresholding2<INPUT, OUTPUT>::~YenThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void YenThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void YenThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float hf, hb;
            float maxValue= -FLT_MAX;
            float value;
            int maxIdx= 0;

            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                hf= hb= 0;
                if ( h.pIndex(i) == 0 || h.pIndex(i) == 1 )
                    continue;
                for ( unsigned int j= 0; j < i; ++j )
                    hb+= (h(j)/h.pIndex(i))*(h(j)/h.pIndex(i));
                for ( unsigned int j= i; j < h.size(); ++j )
                    hf+= (h(j)/(1.0 - h.pIndex(i)))*(h(j)/(1.0 - h.pIndex(i)));
                if ( hb == 0 || hf == 0 )
                    continue;
                value= - log(hb) - log(hf);
                //printf("%f %f %f\n", value, hf, hb);
                if ( value > maxValue )
                {
                    maxValue= value;
                    maxIdx= i;
                }
            }

            th= h.indexToValue(maxIdx);
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
    class SahooThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        SahooThresholding2(int resolution);

        SahooThresholding2(SahooThresholding2& o);

        ~SahooThresholding2();

        float threshold0(Histogram& h, float p);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    SahooThresholding2<INPUT, OUTPUT>::SahooThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    SahooThresholding2<INPUT, OUTPUT>::SahooThresholding2(SahooThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    SahooThresholding2<INPUT, OUTPUT>::~SahooThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    float SahooThresholding2<INPUT, OUTPUT>::threshold0(Histogram& h, float p)
    {
        float hb, hf;
        float maxValue= -FLT_MAX;
        int maxIdx= 0;
        float value;

        for ( unsigned int i= 0; i < h.size(); ++i )
        {
            hb= hf= 0;
            for ( unsigned int j= 0; j < i; ++j )
                hb+= pow(h(j)/h.pIndex(i), p);
            for ( unsigned int j= i; j < h.size(); ++j )
                hf+= pow(h(j)/h.pIndex(i), p);
            value= 1.0/(1-p)*(log(hb) + log(hf));
            if ( value > maxValue )
            {
                maxValue= value;
                maxIdx= i;
            }
        }
        return h.indexToValue(maxIdx);
    }

    template<typename INPUT, typename OUTPUT>
    void SahooThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void SahooThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float level5= 5.0*h.d;

            Vector<float> ts;
            ts.resize(3);
            ts[0]= threshold0(h, 0.5);
            ts[1]= threshold0(h, 1.0);
            ts[2]= threshold0(h, 2.0);
            sort(ts.begin(), ts.end());
            float t1, t2, t3;
            t1= ts[0];
            t2= ts[1];
            t3= ts[2];

            float b1= 0, b2= 0, b3= 0;
            if ( (fabs(t1 - t2) <= level5 && fabs(t2 - t3) <= level5) || (fabs(t1 - t2) > level5 && fabs(t2 - t3) > level5 ) )
            {
                b1= 1;
                b2= 2;
                b3= 1;
            }
            else if ( fabs(t1 - t2) <= level5 && fabs(t2 - t3) > level5 )
            {
                b1= 0;
                b2= 1;
                b3= 3;
            }
            else if ( fabs(t1 - t2) > level5 && fabs(t2 - t3) <= level5 )
            {
                b1= 3;
                b2= 1;
                b3= 0;
            }

            float w= h.pValue(t3) - h.pValue(t1);
            float t= t1*(h.pValue(t1) + 0.25*w*b1) + 0.25*t2*w*b2 + t3*(1.0 - h.pValue(t3) + 0.25*w*b3);

            th= t;
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
    class PunAThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        PunAThresholding2(int resolution);

        PunAThresholding2(PunAThresholding2& o);

        ~PunAThresholding2();

        float H(Histogram& h, int i);

        float Hf(Histogram& h, int i);

        float alpha(Histogram& h, int i);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    PunAThresholding2<INPUT, OUTPUT>::PunAThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    PunAThresholding2<INPUT, OUTPUT>::PunAThresholding2(PunAThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    PunAThresholding2<INPUT, OUTPUT>::~PunAThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    float PunAThresholding2<INPUT, OUTPUT>::H(Histogram& h, int i)
    {
        float f= 0, b= 0;
        for ( int j= 0; j < i; ++j )
            b+= h(j)*log(h(j));
        for ( unsigned int j= i; j < h.size(); ++j )
            f+= h(j)*log(h(j));
        return -f-b;
    }

    template<typename INPUT, typename OUTPUT>
    float PunAThresholding2<INPUT, OUTPUT>::Hf(Histogram& h, int i)
    {
        return -h.pIndex(i)*log(h.pIndex(i)) - (1.0 - h.pIndex(i))*log(1.0 - h.pIndex(i));
    }

    template<typename INPUT, typename OUTPUT>
    float PunAThresholding2<INPUT, OUTPUT>::alpha(Histogram &h, int i)
    {
        float aMax= 0;
        float maxValue= -FLT_MAX;
        float value;

        for ( float a= 0.0; a <= 1.0; a+= 0.05 )
        {
            float maxB= 0;
            for ( int j= 0; j < i; ++j )
                if ( h(j) > maxB )
                    maxB= h(j);
            float maxF= 0;
            for ( unsigned int j= i; j < h.size(); ++j )
                if ( h(j) > maxF )
                    maxF= h(j);

            value= a*log(h.pIndex(i))/log(maxB) + (1.0 - a)*log(1.0 - h.pIndex(i))/log(maxF);
            if ( value > maxValue )
            {
                maxValue= value;
                aMax= a;
            }
        }

        return aMax;
    }

    template<typename INPUT, typename OUTPUT>
    void PunAThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void PunAThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float minDiff= FLT_MAX;
            int minIdx= 0;
            float value;
            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                value= fabs(Hf(h, i) - alpha(h, i)*H(h, i));
                if ( value < minDiff )
                {
                    minDiff= value;
                    minIdx= i;
                }
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
    class PunBThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        PunBThresholding2(int resolution);

        PunBThresholding2(PunBThresholding2& o);

        ~PunBThresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    PunBThresholding2<INPUT, OUTPUT>::PunBThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    PunBThresholding2<INPUT, OUTPUT>::PunBThresholding2(PunBThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    PunBThresholding2<INPUT, OUTPUT>::~PunBThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void PunBThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void PunBThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float sum= 0;
            int na= 0;
            for ( unsigned na= 0; na < h.size(); ++na )
            {
                sum+= h(na);
                if ( sum >= 0.5 )
                    break;
            }

            float tmp1= 0, tmp2= 0;
            for ( int i= 0; i < na; ++i )
            {
                if ( h(i) == 0 )
                    continue;
                tmp1+= h(i)*log(h(i));
            }
            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                if ( h(i) == 0 )
                    continue;
                tmp2+= h(i)*log(h(i));
            }
            float alpha= tmp1/tmp2;

            //printf("alpha: %f\n", alpha);

            float check= 0.5 + fabs(0.5 - alpha);
            //printf("check: %f\n", check);
            sum= 0;
            unsigned int s;
            for ( s= 0; s < h.size(); ++s )
            {
                sum+= h(s);
                if ( sum >= check )
                    break;
            }
            float sumtmp= sum - h(s - 1);
            if ( fabs(sum - check) > fabs(sumtmp - check) )
                --s;

            th= h.indexToValue(s);
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
    class LiThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        LiThresholding2(int resolution);

        LiThresholding2(LiThresholding2& o);

        ~LiThresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    LiThresholding2<INPUT, OUTPUT>::LiThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    LiThresholding2<INPUT, OUTPUT>::LiThresholding2(LiThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    LiThresholding2<INPUT, OUTPUT>::~LiThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void LiThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void LiThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float minValue= FLT_MAX;
            int minIdx= 0;
            float value;

            float tmp1;
            float tmp2;
            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                tmp1= tmp2= 0;
                for ( unsigned int j= 0; j < i; ++j )
                {
                    if ( h.indexToValue(j) == 0 || h.mbIndex(j) == 0 )
                        continue;
                    tmp1+= h.indexToValue(j)*h(j)*log(h.indexToValue(j)/h.mbIndex(i));
                }
                for ( unsigned int j= i; j < h.size(); ++j )
                {
                    if ( h.indexToValue(j) == 0 || h.mfIndex(j) == 0 )
                        continue;
                    tmp2+= h.indexToValue(j)*h(j)*log(h.indexToValue(j)/h.mfIndex(i));
                }
                value= tmp1 + tmp2;

                //printf("%f %f %f\n", value, tmp1, tmp2);
                if ( value < minValue )
                {
                    minValue= value;
                    minIdx= i;
                }
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
    class BrinkThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        BrinkThresholding2(int resolution);

        BrinkThresholding2(BrinkThresholding2& o);

        ~BrinkThresholding2();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    BrinkThresholding2<INPUT, OUTPUT>::BrinkThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    BrinkThresholding2<INPUT, OUTPUT>::BrinkThresholding2(BrinkThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    BrinkThresholding2<INPUT, OUTPUT>::~BrinkThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void BrinkThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void BrinkThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float minValue= FLT_MAX;
            int minIdx= 0;
            float value;

            float tmp1;
            float tmp2;
            float tmp;
            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                tmp1= tmp2= 0;
                for ( unsigned int j= 0; j < i; ++j )
                {
                    if ( h.indexToValue(j) == 0 || h.mbIndex(i) == 0 )
                        continue;
                    tmp= h.mbIndex(i)*log(h.mbIndex(i)/h.indexToValue(j)) + h.indexToValue(j)*log(h.indexToValue(j)/h.mbIndex(i));
                    tmp1+= h(j)*tmp;
                }
                for ( unsigned int j= i; j < h.size(); ++j )
                {
                    if ( h.indexToValue(j) == 0 || h.mfIndex(j) == 0 )
                        continue;
                    tmp= h.mfIndex(i)*log(h.mfIndex(i)/h.indexToValue(j)) + h.indexToValue(j)*log(h.indexToValue(j)/h.mfIndex(i));
                    tmp2+= h(j)*tmp;
                }
                value= tmp1 + tmp2;

                //printf("%f %f %f\n", value, tmp1, tmp2);
                if ( value < minValue )
                {
                    minValue= value;
                    minIdx= i;
                }
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
    class PalThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        PalThresholding2(int resolution);

        PalThresholding2(PalThresholding2& o);

        ~PalThresholding2();

        float qf(Histogram& h, int i);

        float qb(Histogram& h, int i);

        float pf(Histogram& h, int i);

        float pb(Histogram& h, int i);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    PalThresholding2<INPUT, OUTPUT>::PalThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    PalThresholding2<INPUT, OUTPUT>::PalThresholding2(PalThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    PalThresholding2<INPUT, OUTPUT>::~PalThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    float PalThresholding2<INPUT, OUTPUT>::qf(Histogram& h, int i)
    {
        return poissonDistribution(h.mfIndex255(i), i);
    }

    template<typename INPUT, typename OUTPUT>
    float PalThresholding2<INPUT, OUTPUT>::qb(Histogram& h, int i)
    {
        return poissonDistribution(h.mbIndex255(i), i);
    }

    template<typename INPUT, typename OUTPUT>
    float PalThresholding2<INPUT, OUTPUT>::pf(Histogram& h, int i)
    {
        if ( h.pIndex(i) == 1 )
            return 0;
        return h(i)/(1.0 - h.pIndex(i));
    }

    template<typename INPUT, typename OUTPUT>
    float PalThresholding2<INPUT, OUTPUT>::pb(Histogram& h, int i)
    {
        if ( h.pIndex(i) == 0 )
            return 0;
        return h(i)/h.pIndex(i);
    }

    template<typename INPUT, typename OUTPUT>
    void PalThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void PalThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float maxValue= -FLT_MAX;
            int maxIdx= 0;
            float value;

            float tmp1;
            float tmp2;
            float tmp;
            double pbhj, qbhj, pfhj, qfhj;
            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                tmp1= tmp2= 0;
                for ( unsigned int j= 0; j < i; ++j )
                {
                    pbhj= pb(h,j);
                    qbhj= qb(h,j);
                    if ( pbhj <= 0 || qbhj <= 0 || pbhj > 1 || qbhj > 1 || pbhj != pbhj || qbhj != qbhj )
                        continue;
                    tmp= pbhj*log(pbhj/qbhj) + qbhj*log(qbhj/pbhj);
                    tmp1+= tmp;
                }
                for ( unsigned int j= i; j < h.size(); ++j )
                {
                    pfhj= pf(h,j);
                    qfhj= qf(h,j);
                    if ( pfhj <= 0 || qfhj <= 0 || pfhj > 1 || qfhj > 1 || pfhj != pfhj || qfhj != qfhj)
                        continue;
                    tmp= pfhj*log(pfhj/qfhj) + qfhj*log(qfhj/pfhj);
                    tmp2+= tmp;
                }
                value= tmp1 + tmp2;

                if ( value > maxValue )
                {
                    maxValue= value;
                    maxIdx= i;
                }
            }

            th= h.indexToValue(maxIdx);
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
    class ShanbagThresholding2: public Transform2<INPUT, OUTPUT>, public Threshold2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        ShanbagThresholding2(int resolution);

        ShanbagThresholding2(ShanbagThresholding2& o);

        ~ShanbagThresholding2();

        float muf(Histogram& h, int T, int i);

        float mub(Histogram& h, int T, int i);

        float Hf(Histogram& h, int T);

        float Hb(Histogram& h, int T);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void threshold(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL, INPUT* th1= NULL, Image<INPUT>* th2= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    ShanbagThresholding2<INPUT, OUTPUT>::ShanbagThresholding2(int resolution)
        : Transform2<INPUT, OUTPUT>(), Threshold2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    ShanbagThresholding2<INPUT, OUTPUT>::ShanbagThresholding2(ShanbagThresholding2& o)
        : Transform2<INPUT, OUTPUT>(o), Threshold2<INPUT, OUTPUT>(o)
    {
        this->resolution= o.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    ShanbagThresholding2<INPUT, OUTPUT>::~ShanbagThresholding2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    float ShanbagThresholding2<INPUT, OUTPUT>::muf(Histogram& h, int T, int i)
    {
        float res= 0;
        for ( int j= T; j < i; ++j )
            res+= h(j);
        return 0.5 + res/(2*(1.0 - h.pIndex(T)));
    }

    template<typename INPUT, typename OUTPUT>
    float ShanbagThresholding2<INPUT, OUTPUT>::mub(Histogram& h, int T, int i)
    {
        float res= 0;
        for ( int j= i; j < T; ++j )
            res+= h(j);
        return 0.5 + res/(2*h.pIndex(T));
    }

    template<typename INPUT, typename OUTPUT>
    float ShanbagThresholding2<INPUT, OUTPUT>::Hf(Histogram& h, int i)
    {
        float res= 0;
        for ( unsigned int j= i; j < h.size(); ++j )
            res+= (h(j)/(1.0 - h.pIndex(i)))*log(muf(h, i, j));
        return -res;
    }

    template<typename INPUT, typename OUTPUT>
    float ShanbagThresholding2<INPUT, OUTPUT>::Hb(Histogram& h, int i)
    {
        float res= 0;
        for ( int j= 0; j < i; ++j )
            res+= (h(j)/h.pIndex(i))*log(mub(h, i, j));
        return -res;
    }

    template<typename INPUT, typename OUTPUT>
    void ShanbagThresholding2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        threshold(input, output, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void ShanbagThresholding2<INPUT, OUTPUT>::threshold(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support, INPUT* th1, Image<INPUT>* /*th2*/)
    {
        INPUT min, max;
        INPUT th;
        input.getMinMax(min, max, roi);

        if ( max > min )
        {
            Histogram h;
            h.computeHistogram(&input, resolution, min, max, support);
            h.normalize();

            float minValue= FLT_MAX;
            int minIdx= 0;
            float value;

            //float tmp1;
            //float tmp2;
            //float tmp;
            for ( unsigned int i= 0; i < h.size(); ++i )
            {
                value= fabs(Hf(h, i) - Hb(h, i));

                //printf("%f %f %f\n", value, Hf(h,i), Hb(h,i));
                if ( value < minValue )
                {
                    minValue= value;
                    minIdx= i;
                }
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

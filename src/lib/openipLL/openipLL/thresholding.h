#ifndef THRESHOLDING_H
#define THRESHOLDING_H

#include <openipDS/Vector.h>
#include <openipDS/Histogram.h>
#include <openipDS/Feature3.h>
#include <openipDS/MatchedFilter2s.h>

#include <openipLL/clusterThresholding.h>
#include <openipLL/entropyThresholding.h>
#include <openipLL/attributeSimilarityThresholding.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class LocalThresholding: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        LocalThresholding(int window, int resolution);

        LocalThresholding(const LocalThresholding& l);

        ~LocalThresholding();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        Transform2<INPUT, OUTPUT>* t;
    };

    template<typename INPUT, typename OUTPUT>
    class ThresholdAdaptive: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        ThresholdAdaptive(float th);

        ThresholdAdaptive(const ThresholdAdaptive& l);

        ~ThresholdAdaptive();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        Transform2<INPUT, OUTPUT>* t;
        float th;
    };

    template<typename INPUT, typename OUTPUT>
    ThresholdAdaptive<INPUT, OUTPUT>::ThresholdAdaptive(float th)
        : Transform2<INPUT, OUTPUT>()
    {
        this->th= th;
    }

    template<typename INPUT, typename OUTPUT>
    ThresholdAdaptive<INPUT, OUTPUT>::ThresholdAdaptive(const ThresholdAdaptive& t)
        : Transform2<INPUT, OUTPUT>(t)
    {
        this->th= t.th;
    }

    template<typename INPUT, typename OUTPUT>
    ThresholdAdaptive<INPUT, OUTPUT>::~ThresholdAdaptive()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ThresholdAdaptive<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        /*StandardDeviationFeature2<INPUT, float> sdf(2);
        Image<float> stdev;
        stdev.resizeImage(input);
        sdf.apply(input, stdev, roi, support);

        Image<float> tmp;
        tmp= stdev;
        tmp.normalize(0, 255);
        writeImage("th_stdev.png", tmp);

        RiddlerThresholding2<float, unsigned char> rt(256);
        Image<unsigned char> thres;
        thres.resizeImage(input);
        Image<unsigned char> thres2;
        thres2.resizeImage(input);
        rt.apply(stdev, thres);

        tmp= thres;
        tmp.normalize(0, 255);
        writeImage("th_thres.png", tmp);

        StructuringElementDisk sed(3);
        sed.updateStride(input.columns);
        grayscaleDilate(&thres, &thres2, sed, NULL);
        tmp= thres2;
        tmp.normalize(0, 255);
        writeImage("th_thres2.png", tmp);

        rt.apply(input, output, &thres2, &thres2);*/

        MatchedCCorrelationGaussianFilter2<INPUT, float> mcgf(15.0, 1.0, 1.0, 11.0, -1.0, 0.0);
        mcgf.setMatchType(1.0, 1.0, 3.0, MATCHED_GAUSSIAN_FILTER_SIGMA);

        Image<float> tmp;
        tmp.resizeImage(input);
        mcgf.apply(input, tmp, roi, support);
        Image<float> tmp2;
        tmp2.resizeImage(input);
        tmp2= tmp;
        tmp2.normalize(0, 255);
        writeImage("th_mcgf.png", tmp2);

        for ( unsigned int i= 0; i < tmp2.n; ++i )
            if ( tmp(i) > 0.4 && (*roi)(i) )
                output(i)= 1;
            else
                output(i)= 0;
    }


    template<typename INPUT, typename OUTPUT>
    class ThresholdAtRange: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        ThresholdAtRange(float th);

        ThresholdAtRange(const ThresholdAtRange& l);

        ~ThresholdAtRange();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        Transform2<INPUT, OUTPUT>* t;
        float th;
    };

    template<typename INPUT, typename OUTPUT>
    ThresholdAtRange<INPUT, OUTPUT>::ThresholdAtRange(float th)
        : Transform2<INPUT, OUTPUT>()
    {
        this->th= th;
    }

    template<typename INPUT, typename OUTPUT>
    ThresholdAtRange<INPUT, OUTPUT>::ThresholdAtRange(const ThresholdAtRange& t)
        : Transform2<INPUT, OUTPUT>(t)
    {
        this->th= t.th;
    }

    template<typename INPUT, typename OUTPUT>
    ThresholdAtRange<INPUT, OUTPUT>::~ThresholdAtRange()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ThresholdAtRange<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> */*support*/)
    {
        INPUT min, max;
        input.getMinMax(min, max, roi);

        float threshold= min;

        output= 0;

        threshold= min + (max - min)*th;

        int k= 0;
        for ( unsigned int i= 0; i < input.n; ++i )
            //if ( !roi || (*roi)(i) )
            {
                if ( input(i) > threshold )
                {
                    ++k;
                    output(i)= 1;
                }
                else
                    output(i)= 0;
            }
            /*else
                output(i)= 0;*/
        printf("%d\n", k);
    }

    template<typename INPUT, typename OUTPUT>
    class ThresholdAtCorrelation: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::getProposedBorder;

        ThresholdAtCorrelation(int resolution);

        ThresholdAtCorrelation(const ThresholdAtCorrelation& o);

        ~ThresholdAtCorrelation();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int resolution;
    };

    template<typename INPUT, typename OUTPUT>
    ThresholdAtCorrelation<INPUT, OUTPUT>::ThresholdAtCorrelation(int resolution)
        : Transform2<INPUT, OUTPUT>()
    {
        this->resolution= resolution;
    }

    template<typename INPUT, typename OUTPUT>
    ThresholdAtCorrelation<INPUT, OUTPUT>::ThresholdAtCorrelation(const ThresholdAtCorrelation& t)
        : Transform2<INPUT, OUTPUT>(t)
    {
        this->resolution= t.resolution;
    }

    template<typename INPUT, typename OUTPUT>
    ThresholdAtCorrelation<INPUT, OUTPUT>::~ThresholdAtCorrelation()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ThresholdAtCorrelation<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> */*support*/)
    {
        INPUT min, max;
        input.getMinMax(min, max, roi);

        float d= (max - min)/resolution;
        float threshold= min;

        output= 0;

        Vector<float> correlations;

        while ( threshold < max )
        {
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( input(i) > threshold )
                    output(i)= 255;
                else
                    output(i)= 0;

            correlations.push_back(fabs(input.correlation(output, roi)));

            threshold+= d;
        }

        int maxIdx= 0;
        for ( unsigned int i= 0; i < correlations.size(); ++i )
            if ( correlations(maxIdx) < correlations(i) )
                maxIdx= i;

        for ( unsigned int i= 0; i < correlations.size(); ++i )
            printf("%f ", correlations(i));
        printf("\n");

        threshold= min + maxIdx*d;

        tprintf("final thresholding %f %f %d %f\n", threshold, min, maxIdx, d);
        int k= 0;
        for ( unsigned int i= 0; i < input.n; ++i )
            //if ( !roi || (*roi)(i) )
            {
                if ( input(i) > threshold )
                {
                    ++k;
                    output(i)= 1;
                }
                else
                    output(i)= 0;
            }
            /*else
                output(i)= 0;*/
        printf("%d\n", k);
    }

}

#endif // THRESHOLDING_H

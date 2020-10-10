#ifndef STATISTICALTESTS_H
#define STATISTICALTESTS_H

#include <openipDS/Vector.h>
#include <openipML/StatisticalTest.h>
#include <openipML/statisticalFunctions.h>

extern int resultOnError;

namespace openip
{
    //#ifndef RESULTONERROR_
    //#define RESULTONERROR_

    //#endif

    template<typename T>
    class OneSampleUTest: public StatisticalTest<T>
    {
    public:
        using StatisticalTest<T>::significance;

        OneSampleUTest(double ssig= 0.95);

        OneSampleUTest(const OneSampleUTest& o);

        ~OneSampleUTest();

        double test(Vector<T>& sample, double tmu, double tsigma);

        double test(double sampleMean, double sampleSize, double tmu, double tsigma);

        Normaldist* nd;
    };

    template<typename T>
    OneSampleUTest<T>::OneSampleUTest(double ssig)
        : StatisticalTest<T>(ssig)
    {
        nd= new Normaldist(0,1);
    }

    template<typename T>
    OneSampleUTest<T>::OneSampleUTest(const OneSampleUTest& o)
        : StatisticalTest<T>(o)
    {
    }

    template<typename T>
    OneSampleUTest<T>::~OneSampleUTest()
    {
        delete nd;
    }

    template<typename T>
    double OneSampleUTest<T>::test(Vector<T>& sample, double tmu, double tsigma)
    {
        return this->test(sample.getMean(), sample.size(), tmu, tsigma);
    }

    template<typename T>
    double OneSampleUTest<T>::test(double sampleMean, double sampleSize, double tmu, double tsigma)
    {
        errorInDistributionArgs= 0;
        double ts= (sampleMean - tmu)/tsigma*sqrt(sampleSize);
        double threshold= 1 - significance/2;

        //printf("%f %f %f\n", ts, threshold, nd->invcdf(threshold));
        float tmp= nd->invcdf(threshold);

        if ( errorInDistributionArgs )
            return resultOnError;

        if ( fabs(ts) < tmp )
            return 1;
        else
            return 0;
    }

    template<typename T>
    class TwoSampleUTest: public StatisticalTest<T>
    {
    public:
        using StatisticalTest<T>::significance;

        TwoSampleUTest(double ssig= 0.95);

        TwoSampleUTest(const TwoSampleUTest& o);

        ~TwoSampleUTest();

        double test(Vector<T>& sample1, Vector<T>& sample2, double tsigma1, double tsigma2);

        double test(double sample1Mean, double sample1Size, double sample2Mean, double sample2Size, double tsigma1, double tsigma2);

        Normaldist* nd;
    };

    template<typename T>
    TwoSampleUTest<T>::TwoSampleUTest(double ssig)
        : StatisticalTest<T>(ssig)
    {
        nd= new Normaldist(0,1);
    }

    template<typename T>
    TwoSampleUTest<T>::TwoSampleUTest(const TwoSampleUTest& o)
        : StatisticalTest<T>(o)
    {
    }

    template<typename T>
    TwoSampleUTest<T>::~TwoSampleUTest()
    {
        delete nd;
    }

    template<typename T>
    double TwoSampleUTest<T>::test(Vector<T>& sample1, Vector<T>& sample2, double tsigma1, double tsigma2)
    {
        return this->test(sample1.getMean(), sample1.size(), sample2.getMean(), sample2.size(), tsigma1, tsigma2);
    }

    template<typename T>
    double TwoSampleUTest<T>::test(double sample1Mean, double sample1Size, double sample2Mean, double sample2Size, double tsigma1, double tsigma2)
    {
        errorInDistributionArgs= 0;
        double ts= (sample1Mean - sample2Mean)/sqrt(tsigma1/sample1Size + tsigma2/sample2Size);
        double threshold= 1 - significance/2;

        //printf("%f %f %f\n", ts, threshold, nd->invcdf(threshold));

        float tmp= nd->invcdf(threshold);

        if ( errorInDistributionArgs )
            return resultOnError;

        if ( fabs(ts) < tmp )
            return 1;
        else
            return 0;
    }

    template<typename T>
    class OneSampleTTest: public StatisticalTest<T>
    {
    public:
        using StatisticalTest<T>::significance;

        OneSampleTTest(double ssig= 0.95);

        OneSampleTTest(const OneSampleTTest& o);

        ~OneSampleTTest();

        double test(Vector<T>& sample, double tmu);

        double test(double sampleMean, double sampleSize, double sampleVariance, double tmu);

        Studenttdist* nd;
    };

    template<typename T>
    OneSampleTTest<T>::OneSampleTTest(double ssig)
        : StatisticalTest<T>(ssig)
    {
        nd= new Studenttdist(2,0,1);
    }

    template<typename T>
    OneSampleTTest<T>::OneSampleTTest(const OneSampleTTest& o)
        : StatisticalTest<T>(o)
    {
    }

    template<typename T>
    OneSampleTTest<T>::~OneSampleTTest()
    {
        delete nd;
    }

    template<typename T>
    double OneSampleTTest<T>::test(Vector<T>& sample, double tmu)
    {
        /*for ( int i= 0; i < sample.size(); ++i )
            printf("%f ", sample(i));
        printf("\n"); fflush(stdout);*/
        double sampleSize= double(sample.size());
        double sampleMean= sample.getMean();
        double sampleVariance= sample.getVariance();
        //printf("%f %f %f %f\n", sampleMean, sampleSize, sampleVariance, tmu); fflush(stdout);
        //printf("qwer\n"); fflush(stdout);
        double res= test(sampleMean, sampleSize, sampleVariance, tmu);
        //printf("asdf\n"); fflush(stdout);
        return res;
    }

    template<typename T>
    double OneSampleTTest<T>::test(double sampleMean, double sampleSize, double sampleVariance, double tmu)
    {
        //printf("mean: %f, size: %f, var: %f tmu: %f\n", sampleMean, sampleSize, sampleVariance, tmu); fflush(stdout);
        errorInDistributionArgs= 0;
        delete nd;
        nd= new Studenttdist(sampleSize-1, 0, 1);

        double ts= (sampleMean - tmu)/sqrt(sampleSize*sampleVariance/(sampleSize-1))*sqrt(sampleSize);
        double threshold= (1 - significance)/2;

        //printf("ostinner: %f %f %f\n", ts, threshold, nd->invcdf(threshold));

        float tmp= fabs(nd->invcdf(threshold));

        if ( errorInDistributionArgs )
            return resultOnError;

        if ( fabs(ts) < tmp )
            return 1;
        else
            return 0;
    }

    template<typename T>
    class TwoSampleTTest: public StatisticalTest<T>
    {
    public:
        using StatisticalTest<T>::significance;

        TwoSampleTTest(double ssig= 0.95);

        TwoSampleTTest(const TwoSampleTTest& o);

        ~TwoSampleTTest();

        double test(Vector<T>& sample1, Vector<T>& sample2);

        double test(double sample1Mean, double sample1Size, double sample1Variance, double sample2Mean, double sample2Size, double sample2Variance);

        OneSampleTTest<T>* ostt;
        Vector<T> tmp;
    };

    template<typename T>
    TwoSampleTTest<T>::TwoSampleTTest(double ssig)
        : StatisticalTest<T>(ssig)
    {
        ostt= new OneSampleTTest<T>(ssig);
    }

    template<typename T>
    TwoSampleTTest<T>::TwoSampleTTest(const TwoSampleTTest& o)
        : StatisticalTest<T>(o)
    {
    }

    template<typename T>
    TwoSampleTTest<T>::~TwoSampleTTest()
    {
    }

    template<typename T>
    double TwoSampleTTest<T>::test(Vector<T>& sample1, Vector<T>& sample2)
    {
        tmp.resize(MIN(sample1.size(), sample2.size()));

        double tmp1, tmp2;

        if ( sample1.size() < sample2.size() )
        {
            tmp.resize(sample1.size());
            double n1pn2= sqrt(float(sample1.size())/sample2.size());
            double n1mn2= 1./sqrt(float(sample1.size()*sample2.size()));
            double invn2= 1./float(sample2.size());

            tmp1= tmp2= 0;
            for ( unsigned int j= 0; j < sample1.size(); ++j )
                tmp1+= sample2(j);
            for ( unsigned int j= 0; j < sample2.size(); ++j )
                tmp2+= sample2(j);

            double c= n1mn2*tmp1 - invn2*tmp2;

            for ( unsigned int i= 0; i < sample1.size(); ++i )
            {
                tmp(i)= sample1(i) - n1pn2*sample2(i) + c;
            }

            return ostt->test(tmp, 0);
        }
        else
        {
            tmp.resize(sample2.size());
            double n1pn2= sqrt(float(sample2.size())/sample1.size());
            double n1mn2= 1./sqrt(float(sample2.size()*sample1.size()));
            double invn2= 1./float(sample1.size());

            tmp1= tmp2= 0;
            for ( unsigned int j= 0; j < sample2.size(); ++j )
                tmp1+= sample1(j);
            for ( unsigned int j= 0; j < sample1.size(); ++j )
                tmp2+= sample1(j);

            double c= n1mn2*tmp1 - invn2*tmp2;

            for ( unsigned int i= 0; i < sample2.size(); ++i )
            {
                tmp(i)= sample2(i) - n1pn2*sample1(i) + c;
            }

            return ostt->test(tmp, 0);
        }

    }

    template<typename T>
    double TwoSampleTTest<T>::test(double sample1Mean, double sample1Size, double sample1Variance, double sample2Mean, double sample2Size, double sample2Variance)
    {
        return 0;
    }

    template<typename T>
    class FTest: public StatisticalTest<T>
    {
    public:
        using StatisticalTest<T>::significance;

        FTest(double ssig= 0.95);

        FTest(const FTest& o);

        ~FTest();

        double test(Vector<T>& sample1, Vector<T>& sample2);

        double test(double sample1Size, double sample1Variance, double sample2Size, double sample2Variance);

        Fdist* fdist1;
    };

    template<typename T>
    FTest<T>::FTest(double ssig)
        : StatisticalTest<T>(ssig)
    {
    }

    template<typename T>
    FTest<T>::FTest(const FTest& o)
        : StatisticalTest<T>(o)
    {
    }

    template<typename T>
    FTest<T>::~FTest()
    {
    }

    template<typename T>
    double FTest<T>::test(Vector<T>& sample1, Vector<T>& sample2)
    {
        return this->test(sample1.size(), sample1.getVariance(), sample2.size(), sample2.getVariance());
    }

    template<typename T>
    double FTest<T>::test(double sample1Size, double sample1Variance, double sample2Size, double sample2Variance)
    {
        //double s1star= sample1Variance*sample1Size/(sample1Size-1);
        //double s2star= sample2Variance*sample2Size/(sample2Size-1);
        errorInDistributionArgs= 0;
        double s1star= sample1Variance;
        double s2star= sample2Variance;

        if ( s1star > s2star )
        {
            //printf("F %f %f\n", sample1Size-1, sample2Size-1);
            fdist1= new Fdist(sample1Size-1, sample2Size-1);
            //double threshold= significance;
            //printf("%f %f %f\n", s1star/s2star, threshold, fdist1->invcdf(significance));

            float tmp= fdist1->invcdf(significance);

            if ( errorInDistributionArgs )
                return resultOnError;

            if ( s1star/s2star < tmp )
                return 1.0;
            else
                return 0.0;
        }
        else
        {
            fdist1= new Fdist(sample2Size-1, sample1Size-1);
            double threshold= significance;

            float tmp= fdist1->invcdf(threshold);

            if ( errorInDistributionArgs )
                return resultOnError;

            //printf("%f %f %f\n", s2star/s1star, threshold, fdist1->invcdf(threshold));
            if ( s2star/s1star < tmp )
                return 1.0;
            else
                return 0.0;
        }

        return 0;
    }

    template<typename T>
    class ChiSquareTest: public StatisticalTest<T>
    {
    public:
        using StatisticalTest<T>::significance;

        ChiSquareTest(double ssig= 0.95);

        ChiSquareTest(const ChiSquareTest& o);

        ~ChiSquareTest();

        double test(Vector<T>& sample1, double tsigma);

        double test(double sampleSize, double sampleVariance, double tsigma);

        Chisquaredist* csd;
    };

    template<typename T>
    ChiSquareTest<T>::ChiSquareTest(double ssig)
        : StatisticalTest<T>(ssig)
    {
    }

    template<typename T>
    ChiSquareTest<T>::ChiSquareTest(const ChiSquareTest& o)
        : StatisticalTest<T>(o)
    {
    }

    template<typename T>
    ChiSquareTest<T>::~ChiSquareTest()
    {
    }

    template<typename T>
    double ChiSquareTest<T>::test(Vector<T>& sample1, double tsigma)
    {
        return this->test(sample1.size(), sample1.getVariance(), tsigma);
    }

    template<typename T>
    double ChiSquareTest<T>::test(double sampleSize, double sampleVariance, double tsigma)
    {
        errorInDistributionArgs= 0;
        csd= new Chisquaredist(sampleSize-1);
        double sstar= sampleVariance*sampleSize/(sampleSize-1);
        double ts= sampleVariance*sampleSize/tsigma/tsigma;

        printf("ts: %f\n", ts);
        double thresholdLower= (1. - significance)/2.;
        double thresholdUpper= 1 - (1. - significance)/2.;

        printf("ths: %f %f\n", thresholdLower, thresholdUpper);
        printf("p: %f\n", csd->cdf(csd->invcdf(thresholdUpper)) - csd->cdf(csd->invcdf(thresholdLower)));

        float a= csd->invcdf(thresholdLower);
        float b= csd->invcdf(thresholdUpper);

        if ( errorInDistributionArgs )
            return resultOnError;

        if ( a <= ts && ts <= b )
        {
            return 1;
        }

        return 0;
    }
}

#endif // STATISTICALTESTS_H

#ifndef HIGHERORDERSTATISTICS_H
#define HIGHERORDERSTATISTICS_H

#include <openipLL/FourierFilter1.h>
#include <openipLL/FourierFilter2.h>
#include <openipDS/Transform2s.h>
#include <openipDS/StatisticalFeature2s.h>
#include <openipLL/HigherOrderStatistics1.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class BicoherenceEstimator1: public Transform2<INPUT, OUTPUT>
    {
    public:
        BicoherenceEstimator1();

        BicoherenceEstimator1(const BicoherenceEstimator1& b);

        ~BicoherenceEstimator1();

        void apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi= NULL, Image<unsigned char> *support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    class BicoherenceEstimator2: public Transform2<INPUT, OUTPUT>
    {
    public:
        BicoherenceEstimator2();

        BicoherenceEstimator2(const BicoherenceEstimator2& b);

        ~BicoherenceEstimator2();

        void apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi= NULL, Image<unsigned char> *support= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    BicoherenceEstimator1<INPUT, OUTPUT>::BicoherenceEstimator1()
        : Transform2<INPUT, OUTPUT>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    BicoherenceEstimator1<INPUT, OUTPUT>::BicoherenceEstimator1(const BicoherenceEstimator1& b)
        : Transform2<INPUT, OUTPUT>(b)
    {
    }

    template<typename INPUT, typename OUTPUT>
    BicoherenceEstimator1<INPUT, OUTPUT>::~BicoherenceEstimator1()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void BicoherenceEstimator1<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
    {
        VectorBispectrumEstimator1<double, double> be;
        Image<std::complex<double> > bispectrum;
        Image<double> a;
        Image<double> b;

        bispectrum.resizeImage(input.columns, input.columns);
        a.resizeImage(input.columns, input.columns);
        b.resizeImage(input.columns, input.columns);

        output.resizeImage(input.columns, input.columns);

        bispectrum= 0;
        a= 0;
        b= 0;
        output= 0;

        float m;
        m= input.getMean();
        input-= m;

        Vector<double> v;
        v.resize(input.columns);

        for ( int i= 0; i < input.rows; i++ )
        {
            for ( int j= 0; j < input.columns; ++j )
                v(j)= input(i,j);
            be.apply(v, bispectrum, &a, &b);
        }

        bispectrum/= std::complex<double>(input.rows, 0);
        a/= input.rows;
        b/= input.rows;

        Image<double> tmp;
        tmp.resizeImage(a.columns, b.columns);

        for ( int i= 0; i < bispectrum.rows; ++i )
            for ( int j= 0; j < bispectrum.columns; ++j )
            {
                if ( a(i,j) * b(i,j) > 0 )
                    output(i,j)= norm(bispectrum(i,j))/sqrt(a(i,j)*b(i,j));
                else
                    output(i,j)= 0;
                tmp(i,j)= norm(bispectrum(i,j));
            }

        writeImage("tmp.jpg", tmp);
        OUTPUT min, max;
        float mean, var;
        output.getMinMax(min, max);
        output.getMeanAndVariance(mean, var);
        printf("%f %f %f %f\n", min, max, mean, sqrt(var));
    }

    template<typename INPUT, typename OUTPUT>
    BicoherenceEstimator2<INPUT, OUTPUT>::BicoherenceEstimator2()
        : Transform2<INPUT, OUTPUT>()
    {
    }

    template<typename INPUT, typename OUTPUT>
    BicoherenceEstimator2<INPUT, OUTPUT>::BicoherenceEstimator2(const BicoherenceEstimator2& b)
        : Transform2<INPUT, OUTPUT>(b)
    {
    }

    template<typename INPUT, typename OUTPUT>
    BicoherenceEstimator2<INPUT, OUTPUT>::~BicoherenceEstimator2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void BicoherenceEstimator2<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
    {
        VectorBispectrumEstimator2<double, double> be;
        Image<double > bispectrum;
        Image<double> a;
        Image<double> b;

        bispectrum.resizeImage(input.columns, input.columns);
        a.resizeImage(input.columns, input.columns);
        b.resizeImage(input.columns, input.columns);

        output.resizeImage(input.columns, input.columns);

        bispectrum= 0;
        a= 0;
        b= 0;
        output= 0;

        float m;
        m= input.getMean();
        input-= m;

        Vector<double> v;
        v.resize(input.columns);

        for ( int i= 0; i < input.rows; i+= 10 )
        {
            for ( int j= 0; j < input.columns; ++j )
                v(j)= input(i,j);
            //be.apply(v, bispectrum, &a, &b);
        }

        bispectrum/= input.rows/10;
        a/= input.rows/10;
        b/= input.rows/10;

        Image<double> tmp;
        tmp.resizeImage(a.columns, b.columns);

        output= bispectrum;
        /*for ( int i= 0; i < bispectrum.rows; ++i )
            for ( int j= 0; j < bispectrum.columns; ++j )
            {
                if ( a(i,j) * b(i,j) > 0 )
                    output(i,j)= norm(bispectrum(i,j))/sqrt(a(i,j)*b(i,j));
                else
                    output(i,j)= 0;
                tmp(i,j)= norm(bispectrum(i,j));
            }

        writeImage("tmp.jpg", tmp);*/
        OUTPUT min, max;
        float mean, var;
        output.getMinMax(min, max);
        output.getMeanAndVariance(mean, var);
        printf("%f %f %f %f\n", min, max, mean, sqrt(var));
    }
}

#endif // HIGHERORDERSTATISTICS_H

#ifndef HIGHERORDERSTATISTICS1_H
#define HIGHERORDERSTATISTICS1_H

#include <openipLL/FourierFilter1.h>
#include <openipDS/Vector.h>
#include <openipDS/Image.h>
#include <openipDS/mathFunctions.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class VectorBispectrumEstimator1
    {
    public:
        VectorBispectrumEstimator1();

        VectorBispectrumEstimator1(const VectorBispectrumEstimator1& b);

        ~VectorBispectrumEstimator1();

        void apply(Vector<INPUT>& input, Image<std::complex<OUTPUT> >& output, Image<OUTPUT>* a= NULL, Image<OUTPUT>* b= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    VectorBispectrumEstimator1<INPUT, OUTPUT>::VectorBispectrumEstimator1()
    {
    }

    template<typename INPUT, typename OUTPUT>
    VectorBispectrumEstimator1<INPUT, OUTPUT>::VectorBispectrumEstimator1(const VectorBispectrumEstimator1& b)
    {
    }

    template<typename INPUT, typename OUTPUT>
    VectorBispectrumEstimator1<INPUT, OUTPUT>::~VectorBispectrumEstimator1()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void VectorBispectrumEstimator1<INPUT, OUTPUT>::apply(Vector<INPUT>& input, Image<std::complex<OUTPUT> >& output, Image<OUTPUT>* a, Image<OUTPUT>* b)
    {
        FourierTransformer1 trans(input.size());
        Vector<std::complex<double> > ft;
        ft.resize(input.size());
        trans.transform(input.data(), ft.data());

        for ( unsigned int i= 0; i < ft.size(); ++i )
            std::cout << ft(i) << " ";
        std::cout << std::endl;

        // based on Hinich
        for ( unsigned int j= 0; j < input.size()/2; ++j )
            for ( unsigned int k= 0; k <= j; ++k )
            {
                if ( 2*j + k <= input.size() )
                {
                    output(j,k)+= ft(j)*ft(k)*conj(ft(j+k));
                    if ( a )
                        (*a)(j,k)+= norm(ft(j)*ft(k))*norm(ft(j)*ft(k));
                    if ( b )
                        (*b)(j,k)+= norm(ft(j+k))*norm(ft(j+k));
                }
            }
    }

    template<typename INPUT, typename OUTPUT>
    class ThirdMomentEstimator
    {
    public:
        ThirdMomentEstimator();

        ThirdMomentEstimator(const ThirdMomentEstimator& b);

        ~ThirdMomentEstimator();

        void apply(Vector<INPUT>& input, Image<OUTPUT >& output, Image<OUTPUT>* a= NULL, Image<OUTPUT>* b= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    ThirdMomentEstimator<INPUT, OUTPUT>::ThirdMomentEstimator()
    {
    }

    template<typename INPUT, typename OUTPUT>
    ThirdMomentEstimator<INPUT, OUTPUT>::ThirdMomentEstimator(const ThirdMomentEstimator& b)
    {
    }

    template<typename INPUT, typename OUTPUT>
    ThirdMomentEstimator<INPUT, OUTPUT>::~ThirdMomentEstimator()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ThirdMomentEstimator<INPUT, OUTPUT>::apply(Vector<INPUT>& input, Image<OUTPUT>& output, Image<OUTPUT>* a, Image<OUTPUT>* b)
    {
        float muhat= 0;
        float tmp= 0;

        int phi, tau;

        output.resizeImage(input.size(), input.size());
        int M= input.size()/2;

        for ( int r= -M; r < M; ++r )
            for ( int s= -M; s < M; ++s )
            {
                phi= MAX(0, MAX(r,s));
                tau= MIN(0, MIN(r,s));

                if ( 0 <= r && r <= s && s <= M && r+s > 0 )
                {
                    tmp= 0;
                    for ( int i= 1 + abs(tau); i <= input.size() - phi; ++i )
                    {
                        int i1;
                        int i2;
                        i1= i + r;
                        i2= i + s;

                        if ( i1 < 0 ) i1+= input.size();
                        if ( i2 < 0 ) i2+= input.size();

                        tmp+= input(i)*input(i1)*input(i2);
                    }
                    tmp/= input.size();
                    output(r + M, s + M)+= tmp;
                }
            }
    }

    template<typename INPUT, typename OUTPUT>
    class VectorBispectrumEstimator2
    {
    public:
        VectorBispectrumEstimator2();

        VectorBispectrumEstimator2(const VectorBispectrumEstimator2& b);

        ~VectorBispectrumEstimator2();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<OUTPUT>* a= NULL, Image<OUTPUT>* b= NULL);
    };

    template<typename INPUT, typename OUTPUT>
    VectorBispectrumEstimator2<INPUT, OUTPUT>::VectorBispectrumEstimator2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    VectorBispectrumEstimator2<INPUT, OUTPUT>::VectorBispectrumEstimator2(const VectorBispectrumEstimator2& b)
    {
    }

    template<typename INPUT, typename OUTPUT>
    VectorBispectrumEstimator2<INPUT, OUTPUT>::~VectorBispectrumEstimator2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void VectorBispectrumEstimator2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<OUTPUT>* a, Image<OUTPUT>* b)
    {
        int M= input.columns/2;

        float tmp;
        for ( int r= 0; r < output.rows; ++r )
            for ( int s= 0; s < output.columns; ++s )
                for ( int rh= -M; rh < M; ++rh )
                    for ( int sh= -M; sh < M; ++sh )
                    {
                        //int i1= ;
                        int i2;

                        //tmp+= input()
                    }
    }
}

#endif // HIGHERORDERSTATISTICS1_H

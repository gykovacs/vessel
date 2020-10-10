#ifndef ANISOTROPICDIFFUSIONPERONAMALIK_H
#define ANISOTROPICDIFFUSIONPERONAMALIK_H

#include <openipDS/Transform2.h>
#include <openipDS/StructuringElement2s.h>
#include <openipDS/GaborFilter2.h>
#include <openipDS/CurvedGaborFilter2.h>
#include <openipLL/thinning.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class AnisotropicDiffusionPeronaMalik: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;

        AnisotropicDiffusionPeronaMalik(float K= 1, float lambda= 1, int steps= 10);

        AnisotropicDiffusionPeronaMalik(const AnisotropicDiffusionPeronaMalik& adpm);

        ~AnisotropicDiffusionPeronaMalik();

        float g(float x);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int steps;
        float lambda;
        float K;
    };

    template<typename INPUT, typename OUTPUT>
    AnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>::AnisotropicDiffusionPeronaMalik(float K, float lambda, int steps)
        : Transform2<INPUT, OUTPUT>()
    {
        this->steps= steps;
        this->lambda= lambda;
        this->K= K;

        stringstream ss;
        ss << "AnisotropicDiffusionPeronaMalik " << K << " " << lambda << " " << steps;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    AnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>::AnisotropicDiffusionPeronaMalik(const AnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>& adpm)
        : Transform2<INPUT, OUTPUT>(adpm)
    {
        this->steps= adpm.steps;
        this->lambda= adpm.lambda;
        this->K= adpm.K;
    }

    template<typename INPUT, typename OUTPUT>
    AnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>::~AnisotropicDiffusionPeronaMalik()
    {
    }

    template<typename INPUT, typename OUTPUT>
    float AnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>::g(float x)
    {
        //return 1.0/(1.0 + x*x/K*K);
        return exp(-x*x/K*K);
    }

    template<typename INPUT, typename OUTPUT>
    void AnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        printf("applying...\n"); fflush(stdout);
        //int changes= 0;
        int step= 0;

        output= input;
        StructuringElementDisk sed(1, input.columns);
        sed.updateStride(input.columns);

        Image<OUTPUT> tmp;
        tmp.resizeImage(output);
        tmp= input;

        //float mg;
        float magn;
        do
        {
            for ( unsigned int i= - sed.getMin(); i < tmp.n - sed.getMax(); ++i )
            {
                magn= 0;
                //mg= 0;
                for ( unsigned int j= 0; j < sed.size(); ++j )
                {
                    if ( !support || (*support)(i + sed(j)))
                    {
                        magn+= g(tmp(i + sed(j)) - tmp(i))*(tmp(i + sed(j)) - tmp(i));
                        //magn+= g(tmp(i + sed(j)) - tmp(i))*(tmp(i + sed(j)));
                        //mg+= g(tmp(i + sed(j)) - tmp(i));
                    }
                    //magn/= mg;
                }
                if ( !roi || (*roi)(i) )
                    output(i)+= lambda/sed.size()*magn;
            }

            tmp= output;

            ++step;
            printf("."); fflush(stdout);
        }while(step < steps);
        output.normalize(0,1,roi);
    }

    template<typename INPUT, typename OUTPUT>
    class MaskedAnisotropicDiffusionPeronaMalik: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::descriptor;
        using Transform2<INPUT, OUTPUT>::apply;

        MaskedAnisotropicDiffusionPeronaMalik(float K= 1, float lambda= 1, int steps= 10);

        MaskedAnisotropicDiffusionPeronaMalik(const MaskedAnisotropicDiffusionPeronaMalik& adpm);

        ~MaskedAnisotropicDiffusionPeronaMalik();

        float g(float x);

        int neighbor(float t);

        virtual void apply(Image<INPUT>& input, Image<unsigned char>& mask, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        int steps;
        float lambda;
        float K;
    };

    template<typename INPUT, typename OUTPUT>
    MaskedAnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>::MaskedAnisotropicDiffusionPeronaMalik(float K, float lambda, int steps)
        : Transform2<INPUT, OUTPUT>()
    {
        this->steps= steps;
        this->lambda= lambda;
        this->K= K;

        stringstream ss;
        ss << "AnisotropicDiffusionPeronaMalik " << K << " " << lambda << " " << steps;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    MaskedAnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>::MaskedAnisotropicDiffusionPeronaMalik(const MaskedAnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>& adpm)
        : Transform2<INPUT, OUTPUT>(adpm)
    {
        this->steps= adpm.steps;
        this->lambda= adpm.lambda;
        this->K= adpm.K;
    }

    template<typename INPUT, typename OUTPUT>
    MaskedAnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>::~MaskedAnisotropicDiffusionPeronaMalik()
    {
    }

    template<typename INPUT, typename OUTPUT>
    float MaskedAnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>::g(float x)
    {
        //return 1.0/(1.0 + x*x/K*K);
        return exp(-x*x/K*K);
    }

    template<typename INPUT, typename OUTPUT>
    int MaskedAnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>::neighbor(float t)
    {
        if ( t >= 7*M_PI/8 || t < M_PI/8 )
            return 0;
        if ( t >= M_PI/8 && t < 3*M_PI/8 )
            return 1;
        if ( t >= 3*M_PI/8 && t < 5*M_PI/8 )
            return 2;
        if ( t >= 5*M_PI/8 && t < 7*M_PI/8 )
            return 3;
        return 0;
    }

    template<typename INPUT, typename OUTPUT>
    void MaskedAnisotropicDiffusionPeronaMalik<INPUT, OUTPUT>::apply(Image<INPUT> &input, Image<unsigned char>& mask, Image<OUTPUT> &output, Image<unsigned char> *roi, Image<unsigned char> *support)
    {
        printf("applying...\n"); fflush(stdout);

        int step= 0;

        output= input;
        StructuringElementDisk sed(1, input.columns);
        sed.updateStride(input.columns);

        printf("ccc"); fflush(stdout);
        MatchedCurvedGaborFilter2RMagnitude<float, float> mcgf(0, 1, 0, 1.4, 1, 1.4, 0, 0.05, 3.14, 5, 3.14, 0.5);
        printf("ddd"); fflush(stdout);

        Image<float> magn;
        Image<int> label;
        magn.resizeImage(input);
        label.resizeImage(input);

        printf("aaa"); fflush(stdout);
        mcgf.apply(input, magn, label, roi, support);
        printf("%f %f\n", magn.getMin(), magn.getMax());
        magn.normalize(0, 255);
        writeImage("magn.jpg", magn);
        printf("bbb"); fflush(stdout);

        Image<unsigned char> thinned;
        thinned.resizeImage(input);
        thinningCWSI(mask, thinned);

        Image<float> thetas;
        thetas.resizeImage(input);

        for ( unsigned int i= 0; i < label.n; ++i )
            thetas(i)= label(i)*0.05;

        Image<float> thetas2;
        thetas2.resizeImage(input);
        thetas2= thetas;

        int maxIdx;
        for ( int i= -sed.getMin(); i < int(mask.n) - sed.getMax(); ++i )
        {
            maxIdx= 0;
            for ( unsigned int j= 0; j < sed.size(); ++j )
            {
                if ( magn(i + sed(j)) > magn(i + sed(maxIdx)) )
                {
                    maxIdx= j;
                }
            }
            thetas2(i)= thetas(i + maxIdx);
        }
        thetas= thetas2;

        output= mask;

        //float magn;


        float t;

        int begin= -sed.getMin();
        int end= mask.n - sed.getMax();

        printf("%d %d\n", begin, end);

        do
        {
            for ( int i= begin; i < end; ++i )
            {
                if ( (mask)(i) )
                {
                    //printf(","); fflush(stdout);
                    t= thetas(i);

                    int a= neighbor(t);
                    if ( a == 2 )
                    {
                        output(i + 1)= 255;
                        output(i - 1)= 255;
                    }
                    /*else if ( a == 1 )
                    {
                        output(i - output.columns + 1)= 255;
                        output(i + output.columns - 1)= 255;
                    }*/
                    else if ( a == 0 )
                    {
                        output(i - output.columns)= 255;
                        output(i + output.columns)= 255;
                    }
                    /*else if ( a == 3 )
                    {
                        output(i - output.columns - 1)= 255;
                        output(i + output.columns + 1)= 255;
                    }*/
                }
            }

            mask= output;

            ++step;
            printf("."); fflush(stdout);
        }while(step < steps);
    }
}

#endif // ANISOTROPICDIFFUSIONPERONAMALIK_H

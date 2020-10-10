#include <openipDS/Histogram.h>
#include <openipDS/StructuringElement2s.h>
#include <openipDS/Filter2s.h>
#include <openipDS/RunningStructuringElements.h>

#include <openipLL/imageCorrection.h>
#include <openipLL/imageIO.h>
#include <openipLL/ctmf.h>
#include <openipLL/morphology.h>

#include <iostream>

using namespace std;

namespace openip
{
    /*GammaFunction::GammaFunction(Vector<float>& v)
    : RealFunction()
    {
        this->v= v;
    }

    GammaFunction::GammaFunction(const GammaFunction& g)
    : RealFunction(g)
    {
        this->v= g.v;
    }

    GammaFunction::~GammaFunction()
    {
    }

    float GammaFunction::value(float x)
    {
        float res= 0.0f;
        float mean= 0.0f;

        for ( int i= 0; i < v.size(); ++i )
            mean+= pow(v(i),x);
        mean/=v.size();
        //mean*= mean;

        printf("mean: %f\n", mean);
        //for ( int i= 0; i < v.size(); ++i )
        //    res+= pow(v(i),x) * pow(v(i),x);


        //res/=v.size();
        //res= res-mean;

        //return res;
        for ( int i= 0; i < v.size(); ++i )
            res+= (mean - pow(v(i),x))*(mean - pow(v(i),x));
        return res/v.size();
    }

    float GammaFunction::derivative(float x)
    {
        double a= 0.0f;
        double b= 0.0f;
        double c= 0.0f;

        int n= 0;
        for ( int i= 0; i < v.size(); ++i )
        {
            //if ( v(i) > 0 )
            {
                a+= 2 * pow(v(i),2*x) *log(v(i));
                b+= pow(v(i),x)*log(v(i));
                c+= pow(v(i),x);
                ++n;
            }
        }

        return a/v.size()-2*c*b/(v.size()*v.size());
    }

    float GammaFunction::getGamma()
    {
        float E= 0, F= 0, G= 0, H= 0, I= 0, J= 0, L= 0, K= 0, D= 0;
        float w, l;
        float n= v.size();

        for ( int i= 0; i < v.size(); ++i )
        {
            w= v(i);
            l= log(v(i));

            E+= l;
            D+= l*l;
            K+= l*l*l;
            L+= l*l*l*l;
        }

        //return -2.0/3.0*(-D*n + E*E)/(-K*n + E*D);
        float res= -1.0/2.0;
        //res*= 1.0/(-7*L*n + 4*E*K + 3*D*D);
        //res*= (-9*K*n + 9*E*D + sqrt(9*E*E*D*D-66*E*D*K*n + 81*K*K*n*n - 168*L*n*n*D + 168*L*n*E*E - 96*E*E*E*K+72*D*D*D*n));
        //return 9*E*E*D*D-66*E*D*K*n + 81*K*K*n*n - 168*L*n*n*D + 168*L*n*E*E - 96*E*E*E*K+72*D*D*D*n;
        //return -(log(log(v(0))/log(v(1))))/(log(v(0))-log(v(1)));
    //}

    //float GammaFunction::secondDerivative(float x)
    //{
      //  return 0.0;
    //}*/

    DGammaFunction::DGammaFunction(Vector<float>& v)
    : RealFunction()
    {
        this->v= v;
    }

    DGammaFunction::DGammaFunction(const DGammaFunction& g)
    : RealFunction(g)
    {
        this->v= g.v;
    }

    DGammaFunction::~DGammaFunction()
    {
    }

    double DGammaFunction::value(double x)
    {
        float a= 0.0f;
        float b= 0.0f;
        float c= 0.0f;

        int n= 0;
        for ( unsigned int i= 0; i < v.size(); ++i )
        {
            //if ( v(i) > 0 )
            {
                a+= 2 * pow(v(i),x) * pow(v(i),x)*log(v(i));
                b+= pow(v(i),x)*log(v(i));
                c+= pow(v(i),x);
                ++n;
            }
        }

        return a/v.size()-2*c*b/(v.size()*v.size());
        
    }

    double DGammaFunction::derivative(double)
    {
        return 0.0;
    }

    double DGammaFunction::secondDerivative(double)
    {
        return 0.0;
    }

    void gammaCorrection(Vector<float>* input, Vector<float>* output, Vector<float>* mask, float gamma)
    {
        if ( mask == NULL )
            //#pragma omp parallel for
            for ( unsigned int i= 0; i < input->size(); ++i )
                (*output)(i)= pow(((float)(*input)(i))/*/255.0*/, gamma)/**255*/;
        else
            //#pragma omp parallel for
            for ( unsigned int i= 0; i < input->size(); ++i )
                if ( (*mask)(i) > 0 )
                    (*output)(i)= pow(((float)(*input)(i))/*/255.0*/, gamma)/**255*/;
    }

    /**
     * sigmoid function, evaluates the function at f
     * @param f the point to evaluate the function at
     * @param mean method parameter
     * @param var method parameter
     * @return the function value
     */
    float sigm(float f, float mean, float var)
    {
        return 1.0/(1 + exp((mean - f)/sqrt(var)));
    }
}

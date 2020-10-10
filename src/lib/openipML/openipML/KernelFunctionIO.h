/* 
 * File:   KernelFunctionIO.h
 * Author: gykovacs
 *
 * Created on 2011. február 23., 0:02
 */

#ifndef KERNELFUNCTIONIO_H
#define	KERNELFUNCTIONIO_H

#include <iostream>

#include <openipML/SimilarityFunction.h>
#include <openipML/KernelFunctions.h>
#include <openipML/CorrelationFunctions.h>
#include <openipML/KernelFunctions.h>

namespace openip
{
    template<typename T>
    class KernelFunctionIO
    {
    public:
        KernelFunctionIO();

        KernelFunctionIO(const KernelFunctionIO& b);

        ~KernelFunctionIO();

        int saveKernelFunctionToFile(KernelFunction<T>* bf, std::ofstream& file);
	
	//int saveKernelFunctionToFile(KernelFunction<T>* bf, std::ofstream& file)= 0;

        KernelFunction<T>* openKernelFunctionFromFile(std::ifstream& file);

        KernelFunction<T>* createKernelFunctionFromString(std::string str);
    };

    template<typename T>
    KernelFunctionIO<T>::KernelFunctionIO()
    {
    }

    template<typename T>
    KernelFunctionIO<T>::KernelFunctionIO(const KernelFunctionIO& b)
    {
    }

    template<typename T>
    KernelFunctionIO<T>::~KernelFunctionIO()
    {
    }

    template<typename T>
    int KernelFunctionIO<T>::saveKernelFunctionToFile(KernelFunction<T>* df, std::ofstream& file)
    {
        file << df->descriptor << std::endl;
        return 0;
    }

    template<typename T>
    KernelFunction<T>* KernelFunctionIO<T>::openKernelFunctionFromFile(std::ifstream& file)
    {
        std::string tmp;
        getline(file, tmp);
        //cout << "kernel " << tmp << std::endl;
        char kernelName[60];
        std::stringstream ss(tmp);
        ss >> kernelName;

        double a, b, c;

        if ( strcmp(kernelName, "LinearKernel") == 0 )
        {
            ss >> a;
            return new LinearKernel<T>(a);
        }
        else if ( strcmp(kernelName, "DotProductKernel") == 0 )
        {
            return new DotProductKernel<T>();
        }
        else if ( strcmp(kernelName, "CenteredDotProductKernel") == 0 )
        {
            return new CenteredDotProductKernel<T>();
        }
        else if ( strcmp(kernelName, "PolynomialKernel") == 0 )
        {
            ss >> a; ss >> b; ss >> c;
            return new PolynomialKernel<T>(a, b, c);
        }
        else if ( strcmp(kernelName, "HPolynomialKernel") == 0 )
        {
            ss >> a;
            return new HPolynomialKernel<T>(a);
        }
        else if ( strcmp(kernelName, "CenteredPolynomialKernel1") == 0 )
        {
            ss >> a; ss >> b; ss >> c;
            return new CenteredPolynomialKernel1<T>(a, b, c);
        }
        else if ( strcmp(kernelName, "CenteredPolynomialKernel2") == 0 )
        {
            ss >> a; ss >> b; ss >> c;
            return new CenteredPolynomialKernel2<T>(a, b, c);
        }
        else if ( strcmp(kernelName, "CenteredHPolynoialKernel1") == 0 )
        {
            ss >> a;
            return new CenteredHPolynomialKernel1<T>(a);
        }
        else if ( strcmp(kernelName, "CenteredHPolynomialKernel2") == 0 )
        {
            ss >> a;
            return new CenteredHPolynomialKernel2<T>(a);
        }
        else if ( strcmp(kernelName, "GaussianKernel") == 0 )
        {
            ss >> a;
            return new GaussianKernel<T>(a);
        }
        else if ( strcmp(kernelName, "ExponentialKernel") == 0 )
        {
            ss >> a;
            return new ExponentialKernel<T>(a);
        }
        else if ( strcmp(kernelName, "LaplacianKernel") == 0 )
        {
            ss >> a;
            return new LaplacianKernel<T>(a);
        }
        else if ( strcmp(kernelName, "ANOVAKernel") == 0 )
        {
            ss >> a; ss >> b; ss >> c;
            return new ANOVAKernel<T>(a, b, c);
        }
        else if ( strcmp(kernelName, "HyperbolicTangentKernel") == 0 )
        {
            ss >> a; ss >> b;
            return new HyperbolicTangentKernel<T>(a, b);
        }
        else if ( strcmp(kernelName, "RationalQuadraticKernel") == 0 )
        {
            ss >> a;
            return new RationalQuadraticKernel<T>(a);
        }
        else if ( strcmp(kernelName, "MultiquadraticKernel") == 0 )
        {
            ss >> a;
            return new MultiquadraticKernel<T>(a);
        }
        else if ( strcmp(kernelName, "InverseMultiquadraticKernel") == 0 )
        {
            ss >> a;
            return new InverseMultiquadraticKernel<T>(a);
        }
        else if ( strcmp(kernelName, "CircularKernel") == 0 )
        {
            ss >> a;
            return new CircularKernel<T>(a);
        }
        else if ( strcmp(kernelName, "SphericalKernel") == 0 )
        {
            ss >> a;
            return new SphericalKernel<T>(a);
        }
        else if ( strcmp(kernelName, "WaveKernel") == 0 )
        {
            ss >> a;
            return new WaveKernel<T>(a);
        }
        else if ( strcmp(kernelName, "PowerKernel") == 0 )
        {
            ss >> a;
            return new PowerKernel<T>(a);
        }
        else if ( strcmp(kernelName, "LogKernel") == 0 )
        {
            ss >> a;
            return new LogKernel<T>(a);
        }
        else if ( strcmp(kernelName, "SplineKernel") == 0 )
        {
            return new SplineKernel<T>();
        }
        else if ( strcmp(kernelName, "BSplineKernel") == 0 )
        {
            ss >> a;
            return new BSplineKernel<T>(a);
        }
        else if ( strcmp(kernelName, "CauchyKernel") == 0 )
        {
            ss >> a;
            return new CauchyKernel<T>(a);
        }
        else if ( strcmp(kernelName, "ChiSquareKernel") == 0 )
        {
            return new ChiSquareKernel<T>();
        }
        else if ( strcmp(kernelName, "HistogramIntersectionKernel") == 0 )
        {
            return new HistogramIntersectionKernel<T>();
        }
        else if ( strcmp(kernelName, "GeneralizedHistogramIntersectionKernel") == 0 )
        {
            ss >> a; ss >> b;
            return new GeneralizedHistogramIntersectionKernel<T>(a, b);
        }
        else if ( strcmp(kernelName, "GeneralizedTStudentKernel") == 0 )
        {
            ss >> a;
            return new GeneralizedTStudentKernel<T>(a);
        }
        else if ( strcmp(kernelName, "WaveletKernel") == 0 )
        {
            ss >> a; ss >> b;
            return new WaveletKernel<T>(a, b);
        }

        return NULL;
    }

    template<typename T>
    KernelFunction<T>* KernelFunctionIO<T>::createKernelFunctionFromString(std::string str)
    {
        stringstream ss;
        ss.str(str);
        string name;
        ss >> name;
        float a, b, c;

        if ( str.find("DotProductKernel") == 0 )
        {
            return new DotProductKernel<T>();
        }
        if ( str.find("CenteredDotProductKernel") == 0 )
        {
            return new CenteredDotProductKernel<T>();
        }
        if ( str.find("PolynomialKernel") == 0 )
        {
            ss >> a;
            ss >> b;
            ss >> c;
            return new PolynomialKernel<T>(a, b, c);
        }
        if ( str.find("CenteredPolynomialKernel1") == 0 )
        {
            ss >> a;
            ss >> b;
            ss >> c;
            return new CenteredPolynomialKernel1<T>(a, b, c);
        }
        if ( str.find("CenteredPolynomialKernel2") == 0 )
        {
            ss >> a;
            ss >> b;
            ss >> c;
            return new CenteredPolynomialKernel2<T>(a, b, c);
        }
        if ( str.find("HPolynomialKernel") == 0 )
        {
            ss >> a;
            return new HPolynomialKernel<T>(a);
        }
        if ( str.find("CenteredHPolynomialKernel1") == 0 )
        {
            ss >> a;
            return new CenteredHPolynomialKernel1<T>(a);
        }
        if ( str.find("CenteredHPolynomialKernel2") == 0 )
        {
            ss >> a;
            return new CenteredHPolynomialKernel2<T>(a);
        }
        if ( str.find("GaussianKernel") == 0 )
        {
            ss >> a;
            return new GaussianKernel<T>(a);
        }
        return NULL;
    }
}


#endif	/* KERNELFUNCTIONIO_H */


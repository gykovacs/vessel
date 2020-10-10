/* 
 * File:   CorrelationFunctions.h
 * Author: gykovacs
 *
 * Created on 2011. február 21., 23:09
 */

#ifndef CORRELATIONFUNCTIONS_H
#define	CORRELATIONFUNCTIONS_H

#include <openipML/SimilarityFunction.h>
#include <vector>

namespace openip
{
    template<typename T>
    class NonCenteredCorrelation: public SimilarityFunction<T>
    {
    public:
        using SimilarityFunction<T>::descriptor;

        NonCenteredCorrelation();

        NonCenteredCorrelation(const NonCenteredCorrelation& n);

        ~NonCenteredCorrelation();

        double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    NonCenteredCorrelation<T>::NonCenteredCorrelation()
    : SimilarityFunction<T>()
    {
    }

    template<typename T>
    NonCenteredCorrelation<T>::NonCenteredCorrelation(const NonCenteredCorrelation& n)
    : SimilarityFunction<T>(n)
    {
    }

    template<typename T>
    NonCenteredCorrelation<T>::~NonCenteredCorrelation()
    {
    }

    template<typename T>
    double NonCenteredCorrelation<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0, na= 0.0, nb= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
        {
            sum+= a(i)*b(i);
            na+= a(i)*a(i);
            nb+= b(i)*b(i);
        }

        return sum/(sqrt(na*nb));
    }

    template<typename T>
    class CenteredCorrelation: public SimilarityFunction<T>
    {
    public:
        CenteredCorrelation();

        CenteredCorrelation(const CenteredCorrelation& n);

        ~CenteredCorrelation();

        double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    CenteredCorrelation<T>::CenteredCorrelation()
    : SimilarityFunction<T>()
    {
    }

    template<typename T>
    CenteredCorrelation<T>::CenteredCorrelation(const CenteredCorrelation& n)
    : SimilarityFunction<T>(n)
    {
    }

    template<typename T>
    CenteredCorrelation<T>::~CenteredCorrelation()
    {
    }

    template<typename T>
    double CenteredCorrelation<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0, na= 0.0, nb= 0.0, ma= 0.0, mb= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
        {
            ma+= a(i);
            mb+= b(i);
        }
        ma/= a.size();
        mb/= b.size();

        for ( unsigned int i= 0; i < a.size(); ++i )
        {
            sum+= (a(i) - ma)*(b(i) - mb);
            na+= (a(i) - ma)*(a(i) - ma);
            nb+= (b(i) - mb)*(b(i) - mb);
        }

        return sum/(sqrt(na*nb));
    }

    template<typename T>
    class KernelNonCenteredCorrelation: public SimilarityFunction<T>
    {
    public:
        KernelNonCenteredCorrelation(KernelFunction<T>* kernel);

        KernelNonCenteredCorrelation(const KernelNonCenteredCorrelation& n);

        ~KernelNonCenteredCorrelation();

        double evaluate(Vector<T>& a, Vector<T>& b);

        KernelFunction<T>* kernel;
    };

    template<typename T>
    KernelNonCenteredCorrelation<T>::KernelNonCenteredCorrelation(KernelFunction<T>* kernel)
    : SimilarityFunction<T>()
    {
        this->kernel= kernel;
    }

    template<typename T>
    KernelNonCenteredCorrelation<T>::KernelNonCenteredCorrelation(const KernelNonCenteredCorrelation& n)
    : SimilarityFunction<T>(n), kernel(n.kernel)
    {
    }

    template<typename T>
    KernelNonCenteredCorrelation<T>::~KernelNonCenteredCorrelation()
    {
    }

    template<typename T>
    double KernelNonCenteredCorrelation<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        return kernel->evaluate(a,b)/sqrt(kernel->evaluate(a,a) * (kernel->evaluate(b,b)));
    }

    template<typename T>
    class KernelCenteredCorrelation: public SimilarityFunction<T>
    {
    public:
        KernelCenteredCorrelation(KernelFunction<T>* kernel);

        KernelCenteredCorrelation(const KernelCenteredCorrelation& n);

        ~KernelCenteredCorrelation();

        double evaluate(Vector<T>& a, Vector<T>& b);

        KernelFunction<T>* kernel;
    };

    template<typename T>
    KernelCenteredCorrelation<T>::KernelCenteredCorrelation(KernelFunction<T>* kernel)
    : SimilarityFunction<T>()
    {
        this->kernel= kernel;
    }

    template<typename T>
    KernelCenteredCorrelation<T>::KernelCenteredCorrelation(const KernelCenteredCorrelation& n)
    : SimilarityFunction<T>(n), kernel(n.kernel)
    {
    }

    template<typename T>
    KernelCenteredCorrelation<T>::~KernelCenteredCorrelation()
    {
    }

    template<typename T>
    double KernelCenteredCorrelation<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double suma= kernel->feature(a);
        double sumb= kernel->feature(b);
        //printf("%f %f %f %f %f\t\t", suma, sumb, kernel->evaluate(a, b), kernel->evaluate(a,a), kernel->evaluate(b,b)); fflush(stdout);
        double res= (kernel->evaluate(a,b) - suma*sumb)/sqrt((kernel->evaluate(a,a)-suma*suma) * (kernel->evaluate(b,b)-sumb*sumb));
        //printf("%f %f %f %f %f\t", kernel->evaluate(a, b), kernel->evaluate(a,a), kernel->evaluate(b, b), suma, sumb);
        return res;
    }
}

#endif	/* CORRELATIONFUNCTIONS_H */


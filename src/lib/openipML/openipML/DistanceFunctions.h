/* 
 * File:   DistanceFunctions.h
 * Author: gykovacs
 *
 * Created on 2011. február 21., 18:54
 */

#ifndef DISTANCEFUNCTIONS_H
#define	DISTANCEFUNCTIONS_H

#include <math.h>

#include <openipML/BivariateFunction.h>
#include <openipML/CorrelationFunctions.h>

#include "CorrelationFunctions.h"

namespace openip
{
    template<typename T>
    class DistanceFunction: public BivariateFunction<T>
    {
    public:
        using BivariateFunction<T>::descriptor;

        DistanceFunction();

        DistanceFunction(const DistanceFunction& b);

        virtual ~DistanceFunction();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    DistanceFunction<T>::DistanceFunction()
    : BivariateFunction<T>()
    {
    }

    template<typename T>
    DistanceFunction<T>::DistanceFunction(const DistanceFunction& b)
    : BivariateFunction<T>(b)
    {
    }

    template<typename T>
    DistanceFunction<T>::~DistanceFunction<T>()
    {
    }

    template<typename T>
    double DistanceFunction<T>::evaluate(Vector<T>&, Vector<T>&)
    {
        return 0.0;
    }

    template<typename T>
    class EuclideanDistance: public DistanceFunction<T>
    {
    public:
        EuclideanDistance();

        EuclideanDistance(const EuclideanDistance& e);

        ~EuclideanDistance();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    EuclideanDistance<T>::EuclideanDistance()
    : DistanceFunction<T>()
    {
    }

    template<typename T>
    EuclideanDistance<T>::EuclideanDistance(const EuclideanDistance& b)
    : DistanceFunction<T>(b)
    {
    }

    template<typename T>
    EuclideanDistance<T>::~EuclideanDistance<T>()
    {
    }

    template<typename T>
    double EuclideanDistance<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));

        //return sqrt(sum);
	return sum;
    }

    template<typename T>
    class CenteredEuclideanDistance: public DistanceFunction<T>
    {
    public:
        CenteredEuclideanDistance();

        CenteredEuclideanDistance(const CenteredEuclideanDistance& e);

        ~CenteredEuclideanDistance();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    CenteredEuclideanDistance<T>::CenteredEuclideanDistance()
    : DistanceFunction<T>()
    {
    }

    template<typename T>
    CenteredEuclideanDistance<T>::CenteredEuclideanDistance(const CenteredEuclideanDistance& b)
    : DistanceFunction<T>(b)
    {
    }

    template<typename T>
    CenteredEuclideanDistance<T>::~CenteredEuclideanDistance<T>()
    {
    }

    template<typename T>
    double CenteredEuclideanDistance<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;

        double meana= 0, meanb= 0;
        for ( unsigned int i= 0; i < a.size(); ++i )
        {
            meana+= a(i);
            meanb+= b(i);
        }
        meana/= a.size();
        meanb/= b.size();

        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - meana - b(i) + meanb)*(a(i) - meana - b(i) + meanb);

        return sqrt(sum);
    }

    template<typename T>
    class HausdorffDistance: public DistanceFunction<T>
    {
    public:
        HausdorffDistance();

        HausdorffDistance(const HausdorffDistance& h);

        ~HausdorffDistance();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    HausdorffDistance<T>::HausdorffDistance()
    : DistanceFunction<T>()
    {
    }

    template<typename T>
    HausdorffDistance<T>::HausdorffDistance(const HausdorffDistance& b)
    : DistanceFunction<T>(b)
    {
    }

    template<typename T>
    HausdorffDistance<T>::~HausdorffDistance<T>()
    {
    }

    template<typename T>
    double HausdorffDistance<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < a.size(); ++i )
            if ( fabs(a(i) - b(i)) > sum )
                sum= fabs(a(i) - b(i));

        return sum;
    }

    template<typename T>
    class SquaredEuclideanDistance: public DistanceFunction<T>
    {
    public:
        SquaredEuclideanDistance();

        SquaredEuclideanDistance(const SquaredEuclideanDistance& s);

        ~SquaredEuclideanDistance();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    SquaredEuclideanDistance<T>::SquaredEuclideanDistance()
    : DistanceFunction<T>()
    {
    }

    template<typename T>
    SquaredEuclideanDistance<T>::SquaredEuclideanDistance(const SquaredEuclideanDistance& b)
    : DistanceFunction<T>(b)
    {
    }

    template<typename T>
    SquaredEuclideanDistance<T>::~SquaredEuclideanDistance<T>()
    {
    }

    template<typename T>
    double SquaredEuclideanDistance<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));

        return sum;
    }

    template<typename T>
    class KernelEuclideanDistance: public DistanceFunction<T>
    {
    public:
        KernelEuclideanDistance(KernelFunction<T>* kernel);

        KernelEuclideanDistance(const KernelEuclideanDistance& s);

        ~KernelEuclideanDistance();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        KernelFunction<T>* kernel;
    };

    template<typename T>
    KernelEuclideanDistance<T>::KernelEuclideanDistance(KernelFunction<T>* kernel)
    : DistanceFunction<T>()
    {
        this->kernel= kernel;
    }

    template<typename T>
    KernelEuclideanDistance<T>::KernelEuclideanDistance(const KernelEuclideanDistance& b)
    : DistanceFunction<T>(b), kernel(b.kernel)
    {
    }

    template<typename T>
    KernelEuclideanDistance<T>::~KernelEuclideanDistance<T>()
    {
    }

    template<typename T>
    double KernelEuclideanDistance<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        return (*kernel).evaluate(a,a) + (*kernel).evaluate(b,b) - 2 * (*kernel).evaluate(a,b);
    }
    
    template<typename T>
    class KernelEuclidean2Distance: public DistanceFunction<T>
    {
    public:
        KernelEuclidean2Distance(KernelFunction<T>* kernel);

        KernelEuclidean2Distance(const KernelEuclidean2Distance& s);

        ~KernelEuclidean2Distance();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        KernelFunction<T>* kernel;
    };

    template<typename T>
    KernelEuclidean2Distance<T>::KernelEuclidean2Distance(KernelFunction<T>* kernel)
    : DistanceFunction<T>()
    {
        this->kernel= kernel;
    }

    template<typename T>
    KernelEuclidean2Distance<T>::KernelEuclidean2Distance(const KernelEuclidean2Distance& b)
    : DistanceFunction<T>(b), kernel(b.kernel)
    {
    }

    template<typename T>
    KernelEuclidean2Distance<T>::~KernelEuclidean2Distance<T>()
    {
    }

    template<typename T>
    double KernelEuclidean2Distance<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
      double sum= 0;
      for ( unsigned int i= 0; i < a.size(); ++i )
	sum+= (a(i) - b(i))*(a(i) - b(i));
      return sum;
    }

    template<typename T>
    class NonCenteredCorrelationDistance: public DistanceFunction<T>, public NonCenteredCorrelation<T>
    {
    public:
        NonCenteredCorrelationDistance();

        NonCenteredCorrelationDistance(const NonCenteredCorrelationDistance& s);

        ~NonCenteredCorrelationDistance();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    NonCenteredCorrelationDistance<T>::NonCenteredCorrelationDistance()
    : DistanceFunction<T>(), NonCenteredCorrelation<T>()
    {
    }

    template<typename T>
    NonCenteredCorrelationDistance<T>::NonCenteredCorrelationDistance(const NonCenteredCorrelationDistance& b)
    : DistanceFunction<T>(b), NonCenteredCorrelation<T>(b)
    {
    }

    template<typename T>
    NonCenteredCorrelationDistance<T>::~NonCenteredCorrelationDistance<T>()
    {
    }

    template<typename T>
    double NonCenteredCorrelationDistance<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        return 1.0 - fabs(this->NonCenteredCorrelation<T>::evaluate(a, b));
    }

    template<typename T>
    class CenteredCorrelationDistance: public DistanceFunction<T>, public CenteredCorrelation<T>
    {
    public:
        CenteredCorrelationDistance();

        CenteredCorrelationDistance(const CenteredCorrelationDistance& s);

        ~CenteredCorrelationDistance();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    CenteredCorrelationDistance<T>::CenteredCorrelationDistance()
    : DistanceFunction<T>(), CenteredCorrelation<T>()
    {
    }

    template<typename T>
    CenteredCorrelationDistance<T>::CenteredCorrelationDistance(const CenteredCorrelationDistance& b)
    : DistanceFunction<T>(b), CenteredCorrelation<T>(b)
    {
    }

    template<typename T>
    CenteredCorrelationDistance<T>::~CenteredCorrelationDistance<T>()
    {
    }

    template<typename T>
    double CenteredCorrelationDistance<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        return 1.0 - fabs(this->CenteredCorrelation<T>::evaluate(a, b));
    }

    template<typename T>
    class KernelNonCenteredCorrelationDistance: public DistanceFunction<T>, public KernelNonCenteredCorrelation<T>
    {
    public:
        KernelNonCenteredCorrelationDistance(KernelFunction<T>* k);

        KernelNonCenteredCorrelationDistance(const KernelNonCenteredCorrelationDistance& s);

        ~KernelNonCenteredCorrelationDistance();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    KernelNonCenteredCorrelationDistance<T>::KernelNonCenteredCorrelationDistance(KernelFunction<T>* k)
    : DistanceFunction<T>(), KernelNonCenteredCorrelation<T>(k)
    {
    }

    template<typename T>
    KernelNonCenteredCorrelationDistance<T>::KernelNonCenteredCorrelationDistance(const KernelNonCenteredCorrelationDistance& b)
    : DistanceFunction<T>(b), KernelNonCenteredCorrelation<T>(b)
    {
    }

    template<typename T>
    KernelNonCenteredCorrelationDistance<T>::~KernelNonCenteredCorrelationDistance<T>()
    {
    }

    template<typename T>
    double KernelNonCenteredCorrelationDistance<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        return 1.0 - fabs(this->KernelNonCenteredCorrelation<T>::evaluate(a, b));
    }

    template<typename T>
    class KernelCenteredCorrelationDistance: public DistanceFunction<T>, public KernelCenteredCorrelation<T>
    {
    public:
        KernelCenteredCorrelationDistance(KernelFunction<T>* k);

        KernelCenteredCorrelationDistance(const KernelCenteredCorrelationDistance& s);

        ~KernelCenteredCorrelationDistance();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    KernelCenteredCorrelationDistance<T>::KernelCenteredCorrelationDistance(KernelFunction<T>* k)
    : DistanceFunction<T>(), KernelCenteredCorrelation<T>(k)
    {
    }

    template<typename T>
    KernelCenteredCorrelationDistance<T>::KernelCenteredCorrelationDistance(const KernelCenteredCorrelationDistance& b)
    : DistanceFunction<T>(b), KernelCenteredCorrelation<T>(b)
    {
    }

    template<typename T>
    KernelCenteredCorrelationDistance<T>::~KernelCenteredCorrelationDistance<T>()
    {
    }

    template<typename T>
    double KernelCenteredCorrelationDistance<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        return 1.0 - fabs(this->KernelCenteredCorrelation<T>::evaluate(a, b));
    }

    template<typename T>
    class KernelCenteredEuclideanDistance: public DistanceFunction<T>
    {
    public:
        KernelCenteredEuclideanDistance(KernelFunction<T>* k);

        KernelCenteredEuclideanDistance(const KernelCenteredEuclideanDistance& e);

        ~KernelCenteredEuclideanDistance();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        KernelFunction<T>* k;
    };

    template<typename T>
    KernelCenteredEuclideanDistance<T>::KernelCenteredEuclideanDistance(KernelFunction<T>* k)
    : DistanceFunction<T>()
    {
        this->k= k;
    }

    template<typename T>
    KernelCenteredEuclideanDistance<T>::KernelCenteredEuclideanDistance(const KernelCenteredEuclideanDistance& b)
    : DistanceFunction<T>(b), k(b.k)
    {
    }

    template<typename T>
    KernelCenteredEuclideanDistance<T>::~KernelCenteredEuclideanDistance<T>()
    {
    }

    template<typename T>
    double KernelCenteredEuclideanDistance<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        double ta= k->feature(a);
        double tb= k->feature(b);

        sum= k->evaluate(a,a) + k->evaluate(b,b) - k->evaluate(a,b)*2 - ta*ta - tb*tb + 2*ta*tb;

        return sum;
    }

}

#endif	/* DISTANCEFUNCTIONS_H */


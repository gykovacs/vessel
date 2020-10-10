/* 
 * File:   KernelFunctions.h
 * Author: gykovacs
 *
 * Created on 2011. február 21., 18:54
 */

#ifndef KERNELFUNCTIONS_H
#define	KERNELFUNCTIONS_H

#include <math.h>

#include <openipDS/mathFunctions.h>

#include <openipML/BivariateFunction.h>

namespace openip
{
    template<typename T>
    class KernelFunction: public BivariateFunction<T>
    {
    public:
        using BivariateFunction<T>::descriptor;

        KernelFunction(int unknown= 28);

        KernelFunction(const KernelFunction& k);

        virtual ~KernelFunction();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
        
        virtual double feature(Vector<T>& a);
	
	int unknown;
	int unknownSet;
    };

    template<typename T>
    KernelFunction<T>::KernelFunction(int unknown)
    : BivariateFunction<T>()
    {
      this->unknown= unknown;
      if ( unknown != 28 )
	unknownSet= 1;
      else
	unknownSet= 0;
    }

    template<typename T>
    KernelFunction<T>::KernelFunction(const KernelFunction& b)
    : BivariateFunction<T>(b)
    {
      this->unknown= b.unknown;
      this->unknownSet= b.unknownSet;
    }

    template<typename T>
    KernelFunction<T>::~KernelFunction()
    {
    }

    template<typename T>
    double KernelFunction<T>::evaluate(Vector<T>&, Vector<T>&)
    {
        return 0.0;
    }
    
    template<typename T>
    double KernelFunction<T>::feature(Vector<T>&)
    {
        return 0.0;
    }

    template<typename T>
    class DotProductKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;
	using KernelFunction<T>::unknown;
	using KernelFunction<T>::unknownSet;

        DotProductKernel(int unknown= 28);

        DotProductKernel(const DotProductKernel& l);

        ~DotProductKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double c;
    };

    template<typename T>
    DotProductKernel<T>::DotProductKernel(int unknown)
    : KernelFunction<T>(unknown)
    {
        std::stringstream ss;
        ss << "DotProductKernel " << unknown;
        descriptor= ss.str();
    }

    template<typename T>
    DotProductKernel<T>::DotProductKernel(const DotProductKernel& b)
    : KernelFunction<T>(b)
    {
    }

    template<typename T>
    DotProductKernel<T>::~DotProductKernel()
    {
    }

    template<typename T>
    double DotProductKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double d= 0.0;
	int flag= 0;
	if ( !unknownSet )
	  for ( unsigned int i= 0; i < a.size(); ++i )
            d+= a(i)*b(i);
	else
	{
	  for ( unsigned int i= 0; i < a.size(); ++i )
	    if ( a(i) != unknown && b(i) != unknown )
	    {
	      d+= a(i)*b(i);
	      flag= 1;
	    }
	  if ( !flag )
	    return FLT_MAX;
	}
        return d;
    }
    
    template<typename T>
    class CenteredDotProductKernel: public DotProductKernel<T>
    {
    public:
        using DotProductKernel<T>::descriptor;

        CenteredDotProductKernel();

        CenteredDotProductKernel(const CenteredDotProductKernel& l);

        ~CenteredDotProductKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
        
        virtual double feature(Vector<T>& a);

        double c;
    };

    template<typename T>
    CenteredDotProductKernel<T>::CenteredDotProductKernel()
    : DotProductKernel<T>()
    {
        std::stringstream ss;
        ss << "CenteredDotProductKernel ";
        descriptor= ss.str();
    }

    template<typename T>
    CenteredDotProductKernel<T>::CenteredDotProductKernel(const CenteredDotProductKernel& b)
    : DotProductKernel<T>(b)
    {
    }

    template<typename T>
    CenteredDotProductKernel<T>::~CenteredDotProductKernel()
    {
    }

    template<typename T>
    double CenteredDotProductKernel<T>::feature(Vector<T>& a)
    {
        double sum= 0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= a(i);
        return sum/sqrt(float(a.size()));
    }
    
    template<typename T>
    double CenteredDotProductKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        return DotProductKernel<T>::evaluate(a, b) - feature(a) * feature(b);
    }
    
    template<typename T>
    class LinearKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        LinearKernel(double c, int unknown= 28);

        LinearKernel(const LinearKernel& l);

        ~LinearKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double c;
    };

    template<typename T>
    LinearKernel<T>::LinearKernel(double c, int unknown)
    : KernelFunction<T>(unknown)
    {
        this->c= c;

        std::stringstream ss;
        ss << "LinearKernel " << c << " " << unknown;
        descriptor= ss.str();
    }

    template<typename T>
    LinearKernel<T>::LinearKernel(const LinearKernel& b)
    : KernelFunction<T>(b), c(b.c)
    {
    }

    template<typename T>
    LinearKernel<T>::~LinearKernel()
    {
    }

    template<typename T>
    double LinearKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double d= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            d+= a(i)*b(i);
        return d + c;
    }

    template<typename T>
    class ExponentialKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        ExponentialKernel(double sigma);

        ExponentialKernel(const ExponentialKernel& e);

        ~ExponentialKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double sigma;
    };

    template<typename T>
    ExponentialKernel<T>::ExponentialKernel(double sigma)
    : KernelFunction<T>()
    {
        this->sigma= sigma * sigma;

        std::stringstream ss;
        ss << "ExponentialKernel " << sigma;
        descriptor= ss.str();
    }

    template<typename T>
    ExponentialKernel<T>::ExponentialKernel(const ExponentialKernel& b)
    : KernelFunction<T>(b), sigma(b.sigma)
    {
    }

    template<typename T>
    ExponentialKernel<T>::~ExponentialKernel()
    {
    }

    template<typename T>
    double ExponentialKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));
        return exp(-sqrt(sum)/(2.0*sigma));
    }

    template<typename T>
    class LaplacianKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        LaplacianKernel(double sigma);

        LaplacianKernel(const LaplacianKernel& e);

        ~LaplacianKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double sigma;
    };

    template<typename T>
    LaplacianKernel<T>::LaplacianKernel(double sigma)
    : KernelFunction<T>()
    {
        this->sigma= sigma;

        std::stringstream ss;
        ss << "LaplacianKernel " << sigma;
        descriptor= ss.str();
    }

    template<typename T>
    LaplacianKernel<T>::LaplacianKernel(const LaplacianKernel& b)
    : KernelFunction<T>(b), sigma(b.sigma)
    {
    }

    template<typename T>
    LaplacianKernel<T>::~LaplacianKernel()
    {
    }

    template<typename T>
    double LaplacianKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));
        return exp(-sqrt(sum)/(sigma));
    }

    template<typename T>
    class ANOVAKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        ANOVAKernel(double k, double d, double sigma);

        ANOVAKernel(const ANOVAKernel& a);

        ~ANOVAKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double k;
        double d;
        double sigma;
    };

    template<typename T>
    ANOVAKernel<T>::ANOVAKernel(double k, double d, double sigma)
    : KernelFunction<T>()
    {
        this->k= k;
        this->d= d;
        this->sigma= sigma;

        std::stringstream ss;
        ss << "ANOVAKernel " << k << " " << d << " " << sigma;
        descriptor= ss.str();
    }

    template<typename T>
    ANOVAKernel<T>::ANOVAKernel(const ANOVAKernel& b)
    : KernelFunction<T>(b), k(b.k), d(b.d), sigma(b.sigma)
    {
    }

    template<typename T>
    ANOVAKernel<T>::~ANOVAKernel()
    {
    }

    template<typename T>
    double ANOVAKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        double tmp;
        for ( unsigned int i= 0; i < a.size(); ++i )
        {
            tmp= pow(a(i),k) - pow(b(i),k);
            tmp*= tmp;
            sum+= pow(exp(-sigma*tmp), d);
        }
        return sum;
    }

    template<typename T>
    class HyperbolicTangentKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        HyperbolicTangentKernel(double alpha, double c);

        HyperbolicTangentKernel(const HyperbolicTangentKernel& h);

        ~HyperbolicTangentKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double alpha;
        double c;
    };

    template<typename T>
    HyperbolicTangentKernel<T>::HyperbolicTangentKernel(double alpha, double c)
    : KernelFunction<T>()
    {
        this->alpha= alpha;
        this->c= c;

        std::stringstream ss;
        ss << "HyperbolicTangentKernel " << alpha << " " << c;
        descriptor= ss.str();
    }

    template<typename T>
    HyperbolicTangentKernel<T>::HyperbolicTangentKernel(const HyperbolicTangentKernel& b)
    : KernelFunction<T>(b), alpha(b.alpha), c(b.c)
    {
    }

    template<typename T>
    HyperbolicTangentKernel<T>::~HyperbolicTangentKernel()
    {
    }

    template<typename T>
    double HyperbolicTangentKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= a(i)*b(i);
        return tanh(alpha*sum + c);
    }

    template<typename T>
    class RationalQuadraticKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        RationalQuadraticKernel(double c);

        RationalQuadraticKernel(const RationalQuadraticKernel& r);

        ~RationalQuadraticKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double c;
    };

    template<typename T>
    RationalQuadraticKernel<T>::RationalQuadraticKernel(double c)
    : KernelFunction<T>()
    {
        this->c= c;

        std::stringstream ss;
        ss << "RationalQuadraticKernel " << c;
        descriptor= ss.str();
    }

    template<typename T>
    RationalQuadraticKernel<T>::RationalQuadraticKernel(const RationalQuadraticKernel& b)
    : KernelFunction<T>(b), c(b.c)
    {
    }

    template<typename T>
    RationalQuadraticKernel<T>::~RationalQuadraticKernel()
    {
    }

    template<typename T>
    double RationalQuadraticKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));
        return 1.0 - sum/(sum + c);
    }

    template<typename T>
    class MultiquadraticKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        MultiquadraticKernel(double c);

        MultiquadraticKernel(const MultiquadraticKernel& m);

        ~MultiquadraticKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double c;
    };

    template<typename T>
    MultiquadraticKernel<T>::MultiquadraticKernel(double c)
    : KernelFunction<T>()
    {
        this->c= c;

        std::stringstream ss;
        ss << "MultiquadraticKernel " << c;
        descriptor= ss.str();
    }

    template<typename T>
    MultiquadraticKernel<T>::MultiquadraticKernel(const MultiquadraticKernel& b)
    : KernelFunction<T>(b), c(b.c)
    {
    }

    template<typename T>
    MultiquadraticKernel<T>::~MultiquadraticKernel()
    {
    }

    template<typename T>
    double MultiquadraticKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));
        return sqrt(sum + c*c);
    }

    template<typename T>
    class InverseMultiquadraticKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        InverseMultiquadraticKernel(double c);

        InverseMultiquadraticKernel(const InverseMultiquadraticKernel& m);

        ~InverseMultiquadraticKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double c;
    };

    template<typename T>
    InverseMultiquadraticKernel<T>::InverseMultiquadraticKernel(double c)
    : KernelFunction<T>()
    {
        this->c= c;

        std::stringstream ss;
        ss << "InverseMultiquadraticKernel " << c;
        descriptor= ss.str();
    }

    template<typename T>
    InverseMultiquadraticKernel<T>::InverseMultiquadraticKernel(const InverseMultiquadraticKernel& b)
    : KernelFunction<T>(b), c(b.c)
    {
    }

    template<typename T>
    InverseMultiquadraticKernel<T>::~InverseMultiquadraticKernel()
    {
    }

    template<typename T>
    double InverseMultiquadraticKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));
        return 1.0/sqrt(sum + c*c);
    }

    template<typename T>
    class CircularKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        CircularKernel(double sigma);

        CircularKernel(const CircularKernel& c);

        ~CircularKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double sigma;
    };

    template<typename T>
    CircularKernel<T>::CircularKernel(double sigma)
    : KernelFunction<T>()
    {
        this->sigma= sigma;

        std::stringstream ss;
        ss << "CircularKernel " << sigma;
        descriptor= ss.str();
    }

    template<typename T>
    CircularKernel<T>::CircularKernel(const CircularKernel& b)
    : KernelFunction<T>(b), sigma(b.sigma)
    {
    }

    template<typename T>
    CircularKernel<T>::~CircularKernel()
    {
    }

    template<typename T>
    double CircularKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));
        if ( sqrt(sum) < sigma )
            return 2.0/M_PI*(acos(-sqrt(sum)/sigma) - sqrt(sum)/sigma*sqrt(1.0 - sqrt(sum)/sigma));
        else
            return 0.0;
    }

    template<typename T>
    class SphericalKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        SphericalKernel(double sigma);

        SphericalKernel(const SphericalKernel& w);

        ~SphericalKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double sigma;
    };

    template<typename T>
    SphericalKernel<T>::SphericalKernel(double sigma)
    : KernelFunction<T>()
    {
        this->sigma= sigma;

        std::stringstream ss;
        ss << "SphericalKernel " << sigma;
        descriptor= ss.str();
    }

    template<typename T>
    SphericalKernel<T>::SphericalKernel(const SphericalKernel& b)
    : KernelFunction<T>(b), sigma(b.sigma)
    {
    }

    template<typename T>
    SphericalKernel<T>::~SphericalKernel()
    {
    }

    template<typename T>
    double SphericalKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));

        if ( sum < sigma )
            return 0.0;
        else
            return 1.0 - 3.0/2.0*sqrt(sum)/sigma + 1.0/2.0*(pow(sqrt(sum)/sigma, 3.0));
    }

    template<typename T>
    class WaveKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        WaveKernel(double theta);

        WaveKernel(const WaveKernel& w);

        ~WaveKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double theta;
    };

    template<typename T>
    WaveKernel<T>::WaveKernel(double theta)
    : KernelFunction<T>()
    {
        this->theta= theta;

        std::stringstream ss;
        ss << "WaveKernel " << theta;
        descriptor= ss.str();
    }

    template<typename T>
    WaveKernel<T>::WaveKernel(const WaveKernel& b)
    : KernelFunction<T>(b), theta(b.theta)
    {
    }

    template<typename T>
    WaveKernel<T>::~WaveKernel()
    {
    }

    template<typename T>
    double WaveKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));

        return theta/sqrt(sum) * sin(sqrt(sum)/theta);
    }

    template<typename T>
    class PowerKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        PowerKernel(double d);

        PowerKernel(const PowerKernel& p);

        ~PowerKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double d;
    };

    template<typename T>
    PowerKernel<T>::PowerKernel(double d)
    : KernelFunction<T>()
    {
        this->d= d;

        std::stringstream ss;
        ss << "PowerKernel " << d;
        descriptor= ss.str();
    }

    template<typename T>
    PowerKernel<T>::PowerKernel(const PowerKernel& b)
    : KernelFunction<T>(b), d(b.d)
    {
    }

    template<typename T>
    PowerKernel<T>::~PowerKernel()
    {
    }

    template<typename T>
    double PowerKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));

        return -pow(sqrt(sum), d);
    }

    template<typename T>
    class LogKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        LogKernel(double d);

        LogKernel(const LogKernel& l);

        ~LogKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double d;
    };

    template<typename T>
    LogKernel<T>::LogKernel(double d)
    : KernelFunction<T>()
    {
        this->d= d;

        std::stringstream ss;
        ss << "LogKernel " << d;
        descriptor= ss.str();
    }

    template<typename T>
    LogKernel<T>::LogKernel(const LogKernel& b)
    : KernelFunction<T>(b), d(b.d)
    {
    }

    template<typename T>
    LogKernel<T>::~LogKernel()
    {
    }

    template<typename T>
    double LogKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));

        return -log(pow(sqrt(sum), d) + 1.0);
    }

    template<typename T>
    class SplineKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        SplineKernel();

        SplineKernel(const SplineKernel& s);

        ~SplineKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    SplineKernel<T>::SplineKernel()
    : KernelFunction<T>()
    {
        std::stringstream ss;
        ss << "SplineKernel ";
        descriptor= ss.str();
    }

    template<typename T>
    SplineKernel<T>::SplineKernel(const SplineKernel& b)
    : KernelFunction<T>(b)
    {
    }

    template<typename T>
    SplineKernel<T>::~SplineKernel()
    {
    }

    template<typename T>
    double SplineKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        Vector<T> minxy(a);
        for ( unsigned int i= 0; i < a.size(); ++i )
            minxy(i)= a(i) < b(i) ? a(i) : b(i);

        double sum= 0.0;
        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= 1 + a(i)*b(i) + a(i)*b(i)*minxy(i) - (a(i) + b(i))/2*(minxy(i)*minxy(i)) + 1.0/3.0*(minxy(i)*minxy(i)*minxy(i));
        return sum;
    }

    template<typename T>
    class BSplineKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        BSplineKernel(int n);

        BSplineKernel(const BSplineKernel& s);

        ~BSplineKernel();

        double factorial(double d);

        double step(double d, double n, double k);

        double bn(double x, double n);

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        int n;
    };

    template<typename T>
    BSplineKernel<T>::BSplineKernel(int n)
    : KernelFunction<T>()
    {
        this->n= n;

        std::stringstream ss;
        ss << "BSplineKernel " << n;
        descriptor= ss.str();
    }

    template<typename T>
    BSplineKernel<T>::BSplineKernel(const BSplineKernel& b)
    : KernelFunction<T>(b), n(b.n)
    {
    }

    template<typename T>
    BSplineKernel<T>::~BSplineKernel()
    {
    }

    template<typename T>
    double BSplineKernel<T>::factorial(double d)
    {
        double res= 1.0;
        for ( unsigned int i= 2; i <= d; ++i )
            res*= i;
        return res;
    }

    template<typename T>
    double BSplineKernel<T>::step(double x, double n, double k)
    {
        double tmp;
        tmp= pow(x + (n+1.0)/2.0 - k,n);
        if ( tmp <= 0 )
            tmp= 0;
        return factorial(n+1)/(factorial(k)*factorial(n+1-k))*pow(-1,k)*tmp;
    }

    template<typename T>
    double BSplineKernel<T>::bn(double x, double n)
    {
        double sum= 0.0;
        for ( unsigned int i= 0; i < n+1; ++i )
            sum+= step(x, n, i);
        return sum/factorial(n);
    }

    template<typename T>
    double BSplineKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 1.0;

        for ( unsigned int i= 0; i < a.size(); ++i )
            sum*= bn(a(i) - b(i), this->n);

        return sum;
    }

    template<typename T>
    class CauchyKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        CauchyKernel(double sigma);

        CauchyKernel(const CauchyKernel& s);

        ~CauchyKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double sigma;
    };

    template<typename T>
    CauchyKernel<T>::CauchyKernel(double sigma)
    : KernelFunction<T>()
    {
        this->sigma= sigma;

        std::stringstream ss;
        ss << "CauchyKernel " << sigma;
        descriptor= ss.str();
    }

    template<typename T>
    CauchyKernel<T>::CauchyKernel(const CauchyKernel& b)
    : KernelFunction<T>(b), sigma(b.sigma)
    {
    }

    template<typename T>
    CauchyKernel<T>::~CauchyKernel()
    {
    }

    template<typename T>
    double CauchyKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));

        return 1.0/(1.0 + sum/sigma);
    }


    template<typename T>
    class HistogramIntersectionKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        HistogramIntersectionKernel();

        HistogramIntersectionKernel(const HistogramIntersectionKernel& s);

        ~HistogramIntersectionKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };

    template<typename T>
    HistogramIntersectionKernel<T>::HistogramIntersectionKernel()
    : KernelFunction<T>()
    {
        std::stringstream ss;
        ss << "HistogramIntersectionKernel ";
        descriptor= ss.str();
    }

    template<typename T>
    HistogramIntersectionKernel<T>::HistogramIntersectionKernel(const HistogramIntersectionKernel& b)
    : KernelFunction<T>(b)
    {
    }

    template<typename T>
    HistogramIntersectionKernel<T>::~HistogramIntersectionKernel()
    {
    }

    template<typename T>
    double HistogramIntersectionKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= a(i) < b(i) ? a(i) : b(i);

        return sum;
    }

    template<typename T>
    class GeneralizedHistogramIntersectionKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        GeneralizedHistogramIntersectionKernel(double alpha, double beta);

        GeneralizedHistogramIntersectionKernel(const GeneralizedHistogramIntersectionKernel& s);

        ~GeneralizedHistogramIntersectionKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double alpha;
        double beta;
    };

    template<typename T>
    GeneralizedHistogramIntersectionKernel<T>::GeneralizedHistogramIntersectionKernel(double alpha, double beta)
    : KernelFunction<T>()
    {
        this->alpha= alpha;
        this->beta= beta;

        std::stringstream ss;
        ss << "GeneralizedHistogramIntersectionKernel " << alpha << " " << beta;
        descriptor= ss.str();
    }

    template<typename T>
    GeneralizedHistogramIntersectionKernel<T>::GeneralizedHistogramIntersectionKernel(const GeneralizedHistogramIntersectionKernel& b)
    : KernelFunction<T>(b), alpha(b.alpha), beta(b.beta)
    {
    }

    template<typename T>
    GeneralizedHistogramIntersectionKernel<T>::~GeneralizedHistogramIntersectionKernel()
    {
    }

    template<typename T>
    double GeneralizedHistogramIntersectionKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= pow(fabs(a(i)), alpha) < pow(fabs(b(i)), beta) ? pow(fabs(a(i)), alpha) : pow(fabs(b(i)), beta);

        return sum;
    }

    template<typename T>
    class GeneralizedTStudentKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        GeneralizedTStudentKernel(double d);

        GeneralizedTStudentKernel(const GeneralizedTStudentKernel& s);

        ~GeneralizedTStudentKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double d;
    };

    template<typename T>
    GeneralizedTStudentKernel<T>::GeneralizedTStudentKernel(double d)
    : KernelFunction<T>()
    {
        this->d= d;

        std::stringstream ss;
        ss << "GeneralizedTStudentKernel " << d;
        descriptor= ss.str();
    }

    template<typename T>
    GeneralizedTStudentKernel<T>::GeneralizedTStudentKernel(const GeneralizedTStudentKernel& b)
    : KernelFunction<T>(b), d(b.d)
    {
    }

    template<typename T>
    GeneralizedTStudentKernel<T>::~GeneralizedTStudentKernel()
    {
    }

    template<typename T>
    double GeneralizedTStudentKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < a.size(); ++i )
            sum+= (a(i) - b(i))*(a(i) - b(i));

        return 1.0/(1.0 + pow(sqrt(sum), d));
    }

    template<typename T>
    class WaveletKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        WaveletKernel(double a, double c);

        WaveletKernel(const WaveletKernel& s);

        ~WaveletKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        double a;
        double c;
    };

    template<typename T>
    WaveletKernel<T>::WaveletKernel(double a, double c)
    : KernelFunction<T>()
    {
        this->a= a;
        this->c= c;

        std::stringstream ss;
        ss << "WaveletKernel " << a << " " << c;
        descriptor= ss.str();
    }

    template<typename T>
    WaveletKernel<T>::WaveletKernel(const WaveletKernel& b)
    : KernelFunction<T>(b), a(b.a), c(b.c)
    {
    }

    template<typename T>
    WaveletKernel<T>::~WaveletKernel()
    {
    }

    template<typename T>
    double WaveletKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 1.0;
        double tmpa, tmpb;

        for ( unsigned int i= 0; i < a.size(); ++i )
        {
            tmpa= (a(i) - c)/this->a;
            tmpb= (b(i) - c)/this->a;
            sum*= cos(1.75*tmpa)*exp(-tmpa*tmpa/2.0)*cos(1.75*tmpb)*exp(-tmpb*tmpb/2.0);
        }

        return sum;
    }

    template<typename T>
    class ChiSquareKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        ChiSquareKernel();

        ChiSquareKernel(const ChiSquareKernel& s);

        ~ChiSquareKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        virtual double feature(Vector<T>& a);
    };

    template<typename T>
    ChiSquareKernel<T>::ChiSquareKernel()
    : KernelFunction<T>()
    {
        std::stringstream ss;
        ss << "ChiSquareKernel ";
        descriptor= ss.str();
    }

    template<typename T>
    ChiSquareKernel<T>::ChiSquareKernel(const ChiSquareKernel& b)
    : KernelFunction<T>(b)
    {
    }

    template<typename T>
    ChiSquareKernel<T>::~ChiSquareKernel()
    {
    }

    template<typename T>
    double ChiSquareKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < a.size(); ++i )
        {
            sum+= 2*a(i)*b(i)/(a(i) + b(i));
        }

        return sum;
    }

    template<typename T>
    double ChiSquareKernel<T>::feature(Vector<T>&)
    {
        return 0;
    }

    template<typename T>
    class HellingerKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        HellingerKernel();

        HellingerKernel(const HellingerKernel& s);

        ~HellingerKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        virtual double feature(Vector<T>& a);
    };

    template<typename T>
    HellingerKernel<T>::HellingerKernel()
    : KernelFunction<T>()
    {
        std::stringstream ss;
        ss << "HellingerKernel ";
        descriptor= ss.str();
    }

    template<typename T>
    HellingerKernel<T>::HellingerKernel(const HellingerKernel& b)
    : KernelFunction<T>(b)
    {
    }

    template<typename T>
    HellingerKernel<T>::~HellingerKernel()
    {
    }

    template<typename T>
    double HellingerKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < a.size(); ++i )
        {
            sum+= sqrt(a(i)*b(i));
        }

        return sum;
    }

    template<typename T>
    double HellingerKernel<T>::feature(Vector<T>&)
    {
        return 0;
    }

    template<typename T>
    class IntersectionKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        IntersectionKernel();

        IntersectionKernel(const IntersectionKernel& s);

        ~IntersectionKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        virtual double feature(Vector<T>& a);
    };

    template<typename T>
    IntersectionKernel<T>::IntersectionKernel()
    : KernelFunction<T>()
    {
        std::stringstream ss;
        ss << "IntersectionKernel ";
        descriptor= ss.str();
    }

    template<typename T>
    IntersectionKernel<T>::IntersectionKernel(const IntersectionKernel& b)
    : KernelFunction<T>(b)
    {
    }

    template<typename T>
    IntersectionKernel<T>::~IntersectionKernel()
    {
    }

    template<typename T>
    double IntersectionKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < a.size(); ++i )
        {
            sum+= a(i) < b(i) ? a(i) : b(i);
        }

        return sum;
    }

    template<typename T>
    double IntersectionKernel<T>::feature(Vector<T>&)
    {
        return 0;
    }

    template<typename T>
    class PolynomialKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        PolynomialKernel(int d, float a, float b, int plus= 0, int unknown= 28);

        PolynomialKernel(const PolynomialKernel& s);

        ~PolynomialKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        virtual double feature(Vector<T>& a);

        vector<double> lambdas;
        int d;
        float a;
        float b;
        int plus;

        vector<vector<int> > alpha;
        vector<unsigned char> flags;
        vector<float> coeff;
        int num;
    };

    template<typename T>
    PolynomialKernel<T>::PolynomialKernel(int d, float a, float b, int plus, int unknown)
    : KernelFunction<T>(unknown)
    {
        this->d= d;
        this->a= a;
        this->b= b;
        this->plus= plus;
        lambdas.resize(d+1);
        std::stringstream ss;
        ss << "PolynomialKernel " << d << " " << a << " " << b << " " << plus << " " << unknown;
        descriptor= ss.str();
    }

    template<typename T>
    PolynomialKernel<T>::PolynomialKernel(const PolynomialKernel& b)
    : KernelFunction<T>(b)
    {
        this->d= b.d;
        this->a= b.a;
        this->b= b.b;
        this->plus= b.plus;
        this->lambdas= lambdas;
    }

    template<typename T>
    PolynomialKernel<T>::~PolynomialKernel()
    {
    }

    template<typename T>
    double PolynomialKernel<T>::evaluate(Vector<T>& x, Vector<T>& y)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < x.size(); ++i )
        {
            sum+= x(i)*y(i);
        }

        return pow(b + a*sum, d);
    }

    template<typename T>
    double PolynomialKernel<T>::feature(Vector<T>&)
    {
        return 0;
    }

    template<typename T>
    class GaussianKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        GaussianKernel(float sigma, int plus= 0, int unknown= 28);

        GaussianKernel(const GaussianKernel& s);

        ~GaussianKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        virtual double feature(Vector<T>& a);

        vector<double> lambdas;
        float sigma;
        int plus;
        unsigned int n;

        vector<vector<int> > alpha;
        vector<unsigned char> flags;
    };


    template<typename T>
    GaussianKernel<T>::GaussianKernel(float sigma, int plus, int unknown)
    : KernelFunction<T>(unknown)
    {
        this->sigma= sigma;
        this->plus= plus;

        this->n= -1;
        std::stringstream ss;
        ss << "GaussianKernel " << sigma << " " << plus << " " << unknown;
        descriptor= ss.str();
    }

    template<typename T>
    GaussianKernel<T>::GaussianKernel(const GaussianKernel& b)
    : KernelFunction<T>(b)
    {
        this->sigma= b.sigma;
        this->plus= b.plus;
        this->lambdas= lambdas;
    }

    template<typename T>
    GaussianKernel<T>::~GaussianKernel()
    {
    }

    template<typename T>
    double GaussianKernel<T>::evaluate(Vector<T>& x, Vector<T>& y)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < x.size(); ++i )
        {
            sum+= (x(i)-y(i))*(x(i) - y(i));
        }
        sum= sqrt(sum);

        return exp(-sum/(2*sigma*sigma));
    }

    template<typename T>
    double GaussianKernel<T>::feature(Vector<T>& )
    {
/*        float sum= 0;
        float m= 0;
        float xx= 0;
        for ( unsigned int i= 0; i < v.size(); ++i )
        {
            sum+= v(i);
            xx+= v(i)*v(i);
        }

        for ( int k= 0; k < K; ++k )
        {
            m+= pow(v.size(), k);

            sum+= (1/(pow(sigma, 2*k)*fakt(k)))*pow(sum,k);
        }*/

        //printf("%f,%f,%f ", sum, exp(-xx/(2*sigma*sigma))*sum,sqrt(m));
        //return exp(-xx/(2*sigma*sigma))*sum/sqrt(m);
        return 0;
    }

    template<typename T>
    class HPolynomialKernel: public KernelFunction<T>
    {
    public:
        using KernelFunction<T>::descriptor;

        HPolynomialKernel(int d, int plus= 0);

        HPolynomialKernel(const HPolynomialKernel& s);

        ~HPolynomialKernel();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);

        virtual double feature(Vector<T>& a);

        vector<double> lambdas;
        int d;
        int plus;

        vector<vector<int> > alpha;
        vector<unsigned char> flags;
        vector<float> coeff;
        int num;
    };

    template<typename T>
    HPolynomialKernel<T>::HPolynomialKernel(int d, int plus)
    : KernelFunction<T>()
    {
        this->d= d;
        this->plus= plus;
        lambdas.resize(d+1);
        std::stringstream ss;
        ss << "HPolynomialKernel " << d << " " << plus;
        descriptor= ss.str();
    }

    template<typename T>
    HPolynomialKernel<T>::HPolynomialKernel(const HPolynomialKernel& b)
    : KernelFunction<T>(b)
    {
        this->d= b.d;
        this->plus= b.plus;
        this->lambdas= lambdas;
    }

    template<typename T>
    HPolynomialKernel<T>::~HPolynomialKernel()
    {
    }

    template<typename T>
    double HPolynomialKernel<T>::evaluate(Vector<T>& x, Vector<T>& y)
    {
        double sum= 0.0;

        for ( unsigned int i= 0; i < x.size(); ++i )
        {
            sum+= x(i)*y(i);
        }

        return pow(sum, d);
    }

    template<typename T>
    double HPolynomialKernel<T>::feature(Vector<T>&)
    {
        return 0;
    }
    
    template<typename T>
    class CenteredPolynomialKernel1: public PolynomialKernel<T>
    {
    public:
        using PolynomialKernel<T>::descriptor;
        using PolynomialKernel<T>::feature;
        using PolynomialKernel<T>::d;
        using PolynomialKernel<T>::alpha;
        using PolynomialKernel<T>::lambdas;
        using PolynomialKernel<T>::a;
        using PolynomialKernel<T>::b;
        using PolynomialKernel<T>::plus;
        using PolynomialKernel<T>::flags;
        using PolynomialKernel<T>::coeff;
        using PolynomialKernel<T>::num;

        CenteredPolynomialKernel1(int d, float a, float b, int plus= 0);

        CenteredPolynomialKernel1(const CenteredPolynomialKernel1& s);

        ~CenteredPolynomialKernel1();
        
        virtual double feature(Vector<T>& a);

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };
    
    template<typename T>
    CenteredPolynomialKernel1<T>::CenteredPolynomialKernel1(int d, float a, float b, int plus)
    : PolynomialKernel<T>(d, a, b, plus)
    {
        std::stringstream ss;
        ss << "CenteredPolynomialKernel1 " << d << " " << a << " " << b << " " << plus;
        descriptor= ss.str();
    }
    
    template<typename T>
    CenteredPolynomialKernel1<T>::CenteredPolynomialKernel1(const CenteredPolynomialKernel1& c)
    : PolynomialKernel<T>(c)
    {
    }
    
    template<typename T>
    CenteredPolynomialKernel1<T>::~CenteredPolynomialKernel1()
    {
    }
    
    template<typename T>
    double CenteredPolynomialKernel1<T>::feature(Vector<T>& v)
    {
        double total= 0;
        int sum;
        float f= 0;
        int m= v.size();
        int n= pow(m+1,d);

        if ( alpha.size() == 0 )
        {
            alpha.resize(n);
            coeff.resize(n);
            for ( int i= 0; i < n; ++i )
                alpha[i].resize(v.size());

            for ( unsigned int j= 0; j < v.size(); ++j )
                    alpha[0][j]= 0;

            for ( int i= 1; i < n; ++i )
            {
                for ( unsigned int j= 0; j < v.size(); ++j )
                    alpha[i][j]= alpha[i-1][j];
                alpha[i][0]= alpha[i-1][0] + 1;
                if ( alpha[i][0] <= d )
                    continue;
                for ( unsigned int j= 1; j < v.size(); ++j )
                {
                    alpha[i][j-1]= 0;
                    alpha[i][j]= alpha[i][j]+1;
                    if ( alpha[i][j] <= d )
                        break;
                }
            }
            flags.resize(n);
            num= 0;
            float faktorial;
            for ( unsigned int i= 0; i < alpha.size(); ++i )
            {
                sum= 0;
                for ( unsigned int j= 0; j < alpha[i].size(); ++j )
                    sum+= alpha[i][j];

                if ( sum <= d )
                {
                    faktorial= 1;
                    faktorial*= fakt(this->d);
                    for ( unsigned int j= 0; j < alpha[i].size(); ++j )
                        faktorial/= fakt(alpha[i][j]);

                    faktorial*= pow(this->b, (d-sum)/2.0);
                    faktorial*= pow(this->a, sum/2.0);

                    //coeff[i]= sqrt(faktorial);
                    coeff[i]= 1;
                    ++num;
                    flags[i]= 1;
                }
                else
                    flags[i]= 0;
            }
        }

        for ( unsigned int i= 0; i < alpha.size(); ++i )
        {
            if ( flags[i] )
            {
                f= 1;
                for ( unsigned int j= 0; j < alpha[i].size(); ++j )
                    if ( alpha[i][j] )
                        f*= pow(v(j), alpha[i][j]);
                f*= coeff[i];
                total += f;
            }
        }

        return total/sqrt(num + plus);
    }
    
    template<typename T>
    double CenteredPolynomialKernel1<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        return PolynomialKernel<T>::evaluate(a, b) - (feature(a) * feature(b) / pow(a.size() + 1, float(d)/2));
    }
    
    template<typename T>
    class CenteredPolynomialKernel2: public PolynomialKernel<T>
    {
    public:
        using PolynomialKernel<T>::descriptor;
        using PolynomialKernel<T>::feature;
        using PolynomialKernel<T>::d;
        using PolynomialKernel<T>::alpha;
        using PolynomialKernel<T>::lambdas;
        using PolynomialKernel<T>::a;
        using PolynomialKernel<T>::b;
        using PolynomialKernel<T>::plus;
        using PolynomialKernel<T>::flags;
        using PolynomialKernel<T>::coeff;
        using PolynomialKernel<T>::num;

        CenteredPolynomialKernel2(int d, float a, float b, int plus= 0);

        CenteredPolynomialKernel2(const CenteredPolynomialKernel2& s);

        ~CenteredPolynomialKernel2();
        
        virtual double feature(Vector<T>& a);

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };
    
    template<typename T>
    CenteredPolynomialKernel2<T>::CenteredPolynomialKernel2(int d, float a, float b, int plus)
    : PolynomialKernel<T>(d, a, b, plus)
    {
        std::stringstream ss;
        ss << "CenteredPolynomialKernel2 " << d << " " << a << " " << b << " " << plus;
        descriptor= ss.str();
    }
    
    template<typename T>
    CenteredPolynomialKernel2<T>::CenteredPolynomialKernel2(const CenteredPolynomialKernel2& c)
    : PolynomialKernel<T>(c)
    {
    }
    
    template<typename T>
    CenteredPolynomialKernel2<T>::~CenteredPolynomialKernel2()
    {
    }
    
    
    template<typename T>
    double CenteredPolynomialKernel2<T>::feature(Vector<T>& v)
    {
        float sum= 0;
        for ( unsigned int i= 0; i < v.size(); ++i )
            sum+= v(i);

        return pow(b + a*sum, d)/sqrt(pow(v.size()+1, d) + plus);
    }
    
    template<typename T>
    double CenteredPolynomialKernel2<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        return PolynomialKernel<T>::evaluate(a, b) - (feature(a) * feature(b) / pow(a.size() + 1, float(d)/2));
    }
    
    template<typename T>
    class CenteredHPolynomialKernel1: public HPolynomialKernel<T>
    {
    public:
        using HPolynomialKernel<T>::descriptor;
        using HPolynomialKernel<T>::feature;
        using HPolynomialKernel<T>::d;
        using HPolynomialKernel<T>::alpha;
        using HPolynomialKernel<T>::lambdas;
        using HPolynomialKernel<T>::plus;
        using HPolynomialKernel<T>::flags;
        using HPolynomialKernel<T>::coeff;
        using HPolynomialKernel<T>::num;

        CenteredHPolynomialKernel1(int d, int plus= 0);

        CenteredHPolynomialKernel1(const CenteredHPolynomialKernel1& s);

        ~CenteredHPolynomialKernel1();
        
        virtual double feature(Vector<T>& a);

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };
    
    template<typename T>
    CenteredHPolynomialKernel1<T>::CenteredHPolynomialKernel1(int d, int plus)
    : HPolynomialKernel<T>(d, plus)
    {
        std::stringstream ss;
        ss << "CenteredHPolynomialKernel " << d << " " << plus;
        descriptor= ss.str();
    }
    
    template<typename T>
    CenteredHPolynomialKernel1<T>::CenteredHPolynomialKernel1(const CenteredHPolynomialKernel1& c)
    : HPolynomialKernel<T>(c)
    {
    }
    
    template<typename T>
    CenteredHPolynomialKernel1<T>::~CenteredHPolynomialKernel1()
    {
    }
    
    template<typename T>
    double CenteredHPolynomialKernel1<T>::feature(Vector<T>& v)
    {
        double total= 0;
        int sum;
        float f= 0;
        int m= v.size();
        int n= pow(m+1,d);

        if ( alpha.size() == 0 )
        {
            alpha.resize(n);
            coeff.resize(n);
            for ( int i= 0; i < n; ++i )
                alpha[i].resize(v.size());

            for ( unsigned int j= 0; j < v.size(); ++j )
                    alpha[0][j]= 0;

            for ( int i= 1; i < n; ++i )
            {
                for ( unsigned int j= 0; j < v.size(); ++j )
                    alpha[i][j]= alpha[i-1][j];
                alpha[i][0]= alpha[i-1][0] + 1;
                if ( alpha[i][0] <= d )
                    continue;
                for ( unsigned int j= 1; j < v.size(); ++j )
                {
                    alpha[i][j-1]= 0;
                    alpha[i][j]= alpha[i][j]+1;
                    if ( alpha[i][j] <= d )
                        break;
                }
            }
            flags.resize(n);
            num= 0;
            float faktorial;

            for ( unsigned int i= 0; i < alpha.size(); ++i )
            {
                sum= 0;
                faktorial= 1;
                for ( unsigned int j= 0; j < alpha[i].size(); ++j )
                    sum+= alpha[i][j];

                if ( sum == d )
                {
                    ++num;
                    flags[i]= 1;

                    faktorial*= fakt(d);
                    for ( unsigned int j= 0; j < alpha[i].size(); ++j )
                        faktorial/= fakt(alpha[i][j]);
                    coeff[i]= sqrt(faktorial);
                }
                else
                    flags[i]= 0;
            }
        }

        for ( unsigned int i= 0; i < alpha.size(); ++i )
        {
            if ( flags[i] )
            {
                f= 1;

                for ( unsigned int j= 0; j < alpha[i].size(); ++j )
                    if ( alpha[i][j] )
                        f*= pow(v(j), alpha[i][j]);

                f*= coeff[i];
                total += f;
            }
        }

        return total/sqrt(num + plus);
    }
    
    template<typename T>
    double CenteredHPolynomialKernel1<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        return HPolynomialKernel<T>::evaluate(a, b) - (feature(a) * feature(b) / pow(a.size(), float(d)/2));
    }
    
    template<typename T>
    class CenteredHPolynomialKernel2: public HPolynomialKernel<T>
    {
    public:
        using HPolynomialKernel<T>::descriptor;
        using HPolynomialKernel<T>::feature;
        using HPolynomialKernel<T>::d;
        using HPolynomialKernel<T>::alpha;
        using HPolynomialKernel<T>::lambdas;
        using HPolynomialKernel<T>::plus;
        using HPolynomialKernel<T>::flags;
        using HPolynomialKernel<T>::coeff;
        using HPolynomialKernel<T>::num;

        CenteredHPolynomialKernel2(int d, int plus= 0);

        CenteredHPolynomialKernel2(const CenteredHPolynomialKernel2& s);

        ~CenteredHPolynomialKernel2();

        virtual double feature(Vector<T>& v);
        
        virtual double evaluate(Vector<T>& a, Vector<T>& b);
    };
    
    template<typename T>
    CenteredHPolynomialKernel2<T>::CenteredHPolynomialKernel2(int d, int plus)
    : HPolynomialKernel<T>(d, plus)
    {
        std::stringstream ss;
        ss << "CenteredHPolynomialKernel2 " << d << " " << plus;
        descriptor= ss.str();
    }
    
    template<typename T>
    CenteredHPolynomialKernel2<T>::CenteredHPolynomialKernel2(const CenteredHPolynomialKernel2& c)
    : HPolynomialKernel<T>(c)
    {
    }
    
    template<typename T>
    CenteredHPolynomialKernel2<T>::~CenteredHPolynomialKernel2()
    {
    }
    
    template<typename T>
    double CenteredHPolynomialKernel2<T>::feature(Vector<T>& v)
    {
        float sum= 0;
        for ( unsigned int i= 0; i < v.size(); ++i )
            sum+= v(i);

        return pow(sum, d)/sqrt(pow(v.size(), d) + plus);
    }
    
    template<typename T>
    double CenteredHPolynomialKernel2<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
        return HPolynomialKernel<T>::evaluate(a, b) - (feature(a) * feature(b) / pow(a.size(), float(d)/2));
    }
    
  template<typename T>
  class PETKernel: public KernelFunction<T>
  {
  public:
      using KernelFunction<T>::descriptor;

      PETKernel(int resolution);

      PETKernel(const PETKernel& l);

      ~PETKernel();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);

      int resolution;
      Vector<float> rand1;
      Vector<float> rand2;
  };

  template<typename T>
  PETKernel<T>::PETKernel(int resolution)
  : KernelFunction<T>()
  {
      this->resolution= resolution;
      std::stringstream ss;
      ss << "PETKernel " << resolution;
      descriptor= ss.str();
      for ( int i= 0; i < resolution; ++i )
      {
	rand1.push_back(double(rand())/double(RAND_MAX));
	rand2.push_back(double(rand())/double(RAND_MAX));
      }
      
      for ( unsigned int i= 0; i < rand1.size()-2; ++i )
	printf("%f %f ", rand1(i), rand2(i));
      rand1.push_back(1);
      rand2.push_back(1);
      rand1.push_back(0);
      rand2.push_back(0);
      printf("\n");
  }

  template<typename T>
  PETKernel<T>::PETKernel(const PETKernel& b)
  : KernelFunction<T>(b)
  {
    this->resolution= b.resolution;
  }

  template<typename T>
  PETKernel<T>::~PETKernel()
  {
  }

  template<typename T>
  double PETKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
      double d= 0.0;
      
      double m1= a(0);
      double mu1= a(1);
      double m2= b(0);
      double mu2= b(1);

      double am1= sqrt(1 - m1*m1);
      double am2= sqrt(1 - m2*m2);

      int resolution= this->resolution;

      double du1= am1*2/resolution;
      double du2= am2*2/resolution;

      double e, f, g, h, min1, min2;

      double u1= -am1, u2= -am2;

      double cm1= cos(mu1);
      double cm2= cos(mu2);
      double sm1= sin(mu1);
      double sm2= sin(mu2);
      
      double aa= m1*cm1;
      double bb= m2*cm2;
      double cc= m1*sm1;
      double dd= m2*sm2;
      
      for ( int i= 0; i < resolution+1; ++i )
      {
	u2= -am2;
	//u1= -am1 + rand1(i)*2*am1;
	
	for ( int j= 0; j < resolution+1; ++j )
	{
	  //u2= -am2 + rand2(j)*2*am2;
	  e= aa + u1*sm1;
	  f= bb + u2*sm2;

	  g= cc - u1*cm1;
	  h= dd - u2*cm2;

	  min1= e < f ? e : f;
	  min2= g < h ? g : h;

	  d+= (2 + min1)*(2 + min2);

	  u2+= du2;
	}
	u1+= du1;
      }

      /*d/= resolution*resolution;
      d*= 2*am1*2*am2;*/
      //return d/2.0;
      
      d*= 2*am1*2*am2;
      d/= (resolution+1)*(resolution+1);
      
      return d;
  }
  
  template<typename T>
  class PETKernelMC: public PETKernel<T>
  {
  public:
      using PETKernel<T>::descriptor;
      using PETKernel<T>::resolution;

      PETKernelMC(int resolution);

      PETKernelMC(const PETKernelMC& l);

      ~PETKernelMC();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      Vector<double> randomNumbers;
  };

  template<typename T>
  PETKernelMC<T>::PETKernelMC(int resolution)
  : PETKernel<T>(resolution)
  {
      std::stringstream ss;
      ss << "PETKernelMC " << resolution;
      descriptor= ss.str();
      
      for ( int i= 0; i < 10*resolution*resolution; ++i )
	randomNumbers.push_back(double(rand())/RAND_MAX);
  }

  template<typename T>
  PETKernelMC<T>::PETKernelMC(const PETKernelMC& b)
  : PETKernel<T>(b)
  {
  }

  template<typename T>
  PETKernelMC<T>::~PETKernelMC()
  {
  }

  template<typename T>
  double PETKernelMC<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
      double d= 0.0;
      
      double m1= a(0);
      double mu1= a(1);
      double m2= b(0);
      double mu2= b(1);

      float am1= sqrt(1 - m1*m1);
      float am2= sqrt(1 - m2*m2);

      int resolution= this->resolution;

      double du1= am1*2/resolution;
      double du2= am2*2/resolution;

      double e, f, g, h, min1, min2;

      int resolution2= resolution*resolution;
      double r1, r2;
      int startIdx= rand()%resolution;
      int startIdx2= rand()%resolution;
      double d2= 0;
      double u1, u2;
      
      double cm1= cos(mu1);
      double cm2= cos(mu2);
      double sm1= sin(mu1);
      double sm2= sin(mu2);
      
      double aa= m1*cm1;
      double bb= m2*cm2;
      double cc= m1*sm1;
      double dd= m2*sm2;
      
      for ( int i= 0; i < resolution2; ++i )
      {
	r1= randomNumbers((startIdx + i*2)%randomNumbers.size());
	r2= randomNumbers((startIdx2 + i*2 + 1)%randomNumbers.size());
	
	u1= -am1 + r1*(2*am1);
	u2= -am2 + r2*(2*am2);
	
	e= aa + u1*sm1;
	f= bb + u2*sm2;

	g= cc - u1*cm1;
	h= dd - u2*cm2;

	min1= e < f ? e : f;
	min2= g < h ? g : h;

	d2+= (2 + min1)*(2 + min2);
      }
      
      d2/= resolution2;
      d2*= 2*am1*2*am2;
      
      return d2;
  }

  template<typename T>
  class PETKernelAnalytic: public KernelFunction<T>
  {
  public:
      using KernelFunction<T>::descriptor;

      PETKernelAnalytic();

      PETKernelAnalytic(const PETKernelAnalytic& l);

      ~PETKernelAnalytic();

      double min(double a, double b);
      double max(double a, double b);
      
      double min(double a, double b, double c);
      double max(double a, double b, double c);
      
      double termI1Aint(double m1, double mu1, double m2, double mu2);
      double termI1Bint(double m1, double mu1, double m2, double mu2);
      double termI2Aint(double m1, double mu1, double m2, double mu2);
      double termI2Bint(double m1, double mu1, double m2, double mu2);
      
      double termI1AA1(double m1, double mu1, double m2, double mu2);
      double termI1AA2(double m1, double mu1, double m2, double mu2);
      double termI1AA3(double m1, double mu1, double m2, double mu2);
      double termI1AA4(double m1, double mu1, double m2, double mu2);
      
      double termI1AB1(double m1, double mu1, double m2, double mu2);
      double termI1AB2(double m1, double mu1, double m2, double mu2);
      double termI1AB3(double m1, double mu1, double m2, double mu2);
      double termI1AB4(double m1, double mu1, double m2, double mu2);
      
      double termI1BA1(double m1, double mu1, double m2, double mu2);
      double termI1BA2(double m1, double mu1, double m2, double mu2);
      double termI1BA3(double m1, double mu1, double m2, double mu2);
      double termI1BA4(double m1, double mu1, double m2, double mu2);
      
      double termI1BB1(double m1, double mu1, double m2, double mu2);
      double termI1BB2(double m1, double mu1, double m2, double mu2);
      double termI1BB3(double m1, double mu1, double m2, double mu2);
      double termI1BB4(double m1, double mu1, double m2, double mu2);
      
      virtual double evaluate(Vector<T>& a, Vector<T>& b);
  };

  template<typename T>
  PETKernelAnalytic<T>::PETKernelAnalytic()
  : KernelFunction<T>()
  {
      std::stringstream ss;
      ss << "PETKernelAnalytic ";
      descriptor= ss.str();
  }

  template<typename T>
  PETKernelAnalytic<T>::PETKernelAnalytic(const PETKernelAnalytic& b)
  : KernelFunction<T>(b)
  {
  }

  template<typename T>
  PETKernelAnalytic<T>::~PETKernelAnalytic()
  {
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::min(double a, double b)
  {
    return a < b ? a : b;
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::max(double a, double b)
  {
    return a > b ? a : b;
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::min(double a, double b, double c)
  {
    return min(min(a,b),c);
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::max(double a, double b, double c)
  {
    return max(max(a,b),c);
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::termI1Aint(double m1, double mu1, double m2, double mu2)
  {
    printf("i1a\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= -am2;
      upper1= min(am2, alpha + v*beta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m1*cos(mu1) + u*sin(mu1))*(2 + min(m1*sin(mu1) - u*cos(mu1), (m2*sin(mu2) - v*cos(mu2))))*du*dv;
      }
    }
    
    return d;
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::termI1Bint(double m1, double mu1, double m2, double mu2)
  {
    printf("i1b\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, alpha + v*beta);
      upper1= am2;
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m1*cos(mu1) + u*sin(mu1))*(2 + min(m1*sin(mu1) - u*cos(mu1), (m2*sin(mu2) - v*cos(mu2))))*du*dv;
      }
    }
    
    return d;
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::termI2Aint(double m1, double mu1, double m2, double mu2)
  {
    printf("i2a\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, alpha + v*beta);
      upper1= am2;
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;

      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m2*cos(mu2) + v*sin(mu2))*(2 + min(m1*sin(mu1) - u*cos(mu1), (m2*sin(mu2) - v*cos(mu2))))*du*dv;
      }
    }
    
    return d;
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::termI2Bint(double m1, double mu1, double m2, double mu2)
  {
    printf("i2b\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= -am2;
      upper1= min(am2, alpha + v*beta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m2*cos(mu2) + v*sin(mu2))*(2 + min(m1*sin(mu1) - u*cos(mu1), (m2*sin(mu2) - v*cos(mu2))))*du*dv;
      }
    }
    
    return d;
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::termI1AA1(double m1, double mu1, double m2, double mu2)
  {
    printf("i1aa1\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= -am2;
      upper1= min(am2, alpha + v*beta, gamma - v*delta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m1*cos(mu1) + u*sin(mu1))*(2 + m1*sin(mu1) - u*cos(mu1))*du*dv;
      }
    }
    
    return d;
  }
  template<typename T>
  double PETKernelAnalytic<T>::termI1AA2(double m1, double mu1, double m2, double mu2)
  {
    printf("i1aa2\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, gamma - v*delta);
      upper1= min(am2, alpha + v*beta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m1*cos(mu1) + u*sin(mu1))*(2 + m2*sin(mu2) - v*cos(mu2))*du*dv;
      }
    }
    
    return d;
  }
  template<typename T>
  double PETKernelAnalytic<T>::termI1AA3(double m1, double mu1, double m2, double mu2)
  {
    printf("i1aa3\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, alpha + v*beta);
      upper1= min(am2, gamma - v*delta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m2*cos(mu2) + v*sin(mu2))*(2 + m1*sin(mu1) - u*cos(mu1))*du*dv;
      }
    }
    
    return d;
  }
  template<typename T>
  double PETKernelAnalytic<T>::termI1AA4(double m1, double mu1, double m2, double mu2)
  {
    printf("i1aa4\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, alpha + v*beta, gamma - v*delta);
      upper1= am2;
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m2*cos(mu2) + v*sin(mu2))*(2 + m2*sin(mu2) - v*cos(mu2))*du*dv;
      }
    }
    
    return d;
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::termI1AB1(double m1, double mu1, double m2, double mu2)
  {
    printf("i1ab1\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, gamma - v*delta);
      upper1= min(am2, alpha + v*beta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m1*cos(mu1) + u*sin(mu1))*(2 + m1*sin(mu1) - u*cos(mu1))*du*dv;
      }
    }
    
    return d;
  }
  template<typename T>
  double PETKernelAnalytic<T>::termI1AB2(double m1, double mu1, double m2, double mu2)
  {
    printf("i1ab2\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= -am2;
      upper1= min(am2, alpha + v*beta, gamma - v*delta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m1*cos(mu1) + u*sin(mu1))*(2 + m2*sin(mu2) - v*cos(mu2))*du*dv;
      }
    }
    
    return d;
  }
  template<typename T>
  double PETKernelAnalytic<T>::termI1AB3(double m1, double mu1, double m2, double mu2)
  {
    printf("i1ab3\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, alpha + v*beta, gamma - v*delta);
      upper1= am2;
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m2*cos(mu2) + v*sin(mu2))*(2 + m1*sin(mu1) - u*cos(mu1))*du*dv;
      }
    }
    
    return d;
  }
  template<typename T>
  double PETKernelAnalytic<T>::termI1AB4(double m1, double mu1, double m2, double mu2)
  {
    printf("i1ab4\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, alpha + v*beta);
      upper1= min(am2, gamma - v*delta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m2*cos(mu2) + v*sin(mu2))*(2 + m2*sin(mu2) - v*cos(mu2))*du*dv;
      }
    }
    
    return d;
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::termI1BA1(double m1, double mu1, double m2, double mu2)
  {
    printf("i1ba1\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, alpha + v*beta);
      upper1= min(am2, gamma - v*delta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m1*cos(mu1) + u*sin(mu1))*(2 + m1*sin(mu1) - u*cos(mu1))*du*dv;
      }
    }
    
    return d;
  }
  template<typename T>
  double PETKernelAnalytic<T>::termI1BA2(double m1, double mu1, double m2, double mu2)
  {
    printf("i1ba2\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, alpha + v*beta, gamma - v*delta);
      upper1= am2;
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m1*cos(mu1) + u*sin(mu1))*(2 + m2*sin(mu2) - v*cos(mu2))*du*dv;
      }
    }
    
    return d;
  }
  template<typename T>
  double PETKernelAnalytic<T>::termI1BA3(double m1, double mu1, double m2, double mu2)
  {
    printf("i1ba3\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= -am2;
      upper1= min(am2, alpha + v*beta, gamma - v*delta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m2*cos(mu2) + v*sin(mu2))*(2 + m1*sin(mu1) - u*cos(mu1))*du*dv;
      }
    }
    
    return d;
  }
  template<typename T>
  double PETKernelAnalytic<T>::termI1BA4(double m1, double mu1, double m2, double mu2)
  {
    printf("i1ba4\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, gamma - v*delta);
      upper1= min(am2, alpha + v*beta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m2*cos(mu2) + v*sin(mu2))*(2 + m2*sin(mu2) - v*cos(mu2))*du*dv;
      }
    }
    
    return d;
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::termI1BB1(double m1, double mu1, double m2, double mu2)
  {
    printf("i1bb1\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, alpha + v*beta, gamma - v*delta);
      upper1= am2;
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m1*cos(mu1) + u*sin(mu1))*(2 + m1*sin(mu1) - u*cos(mu1))*du*dv;
      }
    }
    
    return d;
  }
  template<typename T>
  double PETKernelAnalytic<T>::termI1BB2(double m1, double mu1, double m2, double mu2)
  {
    printf("i1bb2\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(am2, alpha + v*beta);
      upper1= min(am2, gamma - v*delta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m1*cos(mu1) + u*sin(mu1))*(2 + m2*sin(mu2) - v*cos(mu2))*du*dv;
      }
    }
    
    return d;
  }
  template<typename T>
  double PETKernelAnalytic<T>::termI1BB3(double m1, double mu1, double m2, double mu2)
  {
    printf("i1bb3\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= max(-am2, gamma - v*delta);
      upper1= min(am2, alpha + v*beta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m2*cos(mu2) + v*sin(mu2))*(2 + m1*sin(mu1) - u*cos(mu1))*du*dv;
      }
    }
    
    return d;
  }
  template<typename T>
  double PETKernelAnalytic<T>::termI1BB4(double m1, double mu1, double m2, double mu2)
  {
    printf("i1bb4\n");
    double am1= sqrt(1 - m1*m1);
    double am2= sqrt(1 - m2*m2);
    double alpha= (m2*cos(mu2) - m1*cos(mu1))/sin(mu1);
    double beta= sin(mu2)/sin(mu1);
    double gamma= -(m2*sin(mu2) - m1*sin(mu1))/cos(mu1);
    double delta= -cos(mu2)/cos(mu1);
    
    double lower2= -am1;
    double upper2= am1;
    double lower1, upper1;
    
    if ( fabs(upper2 - lower2) < 1e-10 )
      return 0;
    
    double d= 0;
    
    int n= 4000;
    
    double dv= (upper2 - lower2)/n;
    
    for ( double v= lower2; v < upper2; v+= dv)
    {
      lower1= -am2;
      upper1= min(am2, alpha + v*beta, gamma - v*delta);
      
      if ( fabs(upper1 - lower1) < 1e-10 )
	continue;
      
      double du= (upper1 - lower1)/n;
      for ( double u= lower1; u < upper1; u+= du)
      {
	d+= (2 + m2*cos(mu2) + v*sin(mu2))*(2 + m2*sin(mu2) - v*cos(mu2))*du*dv;
      }
    }
    
    return d;
  }
  
  template<typename T>
  double PETKernelAnalytic<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
      double d= 0.0;
      
      float m1= a(0);
      float mu1= a(1);
      float m2= b(0);
      float mu2= b(1);

      double term1, term2, term3, term4;
      double terma, termb;
      
      if ( sin(mu1) > 0 )
      {
	terma= termI1Aint(m1, mu1, m2, mu2);
	termb= termI2Aint(m1, mu1, m2, mu2);
	d= terma + termb;
	printf("%f %f\n", terma, termb);
	printf("%f\n", terma + termb);
      }
      
      if ( sin(mu1) > 0 && cos(mu1) < 0 )
      {
	term1= termI1AA1(m1, mu1, m2, mu2);
	term2= termI1AA2(m1, mu1, m2, mu2);
	term3= termI1AA3(m1, mu1, m2, mu2);
	term4= termI1AA4(m1, mu1, m2, mu2);
	printf("%f %f\n", term1 + term2, term3 + term4);
	printf("%f %f %f %f\n", term1, term2, term3, term4);
      }
      else if ( sin(mu1) > 0 && cos(mu1) > 0 )
      {
	term1= termI1AB1(m1, mu1, m2, mu2);
	term2= termI1AB2(m1, mu1, m2, mu2);
	term3= termI1AB3(m1, mu1, m2, mu2);
	term4= termI1AB4(m1, mu1, m2, mu2);
	printf("%f %f\n", term1 + term2, term3 + term4);
	printf("%f %f %f %f\n", term1, term2, term3, term4);
      }
      else if ( sin(mu1) < 0 && cos(mu1) < 0 )
      {
	term1= termI1BA1(m1, mu1, m2, mu2);
	term2= termI1BA2(m1, mu1, m2, mu2);
	term3= termI1BA3(m1, mu1, m2, mu2);
	term4= termI1BA4(m1, mu1, m2, mu2);
      }
      else if ( sin(mu1) < 0 && cos(mu1) > 0 )
      {
	term1= termI1BB1(m1, mu1, m2, mu2);
	term2= termI1BB2(m1, mu1, m2, mu2);
	term3= termI1BB3(m1, mu1, m2, mu2);
	term4= termI1BB4(m1, mu1, m2, mu2);
      }
      
      d= term1 + term2 + term3 + term4;
      
      return d;
  }
  
  template<typename T>
  class PETCrossKernel: public KernelFunction<T>
  {
  public:
      using KernelFunction<T>::descriptor;

      PETCrossKernel(int resolution= 1000);

      PETCrossKernel(const PETCrossKernel& l);

      ~PETCrossKernel();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);

      int resolution;
  };

  template<typename T>
  PETCrossKernel<T>::PETCrossKernel(int resolution)
  : KernelFunction<T>()
  {
      this->resolution= resolution;
    
      std::stringstream ss;
      ss << "PETCrossKernel " << resolution;
      descriptor= ss.str();
  }

  template<typename T>
  PETCrossKernel<T>::PETCrossKernel(const PETCrossKernel& b)
  : KernelFunction<T>(b)
  {
    this->resolution= b.resolution;
  }

  template<typename T>
  PETCrossKernel<T>::~PETCrossKernel()
  {
  }

  template<typename T>
  double PETCrossKernel<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
      double d= 0.0;
      
      float m1= a(0);
      float mu1= a(1);
      float x= b(0);
      float y= b(1);

      float am1= sqrt(1 - m1*m1);

      int resolution= this->resolution;

      float du1= am1*2/resolution;

      float e, g, min1, min2;

      float u1= -am1;
      for ( int i= 0; i < resolution; ++i )
      {
	e= m1*cos(mu1) + u1*sin(mu1);
	g= m1*sin(mu1) - u1*cos(mu1);

	min1= e < x ? e : x;
	min2= g < y ? g : y;

	d+= (2 + min1)*(2 + min2)*du1;

	u1+= du1;
      }
      
      return d;
  }
  
  template<typename T>
  class PETCrossKernelMC: public PETCrossKernel<T>
  {
  public:
      using KernelFunction<T>::descriptor;

      PETCrossKernelMC(int resolution);

      PETCrossKernelMC(const PETCrossKernelMC& l);

      ~PETCrossKernelMC();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);

      int resolution;
      Vector<float> randomNumbers;
  };

  template<typename T>
  PETCrossKernelMC<T>::PETCrossKernelMC(int resolution)
  : PETCrossKernel<T>()
  {
      this->resolution= resolution;
    
      for ( int i= 0; i < 10*resolution*resolution; ++i )
	randomNumbers.push_back(float(rand())/RAND_MAX);
      
      std::stringstream ss;
      ss << "PETCrossKernelMC " << resolution;
      descriptor= ss.str();
  }

  template<typename T>
  PETCrossKernelMC<T>::PETCrossKernelMC(const PETCrossKernelMC& b)
  : PETCrossKernel<T>(b)
  {
    this->resolution= b.resolution;
  }

  template<typename T>
  PETCrossKernelMC<T>::~PETCrossKernelMC()
  {
  }

  template<typename T>
  double PETCrossKernelMC<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
      double d= 0.0;
      
      float m1= a(0);
      float mu1= a(1);
      float x= b(0);
      float y= b(1);

      float am1= sqrt(1 - m1*m1);

      int resolution= this->resolution;



      float e, g, min1, min2;

      float u1= -am1;
      

      double r1;
      int startIdx= rand()%randomNumbers.size();
      for ( int i= 0; i < resolution; ++i )
      {
	//r1= double(rand())/RAND_MAX;
	r1= randomNumbers((startIdx + i*2)%randomNumbers.size());
	
	u1= -am1 + (2*am1)*r1;
	
	e= m1*cos(mu1) + u1*sin(mu1);
	g= m1*sin(mu1) - u1*cos(mu1);

	min1= e < x ? e : x;
	min2= g < y ? g : y;

	d+= (2 + min1)*(2 + min2);
      }
      
      d/= resolution;
      d*= 2*am1;

      return d;
  }
  
  template<typename T>
  class PETCrossKernelAnalytic: public PETCrossKernel<T>
  {
  public:
      using KernelFunction<T>::descriptor;

      PETCrossKernelAnalytic();

      PETCrossKernelAnalytic(const PETCrossKernelAnalytic& l);

      ~PETCrossKernelAnalytic();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      double analyticTerm1a(float am, float m, float mu, float x, float y);
      double analyticTerm1b(float am, float m, float mu, float x, float y);
      
      double min(double a, double b);
      double max(double a, double b);
      double min(double a, double b, double c);
      double max(double a, double b, double c);
      
      double analyticCaseITerm1(float am, float m, float mu, float x, float y);
      double analyticCaseITerm2(float am, float m, float mu, float x, float y);
      double analyticCaseITerm1(float lower, float upper, float m, float mu, float x, float y);
      double analyticCaseITerm2(float lower, float upper, float m, float mu, float x, float y);
      double analyticCaseITerm3(float am, float m, float mu, float x, float y);
      double analyticCaseITerm4(float am, float m, float mu, float x, float y);
      double analyticCaseITerm3(float lower, float upper, float m, float mu, float x, float y);
      double analyticCaseITerm4(float lower, float upper, float m, float mu, float x, float y);
      double analyticCaseIII1(float am, float m, float mu, float x, float y);
      double analyticCaseIII2(float am, float m, float mu, float x, float y);
      double analyticCaseIV1(float am, float m, float mu, float x, float y);
      double analyticCaseIV2(float am, float m, float mu, float x, float y);
      double analyticCaseIII1(float lower, float upper, float m, float mu, float x, float y);
      double analyticCaseIV1(float lower, float upper, float m, float mu, float x, float y);
      double analyticCaseIII2(float lower, float upper, float m, float mu, float x, float y);
      double analyticCaseIV2(float lower, float upper, float m, float mu, float x, float y);
  };

  template<typename T>
  PETCrossKernelAnalytic<T>::PETCrossKernelAnalytic()
  : PETCrossKernel<T>()
  {
      std::stringstream ss;
      ss << "PETCrossKernelAnalytic ";
      descriptor= ss.str();
  }

  template<typename T>
  PETCrossKernelAnalytic<T>::PETCrossKernelAnalytic(const PETCrossKernelAnalytic& b)
  : PETCrossKernel<T>(b)
  {
  }

  template<typename T>
  PETCrossKernelAnalytic<T>::~PETCrossKernelAnalytic()
  {
  }

  template<typename T>
  double PETCrossKernelAnalytic<T>::min(double a, double b)
  {
    return a < b ? a : b;
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::max(double a, double b)
  {
    return a > b ? a : b;
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::min(double a, double b, double c)
  {
    return min(a, min(b,c));
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::max(double a, double b, double c)
  {
    return max(a, max(b,c));
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticTerm1a(float am, float m, float mu, float x, float y)
  {
    double d= 0;
    double alpha= (x - m*cos(mu))/sin(mu);
    double beta= (m*sin(mu) - y)/cos(mu);
    double lower= -am;
    double upper= min(alpha, am);
    
    //printf("term1a: %f %f\n", lower, upper);
    
    if ( upper <= lower )
      return 0;
    
    int n= 2000;
    double du= (upper - lower)/2000;
    int minflag0= 0;
    int minflag1= -1;
    for ( double u= lower; u < upper; u+= du )
    {
      double min1= (m*sin(mu) - u*cos(mu)) < y ? (m*sin(mu) - u*cos(mu)) : y;
      minflag0= (m*sin(mu) - u*cos(mu)) < y ? 0 : 1;
      
      /*if ( minflag1 != -1 && minflag1 != minflag0 )
	printf("threshold term1a: %f, flag: %d, %d\n", u, minflag0, minflag1);*/
      
      d+= (2 + m*cos(mu) + u*sin(mu))*(2 + min1)*du;
      
      minflag1= minflag0;
    }
    
    return d;
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticTerm1b(float am, float m, float mu, float x, float y)
  {
    double d= 0;
    double alpha= (x - m*cos(mu))/sin(mu);
    double beta= (m*sin(mu) - y)/cos(mu);
    double lower= max(alpha, -am);
    double upper= am;
    
    if ( upper < lower )
      return 0;
    
    int n= 1000000;
    for ( int i= 0; i < n; ++i )
    {
      double r= double(rand())/double(RAND_MAX);
      
      double u= lower + (upper - lower)*r;
      double min1= (m*sin(mu) - u*cos(mu)) < y ? (m*sin(mu) - u*cos(mu)) : y;
      
      d+= (2 + x)*(2 + min1);
    }
    
    d*= (upper - lower);
    d/= n;
    
    return d;
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseITerm1(float am, float m, float mu, float x, float y)
  {
    
    double alpha= (x - m*cos(mu))/sin(mu);
    double beta= (m*sin(mu) - y)/cos(mu);
    
    double lower= 0, upper= 0;
    
    if ( sin(mu) > 0 && cos(mu) > 0 )
    {
      lower= max(beta, -am);
      upper= min(am, alpha);
    }
    if ( sin(mu) > 0 && cos(mu) < 0 )
    {
      lower= -am;
      upper= min(am, alpha, beta);
    }
    if ( sin(mu) < 0 && cos(mu) > 0 )
    {
      lower= max(alpha, -am, beta);
      upper= am;
    }
    if ( sin(mu) < 0 && cos(mu) < 0 )
    {
      lower= max(alpha, -am);
      upper= min(am, beta);
    }
    
    //printf("term1: %f %f\n", lower, upper);
    
    return analyticCaseITerm1(lower, upper, m, mu, x, y);
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseITerm2(float am, float m, float mu, float x, float y)
  {
    
    double alpha= (x - m*cos(mu))/sin(mu);
    double beta= (m*sin(mu) - y)/cos(mu);

    double lower= 0, upper= 0;
    
    if ( sin(mu) > 0 && cos(mu) > 0 )
    {
      lower= -am;
      upper= min(beta, am, alpha);
    }
    if ( sin(mu) > 0 && cos(mu) < 0 )
    {
      lower= max(-am, beta);
      upper= min(am, alpha);
    }
    if ( sin(mu) < 0 && cos(mu) > 0 )
    {
      lower= max(alpha, -am);
      upper= min(am, beta);
    }
    if ( sin(mu) < 0 && cos(mu) < 0 )
    {
      lower= max(alpha, -am, beta);
      upper= am;
    }
    
    //printf("term2: %f %f\n", lower, upper);
    
    return analyticCaseITerm2(lower, upper, m, mu, x, y);
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseITerm3(float am, float m, float mu, float x, float y)
  {
    
    double alpha= (x - m*cos(mu))/sin(mu);
    double beta= (m*sin(mu) - y)/cos(mu);

    double lower= 0, upper= 0;
    
    if ( sin(mu) > 0 && cos(mu) > 0 )
    {
      lower= max(alpha, -am, beta);
      upper= am;
    }
    if ( sin(mu) > 0 && cos(mu) < 0 )
    {
      lower= max(alpha, -am);
      upper= min(am, beta);
    }
    if ( sin(mu) < 0 && cos(mu) > 0 )
    {
      lower= max(-am, beta);
      upper= min(am, alpha);
    }
    if ( sin(mu) < 0 && cos(mu) < 0 )
    {
      lower= -am;
      upper= min(am, alpha, beta);
    }
    
    //printf("term3: %f %f\n", lower, upper);
    
    return analyticCaseITerm3(lower, upper, m, mu, x, y);
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseITerm4(float am, float m, float mu, float x, float y)
  {
    
    double alpha= (x - m*cos(mu))/sin(mu);
    double beta= (m*sin(mu) - y)/cos(mu);

    double lower= 0, upper= 0;
    
    if ( sin(mu) > 0 && cos(mu) > 0 )
    {
      lower= max(alpha, -am);
      upper= min(beta, am);
    }
    if ( sin(mu) > 0 && cos(mu) < 0 )
    {
      lower= max(alpha, -am, beta);
      upper= am;
    }
    if ( sin(mu) < 0 && cos(mu) > 0 )
    {
      lower= -am;
      upper= min(am, alpha, beta);
    }
    if ( sin(mu) < 0 && cos(mu) < 0 )
    {
      lower= max(-am, beta);
      upper= min(am, alpha);
    }
    
    //printf("term4: %f %f\n", lower, upper);
    
    return analyticCaseITerm4(lower, upper, m, mu, x, y);
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseIII1(float am, float m, float mu, float x, float y)
  {
    
    double gamma= -y/cos(mu);
    
    double lower, upper;
    
    if ( cos(mu) > 0 )
    {
      lower= max(gamma, -am);
      upper= am;
    }
    else
    {
      lower= -am;
      upper= min(gamma, am);
    }
    
    return analyticCaseIII1(lower, upper, m, mu, x, y);
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseIII2(float am, float m, float mu, float x, float y)
  {
    
    double gamma= -y/cos(mu);
    
    double lower, upper;
    
    if ( cos(mu) > 0 )
    {
      lower= -am;
      upper= min(gamma, am);
    }
    else
    {
      lower= max(gamma, -am);
      upper= am;
    }
    
    return analyticCaseIII2(lower, upper, m, mu, x, y);
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseIII1(float lower, float upper, float m, float mu, float x, float /*y*/)
  {
    double d= 0;
    
    if ( upper <= lower )
      return 0;
    
    /*int n= 2000;
    double du= (upper - lower)/n;
    for ( double u= lower; u < upper; u+= du )
      d+= (4 + 2*min(m*cos(mu), x) + (2 + min(m*cos(mu),x))*(-u*cos(mu)))*du;*/
    
    d= (4 + 2*min(m*cos(mu),x))*(upper - lower);
    d+= (2 + min(m*cos(mu),x))*(-cos(mu))*0.5*(upper*upper - lower*lower);
    
    return d;
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseIII2(float lower, float upper, float m, float mu, float x, float y)
  {
    double d= 0;
    
    if ( upper <= lower )
      return 0;
    
    /*int n= 2000;
    double du= (upper - lower)/n;
    for ( double u= lower; u < upper; u+= du )
      d+= (4 + 2*min(m*cos(mu), x) + (2 + min(m*cos(mu),x))*(y))*du;*/
    
    d= (4 + 2*min(m*cos(mu),x) + (2 + min(m*cos(mu), x))*y)*(upper - lower);
    
    return d;
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseIV1(float am, float m, float mu, float x, float y)
  {
    
    double delta= x/sin(mu);
    
    double lower, upper;
    
    if ( sin(mu) > 0 )
    {
      lower= -am;
      upper= min(am, delta);
    }
    else
    {
      lower= max(-am, delta);
      upper= am;
    }
    
    return analyticCaseIV1(lower, upper, m, mu, x, y);
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseIV2(float am, float m, float mu, float x, float y)
  {
    
    double delta= x/sin(mu);
    
    double lower, upper;
    
    if ( sin(mu) > 0 )
    {
      lower= max(-am, delta);
      upper= am;
    }
    else
    {
      lower= -am;
      upper= min(am, delta);
    }
    
    return analyticCaseIV2(lower, upper, m, mu, x, y);
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseITerm1(float lower, float upper, float m, float mu, float /*x*/, float /*y*/)
  {
    double d= 0;
    
    if ( upper <= lower )
      return 0;
    
    /*int n= 2000;
    double du= (upper - lower)/n;
    for ( double u= lower; u < upper; u+= du )
      d+= (2 + m*cos(mu) + u*sin(mu))*(2 + m*sin(mu) - u*cos(mu))*du;*/
    
    d= (4 + 2*m*cos(mu) + 2*m*sin(mu) + m*m*cos(mu)*sin(mu))*(upper - lower);
    d+= (2*sin(mu) + m*sin(mu)*sin(mu) - 2*cos(mu) - m*cos(mu)*cos(mu))*0.5*(upper*upper - lower*lower);
    d+= -(sin(mu)*cos(mu))*1.0/3.0*(upper*upper*upper - lower*lower*lower);
    
    return d;
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseITerm2(float lower, float upper, float m, float mu, float /*x*/, float y)
  {
    double d= 0;
    
    if ( upper <= lower )
      return 0;
    
    /*int n= 2000;
    double du= (upper - lower)/n;
    for ( double u= lower; u < upper; u+= du )
      d+= (2 + m*cos(mu) + u*sin(mu))*(2 + y)*du;*/
    
    d= (4 + 2*y + 2*m*cos(mu) + y*m*cos(mu))*(upper - lower);
    d+= (2*sin(mu) + y*sin(mu))*0.5*(upper*upper - lower*lower);
    
    return d;
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseITerm3(float lower, float upper, float m, float mu, float x, float /*y*/)
  {
    double d= 0;
    
    if ( upper <= lower )
      return 0;
    
    /*int n= 2000;
    double du= (upper - lower)/n;
    for ( double u= lower; u < upper; u+= du )
      d+= (2 + x)*(2 + m*sin(mu) - u*cos(mu))*du;*/
    
    d= (4 + 2*m*sin(mu) + 2*x + m*x*sin(mu))*(upper - lower);
    d+= -(2*cos(mu) + x*cos(mu))*0.5*(upper*upper - lower*lower);
    
    return d;
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseITerm4(float lower, float upper, float /*m*/, float /*mu*/, float x, float y)
  {
    double d= 0;
    
    if ( upper <= lower )
      return 0;
    
    /*int n= 2000;
    double du= (upper - lower)/n;
    for ( double u= lower; u < upper; u+= du )
      d+= (2 + x)*(2 + y)*du;*/
    
    d= (2 + x)*(2 + y)*(upper - lower);
    
    return d;
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseIV1(float lower, float upper, float m, float mu, float /*x*/, float y)
  {
    double d= 0;
    
    if ( upper <= lower )
      return 0;
    
    /*int n= 2000;
    double du= (upper - lower)/n;
    for ( double u= lower; u < upper; u+= du )
      d+= (4 + 2*min(m*sin(mu), y) + (2 + min(m*sin(mu),y))*(u*sin(mu)))*du;*/
    
    d= (4 + 2*min(m*sin(mu), y))*(upper - lower);
    d+= (2 + min(m*sin(mu), y))*sin(mu)*0.5*(upper*upper - lower*lower);
    
    return d;
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::analyticCaseIV2(float lower, float upper, float m, float mu, float x, float y)
  {
    double d= 0;
    
    if ( upper <= lower )
      return 0;
    
    /*int n= 2000;
    double du= (upper - lower)/n;
    for ( double u= lower; u < upper; u+= du )
      d+= (4 + 2*min(m*sin(mu), y) + (2 + min(m*sin(mu),y))*(x))*du;*/
    
    d= (4 + 2*min(m*sin(mu), y) + (2 + min(m*sin(mu),y))*x)*(upper - lower);
    
    return d;
  }
  
  template<typename T>
  double PETCrossKernelAnalytic<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
      double d= 0.0;
      
      double m1= a(0);
      double mu1= a(1);
      double x= b(0);
      double y= b(1);

      double am1= sqrt(1 - m1*m1);
      
      /*if ( sin(mu1) == 0 && cos(mu1) == 0 )
	return (2 + min(0, x))*(2 + min(0, y))*2*am1;
      else if ( sin(mu1) == 0 )
	return analyticCaseIII1(am1, m1, mu1, x, y) + analyticCaseIII2(am1, m1, mu1, x, y);
      else if ( cos(mu1) == 0 )
	return analyticCaseIV1(am1, m1, mu1, x, y) + analyticCaseIV2(am1, m1, mu1, x, y);*/
      
      if ( fabs(sin(mu1)) < 0.001 && fabs(cos(mu1)) < 0.001 )
	return (2 + min(0, x))*(2 + min(0, y))*2*am1;
      else if ( fabs(sin(mu1)) < 0.001 )
	return analyticCaseIII1(am1, m1, mu1, x, y) + analyticCaseIII2(am1, m1, mu1, x, y);
      else if ( fabs(cos(mu1)) < 0.00 )
	return analyticCaseIV1(am1, m1, mu1, x, y) + analyticCaseIV2(am1, m1, mu1, x, y);
      
      double term1= analyticCaseITerm1(am1, m1, mu1, x, y);
      double term2= analyticCaseITerm2(am1, m1, mu1, x, y);
      double term3= analyticCaseITerm3(am1, m1, mu1, x, y);
      double term4= analyticCaseITerm4(am1, m1, mu1, x, y);
      
      d= term1 + term2 + term3 + term4;

      return d;
  }
}

#endif	/* KERNELFUNCTIONS_H */


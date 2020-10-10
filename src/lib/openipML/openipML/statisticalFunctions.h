#ifndef STATISTICALFUNCTIONS_H
#define STATISTICALFUNCTIONS_H

#include <openipDS/mathFunctions.h>

extern int errorInDistributionArgs;

namespace openip
{
    //#ifndef ERRORINDISTRIBUTIONARGS_
    //#define ERRORINDISTRIBUTIONARGS_

    //#endif

    double gammaln(const double xx);

    double factorial(const int n);

    double factorialln(const int n);

    double binomialcoefficient(const int n, const int k);

    double betaFunction(const double z, const double w);

    class Gauleg18
    {
    public:
        static const int ngau= 18;
        static const double y[18];
        static const double w[18];
    };

    class Gamma: public Gauleg18
    {
    public:
        static const int ASWITCH= 100;
        static const double EPS;
        static const double FPMIN;
        double gln;

        double gammp(const double a, const double x);

        double gammq(const double a, const double x );

        double gser(const double a, const double x );

        double gcf(const double a, const double x);

        double gammpapprox(double a, double x, int psig);

        double invgammp(double p, double a);
    };

    class Erf{
    public:
        static const int ncof= 28;
        static const double cof[28];

        inline double erf(double x);

        inline double erfc(double x);

        double erfccheb(double z );

        double inverfc(double p);

        inline double inverf(double p);
    };

    double erfcc(const double x);

    double expint(const int n, const double x);

    double ei(const double x);

    class Beta: Gauleg18
    {
    public:
        static const int SWITCH= 3000;
        static const double EPS, FPMIN;

        double betai(const double a, const double b, const double x);

        double betacf(const double a, const double b, const double x);

        double betaiapprox(double a, double b, double x);

        double invbetai(double p, double a, double b);
    };

    class Normaldist: public Erf
    {
    public:
        double mu, sig;
        Normaldist(double mmu= 0., double ssig= 1.);

        double p(double x);

        double cdf(double x);

        double invcdf(double p);
    };

    class Cauchydist
    {
    public:
        double mu, sig;
        Cauchydist(double mmu= 0., double ssig= 1.);

        double p(double x);

        double cdf(double x);

        double invcdf(double p);
    };

    class Studenttdist: public Beta
    {
    public:
        double nu, mu, sig, np, fac;
        Studenttdist(double nnu, double mmu= 0., double ssig= 1.);

        double p(double t);

        double cdf(double t);

        double invcdf(double p);

        double aa(double t);

        double invaa(double p);
    };

    class Logisticdist
    {
    public:
        double mu, sig;
        Logisticdist(double mmu= 0., double ssig= 1.);

        double p(double x);

        double cdf(double x);

        double invcdf(double p);
    };

    class Expondist
    {
    public:
        double bet;
        Expondist(double bbet);

        double p(double x);

        double cdf(double x);

        double invcdf(double p);
    };

    class Lognormaldist: public Erf
    {
    public:
        double mu, sig;
        Lognormaldist(double mmu= 0., double ssig= 1.);

        double p(double x);

        double cdf(double x);

        double invcdf(double p);
    };

    class Chisquaredist: public Gamma
    {
    public:
        double nu, fac;
        Chisquaredist(double nnu);

        double p(double x2);

        double cdf(double x2);

        double invcdf(double p );
    };

    class Gammadist: public Gamma
    {
    public:
        double alph, bet, fac;
        Gammadist(double aalph, double bbet= 1. );

        double p(double x);

        double cdf(double x);

        double invcdf(double p);
    };

    class Fdist: public Beta
    {
    public:
        double nu1, nu2;
        double fac;
        Fdist(double nnu1, double nnu2);

        double p(double f);

        double cdf(double f);

        double invcdf(double p);
    };

    class Betadist: public Beta
    {
    public:
        double alph, bet, fac;
        Betadist(double aalph, double bbet);

        double p( double x );

        double cdf(double x);

        double invcdf(double p);
    };

    class KSdist
    {
    public:
        double pks(double z);

        double qks(double z);

        double invqks(double q);

        double invpks(double p);
    };

    class Poissondist: public Gamma
    {
    public:
        double lam;
        Poissondist(double llam);

        double p(int n);

        double cdf(int n);

        int invcdf(double p);
    };

    class Binomialdist: public Beta
    {
    public:
        int n;
        double pe, fac;
        Binomialdist(int nn, double ppe);

        double p(int k);

        double cdf(int k);

        int invcdf(double p );
    };

    double invxlogx(double y);
}

#endif // STATISTICALFUNCTIONS_H

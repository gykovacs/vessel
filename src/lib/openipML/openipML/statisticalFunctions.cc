#include <openipML/statisticalFunctions.h>
#include <openipDS/Stopper.h>

#include <limits>
#include <vector>

#define throw(X) (printf(X))
#define SQR(X) (X*X)

using namespace std;

int errorInDistributionArgs= 0;

namespace openip
{
        double gammaln(const double xx)
    {
        int j;
        double x, tmp, y, ser;
        static const double cof[14]= {57.1562356658629235,-59.5979603554754912, 14.1360979747417471,-0.491913816197620199, .339946499848118887e-4,
                                      .465236289270485756e-4,-.983744753048795646e-4, .158088703224912494e-3,-.210264441724104883e-3,.217439618115212643e-3,
                                      -.164318106536763890e-3,.844182239838527433e-4,-.261908384015814087e-4,.368991826595316234e-5};
        if ( xx <= 0 )
        {
            errorInDistributionArgs= 1;
            return tprintf("bad arg in gammaln");
        }
        y= x= xx;
        tmp= x + 5.24218750000000000;
        tmp= (x + 0.5)*log(tmp)-tmp;
        ser= 0.999999999999997092;
        for ( j= 0; j < 14; ++j )
            ser+= cof[j]/++y;
        return tmp+log(2.5066282746310005*ser/x);
    }

    double factorial(const int n)
    {
        static vector<double> a(171);
        static bool init= true;
        if ( init )
        {
            init= false;
            a[0]= 1.;
            for ( int i= 1; i < 171; ++i )
                a[i]= i*a[i-1];
        }
        if ( n < 0 || n > 170 )
        {
            errorInDistributionArgs= 1;
            return tprintf("factorial out of range");
        }
        return a[n];
    }

    double factorialln(const int n)
    {
        static const int NTOP= 2000;
        static vector<double> a(NTOP);
        static bool init= true;
        if ( init )
        {
            init= false;
            for ( int i= 0; i < NTOP; ++i )
                a[i]= gammaln(i+1.);
        }
        if ( n < 0 )
        {
            errorInDistributionArgs= 1;
            return tprintf("negative arg in factln");
        }
        if ( n < NTOP )
            return a[n];
        return gammaln(n+1.);
    }

    double binomialcoefficient(const int n, const int k)
    {
        if ( n < 0 || k < 0 || k > n )  return tprintf("bad args in binomialcoefficient");
        if ( n < 171 )  return floor(0.5 + factorialln(n)/(factorialln(k)*factorialln(n-k)));
        return floor(0.5 + exp(factorialln(n) - factorialln(k) - factorialln(n-k)));
    }

    double betaFunction(const double z, const double w)
    {
        return exp(gammaln(z) + gammaln(w) - gammaln(z+w));
    }

    const double Gauleg18::y[18] = {0.0021695375159141994,
    0.011413521097787704,0.027972308950302116,0.051727015600492421,
    0.082502225484340941, 0.12007019910960293,0.16415283300752470,
    0.21442376986779355, 0.27051082840644336, 0.33199876341447887,
    0.39843234186401943, 0.46931971407375483, 0.54413605556657973,
    0.62232745288031077, 0.70331500465597174, 0.78649910768313447,
    0.87126389619061517, 0.95698180152629142};
    const double Gauleg18::w[18] = {0.0055657196642445571,
    0.012915947284065419,0.020181515297735382,0.027298621498568734,
    0.034213810770299537,0.040875750923643261,0.047235083490265582,
    0.053244713977759692,0.058860144245324798,0.064039797355015485,
    0.068745323835736408,0.072941885005653087,0.076598410645870640,
    0.079687828912071670,0.082187266704339706,0.084078218979661945,
    0.085346685739338721,0.085983275670394821};

        double Gamma::gammp(const double a, const double x)
        {
            if (x < 0.0 || a <= 0.0) { errorInDistributionArgs= 1; return tprintf("bad args in gammp");  }
                            if (x == 0.0) return 0.0;
                            else if ((int)a >= ASWITCH) return gammpapprox(a,x,1);
                            else if (x < a+1.0) return gser(a,x);
                            else return 1.0-gcf(a,x);
        }

        double Gamma::gammq(const double a, const double x )
        {
            if (x < 0.0 || a <= 0.0) { errorInDistributionArgs= 1; return tprintf("bad args in gammq");  }
                            if (x == 0.0) return 1.0;
                            else if ((int)a >= ASWITCH) return gammpapprox(a,x,0);
                            else if (x < a+1.0) return 1.0-gser(a,x);
                            else return gcf(a,x);
        }

        double Gamma::gser(const double a, const double x )
        {
            double sum,del,ap;
                            gln=gammaln(a);
                            ap=a;
                            del=sum=1.0/a;
                            for (;;) {
                                    ++ap;
                                    del *= x/ap;
                                    sum += del;
                                    if (fabs(del) < fabs(sum)*EPS) {
                                            return sum*exp(-x+a*log(x)-gln);
                                    }
                            }
        }

        double Gamma::gcf(const double a, const double x)
        {
            int i;
                            double an,b,c,d,del,h;
                            gln=gammaln(a);
                            b=x+1.0-a;
                            c=1.0/FPMIN;
                            d=1.0/b;
                            h=d;
                            for (i=1;;i++) {
                                    an = -i*(i-a);
                                    b += 2.0;
                                    d=an*d+b;
                                    if (fabs(d) < FPMIN) d=FPMIN;
                                    c=b+an/c;
                                    if (fabs(c) < FPMIN) c=FPMIN;
                                    d=1.0/d;
                                    del=d*c;
                                    h *= del;
                                    if (fabs(del-1.0) <= EPS) break;
                            }
                            return exp(-x+a*log(x)-gln)*h;
        }

        double Gamma::gammpapprox(double a, double x, int psig)
        {
            int j;
                            double xu,t,sum,ans;
                            double a1 = a-1.0, lna1 = log(a1), sqrta1 = sqrt(a1);
                            gln = gammaln(a);
                            if (x > a1) xu = MAX(a1 + 11.5*sqrta1, x + 6.0*sqrta1);
                            else xu = MAX(0.,MIN(a1 - 7.5*sqrta1, x - 5.0*sqrta1));
                            sum = 0;
                            for (j=0;j<ngau;j++) {
                                    t = x + (xu-x)*y[j];
                                    sum += w[j]*exp(-(t-a1)+a1*(log(t)-lna1));
                            }
                            ans = sum*(xu-x)*exp(a1*(lna1-1.)-gln);
                            return (psig?(ans>0.0? 1.0-ans:-ans):(ans>=0.0? ans:1.0+ans));
        }

        double Gamma::invgammp(double p, double a)
        {
            int j;
                    double x,err,t,u,pp,lna1=0,afac=0,a1=a-1;
                    const double EPS=1.e-8;
                    gln=gammaln(a);
                    if (a <= 0.) { errorInDistributionArgs= 1; return tprintf("a must be pos in invgammap"); }
                    if (p >= 1.) return MAX(100.,a + 100.*sqrt(a));
                    if (p <= 0.) return 0.0;
                    if (a > 1.) {
                            lna1=log(a1);
                            afac = exp(a1*(lna1-1.)-gln);
                            pp = (p < 0.5)? p : 1. - p;
                            t = sqrt(-2.*log(pp));
                            x = (2.30753+t*0.27061)/(1.+t*(0.99229+t*0.04481)) - t;
                            if (p < 0.5) x = -x;
                            x = MAX(1.e-3,a*pow(1.-1./(9.*a)-x/(3.*sqrt(a)),3));
                    } else {
                            t = 1.0 - a*(0.253+a*0.12);
                            if (p < t) x = pow(p/t,1./a);
                            else x = 1.-log(1.-(p-t)/(1.-t));
                    }
                    for (j=0;j<12;j++) {
                            if (x <= 0.0) return 0.0;
                            err = gammp(a,x) - p;
                            if (a > 1.) t = afac*exp(-(x-a1)+a1*(log(x)-lna1));
                            else t = exp(-x+a1*log(x)-gln);
                            u = err/t;
                            x -= (t = u/(1.-0.5*MIN(1.,u*((a-1.)/x - 1))));
                            if (x <= 0.) x = 0.5*(x + t);
                            if (fabs(t) < EPS*x ) break;
                    }
                    return x;
        }

    const double Gamma::EPS= numeric_limits<double>::epsilon();
    const double Gamma::FPMIN= numeric_limits<double>::min()/EPS;

        inline double Erf::erf(double x)
        {
            if ( x >= 0. )  return 1.0 - erfccheb(x);
            else return erfccheb(-x) - 1.0;
        }

        inline double Erf::erfc(double x)
        {
            if ( x >= 0. )  return erfccheb(x);
            else return 2.0 - erfccheb(-x);
        }

        double Erf::erfccheb(double z )
        {
            int j;
            double t, ty, tmp, d= 0., dd= 0.;
            if ( z < 0. ) { errorInDistributionArgs= 1; return tprintf("erfccheb requires nonnegative argument"); }
            t= 2./(2. + z);
            ty= 4.*t - 2.;
            for ( j= ncof - 1; j > 0; --j )
            {
                tmp= d;
                d= ty * d - dd + cof[j];
                dd= tmp;
            }
            return t*exp(-z*z + 0.5*(cof[0] + ty*d) - dd);
        }

        double Erf::inverfc(double p)
        {
            double x, err, t, pp;
            if ( p >= 2.0) return -100.;
            if ( p <= 0.0) return 100.;
            pp= (p < 1.0) ? p : 2. - p;
            t= sqrt(-2.*log(pp/2.));
            x= -0.70711*((2.30753 + t*0.27061)/(1. + t*(0.99229 + t*0.04481)) - t);
            for ( int j= 0; j < 2; ++j )
            {
                err= erfc(x) - pp;
                x += err/(1.12837916709551257*exp(-SQR(x))-x*err);
            }
            return (p < 1.0 ? x : -x);
        }

        inline double Erf::inverf(double p)
        {
            return inverfc(1.-p);
        }

    const double Erf::cof[28]= {-1.3026537197817194, 6.4196979235649026e-1,1.9476473204185836e-2, -9.561514786808631e-3, -9.46595344482036e-4,
                                3.66839497852761e-4,4.2523324806907e-5,-2.0278578112534e-5,-1.624290004647e-6,1.303655835580e-6,1.5626441722e-8,-8.5238095915e-8,
                                6.529054439e-9, 5.059343495e-9, -9.91364156e-10, -2.27365122e-10, 9.6467911e-11, 2.394039e-12, -6.886027e-12, 8.94487e-13, 3.13092e-13,
                                -1.12708e-13, 3.81e-16, 7.106e-15, -1.523e-15, -9.4e-17, 1.21e-16, -2.8e-17};

    double erfcc(const double x)
    {
        double t, z= fabs(x), ans;
        t= 2./(2. + z);
        ans= t*exp(-z*z-1.26551223 + t*(1.00002368 + t*(0.37409196 + t*(0.09678418 + t*(-0.18628806 + t*(0.27886807 + t*(-1.13520398 + t*(1.48851587 + t*(-0.82215223 + t*0.17087277)))))))));
        return (x >= 0.0 ? ans : 2.0 - ans );
    }

    double expint(const int n, const double x)
    {
        static const int MAXIT= 100;
        static const double EULER= 0.577215664901533,
                EPS= numeric_limits<double>::epsilon(),
                BIG= numeric_limits<double>::max()-EPS;

        int i, ii, nmi= n-1;
        double a, b, c, d, del, fact, h, psi, ans;
        if ( n < 0 || x < 0.0 || (x == 0.0 && (n == 0 || n == 1)))
        {
            errorInDistributionArgs= 1;
            return tprintf("bad arguments in expint");
        }
        if ( n == 0 )   ans= exp(-x)/x;
        else
        {
            if ( x == 0.0 )
                ans= 1.0/nmi;
            else
            {
                if ( x > 1.0 )
                {
                    b= x + n;
                    c= BIG;
                    d= 1.0/b;
                    h= d;
                    for ( i= 1; i <= MAXIT; ++i )
                    {
                        a= -i*(nmi + i);
                        b+= 2.0;
                        d= 1.0/(a*d + b);
                        c= b + a/c;
                        del= c*d;
                        h*= del;
                        if ( abs(del - 1.0) <= EPS)
                        {
                            ans= h*exp(-x);
                            return ans;
                        }
                    }
                    errorInDistributionArgs= 1;
                    return tprintf("continued fraction failed in expint");

                }
                else
                {
                    ans= (nmi!= 0 ? 1.0/nmi : -log(x)-EULER);
                    fact= 1.0;
                    for ( i= 1; i <= MAXIT; ++i )
                    {
                        fact*= -x/i;
                        if ( i != nmi )
                            del= -fact/(i-nmi);
                        else
                        {
                            psi= -EULER;
                            for ( ii= 1; ii <= nmi; ++ii)
                                psi+= 1.0/ii;
                            del= fact*(-log(x) + psi);
                        }
                        ans+= del;
                        if ( abs(del) < abs(ans)*EPS) return ans;
                    }
                    errorInDistributionArgs= 1;
                    return tprintf("series failed in expint");
                }
            }
        }
        return ans;
    }

    double ei(const double x)
    {
        static const int MAXIT= 100;
        static const double EULER= 0.577215664901533,
                EPS= numeric_limits<double>::epsilon(),
                FPMIN= numeric_limits<double>::min()/EPS;

        int k;
        double fact, prev, sum, term;

        if ( x <= 0.0) { errorInDistributionArgs= 1; return tprintf("Bad argument in ei"); }
        if ( x < FPMIN )    return log(x) + EULER;
        if ( x <= -log(EPS) )
        {
            sum= 0.0;
            fact= 1.0;
            for ( k= 1; k <= MAXIT; ++k )
            {
                fact*= x/k;
                term= fact/k;
                sum+= term;
                if ( term < EPS*sum )   break;
            }
            if ( k > MAXIT ) { errorInDistributionArgs= 1; return tprintf("Series failed in ei"); }
            return sum + log(x) + EULER;
        }
        else
        {
            sum= 0.0;
            term= 1.0;
            for ( k= 1; k <= MAXIT; ++k )
            {
                prev= term;
                term*= k/x;
                if ( term < EPS ) break;
                if ( term < prev ) sum+= term;
                else
                {
                    sum-= prev;
                    break;
                }
            }
            return exp(x)*(1.0 + sum)/x;
        }
    }

        double Beta::betai(const double a, const double b, const double x)
        {
            double bt;
                            if (a <= 0.0 || b <= 0.0) { errorInDistributionArgs= 1; return tprintf("Bad a or b in routine betai"); }
                            if (x < 0.0 || x > 1.0) { errorInDistributionArgs= 1; return tprintf("Bad x in routine betai"); }
                            if (x == 0.0 || x == 1.0) return x;
                            if (a > SWITCH && b > SWITCH) return betaiapprox(a,b,x);
                            bt=exp(gammaln(a+b)-gammaln(a)-gammaln(b)+a*log(x)+b*log(1.0-x));
                            if (x < (a+1.0)/(a+b+2.0)) return bt*betacf(a,b,x)/a;
                            else return 1.0-bt*betacf(b,a,1.0-x)/b;
        }

        double Beta::betacf(const double a, const double b, const double x)
        {
            int m,m2;
                            double aa,c,d,del,h,qab,qam,qap;
                            qab=a+b;
                            qap=a+1.0;
                            qam=a-1.0;
                            c=1.0;
                            d=1.0-qab*x/qap;
                            if (fabs(d) < FPMIN) d=FPMIN;
                            d=1.0/d;
                            h=d;
                            for (m=1;m<10000;m++) {
                                    m2=2*m;
                                    aa=m*(b-m)*x/((qam+m2)*(a+m2));
                                    d=1.0+aa*d;
                                    if (fabs(d) < FPMIN) d=FPMIN;
                                    c=1.0+aa/c;
                                    if (fabs(c) < FPMIN) c=FPMIN;
                                    d=1.0/d;
                                    h *= d*c;
                                    aa = -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));
                                    d=1.0+aa*d;
                                    if (fabs(d) < FPMIN) d=FPMIN;
                                    c=1.0+aa/c;
                                    if (fabs(c) < FPMIN) c=FPMIN;
                                    d=1.0/d;
                                    del=d*c;
                                    h *= del;
                                    if (fabs(del-1.0) <= EPS) break;
                            }
                            return h;
        }

        double Beta::betaiapprox(double a, double b, double x)
        {
            int j;
                            double xu,t,sum,ans;
                            double a1 = a-1.0, b1 = b-1.0, mu = a/(a+b);
                            double lnmu=log(mu),lnmuc=log(1.-mu);
                            t = sqrt(a*b/(SQR(a+b)*(a+b+1.0)));
                            if (x > a/(a+b)) {
                                    if (x >= 1.0) return 1.0;
                                    xu = MIN(1.,MAX(mu + 10.*t, x + 5.0*t));
                            } else {
                                    if (x <= 0.0) return 0.0;
                                    xu = MAX(0.,MIN(mu - 10.*t, x - 5.0*t));
                            }
                            sum = 0;
                            for (j=0;j<18;j++) {
                                    t = x + (xu-x)*y[j];
                                    sum += w[j]*exp(a1*(log(t)-lnmu)+b1*(log(1-t)-lnmuc));
                            }
                            ans = sum*(xu-x)*exp(a1*lnmu-gammaln(a)+b1*lnmuc-gammaln(b)+gammaln(a+b));
                            return ans>0.0? 1.0-ans : -ans;
        }

        double Beta::invbetai(double p, double a, double b)
        {
            const double EPS = 1.e-8;
                            double pp,t,u,err,x,al,h,w,afac,a1=a-1.,b1=b-1.;
                            int j;
                            if (p <= 0.) return 0.;
                            else if (p >= 1.) return 1.;
                            else if (a >= 1. && b >= 1.) {
                                    pp = (p < 0.5)? p : 1. - p;
                                    t = sqrt(-2.*log(pp));
                                    x = (2.30753+t*0.27061)/(1.+t*(0.99229+t*0.04481)) - t;
                                    if (p < 0.5) x = -x;
                                    al = (SQR(x)-3.)/6.;
                                    h = 2./(1./(2.*a-1.)+1./(2.*b-1.));
                                    w = (x*sqrt(al+h)/h)-(1./(2.*b-1)-1./(2.*a-1.))*(al+5./6.-2./(3.*h));
                                    x = a/(a+b*exp(2.*w));
                            } else {
                                    double lna = log(a/(a+b)), lnb = log(b/(a+b));
                                    t = exp(a*lna)/a;
                                    u = exp(b*lnb)/b;
                                    w = t + u;
                                    if (p < t/w) x = pow(a*w*p,1./a);
                                    else x = 1. - pow(b*w*(1.-p),1./b);
                            }
                            afac = -gammaln(a)-gammaln(b)+gammaln(a+b);
                            for (j=0;j<10;j++) {
                                    if (x == 0. || x == 1.) return x;
                                    err = betai(a,b,x) - p;
                                    t = exp(a1*log(x)+b1*log(1.-x) + afac);
                                    u = err/t;
                                    x -= (t = u/(1.-0.5*MIN(1.,u*(a1/x - b1/(1.-x)))));
                                    if (x <= 0.) x = 0.5*(x + t);
                                    if (x >= 1.) x = 0.5*(x + t + 1.);
                                    if (fabs(t) < EPS*x && j > 0) break;
                            }
                            return x;
        }

    const double Beta::EPS= numeric_limits<double>::epsilon();
    const double Beta::FPMIN= numeric_limits<double>::min()/EPS;

        Normaldist::Normaldist(double mmu, double ssig): mu(mmu), sig(ssig)
        {
            if ( sig <= 0. ) { errorInDistributionArgs= 1; tprintf("bad sig in Normaldist"); }
        }

        double Normaldist::p(double x)
        {
            return (0.398942280401432678/sig)*exp(-0.5*SQR((x-mu)/sig));
        }

        double Normaldist::cdf(double x)
        {
            return 0.5*erfc(-0.707106781186547524*(x-mu)/sig);
        }

        double Normaldist::invcdf(double p)
        {
            if ( p <= 0. || p >= 1. ) { errorInDistributionArgs= 1; return tprintf("bad p in Normaldist"); }
            return -1.41421356237309505*sig*inverfc(2.*p) + mu;
        }


        Cauchydist::Cauchydist(double mmu, double ssig): mu(mmu), sig(ssig)
        {
            if ( sig <= 0. ) { errorInDistributionArgs= 1; tprintf("bad sig in Cauchydist"); }
        }

        double Cauchydist::p(double x)
        {
            return 0.318309886183790671/(sig*(1.+SQR((x-mu)/sig)));
        }

        double Cauchydist::cdf(double x)
        {
            return 0.5 + 0.318309886183790671*atan2(x-mu,sig);
        }

        double Cauchydist::invcdf(double p)
        {
            if ( p <= 0. || p >= 1. ) { errorInDistributionArgs= 1; return tprintf("bad p in Cauchydist"); }
            return mu + sig*tan(3.14159265358979324 *(p-0.5));
        }


        Studenttdist::Studenttdist(double nnu, double mmu, double ssig)
            : nu(nnu), mu(mmu), sig(ssig)
        {
            if ( sig <= 0. || nu <= 0. ) { errorInDistributionArgs= 1; tprintf("bad sig, nu in Studenttdist"); }
            np= 0.5*(nu + 1.);
            fac= gammaln(np) - gammaln(0.5*nu);
        }

        double Studenttdist::p(double t)
        {
            return exp(-np*log(1. + SQR((t - mu)/sig)/nu)+fac)/(sqrt(3.14159265358979324*nu)*sig);
        }

        double Studenttdist::cdf(double t)
        {
            double p= 0.5*betai(0.5*nu, 0.5, nu/(nu + SQR((t-mu)/sig)));
            if ( t >= mu )  return 1. - p;
            else    return p;
        }

        double Studenttdist::invcdf(double p)
        {
            if ( p <= 0. || p >= 1. ) { errorInDistributionArgs= 1; return tprintf("bad p in Studentdist"); }
            double x= invbetai(2.*MIN(p,1.-p), 0.5*nu, 0.5);
            x= sig*sqrt(nu*(1.-x)/x);
            return (p >= 0.5 ? mu +x : mu - x);
        }

        double Studenttdist::aa(double t)
        {
            if ( t < 0. ) { errorInDistributionArgs= 1; return tprintf("bad t in studenttidst"); }
            return 1.-betai(0.5*nu, 0.5, nu/(nu + SQR(t)));
        }

        double Studenttdist::invaa(double p)
        {
            if ( p < 0. || p >= 1. ) { errorInDistributionArgs= 1; return tprintf("bad p in Studentdist"); }
            double x= invbetai(1.-p, 0.5*nu, 0.5);
            return sqrt(nu*(1.-x)/x);
        }

        Logisticdist::Logisticdist(double mmu, double ssig) : mu(mmu), sig(ssig)
        {
            if ( sig <= 0.) { errorInDistributionArgs= 1; tprintf("bad sig in Logisticdist"); }
        }

        double Logisticdist::p(double x)
        {
            double e= exp(-abs(1.81379936423421785*(x-mu)/sig));
            return 1.81379936423421785*e/(sig*SQR(1.+e));
        }

        double Logisticdist::cdf(double x)
        {
            double e= exp(-abs(1.81379936423421785*(x-mu)/sig));
            if ( x >= mu )  return 1./(1. + e);
            else    return e/(1. + e);
        }

        double Logisticdist::invcdf(double p)
        {
            if ( p <= 0. || p >= 1. ) { errorInDistributionArgs= 1; return tprintf("bad p in Logisticdist"); }
            return mu + 0.551328895421792049*sig*log(p/(1.-p));
        }


        Expondist::Expondist(double bbet): bet(bbet)
        {
            if ( bet <= 0. ) { errorInDistributionArgs= 1; tprintf("bad bet in Expondist"); }
        }

        double Expondist::p(double x)
        {
            if ( x < 0. ) { errorInDistributionArgs= 1; return tprintf("bad x in Expondist"); }
            return bet*exp(-bet*x);
        }

        double Expondist::cdf(double x)
        {
            if ( x < 0. ) { errorInDistributionArgs= 1; return tprintf("bad x in Expondist"); }
            return 1.-exp(-bet*x);
        }

        double Expondist::invcdf(double p)
        {
            if ( p < 0. || p >= 1. ) { errorInDistributionArgs= 1; return tprintf("bad p in expondist"); }
            return -log(1.-p)/bet;
        }


        Lognormaldist::Lognormaldist(double mmu, double ssig): mu(mmu), sig(ssig)
        {
            if ( sig <= 0. ) { errorInDistributionArgs= 1; tprintf("bad sig in Lognormaldist"); }
        }

        double Lognormaldist::p(double x)
        {
            if ( x < 0. ) { errorInDistributionArgs= 1; return tprintf("bad x in Lognormaldist"); }
            if ( x == 0. )  return 0.;
            return (0.398942280401432678 / (sig*x))*exp(-0.5*SQR((log(x)-mu)/sig));
        }

        double Lognormaldist::cdf(double x)
        {
            if ( x < 0. ) { errorInDistributionArgs= 1; return tprintf("bad x in Lognormaldist"); }
            if ( x == 0. )  return 0.;
            return 0.5*erfc(-0.707106781186547524*(log(x)-mu)/sig);
        }

        double Lognormaldist::invcdf(double p)
        {
            if ( p <= 0. || p >= 1. ) { errorInDistributionArgs= 1; return tprintf("bad p in Lognormaldist"); }
            return exp(-1.41421356237309505*sig*inverfc(2.*p)+mu);
        }


        Chisquaredist::Chisquaredist(double nnu): nu(nnu)
        {
            if ( nu <= 0 ) { errorInDistributionArgs= 1; tprintf("bad nu in Chisquaredist"); }
            fac= 0.693147180559945309*(0.5*nu)+gammaln(0.5*nu);
        }

        double Chisquaredist::p(double x2)
        {
            if ( x2 <= 0. ) { errorInDistributionArgs= 1; return tprintf("bad x2 in Chisquaredist"); }
            return exp(-0.5*(x2-(nu-2.)*log(x2))-fac);
        }

        double Chisquaredist::cdf(double x2)
        {
            if ( x2 < 0. ) { errorInDistributionArgs= 1; return tprintf("bad x2 in Chisquaredist"); }
            return gammp(0.5*nu, 0.5*x2);
        }

        double Chisquaredist::invcdf(double p )
        {
            if ( p < 0. || p >= 1. ) { errorInDistributionArgs= 1; return tprintf("bad p in Chisquaredist"); }
            return 2.*invgammp(p,0.5*nu);
        }


        Gammadist::Gammadist(double aalph, double bbet): alph(aalph), bet(bbet)
        {
            if (alph <= 0. || bet <= 0.) { errorInDistributionArgs= 1; tprintf("bad alph,bet in Gammadist"); }
                            fac = alph*log(bet)-gammaln(alph);
        }

        double Gammadist::p(double x)
        {
            if (x <= 0.) { errorInDistributionArgs= 1; return tprintf("bad x in Gammadist"); }
                            return exp(-bet*x+(alph-1.)*log(x)+fac);
        }

        double Gammadist::cdf(double x)
        {
            if (x < 0.) { errorInDistributionArgs= 1; return tprintf("bad x in Gammadist"); }
                            return gammp(alph,bet*x);
        }

        double Gammadist::invcdf(double p)
        {
            if (p < 0. || p >= 1.) { errorInDistributionArgs= 1; return tprintf("bad p in Gammadist"); }
                            return invgammp(p,alph)/bet;
        }


        Fdist::Fdist(double nnu1, double nnu2) : nu1(nnu1), nu2(nnu2)
        {
            if (nu1 <= 0. || nu2 <= 0.) { errorInDistributionArgs= 1; tprintf("bad nu1,nu2 in Fdist"); }
                            fac = 0.5*(nu1*log(nu1)+nu2*log(nu2))+gammaln(0.5*(nu1+nu2))
                                    -gammaln(0.5*nu1)-gammaln(0.5*nu2);
        }

        double Fdist::p(double f)
        {
            if (f <= 0.) { errorInDistributionArgs= 1; return tprintf("bad f in Fdist"); }
                            return exp((0.5*nu1-1.)*log(f)-0.5*(nu1+nu2)*log(nu2+nu1*f)+fac);
        }

        double Fdist::cdf(double f)
        {
            if (f < 0.) { errorInDistributionArgs= 1; return tprintf("bad f in Fdist"); }
                            return betai(0.5*nu1,0.5*nu2,nu1*f/(nu2+nu1*f));
        }

        double Fdist::invcdf(double p)
        {
            if (p <= 0. || p >= 1.) { errorInDistributionArgs= 1; return tprintf("bad p in Fdist"); }
                            double x = invbetai(p,0.5*nu1,0.5*nu2);
                            return nu2*x/(nu1*(1.-x));
        }


        Betadist::Betadist(double aalph, double bbet): alph(aalph), bet(bbet)
        {
            if (alph <= 0. || bet <= 0.) { errorInDistributionArgs= 1; tprintf("bad alph,bet in Betadist"); }
                            fac = gammaln(alph+bet)-gammaln(alph)-gammaln(bet);
        }

        double Betadist::p( double x )
        {
            if (x <= 0. || x >= 1.) { errorInDistributionArgs= 1; return tprintf("bad x in Betadist"); }
                            return exp((alph-1.)*log(x)+(bet-1.)*log(1.-x)+fac);
        }

        double Betadist::cdf(double x)
        {
            if (x < 0. || x > 1.) { errorInDistributionArgs= 1; return tprintf("bad x in Betadist"); }
                            return betai(alph,bet,x);
        }

        double Betadist::invcdf(double p)
        {
            if (p < 0. || p > 1.) { errorInDistributionArgs= 1; return tprintf("bad p in Betadist"); }
                            return invbetai(p,alph,bet);
        }


        double KSdist::pks(double z)
        {
            if ( z < 0. )   { errorInDistributionArgs= 1; return tprintf("bad z in KSdist"); }
            if ( z == 0. )  return 0.;
            if ( z < 1.18 )
            {
                double y= exp(-1.24470055013616983/SQR(z));
                return 2.25675833419102515*sqrt(-log(y))*(y + pow(y,9) + pow(y,25) + pow(y,49));
            }
            else
            {
                double x= exp(-2.*SQR(z));
                return 1. - 2.*(x - pow(x,4) + pow(x,9));
            }
        }

        double KSdist::qks(double z)
        {
            if ( z < 0. )   { errorInDistributionArgs= 1; return tprintf("bad z in KSdist"); }
            if ( z == 0. )  return 1.;
            if ( z < 1.18)  return 1.-pks(z);
            double x= exp(-2.*SQR(z));
            return 2.*(x - pow(x,4) + pow(x,9));
        }

        double KSdist::invqks(double q)
        {
            double y, logy, /*yp,*/ x, xp, f, ff, u, t;
            if ( q <= 0. || q > 1. )   { errorInDistributionArgs= 1; return tprintf("bad q in KSdist"); }
            if ( q == 1. )  return 0.;
            if ( q > 0.3 )
            {
                f= -0.392699081698724155*SQR(1.-q);
                y= invxlogx(f);
                do {
                    //yp= y;
                    logy= log(y);
                    ff= f/SQR(1. + pow(y,4) + pow(y,12));
                    u= (y*logy - ff)/(1. + logy);
                    y= y - (t= u/MAX(0.5,1.-0.5*u/(y*(1.+logy))));
                } while (abs(t/y) > 1.e-15);
                return 1.5707963267948962/sqrt(-log(y));
            } else
            {
                x= 0.03;
                do {
                    xp= x;
                    x= 0.5*q + pow(x,4) - pow(x,9);
                    if ( x > 0.06 ) x+= pow(x,16) - pow(x,25);
                } while (abs((xp-x)/x) > 1.e-15);
                return sqrt(-0.5*log(x));
            }
        }

        double KSdist::invpks(double p) {return invqks(1.-p);}


        Poissondist::Poissondist(double llam) : lam(llam)
        {
            if ( lam <= 0. ) { errorInDistributionArgs= 1; tprintf("bad lam in Poissondist"); }
        }

        double Poissondist::p(int n)
        {
            if ( n < 0 )  { errorInDistributionArgs= 1; return tprintf("bad n in Poissondist"); }
            return exp(-lam + n*log(lam) - gammaln(n + 1.));
        }

        double Poissondist::cdf(int n)
        {
            if ( n < 0 ) { errorInDistributionArgs= 1; return tprintf("bad n in Poissondist"); }
            if ( n == 0 )   return 0.;
            return gammq((double)n, lam);
        }

        int Poissondist::invcdf(double p)
        {
            int n, n1, nu, inc= 1;
            if ( p <= 0. || p >= 1. ) { errorInDistributionArgs= 1; return tprintf("bad p in Poissondist"); }
            if ( p < exp(-lam)) return 0;
            n= (int)MAX(sqrt(lam), 5.);
            if ( p < cdf(n))
            {
                do {
                    n= MAX(n - inc, 0);
                    inc*= 2;
                } while ( p < cdf(n) );
                n1= n; nu= n + inc/2;
            }
            else
            {
                do {
                    n+= inc;
                    inc*= 2;
                } while ( p > cdf(n) );
                nu= n; n1= n - inc/2;
            }
            while ( (nu= n1) > 1 )
            {
                n= (n1 + nu) / 2;
                if ( p < cdf(n)) nu = n;
                else n1= n;
            }
            return n1;
        }


        Binomialdist::Binomialdist(int nn, double ppe): n(nn), pe(ppe)
        {
            if ( n <= 0 || pe <= 0. || pe >= 1. ) { errorInDistributionArgs= 1; tprintf("bad args in Binomialdist"); }
            fac= gammaln(n + 1.);
        }

        double Binomialdist::p(int k)
        {
            if ( k < 0 )   { errorInDistributionArgs= 1; return tprintf("bad k in Binomialdist"); }
            if ( k > n )    return 0.;
            return exp(k*log(pe) + (n-k)*log(1.-pe) + fac - gammaln(k + 1.) - gammaln(n - k + 1.));
        }

        double Binomialdist::cdf(int k)
        {
            if ( k < 0 )   { errorInDistributionArgs= 1; return tprintf("bad k in Binomialdist"); }
            if ( k == 0 ) return 0.;
            if ( k > n )   return 1.;
            return 1. - betai((double)k, n - k + 1., pe);
        }

        int Binomialdist::invcdf(double p )
        {
            int k, k1, ku, inc= 1;
            if ( p <= 0. || p >= 1. ) { errorInDistributionArgs= 1; return tprintf("bad p in Bionomialdist"); }

            k= MAX(0, MIN(n, (int)(n*pe)));

            if ( p < cdf(k))
            {
                do{
                    k= MAX(k - inc, 0);
                    inc*= 2;
                } while (p < cdf(k));
                k1= k; ku= k + inc/2;
            }
            else
            {
                do
                {
                    k= MIN(k + inc, n+1);
                    inc*= 2;
                } while ( p > cdf(k) );
                ku= k; k1= k - inc/2;
            }
            while ( ku - k1 > 1 )
            {
                k= (k1 + ku)/2;
                if ( p < cdf(k) )   ku= k;
                else    k1= k;
            }

            return k1;
        }

    double invxlogx(double y)
    {
        const double ooe= 0.367879441171442322;
        double t, u, to= 0.;
        if ( y >= 0. || y <= -ooe ) { errorInDistributionArgs= 1; return tprintf("no such inverse value"); }
        if ( y < -0.2 ) u= log(ooe - sqrt(2*ooe*(y + ooe)));
        else u= -10.;
        do
        {
            u+= (t= (log(y/u)-u)*(u/(1. + u)));
            if ( t < 1.e-8 && abs(t + to) < 0.01*abs(t)) break;
            to= t;
        }
        while ( abs(t/u) > 1.e-15 );
        return exp(u);
    }
}

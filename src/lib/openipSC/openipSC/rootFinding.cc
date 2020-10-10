#include <math.h>

#include <openipSC/rootFinding.h>

namespace openip
{
    double bisectionMethod(RealFunction* rf, double a, double b)
    {
        double midpoint= (a + b)/2;
        while ( fabs(b - a) > 2 * 0.00000001 )
        {
            midpoint= (b + a)/2.0;

            if ( rf->value(a) * rf->value(midpoint) < 0 )
                b= midpoint;
            else if ( rf->value(b) * rf->value(midpoint) < 0 )
                a= midpoint;
            else
                return midpoint;
        }

        return midpoint;
    }

    double newtonIteration(RealFunction*, double)
    {
        return 0.0;
    }

    double secantMethod(RealFunction* r, double x0, double x1)
    {
        float d;
        for ( int i= 0; i < 100; ++i )
        {
            d= (x1-x0)/(r->value(x1)-r->value(x0))*r->value(x0);
            if ( fabs(d) < 0.0001 )
                return d;
            x0= x1;
            x1= x1-d;
        }
        return x1;
    }
}

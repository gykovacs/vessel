#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <openipDS/Vector.h>
#include <openipDS/Image.h>
#include <openipDS/Volume.h>

#include <openipML/FuzzyMembershipFunctions.h>

namespace openip
{
  template<typename T>
  double fuzzyMeasure(Vector<T>& v, int a, int b, FuzzyMembershipFunction<T>& f);
  
  template<typename T>
  double fuzzyMeasure(Image<T>& i, int a, int b, FuzzyMembershipFunction<T>& f, int resolution= 100);
  
  template<typename T>
  double fuzzyMeasure(Volume<T>& v, int a, int b, FuzzyMembershipFunction<T>& f, int resolution= 100);
}

namespace openip
{
  template<typename T>
  double fuzzyMeasure(Vector<T>& v, int a, int b, FuzzyMembershipFunction<T>& f)
  {
    double result= 0;
    for ( int i= a; i <= b; ++i )
      result+= f.fuzzy(v(i));
    return result;
  }

  template<typename T>
  double fuzzyMeasure(Image<T>& v, int a, int b, FuzzyMembershipFunction<T>& f, int resolution)
  {
    int ra, rb, ca, cb;
    v.getCoordinate2D(a, ra, ca);
    v.getCoordinate2D(b, rb, cb);
    
    double dr, dc;
    dr= (rb-ra)/double(resolution);
    dc= (cb-ca)/double(resolution);
    
    double rr= ra, cc= ca;
    
    double result= 0;
    
    for ( int i= 0; i <= resolution; ++i )
    {
      result+= f.fuzzy(v(floor(rr), floor(cc)));
      rr+= dr;
      cc+= dc;
    }
    
    return result/resolution*sqrt((ra - rb)*(ra - rb) + (ca - cb)*(ca - cb));
  }

  template<typename T>
  double fuzzyMeasure(Volume<T>& v, int a, int b, FuzzyMembershipFunction<T>& f, int resolution)
  {
    int ra, rb, ca, cb, sa, sb;
    v.getCoordinate3D(a, sa, ra, ca);
    v.getCoordinate3D(b, sb, rb, cb);
    
    //printf("%d %d %d,%d,%d -- %d,%d,%d\n", a, b, sa, ra, ca, sb, rb, cb);
    
    double ds, dr, dc;
    ds= (sb-sa)/double(resolution);
    dr= (rb-ra)/double(resolution);
    dc= (cb-ca)/double(resolution);
    
    double ss= sa, rr= ra, cc= ca;
    
    double result= 0;
    
    for ( int i= 0; i <= resolution; ++i )
    {
      result+= f.fuzzy(v(floor(ss), floor(rr), floor(cc)));
      ss+= ds;
      rr+= dr;
      cc+= dc;
    }
    
    return result/resolution*sqrt((sa - sb)*(sa - sb) + (ra - rb)*(ra - rb) + (ca - cb)*(ca - cb));
  }
}

#endif
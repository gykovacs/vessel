#ifndef FUZZY_MEMBERSHIP_FUNCTIONS_H
#define FUZZY_MEMBERSHIP_FUNCTIONS_H

#include <openipML/FuzzyMembershipFunction.h>

namespace openip
{
  template<typename T>
  class CrispFuzzyMembershipFunction: public FuzzyMembershipFunction<T>
  {
    public:
      using FuzzyMembershipFunction<T>::fuzzy;
      
      CrispFuzzyMembershipFunction(T th);

      CrispFuzzyMembershipFunction(const CrispFuzzyMembershipFunction& f);

      virtual ~CrispFuzzyMembershipFunction();

      virtual T fuzzy(T v);

      T threshold;
  };
  
  template<typename T>
  CrispFuzzyMembershipFunction<T>::CrispFuzzyMembershipFunction(T t)
  : FuzzyMembershipFunction<T>(), threshold(t)
  {
  }
  
  template<typename T>
  CrispFuzzyMembershipFunction<T>::CrispFuzzyMembershipFunction(const CrispFuzzyMembershipFunction& f)
  : FuzzyMembershipFunction<T>(f), threshold(f.threshold)
  {
  }
  
  template<typename T>
  CrispFuzzyMembershipFunction<T>::~CrispFuzzyMembershipFunction()
  {
  }
  
  template<typename T>
  T CrispFuzzyMembershipFunction<T>::fuzzy(T v)
  {
    return v > threshold ? 1 : 0;
  }
  
  template<typename T>
  class TriangleFuzzyMembershipFunction: public FuzzyMembershipFunction<T>
  {
    public:
      using FuzzyMembershipFunction<T>::fuzzy;
      
      TriangleFuzzyMembershipFunction(T a, T b, T c);

      TriangleFuzzyMembershipFunction(const TriangleFuzzyMembershipFunction& f);

      virtual ~TriangleFuzzyMembershipFunction();

      virtual T fuzzy(T v);

      T a, b, c;
  };
  
  template<typename T>
  TriangleFuzzyMembershipFunction<T>::TriangleFuzzyMembershipFunction(T aa, T bb, T cc)
  : FuzzyMembershipFunction<T>(), a(aa), b(bb), c(cc)
  {
  }
  
  template<typename T>
  TriangleFuzzyMembershipFunction<T>::TriangleFuzzyMembershipFunction(const TriangleFuzzyMembershipFunction& f)
  : FuzzyMembershipFunction<T>(f), a(f.a), b(f.b), c(f.c)
  {
  }
  
  template<typename T>
  TriangleFuzzyMembershipFunction<T>::~TriangleFuzzyMembershipFunction()
  {
  }
  
  template<typename T>
  T TriangleFuzzyMembershipFunction<T>::fuzzy(T v)
  {
    if ( v <= a )
      return 0;
    if ( v >= b )
      return 0;
    if ( a <= v && v <= b )
      return (v-a)/(b-a);
    if ( b <= v && v <= c )
      return (c-v)/(c-b);
  }
  
  template<typename T>
  class TrapezoidalFuzzyMembershipFunction: public FuzzyMembershipFunction<T>
  {
    public:
      using FuzzyMembershipFunction<T>::fuzzy;
      
      TrapezoidalFuzzyMembershipFunction(T a, T b, T c, T d);

      TrapezoidalFuzzyMembershipFunction(const TrapezoidalFuzzyMembershipFunction& f);

      virtual ~TrapezoidalFuzzyMembershipFunction();

      virtual T fuzzy(T v);

      T a, b, c, d;
  };
  
  template<typename T>
  TrapezoidalFuzzyMembershipFunction<T>::TrapezoidalFuzzyMembershipFunction(T aa, T bb, T cc, T dd)
  : FuzzyMembershipFunction<T>(), a(aa), b(bb), c(cc), d(dd)
  {
  }
  
  template<typename T>
  TrapezoidalFuzzyMembershipFunction<T>::TrapezoidalFuzzyMembershipFunction(const TrapezoidalFuzzyMembershipFunction& f)
  : FuzzyMembershipFunction<T>(f), a(f.a), b(f.b), c(f.c), d(f.d)
  {
  }
  
  template<typename T>
  TrapezoidalFuzzyMembershipFunction<T>::~TrapezoidalFuzzyMembershipFunction()
  {
  }
  
  template<typename T>
  T TrapezoidalFuzzyMembershipFunction<T>::fuzzy(T v)
  {
    if ( v <= a )
      return 0;
    if ( v >= b )
      return 0;
    if ( a <= v && v <= b )
      return (v-a)/(b-a);
    if ( v <= b && v <= c )
      return 1;
    if ( c <= v && v <= d )
      return (d-v)/(d-c);
  }
  
  template<typename T>
  class GaussianFuzzyMembershipFunction: public FuzzyMembershipFunction<T>
  {
    public:
      using FuzzyMembershipFunction<T>::fuzzy;
      
      GaussianFuzzyMembershipFunction(T m, T sigma);

      GaussianFuzzyMembershipFunction(const GaussianFuzzyMembershipFunction& f);

      virtual ~GaussianFuzzyMembershipFunction();

      virtual T fuzzy(T v);

      T m, sigma;
  };
  
  template<typename T>
  GaussianFuzzyMembershipFunction<T>::GaussianFuzzyMembershipFunction(T mm, T ss)
  : FuzzyMembershipFunction<T>(), m(mm), sigma(ss)
  {
  }
  
  template<typename T>
  GaussianFuzzyMembershipFunction<T>::GaussianFuzzyMembershipFunction(const GaussianFuzzyMembershipFunction& f)
  : FuzzyMembershipFunction<T>(f), m(f.m), sigma(f.sigma)
  {
  }
  
  template<typename T>
  GaussianFuzzyMembershipFunction<T>::~GaussianFuzzyMembershipFunction()
  {
  }
  
  template<typename T>
  T GaussianFuzzyMembershipFunction<T>::fuzzy(T v)
  {
    return exp(-0.5*(v - m)*(v - m)/(sigma*sigma));
  }
  
  
  template<typename T>
  class GeneralizedBellFuzzyMembershipFunction: public FuzzyMembershipFunction<T>
  {
    public:
      using FuzzyMembershipFunction<T>::fuzzy;
      
      GeneralizedBellFuzzyMembershipFunction(T a, T b, T c);

      GeneralizedBellFuzzyMembershipFunction(const GeneralizedBellFuzzyMembershipFunction& f);

      virtual ~GeneralizedBellFuzzyMembershipFunction();

      virtual T fuzzy(T v);

      T a, b, c;
  };
  
  template<typename T>
  GeneralizedBellFuzzyMembershipFunction<T>::GeneralizedBellFuzzyMembershipFunction(T aa, T bb, T cc)
  : FuzzyMembershipFunction<T>(), a(aa), b(bb), c(cc)
  {
  }
  
  template<typename T>
  GeneralizedBellFuzzyMembershipFunction<T>::GeneralizedBellFuzzyMembershipFunction(const GeneralizedBellFuzzyMembershipFunction& f)
  : FuzzyMembershipFunction<T>(f), a(f.a), b(f.b), c(f.c)
  {
  }
  
  template<typename T>
  GeneralizedBellFuzzyMembershipFunction<T>::~GeneralizedBellFuzzyMembershipFunction()
  {
  }
  
  template<typename T>
  T GeneralizedBellFuzzyMembershipFunction<T>::fuzzy(T v)
  {
    return 1.0/(1 + pow(fabs((v-c)/a),2*b));
  }
  
  template<typename T>
  class SigmoidFuzzyMembershipFunction: public FuzzyMembershipFunction<T>
  {
    public:
      using FuzzyMembershipFunction<T>::fuzzy;
      
      SigmoidFuzzyMembershipFunction(T a, T c);

      SigmoidFuzzyMembershipFunction(const SigmoidFuzzyMembershipFunction& f);

      virtual ~SigmoidFuzzyMembershipFunction();

      virtual T fuzzy(T v);

      T a, c;
  };
  
  template<typename T>
  SigmoidFuzzyMembershipFunction<T>::SigmoidFuzzyMembershipFunction(T aa, T cc)
  : FuzzyMembershipFunction<T>(), a(aa), c(cc)
  {
  }
  
  template<typename T>
  SigmoidFuzzyMembershipFunction<T>::SigmoidFuzzyMembershipFunction(const SigmoidFuzzyMembershipFunction& f)
  : FuzzyMembershipFunction<T>(f), a(f.a), c(f.c)
  {
  }
  
  template<typename T>
  SigmoidFuzzyMembershipFunction<T>::~SigmoidFuzzyMembershipFunction()
  {
  }
  
  template<typename T>
  T SigmoidFuzzyMembershipFunction<T>::fuzzy(T v)
  {
    /*float cc= c/100;
    float vv= v/100;
    return 1.0/(1 + exp(-a*(vv-cc)));*/
    return 1.0/(1 + exp(-a*(v-c)));
  }
  
  template<typename T>
  class LRFuzzyMembershipFunction: public FuzzyMembershipFunction<T>
  {
    public:
      using FuzzyMembershipFunction<T>::fuzzy;
      
      LRFuzzyMembershipFunction(T a, T b, T c);

      LRFuzzyMembershipFunction(const LRFuzzyMembershipFunction& f);

      virtual ~LRFuzzyMembershipFunction();

      virtual T fuzzy(T v);

      T a, b, c;
  };
  
  template<typename T>
  LRFuzzyMembershipFunction<T>::LRFuzzyMembershipFunction(T aa, T bb, T cc)
  : FuzzyMembershipFunction<T>(), a(aa), b(bb), c(cc)
  {
  }
  
  template<typename T>
  LRFuzzyMembershipFunction<T>::LRFuzzyMembershipFunction(const LRFuzzyMembershipFunction& f)
  : FuzzyMembershipFunction<T>(f), a(f.a), b(f.b), c(f.c)
  {
  }
  
  template<typename T>
  LRFuzzyMembershipFunction<T>::~LRFuzzyMembershipFunction()
  {
  }
  
  template<typename T>
  T LRFuzzyMembershipFunction<T>::fuzzy(T v)
  {
    if ( v <= c )
    {
      T tmp;
      tmp= (c - v)/a;
      tmp= 1 - tmp*tmp;
      return tmp < 0 ? 0 : sqrt(tmp);
    }
    else
    {
      return exp(-pow(fabs(v),3.0));
    }
  }
}

#endif

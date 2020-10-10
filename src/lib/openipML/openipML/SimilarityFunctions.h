#ifndef SIMILARITY_FUNCTIONS_H
#define SIMILARITY_FUNCTIONS_H

#include <openipML/SimilarityFunction.h>
#include <openipDS/Image.h>
#include <openipML/DiscreteHistogram.h>

namespace openip
{
  template<typename T>
  class CorrelationCoefficientSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      CorrelationCoefficientSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
      
      double normb;
  };

  template<typename T>
  CorrelationCoefficientSF<T>::CorrelationCoefficientSF()
  : SimilarityFunction<T>()
  {
    descriptor= string("CorrelationCoefficientSF");
  }

  template<typename T>
  double CorrelationCoefficientSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    return a.inner(b)/sqrt(a.inner(a)*b.inner(b));
  }
  
  template<typename T>
  double CorrelationCoefficientSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    double inner= 0;
    double norma= 0;
    double value;
    for ( unsigned int i= 0; i < b.size(); ++i )
    {
      value= a(n + b(i).first);
      inner+= value*b(i).second;
      norma+= value*value;
    }
    
    return inner/sqrt(norma*normb);
  }
  
  template<typename T>
  void CorrelationCoefficientSF<T>::init(Template<T>& b)
  {
    Vector<float> weights;
    b.getWeights(weights);
    normb= b.inner(b);
    
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* CorrelationCoefficientSF<T>::clone()
  {
    return new CorrelationCoefficientSF<T>();
  }
  
  template<typename T>
  class PearsonCorrelationCoefficientSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      PearsonCorrelationCoefficientSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
      
      double normb;
      double meanb;
  };

  template<typename T>
  PearsonCorrelationCoefficientSF<T>::PearsonCorrelationCoefficientSF()
  : SimilarityFunction<T>()
  {
    descriptor= string("PearsonCorrelationCoefficientSF");
  }

  template<typename T>
  double PearsonCorrelationCoefficientSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    double meana= a.getMean();
    double meanb= b.getMean();
    
    double inner= 0, norma= 0, normb= 0;
    for ( unsigned int i= 0; i < b.size(); ++i )
    {
      inner+= (a(i) - meana)*(b(i) - meanb);
      norma+= (a(i) - meana)*(a(i) - meana);
      normb+= (b(i) - meanb)*(b(i) - meanb);
    }
    
    return inner/sqrt(norma*normb);
  }
  
  template<typename T>
  double PearsonCorrelationCoefficientSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    double meana= 0;
    double norma= 0;
    double inner= 0;
    double value;
    
    for ( unsigned int i= 0; i < b.size(); ++i )
      meana+= a(n + b(i).first);
    meana/= b.size();
    
    for ( unsigned int i= 0; i < b.size(); ++i )
    {
      value= a(n + b(i).first);
      inner+= (value - meana)*(b(i).second - meanb);
      norma+= (value - meana)*(value - meana);
    }
    
    /*if ( rand() % 1000000 == 0 )
    {
#pragma omp critical
        {
        for ( int i= 0; i < b.size(); ++i )
            printf("%f,%f ", a(n + b(i).first), b(i).second);
        printf("\n");
        }
    }*/
    
    if ( norma < 0.0000001 || normb < 0.0000001 )
        return 0;
    else
        return inner/sqrt(norma*normb);
  }
  
  template<typename T>
  void PearsonCorrelationCoefficientSF<T>::init(Template<T>& b)
  {
    meanb= 0;
    normb= 0;
    
    for ( unsigned int i= 0; i < b.size(); ++i )
      meanb+= b(i).second;
    meanb/= b.size();
    for ( unsigned int i= 0; i < b.size(); ++i )
      normb+= (b(i).second - meanb)*(b(i).second - meanb);
    
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* PearsonCorrelationCoefficientSF<T>::clone()
  {
    return new PearsonCorrelationCoefficientSF<T>();
  }
  
  template<typename T>
  class TanimotoSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      TanimotoSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
  };
  
  template<typename T>
  TanimotoSF<T>::TanimotoSF()
  : SimilarityFunction<T>()
  {
    descriptor= string("TanimotoSF");
  }

  template<typename T>
  double TanimotoSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    double inner= a.inner(b);
    double dist= 0;
    for ( unsigned int i= 0; i < b.size(); ++i )
      dist+= (a(i) - b(i))*(a(i) - b(i));
    
    return inner/(dist + inner);
  }
  
  template<typename T>
  double TanimotoSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    double inner= 0;
    double dist= 0;
    double value;
    
    for ( unsigned int i= 0; i < b.size(); ++i )
    {
      value= a(n + b(i).first);
      inner+= value*b(i).second;
      dist+= (value - b(i).second)*(value - b(i).second);
    }
    
    return inner/(dist + inner);
  }
  
  template<typename T>
  void TanimotoSF<T>::init(Template<T>& /*b*/)
  {
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* TanimotoSF<T>::clone()
  {
    return new TanimotoSF<T>();
  }
  
  template<typename T>
  class StochasticSignChangeSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      StochasticSignChangeSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
  };
  
  template<typename T>
  StochasticSignChangeSF<T>::StochasticSignChangeSF()
  : SimilarityFunction<T>()
  {
    descriptor= string("StochasticSignChangeSF");
  }

  template<typename T>
  double StochasticSignChangeSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    int sc= 0;
    
    T diff0, diff1;
    for ( int i= 0; i < int(a.size())-1; ++i )
    {
      diff0= a(i) - b(i);
      diff1= a(i+1) - b(i+1);
      if ( (diff0 < 0 && diff1 > 0) || (diff1 < 0 && diff0 > 0) )
	sc++;
    }
    
    return sc;
  }
  
  template<typename T>
  double StochasticSignChangeSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    int sc= 0;
    T diff0, diff1;
    for ( int i= 0; i < int(b.size())-1; ++i )
    {
      diff0= a(n + b(i).first) - b(i).second;
      diff1= a(n + b(i+1).first) - b(i+1).second;
      if ( (diff0 < 0 && diff1 > 0) || (diff1 < 0 && diff0 > 0) )
	sc++;
    }
    return sc;
  }
  
  template<typename T>
  void StochasticSignChangeSF<T>::init(Template<T>& /*b*/)
  {
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* StochasticSignChangeSF<T>::clone()
  {
    return new StochasticSignChangeSF<T>();
  }
  
  template<typename T>
  class DeterministicSignChangeSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      DeterministicSignChangeSF(double q= 0.5);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
      
      double q;
  };
  
  template<typename T>
  DeterministicSignChangeSF<T>::DeterministicSignChangeSF(double qq)
  : SimilarityFunction<T>(), q(qq)
  {
    stringstream ss;
    ss << string("DeterministicSignChangeSF ") << qq;
    descriptor= ss.str();
  }

  template<typename T>
  double DeterministicSignChangeSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    int sc= 0;
    int mult= 1;
    
    T diff0, diff1;
    for ( int i= 0; i < int(a.size())-1; ++i )
    {
      diff0= a(i) + q*mult - b(i);
      diff1= a(i+1) + q*mult*(-1) - b(i+1);
      if ( (diff0 < 0 && diff1 > 0) || (diff1 < 0 && diff0 > 0) )
	sc++;
      
      mult*= -1;
    }
    
    return sc;
  }
  
  template<typename T>
  double DeterministicSignChangeSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    int sc= 0;
    T diff0, diff1;
    int mult= 1;
    
    for ( unsigned int i= 0; i < b.size()-1; ++i )
    {
      diff0= a(n + b(i).first) + q*mult - b(i).second;
      diff1= a(n + b(i+1).first) + q*mult*(-1) - b(i+1).second;
      
      if ( (diff0 < 0 && diff1 > 0) || (diff1 < 0 && diff0 > 0) )
	sc++;
      
      mult*= -1;
    }
    return sc;
  }
  
  template<typename T>
  void DeterministicSignChangeSF<T>::init(Template<T>& /*b*/)
  {
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* DeterministicSignChangeSF<T>::clone()
  {
    return new DeterministicSignChangeSF<T>(q);
  }
  
  template<typename T>
  class MinimumRatioSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      MinimumRatioSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
  };
  
  template<typename T>
  MinimumRatioSF<T>::MinimumRatioSF()
  : SimilarityFunction<T>()
  {
    stringstream ss;
    ss << string("MinimumRatioSF");
    descriptor= ss.str();
  }

  template<typename T>
  double MinimumRatioSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    double mean= 0;
    double r0, r1;
    
    for ( unsigned int i= 0; i < a.size(); ++i )
    {
      if ( b(i) != 0 && a(i) != 0 )
      {
	r0= a(i)/b(i);
	r1= 1.0/r0;
      }
      else if ( b(i) == 0 )
      {
	r0= FLT_MAX;
	r1= 0;
      }
      else if ( a(i) == 0 )
      {
	r0= 0;
	r1= FLT_MAX;
      }
      else
      {
	r0= r1= 0;
      }
      
      if ( r0 < r1 )
	mean+= r0;
      else
	mean+= r1;
    }
    
    return mean/b.size();
  }
  
  template<typename T>
  double MinimumRatioSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    double mean= 0;
    double r0, r1;
    
    for ( unsigned int i= 0; i < b.size(); ++i )
    {
      if ( b(i).second != 0 && a(n + b(i).first) != 0 )
      {
	r0= a(n + b(i).first)/b(i).second;
	r1= 1.0/r0;
      }
      else if ( b(i).second == 0 )
      {
	r0= FLT_MAX;
	r1= 0;
      }
      else if ( a(n + b(i).first) == 0 )
      {
	r0= 0;
	r1= FLT_MAX;
      }
      else
      {
	r0= r1= 0;
      }
      
      if ( r0 < r1 )
	mean+= r0;
      else
	mean+= r1;
    }
    
    return mean/b.size();
  }
  
  template<typename T>
  void MinimumRatioSF<T>::init(Template<T>& /*b*/)
  {
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* MinimumRatioSF<T>::clone()
  {
    return new MinimumRatioSF<T>();
  }
  
  
  template<typename T>
  class SpearmanRhoSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::lastTemplate;

      SpearmanRhoSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      void rank(Vector<T>& input, Vector<int>& ranked);
      
      double computePCC();
      
      virtual SimilarityFunction<T>* clone();
      
      virtual void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
      
      Vector<int> rankeda;
      Vector<int> rankedb;
      
      Vector<T> tmp;
  };
  
  template<typename T>
  SpearmanRhoSF<T>::SpearmanRhoSF()
  : SimilarityFunction<T>()
  {
    stringstream ss;
    ss << string("SpearmanRhoSF");
    descriptor= ss.str();
  }

  template<typename T>
  double SpearmanRhoSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    rank(a, rankeda);
    rank(b, rankedb);
    
    return computePCC();
  }
  
  template<typename T>
  double SpearmanRhoSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
        
    tmp.resize(b.size());
    for ( unsigned int i= 0; i < b.size(); ++i )
      tmp(i)= a(n + b(i).first);
    
    rank(tmp, rankeda);
	
    return computePCC();
  }
  
  template<typename T>
  void SpearmanRhoSF<T>::init(Template<T>& b)
  {
    Vector<T> weights;
    b.getWeights(weights);
    
    rank(weights, rankedb);
    
    initialized= 1;
  }
  
  template<typename T>
  void SpearmanRhoSF<T>::rank(Vector<T>& b, Vector<int>& ranked)
  {
    Vector<IndexWeightPair<T> > data;
    for ( unsigned int i= 0; i < b.size(); ++i )
      data.push_back(IndexWeightPair<T>(i, b(i)));
    sort(data.begin(), data.end());
    
    ranked.resize(b.size());
    
    for ( unsigned int i= 0; i < data.size(); ++i )
      ranked(data(i).index)= i;
  }
  
  template<typename T>
  double SpearmanRhoSF<T>::computePCC()
  {
    double meana= rankeda.getMean();
    double meanb= rankedb.getMean();
    
    double inner= 0, norma= 0, normb= 0;
    for ( unsigned int i= 0; i < rankedb.size(); ++i )
    {
      inner+= (rankeda(i) - meana)*(rankedb(i) - meanb);
      norma+= (rankeda(i) - meana)*(rankeda(i) - meana);
      normb+= (rankedb(i) - meanb)*(rankedb(i) - meanb);
    }
    
    return inner/sqrt(norma*normb);
  }
  
  template<typename T>
  SimilarityFunction<T>* SpearmanRhoSF<T>::clone()
  {
    return new SpearmanRhoSF<T>();
  }
  
  template<typename T>
  void SpearmanRhoSF<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
  {
    temp.updateStride(input.columns);
    
    /*Image<T> filtered;
    filtered.resizeImage(input);
    filtered= 0;
    GaussianFilter2<T, T> gf(1, 7);
    gf.updateStride(input.columns);
    gf.apply(input, filtered);*/
    
    for ( int i= -temp.getMin(); i < int(input.size()) - temp.getMax(); ++i )
      output(i)= evaluate(input, temp, i);
  }
  
  template<typename T>
  class KendallTauSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      KendallTauSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
  };
  
  template<typename T>
  KendallTauSF<T>::KendallTauSF()
  : SimilarityFunction<T>()
  {
    stringstream ss;
    ss << string("KendallTauSF");
    descriptor= ss.str();
  }

  template<typename T>
  double KendallTauSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    int Nc= 0;
    int Nd= 0;
    
    for ( unsigned int i= 0; i < a.size(); ++i )
      for ( unsigned int j= i+1; j < a.size(); ++j )
	if ( (a(j) - a(i))*(b(j) - b(i)) > 0 )
	  Nc+= 1;
	else
	  Nd+= 1;
    
    return 2.0*double(Nc - Nd)/(a.size()*(a.size()-1));
  }
  
  template<typename T>
  double KendallTauSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    int Nc= 0;
    int Nd= 0;
    
    for ( unsigned int i= 0; i < b.size(); ++i )
      for ( unsigned int j= i+1; j < b.size(); ++j )
	if ( (a(n + b(j).first) - a(n + b(i).first))*(b(j).second - b(i).second) > 0 )
	  Nc+= 1;
	else
	  Nd+= 1;
    
    return (Nc - Nd)/(float(b.size()*(b.size()-1)/2.0));
  }
  
  template<typename T>
  void KendallTauSF<T>::init(Template<T>& /*b*/)
  {
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* KendallTauSF<T>::clone()
  {
    return new KendallTauSF<T>();
  }
  
  template<typename T>
  class GreatestDeviationSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::lastTemplate;

      GreatestDeviationSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      void rank(Vector<T>& input, Vector<int>& ranked);
      
      double computeScore();
      
      virtual SimilarityFunction<T>* clone();
      
      virtual void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
      
      Vector<int> rankeda;
      Vector<int> rankedb;
      
      Vector<T> tmp;
      Vector<int> di;
      Vector<int> Di;
  };
  
  template<typename T>
  GreatestDeviationSF<T>::GreatestDeviationSF()
  : SimilarityFunction<T>()
  {
    stringstream ss;
    ss << string("GreatestDeviationSF");
    descriptor= ss.str();
  }

  template<typename T>
  double GreatestDeviationSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    rank(a, rankeda);
    rank(b, rankedb);
    
    return computeScore();
  }
  
  template<typename T>
  double GreatestDeviationSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
        
    tmp.resize(b.size());
    for ( unsigned int i= 0; i < b.size(); ++i )
      tmp(i)= a(n + b(i).first);
    
    rank(tmp, rankeda);
	
    return computeScore();
  }
  
  template<typename T>
  void GreatestDeviationSF<T>::init(Template<T>& b)
  {
    Vector<T> weights;
    b.getWeights(weights);
    
    rank(weights, rankedb);
    
    initialized= 1;
  }
  
  template<typename T>
  void GreatestDeviationSF<T>::rank(Vector<T>& b, Vector<int>& ranked)
  {
    Vector<IndexWeightPair<T> > data;
    for ( unsigned int i= 0; i < b.size(); ++i )
      data.push_back(IndexWeightPair<T>(i, b(i)));
    sort(data.begin(), data.end());
    
    ranked.resize(b.size());
    
    for ( unsigned int i= 0; i < data.size(); ++i )
      ranked(data(i).index)= i;
  }
  
  template<typename T>
  double GreatestDeviationSF<T>::computeScore()
  {
    di.resize(rankeda.size());
    Di.resize(rankeda.size());
    di= 0;
    Di= 0;
    
    for ( unsigned int i= 0; i < di.size(); ++i )
      for ( unsigned int j= 0; j <= i; ++j )
      {
	if ( rankeda(i) <= int(i) && int(i) < rankedb(j) )
	  di(i)++;
	if ( int(rankeda.size() + 1 - rankeda(i)) > rankedb(j) )
	  Di(i)++;
      }
    
    return 2.0*double(Di.getMax() - di.getMax())/double(rankeda.size());
  }
  
  template<typename T>
  SimilarityFunction<T>* GreatestDeviationSF<T>::clone()
  {
    return new GreatestDeviationSF<T>();
  }
  
  template<typename T>
  void GreatestDeviationSF<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
  {
    temp.updateStride(input.columns);
    
    Image<T> filtered;
    filtered.resizeImage(input);
    filtered= 0;
    GaussianFilter2<T, T> gf(1, 7);
    gf.updateStride(input.columns);
    gf.apply(input, filtered);
    
    for ( int i= -temp.getMin(); i < int(input.size()) - temp.getMax(); ++i )
      output(i)= evaluate(filtered, temp, i);
  }
  
  template<typename T>
  class OrdinalMeasureSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      OrdinalMeasureSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      void rank(Vector<T>& input, Vector<int>& ranked);
      
      double computeScore();
      
      virtual SimilarityFunction<T>* clone();
      
      Vector<int> rankeda;
      Vector<int> rankedb;
      
      Vector<T> tmp;
      Vector<int> Di;
  };
  
  template<typename T>
  OrdinalMeasureSF<T>::OrdinalMeasureSF()
  : SimilarityFunction<T>()
  {
    stringstream ss;
    ss << string("OrdinalMeasureSF");
    descriptor= ss.str();
  }

  template<typename T>
  double OrdinalMeasureSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    rank(a, rankeda);
    rank(b, rankedb);
    
    return computeScore();
  }
  
  template<typename T>
  double OrdinalMeasureSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
        
    tmp.resize(b.size());
    for ( unsigned int i= 0; i < b.size(); ++i )
      tmp(i)= a(n + b(i).first);
    
    rank(tmp, rankeda);
	
    return computeScore();
  }
  
  template<typename T>
  void OrdinalMeasureSF<T>::init(Template<T>& b)
  {
    Vector<T> weights;
    b.getWeights(weights);
    
    rank(weights, rankedb);
    
    initialized= 1;
  }
  
  template<typename T>
  void OrdinalMeasureSF<T>::rank(Vector<T>& b, Vector<int>& ranked)
  {
    Vector<IndexWeightPair<T> > data;
    for ( unsigned int i= 0; i < b.size(); ++i )
      data.push_back(IndexWeightPair<T>(i, b(i)));
    sort(data.begin(), data.end());
    
    ranked.resize(b.size());
    
    for ( unsigned int i= 0; i < data.size(); ++i )
      ranked(data(i).index)= i;
  }
  
  template<typename T>
  double OrdinalMeasureSF<T>::computeScore()
  {
    Di.resize(rankeda.size());
    Di= 0;
    
    for ( unsigned int i= 0; i < Di.size(); ++i )
      for ( unsigned int j= 0; j <= i; ++j )
      {
	if ( (int)(rankeda.size() + 1 - rankeda(i)) > rankedb(j) )
	  Di(i)++;
      }
    
    return 2.0*double(Di.getMax())/double(rankeda.size());
  }
  
  template<typename T>
  SimilarityFunction<T>* OrdinalMeasureSF<T>::clone()
  {
    return new OrdinalMeasureSF<T>();
  }
  
  template<typename T>
  class CorrelationRatioSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      CorrelationRatioSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual void initialize(Vector<T>& b);
      
      double computeScore(Vector<T>& a, int n);
      
      virtual SimilarityFunction<T>* clone();
      
      Vector<Vector<int> > positions;
      Vector<T> values;
      Vector<T> means;
      Vector<T> mean2s;
  };
  
  template<typename T>
  CorrelationRatioSF<T>::CorrelationRatioSF()
  : SimilarityFunction<T>()
  {
    stringstream ss;
    ss << string("CorrelationRatioSF");
    descriptor= ss.str();
  }

  template<typename T>
  double CorrelationRatioSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initialize(b);
    
    return computeScore(a, 0);
  }
  
  template<typename T>
  double CorrelationRatioSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
        
    return computeScore(a, n);
  }
  
  template<typename T>
  void CorrelationRatioSF<T>::init(Template<T>& b)
  {
    Vector<T> weights;
    b.getWeights(weights);
    
    initialize(weights);
    
    for ( unsigned int i= 0; i < positions.size(); ++i )
      for ( unsigned int j= 0; j < positions(i).size(); ++j )
	positions(i)(j)= b(positions(i)(j)).first;
    
    initialized= 1;
  }
  
  template<typename T>
  void CorrelationRatioSF<T>::initialize(Vector<T>& b)
  {
    values= b;
    
    sort(values.begin(), values.end());
    typename Vector<T>::iterator it= unique(values.begin(), values.end());
    values.resize(distance(values.begin(), it));
    
    positions.resize(values.size());
    
    for ( unsigned int i= 0; i < values.size(); ++i )
      for ( unsigned int j= 0; j < b.size(); ++j )
	if ( values(i) == b(j) )
	  positions(i).push_back(j);
  }
  
  template<typename T>
  double CorrelationRatioSF<T>::computeScore(Vector<T>& a, int n)
  {
    means.resize(values.size());
    means= 0;
    mean2s.resize(values.size());
    mean2s= 0;
    for ( unsigned int i= 0; i < positions.size(); ++i )
    {
      for ( unsigned int j= 0; j < positions(i).size(); ++j )
      {
	means(i)+= a(n + positions(i)(j));
	mean2s(i)+= a(n + positions(i)(j))*a(n + positions(i)(j));
      }
      means(i)/= positions(i).size();
      mean2s(i)/= positions(i).size();
    }
    
    double result= 0;
    for ( unsigned int i= 0; i < values.size(); ++i )
      result+= positions(i).size()*(mean2s(i) - means(i)*means(i));
    result/= a.size();
    
    return sqrt(1 - result);
  }
  
  template<typename T>
  SimilarityFunction<T>* CorrelationRatioSF<T>::clone()
  {
    return new CorrelationRatioSF<T>();
  }
  
  template<typename T>
  class EnergyOfJointProbabilityDistributionSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      EnergyOfJointProbabilityDistributionSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
      
      DiscreteJointHistogram jpd;
      Vector<float> weights;
      Vector<float> tmp;
  };
  
  template<typename T>
  EnergyOfJointProbabilityDistributionSF<T>::EnergyOfJointProbabilityDistributionSF()
  : SimilarityFunction<T>()
  {
    stringstream ss;
    ss << string("EnergyOfJointProbabilityDistributionSF");
    descriptor= ss.str();
  }

  template<typename T>
  double EnergyOfJointProbabilityDistributionSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    jpd.computeHistogram(a, b, 100);
    jpd.normalize();
    
    float result= 0;
    for ( unsigned int i= 0; i < jpd.n; ++i )
      result+= jpd(i)*jpd(i);
    
    return result;
  }
  
  template<typename T>
  double EnergyOfJointProbabilityDistributionSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    tmp.resize(weights.size());
    for ( unsigned int i= 0; i < b.size(); ++i )
      tmp(i)= a(n + b(i).first);
    
    return evaluate(tmp, weights);
  }
  
  template<typename T>
  void EnergyOfJointProbabilityDistributionSF<T>::init(Template<T>& b)
  {
    b.getWeights(weights);
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* EnergyOfJointProbabilityDistributionSF<T>::clone()
  {
    return new EnergyOfJointProbabilityDistributionSF<T>();
  }
  
  template<typename T>
  class MaterialSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      MaterialSF(int k= 4);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
      
      DiscreteJointHistogram jpda;
      DiscreteJointHistogram jpdb;
      int k;
      Vector<unsigned char> maska;
      Vector<unsigned char> maskb;
      
      Vector<int> peakinda;
      Vector<int> peakindb;
      Vector<float> peakvala;
      Vector<float> peakvalb;
      
      Vector<float> weights;
      Vector<float> values;
  };
  
  template<typename T>
  MaterialSF<T>::MaterialSF(int kk)
  : SimilarityFunction<T>(), k(kk)
  {
    stringstream ss;
    ss << string("MaterialSF ") << k;
    descriptor= ss.str();
  }

  template<typename T>
  double MaterialSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    if ( maska.size() != a.size() )
    {
      maska.resize(a.size());
      maskb.resize(b.size());
      maska= 0;
      maskb= 0;
      for ( unsigned int i= 0; i < a.size(); i+= k )
	maska(i)= 1;
      for ( unsigned int i= k/2; i < b.size(); i+= k )
	maskb(i)= 1;
    }

    jpda.computeHistogram(a, b, 100, &maska);
    jpdb.computeHistogram(a, b, 100, &maskb);
    
    jpda.normalize();
    jpdb.normalize();
    
    jpda.smoothWithGaussianKernel(3);
    jpdb.smoothWithGaussianKernel(3);
    
    jpda.getPeaksInColumns(peakinda, peakvala);
    jpdb.getPeaksInColumns(peakindb, peakvalb);
    
    float result= 0, tmp;
    for ( unsigned int i= 0; i < peakinda.size(); ++i )
    {
      tmp= peakvala(i) < peakvalb(i) ? peakvala(i) : peakvalb(i);
      result+= tmp/(fabs(peakinda(i) - peakindb(i)) + 1);
    }
    
    return result;
  }
  
  template<typename T>
  double MaterialSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    values.resize(weights.size());
    for ( unsigned int i= 0; i < b.size(); ++i )
      values(i)= a(n + b(i).first);
    
    return evaluate(values, weights);
  }
  
  template<typename T>
  void MaterialSF<T>::init(Template<T>& b)
  {
    b.getWeights(weights);
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* MaterialSF<T>::clone()
  {
    return new MaterialSF<T>(k);
  }
  
  template<typename T>
  class ShannonMutualInformationSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      ShannonMutualInformationSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
      
      DiscreteJointHistogram jpd;
      
      Vector<float> weights;
      Vector<float> values;
  };
  
  template<typename T>
  ShannonMutualInformationSF<T>::ShannonMutualInformationSF()
  : SimilarityFunction<T>()
  {
    stringstream ss;
    ss << string("ShannonMutualInformationSF ");
    descriptor= ss.str();
  }

  template<typename T>
  double ShannonMutualInformationSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    jpd.computeHistogram(a, b, 100);
    jpd.normalize();
    
    return jpd.getShannonEntropy();
  }
  
  template<typename T>
  double ShannonMutualInformationSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    values.resize(weights.size());
    for ( unsigned int i= 0; i < b.size(); ++i )
      values(i)= a(n + b(i).first);
    
    return evaluate(values, weights);
  }
  
  template<typename T>
  void ShannonMutualInformationSF<T>::init(Template<T>& b)
  {
    b.getWeights(weights);
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* ShannonMutualInformationSF<T>::clone()
  {
    return new ShannonMutualInformationSF<T>();
  }
  
  template<typename T>
  class RenyiMutualInformationSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      RenyiMutualInformationSF(float alpha= 2);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
      
      DiscreteJointHistogram jpd;
      
      Vector<float> weights;
      Vector<float> values;
      
      float alpha;
  };
  
  template<typename T>
  RenyiMutualInformationSF<T>::RenyiMutualInformationSF(float a)
  : SimilarityFunction<T>(), alpha(a)
  {
    stringstream ss;
    ss << string("RenyiMutualInformationSF ") << alpha;
    descriptor= ss.str();
  }

  template<typename T>
  double RenyiMutualInformationSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    jpd.computeHistogram(a, b, 100);
    jpd.normalize();
    
    return jpd.getRenyiEntropy(alpha);
  }
  
  template<typename T>
  double RenyiMutualInformationSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    values.resize(weights.size());
    for ( unsigned int i= 0; i < b.size(); ++i )
      values(i)= a(n + b(i).first);
    
    return evaluate(values, weights);
  }
  
  template<typename T>
  void RenyiMutualInformationSF<T>::init(Template<T>& b)
  {
    b.getWeights(weights);
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* RenyiMutualInformationSF<T>::clone()
  {
    return new RenyiMutualInformationSF<T>(alpha);
  }
  
  template<typename T>
  class TsallisMutualInformationSF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      TsallisMutualInformationSF(float q= 2);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
      
      DiscreteJointHistogram jpd;
      
      Vector<float> weights;
      Vector<float> values;
      
      float q;
  };
  
  template<typename T>
  TsallisMutualInformationSF<T>::TsallisMutualInformationSF(float a)
  : SimilarityFunction<T>(), q(a)
  {
    stringstream ss;
    ss << string("TsallisMutualInformationSF ") << q;
    descriptor= ss.str();
  }

  template<typename T>
  double TsallisMutualInformationSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    jpd.computeHistogram(a, b, 100);
    jpd.normalize();
    
    return jpd.getTsallisEntropy(q);
  }
  
  template<typename T>
  double TsallisMutualInformationSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    values.resize(weights.size());
    for ( unsigned int i= 0; i < b.size(); ++i )
      values(i)= a(n + b(i).first);
    
    return evaluate(values, weights);
  }
  
  template<typename T>
  void TsallisMutualInformationSF<T>::init(Template<T>& b)
  {
    b.getWeights(weights);
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* TsallisMutualInformationSF<T>::clone()
  {
    return new TsallisMutualInformationSF<T>(q);
  }
  
  template<typename T>
  class FInformationMeasureISF: public SimilarityFunction<T>
  {
  public:
      using SimilarityFunction<T>::descriptor;
      using SimilarityFunction<T>::initialized;
      using SimilarityFunction<T>::apply1;
      using SimilarityFunction<T>::apply2;
      using SimilarityFunction<T>::lastTemplate;

      FInformationMeasureISF(float alpha= 2);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual SimilarityFunction<T>* clone();
      
      DiscreteJointHistogram jpd;
      
      Vector<float> weights;
      Vector<float> values;
      
      float alpha;
  };
  
  template<typename T>
  FInformationMeasureISF<T>::FInformationMeasureISF(float a)
  : SimilarityFunction<T>(), alpha(a)
  {
    stringstream ss;
    ss << string("FInformationMeasureISF ") << alpha;
    descriptor= ss.str();
  }

  template<typename T>
  double FInformationMeasureISF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    jpd.computeHistogram(a, b, 100);
    jpd.normalize();
    
    return jpd.getFInformationI(alpha);
  }
  
  template<typename T>
  double FInformationMeasureISF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    values.resize(weights.size());
    for ( unsigned int i= 0; i < b.size(); ++i )
      values(i)= a(n + b(i).first);
    
    return evaluate(values, weights);
  }
  
  template<typename T>
  void FInformationMeasureISF<T>::init(Template<T>& b)
  {
    b.getWeights(weights);
    initialized= 1;
  }
  
  template<typename T>
  SimilarityFunction<T>* FInformationMeasureISF<T>::clone()
  {
    return new FInformationMeasureISF<T>(alpha);
  }
}

#endif
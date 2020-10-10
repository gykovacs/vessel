#ifndef DISSIMILARITY_FUNCTIONS_H
#define DISSIMILARITY_FUNCTIONS_H

#include <openipML/SimilarityFunction.h>
#include <openipDS/Image.h>
#include <openipML/DiscreteHistogram.h>
#include <openipML/KMeansClusteringDiscretizer.h>
#include <openipML/EqualWidthDiscretizer.h>
#include <openipML/EqualFrequencyDiscretizer.h>
#include <openipSC/matrixOperations.h>
#include <openipML/PoolAdjacentViolatorsAlgorithm.h>
#include <openipLL/thinning.h>

#include <vector>
#include <exception>
#include <stdexcept>
#include <algorithm>

namespace openip
{
  template<typename T>
  class L1NormDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      L1NormDSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
  };

  template<typename T>
  L1NormDSF<T>::L1NormDSF()
  : DissimilarityFunction<T>()
  {
    descriptor= string("L1NormDSF");
  }

  template<typename T>
  double L1NormDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    float result= 0;
    for ( unsigned int i= 0; i < a.size(); ++i )
      result+= fabs(a(i) - b(i));
    return result;
  }
  
  template<typename T>
  double L1NormDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    float result= 0;
    for ( unsigned int i= 0; i < b.size(); ++i )
      result+= fabs(a(n + b(i).first) - b(i).second);
    
    return result;
  }
  
  template<typename T>
  void L1NormDSF<T>::init(Template<T>& /*b*/)
  {
    initialized= 1;
  }
  
  template<typename T>
  DissimilarityFunction<T>* L1NormDSF<T>::clone()
  {
    return new L1NormDSF<T>();
  }
  
  template<typename T>
  class MedianOfAbsoluteDifferencesDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MedianOfAbsoluteDifferencesDSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      Vector<double> diffs;
  };

  template<typename T>
  MedianOfAbsoluteDifferencesDSF<T>::MedianOfAbsoluteDifferencesDSF()
  : DissimilarityFunction<T>()
  {
    descriptor= string("MedianOfAbsoluteDifferencesDSF");
  }

  template<typename T>
  double MedianOfAbsoluteDifferencesDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    diffs.resize(a.size());
    
    for ( unsigned int i= 0; i < a.size(); ++i )
      diffs(i)= fabs(a(i) - b(i));
    
    return diffs.getMedian();
  }
  
  template<typename T>
  double MedianOfAbsoluteDifferencesDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    diffs.resize(b.size());
    
    for ( unsigned int i= 0; i < b.size(); ++i )
      diffs(i)= fabs(a(n + b(i).first) - b(i).second);
    
    return diffs.getMedian();
  }
  
  template<typename T>
  void MedianOfAbsoluteDifferencesDSF<T>::init(Template<T>& /*b*/)
  {
    initialized= 1;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MedianOfAbsoluteDifferencesDSF<T>::clone()
  {
    return new MedianOfAbsoluteDifferencesDSF<T>();
  }
  
  template<typename T>
  class SquareL2NormDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      SquareL2NormDSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
  };

  template<typename T>
  SquareL2NormDSF<T>::SquareL2NormDSF()
  : DissimilarityFunction<T>()
  {
    descriptor= string("SquareL2NormDSF");
  }

  template<typename T>
  double SquareL2NormDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    float result= 0;
    for ( unsigned int i= 0; i < a.size(); ++i )
      result+= (a(i) - b(i))*(a(i) - b(i));
    return result;
  }
  
  template<typename T>
  double SquareL2NormDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    float result= 0;
    for ( unsigned int i= 0; i < b.size(); ++i )
      result+= (a(n + b(i).first) - b(i).second)*(a(n + b(i).first) - b(i).second);
    
    return result;
  }
  
  template<typename T>
  void SquareL2NormDSF<T>::init(Template<T>& /*b*/)
  {
    initialized= 1;
  }
  
  template<typename T>
  DissimilarityFunction<T>* SquareL2NormDSF<T>::clone()
  {
    return new SquareL2NormDSF<T>();
  }
  
  template<typename T>
  class MedianOfSquareDifferencesDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MedianOfSquareDifferencesDSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      Vector<double> diffs;
  };

  template<typename T>
  MedianOfSquareDifferencesDSF<T>::MedianOfSquareDifferencesDSF()
  : DissimilarityFunction<T>()
  {
    descriptor= string("MedianOfSquareDifferencesDSF");
  }

  template<typename T>
  double MedianOfSquareDifferencesDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    diffs.resize(a.size());
    
    for ( unsigned int i= 0; i < a.size(); ++i )
      diffs(i)= (a(i) - b(i))*(a(i) - b(i));
    
    return diffs.getMedian();
  }
  
  template<typename T>
  double MedianOfSquareDifferencesDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    diffs.resize(b.size());
    
    for ( unsigned int i= 0; i < b.size(); ++i )
      diffs(i)= (a(n + b(i).first) - b(i).second)*(a(n + b(i).first) - b(i).second);
    
    return diffs.getMedian();
  }
  
  template<typename T>
  void MedianOfSquareDifferencesDSF<T>::init(Template<T>& /*b*/)
  {
    initialized= 1;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MedianOfSquareDifferencesDSF<T>::clone()
  {
    return new MedianOfSquareDifferencesDSF<T>();
  }
  
  template<typename T>
  class NormalizedSquareL2NormDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      NormalizedSquareL2NormDSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      float meanb;
      float stdb;
  };

  template<typename T>
  NormalizedSquareL2NormDSF<T>::NormalizedSquareL2NormDSF()
  : DissimilarityFunction<T>()
  {
    descriptor= string("NormalizedSquareL2NormDSF");
    meanb= stdb= 0;
  }

  template<typename T>
  double NormalizedSquareL2NormDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    float result= 0;
    float tmp, meana= a.getMean(), stda= a.getStandardDeviation();
    meanb= b.getMean(), stdb= b.getStandardDeviation();
    for ( unsigned int i= 0; i < a.size(); ++i )
    {
      tmp= (a(i) - meana)/stda - (b(i) - meanb)/stdb;
      result= tmp*tmp;
    }
    
    return result;
  }
  
  template<typename T>
  double NormalizedSquareL2NormDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    float result= 0;
    float tmp, meana= 0, stda= 0;
    
    for ( unsigned int i= 0; i < b.size(); ++i )
    {
      meana+= a(n + b(i).first);
      stda+= a(n + b(i).first)*a(n + b(i).first);
    }
    meana/= b.size();
    stda/= b.size();
    stda= sqrt(stda - meana*meana);
    
    for ( unsigned int i= 0; i < b.size(); ++i )
    {
      tmp= (a(n + b(i).first) - meana)/stda - (b(i).second - meanb)/stdb;
      result= tmp*tmp;
    }
    
    return result;
  }
  
  template<typename T>
  void NormalizedSquareL2NormDSF<T>::init(Template<T>& b)
  {
    initialized= 1;
    Vector<float> weights;
    b.getWeights(weights);
    meanb= weights.getMean();
    stdb= weights.getStandardDeviation();
  }
  
  template<typename T>
  DissimilarityFunction<T>* NormalizedSquareL2NormDSF<T>::clone()
  {
    return new NormalizedSquareL2NormDSF<T>();
  }
  
  template<typename T>
  class IncrementalSignDistanceDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      IncrementalSignDistanceDSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
  };

  template<typename T>
  IncrementalSignDistanceDSF<T>::IncrementalSignDistanceDSF()
  : DissimilarityFunction<T>()
  {
    descriptor= string("IncrementalSignDistanceDSF");
  }

  template<typename T>
  double IncrementalSignDistanceDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    float result= 0;
    
    for ( int i= 0; i < int(a.size())-1; ++i )
      if ( (a(i) > a(i+1) && b(i) <= b(i+1)) || (a(i) <= a(i+1) && b(i) > b(i+1)) )
	result+= 1.0f;
    
    return result;
  }
  
  template<typename T>
  double IncrementalSignDistanceDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    float result= 0;
    
    for ( int i= 0; i < int(b.size())-1; ++i )
      if ( (a(n + b(i).first) > a(n + b(i+1).first) && b(i).second <= b(i+1).second) || (a(n + b(i).first) <= a(n + b(i+1).first) && b(i).second > b(i+1).second) )
	result+= 1.0f;
    
    return result;
  }
  
  template<typename T>
  void IncrementalSignDistanceDSF<T>::init(Template<T>& /*b*/)
  {
    initialized= 1;
  }
  
  template<typename T>
  DissimilarityFunction<T>* IncrementalSignDistanceDSF<T>::clone()
  {
    return new IncrementalSignDistanceDSF<T>();
  }
  
  template<typename T>
  class IntensityRatioVarianceDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      IntensityRatioVarianceDSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      Vector<float> tmp;
  };

  template<typename T>
  IntensityRatioVarianceDSF<T>::IntensityRatioVarianceDSF()
  : DissimilarityFunction<T>()
  {
    descriptor= string("IntensityRatioVarianceDSF");
  }

  template<typename T>
  double IntensityRatioVarianceDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    float epsilon= 1;
    
    tmp.resize(a.size());
    
    for ( unsigned int i= 0; i < a.size(); ++i )
      tmp(i)= (a(i) + epsilon)/(b(i) + epsilon);
    
    return tmp.getVariance();
  }
  
  template<typename T>
  double IntensityRatioVarianceDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    float epsilon= 1;

    tmp.resize(b.size());
    
    for ( unsigned int i= 0; i < b.size(); ++i )
      tmp(i)= (a(n + b(i).first) + epsilon)/(b(i).second + epsilon);
    
    return tmp.getVariance();
  }
  
  template<typename T>
  void IntensityRatioVarianceDSF<T>::init(Template<T>& /*b*/)
  {
    initialized= 1;
  }
  
  template<typename T>
  DissimilarityFunction<T>* IntensityRatioVarianceDSF<T>::clone()
  {
    return new IntensityRatioVarianceDSF<T>();
  }
  
  template<typename T>
  class RankDistanceDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::lastTemplate;

      RankDistanceDSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      void rank(Vector<T>& input, Vector<int>& ranked);
      
      double computeDist();
      
      virtual DissimilarityFunction<T>* clone();
      
      //virtual void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
      
      Vector<int> rankeda;
      Vector<int> rankedb;
      
      Vector<T> tmp;
  };
  
  template<typename T>
  RankDistanceDSF<T>::RankDistanceDSF()
  : DissimilarityFunction<T>()
  {
    stringstream ss;
    ss << string("RankDistanceDSF");
    descriptor= ss.str();
  }

  template<typename T>
  double RankDistanceDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    rank(a, rankeda);
    rank(b, rankedb);
    
    return computeDist();
  }
  
  template<typename T>
  double RankDistanceDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
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
	
    return computeDist();
  }
  
  template<typename T>
  void RankDistanceDSF<T>::init(Template<T>& b)
  {
    Vector<T> weights;
    b.getWeights(weights);
    
    rank(weights, rankedb);
    
    initialized= 1;
  }
  
  template<typename T>
  void RankDistanceDSF<T>::rank(Vector<T>& b, Vector<int>& ranked)
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
  double RankDistanceDSF<T>::computeDist()
  {
    float result= 0;
    for ( unsigned int i= 0; i < rankeda.size(); ++i )
      result+= fabs((rankeda(i) - rankedb(i)));
    result/= rankeda.size();
    
    return result;
  }
  
  template<typename T>
  DissimilarityFunction<T>* RankDistanceDSF<T>::clone()
  {
    return new RankDistanceDSF<T>();
  }
  
  //template<typename T>
  //void RankDistanceDSF<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
  /*{
    temp.updateStride(input.columns);
    
    Image<T> filtered;
    filtered.resizeImage(input);
    filtered= 0;
    GaussianFilter2<T, T> gf(1, 7);
    gf.updateStride(input.columns);
    gf.apply(input, filtered);
    
    int start= -temp.getMin();
    int end= int(input.n) - temp.getMax();
    
#pragma omp parallel for
    for ( int i= start; i < end; ++i )
      output(i)= evaluate(filtered, temp, i);
  }*/
  
  template<typename T>
  class JointEntropyDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      JointEntropyDSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      DiscreteJointHistogram jpd;
      
      Vector<float> weights;
      Vector<float> values;
  };
  
  template<typename T>
  JointEntropyDSF<T>::JointEntropyDSF()
  : DissimilarityFunction<T>()
  {
    stringstream ss;
    ss << string("JointEntropyDSF ");
    descriptor= ss.str();
  }

  template<typename T>
  double JointEntropyDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    jpd.computeHistogram(a, b, 100);
    jpd.normalize();
    
    return jpd.getShannonJointEntropy();
  }
  
  template<typename T>
  double JointEntropyDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
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
  void JointEntropyDSF<T>::init(Template<T>& b)
  {
    b.getWeights(weights);
    initialized= 1;
  }
  
  template<typename T>
  DissimilarityFunction<T>* JointEntropyDSF<T>::clone()
  {
    return new JointEntropyDSF<T>();
  }
  
  template<typename T>
  class ExclusiveFInformationDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      ExclusiveFInformationDSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      DiscreteJointHistogram jpd;
      
      Vector<float> weights;
      Vector<float> values;
  };
  
  template<typename T>
  ExclusiveFInformationDSF<T>::ExclusiveFInformationDSF()
  : DissimilarityFunction<T>()
  {
    stringstream ss;
    ss << string("ExclusiveFInformationDSF ");
    descriptor= ss.str();
  }

  template<typename T>
  double ExclusiveFInformationDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    jpd.computeHistogram(a, b, 100);
    jpd.normalize();
    
    return jpd.getShannonJointEntropy() - jpd.getShannonEntropy();
  }
  
  template<typename T>
  double ExclusiveFInformationDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
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
  void ExclusiveFInformationDSF<T>::init(Template<T>& b)
  {
    b.getWeights(weights);
    initialized= 1;
  }
  
  template<typename T>
  DissimilarityFunction<T>* ExclusiveFInformationDSF<T>::clone()
  {
    return new ExclusiveFInformationDSF<T>();
  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template<typename T>
  int decision(Image<T>& input, Template2<T>& temp, int k1, int k2, int disc)
  {
    double same= 0;
    double neighbors= 0;

      {
        DataSet<T> dataset;
        ContinuousFeatureVector entry;
        entry.resize(1);
        Vector<float> weights;
        temp.getWeights(weights);
        for ( unsigned int i= 0; i < weights.size(); ++i )
        {
            entry(0)= weights(i);
            dataset.push_back(entry);
        }
        Discretizer<T>* discretizer;
        if ( disc == 0 )
            discretizer= new EqualWidthDiscretizer<T>(k2);
        else if ( disc == 1 )
            discretizer= new KMeansClusteringDiscretizer<T>(k2, 28, 100);
        else
            discretizer= new EqualWidthDiscretizer<T>(k2);
            
        discretizer->init(dataset);
        
        Vector<float> borders;
        borders.clear();
        for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
            borders.push_back(discretizer->data(i)(0));
        borders.insert(borders.begin(), -FLT_MAX);
        borders.push_back(FLT_MAX);

        //Vector<float> weights= temp.getWeights();
                
        Image<unsigned char> labelimage;
        labelimage.resizeImage(temp.image);
        //tprintf("template image: %d %d\n", lastTemplate->image.rows, lastTemplate->image.columns);
        for ( int i= 0; i < /*(*lastTemplate)*/temp.image.n; ++i )
        {
            for ( int j= 0; j < borders.size(); ++j )
                if ( temp.image(i) < borders(j) )
                {
                    labelimage(i)= j-1;
                    break;
                }
        }
        int nSame= 0;
        for ( int i= 1; i < labelimage.rows-1; ++i )
            for ( int j= 1; j < labelimage.columns-1; ++j )
                {
                    if ( labelimage(i,j) == labelimage(i,j+1) )
                        same+= 1;
                    if ( labelimage(i,j) == labelimage(i,j-1) )
                        same+= 1;
                    if ( labelimage(i,j) == labelimage(i-1,j) )
                        same+= 1;
                    if ( labelimage(i,j) == labelimage(i+1,j) )
                        same+= 1;
                    nSame++;
                }
        same/= nSame;
        tprintf("neighbors: %f\n", same);

      }
      {
            Image<T> slicedImage0;
            slicedImage0.resizeImage(input);
                
            DataSet<T> dataset;
            ContinuousFeatureVector entry;
            entry.classIndex= 0;
            entry.resize(1);
            for ( unsigned int i= 0; i < input.n; ++i )
            {
                entry(0)= input(i);
                dataset.push_back(entry);
            }
                
            Discretizer<T>* discretizer;
            if ( disc == 0 )
                discretizer= new EqualWidthDiscretizer<T>(k1);
            else if ( disc == 1 )
                discretizer= new KMeansClusteringDiscretizer<T>(k1, 28, 25);
            else
                discretizer= new EqualWidthDiscretizer<T>(k1);
            
            discretizer->init(dataset);
            
            Vector<float> borders;
            borders.clear();
            for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
                borders.push_back(discretizer->data(i)(0));
            borders.insert(borders.begin(), -FLT_MAX);
            borders.push_back(FLT_MAX);
                        
            for ( unsigned int i= 0; i < input.n; ++i )
                for ( unsigned int j= 0; j < borders.size(); ++j )
                {
                    if ( input(i) < borders(j) )
                    {
                        slicedImage0(i)= j-1;
                        break;
                    }
                }
                        
            delete discretizer;
            
            int nNeighbors= 0;
            
            for ( int i= 1; i < slicedImage0.rows-1; ++i )
                for ( int j= 1; j < slicedImage0.columns-1; ++j )
                {
                    if ( slicedImage0(i,j) == slicedImage0(i,j+1) )
                        neighbors+= 1;
                    if ( slicedImage0(i,j) == slicedImage0(i,j-1) )
                        neighbors+= 1;
                    if ( slicedImage0(i,j) == slicedImage0(i+1,j) )
                        neighbors+= 1;
                    if ( slicedImage0(i,j) == slicedImage0(i-1,j) )
                        neighbors+= 1;
                    ++nNeighbors;
                }
            neighbors/= nNeighbors;
      }
      
      tprintf("decision: %f %f, %d\n", same, neighbors, same >= neighbors );
      if ( same < neighbors )
          return 1;
      else if ( same >= neighbors )
          return 0;
      return 0;
  }
  
  
  
  
  
  template<typename T>
  class MatchingByToneMappingPWCDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MatchingByToneMappingPWCDSF(int bins, int discretizer= 0);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a, int n);
      
      int bins;
      Vector<Vector<int> > slices;
      Vector<Vector<int> > masks;
      Vector<float> borders;
      Vector<float> means;
      Vector<float> nums;
      int discretizer;
  };
  
  template<typename T>
  MatchingByToneMappingPWCDSF<T>::MatchingByToneMappingPWCDSF(int b, int d)
  : DissimilarityFunction<T>(), bins(b), discretizer(d)
  {
    stringstream ss;
    ss << string("MatchingByToneMappingPWCDSF ") << bins << " " << discretizer;
    descriptor= ss.str();
  }

  template<typename T>
  double MatchingByToneMappingPWCDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initPositions(b);
    
    double score= computeScore(a, 0);
    double result= score/(a.size()*a.getVariance());
    return result;
  }
  
  template<typename T>
  double MatchingByToneMappingPWCDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      init(b);
      lastTemplate= &b;
    }
    
    double mean= 0;
    double mean2= 0;
    
    for ( unsigned int i= 0; i < b.size(); ++i )
    {
      mean+= a(n + b(i).first);
      mean2+= a(n + b(i).first)*a(n + b(i).first);
    }
    mean/= b.size();
    mean2/= b.size();
    double var= mean2 - mean*mean;
    
    double score= computeScore(a, n);
    double result= score/(b.size()*var);
    if ( var < 0.0001 )
        return 1;

    return result;
  }
  
  template<typename T>
  void MatchingByToneMappingPWCDSF<T>::init(Template<T>& b)
  {
    Vector<T> weights;
    b.getWeights(weights);
    
    initPositions(weights);
    
    int flag;
    for ( unsigned int k= 0; k < weights.size(); ++k )
    {
      for ( unsigned int i= 0; i < slices.size(); ++i )
      {
	flag= 0;
	for ( unsigned int j= 0; j < slices(i).size(); ++j )
	  if ( masks(i)(j) && slices(i)(j) == int(k) )
	  {
	    slices(i)(j)= b(k).first;
            masks(i)(j)= 0;
	    flag= 1;
	    break;
	  }
	if ( flag )
	  continue;
      }
    }
    
    initialized= 1;
  }
  
  template<typename T>
  void MatchingByToneMappingPWCDSF<T>::initPositions(Vector<T>& weights)
  {
    DataSet<T> dataset;
    ContinuousFeatureVector entry;
    entry.resize(1);
    for ( unsigned int i= 0; i < weights.size(); ++i )
    {
      entry(0)= weights(i);
      dataset.push_back(entry);
    }
    
    int error= 0;
    do
    {
        error= 0;
        Discretizer<T>* discretizer;
        if ( this->discretizer == 0 )
            discretizer= new EqualWidthDiscretizer<T>(bins);
        else if ( this->discretizer == 1 )
            discretizer= new KMeansClusteringDiscretizer<T>(bins, 28, 25);
        else
            discretizer= new EqualWidthDiscretizer<T>(bins);
        
        discretizer->init(dataset);
        
        borders.clear();
        for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
            borders.push_back(discretizer->data(i)(0));
        borders.insert(borders.begin(), -FLT_MAX);
        borders.push_back(FLT_MAX);
        slices.clear();
        masks.clear();
        slices.resize(bins);
        masks.resize(bins);
        
        for ( unsigned int i= 0; i < weights.size(); ++i )
        for ( unsigned int j= 0; j < borders.size(); ++j )
        {
            if ( weights(i) < borders(j) )
            {
            slices(j-1).push_back(i);
            masks(j-1).push_back(1);
            break;
            }
        }
        
        for ( unsigned int i= 0; i < slices.size(); ++i )
            if ( slices(i).size() == 0 )
            {
                error= 1;
                --bins;
                break;
            }
        delete discretizer;
    }while ( error );
      
    means.resize(bins);
    nums.resize(bins);
  }
  
  template<typename T>
  double MatchingByToneMappingPWCDSF<T>::computeScore(Vector<T>& a, int n)
  {
    means= 0;
    nums= 0;
    
    for ( unsigned int i= 0; i < slices.size(); ++i )
      for ( unsigned int j= 0; j < slices(i).size(); ++j )
      {
	means(i)+= a(n + slices(i)(j));
	nums(i)+= 1.0;
      }
      
    for ( unsigned int i= 0; i < means.size(); ++i )
      if ( nums(i) > 0 )
	means(i)/= nums(i);
      else
	means(i)= 0;
      
    double result= 0;
    for ( unsigned int i= 0; i < slices.size(); ++i )
      for ( unsigned int j= 0; j < slices(i).size(); ++j )
	result+= (means(i) - a(n + slices(i)(j)))*(means(i) - a(n + slices(i)(j)));
      
    
    
    return result;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MatchingByToneMappingPWCDSF<T>::clone()
  {
    return new MatchingByToneMappingPWCDSF<T>(bins, discretizer);
  }
  

  
  template<typename T>
  class MatchingByMonotonousToneMappingPWCDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;
      using DissimilarityFunction<T>::flag;

      MatchingByMonotonousToneMappingPWCDSF(int bins, int discretizer= 0, float weight= 1, int wtp= 0, int nonmonotonic= 0);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a, int n);
      
      int bins;
      int obins;
      Vector<Vector<int> > slices;
      Vector<Vector<int> > masks;
      Vector<float> borders;
      Vector<float> means;
      Vector<float> meansPAVA;
      Vector<float> weightsPAVA;
      Vector<float> nums;
      PoolAdjacentViolatorsAlgorithm<T,T> pava;
      Vector<int> sliceMask;
      int okslices;
      int discretizer;
      float same;
      float weight;
      int wtp;
      int nonmonotonic;
      
      //static int determinedBins= 0;
  };
  
  template<typename T>
  MatchingByMonotonousToneMappingPWCDSF<T>::MatchingByMonotonousToneMappingPWCDSF(int b, int d, float w, int wt, int nm)
  : DissimilarityFunction<T>(), bins(b), discretizer(d), weight(w), wtp(wt), nonmonotonic(nm)
  {
    stringstream ss;
    ss << string("MatchingByMonotonousToneMappingPWCDSF ") << bins << " " << discretizer << " " << weight << " " << wtp << " " << nonmonotonic;
    obins= bins;
    descriptor= ss.str();
  }

  template<typename T>
  double MatchingByMonotonousToneMappingPWCDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initPositions(b);
    
    double score= computeScore(a, 0);
    double result= score/(a.size()*a.getVariance());
    return result;
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWCDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    double result= 0;
    if ( !initialized || lastTemplate != &b )
    {
        lastTemplate= &b;
      init(b);
    }
    
    double mean= 0;
    double mean2= 0;
    
    for ( unsigned int i= 0; i < b.size(); ++i )
    {
      mean+= a(n + b(i).first);
      mean2+= a(n + b(i).first)*a(n + b(i).first);
    }
    mean/= b.size();
    mean2/= b.size();
    
    double score= computeScore(a, n);
    result= score;

    return result;
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWCDSF<T>::init(Template<T>& b)
  {
    Vector<T> weights;
    b.getWeights(weights);
    
    printf("w: %d\n", weights.size());
    initPositions(weights);

/*    Image<float> tmpimage(sqrt(b.size()), sqrt(b.size()));
    tmpimage= 0;
    for ( unsigned int i= 0; i < slices.size(); ++i )
        for ( unsigned int j= 0; j < slices(i).size(); ++j )
            tmpimage(slices(i)(j))= i;
    tmpimage.normalize(0, 255);
    writeImage("tmpimage.bmp", tmpimage);*/
    
    int flag;
    for ( unsigned int k= 0; k < weights.size(); ++k )
    {
      for ( unsigned int i= 0; i < slices.size(); ++i )
      {
	flag= 0;
	for ( unsigned int j= 0; j < slices(i).size(); ++j )
	  if ( masks(i)(j) && slices(i)(j) == int(k) )
	  {
	    slices(i)(j)= b(k).first;
            masks(i)(j)= 0;
	    flag= 1;
	    break;
	  }
	if ( flag )
	  continue;
      }
    }
    
    initialized= 1;
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWCDSF<T>::initPositions(Vector<T>& weights)
  {
    DataSet<T> dataset;
    ContinuousFeatureVector entry;
    entry.classIndex= 0;
    entry.resize(1);
    for ( unsigned int i= 0; i < weights.size(); ++i )
    {
      entry(0)= weights(i);
      dataset.push_back(entry);
    }
    
//#pragma omp critical
{
    int error= 0;
    do
    {
        error= 0;
        Discretizer<T>* discretizer;
        if ( this->discretizer == 0 )
            discretizer= new EqualWidthDiscretizer<T>(bins);
        else if ( this->discretizer == 1 )
            discretizer= new KMeansClusteringDiscretizer<T>(bins, 28, 100);
        else
            discretizer= new EqualWidthDiscretizer<T>(bins);
        
        discretizer->init(dataset);
        
        borders.clear();
        for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
            borders.push_back(discretizer->data(i)(0));
        borders.insert(borders.begin(), -FLT_MAX);
        borders.push_back(FLT_MAX);
        slices.clear();
        masks.clear();
        slices.resize(bins);
        masks.resize(bins);
        
        for ( unsigned int i= 0; i < weights.size(); ++i )
        for ( unsigned int j= 0; j < borders.size(); ++j )
        {
            if ( weights(i) < borders(j) )
            {
                slices(j-1).push_back(i);
                masks(j-1).push_back(1);
                break;
            }
        }
        
        for ( unsigned int i= 0; i < slices.size(); ++i )
            if ( slices(i).size() == 0 )
            if ( /*slices(i).size() < weights.size()/(3.0*bins) &&*/ bins > 2 )
            {
                error= 1;
                bins--;
                break;
            }
            
        Image<unsigned char> labelimage;
        labelimage.resizeImage(lastTemplate->image);
        tprintf("template image: %d %d\n", lastTemplate->image.rows, lastTemplate->image.columns);
        for ( int i= 0; i < (*lastTemplate).image.n; ++i )
        {
            for ( int j= 0; j < borders.size(); ++j )
                if ( lastTemplate->image(i) < borders(j) )
                {
                    labelimage(i)= j-1;
                    break;
                }
        }
        /*for ( int i= 0; i < labelimage.n; ++i )
            printf("%d ", labelimage(i));
        printf("\n");*/
        same= 0;
        for ( int i= 1; i < labelimage.rows-1; ++i )
            for ( int j= 1; j < labelimage.columns-1; ++j )
                {
                    if ( labelimage(i,j) == labelimage(i,j+1) )
                        same+= 1;
                    if ( labelimage(i,j) == labelimage(i,j-1) )
                        same+= 1;
                    if ( labelimage(i,j) == labelimage(i-1,j) )
                        same+= 1;
                    if ( labelimage(i,j) == labelimage(i+1,j) )
                        same+= 1;
                }
        same/= double(labelimage.rows-2)*(labelimage.columns-2);
        tprintf("neighbors: %f\n", same);
        
        /*if ( same < 2.5 && bins > 2 )
        {
            error= 1;
            --bins;
        }*/
        
        delete discretizer;
    }while ( error );
}

    /*for ( int i= 0; i < slices.size(); ++i )
        tprintf("%d\n", slices(i).size());*/
    Image<unsigned char> sliceImage;
    sliceImage.resizeImage(lastTemplate->image);
    sliceImage= 0;
    
    for ( int i= 0; i < slices.size(); ++i )
        for ( int j= 0; j < slices(i).size(); ++j )
            sliceImage(slices(i)(j))= i+1;
    Image<unsigned char> tosaveslice;
    tosaveslice.resizeImage(sliceImage);
    tosaveslice= sliceImage;
    tosaveslice.normalize(0, 255);
    writeImage("sliceImage.bmp", tosaveslice);
    
    Image<unsigned char> thinningIn;
    Image<unsigned char> thinningOut;
    thinningIn.resizeImage(sliceImage);
    thinningOut.resizeImage(sliceImage);
    for ( int i= 0; i < slices.size(); ++i )
    {
        thinningIn= 0;
        thinningOut= 0;
        
        for ( int j= 0; j < sliceImage.n; ++j )
            if ( sliceImage(j) == i+1 )
                thinningIn(j)= 255;
        /*thinningCWSI(thinningIn, thinningOut);
        thinningIn= 0;
        thinningCWSI1(thinningOut, thinningIn);
        thinningOut= 0;
        thinningCWSI1(thinningIn, thinningOut);*/
        /*writeImage("a.bmp", thinningIn);
        writeImage("b.bmp", thinningOut);
        for ( int j= 0; j < thinningOut.n; ++j )
            if ( thinningIn(j) != thinningOut(j) )
            {
                //printf("."); fflush(stdout);
                Vector<int>::iterator position = std::find(slices(i).begin(), slices(i).end(), j);
                if (position != slices(i).end()) // == myVector.end() means the element was not found
                    slices(i).erase(position);
            }*/
    }
    //printf("\n");
    
    sliceImage= 0;
    for ( int i= 0; i < slices.size(); ++i )
        for ( int j= 0; j < slices(i).size(); ++j )
            sliceImage(slices(i)(j))= i+1;
    sliceImage.normalize(0, 255);
    writeImage("sliceImage2.bmp", sliceImage);
    
    /*for ( int i= 0; i < slices.size(); ++i )
        tprintf("%d\n", slices(i).size());*/

    tprintf("discretization finished: %d -- id %d\n", bins, omp_get_thread_num());
    
    sliceMask.resize(slices.size());
    sliceMask= 1;
    okslices= slices.size();

    for ( unsigned int i= 0; i < slices.size(); ++i )
        if ( 0 )
        {
            sliceMask(i)= 0;
            --okslices;
        }
        
    means.resize(bins);
    nums.resize(bins);
    meansPAVA.resize(bins);
    weightsPAVA.resize(bins);
    
    for ( unsigned int i= 0; i < slices.size(); ++i )
      weightsPAVA(i)= slices(i).size();
    
  }
  
  template<typename T>
  double meanNumberOfNeighbors(Template2<T>& temp, Vector<float>& borders)
  {
    Image<unsigned char> labelimage;
    labelimage.resizeImage(temp.image);
    //tprintf("template image: %d %d\n", lastTemplate->image.rows, lastTemplate->image.columns);
    for ( int i= 0; i < temp.image.n; ++i )
    {
        for ( int j= 0; j < borders.size(); ++j )
            if ( temp.image(i) < borders(j) )
            {
                labelimage(i)= j-1;
                break;
            }
    }

    double same= 0;
    for ( int i= 1; i < labelimage.rows-1; ++i )
        for ( int j= 1; j < labelimage.columns-1; ++j )
            {
                if ( labelimage(i,j) == labelimage(i,j+1) )
                    same+= 1;
                if ( labelimage(i,j) == labelimage(i,j-1) )
                    same+= 1;
                if ( labelimage(i,j) == labelimage(i-1,j) )
                    same+= 1;
                if ( labelimage(i,j) == labelimage(i+1,j) )
                    same+= 1;
            }
    same/= double(labelimage.rows-2)*(labelimage.columns-2);
    tprintf("neighbors: %f\n", same);
    return same;
  }
  
  template<typename T>
  double meanNumberOfNeighbors(Vector<T>& a, int n, Template<T>& temp, int bins)
  {
    float tmin= FLT_MAX, tmax= -FLT_MAX;
    for ( unsigned int i= 0; i < temp.size(); ++i )
        if ( a(n + (temp)(i).first) < tmin )
            tmin= a(n + (temp)(i).first);
        else if ( a(n + (temp)(i).first) > tmax )
            tmax= a(n + (temp)(i).first);
    
    Vector<float> tborders;
    tborders.push_back(-FLT_MAX);
    for ( unsigned int i= 1; i < bins-1; ++i )
    {
        tborders.push_back(tmin + (tmax - tmin)/double(bins)*i);
    }
    tborders.push_back(FLT_MAX);
    
    Image<unsigned char> labelimage;
    labelimage.resizeImage(temp.image);
    
    //tprintf("template image: %d %d\n", lastTemplate->image.rows, lastTemplate->image.columns);
    for ( int i= 0; i < temp.size(); ++i )
    {
        for ( int j= 0; j < tborders.size(); ++j )
            if ( a(n + temp(i).first) < tborders(j) )
            {
                labelimage(i)= j-1;
                break;
            }
    }
    
    double same= 0;
    for ( int i= 1; i < labelimage.rows-1; ++i )
        for ( int j= 1; j < labelimage.columns-1; ++j )
            {
                if ( labelimage(i,j) == labelimage(i,j+1) )
                    same+= 1;
                if ( labelimage(i,j) == labelimage(i,j-1) )
                    same+= 1;
                if ( labelimage(i,j) == labelimage(i-1,j) )
                    same+= 1;
                if ( labelimage(i,j) == labelimage(i+1,j) )
                    same+= 1;
            }
    same/= double(labelimage.rows-2)*(labelimage.columns-2);
    //tprintf("neighbors: %f\n", same);
    return same;
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWCDSF<T>::computeScore(Vector<T>& a, int n)
  {
    means= 0;
    nums= 0;
    
    double result= 0;
    
    //double samewtp= meanNumberOfNeighbors(a, n, *lastTemplate, bins);
    double samewtp= 0.1;
    
    /*if ( n % 1000 == 0 )
        printf("%f,%f ", same, samewtp);*/
    
    if ( wtp == 0 || wtp == 2 || wtp == 3 || (wtp == 4 && same > samewtp) )
    {
        sliceMask= 1;
        
        for ( unsigned int i= 0; i < slices.size(); ++i )
        {
        if ( sliceMask(i) )
        for ( unsigned int j= 0; j < slices(i).size(); ++j )
        {
            means(i)+= a(n + slices(i)(j));
            nums(i)+= 1.0;
        }
        }
        
        double wmean= 0;
        double wmean2= 0;
        for ( unsigned int i= 0; i < means.size(); ++i )
        {
        if ( nums(i) > 0 )
            means(i)/= nums(i);
        else
            means(i)= 0;
        wmean+= means(i)*nums(i);
        }
        wmean/= nums.getSum();
        
        for ( unsigned int i= 0; i < slices.size(); ++i )
            if ( sliceMask(i) )
                for ( unsigned int j= 0; j < slices(i).size(); ++j )
                    wmean2+= (wmean - a(n + slices(i)(j)))*(wmean - a(n + slices(i)(j)));

        if ( wmean2 < 0.01 )
            return 1;
        for ( unsigned int i= 0; i < means.size(); ++i )
            if ( !sliceMask(i) )
            {
                for ( unsigned int j= 0; j < means.size(); ++j )
                {
                    if ( i + j < means.size() )
                    {
                        if ( means(i + j) != 0 )
                        {
                            means(i)= means(i + j);
                            break;
                        }
                    }
                    if ( i - j >= 0 )
                    {
                        if ( means(i - j) != 0 )
                        {
                            means(i)= means(i - j);
                            break;
                        }
                    }
                }
            }
        
        Vector<float> newMeans(okslices);
        Vector<float> newWeights(okslices);
        Vector<float> newMeansPAVA(okslices);
        
        int nn= 0;
        for ( unsigned int i= 0; i < sliceMask.size(); ++i )
        {
            if ( sliceMask(i) )
            {
                newMeans(nn)= means(i);
                newWeights(nn)= weightsPAVA(i);
                ++nn;
            }
        }
        
        pava.apply(newMeans, newWeights, newMeansPAVA);
        
        meansPAVA= means;
        nn= 0;
        for ( unsigned int i= 0; i < sliceMask.size(); ++i )
        {
            if ( sliceMask(i) )
            {
                meansPAVA(i)= newMeansPAVA(nn);
                ++nn;
            }
        }
        
        /*if ( flag ) 
        {
            cout << meansPAVA << endl;
            cout << newMeans << endl;
            cout << newMeansPAVA << endl;
            
            Image<float> tmpimage(sqrt(lastTemplate->size()), sqrt(lastTemplate->size()));
            tmpimage= 0;
            for ( unsigned int i= 0; i < lastTemplate->size(); ++i )
                tmpimage(i)= a((*lastTemplate)(i).first + n);
            tmpimage.normalize(0, 255);
            writeImage("tmpimage2.bmp", tmpimage);
        }*/
        
        result= 0;
        for ( unsigned int i= 0; i < slices.size(); ++i )
        {
        if ( sliceMask(i) )
        {
            for ( unsigned int j= 0; j < slices(i).size(); ++j )
            {
                float tmp= meansPAVA(i)*weight + means(i)*(1 - weight);
                result+= (tmp - a(n + slices(i)(j)))*(tmp - a(n + slices(i)(j)));
            }
        }
        }
    
/*    if ( flag )
    {
        cout << result << endl;
        cout << wmean2 << endl;
        cout << sliceMask << endl;
        cout << result/wmean2 << endl;
    }*/
    
        result/= wmean2;
    }
    
    float tresult= 0;
    
    if ( wtp == 1 || wtp == 2 || wtp == 3 || (wtp == 4 && same < samewtp) )
    {
        double sametmp;
        int tbins= bins;
        /*tbins++;
        do
        {
            --tbins;
            sametmp= meanNumberOfNeighbors(a, n, *lastTemplate, tbins);
        } while ( sametmp < 2.5 && tbins > 2 );*/
        
        
        float tmin= FLT_MAX, tmax= -FLT_MAX;
        for ( unsigned int i= 0; i < lastTemplate->size(); ++i )
            if ( a(n + (*lastTemplate)(i).first) < tmin )
                tmin= a(n + (*lastTemplate)(i).first);
            else if ( a(n + (*lastTemplate)(i).first) > tmax )
                tmax= a(n + (*lastTemplate)(i).first);
        
        Vector<float> tborders;
        tborders.push_back(-FLT_MAX);
        for ( unsigned int i= 1; i < tbins-1; ++i )
        {
            tborders.push_back(tmin + (tmax - tmin)/double(tbins)*i);
        }
        tborders.push_back(FLT_MAX);
        
        Vector<Vector<int> > tslices;
        tslices.resize(tborders.size()-1);
        for ( unsigned int i= 0; i < (*lastTemplate).size(); ++i )
            for ( unsigned int j= 0; j < tborders.size(); ++j )
            {
                if ( a(n + (*lastTemplate)(i).first) < tborders(j) )
                {
                    tslices(j-1).push_back(i);
                    break;
                }
            }
        
        Vector<float> tmean;
        Vector<int> tns;
        tmean.resize(tborders.size()-1);
        tns.resize(tborders.size()-1);
        tmean= 0;
        tns= 0;
        for ( unsigned int i= 0; i < tslices.size(); ++i )
            for ( unsigned int j= 0; j < tslices(i).size(); ++j )
            {
                tmean(i)+= (*lastTemplate)(tslices(i)(j)).second;
                tns(i)+= 1;
            }
        for ( unsigned int i= 0; i < tmean.size(); ++i )
            tmean(i)/= tns(i);
        
        Vector<float> tmeanpava;
        Vector<float> tweights;
        tmeanpava.resize(tmean.size());
        tweights.resize(tmean.size());
        tweights= tns;
        
        pava.apply(tmean, tweights, tmeanpava);
        //tmean= tmeanpava;
            
        float tdist= 0;
        for ( unsigned int i= 0; i < tslices.size(); ++i )
            for ( unsigned int j= 0; j < tslices(i).size(); ++j )
            {
                float tmp= tmeanpava(i)*weight + tmean(i)*(1 - weight);
                tdist+= (tmp - (*lastTemplate)(tslices(i)(j)).second)*(tmp - (*lastTemplate)(tslices(i)(j)).second);
            }
        
        float tvar= 0;
        float tmean0= 0;
        for ( unsigned int i= 0; i < (*lastTemplate).size(); ++i )
            tmean0+= (*lastTemplate)(i).second;
        tmean0/= (*lastTemplate).size();
        for ( unsigned int i= 0; i < (*lastTemplate).size(); ++i )
            tvar+= (tmean0 - (*lastTemplate)(i).second)*(tmean0 - (*lastTemplate)(i).second);

        tresult= tdist/tvar;
    }
    
    if ( wtp == 1)
        return tresult;
    else if ( wtp == 0 )
        return result;
    else if ( wtp == 2 )
        return tresult + result;
    else if ( wtp == 3 )
        return tresult < result ? tresult : result;
    else if ( wtp == 4 && same > samewtp ) 
        return result;
    else if ( wtp == 4 && same < samewtp )
        return tresult;
    //if ( same > 2 )
        //return result;
    //else
        //return tresult;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MatchingByMonotonousToneMappingPWCDSF<T>::clone()
  {
    return new MatchingByMonotonousToneMappingPWCDSF<T>(bins, discretizer, weight, wtp, nonmonotonic);
  }
  
  
  
  
  template<typename T>
  class MatchingByToneMappingPWCDSFWtP: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MatchingByToneMappingPWCDSFWtP(int bins, int discretizer= 0);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a, int n);
      
      virtual void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
      
      int bins, obins;
      Vector<Vector<int> > slices;
      Vector<Vector<int> > masks;
      Vector<float> borders;
      Vector<float> means;
      Vector<float> meansPAVA;
      Vector<float> weightsPAVA;
      Vector<float> nums;
      PoolAdjacentViolatorsAlgorithm<T,T> pava;
      int discretizer;
  };
  
  template<typename T>
  MatchingByToneMappingPWCDSFWtP<T>::MatchingByToneMappingPWCDSFWtP(int b, int d)
  : DissimilarityFunction<T>(), bins(b), obins(b), discretizer(d)
  {
    stringstream ss;
    ss << string("MatchingByToneMappingPWCDSFWtP ") << bins << " " << discretizer;
    descriptor= ss.str();
  }

  template<typename T>
  double MatchingByToneMappingPWCDSFWtP<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initPositions(b);
    
    double score= computeScore(a, 0);
    double result= score;
    
    return result;
  }
  
  template<typename T>
  void MatchingByToneMappingPWCDSFWtP<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
  {
    cout << descriptor << endl;
      
    temp.updateStride(input.columns);
    bins= obins;
    Image<T> slicedImage;
    slicedImage.resizeImage(input);
    
    DataSet<T> dataset;
    ContinuousFeatureVector entry;
    entry.classIndex= 0;
    entry.resize(1);
    for ( unsigned int i= 0; i < input.n; ++i )
    {
        entry(0)= input(i);
        dataset.push_back(entry);
    }
    
    int error= 0;
    Vector<int> ns;
    try{
        do
        {
            ns.resize(bins);
            ns= 0;
            error= 0;
            Discretizer<T>* discretizer;
            if ( this->discretizer == 0 )
                discretizer= new EqualWidthDiscretizer<T>(bins);
            else if ( this->discretizer == 1 )
                discretizer= new KMeansClusteringDiscretizer<T>(bins, 28, 25);
            else
                discretizer= new EqualWidthDiscretizer<T>(bins);
            
            discretizer->init(dataset);
            
            borders.clear();
            for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
                borders.push_back(discretizer->data(i)(0));
            borders.insert(borders.begin(), -FLT_MAX);
            borders.push_back(FLT_MAX);
            
            for ( unsigned int i= 0; i < input.n; ++i )
                for ( unsigned int j= 0; j < borders.size(); ++j )
                {
                    if ( input(i) < borders(j) )
                    {
                        slicedImage(i)= j-1;
                        ns(j-1)++;
                        break;
                    }
                }
            
            for ( unsigned int i= 0; i < ns.size(); ++i )
                if ( ns(i) == 0 )
                {
                    error= 1;
                    bins--;
                    break;
                }
            delete discretizer;
        }while ( error );
    }
    catch(std::length_error e)
    {
        bins= 0;
        output= 1;
        return;
    }
    output= 1;
    
    /*for ( unsigned int i= 0; i < ns.size(); ++i )
        printf("ns %d = %d\n", i, ns(i));*/

    Vector<Image<T> > conv;
    Vector<Image<int> > convns;
    conv.resize(bins);
    convns.resize(bins);
    for ( unsigned int i= 0; i < conv.size(); ++i )
    {
        conv(i).resizeImage(input);
        convns(i).resizeImage(input);
    }
    for ( unsigned int i= 0; i < conv.size(); ++i )
    {
        conv(i)= 0;
        convns(i)= 0;
    }
    
    Vector<float> sliceMeans;
    Vector<int> sliceNs;
    sliceMeans.resize(bins);
    sliceNs.resize(bins);
    
    int min= -temp.getMin();
    int max= int(input.n) - temp.getMax();
    
    #pragma omp parallel for
    for ( int i= min; i < max; ++i )
    {
        Vector<int> sliceNs;
        sliceNs.resize(bins);
        sliceNs= 0;
        for ( unsigned int j= 0; j < temp.size(); ++j )
        {
            int n= temp(j).first;
            if ( i + n >= 0 && i + n < int(input.n) )
            {
                conv(slicedImage(i + n))(i)+= temp(j).second;
                sliceNs(slicedImage(i + n))++;
            }
        }
        for ( unsigned int j= 0; j < sliceNs.size(); ++j )
            if ( sliceNs(j) > 0 )
            {
                conv(j)(i)/= sliceNs(j);
                convns(j)(i)= sliceNs(j);
            }
    }
    
    float mean= 0;
    float mean2= 0;
    for ( unsigned int i= 0; i < temp.size(); ++i )
    {
        mean+= temp(i).second;
        mean2+= (temp(i).second)*(temp(i).second);
    }
    mean2/= temp.size();
    mean/= temp.size();
    float var= (mean2 - mean*mean)*temp.size();
    
    //tprintf("var: %f\n", var);
    
    int start= -temp.getMin();
    int end= int(input.n) - temp.getMax();
    
    #pragma omp parallel for
    for ( int i= start; i < end; ++i )
    {
        float res= 0;
        for ( unsigned int j= 0; j < temp.size(); ++j )
        {
            int n= temp(j).first;
            res+= (conv(slicedImage(i + n))(i) - temp(j).second)*(conv(slicedImage(i + n))(i) - temp(j).second);
        }
        output(i)= res/var;
    }
  }
  
  template<typename T>
  double MatchingByToneMappingPWCDSFWtP<T>::evaluate(Vector<T>& /*a*/, Template<T>& /*b*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  void MatchingByToneMappingPWCDSFWtP<T>::init(Template<T>& /*b*/)
  {
  }
  
  template<typename T>
  void MatchingByToneMappingPWCDSFWtP<T>::initPositions(Vector<T>& /*weights*/)
  {
  }
  
  template<typename T>
  double MatchingByToneMappingPWCDSFWtP<T>::computeScore(Vector<T>& /*a*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MatchingByToneMappingPWCDSFWtP<T>::clone()
  {
    return new MatchingByToneMappingPWCDSFWtP<T>(bins, discretizer);
  }
  
  
  
  
  
  
  template<typename T>
  class MatchingByMonotonousToneMappingPWCDSFWtP: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MatchingByMonotonousToneMappingPWCDSFWtP(int bins, int discretizer= 0, float weight= 1);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a, int n);
      
      virtual void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
      
      int bins, obins;
      Vector<Vector<int> > slices;
      Vector<Vector<int> > masks;
      Vector<float> borders;
      Vector<float> means;
      Vector<float> meansPAVA;
      Vector<float> weightsPAVA;
      Vector<float> nums;
      PoolAdjacentViolatorsAlgorithm<T,T> pava;
      int discretizer;
      float weight;
  };
  
  template<typename T>
  MatchingByMonotonousToneMappingPWCDSFWtP<T>::MatchingByMonotonousToneMappingPWCDSFWtP(int b, int d, float w)
  : DissimilarityFunction<T>(), bins(b), obins(b), discretizer(d), weight(w)
  {
    stringstream ss;
    ss << string("MatchingByMonotonousToneMappingPWCDSFWtP ") << bins << " " << discretizer;
    descriptor= ss.str();
  }

  template<typename T>
  double MatchingByMonotonousToneMappingPWCDSFWtP<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initPositions(b);
    
    double score= computeScore(a, 0);
    double result= score;
    
    return result;
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWCDSFWtP<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
  {
    cout << descriptor << endl;
      
    temp.updateStride(input.columns);
    bins= obins;
    Image<T> slicedImage;
    slicedImage.resizeImage(input);
    
    DataSet<T> dataset;
    ContinuousFeatureVector entry;
    entry.classIndex= 0;
    entry.resize(1);
    for ( unsigned int i= 0; i < input.n; ++i )
    {
        entry(0)= input(i);
        dataset.push_back(entry);
    }
    
    int error= 0;
    Vector<int> ns;
    try{
        do
        {
            ns.resize(bins);
            ns= 0;
            error= 0;
            Discretizer<T>* discretizer;
            if ( this->discretizer == 0 )
                discretizer= new EqualWidthDiscretizer<T>(bins);
            else if ( this->discretizer == 1 )
                discretizer= new KMeansClusteringDiscretizer<T>(bins, 28, 25);
            else
                discretizer= new EqualWidthDiscretizer<T>(bins);
            
            discretizer->init(dataset);
            
            borders.clear();
            for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
                borders.push_back(discretizer->data(i)(0));
            borders.insert(borders.begin(), -FLT_MAX);
            borders.push_back(FLT_MAX);
            
            for ( unsigned int i= 0; i < input.n; ++i )
                for ( unsigned int j= 0; j < borders.size(); ++j )
                {
                    if ( input(i) < borders(j) )
                    {
                        slicedImage(i)= j-1;
                        ns(j-1)++;
                        break;
                    }
                }
            
            double neighbors= 0;
            int nNeighbors= 0;
            for ( int i= 1; i < slicedImage.rows-1; ++i )
                for ( int j= 1; j < slicedImage.columns-1; ++j )
                {
                    if ( slicedImage(i,j) == slicedImage(i,j-1) )
                        neighbors+= 1;
                    if ( slicedImage(i,j) == slicedImage(i,j+1) )
                        neighbors+= 1;
                    if ( slicedImage(i,j) == slicedImage(i-1,j) )
                        neighbors+= 1;
                    if ( slicedImage(i,j) == slicedImage(i+1,j) )
                        neighbors+= 1;
                    ++nNeighbors;
                }
            neighbors/= nNeighbors;
            tprintf("neighbors: %f %d\n", neighbors, bins);
            if ( neighbors < 2.5 && bins > 2 )
            {
                error= 1;
                bins--;
            }
            else
            {
                for ( unsigned int i= 0; i < ns.size(); ++i )
                    if ( ns(i) == 0 && bins > sqrt(obins) )
                    {
                        error= 1;
                        bins--;
                        break;
                    }
                    else if ( ns(i) == 0 && bins <= sqrt(obins) )
                    {
                        error= 0;
                        break;
                    }
                    
            }
            delete discretizer;
            //break;
            tprintf("trying new bins: %d\n", bins);
        }while ( error );
    }
    catch(std::length_error e)
    {
        printf("=============================================================================================== %d\n", bins);
        bins= 0;
        output= 1;
        return;
    }
    
    Image<unsigned char> thinningIn;
    Image<unsigned char> thinningOut;
    Image<T> slicedImage2;
    slicedImage2.resizeImage(slicedImage);
    slicedImage2= slicedImage;
    
    thinningIn.resizeImage(slicedImage);
    thinningOut.resizeImage(slicedImage);
    
    /*for ( int i= 0; i < borders.size(); ++i )
    {
        thinningIn= 0;
        thinningOut= 0;
        for ( int j= 0; j < slicedImage.n; ++j )
            if ( slicedImage(j) == i )
                thinningIn(j)= 255;
        thinningCWSI(thinningIn, thinningOut);
        for ( int j= 0; j < thinningOut.n; ++j )
            if ( thinningIn(j) != thinningOut(j) )
                slicedImage2(j)= -1;
    }*/
    
    output= 1;
    
    /*for ( unsigned int i= 0; i < ns.size(); ++i )
        printf("ns %d = %d\n", i, ns(i));*/

    Vector<Image<T> > conv;
    Vector<Image<int> > convns;
    conv.resize(bins);
    convns.resize(bins);
    for ( unsigned int i= 0; i < conv.size(); ++i )
    {
        conv(i).resizeImage(input);
        convns(i).resizeImage(input);
    }
    for ( unsigned int i= 0; i < conv.size(); ++i )
    {
        conv(i)= 0;
        convns(i)= 0;
    }
    
    Vector<float> sliceMeans;
    Vector<int> sliceNs;
    sliceMeans.resize(bins);
    sliceNs.resize(bins);
    
    int start= -temp.getMin();
    int end= int(input.n) - temp.getMax();
    
    #pragma omp parallel for
    for ( int i= start; i < end; ++i )
    {
        Vector<int> sliceNs;
        sliceNs.resize(bins);
        sliceNs= 0;
        for ( unsigned int j= 0; j < temp.size(); ++j )
        {
            int n= temp(j).first;
            if ( i + n >= 0 && i + n < int(input.n) )
            {
                if ( slicedImage2(i + n) >= 0 )
                {
                    conv(slicedImage(i + n))(i)+= temp(j).second;
                    sliceNs(slicedImage(i + n))++;
                }
            }
        }
        for ( unsigned int j= 0; j < sliceNs.size(); ++j )
            if ( sliceNs(j) > 0 )
            {
                conv(j)(i)/= sliceNs(j);
                convns(j)(i)= sliceNs(j);
            }
    }
    
    Vector<float> means;
    Vector<float> meansPAVA;
    Vector<float> weights;

    start= -temp.getMin();
    end= int(input.n) - temp.getMax();
    
    for ( int i= start; i < end; ++i )
    {
        means.clear();
        meansPAVA.clear();
        weights.clear();
        for ( int j= 0; j < bins; ++j )
        {
            if ( convns(j)(i) > 0 )
            {
                means.push_back(conv(j)(i));
                weights.push_back(convns(j)(i));
            }
        }
        meansPAVA.resize(means.size());
        pava.apply(means, weights, meansPAVA);
        int k= 0;
        for ( int j= 0; j < bins; ++j )
        {
            if ( convns(j)(i) > 0 )
            {
                conv(j)(i)= weight*meansPAVA(k) + (1-weight)*means(k);
                ++k;
            }
        }
    }
    
    float mean= 0;
    float mean2= 0;
    for ( unsigned int i= 0; i < temp.size(); ++i )
    {
        mean+= temp(i).second;
        mean2+= (temp(i).second)*(temp(i).second);
    }
    mean2/= temp.size();
    mean/= temp.size();
    float var= (mean2 - mean*mean)*temp.size();
    
    //tprintf("var: %f\n", var);
    
    start= -temp.getMin();
    end= int(input.n) - temp.getMax();
    
    #pragma omp parallel for
    for ( int i= start; i < end; ++i )
    {
        float res= 0;
        for ( unsigned int j= 0; j < temp.size(); ++j )
        {
            int n= temp(j).first;
            res+= (conv(slicedImage(i + n))(i) - temp(j).second)*(conv(slicedImage(i + n))(i) - temp(j).second);
        }
        output(i)= res/var;
    }
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWCDSFWtP<T>::evaluate(Vector<T>& /*a*/, Template<T>& /*b*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWCDSFWtP<T>::init(Template<T>& /*b*/)
  {
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWCDSFWtP<T>::initPositions(Vector<T>& /*weights*/)
  {
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWCDSFWtP<T>::computeScore(Vector<T>& /*a*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MatchingByMonotonousToneMappingPWCDSFWtP<T>::clone()
  {
    return new MatchingByMonotonousToneMappingPWCDSFWtP<T>(bins, discretizer, weight);
  }
  
  
  
  
  
  
  template<typename T>
  class MatchingByToneMappingPWLDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MatchingByToneMappingPWLDSF(int bins, int discretizer= 0);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a);
      
      virtual double r(double x, int i);
      
      void printMatrix(char* label, Matrix<float>& A);
      
      Vector<Vector<int> > slices;
      Vector<Vector<int> > masks;
      Vector<float> borders;
      Vector<float> bordersT;
      Vector<float> means;
      Vector<float> nums;
      Matrix<float> Q, QT, QTQ, QTQinv, QTQinvQT, QQTQinvQT;
      Vector<float> tmp, tmp2;
      
      int bins;
      int length;
      int n;
      Vector<float>* atmp;
      int error;
      int discretizer;
      int sing;
  };
  
  template<typename T>
  MatchingByToneMappingPWLDSF<T>::MatchingByToneMappingPWLDSF(int b, int d)
  : DissimilarityFunction<T>(), bins(b), discretizer(d)
  {
    stringstream ss;
    ss << string("MatchingByToneMappingPWLDSF ") << bins << " " << discretizer;
    descriptor= ss.str();
    error= 0;
  }

  template<typename T>
  double MatchingByToneMappingPWLDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initPositions(b);
    
    double score= computeScore(a);
    double result= score/(a.size()*a.getVariance());
    return result;
  }
  
  template<typename T>
  double MatchingByToneMappingPWLDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
        lastTemplate= &b;
      init(b);
    }
    
    for ( unsigned int i= 0; i < b.size(); ++i )
      tmp(i)= a(n + b(i).first);
    
    double mean= 0;
    double mean2= 0;
    
    for ( unsigned int i= 0; i < b.size(); ++i )
    {
      mean+= a(n + b(i).first);
      mean2+= a(n + b(i).first)*a(n + b(i).first);
    }
    mean/= b.size();
    mean2/= b.size();
    double var= mean2 - mean*mean;
    
    this->n= n;
    this->atmp= &a;
    double score= computeScore(tmp);
    if ( var < 0.0001 )
        return 1;
    double result= score/(b.size()*var);

    return result;
  }
  
  template<typename T>
  void MatchingByToneMappingPWLDSF<T>::init(Template<T>& b)
  {
    Vector<T> weights;
    b.getWeights(weights);

    length= b.size();
    
    initPositions(weights);
    
    int flag;
    for ( unsigned int k= 0; k < weights.size(); ++k )
    {
      for ( unsigned int i= 0; i < slices.size(); ++i )
      {
	flag= 0;
	for ( unsigned int j= 0; j < slices(i).size(); ++j )
	  if ( masks(i)(j) && slices(i)(j) == int(k) )
	  {
	    slices(i)(j)= b(k).first;
            masks(i)(j)= 0;
	    flag= 1;
	    break;
	  }
	if ( flag )
	  continue;
      }
    }
    
    initialized= 1;
  }
  
  template<typename T>
  void MatchingByToneMappingPWLDSF<T>::initPositions(Vector<T>& weights)
  {
    DataSet<T> dataset;
    ContinuousFeatureVector entry;
    entry.resize(1);
    for ( unsigned int i= 0; i < weights.size(); ++i )
    {
      entry(0)= weights(i);
      dataset.push_back(entry);
    }
    
    do
    {
        bins--;
        do 
        {
            error= 0;
            Discretizer<T>* discretizer;
            if ( this->discretizer == 0 )
                discretizer= new EqualWidthDiscretizer<T>(bins, -28);
            else if ( this->discretizer == 1 )
                discretizer= new KMeansClusteringDiscretizer<T>(bins, -28, 25);
            else
                discretizer= new EqualWidthDiscretizer<T>(bins, -28);
            
            discretizer->init(dataset);
            
            if ( discretizer->error )
            {
                --bins;
                error= 1;
                delete discretizer;
                continue;
            }
            
            borders.clear();
            for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
            borders.push_back(discretizer->data(i)(0));
            
            if ( weights.getMin() < 0 )
                borders.insert(borders.begin(), 2*weights.getMin());
            else
                borders.insert(borders.begin(), -weights.getMin());
            
            borders.push_back(weights.getMax() + (weights.getMax() - borders(borders.size()-1)));
            slices.clear();
            masks.clear();
            slices.resize(bins);
            masks.resize(bins);
            
            for ( unsigned int i= 0; i < weights.size(); ++i )
            {
                unsigned int j= 0;
                for ( j= 0; j < borders.size(); ++j )
                {
                    if ( weights(i) < borders(j) )
                    {
                        slices(j-1).push_back(i);
                        masks(j-1).push_back(1);
                        break;
                    }
                }
            }
            
            for ( unsigned int i= 0; i < slices.size(); ++i )
                if ( slices(i).size() == 0 )
                {
                    error= 1;
                    --bins;
                    break;
                }
                
            Image<unsigned char> labelimage;
            labelimage.resizeImage(lastTemplate->image);
            tprintf("template image: %d %d\n", lastTemplate->image.rows, lastTemplate->image.columns);
            for ( int i= 0; i < (*lastTemplate).image.n; ++i )
            {
                for ( int j= 0; j < borders.size(); ++j )
                    if ( lastTemplate->image(i) < borders(j) )
                    {
                        labelimage(i)= j-1;
                        break;
                    }
            }
            /*for ( int i= 0; i < labelimage.n; ++i )
                printf("%d ", labelimage(i));
            printf("\n");*/
            float same= 0;
            for ( int i= 1; i < labelimage.rows-1; ++i )
                for ( int j= 1; j < labelimage.columns-1; ++j )
                    {
                        if ( labelimage(i,j) == labelimage(i,j+1) )
                            same+= 1;
                        if ( labelimage(i,j) == labelimage(i,j-1) )
                            same+= 1;
                        if ( labelimage(i,j) == labelimage(i-1,j) )
                            same+= 1;
                        if ( labelimage(i,j) == labelimage(i+1,j) )
                            same+= 1;
                    }
            same/= double(labelimage.rows-2)*(labelimage.columns-2);
            tprintf("neighbors: %f\n", same);
            if ( same < 2 && bins > 2 )
            {
                error= 1;
                bins--;
            }

            delete discretizer;
        }while ( error ==  1 );
        
        means.resize(bins);
        nums.resize(bins);
        
        bordersT.resize(bins+1);
        Q.resize(length, bins+1);
        QT.resize(bins+1, length);
        QTQ.resize(bins+1, bins+1);
        QTQinv.resize(bins+1, bins+1);
        QTQinvQT.resize(bins+1, length);
        QQTQinvQT.resize(length, length);
        
        for ( int i= 0; i < Q.rows; ++i )
        for ( int j= 0; j < Q.columns; ++j )
            Q(i,j)= 0;
        
        float ri;
        for ( int i= 0; i < length; ++i )
        for ( unsigned int j= 0; j < borders.size(); ++j )
            if ( weights(i) < borders(j) )
            {
            ri= r(weights(i), j);
            Q(i, j-1)= (1.0 - ri);
            Q(i, j)= ri;
            break;
            }

        QT= Q;
        QT.transpose();
        QTQ= QT^Q;
        
        //printf("QTQ.rows: %d\n", QTQ.rows);
        
    } while ( fabs(determinantSquareMatrix(&(QTQ(0)), QTQ.rows)) < 0.01 );
    
    {
        invertSquareMatrix(&(QTQ(0)), QTQ.rows, &(QTQinv(0)));
    //    sing= 0;
    }
    /*else
        sing= 1;*/
        
    QTQinvQT=QTQinv^QT;
    QQTQinvQT=Q^QTQinvQT;
    
    tmp.resize(length);
    tmp2.resize(length);
  }
  
  template<typename T>
  void MatchingByToneMappingPWLDSF<T>::printMatrix(char* label, Matrix<float>& A)
  {
    printf("%s\n", label);
    for ( int i= 0; i < A.rows; ++i )
    {
      for ( int j= 0; j < A.columns; ++j )
	printf("%f ", A(i,j));
      printf("\n");
    }
    for ( int i= 0; i < A.rows; ++i )
    {
      float flag= 0;
      for ( int j= 0; j < A.columns; ++j )
	flag+= A(i,j);
      if ( flag == 0 )
	printf("empty row: %d, %f\n", i, flag);
    }
    for ( int i= 0; i < A.columns; ++i )
    {
      float flag= 0;
      for ( int j= 0; j < A.rows; ++j )
	flag+= A(j,i);
      if ( flag == 0 )
	printf("empty column: %d %f\n", i, flag);
    }
    fflush(stdout);
  }
  
  template<typename T>
  double MatchingByToneMappingPWLDSF<T>::r(double x, int j)
  {
    return (x - borders(j-1))/(borders(j) - borders(j-1));
  }
  
  template<typename T>
  double MatchingByToneMappingPWLDSF<T>::computeScore(Vector<T>& a)
  {
    if ( sing == 1 )
        return 0;
    bordersT= QTQinvQT^a;
    tmp2= Q^bordersT;

    double result= 0;
    for ( unsigned int i= 0; i < a.size(); ++i )
      result+= (tmp2(i) - a(i))*(tmp2(i) - a(i));
    
    return result;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MatchingByToneMappingPWLDSF<T>::clone()
  {
    return new MatchingByToneMappingPWLDSF<T>(bins, discretizer);
  }  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class SortStruct
  {
  public:
      SortStruct(float a= 0, int b= 0)
      {
          this->a= a;
          this->b= b;
      }
      
      SortStruct(const SortStruct& s)
      {
          a= s.a;
          b= s.b;
      }
      
      SortStruct& operator=(const SortStruct& s)
      {
          a= s.a;
          b= s.b;
          return *this;
      }
      
      float a;
      int b;
  };
  
  class SortStructComparator
  {
  public:
      bool operator()(const SortStruct* x, const SortStruct* y)
      {
          return x->a < y->a;
      }
  };
  
  class SortStructComparatorRef
  {
  public:
      bool operator()(const SortStruct& x, const SortStruct& y)
      {
          return x.a < y.a;
      }
  };
   
  template<typename T>
  class MatchingByMonotonousToneMappingPWLDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MatchingByMonotonousToneMappingPWLDSF(int bins, int discretizer= 0, float weight= 1, int wtp= 0, int pen= 1);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a, int n= -1);
      
      virtual double r(double x, int i);
      
      virtual double r(double x, int j, Vector<double>& borders);
      
      virtual void doDiscretization(int method, Vector<T>& data, Vector<double>& borders, int& output_bins, Matrix<double> &Q, Matrix<double> &QT, Matrix<double> &QTQ, Matrix<double> &QTQinv, Vector<Vector<int> >* slices= NULL, double* det= NULL);
      
      void printMatrix(char* label, Matrix<float>& A);
      
      Vector<Vector<int> > slices;
      Vector<Vector<int> > masks;
      Vector<double> borders;
      Vector<double> bordersPAVA;
      Vector<double> bordersTmp;
      Vector<double> means;
      Vector<double> meansT;
      Vector<double> meansPAVA;
      Vector<double> weightsPAVA;
      Vector<double> nums;
      Matrix<double> Q, QT, QTQ, QTQinv, QTQinvQT, QQTQinvQT;
      Matrix<double> wQ, wQT, wQTQ, wQTQinv, wQTQinvQT, wQQTQinvQT;
      Vector<double> tmp;
      Vector<double> tmp2, tmp2PAVA, tmp2Worst, tmp3;
      Vector<double> templateWeights;
      Vector<Vector<int> > sparse;
      int pen;
      
      PoolAdjacentViolatorsAlgorithm<double, double> pava;
      
      float same;
      int bins;
      int length;
      int n;
      int error;
      Vector<float>* atmp;
      Vector<int> p;
      Vector<int> p_inv;
      int discretizer;
      float weight;
      int wtp;
      
      Vector<double> wborders;
        Vector<double> wpattern;
        Vector<double> waa;
        Vector<double> middle;
        Vector<double> wtmp2;
        Vector<double> wttt, wwww, wweights;    
        Vector<double> wres;
        Vector<T> wa;
  };
  
  template<typename T>
  MatchingByMonotonousToneMappingPWLDSF<T>::MatchingByMonotonousToneMappingPWLDSF(int b, int d, float w, int wt, int p)
  : DissimilarityFunction<T>(), bins(b), discretizer(d), weight(w), wtp(wt), pen(p)
  {
    stringstream ss;
    ss << string("MatchingByMonotonousToneMappingPWLDSF ") << bins << " " << discretizer << " " << weight << " " << wtp << " " << pen;
    descriptor= ss.str();

    error= 0;
  }

  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSF<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initPositions(b);
    
    double score= computeScore(a, 0);
    double result= score/(a.size()*a.getVariance());
    return result;
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSF<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
  {
    if ( !initialized || lastTemplate != &b )
    {
      lastTemplate= &b;
      init(b);
    }
    
    /*for ( unsigned int i= 0; i < b.size(); ++i )
      tmp(i)= a(n + b(i).first);*/
    
    double score= computeScore(a, n);
    //double score= 0;

    return score;
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWLDSF<T>::init(Template<T>& b)
  {
    Vector<T> weights;
    b.getWeights(weights);
    templateWeights= weights;

    length= b.size();
    
    initPositions(weights);
    
    initialized= 1;
  }
  
  
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWLDSF<T>::initPositions(Vector<T>& weights)
  {
    tprintf("do discretization\n");
    int bbins= bins;
    doDiscretization(this->discretizer, weights, borders, bins, Q, QT, QTQ, QTQinv, &slices);
    if ( bins == 1 || bins == 0 )
        bins= bbins;
    
    same= 0;
    
    tprintf("neighbors: %f\n", same);
    
    tprintf("discretization finished: %d -- id %d\n", bins, omp_get_thread_num());
        
    QTQinvQT.resize(bins+1, length);
    QQTQinvQT.resize(length, length);
        
    QTQinvQT=QTQinv^QT;
    QQTQinvQT=Q^QTQinvQT;
    
    means.resize(bins);
    meansPAVA.resize(bins);
    meansT.resize(bins);
    weightsPAVA.resize(bins);
    nums.resize(bins);
    bordersPAVA.resize(bins+1);
    bordersTmp.resize(bins+1);
    tmp.resize(length);
    tmp2.resize(length);
    tmp2PAVA.resize(length);
    tmp2Worst.resize(length);
    
    for ( unsigned int i= 0; i < slices.size(); ++i )
        weightsPAVA(i)= slices(i).size();
    
    p.resize(length);
    p_inv.resize(length);
    
    sparse.resize(QQTQinvQT.rows);
    int sp= 0;
    for ( int i= 0; i < QQTQinvQT.rows; ++i )
    {
        for ( int j= 0; j < QQTQinvQT.columns; ++j )
        {
            if ( fabs(QQTQinvQT(i,j)) > 0.05 )
            {
                sparse(i).push_back(j);
                sp++;
            }
        }
    }
    
    tprintf("sorting %d %d\n", omp_get_thread_num(), bins);
    
    vector<SortStruct*> v(tmp2.size());
    for ( unsigned int i= 0; i < tmp2.size(); ++i )
    {
        v[i]= new SortStruct(templateWeights(i), i);
    }
    sort(v.begin(), v.end(), SortStructComparator());
    
    for ( unsigned int i= 0; i < tmp2.size(); ++i )
        p(i)= v[i]->b;
    for ( unsigned int i= 0; i < tmp2.size(); ++i )
        p_inv(p(i))= i;
    for ( int i= 0; i < v.size(); ++i )
        delete v[i];
    tprintf("init positions finished\n");
    
    wpattern.resize(weights.size());
    for ( int i= 0; i < weights.size(); ++i )
        wpattern(i)= weights(i);
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWLDSF<T>::printMatrix(char* label, Matrix<float>& A)
  {
    printf("%s\n", label);
    for ( int i= 0; i < A.rows; ++i )
    {
      for ( int j= 0; j < A.columns; ++j )
	printf("%f ", A(i,j));
      printf("\n");
    }
    
    for ( int i= 0; i < A.rows; ++i )
    {
      float flag= 0;
      for ( int j= 0; j < A.columns; ++j )
	flag+= A(i,j);
      if ( flag == 0 )
	printf("empty row: %d, %f\n", i, flag);
    }
    for ( int i= 0; i < A.columns; ++i )
    {
      float flag= 0;
      for ( int j= 0; j < A.rows; ++j )
	flag+= A(j,i);
      if ( flag == 0 )
	printf("empty column: %d %f\n", i, flag);
    }
    fflush(stdout);
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSF<T>::r(double x, int j)
  {
    return (x - borders(j-1))/(borders(j) - borders(j-1));
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSF<T>::r(double x, int j, Vector<double>& borders)
  {
    return (x - borders(j-1))/(borders(j) - borders(j-1));
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWLDSF<T>::doDiscretization(int method, Vector<T>& data, Vector<double>& borders, int& output_bins, Matrix<double> &Q, Matrix<double> &QT, Matrix<double> &QTQ, Matrix<double> &QTQinv, Vector<Vector<int> >* slices, double* det)
  {
      int bins= output_bins;

      ++bins;
      
      double determinant;
      do
      {
        --bins;

        if ( (bins < 2 && output_bins != 1) || (bins < 1 && output_bins == 1) )
        {
            output_bins= bins;
            return;
        }
        
        if ( slices )
        {
              slices->clear();
              slices->resize(bins);
        }
        
        borders.clear();
        if ( method == 0 )
        {
            float tmin= FLT_MAX, tmax= -FLT_MAX;
            for ( unsigned int i= 0; i < data.size(); ++i )
            {
                if ( data(i) < tmin )
                    tmin= data(i);
                if ( data(i) > tmax )
                    tmax= data(i);
            }
          
            
            borders.push_back(tmin-0.5);
            for ( int i= 1; i < bins; ++i )
                borders.push_back(floor(tmin + (tmax - tmin)/double(bins)*i) + 0.5);
            borders.push_back(tmax+0.5);
        }
        if ( method == 1 )
        {
            
        }
        if ( method == 2 )
        {
            
        }
        
        length= data.size();
        
        Q.resize(length, bins+1);
        QT.resize(bins+1, length);
        QTQ.resize(bins+1, bins+1);
        QTQinv.resize(bins+1, bins+1);
        
        for ( int i= 0; i < Q.rows; ++i )
            for ( int j= 0; j < Q.columns; ++j )
                Q(i,j)= 0;
        
        double ri;
        for ( int i= 0; i < length; ++i )
        for ( unsigned int j= 0; j < borders.size(); ++j )
            if ( data(i) < borders(j) )
            {
                ri= r(data(i), j, borders);
                Q(i, j-1)= (1.0 - ri);
                Q(i, j)= ri;
                if ( slices )
                    slices->operator()(j-1).push_back(i);
                break;
            }

        if ( slices )
        {
            int flag= 0;
            for ( unsigned int k= 0; k < slices->size(); ++k )
                if ( (*slices)(k).size() == 0 )
                    ++flag;
            if ( flag )
                continue;
        }
                    
            
        QT= Q;
        QT.transpose();
        QTQ= QT^Q;
        
        determinant= determinantSquareMatrix(&(QTQ(0)), QTQ.rows);
    }
    while ( fabs(determinant) < 0.01 );
    
    if ( det )
        *det= determinant;
    if ( rand()%10000 == 0 )
    {
        cout << borders << endl;
        //cout << slices(0) << endl;
        //cout << slices(1) << endl;
    }
    
    output_bins= bins;
    invertSquareMatrix(&(QTQ(0)), QTQ.rows, &(QTQinv(0)));                              
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSF<T>::computeScore(Vector<T>& data, int n)
  {
    //printf("s%d ", omp_get_thread_num()); fflush(stdout);
    double result= 0;
    
    //double samewtp= meanNumberOfNeighbors(data, n, *lastTemplate, bins);
    double samewtp= 0;
    
    if ( n % 1000 == 0 )
    {
        //printf("%f,%f ", same, samewtp);
        //fflush(stdout);
    }
    
    if ( wtp == 0 || wtp == 2 || wtp == 3 || (wtp == 4 && same >= samewtp) )
    {
        //printf("aaa"); fflush(stdout);
        Vector<double> a;
        a.resize(lastTemplate->size());
        for ( int i= 0; i < lastTemplate->size(); ++i )
            a(i)= data(n + (*lastTemplate)(i).first);
        //double mean3= 0;
        Vector<double> aa;
        Vector<double> middle;
        aa.resize(a.size());
        middle.resize(a.size());
        aa= a;

        tmp2= aa;
        
        //printf("bbb"); fflush(stdout);
        Vector<double> ttt, www, weights;    
        weights.resize(tmp2.size());
        ttt.resize(tmp2.size());
        www.resize(tmp2.size());
        
        for ( unsigned int i= 0; i < tmp2.size(); ++i )
        ttt(i)= tmp2(p(i)); 
        
        weights= 1;
        
        pava.apply(ttt, weights, www);
        
        Vector<double> res(tmp2.size());
        for ( unsigned int i= 0; i < tmp2.size(); ++i )
            res(p(i))= www(i);

        //printf("ccc"); fflush(stdout);
        borders= QTQinvQT^aa;
        tmp2PAVA= Q^borders;
        
        double mean= aa.getMean();
        
        double B= 0, varA= 0, varB= 0, D= 0;
        for ( unsigned int i= 0; i < a.size(); ++i )
        {
            varB+= (aa(i) - mean)*(aa(i) - mean);
            varA+= (aa(i) - mean)*(aa(i) - mean);
        }
        
        for ( unsigned int i= 0; i < tmp2.size(); ++i )
        {
            B+= (res(i) - aa(i))*(res(i) - aa(i));
            D+= (aa(i) - tmp2PAVA(i))*(aa(i) - tmp2PAVA(i));
        }

        double penalty= B/varB;
        if ( varA < 0.01 || varB < 0.01 )
            return 1;
        
        ///////
        Vector<double> meanInSlices(bins);
        meanInSlices= 0;
        for ( int i= 0; i < slices.size(); ++i )
            for ( int j= 0; j < slices(i).size(); ++j )
                meanInSlices(i)+= aa(slices(i)(j));
        
        for ( int i= 0; i < slices.size(); ++i )
            if ( slices(i).size() > 0 )
                meanInSlices(i)/= slices(i).size();
            else
                meanInSlices(i)= 0;
            
        //printf("%f ", meanInSlices(0));

        Vector<double> meanWeights(meanInSlices.size());
        for ( int i= 0; i < slices.size(); ++i )
            meanWeights(i)= slices(i).size();
        
        Vector<double> meanOutput(bins);
        meanOutput= meanInSlices;

        pava.apply(meanInSlices, meanWeights, meanOutput);
        penalty= 0.0;
        double varb= 0;
        for ( int i= 0; i < slices.size(); ++i )
        {
            penalty+= (meanInSlices(i) - meanOutput(i))*(meanInSlices(i) - meanOutput(i))*meanWeights(i);
            varb+= (meanInSlices(i) - mean)*(meanInSlices(i) - mean)*meanWeights(i);
        }
        //printf("%f,%d,%d,%f,%f ", penalty, wtp, slices.size(), meanInSlices(0), meanOutput(0));
        //penalty/= varA;
        penalty/= varb;
        
        ///////
        //double dis1= D/varA + ( ( weight > 0.5 ) ? (1 - D/varA)*(B/varB) : 0 );
        double dis1;
        if ( weight > 0.5 )
            dis1= D/varA + (1 - D/varA)*penalty;
        else
            dis1= D/varA;
        double dis= dis1;
        result= dis;
    }
    
    float tresult;
    
    if ( wtp == 1 || wtp == 2 || wtp == 3 || (wtp == 4 && same < samewtp) )
    {
        wa.resize(lastTemplate->size());
        for ( int i= 0; i < lastTemplate->size(); ++i )
            wa(i)= data(n + (*lastTemplate)(i).first);
        length= lastTemplate->size();
        
        int tbins= bins;
        Vector<Vector<int> > slices;
        double det;
        //printf("b%d ", omp_get_thread_num()); fflush(stdout)
        doDiscretization(this->discretizer, wa, wborders, tbins, wQ, wQT, wQTQ, wQTQinv, &slices, &det);
        //printf("e%d ", omp_get_thread_num()); fflush(stdout);
        
        if ( (bins > 1 && tbins < 2) || (bins == 1 && tbins < 1) )
        {
            return 1;
        }
        
        wQTQinvQT.resize(tbins+1, length);
            
        wQTQinvQT=wQTQinv^wQT;
        
        wborders= wQTQinvQT^wpattern;
        tmp2PAVA= wQ^wborders;
        
        wweights.resize(wpattern.size());
        wttt.resize(wpattern.size());
        wwww.resize(wpattern.size());
        
        for ( unsigned int i= 0; i < wpattern.size(); ++i )
            wttt(i)= wa(p(i)); 
        
        wweights= 1;
        
        /*pava.apply(wttt, wweights, wwww);
        
        wres.resize(wpattern.size());
        for ( unsigned int i= 0; i < wpattern.size(); ++i )
            wres(p(i))= wwww(i);*/
        
        double mean= wpattern.getMean();
        
        double B= 0, varA= 0, varB= 0, D= 0;
        for ( unsigned int i= 0; i < wa.size(); ++i )
        {
            varB+= (wa(i) - mean)*(wa(i) - mean);
            varA+= (wpattern(i) - mean)*(wpattern(i) - mean);
        }
        
        for ( unsigned int i= 0; i < wpattern.size(); ++i )
        {
            //B+= (wres(i) - wa(i))*(wres(i) - wa(i));
            D+= (wpattern(i) - tmp2PAVA(i))*(wpattern(i) - tmp2PAVA(i));
        }
        
        double penalty= (B/varB);
        ///////
        if ( pen )
        {
            Vector<double> meanInSlices(tbins);
            meanInSlices= 0;
            for ( int i= 0; i < slices.size(); ++i )
                for ( int j= 0; j < slices(i).size(); ++j )
                    meanInSlices(i)+= wpattern(slices(i)(j));
            for ( int i= 0; i < slices.size(); ++i )
                if ( slices(i).size() > 0 )
                    meanInSlices(i)/= slices(i).size();
                else
                    meanInSlices(i)= 0;
            Vector<double> meanWeights(meanInSlices.size());
            for ( int i= 0; i < slices.size(); ++i )
                meanWeights(i)= slices(i).size();
            Vector<double> meanOutput(tbins);
            meanOutput= meanInSlices;
            //printf("p%d ", omp_get_thread_num()); fflush(stdout);
            pava.apply(meanInSlices, meanWeights, meanOutput);
            //printf("q%d ", omp_get_thread_num()); fflush(stdout);
            penalty= 0.0;
            double varb= 0.0;
            for ( int i= 0; i < slices.size(); ++i )
            {
                penalty+= (meanInSlices(i) - meanOutput(i))*(meanInSlices(i) - meanOutput(i))*meanWeights(i);
                varb+= (meanInSlices(i) - mean)*(meanInSlices(i) - mean)*meanWeights(i);
            }
            //penalty/= varA;
            penalty/= varb;
            ///////
            
            if ( varB < 0.01 )
                return 1;
            
        }
        double dis1;
        if ( weight > 0.5 )
            dis1= D/varA + (1 - D/varA)*penalty;
        else
            dis1= D/varA;
        double dis= dis1;
        if ( fabs(dis) > 1.02 )
        {
            printf("%f,%f, %f,%f,%f ", dis, det, D, varA, penalty);
            for ( int i= 0; i < tmp2PAVA.size(); ++i )
                printf("%f ", tmp2PAVA(i));
            printf("\n\n");

            printf("wQ: %d %d\n", wQ.rows, wQ.columns); fflush(stdout);
            for ( int i= 0; i < wQ.rows; ++i )
            {
                for ( int j= 0; j < wQ.columns; ++j )
                    printf("%f ", wQ(i,j));
                printf("\n");
            }
            printf("\n\n"); fflush(stdout);
            
            printf("wQTQ: %d %d\n", wQTQ.rows, wQTQ.columns); fflush(stdout);
            for ( int i= 0; i < wQTQ.rows; ++i )
            {
                for ( int j= 0; j < wQTQ.columns; ++j )
                    printf("%f ", wQTQ(i,j));
                printf("\n");
            }
            printf("\n\n"); fflush(stdout);
            printf("wQTQinv: %d %d\n", wQTQ.rows, wQTQ.columns); fflush(stdout);
            for ( int i= 0; i < wQTQinv.rows; ++i )
            {
                for ( int j= 0; j < wQTQinv.columns; ++j )
                    printf("%f ", wQTQinv(i,j));
                printf("\n");
            }
            printf("\n\n"); fflush(stdout);
            
            Matrix<double> tmp(wQTQ.rows, wQTQ.columns);
            tmp= wQTQ^wQTQinv;
            
            for ( int i= 0; i < tmp.rows; ++i )
            {
                for ( int j= 0; j < tmp.columns; ++j )
                    printf("%f ", tmp(i,j));
                printf("\n");
            }
            printf("\n\n");
            fflush(stdout);
            
            //getchar();
        }
        
        /*if ( rand()%10000 == 0 )
        {
            printf("wQ: %d %d\n", wQ.rows, wQ.columns); fflush(stdout);
            for ( int i= 0; i < wQ.rows; ++i )
            {
                for ( int j= 0; j < wQ.columns; ++j )
                    printf("%f ", wQ(i,j));
                printf("\n");
            }
        }*/
        
        tresult= dis;
    }
    
    //printf("f%d ", omp_get_thread_num()); fflush(stdout);
    
    if ( wtp == 1)
        return tresult;
    else if ( wtp == 0 )
        return result;
    else if ( wtp == 2 )
        return tresult + result;
    else if ( wtp == 3 )
        return tresult < result ? tresult : result;
    else if ( (wtp == 4 && same < samewtp) )
        return tresult;
    else if ( (wtp == 4 && same >= samewtp) )
        return result;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MatchingByMonotonousToneMappingPWLDSF<T>::clone()
  {
    return new MatchingByMonotonousToneMappingPWLDSF<T>(bins, discretizer, weight, wtp, pen);
  }
  
  
  
  
  
  
  
  template<typename T>
  class MatchingByToneMappingPWLDSFWtP: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      //using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MatchingByToneMappingPWLDSFWtP(int bins, int discretizer= 0);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a);
      
      virtual double r2(double x, int i, Vector<float>& borders);
      
      virtual void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/);
      
      Vector<Vector<int> > slices;
      Vector<float> borders;
      
      int bins;
      int obins;
      int length;
      int n;
      int error;
      int discretizer;
  };
  
  template<typename T>
  MatchingByToneMappingPWLDSFWtP<T>::MatchingByToneMappingPWLDSFWtP(int b, int d)
  : DissimilarityFunction<T>(), bins(b), discretizer(d)
  {
    obins= bins;
    stringstream ss;
    ss << string("MatchingByToneMappingPWLDSFWtP ") << bins << " " << discretizer;
    descriptor= ss.str();
    error= 0;
  }

  template<typename T>
  void MatchingByToneMappingPWLDSFWtP<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
  {
    cout << descriptor << endl;
      
    temp.updateStride(input.columns);
    bins= obins;
    Image<T> slicedImage;
    slicedImage.resizeImage(input);
    
    double min= FLT_MAX;
    double max= -FLT_MAX;
    
    for ( unsigned int i= 0; i < input.n; ++i )
    {
        if ( input(i) < min )
            min= input(i);
        if ( input(i) > max )
            max= input(i);
    }
    
    DataSet<T> dataset;
    ContinuousFeatureVector entry;
    entry.classIndex= 0;
    entry.resize(1);
    for ( unsigned int i= 0; i < input.n; ++i )
    {
        entry(0)= input(i);
        dataset.push_back(entry);
    }
    
    int error= 0;
    Vector<int> ns;
    try{
        do
        {
            ns.resize(bins);
            ns= 0;
            error= 0;
            Discretizer<T>* discretizer;
            if ( this->discretizer == 0 )
                discretizer= new EqualWidthDiscretizer<T>(bins);
            else if ( this->discretizer == 1 )
                discretizer= new KMeansClusteringDiscretizer<T>(bins, 28, 25);
            else
                discretizer= new EqualWidthDiscretizer<T>(bins);
            discretizer->init(dataset);
            
            borders.clear();
            for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
                borders.push_back(discretizer->data(i)(0));
            borders.insert(borders.begin(), min-1);
            borders.push_back(max+1);
            
            for ( unsigned int i= 0; i < input.n; ++i )
                for ( unsigned int j= 0; j < borders.size(); ++j )
                {
                    if ( input(i) < borders(j) )
                    {
                        slicedImage(i)= j-1;
                        ns(j-1)++;
                        break;
                    }
                }
            
            for ( unsigned int i= 0; i < ns.size(); ++i )
                if ( ns(i) == 0 )
                {
                    error= 1;
                    bins--;
                    break;
                }
            delete discretizer;
        }while ( error );
    }
    catch(std::length_error e)
    {
        bins= 0;
        output= 1;
        return;
    }
    output= 1;
    
    /*for ( unsigned int i= 0; i < ns.size(); ++i )
        printf("ns %d = %d\n", i, ns(i));*/

    float mean= 0;
    float mean2= 0;
    for ( unsigned int i= 0; i < temp.size(); ++i )
    {
        mean+= temp(i).second;
        mean2+= (temp(i).second)*(temp(i).second);
    }
    mean2/= temp.size();
    mean/= temp.size();
    float var= (mean2 - mean*mean)*temp.size();
    
    //tprintf("var: %f\n", var);
    
    #pragma omp parallel num_threads(8)
    {
        Vector<double> G;
        Vector<double> Ginv;
        Vector<double> QTp;
        Vector<int> sliceMap;
        Vector<float> bordersUpdated;
        Vector<double> Gsmall;
        Vector<double> Qsmall;
        Vector<double> GinvQTp;
        Vector<double> result;
        double ri;
        G.resize((bins+1)*(bins+1));

        int start= -temp.getMin();
        int end= int(input.n) - temp.getMax();
        #pragma omp for
        for ( int i= start; i < end; ++i )
        {
            G= 0;
            for ( unsigned int j= 0; j < temp.size(); ++j )
                for ( unsigned int k= 0; k < borders.size(); ++k )
                {
                    if ( input(i + temp(j).first) < borders(k) )
                    {
                        ri= r2(input(i + temp(j).first), k, borders);
                        G(k*(bins+1) + k)+= ri*ri;
                        G((k-1)*(bins+1) + k - 1)+= (1 - ri)*(1 - ri);
                        G((k-1)*(bins+1) + k)+= (1 - ri)*ri;
                        G(k*(bins+1) + k-1)+= (1 - ri)*ri;
                        break;
                    }
                }
                
            sliceMap.resize(bins+1);
            sliceMap= 0;
            
            for ( int j= 0; j < bins+1; ++j )
                for ( int k= 0; k < bins+1; ++k )
                    if ( G(j*(bins+1) + k) != 0 )
                        sliceMap(j)++;
                    
            int numberOfSlices= 0;
            int numberOfBins= 0;
            for ( unsigned int j= 0; j < sliceMap.size(); ++j )
                if ( sliceMap(j) > 0 )
                    numberOfSlices++;
            
            bordersUpdated.clear();
            for ( unsigned int j= 0; j < sliceMap.size(); ++j )
                if ( sliceMap(j) )
                {
                    bordersUpdated.push_back(borders(j));
                    numberOfBins+= 1;
                }

            if ( numberOfBins <= 1 )
            {
                output(i)= 1;
                continue;
            }
                
            Gsmall.resize(numberOfBins*numberOfBins);
            Qsmall.resize(numberOfBins*temp.size());
            Gsmall= 0;
            Qsmall= 0;
            for ( unsigned int j= 0; j < temp.size(); ++j )
                for ( unsigned int k= 0; k < bordersUpdated.size(); ++k )
                {
                    if ( input(i + temp(j).first) < bordersUpdated(k) )
                    {
                        ri= r2(input(i + temp(j).first), k, bordersUpdated);
                        Qsmall(j*numberOfBins + k)= ri;
                        Qsmall(j*numberOfBins + k-1)= 1 - ri;
                        Gsmall(k*(numberOfBins) + k)+= ri*ri;
                        Gsmall((k-1)*(numberOfBins) + k - 1)+= (1 - ri)*(1 - ri);
                        Gsmall((k-1)*(numberOfBins) + k)+= (1 - ri)*ri;
                        Gsmall(k*(numberOfBins) + k-1)+= (1 - ri)*ri;
                        break;
                    }
                }
            
            Ginv.resize(numberOfBins*numberOfBins);
            if ( fabs(determinantSquareMatrix(&(Gsmall(0)), numberOfBins)) > 0.01 )
                invertSquareMatrix(&(Gsmall(0)), numberOfBins, &(Ginv(0)));
            else
            {
                output(i)= 1;
                continue;
            }
            
            QTp.resize(numberOfBins);
            QTp= 0;
            
            for ( int j= 0; j < numberOfBins; ++j )
                for ( unsigned int k= 0; k < temp.size(); ++k )
                    QTp(j)+= Qsmall(k*numberOfBins + j)*temp(k).second;
                
            GinvQTp.resize(numberOfBins);
            GinvQTp= 0;
            
            for ( int j= 0; j < numberOfBins; ++j )
                for ( int k= 0; k < numberOfBins; ++k )
                    GinvQTp(j)+= Ginv(j*numberOfBins + k)*QTp(k);
            
            result.resize(temp.size());
            result= 0;
            
            for ( unsigned int j= 0; j < temp.size(); ++j )
                for ( int k= 0; k < numberOfBins; ++k )
                    result(j)+= Qsmall(j*numberOfBins + k)*GinvQTp(k);
            
            double dist= 0;
            for ( unsigned int j= 0; j < result.size(); ++j )
                dist+= (temp(j).second - result(j))*(temp(j).second - result(j));
            /*if ( fabs(dist)/var < 0.001 )
            {
                printf("%f ", dist);
                for ( int j= 0; j < temp.size(); j+= 10 )
                    printf("%f,%f ", temp(j).second, result(j));
                printf("\n");
            }*/
            output(i)= dist/var;
        }
    }
    
    //printf("aaa"); fflush(stdout);
    
    /*for ( int i= 0; i < output.n; ++i )
            if ( output(i) < 0.001 )
                printf(".");*/
  }
  
  template<typename T>
  double MatchingByToneMappingPWLDSFWtP<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initPositions(b);
    
    double score= computeScore(a);
    double result= score/(a.size()*a.getVariance());
    return result;
  }
  
  template<typename T>
  double MatchingByToneMappingPWLDSFWtP<T>::evaluate(Vector<T>& /*a*/, Template<T>& /*b*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  void MatchingByToneMappingPWLDSFWtP<T>::init(Template<T>& /*b*/)
  {
    
  }
  
  template<typename T>
  void MatchingByToneMappingPWLDSFWtP<T>::initPositions(Vector<T>& /*weights*/)
  {
    
  }
  
  template<typename T>
  double MatchingByToneMappingPWLDSFWtP<T>::r2(double x, int j, Vector<float>& borders)
  {
    return (x - borders(j-1))/(borders(j) - borders(j-1));
  }
  
  template<typename T>
  double MatchingByToneMappingPWLDSFWtP<T>::computeScore(Vector<T>& /*a*/)
  {
    return 0;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MatchingByToneMappingPWLDSFWtP<T>::clone()
  {
    return new MatchingByToneMappingPWLDSFWtP<T>(bins, discretizer);
  }
  
  
  
  
  
  
  template<typename T>
  class MatchingByMonotonousToneMappingPWLDSFWtP: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MatchingByMonotonousToneMappingPWLDSFWtP(int bins, int discretizer= 0, int penalty= 0);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a);
      
      virtual double r2(double x, int i, Vector<float>& borders);
      
      void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/);
      
      Vector<Vector<int> > slices;
      Vector<float> borders;
      
      int bins;
      int obins;
      int error;
      int discretizer;
      int penalty;
  };
  
  template<typename T>
  MatchingByMonotonousToneMappingPWLDSFWtP<T>::MatchingByMonotonousToneMappingPWLDSFWtP(int b, int d, int p)
  : DissimilarityFunction<T>(), bins(b), discretizer(d), penalty(p)
  {
    obins= bins;
    stringstream ss;
    ss << string("MatchingByMonotonousToneMappingPWLDSFWtP ") << bins << " " << discretizer << " " << penalty;
    descriptor= ss.str();
    error= 0;
  }

  template<typename T>
  void MatchingByMonotonousToneMappingPWLDSFWtP<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
  {
    cout << descriptor << endl;
      
    temp.updateStride(input.columns);
    bins= obins;
    Image<T> slicedImage;
    slicedImage.resizeImage(input);
    
    double min= FLT_MAX;
    double max= -FLT_MAX;
    
    for ( unsigned int i= 0; i < input.n; ++i )
    {
        if ( input(i) < min )
            min= input(i);
        if ( input(i) > max )
            max= input(i);
    }
    
    DataSet<T> dataset;
    ContinuousFeatureVector entry;
    entry.classIndex= 0;
    entry.resize(1);
    for ( unsigned int i= 0; i < input.n; ++i )
    {
        entry(0)= input(i);
        dataset.push_back(entry);
    }
    
    int error= 0;
    Vector<int> ns;
    try{
        do
        {
            ns.resize(bins);
            ns= 0;
            error= 0;
            Discretizer<T>* discretizer;
            if ( this->discretizer == 0 )
                discretizer= new EqualWidthDiscretizer<T>(bins);
            else if ( this->discretizer == 1 )
                discretizer= new KMeansClusteringDiscretizer<T>(bins, 28, 25);
            else
                discretizer= new EqualWidthDiscretizer<T>(bins);
            
            discretizer->init(dataset);
            
            borders.clear();
            for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
                borders.push_back(discretizer->data(i)(0));
            borders.insert(borders.begin(), min-1);
            borders.push_back(max+1);
            
            for ( unsigned int i= 0; i < input.n; ++i )
                for ( unsigned int j= 0; j < borders.size(); ++j )
                {
                    if ( input(i) < borders(j) )
                    {
                        slicedImage(i)= j-1;
                        ns(j-1)++;
                        break;
                    }
                }
            
            double neighbors= 0;
            int nNeighbors= 0;
            for ( int i= 1; i < slicedImage.rows-1; ++i )
                for ( int j= 1; j < slicedImage.columns-1; ++j )
                {
                    if ( slicedImage(i,j) == slicedImage(i,j-1) )
                        neighbors+= 1;
                    if ( slicedImage(i,j) == slicedImage(i,j+1) )
                        neighbors+= 1;
                    if ( slicedImage(i,j) == slicedImage(i-1,j) )
                        neighbors+= 1;
                    if ( slicedImage(i,j) == slicedImage(i+1,j) )
                        neighbors+= 1;
                    ++nNeighbors;
                }
            neighbors/= nNeighbors;
            tprintf("neighbors: %f\n", neighbors);
            /*if ( neighbors < 2.5 && bins > 2 )
            {
                error= 1;
                bins--;
            }
            else*/
            {
                for ( unsigned int i= 0; i < ns.size(); ++i )
                    if ( ns(i) == 0 && bins > sqrt(obins) )
                    {
                        error= 1;
                        bins--;
                        break;
                    }
                    else if ( ns(i) == 0 )
                    {
                        error= 0;
                        break;
                    }
            }
            delete discretizer;
            break;
        }while ( error );
    }
    catch(std::length_error e)
    {
        bins= 0;
        output= 1;
        return;
    }
    output= 1;
    
    /*for ( unsigned int i= 0; i < ns.size(); ++i )
        printf("ns %d = %d\n", i, ns(i));*/

    float mean= 0;
    float mean2= 0;
    for ( unsigned int i= 0; i < temp.size(); ++i )
    {
        mean+= temp(i).second;
        mean2+= (temp(i).second)*(temp(i).second);
    }
    mean2/= temp.size();
    mean/= temp.size();
    float var= (mean2 - mean*mean)*temp.size();
    
    //tprintf("var: %f\n", var);
    
    Vector<int> pReverse(temp.size());
    Vector<int> pInvReverse(temp.size());
    
    vector<SortStruct> v(temp.size());
    for ( unsigned int j= 0; j < temp.size(); ++j )
        v[j]= SortStruct(temp(j).second, j);

    sort(v.begin(), v.end(), SortStructComparatorRef());
    
    for ( unsigned int j= 0; j < v.size(); ++j )
        pReverse(j)= v[j].b;
    for ( unsigned int j= 0; j < v.size(); ++j )
        pInvReverse(pReverse(j))= j;
    
    #pragma omp parallel num_threads(8)
    {
        Vector<double> G;
        Vector<double> Ginv;
        Vector<double> QTp;
        Vector<int> sliceMap;
        Vector<float> bordersUpdated;
        Vector<double> Gsmall;
        Vector<double> Qsmall;
        Vector<double> GinvQTp;
        Vector<double> result;
        PoolAdjacentViolatorsAlgorithm<double, double> pava;
        Vector<double> resultPAVA(temp.size());
        Vector<double> weights(temp.size());
        Vector<int> p(temp.size());
        Vector<int> p_inv(temp.size());
        Vector<double> pava0(temp.size()), pava1(temp.size()), pava2(temp.size());
        Vector<double> pavaResult(temp.size());
        weights= 1;
        double ri;
        G.resize((bins+1)*(bins+1));

        int start= -temp.getMin();
        int end= int(input.n) - temp.getMax();
        #pragma omp for
        for ( int i= start; i < end; ++i )
        {
            G= 0;
            for ( unsigned int j= 0; j < temp.size(); ++j )
                for ( unsigned int k= 0; k < borders.size(); ++k )
                {
                    if ( input(i + temp(j).first) < borders(k) )
                    {
                        ri= r2(input(i + temp(j).first), k, borders);
                        G(k*(bins+1) + k)+= ri*ri;
                        G((k-1)*(bins+1) + k - 1)+= (1 - ri)*(1 - ri);
                        G((k-1)*(bins+1) + k)+= (1 - ri)*ri;
                        G(k*(bins+1) + k-1)+= (1 - ri)*ri;
                        break;
                    }
                }
                
            sliceMap.resize(bins+1);
            sliceMap= 0;
            
            for ( int j= 0; j < bins+1; ++j )
                for ( int k= 0; k < bins+1; ++k )
                    if ( G(j*(bins+1) + k) != 0 )
                        sliceMap(j)++;
                    
            int numberOfSlices= 0;
            int numberOfBins= 0;
            for ( unsigned int j= 0; j < sliceMap.size(); ++j )
                if ( sliceMap(j) > 0 )
                    numberOfSlices++;
            
            bordersUpdated.clear();
            for ( unsigned int j= 0; j < sliceMap.size(); ++j )
                if ( sliceMap(j) )
                {
                    bordersUpdated.push_back(borders(j));
                    numberOfBins+= 1;
                }

            if ( numberOfBins <= 1 )
            {
                output(i)= 1;
                continue;
            }
                
            Gsmall.resize(numberOfBins*numberOfBins);
            Qsmall.resize(numberOfBins*temp.size());
            Gsmall= 0;
            Qsmall= 0;
            for ( unsigned int j= 0; j < temp.size(); ++j )
                for ( unsigned int k= 0; k < bordersUpdated.size(); ++k )
                {
                    if ( input(i + temp(j).first) < bordersUpdated(k) )
                    {
                        ri= r2(input(i + temp(j).first), k, bordersUpdated);
                        Qsmall(j*numberOfBins + k)= ri;
                        Qsmall(j*numberOfBins + k-1)= 1 - ri;
                        Gsmall(k*(numberOfBins) + k)+= ri*ri;
                        Gsmall((k-1)*(numberOfBins) + k - 1)+= (1 - ri)*(1 - ri);
                        Gsmall((k-1)*(numberOfBins) + k)+= (1 - ri)*ri;
                        Gsmall(k*(numberOfBins) + k-1)+= (1 - ri)*ri;
                        break;
                    }
                }
            
            Ginv.resize(numberOfBins*numberOfBins);
            if ( fabs(determinantSquareMatrix(&(Gsmall(0)), numberOfBins)) > 0.01 )
                invertSquareMatrix(&(Gsmall(0)), numberOfBins, &(Ginv(0)));
            else
            {
                output(i)= 1;
                continue;
            }
            
            QTp.resize(numberOfBins);
            QTp= 0;
            
            for ( int j= 0; j < numberOfBins; ++j )
                for ( unsigned int k= 0; k < temp.size(); ++k )
                    QTp(j)+= Qsmall(k*numberOfBins + j)*temp(k).second;
                
            GinvQTp.resize(numberOfBins);
            GinvQTp= 0;
            
            for ( int j= 0; j < numberOfBins; ++j )
                for ( int k= 0; k < numberOfBins; ++k )
                    GinvQTp(j)+= Ginv(j*numberOfBins + k)*QTp(k);
            
            result.resize(temp.size());
            result= 0;
            
            for ( unsigned int j= 0; j < temp.size(); ++j )
                for ( int k= 0; k < numberOfBins; ++k )
                    result(j)+= Qsmall(j*numberOfBins + k)*GinvQTp(k);
                
            /********/
            
            double distPAVA= 0;
            double aaa= 0;
            
            penalty= 2;
            if ( penalty == 0 )
            {
                vector<SortStruct> v(temp.size());
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    v[j]= SortStruct(input(i + temp(j).first), j);

                sort(v.begin(), v.end(), SortStructComparatorRef());
                
                for ( unsigned int j= 0; j < v.size(); ++j )
                    p(j)= v[j].b;
                for ( unsigned int j= 0; j < v.size(); ++j )
                    p_inv(p(j))= j;
                
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    pava0(j)= temp(p(j)).second; 
                
                weights= 1;
                
                pava.apply(pava0, weights, pava1);
                
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    pavaResult(p(j))= pava1(j);

                for ( unsigned int j= 0; j < temp.size(); ++j )
                    distPAVA+= (temp(j).second - pavaResult(j))*(temp(j).second - pavaResult(j));
                distPAVA/= var;
                aaa= distPAVA;
            }
            else if ( penalty == 1 )
            {
            
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    pava0(j)= input(i + temp(pReverse(j)).first); 
                
                weights= 1;
                
                pava.apply(pava0, weights, pava1);
                
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    pavaResult(pReverse(j))= pava1(j);
            
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    distPAVA+= (input(i + temp(j).first) - pavaResult(j))*(input(i + temp(j).first) - pavaResult(j));
                double meanWindow= 0, varWindow= 0;
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    meanWindow+= input(i + temp(j).first);
                meanWindow/= temp.size();
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    varWindow+= (input(i + temp(j).first) - meanWindow)*(input(i + temp(j).first) - meanWindow);
                distPAVA/= varWindow;
                //printf("%f,%f ", aaa, distPAVA); fflush(stdout);
            }
            else if ( penalty == 2 )
            {
                
            }
            double dist= 0;
            for ( unsigned int j= 0; j < result.size(); ++j )
                dist+= (temp(j).second - result(j))*(temp(j).second - result(j));
            
            dist/= var;
            
            output(i)= dist + (1 - dist)*distPAVA;
        }
    }
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSFWtP<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initPositions(b);
    
    double score= computeScore(a);
    double result= score/(a.size()*a.getVariance());
    return result;
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSFWtP<T>::evaluate(Vector<T>& /*a*/, Template<T>& /*b*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWLDSFWtP<T>::init(Template<T>& /*b*/)
  {
    
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWLDSFWtP<T>::initPositions(Vector<T>& /*weights*/)
  {
    
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSFWtP<T>::r2(double x, int j, Vector<float>& borders)
  {
    return (x - borders(j-1))/(borders(j) - borders(j-1));
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSFWtP<T>::computeScore(Vector<T>& /*a*/)
  {
    return 0;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MatchingByMonotonousToneMappingPWLDSFWtP<T>::clone()
  {
    return new MatchingByMonotonousToneMappingPWLDSFWtP<T>(bins, discretizer, penalty);
  }
  
  
  
  
  
  
  
  
  
  
  
  template<typename T>
  class LBPDSF: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      LBPDSF();

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a);
      
      //void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/);
      
      void apply2(Image<T>& input, Image<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/);
  };
  
  template<typename T>
  LBPDSF<T>::LBPDSF()
  : DissimilarityFunction<T>()
  {
    stringstream ss;
    ss << string("LBPDSF ");
    descriptor= ss.str();
  }

  template<typename T>
  void LBPDSF<T>::apply2(Image<T>& input, Image<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
  {
    Vector<float> hist(256);
    Vector<float> tmpHist(256);
    
    output= 0;
    hist= 0;
    
    int flag= 0;
    for ( int i= 1; i < temp.rows-1; ++i )
        for ( int j= 1; j < temp.columns-1; ++j )
        {
            flag= 0;
            if ( temp(i,j-1) < temp(i,j) )
                flag|= 1;
            if ( temp(i,j+1) < temp(i,j) )
                flag|= 2;
            if ( temp(i-1,j) < temp(i,j) )
                flag|= 4;
            if ( temp(i+1,j) < temp(i,j) )
                flag|= 8;
            if ( temp(i+1,j+1) < temp(i,j) )
                flag|= 16;
            if ( temp(i+1,j-1) < temp(i,j) )
                flag|= 32;
            if ( temp(i-1,j+1) < temp(i,j) )
                flag|= 64;
            if ( temp(i-1,j-1) < temp(i,j) )
                flag|= 128;
            hist(flag)+= 1;
        }
        
    hist/= hist.getSum();
    
    for ( int k= 0; k < input.rows - temp.rows; ++k )
        for ( int l= 0; l < input.columns - temp.columns; ++l )
        {
            tmpHist= 0;
            
            for ( int i= 1; i < temp.rows-1; ++i )
                for ( int j= 1; j < temp.columns-1; ++j )
                {
                    flag= 0;
                    
                    if ( input(k+i,l+j-1) < input(k+i,l+j) )
                        flag|= 1;
                    if ( input(k+i,l+j+1) < input(k+i,l+j) )
                        flag|= 2;
                    if ( input(k+i-1,l+j) < input(k+i,l+j) )
                        flag|= 4;
                    if ( input(k+i+1,l+j) < input(k+i,l+j) )
                        flag|= 8;
                    if ( input(k+i+1,l+j+1) < input(k+i,l+j) )
                        flag|= 16;
                    if ( input(k+i+1,l+j-1) < input(k+i,l+j) )
                        flag|= 32;
                    if ( input(k+i-1,l+j+1) < input(k+i,l+j) )
                        flag|= 64;
                    if ( input(k+i-1,l+j-1) < input(k+i,l+j) )
                        flag|= 128;
                    tmpHist(flag)+= 1;
                }
            tmpHist/= tmpHist.getSum();
            
            float res= 0;
            for ( unsigned int i= 0; i < hist.size(); ++i )
                res+= (hist(i) - tmpHist(i))*(hist(i) - tmpHist(i));
            
            output(k + temp.rows/2, l + temp.columns/2)= sqrt(res);
        }
  }
  
  template<typename T>
  double LBPDSF<T>::evaluate(Vector<T>& /*a*/, Vector<T>& /*b*/)
  {
      return 0;
  }
  
  template<typename T>
  double LBPDSF<T>::evaluate(Vector<T>& /*a*/, Template<T>& /*b*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  void LBPDSF<T>::init(Template<T>& /*b*/)
  {
    
  }
  
  template<typename T>
  void LBPDSF<T>::initPositions(Vector<T>& /*weights*/)
  {
    
  }
  
  template<typename T>
  double LBPDSF<T>::computeScore(Vector<T>& /*a*/)
  {
    return 0;
  }
  
  template<typename T>
  DissimilarityFunction<T>* LBPDSF<T>::clone()
  {
    return new LBPDSF<T>();
  }
  
  
  
  
  
  
  
  
  template<typename T>
  class MatchingByMonotonousToneMappingPWCDSFSym: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MatchingByMonotonousToneMappingPWCDSFSym(int bins, int discretizer= 0);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a, int n);
      
      virtual void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
      
      int bins, obins;
      Vector<Vector<int> > slices;
      Vector<Vector<int> > masks;
      Vector<float> borders;
      Vector<float> means;
      Vector<float> meansPAVA;
      Vector<float> weightsPAVA;
      Vector<float> nums;
      PoolAdjacentViolatorsAlgorithm<T,T> pava;
      int discretizer;
  };
  
  template<typename T>
  MatchingByMonotonousToneMappingPWCDSFSym<T>::MatchingByMonotonousToneMappingPWCDSFSym(int b, int d)
  : DissimilarityFunction<T>(), bins(b), obins(b), discretizer(d)
  {
    stringstream ss;
    ss << string("MatchingByMonotonousToneMappingPWCDSFSym ") << bins << " " << discretizer;
    descriptor= ss.str();
  }

  template<typename T>
  double MatchingByMonotonousToneMappingPWCDSFSym<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initPositions(b);
    
    double score= computeScore(a, 0);
    double result= score;
    
    return result;
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWCDSFSym<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
  {
    cout << descriptor << endl;
    
    /*Image<float> stdevImage;
    stdevImage.resizeImage(input);
    StandardDeviationFeature2<float, float> sdf(2);
    sdf.apply(input, stdevImage);
    
    float minStd= FLT_MAX;
    for ( int i= 7; i < input.rows - 7; ++i )
        for ( int j= 7; j < input.columns - 7; ++j )
        {
            if ( stdevImage.isRealImagePixel(i,j) )
                if ( stdevImage(i,j) < minStd )
                    minStd= stdevImage(i,j);
        }
    Image<float> w;
    temp.dumpTemplate(w);

    Border2 b= sdf.getProposedBorder();
    b.borderMode= BORDER_MODE_MIRRORED;
    w.setBorder(b);
    Image<float> woutput;
    woutput.resizeImage(w);
    sdf.apply(w, woutput);
    woutput.removeBorder();
    
    float stdTemplate= FLT_MAX;
    for ( int i= 1; i < woutput.rows - 1; ++i )
        for ( int j= 1; j < woutput.columns - 1; ++j )
            if ( woutput(i,j) < stdTemplate )
                stdTemplate= woutput(i,j);
    
    cout << "stdTemplate " << stdTemplate << " minStd " << minStd << " " << stdevImage.getMax() << " " << woutput.getMax() << endl;*/
    
    //if ( /*stdTemplate > minStd*/ woutput.getMax() > stdevImage.getMax() )
    //if ( 1 )
    if ( !decision(input, temp, 32, 8, 1) )
    {
        cout << descriptor << " " << "chosen: " << "WtP" << endl;
        /*temp.updateStride(input.columns);
        bins= obins;
        Image<T> slicedImage;
        slicedImage.resizeImage(input);
        
        DataSet<T> dataset;
        ContinuousFeatureVector entry;
        entry.classIndex= 0;
        entry.resize(1);
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            entry(0)= input(i);
            dataset.push_back(entry);
        }
        
        int error= 0;
        Vector<int> ns;
        try{
            do
            {
                ns.resize(bins);
                ns= 0;
                error= 0;
                Discretizer<T>* discretizer;
                if ( this->discretizer == 0 )
                    discretizer= new EqualWidthDiscretizer<T>(bins);
                else if ( this->discretizer == 1 )
                    discretizer= new KMeansClusteringDiscretizer<T>(bins, 28, 25);
                else
                    discretizer= new EqualWidthDiscretizer<T>(bins);
                
                discretizer->init(dataset);
                
                borders.clear();
                for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
                    borders.push_back(discretizer->data(i)(0));
                borders.insert(borders.begin(), -FLT_MAX);
                borders.push_back(FLT_MAX);
                
                for ( unsigned int i= 0; i < input.n; ++i )
                    for ( unsigned int j= 0; j < borders.size(); ++j )
                    {
                        if ( input(i) < borders(j) )
                        {
                            slicedImage(i)= j-1;
                            ns(j-1)++;
                            break;
                        }
                    }
                
                for ( unsigned int i= 0; i < ns.size(); ++i )
                    if ( ns(i) == 0 )
                    {
                        error= 1;
                        bins--;
                        break;
                    }
                delete discretizer;
            }while ( error );
        }
        catch(std::length_error e)
        {
            bins= 0;
            output= 1;
            return;
        }
        output= 1;
        
        /*for ( unsigned int i= 0; i < ns.size(); ++i )
            printf("ns %d = %d\n", i, ns(i));*/

        /*Vector<Image<T> > conv;
        Vector<Image<int> > convns;
        conv.resize(bins);
        convns.resize(bins);
        for ( unsigned int i= 0; i < conv.size(); ++i )
        {
            conv(i).resizeImage(input);
            convns(i).resizeImage(input);
        }
        for ( unsigned int i= 0; i < conv.size(); ++i )
        {
            conv(i)= 0;
            convns(i)= 0;
        }
        
        Vector<float> sliceMeans;
        Vector<int> sliceNs;
        sliceMeans.resize(bins);
        sliceNs.resize(bins);
        
        int start= -temp.getMin();
        int end= int(input.n) - temp.getMax();
        
        #pragma omp parallel for
        for ( int i= start; i < end; ++i )
        {
            Vector<int> sliceNs;
            sliceNs.resize(bins);
            sliceNs= 0;
            for ( unsigned int j= 0; j < temp.size(); ++j )
            {
                int n= temp(j).first;
                if ( i + n >= 0 && i + n < int(input.n) )
                {
                    conv(slicedImage(i + n))(i)+= temp(j).second;
                    sliceNs(slicedImage(i + n))++;
                }
            }
            for ( unsigned int j= 0; j < sliceNs.size(); ++j )
                if ( sliceNs(j) > 0 )
                {
                    conv(j)(i)/= sliceNs(j);
                    convns(j)(i)= sliceNs(j);
                }
        }
        
        Vector<float> means;
        Vector<float> meansPAVA;
        Vector<float> weights;

        start= -temp.getMin();
        end= int(input.n) - temp.getMax();
        
        for ( int i= start; i < end; ++i )
        {
            means.clear();
            meansPAVA.clear();
            weights.clear();
            for ( int j= 0; j < bins; ++j )
            {
                if ( convns(j)(i) > 0 )
                {
                    means.push_back(conv(j)(i));
                    weights.push_back(convns(j)(i));
                }
            }
            meansPAVA.resize(means.size());
            pava.apply(means, weights, meansPAVA);
            int k= 0;
            for ( int j= 0; j < bins; ++j )
            {
                if ( convns(j)(i) > 0 )
                {
                    conv(j)(i)= meansPAVA(k++);
                }
            }
        }
        
        float mean= 0;
        float mean2= 0;
        for ( unsigned int i= 0; i < temp.size(); ++i )
        {
            mean+= temp(i).second;
            mean2+= (temp(i).second)*(temp(i).second);
        }
        mean2/= temp.size();
        mean/= temp.size();
        float var= (mean2 - mean*mean)*temp.size();
        
        //tprintf("var: %f\n", var);
        
        start= -temp.getMin();
        end= int(input.n) - temp.getMax();
        
        #pragma omp parallel for
        for ( int i= start; i < end; ++i )
        {
            float res= 0;
            for ( unsigned int j= 0; j < temp.size(); ++j )
            {
                int n= temp(j).first;
                res+= (conv(slicedImage(i + n))(i) - temp(j).second)*(conv(slicedImage(i + n))(i) - temp(j).second);
            }
            output(i)= res/var;
        }*/
        MatchingByMonotonousToneMappingPWCDSFWtP<T> mm(32, discretizer);
        //Image<T> output;
        
        //output2.resizeImage(output);
        mm.apply2(input, temp, output);
    }
    else
    {
        cout << descriptor << " " << "chosen: " << "Simple" << endl;
        MatchingByMonotonousToneMappingPWCDSF<T> mm(8, discretizer);
        /*Image<T> output2;
        output2.resizeImage(output);*/
        mm.apply2(input, temp, output);
        
        /*for ( int i= 0; i < output2.n; ++i )
            output(i)= output2(i);*/
    }
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWCDSFSym<T>::evaluate(Vector<T>& /*a*/, Template<T>& /*b*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWCDSFSym<T>::init(Template<T>& /*b*/)
  {
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWCDSFSym<T>::initPositions(Vector<T>& /*weights*/)
  {
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWCDSFSym<T>::computeScore(Vector<T>& /*a*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MatchingByMonotonousToneMappingPWCDSFSym<T>::clone()
  {
    return new MatchingByMonotonousToneMappingPWCDSFSym<T>(bins, discretizer);
  }
  
  
  
  
  
  
  
  
  
  template<typename T>
  class MatchingByMonotonousToneMappingPWLDSFSym: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MatchingByMonotonousToneMappingPWLDSFSym(int bins, int discretizer= 0, int penalty= 0);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a);
      
      virtual double r2(double x, int i, Vector<float>& borders);
      
      void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/);
      
      Vector<Vector<int> > slices;
      Vector<float> borders;
      
      int bins;
      int obins;
      int error;
      int discretizer;
      int penalty;
  };
  
  template<typename T>
  MatchingByMonotonousToneMappingPWLDSFSym<T>::MatchingByMonotonousToneMappingPWLDSFSym(int b, int d, int p)
  : DissimilarityFunction<T>(), bins(b), discretizer(d), penalty(p)
  {
    obins= bins;
    stringstream ss;
    ss << string("MatchingByMonotonousToneMappingPWLDSFSym ") << bins << " " << discretizer << " " << penalty;
    descriptor= ss.str();
    error= 0;
  }

  template<typename T>
  void MatchingByMonotonousToneMappingPWLDSFSym<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
  {
    cout << descriptor << endl;
    
    Image<float> stdevImage;
    stdevImage.resizeImage(input);
    StandardDeviationFeature2<float, float> sdf(2);
    sdf.apply(input, stdevImage);
    
    float minStd= FLT_MAX;
    for ( int i= 7; i < input.rows - 7; ++i )
        for ( int j= 7; j < input.columns - 7; ++j )
        {
            if ( stdevImage.isRealImagePixel(i,j) )
                if ( stdevImage(i,j) < minStd )
                    minStd= stdevImage(i,j);
        }
    Image<float> w;
    temp.dumpTemplate(w);

    Border2 b= sdf.getProposedBorder();
    b.borderMode= BORDER_MODE_MIRRORED;
    w.setBorder(b);
    Image<float> woutput;
    woutput.resizeImage(w);
    sdf.apply(w, woutput);
    woutput.removeBorder();
    
    float stdTemplate= FLT_MAX;
    for ( int i= 1; i < woutput.rows - 1; ++i )
        for ( int j= 1; j < woutput.columns - 1; ++j )
            if ( woutput(i,j) < stdTemplate )
                stdTemplate= woutput(i,j);
    
    cout << "stdTemplate " << stdTemplate << " minStd " << minStd << " " << stdevImage.getMax() << " " << woutput.getMax() << endl;
    
    //if ( /*stdTemplate > minStd*/ woutput.getMax() > stdevImage.getMax() )
    if ( 1 )
    {
        cout << descriptor << " " << "chosen: " << "WtP" << endl;
        temp.updateStride(input.columns);
        bins= obins;
        Image<T> slicedImage;
        slicedImage.resizeImage(input);
        
        double min= FLT_MAX;
        double max= -FLT_MAX;
        
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            if ( input(i) < min )
                min= input(i);
            if ( input(i) > max )
                max= input(i);
        }
        
        DataSet<T> dataset;
        ContinuousFeatureVector entry;
        entry.classIndex= 0;
        entry.resize(1);
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            entry(0)= input(i);
            dataset.push_back(entry);
        }
        
        int error= 0;
        Vector<int> ns;
        try{
            do
            {
                ns.resize(bins);
                ns= 0;
                error= 0;
                Discretizer<T>* discretizer;
                if ( this->discretizer == 0 )
                    discretizer= new EqualWidthDiscretizer<T>(bins);
                else if ( this->discretizer == 1 )
                    discretizer= new KMeansClusteringDiscretizer<T>(bins, 28, 25);
                else
                    discretizer= new EqualWidthDiscretizer<T>(bins);
                
                discretizer->init(dataset);
                
                borders.clear();
                for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
                    borders.push_back(discretizer->data(i)(0));
                borders.insert(borders.begin(), min-1);
                borders.push_back(max+1);
                
                for ( unsigned int i= 0; i < input.n; ++i )
                    for ( unsigned int j= 0; j < borders.size(); ++j )
                    {
                        if ( input(i) < borders(j) )
                        {
                            slicedImage(i)= j-1;
                            ns(j-1)++;
                            break;
                        }
                    }
                
                for ( unsigned int i= 0; i < ns.size(); ++i )
                    if ( ns(i) == 0 )
                    {
                        error= 1;
                        bins--;
                        break;
                    }
                delete discretizer;
            }while ( error );
        }
        catch(std::length_error e)
        {
            bins= 0;
            output= 1;
            return;
        }
        output= 1;
        
        /*for ( unsigned int i= 0; i < ns.size(); ++i )
            printf("ns %d = %d\n", i, ns(i));*/

        float mean= 0;
        float mean2= 0;
        for ( unsigned int i= 0; i < temp.size(); ++i )
        {
            mean+= temp(i).second;
            mean2+= (temp(i).second)*(temp(i).second);
        }
        mean2/= temp.size();
        mean/= temp.size();
        float var= (mean2 - mean*mean)*temp.size();
        
        //tprintf("var: %f\n", var);
        
        Vector<int> pReverse(temp.size());
        Vector<int> pInvReverse(temp.size());
        
        vector<SortStruct> v(temp.size());
        for ( unsigned int j= 0; j < temp.size(); ++j )
            v[j]= SortStruct(temp(j).second, j);

        sort(v.begin(), v.end(), SortStructComparatorRef());
        
        for ( unsigned int j= 0; j < v.size(); ++j )
            pReverse(j)= v[j].b;
        for ( unsigned int j= 0; j < v.size(); ++j )
            pInvReverse(pReverse(j))= j;
        
        #pragma omp parallel num_threads(8)
        {
            Vector<double> G;
            Vector<double> Ginv;
            Vector<double> QTp;
            Vector<int> sliceMap;
            Vector<float> bordersUpdated;
            Vector<double> Gsmall;
            Vector<double> Qsmall;
            Vector<double> GinvQTp;
            Vector<double> result;
            PoolAdjacentViolatorsAlgorithm<double, double> pava;
            Vector<double> resultPAVA(temp.size());
            Vector<double> weights(temp.size());
            Vector<int> p(temp.size());
            Vector<int> p_inv(temp.size());
            Vector<double> pava0(temp.size()), pava1(temp.size()), pava2(temp.size());
            Vector<double> pavaResult(temp.size());
            weights= 1;
            double ri;
            G.resize((bins+1)*(bins+1));

            int start= -temp.getMin();
            int end= int(input.n) - temp.getMax();
            #pragma omp for
            for ( int i= start; i < end; ++i )
            {
                G= 0;
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    for ( unsigned int k= 0; k < borders.size(); ++k )
                    {
                        if ( input(i + temp(j).first) < borders(k) )
                        {
                            ri= r2(input(i + temp(j).first), k, borders);
                            G(k*(bins+1) + k)+= ri*ri;
                            G((k-1)*(bins+1) + k - 1)+= (1 - ri)*(1 - ri);
                            G((k-1)*(bins+1) + k)+= (1 - ri)*ri;
                            G(k*(bins+1) + k-1)+= (1 - ri)*ri;
                            break;
                        }
                    }
                    
                sliceMap.resize(bins+1);
                sliceMap= 0;
                
                for ( int j= 0; j < bins+1; ++j )
                    for ( int k= 0; k < bins+1; ++k )
                        if ( G(j*(bins+1) + k) != 0 )
                            sliceMap(j)++;
                        
                int numberOfSlices= 0;
                int numberOfBins= 0;
                for ( unsigned int j= 0; j < sliceMap.size(); ++j )
                    if ( sliceMap(j) > 0 )
                        numberOfSlices++;
                
                bordersUpdated.clear();
                for ( unsigned int j= 0; j < sliceMap.size(); ++j )
                    if ( sliceMap(j) )
                    {
                        bordersUpdated.push_back(borders(j));
                        numberOfBins+= 1;
                    }

                if ( numberOfBins <= 1 )
                {
                    output(i)= 1;
                    continue;
                }
                    
                Gsmall.resize(numberOfBins*numberOfBins);
                Qsmall.resize(numberOfBins*temp.size());
                Gsmall= 0;
                Qsmall= 0;
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    for ( unsigned int k= 0; k < bordersUpdated.size(); ++k )
                    {
                        if ( input(i + temp(j).first) < bordersUpdated(k) )
                        {
                            ri= r2(input(i + temp(j).first), k, bordersUpdated);
                            Qsmall(j*numberOfBins + k)= ri;
                            Qsmall(j*numberOfBins + k-1)= 1 - ri;
                            Gsmall(k*(numberOfBins) + k)+= ri*ri;
                            Gsmall((k-1)*(numberOfBins) + k - 1)+= (1 - ri)*(1 - ri);
                            Gsmall((k-1)*(numberOfBins) + k)+= (1 - ri)*ri;
                            Gsmall(k*(numberOfBins) + k-1)+= (1 - ri)*ri;
                            break;
                        }
                    }
                
                Ginv.resize(numberOfBins*numberOfBins);
                if ( fabs(determinantSquareMatrix(&(Gsmall(0)), numberOfBins)) > 0.01 )
                    invertSquareMatrix(&(Gsmall(0)), numberOfBins, &(Ginv(0)));
                else
                {
                    output(i)= 1;
                    continue;
                }
                
                QTp.resize(numberOfBins);
                QTp= 0;
                
                for ( int j= 0; j < numberOfBins; ++j )
                    for ( unsigned int k= 0; k < temp.size(); ++k )
                        QTp(j)+= Qsmall(k*numberOfBins + j)*temp(k).second;
                    
                GinvQTp.resize(numberOfBins);
                GinvQTp= 0;
                
                for ( int j= 0; j < numberOfBins; ++j )
                    for ( int k= 0; k < numberOfBins; ++k )
                        GinvQTp(j)+= Ginv(j*numberOfBins + k)*QTp(k);
                
                result.resize(temp.size());
                result= 0;
                
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    for ( int k= 0; k < numberOfBins; ++k )
                        result(j)+= Qsmall(j*numberOfBins + k)*GinvQTp(k);
                    
                /********/
                
                double distPAVA= 0;
                
                if ( penalty == 0 )
                {
                    vector<SortStruct> v(temp.size());
                    for ( unsigned int j= 0; j < temp.size(); ++j )
                        v[j]= SortStruct(input(i + temp(j).first), j);

                    sort(v.begin(), v.end(), SortStructComparatorRef());
                    
                    for ( unsigned int j= 0; j < v.size(); ++j )
                        p(j)= v[j].b;
                    for ( unsigned int j= 0; j < v.size(); ++j )
                        p_inv(p(j))= j;
                    
                    for ( unsigned int j= 0; j < temp.size(); ++j )
                        pava0(j)= temp(p(j)).second; 
                    
                    weights= 1;
                    
                    pava.apply(pava0, weights, pava1);
                    
                    for ( unsigned int j= 0; j < temp.size(); ++j )
                        pavaResult(p(j))= pava1(j);

                    for ( unsigned int j= 0; j < temp.size(); ++j )
                        distPAVA+= (temp(j).second - pavaResult(j))*(temp(j).second - pavaResult(j));
                    distPAVA/= var;
                }
                else if ( penalty == 1 )
                {
                
                    for ( unsigned int j= 0; j < temp.size(); ++j )
                        pava0(j)= input(i + temp(pReverse(j)).first); 
                    
                    weights= 1;
                    
                    pava.apply(pava0, weights, pava1);
                    
                    for ( unsigned int j= 0; j < temp.size(); ++j )
                        pavaResult(pReverse(j))= pava1(j);
                
                    for ( unsigned int j= 0; j < temp.size(); ++j )
                        distPAVA+= (input(i + temp(j).first) - pavaResult(j))*(input(i + temp(j).first) - pavaResult(j));
                    double meanWindow= 0, varWindow= 0;
                    for ( unsigned int j= 0; j < temp.size(); ++j )
                        meanWindow+= input(i + temp(j).first);
                    meanWindow/= temp.size();
                    for ( unsigned int j= 0; j < temp.size(); ++j )
                        varWindow+= (input(i + temp(j).first) - meanWindow)*(input(i + temp(j).first) - meanWindow);
                    distPAVA/= varWindow;
                }   
                double dist= 0;
                for ( unsigned int j= 0; j < result.size(); ++j )
                    dist+= (temp(j).second - result(j))*(temp(j).second - result(j));
                
                dist/= var;
                
                output(i)= dist + (1 - dist)*distPAVA;
            }
        }
    }
    else
    {
        cout << descriptor << " " << "chosen: " << "Simple" << endl;
        MatchingByMonotonousToneMappingPWLDSF<T> mm(bins, discretizer);
        Image<T> output2;
        output2.resizeImage(output);
        mm.apply2(input, temp, output2);
        
        for ( int i= 0; i < output2.n; ++i )
            output(i)= output2(i);
    }
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSFSym<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initPositions(b);
    
    double score= computeScore(a);
    double result= score/(a.size()*a.getVariance());
    return result;
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSFSym<T>::evaluate(Vector<T>& /*a*/, Template<T>& /*b*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWLDSFSym<T>::init(Template<T>& /*b*/)
  {
    
  }
  
  template<typename T>
  void MatchingByMonotonousToneMappingPWLDSFSym<T>::initPositions(Vector<T>& /*weights*/)
  {
    
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSFSym<T>::r2(double x, int j, Vector<float>& borders)
  {
    return (x - borders(j-1))/(borders(j) - borders(j-1));
  }
  
  template<typename T>
  double MatchingByMonotonousToneMappingPWLDSFSym<T>::computeScore(Vector<T>& /*a*/)
  {
    return 0;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MatchingByMonotonousToneMappingPWLDSFSym<T>::clone()
  {
    return new MatchingByMonotonousToneMappingPWLDSFSym<T>(bins, discretizer, penalty);
  }
  
  
  
  
  template<typename T>
  class MatchingByToneMappingPWCDSFSym: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MatchingByToneMappingPWCDSFSym(int bins, int discretizer= 0);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a, int n);
      
      virtual void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
      
      int bins, obins;
      Vector<Vector<int> > slices;
      Vector<Vector<int> > masks;
      Vector<float> borders;
      Vector<float> means;
      Vector<float> meansPAVA;
      Vector<float> weightsPAVA;
      Vector<float> nums;
      PoolAdjacentViolatorsAlgorithm<T,T> pava;
      int discretizer;
  };
  
  template<typename T>
  MatchingByToneMappingPWCDSFSym<T>::MatchingByToneMappingPWCDSFSym(int b, int d)
  : DissimilarityFunction<T>(), bins(b), obins(b), discretizer(d)
  {
    stringstream ss;
    ss << string("MatchingByToneMappingPWCDSFSym ") << bins << " " << discretizer;
    descriptor= ss.str();
  }

  template<typename T>
  double MatchingByToneMappingPWCDSFSym<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initPositions(b);
    
    double score= computeScore(a, 0);
    double result= score;
    
    return result;
  }
  
  template<typename T>
  void MatchingByToneMappingPWCDSFSym<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
  {
    
    cout << descriptor << endl;
    
    Image<float> stdevImage;
    stdevImage.resizeImage(input);
    StandardDeviationFeature2<float, float> sdf(2);
    sdf.apply(input, stdevImage);
    
    float minStd= FLT_MAX;
    for ( int i= 7; i < input.rows - 7; ++i )
        for ( int j= 7; j < input.columns - 7; ++j )
        {
            if ( stdevImage.isRealImagePixel(i,j) )
                if ( stdevImage(i,j) < minStd )
                    minStd= stdevImage(i,j);
        }
    Image<float> w;
    temp.dumpTemplate(w);

    Border2 b= sdf.getProposedBorder();
    b.borderMode= BORDER_MODE_MIRRORED;
    w.setBorder(b);
    Image<float> woutput;
    woutput.resizeImage(w);
    sdf.apply(w, woutput);
    woutput.removeBorder();
    
    float stdTemplate= FLT_MAX;
    for ( int i= 1; i < woutput.rows - 1; ++i )
        for ( int j= 1; j < woutput.columns - 1; ++j )
            if ( woutput(i,j) < stdTemplate )
                stdTemplate= woutput(i,j);
    
    
    
    cout << "stdTemplate " << stdTemplate << " minStd " << minStd << " " << stdevImage.getMax() << " " << woutput.getMax() << endl;
    
    if ( stdTemplate > minStd )
    {
        cout << descriptor << " " << "chosen: " << "WtP" << endl;
      
        temp.updateStride(input.columns);
        bins= obins;
        Image<T> slicedImage;
        slicedImage.resizeImage(input);
        
        DataSet<T> dataset;
        ContinuousFeatureVector entry;
        entry.classIndex= 0;
        entry.resize(1);
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            entry(0)= input(i);
            dataset.push_back(entry);
        }
        
        int error= 0;
        Vector<int> ns;
        try{
            do
            {
                ns.resize(bins);
                ns= 0;
                error= 0;
                Discretizer<T>* discretizer;
                if ( this->discretizer == 0 )
                    discretizer= new EqualWidthDiscretizer<T>(bins);
                else if ( this->discretizer == 1 )
                    discretizer= new KMeansClusteringDiscretizer<T>(bins, 28, 25);
                else
                    discretizer= new EqualWidthDiscretizer<T>(bins);
                
                discretizer->init(dataset);
                
                borders.clear();
                for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
                    borders.push_back(discretizer->data(i)(0));
                borders.insert(borders.begin(), -FLT_MAX);
                borders.push_back(FLT_MAX);
                
                for ( unsigned int i= 0; i < input.n; ++i )
                    for ( unsigned int j= 0; j < borders.size(); ++j )
                    {
                        if ( input(i) < borders(j) )
                        {
                            slicedImage(i)= j-1;
                            ns(j-1)++;
                            break;
                        }
                    }
                
                for ( unsigned int i= 0; i < ns.size(); ++i )
                    if ( ns(i) == 0 )
                    {
                        error= 1;
                        bins--;
                        break;
                    }
                delete discretizer;
            }while ( error );
        }
        catch(std::length_error e)
        {
            bins= 0;
            output= 1;
            return;
        }
        output= 1;
        
        /*for ( unsigned int i= 0; i < ns.size(); ++i )
            printf("ns %d = %d\n", i, ns(i));*/

        Vector<Image<T> > conv;
        Vector<Image<int> > convns;
        conv.resize(bins);
        convns.resize(bins);
        for ( unsigned int i= 0; i < conv.size(); ++i )
        {
            conv(i).resizeImage(input);
            convns(i).resizeImage(input);
        }
        for ( unsigned int i= 0; i < conv.size(); ++i )
        {
            conv(i)= 0;
            convns(i)= 0;
        }
        
        Vector<float> sliceMeans;
        Vector<int> sliceNs;
        sliceMeans.resize(bins);
        sliceNs.resize(bins);
        
        int min= -temp.getMin();
        int max= int(input.n) - temp.getMax();
        
        #pragma omp parallel for
        for ( int i= min; i < max; ++i )
        {
            Vector<int> sliceNs;
            sliceNs.resize(bins);
            sliceNs= 0;
            for ( unsigned int j= 0; j < temp.size(); ++j )
            {
                int n= temp(j).first;
                if ( i + n >= 0 && i + n < int(input.n) )
                {
                    conv(slicedImage(i + n))(i)+= temp(j).second;
                    sliceNs(slicedImage(i + n))++;
                }
            }
            for ( unsigned int j= 0; j < sliceNs.size(); ++j )
                if ( sliceNs(j) > 0 )
                {
                    conv(j)(i)/= sliceNs(j);
                    convns(j)(i)= sliceNs(j);
                }
        }
        
        float mean= 0;
        float mean2= 0;
        for ( unsigned int i= 0; i < temp.size(); ++i )
        {
            mean+= temp(i).second;
            mean2+= (temp(i).second)*(temp(i).second);
        }
        mean2/= temp.size();
        mean/= temp.size();
        float var= (mean2 - mean*mean)*temp.size();
        
        //tprintf("var: %f\n", var);
        
        int start= -temp.getMin();
        int end= int(input.n) - temp.getMax();
        
        #pragma omp parallel for
        for ( int i= start; i < end; ++i )
        {
            float res= 0;
            for ( unsigned int j= 0; j < temp.size(); ++j )
            {
                int n= temp(j).first;
                res+= (conv(slicedImage(i + n))(i) - temp(j).second)*(conv(slicedImage(i + n))(i) - temp(j).second);
            }
            output(i)= res/var;
        }
    }
    else
    {
        cout << descriptor << " " << "chosen: " << "Simple" << endl;
        MatchingByToneMappingPWCDSF<T> mm(bins, discretizer);
        Image<T> output2;
        output2.resizeImage(output);
        mm.apply2(input, temp, output2);
        
        for ( int i= 0; i < output2.n; ++i )
            output(i)= output2(i);
    }
  }
  
  template<typename T>
  double MatchingByToneMappingPWCDSFSym<T>::evaluate(Vector<T>& /*a*/, Template<T>& /*b*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  void MatchingByToneMappingPWCDSFSym<T>::init(Template<T>& /*b*/)
  {
  }
  
  template<typename T>
  void MatchingByToneMappingPWCDSFSym<T>::initPositions(Vector<T>& /*weights*/)
  {
  }
  
  template<typename T>
  double MatchingByToneMappingPWCDSFSym<T>::computeScore(Vector<T>& /*a*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MatchingByToneMappingPWCDSFSym<T>::clone()
  {
    return new MatchingByToneMappingPWCDSFSym<T>(bins, discretizer);
  }
  
  
  
  template<typename T>
  class MatchingByToneMappingPWLDSFSym: public DissimilarityFunction<T>
  {
  public:
      using DissimilarityFunction<T>::descriptor;
      using DissimilarityFunction<T>::initialized;
      using DissimilarityFunction<T>::apply1;
      //using DissimilarityFunction<T>::apply2;
      using DissimilarityFunction<T>::lastTemplate;

      MatchingByToneMappingPWLDSFSym(int bins, int discretizer= 0);

      virtual double evaluate(Vector<T>& a, Vector<T>& b);
      
      virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
      
      virtual void init(Template<T>& b);
      
      virtual DissimilarityFunction<T>* clone();
      
      virtual void initPositions(Vector<T>& b);
      
      virtual double computeScore(Vector<T>& a);
      
      virtual double r2(double x, int i, Vector<float>& borders);
      
      virtual void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/);
      
      Vector<Vector<int> > slices;
      Vector<float> borders;
      
      int bins;
      int obins;
      int length;
      int n;
      int error;
      int discretizer;
  };
  
  template<typename T>
  MatchingByToneMappingPWLDSFSym<T>::MatchingByToneMappingPWLDSFSym(int b, int d)
  : DissimilarityFunction<T>(), bins(b), discretizer(d)
  {
    obins= bins;
    stringstream ss;
    ss << string("MatchingByToneMappingPWLDSFSym ") << bins << " " << discretizer;
    descriptor= ss.str();
    error= 0;
  }

  template<typename T>
  void MatchingByToneMappingPWLDSFSym<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
  {
    cout << descriptor << endl;
    
    Image<float> stdevImage;
    stdevImage.resizeImage(input);
    StandardDeviationFeature2<float, float> sdf(2);
    sdf.apply(input, stdevImage);
    
    float minStd= FLT_MAX;
    for ( int i= 7; i < input.rows - 7; ++i )
        for ( int j= 7; j < input.columns - 7; ++j )
        {
            if ( stdevImage.isRealImagePixel(i,j) )
                if ( stdevImage(i,j) < minStd )
                    minStd= stdevImage(i,j);
        }
    Image<float> w;
    temp.dumpTemplate(w);

    Border2 b= sdf.getProposedBorder();
    b.borderMode= BORDER_MODE_MIRRORED;
    w.setBorder(b);
    Image<float> woutput;
    woutput.resizeImage(w);
    sdf.apply(w, woutput);
    woutput.removeBorder();
    
    float stdTemplate= FLT_MAX;
    for ( int i= 1; i < woutput.rows - 1; ++i )
        for ( int j= 1; j < woutput.columns - 1; ++j )
            if ( woutput(i,j) < stdTemplate )
                stdTemplate= woutput(i,j);
    
    cout << "stdTemplate " << stdTemplate << " minStd " << minStd << " " << stdevImage.getMax() << " " << woutput.getMax() << endl;
    
    if ( stdTemplate > minStd )
    {
        cout << descriptor << " " << "chosen: " << "WtP" << endl;
      
        temp.updateStride(input.columns);
        bins= obins;
        Image<T> slicedImage;
        slicedImage.resizeImage(input);
        
        double min= FLT_MAX;
        double max= -FLT_MAX;
        
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            if ( input(i) < min )
                min= input(i);
            if ( input(i) > max )
                max= input(i);
        }
        
        DataSet<T> dataset;
        ContinuousFeatureVector entry;
        entry.classIndex= 0;
        entry.resize(1);
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            entry(0)= input(i);
            dataset.push_back(entry);
        }
        
        int error= 0;
        Vector<int> ns;
        try{
            do
            {
                ns.resize(bins);
                ns= 0;
                error= 0;
                Discretizer<T>* discretizer;
                if ( this->discretizer == 0 )
                    discretizer= new EqualWidthDiscretizer<T>(bins);
                else if ( this->discretizer == 1 )
                    discretizer= new KMeansClusteringDiscretizer<T>(bins, 28, 25);
                else
                    discretizer= new EqualWidthDiscretizer<T>(bins);
                discretizer->init(dataset);
                
                borders.clear();
                for ( unsigned int i= 0; i < discretizer->data.size(); ++i )
                    borders.push_back(discretizer->data(i)(0));
                borders.insert(borders.begin(), min-1);
                borders.push_back(max+1);
                
                for ( unsigned int i= 0; i < input.n; ++i )
                    for ( unsigned int j= 0; j < borders.size(); ++j )
                    {
                        if ( input(i) < borders(j) )
                        {
                            slicedImage(i)= j-1;
                            ns(j-1)++;
                            break;
                        }
                    }
                
                for ( unsigned int i= 0; i < ns.size(); ++i )
                    if ( ns(i) == 0 )
                    {
                        error= 1;
                        bins--;
                        break;
                    }
                delete discretizer;
            }while ( error );
        }
        catch(std::length_error e)
        {
            bins= 0;
            output= 1;
            return;
        }
        output= 1;
        
        /*for ( unsigned int i= 0; i < ns.size(); ++i )
            printf("ns %d = %d\n", i, ns(i));*/

        float mean= 0;
        float mean2= 0;
        for ( unsigned int i= 0; i < temp.size(); ++i )
        {
            mean+= temp(i).second;
            mean2+= (temp(i).second)*(temp(i).second);
        }
        mean2/= temp.size();
        mean/= temp.size();
        float var= (mean2 - mean*mean)*temp.size();
        
        //tprintf("var: %f\n", var);
        
        #pragma omp parallel num_threads(8)
        {
            Vector<double> G;
            Vector<double> Ginv;
            Vector<double> QTp;
            Vector<int> sliceMap;
            Vector<float> bordersUpdated;
            Vector<double> Gsmall;
            Vector<double> Qsmall;
            Vector<double> GinvQTp;
            Vector<double> result;
            double ri;
            G.resize((bins+1)*(bins+1));

            int start= -temp.getMin();
            int end= int(input.n) - temp.getMax();
            #pragma omp for
            for ( int i= start; i < end; ++i )
            {
                G= 0;
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    for ( unsigned int k= 0; k < borders.size(); ++k )
                    {
                        if ( input(i + temp(j).first) < borders(k) )
                        {
                            ri= r2(input(i + temp(j).first), k, borders);
                            G(k*(bins+1) + k)+= ri*ri;
                            G((k-1)*(bins+1) + k - 1)+= (1 - ri)*(1 - ri);
                            G((k-1)*(bins+1) + k)+= (1 - ri)*ri;
                            G(k*(bins+1) + k-1)+= (1 - ri)*ri;
                            break;
                        }
                    }
                    
                sliceMap.resize(bins+1);
                sliceMap= 0;
                
                for ( int j= 0; j < bins+1; ++j )
                    for ( int k= 0; k < bins+1; ++k )
                        if ( G(j*(bins+1) + k) != 0 )
                            sliceMap(j)++;
                        
                int numberOfSlices= 0;
                int numberOfBins= 0;
                for ( unsigned int j= 0; j < sliceMap.size(); ++j )
                    if ( sliceMap(j) > 0 )
                        numberOfSlices++;
                
                bordersUpdated.clear();
                for ( unsigned int j= 0; j < sliceMap.size(); ++j )
                    if ( sliceMap(j) )
                    {
                        bordersUpdated.push_back(borders(j));
                        numberOfBins+= 1;
                    }

                if ( numberOfBins <= 1 )
                {
                    output(i)= 1;
                    continue;
                }
                    
                Gsmall.resize(numberOfBins*numberOfBins);
                Qsmall.resize(numberOfBins*temp.size());
                Gsmall= 0;
                Qsmall= 0;
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    for ( unsigned int k= 0; k < bordersUpdated.size(); ++k )
                    {
                        if ( input(i + temp(j).first) < bordersUpdated(k) )
                        {
                            ri= r2(input(i + temp(j).first), k, bordersUpdated);
                            Qsmall(j*numberOfBins + k)= ri;
                            Qsmall(j*numberOfBins + k-1)= 1 - ri;
                            Gsmall(k*(numberOfBins) + k)+= ri*ri;
                            Gsmall((k-1)*(numberOfBins) + k - 1)+= (1 - ri)*(1 - ri);
                            Gsmall((k-1)*(numberOfBins) + k)+= (1 - ri)*ri;
                            Gsmall(k*(numberOfBins) + k-1)+= (1 - ri)*ri;
                            break;
                        }
                    }
                
                Ginv.resize(numberOfBins*numberOfBins);
                if ( fabs(determinantSquareMatrix(&(Gsmall(0)), numberOfBins)) > 0.01 )
                    invertSquareMatrix(&(Gsmall(0)), numberOfBins, &(Ginv(0)));
                else
                {
                    output(i)= 1;
                    continue;
                }
                
                QTp.resize(numberOfBins);
                QTp= 0;
                
                for ( int j= 0; j < numberOfBins; ++j )
                    for ( unsigned int k= 0; k < temp.size(); ++k )
                        QTp(j)+= Qsmall(k*numberOfBins + j)*temp(k).second;
                    
                GinvQTp.resize(numberOfBins);
                GinvQTp= 0;
                
                for ( int j= 0; j < numberOfBins; ++j )
                    for ( int k= 0; k < numberOfBins; ++k )
                        GinvQTp(j)+= Ginv(j*numberOfBins + k)*QTp(k);
                
                result.resize(temp.size());
                result= 0;
                
                for ( unsigned int j= 0; j < temp.size(); ++j )
                    for ( int k= 0; k < numberOfBins; ++k )
                        result(j)+= Qsmall(j*numberOfBins + k)*GinvQTp(k);
                
                double dist= 0;
                for ( unsigned int j= 0; j < result.size(); ++j )
                    dist+= (temp(j).second - result(j))*(temp(j).second - result(j));
                /*if ( fabs(dist)/var < 0.001 )
                {
                    printf("%f ", dist);
                    for ( int j= 0; j < temp.size(); j+= 10 )
                        printf("%f,%f ", temp(j).second, result(j));
                    printf("\n");
                }*/
                output(i)= dist/var;
            }
        }
        
        //printf("aaa"); fflush(stdout);
        
        /*for ( int i= 0; i < output.n; ++i )
                if ( output(i) < 0.001 )
                    printf(".");*/
    }
    else
    {
        cout << descriptor << " " << "chosen: " << "Simple" << endl;
        MatchingByToneMappingPWLDSF<T> mm(bins, discretizer);
        Image<T> output2;
        output2.resizeImage(output);
        mm.apply2(input, temp, output2);
        
        for ( int i= 0; i < output2.n; ++i )
            output(i)= output2(i);
    }
  }
  
  template<typename T>
  double MatchingByToneMappingPWLDSFSym<T>::evaluate(Vector<T>& a, Vector<T>& b)
  {
    initPositions(b);
    
    double score= computeScore(a);
    double result= score/(a.size()*a.getVariance());
    return result;
  }
  
  template<typename T>
  double MatchingByToneMappingPWLDSFSym<T>::evaluate(Vector<T>& /*a*/, Template<T>& /*b*/, int /*n*/)
  {
    return 0;
  }
  
  template<typename T>
  void MatchingByToneMappingPWLDSFSym<T>::init(Template<T>& /*b*/)
  {
    
  }
  
  template<typename T>
  void MatchingByToneMappingPWLDSFSym<T>::initPositions(Vector<T>& /*weights*/)
  {
    
  }
  
  template<typename T>
  double MatchingByToneMappingPWLDSFSym<T>::r2(double x, int j, Vector<float>& borders)
  {
    return (x - borders(j-1))/(borders(j) - borders(j-1));
  }
  
  template<typename T>
  double MatchingByToneMappingPWLDSFSym<T>::computeScore(Vector<T>& /*a*/)
  {
    return 0;
  }
  
  template<typename T>
  DissimilarityFunction<T>* MatchingByToneMappingPWLDSFSym<T>::clone()
  {
    return new MatchingByToneMappingPWLDSFSym<T>(bins, discretizer);
  }
}
#endif

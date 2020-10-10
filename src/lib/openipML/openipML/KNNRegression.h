#ifndef _KNN_REGRESSION_H_
#define _KNN_REGRESSION_H_

#include <string>

#include <openipDS/Vector.h>
#include <openipDS/RegressionDataSet.h>
#include <openipML/Regression.h>
#include <openipSC/matrixOperations.h>
#include <openipML/fann/doublefann.h>
#include <openipML/fann/fann_cpp.h>

namespace openip
{
  template<typename T>
  class KNNRegression: public Regression<T>
  {
    public:
      using Regression<T>::probabilities;
      using Regression<T>::preprocessingNames;
      using Regression<T>::featureNames;
      
      KNNRegression(int k);

      KNNRegression(KNNRegression& r);

      ~KNNRegression();

      virtual int train(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL);

      virtual float regress(Vector<T>& c, Vector<float>* probabilities= NULL);

      virtual int saveModelToFile(ofstream& file);

      virtual int openModelFromFile(ifstream& file);
      
      int k;
      /** dataset */
      RegressionDataSet<T> data;
      /** distances */
      Vector<float> dist;
      /** pointer to training mask */
      Vector<unsigned char>* mask;
  };
  
  template<typename T>
  KNNRegression<T>::KNNRegression(int k)
  : Regression<T>()
  {
    this->k= k;
  }
  
  template<typename T>
  KNNRegression<T>::KNNRegression(KNNRegression& s)
  : Regression<T>(s)
  {
    this->k= s.k;
  }
  
  template<typename T>
  KNNRegression<T>::~KNNRegression()
  {
  }
  
  template<typename T>
  int KNNRegression<T>::train(RegressionDataSet<T>& data, Vector<unsigned char>* mask)
  {
    this->featureNames= data.featureNames;
    this->preprocessingNames= data.preprocessingNames;

    this->data= data;
    this->data.init();
    this->mask= mask;
    return 0;
  }

  template<typename T>
  float KNNRegression<T>::regress(Vector<T>& c, Vector<float>* probabilities)
  {
    dist.resize(data.size());

    dist= 0;
    if ( probabilities == NULL )
	probabilities= &(this->probabilities);
    *probabilities= 0;

    float minDist;
    int minIdx;

    #pragma omp parallel for
    for ( unsigned int i= 0; i < data.size(); ++i )
    {
	if ( !mask || (*mask)(i) )
	    dist(i)= data(i).dist2(c);
    }

    Vector<int> indices;
    for ( int i= 0; i < k; ++i )
    {
	minIdx= 0;
	minDist= FLT_MAX;
	for ( unsigned int j= 0; j < dist.size(); ++j )
	{
	    if ( !mask || (*mask)(j) )
	    {
		if ( dist(j) < minDist )
		{
		    minDist= dist(j);
		    minIdx= j;
		}
	    }
	}
	indices.push_back(minIdx);
	dist(minIdx)= FLT_MAX;
    }

    float mean= 0;
    for ( unsigned int i= 0; i < indices.size(); ++i )
      mean+= data(indices(i)).targetValue;
    mean/= indices.size();
    
    return mean;
  }

  template<typename T>
  int KNNRegression<T>::saveModelToFile(ofstream& /*file*/)
  {
    /*Regression<T>::saveModelToFile(file);
    
    file << betaHat.size() << endl;
    for ( int i= 0; i < betaHat.size(); ++i )
      file << betaHat(i) << endl;*/
    
    return 0;
  }

  template<typename T>
  int KNNRegression<T>::openModelFromFile(ifstream& /*file*/)
  {
    /*Regression<T>::openModelFromFile(file);
    
    int n;
    file >> n;
    float b;
    betaHat.resize(n);
    for ( int i= 0; i < n; ++i )
      file >> betaHat(i);*/
    
    return 0;
  }
}

#endif
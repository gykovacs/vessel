#ifndef _UNIVARIATE_LINEAR_REGRESSION_H_
#define _UNIVARIATE_LINEAR_REGRESSION_H_

#include <string>

#include <openipDS/Vector.h>
#include <openipDS/RegressionDataSet.h>
#include <openipML/Regression.h>

namespace openip
{
  template<typename T>
  class UnivariateLinearRegression: public Regression<T>
  {
    public:
      using Regression<T>::probabilities;
      using Regression<T>::preprocessingNames;
      using Regression<T>::featureNames;
      
      UnivariateLinearRegression();

      UnivariateLinearRegression(UnivariateLinearRegression& r);

      ~UnivariateLinearRegression();

      virtual int train(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL);

      virtual float regress(Vector<T>& c, Vector<float>* probabilities= NULL);

      virtual int saveModelToFile(ofstream& file);

      virtual int openModelFromFile(ifstream& file);
      
      float beta;
      float alpha;
      float r;
  };
  
  template<typename T>
  UnivariateLinearRegression<T>::UnivariateLinearRegression()
  : Regression<T>()
  {
  }
  
  template<typename T>
  UnivariateLinearRegression<T>::UnivariateLinearRegression(UnivariateLinearRegression& r)
  : Regression<T>(r)
  {
  }
  
  template<typename T>
  UnivariateLinearRegression<T>::~UnivariateLinearRegression()
  {
  }
  
  template<typename T>
  int UnivariateLinearRegression<T>::train(RegressionDataSet<T>& data, Vector<unsigned char>* mask)
  {
    this->preprocessingNames= data.preprocessingNames;
    this->featureNames= data.featureNames;
    
    data.init();
    
    float ymean= 0, xmean= 0, x2mean= 0, y2mean= 0, covar= 0, varx2= 0, vary2= 0, xymean= 0;
    int n= 0;
    
    for ( unsigned int i= 0; i < data.size(); ++i )
    {
      if ( !mask || (*mask)(i) )
      {
	ymean+= data(i).targetValue;
	xmean+= data(i)(0);
	y2mean+= data(i).targetValue * data(i).targetValue;
	x2mean+= data(i)(0)*data(i)(0);
	xymean+= data(i)(0)*data(i).targetValue;
	++n;
      }
    }
    
    ymean/= n;
    xmean/= n;
    y2mean/= n;
    x2mean/= n;
    xymean/= n;
    
    for ( unsigned int i= 0; i < data.size(); ++i )
    {
      if ( !mask || (*mask)(i) )
      {
	covar+= (data(i)(0) - xmean) * (data(i).targetValue - ymean);
	varx2+= (data(i)(0) - xmean) * (data(i)(0) - xmean);
	vary2+= (data(i)(0) - ymean) * (data(i)(0) - ymean);
      }
    }
    
    beta= covar/varx2;
    alpha= ymean - beta*xmean;
    
    r= (xymean - xmean*ymean)/(sqrt((x2mean - xmean*xmean)*(y2mean - ymean*ymean)));
    
    //cout << "r^2: " << r*r << endl;
    //cout << "parameters: " << beta << " " << alpha << endl;
    
    return 0;
  }

  template<typename T>
  float UnivariateLinearRegression<T>::regress(Vector<T>& c, Vector<float>* /*probabilities*/)
  {
    return c(0)*beta + alpha;
  }

  template<typename T>
  int UnivariateLinearRegression<T>::saveModelToFile(ofstream& file)
  {
    file << "UnivariateLinearRegression" << std::endl;

    Regression<T>::saveModelToFile(file);
    
    file << alpha << std::endl;
    file << beta << std::endl;
    file << r << std::endl;
    
    return 0;
  }

  template<typename T>
  int UnivariateLinearRegression<T>::openModelFromFile(ifstream& file)
  {
    std::string tmp;
    getline(file, tmp);
    if ( tmp.find("UnivariateLinearRegression") == 0 )
    {
      Regression<T>::openModelFromFile(file);
      
      file >> alpha;
      file >> beta;
      file >> r;
      
      return 0;
    }
    
    return 1;
  }
}

#endif
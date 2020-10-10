#ifndef _REGRESSION_H_
#define _REGRESSION_H_

#include <string>

#include <openipDS/Vector.h>
#include <openipDS/RegressionDataSet.h>

namespace openip
{
  template<typename T>
  class Regression
  {
    public:
      Regression();

      Regression(Regression& r);

      virtual ~Regression();

      virtual int train(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL);

      virtual float regress(Vector<T>& c, Vector<float>* probabilities= NULL);
      
      virtual float evaluate(RegressionDataSet<T>& data, int measure= 9);

      virtual int saveModelToFile(const char* filename);

      virtual int openModelFromFile(const char* filename);

      virtual int saveModelToFile(ofstream& file);

      virtual int openModelFromFile(ifstream& file);

      Vector<float> probabilities;
      Vector<std::string> preprocessingNames;
      Vector<std::string> featureNames;
  };
  
  template<typename T>
  Regression<T>::Regression()
  {
  }
  
  template<typename T>
  Regression<T>::Regression(Regression& r)
  {
    probabilities= r.probabilities;
    preprocessingNames= r.preprocessingNames;
    featureNames= r.featureNames;
  }
  
  template<typename T>
  Regression<T>::~Regression()
  {
  }
  
  template<typename T>
  float Regression<T>::evaluate(RegressionDataSet<T>& data, int /*measure*/)
  {
    float tmp, meanSquaredError= 0;
    for ( unsigned int i= 0; i < data.size(); ++i )
    {
      tmp= regress(data(i), NULL);
      meanSquaredError+= (tmp - data(i).targetValue)*(tmp - data(i).targetValue);
    }
    meanSquaredError/= data.size();
    return meanSquaredError;
  }
  
  template<typename T>
  int Regression<T>::train(RegressionDataSet<T>& /*data*/, Vector<unsigned char>* /*mask*/)
  {
    return 0;
  }

  template<typename T>
  float Regression<T>::regress(Vector<T>& /*c*/, Vector<float>* /*probabilities*/)
  {
    return 0.0f;
  }

  template<typename T>
  int Regression<T>::saveModelToFile(const char* filename)
  {
    std::ofstream file;
    file.open(filename);

    saveModelToFile(file);
    
    return 0;
  }

  template<typename T>
  int Regression<T>::openModelFromFile(const char* filename)
  {
    std::ifstream file;
    file.open(filename);

    openModelFromFile(file);
    
    return 0;
  }

  template<typename T>
  int Regression<T>::saveModelToFile(ofstream& file)
  {
    file << featureNames.size() << std::endl;
    for ( unsigned int i= 0; i < featureNames.size(); ++i )
	file << featureNames(i) << std::endl;
    file << preprocessingNames.size() << std::endl;
    for ( unsigned int i= 0; i < preprocessingNames.size(); ++i )
	file << preprocessingNames(i) << std::endl;
    
    return 0;
  }

  template<typename T>
  int Regression<T>::openModelFromFile(ifstream& file)
  {
    std::string tmp;
    int k;
    
    getline(file, tmp);
    sscanf(tmp.c_str(), "%d", &k);
    for ( int i= 0; i < k; ++i )
    {
	getline(file, tmp);
	featureNames.push_back(tmp);
    }

    getline(file, tmp);
    sscanf(tmp.c_str(), "%d", &k);
    for ( int i= 0; i < k; ++i )
    {
	getline(file, tmp);
	preprocessingNames.push_back(tmp);
    }
    
    return 0;
  }
}

#endif
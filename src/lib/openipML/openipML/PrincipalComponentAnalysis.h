#ifndef PrincipalComponentAnalysis_H
#define PrincipalComponentAnalysis_H

#include <iostream>

#include <openipDS/Vector.h>
#include <openipDS/DataSet.h>
#include <openipDS/RegressionDataSet.h>
#include <openipDS/Matrix.h>

#include <openipSC/matrixOperations.h>

namespace openip
{
    template<typename T>
    class PrincipalComponentAnalysis
    {
    public:
        PrincipalComponentAnalysis(int unknown= 28);

        PrincipalComponentAnalysis(const PrincipalComponentAnalysis& p);

        ~PrincipalComponentAnalysis();

        void train(DataSet<T>& d, int output);
	
	void train(RegressionDataSet<T>& d, int output);

        Vector<T> transform(Vector<T>& v);

        FeatureVector<T> transform(FeatureVector<T>& v);
	
	RegressionFeatureVector<T> transform(RegressionFeatureVector<T>& v);

        void transform(Vector<T>& input, Vector<T>& output);

        void transform(DataSet<T>& input, DataSet<T>& output);

        void transform(FeatureVector<T>& input, FeatureVector<T>& output);
	
        void transform(RegressionDataSet<T>& input, RegressionDataSet<T>& output);

        void transform(RegressionFeatureVector<T>& input, RegressionFeatureVector<T>& output);

        int saveToFile(std::ofstream& file);

        int openFromFile(std::ifstream& file);

        int saveToFile(char* filename);

        int openFromFile(char* filename);

        Matrix<T> transformation;
        Vector<T> mean;
        int inputDimensions;
        int outputDimensions;
	
	int unknown;
	int unknownSet;
	
	Vector<T> tmp;
    };

    template<typename T>
    PrincipalComponentAnalysis<T>::PrincipalComponentAnalysis(int unknown)
    {
      this->unknown= unknown;
      if ( unknown == 28 )
	unknownSet= 0;
      else
	unknownSet= 1;
    }

    template<typename T>
    PrincipalComponentAnalysis<T>::PrincipalComponentAnalysis(const PrincipalComponentAnalysis& p)
    {
        this->transform= p.transform;
        this->inputDimensions= p.inputDimensions;
        this->outputDimensions= p.outputDimensions;
	this->mean= p.mean;
	this->unknownSet= p.unknownSet;
	this->unknown= p.unknown;
    }

    template<typename T>
    PrincipalComponentAnalysis<T>::~PrincipalComponentAnalysis()
    {
    }

    template<typename T>
    void PrincipalComponentAnalysis<T>::train(DataSet<T>& d, int output)
    {
        inputDimensions= d(0).size();
        outputDimensions= output < inputDimensions ? output : inputDimensions;

        Vector<T> mean;
        mean.resize(inputDimensions);
	mean= 0;

	if ( !unknownSet )
	{
	  for ( unsigned int i= 0; i < d.size(); ++i )
	      mean+= d(i);
	  mean/= d.size();
	}
	else
	{
	  Vector<int> n;
	  n.resize(inputDimensions);
	  n= 0;
	  for ( unsigned int i= 0; i < d.size(); ++i )
	    for ( int j= 0; j < d.dimension; ++j )
	      if ( d(i)(j) != unknown )
	      {
		n(j)+= 1;
		mean(j)+= d(i)(j);
	      }
	  for ( int j= 0; j < d.dimension; ++j )
	    mean(j)/= n(j);
	  //cout << n << std::endl;
	}

        this->mean= mean;
	//cout << mean << std::endl;

        Matrix<T> covariance;
        covariance.resize(inputDimensions, inputDimensions);
        covariance= 0;

	if ( !unknownSet )
	{
	  Vector<T> tmp;
	  tmp.resize(inputDimensions);
	  for ( unsigned int i= 0; i < d.size(); ++i )
	  {
	      tmp= mean - d(i);
	      covariance+= tmp.columnRowProduct(tmp);
	  }
	  covariance/= (d.size()-1);
	}
	else
	{
	  Vector<T> tmp;
	  tmp.resize(inputDimensions);
	  for ( unsigned int i= 0; i < d.size(); ++i )
	  {
	      tmp= mean - d(i);
	      for ( int j= 0; j < d.dimension; ++j )
		if ( d(i)(j) == unknown )
		  tmp(j)= 0;
	      covariance+= tmp.columnRowProduct(tmp);
	  }
	  covariance/= (d.size()-1);
	}
	//cout << covariance << std::endl;

        Matrix<double> covariance2;
        covariance2= covariance;

        Matrix<double> eigenvectors;
        eigenvectors.resize(inputDimensions, inputDimensions);

        Vector<double> eigenvalues;
        eigenvalues.resize(inputDimensions);

        computeEigenvectorsAndEigenvalues(covariance2.data(), inputDimensions, eigenvectors.data(), eigenvalues.data());

        transformation.resize(inputDimensions, outputDimensions);
        for ( int i= 0; i < output; ++i )
        {
            for ( int j= 0; j < inputDimensions; ++j )
                transformation(j,i)= eigenvectors(j,inputDimensions - i - 1);
        }
        transformation.transpose();
    }

    template<typename T>
    void PrincipalComponentAnalysis<T>::train(RegressionDataSet<T>& d, int output)
    {
        inputDimensions= d(0).size();
        outputDimensions= output < inputDimensions ? output : inputDimensions;

        Vector<T> mean;
        mean.resize(inputDimensions);
	mean= 0;

	if ( !unknownSet )
	{
	  for ( unsigned int i= 0; i < d.size(); ++i )
	      mean+= d(i);
	  mean/= d.size();
	}
	else
	{
	  Vector<int> n;
	  n.resize(inputDimensions);
	  n= 0;
	  for ( unsigned int i= 0; i < d.size(); ++i )
	    for ( int j= 0; j < d.dimension; ++j )
	      if ( d(i)(j) != unknown )
	      {
		n(j)+= 1;
		mean(j)+= d(i)(j);
	      }
	  for ( int j= 0; j < d.dimension; ++j )
	    mean(j)/= n(j);
	  //cout << n << std::endl;
	}

        this->mean= mean;
	//cout << mean << std::endl;

        Matrix<T> covariance;
        covariance.resize(inputDimensions, inputDimensions);
        covariance= 0;

	if ( !unknownSet )
	{
	  Vector<T> tmp;
	  tmp.resize(inputDimensions);
	  for ( unsigned int i= 0; i < d.size(); ++i )
	  {
	      tmp= mean - d(i);
	      covariance+= tmp.columnRowProduct(tmp);
	  }
	  covariance/= (d.size()-1);
	}
	else
	{
	  Vector<T> tmp;
	  tmp.resize(inputDimensions);
	  for ( unsigned int i= 0; i < d.size(); ++i )
	  {
	      tmp= mean - d(i);
	      for ( int j= 0; j < d.dimension; ++j )
		if ( d(i)(j) == unknown )
		  tmp(j)= 0;
	      covariance+= tmp.columnRowProduct(tmp);
	  }
	  covariance/= (d.size()-1);
	}
	//cout << covariance << std::endl;

        Matrix<double> covariance2;
        covariance2= covariance;

        Matrix<double> eigenvectors;
        eigenvectors.resize(inputDimensions, inputDimensions);

        Vector<double> eigenvalues;
        eigenvalues.resize(inputDimensions);

        computeEigenvectorsAndEigenvalues(covariance2.data(), inputDimensions, eigenvectors.data(), eigenvalues.data());

        transformation.resize(inputDimensions, outputDimensions);
        for ( int i= 0; i < output; ++i )
        {
            for ( int j= 0; j < inputDimensions; ++j )
                transformation(j,i)= eigenvectors(j,inputDimensions - i - 1);
        }
        transformation.transpose();
    }
    
    template<typename T>
    void PrincipalComponentAnalysis<T>::transform(Vector<T>& input, Vector<T>& output)
    {
      if ( !unknownSet )
        output= transformation^(input - mean);
      else
      {
	tmp.resize(input.size());
	for ( unsigned int j= 0; j < input.size(); ++j )
	  if ( input(j) == unknown )
	    tmp(j)= mean(j);
	  else
	    tmp(j)= input(j);

	output= transformation^(tmp - mean);
      }
    }

    template<typename T>
    Vector<T> PrincipalComponentAnalysis<T>::transform(Vector<T>& input)
    {
        Vector<T> tmp;
        tmp.resize(outputDimensions);
        transform(input, tmp);
        return tmp;
    }

    template<typename T>
    void PrincipalComponentAnalysis<T>::transform(FeatureVector<T>& input, FeatureVector<T>& output)
    {
        Vector<T> tmp= input;
        tmp= transformation^(tmp - mean);
        output= FeatureVector<T>(input.classIndex, tmp);
    }
    
    template<typename T>
    void PrincipalComponentAnalysis<T>::transform(RegressionFeatureVector<T>& input, RegressionFeatureVector<T>& output)
    {
        Vector<T> tmp= input;
        tmp= transformation^(tmp - mean);
        output= RegressionFeatureVector<T>(input.targetValue, tmp);
    }

    template<typename T>
    FeatureVector<T> PrincipalComponentAnalysis<T>::transform(FeatureVector<T>& input)
    {
        FeatureVector<T> tmp;
        tmp.resize(outputDimensions);
        transform(input, tmp);
        return tmp;
    }
    
    template<typename T>
    RegressionFeatureVector<T> PrincipalComponentAnalysis<T>::transform(RegressionFeatureVector<T>& input)
    {
        RegressionFeatureVector<T> tmp;
        tmp.resize(outputDimensions);
        transform(input, tmp);
	tmp.targetValue= input.targetValue;
        return tmp;
    }

    template<typename T>
    void PrincipalComponentAnalysis<T>::transform(DataSet<T> &input, DataSet<T> &output)
    {
        output.clear();
	output.preprocessingNames= input.preprocessingNames;
	output.featureNames= input.featureNames;
	output.classNames= input.classNames;
        for ( unsigned int i= 0; i < input.size(); ++i )
            output.push_back(transform(input(i)));
    }
    
    template<typename T>
    void PrincipalComponentAnalysis<T>::transform(RegressionDataSet<T> &input, RegressionDataSet<T> &output)
    {
        output.clear();
	output.preprocessingNames= input.preprocessingNames;
	output.featureNames= input.featureNames;
        for ( unsigned int i= 0; i < input.size(); ++i )
            output.push_back(transform(input(i)));
    }

    template<typename T>
    int PrincipalComponentAnalysis<T>::saveToFile(std::ofstream& file)
    {
        file << mean << std::endl;
        file << transformation << std::endl;
	file << unknownSet << std::endl;
	file << unknown << std::endl;
        return 0;
    }

    template<typename T>
    int PrincipalComponentAnalysis<T>::openFromFile(std::ifstream& file)
    {
        file >> mean;
        file >> transformation;
	file >> unknownSet;
	file >> unknown;
        return 0;
    }

    template<typename T>
    int PrincipalComponentAnalysis<T>::saveToFile(char* filename)
    {
        std::ofstream file;
        file.open(filename);
        saveToFile(file);
        file.close();
        return 0;
    }

    template<typename T>
    int PrincipalComponentAnalysis<T>::openFromFile(char* filename)
    {
        std::ifstream file;
        file.open(filename);
        openFromFile(file);
        file.close();
        return 0;
    }
}

#endif // PrincipalComponentAnalysis_H

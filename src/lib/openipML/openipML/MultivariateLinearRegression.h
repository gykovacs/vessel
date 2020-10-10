#ifndef _MULTIVARIATE_LINEAR_REGRESSION_H_
#define _MULTIVARIATE_LINEAR_REGRESSION_H_

#include <string>

#include <openipDS/Vector.h>
#include <openipDS/RegressionDataSet.h>
#include <openipML/Regression.h>
#include <openipSC/matrixOperations.h>
#include <openipML/PrincipalComponentAnalysis.h>

namespace openip
{
  template<typename T>
  class MultivariateLinearRegression: public Regression<T>
  {
    public:
      using Regression<T>::probabilities;
      using Regression<T>::preprocessingNames;
      using Regression<T>::featureNames;
      
      MultivariateLinearRegression(int maxit= 1000000, float tol= 1e-4, int pca= 0);

      MultivariateLinearRegression(MultivariateLinearRegression& r);

      ~MultivariateLinearRegression();

      virtual int train(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL);

      virtual float regress(Vector<T>& c, Vector<float>* probabilities= NULL);
      
      void resampleHistograms(RegressionDataSet<T>& data, RegressionDataSet<T>& output, int outputDimensions);
      
      void resampleHistogram(Vector<T>& data, Vector<T>& output, int outputDimensions);
      
      void printMatrix(char const* name, Matrix<T>& m);
      
      void printVector(char const* name, Vector<T>& m);
      
      virtual int saveModelToFile(ofstream& file);

      virtual int openModelFromFile(ifstream& file);
      
      float beta;
      float alpha;
      float r;
      RegressionDataSet<float> rds;
      
      Vector<float> betaHat;
      int pca;
      PrincipalComponentAnalysis<T> principalComponentAnalysis;
      int maxit;
      float tol;
      int resampleHist;
  };
  
  template<typename T>
  MultivariateLinearRegression<T>::MultivariateLinearRegression(int maxit, float tol, int pca)
  : Regression<T>()
  {
    this->pca= pca;
    this->maxit= maxit;
    this->tol= tol;
  }
  
  template<typename T>
  MultivariateLinearRegression<T>::MultivariateLinearRegression(MultivariateLinearRegression& r)
  : Regression<T>(r)
  {
    this->pca= r.pca;
    this->maxit= maxit;
    this->tol= tol;
  }
  
  template<typename T>
  MultivariateLinearRegression<T>::~MultivariateLinearRegression()
  {
  }
  
  template<typename T>
  void MultivariateLinearRegression<T>::printMatrix(char const* name, Matrix<T>& m)
  {
    printf("%s %d %d\n", name, m.rows, m.columns);
    for ( int i= 0; i < m.rows; ++i )
    {
      for ( int j= 0; j < m.columns; ++j )
	printf("%f ", m(i,j));
      printf("\n");
    }
    printf("\n");
    fflush(stdout);
  }
  
  template<typename T>
  void MultivariateLinearRegression<T>::printVector(char const* name, Vector<T>& m)
  {
    printf("%s %zd\n", name, m.size());
    for ( unsigned int i= 0; i < m.size(); ++i )
      printf("%f ", m(i));
    printf("\n\n");
    fflush(stdout);
  }
  
  template<typename T>
  void MultivariateLinearRegression<T>::resampleHistograms(RegressionDataSet<T>& data, RegressionDataSet<T>& output, int outputDimensions)
  {
    int inputDimensions= data(0).size();
    tprintf("input, output dimensions: %d/%d\n", inputDimensions, outputDimensions);
    float ratio= float(inputDimensions)/outputDimensions;
    
    int whole= int(ratio);
    //float frac= ratio - whole;
    tprintf("sampling width: %d\n", whole);
    
    output.preprocessingNames= data.preprocessingNames;
    for ( int i= 0; i < outputDimensions; ++i )
      output.featureNames.push_back(data.featureNames(i));
    
    for ( unsigned int i= 0; i < data.size(); ++i )
    {
      RegressionFeatureVector<T> rfv;
      rfv.targetValue= data(i).targetValue;
      
      for ( float j= 0; j < outputDimensions; ++j )
      {
	float t= 0;
	for ( int k= 0; k < whole; ++k )
	  t+= data(i)(j*whole + k);
	rfv.push_back(t);
      }
      output.push_back(rfv);
    }
    /*for ( int i= 0; i < output.size(); ++i )
    {
      for ( int j= 0; j < output(i).size(); ++j )
	printf("%f ", output(i)(j));
      printf("\n");
    }*/
  }
  
  template<typename T>
  void MultivariateLinearRegression<T>::resampleHistogram(Vector<T>& data, Vector<T>& output, int outputDimensions)
  {
    int inputDimensions= data.size();
    tprintf("input, output dimensions: %d/%d\n", inputDimensions, outputDimensions);
    float ratio= float(inputDimensions)/outputDimensions;
    
    output.clear();
    
    int whole= int(ratio);
    //float frac= ratio - whole;
    tprintf("sampling width: %d\n", whole);
    
    for ( float j= 0; j < outputDimensions; ++j )
    {
      float t= 0;
      for ( int k= 0; k < whole; ++k )
	t+= data(j*whole + k);
      output.push_back(t);
    }
  }
  
  template<typename T>
  int MultivariateLinearRegression<T>::train(RegressionDataSet<T>& data, Vector<unsigned char>* mask)
  {
    this->featureNames= data.featureNames;
    this->preprocessingNames= data.preprocessingNames;
    
    data.init();
    rds.clear();
    rds.preprocessingNames.clear();
    rds.featureNames.clear();
    
    /*for ( int i= 0; i < data.size(); ++i )
      for ( int j= 0; j < data(i).size(); ++j )
	data(i)(j)/= 1000;*/
    
    if ( pca )
    {
      int n= 0;
      tprintf("applying pca\n");
      RegressionDataSet<T> pcadata;
      for ( unsigned int i= 0; i < data.size(); ++i )
	if ( !mask || (*mask)(i) )
	{
	  pcadata.push_back(data(i));
	  ++n;
	}

      tprintf("number of samples: %d\n", n);
      principalComponentAnalysis.train(pcadata, pca);
      principalComponentAnalysis.transform(pcadata, rds);
      rds.init();
      for ( unsigned int i= 0; i < rds.size(); ++i )
	printVector("rds", rds(i));
    }
    else
    {
      for ( unsigned int i= 0; i < data.size(); ++i )
	if ( !mask || (*mask)(i) )
	{
	  rds.push_back(data(i));
	}
      rds.preprocessingNames= data.preprocessingNames;
      rds.featureNames= data.featureNames;
      rds.init();

      resampleHist= 2;
      RegressionDataSet<float> test;
      resampleHistograms(rds, test, resampleHist);
      rds= test;
      rds.featureNames.push_back(string("offset"));
      for ( unsigned int i= 0; i < rds.size(); ++i )
	rds(i).push_back(1);
      rds.init();
    }
    
    for ( unsigned int i= 0; i < rds.size(); ++i )
    {
      printf("%f ", rds(i).targetValue);
      for ( unsigned int j= 0; j < rds(i).size(); ++j )
	printf("%f ", rds(i)(j));
      printf("\n");
    }
	
    
    int rows= rds.size();;
    int columns= rds(0).size();
    
    tprintf("X dimensions: %d, %d\n", rows, columns);
    Matrix<float> X(rows, columns);
    Vector<float> y(rows);
    
    int n= 0;
    for ( unsigned int i= 0; i < rds.size(); ++i )
      if ( !mask || (*mask)(i) )
      {
	for ( unsigned int j= 0; j < rds(i).size(); ++j )
	  X(n,j)= rds(i)(j);
	y(n)= rds(i).targetValue;
	++n;
      }
    
    /*Vector<float> beta(columns);
    Matrix<float> J(rows, columns);
    Vector<float> deltabeta(columns);
    Vector<float> deltay(rows);
    
    Matrix<float> lefthand(columns, columns);
    Vector<float> righthand(columns);
    Matrix<float> JT;
    
    for ( int i= 0; i < beta.size(); ++i )
      beta(i)= double(rand())/double(RAND_MAX);
      //beta(i)= 1;
    
    int m= 10000;
    for ( int i= 0; i < maxit; ++i )
    {
      for ( int j= 0; j < rows; ++j )
	for ( int k= 0; k < columns; ++k )
	  J(j,k)= X(j,k);

      if ( i % m == 0 )
	printMatrix("J", J);
      
      JT= J;
      JT.transpose();
      lefthand= JT^J;
      deltay= X^beta;
      deltay= y - deltay;
      
      if ( i % m == 0 )
      {
	printMatrix("JT", JT);
	printMatrix("lefthand", lefthand);
	printVector("deltay", deltay);
      }
      
      righthand= JT^deltay;
      
      if ( i % m == 0 )
	printVector("righthand", righthand);
      
      solveLinearEquationSystem(&(lefthand(0,0)), lefthand.rows, lefthand.columns,  &(righthand(0)), &(deltabeta(0)));

      if ( i % m == 0 )
	printVector("deltabeta", deltabeta);
      
      beta+= deltabeta*0.1;

      if ( i % m == 0 )
      {
	printVector("beta", beta);
	printVector("y", y);
      }
      
      deltay= X^beta;
      if ( i % m == 0 )
	printVector("y_hat", deltay);
            
      deltay= y - deltay;
      
      if ( i % m == 0 )
	printVector("deltay", deltay);
      
      float r2= 0;
      for ( int j= 0; j < deltay.size(); ++j )
	r2+= deltay(j)*deltay(j);

      if ( i % m == 0 )
	printf("iteration: %d, r^2: %f\n", i, r2); fflush(stdout);
    
      float maxdeltabetaperbeta= 0;
      for ( int j= 0; j < deltabeta.size(); ++j )
	if ( beta(j) != 0 )
	  if ( fabs(deltabeta(j)/beta(j)) > maxdeltabetaperbeta)
	    maxdeltabetaperbeta= fabs(deltabeta(j)/beta(j));
      
      if ( maxdeltabetaperbeta < tol )
	break;
    }*/
    
    Matrix<float> XT;
    XT= X;
    XT.transpose();
    Matrix<float> XTX;
    XTX= XT^X;
    Matrix<float> XTXinv(XTX.rows, XTX.columns);
    invertSquareMatrix(&(XTX(0,0)), XTX.rows, &(XTXinv(0,0)));
    Matrix<float> P;
    P= XTXinv^XT;
    Vector<float> b= P^y;
    
    printVector("b^hat_1", b);
    //printVector("b^hat_2", beta);
    //betaHat= beta;
    betaHat= b;
//    getchar();
    
    return 0;
  }

  template<typename T>
  float MultivariateLinearRegression<T>::regress(Vector<T>& c, Vector<float>* /*probabilities*/)
  {
    if ( pca )
    {
      Vector<T> v(pca);
      principalComponentAnalysis.transform(c, v);
      printVector("pca.mean", principalComponentAnalysis.mean);
      //printMatrix("pca.transformation", principalComponentAnalysis.transformation);
      printVector("c", c);
      printVector("v", v);
      printVector("betaHat", betaHat);
      
      float t= 0;
      for ( unsigned int i= 0; i < betaHat.size(); ++i )
	t+= betaHat(i)*v(i);
      printf("%f\n", t);
      return t;
    }
    float t= 0;
    Vector<T> output;
    resampleHistogram(c, output, resampleHist);
    output.push_back(1);
    for ( unsigned int i= 0; i < betaHat.size(); ++i )
      t+= betaHat(i)*output(i);
    printf("%f\n", t);
    return t;
  }

  template<typename T>
  int MultivariateLinearRegression<T>::saveModelToFile(ofstream& file)
  {
    Regression<T>::saveModelToFile(file);
    
    file << betaHat.size() << endl;
    for ( unsigned int i= 0; i < betaHat.size(); ++i )
      file << betaHat(i) << endl;
    
    return 0;
  }

  template<typename T>
  int MultivariateLinearRegression<T>::openModelFromFile(ifstream& file)
  {
    Regression<T>::openModelFromFile(file);
    
    int n;
    file >> n;
    //float b;
    betaHat.resize(n);
    for ( int i= 0; i < n; ++i )
      file >> betaHat(i);
    
    return 0;
  }
}

#endif
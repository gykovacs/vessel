#ifndef _PTV_MULTIVARIATE_REGRESSION_H_
#define _PTV_MULTIVARIATE_REGRESSION_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <openipDS/Vector.h>
#include <openipDS/RegressionDataSet.h>
#include <openipML/Regression.h>
#include <openipSC/matrixOperations.h>
#include <openipML/PrincipalComponentAnalysis.h>
#include <openipML/UnivariateLinearRegression.h>
#include <openipML/KFoldCrossValidator.h>

namespace openip
{
  template<typename T>
  class PenalizedTotalVariationMultivariateRegression: public Regression<T>
  {
    public:
      using Regression<T>::probabilities;
      using Regression<T>::preprocessingNames;
      using Regression<T>::featureNames;
      
      PenalizedTotalVariationMultivariateRegression(float beta= 1, int maxit= 1000000, float tol= 1e-4);

      PenalizedTotalVariationMultivariateRegression(PenalizedTotalVariationMultivariateRegression& r);

      ~PenalizedTotalVariationMultivariateRegression();

      virtual int train(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL);

      virtual float regress(Vector<T>& c, Vector<float>* probabilities= NULL);
      
      float suboptimize(RegressionDataSet<T>& data, float currentObjective, KFoldCrossValidator<float>& kfcv, RegressionDataSet<T>& tmpdataset, int iterations, UnivariateLinearRegression<float>& ulr);
      
      void printMatrix(char* name, Matrix<double>& m);
      
      void printVector(char* name, Vector<double>& m);
      
      virtual int saveModelToFile(ofstream& file);

      virtual int openModelFromFile(ifstream& file);
      
      RegressionDataSet<float> rds;
      Vector<double> parameters;
      
      int maxit;
      float tol;
      float a, b;
      int lower, upper;
      float beta;
  };
  
  template<typename T>
  PenalizedTotalVariationMultivariateRegression<T>::PenalizedTotalVariationMultivariateRegression(float beta, int maxit, float tol)
  : Regression<T>()
  {
    this->maxit= maxit;
    this->tol= tol;
    this->beta= beta;
    
    parameters.resize(8);
    parameters= 0;
  }
  
  template<typename T>
  PenalizedTotalVariationMultivariateRegression<T>::PenalizedTotalVariationMultivariateRegression(PenalizedTotalVariationMultivariateRegression& r)
  : Regression<T>(r)
  {
    this->maxit= r.maxit;
    this->tol= r.tol;
    this->parameters= r.parameters;
    this->beta= r.beta;
  }
  
  template<typename T>
  PenalizedTotalVariationMultivariateRegression<T>::~PenalizedTotalVariationMultivariateRegression()
  {
  }
  
  template<typename T>
  void PenalizedTotalVariationMultivariateRegression<T>::printMatrix(char* name, Matrix<double>& m)
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
  void PenalizedTotalVariationMultivariateRegression<T>::printVector(char* name, Vector<double>& m)
  {
    printf("%s %d\n", name, m.size());
    for ( int i= 0; i < m.size(); ++i )
      printf("%f ", m(i));
    printf("\n\n");
    fflush(stdout);
  }
  
  template<typename T>
  float PenalizedTotalVariationMultivariateRegression<T>::suboptimize(RegressionDataSet<T>& data, float currentObjective, KFoldCrossValidator<float>& kfcv, RegressionDataSet<T>& tmpdataset, int iterations, UnivariateLinearRegression<float>& ulr)
  {
    int coord;
    float lastValue;
    int first;
    int last;
    float newObjective;
    float lastObjective= -1;
    float value;
    float tv, maxtv, sumtv;
    float besta= 0, bestb= 0;
    //float newValue;
    currentObjective= FLT_MAX;
    
    first= floor(parameters(parameters.size()-2)+0.5);
    last= floor(parameters(parameters.size()-1)+0.5);
    
    int n= 0;
    
    for ( int j= 0; j < iterations; ++j )
    {
      lastObjective= currentObjective;
    for ( int i= first; i <= last; ++i )
    {
      coord= i;
      for ( int k= 0; k < 10; ++k )
      {
	lastValue= parameters(coord);
	//newValue= 0;
	
	// computing optimal value for coord
	/*{
	  float sumxk= 0;
	  float sumxk2= 0;
	  float sumxksquare= 0;
	  float ax= 0;
	  float axx= 0;
	  
	  for ( int i= 0; i < data.size(); ++i )
	  {
	    sumxk+= data(i)(k);
	    sumxk2+= data(i)(k);
	    
	    
	  }
	  sumxksquare= sumxk2*sumxk2;
	  
	  
	}*/
	
	if ( coord == first )
	{
	  n= 0;
	  do
	  {
	    parameters(coord)= parameters(coord+1)*double(rand())/double(RAND_MAX);
	    ++n;
	    if ( n == 10000 )
	      break;
	  } while ( parameters(coord) > parameters(coord+1) );
	  if ( n == 10000 )
	  {
	    parameters(coord)= lastValue;
	    continue;
	  }
	}
	if ( coord == last )
	  parameters(coord)= 1.0;
	if ( coord > first && coord < last )
	{
	  n= 0;
	  do
	  {
	    parameters(coord)= parameters(coord-1) + (parameters(coord+1)-parameters(coord-1))*double(rand())/double(RAND_MAX);
	    ++n;
	    if ( n == 10000 )
	      break;
	  } while ( parameters(coord) < parameters(coord-1) || parameters(coord) > parameters(coord+1) );
	  if ( n == 10000 )
	  {
	    parameters(coord)= lastValue;
	    continue;
	  }
	}
	
	newObjective= 0;
      
	// computing the new objective
	for ( unsigned int m= 0; m < data.size(); ++m )
	{
	  value= 0;
	  for ( int n= first; n <= last; ++n )
	    value+= data(m)(n)*parameters(n);

	  tmpdataset(m)(0)= value;
	  tmpdataset(m).targetValue= data(m).targetValue;
	}
	
	newObjective= kfcv.validate(&ulr, tmpdataset);
	a= ulr.beta;
	b= ulr.alpha;
	
	tv= 0;
	maxtv= 0;
	sumtv= 0;
	for ( int m= first; m < last; ++m )
	{
	  tv= (parameters(m)-parameters(m+1))*(parameters(m)-parameters(m+1));
	  if ( tv > maxtv )
	    maxtv= tv;
	  sumtv+= tv;
	}
	sumtv/=(last - first);
	
	float min= FLT_MAX;
	float max= 0;
	for ( int l= first; l <= last; ++l )
	{
	  if ( parameters(l) < min )
	    min= parameters(l);
	  if ( parameters(l) > max )
	    max= parameters(l);
	}
	maxtv/=(max - min)*(max - min);
	sumtv/=(max - min)*(max - min);
	newObjective+= beta*maxtv;
	
	if ( j % 100 == 0 && i == first )
	{
	  /*printf("%d %f %f %f %f\n", coord, lastValue, parameters(coord), currentObjective, newObjective);
	  getchar();*/
	}
	
	if ( newObjective < currentObjective )
	{
	  currentObjective= newObjective;
	  besta= a;
	  bestb= b;
	}
	else
	{
	  parameters(coord)= lastValue;
	}
      }
    }
    if ( lastObjective <= currentObjective )
      break;
    }
    
    a= besta;
    b= bestb;
    return currentObjective;
  }
  
  template<typename T>
  int PenalizedTotalVariationMultivariateRegression<T>::train(RegressionDataSet<T>& data, Vector<unsigned char>* mask)
  {
    tprintf("tolerance: %.20f\n", tol);
    this->featureNames= data.featureNames;
    this->preprocessingNames= data.preprocessingNames;
    data.init();
    
    // initial variables
    float bestobj= FLT_MAX;
    Vector<float> bestparameters;
    Vector<float> oldparameters;
    RegressionDataSet<float> tmpdataset;
    UnivariateLinearRegression<float> ulr;
    float temperature= 1;
    float annealing= 0.9;
    float objective= FLT_MAX;
    float newObjective= -1;
    float bestObjective= FLT_MAX;
    float olda, oldb;
    float besta= 0, bestb= 0;
    KFoldCrossValidator<float> kfcv(1, Validator<float>::VALIDATOR_MSE);
    RegressionFeatureVector<float> rfv0;
    
    // initialization of variables
    a= 1; b= 1;
    lower= 0;
    upper= data(0).size()-1;
    temperature= 1;
    annealing= 0.9;
    objective= FLT_MAX;
    newObjective= -1;
    
    parameters.resize(data(0).size()+2);
    for ( unsigned int i= 0; i < parameters.size()-2; ++i )
      parameters(i)= float(i)/float(parameters.size()-2);
    parameters(parameters.size()-2)= 0;
    parameters(parameters.size()-1)= data(0).size()-1;
    
    bestparameters= parameters;
    
    // filtering input data for training
    rds.clear();
    rds.preprocessingNames.clear();
    rds.featureNames.clear();
    
    for ( unsigned int i= 0; i < data.size(); ++i )
      if ( !mask || (*mask)(i) )
	rds.push_back(data(i));
      else if ( mask && !(*mask)(i) )
	rfv0= data(i);

    rds.preprocessingNames= data.preprocessingNames;
    rds.featureNames= data.featureNames;
    rds.init();
    tprintf("size: %d %d\n", rds.size(), rds(0).size());

    // initializing tmpdataset
    tmpdataset.featureNames.push_back(string("value"));
    for ( unsigned int i= 0; i < rds.size(); ++i )
    {
      RegressionFeatureVector<float> rfv;
      rfv.push_back(0);
      tmpdataset.push_back(rfv);
    }
    
    int i;
    bestparameters= parameters;
    
    // starting main iterations
    for ( int k= 0; k < 10; ++k )
    {
      temperature= 1;
      parameters= bestparameters;
      oldparameters= bestparameters;
      objective= FLT_MAX;
      
      for ( i= 0; i < maxit && temperature > tol; ++i )
      {
	oldparameters= parameters;
	olda= a;
	oldb= b;
	
	int first= int(floor(parameters(parameters.size()-2)+0.5));
	int last= int(floor(parameters(parameters.size()-1)+0.5));
	
	int coord= rand()%2 ? parameters.size()-2 : parameters.size()-1;
	int step0= rand()%10;
	int step= rand()%2 ? step0 : -step0;
	
	if ( coord == int(parameters.size())-2 )
	{
	  int before= parameters(parameters.size()-2);
	  
	  parameters(parameters.size()-2)+= step;
	  if ( parameters(parameters.size()-2) < 0 )
	    parameters(parameters.size()-2)= 0;
	  if ( parameters(parameters.size()-2) > parameters(parameters.size()-1) )
	    parameters(parameters.size()-2)= parameters(parameters.size()-1)-10;
	  if ( parameters(parameters.size()-2) < 0 )
	    parameters(parameters.size()-2)= 0;
	  int current= parameters(parameters.size()-2);
	  
	  if ( current < before )
	  {
	    for ( int i= before-1; i >= 0; --i )
	      parameters(i)= parameters(before)/float(before)*(i);
	  }
	  //printf("%d %d\n", before, current);
	}
	else if ( coord == int(parameters.size())-1 )
	{
	  int before= parameters(parameters.size()-1);
	  
	  parameters(parameters.size()-1)+= step;
	  if ( parameters(parameters.size()-1) > data(0).size()-1 )
	    parameters(parameters.size()-1)= data(0).size()-1;
	  if ( parameters(parameters.size()-1) < parameters(parameters.size()-2) )
	    parameters(parameters.size()-1)= parameters(parameters.size()-2)+10;
	  if ( parameters(parameters.size()-1) > data(0).size()-1 )
	    parameters(parameters.size()-1)= data(0).size()-1;
	  int current= parameters(parameters.size()-1);
	  
	  if ( current > before )
	  {
	    for ( int i= before; i < int(parameters.size())-2; ++i )
	      parameters(i)= parameters(before-1) + (1 - parameters(before-1))/float(parameters.size()-2 - before+1)*(i-before+1);
	  }
	  parameters(current)= 1.0;
	}
	
	first= int(floor(parameters(parameters.size()-2)+0.5));
	last= int(floor(parameters(parameters.size()-1)+0.5));
	
	newObjective= suboptimize(rds, objective, kfcv, tmpdataset, 200, ulr);
	
	if ( i % ( maxit/10) == 0 )
	{
	  tprintf("%f %f %f %f %d %d %d %d\n", bestobj, objective, newObjective, temperature, i, k, first, last);
	  for ( int i= first; i <= last; ++i )
	    printf("%f ", parameters(i));
	  printf("\n");
	}
	
	if ( newObjective < bestobj )
	{
	  bestparameters= parameters;
	  bestobj= objective;
	  besta= a;
	  bestb= b;
	}
	if ( newObjective < objective )
	{
	  temperature*= annealing;
	  objective= newObjective;
	}
	else if ( newObjective >= objective )
	{
	  float r= double(rand())/double(RAND_MAX);
	  if ( r > exp(-temperature/(newObjective - objective)) )
	    objective= newObjective;
	  else
	  {
	    parameters= oldparameters;
	    a= olda;
	    b= oldb;
	  }
	}
	else
	{
	  parameters= oldparameters;
	  a= olda;
	  b= oldb;
	}
      }
    }
    
    printf("objective: %f, bestObjective: %f, temperature: %f, iteration: %d\n", objective, bestObjective, temperature, i);
    cout << "best parameters " << bestparameters << endl;
    
    parameters= bestparameters;
    a= besta;
    b= bestb;
    float tmp0= this->regress(rfv0);
    cout << "mse: " << (tmp0 - rfv0.targetValue)*(tmp0 - rfv0.targetValue) << endl;
    cout << "best parameters: " << a << " " << b << endl;
    
    /*UnivariateLinearRegression<float> ulr2;
    RegressionDataSet<float> tmpdataset2;
    tmpdataset2.featureNames.push_back(string("value"));*/
    
    /*int first= int(floor(parameters(parameters.size()-2)+0.5));
    int last= int(floor(parameters(parameters.size()-1)+0.5));
    
    for ( int i= 0; i < rds.size(); ++i )
    {
      float value= 0;
      for ( int j= first; j <= last; ++j )
      {
	value+= rds(i)(j)*parameters(j);
      }
      RegressionFeatureVector<float> rfv;
      rfv.push_back(value);
      rfv.targetValue= rds(i).targetValue;
      tmpdataset2.push_back(rfv);
    }
    ulr2.train(tmpdataset2);
    a= ulr2.beta;
    b= ulr2.alpha;*/
    
    /*printf("best objective: %f\n", bestobj);
    printf("parameters: %f %f\n", a, b);*/
    /*for ( int i= 0; i < 8; ++i )
      printf("%f ", parameters(i));
    printf("\n");*/
    /*printf("weights: \n");
    for ( int i= 0; i < 120; ++i )
      printf("%f ", parameters(i));
    printf("\n");
    
    cerr << parameters << endl;*/
    
    return 0;
  }

  template<typename T>
  float PenalizedTotalVariationMultivariateRegression<T>::regress(Vector<T>& v, Vector<float>* /*probabilities*/)
  {
    double w= 0;
    //double tmp;
    
    int first= int(floor(parameters(parameters.size()-2)+0.5));
    int last= int(floor(parameters(parameters.size()-1)+0.5));
    
    for ( int i= first; i <= last; ++i )
    {
      w+= v(i)*parameters(i);
    }
    
    w*= a;
    w+= b;
    
    return w;
  }

  template<typename T>
  int PenalizedTotalVariationMultivariateRegression<T>::saveModelToFile(ofstream& file)
  {
    file << "SigmoidWeightedMultivariateRegression" << endl;
    
    Regression<T>::saveModelToFile(file);
    
    file << parameters.size() << endl;
    for ( unsigned int i= 0; i < parameters.size(); ++i )
      file << parameters(i) << endl;
    
    return 0;
  }

  template<typename T>
  int PenalizedTotalVariationMultivariateRegression<T>::openModelFromFile(ifstream& file)
  {
    string tmp;
    getline(file, tmp);
    
    if ( tmp.find_first_of(string("SigmoidWeightedMultivariateRegression")) == 0 )
    {
      Regression<T>::openModelFromFile(file);
      
      int n;
      file >> n;
      parameters.resize(n);
      
      for ( int i= 0; i < n; ++i )
	file >> parameters(i);
      
      return 0;
    }
    
    return 1;
  }
}

#endif

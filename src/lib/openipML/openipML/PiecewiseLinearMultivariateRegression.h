#ifndef _PL_MULTIVARIATE_REGRESSION_H_
#define _PL_MULTIVARIATE_REGRESSION_H_

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
  class PiecewiseLinearMultivariateRegression: public Regression<T>
  {
    public:
      using Regression<T>::probabilities;
      using Regression<T>::preprocessingNames;
      using Regression<T>::featureNames;
      
      PiecewiseLinearMultivariateRegression(float beta= 1, int maxit= 1000000, float tol= 1e-4);

      PiecewiseLinearMultivariateRegression(PiecewiseLinearMultivariateRegression& r);

      ~PiecewiseLinearMultivariateRegression();

      virtual int train(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL);

      virtual float regress(Vector<T>& c, Vector<float>* probabilities= NULL);
      
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
  PiecewiseLinearMultivariateRegression<T>::PiecewiseLinearMultivariateRegression(float beta, int maxit, float tol)
  : Regression<T>()
  {
    this->maxit= maxit;
    this->tol= tol;
    this->beta= beta;
    
    parameters.resize(8);
    parameters= 0;
  }
  
  template<typename T>
  PiecewiseLinearMultivariateRegression<T>::PiecewiseLinearMultivariateRegression(PiecewiseLinearMultivariateRegression& r)
  : Regression<T>(r)
  {
    this->maxit= r.maxit;
    this->tol= r.tol;
    this->parameters= r.parameters;
    this->beta= r.beta;
  }
  
  template<typename T>
  PiecewiseLinearMultivariateRegression<T>::~PiecewiseLinearMultivariateRegression()
  {
  }
  
  template<typename T>
  void PiecewiseLinearMultivariateRegression<T>::printMatrix(char* name, Matrix<double>& m)
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
  void PiecewiseLinearMultivariateRegression<T>::printVector(char* name, Vector<double>& m)
  {
    printf("%s %d\n", name, m.size());
    for ( int i= 0; i < m.size(); ++i )
      printf("%f ", m(i));
    printf("\n\n");
    fflush(stdout);
  }
  
  template<typename T>
  int PiecewiseLinearMultivariateRegression<T>::train(RegressionDataSet<T>& data, Vector<unsigned char>* mask)
  {
    tprintf("tolerance: %.20f\n", tol);
    this->featureNames= data.featureNames;
    this->preprocessingNames= data.preprocessingNames;
    
    float bestobj= FLT_MAX;;
    Vector<float> bestparameters;
    
    float temperature= 1;
    float annealing= 0.999;
    float objective= FLT_MAX;
    float newObjective= -1;
    //float beta= 1000000;
    float olda, oldb;
    float besta, bestb;
    a= 1; b= 1;
    lower= 0;
    upper= data(0).size()-1;
    
    KFoldCrossValidator<float> kfcv(1, Validator<float>::VALIDATOR_MSE);
    parameters.resize(data(0).size()+2);
    
    for ( int i= 0; i < parameters.size()-2; ++i )
      parameters(i)= float(i)/float(parameters.size()-2);
    parameters(parameters.size()-2)= 0;
    parameters(parameters.size()-1)= data(0).size()-1;
    
    bestparameters= parameters;
    
    RegressionFeatureVector<float> rfv0;
    
    data.init();
    rds.clear();
    rds.preprocessingNames.clear();
    rds.featureNames.clear();
    
    for ( int i= 0; i < data.size(); ++i )
      if ( !mask || (*mask)(i) )
	rds.push_back(data(i));
      else if ( mask && !(*mask)(i) )
	rfv0= data(i);

    rds.preprocessingNames= data.preprocessingNames;
    rds.featureNames= data.featureNames;
    rds.init();

    temperature= 1;
    annealing= 0.9999;
    objective= FLT_MAX;
    newObjective= -1;
    Vector<float> old_parameters;
    
    RegressionDataSet<float> tmpdataset;
    UnivariateLinearRegression<float> ulr;
    tmpdataset.featureNames.push_back(string("value"));
    for ( int i= 0; i < rds.size(); ++i )
    {
      RegressionFeatureVector<float> rfv;
      rfv.push_back(0);
      tmpdataset.push_back(rfv);
    }
    
    int i;
    float bestobjtmp= 0;
    
    tprintf("size: %d %d\n", rds.size(), rds(0).size());
    
    
    
    for ( i= 0; i < maxit && temperature > tol; ++i )
    {
      old_parameters= parameters;
      olda= a;
      oldb= b;
      int coord;
      int sampling= 2;
      
      if ( i % (maxit/10) == 0 )
      {
	sampling= parameters.size()-2;
	old_parameters= bestparameters;
	parameters= bestparameters;
	temperature= temperature*4;
      }
      
      for ( int j= 0; j < sampling; ++j )
      {
      int first= int(floor(parameters(parameters.size()-2)+0.5));
      int last= int(floor(parameters(parameters.size()-1)+0.5));
      
	if ( j == 0 )
	  coord= rand()%(parameters.size());
	else if ( coord == parameters.size()-2 )
	  coord= first;
	else if ( coord == parameters.size()-1 )
	  last;
	else if ( coord == first )
	  coord++;
	else if ( coord == last )
	  coord--;
	else if ( rand()%2 )
	  coord++;
	else
	  coord--;
      
      /*for ( int j= 0; j < parameters.size(); ++j )
	printf("%f ", parameters(j));
      printf("\n");
      printf("%d\n", coord);
      fflush(stdout);*/
      
      if ( coord == first )
      {
	do
	{
	  parameters(coord)= double(rand())/double(RAND_MAX)*parameters(first+1);
	} while ( parameters(coord) > parameters(first+1) );
      }
      else if ( coord == last )
      {
	do
	{
	  parameters(coord)= parameters(last-1) + double(rand())/double(RAND_MAX)*(1-parameters(last));
	} while ( parameters(coord) < parameters(last-1) );
      }
      else if ( coord < parameters.size()-2 && coord >= first && coord <= last )
      {
	do
	{
	  parameters(coord)= parameters(coord-1)+double(rand())/double(RAND_MAX)*(parameters(coord+1)-parameters(coord-1));
	} while (parameters(coord) < parameters(coord-1) || parameters(coord) > parameters(coord+1) );
      }
      else if ( coord == parameters.size()-2 )
      {
	int step0= rand()%10;
	//int step0= 1;
	int before= parameters(parameters.size()-2);
	int step= rand()%2 ? -step0 : +step0;
	
	parameters(parameters.size()-2)+= step;
	if ( parameters(parameters.size()-2) < 0 )
	  parameters(parameters.size()-2)= 0;
	if ( parameters(parameters.size()-2) > parameters(parameters.size()-1) )
	  parameters(parameters.size()-2)= parameters(parameters.size()-1)-1;
	if ( parameters(parameters.size()-2) < 0 )
	  parameters(parameters.size()-2)= 0;
	int current= parameters(parameters.size()-2);
	
	if ( current < before )
	{
	  for ( int i= before-1; i >= 0; --i )
	    parameters(i)= parameters(before)/float(before)*(i);
	    /*do
	    {
	      
	      parameters(coord)= double(rand())/double(RAND_MAX);
	    } while ( parameters(coord) > parameters(coord+1) );*/
	}
	
	//coord= int(floor(parameters(parameters.size()-2)+0.5));
	
      }
      else if ( coord == parameters.size()-1 )
      {
	int step0= rand()%10;
	//int step0= 1;
	int step= rand()%2 ? -step0 : +step0;
	int before= parameters(parameters.size()-1);
	parameters(parameters.size()-1)+= step;
	if ( parameters(parameters.size()-1) > data(0).size()-1 )
	  parameters(parameters.size()-1)= data(0).size()-1;
	if ( parameters(parameters.size()-1) < parameters(parameters.size()-2) )
	  parameters(parameters.size()-1)= parameters(parameters.size()-2)+1;
	if ( parameters(parameters.size()-1) > data(0).size()-1 )
	  parameters(parameters.size()-1)= data(0).size()-1;
	int current= parameters(parameters.size()-1);
	
	if ( current > before )
	{
	  for ( int i= before+1; i < parameters.size()-2; ++i )
	    parameters(i)= parameters(before) + (1 - parameters(before))/float(parameters.size()-2 - before)*(i-before);
	}
	/*coord= int(floor(parameters(parameters.size()-1)+0.5));
	do
	{
	  parameters(coord)= double(rand())/double(RAND_MAX);
	} while ( parameters(coord) < parameters(coord-1) );*/
      }
      else
	continue;
      }
      
      int first= int(floor(parameters(parameters.size()-2)+0.5));
      int last= int(floor(parameters(parameters.size()-1)+0.5));
      
      newObjective= 0;
      float tmp;
      float newobjtmp= 0;
      // computing the new objective
      {
	for ( int i= 0; i < rds.size(); ++i )
	{
	  float value= 0;
	  for ( int j= first; j <= last; ++j )
	    value+= rds(i)(j)*parameters(j);

	  tmpdataset(i)(0)= value;
	  tmpdataset(i).targetValue= rds(i).targetValue;
	}
	
	newObjective= kfcv.validate(&ulr, tmpdataset);
	newobjtmp= newObjective;
	a= ulr.beta;
	b= ulr.alpha;
	
	float tv= 0;
	float maxtv= 0;
	float sumtv= 0;
	for ( int j= first; j < last; ++j )
	{
	  tv= (parameters(j)-parameters(j+1))*(parameters(j)-parameters(j+1));
	  if ( tv > maxtv )
	    maxtv= tv;
	  sumtv+= tv;
	}
	sumtv/(last - first);
	//newObjective+= beta*sqrt(maxtv);
	newObjective+= beta*sumtv;
      }
      
      if ( i % 10000 == 0 )
      {
      printf("%f %f %f %f %f %d\n", bestobjtmp, bestobj, objective, newObjective, temperature, i);
      /*for ( int j= 0; j < bestparameters.size(); ++j )
	printf("%f ", bestparameters(j));
      printf("\n");*/
      }
      //getchar();
      
      if ( objective < bestobj )
      {
	bestparameters= parameters;
	bestobj= objective;
	bestobjtmp= newobjtmp;
	besta= a;
	bestb= b;
      }
      
      if ( newObjective < objective )
      {
	temperature*= annealing;
	objective= newObjective;
	//printf("%d ", coord); 
      }
      else if ( newObjective > objective )
      {
	float r= double(rand())/double(RAND_MAX);
	if ( r > exp(-temperature/(newObjective - objective)) )
	{
	  objective= newObjective;
	}
	else
	{
	  parameters= old_parameters;
	  a= olda;
	  b= oldb;
	}
      }
      else
      {
	parameters= old_parameters;
	a= olda;
	b= oldb;
      }
      
    }
    printf("objective: %f, temperature: %f, iteration: %d\n", objective, temperature, i);
    cout << "best objective " << bestobj << endl;
    cout << "best parameters " << bestparameters << endl;
    this->parameters= bestparameters;
    a= besta;
    b= bestb;
    float tmp0= this->regress(rfv0);
    cout << "mse: " << (tmp0 - rfv0.targetValue)*(tmp0 - rfv0.targetValue) << endl;
    
    
    parameters= bestparameters;
    a= besta;
    b= bestb;
    
    UnivariateLinearRegression<float> ulr2;
    RegressionDataSet<float> tmpdataset2;
    tmpdataset2.featureNames.push_back(string("value"));
    
    int first= int(floor(parameters(parameters.size()-2)+0.5));
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
    b= ulr2.alpha;
    
    printf("best objective: %f\n", bestobj);
    printf("parameters: %f %f\n", a, b);
    /*for ( int i= 0; i < 8; ++i )
      printf("%f ", parameters(i));
    printf("\n");*/
    printf("weights: \n");
    for ( int i= 0; i < 120; ++i )
      printf("%f ", parameters(i));
    printf("\n");
    
    cerr << parameters << endl;
    
    return 0;
  }

  template<typename T>
  float PiecewiseLinearMultivariateRegression<T>::regress(Vector<T>& v, Vector<float>* probabilities)
  {
    double w= 0;
    double tmp;
    
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
  int PiecewiseLinearMultivariateRegression<T>::saveModelToFile(ofstream& file)
  {
    file << "SigmoidWeightedMultivariateRegression" << endl;
    
    Regression<T>::saveModelToFile(file);
    
    file << parameters.size() << endl;
    for ( int i= 0; i < parameters.size(); ++i )
      file << parameters(i) << endl;
    
    return 0;
  }

  template<typename T>
  int PiecewiseLinearMultivariateRegression<T>::openModelFromFile(ifstream& file)
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

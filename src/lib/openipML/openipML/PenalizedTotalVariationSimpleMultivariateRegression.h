#ifndef _PTVS_MULTIVARIATE_REGRESSION_H_
#define _PTVS_MULTIVARIATE_REGRESSION_H_

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
  class PenalizedTotalVariationSimpleMultivariateRegression: public Regression<T>
  {
    public:
      using Regression<T>::probabilities;
      using Regression<T>::preprocessingNames;
      using Regression<T>::featureNames;
      
      PenalizedTotalVariationSimpleMultivariateRegression(float beta= 1, int maxit= 1000000, float tol= 1e-4);

      PenalizedTotalVariationSimpleMultivariateRegression(PenalizedTotalVariationSimpleMultivariateRegression& r);

      ~PenalizedTotalVariationSimpleMultivariateRegression();

      virtual int train(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL);

      virtual float regress(Vector<T>& c, Vector<float>* probabilities= NULL);
      
      float gradient(int k, RegressionDataSet<T>& data);
      
      float suboptimize(RegressionDataSet<T>& data, float currentObjective, KFoldCrossValidator<float>& kfcv, RegressionDataSet<T>& tmpdataset, int iterations, UnivariateLinearRegression<float>& ulr, int& subiterations);
      
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
      
      int gradientBaseComputed;
      Vector<float> gradientBase;
  };
  
  template<typename T>
  PenalizedTotalVariationSimpleMultivariateRegression<T>::PenalizedTotalVariationSimpleMultivariateRegression(float beta, int maxit, float tol)
  : Regression<T>()
  {
    this->maxit= maxit;
    this->tol= tol;
    this->beta= beta;
    
    parameters.resize(8);
    parameters= 0;
  }
  
  template<typename T>
  PenalizedTotalVariationSimpleMultivariateRegression<T>::PenalizedTotalVariationSimpleMultivariateRegression(PenalizedTotalVariationSimpleMultivariateRegression& r)
  : Regression<T>(r)
  {
    this->maxit= r.maxit;
    this->tol= r.tol;
    this->parameters= r.parameters;
    this->beta= r.beta;
  }
  
  template<typename T>
  PenalizedTotalVariationSimpleMultivariateRegression<T>::~PenalizedTotalVariationSimpleMultivariateRegression()
  {
  }
  
  template<typename T>
  void PenalizedTotalVariationSimpleMultivariateRegression<T>::printMatrix(char* name, Matrix<double>& m)
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
  void PenalizedTotalVariationSimpleMultivariateRegression<T>::printVector(char* name, Vector<double>& m)
  {
    printf("%s %d\n", name, m.size());
    for ( int i= 0; i < m.size(); ++i )
      printf("%f ", m(i));
    printf("\n\n");
    fflush(stdout);
  }
  
  template<typename T>
  float PenalizedTotalVariationSimpleMultivariateRegression<T>::gradient(int coord, RegressionDataSet<T>& data)
  {
    int first= floor(parameters(parameters.size()-2)+0.5);
    int last= floor(parameters(parameters.size()-1)+0.5);
    
    if ( !gradientBaseComputed )
    {
      gradientBase= 0;
      
      float p;
      for ( int i= first; i <= last; ++i )
      {
	for ( unsigned int j= 0; j < data.size(); ++j )
	{
	  p= 0;
	  for ( int k= first; k <= last; ++k )
	    p+= parameters(k)*data(j)(k);
	  p*= a;
	  p+= b;
	  p-= data(j).targetValue;
	  
	  gradientBase(i)+= p*data(j)(i)*a;
	}
      }
      
      gradientBaseComputed= 1;
    }
    
    if ( coord == first )
    {
      float tmp= gradientBase(first);
      
      float a0= parameters(first);
      float a1= parameters(first+1);
      float an= 1;
      float n= (last - first + 1);
      
      float numerator= 2*(a0-a1)*(a0-an)*(a0-an) - 2*(a0-an)*(a0-a1)*(a0-a1);
      float denumerator= (a0-an)*(a0-an)*(a0-an)*(a0-an);
      float reg= beta/n*numerator/denumerator;
      
      tmp+= reg;
      
      return tmp;
    }
    else if ( coord > first )
    {
      float tmp= gradientBase(coord);
      
      float ak= parameters(coord);
      float akm1= parameters(coord-1);
      float akp1= parameters(coord+1);
      float a0= parameters(first);
      float an= 1;
      float n= (last - first + 1);
      
      float reg= beta/(n*(a0-an)*(a0-an))*(2*ak - akp1 - akm1);
      
      tmp+= reg;
      return tmp;
    }
    
    return 0;
  }
  
  template<typename T>
  float PenalizedTotalVariationSimpleMultivariateRegression<T>::suboptimize(RegressionDataSet<T>& data, float currentObjective, KFoldCrossValidator<float>& /*kfcv*/, RegressionDataSet<T>& tmpdataset, int iterations, UnivariateLinearRegression<float>& ulr, int& subiterations)
  {
    tprintf("suboptimize...\n");
    //int coord;
    //float lastValue;
    int first;
    int last;
    float newObjective;
    //float lastObjective= -1;
    //float value;
    float tv, maxtv, sumtv;
    float besta, bestb;
    //float newValue;
    //float lasta, newa;
    //float lastb, newb;

    currentObjective= FLT_MAX;
    Vector<float> gradientVector;
    Vector<float> lastParameters;
    gradientVector.resize(parameters.size());
    gradientBase.resize(parameters.size());
    
    first= floor(parameters(parameters.size()-2)+0.5);
    last= floor(parameters(parameters.size()-1)+0.5);
    
    //int n= 0;
    
    int j;
    for ( j= 0; j < iterations; ++j )
    {
      //lastObjective= currentObjective;
      lastParameters= parameters;
      gradientBaseComputed= 0;
      
      float maxGradient= 0;
      //int maxIdx= 0;
      
      for ( int i= first; i <= last-1; ++i )
      {
	float grad= gradient(i, data);
	gradientVector(i)= grad;
	if ( grad > maxGradient )
	{
	  maxGradient= i;
	  //maxIdx= i;
	}
      }
      
      //lasta= a;
      //lastb= b;
      
      float step= 0.001;
      float maxgrad= 0;
      
      for ( int k= first; k <= last-1; ++k )
	if ( fabs(gradientVector(k)) > maxgrad )
	  maxgrad= fabs(gradientVector(k));
      gradientVector/= maxgrad;
      
      float changea= 0;
      for ( int k= first; k <= last-1; ++k )
      {
	if ( k == first )
	{
	  if ( gradientVector(k) < 0 )
	    if ( fabs(parameters(k+1) - parameters(k)) > 2*step*fabs(gradientVector(k)) )
	      parameters(k)-= step*gradientVector(k);
	  if ( gradientVector(k) > 0 )
	    if ( fabs(0 - parameters(k)) > 2*step*fabs(gradientVector(k)) )
	      parameters(k)-= step*gradientVector(k);
	  if ( parameters(k) < 0 )
	    parameters(k)= 0;
	  changea+= fabs(lastParameters(first) - parameters(first));    
	  continue;
	}
	
	if ( gradientVector(k) < 0 )
	{
	  if ( fabs(parameters(k+1) - parameters(k)) > 2*step*fabs(gradientVector(k)) )
	  {
	    parameters(k)-= step*gradientVector(k);
	    changea+= fabs(step*gradientVector(k));
	  }
	}
	
	if ( gradientVector(k) > 0 )
	{
	  if ( fabs(parameters(k-1) - parameters(k)) > 2*step*fabs(gradientVector(k)) )
	  {
	    parameters(k)-= step*gradientVector(k);
	    changea+= fabs(step*gradientVector(k));
	  }
	}
	
      }
      
      newObjective= 0;
      // computing the new objective
      for ( unsigned int i= 0; i < data.size(); ++i )
      {
	float value= 0;
	for ( int j= first; j <= last; ++j )
	  value+= data(i)(j)*parameters(j);
	tmpdataset(i)(0)= value;
	tmpdataset(i).targetValue= data(i).targetValue;
      }
      
      ulr.train(tmpdataset);
      float tmpval;
      for ( unsigned int i= 0; i < tmpdataset.size(); ++i )
      {
	tmpval= ulr.regress(tmpdataset(i));
	newObjective+= (tmpdataset(i).targetValue - tmpval)*(tmpdataset(i).targetValue - tmpval);
      }
      newObjective/= tmpdataset.size();
      //newObjective= kfcv.validate(&ulr, tmpdataset);
      a= ulr.beta;
      b= ulr.alpha;
	  
      tv= 0;
      maxtv= 0;
      sumtv= 0;
      for ( int j= first; j < last; ++j )
      {
	tv= (parameters(j)-parameters(j+1))*(parameters(j)-parameters(j+1));
	if ( tv > maxtv )
	  maxtv= tv;
	sumtv+= tv;
      }
      sumtv/=(last - first + 1);
      
      float min= parameters(first);
      float max= parameters(last);

      maxtv/=(max - min)*(max - min);
      sumtv/=(max - min)*(max - min);
      
      //float newobjtmp= newObjective;
      newObjective+= beta*sumtv;
	
      float change= 0;
      for ( int k= first; k <= last-1; ++k )
	change+= fabs(parameters(k) - lastParameters(k));
      
      currentObjective= newObjective;
      bestb= b;
      besta= a;
      //lastb= b;
      //lasta= a;
      lastParameters= parameters;
      
      //printf("%f ", changea); fflush(stdout);
      if ( changea < 0.00001 )
	break;
    }
    
    subiterations= j;
    tprintf("itarations: %d %d %d\n", j, first, last);
    
    a= besta;
    b= bestb;
    return currentObjective;
  }
  
  template<typename T>
  int PenalizedTotalVariationSimpleMultivariateRegression<T>::train(RegressionDataSet<T>& data, Vector<unsigned char>* mask)
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
    //float bestObjective= FLT_MAX;
    float olda, oldb;
    float besta= 0, bestb= 0;
    KFoldCrossValidator<float> kfcv(1, Validator<float>::VALIDATOR_MSE);
    RegressionFeatureVector<float> rfv0;
    int sit= 0;    
    
    // initialization of variables
    a= 1; b= 1;
    lower= 0;
    upper= data(0).size()-1;
    temperature= 1;
    annealing= 0.99;
    objective= FLT_MAX;
    newObjective= -1;
    
    
    
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
    
    float sumy= 0;
    for ( unsigned int i= 0; i < rds.size(); ++i )
      sumy+= rds(i).targetValue;
    sumy/= rds.size();
    float sumx= 0;
    for ( unsigned int i= 0; i < rds.size(); ++i )
      for ( unsigned int j= 0; j < rds(i).size(); ++j )
	sumx+= rds(i)(j);
    sumx/= rds.size();
    float est= sumy/sumx;
    printf("%f %f %f\n", sumx, sumy, est);
    parameters.resize(data(0).size()+2);
    for ( unsigned int i= 0; i < parameters.size()-2; ++i )
      parameters(i)= est+i*0.0000001;
    
    
    for ( unsigned int i= 0; i < parameters.size()-2; ++i )
      parameters(i)= float(i)/float(parameters.size()-2);
    parameters(parameters.size()-2)= 0;
    parameters(parameters.size()-1)= data(0).size()-1;
    parameters(parameters.size()-3)= 1;
    
    bestparameters= parameters;

    // initializing tmpdataset
    tmpdataset.featureNames.push_back(string("value"));
    for ( unsigned int i= 0; i < rds.size(); ++i )
    {
      RegressionFeatureVector<float> rfv;
      rfv.push_back(0);
      tmpdataset.push_back(rfv);
    }
    
    for ( unsigned int i= 0; i <= parameters.size()-3; ++i )
	parameters(i)= 0.75 + (float(i)-0)/float(parameters.size()-2)/4.0;
    parameters(parameters.size()-3)= 1;
    
    int i;
    bestparameters= parameters;
    
    tprintf("starting main iteration\n");
    // starting main iterations
    //for ( int k= 0; k < 10; ++k )
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
	int step0= rand()%30;
	int step= rand()%2 ? step0 : -step0;
	
	
	if ( coord == int(parameters.size())-2 && step0 != 0 )
	{
	  int before= parameters(parameters.size()-2);
	  
	  parameters(parameters.size()-2)+= step;
	  if ( parameters(parameters.size()-2) < 0 )
	    parameters(parameters.size()-2)= 0;
	  if ( parameters(parameters.size()-2) > parameters(parameters.size()-1) )
	    parameters(parameters.size()-2)= parameters(parameters.size()-1)-10;
	  if ( parameters(parameters.size()-2) < 0 )
	    parameters(parameters.size()-2)= 0;
	  if ( fabs(parameters(parameters.size()-2) - parameters(parameters.size()-1)) < parameters.size()/2 )
	    parameters(parameters.size()-2)-= step;
	  int current= parameters(parameters.size()-2);
	  
	  if ( current < before )
	  {
	    for ( int i= before-1; i >= 0; --i )
	      parameters(i)= parameters(before)/float(before)*(i);
	  }
	  
	  /*for ( int i= current; i <= last; ++i )
	    parameters(i)= 0.75 + (float(i)-current)/float(last-current)/4.0;
	  parameters(last)= 1;*/
	  //printf("%d %d\n", before, current);
	}
	else if ( coord == int(parameters.size())-1 && step0 != 0)
	{
	  int before= parameters(parameters.size()-1);
	  
	  parameters(parameters.size()-1)+= step;
	  if ( parameters(parameters.size()-1) > data(0).size()-1 )
	    parameters(parameters.size()-1)= data(0).size()-1;
	  if ( parameters(parameters.size()-1) < parameters(parameters.size()-2) )
	    parameters(parameters.size()-1)= parameters(parameters.size()-2)+10;
	  if ( parameters(parameters.size()-1) > data(0).size()-1 )
	    parameters(parameters.size()-1)= data(0).size()-1;
	  if ( fabs(parameters(parameters.size()-2) - parameters(parameters.size()-1)) < parameters.size()/2 )
	    parameters(parameters.size()-1)-= step;
	  
	  int current= floor(parameters(parameters.size()-1)+0.5);
	  parameters(parameters.size()-1)= current;
	  
	  if ( current > before )
	  {
	    for ( int i= before; i < int(parameters.size())-2; ++i )
	      parameters(i)= parameters(before-1) + (1 - parameters(before-1))/float(parameters.size()-2 - before+1)*(i-before+1);
	  }
	  
	  /*for ( int i= first; i <= current; ++i )
	    parameters(i)= 0.75 + (float(i)-first)/float(current-first)/4.0;*/
	  
	  parameters(current)= 1.0;
	}
	
	first= int(floor(parameters(parameters.size()-2)+0.5));
	last= int(floor(parameters(parameters.size()-1)+0.5));
	
	int subiterations;
	newObjective= suboptimize(rds, objective, kfcv, tmpdataset, 1000, ulr, subiterations);
	
	sit+= subiterations;
	if ( i % ( maxit/10) == 0 )
	{
	  tprintf("%f %f %f %f %d %d %d %d %f %f\n", bestobj, objective, newObjective, temperature, i, first, last, sit, bestparameters(bestparameters.size()-2), bestparameters(bestparameters.size()-1));
	  for ( int i= first; i <= last; ++i )
	    printf("%f ", parameters(i));
	  printf("\n");
	  sit= 0;
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
      
      tprintf("%f %f %f %f %d\n", bestobj, objective, newObjective, temperature, i);
      /*for ( int i= floor(parameters(parameters.size()-2)+0.5); i <= floor(parameters(parameters.size()-1)+0.5); ++i )
	printf("%f ", parameters(i));
      printf("%f %f\n", parameters(parameters.size()-2), parameters(parameters.size()-1));*/
    }
    
    printf("objective: %f, bestObjective: %f, temperature: %f, iteration: %d\n", objective, bestobj, temperature, i);
    cout << "best parameters: " << endl;
    for ( int i= floor(bestparameters(bestparameters.size()-2)+0.5); i <= floor(bestparameters(bestparameters.size()-1)+0.5); ++i )
	printf("%f ", bestparameters(i));
    printf("%f %f\n", bestparameters(bestparameters.size()-2), bestparameters(bestparameters.size()-1));
    
    parameters= bestparameters;
    a= besta;
    b= bestb;
    /*float tmp0= this->regress(rfv0);
    cout << "mse: " << (tmp0 - rfv0.targetValue)*(tmp0 - rfv0.targetValue) << endl;
    cout << "best parameters: " << a << " " << b << endl;*/
    
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
    
    ofstream weights("ptvsm.weights");
    for ( int i= floor(bestparameters(bestparameters.size()-2)+0.5); i < floor(bestparameters(bestparameters.size()-1)+0.5); ++i )
    {
      weights << i << " " << parameters(i) << endl;
    }
    weights.close();
    
    return 0;
  }

  template<typename T>
  float PenalizedTotalVariationSimpleMultivariateRegression<T>::regress(Vector<T>& v, Vector<float>* /*probabilities*/)
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
  int PenalizedTotalVariationSimpleMultivariateRegression<T>::saveModelToFile(ofstream& file)
  {
    cout << "saving model..." << endl;
    file << "PenalizedTotalVariationSimpleMultivariateRegression" << endl;
    
    Regression<T>::saveModelToFile(file);
    
    file << parameters.size() << endl;
    for ( unsigned int i= 0; i < parameters.size(); ++i )
      file << parameters(i) << endl;
    file << a << endl;
    file << b << endl;
    
    return 0;
  }

  template<typename T>
  int PenalizedTotalVariationSimpleMultivariateRegression<T>::openModelFromFile(ifstream& file)
  {
    string tmp;
    getline(file, tmp);
    
    if ( tmp.find_first_of(string("PenalizedTotalVariationSimpleMultivariateRegression")) == 0 )
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

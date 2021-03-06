#ifndef _SIGMOID_WEIGHTED_MULTIVARIATE_REGRESSION_H_
#define _SIGMOID_WEIGHTED_MULTIVARIATE_REGRESSION_H_

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
  class SigmoidWeightedMultivariateRegression: public Regression<T>
  {
    public:
      using Regression<T>::probabilities;
      using Regression<T>::preprocessingNames;
      using Regression<T>::featureNames;
      
      SigmoidWeightedMultivariateRegression(int maxit= 1000000, float tol= 1e-4);

      SigmoidWeightedMultivariateRegression(SigmoidWeightedMultivariateRegression& r);

      ~SigmoidWeightedMultivariateRegression();

      virtual int train(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL);

      virtual float regress(Vector<T>& c, Vector<float>* probabilities= NULL);
      
      void printMatrix(char* name, Matrix<double>& m);
      
      void printVector(char* name, Vector<double>& m);
      
      virtual int saveModelToFile(ofstream& file);

      virtual int openModelFromFile(ifstream& file);
      
      inline double a();
      
      inline double b();
      
      inline double alpha();
      
      inline double beta();
      
      inline double gamma();
      
      inline double delta();
      
      inline double mean();
      
      inline double sigma();
      
      inline double weight(int i);
      
      inline double jacobi(Vector<T>& v, int i);
      
      RegressionDataSet<float> rds;
      Vector<double> parameters;
      
      int maxit;
      float tol;
  };
  
  template<typename T>
  SigmoidWeightedMultivariateRegression<T>::SigmoidWeightedMultivariateRegression(int maxit, float tol)
  : Regression<T>()
  {
    this->maxit= maxit;
    this->tol= tol;
    
    parameters.resize(10);
    parameters= 0;
  }
  
  template<typename T>
  SigmoidWeightedMultivariateRegression<T>::SigmoidWeightedMultivariateRegression(SigmoidWeightedMultivariateRegression& r)
  : Regression<T>(r)
  {
    this->maxit= r.maxit;
    this->tol= r.tol;
    this->parameters= r.parameters;
  }
  
  template<typename T>
  SigmoidWeightedMultivariateRegression<T>::~SigmoidWeightedMultivariateRegression()
  {
  }
  
  template<typename T>
  void SigmoidWeightedMultivariateRegression<T>::printMatrix(char* name, Matrix<double>& m)
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
  void SigmoidWeightedMultivariateRegression<T>::printVector(char* name, Vector<double>& m)
  {
    printf("%s %d\n", name, m.size());
    for ( int i= 0; i < m.size(); ++i )
      printf("%f ", m(i));
    printf("\n\n");
    fflush(stdout);
  }
  
  template<typename T>
  double SigmoidWeightedMultivariateRegression<T>::a()
  {
    return parameters(0);
  }
  
  template<typename T>
  double SigmoidWeightedMultivariateRegression<T>::b()
  {
    return parameters(1);
  }
  
  template<typename T>
  double SigmoidWeightedMultivariateRegression<T>::alpha()
  {
    return parameters(2);
  }
  
  template<typename T>
  double SigmoidWeightedMultivariateRegression<T>::beta()
  {
    return parameters(3);
  }
  
  template<typename T>
  double SigmoidWeightedMultivariateRegression<T>::gamma()
  {
    return parameters(4);
  }
  
  template<typename T>
  double SigmoidWeightedMultivariateRegression<T>::delta()
  {
    return parameters(5);
  }
  
  template<typename T>
  double SigmoidWeightedMultivariateRegression<T>::mean()
  {
    return parameters(8);
  }
  
  template<typename T>
  double SigmoidWeightedMultivariateRegression<T>::sigma()
  {
    return parameters(9);
  }
  
  template<typename T>
  double SigmoidWeightedMultivariateRegression<T>::weight(int i)
  {
    double w= 0;
    
    w+= 1;
    w+= exp(-alpha()*i +alpha()*beta());
    w+= exp(gamma()*i - gamma()*delta());
    w+= exp((gamma() - alpha())*i + alpha()*beta() - gamma()*delta());
    
    w= 1.0/w;
    //w=1;
    w*= exp(-(mean()-i)*(mean()-i)/sigma());
    //return w + /*parameters(7) +*/ parameters(6)*(i+1) + parameters(7)*(i+1)*(i+1);
    return w + parameters(6)*(i+1)/* + w*parameters(7)*(i+1)*(i+1)*/;
    //return 1 + parameters(6)*i;
    //return (i*1)*(i+1) + parameters(6)*(i+1);
  }
  
  template<typename T>
  double SigmoidWeightedMultivariateRegression<T>::jacobi(Vector<T>& v, int i)
  {
    double w= 0, tmp;
    
    if ( i == 0 )
    {
      for ( int j= 0; j < v.size(); ++j )
	w+= v(j)*weight(j);
      w*= a();
      w+= b();
      
      return w;
    }
    if ( i == 1 )
    {
      return 1;
    }
    if ( i == 2 )
    {
      for ( int j= 0; j < v.size(); ++j )
      {
	tmp= 0;
	tmp+= exp(-alpha()*j-alpha()*beta())*(-j-beta());
	tmp+= exp((gamma()-alpha())*j + gamma()*delta()-alpha()*beta())*(-j-beta());
	tmp*= weight(j);
	tmp*= weight(j);
	tmp*= (-1);
	tmp*= v(j);
	w+= tmp;
      }
      w*= a();
      return w;
    }
    if ( i == 3 )
    {
      for ( int j= 0; j < v.size(); ++j )
      {
	tmp= 0;
	tmp+= exp(-alpha()*j-alpha()*beta())*(-alpha());
	tmp+= exp((gamma()-alpha())*j + gamma()*delta()-alpha()*beta())*(-alpha());
	tmp*= weight(j);
	tmp*= weight(j);
	tmp*= (-1);
	tmp*= v(j);
	w+= tmp;
      }
      w*= a();
      return w;
    }
    if ( i == 4 )
    {
      for ( int j= 0; j < v.size(); ++j )
      {
	tmp= 0;
	tmp+= exp(gamma()*j+gamma()*delta())*(j+delta());
	tmp+= exp((gamma()-alpha())*j + gamma()*delta()-alpha()*beta())*(j+delta());
	tmp*= weight(j);
	tmp*= weight(j);
	tmp*= (-1);
	tmp*= v(j);
	w+= tmp;
      }
      w*= a();
      return w;
    }
    if ( i == 5 )
    {
      for ( int j= 0; j < v.size(); ++j )
      {
	tmp= 0;
	tmp+= exp(gamma()*j+gamma()*delta())*(gamma());
	tmp+= exp((gamma()-alpha())*j + gamma()*delta()-alpha()*beta())*(gamma());
	tmp*= weight(j);
	tmp*= weight(j);
	tmp*= (-1);
	tmp*= v(j);
	w+= tmp;
      }
      w*= a();
      return w;
    }
    
  }
  
  template<typename T>
  int SigmoidWeightedMultivariateRegression<T>::train(RegressionDataSet<T>& data, Vector<unsigned char>* mask)
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
    
    KFoldCrossValidator<float> kfcv(1, Validator<float>::VALIDATOR_MSE);
    parameters(0)= 0.01;
    parameters(1)= 30;
    parameters(2)= 1;
    parameters(3)= 0;
    parameters(4)= 1;
    parameters(5)= data(0).size();
    parameters(6)= 0.1;
    parameters(7)= 0.1;
    parameters(8)= data(0).size()/2;
    parameters(9)= sqrt(data(0).size());
    
    bestparameters= parameters;
    
    RegressionFeatureVector<float> rfv0;
    
    for ( int iter= 0; iter < 10; ++iter )
    {
    
      
    
      
    parameters= bestparameters;
    /*parameters(2)= 5;
    parameters(3)= 162;
    parameters(5)= 120;*/
    
    /*printf("weights: \n");
    for ( int i= 0; i < 120; ++i )
      printf("%f ", weight(i));
    printf("\n");*/
    
    //getchar();
    
    data.init();
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

    temperature= 1;
    annealing= 0.95;
    objective= FLT_MAX;
    newObjective= -1;
    Vector<float> old_parameters;
    
    /*for ( int i= 0; i < rds.size(); ++i )
    {
      printf("%f ", rds(i).targetValue);
      for ( int j= 0; j < rds(i).size(); ++j )
	printf("%f ", rds(i)(j));
      printf("\n");
    }*/
    
    
    /*Vector<float> p2;
    p2.push_back(0.001);
    p2.push_back(0.002);
    p2.push_back(0.005);
    p2.push_back(0.01);
    p2.push_back(0.02);
    p2.push_back(0.05);
    p2.push_back(0.1);
    p2.push_back(0.2);
    p2.push_back(0.5);
    p2.push_back(1);
    p2.push_back(2);
    p2.push_back(5);
    p2.push_back(10);
    p2.push_back(20);
    p2.push_back(50);
    p2.push_back(100);
    Vector<float> p4;
    p4= p2;
    Vector<float> p3;
    for ( int i= -20; i < 50; ++i )
      p3.push_back(i);
    Vector<float> p5;
    for ( int i= 50; i < 140; ++i )
      p5.push_back(i);
    
    float bestobj= FLT_MAX;
    int bestp2;
    int bestp3;
    int bestp4;
    int bestp5;
    
    
    
    RegressionDataSet<float> tmpdataset;
    UnivariateLinearRegression<float> ulr;
    tmpdataset.featureNames.push_back(string("value"));
    for ( int i= 0; i < rds.size(); ++i )
    {
      RegressionFeatureVector<float> rfv;
      rfv.push_back(0);
      tmpdataset.push_back(rfv);
    }
    KFoldCrossValidator<float> kfcv(1, Validator<float>::VALIDATOR_MSE);
    
    int n= 0;
    
    for ( int i= 0; i < p2.size(); ++i )
      for ( int j= 0; j < p3.size(); ++j )
	for ( int k= 0; k < p4.size(); ++k )
	  for ( int l= 0; l < p5.size(); ++l )
	  {
	    ++n;
	    
	    parameters(2)= p2(i);
	    parameters(3)= p3(j);
	    parameters(4)= p4(k);
	    parameters(5)= p5(l);
	    
	    if ( p3(j) < p5(j) )
	    {
	      
	      
	      for ( int i= 0; i < rds.size(); ++i )
	      {
		float value= 0;
		for ( int j= 0; j < rds(i).size(); ++j )
		  value+= rds(i)(j)*weight(j);

		tmpdataset(i)(0)= value;
		tmpdataset(i).targetValue= rds(i).targetValue;
	      }
	      
	      newObjective= kfcv.validate(&ulr, tmpdataset);
	      parameters(0)= ulr.beta;
	      parameters(1)= ulr.alpha;
	      
	      if ( newObjective < bestobj )
	      {
		bestp2= i;
		bestp3= j;
		bestp4= k;
		bestp5= l;
		bestobj= newObjective;
		
		
	      }
	      
	      if ( n % 1000 == 0)
		  printf("%d/%d %f %f %f %f %f %f\n", n, p2.size()*p3.size()*p4.size()*p5.size(), bestobj, newObjective, p2(bestp2), p3(bestp3), p4(bestp4), p5(bestp5));
	    }
	    else
	      continue;
	  }
    
    parameters(2)= p2(bestp2);
    parameters(3)= p3(bestp3);
    parameters(4)= p4(bestp4);
    parameters(5)= p5(bestp5);
    
    UnivariateLinearRegression<float> ulr2;
    RegressionDataSet<float> tmpdataset2;
    tmpdataset2.featureNames.push_back(string("value"));
    for ( int i= 0; i < rds.size(); ++i )
    {
      float value= 0;
      for ( int j= 0; j < rds(i).size(); ++j )
      {
	value+= rds(i)(j)*weight(j);
      }
      RegressionFeatureVector<float> rfv;
      rfv.push_back(value);
      rfv.targetValue= rds(i).targetValue;
      tmpdataset2.push_back(rfv);
    }
    ulr2.train(tmpdataset2);
    parameters(0)= ulr2.beta;
    parameters(1)= ulr2.alpha;
    
    return 0;
    */
    
    
    
    RegressionDataSet<float> tmpdataset;
    UnivariateLinearRegression<float> ulr;
    tmpdataset.featureNames.push_back(string("value"));
    for ( unsigned int i= 0; i < rds.size(); ++i )
    {
      RegressionFeatureVector<float> rfv;
      rfv.push_back(0);
      tmpdataset.push_back(rfv);
    }
    
    int i;
    //srand(200);
    

    
    tprintf("size: %d %d\n", rds.size(), rds(0).size());
    
    for ( i= 0; i < maxit && temperature > tol; ++i )
    {
      old_parameters= parameters;
      int coord= rand()%8;
      //int sign= rand()%2;
      //printf("%d\n", coord);
      
      /*if ( coord != 4 )
	continue;*/
      
      {
	if ( coord == 0 || i == 0 )
	  parameters(2)= parameters(2)-0.5 + double(rand())/double(RAND_MAX);
	if ( coord == 1 || i == 0 )
	  parameters(3)= parameters(3)-1 + double(rand())/double(RAND_MAX)*2;
	if ( coord == 2 || i == 0 )
	  parameters(4)= parameters(4)-0.5 + double(rand())/double(RAND_MAX);
	if ( coord == 3 || i == 0 )
	  parameters(5)= parameters(5)-1 + double(rand())/double(RAND_MAX)*2;
	if ( coord == 4 || i == 0 )
	  parameters(6)= parameters(6) - 0.01 + double(rand())/double(RAND_MAX) * 0.02;
	if ( coord == 5 || i == 0 )
	  parameters(7)= parameters(7) - 0.01 + double(rand())/double(RAND_MAX) * 0.02;
	if ( coord == 6 || i == 0 )
	  parameters(8)= parameters(8) - 1 + double(rand())/double(RAND_MAX) * 2;
	if ( coord == 7 || i == 0 )
	  parameters(9)= parameters(9) - 1 + double(rand())/double(RAND_MAX) * 2;
	
      }

      if ( parameters(5) < parameters(3) /*|| parameters(5) < rds(0).size()/2*/ || parameters(5) > rds(0).size()*1.5 || parameters(3) < -0.5*rds(0).size() /*|| parameters(3) > 0.5*rds(0).size()*/ || parameters(6) < -1 || parameters(7) < 0 || parameters(6) > 1 || parameters(7) > 1 || parameters(8) < 0 || parameters(8) > rds(0).size() || parameters(9) > 20*rds(0).size() || parameters(9) < 0)
      {
	//--i;
	parameters= old_parameters;
	continue;
      }
      if ( parameters(2) <= 0 || parameters(4) <= 0 )
      {
	//--i;
	parameters= old_parameters;
	continue;
      }
      
      newObjective= 0;
      //float tmp;
      // computing the new objective
      {
	for ( unsigned int i= 0; i < rds.size(); ++i )
	{
	  float value= 0;
	  for ( unsigned int j= 0; j < rds(i).size(); ++j )
	    value+= rds(i)(j)*weight(j);

	  tmpdataset(i)(0)= value;
	  tmpdataset(i).targetValue= rds(i).targetValue;
	}
	
	newObjective= kfcv.validate(&ulr, tmpdataset);
	parameters(0)= ulr.beta;
	parameters(1)= ulr.alpha;
	if ( parameters(1) < 0 )
	  continue;
      }
      
      if ( i % 10000 == 0 )
      {
      printf("%f %f %f %f %d %f %f %f %f %f %f %f %f %f %f\n", bestobj, objective, newObjective, temperature, i, parameters(0), parameters(1), parameters(2), parameters(3), parameters(4), parameters(5), parameters(6), parameters(7), parameters(8), parameters(9)); fflush(stdout);
      }
      //getchar();
      
      if ( objective < bestobj )
      {
	bestparameters= parameters;
	bestobj= objective;
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
	  parameters= old_parameters;
      }
      else
      {
	parameters= old_parameters;
      }
      
    }
    printf("objective: %f, temperature: %f, iteration: %d\n", objective, temperature, i);
    cout << "best objective " << bestobj << endl;
    cout << "best parameters " << bestparameters << endl;
    this->parameters= bestparameters;
    float tmp0= this->regress(rfv0);
    cout << "mse: " << (tmp0 - rfv0.targetValue)*(tmp0 - rfv0.targetValue) << endl;
    }
    
    
    parameters= bestparameters;
    
    UnivariateLinearRegression<float> ulr2;
    RegressionDataSet<float> tmpdataset2;
    tmpdataset2.featureNames.push_back(string("value"));
    for ( unsigned int i= 0; i < rds.size(); ++i )
    {
      float value= 0;
      for ( unsigned int j= 0; j < rds(i).size(); ++j )
      {
	value+= rds(i)(j)*weight(j);
      }
      RegressionFeatureVector<float> rfv;
      rfv.push_back(value);
      rfv.targetValue= rds(i).targetValue;
      tmpdataset2.push_back(rfv);
    }
    ulr2.train(tmpdataset2);
    parameters(0)= ulr2.beta;
    parameters(1)= ulr2.alpha;
    
    
    printf("best objective: %f\n", bestobj);
    printf("parameters: \n");
    for ( int i= 0; i < 8; ++i )
      printf("%f ", parameters(i));
    printf("\n");
    printf("weights: \n");
    for ( int i= 0; i < 120; ++i )
      printf("%f ", weight(i));
    printf("\n");
    
    cerr << parameters << endl;
    
    //getchar();
    
    /*int rows= rds.size();
    int columns= parameters.size();
    int xcols= rds(0).size();
    
    tprintf("X dimensions: %d, %d\n", rows, xcols);
    Matrix<double> X(rows, xcols);
    Vector<double> y(rows);
    
    int n= 0;
    for ( int i= 0; i < rds.size(); ++i )
    {
	for ( int j= 0; j < rds(i).size(); ++j )
	  X(i,j)= rds(i)(j);
	y(n)= rds(i).targetValue;
	++n;
    }
    
    Matrix<double> J(rows, columns);
    Vector<double> deltabeta(columns);
    Vector<double> deltay(rows);
    
    Matrix<double> lefthand(columns, columns);
    Vector<double> righthand(columns);
    Matrix<double> JT(rows, columns);
    
    for ( int i= 0; i < parameters.size(); ++i )
      parameters(i)= double(rand())/double(RAND_MAX);
    parameters(0)= 0.01;
    parameters(1)= 30;
    parameters(2)= 1;
    parameters(3)= 0;
    parameters(4)= 1;
    parameters(5)= -120;
    
    for ( int i= 0; i < 120; ++i )
      printf("%f ", weight(i));
    printf("\n");
    getchar();
    
    int m= 10000;
    for ( int i= 0; i < maxit; ++i )
    {
      for ( int j= 0; j < rows; ++j )
	for ( int k= 0; k < columns; ++k )
	  J(j,k)= jacobi(rds(j),k);

      //if ( i % m == 0 )
	printMatrix("J", J);
      
	getchar();
      JT= J;
      printMatrix("JT", JT); fflush(stdout);
      JT.transpose();
      printMatrix("JT transposed", JT); fflush(stdout);
      lefthand= JT^J;
      printMatrix("lefthand", lefthand); fflush(stdout);
      getchar();
      for ( int j= 0; j < deltay.size(); ++j )
	deltay(j)= y(j) - regress(rds(j));
      
      //if ( i % m == 0 )
      {
	
	
	printVector("deltay", deltay);
      }
      
      getchar();
      righthand= JT^deltay;
      
      //if ( i % m == 0 )
	printVector("righthand", righthand);
      
	getchar();
      solveLinearEquationSystem(&(lefthand(0,0)), lefthand.rows, lefthand.columns,  &(righthand(0)), &(deltabeta(0)));

      //if ( i % m == 0 )
	printVector("deltabeta", deltabeta);
      
      parameters+= deltabeta*0.0001;
      if ( parameters(0) < 0.001 )
	parameters(0)= 0.001;

      //if ( i % m == 0 )
      {
	printVector("parameters", parameters);
	printVector("y", y);
      }
      
      printf("y: \n");
      for ( int j= 0; j < deltay.size(); ++j )      
      {
	printf("%f ", regress(rds(j)));
	deltay(j)= y(j) - regress(rds(j));
      }
      printf("\n");
      
      //if ( i % m == 0 )
	printVector("deltay", deltay);
      
      double r2= 0;
      for ( int j= 0; j < deltay.size(); ++j )
	r2+= deltay(j)*deltay(j);

      //if ( i % m == 0 )
	printf("iteration: %d, r^2: %f\n", i, r2); fflush(stdout);
    
      float maxdeltabetaperbeta= 0;
      for ( int j= 0; j < deltabeta.size(); ++j )
	if ( parameters(j) != 0 )
	  if ( fabs(deltabeta(j)/parameters(j)) > maxdeltabetaperbeta)
	    maxdeltabetaperbeta= fabs(deltabeta(j)/parameters(j));
      
      if ( maxdeltabetaperbeta < tol )
	break;
      
      getchar();*/
    //}
    
    
    return 0;
  }

  template<typename T>
  float SigmoidWeightedMultivariateRegression<T>::regress(Vector<T>& v, Vector<float>* /*probabilities*/)
  {
    double w= 0;
    //double tmp;
    
    for ( unsigned int i= 0; i < v.size(); ++i )
    {
      /*tmp= 0;
      tmp+= 1;
      tmp+= exp(-alpha()*i - alpha()*beta());
      tmp+= exp(gamma()*i + gamma()*delta());
      tmp+= exp((gamma() - alpha())*i + gamma()*delta()-alpha()*beta());*/
      
      w+= v(i)*weight(i);
    }
    
    w*= a();
    w+= b();
    
    return w;
  }

  template<typename T>
  int SigmoidWeightedMultivariateRegression<T>::saveModelToFile(ofstream& file)
  {
    file << "SigmoidWeightedMultivariateRegression" << endl;
    
    Regression<T>::saveModelToFile(file);
    
    file << parameters.size() << endl;
    for ( unsigned int i= 0; i < parameters.size(); ++i )
      file << parameters(i) << endl;
    
    return 0;
  }

  template<typename T>
  int SigmoidWeightedMultivariateRegression<T>::openModelFromFile(ifstream& file)
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
#ifndef PARTIAL_LEAST_SQUARES_REGRESSION
#define PARTIAL_LEAST_SQUARES_REGRESSION

#include <string>

#include <openipDS/Vector.h>
#include <openipDS/RegressionDataSet.h>
#include <openipML/Regression.h>
#include <openipSC/matrixOperations.h>
#include <openipML/PrincipalComponentAnalysis.h>

namespace openip
{
  template<typename T>
  class PartialLeastSquaresRegression: public Regression<T>
  {
    public:
      using Regression<T>::probabilities;
      using Regression<T>::preprocessingNames;
      using Regression<T>::featureNames;
      
      PartialLeastSquaresRegression(int factors, int maxit= 1000000, float tol= 1e-4, int pca= 0);

      PartialLeastSquaresRegression(PartialLeastSquaresRegression& r);

      ~PartialLeastSquaresRegression();

      virtual int train(RegressionDataSet<T>& data, Vector<unsigned char>* mask= NULL);

      virtual float regress(Vector<T>& c, Vector<float>* probabilities= NULL);
      
      void printMatrix(char const* name, Matrix<T>& m);
      
      void printVector(char* name, Vector<T>& m);
      
      virtual int saveModelToFile(ofstream& file);

      virtual int openModelFromFile(ifstream& file);
      
      RegressionDataSet<float> rds;
      
      int pca;
      PrincipalComponentAnalysis<T> principalComponentAnalysis;
      
      int maxit;
      float tol;
      
      Matrix<float> B;
      Vector<float> meanX;
      Vector<float> meanY;
      Vector<float> stdevX;
      int factors;
  };
  
  template<typename T>
  PartialLeastSquaresRegression<T>::PartialLeastSquaresRegression(int factors, int maxit, float tol, int pca)
  : Regression<T>()
  {
    this->pca= pca;
    this->maxit= maxit;
    this->tol= tol;
    this->factors= factors;
  }
  
  template<typename T>
  PartialLeastSquaresRegression<T>::PartialLeastSquaresRegression(PartialLeastSquaresRegression& r)
  : Regression<T>(r)
  {
    this->pca= r.pca;
    this->maxit= r.maxit;
    this->tol= r.tol;
    this->factors= r.factors;
  }
  
  template<typename T>
  PartialLeastSquaresRegression<T>::~PartialLeastSquaresRegression()
  {
  }
  
  template<typename T>
  void PartialLeastSquaresRegression<T>::printMatrix(char const* name, Matrix<T>& m)
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
  void PartialLeastSquaresRegression<T>::printVector(char* name, Vector<T>& m)
  {
    printf("%s %d\n", name, m.size());
    for ( int i= 0; i < m.size(); ++i )
      printf("%f ", m(i));
    printf("\n\n");
    fflush(stdout);
  }
  
  template<typename H>
  int PartialLeastSquaresRegression<H>::train(RegressionDataSet<H>& data, Vector<unsigned char>* mask)
  {
    this->preprocessingNames= data.preprocessingNames;
    this->featureNames= data.featureNames;
    
    data.init();
    rds.clear();
    rds.preprocessingNames.clear();
    rds.featureNames.clear();
    B.clear();
    meanX.clear();
    meanY.clear();
    stdevX.clear();
    
    if ( pca )
    {
      int n= 0;
      tprintf("applying pca\n");
      RegressionDataSet<H> pcadata;
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
      /*for ( int i= 0; i < rds.size(); ++i )
	printVector("rds", rds(i));*/
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
    }
    
    tprintf("initialize variables\n");
    tprintf("rows: %d\n", rds.size());
    int rows= rds.size();
    int xcols= rds(0).size();
    int ycols= 1;
    
    //int nobj= rows;
    int npred= xcols;
    int nresp= 1;
    int ncomp= factors;
    
    meanX.resize(xcols);
    meanY.resize(ycols);
    stdevX.resize(xcols);
    
    tprintf("compute means\n");
    for ( unsigned int i= 0; i < rds.size(); ++i )
    {
      for ( unsigned int j= 0; j < rds(0).size(); ++j )
      {
	meanX(j)+= rds(i)(j);
	stdevX(j)+= rds(i)(j)*rds(i)(j);
      }
      meanY(0)+= rds(i).targetValue;
    }
    meanX/= rows;
    meanY/= rows;
    
    tprintf("setting up X\n");
    Matrix<float> X(rows, xcols);
    for ( int i= 0; i < rows; ++i )
    {
      for ( int j= 0; j < xcols; ++j )
      {
	X(i,j)= (rds(i)(j)) - meanX(j);
	//printf("%f ", X(i,j));
      }
      //printf("\n");
    }
    //printf("\n");
    
    tprintf("setting up Y\n");
    Matrix<float> Y(rows, ycols);
    for ( int i= 0; i < rows; ++i )
      for ( int j= 0; j < ycols; ++j )
      {
	//printf("%f ", rds(i).targetValue);
	Y(i,j)= rds(i).targetValue - meanY(0);
      }
    //printf("\n");
      
    //printVector("xmeans", meanX);
    //printVector("ymean", meanY);

    tprintf("initialize matrices\n");
    Matrix<float> R(npred, ncomp);
    Matrix<float> P(npred, ncomp);
    R= 0;
    P= 0;
    
    Matrix<float> tQ(ncomp, nresp);
    tQ= 0;
    Vector<Matrix<float> > B;
    B.resize(ncomp);
    for ( int i= 0; i < ncomp; ++i )
    {
      B(i)= Matrix<float>(nresp, npred);
      B(i)= 0;
    }
    
    tprintf("step 1\n");
    // step 1
    
    Matrix<float> XtY;
    Matrix<float> Xt;
    Xt= X;
    Xt.transpose();
    XtY= Xt^Y;
    
    for ( int a= 0; a < ncomp; ++a )
    {
      tprintf("step 2\n");
      // step 2
      //printVector("XtY", XtY);
      Vector<float> wa(X.columns);
      for ( unsigned int i= 0; i < wa.size(); ++i )
	wa(i)= XtY(i);
      float wanorm= wa.getNorm2();
      for ( unsigned int i= 0; i < wa.size(); ++i )
	wa(i)/= wanorm;
      //wa/= sqrt(wa.getNorm2());
      //printVector("wa", wa);
      
      tprintf("step 3\n");
      // step 3
      Vector<float> ra= wa;
      for ( int j= 0; j < a; ++j )
      {
	Vector<float> pj(P.rows);
	Vector<float> rj(R.rows);
	P.getColumn(pj, j);
	R.getColumn(rj, j);
	float pjwa= pj.inner(wa);
	//float pjwa= pj^wa;
	ra-= rj*pjwa;
      }
      
      //printVector("ra", ra);
      
      tprintf("step 4\n");
      // step 4
      Vector<float> ta;
      ta= X^ra;
      //printVector("ta", ta);
      float tsq= ta.getNorm2()*ta.getNorm2();
      Vector<float> pa;
      Matrix<float> Xt= X;
      Xt.transpose();
      pa= Xt^ta;
      pa/= tsq;
      Vector<float> qa;
      Matrix<float> XtYt= XtY;
      XtYt.transpose();
      qa= XtYt^ra;
      qa/= tsq;
      
      //printVector("pa", pa);
      //printVector("qa", qa);
      
      tprintf("step 5 %f\n", tsq);
      // step 5
      Matrix<float> paqa(XtY.rows, XtY.columns);
      //paqa= pa.columnRowProduct(qa);
      paqa.setColumn(pa, 0);
      paqa*= qa(0);
      paqa*= tsq;
      //printMatrix("paqa", paqa);
      XtY-= paqa;
      
      tprintf("step 6-8\n");
      /*tprintf("R.rows, R.columns: %d %d\n", R.rows, R.columns);
      tprintf("P.rows, P.columns: %d %d\n", P.rows, P.columns);
      tprintf("ra.size(): %d\n", ra.size());
      tprintf("pa.size(): %d\n", pa.size());
      tprintf("tQ.rows, tQ.columns: %d %d\n", tQ.rows, tQ.columns);
      tprintf("qa.size(): %d\n", qa.size());*/
      // step 6-8
      R.setColumn(ra, a);
      P.setColumn(pa, a);
      tQ.setRow(qa,a);
      
      Matrix<float> Rs(R.rows, a+1);
      Vector<float> Rv(R.rows);
      Matrix<float> tQs(a+1, tQ.columns);
      Vector<float> tQv(tQ.columns);
      for ( int j= 0; j <= a; ++j )
      {
	R.getColumn(Rv, j);
	Rs.setColumn(Rv, j);
	tQ.getRow(tQv, j);
	tQs.setRow(tQv, j);
      }
      
      /*printMatrix("Rs", Rs);
      printMatrix("tQs", tQs);*/
      B(a)= Rs^tQs;
      tprintf("iteration finished\n");
      //printMatrix("B(a)", B(a));
    }
    
    tprintf("finalize computations, B.rows, B.columns: %d %d\n", B(ncomp-1).rows, B(ncomp-1).columns);
    Matrix<float> BB= B(ncomp-1);
    BB.transpose();
    printMatrix("B", BB);
    this->B= B(ncomp-1);
    
    ofstream weights("pls.weights");
    for ( int i= 0; i < this->B.rows; ++i )
    {
      weights << i << " " << this->B(i,0) << endl;
    }
    weights.close();
    
    tprintf("train function finished\n");
    
    return 0;
  }
  
  template<typename T>
  float PartialLeastSquaresRegression<T>::regress(Vector<T>& c, Vector<float>* /*probabilities*/)
  {
    float d= 0;
    for ( int i= 0; i < B.rows; ++i )
    {
      d+= (c(i) - meanX(i))*B(i,0);
    }
    
    d+= meanY(0);
    
    return d;
  }

  template<typename T>
  int PartialLeastSquaresRegression<T>::saveModelToFile(ofstream& file)
  {
    tprintf("saving PLS regression model to file...\n");
    file << "PartialLeastSquaresRegression" << std::endl;

    Regression<T>::saveModelToFile(file);
    
    file << B.rows << " " << B.columns << endl;
    
    for ( int i= 0; i < B.rows; ++i )
    {
      for ( int j= 0; j < B.columns; ++j )
	file << B(i,j) << " ";
      file << endl;
    }
    
    file << meanX.size() << endl;
    
    for ( unsigned int i= 0; i < meanX.size(); ++i )
      file << meanX(i) << " ";
    file << endl;
    
    file << meanY.size() << endl;
    
    for ( unsigned int i= 0; i < meanY.size(); ++i )
      file << meanY(i) << " ";
    file << endl;
    
    file << stdevX.size() << endl;
    
    for ( unsigned int i= 0; i < stdevX.size(); ++i )
      file << stdevX(i) << " ";
    file << endl;
    
    return 0;
  }

  template<typename T>
  int PartialLeastSquaresRegression<T>::openModelFromFile(ifstream& file)
  {
    std::string tmp;
    getline(file, tmp);
    if ( tmp.find("PartialLeastSquaresRegression") == 0 )
    {
      Regression<T>::openModelFromFile(file);
      
      int brows, bcols;
      file >> brows;
      file >> bcols;
      B.resize(brows, bcols);
      
      float tmp;
      for ( int i= 0; i < brows; ++i )
	for ( int j= 0; j < bcols; ++j )
	{
	  file >> tmp;
	  B(i,j)= tmp;
	}

      meanX.clear();
      int meanxsize;
      file >> meanxsize;
      for ( int i= 0; i < meanxsize; ++i )
      {
	file >> tmp;
	meanX.push_back(tmp);
      }
      
      meanY.clear();
      int meanysize;
      file >> meanysize;
      for ( int i= 0; i < meanysize; ++i )
      {
	file >> tmp;
	meanY.push_back(tmp);
      }
      
      stdevX.clear();
      int stdevxsize;
      file >> stdevxsize;
      for ( int i= 0; i < stdevxsize; ++i )
      {
	file >> tmp;
	stdevX.push_back(tmp);
      }
      
      return 0;
    }
    
    return 1;
  }
}

#endif
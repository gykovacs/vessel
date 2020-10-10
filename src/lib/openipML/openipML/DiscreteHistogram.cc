#include <openipML/DiscreteHistogram.h>
#include <openipDS/Filter1s.h>
#include <openipDS/Filter2s.h>

#include <cmath>

namespace openip
{
  DiscreteHistogram::DiscreteHistogram(int n)
  : Vector<float>(), bins(n)
  {
  }
      
  DiscreteHistogram::DiscreteHistogram(const DiscreteHistogram& dh)
  : Vector<float>(dh), bins(dh.bins)
  {
  }

  DiscreteHistogram::~DiscreteHistogram()
  {
  }

  void DiscreteHistogram::normalize()
  {
    (*this)/= this->getSum();
  }
  
  void DiscreteHistogram::smoothWithGaussianKernel(float stdev)
  {
    GaussianFilter1<float, float> gf(stdev, 3*stdev);
    Vector<float> res;
    res.resize(this->size());
    for ( unsigned int i= 0; i < res.size(); ++i )
      res(i)= (*this)(i);
    gf.apply((*this), res);
    for ( unsigned int i= 0; i < this->size(); ++i )
      (*this)(i)= res(i);
  }
  
  DiscreteJointHistogram::DiscreteJointHistogram(int n)
  : Image<float>(), bins(n)
  {
  }
      
  DiscreteJointHistogram::DiscreteJointHistogram(const DiscreteJointHistogram& dh)
  : Image<float>(dh), bins(dh.bins)
  {
  }

  DiscreteJointHistogram::~DiscreteJointHistogram()
  {
  }

  void DiscreteJointHistogram::normalize()
  {
    (*this)/= this->getSum();
  }
  
  void DiscreteJointHistogram::smoothWithGaussianKernel(float stdev)
  {
    GaussianFilter2<float, float> gf(stdev, stdev*6);
    Image<float> res;
    res.resizeImage((*this));
    for ( unsigned int i= 0; i < res.n; ++i )
      res(i)= (*this)(i);
    gf.apply((*this), res);
    for ( unsigned int i= 0; i < res.n; ++i )
      (*this)(i)= res(i);
  }
  
  void DiscreteJointHistogram::getPeaksInColumns(Vector<int>& indices, Vector<float>& values)
  {
    indices.resize(this->columns);
    values.resize(this->columns);
    
    values= 0;
    
    for ( int i= 0; i < this->columns; ++i )
    {
      for ( int j= 0; j < this->rows; ++j )
	if ( (*this)(j,i) > values(i) )
	{
	  values(i)= (*this)(j,i);
	  indices(i)= j;
	}
    }
  }
  
  void DiscreteJointHistogram::getMarginalDistribution(int dim, Vector<float>& p)
  {
    if ( dim == 0 )
    {
      p.resize(this->columns);
      p= 0;
      
      for ( int i= 0; i < this->columns; ++i )
	for ( int j= 0; j < this->rows; ++j )
	  p(i)+= (*this)(j,i);
    }
    else
    {
      p.resize(this->rows);
      p= 0;
      
      for ( int i= 0; i < this->columns; ++i )
	for ( int j= 0; j < this->rows; ++j )
	  p(j)+= (*this)(j,i);
    }
  }
      
  float DiscreteJointHistogram::getShannonJointEntropy()
  {
    float result= 0;
    for ( unsigned int i= 0; i < this->n; ++i )
      if ( (*this)(i) > 0 )
	result+= -(*this)(i)*log2((*this)(i));
      
    return result;
  }
  
  float DiscreteJointHistogram::getShannonMarginalEntropy(int dim)
  {
    Vector<float> p;
    getMarginalDistribution(dim, p);
    
    float result= 0;
    for ( unsigned int i= 0; i < p.size(); ++i )
      if ( p(i) > 0 )
	result+= -p(i)*log2(p(i));
    
    return result;
  }
  
  float DiscreteJointHistogram::getShannonEntropy()
  {
    return getShannonMarginalEntropy(0) + getShannonMarginalEntropy(1) - getShannonJointEntropy();
  }
  
  float DiscreteJointHistogram::getRenyiJointEntropy(float alpha)
  {
    float result= 0;
    for ( unsigned int i= 0; i < this->n; ++i )
      result+= pow((*this)(i), alpha);
      
    return 1.0/(1.0 - alpha)*log2(result);
  }
  
  float DiscreteJointHistogram::getRenyiMarginalEntropy(float alpha, int dim)
  {
    Vector<float> p;
    getMarginalDistribution(dim, p);
    
    float result= 0;
    for ( unsigned int i= 0; i < p.size(); ++i )
      result+= pow(p(i), alpha);
    
    return 1.0/(1.0 - alpha)*log2(result);
  }
  
  float DiscreteJointHistogram::getRenyiEntropy(float alpha)
  {
    return (getRenyiMarginalEntropy(alpha, 0) + getRenyiMarginalEntropy(alpha, 1)) / getRenyiJointEntropy(alpha);
  }
  
  float DiscreteJointHistogram::getTsallisJointEntropy(float q)
  {
    float result= 0;
    for ( unsigned int i= 0; i < this->n; ++i )
      result+= pow((*this)(i), q);
      
    return 1.0/(q-1.0)*(1.0 - result);
  }
  
  float DiscreteJointHistogram::getTsallisMarginalEntropy(float q, int dim)
  {
    Vector<float> p;
    getMarginalDistribution(dim, p);
    
    float result= 0;
    for ( unsigned int i= 0; i < p.size(); ++i )
      result+= p(i)*(1 - pow(p(i), q-1));
    
    return 1.0/(q-1.0)*result;
  }
  
  float DiscreteJointHistogram::getTsallisEntropy(float q)
  {
    return (getTsallisMarginalEntropy(q, 0) + getTsallisMarginalEntropy(q, 1)) / getTsallisJointEntropy(q);
  }
  
  float DiscreteJointHistogram::getFInformationI(float alpha)
  {
    Vector<float> pc;
    getMarginalDistribution(0, pc);
    Vector<float> pr;
    getMarginalDistribution(1, pr);
    
    float result= 0;
    float tmp;
    for ( int i= 0; i < this->rows; ++i )
      for ( int j= 0; j < this->columns; ++j )
      {
	tmp= pow(pc(j)*pr(i), alpha-1);
	if ( tmp > 0 )
	  result+= pow((*this)(i,j), alpha)/tmp;
      }
    
    return 1.0/(alpha*(alpha-1))*(result-1);
  }
}

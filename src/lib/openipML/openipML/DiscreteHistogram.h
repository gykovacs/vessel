#ifndef DISCRETE_HISTOGRAM_H
#define DISCRETE_HISTOGRAM_H

#include <openipDS/Vector.h>
#include <openipDS/Image.h>

namespace openip
{
  class DiscreteHistogram: public Vector<float>
  {
    public:
      DiscreteHistogram(int n= -1);
      
      DiscreteHistogram(const DiscreteHistogram& dh);

      ~DiscreteHistogram();

      template<typename T>
      void computeHistogram(Vector<T>& data, int bins= -1, Vector<unsigned char>* mask= NULL);

      void normalize();
      
      void smoothWithGaussianKernel(float stdev= 3);
      
      int bins;
  };

  template<typename T>
  void DiscreteHistogram::computeHistogram(Vector<T>& data, int b, Vector<unsigned char>* mask)
  {
    float min= data.getMin(mask), max= data.getMax(mask);
    
    if ( b > 0 )
      bins= b;
     
    resize(bins);
    (*this)= 0;

    float d= (max - min)/bins;

    if ( d == 0 )
      (*this)(0)= data.numberOfNonZeroElements();

    int idx= 0;
    for ( int i= 0; i < data.size(); ++i )
    {
      if ( !mask || (*mask)(i) )
      {
	idx= floor((data(i) - min)/d);
	if ( idx < bins )
	  (*this)(idx)++;
	else
	  (*this)(idx-1)++;
      }
    }
  }

  class DiscreteJointHistogram: public Image<float>
  {
    public:
      DiscreteJointHistogram(int n= -1);
      
      DiscreteJointHistogram(const DiscreteJointHistogram& dh);

      ~DiscreteJointHistogram();

      template<typename T>
      void computeHistogram(Vector<T>& data0, Vector<T>& data1, int bins= -1, Vector<unsigned char>* mask= NULL);

      void normalize();
      
      void smoothWithGaussianKernel(float stdev= 3);
      
      void getPeaksInColumns(Vector<int>& indices, Vector<float>& values);
      
      void getMarginalDistribution(int dim, Vector<float>& p);
      
      float getShannonJointEntropy();
      
      float getShannonMarginalEntropy(int dim);
      
      float getShannonEntropy();
      
      float getRenyiJointEntropy(float alpha);
      
      float getRenyiMarginalEntropy(float alpha, int dim);
      
      float getRenyiEntropy(float alpha);
      
      float getTsallisJointEntropy(float q);
      
      float getTsallisMarginalEntropy(float q, int dim);
      
      float getTsallisEntropy(float q);
      
      float getFInformationI(float alpha);

      int bins;
  };

  template<typename T>
  void DiscreteJointHistogram::computeHistogram(Vector<T>& data0, Vector<T>& data1, int b, Vector<unsigned char>* mask)
  {
    float min0= data0.getMin(mask), max0= data0.getMax(mask);
    float min1= data1.getMin(mask), max1= data1.getMax(mask);
    
    if ( b > 0 )
      bins= b;
    
    resizeImage(bins, bins);
    
    this->Image<float>::operator=(0);
    
    float d0= (max0 - min0)/bins;
    float d1= (max1 - min1)/bins;

    int idx0= 0;
    int idx1= 0;

    for ( unsigned int i= 0; i < data0.size(); ++i )
    {
      if ( !mask || (*mask)(i) )
      {
	if ( d0 > 0 )
	  idx0= floor((data0(i) - min0)/d0);
	else
	  idx0= 0;

	if ( d1 > 0 )
	  idx1= floor((data1(i) - min1)/d1);
	else
	  idx1= 0;

	if ( idx0 < bins && idx1 < bins )
	  (*this)(idx0, idx1)++;
	else if ( idx0 < bins && idx1 >= bins )
	  (*this)(idx0, bins-1)++;
	else if ( idx0 >= bins && idx1 < bins )
	  (*this)(bins-1, idx1)++;
	else
	  (*this)(bins-1, bins-1)++;
      }
    }
  }
}

#endif
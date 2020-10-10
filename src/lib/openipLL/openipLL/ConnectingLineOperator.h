#ifndef CONNECTING_LINE_OPERATOR_H_
#define CONNECTING_LINE_OPERATOR_H_

#include <openipDS/Filter2s.h>
#include <openipDS/MatchedFilter2s.h>
#include <openipDS/FilterSet2.h>
#include <openipLL/imageIO.h>

namespace openip
{
  template<typename INPUT, typename OUTPUT>
    class ConnectingFilter2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;

        /**
         * constructor
         * @param stride stride of the images to apply on
         */
        ConnectingFilter2(float theta, int width, int length1, int length2, int mask, float eps, int stride= 4000);
	
	virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
	
	void getPositions(Image<unsigned char>& input, Vector<int>& results, int seed, int color);
	
	float theta;
	float width;
	float length1;
	float length2;
	float mask;
	float eps;
	int nf;
	int nb;
    };

    template<typename INPUT, typename OUTPUT>
    ConnectingFilter2<INPUT, OUTPUT>::ConnectingFilter2(float theta, int width, int length1, int length2, int mask, float eps, int stride)
    : Filter2<INPUT, OUTPUT, float>()
    {
        this->stride= stride;
        std::stringstream ss;
        ss << "ConnectingFilter2 " << theta << " " << width << " " << length1 << " " << length2 << " " << mask << " " << eps << " " << stride;
        descriptor= ss.str();
	
	this->theta= theta;
	this->width= width;
	this->length1= length1;
	this->length2= length2;
	this->mask= mask;
	this->eps= eps;
	this->stride= stride;
	nf= nb= 0;
	
	for ( int i= -length1 - mask; i < 0; ++i )
	{
	  int r= cos(theta)*i - sin(theta)*0;
	  int c= sin(theta)*i + cos(theta)*0;
	  int n= r*stride + c;
	  for ( unsigned int j= 0; j < this->size(); ++j )
	    if ( (*this)(j).first == n )
	      continue;
	  this->push_back(PositionWeightPair<float>(n, 1));
	}
	for ( int i= 1; i <= length2 + mask; ++i )
	{
	  int r= cos(theta)*i - sin(theta)*0;
	  int c= sin(theta)*i + cos(theta)*0;
	  int n= r*stride + c;
	  for ( unsigned int j= 0; j < this->size(); ++j )
	    if ( (*this)(j).first == n )
	      continue;
	  this->push_back(PositionWeightPair<float>(n, 2));
	}
	
	unsigned int j;
	for ( j= 0; j < this->size(); ++j )
	  if ( (*this)(j).first == 0 )
	  {
	    (*this)(j).second= 0;
	    break;
	  }
	if ( j == this->size() )
	  this->push_back(PositionWeightPair<float>(0, 0));
	
	this->computeMinMax();
    }
    
    template<typename INPUT, typename OUTPUT>
    OUTPUT ConnectingFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
      float mean1, stdev1, mean2, stdev2, mean0, stdev0;
      mean1= mean2= mean0= 0;
      stdev1= stdev2= stdev0= 0;
      /*int*/ /*n0,*/ /*n1,*/ /*n2;*/
      /*n0=*/ /*n1=*/ /*n2= 0;*/
      
      Vector<int> pos0;
      Vector<int> pos1;
      Vector<int> pos2;
      int seed1= -1;
      int seed2= 1000;
      
      if ( (*support)(n) != 0 )
	return 0;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	if ( fabs((*this)(i).second - 1) < 0.1 && (*support)(n + (*this)(i).first) == 255 )
	{
	  if ( int(i) > seed1 )
	    seed1= i;
	  (*support)(n + (*this)(i).first)= 252;
	}
	else if ( fabs((*this)(i).second - 1) < 0.1 && (*support)(n + (*this)(i).first) == 0 )
	{
	  (*support)(n + (*this)(i).first)= 254;
	}
      }
	
      for ( unsigned int i= 0; i < this->size(); ++i )
	if ( fabs((*this)(i).second - 2) < 0.1 && (*support)(n + (*this)(i).first) == 255 )
	{
	  if ( int(i) < seed2 )
	    seed2= i;
	  (*support)(n + (*this)(i).first)= 250;
	}
	else if ( fabs((*this)(i).second - 2) < 0.1 && (*support)(n + (*this)(i).first) == 0 )
	{
	  (*support)(n + (*this)(i).first)= 254;
	}
      (*support)(n)= 254;

      
      if ( seed1 == -1 || seed2 == 1000 )
      {
	for ( unsigned int i= 0; i < this->size(); ++i )
	if ( (*support)(n + (*this)(i).first) == 254 || (*support)(n + (*this)(i).first) == 253  )
	{
	  (*support)(n + (*this)(i).first)= 0;
	}
	else if ( (*support)(n + (*this)(i).first) <= 252 && (*support)(n + (*this)(i).first) > 248 )
	{
	  (*support)(n + (*this)(i).first)= 255;
	}
	return 0;
      }
      
      //printf("%d.%d.%d ", n, n + (*this)(seed1).first, n + (*this)(seed2).first); fflush(stdout);
      getPositions(*support, pos0, n, 254);
      getPositions(*support, pos1, n + (*this)(seed1).first, 252);
      getPositions(*support, pos2, n + (*this)(seed2).first, 250);
      
      for ( unsigned int i= 0; i < this->size(); ++i )
	if ( (*support)(n + (*this)(i).first) == 254 || (*support)(n + (*this)(i).first) == 253  )
	{
	  (*support)(n + (*this)(i).first)= 0;
	}
	else if ( (*support)(n + (*this)(i).first) <= 252 && (*support)(n + (*this)(i).first) > 248 )
	{
	  (*support)(n + (*this)(i).first)= 255;
	}

      //printf("%d.%d.%d ", pos0.size(), pos1.size(), pos2.size());
      if ( pos1.size() < 2 || pos2.size() < 2 || pos0.size() == 0 )
	return 0;
      
      for ( unsigned int i= 0; i < pos0.size(); ++i )
      {
	mean0+= input(pos0(i));
	stdev0+= input(pos0(i))*input(pos0(i));
      }
      for ( unsigned int i= 0; i < pos1.size(); ++i )
      {
	mean1+= input(pos1(i));
	stdev1+= input(pos1(i))*input(pos1(i));
      }

      for ( unsigned int i= 0; i < pos2.size(); ++i )
      {
	mean2+= input(pos2(i));
	stdev2+= input(pos2(i))*input(pos2(i));
      }
      
      mean0/= pos0.size();
      mean1/= pos1.size();
      mean2/= pos2.size();
      
      stdev0/= pos0.size();
      stdev1/= pos1.size();
      stdev2/= pos2.size();
      
      stdev0-= mean0*mean0;
      stdev1-= mean1*mean1;
      stdev2-= mean2*mean2;
      
      stdev0= sqrt(stdev0);
      stdev1= sqrt(stdev1);
      stdev2= sqrt(stdev2);
      
      if ( fabs(mean0 - mean1) < eps && fabs(mean1 - mean2) < eps && fabs(mean2 - mean0) < eps )
	return 1;
      
      return 0;
    }
    
    template<typename INPUT, typename OUTPUT>
    void ConnectingFilter2<INPUT, OUTPUT>::getPositions(Image<unsigned char>& input, Vector<int>& results, int seed, int color)
    {
      Vector<int> tmp;
      Vector<int> pos;
      pos.push_back(1);
      pos.push_back(-1);
      pos.push_back(input.columns);
      pos.push_back(-input.columns);
      pos.push_back(1 + input.columns);
      pos.push_back(1 - input.columns);
      pos.push_back(-1 + input.columns);
      pos.push_back(-1 - input.columns);
      
      results.clear();
      tmp.push_back(seed);
      
      while ( tmp.size() != 0 )
      {
	//if ( input(tmp(0)) == color )
	{
	  input(tmp(0))= color-1;
	  results.push_back(tmp(0));
	  
	  for ( unsigned int i= 0; i < pos.size(); ++i )
	    if ( input(tmp(0)+pos(i)) == color )
	    {
	      tmp.push_back(tmp(0)+pos(i));
	      input(tmp(0)+pos(i))= color-1;
	    }
	}
	tmp.erase(tmp.begin());
      }
    }
    
    template<typename INPUT, typename OUTPUT>
    class ConnectingDirectionFilter2: public Filter2<INPUT, OUTPUT, float>
    {
    public:
        using Filter2<INPUT, OUTPUT, float>::stride;
        using Filter2<INPUT, OUTPUT, float>::updateStride;
        using Filter2<INPUT, OUTPUT, float>::apply;
        using Filter2<INPUT, OUTPUT, float>::descriptor;

        /**
         * constructor
         * @param stride stride of the images to apply on
         */
        ConnectingDirectionFilter2(float theta, int width, int length1, int length2, int mask, float eps, int stride= 4000);
	
	virtual OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
	
	float theta;
	float width;
	float length1;
	float length2;
	float mask;
	float eps;
	int nf;
	int nb;
    };

    template<typename INPUT, typename OUTPUT>
    ConnectingDirectionFilter2<INPUT, OUTPUT>::ConnectingDirectionFilter2(float theta, int width, int length1, int length2, int mask, float eps, int stride)
    : Filter2<INPUT, OUTPUT, float>()
    {
        this->stride= stride;
        std::stringstream ss;
        ss << "ConnectingDirectionFilter2 " << theta << " " << width << " " << length1 << " " << length2 << " " << mask << " " << eps << " " << stride;
        descriptor= ss.str();
	
	this->theta= theta;
	this->width= width;
	this->length1= length1;
	this->length2= length2;
	this->mask= mask;
	this->eps= eps;
	this->stride= stride;
	nf= nb= 0;
	
	for ( int i= -length1 - mask; i <= length2 + mask; ++i )
	{
	  int r= cos(theta)*i - sin(theta)*0;
	  int c= sin(theta)*i + cos(theta)*0;
	  this->push_back(PositionWeightPair<float>(r*stride + c, 0));
	}
	
	this->computeMinMax();
    }
    
    template<typename INPUT, typename OUTPUT>
    OUTPUT ConnectingDirectionFilter2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* /*support*/)
    {
      float mean= 0;
      float stdev= 0;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	mean+= input(n + (*this)(i).first);
	stdev+= input(n + (*this)(i).first)*input(n + (*this)(i).first);
      }
      
      mean/= this->size();
      stdev/= this->size();
      
      stdev-= mean*mean;
      stdev= sqrt(stdev);
      
      return stdev;
    }
  
  
    template<typename INPUT, typename OUTPUT>
    class ConnectingLineOperator2
    {
    public:
	string descriptor;

        /**
         * constructor
         * @param kernel kernel size
         * @param sigma variance
         * @param alpha alpha parameter
         * @param beta beta parameter
         * @param delta delta parameter
         * @param gamma gamma parameter
         * @param stride stride of the images to apply to
         */
        ConnectingLineOperator2(float theta0, float thetas, float theta1, float l0, float ls, float l1, float w0, float ws, float w1, float m0, float ms, float m1, float eps, int stride= 4000);

        /**
         * copy constructor
         * @param m instance to copy
         */
        ConnectingLineOperator2(const ConnectingLineOperator2& m);
	
	virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
	
	virtual int apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);
	
	void updateStride(int stride);
	
        /** stride */
        int stride;
        /** kernel size */
	float theta0;
	float thetas;
	float theta1;
	
        float l0;
	float ls;
	float l1;
	
	float w0;
	float ws;
	float w1;
	
	float m0;
	float ms;
	float m1;
	
	float eps;
	
	FilterSet2<float, float, float> directionFilters;
	FilterSet2<float, float, float> connectingFilters;
    };

    
    template<typename INPUT, typename OUTPUT>
    ConnectingLineOperator2<INPUT, OUTPUT>::ConnectingLineOperator2(float theta0, float thetas, float theta1, float l0, float ls, float l1, float w0, float ws, float w1, float m0, float ms, float m1, float eps, int stride)
    {
        stringstream ss;
        ss << "ConnectingLineOperator2 " << theta0 << " " << thetas << " " << theta1 << " " << l0 << " " << ls << " " << l1 << " " << w0 << " " << ws << " " << w1 << " " << m0 << " " << ms << " " << m1 << " " << eps << " " << stride;
        descriptor= ss.str();

	this->theta0= theta0;
	this->thetas= thetas;
	this->theta1= theta1;
	this->l0= l0;
	this->ls= ls;
	this->l1= l1;
	this->w0= w0;
	this->ws= ws;
	this->w1= w1;
	this->m0= m0;
	this->ms= ms;
	this->m1= m1;
	this->eps= eps;
	
	for ( float t= theta0; t <= theta1; t+= thetas )
	  for ( float l= l0; l <= l1; l+= ls )
	    for ( float ll= l0; ll <= l1; ll+= ls )
	      for ( float w= w0; w <= w1; w+= ws )
		for ( float m= m0; m <= m1; m+= ms )
		{
		  connectingFilters.push_back(new ConnectingFilter2<float, float>(t, w, l, ll, m, eps, stride) );
		  directionFilters.push_back(new ConnectingDirectionFilter2<float, float>(t, w, l, ll, m, eps, stride) );
		}
    }

    template<typename INPUT, typename OUTPUT>
    ConnectingLineOperator2<INPUT, OUTPUT>::ConnectingLineOperator2(const ConnectingLineOperator2& m)
    {
    }
    
    template<typename INPUT, typename OUTPUT>
    void ConnectingLineOperator2<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& /*output*/, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
      Image<unsigned char> result;
      result.resizeImage(*support);
      result= *support;
      
      directionFilters.updateStride(input.columns);
      directionFilters.computeMinMax();
      connectingFilters.updateStride(input.columns);
      connectingFilters.computeMinMax();
      int min= directionFilters.getMin();
      int max= directionFilters.getMax();
      
      printf("%d %d\n", min, max);
      for ( int i= -min; i < int(input.n) - max; ++i )
      {
	if ((*roi)(i))
	{
	  /*int direction= -1;
	  float stdev= FLT_MAX;
	  for ( int k= 0; k < directionFilters.size(); ++k )
	  {
	    float tmp;
	    tmp= directionFilters[k]->apply(input, i, support);
	    if ( tmp < stdev )
	    {
	      stdev= tmp;
	      direction= k;
	    }
	  }*/

	  //printf("%f,%d ", stdev, direction); fflush(stdout);
	  //printf("%d ", i); fflush(stdout);
	  for ( unsigned int k= 0; k < connectingFilters.size(); ++k )
	  {
	    //printf("%d ", k); fflush(stdout);
	    if ( connectingFilters[k]->apply(input, i, support) > 0.1 )
	    {
	      result(i)= 255;
	      break;
	    }
	  }
	  
	}
      }
      
      *support= result;
    }

    template<typename INPUT, typename OUTPUT>
    int ConnectingLineOperator2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {
      /*int minDir;
      float minValue= FLT_MAX;
      float tmp;*/
      /*for ( int k= 0; k < connectingFilters.size(); ++k )
      {
	 tmp= directionFilters[k]->apply(input, n, support);
	 if ( tmp < minValue )
	 {
	   minValue= tmp;
	   minDir= k;
	 }
      }*/
      
      //printf("%d,%d,%f ", minDir, directionFilters.size(), minValue); fflush(stdout);
      
      for ( unsigned int k= 0; k < connectingFilters.size(); ++k )
	if ( connectingFilters[k]->apply(input, n, support) > 0.1 )
	  return 1;
      return 0;
    }
    
    template<typename INPUT, typename OUTPUT>
    void ConnectingLineOperator2<INPUT, OUTPUT>::updateStride(int stride)
    {
      directionFilters.updateStride(stride);
      connectingFilters.updateStride(stride);
      directionFilters.computeMinMax();
      connectingFilters.computeMinMax();
    }
}

#endif
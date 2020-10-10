#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <vector>
#include <set>
#include <stdio.h>
#include <math.h>

#include <openipDS/Image.h>
#include <openipDS/mathFunctions.h>
#include <openipDS/StructuringElement2.h>
#include <openipDS/PositionWeightPair.h>
#include <openipDS/drawing.h>
#include <openipDS/Set.h>
#include <openipDS/Pixel.h>

namespace openip
{
    template<typename WEIGHTS>
    class Template : public Set<PositionWeightPair<WEIGHTS> >
    {
    public:
        using Set<PositionWeightPair<WEIGHTS> >::size;
        Image<float> image;
        /**
         * Filter::iterator typedef
         */
        typedef typename Set<PositionWeightPair<WEIGHTS> >::iterator fIt;

        /**
         * Filter::const_iterator typedef
         */
        typedef typename Set<PositionWeightPair<WEIGHTS> >::const_iterator const_fIt;

        /**
         * default constructor
         */
        Template();

        /**
         * copy constructor
         * @param f Filter object to copy
         */
        Template(const Template& f);

	template<typename T>
	Template(Vector<T>& temp, int center= -1);
	
        /**
         * destructor
         */
        virtual ~Template();

        /**
         * adds a new element to the filter
         * @param pos relative position to 0 in row-continuous representation
         * @param weight weight of the element
         */
        void addElement(int pos, WEIGHTS weight);
	
	void add(int pos, WEIGHTS weight);

        /**
         * resizes the parameter and dumps the filter in it
         * @param im output parameter, puts the elements of the filter in it
         */
        void dumpTemplate(Vector<WEIGHTS>& im);

        /**
         * computes the min and max values, representing the smallest and
         * greatest pixel positions relative to 0
         */
        virtual void computeMinMax();

        /**
         * normalizes the sum of filter elements to 1
         */
        virtual void normalize();

        /**
         * transforms the weights to have zero mean
         */
        virtual void zeroMean();

        virtual void zeroMiddle();

        virtual void add(WEIGHTS w);
	
        /**
         * multiplies the weights of the object by -1
         */
        virtual void invert();

        virtual void getWeights(Vector<WEIGHTS>& v);

        virtual void setWeights(Vector<WEIGHTS>& v);
	
	virtual int getMin();
	
	virtual int getMax();
	
	virtual Border1 getProposedBorder1();
	
	int min;
	
	int max;
	
	float normalizer;
	
	Vector<float> descriptors;
	float getDescriptor(int i);
	
	virtual void computeDescriptors();
    };
    
    template<typename WEIGHTS>
    Template<WEIGHTS>::Template()
    : Set<PositionWeightPair<WEIGHTS> >()
    {
      normalizer= -1;
    }

    template<typename WEIGHTS>
    Template<WEIGHTS>::Template(const Template& f)
    : Set<PositionWeightPair<WEIGHTS> >(f), normalizer(f.normalizer)
    {
    }

    template<typename WEIGHTS> template<typename T>
    Template<WEIGHTS>::Template(Vector<T>& temp, int center)
    : Set<PositionWeightPair<WEIGHTS> >(), normalizer(-1)
    {
      if ( center == -1 )
	for ( int i= 0; i < temp.size(); ++i )
	  this->push_back(PositionWeightPair<WEIGHTS>(i - temp.size()/2, WEIGHTS(temp(i))));
      else
	for ( int i= 0; i < temp.size(); ++i )
	  this->push_back(PositionWeightPair<WEIGHTS>(i - center, WEIGHTS(temp(i))));
    }
    
    template<typename WEIGHTS>
    Template<WEIGHTS>::~Template()
    {
    }

    template<typename WEIGHTS>
    void Template<WEIGHTS>::addElement(int pos, WEIGHTS weight)
    {
        this->push_back(PositionWeightPair<WEIGHTS>(pos, weight));
    }

    template<typename WEIGHTS>
    void Template<WEIGHTS>::add(int pos, WEIGHTS weight)
    {
        addElement(pos, weight);
    }

    template<typename WEIGHTS>
    void Template<WEIGHTS>::normalize()
    {
        float sum= 0;
        typename Template<WEIGHTS>::fIt mit= this->begin();
        while( mit != this->end() )
        {
            sum+= fabs((*mit).second);
            ++mit;
        }
        mit= this->begin();
        while( mit != this->end() )
        {
            (*mit).second/= sum;
            ++mit;
        }
    }

    template<typename WEIGHTS>
    void Template<WEIGHTS>::zeroMean()
    {
        float sum= 0;
        typename Template<WEIGHTS>::fIt mit= this->begin();
        for ( ; mit != this->end(); ++mit )
            sum+= (*mit).second;
        sum/= this->size();
        for ( mit= this->begin(); mit != this->end(); ++mit )
            (*mit).second-= sum;
    }

    template<typename WEIGHTS>
    void Template<WEIGHTS>::zeroMiddle()
    {
        float middle= 0;
        typename Template<WEIGHTS>::fIt mit= this->begin();
        for ( ; mit != this->end(); ++mit )
            if ( (*mit).first == 0 )
                middle= (*mit).second;

        for ( mit= this->begin(); mit != this->end(); ++mit )
            (*mit).second-= middle;
    }

    template<typename WEIGHTS>
    void Template<WEIGHTS>::add(WEIGHTS w)
    {
        typename Template<WEIGHTS>::fIt mit;

        for ( mit= this->begin(); mit != this->end(); ++mit )
            (*mit).second+= w;
    }

    template<typename WEIGHTS>
    void Template<WEIGHTS>::invert()
    {
        typename Template<WEIGHTS>::fIt mit= this->begin();
        for ( ; mit != this->end(); ++mit )
            (*mit).second= (*mit).second * (-1);
    }

    template<typename WEIGHTS>
    void Template<WEIGHTS>::getWeights(Vector<WEIGHTS>& v)
    {
        v.resize(this->size());
        for ( unsigned int i= 0; i < v.size(); ++i )
            v(i)= (*this)(i).second;
    }

    template<typename WEIGHTS>
    void Template<WEIGHTS>::setWeights(Vector<WEIGHTS>& v)
    {
        if ( this->size() != v.size() )
            return;
        for ( unsigned int i= 0; i < v.size(); ++i )
            (*this)(i).second= v(i);
    }
    
    template<typename WEIGHTS>
    void Template<WEIGHTS>::computeMinMax()
    {
        //printf("cmm\n"); fflush(stdout);
        int min= INT_MAX;
        int max= -INT_MAX;

        //printf("cmm iter \n"); fflush(stdout);
        typename Template<WEIGHTS>::fIt mit= this->begin();
        while( mit != this->end() )
        {
            if ( (*mit).first < min )
                min= (*mit).first;
            if ( (*mit).first > max )
                max= (*mit).first;
            ++mit;
        }
        //printf("cmm iter finished\n"); fflush(stdout);
        if ( abs(min) < abs(max) )
        {
            min= -max;
        }
        else
        {
            max= -min;
        }
        //printf("cmm critical %d\n", omp_get_thread_num()); fflush(stdout);
//#pragma omp critical
{
        this->min= min;
        this->max= max;
}
        //printf("cmm critical finished\n"); fflush(stdout);
    }
    
    template<typename WEIGHTS>
    void Template<WEIGHTS>::dumpTemplate(Vector<WEIGHTS>& v)
    {
    }
    
    template<typename WEIGHTS>
    Border1 Template<WEIGHTS>::getProposedBorder1()
    {
      computeMinMax();
      return Border1(-min, max, BORDER_MODE_ZERO);
    }
    
    template<typename WEIGHTS>
    int Template<WEIGHTS>::getMin()
    {
      computeMinMax();
      int m;
//#pragma omp critical
      m= this->min;
      return m;
    }
    
    template<typename WEIGHTS>
    int Template<WEIGHTS>::getMax()
    {
      computeMinMax();
      int m;
//#pragma omp critical
      m= this->max;
      return m;
    }
    
    template<typename WEIGHTS>
    float Template<WEIGHTS>::getDescriptor(int i)
    {
      return descriptors(i);
    }
    
    template<typename WEIGHTS>
    void Template<WEIGHTS>::computeDescriptors()
    {
      float mean= 0, mean2= 0;
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	mean+= (*this)(i).second;
	mean+= ((*this)(i).second)*((*this)(i).second);
      }
      mean/= this->size();
      mean2/= this->size();
      
      descriptors.push_back(mean);
      descriptors.push_back(sqrt(mean2 - mean*mean));
      descriptors.push_back(mean2*this->size());
    }
    
}
#endif

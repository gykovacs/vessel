/* 
 * File:   SimilarityFunction.h
 * Author: gykovacs
 *
 * Created on 2011. február 21., 23:06
 */

#ifndef SIMILARITYFUNCTION_H
#define	SIMILARITYFUNCTION_H

#include <openipDS/Template.h>
#include <openipDS/Template2.h>
#include <openipDS/Image.h>
#include <openipML/BivariateFunction.h>

namespace openip
{
    template<typename T>
    class SimilarityFunction: public BivariateFunction<T>
    {
    public:
        using BivariateFunction<T>::descriptor;

        SimilarityFunction();

        SimilarityFunction(const SimilarityFunction& b);

        virtual ~SimilarityFunction();
	
        virtual double evaluate(Vector<T>& a, Vector<T>& b);
	
	virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
	
	virtual void apply1(Vector<T>& input, Template<T>& temp, Vector<T>& output, Vector<unsigned char>* roi= NULL, Vector<unsigned char>* support= NULL);
	
	virtual void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
	virtual void init(Template<T>& b);
	
	virtual SimilarityFunction<T>* clone();
        
        std::string getDescriptor(char space= ' ', int i= 0);
	
	Template<T>* lastTemplate;
	int initialized;
    };

    template<typename T>
    std::string SimilarityFunction<T>::getDescriptor(char space, int i)
    {
        if ( i == 0 )
        {
            std::string a= this->descriptor;
            for ( unsigned int i= 0; i < a.size(); ++i )
                if ( a[i] == ' ' )
                    a[i]= space;
            return a;
        }
        else
            return std::string("padding");
    }
    
    template<typename T>
    SimilarityFunction<T>::SimilarityFunction()
    : BivariateFunction<T>()
    {
      lastTemplate= NULL;
      initialized= 0;
    }

    template<typename T>
    SimilarityFunction<T>::SimilarityFunction(const SimilarityFunction& b)
    : BivariateFunction<T>(b)
    {
      lastTemplate= b.lastTemplate;
      initialized= b.initialized;
    }

    template<typename T>
    SimilarityFunction<T>::~SimilarityFunction<T>()
    {
    }

    template<typename T>
    SimilarityFunction<T>* SimilarityFunction<T>::clone()
    {
      return NULL;
    }
    
    template<typename T>
    double SimilarityFunction<T>::evaluate(Vector<T>&, Vector<T>&)
    {
        return 0.0;
    }
    
    template<typename T>
    double SimilarityFunction<T>::evaluate(Vector<T>& /*a*/, Template<T>& /*b*/, int /*n*/)
    {
      return 0.0;
    }
    
    template<typename T>
    void SimilarityFunction<T>::apply1(Vector<T>& input, Template<T>& temp, Vector<T>& output, Vector<unsigned char>* /*roi*/, Vector<unsigned char>* /*support*/)
    {
      for ( int i= -temp.getMin(); i < int(input.size()) - temp.getMax(); ++i )
	output(i)= evaluate(input, temp, i);
    }
    
    template<typename T>
    void SimilarityFunction<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
    {
      printf("zzz"); fflush(stdout);
      temp.updateStride(input.columns);
      printf("lll"); fflush(stdout);
      for ( int i= -temp.getMin(); i < int(input.size()) - temp.getMax(); ++i )
	output(i)= evaluate(input, temp, i);
      printf("hhh"); fflush(stdout);
    }
    
    template<typename T>
    void SimilarityFunction<T>::init(Template<T>& /*b*/)
    {
    }
    
    template<typename T>
    class DissimilarityFunction: public BivariateFunction<T>
    {
    public:
        using BivariateFunction<T>::descriptor;

        DissimilarityFunction();
	
	DissimilarityFunction(SimilarityFunction<T>* sf);

        DissimilarityFunction(const DissimilarityFunction& b);
	
        virtual ~DissimilarityFunction();

        virtual double evaluate(Vector<T>& a, Vector<T>& b);
	
	virtual double evaluate(Vector<T>& a, Template<T>& b, int n);
	
	virtual void apply1(Vector<T>& input, Template<T>& temp, Vector<T>& output, Vector<unsigned char>* roi= NULL, Vector<unsigned char>* support= NULL);
	
	virtual void apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
        
        virtual void apply2(Image<T>& input, Image<T>& temp, Image<T>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);
	
	virtual DissimilarityFunction<T>* clone();
        
        virtual std::string getDescriptor(char space= ' ', int i= 0);	
        
	Template<T>* lastTemplate;
	int initialized;
	SimilarityFunction<T>* sf;
        bool flag;
    };

    template<typename T>
    std::string DissimilarityFunction<T>::getDescriptor(char space, int i)
    {
        if ( i == 0 )
        {
            std::string a= this->descriptor;
            for ( unsigned int i= 0; i < a.size(); ++i )
                if ( a[i] == ' ' )
                    a[i]= space;
            return a;
        }
        else
            return std::string("padding");
    }
    
    template<typename T>
    DissimilarityFunction<T>::DissimilarityFunction()
    : BivariateFunction<T>()
    {
      lastTemplate= NULL;
      initialized= 0;
      sf= NULL;
      flag= false;
    }
    
    template<typename T>
    DissimilarityFunction<T>::DissimilarityFunction(SimilarityFunction<T>* sf)
    : BivariateFunction<T>()
    {
      lastTemplate= NULL;
      initialized= 0;
      this->sf= sf->clone();
      stringstream ss;
      ss << string("SimilarityFunctionBasedDissimilarityFunction ") << sf->descriptor;
      this->descriptor= ss.str();
      flag= false;
    }

    template<typename T>
    DissimilarityFunction<T>::DissimilarityFunction(const DissimilarityFunction& b)
    : BivariateFunction<T>(b)
    {
      lastTemplate= b.lastTemplate;
      initialized= b.initialized;
      sf= b.sf;
      flag= b.flag;
    }

    template<typename T>
    DissimilarityFunction<T>::~DissimilarityFunction<T>()
    {
      if ( sf != NULL )
	delete sf;
    }

    template<typename T>
    double DissimilarityFunction<T>::evaluate(Vector<T>& a, Vector<T>& b)
    {
      if ( sf == NULL )
	return 0.0;
      else
	return -sf->evaluate(a, b);
    }
    
    template<typename T>
    double DissimilarityFunction<T>::evaluate(Vector<T>& a, Template<T>& b, int n)
    {
      if ( sf == NULL )
	return 0.0;
      else
      {
	return -sf->evaluate(a, b, n);
      }
    }
    
    template<typename T>
    void DissimilarityFunction<T>::apply1(Vector<T>& input, Template<T>& temp, Vector<T>& output, Vector<unsigned char>* /*roi*/, Vector<unsigned char>* /*support*/)
    {
        {
            for ( int i= -temp.getMin(); i < int(input.size()) - temp.getMax(); ++i )
                output(i)= evaluate(input, temp, i);
        }
    }
    
    template<typename T>
    void DissimilarityFunction<T>::apply2(Image<T>& input, Template2<T>& temp, Image<T>& output, Image<unsigned char>* roi, Image<unsigned char>* /*support*/)
    {
      //int r= 130;
      //int c= 81;
        cout << descriptor << endl;
        printf("input.columns: %d\n", input.columns);
        temp.updateStride(input.columns);
        
        int begin= -temp.getMin();
        int end= int(input.size()) - temp.getMax();
        
        Vector<unsigned char> mask;
        mask.resize(end - begin);
        mask= 0;
        //printf("%d %d\n", begin, end);

        #pragma omp parallel
        {
            printf("before clone\n"); fflush(stdout);
            DissimilarityFunction<T>* t= this->clone();
            printf("after clone\n"); fflush(stdout);
            int j= 0;
            #pragma omp for
            for ( int i= begin; i < end; ++i )
            {
                //printf("a%d ", omp_get_thread_num()); fflush(stdout);
                if ( j == 0 )
                {
                    //printf("%d\n", i);
                    j++;
                }
                /*int rr, cc;
                input.getCoordinate2D(i, rr, cc);
                rr-= input.topBorder;
                cc-= input.leftBorder;
                if ( rr == r && cc == c )
                {
                    flag= true;
                }
                else
                    flag= false;*/
                //#pragma omp critical
                {
                        mask(i - begin)++;
                }
                //printf("i%d/%d ", end - begin, i - begin); fflush(stdout);
                if ( input.isRealImagePixel(i) && (!roi || (*roi)(i)) )
                    output(i)= t->evaluate(input, temp, i);
                //printf("r%d ", omp_get_thread_num()); fflush(stdout);
            }
            //printf("fine\n"); fflush(stdout);
            delete t;
        }
        
        printf("finished\n"); fflush(stdout);
        
        /*for ( unsigned int i= 0; i < mask.size(); ++i )
            if ( mask(i) != 1 )
                printf("%d,%d ", i, mask(i));*/
    }
    
    template<typename T>
    void DissimilarityFunction<T>::apply2(Image<T>& input, Image<T>& temp, Image<T>& output, Image<unsigned char>* /*roi*/, Image<unsigned char>* /*support*/)
    {
      
    }
    
    template<typename T>
    DissimilarityFunction<T>* DissimilarityFunction<T>::clone()
    {
      if ( sf != NULL )
      {
	return new DissimilarityFunction<T>(sf->clone());
      }
      else
      {
	return NULL;
      }
    }
    
  template<typename T>
  class IndexWeightPair
  {
  public:
    IndexWeightPair(int i, T w)
    : index(i), weight(w)
    {
    }
    
    IndexWeightPair(const IndexWeightPair& i)
    : index(i.index), weight(i.weight)
    {
    }
    
    IndexWeightPair& operator=(const IndexWeightPair& i)
    {
      index= i.index;
      weight= i.weight;
      return (*this);
    }
    
    bool operator< (const IndexWeightPair& i) const
    {
      return weight < i.weight;
    }
    
    bool operator<= (const IndexWeightPair& i) const
    {
      return weight <= i.weight;
    }
    
    bool operator> (const IndexWeightPair& i) const
    {
      return weight > i.weight;
    }
    
    bool operator>= (const IndexWeightPair& i) const
    {
      return weight >= i.weight;
    }
    
    int index;
    T weight;
  };
}

#endif	/* SIMILARITYFUNCTION_H */


#ifndef TEMPLATE2_H
#define TEMPLATE2_H

#include <openipDS/mathFunctions.h>
#include <openipDS/PositionWeightPair.h>
#include <openipDS/drawing.h>
#include <openipDS/Set.h>
#include <openipDS/Template.h>

namespace openip
{
    template<typename WEIGHTS>
    class Template2 : public Template<WEIGHTS>
    {
    public:
        using Template<WEIGHTS>::size;
	using Template<WEIGHTS>::normalize;
	using Template<WEIGHTS>::push_back;
	using Template<WEIGHTS>::zeroMean;
	using Template<WEIGHTS>::zeroMiddle;
	using Template<WEIGHTS>::add;
	using Template<WEIGHTS>::invert;
	using Template<WEIGHTS>::getWeights;
	using Template<WEIGHTS>::setWeights;
	using Template<WEIGHTS>::getMin;
	using Template<WEIGHTS>::getMax;
	using Template<WEIGHTS>::computeMinMax;
	using Template<WEIGHTS>::normalizer;
        using Template<WEIGHTS>::image;
	
        /**
         * Filter::iterator typedef
         */
        typedef typename Template<WEIGHTS>::iterator fIt;

        /**
         * Filter::const_iterator typedef
         */
        typedef typename Template<WEIGHTS>::const_iterator const_fIt;

        /**
         * default constructor
	  */
        Template2(int stride= 3000);

        /**
         * copy constructor
         * @param f Filter object to copy
         */
        Template2(const Template2& f);

	template<typename T>
	Template2(Image<T>& data, int crow= -1, int ccol= -1);
	
        /**
         * destructor
         */
        virtual ~Template2();

        /**
         * get number of rows in the filter elements
         * @return number of rows (height)
         */
        virtual int getRows();

        /**
         * get number of columns in the filter elements
         * @return number of columns (width)
         */
        virtual int getColumns();

        /**
         * resizes the parameter and dumps the filter in it
         * @param im output parameter, puts the elements of the filter in it
         */
        void dumpTemplate(Image<WEIGHTS>& im);

	virtual int updateStride(int stride);
	
	virtual Border2 getProposedBorder();
	
	virtual Border2 getProposedBorder2();
	
	virtual void getCoordinate2D(int n, int& rows, int& columns);
        
        void applyGeometricalTransformation(int type, float eps);
	
        /**
         * stride of the images on which the filter can be applied in
         * image space
         */
        int stride;
	
	Vector<int> rowCache;
	Vector<int> columnCache;
	
	int cacheinit;
    };
    
    template<typename WEIGHTS>
    void Template2<WEIGHTS>::applyGeometricalTransformation(int type, float eps)
    {
        Vector<float> tr;
        Vector<float> tc;
        Vector<float> tw;
        
        float aaa, bbb, ccc, ddd;
        
        for ( int i= 0; i < this->size(); ++i )
        {
            int r, c;
            this->getCoordinate2D(this->operator()(i).first, r, c);
            float rr, cc;
            float d= sqrt(r*r + c*c);
            float size= this->stride;
            float factor= exp(-2*d/size);

            if ( type == 0 )
            {
                printf("===============ROTATION %f\n", eps);
                aaa = 1.0 - eps*factor;
                bbb= -eps*factor;
                ccc= eps*factor;
                ddd= 1.0 - eps*factor;
            }
            else if ( type == 1 )
            {
                printf("===============SCALING %f\n", eps);
                if ( rand() % 2 )
                    aaa= 1.0 + eps*factor;
                else
                    aaa= 1.0 - eps*factor;
                bbb= 0;
                ccc= 0;
                if ( rand() % 2 )
                    ddd= 1.0 + eps*factor;
                else
                    ddd= 1.0 - eps*factor;
            }
            else if ( type == 2 )
            {
                printf("================SKEW %f\n", eps);
                aaa= 1.0;
                bbb= eps*factor;
                ccc= 0;
                ddd= 1.0;
            }
            
            rr= aaa*r + bbb*c;
            cc= ccc*r + ddd*c;
            
            tr.push_back(rr);
            tc.push_back(cc);
            tw.push_back(this->operator()(i).second);
            printf("%d,%d,%d,%f ", i, r, c, this->operator()(i).second);
        }
        
        printf("\n\n");
        
        for ( int i= 0; i < this->size(); ++i )
        {
            int r, c;
            this->getCoordinate2D(this->operator()(i).first, r, c);
            
            Vector<float> dists;
            
            for ( int j= 0; j < tr.size(); ++j )
            {
                float d= (r - tr(j))*(r - tr(j)) + (c - tc(j))*(c - tc(j));
                d= sqrt(d);
                dists.push_back(d);
            }
            
            Vector<int> closest;
            Vector<float> mindist;
            Vector<float> minr;
            Vector<float> minc;
            int minIdx= 0;
            for ( int j= 0; j < dists.size(); ++j )
            {
                if ( dists(j) < dists(minIdx) )
                    minIdx= j;
            }
            mindist.push_back(dists(minIdx));
            minr.push_back(tr(minIdx));
            minc.push_back(tc(minIdx));
            closest.push_back(minIdx);
            dists(minIdx)= FLT_MAX;
            
            for ( int j= 0; j < dists.size(); ++j )
            {
                if ( dists(j) < dists(minIdx) )
                    minIdx= j;
            }
            mindist.push_back(dists(minIdx));
            minr.push_back(tr(minIdx));
            minc.push_back(tc(minIdx));
            closest.push_back(minIdx);
            dists(minIdx)= FLT_MAX;
            
            for ( int j= 0; j < dists.size(); ++j )
            {
                if ( dists(j) < dists(minIdx) )
                    minIdx= j;
            }
            mindist.push_back(dists(minIdx));
            minr.push_back(tr(minIdx));
            minc.push_back(tc(minIdx));
            closest.push_back(minIdx);
            dists(minIdx)= FLT_MAX;
            
            for ( int j= 0; j < dists.size(); ++j )
            {
                if ( dists(j) < dists(minIdx) )
                    minIdx= j;
            }
            mindist.push_back(dists(minIdx));
            minr.push_back(tr(minIdx));
            minc.push_back(tc(minIdx));
            closest.push_back(minIdx);
            dists(minIdx)= FLT_MAX;
            
            float value= 0, norm= 0;
            for ( int j= 0; j < closest.size(); ++j )
            {
                value+= exp(-mindist(j)*mindist(j)) * tw(closest(j));
                norm+= exp(-mindist(j)*mindist(j));
            }
            
            printf("%d, %d ", r, c);
            
            printf("%d,%f,%f,%f,%f ", closest(0), mindist(0), tw(closest(0)), minr(0), minc(0));
            printf("%d,%f,%f,%f,%f ", closest(1), mindist(1), tw(closest(1)), minr(1), minc(1));
            printf("%d,%f,%f,%f,%f ", closest(2), mindist(2), tw(closest(2)), minr(2), minc(2));
            printf("%d,%f,%f,%f,%f ", closest(3), mindist(3), tw(closest(3)), minr(3), minc(3));
            
            printf("%f ", this->operator()(i).second);
            
            value/= norm;
            this->operator()(i).second= value;
            
            printf("=> %f \n", value);
            this->image(i)= value;
            //printf("%d,%d,%f ", r, c, this->operator()(i).second);
        }
    }
    
    template<typename WEIGHTS>
    Template2<WEIGHTS>::Template2(int s)
    : Template<WEIGHTS>(), stride(s), cacheinit(0)
    {
    }
    
    template<typename WEIGHTS>
    Template2<WEIGHTS>::Template2(const Template2& t)
    : Template<WEIGHTS>(t), stride(t.stride), rowCache(t.rowCache), columnCache(t.columnCache), cacheinit(t.cacheinit)
    {
        image.resizeImage(t.image);
        image= t.image;
    }
    
    template<typename WEIGHTS> template<typename T>
    Template2<WEIGHTS>::Template2(Image<T>& data, int crow, int ccol)
    : Template<WEIGHTS>(), stride(3000), cacheinit(0)
    {
        if ( ccol == -1 && crow == -1 )
            for ( int i= 0; i < data.rows; ++i )
                for ( int j= 0; j < data.columns; ++j )
                    this->push_back(PositionWeightPair<WEIGHTS>((i - data.rows/2)*stride + (j - data.columns/2), data(i,j)));
        else if ( ccol == -1 )
            for ( int i= 0; i < data.rows; ++i )
                for ( int j= 0; j < data.columns; ++j )
                    this->push_back(PositionWeightPair<WEIGHTS>(i*stride + j - crow, data(i,j)));
        else
            for ( int i= 0; i < data.rows; ++i )
                for ( int j= 0; j < data.columns; ++j )
                    this->push_back(PositionWeightPair<WEIGHTS>((i - crow)*stride + (j-ccol), data(i,j)));
        image.resizeImage(data);
        image= data;
    }
    
    
    template<typename WEIGHTS>
    Template2<WEIGHTS>::~Template2()
    {
    }
    
    template<typename WEIGHTS>
    int Template2<WEIGHTS>::getRows()
    {
        if ( this->stride != -1 )
        {
            /*int minn= 0, maxn= 0;
            for (int i= this->min; abs(i) > this->stride/2; i+= this->stride)
                ++minn;
            for (int i= this->max; abs(i) > this->stride/2; i-= this->stride)
                ++maxn;
            int n= (minn < maxn) ? maxn : minn
            return n*2+1;*/
            int minr= INT_MAX, maxr= -INT_MAX, r, c;
            for ( unsigned int i= 0; i < this->size(); ++i )
            {
                this->getCoordinate2D((*this)(i).first, r, c);
                if ( r < minr )
                    minr= r;
                if ( r > maxr )
                    maxr= r;
            }
            
            maxr= abs(maxr);
            minr= abs(minr);
            
            int max= (maxr > minr) ? maxr : minr;
            return 2*max + 1;
        }
        else
        {
            return 1;
        }
    }

    template<typename WEIGHTS>
    int Template2<WEIGHTS>::getColumns()
    {
        //int minx= INT_MAX, maxx= -INT_MAX;
        //for ( typename Template2<WEIGHTS>::fIt fit= this->begin(); fit != this->end(); ++fit )
        {
            /*int m= fit->first%this->stride;
            if ( abs(m) > this->stride/2 )
                m+= m > 0 ? -this->stride : this->stride;
            if ( m < minx )
                minx= m;
            if ( m > maxx )
                maxx= m;*/
        }
        //return maxx - minx + 1;
        
        int minc= INT_MAX, maxc= -INT_MAX, r, c;
            for ( unsigned int i= 0; i < this->size(); ++i )
            {
                this->getCoordinate2D((*this)(i).first, r, c);
                if ( c < minc )
                    minc= c;
                if ( c > maxc )
                    maxc= c;
            }
            
            maxc= abs(maxc);
            minc= abs(minc);
            
            int max= (maxc > minc) ? maxc : minc;
            return 2*max + 1;
        
    }
    
    template<typename WEIGHTS>
    void Template2<WEIGHTS>::getCoordinate2D(int n, int& rows, int& columns)
    {
        /*rows= n / this->stride;
        if ( abs(n % this->stride) > this->stride/2 )
            rows= n > 0 ? rows + 1 : rows - 1;
        columns= n % this->stride;
        if ( abs(columns) > this->stride/2 )
            columns+= columns > 0 ? -this->stride : this->stride;*/
        rows= columns= 0;
        if ( n > 0 )
        {
            while ( n > stride/2 )
            {
                n -= stride;
                rows++;
            }
            columns= n;
        }
        else 
        {
            while ( n < -stride/2 )
            {
                n+= stride;
                --rows;
            }
            columns= n;
        }
    }
    
    template<typename WEIGHTS>
    Border2 Template2<WEIGHTS>::getProposedBorder2()
    {
        //printf("gpb2\n"); fflush(stdout);
        this->computeMinMax();
        //printf("cmm finished\n"); fflush(stdout);

        int w= this->getMax()/this->stride + 2;
        //printf("ret gpb2\n"); fflush(stdout);
        return Border2(w, w, w, w);
    }
    
    template<typename WEIGHTS>
    Border2 Template2<WEIGHTS>::getProposedBorder()
    {
        //printf("gpb\n"); fflush(stdout);
        Border2 res= getProposedBorder2();
        //printf("gpb2 computed\n"); fflush(stdout);
        return getProposedBorder2();
    }
    
    template<typename WEIGHTS>
    void Template2<WEIGHTS>::dumpTemplate(Image<WEIGHTS>& im)
    {
        //printf("%d %d\n", this->getRows(), this->getColumns());
        im.resizeImage(this->getRows() + 3, this->getColumns() + 3);
        im= 0;
        float max= -FLT_MAX;
        for ( unsigned int i= 0; i < this->size(); ++i )
            if ( (*this)(i).second > max )
                max= (*this)(i).second;
        //im= max;

        int x, y;
        for ( fIt fit= this->begin(); fit != this->end(); ++fit )
        {
            this->getCoordinate2D(fit->first, x, y);
            /*x= fit->first/this->stride;
            if ( abs(fit->first%this->stride) > this->stride/2 )
                x= (fit->first > 0) ? x+1 : x-1;

            y= fit->first%this->stride;
            if ( abs(y) > this->stride/2 )
                y+= y > 0? -this->stride : this->stride;*/
            im.get(x,y)= fit->second;
        }
    }
    
    template<typename WEIGHTS>
    int Template2<WEIGHTS>::updateStride(int stride)
    {
      if ( this->stride != stride )
      {
	  int x, y;
	  for ( fIt fit= this->begin(); fit != this->end(); ++fit )
	  {
	      x= fit->first/this->stride;
	      if ( abs(fit->first%this->stride) > this->stride/2 )
		  x= (fit->first > 0) ? x+1 : x-1;

	      y= fit->first%this->stride;
	      if ( abs(y) > this->stride/2 )
		  y+= y > 0? -this->stride : this->stride;
	      fit->first= x*stride + y;
	  }
	  this->stride= stride;
	  computeMinMax();
	  return 1;
      }
      return 0;
    }
}

#endif

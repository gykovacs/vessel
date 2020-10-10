#include <openipDS/Region3Set.h>
#include <openipDS/Stopper.h>

namespace openip
{
    Vector<Vector<float> > getCentersOfGravity(Region3Set& rs)
    {
      Vector<Vector<float> > result;
      for ( unsigned int i= 0; i < rs.size(); ++i )
	result.push_back(rs(i).getCenterOfGravity());
      return result;
    }
  
    Region3Set::Region3Set()
        : Vector<Region3>()
    {
    }

    Region3Set::Region3Set(const Region3Set& r)
        : Vector<Region3>(r)
    {
    }

    Region3Set::~Region3Set()
    {
    }

    Region3Set Region3Set::getLargestNRegion(int n)
    {
        Region3Set result;

        Region3SizeComparator r;

        sort(begin(), end(), r);

        //tprintf("t2 %d %d\n", this->size(), n);
        int s= this->size();
        int lrange= s-1-n >= 0 ? s-1-n : 0;
        for ( int i= s - 1; i >= lrange; --i )
        {
            //printf("."); fflush(stdout);
            result.push_back((*this)(i));
        }

        //tprintf("r.size %d\n", result.size());
        return result;
    }
    
    Region3Set Region3Set::getRegionsLargerThan(unsigned int rsize)
    {
      Region3Set result;
      
      for ( unsigned int i= 0; i < this->size(); ++i )
	if ( (*this)(i).size() > rsize )
	  result.push_back((*this)(i));
      return result;
    }

    bool VectorCoordinate0Comparator::operator() (const Vector<float>& a, const Vector<float>& b)
    {
        return a(0) < b(0);
    }

    bool VectorCoordinate1Comparator::operator() (const Vector<float>& a, const Vector<float>& b)
    {
        return a(1) < b(1);
    }

    bool VectorCoordinate2Comparator::operator() (const Vector<float>& a, const Vector<float>& b)
    {
        return a(2) < b(2);
    }

    void Region3Set::processSlice(Vector<Vector<float> >& centers, Vector<Vector<float> >& oneSlice, Vector<int>& result, float drf, float dcf)
    {
        VectorCoordinate1Comparator v1c;
        sort(oneSlice.begin(), oneSlice.end(), v1c);

        printf("slice: ");
        for ( unsigned int i= 0; i < oneSlice.size(); ++i )
            printf("%f ", oneSlice(i)(1));
        printf("\n");

        int lastRowBreak= 0;
        int rowBreak= 0;

        tprintf("oneSlice\n");
        int j;
        for ( j= 0; j < int(oneSlice.size()) - 1; ++j )
        {
            if ( (oneSlice(j+1)(1) - oneSlice(j)(1) >= drf*0.8) )
            {
                lastRowBreak= rowBreak;
                rowBreak= j+1;

                tprintf("rowBreak: %d\n", rowBreak);

                Vector<Vector<float> > oneRow;
                for ( int k= lastRowBreak; k < rowBreak; ++k )
                    oneRow.push_back(oneSlice(k));

                processRow(centers, oneRow, result, dcf);
            }
        }
        lastRowBreak= rowBreak;
        rowBreak= oneSlice.size();

        Vector<Vector<float> > oneRow;
        for ( int k= lastRowBreak; k < rowBreak; ++k )
            oneRow.push_back(oneSlice(k));

        processRow(centers, oneRow, result, dcf);
    }

    void Region3Set::processRow(Vector<Vector<float> >& centers, Vector<Vector<float> >& oneRow, Vector<int>& result, float /*d*/)
    {
        VectorCoordinate2Comparator v2c;
        sort(oneRow.begin(), oneRow.end(), v2c);

        printf("row: ");
        for ( unsigned int i= 0; i < oneRow.size(); ++i )
            printf("%f ", oneRow(i)(2));
        printf("\n");

        for ( unsigned int k= 0; k < oneRow.size(); ++k )
            for ( unsigned int l= 0; l < centers.size(); ++l )
            {
                //printf("%f ", centers(l).dist(oneRow(k)));
                if ( centers(l) == oneRow(k) )
                {
                    result.push_back(l);
                    break;
                }
            }
    }

    Region3Set Region3Set::getOrderedNRegion(int n)
    {
        Region3Set results;
        Region3Set final;

        tprintf("this->size: %d\n", this->size());
        results= this->getLargestNRegion(n);

        tprintf("results.size: %d\n", results.size());
        Vector<Vector<float> > centers;
        Vector<Vector<float> > all;
        for ( unsigned int i= 0; i < results.size(); ++i )
        {
            centers.push_back(results(i).getCenterOfBoundingBox());
            all.push_back(centers(centers.size()-1));
            cout << results(i).getCenterOfBoundingBox() << std::endl;
        }

        float ds, dr, dc;
        ds= dr= dc= FLT_MAX;

        float dsf, drf, dcf;
        dsf= drf= dcf= FLT_MAX;
        float gm= FLT_MAX;
        for ( unsigned int i= 0; i < centers.size(); ++i )
        {
            int minIdx= 0;
            float minValue= FLT_MAX;
            for ( unsigned int j= 0; j < centers.size(); ++j )
            {
                if ( i != j )
                {
                    if ( centers(i).dist(centers(j)) < minValue )
                    {
                        minValue= centers(i).dist(centers(j));
                        minIdx= j;
                    }
                }
            }

            ds= fabs(centers(i)(0) - centers(minIdx)(0));
            dr= fabs(centers(i)(1) - centers(minIdx)(1));
            dc= fabs(centers(i)(2) - centers(minIdx)(2));

            printf("%f %f %f\n", ds, dr, dc);

            if ( ds > dr && ds > dc )
            {
                if ( ds < dsf )
                    dsf= ds;
            }
            if ( dr > ds && dr > dc )
            {
                if ( dr < drf )
                    drf= dr;
            }
            if ( dc > ds && dc > dr )
            {
                if ( dc < dcf )
                    dcf= dc;
            }

            if ( minValue < gm )
                gm= minValue;
        }

        float d= FLT_MAX;
        if ( dsf < FLT_MAX )
            d= dsf;
        if ( drf < d )
            d= drf;
        if ( dcf < d )
            d= dcf;

        dcf= drf= dsf= sqrt(gm)*0.8;

        tprintf("%f %f %f\n", dsf, drf, dcf);

        tprintf("d: %f\n", d);
        VectorCoordinate0Comparator v0c;

        sort(centers.begin(), centers.end(), v0c);

        tprintf("%f %f %f...\n", centers(0)(0), centers(1)(0), centers(2)(0));

        for ( unsigned int i= 0; i < centers.size(); ++i )
            cout << centers(i) << std::endl;

        printf("full: ");
        for ( unsigned int i= 0; i < centers.size(); ++i )
            printf("%f ", centers(i)(0));
        printf("\n");

        Vector<int> res;

        int lastSliceBreak= 0;
        int sliceBreak= 0;
        int i;
        for ( i= 0; i < int(centers.size()) - 1; ++i )
        {
            if ( centers(i+1)(0) - centers(i)(0) >= dsf*0.8 )
            {
                lastSliceBreak= sliceBreak;
                sliceBreak= i+1;

                tprintf("slicebreak: %d\n", sliceBreak);

                Vector<Vector<float> > oneSlice;
                for ( int j= lastSliceBreak; j < sliceBreak; ++j )
                    oneSlice.push_back(centers(j));

                processSlice(all, oneSlice, res, drf, dcf);

            }
        }
        lastSliceBreak= sliceBreak;
        sliceBreak= centers.size();

        Vector<Vector<float> > oneSlice;
        for ( int j= lastSliceBreak; j < sliceBreak; ++j )
            oneSlice.push_back(centers(j));

        processSlice(all, oneSlice, res, drf, dcf);

        for ( unsigned int i= 0; i < res.size(); ++i )
            final.push_back(results(res(i)));

        return final;
    }
  
  int Region3Set::indexOfRegionWithSmallestCoordinate(int dimension, int direction)
  {
    int idx= 0;
    float coordinate;
    
    float tmpcoord;
    Vector<float> tmp;
    
    tmp= (*this)(0).getCenterOfGravity();
    coordinate= tmp(dimension);
    
    for ( unsigned int i= 0; i < this->size(); ++i )
    {
      tmp= (*this)(i).getCenterOfGravity();
      tmpcoord= tmp(dimension);
      
      if ( direction == 0 )
	if ( tmpcoord < coordinate )
	{
	  coordinate= tmpcoord;
	  idx= i;
	}
      if ( direction == 1 )
	if ( tmpcoord > coordinate )
	{
	  coordinate= tmpcoord;
	  idx= i;
	}
    }
    
    return idx;
  }
  
  void Region3Set::sortRegionsInDirection(int dimension, int direction)
  {
    Region3Set tmp;
    int idx;
    
    while ( this->size() != 0 )
    {
      idx= this->indexOfRegionWithSmallestCoordinate(dimension, direction);
      tmp.push_back((*this)(idx));
      this->erase(this->begin() + idx);
    }
    
    (*this)= tmp;
  }
  
  int Region3Set::minAxialCoordinate()
  {
    int minAxialCoordinate= 10000000;
    int s, r, c;
    for ( unsigned int i= 0; i < this->size(); ++i )
      for ( unsigned int j= 0; j < (*this)(i).size(); ++j )
      {
	(*this)(i).getCoordinate3D((*this)(i)(j), s, r, c);
	if ( s < minAxialCoordinate )
	  minAxialCoordinate= s;
      }
    return s;
  }
  
  int Region3Set::maxAxialCoordinate()
  {
    int maxAxialCoordinate= 0;
    int s, r, c;
    for ( unsigned int i= 0; i < this->size(); ++i )
      for ( unsigned int j= 0; j < (*this)(i).size(); ++j )
      {
	(*this)(i).getCoordinate3D((*this)(i)(j), s, r, c);
	if ( s > maxAxialCoordinate )
	  maxAxialCoordinate= s;
      }
    return s;
  }
  
  void Region3Set::add(Region3Set& r)
  {
    for ( unsigned int i= 0; i < r.size(); ++i )
      this->push_back(r(i));
  }
  
  Region3 Region3Set::getLongestRegion(int dim)
  {
    int maxIdx= 0;
    for ( unsigned int i= 0; i < this->size(); ++i )
      if ( (*this)(i).length(dim) > (*this)(maxIdx).length(dim) )
	maxIdx= i;
    return (*this)(maxIdx);
  }
  
  Region3 Region3Set::getLargestRegion()
  {
    int maxIdx= 0;
    for ( unsigned int i= 0; i < this->size(); ++i )
      if ( (*this)(i).size() > (*this)(maxIdx).size() )
	maxIdx= i;
    return (*this)(maxIdx);
  }
  
  void Region3Set::getMinMaxCoordinates(int& smin, int& smax, int& rmin, int& rmax, int& cmin, int& cmax)
  {
    smin= 1000000;
    smax= 0;
    rmin= 1000000;
    rmax= 0;
    cmin= 1000000;
    cmax= 0;
    
    int s0, s1, r0, r1, c0, c1;
    
    for ( unsigned int i= 0; i < this->size(); ++i )
    {
      (*this)(i).getMinMaxCoordinates(s0, s1, r0, r1, c0, c1);
      if ( s0 < smin )
	smin= s0;
      if ( r0 < rmin )
	rmin= r0;
      if ( c0 < cmin )
	cmin= c0;
      if ( s1 > smax )
	smax= s1;
      if ( r1 > rmax )
	rmax= r1;
      if ( c1 > cmax )
	cmax= c1;
    }
  }
}

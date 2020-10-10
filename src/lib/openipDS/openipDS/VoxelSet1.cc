#include <limits.h>
#include <openipDS/VoxelSet1.h>

namespace openip
{
    VoxelSet1::VoxelSet1()
    : VoxelSet<Voxel1>()
    {
    }
        
    VoxelSet1::VoxelSet1(const VoxelSet1& v)
    : VoxelSet<Voxel1>(v)
    {
    }

    VoxelSet1::~VoxelSet1()
    {
    }
    
    VoxelSet1& VoxelSet1::operator=(const VoxelSet1& v)
    {
        this->VoxelSet<Voxel1>::operator=(v);
        return *this;
    }
    
    void VoxelSet1::computeMinMax()
    {
        min= INT_MAX;
        max= -INT_MAX;
        minX= INT_MAX;
        minY= INT_MAX;
        minZ= INT_MAX;
        maxX= -INT_MAX;
        maxY= -INT_MAX;
        maxZ= -INT_MAX;

        int x, y;

        for (VoxelSet1::pIt pit= this->begin(); pit != this->end(); ++pit )
        {
            x= *pit % stride;
            y= *pit / stride;
            if ( *pit < min )
                min= *pit;
            if ( *pit > max )
                max= *pit;
            if ( x < minX )
                minX= x;
            if ( x > maxX )
                maxX= x;
            if ( y < minY )
                minY= y;
            if ( y > maxY )
                maxY= y;
        }
    }

    void VoxelSet1::computeMinMaxXYZ()
    {
        minX= INT_MAX;
        minY= INT_MAX;
        minZ= INT_MAX;
        maxX= -INT_MAX;
        maxY= -INT_MAX;
        maxZ= -INT_MAX;

        int x, y;

        for (VoxelSet1::pIt pit= this->begin(); pit != this->end(); ++pit )
        {
            x= *pit % stride;
            y= *pit / stride;
            if ( x < minX )
                minX= x;
            if ( x > maxX )
                maxX= x;
            if ( y < minY )
                minY= y;
            if ( y > maxY )
                maxY= y;
        }
    }

    void VoxelSet1::computeMinMaxOnly()
    {
        min= INT_MAX;
        max= -INT_MAX;

        for (VoxelSet1::pIt pit= this->begin(); pit != this->end(); ++pit )
        {
            if ( *pit < min )
                min= *pit;
            if ( *pit > max )
                max= *pit;
        }
    }

    int VoxelSet1::updateStride(int stride, int slicePitch)
    {
        if ( this->stride != stride || this->slicePitch != slicePitch )
        {
            int x, y, z, tmp;
            for ( VoxelSet1::iterator pit= this->begin(); pit != this->end(); ++pit )
            {
                tmp= *pit;
                z= tmp / this->slicePitch;
                if ( abs(tmp % this->slicePitch) > this->slicePitch / 2)
                    z= (tmp > 0) ? z + 1 : z - 1;

                tmp= tmp % this->slicePitch;
                if ( tmp < 0 && tmp < -this->slicePitch/2 )
                    tmp= this->slicePitch+tmp;
                else if ( tmp > 0 && tmp > this->slicePitch/2 )
                    tmp= tmp - this->slicePitch;
                
                x= tmp / this->stride;
                
                if ( abs(tmp % this->stride) > this->stride/2 )
                    x= (tmp > 0) ? x+1 : x-1;

                y= tmp % this->stride;
                if ( abs(y) > this->stride/2 )
                    y+= y > 0 ? -this->stride : this->stride;

                *pit= z*slicePitch + x*stride + y;
            }
            this->slicePitch= slicePitch;
            this->stride= stride;
            this->computeMinMaxOnly();

            return 1;
        }
        return 0;
    }

    int VoxelSet1::getRows()
    {
        int s, r, c;
        
        int minx= INT_MAX, maxx= -INT_MAX;
        for ( VoxelSet1::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            this->getCoordinates3D(*fit, s, r, c);
            if ( r < minx )
                minx= r;
            if ( r > maxx )
                maxx= r;
        }
        return maxx - minx + 1;
    }

    int VoxelSet1::getColumns()
    {
        int s, r, c;
        
        int minx= INT_MAX, maxx= -INT_MAX;
        for ( VoxelSet1::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            this->getCoordinates3D(*fit, s, r, c);
            if ( c < minx )
                minx= c;
            if ( c > maxx )
                maxx= c;
        }
        return maxx - minx + 1;
    }
    
    int VoxelSet1::getSlices()
    {
        int s, r, c;
        
        int minx= INT_MAX, maxx= -INT_MAX;
        for ( VoxelSet1::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            this->getCoordinates3D(*fit, s, r, c);
            if ( s < minx )
                minx= s;
            if ( s > maxx )
                maxx= s;
        }
        return maxx - minx + 1;
    }
    
    void VoxelSet1::getCoordinates3D(int n, int& slice, int& row, int& column)
    {
        slice= 0;
        if ( n < 0 )
        {
            while ( n < -slicePitch/2 )
            {
                ++slice;
                n+= slicePitch;
            }
        }
        else
        {
            while ( n > slicePitch/2 )
            {
                --slice;
                n-= slicePitch;
            }
        }
        
        n= n % slicePitch;
        column= 0;
        if ( n < 0 )
        {
            while( n < -stride/2 )
            {
                ++column;
                n+= stride;
            }
        }
        else
        {
            while( n > stride/2 )
            {
                --column;
                n-= stride;
            }
        }
        row= n;
    }
    
    Border3 VoxelSet1::getProposedBorder()
    {
      int smax, smin, rmax, rmin, cmax, cmin;
      smax= rmax= cmax= -INT_MAX;
      smin= rmin= cmin= INT_MAX;
      
      int s, r, c;
	
      for ( unsigned int i= 0; i < this->size(); ++i )
      {
	getCoordinates3D((*this)(i), s, r, c);
      
	if ( s > smax )
	  smax= s;
	if ( s < smin )
	  smin= s;
	if ( r > rmax )
	  rmax= r;
	if ( r < rmin )
	  rmin= r;
	if ( c > cmax )
	  cmax= c;
	if ( c < cmin )
	  cmin= c;
      }
      
      return Border3(-rmin, rmax, -cmin, cmax, -smin, smax, BORDER_MODE_MIRRORED);
    }
    
    void VoxelSet1::getSlicesRowsColumns(int& slices, int& rows, int& columns)
    {
        int s, r, c;
        
        int mins= INT_MAX, maxs= -INT_MAX;
        int minr= INT_MAX, maxr= -INT_MAX;
        int minc= INT_MAX, maxc= -INT_MAX;
        
        for ( VoxelSet1::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            this->getCoordinates3D(*fit, s, r, c);
            if ( r < minr )
                minr= r;
            if ( r > maxr )
                maxr= r;
            if ( c < minc )
                minc= c;
            if ( c > maxc )
                maxc= c;
            if ( s < mins )
                mins= s;
            if ( s > maxs )
                maxs= s;
        }
        slices= maxs - mins + 1;
        rows= maxr - minr + 1;
        columns= maxc - minc + 1;
    }
}

#include <limits.h>
#include <openipDS/PixelSet1.h>

namespace openip
{
    PixelSet1::PixelSet1()
    : PixelSet<Pixel1>()
    {
    }
        
    PixelSet1::PixelSet1(const PixelSet1& p)
    : PixelSet<Pixel1>(p)
    {
    }

    PixelSet1::~PixelSet1()
    {
    }
    
    PixelSet1& PixelSet1::operator=(const PixelSet1& p)
    {
        this->PixelSet<Pixel1>::operator=(p);
        return *this;
    }
    
    void PixelSet1::computeMinMax()
    {
        min= INT_MAX;
        max= -INT_MAX;
        minX= INT_MAX;
        minY= INT_MAX;
        maxX= -INT_MAX;
        maxY= -INT_MAX;

        int x, y;

        for (PixelSet1::pIt pit= this->begin(); pit != this->end(); ++pit )
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

    void PixelSet1::computeMinMaxXY()
    {
        minX= INT_MAX;
        minY= INT_MAX;
        maxX= -INT_MAX;
        maxY= -INT_MAX;

        int x, y;

        for (PixelSet1::pIt pit= this->begin(); pit != this->end(); ++pit )
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

    void PixelSet1::computeMinMaxOnly()
    {
        min= INT_MAX;
        max= -INT_MAX;

        for (PixelSet1::pIt pit= this->begin(); pit != this->end(); ++pit )
        {
            if ( *pit < min )
                min= *pit;
            if ( *pit > max )
                max= *pit;
        }
    }

    int PixelSet1::updateStride(int stride)
    {
        //printf("ps updateStride\n"); fflush(stdout);
        if ( this->stride != stride )
        {
            int x, y;
            for ( PixelSet1::iterator pit= this->begin(); pit != this->end(); ++pit )
            {
                x= *pit / this->stride;
                if ( abs(*pit % this->stride) > this->stride/2 )
                    x= (*pit > 0) ? x+1 : x-1;

                y= *pit % this->stride;
                if ( abs(y) > this->stride/2 )
                    y+= y > 0 ? -this->stride : this->stride;


                *pit= x*stride + y;
                //printf("%d %d %d %d\n", x, y, stride, *pit);
            }
            this->stride= stride;
            this->computeMinMaxOnly();
            return 1;
        }
        return 0;
    }

    int PixelSet1::getRows()
    {
        if ( this->stride != -1 )
        {
            int n= 0;
            for (int i= this->min; abs(i) > this->stride/2; i+= this->stride)
                ++n;
            for (int i= this->max; abs(i) > this->stride/2; i-= this->stride)
                ++n;
            return n+1;
        }
        else
        {
            return 1;
        }
    }

    int PixelSet1::getColumns()
    {
        int minx= INT_MAX, maxx= -INT_MAX;
        for ( PixelSet1::iterator fit= this->begin(); fit != this->end(); ++fit )
        {
            int m= *fit % this->stride;
            if ( abs(m) > this->stride/2 )
                m+= m > 0 ? -this->stride : this->stride;
            if ( m < minx )
                minx= m;
            if ( m > maxx )
                maxx= m;
        }
        return maxx - minx + 1;
    }

    PixelSet1 PixelSet1::operator/(const PixelSet1& s) const
    {
        return PixelSet1(*this) /= s;
    }

    PixelSet1& PixelSet1::operator/=(const PixelSet1& s)
    {
        this->PixelSet<Pixel1>::operator/=(s);
        return *this;
    }
}

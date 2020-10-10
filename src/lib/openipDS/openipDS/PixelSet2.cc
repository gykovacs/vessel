#include <limits.h>

#include <openipDS/PixelSet2.h>

namespace openip
{
    void PixelSet2::computeMinMax()
    {
        min= INT_MAX;
        max= -INT_MAX;
        minX= INT_MAX;
        minY= INT_MAX;
        maxX= -INT_MAX;
        maxY= -INT_MAX;

        int x, y, n;

        for (PixelSet2::pIt pit= this->begin(); pit != this->end(); ++pit )
        {
            x= pit->x();
            y= pit->y();
            n= y*stride + x;
            if ( n < min )
                min= n;
            else if ( n > max )
                max= n;
            if ( x < minX )
                minX= x;
            else if ( x > maxX )
                maxX= x;
            if ( y < minY )
                minY= y;
            else if ( y > maxY )
                maxY= y;
        }
    }

    void PixelSet2::computeMinMaxXY()
    {
        minX= INT_MAX;
        minY= INT_MAX;
        maxX= -INT_MAX;
        maxY= -INT_MAX;

        int x, y;

        for (PixelSet2::pIt pit= this->begin(); pit != this->end(); ++pit )
        {
            x= pit->x();
            y= pit->y();
            if ( x < minX )
                minX= x;
            else if ( x > maxX )
                maxX= x;
            if ( y < minY )
                minY= y;
            else if ( y > maxY )
                maxY= y;
        }
    }

    void PixelSet2::computeMinMaxOnly()
    {
        min= INT_MAX;
        max= -INT_MAX;

        int n;

        for (PixelSet2::pIt pit= this->begin(); pit != this->end(); ++pit )
        {
            n= pit->y() * stride + pit->x();
            if ( n < min )
                min= n;
            else if ( n > max )
                max= n;
        }
    }
}

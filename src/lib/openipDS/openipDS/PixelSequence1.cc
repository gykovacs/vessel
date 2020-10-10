#include <openipDS/PixelSequence1.h>

namespace openip
{
    void PixelSequence1::computeMinMax(int stride)
    {
        min= INT_MAX;
        max= -INT_MAX;
        minX= INT_MAX;
        minY= INT_MAX;
        maxX= -INT_MAX;
        maxY= -INT_MAX;

        int x, y;

        for (PixelSequence1::pIt pit= this->begin(); pit != this->end(); ++pit )
        {
            x= *pit % stride;
            y= *pit / stride;
            if ( *pit < min )
                min= *pit;
            else if ( *pit > max )
                max= *pit;
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

    void PixelSequence1::computeMinMaxXY(int stride)
    {
        minX= INT_MAX;
        minY= INT_MAX;
        maxX= -INT_MAX;
        maxY= -INT_MAX;

        int x, y;

        for (PixelSequence1::pIt pit= this->begin(); pit != this->end(); ++pit )
        {
            x= *pit % stride;
            y= *pit / stride;
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

    void PixelSequence1::computeMinMaxOnly(int)
    {
        min= INT_MAX;
        max= -INT_MAX;

        for (PixelSequence1::pIt pit= this->begin(); pit != this->end(); ++pit )
        {
            if ( *pit < min )
                min= *pit;
            else if ( *pit > max )
                max= *pit;
        }
    }
}

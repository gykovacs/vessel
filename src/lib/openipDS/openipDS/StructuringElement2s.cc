#include <openipDS/StructuringElement2s.h>
#include <openipDS/drawing.h>

#include <algorithm>

namespace openip
{
    StructuringElementSquare::StructuringElementSquare(int size, int stride)
    : StructuringElement2(stride)
    {
        generateStructuringElementSquare(*this, size, stride);
        this->stride= stride;
        this->computeMinMaxOnly();
    }
    
    StructuringElementSquare::StructuringElementSquare(const StructuringElementSquare& s)
    : StructuringElement2(s)
    {
    }
    
    StructuringElementSquare::~StructuringElementSquare()
    {
    }

    StructuringElementDisk::StructuringElementDisk(float r, int stride)
    : StructuringElement2(stride)
    {
        generateStructuringElementDisk(*this, r, stride);
        this->stride= stride;
        this->computeMinMaxOnly();
    }
    
    StructuringElementDisk::StructuringElementDisk(const StructuringElementDisk& s)
    : StructuringElement2(s)
    {
    }
    
    StructuringElementDisk::~StructuringElementDisk()
    {
    }

    StructuringElementRing::StructuringElementRing(int inner, int outer, int stride)
    : StructuringElement2(stride)
    {
        int ii= inner*inner;
        int oo= outer*outer;
        int tmp;
        for ( int i= -outer; i <= outer; ++i )
            for ( int j= -outer; j <= outer; ++j )
            {
                tmp= i*i + j*j;
                if ( tmp >= ii && tmp <= oo )
                    this->push_back(i*stride + j);
            }
        this->computeMinMaxOnly();
    }
    
    StructuringElementRing::StructuringElementRing(const StructuringElementRing& s)
    : StructuringElement2(s)
    {
    }
    
    StructuringElementRing::~StructuringElementRing()
    {
    }
    
    StructuringElementHorizontal::StructuringElementHorizontal(int width, int length, int stride)
    : StructuringElement2(stride)
    {
        for ( int i= -width; i <= width; ++i )
            for ( int j= -length; j <= length; ++j )
            {
		this->push_back(i*stride + j);
            }
        this->computeMinMaxOnly();
    }
    
    StructuringElementHorizontal::StructuringElementHorizontal(const StructuringElementHorizontal& s)
    : StructuringElement2(s)
    {
    }
    
    StructuringElementHorizontal::~StructuringElementHorizontal()
    {
    }
    
    StructuringElementVertical::StructuringElementVertical(int width, int length, int stride)
    : StructuringElement2(stride)
    {
        for ( int i= -width; i <= width; ++i )
            for ( int j= -length; j <= length; ++j )
            {
		this->push_back(j*stride + i);
            }
        this->computeMinMaxOnly();
    }
    
    StructuringElementVertical::StructuringElementVertical(const StructuringElementVertical& s)
    : StructuringElement2(s)
    {
    }
    
    StructuringElementVertical::~StructuringElementVertical()
    {
    }

    StructuringElementDoubleRing::StructuringElementDoubleRing(int, int, int, int, int)
    {
    }
    
    StructuringElementDoubleRing::StructuringElementDoubleRing(const StructuringElementDoubleRing& s)
    : StructuringElement2(s)
    {
    }
    
    StructuringElementDoubleRing::~StructuringElementDoubleRing()
    {
    }

    void generateStructuringElementDisk(StructuringElement2& se, float r, int stride)
    {
        se.clear();
        float rr= r*r;
        for ( int i= -r; i <= r; ++i )
            for ( int j= -r; j <= r; ++j )
                if ( i*i + j*j <= rr )
                    se.push_back(i*stride + j);

        se.stride= stride;
        se.computeMinMaxOnly();
    }

    void generateStructuringElementSquare(StructuringElement2& se, int size, int stride)
    {
        se.clear();
        for ( int i= -size/2; i <= size/2; ++i )
            for ( int j= -size/2; j <= size/2; ++j )
                se.push_back(i * stride + j);
        se.stride= stride;
        se.computeMinMaxOnly();
    }

    void generateStructuringElementCircle(StructuringElement2& se, int r, int stride)
    {
      se.clear();
        if ( r != 0 )
        {
            Image<unsigned char> b(r*2+1, r*2+1);

            b= 0;

            drawCircle(b, r, r, r, (unsigned char)255, false);

            int cX= r;
            int cY= r;
            int pX, pY;

            for ( unsigned int i= 0; i < b.n; ++i )
                if ( b(i) )
                {
                    b.getCoordinate2D(i, pX, pY);
                    se.push_back(pX - cX + (pY - cY) * stride);
                }
            se.computeMinMaxOnly();
        }
        else
        {
            se.push_back(0);
            se.computeMinMaxOnly();
        }
        se.stride= stride;
    }

    StructuringElementRectangle::StructuringElementRectangle(int width, int height, int stride)
        : StructuringElement2()
    {
        for ( int i= -width/2; i <= width/2; ++i )
            for ( int j= -height/2; j <= height/2; ++j )
            {
                this->push_back(j*stride + i);
            }
        this->stride= stride;
        this->computeMinMaxOnly();
    }

    StructuringElementRectangle::StructuringElementRectangle(const StructuringElementRectangle& s)
        : StructuringElement2(s)
    {
    }

    StructuringElementRectangle::~StructuringElementRectangle()
    {
    }

    StructuringElementParallelStripes::StructuringElementParallelStripes(int width, int height, int distance, int stride)
        : StructuringElement2()
    {
        for ( int i= -distance/2 - height; i <= -distance/2; ++i )
            for ( int j= -width/2; j <= width/2; ++j )
            {
                this->push_back(i*stride + j);
            }
        for ( int i= distance/2; i <= distance/2 + height; ++i )
            for ( int j= -width/2; j <= width/2; ++j )
            {
                this->push_back(i*stride + j);
            }
        this->stride= stride;
        this->computeMinMaxOnly();
    }

    StructuringElementParallelStripes::StructuringElementParallelStripes(const StructuringElementParallelStripes& s)
        : StructuringElement2(s)
    {
    }

    StructuringElementParallelStripes::~StructuringElementParallelStripes()
    {
    }

    StructuringElementOneStripe::StructuringElementOneStripe(int width, int height, int distance, int stride)
        : StructuringElement2()
    {
        for ( int i= -distance/2 - height; i <= -distance/2; ++i )
            for ( int j= -width/2; j <= width/2; ++j )
            {
                this->push_back(i*stride + j);
            }
        /*for ( int i= distance/2; i <= distance/2 + height; ++i )
            for ( int j= -width/2; j <= width/2; ++j )
            {
                this->push_back(i*stride + j);
            }*/
        this->stride= stride;
        this->computeMinMaxOnly();
    }

    StructuringElementOneStripe::StructuringElementOneStripe(const StructuringElementOneStripe& s)
        : StructuringElement2(s)
    {
    }

    StructuringElementOneStripe::~StructuringElementOneStripe()
    {
    }

    StructuringElementForIntensityObjects::StructuringElementForIntensityObjects(int w1, int h1, int w2, int h2, int d2, int stride)
        : StructuringElement2()
    {
        StructuringElementRectangle ser(w1, h1, stride);
        StructuringElementParallelStripes seps(w2, h2, d2, stride);
        (*this)= ser;
        (*this)+= seps;
    }

    StructuringElementForIntensityObjects::StructuringElementForIntensityObjects(const StructuringElementForIntensityObjects& s)
        : StructuringElement2(s)
    {
    }

    StructuringElementForIntensityObjects::~StructuringElementForIntensityObjects()
    {
    }
}

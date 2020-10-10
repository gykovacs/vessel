#include <openipDS/StructuringElement2.h>
#include <openipDS/CoordinateTransform2.h>
#include <limits.h>

namespace openip
{
    StructuringElement22::StructuringElement22()
    : PixelSet2()
    {
    }

    StructuringElement22::StructuringElement22(StructuringElement2& se)
    : PixelSet2()
    {
        Pixel2 p2;
        for ( unsigned int i= 0; i < se.size(); ++i )
        {
            se.getXY(se(i), p2.r(), p2.c());
            this->push_back(p2);
        }
    }

    StructuringElement22::StructuringElement22(const StructuringElement22&)
    : PixelSet2()
    {
    }

    StructuringElement22::~StructuringElement22()
    {
    }

    void StructuringElement2::getXY(int n, int& rows, int& columns)
    {
        rows= n / stride;
        if ( abs(n % stride) > stride/2 )
            rows= n > 0 ? rows + 1 : rows - 1;
        columns= n % stride;
        if ( abs(columns) > this->stride/2 )
            columns+= columns > 0 ? -stride : stride;
    }

    StructuringElement2::StructuringElement2(int stride)
    : PixelSet1()
    {
        this->stride= stride;
    }

    StructuringElement2::StructuringElement2(const StructuringElement2& s)
    : PixelSet1(s)
    {
    }

    StructuringElement2::~StructuringElement2()
    {
    }
    
    StructuringElement2& StructuringElement2::operator=(const StructuringElement2& s)
    {
        this->PixelSet1::operator=(s);
        return *this;
    }

    void StructuringElement2::rotate(float alpha)
    {
        Vector2<float> tmp;
        Vector2<float> res;
        Rotation2 rot(alpha);

        //printf("%f, %d\n", alpha, stride);
        int r, c;
        float r1, c1;
        for ( StructuringElement2::iterator it= this->begin(); it != this->end(); ++it )
        {
            this->getXY(*it, r, c);
            tmp.x()= c;
            tmp.y()= r;
            res= rot^tmp;
            r1= int(cos(alpha)*r -sin(alpha)*c);
            c1= int(sin(alpha)*r + cos(alpha)*c);
            //printf("%d %d %f %f %d\n", r, c, r1, c1, int(r1*stride + c1));
            //printf("%d,%d ", *it, int(res.x()*stride + res.y()));
            *it= int(r1*stride + c1);
        }
    }

    std::ostream& operator<<(std::ostream& o, const StructuringElement2& s)
    {
        o << "[StructuringElement2 - size: ";
        o << s.size();
        o << " elements: ";
        for ( StructuringElement2::const_iterator it = s.begin(); it != s.end(); ++it )
        {
            o << *it;
            o << " ";
        }
        o << "]";

        return o;
    }
}

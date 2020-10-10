#include <openipDS/CoordinateTransform3.h>

namespace openip
{
    CoordinateTransform3::CoordinateTransform3()
    : Matrix3<float>()
    {
    }

    CoordinateTransform3::CoordinateTransform3(float* data)
    : Matrix3<float>(data)
    {
    }

    CoordinateTransform3::CoordinateTransform3(float a, float b, float c, float d, float e, float f, float g, float h, float i)
    : Matrix3<float>(a, b, c, d, e, f, g, h, i)
    {
    }

    CoordinateTransform3::CoordinateTransform3(const CoordinateTransform3& t)
    : Matrix3<float>(t)
    {
    }

    CoordinateTransform3::~CoordinateTransform3()
    {
    }

    CoordinateTransform3 operator^(const CoordinateTransform3& a, const CoordinateTransform3& b)
    {
        CoordinateTransform3 tmp;
        tmp.a= a.a*b.a + a.b*b.d + a.c*b.g;
        tmp.b= a.a*b.b + a.b*b.e + a.c*b.h;
        tmp.c= a.a*b.c + a.b*b.f + a.c*b.i;

        tmp.d= a.d*b.a + a.e*b.d + a.f*b.g;
        tmp.e= a.d*b.b + a.e*b.e + a.f*b.h;
        tmp.f= a.d*b.c + a.e*b.f + a.f*b.i;

        tmp.g= a.g*b.a + a.h*b.d + a.i*b.g;
        tmp.h= a.g*b.b + a.h*b.e + a.i*b.h;
        tmp.i= a.g*b.c + a.h*b.f + a.i*b.i;

        return tmp;
    }

    CoordinateTransform3& CoordinateTransform3::operator=(const CoordinateTransform3& b)
    {
        this->Matrix3<float>::operator=(b);
        return *this;
    }

    CoordinateTransform3& CoordinateTransform3::operator+=(const CoordinateTransform3& b)
    {
        this->Matrix3<float>::operator+=(b);
        return *this;
    }

    CoordinateTransform3& CoordinateTransform3::operator-=(const CoordinateTransform3& b)
    {
        this->Matrix3<float>::operator-=(b);
        return *this;
    }

    CoordinateTransform3& CoordinateTransform3::operator*=(const CoordinateTransform3& b)
    {
        this->Matrix3<float>::operator*=(b);
        return *this;
    }

    CoordinateTransform3& CoordinateTransform3::operator/=(const CoordinateTransform3& b)
    {
        this->Matrix3<float>::operator/=(b);
        return *this;
    }

    CoordinateTransform3 CoordinateTransform3::operator+(const CoordinateTransform3& b) const
    {
        CoordinateTransform3 n(*this);
        return n+= b;
    }

    CoordinateTransform3 CoordinateTransform3::operator-(const CoordinateTransform3& b) const
    {
        CoordinateTransform3 n(*this);
        return n-= b;
    }

    CoordinateTransform3 CoordinateTransform3::operator*(const CoordinateTransform3& b) const
    {
        CoordinateTransform3 n(*this);
        return n*= b;
    }

    CoordinateTransform3 CoordinateTransform3::operator/(const CoordinateTransform3& b) const
    {
        CoordinateTransform3 n(*this);
        return n/= b;
    }

    std::ostream& operator<<(std::ostream& o, const CoordinateTransform3& t)
    {
        o << "[CoordinateTransform3 ";
        o << t.a;
        o << ", ";
        o << t.b;
        o << ", ";
        o << t.c;
        o << ", ";
        o << t.d;
        o << ", ";
        o << t.e;
        o << ", ";
        o << t.f;
        o << ", ";
        o << t.g;
        o << ", ";
        o << t.h;
        o << ", ";
        o << t.i;
        o << "]";
        return o;
    }
}

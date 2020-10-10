#include <openipDS/CoordinateTransform2.h>

namespace openip
{
    Rotation2::Rotation2(float a)
    : CoordinateTransform2(cos(a), -sin(a), sin(a), cos(a))
    {
    }

    CoordinateTransform2::CoordinateTransform2()
    : Matrix2<float>()
    {
    }

    CoordinateTransform2::CoordinateTransform2(float* data)
    : Matrix2<float>(data)
    {
    }

    CoordinateTransform2::CoordinateTransform2(float a, float b, float c, float d)
    : Matrix2<float>(a, b, c, d)
    {
    }

    CoordinateTransform2::CoordinateTransform2(const CoordinateTransform2& t)
    : Matrix2<float>(t)
    {
    }

    CoordinateTransform2::~CoordinateTransform2()
    {
    }

    CoordinateTransform2& CoordinateTransform2::operator=(const CoordinateTransform2& b)
    {
        this->Matrix2<float>::operator=(b);
        return *this;
    }

    CoordinateTransform2& CoordinateTransform2::operator+=(const CoordinateTransform2& b)
    {
        this->Matrix2<float>::operator+=(b);
        return *this;
    }

    CoordinateTransform2& CoordinateTransform2::operator-=(const CoordinateTransform2& b)
    {
        this->Matrix2<float>::operator-=(b);
        return *this;
    }

    CoordinateTransform2& CoordinateTransform2::operator*=(const CoordinateTransform2& b)
    {
        this->Matrix2<float>::operator*=(b);
        return *this;
    }

    CoordinateTransform2& CoordinateTransform2::operator/=(const CoordinateTransform2& b)
    {
        this->Matrix2<float>::operator/=(b);
        return *this;
    }

    CoordinateTransform2 CoordinateTransform2::operator+(const CoordinateTransform2& b) const
    {
        CoordinateTransform2 n(*this);
        return n+= b;
    }

    CoordinateTransform2 CoordinateTransform2::operator-(const CoordinateTransform2& b) const
    {
        CoordinateTransform2 n(*this);
        return n-= b;
    }

    CoordinateTransform2 CoordinateTransform2::operator*(const CoordinateTransform2& b) const
    {
        CoordinateTransform2 n(*this);
        return n*= b;
    }

    CoordinateTransform2 CoordinateTransform2::operator/(const CoordinateTransform2& b) const
    {
        CoordinateTransform2 n(*this);
        return n/= b;
    }

    CoordinateTransform2 operator^(const CoordinateTransform2& a, const CoordinateTransform2& b)
    {
        CoordinateTransform2 tmp;
        tmp.a= a.a*b.a + a.b*b.c;
        tmp.b= a.a*b.b + a.b*b.d;
        tmp.c= a.c*b.a + a.d*b.c;
        tmp.d= a.c*b.b + a.d*b.d;

        return tmp;
    }

    std::ostream& operator<<(std::ostream& o, const CoordinateTransform2& t)
    {
        o << "[CoordinateTransform2 ";
        o << t.a;
        o << ", ";
        o << t.b;
        o << ", ";
        o << t.c;
        o << ", ";
        o << t.d;
        o << "]";
        return o;
    }
}

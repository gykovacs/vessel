#include <openipDS/ContinuousFeatureVector.h>

namespace openip
{
    ContinuousFeatureVector::ContinuousFeatureVector()
    : FeatureVector<float>()
    {
    }

    ContinuousFeatureVector::ContinuousFeatureVector(const ContinuousFeatureVector& v)
    : FeatureVector<float>(v)
    {
    }

    ContinuousFeatureVector::ContinuousFeatureVector(int size)
    : FeatureVector<float>(size)
    {
    }

    ContinuousFeatureVector::ContinuousFeatureVector(int classIndex, int size, ...)
    {
        this->classIndex= classIndex;

        va_list vl;
        va_start(vl, size);
        this->resize(size);

        for ( int i= 0; i < size; ++i )
        {
            (*this)(i)= va_arg(vl, double);
        }

        va_end(vl);
    }

    ContinuousFeatureVector::~ContinuousFeatureVector()
    {
    }

    ContinuousFeatureVector& ContinuousFeatureVector::operator=(const ContinuousFeatureVector& b)
    {
        this->FeatureVector<float>::operator=(b);
        return *this;
    }

    ContinuousFeatureVector& ContinuousFeatureVector::operator+=(const ContinuousFeatureVector& b)
    {
        this->FeatureVector<float>::operator+=(b);
        return *this;
    }

    ContinuousFeatureVector& ContinuousFeatureVector::operator-=(const ContinuousFeatureVector& b)
    {
        this->FeatureVector<float>::operator-=(b);
        return *this;
    }

    ContinuousFeatureVector& ContinuousFeatureVector::operator*=(const ContinuousFeatureVector& b)
    {
        this->FeatureVector<float>::operator*=(b);
        return *this;
    }

    ContinuousFeatureVector& ContinuousFeatureVector::operator/=(const ContinuousFeatureVector& b)
    {
        this->FeatureVector<float>::operator/=(b);
        return *this;
    }

    ContinuousFeatureVector ContinuousFeatureVector::operator+(const ContinuousFeatureVector& b) const
    {
        ContinuousFeatureVector tmp(*this);
        return tmp+= b;
    }

    ContinuousFeatureVector ContinuousFeatureVector::operator-(const ContinuousFeatureVector& b) const
    {
        ContinuousFeatureVector tmp(*this);
        return tmp-= b;
    }

    ContinuousFeatureVector ContinuousFeatureVector::operator*(const ContinuousFeatureVector& b) const
    {
        ContinuousFeatureVector tmp(*this);
        return tmp*= b;
    }

    ContinuousFeatureVector ContinuousFeatureVector::operator/(const ContinuousFeatureVector& b) const
    {
        ContinuousFeatureVector tmp(*this);
        return tmp/= b;
    }

    ContinuousFeatureVector& ContinuousFeatureVector::operator=(const float& t)
    {
        this->FeatureVector<float>::operator=(t);
        return *this;
    }

    ContinuousFeatureVector& ContinuousFeatureVector::operator+=(const float& t)
    {
        this->FeatureVector<float>::operator+=(t);
        return *this;
    }

    ContinuousFeatureVector& ContinuousFeatureVector::operator-=(const float& t)
    {
        this->FeatureVector<float>::operator-=(t);
        return *this;
    }

    ContinuousFeatureVector& ContinuousFeatureVector::operator*=(const float& t)
    {
        this->FeatureVector<float>::operator*=(t);
        return *this;
    }

    ContinuousFeatureVector& ContinuousFeatureVector::operator/=(const float& t)
    {
        this->FeatureVector<float>::operator/=(t);
        return *this;
    }

    ContinuousFeatureVector ContinuousFeatureVector::operator+(const float& t) const
    {
        ContinuousFeatureVector tmp(*this);
        return tmp+= t;
    }

    ContinuousFeatureVector ContinuousFeatureVector::operator-(const float& t) const
    {
        ContinuousFeatureVector tmp(*this);
        return tmp-= t;
    }

    ContinuousFeatureVector ContinuousFeatureVector::operator*(const float& t) const
    {
        ContinuousFeatureVector tmp(*this);
        return tmp*= t;
    }

    ContinuousFeatureVector ContinuousFeatureVector::operator/(const float& t) const
    {
        ContinuousFeatureVector tmp(*this);
        return tmp/= t;
    }

    std::ostream& operator<<(std::ostream& o, const ContinuousFeatureVector& v)
    {
        o << "[CFeatureVector - size: ";
        o << v.size();
        o << " : class: ";
        o << v.classIndex;
        o << " : data: ";

        if ( v.size() < 100 )
        {
            for ( ContinuousFeatureVector::const_iterator it= v.begin(); it != v.end(); ++it )
            {
                o << " ";
                o << *it;
            }
        }

        o << "]";

        return o;
    }
}


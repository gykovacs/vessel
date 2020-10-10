#include <openipDS/DiscreteFeatureVector.h>

namespace openip
{
    DiscreteFeatureVector::DiscreteFeatureVector()
    : FeatureVector<int>()
    {
    }

    DiscreteFeatureVector::DiscreteFeatureVector(const DiscreteFeatureVector& v)
    : FeatureVector<int>(v)
    {
    }

    DiscreteFeatureVector::DiscreteFeatureVector(int size)
    : FeatureVector<int>(size)
    {
    }

    DiscreteFeatureVector::DiscreteFeatureVector(int classIndex, int size, ...)
    {
        this->classIndex= classIndex;

        va_list vl;
        va_start(vl, size);
        this->resize(size);

        for ( int i= 0; i < size; ++i )
        {
            (*this)(i)= va_arg(vl, int);
        }

        va_end(vl);
    }

    DiscreteFeatureVector::~DiscreteFeatureVector()
    {
    }

    DiscreteFeatureVector& DiscreteFeatureVector::operator=(const DiscreteFeatureVector& b)
    {
        this->FeatureVector<int>::operator=(b);
        return *this;
    }

    DiscreteFeatureVector& DiscreteFeatureVector::operator+=(const DiscreteFeatureVector& b)
    {
        this->FeatureVector<int>::operator+=(b);
        return *this;
    }

    DiscreteFeatureVector& DiscreteFeatureVector::operator-=(const DiscreteFeatureVector& b)
    {
        this->FeatureVector<int>::operator-=(b);
        return *this;
    }

    DiscreteFeatureVector& DiscreteFeatureVector::operator*=(const DiscreteFeatureVector& b)
    {
        this->FeatureVector<int>::operator*=(b);
        return *this;
    }

    DiscreteFeatureVector& DiscreteFeatureVector::operator/=(const DiscreteFeatureVector& b)
    {
        this->FeatureVector<int>::operator/=(b);
        return *this;
    }

    DiscreteFeatureVector DiscreteFeatureVector::operator+(const DiscreteFeatureVector& b) const
    {
        DiscreteFeatureVector tmp(*this);
        return tmp+= b;
    }

    DiscreteFeatureVector DiscreteFeatureVector::operator-(const DiscreteFeatureVector& b) const
    {
        DiscreteFeatureVector tmp(*this);
        return tmp-= b;
    }

    DiscreteFeatureVector DiscreteFeatureVector::operator*(const DiscreteFeatureVector& b) const
    {
        DiscreteFeatureVector tmp(*this);
        return tmp*= b;
    }

    DiscreteFeatureVector DiscreteFeatureVector::operator/(const DiscreteFeatureVector& b) const
    {
        DiscreteFeatureVector tmp(*this);
        return tmp/= b;
    }

    DiscreteFeatureVector& DiscreteFeatureVector::operator=(const float& t)
    {
        this->FeatureVector<int>::operator=(t);
        return *this;
    }

    DiscreteFeatureVector& DiscreteFeatureVector::operator+=(const float& t)
    {
        this->FeatureVector<int>::operator+=(t);
        return *this;
    }

    DiscreteFeatureVector& DiscreteFeatureVector::operator-=(const float& t)
    {
        this->FeatureVector<int>::operator-=(t);
        return *this;
    }

    DiscreteFeatureVector& DiscreteFeatureVector::operator*=(const float& t)
    {
        this->FeatureVector<int>::operator*=(t);
        return *this;
    }

    DiscreteFeatureVector& DiscreteFeatureVector::operator/=(const float& t)
    {
        this->FeatureVector<int>::operator/=(t);
        return *this;
    }

    DiscreteFeatureVector DiscreteFeatureVector::operator+(const float& t) const
    {
        DiscreteFeatureVector tmp(*this);
        return tmp+= t;
    }

    DiscreteFeatureVector DiscreteFeatureVector::operator-(const float& t) const
    {
        DiscreteFeatureVector tmp(*this);
        return tmp-= t;
    }

    DiscreteFeatureVector DiscreteFeatureVector::operator*(const float& t) const
    {
        DiscreteFeatureVector tmp(*this);
        return tmp*= t;
    }

    DiscreteFeatureVector DiscreteFeatureVector::operator/(const float& t) const
    {
        DiscreteFeatureVector tmp(*this);
        return tmp/= t;
    }

    std::ostream& operator<<(std::ostream& o, const DiscreteFeatureVector& v)
    {
        o << "[DFeatureVector - size: ";
        o << v.size();
        o << " : class: ";
        o << v.classIndex;
        o << " : data: ";

        if ( v.size() < 100 )
        {
            for ( DiscreteFeatureVector::const_iterator it= v.begin(); it != v.end(); ++it )
            {
                o << " ";
                o << *it;
            }
        }

        o << "]";

        return o;
    }
}

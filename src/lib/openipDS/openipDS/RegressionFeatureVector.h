/**
 * @file RegressionFeatureVector.h
 * @author Gyorgy Kovacs <gyuriofkovacs@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * RegressionFeatureVector class represents a vector of features.
 */

#ifndef _REGRESSION_FEATUREVECTOR_H
#define	_REGRESSION_FEATUREVECTOR_H

#include <openipDS/Vector.h>
#include <openipDS/Filter1s.h>

#include <cstdarg>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

namespace openip
{
    template<typename T>
    class RegressionFeatureVector;
  
    template<typename T>
    struct RegressionFeatureVectorComparator
    {
      RegressionFeatureVectorComparator(int coord)
      {
	this->coord= coord;
      }
      
      bool operator()(RegressionFeatureVector<T>& i, RegressionFeatureVector<T>& j)
      {
	return i(coord) < j(coord);
      }
      
      int coord;
    };
  
    /**
    * RegressionFeatureVector represents a vector of features
    */
    template<typename T>
    class RegressionFeatureVector: public Vector<T>
    {
    public:
	/**
	* default constructor
	*/
        RegressionFeatureVector();

	/**
	* copy constructor
	* @param fv instance to copy
	*/
        RegressionFeatureVector(const RegressionFeatureVector& fv);

        /**
         * constructor, for a given size
         * @param size size of the feature vector
         */
        RegressionFeatureVector(int size);

        /**
         * constructor
         * @param targetValue class index of the feature vector
         * @param v elements of the feature vector
         */
        RegressionFeatureVector(float targetValue, Vector<T>& v);

        /**
         * constructor
         * @param targetValue class index of the feature vector
         * @param size size of the feature vector
         * @param ... elements of the feature vector
         */
        RegressionFeatureVector(float targetValue, int size, ...);

	/**
	* destructor
	*/
        ~RegressionFeatureVector();

	/**
	* assignment operator
	* @param b vector to assign
	*/
        RegressionFeatureVector& operator=(const RegressionFeatureVector& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        RegressionFeatureVector& operator+=(const RegressionFeatureVector& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        RegressionFeatureVector& operator-=(const RegressionFeatureVector& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        RegressionFeatureVector& operator*=(const RegressionFeatureVector& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        RegressionFeatureVector& operator/=(const RegressionFeatureVector& b);

        /**
         * adds two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        RegressionFeatureVector operator+(const RegressionFeatureVector& b) const;

        /**
         * subtracts two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        RegressionFeatureVector operator-(const RegressionFeatureVector& b) const;

        /**
         * multiplies two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        RegressionFeatureVector operator*(const RegressionFeatureVector& b) const;

         /**
         * divides two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        RegressionFeatureVector operator/(const RegressionFeatureVector& b) const;

         /**
         * sets the values of this vector instance to paramter t
         *
         * @param t constant value
         * @return this instance
         */
        RegressionFeatureVector& operator=(const float& t);

        /**
         * adds the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        RegressionFeatureVector& operator+=(const float& t);

        /**
         * subtracts the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        RegressionFeatureVector& operator-=(const float& t);

        /**
         * multiplies the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        RegressionFeatureVector& operator*=(const float& t);

        /**
         * divides the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        RegressionFeatureVector& operator/=(const float& t);

        /**
         * adds the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        RegressionFeatureVector operator+(const float& t) const;

        /**
         * subtracts the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        RegressionFeatureVector operator-(const float& t) const;

        /**
         * multiplies the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        RegressionFeatureVector operator*(const float& t) const;

        /**
         * divides the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        RegressionFeatureVector operator/(const float& t) const;

        void shiftToMax();

        void shiftToMin();

        void sort();

        void gauss();

        /**
         * class index of the feature vector
         */
        float targetValue;
    };

    /**
     * prints the description of the parameter RegressionFeatureVector object to the parameter output stream
     * @param o parameter output stream
     * @param v RegressionFeatureVector object to describe
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const openip::RegressionFeatureVector<T>& v);

    /**
     * prints the description of the parameter RegressionFeatureVector object to the parameter output file stream
     * @param o parameter output file stream
     * @param v RegressionFeatureVector object to describe
     * @return reference to the parameter output file stream
     */
    template<typename T>
    std::ofstream& operator<<(std::ofstream& o, const openip::RegressionFeatureVector<T>& v);

    /**
     * fills the parameter RegressionFeatureVector from the parameter input file stream
     * @param o parameter input file stream
     * @param v RegressionFeatureVector object to fill
     * @return reference to the parameter input file stream
     */
    template<typename T>
    std::ifstream& operator>>(std::ifstream& o, openip::RegressionFeatureVector<T>& v);

    /**
     * returns true, if the first coordinate that not equals is smaller in the first parameter, false otherwise
     * @param a first parameter to compare
     * @param b second parameter to compare
     * @return true, if the first coordinate that not equals is smaller in the first parameter, false otherwise
     */
    template<typename T>
    bool operator<(const RegressionFeatureVector<T>& a, const RegressionFeatureVector<T>& b)
    {
        for ( unsigned int i= 0; i < a.size(); ++i )
            if ( a(i) < b(i) )
                return true;
            else if ( b(i) < a(i) )
                return false;

        return false;
    }

    template<typename T>
    RegressionFeatureVector<T>::RegressionFeatureVector()
        : Vector<T>(), targetValue(0)
    {
    }

    template<typename T>
    RegressionFeatureVector<T>::RegressionFeatureVector(const RegressionFeatureVector& v)
    : Vector<T>(v)
    {
        this->targetValue= v.targetValue;
    }

    template<typename T>
    RegressionFeatureVector<T>::RegressionFeatureVector(int size)
        : Vector<T>(size), targetValue(0)
    {
    }

    template<typename T>
    RegressionFeatureVector<T>::RegressionFeatureVector(float targetValue, Vector<T>& v)
        : Vector<T>(v)
    {
        this->targetValue= targetValue;
        this->Vector<T>::operator=(v);
    }

    template<typename T>
    RegressionFeatureVector<T>::RegressionFeatureVector(float targetValue, int size, ...)
        : Vector<T>(size)
    {
        this->targetValue= targetValue;

        va_list vl;
        va_start(vl, size);
        this->resize(size);

        for ( int i= 0; i < size; ++i )
            if ( typeid(T) == typeid(float) )
                (*this)(i)= (double)va_arg(vl,double);
            else
                (*this)(i)= (T)va_arg(vl,T);

        va_end(vl);
    }

    template<typename T>
    RegressionFeatureVector<T>::~RegressionFeatureVector()
    {
    }

    template<typename T>
    RegressionFeatureVector<T>& RegressionFeatureVector<T>::operator=(const RegressionFeatureVector& b)
    {
        this->Vector<T>::operator=(b);
        this->targetValue= b.targetValue;
        return *this;
    }

    template<typename T>
    RegressionFeatureVector<T>& RegressionFeatureVector<T>::operator+=(const RegressionFeatureVector& b)
    {
        this->Vector<T>::operator+=(b);
        return *this;
    }

    template<typename T>
    RegressionFeatureVector<T>& RegressionFeatureVector<T>::operator-=(const RegressionFeatureVector& b)
    {
        this->Vector<T>::operator-=(b);
        return *this;
    }

    template<typename T>
    RegressionFeatureVector<T>& RegressionFeatureVector<T>::operator*=(const RegressionFeatureVector& b)
    {
        this->Vector<T>::operator*=(b);
        return *this;
    }

    template<typename T>
    RegressionFeatureVector<T>& RegressionFeatureVector<T>::operator/=(const RegressionFeatureVector& b)
    {
        this->Vector<T>::operator/=(b);
        return *this;
    }

    template<typename T>
    RegressionFeatureVector<T> RegressionFeatureVector<T>::operator+(const RegressionFeatureVector& b) const
    {
        RegressionFeatureVector<T> tmp(*this);
        return tmp+= b;
    }

    template<typename T>
    RegressionFeatureVector<T> RegressionFeatureVector<T>::operator-(const RegressionFeatureVector& b) const
    {
        RegressionFeatureVector<T> tmp(*this);
        return tmp-= b;
    }

    template<typename T>
    RegressionFeatureVector<T> RegressionFeatureVector<T>::operator*(const RegressionFeatureVector& b) const
    {
        RegressionFeatureVector<T> tmp(*this);
        return tmp*= b;
    }

    template<typename T>
    RegressionFeatureVector<T> RegressionFeatureVector<T>::operator/(const RegressionFeatureVector& b) const
    {
        RegressionFeatureVector tmp(*this);
        return tmp/= b;
    }

    template<typename T>
    RegressionFeatureVector<T>& RegressionFeatureVector<T>::operator=(const float& t)
    {
        this->Vector<T>::operator=(t);
        return *this;
    }

    template<typename T>
    RegressionFeatureVector<T>& RegressionFeatureVector<T>::operator+=(const float& t)
    {
        this->Vector<T>::operator+=(t);
        return *this;
    }

    template<typename T>
    RegressionFeatureVector<T>& RegressionFeatureVector<T>::operator-=(const float& t)
    {
        this->Vector<T>::operator-=(t);
        return *this;
    }

    template<typename T>
    RegressionFeatureVector<T>& RegressionFeatureVector<T>::operator*=(const float& t)
    {
        this->Vector<T>::operator*=(t);
        return *this;
    }

    template<typename T>
    RegressionFeatureVector<T>& RegressionFeatureVector<T>::operator/=(const float& t)
    {
        this->Vector<T>::operator/=(t);
        return *this;
    }

    template<typename T>
    RegressionFeatureVector<T> RegressionFeatureVector<T>::operator+(const float& t) const
    {
        RegressionFeatureVector<T> tmp(*this);
        return tmp+= t;
    }

    template<typename T>
    RegressionFeatureVector<T> RegressionFeatureVector<T>::operator-(const float& t) const
    {
        RegressionFeatureVector<T> tmp(*this);
        return tmp-= t;
    }

    template<typename T>
    RegressionFeatureVector<T> RegressionFeatureVector<T>::operator*(const float& t) const
    {
        RegressionFeatureVector<T> tmp(*this);
        return tmp*= t;
    }

    template<typename T>
    RegressionFeatureVector<T> RegressionFeatureVector<T>::operator/(const float& t) const
    {
        RegressionFeatureVector<T> tmp(*this);
        return tmp/= t;
    }

    template<typename T>
    void RegressionFeatureVector<T>::shiftToMax()
    {
        int maxIdx= 0;
        for ( int i= 1; i < this->size(); ++i )
            if ( (*this)(i) > (*this)(maxIdx) )
                maxIdx= i;
        Vector<T> tmp;
        tmp.resize(this->size());
        for ( int i= 0; i < this->size(); ++i )
            tmp(i)= (*this)((maxIdx + i) < this->size() ? (maxIdx + i) : (maxIdx + i - this->size()));
        for ( int i= 0; i < tmp.size(); ++i )
            (*this)(i)= tmp(i);
    }

    template<typename T>
    void RegressionFeatureVector<T>::shiftToMin()
    {
        int maxIdx= 0;
        for ( int i= 1; i < this->size(); ++i )
            if ( (*this)(i) < (*this)(maxIdx) )
                maxIdx= i;
        Vector<T> tmp;
        tmp.resize(this->size());
        for ( int i= 0; i < this->size(); ++i )
            tmp(i)= (*this)((maxIdx + i) < this->size() ? (maxIdx + i) : (maxIdx + i - this->size()));
        for ( int i= 0; i < tmp.size(); ++i )
            (*this)(i)= tmp(i);
    }

    template<typename T>
    void RegressionFeatureVector<T>::sort()
    {
        std::sort(this->begin(), this->end());
        T tmp;
        for ( int i= 0; i < this->size()/2; ++i )
        {
            tmp= (*this)(i);
            (*this)(i)= (*this)(this->size() - 1 - i);
            (*this)(this->size() - 1 - i)= tmp;
        }
    }

    template<typename T>
    void RegressionFeatureVector<T>::gauss()
    {
        GaussianFilter1<float, float> gf(1, 5, 1, 1, 1, 0);

        Vector<float> v(this->size());
        float f;
        for ( int k= 0; k < this->size(); ++k )
        {
            f= 0;
            for ( int j= 0; j < gf.size(); ++j )
            {
                int p= k + gf(j).first;
                if ( p < 0 )
                    p= this->size() - 1 + p;
                if ( p >= this->size() )
                    p= p - this->size();
                f+= (*this)(p)*gf(j).second;
            }
            v(k)= f;
            //printf("%f ", f);
        }
        //dprintf("\n");
        for ( int k= 0; k < this->size(); ++k )
            (*this)(k)= v(k);
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& o, const openip::RegressionFeatureVector<T>& v)
    {
        o << "[RegressionFeatureVector - size: ";
        o << v.size();
        o << " : target value: ";
        o << v.targetValue;
        o << " : data: ";

        if ( v.size() < 100 )
        {
            for ( typename Vector<T>::const_iterator it= v.begin(); it != v.end(); ++it )
            {
                o << " ";
                o << *it;
            }
        }

        o << "]";

        return o;
    }

    template<typename T>
    std::ofstream& operator<<(std::ofstream& o, const openip::RegressionFeatureVector<T>& v)
    {
        o << v.targetValue << " ";

        for ( typename Vector<T>::const_iterator it= v.begin(); it != v.end(); ++it )
        {
            o << " ";
            o << *it;
        }

        return o;
    }

    template<typename T>
    std::ifstream& operator>>(std::ifstream& o, openip::RegressionFeatureVector<T>& v)
    {
        float n= 0;
        T tmp;

        o >> n;
        v.targetValue= n;

        for ( typename Vector<T>::iterator it= v.begin(); it != v.end(); ++it )
        {
            o >> tmp;
            *it= tmp;
        }

        return o;
    }
}

#endif	/* _FEATUREVECTOR_H */


/**
 * @file FeatureVector.h
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
 * FeatureVector class represents a vector of features.
 */

#ifndef _FEATUREVECTOR_H
#define	_FEATUREVECTOR_H

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
    class FeatureVector;
  
    template<typename T>
    struct FeatureVectorComparator
    {
      FeatureVectorComparator(int coord)
      {
	this->coord= coord;
      }
      
      bool operator()(FeatureVector<T>& i, FeatureVector<T>& j)
      {
	return i(coord) < j(coord);
      }
      
      int coord;
    };
  
    /**
    * FeatureVector represents a vector of features
    */
    template<typename T>
    class FeatureVector: public Vector<T>
    {
    public:
	/**
	* default constructor
	*/
        FeatureVector();

	/**
	* copy constructor
	* @param fv instance to copy
	*/
        FeatureVector(const FeatureVector& fv);

        /**
         * constructor, for a given size
         * @param size size of the feature vector
         */
        FeatureVector(int size);

        /**
         * constructor
         * @param classIndex class index of the feature vector
         * @param v elements of the feature vector
         */
        FeatureVector(int classIndex, Vector<T>& v);

        /**
         * constructor
         * @param classIndex class index of the feature vector
         * @param size size of the feature vector
         * @param ... elements of the feature vector
         */
        FeatureVector(int classIndex, int size, ...);

	/**
	* destructor
	*/
        ~FeatureVector();

	/**
	* assignment operator
	* @param b vector to assign
	*/
        FeatureVector& operator=(const FeatureVector& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        FeatureVector& operator+=(const FeatureVector& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        FeatureVector& operator-=(const FeatureVector& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        FeatureVector& operator*=(const FeatureVector& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        FeatureVector& operator/=(const FeatureVector& b);

        /**
         * adds two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        FeatureVector operator+(const FeatureVector& b) const;

        /**
         * subtracts two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        FeatureVector operator-(const FeatureVector& b) const;

        /**
         * multiplies two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        FeatureVector operator*(const FeatureVector& b) const;

         /**
         * divides two vectors point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        FeatureVector operator/(const FeatureVector& b) const;

         /**
         * sets the values of this vector instance to paramter t
         *
         * @param t constant value
         * @return this instance
         */
        FeatureVector& operator=(const float& t);

        /**
         * adds the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        FeatureVector& operator+=(const float& t);

        /**
         * subtracts the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        FeatureVector& operator-=(const float& t);

        /**
         * multiplies the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        FeatureVector& operator*=(const float& t);

        /**
         * divides the value of paramter t to this instance point-wise
         *
         * @param t constant value
         * @return this instance
         */
        FeatureVector& operator/=(const float& t);

        /**
         * adds the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        FeatureVector operator+(const float& t) const;

        /**
         * subtracts the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        FeatureVector operator-(const float& t) const;

        /**
         * multiplies the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        FeatureVector operator*(const float& t) const;

        /**
         * divides the value of paramter t to this instance and create new instance as result
         *
         * @param t constant value
         * @return new instance
         */
        FeatureVector operator/(const float& t) const;

        void shiftToMax();

        void shiftToMin();

        void sort();

        void gauss();

        /**
         * class index of the feature vector
         */
        int classIndex;
        float weightForeground;
        float weightBackground;
    };

    /**
     * prints the description of the parameter FeatureVector object to the parameter output stream
     * @param o parameter output stream
     * @param v FeatureVector object to describe
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const openip::FeatureVector<T>& v);

    /**
     * prints the description of the parameter FeatureVector object to the parameter output file stream
     * @param o parameter output file stream
     * @param v FeatureVector object to describe
     * @return reference to the parameter output file stream
     */
    template<typename T>
    std::ofstream& operator<<(std::ofstream& o, const openip::FeatureVector<T>& v);

    /**
     * fills the parameter FeatureVector from the parameter input file stream
     * @param o parameter input file stream
     * @param v FeatureVector object to fill
     * @return reference to the parameter input file stream
     */
    template<typename T>
    std::ifstream& operator>>(std::ifstream& o, openip::FeatureVector<T>& v);

    /**
     * returns true, if the first coordinate that not equals is smaller in the first parameter, false otherwise
     * @param a first parameter to compare
     * @param b second parameter to compare
     * @return true, if the first coordinate that not equals is smaller in the first parameter, false otherwise
     */
    template<typename T>
    bool operator<(const FeatureVector<T>& a, const FeatureVector<T>& b)
    {
        for ( unsigned int i= 0; i < a.size(); ++i )
            if ( a(i) < b(i) )
                return true;
            else if ( b(i) < a(i) )
                return false;

        return false;
    }

    template<typename T>
    FeatureVector<T>::FeatureVector()
        : Vector<T>(), classIndex(0), weightForeground(0), weightBackground(0)
    {
    }

    template<typename T>
    FeatureVector<T>::FeatureVector(const FeatureVector& v)
    : Vector<T>(v)
    {
        this->classIndex= v.classIndex;
        this->weightForeground= v.weightForeground;
        this->weightBackground= v.weightBackground;
    }

    template<typename T>
    FeatureVector<T>::FeatureVector(int size)
        : Vector<T>(size), classIndex(0), weightForeground(0), weightBackground(0)
    {
    }

    template<typename T>
    FeatureVector<T>::FeatureVector(int classIndex, Vector<T>& v)
        : Vector<T>(v), weightForeground(0), weightBackground(0)
    {
        this->classIndex= classIndex;
        this->Vector<T>::operator=(v);
    }

    template<typename T>
    FeatureVector<T>::FeatureVector(int classIndex, int size, ...)
        : Vector<T>(size), weightForeground(0), weightBackground(0)
    {
        this->classIndex= classIndex;

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
    FeatureVector<T>::~FeatureVector()
    {
    }

    template<typename T>
    FeatureVector<T>& FeatureVector<T>::operator=(const FeatureVector& b)
    {
        this->Vector<T>::operator=(b);
        this->classIndex= b.classIndex;
        this->weightBackground= b.weightBackground;
        this->weightForeground= b.weightForeground;
        return *this;
    }

    template<typename T>
    FeatureVector<T>& FeatureVector<T>::operator+=(const FeatureVector& b)
    {
        this->Vector<T>::operator+=(b);
        return *this;
    }

    template<typename T>
    FeatureVector<T>& FeatureVector<T>::operator-=(const FeatureVector& b)
    {
        this->Vector<T>::operator-=(b);
        return *this;
    }

    template<typename T>
    FeatureVector<T>& FeatureVector<T>::operator*=(const FeatureVector& b)
    {
        this->Vector<T>::operator*=(b);
        return *this;
    }

    template<typename T>
    FeatureVector<T>& FeatureVector<T>::operator/=(const FeatureVector& b)
    {
        this->Vector<T>::operator/=(b);
        return *this;
    }

    template<typename T>
    FeatureVector<T> FeatureVector<T>::operator+(const FeatureVector& b) const
    {
        FeatureVector<T> tmp(*this);
        return tmp+= b;
    }

    template<typename T>
    FeatureVector<T> FeatureVector<T>::operator-(const FeatureVector& b) const
    {
        FeatureVector<T> tmp(*this);
        return tmp-= b;
    }

    template<typename T>
    FeatureVector<T> FeatureVector<T>::operator*(const FeatureVector& b) const
    {
        FeatureVector<T> tmp(*this);
        return tmp*= b;
    }

    template<typename T>
    FeatureVector<T> FeatureVector<T>::operator/(const FeatureVector& b) const
    {
        FeatureVector tmp(*this);
        return tmp/= b;
    }

    template<typename T>
    FeatureVector<T>& FeatureVector<T>::operator=(const float& t)
    {
        this->Vector<T>::operator=(t);
        return *this;
    }

    template<typename T>
    FeatureVector<T>& FeatureVector<T>::operator+=(const float& t)
    {
        this->Vector<T>::operator+=(t);
        return *this;
    }

    template<typename T>
    FeatureVector<T>& FeatureVector<T>::operator-=(const float& t)
    {
        this->Vector<T>::operator-=(t);
        return *this;
    }

    template<typename T>
    FeatureVector<T>& FeatureVector<T>::operator*=(const float& t)
    {
        this->Vector<T>::operator*=(t);
        return *this;
    }

    template<typename T>
    FeatureVector<T>& FeatureVector<T>::operator/=(const float& t)
    {
        this->Vector<T>::operator/=(t);
        return *this;
    }

    template<typename T>
    FeatureVector<T> FeatureVector<T>::operator+(const float& t) const
    {
        FeatureVector<T> tmp(*this);
        return tmp+= t;
    }

    template<typename T>
    FeatureVector<T> FeatureVector<T>::operator-(const float& t) const
    {
        FeatureVector<T> tmp(*this);
        return tmp-= t;
    }

    template<typename T>
    FeatureVector<T> FeatureVector<T>::operator*(const float& t) const
    {
        FeatureVector<T> tmp(*this);
        return tmp*= t;
    }

    template<typename T>
    FeatureVector<T> FeatureVector<T>::operator/(const float& t) const
    {
        FeatureVector<T> tmp(*this);
        return tmp/= t;
    }

    template<typename T>
    void FeatureVector<T>::shiftToMax()
    {
        int maxIdx= 0;
        for ( unsigned int i= 1; i < this->size(); ++i )
            if ( (*this)(i) > (*this)(maxIdx) )
                maxIdx= i;
        Vector<T> tmp;
        tmp.resize(this->size());
        for ( unsigned int i= 0; i < this->size(); ++i )
            tmp(i)= (*this)((maxIdx + i) < this->size() ? (maxIdx + i) : (maxIdx + i - this->size()));
        for ( unsigned int i= 0; i < tmp.size(); ++i )
            (*this)(i)= tmp(i);
    }

    template<typename T>
    void FeatureVector<T>::shiftToMin()
    {
        int maxIdx= 0;
        for ( unsigned int i= 1; i < this->size(); ++i )
            if ( (*this)(i) < (*this)(maxIdx) )
                maxIdx= i;
        Vector<T> tmp;
        tmp.resize(this->size());
        for ( unsigned int i= 0; i < this->size(); ++i )
            tmp(i)= (*this)((maxIdx + i) < T(this->size()) ? (maxIdx + i) : (maxIdx + i - this->size()));
        for ( unsigned int i= 0; i < tmp.size(); ++i )
            (*this)(i)= tmp(i);
    }

    template<typename T>
    void FeatureVector<T>::sort()
    {
        std::sort(this->begin(), this->end());
        T tmp;
        for ( unsigned int i= 0; i < this->size()/2; ++i )
        {
            tmp= (*this)(i);
            (*this)(i)= (*this)(this->size() - 1 - i);
            (*this)(this->size() - 1 - i)= tmp;
        }
    }

    template<typename T>
    void FeatureVector<T>::gauss()
    {
        GaussianFilter1<float, float> gf(1, 5, 1, 1, 1, 0);

        Vector<float> v(this->size());
        float f;
        for ( unsigned int k= 0; k < this->size(); ++k )
        {
            f= 0;
            for ( unsigned int j= 0; j < gf.size(); ++j )
            {
                int p= k + gf(j).first;
                if ( p < 0 )
                    p= this->size() - 1 + p;
                if ( p >= int(this->size()) )
                    p= p - this->size();
                f+= (*this)(p)*gf(j).second;
            }
            v(k)= f;
            //printf("%f ", f);
        }
        //dprintf("\n");
        for ( unsigned int k= 0; k < this->size(); ++k )
            (*this)(k)= v(k);
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& o, const openip::FeatureVector<T>& v)
    {
        o << "[FeatureVector - size: ";
        o << v.size();
        o << " : class: ";
        o << v.classIndex;
        o << " : weights: ";
        o << v.weightForeground;
        o << ", ";
        o << v.weightBackground;
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
    std::ofstream& operator<<(std::ofstream& o, const openip::FeatureVector<T>& v)
    {
        o << v.classIndex << " " << v.weightForeground << " " << v.weightBackground << " ";
        //o << v.size();

        for ( typename Vector<T>::const_iterator it= v.begin(); it != v.end(); ++it )
        {
            o << " ";
            o << *it;
        }

        return o;
    }

    template<typename T>
    std::ifstream& operator>>(std::ifstream& o, openip::FeatureVector<T>& v)
    {
        int n= 0;
        T tmp;

        o >> n;
        v.classIndex= n;
        o >> v.weightForeground;
        o >> v.weightBackground;
        //o >> n;
        //v.resize(n);

        for ( typename Vector<T>::iterator it= v.begin(); it != v.end(); ++it )
        {
            o >> tmp;
            *it= tmp;
        }

        return o;
    }
}

#endif	/* _FEATUREVECTOR_H */


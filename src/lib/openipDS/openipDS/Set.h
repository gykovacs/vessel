/**
 * @file Set.h
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
 * The Set class represents a set of type T and has
 * several useful operators and member functions defined. The Set is
 * represented as a Vector to make the access of its elements faster,
 * since the StructuringElement and Filter are the inherited class of
 * this Set.
 */


#ifndef _SET_H_
#define	_SET_H_

#include <iostream>
#include <iterator>
#include "Vector.h"
#include <utility>
#include <algorithm>

using namespace std;

namespace openip
{
    /**
     * Set represents a set of type T objects. The set is realized as a vector
     * to achieve fast access insted of fast insertion
     */
    template<typename T>
    class Set : public Vector<T>
    {
    public:
        using Vector<T>::size;
        using Vector<T>::push_back;

        /**
         * Set<T>::iterator typdef
         */
        typedef typename Set<T>::iterator sIt;

        /**
         * Set<T>::const_iterator typdef
         */
        typedef typename Set<T>::const_iterator const_sIt;

        /**
         * default constructor
         */
        Set();

        /**
         * copy constructor
         * @param s instance to copy
         */
        Set(const Set& s);
        
        virtual ~Set(){}

        /**
         * std::set like find function based on algorithm::find.
         * Used for representing set like structures in a vector.
         *
         * @param object object to find
         * @return iterator to the object if found, else this->end()
         */
        const_sIt find(const T& object) const;

        /**
         * std::set like find function based on algorithm::find.
         * Used for representing set like structures in a vector.
         *
         * @param object object to find
         * @return iterator to the object if found, else this->end()
         */
        sIt find(const T& object);

        int contains(const T& object);

        /**
         * std::set like insert function: inserts the element if it
         * is not yet in the vector
         *
         * @param object object to insert
         */
        void insert(const T& object);

        /**
         * assignment operator
         * @param s the instance to copy
         * @return this
         */
        Set& operator=(const Set& s);

        /**
         * union operator, adds the elements of s to this Set
         * @param s the Set whose elements are added to this
         * @return this
         */
        Set& operator+=(const Set& s);

        /**
         * intersetion operator, deletes the elements of this which are not
         * present in s
         * @param s the Set to intersect with
         * @return this
         */
        Set& operator*=(const Set& s);

        /**
         * subtract operator, deletes the elements of Set s from this
         * @param s the Set whose elements are deleted from this
         * @return this
         */
        Set& operator/=(const Set& s);

        /**
         * union operator, union with this and s, returns a new instance
         * @param s Set to union with
         * @return new Set instance
         */
        const Set operator+(const Set& s) const;

        /**
         * intersection operator, intersection with this and s, returns a new
         * instance
         * @param s Set to intersect with
         * @return new Set instance
         */
        const Set operator*(const Set& s) const;

        /**
         * subtract operator, deletes the elements of Set s from this and
         * returns a new instance
         * @param s the Set whose elements are deleted from this
         * @return a new Set instance
         */
        const Set operator/(const Set& s) const;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param s Set object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Set<T>& s)
    {
        o << "[Set - size: ";
        o << s.size();
        o << "]";

        return o;
    }

    template<typename T>
    Set<T>::Set()
    : Vector<T>()
    {
    }

    template<typename T>
    Set<T>::Set(const Set& s)
    : Vector<T>(s)
    {
    }

    template<class T>
    typename Set<T>::const_sIt Set<T>::find(const T& object) const
    {
        return this->Vector<T>::find(object);
    }

    template<typename T>
    typename Set<T>::sIt Set<T>::find(const T& object)
    {
        return this->Vector<T>::find(object);
    }

    template<typename T>
    int Set<T>::contains(const T& object)
    {
        if ( find(object) == this->end() )
            return 0;
        return 1;
    }

    template<typename T>
    void Set<T>::insert(const T& object)
    {
        if ( this->find(object) == this->end() )
            this->push_back(object);
	/*this->push_back(object);
	sort(this->begin(), this->end());
	typename Set<T>::iterator it= unique(this->begin(), this->end());
	this->resize(std::distance(this->begin(), it));*/
    }

    template<typename T>
    Set<T>& Set<T>::operator=(const Set<T>& s)
    {
        this->Vector<T>::operator=(s);
        return *this;
    }

    template<typename T>
    Set<T>& Set<T>::operator+=(const Set<T>& s)
    {
        const_sIt it= s.begin();

        while( it != s.end() )
        {
            this->insert(*it);
            ++it;
        }
        return *this;
    }

    template<typename T>
    Set<T>& Set<T>::operator*=(const Set<T>& s)
    {
        sIt it= this->begin();

        while( it != this->end() )
        {
            if ( s.find(*it) == s.end() )
            {
                this->erase(it);
                it= this->begin();
            }
            ++it;
        }
        return *this;
    }

    template<typename T>
    Set<T>& Set<T>::operator/=(const Set<T>& s)
    {
        typename Set<T>::const_iterator it= s.begin();

        sIt tmp;

        while( it != s.end() )
        {
            tmp= this->find(*it);
            if ( tmp != this->end() )
            {
                this->erase(tmp);
            }
            ++it;
        }

        return *this;
    }

    template<typename T>
    const Set<T> Set<T>::operator+(const Set<T>& s) const
    {
        return Set<T>(*this) += s;
    }

    template<typename T>
    const Set<T> Set<T>::operator*(const Set<T>& s) const
    {
        return Set<T>(*this) *= s;
    }

    template<typename T>
    const Set<T> Set<T>::operator/(const Set<T>& s) const
    {
        return Set<T>(*this)/= s;
    }
}

#endif	/* _SET_H */


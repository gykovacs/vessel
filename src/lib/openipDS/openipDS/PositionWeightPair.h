/**
 * @file PositionWeightPair.h
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
 * The PositionWeightPair represents a building block of Masks and Filters.
 */

#ifndef _POSITIONWEIGHTPAIR_H
#define	_POSITIONWEIGHTPAIR_H

#include <algorithm>
#include <utility>

namespace openip
{
    /**
     * PositionValuePair is the class which is the buildig block of Filters and
     * Masks.
     */
    template<typename T>
    class PositionWeightPair: public std::pair<int, T>
    {
    public:
        using std::pair<int, T>::first;
	using std::pair<int, T>::second;
      
        PositionWeightPair();
        /**
         * constructor to create a PositionValuePair
         * @param first relative position to a pixel
         * @param second weight assigned to the position
         */
        PositionWeightPair(int first, T second);

        /**
         * copy constructor
         * @param p instance to copy
         */
        PositionWeightPair(const PositionWeightPair& p);

        /**
         * destructor
         */
        ~PositionWeightPair();
	
	T operator*(const PositionWeightPair& p);

        /**
         * to string method to describe the matrix
         *
         * @return string description of the matrix
         */
        //std::string toString() const;
    };

    /**
     * prints the description of the parameter PositionWeightPair object to the parameter output stream
     * @param o parameter output stream
     * @param v PositionWeightPair object to describe
     * @return reference to the parameter output stream
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const PositionWeightPair<T>& v)
    {
        o << "[" << v.first << ", " << v.second << "]";

        return o;
    }

    template<typename T>
    PositionWeightPair<T>::PositionWeightPair()
    : pair<int, T>(0, 0)
    {
    }

    template<typename T>
    PositionWeightPair<T>::PositionWeightPair(int first, T second)
    : pair<int, T>(first, second)
    {
    }

    template<typename T>
    PositionWeightPair<T>::PositionWeightPair(const PositionWeightPair& p)
    : pair<int, T>(p)
    {
    }

    template<typename T>
    PositionWeightPair<T>::~PositionWeightPair()
    {
    }
    
    template<typename T>
    T PositionWeightPair<T>::operator*(const PositionWeightPair& p)
    {
      return this->second*p.second;
    }

    /*template<typename T>
    std::ostream& operator<<(std::ostream& o, const PositionWeightPair<T>& v)
    {
        o << "[";
        o << v.first;
        o << ", ";
        o << v.second;
        o << "]";

        return o;
    }*/
}

#endif	/* _POSITIONWEIGHTPAIR_H */


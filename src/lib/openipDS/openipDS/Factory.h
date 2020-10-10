/**
 * @file Factory.h
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
 * The Factory class represents a realization of the Factory Design Pattern and
 * has several useful operators and member functions defined.
 */

#ifndef _FACTORY_H_
#define _FACTORY_H_

#include <queue>

using namespace std;

namespace openip
{
    /**
     * Factory is used to realize the Factory Design Pattern
     */
    template <typename T>
    class Factory : public queue<T>
    {
    public:
        /**
         * default constructor
         */
        Factory();

        /**
         * initializes n elements of T type in the Factory queue
         */
        virtual void _init(unsigned int n)= 0;

    };

    template<typename T>
    Factory<T>::Factory()
    {
    }

    /**
    * << operator
    *
    * @param o output stream object
    * @param f Factory object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Factory<T>& f)
    {
        o << "[Factory - size: ";
        o << f.size();
        o << "]";

        return o;
    }

}
#endif

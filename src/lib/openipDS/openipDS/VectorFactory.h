/**
 * @file VectorFactory.h
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
 * The VectorFactory class related to the Factory Design Pattern
 */

#ifndef _VECTOR_FACTORY_
#define _VECTOR_FACTORY_

#include <queue>

#include <openipDS/Factory.h>
#include <openipDS/Vector.h>

using namespace std;

namespace openip
{
    /**
     * VectorFactory is used to realize Factory Design Pattern on Images
     */
    template <typename T>
    class VectorFactory : public Factory<Vector<T>* >
    {
    public:

        /**
         * default constructor
         * @param length length of vectors to instantiate
         * @param n number of image in the factory, default value 0
         */
        VectorFactory( unsigned int length, unsigned int n= 0 );

        /**
         * _initialize the Factory with n Images with size specified before
         * @param n number of image instances
         */
        void _init( unsigned int n );

        /**
         * set the size of Images to create before instantiating the objects
         * @param length length of vectors to instantiate
         */
        void setVectorSize( unsigned int length );

        /**
         * set the size of Images and the number of instances to instantiate
         * @param length length of vectors to instantiate
         * @param n number of instances in the factory
         */
        void setVectorSize( unsigned int length, unsigned int n );

        /**
         * get an image from the Factory
         * @return pointer to an image instance
         */
        Vector<T>* get();

        /**
         * push back an image pointer into the Factory
         * @param b image pointer to push into the Factory queue
         */
        void push(Vector<T>* b);

        /**
         * number of rows in the image instances
         */
        unsigned int _length;

        /**
         * number of elements in the image instances (_w*_h)
         */
        unsigned int _n;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param v VectorFactory object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<class T>
    std::ostream& operator<<(std::ostream& o, const VectorFactory<T>& v);
}


template<class T>
std::ostream& openip::operator<<(std::ostream& o, const openip::VectorFactory<T>& v)
{
    o << "[VectorFactory - size: ";
    o << v.size();
    o << "]";

    return o;
}

template<class T>
openip::VectorFactory<T>::VectorFactory( unsigned int length, unsigned int n )
    : _length(length)
{
    _init(n);
}

template<class T>
openip::Vector<T>* openip::VectorFactory<T>::get()
{
	Vector<T>* b= this->front();
	this->pop();
	return b;
}

template<class T>
void openip::VectorFactory<T>::push(Vector<T>* b)
{
	this->queue<Vector<T>*>::push(b);
}

template<class T>
void openip::VectorFactory<T>::setVectorSize( unsigned int length )
{
    _length= length;
    _init(_n);
}

template<class T>
void openip::VectorFactory<T>::setVectorSize( unsigned int length, unsigned int n )
{
    _length= length;
    _n= n;
    _init(_n);
}

template<class T>
void openip::VectorFactory<T>::_init( unsigned int n )
{
    for ( unsigned int i= 0; i < n; ++i )
    {
        Vector<T>* b= NULL;
        b= new Vector<T>(_length);

        if ( b == NULL )
                printf("cannot instantiate Vector %d", _length);

        push(b);
    }
}

#endif

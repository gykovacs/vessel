/**
 * @file VolumeFactory.h
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
 * This program is distributed in the hopee that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The VolumeFactory class related to the Factory Design Pattern
 */

#ifndef _VOLUMEFACTORY_H
#define	_VOLUMEFACTORY_H

#include <queue>

#include <openipDS/Factory.h>
#include <openipDS/Image.h>

using namespace std;

namespace openip
{
    /**
     * VolumeFactory is used to realize Factory Design Pattern on Volumes
     */
    template <typename T>
    class VolumeFactory : public Factory<Volume<T>* >
    {
    public:

        /**
         * default constructor
         * @param slices number of slices of volumes in the Factory
         * @param rows number of rows of volumes in the Factory
         * @param columns number of columns of volumes in the Factory
         * @param n number of image in the factory, default value 0
         * @param topBorder number of top border rows of the image
         * @param bottomBorder number of bottom border rows of the image
         * @param leftBorder number of left border columns of the image
         * @param rightBorder number of right border columns of the image
         * @param frontBorder number of front border columns of the volume
         * @param backBorder number of back border columns of the volume
         * @param borderMode mode of filling border pixels
         */
        VolumeFactory( unsigned int slices, unsigned int rows, unsigned int columns, unsigned int n= 0, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int frontBorder= 0, int backBorder= 0, int borderMode= BORDER_MODE_MIRRORED );

        /**
         * _initialize the Factory with n Volumes with size specified before
         * @param n number of image instances
         */
        void _init( unsigned int n );

        /**
         * set the size of Volumes to create before instantiating the objects
         * @param slices number of volume slices
         * @param rows number of volume rows
         * @param columns number of volume columns
         */
        void setVolumeSize( unsigned int slices, unsigned int rows, unsigned int columns );

        /**
         * set the size of Volumes and the number of instances to instantiate
         * @param slices number of volume slices
         * @param rows number of volume rows
         * @param columns number of volume columns
         * @param n number of instances in the factory
         */
        void setVolumeSize( unsigned int slices, unsigned int rows, unsigned int columns, unsigned int n );

        /**
         * get an volume from the Factory
         * @return pointer to an volume instance
         */
        Volume<T>* get();

        /**
         * push back an volume pointer into the Factory
         * @param b volume pointer to push into the Factory queue
         */
        void push(Volume<T>* b);

        /**
         * number of columns in the volume instances
         */
        unsigned int _columns;

        /**
         * number of rows in the volume instances
         */
        unsigned int _rows;

        /**
         * number of slices in the volume
         */
        unsigned int _slices;

        /**
         * number of elements in the volume instances (_rows*_columns*_slices)
         */
        unsigned int _n;

        /**
         * number of top border rows of the volume
         */
        int topBorder;

        /**
         * number of bottom border rows of the volume
         */
        int bottomBorder;

        /**
         * number of left border columns of the volume
         */
        int leftBorder;

        /**
         * number of right border columns of the volume
         */
        int rightBorder;

        /**
         * number of front border columns of the volume
         */
        int frontBorder;

        /**
         * number of back border columns of the volume
         */
        int backBorder;

        /**
         * mode of filling border pixels
         */
        int borderMode;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param v VolumeFactory object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<class T>
    std::ostream& operator<<(std::ostream& o, const VolumeFactory<T>& v);
}

template<class T>
openip::VolumeFactory<T>::VolumeFactory( unsigned int slices, unsigned int rows, unsigned int columns, unsigned int n, int topB, int bottomB, int leftB, int rightB, int frontB, int backB, int borderM )
    : _slices(slices), _columns(columns), _rows(rows), topBorder(topB), bottomBorder(bottomB), leftBorder(leftB), rightBorder(rightB), frontBorder(frontB), backBorder(backB), borderMode(borderM)
{
    _init(n);
}

template<class T>
openip::Volume<T>* openip::VolumeFactory<T>::get()
{
    Volume<T>* b= this->front();
    this->pop();
    return b;
}

template<class T>
void openip::VolumeFactory<T>::push(Volume<T>* b)
{
    this->queue<Volume<T>*>::push(b);
}

template<class T>
void openip::VolumeFactory<T>::setVolumeSize( unsigned int slices, unsigned int rows, unsigned int columns )
{
    _columns= columns;
    _rows= rows;
    _slices= slices;
    _init(_n);
}

template<class T>
void openip::VolumeFactory<T>::setVolumeSize( unsigned int slices, unsigned int rows, unsigned int columns, unsigned int n )
{
    _columns= columns;
    _rows= rows;
    _slices= slices;
    _n= n;
    _init(_n);
}

template<class T>
void openip::VolumeFactory<T>::_init( unsigned int n )
{
    for ( unsigned int i= 0; i < n; ++i )
    {
        Volume<T>* b= NULL;
        b= new Volume<T>(_slices, _rows, _columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

        if ( b == NULL )
                printf("cannot instantiate Volume %d %d %d", _slices, _columns, _rows);

        push(b);
    }
}

template<class T>
std::ostream& openip::operator<<(std::ostream& o, const openip::VolumeFactory<T>& v)
{
    o << "[VolumeFactory - size: ";
    o << v.size();
    o << "]";

    return o;
}


#endif	/* _VOLUMEFACTORY_H */


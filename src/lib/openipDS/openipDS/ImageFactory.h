/**
 * @file ImageFactory.h
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
 * The ImageFactory class related to the Factory Design Pattern
 */

#ifndef _IMAGE_FACTORY_
#define _IMAGE_FACTORY_

#include <queue>

#include <openipDS/Factory.h>
#include <openipDS/Image.h>

using namespace std;

namespace openip
{
    /**
     * ImageFactory is used to realize Factory Design Pattern on Images
     */
    template <typename T>
    class ImageFactory : public Factory<Image<T>* >
    {
    public:

        /**
         * default constructor
         * @param rows number of rows of images in the Factory
         * @param columns number of columns of images in the Factory
         * @param n number of image in the factory, default value 0
         * @param topBorder number of top border rows of the image
         * @param bottomBorder number of bottom border rows of the image
         * @param leftBorder number of left border columns of the image
         * @param rightBorder number of right border columns of the image
         * @param borderMode mode of filling border pixels
         */
        ImageFactory( unsigned int rows, unsigned int columns, unsigned int n= 0, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED );

        /**
         * _initialize the Factory with n Images with size specified before
         * @param n number of image instances
         */
        void _init( unsigned int n );

        /**
         * set the size of Images to create before instantiating the objects
         * @param rows number of image rows
         * @param columns number of image columns
         */
        void setImageSize( unsigned int rows, unsigned int columns );

        /**
         * set the size of Images and the number of instances to instantiate
         * @param rows number of image rows
         * @param columns number of image columns
         * @param n number of instances in the factory
         */
        void setImageSize( unsigned int rows, unsigned int columns, unsigned int n );

        /**
         * get an image from the Factory
         * @return pointer to an image instance
         */
        Image<T>* get();

        /**
         * push back an image pointer into the Factory
         * @param b image pointer to push into the Factory queue
         */
        void push(Image<T>* b);

        /**
         * number of columns in the image instances
         */
        unsigned int _columns;

        /**
         * number of rows in the image instances
         */
        unsigned int _rows;

        /**
         * number of elements in the image instances (_w*_h)
         */
        unsigned int _n;

        /**
         * number of top border rows of the image
         */
        int topBorder;

        /**
         * number of bottom border rows of the image
         */
        int bottomBorder;

        /**
         * number of left border columns of the image
         */
        int leftBorder;

        /**
         * number of right border columns of the image
         */
        int rightBorder;

        /**
         * mode of filling border pixels
         */
        int borderMode;
    };

    /**
    * << operator
    *
    * @param o output stream object
    * @param imageFactory ImageFactory object to describe
    * @return the parameter output stream
    * prints the description of the parameter object to the parameter output stream
    */
    template<class T>
    std::ostream& operator<<(std::ostream& o, const ImageFactory<T>& imageFactory);
}

template<class T>
openip::ImageFactory<T>::ImageFactory( unsigned int rows, unsigned int columns, unsigned int n, int topB, int bottomB, int leftB, int rightB, int borderM )
    : _columns(columns), _rows(rows), topBorder(topB), bottomBorder(bottomB), leftBorder(leftB), rightBorder(rightB), borderMode(borderM)
{
    _init(n);
}

template<class T>
openip::Image<T>* openip::ImageFactory<T>::get()
{
	Image<T>* b= this->front();
	this->pop();
	return b;
}

template<class T>
void openip::ImageFactory<T>::push(Image<T>* b)
{
	this->queue<Image<T>*>::push(b);
}

template<class T>
void openip::ImageFactory<T>::setImageSize( unsigned int rows, unsigned int columns )
{
    _columns= columns;
    _rows= rows;
    _init(_n);
}

template<class T>
void openip::ImageFactory<T>::setImageSize( unsigned int rows, unsigned int columns, unsigned int n )
{
    _columns= columns;
    _rows= rows;
    _n= n;
    _init(_n);
}

template<class T>
void openip::ImageFactory<T>::_init( unsigned int n )
{
    for ( unsigned int i= 0; i < n; ++i )
    {
        Image<T>* b= NULL;
        b= new Image<T>(_rows, _columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

        if ( b == NULL )
                printf("cannot instantiate Image %d %d", _columns, _rows);

        push(b);
    }
}

template<class T>
std::ostream& openip::operator<<(std::ostream& o, const openip::ImageFactory<T>& imageFactory)
{
    o << "[ImageFactory - size: ";
    o << imageFactory.size();
    o << "]";

    return o;
}

#endif

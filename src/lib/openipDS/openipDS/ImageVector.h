/**
 * @file ImageVector.h
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
 * The ImageVector class is a simple container for images.
 */
#ifndef IMAGEVECTOR_H
#define	IMAGEVECTOR_H

#include <openipDS/Image.h>

namespace openip
{
    /**
     * ImageVector is a simple container for images
     */
    template<typename T>
    class ImageVector: public Vector<Image<T> >
    {
    public:
        using Vector<Image<T> >::operator();
        using Vector<Image<T> >::resize;

        /**
         * default constructor
         */
        ImageVector();

        /**
         * copy constructor
         */
        ImageVector(const ImageVector& iv);

        /**
         * destructor
         */
        ~ImageVector();

        /**
         * resizes each image in the vector according to the parameters
         * @param rows number of rows
         * @param columns number of columns
         * @param topBorder top border
         * @param bottomBorder bottom border
         * @param leftBorder left border
         * @param rightBorder right border
         * @param borderMode border mode
         */
        void resizeImages(int rows, int columns, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

        /**
         * resizes each image in the vector according to the parameters
         * @param rows number of rows
         * @param columns number of columns
         * @param b border parmeters
         */
        void resizeImages(int rows, int columns, Border2 b);

        /**
         * resizes each image in the vector to the size of the parameter image
         * @param b the images are resized to the size of this parameter
         */
        template<typename S>
        void resizeImages(Image<S>& b);
    };

    template<typename T>
    ImageVector<T>::ImageVector()
    : Vector<Image<T> >()
    {
    }

    template<typename T>
    ImageVector<T>::ImageVector(const ImageVector& iv)
    : Vector<Image<T> >(iv)
    {
    }

    template<typename T>
    ImageVector<T>::~ImageVector()
    {
    }

    template<typename T> template<typename S>
    void ImageVector<T>::resizeImages(Image<S>& b)
    {
        for ( typename Vector<Image<T> >::iterator it= this->begin(); it != this->end(); ++it )
        {
            (*it).resizeImage(b);
        }
    }

    template<typename T>
    void ImageVector<T>::resizeImages(int rows, int columns, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        for ( typename Vector<Image<T> >::iterator it= this->begin(); it != this->end(); ++it )
        {
            (*it).resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        }
    }

    template<typename T>
    void ImageVector<T>::resizeImages(int rows, int columns, Border2 b)
    {
        this->resizeImages(rows, columns, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
    }
}

#endif	/* IMAGE3_H */


/* 
 * File:   VolumeVector.h
 * Author: gykovacs
 *
 * Created on July 20, 2011, 3:45 PM
 */

#ifndef VOLUMEVECTOR_H
#define	VOLUMEVECTOR_H

#include <openipDS/Volume.h>

namespace openip
{
    /**
     * ImageVector is a simple container for images
     */
    template<typename T>
    class VolumeVector: public Vector<Volume<T> >
    {
    public:
        using Vector<Volume<T> >::operator();
        using Vector<Volume<T> >::resize;

        /**
         * default constructor
         */
        VolumeVector();

        /**
         * copy constructor
         */
        VolumeVector(const VolumeVector& iv);

        /**
         * destructor
         */
        ~VolumeVector();

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
        void resizeVolumes(int slices, int rows, int columns, int frontBorder= 0, int backBorder= 0, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

        /**
         * resizes each image in the vector according to the parameters
         * @param rows number of rows
         * @param columns number of columns
         * @param b border parmeters
         */
        void resizeVolumes(int slices, int rows, int columns, Border3 b);

        /**
         * resizes each image in the vector to the size of the parameter image
         * @param b the images are resized to the size of this parameter
         */
        template<typename S>
        void resizeVolumes(Volume<S>& b);
    };

    template<typename T>
    VolumeVector<T>::VolumeVector()
    : Vector<Volume<T> >()
    {
    }

    template<typename T>
    VolumeVector<T>::VolumeVector(const VolumeVector& iv)
    : Vector<Volume<T> >(iv)
    {
    }

    template<typename T>
    VolumeVector<T>::~VolumeVector()
    {
    }

    template<typename T> template<typename S>
    void VolumeVector<T>::resizeVolumes(Volume<S>& b)
    {
        for ( typename Vector<Volume<T> >::iterator it= this->begin(); it != this->end(); ++it )
        {
            (*it).resizeVolume(b);
        }
    }

    template<typename T>
    void VolumeVector<T>::resizeVolumes(int slices, int rows, int columns, int frontBorder, int backBorder, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        for ( typename Vector<Volume<T> >::iterator it= this->begin(); it != this->end(); ++it )
        {
            (*it).resizeVolume(slices, rows, columns, frontBorder, backBorder, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        }
    }

    template<typename T>
    void VolumeVector<T>::resizeVolumes(int slices, int rows, int columns, Border3 b)
    {
        this->resizeVolumes(slices, rows, columns, b.frontBorder, b.backBorder, b.topBorder, b.bottomBorder, b.leftBorder, b.rightBorder, b.borderMode);
    }
}

#endif	/* VOLUMEVECTOR_H */


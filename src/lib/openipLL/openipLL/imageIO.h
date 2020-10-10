/**
 * @file imageIO.h
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
 * This file contains image I/O functions, using the Image class.
 */

#ifndef _IMAGE_IO_
#define _IMAGE_IO_

#include <openipDS/Image.h>

#include <openipIO/openipIO.h>

#include <stdio.h>

namespace openip
{
    /**
     * Cache constant for reading rgb images
     */
    const unsigned long long CACHE_READ_RGB_IMAGE= CACHE_UCHAR_1 * 3;
    
    /**
     * Cache constant for reading gray level images
     */
    const unsigned long long CACHE_READ_GRAY_IMAGE= CACHE_UCHAR_1;

    /**
     * reads an rgb image with the specified border
     * @param filename image filename
     * @param r red channel image
     * @param g green channel image
     * @param b blue channel image
     * @param topBorder top border
     * @param bottomBorder bottom border
     * @param leftBorder left border
     * @param rightBorder right border
     * @param borderMode border mode
     * @return ImageIOError error code
     */
    template<typename T>
    int readImage(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * reads an rgb image with the specified border
     * @param filename image filename
     * @param r red channel image
     * @param g green channel image
     * @param b blue channel image
     * @param border border
     * @return ImageIOError error code
     */
    template<typename T>
    int readImage(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, Border2 border);

    /**
     * writes an rgb image into file
     * @param filename image filename
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @return ImageIOError error code
     */
    template<typename T>
    int writeImage(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b);

    /**
     * reads a grayscale image with the specified border
     * @param filename image filename
     * @param r intensity image
     * @param channel channel identifier
     * @param topBorder top border
     * @param bottomBorder bottom border
     * @param leftBorder left border
     * @param rightBorder right border
     * @param borderMode border mode
     * @return ImageIOError error code
     */
    template<typename T>
    int readImage(const char* filename, Image<T>& r, int channel= READ_CHANNEL_GRAYSCALE, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * reads a grayscale image with the specified border
     * @param filename image filename
     * @param r intensity image
     * @param border the specified border
     * @param channel channel identifier
     * @return ImageIOError error code
     */
    template<typename T>
    int readImage(const char* filename, Image<T>& r, Border2 border, int channel= READ_CHANNEL_GRAYSCALE);

    /**
     * writes an intensity image into file
     * @param filename image filename
     * @param r intensity image
     * @return ImageIOError error code
     */
    template<typename T>
    int writeImage(const char* filename, Image<T>& r);
    
    
    template<typename T>
    int readVolume(const char* filename, Volume<T>& v, int frontBorder= 0, int backBorder= 0, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);
    
    template<typename T>
    int readVolume(const char* filename, Volume<T>& v, Border3 b);
    
    template<typename T>
    int readVolume(const char* filename, FramedVolume<T>& v, int frontBorder= 0, int backBorder= 0, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);
    
    template<typename T>
    int readVolume(const char* filename, FramedVolume<T>& v, Border3 b);

    template<typename T>
    int writeVolume(const char* filename, const Volume<T>& v);
    
    template<typename T>
    int writeVolume(const char* filename, const FramedVolume<T>& v);
    

    template<typename T>
    int writeImage(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b)
    {
        return writeIMG(filename, r, g, b);
    }

    template<typename T>
    int readImage(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        return readIMG(filename, r, g, b, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
    }

    template<typename T>
    int readImage(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, Border2 border)
    {
        return readIMG(filename, r, g, b, border.topBorder, border.bottomBorder, border.leftBorder, border.rightBorder, border.borderMode);
    }

    template<typename T>
    int writeImage(const char* filename, Image<T>& r)
    {
        return writeIMG(filename, r);
    }

    template<typename T>
    int readImage(const char* filename, Image<T>& r, int channel, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        return readIMG(filename, r, channel, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
    }

    template<typename T>
    int readImage(const char* filename, Image<T>& r, Border2 border, int channel)
    {
        return readIMG(filename, r, channel, border.topBorder, border.bottomBorder, border.leftBorder, border.rightBorder, border.borderMode);
    }

    template<typename T>
    int readVolume(const char* filename, Volume<T>& v, int frontBorder, int backBorder, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        return readVOL(filename, v, frontBorder, backBorder, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
    }
    
    template<typename T>
    int readVolume(const char* filename, Volume<T>& v, Border3 b)
    {
        return readVOL(filename, v, b);
    }
    
    template<typename T>
    int readVolume(const char* filename, FramedVolume<T>& v, int frontBorder, int backBorder, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        return readVOL(filename, v, frontBorder, backBorder, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
    }
    
    template<typename T>
    int readVolume(const char* filename, FramedVolume<T>& v, Border3 b)
    {
        return readVOL(filename, v, b);
    }

    template<typename T>
    int writeVolume(const char* filename, const Volume<T>& v)
    {
        return writeVOL(filename, v);
    }
    
    template<typename T>
    int writeVolume(const char* filename, const FramedVolume<T>& v)
    {
        return writeVOL(filename, v);
    }
}
#endif

/**
 * @file openipIO.h
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
 * image input/output template methods, the methods does not depend on
 * openipDS data structures
 */

#ifndef _IMAGEIO_H_
#define _IMAGEIO_H_

#include <cstdio>
#include <cstdlib>
#include <string>

#include <openipIO/configIO.h>
#include <openipIO/ImageInfo.h>

#include <openipDS/Image.h>
#include <openipDS/Volume.h>
#include <openipDS/FramedVolume.h>

#ifdef USE_JPEG
    #include <openipIO/jpgIO.h>
#endif

#ifdef USE_LIBPNG
    #include <openipIO/pngIO.h>
#endif

#ifdef USE_BMP
    #include <openipIO/bmpIO.h>
#endif

#ifdef USE_TIFF
    #include <openipIO/tifIO.h>
#endif

#ifdef USE_LIBPNG
    #include <openipIO/pgmIO.h>
#endif

#ifdef _GIF_IO_
    #include <openipIO/gifIO.h>
#endif

#ifdef USE_MINC2
    #include <openipIO/mincIO.h>
#endif

#ifdef USE_FLOAT
    #include <openipIO/floatIO.h>
#endif

#ifdef USE_PGM
    #include <openipIO/pgmIO.h>
#endif

/**
 * namespace of all openip objects and functions
 */
namespace openip
{
    /** converts the parameter string to lowercase
     * @param str string to convert to lowercase
     * @return lowercase string
     * @return ImageIOError error code
     */
    std::string toLowercase(std::string str);

    /**
     * reads the size of an image, based on the extension of it and returns
     * the number of rows and columns in the reference parameters
     * @param filename image filename to read the size of
     * @param ii ImageInfo object
     * @return returns an ImageIOError code
     */
    int readIMGInfo(const char* filename, ImageInfo& ii);

    /**
     * reads the three channels of an image file into the r g and b images
     * @param filename image file to read
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @param topBorder number of top border rows
     * @param bottomBorder number of bottom border rows
     * @param leftBorder number of left border columns
     * @param rightBorder number of right border columns
     * @param borderMode border mode
     * @return ImageIOError error code
     */
    template<typename T>
    int readIMG(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * reads one channel of a image file into the img image
     * @param filename image file to read
     * @param img image to read into
     * @param channel channel identifier
     * @param topBorder number of top border rows
     * @param bottomBorder number of bottom border rows
     * @param leftBorder number of left border columns
     * @param rightBorder number of right border columns
     * @param borderMode border mode
     * @return ImageIOError error code
     */
    template<typename T>
    int readIMG(const char* filename, Image<T>& r, int channel= READ_CHANNEL_GRAYSCALE, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * writes columns*rows elements from the r g and b images to the red, green and blue channels into the file
     * @param filename filename to write the bmp into
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @return ImageIOError error code
     */
    template<typename T>
    int writeIMG(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b);

    /**
     * writes columns*rows elements from img image into the file
     * @param filename filename to write the bmp into
     * @param img red channel
     * @return ImageIOError error code
     */
    template<typename T>
    int writeIMG(const char* filename, Image<T>& img);

    template<typename T>
    int readVOL(const char* filename, Volume<T>& v, int frontBorder= 0, int backBorder= 0, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);
    
    template<typename T>
    int readVOL(const char* filename, Volume<T>& v, Border3 b);
    
    template<typename T>
    int readVOL(const char* filename, FramedVolume<T>& v, int frontBorder= 0, int backBorder= 0, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);
    
    template<typename T>
    int readVOL(const char* filename, FramedVolume<T>& v, Border3 b);

    template<typename T>
    int writeVOL(const char* filename, const Volume<T>& v);
    
    template<typename T>
    int writeVOL(const char* filename, const FramedVolume<T>& v);

    template<typename T>
    int readIMG(const char* filename, Image<T>& img, int channel, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        std::string fn(filename);
        fn= toLowercase(fn).substr(fn.length() - 3, 3);

        img.filename= std::string(filename);

        if ( fn == std::string("png") )
            #ifdef USE_LIBPNG
                return readPNG(filename, img, channel, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("jpg") )
            #ifdef USE_JPEG
                return readJPG(filename, img, channel, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("tif") )
            #ifdef USE_TIFF
                return readTIF(filename, img, channel, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("bmp") )
            #ifdef USE_BMP
                return readBMP(filename, img, channel, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
	else if ( fn == std::string("pgm") )
            #ifdef USE_PGM
                return readPGM(filename, img, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("oat") )
            #ifdef USE_FLOAT
                return readFLOAT(filename, img, channel, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else
        {
            return UNKNOWN_EXTENSION;
        }
    }

    template<typename T>
    int readIMG(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        std::string fn(filename);
        fn= toLowercase(fn).substr(fn.length() - 3, 3);

        if ( fn == std::string("png") )
            #ifdef USE_LIBPNG
                return readPNG(filename, r, g, b, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("jpg") )
            #ifdef USE_JPEG
                return readJPG(filename, r, g, b, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("tif") )
            #ifdef USE_TIFF
                return readTIF(filename, r, g, b, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("bmp") )
            #ifdef USE_BMP
                return readBMP(filename, r, g, b, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("oat") )
            #ifdef USE_FLOAT
                return readFLOAT(filename, r, g, b, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAEG_FORMAT;
            #endif
        else
        {
            return UNKNOWN_EXTENSION;
        }
    }

    template<typename T>
    int writeIMG(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b)
    {
        std::string fn(filename);
        fn= toLowercase(fn).substr(fn.length() - 3, 3);

        if ( fn == std::string("png") )
            #ifdef USE_LIBPNG
                return writePNG(filename, r, g, b);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("jpg") )
            #ifdef USE_JPEG
                return writeJPG(filename, r, g, b);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("tif") )
            #ifdef USE_TIFF
                return writeTIF(filename, r, g, b);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("bmp") )
            #ifdef USE_BMP
                return writeBMP(filename, r, g, b);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("oat") )
            #ifdef USE_FLOAT
                return writeFLOAT(filename, r, g, b);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else
            return UNKNOWN_EXTENSION;
    }

    template<typename T>
    int writeIMG(const char* filename, Image<T>& img)
    {
        std::string fn(filename);
        fn= toLowercase(fn).substr(fn.length() - 3, 3);

        if ( fn == std::string("png") )
            #ifdef USE_LIBPNG
                return writePNG(filename, img);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("jpg") )
            #ifdef USE_JPEG
                return writeJPG(filename, img);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("tif") )
            #ifdef USE_TIFF
                return writeTIF(filename, img);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("bmp") )
            #ifdef USE_BMP
                return writeBMP(filename, img);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
	else if ( fn == std::string("pgm") )
            #ifdef USE_PGM
                return writePGM(filename, img);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else if ( fn == std::string("oat") )
            #ifdef USE_FLOAT
                return writeFLOAT(filename, img);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else
            return UNKNOWN_EXTENSION;
    }
    
    template<typename T>
    int readVOL(const char* filename, Volume<T>& 
#ifdef USE_MINC2
    v
#endif
, int 
#ifdef USE_MINC2
frontBorder
#endif
, int 
#ifdef USE_MINC2
backBorder
#endif
, int
#ifdef USE_MINC2
topBorder
#endif
, int 
#ifdef USE_MINC2
bottomBorder
#endif
, int 
#ifdef USE_MINC2
leftBorder
#endif
, int 
#ifdef USE_MINC2
rightBorder
#endif
, int 
#ifdef USE_MINC2 
borderMode
#endif
    )
    {
        std::string fn(filename);
        fn= toLowercase(fn).substr(fn.length() - 3, 3);

        if ( fn == std::string("mnc") )
            #ifdef USE_MINC2
                return readMNC(filename, v, frontBorder, backBorder, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else
            return UNKNOWN_EXTENSION;
    }
    
    template<typename T>
    int readVOL(const char* filename, Volume<T>& 
#ifdef USE_MINC2
v
#endif
, Border3 
#ifdef USE_MINC2
b
#endif
    )
    {
        std::string fn(filename);
        fn= toLowercase(fn).substr(fn.length() - 3, 3);

        if ( fn == std::string("mnc") )
            #ifdef USE_MINC2
                return readMNC(filename, v, b);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else
            return UNKNOWN_EXTENSION;
    }
    
    template<typename T>
    int readVOL(const char* filename, FramedVolume<T>& 
#ifdef USE_MINC2 
v
#endif
, int 
#ifdef USE_MINC2
frontBorder
#endif
, int 
#ifdef USE_MINC2 
backBorder
#endif
, int 
#ifdef USE_MINC2
topBorder
#endif
, int 
#ifdef USE_MINC2
bottomBorder
#endif
, int 
#ifdef USE_MINC2
leftBorder
#endif
, int 
#ifdef USE_MINC2
rightBorder
#endif
, int 
#ifdef USE_MINC2
borderMode
#endif
    )
    {
        std::string fn(filename);
        fn= toLowercase(fn).substr(fn.length() - 3, 3);

        if ( fn == std::string("mnc") )
            #ifdef USE_MINC2
                return readMNC(filename, v, frontBorder, backBorder, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else
            return UNKNOWN_EXTENSION;
    }
    
    template<typename T>
    int readVOL(const char* filename, FramedVolume<T>& 
#ifdef USE_MINC2
v
#endif
, Border3 
#ifdef USE_MINC2
b
#endif
    )
    {
        std::string fn(filename);
        fn= toLowercase(fn).substr(fn.length() - 3, 3);

        if ( fn == std::string("mnc") )
            #ifdef USE_MINC2
                return readMNC(filename, v, b);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else
            return UNKNOWN_EXTENSION;
    }

    template<typename T>
    int writeVOL(const char* filename, const Volume<T>& 
#ifdef USE_MINC2
    v
#endif
    )
    {
        std::string fn(filename);
        fn= toLowercase(fn).substr(fn.length() - 3, 3);
	//printf("writeVOL: %s\n", fn.c_str());
        if ( fn == std::string("mnc") )
            #ifdef USE_MINC2
	    {
		//printf("calling writeMNC\n");
                return writeMNC(filename, v);
	    }
            #else
            {
	      //printf("not supported image format\n");
                return NOT_SUPPORTED_IMAGE_FORMAT;
	    }
            #endif
        else
	{
	  //printf("unknown extension\n");
            return UNKNOWN_EXTENSION;
	}
    }
    
    template<typename T>
    int writeVOL(const char* filename, const FramedVolume<T>& 
#ifdef USE_MINC2
v
#endif
    )
    {
        std::string fn(filename);
        fn= toLowercase(fn).substr(fn.length() - 3, 3);

        if ( fn == std::string("mnc") )
            #ifdef USE_MINC2
                return writeMNC(filename, v);
            #else
                return NOT_SUPPORTED_IMAGE_FORMAT;
            #endif
        else
            return UNKNOWN_EXTENSION;
    }
}

#endif

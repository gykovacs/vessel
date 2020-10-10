/**
 * @file bmpIO.h
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
 * bmp input/output template methods
 */

#ifndef _BMPIO_H_
#define _BMPIO_H_

#ifdef USE_BMP

#include <cstdlib>

#include <openipIO/bmp/EasyBMP.h>
#include <openipIO/ImageInfo.h>

#include <openipDS/Image.h>

/**
 * openip namespace
 */
namespace openip
{
    /**
     * reads the image info of a bmp file
     * @param filename filename
     * @param ii image info object
     * @return ImageIOError error code
     */
    int readBMPInfo(const char* filename, ImageInfo& ii);

    /**
     * reads the three channels of a bmp file into the r, g and b images
     * @param filename bmp image to read
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
    int readBMP(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * reads one channel of a bmp file into the img image
     * @param filename bmp image to read
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
    int readBMP(const char* filename, Image<T>& img, int channel, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * writes columns*rows elements from the r g and b images into the red, green and blue channels of the file
     * @param filename filename to write the bmp into
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @return ImageIOError error code
     */
    template<typename T>
    int writeBMP(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b);

    /**
     * writes columns*rows elements from the img image into the file
     * @param filename filename to write the bmp into
     * @param img intensity image to write into file
     * @return ImageIOError error code
     */
    template<typename T>
    int writeBMP(const char* filename, Image<T>& img);

    template<typename T>
    int readBMP(const char* filename, Image<T>& img, int channel, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        BMP bmpIn;
        bmpIn.ReadFromFile(filename);

        unsigned int columns= bmpIn.TellWidth();
        unsigned int rows= bmpIn.TellHeight();

        RGBApixel x;

        int n= 0;
        int width= columns + leftBorder + rightBorder;
        n+= leftBorder + topBorder * width;

        img.resizeImage(bmpIn.TellHeight(), bmpIn.TellWidth(), topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

        switch (channel)
        {
            case READ_CHANNEL_RED:
                for ( unsigned int i= 0; i < rows; ++i )
                {
                    for ( unsigned int j= 0; j < columns; ++j )
                    {
                         x= bmpIn.GetPixel(j,i);
                         img(n)= x.Red;
                         ++n;
                    }
                    n+= rightBorder + leftBorder;
                }
                break;
            case READ_CHANNEL_GREEN:
                for ( unsigned int i= 0; i < rows; ++i )
                {
                    for ( unsigned int j= 0; j < columns; ++j )
                    {
                         x= bmpIn.GetPixel(j,i);
                         img(n)= x.Green;
                         ++n;
                    }
                    n+= rightBorder + leftBorder;
                }
                break;
            case READ_CHANNEL_BLUE:
                for ( unsigned int i= 0; i < rows; ++i )
                {
                    for ( unsigned int j= 0; j < columns; ++j )
                    {
                         x= bmpIn.GetPixel(j,i);
                         img(n)= x.Blue;
                         ++n;
                    }
                    n+= rightBorder + leftBorder;
                }
                break;
            case READ_CHANNEL_GRAYSCALE:
                for ( unsigned int i= 0; i < rows; ++i )
                {
                    for ( unsigned int j= 0; j < columns; ++j )
                    {
                         x= bmpIn.GetPixel(j,i);
                         img(n)= (x.Red + x.Green + x.Blue)/3;
                         ++n;
                    }
                    n+= rightBorder + leftBorder;
                }
                break;
        }

        return 0;
    }

    template<typename T>
    int readBMP(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        BMP bmpIn;
        bmpIn.ReadFromFile(filename);

        unsigned int columns= bmpIn.TellWidth();
        unsigned int rows= bmpIn.TellHeight();

        RGBApixel x;

        int n= 0;
        int width= columns + leftBorder + rightBorder;
        n+= leftBorder + topBorder * width;

        r.resizeImage(bmpIn.TellHeight(), bmpIn.TellWidth(), topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        g.resizeImage(bmpIn.TellHeight(), bmpIn.TellWidth(), topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        b.resizeImage(bmpIn.TellHeight(), bmpIn.TellWidth(), topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

        for ( unsigned int i= 0; i < rows; ++i )
        {
            for ( unsigned int j= 0; j < columns; ++j )
            {
                 x= bmpIn.GetPixel(j,i);
                 r(n)= x.Red;
                 g(n)= x.Green;
                 b(n)= x.Blue;
                 ++n;
            }
            n+= rightBorder + leftBorder;
        }

        return 0;
    }

    template<typename T>
    int writeBMP(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b)
    {
        BMP bmpOut;

        bmpOut.SetSize(r.columns - r.leftBorder - r.rightBorder, r.rows - r.topBorder - r.bottomBorder);

        RGBApixel x;

        for ( int i= r.leftBorder; i < r.columns - r.rightBorder; ++i )
            for ( int j= r.topBorder; j < r.rows - r.bottomBorder; ++j )
            {
                 x.Red= r(j*r.columns + i);
                 x.Green= g(j*r.columns + i);
                 x.Blue= b(j*r.columns + i);
                 x.Alpha= 0;
                 bmpOut.SetPixel(i - r.leftBorder,j - r.topBorder,x);
            }

        bmpOut.WriteToFile(filename);

        return 0;
    }

    template<typename T>
    int writeBMP(const char* filename, Image<T>& img)
    {
        BMP bmpOut;

        bmpOut.SetSize(img.columns - img.leftBorder - img.rightBorder, img.rows - img.topBorder - img.bottomBorder);

        RGBApixel x;

        for ( int i= img.leftBorder; i < (int)(img.columns) - img.rightBorder; ++i )
            for ( int j= img.topBorder; j < (int)(img.rows) - img.bottomBorder; ++j )
            {
                 x.Red= img(j*img.columns + i);
                 x.Green= img(j*img.columns + i);
                 x.Blue= img(j*img.columns + i);
                 x.Alpha= 0;
                 bmpOut.SetPixel(i - img.leftBorder,j - img.topBorder,x);
            }

        bmpOut.WriteToFile(filename);

        return 0;
    }
}

#endif

#endif
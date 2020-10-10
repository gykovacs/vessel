/**
 * @file tifIO.h
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
 * tif input/output template methods
 */

#ifndef _TIF_IO_H_
#define _TIF_IO_H_

#ifdef USE_TIFF

#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <openipDS/Image.h>

#include <openipIO/configIO.h>
#include <openipIO/ImageInfo.h>

#include <tiffio.h>

namespace openip
{
    /**
     * reads the image info of a jpg file
     * @param filename filename
     * @param ii image info object
     * @return ImageIOError error code
     */
    int readTIFInfo(const char* filename, ImageInfo& ii);

    /**
     * reads the three channels of a tif file into the r, g and b images
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
    int readTIF(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * reads one channel of a tif file into the img image
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
    int readTIF(const char* filename, Image<T>& img, int channel, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * writes columns*rows elements from the r g and b images into the red, green and blue channels of the file
     * @param filename filename to write the bmp into
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @return ImageIOError error code
     */
    template<typename T>
    int writeTIF(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b);

    /**
     * writes columns*rows elements from the img image into the file
     * @param filename filename to write the bmp into
     * @param img intensity image to write into file
     * @return ImageIOError error code
     */
    template<typename T>
    int writeTIF(const char* filename, Image<T>& img);

    template<typename T>
    int readTIF(const char* filename, Image<T>& im, int channel, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        TIFF* tif= TIFFOpen(filename, "r");

        /*TIFFRGBAImage img;
        char emsg[1024];

        if ( TIFFRGBAImageBegin(&img, tif, 0, emsg))
        {
            size_t npixels;
            uint32* raster;

            npixels= img.width * img.height;
            raster= (uint32*) _TIFFmalloc(npixels * sizeof(uint32));

            int columns= leftBorder + img.width + rightBorder;

            int tmp= 0;

            im.resizeImage(img.height, img.width, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

            if ( raster != NULL )
            {
                if (TIFFRGBAImageGet(&img, raster, img.width, img.height))
                {
                    tmp= (topBorder + img.height) * columns - img.width - rightBorder;
                    for ( unsigned int i= 0; i < npixels; ++i )
                    {
                        switch(channel)
                        {
                            case READ_CHANNEL_RED:
                                im(tmp)= (unsigned char)(*&(raster[i]));
                                break;
                            case READ_CHANNEL_GREEN:
                                im(tmp)= (unsigned char)(*(&(raster[i])+1));
                                break;
                            case READ_CHANNEL_BLUE:
                                im(tmp)= (unsigned char)(*(&(raster[i])+2));
                                break;
                            case READ_CHANNEL_GRAYSCALE:
                                im(tmp)= ((unsigned char)(*&(raster[i])) + (unsigned char)(*(&(raster[i])+1)) + (unsigned char)(*(&(raster[i])+2)) )/3;
                                break;
                        }
                        if ( (i % img.width) == 0 && i > 0 )
                            tmp-= 2 * img.width + leftBorder + rightBorder;

                        ++tmp;
                    }
                }
            }

            TIFFRGBAImageEnd(&img);
        }*/

        uint32 width, height, ss;
        uint16 maxv, minv, bps, spp;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
        TIFFGetField(tif, TIFFTAG_MAXSAMPLEVALUE, &maxv);
        TIFFGetField(tif, TIFFTAG_MINSAMPLEVALUE, &minv);

        //printf("%d %d %d %d %d %d\n", width, height, bps, spp, maxv, minv);

        float minvf= minv, maxvf= maxv;

        ss= bps/8*spp*width;

        int tmp= 0;

        int columns= leftBorder + width + rightBorder;

        im.resizeImage(height, width, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

        unsigned char* buffer= (unsigned char*)malloc(sizeof(char)*ss);

        //tmp= (topBorder + height) * columns - width - rightBorder;
        tmp= topBorder * (columns) + rightBorder;

        float v= 0;
        if ( bps == 16 )
        {
            minvf= 100000000;
            maxvf= 0;
            for ( unsigned int i= 0; i < height; ++i )
            {
                TIFFReadScanline(tif, buffer, i);
                for ( unsigned int j= 0; j < width; ++j )
                {
                    if ( bps == 8 )
                        v= buffer[j];
                    else if ( bps == 16 )
                        v= (float)(*(unsigned short*)(&(buffer[2*j])));
                    if ( v < minvf )
                        minvf= v;
                    if ( v > maxvf )
                        maxvf= v;
                }
            }
        }
        float d= fabs(maxvf - minvf);

        for ( unsigned int i= 0; i < height; ++i )
        {
            TIFFReadScanline(tif, buffer, i);

            for ( unsigned int j= 0; j < width; ++j )
            {
                if ( spp == 1 )
                {
                    if ( bps == 8 )
                    {
                        im(tmp)= buffer[j];
                    }
                    else if ( bps == 16 )
                    {
                        im(tmp)= T((((float)(*(unsigned short*)(&(buffer[2*j])))) - minvf)/d*255);
                        /*switch(channel)
                        {
                            case READ_CHANNEL_GRAYSCALE:
                                im(tmp)= T((((float)(*(unsigned short*)(&(buffer[2*j])))) - minvf)/d*255);
                                break;
                            case READ_CHANNEL_RED:
                                im(tmp)= *(unsigned char*)(&(buffer[2*j]));
                                break;
                            case READ_CHANNEL_GREEN:
                                im(tmp)= *((unsigned char*)(&(buffer[2*j]))+1);
                                break;
                        }*/
                    }
                }
                else if ( spp == 2 )
                {
                    if ( bps == 8 )
                    {
                        switch(channel)
                        {
                            case READ_CHANNEL_RED:
                                im(tmp)= (unsigned char)(buffer[j]);
                                break;
                            case READ_CHANNEL_GREEN:
                                im(tmp)= (unsigned char)(buffer[j+1]);
                                break;
                            case READ_CHANNEL_GRAYSCALE:
                                im(tmp)= (unsigned char)(((short)(buffer[j]) + (short)(buffer[j+1]))/2);
                                break;
                        }
                        j+= 1;
                    }
                }
                else if ( spp == 3 )
                {
                    if ( bps == 8 )
                    {
                        //printf(".");
                        switch(channel)
                        {
                            case READ_CHANNEL_RED:
                                im(tmp)= (unsigned char)(buffer[j]);
                                break;
                            case READ_CHANNEL_GREEN:
                                im(tmp)= (unsigned char)(buffer[j+1]);
                                break;
                            case READ_CHANNEL_BLUE:
                                im(tmp)= (unsigned char)(buffer[j+2]);
                                break;
                            case READ_CHANNEL_GRAYSCALE:
                                im(tmp)= (unsigned char)(((short)(buffer[j]) + (short)(buffer[j+1]) + (short)(buffer[j+2]))/3);
                                break;
                        }
                        j+= 2;
                    }
                    else if ( bps == 16 )
                    {
                        switch(channel)
                        {
                            case READ_CHANNEL_RED:
                                im(tmp)= (((float)(*(unsigned short*)(&(buffer[2*j])))) - minvf)/d*255;
                                break;
                            case READ_CHANNEL_GREEN:
                                im(tmp)= (((float)(*(unsigned short*)(&(buffer[2*j + 2])))) - minvf)/d*255;
                                break;
                            case READ_CHANNEL_BLUE:
                                im(tmp)= (((float)(*(unsigned short*)(&(buffer[2*j + 4])))) - minvf)/d*255;
                                break;
                            case READ_CHANNEL_GRAYSCALE:
                                im(tmp)= (unsigned char)(((((float)(*(unsigned short*)(&(buffer[2*j])))) - minvf)/d*255 + (((float)(*(unsigned short*)(&(buffer[2*j + 2])))) - minvf)/d*255 + (((float)(*(unsigned short*)(&(buffer[2*j + 4])))) - minvf)/d*255)/3);
                                break;
                        }
                        j+= 2;
                    }
                }
                else if ( spp == 4 )
                {
                    if ( bps == 8 )
                    {
                        switch(channel)
                        {
                            case READ_CHANNEL_RED:
                                im(tmp)= (unsigned char)(buffer[j]);
                                break;
                            case READ_CHANNEL_GREEN:
                                im(tmp)= (unsigned char)(buffer[j+1]);
                                break;
                            case READ_CHANNEL_BLUE:
                                im(tmp)= (unsigned char)(buffer[j+2]);
                                break;
                            case READ_CHANNEL_GRAYSCALE:
                                im(tmp)= (unsigned char)(((short)(buffer[j]) + (short)(buffer[j+1]) + (short)(buffer[j+2]))/3);
                                break;
                        }
                        j+= 3;
                    }
                    else if ( bps == 16 )
                    {
                        switch(channel)
                        {
                            case READ_CHANNEL_RED:
                                im(tmp)= (((float)(*(short*)(&(buffer[2*j])))) - minvf)/d*255;
                                break;
                            case READ_CHANNEL_GREEN:
                                im(tmp)= (((float)(*(short*)(&(buffer[2*j + 2])))) - minvf)/d*255;
                                break;
                            case READ_CHANNEL_BLUE:
                                im(tmp)= (((float)(*(short*)(&(buffer[2*j + 4])))) - minvf)/d*255;
                                break;
                            case READ_CHANNEL_GRAYSCALE:
                                im(tmp)= (unsigned char)(((((float)(*(unsigned short*)(&(buffer[2*j])))) - minvf)/d*255 + (((float)(*(unsigned short*)(&(buffer[2*j + 2])))) - minvf)/d*255 + (((float)(*(unsigned short*)(&(buffer[2*j + 4])))) - minvf)/d*255)/3);
                                break;
                        }
                        j+= 3;
                    }
                }
                ++tmp;
            }
            tmp+= leftBorder + rightBorder;
        }

        free(buffer);
        TIFFClose(tif);
        return 0;
    }

    template<typename T>
    int readTIF(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        TIFF* tif= TIFFOpen(filename, "r");

        /*TIFFRGBAImage img;
        char emsg[1024];

        if ( TIFFRGBAImageBegin(&img, tif, 0, emsg))
        {
            size_t npixels;
            uint32* raster;

            npixels= img.width * img.height;
            raster= (uint32*) _TIFFmalloc(npixels * sizeof(uint32));

            int columns= leftBorder + img.width + rightBorder;

            int tmp= 0;

            r.resizeImage(img.height, img.width, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            g.resizeImage(img.height, img.width, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
            b.resizeImage(img.height, img.width, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

            if ( raster != NULL )
            {
                if (TIFFRGBAImageGet(&img, raster, img.width, img.height))
                {
                    tmp= (topBorder + img.height) * columns - img.width - rightBorder;
                    for ( unsigned int i= 0; i < npixels; ++i )
                    {
                        r(tmp)= (unsigned char)(*&(raster[i]));
                        g(tmp)= (unsigned char)(*((unsigned char*)&(raster[i])+1));
                        b(tmp)= (unsigned char)(*((unsigned char*)&(raster[i])+2));
                        if ( (i % img.width) == 0 && i > 0 )
                            tmp-= 2 * img.width + leftBorder + rightBorder;

                        ++tmp;
                    }
                }
            }

            TIFFRGBAImageEnd(&img);
        }*/

        uint32 width, height, ss;
        uint16 maxv, minv, bps, spp;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
        TIFFGetField(tif, TIFFTAG_MAXSAMPLEVALUE, &maxv);
        TIFFGetField(tif, TIFFTAG_MINSAMPLEVALUE, &minv);

        float minvf= minv, maxvf= maxv;

        ss= bps/8*spp*width;

        int tmp= 0;

        int columns= leftBorder + width + rightBorder;

        r.resizeImage(height, width, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        g.resizeImage(height, width, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        b.resizeImage(height, width, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

        unsigned char* buffer= (unsigned char*)malloc(sizeof(char)*ss);

        tmp= topBorder * (columns) + rightBorder;

        float v= 0;
        if ( bps == 16 )
        {
            minvf= 100000000;
            maxvf= 0;
            for ( unsigned int i= 0; i < height; ++i )
            {
                TIFFReadScanline(tif, buffer, i);
                for ( unsigned int j= 0; j < width; ++j )
                {
                    if ( bps == 8 )
                        v= buffer[j];
                    else if ( bps == 16 )
                        v= (float)(*(unsigned short*)(&(buffer[2*j])));
                    if ( v < minvf )
                        minvf= v;
                    if ( v > maxvf )
                        maxvf= v;
                }
            }
        }
        float d= fabs(maxvf - minvf);

        for ( unsigned int i= 0; i < height; ++i )
        {
            TIFFReadScanline(tif, buffer, i);

            for ( unsigned int j= 0; j < width; ++j )
            {
                if ( spp == 1 )
                {
                    if ( bps == 8 )
                    {
                        r(tmp)= g(tmp)= b(tmp)= buffer[j];
                    }
                    else if ( bps == 16 )
                    {
                        r(tmp)= g(tmp)= b(tmp)= T((((float)(*(unsigned short*)(&(buffer[2*j])))) - minvf)/d*255);
                    }
                }
                else if ( spp == 3 )
                {
                    if ( bps == 8 )
                    {
                        r(tmp)= (unsigned char)(buffer[j]);
                        g(tmp)= (unsigned char)(buffer[j+1]);
                        b(tmp)= (unsigned char)(buffer[j+2]);
                        j+= 2;
                    }
                    else if ( bps == 16 )
                    {
                        r(tmp)= (((float)(*(unsigned short*)(&(buffer[2*j])))) - minvf)/d*255;
                        g(tmp)= (((float)(*(unsigned short*)(&(buffer[2*j + 2])))) - minvf)/d*255;
                        b(tmp)= (((float)(*(unsigned short*)(&(buffer[2*j + 4])))) - minvf)/d*255;
                        j+= 4;
                    }
                }
                else if ( spp == 4 )
                {
                    if ( bps == 8 )
                    {
                        r(tmp)= (unsigned char)(buffer[j]);
                        g(tmp)= (unsigned char)(buffer[j+1]);
                        b(tmp)= (unsigned char)(buffer[j+2]);
                        j+= 3;
                    }
                    else if ( bps == 16 )
                    {
                        r(tmp)= (((float)(*(short*)(&(buffer[2*j])))) - minvf)/d*255;
                        g(tmp)= (((float)(*(short*)(&(buffer[2*j + 2])))) - minvf)/d*255;
                        b(tmp)= (((float)(*(short*)(&(buffer[2*j + 4])))) - minvf)/d*255;
                        j+= 6;
                    }
                }
                ++tmp;
            }
            tmp+= leftBorder + rightBorder;
        }

        free(buffer);
        TIFFClose(tif);
        return 0;
    }

    template<typename T>
    int writeTIF(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b)
    {
        TIFF* out= TIFFOpen(filename, "w");

        int sampleperpixel= 3;

        std::vector<unsigned char> buffer;

        int width= (r.columns - r.leftBorder - r.rightBorder);
        int height= (r.rows - r.topBorder - r.bottomBorder);
        int n= width*height;

        buffer.resize( n * sampleperpixel);

        int tmp= r.leftBorder + r.topBorder * r.columns;

        for ( int i= 0; i < n; ++i )
        {
            buffer[i*sampleperpixel]= r(tmp);
            buffer[i*sampleperpixel + 1]= g(tmp);
            buffer[i*sampleperpixel + 2]= b(tmp);

            if ( i != 0 && i % width == 0 )
                tmp+= r.leftBorder + r.rightBorder;
            ++tmp;
        }

        TIFFSetField(out, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(out, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, sampleperpixel);
        TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
        TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

        tsize_t linebytes= sampleperpixel * width;

        unsigned char* buf= NULL;

        if ( TIFFScanlineSize(out) == linebytes )
            buf= (unsigned char*)_TIFFmalloc(linebytes);
        else
            buf= (unsigned char*)_TIFFmalloc(TIFFScanlineSize(out));

        TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out, width*sampleperpixel));

        for ( int row= 0; row < height; row++ )
        {
            memcpy(buf, &buffer[(row)*linebytes], linebytes);

            if ( TIFFWriteScanline(out, buf, row, 0) < 0 )
                break;
        }

        (void)TIFFClose(out);

        if ( buf )
            _TIFFfree(buf);

        return 0;
    }

    template<typename T>
    int writeTIF(const char* filename, Image<T>& img)
    {
        TIFF* out= TIFFOpen(filename, "w");

        int sampleperpixel= 1;

        std::vector<unsigned char> buffer;

        int width= (img.columns - img.leftBorder - img.rightBorder);
        int height= (img.rows - img.topBorder - img.bottomBorder);
        int n= width*height;

        buffer.resize( n * sampleperpixel);

        int tmp= img.leftBorder + img.topBorder * img.columns;

        for ( int i= 0; i < n; ++i )
        {
            buffer[i]= img(tmp);

            if ( i != 0 && i % width == 0 )
                tmp+= img.leftBorder + img.rightBorder;
            ++tmp;
        }

        TIFFSetField(out, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(out, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, sampleperpixel);
        TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
        TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(out, TIFFTAG_PHOTOMETRIC, 1);

        tsize_t linebytes= sampleperpixel * width;

        unsigned char* buf= NULL;

        if ( TIFFScanlineSize(out) == linebytes )
            buf= (unsigned char*)_TIFFmalloc(linebytes);
        else
            buf= (unsigned char*)_TIFFmalloc(TIFFScanlineSize(out));

        TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out, width*sampleperpixel));

        for ( int row= 0; row < height; row++ )
        {
            memcpy(buf, &buffer[(row)*linebytes], linebytes);

            if ( TIFFWriteScanline(out, buf, row, 0) < 0 )
                break;
        }

        (void)TIFFClose(out);

        if ( buf )
            _TIFFfree(buf);

        return 0;
    }
}

#endif

#endif

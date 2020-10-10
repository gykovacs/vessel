/**
 * @file jpgIO.h
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
 * jpg input/output template methods
 */

#ifndef _JPEG_IO_H_
#define _JPEG_IO_H_

#include <cstdio>
#include <cstdlib>
#include <vector>

#ifdef USE_JPEG

#include <jpeglib.h>

#include <openipIO/configIO.h>

#include <openipDS/Image.h>

namespace openip
{
    /**
     * reads the image info of a jpg file
     * @param filename filename
     * @param ii image info object
     * @return ImageIOError error code
     */
    int readJPGInfo(const char* filename, ImageInfo& ii);

    /**
     * reads the three channels of a jpg file into the r, g and b images
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
    int readJPG(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * reads one channel of a jpg file into the img image
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
    int readJPG(const char* filename, Image<T>& img, int channel, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * writes columns*rows elements from the r g and b images into the red, green and blue channels of the file
     * @param filename filename to write the bmp into
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @return ImageIOError error code
     */
    template<typename T>
    int writeJPG(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b);

    /**
     * writes columns*rows elements from the img image into the file
     * @param filename filename to write the bmp into
     * @param img intensity image to write into file
     * @return ImageIOError error code
     */
    template<typename T>
    int writeJPG(const char* filename, Image<T>& img);

    template<typename T>
    int readJPG(const char* filename, Image<T>& img, int channel, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        unsigned int columns, rows;

        std::FILE* f= std::fopen(filename, "rb");

        if ( f == NULL )
            return NO_SUCH_FILE;

        struct jpeg_decompress_struct cinfo;

        //JDIMENSION num_scanlines;

        JSAMPARRAY in;

        struct jpeg_error_mgr jerr;

        //int numPixels;

        int row_stride;

        unsigned char *out;

        int count;

        int i;

        cinfo.err= jpeg_std_error(&jerr);

        jpeg_create_decompress(&cinfo);

        jpeg_stdio_src(&cinfo, f);

        (void)jpeg_read_header(&cinfo, TRUE);
        (void)jpeg_start_decompress(&cinfo);

        columns= cinfo.image_width;
        rows= cinfo.image_height;

        //numPixels= columns*rows;

        row_stride= cinfo.output_width * cinfo.output_components;

        in= (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

        count= 0;

        int width= columns + leftBorder + rightBorder;

        count= width * topBorder + leftBorder;

        img.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

        while(cinfo.output_scanline < cinfo.output_height)
        {
            //num_scanlines= jpeg_read_scanlines(&cinfo, in, 1);
            out= in[0];

            if ( cinfo.output_components == 1 )
            {
                for ( i= 0; i < row_stride; ++i )
                {
                    img(count)= out[i];
                    ++count;
                }
            }
            else
            {
                for ( i= 0; i < row_stride; )
                {
                    switch(channel)
                    {
                        case READ_CHANNEL_RED:
                            img(count)= out[i];
                            break;
                        case READ_CHANNEL_GREEN:
                            img(count)= out[i+1];
                            break;
                        case READ_CHANNEL_BLUE:
                            img(count)= out[i+2];
                            break;
                        case READ_CHANNEL_GRAYSCALE:
                            img(count)= (out[i] + out[i+1] + out[i+2])/3;
                            break;
                    }
                    i+= 3;
                    count++;
                }
                count+= leftBorder + rightBorder;
            }
        }

        (void)jpeg_finish_decompress(&cinfo);

        jpeg_destroy_decompress(&cinfo);

        return SUCCESSFUL;
    }

    template<typename T>
    int readJPG(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        unsigned int columns, rows;

        std::FILE* f= std::fopen(filename, "rb");

        if ( f == NULL )
            return NO_SUCH_FILE;

        struct jpeg_decompress_struct cinfo;

        JSAMPARRAY in;

        struct jpeg_error_mgr jerr;

        int row_stride;

        unsigned char *out;

        int count;

        int i;

        cinfo.err= jpeg_std_error(&jerr);

        jpeg_create_decompress(&cinfo);

        jpeg_stdio_src(&cinfo, f);

        (void)jpeg_read_header(&cinfo, TRUE);
        (void)jpeg_start_decompress(&cinfo);

        columns= cinfo.image_width;
        rows= cinfo.image_height;

        row_stride= cinfo.output_width * cinfo.output_components;

        in= (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

        count= 0;

        int width= columns + leftBorder + rightBorder;

        count= width * topBorder + leftBorder;

        r.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        g.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        b.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

        while(cinfo.output_scanline < cinfo.output_height)
        {
            jpeg_read_scanlines(&cinfo, in, 1);
            out= in[0];

            if ( cinfo.output_components == 1 )
            {
                for ( i= 0; i < row_stride; ++i )
                {
                    r(count)= g(count)= b(count)= out[i];
                    ++count;
                }
            }
            else
            {
                for ( i= 0; i < row_stride; )
                {
                    r(count)= out[i++];
                    g(count)= out[i++];
                    b(count++)= out[i++];
                }
                count+= leftBorder + rightBorder;
            }
        }

        (void)jpeg_finish_decompress(&cinfo);

        jpeg_destroy_decompress(&cinfo);

        return SUCCESSFUL;
    }

    template<typename T>
    int writeJPG(const char* filename, Image<T>& img)
    {
        std::vector<unsigned char> buffer;

        int width= img.columns - img.leftBorder - img.rightBorder;
        int height= img.rows - img.topBorder - img.bottomBorder;

        int n= width*height;
        buffer.resize(n);

        int tmp= img.topBorder * img.columns + img.leftBorder;

        for ( int i= 0; i < n; ++i )
        {
            buffer[i]= img(tmp);

            if ( i != 0 && i % width == (width - 1) )
                tmp+= img.leftBorder + img.rightBorder;
            ++tmp;
        }

        FILE* outfile= fopen(filename, "wb");

        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr   jerr;

        cinfo.err= jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);
        jpeg_stdio_dest(&cinfo, outfile);

        cinfo.image_width= width;
        cinfo.image_height= height;
        cinfo.input_components= 1;
        cinfo.in_color_space= JCS_GRAYSCALE;

        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, 100, true);
        jpeg_start_compress(&cinfo, true);

        JSAMPROW row_pointer;

        while( cinfo.next_scanline < cinfo.image_height)
        {
            row_pointer= (JSAMPROW) &buffer[cinfo.next_scanline*width];
            jpeg_write_scanlines(&cinfo, &row_pointer, 1);
        }

        jpeg_finish_compress(&cinfo);
        fclose(outfile);
        return 0;
    }

    template<typename T>
    int writeJPG(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b)
    {
        std::vector<unsigned char> buffer;

        int width= r.columns - r.leftBorder - r.rightBorder;
        int height= r.rows - r.topBorder - r.bottomBorder;

        int n= width*height;
        buffer.resize(n * 3);

        int tmp= r.topBorder * r.columns + r.leftBorder;

        for ( int i= 0; i < n; ++i )
        {
            buffer[i*3]= r(tmp);
            buffer[i*3 + 1]= g(tmp);
            buffer[i*3 + 2]= b(tmp);
            
            if ( i != 0 && i % width == (width - 1) )
                tmp+= r.leftBorder + r.rightBorder;
            ++tmp;
        }

        FILE* outfile= fopen(filename, "wb");

        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr   jerr;

        cinfo.err= jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);
        jpeg_stdio_dest(&cinfo, outfile);

        cinfo.image_width= width;
        cinfo.image_height= height;
        cinfo.input_components= 3;
        cinfo.in_color_space= JCS_RGB;

        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, 100, true);
        jpeg_start_compress(&cinfo, true);

        JSAMPROW row_pointer;

        while( cinfo.next_scanline < cinfo.image_height)
        {
            row_pointer= (JSAMPROW) &buffer[cinfo.next_scanline*3*width];
            jpeg_write_scanlines(&cinfo, &row_pointer, 1);
        }

        jpeg_finish_compress(&cinfo);
        fclose(outfile);
        return 0;
    }
}

#endif	/* _JPGIO_H */

#endif
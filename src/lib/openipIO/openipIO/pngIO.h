/**
 * @file pngIO.h
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
 * png input/output template methods
 */

#ifndef _PNG_IO_H_
#define _PNG_IO_H_

using namespace std;

#ifdef USE_LIBPNG

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <png.h>

#include <openipIO/configIO.h>
#include <openipDS/Image.h>
#include <openipIO/ImageInfo.h>
#include <openipDS/Stopper.h>

namespace openip
{
    /**
     * reads the image info of a png file
     * @param filename filename
     * @param ii image info object
     * @return ImageIOError error code
     */
    int readPNGInfo(const char* filename, ImageInfo& pnginfo);

    /**
     * reads the three channels of a png file into the r, g and b images
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
    int readPNG(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * reads one channel of a png file into the img image
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
    int readPNG(const char* filename, Image<T>& img, int channel, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * writes columns*rows elements from the r g and b images into the red, green and blue channels of the file
     * @param filename filename to write the bmp into
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @return ImageIOError error code
     */
    template<typename T>
    int writePNG(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int compression= 9);

    /**
     * writes columns*rows elements from the img image into the file
     * @param filename filename to write the bmp into
     * @param img intensity image to write into file
     * @return ImageIOError error code
     */
    template<typename T>
    int writePNG(const char* filename, Image<T>& img, int compression= 9);

    template<typename T>
    int readPNG(const char* filename, Image<T>& img, int channel, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        FILE* fp= fopen(filename, "rb");

        png_byte header[8];

        int res= fread(header, 1, 8, fp);
        if ( res != 8 )
            printf("cannot read png header");

        int is_png= !png_sig_cmp(header, 0, 8);

        if (!is_png){ printf("read not png\n"); return 0; }

        png_structp png_ptr= png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

        if (!png_ptr){ printf("png ptr error\n"); return 0; }

        png_infop info_ptr= png_create_info_struct(png_ptr);

        if ( setjmp(png_jmpbuf(png_ptr)))
        {
                printf("read_png_file error during init_io");
                fflush(stdout);
        }

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        png_uint_32 width, height; 
        int bit_depth, color_type, interlace_type, compression_type, filter_method;

        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_method);

        /*
        int columns= info_ptr->width;
        int rows= info_ptr->height;
        int color_type= info_ptr->color_type;
        int bit_depth= info_ptr->bit_depth;*/

        int columns= width;
        int rows= height;

        //int number_of_passes= png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);

        if ( setjmp(png_jmpbuf(png_ptr)) )
        {
            printf("read_png_file error during read_image");
            fflush(stdout);
        }

        png_bytep* row_pointers;

        int rowbytes= png_get_rowbytes(png_ptr, info_ptr);

        row_pointers= (png_bytep*)std::malloc(sizeof(png_bytep) *rows);
        for ( int y= 0; y <rows; ++y )
            row_pointers[y]= (png_byte*)std::malloc(rowbytes);

        png_read_image(png_ptr, row_pointers);

        fclose(fp);

        img.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

        int rr= topBorder*(columns + leftBorder + rightBorder) + leftBorder;

        //printf("color_type: %d, bit_depth: %d\n", color_type, bit_depth);



        // truecolor rgb 24
        if ( color_type == 2 && bit_depth == 8 )
        {
            for ( int i= 0; i < rows; ++i )
            {
                for ( int j= 0; j < rowbytes; j+= 3 )
                {
                    switch(channel)
                    {
                        case READ_CHANNEL_RED:
                            img(rr)= row_pointers[i][j];
                            break;
                        case READ_CHANNEL_GREEN:
                            img(rr)= row_pointers[i][j+1];
                            break;
                        case READ_CHANNEL_BLUE:
                            img(rr)= row_pointers[i][j+2];
                            break;
                        case READ_CHANNEL_GRAYSCALE:
                            img(rr)= (row_pointers[i][j] + row_pointers[i][j+1] + row_pointers[i][j+2])/3;
                            break;
                    }
                    ++rr;
                }
                rr+= rightBorder + leftBorder;
            }
        }

        //truecolor rgb 24, 16 bit samples
        if ( color_type == 2 && bit_depth == 16 )
        {
            cout << "read png 16" << endl;
            for ( int i= 0; i < rows; ++i )
            {
                for ( int j= 0; j < rowbytes; j+= 6 )
                {
                    switch(channel)
                    {
                        case READ_CHANNEL_RED:
                            img(rr)= (short)*(&(row_pointers[i][j*2]));
                            break;
                        case READ_CHANNEL_GREEN:
                            img(rr)= (short)*(&(row_pointers[i][j*2+2]));
                            break;
                        case READ_CHANNEL_BLUE:
                            img(rr)= (short)*(&(row_pointers[i][j*2+4]));
                            break;
                        case READ_CHANNEL_GRAYSCALE:
                            img(rr)= ((short)*(&(row_pointers[i][j*2])) + (short)*(&(row_pointers[i][j*2+2])) + (short)*(&(row_pointers[i][j*2+4])))/3;
                            break;

                    }
                    ++rr;
                }
            }
            cout << "png 16 read" << endl;
        }

        // truecolor rgba 32
        if ( color_type == 6 && bit_depth == 8 )
        {
            for ( int i= 0; i < rows; ++i )
            {
                for ( int j= 0; j < rowbytes; j+= 4 )
                {
                    switch(channel)
                    {
                        case READ_CHANNEL_RED:
                            img(rr)= row_pointers[i][j];
                            break;
                        case READ_CHANNEL_GREEN:
                            img(rr)= row_pointers[i][j+1];
                            break;
                        case READ_CHANNEL_BLUE:
                            img(rr)= row_pointers[i][j+2];
                            break;
                        case READ_CHANNEL_GRAYSCALE:
                            img(rr)= (row_pointers[i][j] + row_pointers[i][j+1] + row_pointers[i][j+2])/3;
                            break;
                    }
                    ++rr;
                }
                rr+= rightBorder + leftBorder;
            }
        }

        // grayscale 8
        if ( color_type == 0 && bit_depth == 8 )
        {
            for ( int i= 0; i < rows; ++i )
            {
                for ( int j= 0; j < rowbytes; ++j )
                {
                    img(rr)= row_pointers[i][j];
                    ++rr;
                }
                rr+= rightBorder + leftBorder;
            }
        }

        if ( color_type == 0 && bit_depth == 1 )
        {
            for ( int i= 0; i < rows; ++i )
            {
                int col= 0;
                for ( int j= 0; j < rowbytes; ++j )
                {
                    char tmp= row_pointers[i][j];
                    img(rr)= (tmp & 128) ? 255 : 0;
                    ++rr;
                    ++col;
                    if ( col == columns ) break;
                    img(rr)= (tmp & 64) ? 255 : 0;
                    ++rr;
                    ++col;
                    if ( col == columns ) break;
                    img(rr)= (tmp & 32) ? 255 : 0;
                    ++rr;
                    ++col;
                    if ( col == columns ) break;
                    img(rr)= (tmp & 16) ? 255 : 0;
                    ++rr;
                    ++col;
                    if ( col == columns ) break;
                    img(rr)= (tmp & 8) ? 255 : 0;
                    ++rr;
                    ++col;
                    if ( col == columns ) break;
                    img(rr)= (tmp & 4) ? 255 : 0;
                    ++rr;
                    ++col;
                    if ( col == columns ) break;
                    img(rr)= (tmp & 2) ? 255 : 0;
                    ++rr;
                    ++col;
                    if ( col == columns ) break;
                    img(rr)= (tmp & 1) ? 255 : 0;
                    ++rr;
                    ++col;
                    if ( col == columns ) break;
                }
                rr+= rightBorder + leftBorder;
            }
        }

        // grayscale alpha 16
        if ( color_type == 4 && bit_depth == 8 )
        {
            for ( int i= 0; i < rows; i+= 2 )
            {
                for ( int j= 0; j < rowbytes; ++j )
                {
                    img(rr)= row_pointers[i][j];
                    ++rr;
                }
                rr+= rightBorder + leftBorder;
            }
        }

        // indexed color || 16 bit_depth
        if ( color_type == 3 )
        {
            printf("png color type is not yet supported %d %d\n", color_type, bit_depth);
        }

        img.fillBorder();

        for ( int y= 0; y <rows; ++y )
            free(row_pointers[y]);

        free(row_pointers);
        png_destroy_info_struct(png_ptr, &info_ptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

        return 0;
    }

    template<typename T>
    int readPNG(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        FILE* fp= fopen(filename, "rb");

        png_byte header[8];

        int res= fread(header, 1, 8, fp);
        if ( res != 8 )
            printf("cannot read png header");

        int is_png= !png_sig_cmp(header, 0, 8);

        if (!is_png){ printf("read not png\n"); return 0; }

        png_structp png_ptr= png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

        if (!png_ptr){ printf("png ptr error\n"); return 0; }

        png_infop info_ptr= png_create_info_struct(png_ptr);

        if ( setjmp(png_jmpbuf(png_ptr)))
        {
                printf("read_png_file error during init_io");
                fflush(stdout);
        }

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        png_uint_32 width, height; 
        int bit_depth, color_type, interlace_type, compression_type, filter_method;

        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_method);

        /*
        int columns= info_ptr->width;
        int rows= info_ptr->height;
        int color_type= info_ptr->color_type;
        int bit_depth= info_ptr->bit_depth;*/

        int columns= width;
        int rows= height;

        //int number_of_passes= png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);

        if ( setjmp(png_jmpbuf(png_ptr)) )
        {
            printf("read_png_file error during read_image");
            fflush(stdout);
        }

        png_bytep* row_pointers;

        int rowbytes= png_get_rowbytes(png_ptr, info_ptr);

        row_pointers= (png_bytep*)std::malloc(sizeof(png_bytep) *rows);
        for ( int y= 0; y <rows; ++y )
            row_pointers[y]= (png_byte*)std::malloc(rowbytes);

        png_read_image(png_ptr, row_pointers);

        fclose(fp);

        r.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        g.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
        b.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

        int rr= topBorder*(columns + leftBorder + rightBorder) + leftBorder;

        if ( color_type == 2 && bit_depth == 8 )
        {
            for ( int i= 0; i < rows; ++i )
            {
                for ( int j= 0; j < rowbytes; j+= 3 )
                {
                    r(rr)= row_pointers[i][j];
                    g(rr)= row_pointers[i][j+1];
                    b(rr)= row_pointers[i][j+2];
                    ++rr;
                }
                rr+= rightBorder + leftBorder;
            }
        }

        if ( color_type == 2 && bit_depth == 16 )
        {
            for ( int i= 0; i < rows; ++i )
            {
                for ( int j= 0; j < rowbytes; j+= 6 )
                {
                    r(rr)= (short)*(&(row_pointers[i][j]));
                    g(rr)= (short)*(&(row_pointers[i][j+2]));
                    b(rr)= (short)*(&(row_pointers[i][j+4]));
                    ++rr;
                }
                rr+= rightBorder + leftBorder;
            }
        }


        if ( color_type == 6 && bit_depth == 8 )
        {
            for ( int i= 0; i < rows; ++i )
            {
                for ( int j= 0; j < rowbytes; j+= 4 )
                {
                    r(rr)= row_pointers[i][j];
                    g(rr)= row_pointers[i][j+1];
                    b(rr)= row_pointers[i][j+2];
                    ++rr;
                }
                rr+= rightBorder + leftBorder;
            }
        }

        if ( color_type == 0 && bit_depth == 8 )
        {
            for ( int i= 0; i < rows; ++i )
            {
                for ( int j= 0; j < rowbytes; ++j )
                {
                    r(rr)= g(rr)= b(rr)= row_pointers[i][j];
                    ++rr;
                }
                rr+= rightBorder + leftBorder;
            }
        }

        if ( color_type == 4 && bit_depth == 8 )
        {
            for ( int i= 0; i < rows; i+= 2 )
            {
                for ( int j= 0; j < rowbytes; ++j )
                {
                    r(rr)= g(rr)= b(rr)= row_pointers[i][j];
                    ++rr;
                }
                rr+= rightBorder + leftBorder;
            }
        }

        if ( color_type == 3 )
        {
            printf("png color type is not yet supported %d %d\n", color_type, bit_depth);
        }

        r.fillBorder();
        g.fillBorder();
        b.fillBorder();

        for ( int y= 0; y <rows; ++y )
            free(row_pointers[y]);

        free(row_pointers);
        png_destroy_info_struct(png_ptr, &info_ptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

        return 0;
    }

    template<typename T>
    int writePNG(const char* filename, Image<T>& img, int compression)
    {
        FILE *fp = fopen(filename, "wb");

        if (!fp){ tprintf("io error\n"); return 0;}

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

        if (!png_ptr){ printf("png ptr error\n"); return 0; }

        png_infop info_ptr = png_create_info_struct(png_ptr);

        if (!info_ptr){ png_destroy_write_struct(&png_ptr, (png_infopp)NULL); return 0; }

        png_init_io(png_ptr, fp);

        png_set_compression_level(png_ptr, compression);

        int width= img.columns - img.leftBorder - img.rightBorder;
        int height= img.rows - img.topBorder - img.bottomBorder;

        png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        png_write_info(png_ptr, info_ptr);

        png_bytepp row_pointers;

        png_uint_32 rowbytes;

        row_pointers= (png_byte**)png_malloc(png_ptr, height*sizeof(png_bytep));

        for ( int i= 0; i < height; ++i )
            row_pointers[i]= (png_byte*)png_malloc(png_ptr, width*sizeof(png_byte));

        rowbytes= width;
        int cc= 0;
        int rr= img.topBorder*img.columns + img.leftBorder;

        for ( int i= 0; i < height; ++i )
        {
            cc= 0;
            for ( unsigned int j= 0; j < rowbytes; j++ )
            {
                row_pointers[i][j]= img(rr+cc);
                cc+= 1;
            }
            rr+= img.columns;
        }

        //png_write_image(png_ptr, row_pointers);
        png_write_rows(png_ptr, row_pointers, height);

        png_write_end(png_ptr, info_ptr);

        png_write_flush(png_ptr);

        png_destroy_write_struct(&png_ptr, &info_ptr);

        for ( int y= 0; y < height; ++y )
            free(row_pointers[y]);

        free(row_pointers);

        return 0;
    }

    template<typename T>
    int writePNG(const char* filename, Image<T>& r, Image<T>& g, Image<T>& b, int compression)
    {
        FILE *fp = fopen(filename, "wb");

        if (!fp){ tprintf("io error\n"); return 0;}

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

        if (!png_ptr){ printf("png ptr error\n"); return 0; }

        png_infop info_ptr = png_create_info_struct(png_ptr);

        if (!info_ptr){ png_destroy_write_struct(&png_ptr, (png_infopp)NULL); return 0; }

        png_init_io(png_ptr, fp);

        png_set_compression_level(png_ptr, compression);

        int width= r.columns - r.leftBorder - r.rightBorder;
        int height= r.rows - r.topBorder - r.bottomBorder;

        png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        png_write_info(png_ptr, info_ptr);

        png_bytepp row_pointers;

        png_uint_32 rowbytes;

        row_pointers= (png_byte**)png_malloc(png_ptr, height*sizeof(png_bytep));

        for ( int i= 0; i < height; ++i )
            row_pointers[i]= (png_byte*)png_malloc(png_ptr, width*3*sizeof(png_byte));

        rowbytes= 3*width;
        int cc= 0;
        int rr= r.topBorder*r.columns + r.leftBorder;

        for ( int i= 0; i < height; ++i )
        {
            cc= 0;
            for ( unsigned int j= 0; j < rowbytes; j+= 3 )
            {
                row_pointers[i][j]= r(rr+cc);
                row_pointers[i][j+1]= g(rr+cc);
                row_pointers[i][j+2]= b(rr+cc);
                cc+= 1;
            }
            rr+= r.columns;
        }

        png_write_rows(png_ptr, row_pointers, height);

        png_write_end(png_ptr, info_ptr);

        png_write_flush(png_ptr);

        png_destroy_write_struct(&png_ptr, &info_ptr);

        for ( int y= 0; y < height; ++y )
            free(row_pointers[y]);

        free(row_pointers);

        return 0;
    }
}

#endif

#endif

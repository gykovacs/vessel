/**
 * @file pgmIO.h
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
 * pgm input/output template methods
 */

#ifndef _PGMIO_H
#define	_PGMIO_H

#ifdef USE_PGM

#include <iostream>
#include <fstream>

#include <openipDS/Image.h>

#include <openipIO/configIO.h>
#include <openipIO/ImageInfo.h>

namespace openip
{
    /**
     * reads the image info of a pgm file
     * @param filename filename
     * @param ii image info object
     * @return ImageIOError error code
     */
    int readPGMInfo(const char* filename, ImageInfo& ii);

    /**
     * reads the data from a PGM file into the img image
     * @param filename pgm image to read
     * @param img image
     * @param topBorder number of top border rows
     * @param bottomBorder number of bottom border rows
     * @param leftBorder number of left border columns
     * @param rightBorder number of right border columns
     * @param borderMode border mode
     * @return ImageIOError error code
     */
    template<typename T>
    int readPGM(const char* filename, Image<T>& img, int topBorder= 0, int bottomBorder= 0, int leftBorder= 0, int rightBorder= 0, int borderMode= BORDER_MODE_MIRRORED);

    /**
     * writes columns*rows elements from img image into the file
     * @param filename filename to write the bmp into
     * @param img red channel
     * @return ImageIOError error code
     */
    template<typename T>
    int writePGM(const char* filename, Image<T>& img);

    template<typename T>
    int readPGM(const char* filename, Image<T>& img, int topBorder, int bottomBorder, int leftBorder, int rightBorder, int borderMode)
    {
        unsigned int columns, rows;

        std::ifstream file;

        file.open(filename, std::ios::in);
        if ( !file )
            return NO_SUCH_FILE;

        std::string line;
        char c;
        char m;

        file >> c;
        if ( c != 'P' )
            return NOT_SUPPORTED_IMAGE_FORMAT;
        file >> m;
        file >> c;

        if ( c == '#' )
            std::getline(file, line);
        else
            file.putback(c);

        file >> columns;
        file >> rows;

        int max;
        file >> max;

        int width= columns + leftBorder + rightBorder;

        img.resizeImage(rows, columns, topBorder, bottomBorder, leftBorder, rightBorder, borderMode);

        /** magic number P2*/
        if ( m == '2' )
        {
            short tmp;
            max= rows*columns;
            int n= topBorder * width + leftBorder;

            for ( unsigned int i= 0; i < rows; ++i )
            {
                for ( unsigned int j= 0; j < columns; ++j )
                {
                    file >> tmp;
                    img(n++)= tmp;
                }
                n+= leftBorder + rightBorder;
            }
        }
        /** magic number P5*/
        else if ( m == '5' )
        {
            /** stored on one byte*/
            if ( max < 256 )
            {
                max= rows*columns;
                int n= topBorder * width + leftBorder;

                unsigned char c2;

                for ( unsigned int i= 0; i < rows; ++i )
                {
                    for ( unsigned int j= 0; j < columns; ++j )
                    {
                        file >> c2;
                        img(n++)= c2;
                    }
                    n+= leftBorder + rightBorder;
                }
            }
            /** stored on two bytes*/
            else
            {
                short s;
                max= rows*columns;
                int n= topBorder * width + leftBorder;

                for ( unsigned int i= 0; i < rows; ++i )
                {
                    for ( unsigned int j= 0; j < columns; ++j )
                    {
                        file >> s;
                        img(n++)= s;
                    }
                    n+= leftBorder + rightBorder;
                }
            }
        }
        /** unknown magic number*/
        else
            return NOT_SUPPORTED_IMAGE_FORMAT;
        
        return SUCCESSFUL;
    }


    template<typename T>
    int writePGM(const char* filename, Image<T>& img)
    {
        std::ofstream file;

        file.open(filename, std::ios::out | std::ios::binary);

        if (!file)
            return CANNOT_OPEN_FILE;
        file << "P5" << std::endl;
        file << "# saved by openipIO" << std::endl;

        unsigned int width= img.columns - img.leftBorder - img.rightBorder;
        unsigned int height= img.rows - img.topBorder - img.bottomBorder;

        file << width << " " << height << std::endl;

        /** in PGM the greatest 65365 is the greates integer that can be stored
         * on two bytes*/
        int max= -1;
        int n= (img.topBorder + img.rows)*(img.leftBorder + img.rightBorder + img.columns);
        for ( int i= 0; i < n; ++i )
            if ( img(i) > max )
                max= img(i);

        file << 255 << std::endl;

        unsigned char s;

        n= img.topBorder * img.columns + img.leftBorder;

        for ( unsigned int i= 0; i < height; ++i )
        {
            for ( unsigned int j= 0; j < width; ++j )
            {
                s= img(n++);
                file.write((char*)&s, 1);
            }
            n+= img.leftBorder + img.rightBorder;
        }
        file.close();

        return 0;
    }

}

#endif

#endif	/* _PGMIO_H */


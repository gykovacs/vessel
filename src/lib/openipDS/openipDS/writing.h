/**
 * @file writing.h
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
 * The methods in this file write strings and character sequences into Images
 */

#ifndef _WRITING_H_
#define _WRITING_H_

#include <openipDS/Image.h>
#include <openipDS/DigitalAlphabet.h>

using namespace openip;

namespace openip
{
    /**
     * writes the string str into the image b using the digital alphabe da
     * in the position r, c with forecolor fcolor and background color bcolor
     * @param b image to write on
     * @param r row index to write there
     * @param c column index to write there
     * @param str character sequence to write on the image
     * @param fcolor foreground color (text color)
     * @param bcolor background color (-1 is transparent)
     * @param da the digital alphabet object
     */
    template<typename T>
    void iprintf(Image<T>& b, int r, int c, char* str, int fcolor, int bcolor, DigitalAlphabet& da)
    {
        int l= 0;
        vector<unsigned char>* letter= NULL;

        while(str[l])
        {
            switch (str[l++])
            {
                case 'a': letter= &(da.a); break;
                case 'b': letter= &(da.b); break;
                case 'c': letter= &(da.c); break;
                case 'd': letter= &(da.d); break;
                case 'e': letter= &(da.e); break;
                case 'f': letter= &(da.f); break;
                case 'g': letter= &(da.g); break;
                case 'h': letter= &(da.h); break;
                case 'i': letter= &(da.i); break;
                case 'j': letter= &(da.j); break;
                case 'k': letter= &(da.k); break;
                case 'l': letter= &(da.l); break;
                case 'm': letter= &(da.m); break;
                case 'n': letter= &(da.n); break;
                case 'o': letter= &(da.o); break;
                case 'p': letter= &(da.p); break;
                case 'q': letter= &(da.q); break;
                case 'r': letter= &(da.r); break;
                case 's': letter= &(da.s); break;
                case 't': letter= &(da.t); break;
                case 'u': letter= &(da.u); break;
                case 'v': letter= &(da.v); break;
                case 'w': letter= &(da.w); break;
                case 'x': letter= &(da.x); break;
                case 'y': letter= &(da.y); break;
                case 'z': letter= &(da.z); break;
                case '0': letter= &(da.d0); break;
                case '1': letter= &(da.d1); break;
                case '2': letter= &(da.d2); break;
                case '3': letter= &(da.d3); break;
                case '4': letter= &(da.d4); break;
                case '5': letter= &(da.d5); break;
                case '6': letter= &(da.d6); break;
                case '7': letter= &(da.d7); break;
                case '8': letter= &(da.d8); break;
                case '9': letter= &(da.d9); break;
                case '.': letter= &(da.sd); break;
                case ',': letter= &(da.sc); break;
                case ' ': letter= &(da.ss); break;
            }

            if ( (unsigned int)c + da.width < (unsigned int)(b.columns) && (unsigned int)c > 0 && (unsigned int)r + da.height < (unsigned int)(b.rows) && (unsigned int)r > 0 )
            {
                int n= 0;
                for ( unsigned int i= 0; i < da.height; ++i )
                    for ( unsigned int j= 0; j < da.width; ++j )
                    {
                        if ( (*letter)[n++] )
                            b(r + i, c + j)= fcolor;
                        else if ( bcolor >= 0 )
                            b(r + i, c + j)= bcolor;
                    }
                for ( unsigned int i= 0; i < da.height; ++i )
		  b(r + i, c + da.width)= bcolor;
            }
            c+= da.width+1;
        }
    }
}

#endif

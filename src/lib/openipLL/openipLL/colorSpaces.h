/**
 * @file colorSpaces.h
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
 * In this file several functions are defined for color space transformations
 */

#ifndef _COLOR_SPACES_H_
#define _COLOR_SPACES_H_

#include <openipDS/Image.h>
#include <openipDS/Histogram.h>

namespace openip
{
    /**
     * rgb to gray pixel conversion
     * @param r input red value
     * @param g input green value
     * @param b input blue value
     * @param gray output gray value
     */
    template<typename T>
    void rgb2gray(T r, T g, T b, T& gray);

    /**
     * rgb to gray image conversion
     * @param r input red channel image
     * @param g input green channel image
     * @param b input blue channel image
     * @param gray output gray level image
     */
    template<typename T>
    void rgb2gray(Image<T>& r, Image<T>& g, Image<T>& b, Image<T>& gray);

    /**
     * rgb to hsv pixel conversion
     * @param r input red value
     * @param g input green value
     * @param b input blue value
     * @param h output hue value
     * @param s output saturation value
     * @param v output value value
     */
    void rgb2hsv(unsigned char r, unsigned char g, unsigned char b, short& h, short& s, short& v);

    /**
     * rgb to hsv image conversion
     * @param r input red channel image
     * @param g input green channel image
     * @param b input blue channel image
     * @param h output hue image
     * @param s output saturation image
     * @param v output value image
     */
    void rgb2hsv(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>&b, Image<short>& h, Image<short>& s, Image<short>&v);

    /**
     * rgb to ycc pixel conversion
     * @param r input red value
     * @param g input green value
     * @param b input blue value
     * @param y output y value
     * @param cb output cb value
     * @param cr output cr value
     */
    void rgb2ycc(unsigned char r, unsigned char g, unsigned char b, unsigned char& y, unsigned char& cb, unsigned char& cr);

    /**
     * rgb to ycc image conversion
     * @param r input red channel image
     * @param g input green channel image
     * @param b input blue channel image
     * @param y output y image
     * @param cb output cb image
     * @param cr output cr image
     */
    void rgb2ycc(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b, Image<unsigned char>& y, Image<unsigned char>& cb, Image<unsigned char>& cr);

    /**
     * ycc to rgb pixel conversion
     * @param y input y value
     * @param cb input cb value
     * @param cr input cr value
     * @param r output red value
     * @param g output green value
     * @param b output blue value
     */
    void ycc2rgb(unsigned char y, unsigned char cb, unsigned char cr, unsigned char& r, unsigned char& g, unsigned char& b);

    /**
     * ycc to rgb image conversion
     * @param y input y image
     * @param cb input cb image
     * @param cr input cr image
     * @param r output red image
     * @param g output green image
     * @param b output blue image
     */
    void ycc2rgb(Image<unsigned char>& y, Image<unsigned char>& cb, Image<unsigned char>& cr, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b);

    /**
     * hsv to rgb pixel conversion
     * @param H input hue value
     * @param S input saturation value
     * @param V input vlaue value
     * @param R output red value
     * @param G output green value
     * @param B output blue value
     */
    void hsv2rgb(short H, short S, short V, unsigned char& R, unsigned char& G, unsigned char& B);

    /**
     * hsv to rgb image conversion
     * @param h input hue image
     * @param s input saturation image
     * @param v input value image
     * @param r output red image
     * @param g output green image
     * @param b output blue image
     */
    void hsv2rgb(Image<short>& h, Image<short>& s, Image<short>& v, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b);

    /**
     * rgb to hsl pixel conversion
     * @param r input red value
     * @param g input green value
     * @param b input blue value
     * @param h output hue value
     * @param s output saturation value
     * @param l output lightness value
     */
    void rgb2hsl(unsigned char r, unsigned char g, unsigned char b, short& h, short& s, short& l);

    /**
     * rgb to hsl image conversion
     * @param r input red channel image
     * @param g input green channel image
     * @param b input blue channel image
     * @param h output hue image
     * @param s output saturation image
     * @param l output lightness image
     */
    void rgb2hsl(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>&b, Image<short>& h, Image<short>& s, Image<short>&l);

    /**
     * hsl to rgb pixel conversion
     * @param H input hue value
     * @param S input saturation value
     * @param L input lightness value
     * @param R output red value
     * @param G output green value
     * @param B output blue value
     */
    void hsl2rgb(short H, short S, short L, unsigned char& R, unsigned char& G, unsigned char& B);

    /**
     * hsl to rgb image conversion
     * @param h input hue image
     * @param s input saturation image
     * @param l input lightness image
     * @param r output red channel image
     * @param g output green channel image
     * @param b output blue channel image
     */
    void hsl2rgb(Image<short>& h, Image<short>& s, Image<short>&l, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>&b);

    /**
     * rgb to xyz pixel conversion
     * @param r input red value
     * @param g input green value
     * @param b input blue value
     * @param x output x value
     * @param y output y value
     * @param z output z value
     */
    void rgb2xyz(unsigned char r, unsigned char g, unsigned char b, unsigned char& x, unsigned char& y, unsigned char& z);

    /**
     * rgb to xyz pixel conversion
     * @param r input red value
     * @param g input green value
     * @param b input blue value
     * @param x output x value
     * @param y output y value
     * @param z output z value
     */
    void rgb2xyz(unsigned char r, unsigned char g, unsigned char b, float& x, float& y, float& z);

    /**
     * rgb to xyz pixel conversion
     * @param r input red value
     * @param g input green value
     * @param b input blue value
     * @param x output x value
     * @param y output y value
     * @param z output z value
     */
    void rgb2xyz(float r, float g, float b, float& x, float& y, float& z);

    /**
     * rgb to xyz image conversion
     * @param r input red channel image
     * @param g input green channel image
     * @param b input blue channel image
     * @param x output x image
     * @param y output y image
     * @param z output z image
     */
    void rgb2xyz(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b, Image<unsigned char>& x, Image<unsigned char>& y, Image<unsigned char>& z);

    /**
     * rgb to xyz image conversion
     * @param r input red channel image
     * @param g input green channel image
     * @param b input blue channel image
     * @param x output x image
     * @param y output y image
     * @param z output z image
     */
    void rgb2xyz(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b, Image<float>& x, Image<float>& y, Image<float>& z);

    /**
     * rgb to xyz image conversion
     * @param r input red channel image
     * @param g input green channel image
     * @param b input blue channel image
     * @param x output x image
     * @param y output y image
     * @param z output z image
     */
    void rgb2xyz(Image<float>& r, Image<float>& g, Image<float>& b, Image<float>& x, Image<float>& y, Image<float>& z);

    /**
     * xyz to rgb pixel conversion
     * @param x input x value
     * @param y input y value
     * @param z input z value
     * @param r output red value
     * @param g output green value
     * @param b output blue value
     */
    void xyz2rgb(unsigned char x, unsigned char y, unsigned char z, unsigned char& r, unsigned char& g, unsigned char& b);

    /**
     * xyz to rgb pixel conversion
     * @param x input x value
     * @param y input y value
     * @param z input z value
     * @param r output red value
     * @param g output green value
     * @param b output blue value
     */
    void xyz2rgb(float x, float y, float z, unsigned char& r, unsigned char& g, unsigned char& b);

    /**
     * xyz to rgb pixel conversion
     * @param x input x value
     * @param y input y value
     * @param z input z value
     * @param r output red value
     * @param g output green value
     * @param b output blue value
     */
    void xyz2rgb(float x, float y, float z, float& r, float& g, float& b);

    /**
     * xyz to rgb image conversion
     * @param x input x image
     * @param y input y image
     * @param z input z image
     * @param r output red channel image
     * @param g output green channel image
     * @param b output blue channel image
     */
    void xyz2rgb(Image<unsigned char>& x, Image<unsigned char>& y, Image<unsigned char>& z, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b);

    /**
     * xyz to rgb image conversion
     * @param x input x image
     * @param y input y image
     * @param z input z image
     * @param r output red channel image
     * @param g output green channel image
     * @param b output blue channel image
     */
    void xyz2rgb(Image<float>& x, Image<float>& y, Image<float>& z, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b);

    /**
     * xyz to rgb image conversion
     * @param x input x image
     * @param y input y image
     * @param z input z image
     * @param r output red channel image
     * @param g output green channel image
     * @param b output blue channel image
     */
    void xyz2rgb(Image<float>& x, Image<float>& y, Image<float>& z, Image<float>& r, Image<float>& g, Image<float>& b);

    /**
     * xyz to luv pixel conversion
     * @param x input x value
     * @param y input y value
     * @param z input z value
     * @param l output l CIELUV value
     * @param u output u CIELUV value
     * @param v output v CIELUV value
     */
    void xyz2luv(float x, float y, float z, float& l, float& u, float& v);

    /**
     * rgb to luv pixel conversion
     * @param r input red value
     * @param g input green value
     * @param b input blue value
     * @param l output l CIELUV value
     * @param u output u CIELUV value
     * @param v output v CIELUV value
     */
    void rgb2luv(float r, float g, float b, float& l, float& u, float& v);

    /**
     * xyz to luv image conversion
     * @param x input x image
     * @param y input y image
     * @param z input z image
     * @param l output l CIELUV image
     * @param u output u CIELUV image
     * @param v output v CIELUV image
     */
    void xyz2luv(Image<float>& x, Image<float>& y, Image<float>& z, Image<float>& l, Image<float>& u, Image<float>& v);

    /**
     * rgb to luv image conversion
     * @param r input red channel image
     * @param g input green channel image
     * @param b input blue channel image
     * @param l output l CIELUV image
     * @param u output u CIELUV image
     * @param v output v CIELUV image
     */
    void rgb2luv(Image<float>& r, Image<float>& g, Image<float>& b, Image<float>& l, Image<float>& u, Image<float>& v);

    /**
     * rgb to luv pixel conversion
     * @param r input red value
     * @param g input green value
     * @param b input blue value
     * @param l output l CIELUV value
     * @param u output u CIELUV value
     * @param v output v CIELUV value
     */
    void rgb2luv(unsigned char r, unsigned char g, unsigned char b, unsigned char& l, unsigned char& u, unsigned char& v);

    /**
     * rgb to luv image conversion
     * @param r input red channel image
     * @param g input green channel image
     * @param b input blue channel image
     * @param l output l CIELUV image
     * @param u output u CIELUV image
     * @param v output v CIELUV image
     *
     */
    void rgb2luv(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b, Image<unsigned char>& l, Image<unsigned char>& u, Image<unsigned char>& v);

    /**
     * rgb to luv pixel conversion
     * @param r input red value
     * @param g input green value
     * @param b input blue value
     * @param l output l CIELUV value
     * @param u output u CIELUV value
     * @param v output v CIELUV value
     */
    void rgb2luv(unsigned char r, unsigned char g, unsigned char b, float& l, float& u, float& v);

    /**
     * rgb to luv image conversion
     * @param r input red channel image
     * @param g input green channel image
     * @param b input blue channel image
     * @param l output l CIELUV image
     * @param u output u CIELUV image
     * @param v output v CIELUV image
     */
    void rgb2luv(Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b, Image<float>& l, Image<float>& u, Image<float>& v);

    /**
     * luv to xyz pixel conversion
     * @param l input l CIELUV value
     * @param u input u CIELUV value
     * @param v input v CIELUV value
     * @param x output x value
     * @param y output y value
     * @param z output z value
     */
    void luv2xyz(float l, float u, float v, float& x, float& y, float& z);

    /**
     * luv to xyz image conversion
     * @param l input l CIELUV image
     * @param u input u CIELUV image
     * @param v input v CIELUV image
     * @param x output x image
     * @param y output y image
     * @param z output z image
     */
    void luv2xyz(Image<float>& l, Image<float>& u, Image<float>& v, Image<float>& x, Image<float>& y, Image<float>& z);

    /**
     * luv to rgb pixel conversion
     * @param l input l CIELUV value
     * @param u input u CIELUV value
     * @param v input v CIELUV value
     * @param r output red value
     * @param g output green value
     * @param b output blue value
     */
    void luv2rgb(float l, float u, float v, unsigned char& r, unsigned char& g, unsigned char& b);

    /**
     * luv to rgb image conversion
     * @param l input l CIELUV image
     * @param u input u CIELUV image
     * @param v input v CIELUV image
     * @param r output red channel image
     * @param g output green channel image
     * @param b output blue channel image
     */
    void luv2rgb(Image<float>& l, Image<float>& u, Image<float>& v, Image<unsigned char>& r, Image<unsigned char>& g, Image<unsigned char>& b);

    /**
     * invert image
     * @param b image to invert
     */
    void invert(Image<unsigned char>& b);

    template<typename T>
    void rgb2gray(T r, T g, T b, T& gray)
    {
        gray= (unsigned char)( (r)*0.3 + (g)*0.59 + (b)*0.11 );
    }

    template<typename T>
    void rgb2gray(Image<T>& r, Image<T>& g, Image<T>& b, Image<T>& gray)
    {
        typename Vector<T>::iterator itR= r.begin();
        typename Vector<T>::iterator itG= g.begin();
        typename Vector<T>::iterator itB= b.begin();
        typename Vector<T>::iterator itGray= gray.begin();

        for ( ; itGray != gray.end(); ++itGray )
        {
            rgb2gray(*itR, *itG, *itB, *itGray);
            ++itR;
            ++itG;
            ++itB;
        }
    }
}

#endif

/**
 * @file drawing.h
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
 * drawing.h contains several useful drawing algorithms
 */

#ifndef _DRAWING_H_
#define _DRAWING_H_

#include <openipDS/Image.h>
#include <openipDS/PixelSequence1.h>
#include <openipDS/PixelSequence2.h>
#include <openipDS/PixelSet1.h>
#include <openipDS/PixelSet2.h>
#include <openipDS/constants.h>

using namespace openip;

namespace openip
{
    /**
     * draws a rectangle on the image
     * @param input image to draw on
     * @param xMin upper left corner x coordinate
     * @param yMin upper left corner y coordinate
     * @param xMax lower right corner x coordinate
     * @param yMax lower right corner y coordinate
     * @param color color
     * @param fill boolean parameter wether to fill the rectangle
     */
    template<typename T>
    void drawRectangle(Image<T>& input, int xMin, int yMin, int xMax, int yMax, T color, bool fill=false);

    /**
     * draws line segment with Bresenham's algorithm
     * @param input image to draw on
     * @param x0 x coordinate of one end of the line segment
     * @param y0 y coordinate of one end of the line segment
     * @param x1 x coordinate of other end of the line segment
     * @param y1 y coordinate of other end of the line segment
     * @param color color of the line segment
     */
    template<typename T>
    void drawLine(Image<T>& input, int x0, int y0, int x1, int y1, T color);

    /**
     * draws line from p0 to p1 in row-continuous representation
     * @param input image to draw on
     * @param p0 one end of the line segment
     * @param p1 other end of the line segment
     * @param color color of the line segment
     */
    template<typename T>
    void drawLine(Image<T>& input, int p0, int p1, T color);

    /**
     * draws spike
     * @param input image to draw on
     * @param x x coordinate of the start point of the spike
     * @param y y coordinate of the start point of the spike
     * @param degree
     * @param strength
     * @param color color of the spike segment
     */
    template<typename T>
    void drawSpike(Image<T>& input, int x, int y, float degree, float strength, T color);

    /**
     * draws a circle on the input image using midpoint algorithm
     * @param input image to draw on
     * @param x0 x coordinate of the center of the circle
     * @param y0 y coordinate of the center of the circle
     * @param radius radius of the circle
     * @param color color of the circle
     * @param fill boolean parameter wether fill the circle or not
     */
    template<typename T>
    void drawCircle(Image<T>& input, int x0, int y0, int radius, T color, bool fill= false);

    /**
     * draws an ellipse on the input image using midpoint algorithm
     * @param input image to draw on
     * @param xc x coordinate of the center of the ellipse
     * @param yc y coordinate of the center of the ellipse
     * @param rx one radius of the ellipse
     * @param ry other radius of the ellipse
     * @param color color of the ellipse
     * @param fill boolean parameter wether fill the ellipse or not
     */
    template<typename T>
    void drawEllipse(Image<T>& input, int xc, int yc, int rx, int ry, T color, bool fill= false);

    /**
     * draws a triangle on the input image
     * @param input image to draw on
     * @param x1 x coordinate of vertice A of the triangle
     * @param y1 y coordinate of vertice A of the triangle
     * @param x2 x coordinate of vertice B of the triangle
     * @param y2 y coordinate of vertice B of the triangle
     * @param x3 x coordinate of vertice C of the triangle
     * @param y3 y coordinate of vertice C of the triangle
     * @param color color of the triangle
     * @param fill boolean parameter wether fill the triangle or not
     */
    template<typename T>
    void drawTriangle(Image<T>& input, float x1, float y1, float x2, float y2, float x3, float y3, T color, bool fill= false);

    /**
     * draws a pixel set on the image
     * @param input image to draw on
     * @param ps pixelset to draw, in row-continuous representation
     * @param color color of the pixels in the set
     */
    template<typename T1>
    void drawPixelSet1(Image<T1>& input, PixelSet1& ps, T1 color);

    /**
     * draws a pixel set on the image
     * @param input image to draw on
     * @param ps pixelset to draw, in two coordinate representation
     * @param color color of the pixels in the set
     */
    template<typename T1>
    void drawPixelSet2(Image<T1>& input, PixelSet2& ps, T1 color);

    /**
     * draws a pixel sequence on the image
     * @param input image to draw on
     * @param ps pixel sequence to draw, in row-continuous representation
     * @param color color of the pixels in the sequence
     */
    template<typename T1>
    void drawPixelSequence1(Image<T1>& input, PixelSequence1& ps, T1 color);

    /**
     * draws a pixel sequence on the image
     * @param input image to draw on
     * @param ps pixel sequence to draw, in two coordinate representation
     * @param color color of the pixels in the sequence
     */
    template<typename T1>
    void drawPixelSequence2(Image<T1>& input, PixelSequence2& ps, T1 color);

    /**
     * draws a cross on the image
     * @param input input image to draw on
     * @param x x coordinate of the cross
     * @param y y coordinate of the cross
     * @param size size of the cross
     * @param color color of the cross
     */
    template<typename T1>
    void drawCross(Image<T1>& input, int x, int y, int size, T1 color);

    /**
     * draws crosses in the positions of the pixel sequence of the image
     * @param input input image to draw on
     * @param ps pixel sequence in which's positions the crosses to draw
     * @param size size of the crosses
     * @param color color of the crosses
     */
    template<typename T1>
    void drawCrosses(Image<T1>& input, PixelSequence1& ps, int size, T1 color);

    /**
     * draws crosses in the positions of the pixel sequence of the image
     * @param input input image to draw on
     * @param ps pixel sequence in which's positions the crosses to draw
     * @param size size of the crosses
     * @param color color of the crosses
     */
    template<typename T1>
    void drawCrosses(Image<T1>& input, PixelSequence2& ps, int size, T1 color);

    /**
     * draws crosses in the positions of the pixel set of the image
     * @param input input image to draw on
     * @param ps pixel set in which's positions the crosses to draw
     * @param size size of the crosses
     * @param color color of the crosses
     */
    template<typename T1>
    void drawCrosses(Image<T1>& input, PixelSet1& ps, int size, T1 color);

    /**
     * draws crosses in the positions of the pixel set of the image
     * @param input input image to draw on
     * @param ps pixel set in which's positions the crosses to draw
     * @param size size of the crosses
     * @param color color of the crosses
     */
    template<typename T1>
    void drawCrosses(Image<T1>& input, PixelSet2& ps, int size, T1 color);

    /**
     * draws connected pixel sequence
     * @param input input image to draw on
     * @param ps pixel sequence which's pixels to draw and connect
     * @param color color of the connected pixel sequence
     * @param connectFirstAndLast boolean parameter wether connect first and last pixels or not
     */
    template<typename T1>
    void drawConnectedPixelSequence2(Image<T1>& input, PixelSequence2& ps, T1 color, bool connectFirstAndLast= false);

    /**
     * draws connected and filled pixel sequence
     * @param input input image to draw on
     * @param ps pixel sequence which's pixels define the polygon
     * @param color color of the connected pixel sequence
     */
    template<typename T1>
    void drawFilledPixelSequence2(Image<T1>& input, PixelSequence2& ps, T1 color);

    /**
     * draws connected pixel sequence
     * @param input input image to draw on
     * @param ps pixel sequence which's pixels to draw and connect, in row-continuous representation
     * @param color color of the connected pixel sequence
     * @param connectFirstAndLast boolean parameter wether connect first and last pixels or not
     */

    template<typename T1>
    void drawConnectedPixelSequence1(Image<T1>& input, PixelSequence1& ps, T1 color, bool connectFirstAndLast= false);

    /**
     * draws connected and filled pixel sequence
     * @param input input image to draw on
     * @param ps pixel sequence which's pixels define the polygon, in row-continuous representation
     * @param color color of the connected pixel sequence
     */
    template<typename T1>
    void drawFilledPixelSequence1(Image<T1>& input, PixelSequence1& ps, T1 color);

    /**
     * draws a hough line on the input image
     * @param input image to draw on
     * @param d distance from origo
     * @param alfa normal vector angle
     */
    template<typename T>
    void drawHoughLine(Image<T>& input, float d, float alfa);

    /**
     * draws a gradient spike on the image
     * @param input image to draw on
     * @param x x coordinate of the gradient to draw
     * @param y y coordinate of the gradient to draw
     * @param gx x component of the gradient vector
     * @param gy y component of the gradient vector
     * @param color color of the spike
     */
    template<typename T>
    void drawGradient(Image<T>& input, int x, int y, float gx, float gy, T color);

    /**
     * draws gradient spikes on the image in the positions of ps pixel sequence
     * @param input image to draw on
     * @param ps pixel sequence in which's positions the gradient spikes to draw,
     * in two coordinate representation
     * @param gx x component map of the gradient vectors
     * @param gy y component map of the gradient vectors
     * @param color color of the spikes
     */
    template<typename T>
    void drawGradients(Image<T>& input, PixelSequence2& ps, Image<float>& gx, Image<float>& gy, T color);

    /**
     * draws gradient spikes on the image in the positions of ps pixel sequence
     * @param input image to draw on
     * @param ps pixel sequence in which's positions the gradient spikes to draw,
     * in row-continuous representation
     * @param gx x component map of the gradient vectors
     * @param gy y component map of the gradient vectors
     * @param color color of the spikes
     */
    template<typename T>
    void drawGradients(Image<T>& input, PixelSequence1& ps, Image<float>& gx, Image<float>& gy, T color);
}

template<typename T>
void openip::drawGradient(Image<T>& input, int x, int y, float gx, float gy, T color)
{
    drawLine(input, x, y, x + gx, y + gy, color);
    drawCircle(input, x, y, 2, color, true);
}

template<typename T>
void openip::drawGradients(Image<T>& input, PixelSequence1& ps, Image<float>& gx, Image<float>& gy, T color)
{
    int x, y;

    for ( PixelSequence1::iterator psit= ps.begin(); psit != ps.end(); ++psit )
    {
        input.getCoordinate2D(*psit, x, y);
        drawGradient(input, x, y, x + gx(x, y), y + gy(x, y));
    }
}

template<typename T>
void openip::drawGradients(Image<T>& input, PixelSequence2& ps, Image<float>& gx, Image<float>& gy, T color)
{
    for ( PixelSequence2::iterator psit= ps.begin(); psit != ps.end(); ++psit )
    {
        drawGradient(input, (*psit).x(), (*psit).y(), gx((*psit).x(), (*psit).y()), gy((*psit).x(), (*psit).y()), color);
    }
}

template<typename T1>
void openip::drawCrosses(Image<T1>& input, PixelSequence2& ps, int size, T1 color)
{
    for ( PixelSequence2::iterator psit= ps.begin(); psit != ps.end(); ++psit )
    {
        drawCross(input, (*psit).x, (*psit).y, size, color);
    }
}

template<typename T1>
void openip::drawCross(Image<T1>& input, int x, int y, int size, T1 color)
{
    if ( x >= 3 * size )
            drawLine(input, x - 3 * size, y, x - size, y, color);
    if ( ((int)(input.columns) - x) > 3 * size )
            drawLine(input, x + size + 1, y, x + 3 * size + 1, y, color);
    if ( y >= 3 * size )
            drawLine(input, x, y - 3 * size, x, y - size, color);
    if ( ((int)(input.rows) - y) > 3 * size )
            drawLine(input, x, y + size + 1, x, y + 3 * size + 1, color);
}

template<typename T>
void openip::drawHoughLine(openip::Image<T>& input, float d, float alfa)
{
    float x0= d / cos(alfa);
    float y0= d / cos(M_PI/2 - alfa);
    float t1= y0 - input.rows;
    float t2= x0 - input.columns;
    float x1= t1 / tan(M_PI/2 - alfa);
    float y1= t2 * tan(M_PI/2 - alfa);

    if ( x0 >= 0 && x0 < input.columns )
    {
        if ( y0 >= 0 && y0 < input.rows )
            drawLine(input, x0, 0, 0, y0, FOREGROUND);
        else if ( y1 >= 0 && y1 < input.rows )
            drawLine(input, x0, 0, (int)(input.columns) - 1, y1, FOREGROUND);
        else if ( x1 >= 0 && x1 < input.columns )
            drawLine(input, x0, 0, x1, input.rows, FOREGROUND);
    }
    else if ( x1 >= 0 && x1 < input.columns )
    {
        if ( y0 >= 0 && y0 < input.rows )
            drawLine(input, x1, (int)(input.rows) - 1, 0, y0, FOREGROUND);
        else if ( y1 >= 0 && y1 < input.rows )
            drawLine(input, x1, (int)(input.rows) - 1, input.columns, y1, FOREGROUND);
    }
    else if ( y0 >= 0 && y0 < input.rows )
        if ( y1 >= 0 && y1 < input.rows )
            drawLine(input, 0, y0, input.columns, y1, FOREGROUND);
}

template<typename T>
void openip::drawRectangle(Image<T>& input, int xMin, int yMin, int xMax, int yMax, T color, bool filled)
{
    for ( int i= xMin; i <= xMax; ++i )
    {
        input(i,yMin)= color;
        input(i,yMax)= color;
    }
    for ( int i= yMin; i <= yMax; ++i )
    {
        input(xMin, i)= color;
        input(xMax, i)= color;
    }

    if ( filled )
        for ( int i= xMin+1; i < xMax; ++i )
            for ( int j= yMin + 1; j < yMax; ++j )
                input(i,j)= color;
}

template<typename T>
void openip::drawLine(Image<T>& input, int x0, int x1, T color)
{
    openip::drawLine(input, x0%input.columns, x0/input.columns, x1%input.columns, x1/input.columns);
}

template<typename T>
void openip::drawCircle(Image<T>& input, int x0, int y0, int radius, T color, bool fill)
{
    int f= 1-radius;
    int ddfx= 1;
    int ddfy= -2*radius;
    int x= 0;
    int y= radius;

    input(x0, y0 + radius)= color;
    input(x0, y0 - radius)= color;
    input(x0 + radius, y0)= color;
    input(x0 - radius, y0)= color;

    if ( fill )
            for ( int i= x0 - radius; i <= x0 + radius; ++i )
                    input(i, y0)= color;

    while( x < y )
    {
        if ( f >= 0 )
        {
            y--;
            ddfy+= 2;
            f+= ddfy;
        }

        x++;
        ddfx+= 2;
        f+= ddfx;

        input(x0 + x, y0 + y)= color;
        input(x0 - x, y0 + y)= color;
        input(x0 + x, y0 - y)= color;
        input(x0 - x, y0 - y)= color;
        input(x0 + y, y0 + x)= color;
        input(x0 - y, y0 + x)= color;
        input(x0 + y, y0 - x)= color;
        input(x0 - y, y0 - x)= color;

        if ( fill )
        {
            for ( int i= x0 - x + 1; i < x0 + x; ++i )
                    input(i, y0 + y)= color;
            for ( int i= x0 - x + 1; i < x0 + x; ++i )
                    input(i, y0 - y)= color;
            for ( int i= x0 - y + 1; i < x0 + y; ++i )
                    input(i, y0 + x)= color;
            for ( int i= x0 - y + 1; i < x0 + y; ++i )
                    input(i, y0 - x)= color;
        }
    }
}

template<typename T>
void openip::drawEllipse(Image<T>& input, int xc, int yc, int rx, int ry, T color, bool fill)
{
    float x, y;
    float p, m;
    x= 0;
    y= ry;
    p= ry*ry - rx*rx*ry - rx/4;
    m= -(x*ry*ry)/(y*rx*rx);

    while(fabs(m) < 1)
    {
        input(xc + x, yc + y)= color;
        input(xc + x, yc - y)= color;
        input(xc - x, yc + y)= color;
        input(xc - x, yc - y)= color;

        if ( fill )
            for ( int i= yc - y; i < yc + y; ++i )
            {
                input(xc - x, i)= color;
                input(xc + x, i)= color;
            }

        if ( p < 0 )
        {
            p+= ry*ry*(2*x + 3);
        }
        else
        {
            p+= ry*ry*(2*x + 3) + rx*rx*(-2*y+2);
            y--;
        }
        x++;
        m= -(x*ry*ry)/(y*rx*rx);
    }

    p=(x+1.0/2)*(x+1.0/2)*ry*ry + (y-1)*(y-1)*rx*rx - rx*rx*ry*ry;

    while( y >= 0)
    {
        input(xc + x, yc + y)= color;
        input(xc + x, yc - y)= color;
        input(xc - x, yc + y)= color;
        input(xc - x, yc - y)= color;

        if ( fill )
            for ( int i= yc - y; i < yc + y; ++i )
            {
                    input(xc - x, i)= color;
                    input(xc + x, i)= color;
            }

        if ( p < 0 )
        {
            x++;
            p+= ry*ry*(2*x + 2) + rx*rx*(-2*y+3);
        }
        else
        {
            p+= rx*rx*(-2*y+3);
        }
        y--;
    }
}

template<typename T>
void openip::drawTriangle(Image<T>& input, float x1, float y1, float x2, float y2, float x3, float y3, T color, bool fill)
{
    if ( !fill )
    {
        drawLine(input, x1, y1, x2, y2, color);
        drawLine(input, x2, y2, x3, y3, color);
        drawLine(input, x3, y3, x1, y1, color);
        return;
    }

    float dx1, dx2, dx3, tmp;

    if ( y2 <= y1 && y2 <= y3 )
    {
        tmp= y2; y2= y1; y1= tmp;
        tmp= x2; x2= x1; x1= tmp;

        if ( y3 < y2 )
        {
            tmp= y2; y2= y3; y3= tmp;
            tmp= x2; x2= x3; x3= tmp;
        }
    }
    else if ( y3 <= y1 && y3 <= y2 )
    {
        tmp= y3; y3= y1; y1= tmp;
        tmp= x3; x3= x1; x1= tmp;

        if ( y3 < y2 )
        {
            tmp= y2; y2= y3; y3= tmp;
            tmp= x2; x2= x3; x3= tmp;
        }
    }
    else if ( y3 < y2 )
    {
        tmp= y2; y2= y3; y3= tmp;
        tmp= x2; x2= x3; x3= tmp;
    }

    if ( (y2 - y1) > 0 ) dx1= (x2 - x1)/(y2 - y1);
    else dx1= x2 - x1;

    if ( (y3 - y1) > 0 ) dx2= (x3 - x1)/(y3 - y1);
    else dx2= 0;

    if ( (y3 - y2) > 0 ) dx3= (x3 - x2)/(y3 - y2);
    else dx3= 0;

    float y, xs, xe;

    xs= x1;
    xe= x1;
    y= y1;

    if ( dx1 > dx2 )
    {
        for (; y <= y2; ++y, xs+= dx2, xe+= dx1)
        {
              for ( int i= xs; i < xe; ++i )
                    input(i, y)= color;
        }

        xe= x2;
        y= y2;
        for (; y <= y3; ++y, xs+= dx2, xe+= dx3)
        {
            for ( int i= xs; i < xe; ++i )
                    input(i, y)= color;
        }
    }
    else
    {
        for (; y <= y2; ++y, xs+= dx1, xe+= dx2)
        {
            for ( int i= xs; i < xe; ++i )
                    input(i, y)= color;
        }

        xs= x2;
        y= y2;
        for (; y <= y3; ++y, xs+= dx3, xe+= dx2)
        {
            for ( int i= xs; i < xe; ++i )
                    input(i, y)= color;
        }
    }
}

template<typename T>
void openip::drawLine(Image<T>& input, int x0, int y0, int x1, int y1, T color)
{
    int dy = y1 - y0;
    int dx = x1 - x0;
    int stepx, stepy;

    if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
    if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }

    input(x0, y0)= color;
    input(x1, y1)= color;
    if (dx > dy) {
        int length = (dx - 1) >> 2;
        int extras = (dx - 1) & 3;
        int incr2 = (dy << 2) - (dx << 1);
        if (incr2 < 0) {
            int c = dy << 1;
            int incr1 = c << 1;
            int d =  incr1 - dx;
            for (int i = 0; i < length; i++) {
                x0 += stepx;
                x1 -= stepx;
                if (d < 0) {						// Pattern:
                    input(x0, y0)= color;			//
                    input(x0 += stepx, y0)= color;	//  x o o
                    input(x1, y1)= color;			//
                    input(x1 -= stepx, y1)= color;
                    d += incr1;
                } else {
                    if (d < c) {							// Pattern:
                        input(x0, y0)= color;				//      o
                        input(x0 += stepx, y0 += stepy)= color;		//  x o
                        input(x1, y1)= color;				//
                        input(x1 -= stepx, y1 -= stepy)= color;
                    } else {
                        input(x0, y0 += stepy)= color;			// Pattern:
                        input(x0 += stepx, y0)= color;			//    o o
                        input(x1, y1 -= stepy)= color;			//  x
                        input(x1 -= stepx, y1)= color;			//
                    }
                    d += incr2;
                }
            }
            if (extras > 0) {
                if (d < 0) {
                    input(x0 += stepx, y0)= color;
                    if (extras > 1) input(x0 += stepx, y0)= color;
                    if (extras > 2) input(x1 -= stepx, y1)= color;
                } else
                if (d < c) {
                    input(x0 += stepx, y0)= color;
                    if (extras > 1) input(x0 += stepx, y0 += stepy)= color;
                    if (extras > 2) input(x1 -= stepx, y1)= color;
                } else {
                    input(x0 += stepx, y0 += stepy)= color;
                    if (extras > 1) input(x0 += stepx, y0)= color;
                    if (extras > 2) input(x1 -= stepx, y1 -= stepy)= color;
                }
            }
        } else {
            int c = (dy - dx) << 1;
            int incr1 = c << 1;
            int d =  incr1 + dx;
            for (int i = 0; i < length; i++) {
                x0 += stepx;
                x1 -= stepx;
                if (d > 0) {
                    input(x0, y0 += stepy)= color;			// Pattern:
                    input(x0 += stepx, y0 += stepy)= color;		//      o
                    input(x1, y1 -= stepy)= color;			//    o
                    input(x1 -= stepx, y1 -= stepy)= color;		//  x
                    d += incr1;
                } else {
                    if (d < c) {
                        input(x0, y0)= color;				// Pattern:
                        input(x0 += stepx, y0 += stepy)= color;       //      o
                        input(x1, y1)= color;                         //  x o
                        input(x1 -= stepx, y1 -= stepy)= color;       //
                    } else {
                        input(x0, y0 += stepy)= color;			// Pattern:
                        input(x0 += stepx, y0)= color;			//    o o
                        input(x1, y1 -= stepy)= color;			//  x
                        input(x1 -= stepx, y1)= color;			//
                    }
                    d += incr2;
                }
            }
            if (extras > 0) {
                if (d > 0) {
                    input(x0 += stepx, y0 += stepy)= color;
                    if (extras > 1) input(x0 += stepx, y0 += stepy)= color;
                    if (extras > 2) input(x1 -= stepx, y1 -= stepy)= color;
                } else
                if (d < c) {
                    input(x0 += stepx, y0)= color;
                    if (extras > 1) input(x0 += stepx, y0 += stepy)= color;
                    if (extras > 2) input(x1 -= stepx, y1)= color;
                } else {
                    input(x0 += stepx, y0 += stepy)= color;
                    if (extras > 1) input(x0 += stepx, y0)= color;
                    if (extras > 2) {
                        if (d > c)
                            input(x1 -= stepx, y1 -= stepy)= color;
                        else
                            input(x1 -= stepx, y1)= color;
                    }
                }
            }
        }
    } else {
        int length = (dy - 1) >> 2;
        int extras = (dy - 1) & 3;
        int incr2 = (dx << 2) - (dy << 1);
        if (incr2 < 0) {
            int c = dx << 1;
            int incr1 = c << 1;
            int d =  incr1 - dy;
            for (int i = 0; i < length; i++) {
                y0 += stepy;
                y1 -= stepy;
                if (d < 0) {
                    input(x0, y0)= color;
                    input(x0, y0 += stepy)= color;
                    input(x1, y1)= color;
                    input(x1, y1 -= stepy)= color;
                    d += incr1;
                } else {
                    if (d < c) {
                        input(x0, y0)= color;
                        input(x0 += stepx, y0 += stepy)= color;
                        input(x1, y1)= color;
                        input(x1 -= stepx, y1 -= stepy)= color;
                    } else {
                        input(x0 += stepx, y0)= color;
                        input(x0, y0 += stepy)= color;
                        input(x1 -= stepx, y1)= color;
                        input(x1, y1 -= stepy)= color;
                    }
                    d += incr2;
                }
            }
            if (extras > 0) {
                if (d < 0) {
                    input(x0, y0 += stepy)= color;
                    if (extras > 1) input(x0, y0 += stepy)= color;
                    if (extras > 2) input(x1, y1 -= stepy)= color;
                } else
                if (d < c) {
                    input(x0, y0 += stepy)= color;
                    if (extras > 1) input(x0 += stepx, y0 += stepy)= color;
                    if (extras > 2) input(x1, y1 -= stepy)= color;
                } else {
                    input(x0 += stepx, y0 += stepy)= color;
                    if (extras > 1) input(x0, y0 += stepy)= color;
                    if (extras > 2) input(x1 -= stepx, y1 -= stepy)= color;
                }
            }
        } else {
            int c = (dx - dy) << 1;
            int incr1 = c << 1;
            int d =  incr1 + dy;
            for (int i = 0; i < length; i++) {
                y0 += stepy;
                y1 -= stepy;
                if (d > 0) {
                    input(x0 += stepx, y0)= color;
                    input(x0 += stepx, y0 += stepy)= color;
                    input(x1 -= stepy, y1)= color;
                    input(x1 -= stepx, y1 -= stepy)= color;
                    d += incr1;
                } else {
                    if (d < c) {
                        input(x0, y0)= color;
                        input(x0 += stepx, y0 += stepy)= color;
                        input(x1, y1)= color;
                        input(x1 -= stepx, y1 -= stepy)= color;
                    } else {
                        input(x0 += stepx, y0)= color;
                        input(x0, y0 += stepy)= color;
                        input(x1 -= stepx, y1)= color;
                        input(x1, y1 -= stepy)= color;
                    }
                    d += incr2;
                }
            }
            if (extras > 0) {
                if (d > 0) {
                    input(x0 += stepx, y0 += stepy)= color;
                    if (extras > 1) input(x0 += stepx, y0 += stepy)= color;
                    if (extras > 2) input(x1 -= stepx, y1 -= stepy)= color;
                } else
                if (d < c) {
                    input(x0, y0 += stepy)= color;
                    if (extras > 1) input(x0 += stepx, y0 += stepy)= color;
                    if (extras > 2) input(x1, y1 -= stepy)= color;
                } else {
                    input(x0 += stepx, y0 += stepy)= color;
                    if (extras > 1) input(x0, y0 += stepy)= color;
                    if (extras > 2) {
                        if (d > c)
                            input(x1 -= stepx, y1 -= stepy)= color;
                        else
                            input(x1, y1 -= stepy)= color;
                    }
                }
            }
        }
    }
}

template<typename T1>
void openip::drawPixelSet1(Image<T1>& input, PixelSet1& ps, T1 color)
{
    PixelSet1::iterator psit= ps.begin();

    while( psit != ps.end() )
    {
        input(*psit)= color;
        psit++;
    }
}

template<typename T1>
void openip::drawPixelSet2(Image<T1>& input, PixelSet2& ps, T1 color)
{
    PixelSet2::iterator psit= ps.begin();

    while( psit != ps.end() )
    {
        input(*psit)= color;
        psit++;
    }
}

template<typename T1>
void openip::drawPixelSequence1(Image<T1>& input, PixelSequence1& ps, T1 color)
{
    PixelSequence1::iterator psit= ps.begin();

    while( psit != ps.end() )
    {
        input(*psit)= color;
        psit++;
    }
}

template<typename T1>
void openip::drawPixelSequence2(Image<T1>& input, PixelSequence2& ps, T1 color)
{
    PixelSequence2::iterator psit= ps.begin();

    while( psit != ps.end() )
    {
        input(*psit)= color;
        psit++;
    }
}

template<typename T1>
void openip::drawConnectedPixelSequence2(Image<T1>& input, PixelSequence2& ps, T1 color, bool connectFirstAndLast)
{
    for ( unsigned int i= 0; i < ps.size()-1; ++i )
        drawLine(input, ps(i).x(), ps(i).y(), ps(i+1).x(), ps(i+1).y(), color);
    if ( connectFirstAndLast )
        drawLine(input, ps(ps.size()-1).x(), ps(ps.size()-1).y(), ps(0).x(), ps(0).y(), color);
}

template<typename T1>
void openip::drawFilledPixelSequence2(Image<T1>& input, PixelSequence2& ps, T1 color)
{
    for ( int i= 1; i < ps.size()-1; ++i )
        drawTriangle(input, ps(0).x(), ps(0).y(), ps(i).x(), ps(i).y(), ps(i+1).x(), ps(i+1).y(), color, true);
}

template<typename T1>
void openip::drawConnectedPixelSequence1(Image<T1>& input, PixelSequence1& ps, T1 color, bool connectFirstAndLast)
{
    for ( unsigned int i= 0; i < ps.size()-1; ++i )
        drawLine(input, ps(i)%input.columns, ps(i)/input.columns, ps(i+1)%input.columns, ps(i+1)/input.columns, color);
    if ( connectFirstAndLast )
        drawLine(input, ps(ps.size()-1)%input.columns, ps(ps.size()-1)/input.columns, ps(0)%input.columns, ps(0)/input.columns, color);
}

template<typename T1>
void openip::drawFilledPixelSequence1(Image<T1>& input, PixelSequence1& ps, T1 color)
{
    for ( int i= 1; i < ps.size()-1; ++i )
        drawTriangle(input, ps(0)%input.columns, ps(0)/input.columns, ps(i)%input.columns, ps(i)/input.columns, ps(i+1)%input.columns, ps(i+1)/input.columns, color, true);
}

template<typename T>
void openip::drawSpike(Image<T>& input, int row, int column, float degree, float strength, T color)
{
    float row1= row + sin(degree)*strength;
    float column1= column + cos(degree)*strength;

    if ( row < 0 || row1 < 0 || column < 0 || column1 < 0 )
        return;
    if ( row >= input.rows || row1 >= input.rows || column >= input.columns || column1 >= input.columns )
        return;
    drawCircle(input, row, column, 1, color, true);
    drawLine(input, row, column, row1, column1, color);
}

#endif

/**
 * @file StructuringElements.h
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
 * This file contains some common, special structuring elements.
 */

#ifndef _STRUCTURINGELEMENTS_H
#define	_STRUCTURINGELEMENTS_H

#include <openipDS/StructuringElement2.h>

namespace openip
{
    /**
     * method to generate disk structuring element
     * @param stride stride of the image, on which this structuring element will be used
     * @param se structuring element to generate
     * @param r radius of the disk shaped structuring element
     */
    void generateStructuringElementDisk(StructuringElement2& se, float r, int stride);

    /**
     * method to generate square structuring element
     * @param stride stride of the image, on which this structuring element will be used
     * @param se structuring element to generate
     * @param size size of a side of the square
     */
    void generateStructuringElementSquare(StructuringElement2& se, int size, int stride);

    /**
     * method to generate circle structuring element
     * @param stride stride of the image, on which this structuring element will be used
     * @param se structuring element to generate
     * @param r radius of the circle shaped structuring element
     */
    void generateStructuringElementCircle(StructuringElement2& se, int r, int stride);

    /**
     * StructuringElementSquare is a square shaped structuring element
     */
    class StructuringElementSquare : public StructuringElement2
    {
    public:
        using StructuringElement2::set;
        using StructuringElement2::operator=;
        using StructuringElement2::updateStride;
        using StructuringElement2::computeMinMax;

        /**
         * constructor to generate square structuring element
         * @param stride stride of the image on which the element will be used
         * @param size of a side of the square
         */
        StructuringElementSquare(int size= 1, int stride= 20000);
        
        StructuringElementSquare(const StructuringElementSquare& s);
        
        ~StructuringElementSquare();
    };

    /**
     * StructuringElementDisk is a disk shaped structuring element
     */
    class StructuringElementDisk : public StructuringElement2
    {
    public:
        using StructuringElement2::set;
        using StructuringElement2::operator=;
        using StructuringElement2::updateStride;
        using StructuringElement2::computeMinMax;

        /**
         * constructor to generate square structuting element
         * @param stride stride of the image on which the element will be used
         * @param r radius of the disk
         */
        StructuringElementDisk(float r= 1, int stride= 20000);
        
        StructuringElementDisk(const StructuringElementDisk& s);
        
        ~StructuringElementDisk();
    };
    
    class StructuringElementHorizontal : public StructuringElement2
    {
    public:
        using StructuringElement2::set;
        using StructuringElement2::operator=;
        using StructuringElement2::updateStride;
        using StructuringElement2::computeMinMax;

        /**
         * constructor to generate square structuring element
         * @param stride stride of the image on which the element will be used
         * @param size of a side of the square
         */
        StructuringElementHorizontal(int width= 1, int length= 5, int stride= 20000);
        
        StructuringElementHorizontal(const StructuringElementHorizontal& s);
        
        ~StructuringElementHorizontal();
    };
    
    class StructuringElementVertical : public StructuringElement2
    {
    public:
        using StructuringElement2::set;
        using StructuringElement2::operator=;
        using StructuringElement2::updateStride;
        using StructuringElement2::computeMinMax;

        /**
         * constructor to generate square structuring element
         * @param stride stride of the image on which the element will be used
         * @param size of a side of the square
         */
        StructuringElementVertical(int width= 1, int length= 5, int stride= 20000);
        
        StructuringElementVertical(const StructuringElementVertical& s);
        
        ~StructuringElementVertical();
    };

    class StructuringElementRing : public StructuringElement2
    {
    public:
        using StructuringElement2::set;
        using StructuringElement2::operator=;
        using StructuringElement2::updateStride;
        using StructuringElement2::computeMinMax;

        StructuringElementRing(int inner, int outer, int stride= 20000);
        
        StructuringElementRing(const StructuringElementRing& s);
        
        ~StructuringElementRing();
    };

    class StructuringElementDoubleRing: public StructuringElement2
    {
    public:
        using StructuringElement2::set;
        using StructuringElement2::operator=;
        using StructuringElement2::updateStride;
        using StructuringElement2::computeMinMax;

        StructuringElementDoubleRing(int inner1, int outer1, int inner2, int outer2, int stride= 20000);
        
        StructuringElementDoubleRing(const StructuringElementDoubleRing& s);
        
        ~StructuringElementDoubleRing();
    };

    class StructuringElementRectangle: public StructuringElement2
    {
    public:
        using StructuringElement2::set;
        using StructuringElement2::operator=;
        using StructuringElement2::translate;
        using StructuringElement2::rotate;
        using StructuringElement2::updateStride;
        using StructuringElement2::computeMinMax;

        StructuringElementRectangle(int width, int height, int stride= 4000);

        StructuringElementRectangle(const StructuringElementRectangle& s);

        ~StructuringElementRectangle();
    };

    class StructuringElementParallelStripes: public StructuringElement2
    {
    public:
        using StructuringElement2::set;
        using StructuringElement2::operator=;
        using StructuringElement2::translate;
        using StructuringElement2::rotate;
        using StructuringElement2::updateStride;
        using StructuringElement2::computeMinMax;

        StructuringElementParallelStripes(int width, int height, int distance, int stride= 4000);

        StructuringElementParallelStripes(const StructuringElementParallelStripes& s);

        ~StructuringElementParallelStripes();
    };

    class StructuringElementOneStripe: public StructuringElement2
    {
    public:
        using StructuringElement2::set;
        using StructuringElement2::operator=;
        using StructuringElement2::translate;
        using StructuringElement2::rotate;
        using StructuringElement2::updateStride;
        using StructuringElement2::computeMinMax;

        StructuringElementOneStripe(int width, int height, int distance, int stride= 4000);

        StructuringElementOneStripe(const StructuringElementOneStripe& s);

        ~StructuringElementOneStripe();
    };

    class StructuringElementConnectingOperator: public StructuringElement2
    {
    public:
        using StructuringElement2::set;
        using StructuringElement2::operator=;
        using StructuringElement2::translate;
        using StructuringElement2::rotate;
        using StructuringElement2::updateStride;
        using StructuringElement2::computeMinMax;

        StructuringElementConnectingOperator(int width, int height, int distance, int stride= 4000);

        StructuringElementConnectingOperator(const StructuringElementConnectingOperator& s);

        ~StructuringElementConnectingOperator();
    };

    class StructuringElementForIntensityObjects: public StructuringElement2
    {
    public:
        using StructuringElement2::set;
        using StructuringElement2::operator=;
        using StructuringElement2::operator+=;
        using StructuringElement2::translate;
        using StructuringElement2::rotate;
        using StructuringElement2::updateStride;
        using StructuringElement2::computeMinMax;

        StructuringElementForIntensityObjects(int w1, int h1, int w2, int h2, int d2, int stride= 4000);

        StructuringElementForIntensityObjects(const StructuringElementForIntensityObjects& s);

        ~StructuringElementForIntensityObjects();
    };
}
#endif	/* _STRUCTURINGELEMENTS_H */


/**
 * @file convexHull.h
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
 * Convex hull extraction algorithms.
 */

#ifndef _CONVEX_HULL_H_
#define _CONVEX_HULL_H_

#include <openipDS/PixelSequence1.h>
#include <openipDS/Region2.h>
#include <openipDS/Region2Set.h>

using namespace openip;

namespace openip
{	
    /** Gift wrapping algorithm for convex hull determination implemented 
    * @param ps input pixel sequence
    * @param ch output convex hull
    */
    
    class ConvexHull
    {
    public:
      ConvexHull(int mode= 0);
      
      ConvexHull(const ConvexHull& c);
      
      ~ConvexHull();
      
      int orientation(int pr, int pc, int qr, int qc, int rr, int rc);
      
      virtual void apply(Region2Set& input, Region2& output);
      
      virtual void apply(Region2& input, Region2& output);
      
      int mode;
    };
    
    class ConvexHullOfPoints
    {
    public:
      ConvexHullOfPoints(int mode= 0);
      
      ConvexHullOfPoints(const ConvexHullOfPoints& c);
      
      ~ConvexHullOfPoints();
      
      int orientation(float pr, float pc, float qr, float qc, float rr, float rc);
      
      virtual void apply(Vector<Vector<float> >& points, Vector<Vector<float> >& ch);
      
      float getArea();
      
      Vector<Vector<float> > convexHull;
      
      int mode;
    };
}

#endif

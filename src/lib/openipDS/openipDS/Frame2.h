/* 
 * File:   Frame.h
 * Author: gykovacs
 *
 * Created on July 18, 2011, 12:51 PM
 */

#ifndef FRAME_H
#define	FRAME_H

#include <openipDS/Vector2.h>

namespace openip
{
    class Frame2
    {
    public:
        Frame2();
        
        Frame2(const Frame2& f);
        
        ~Frame2();
        
        Frame2& operator=(const Frame2& f);
        
        void setFrame(Vector2<float> o, Vector2<float> y, Vector2<float> x);
        
        void getRealCoordinates(float y, float x, float& yr, float& xr);
        
        void getRealCoordinates(float y, float x, Vector2<float>& v);
        
        Vector2<float> origin;
        Vector2<float> baseX;
        Vector2<float> baseY;
    };
}

#endif	/* FRAME_H */


/* 
 * File:   Frame3.h
 * Author: gykovacs
 *
 * Created on July 18, 2011, 12:57 PM
 */

#ifndef FRAME3_H
#define	FRAME3_H

#include <openipDS/Vector3.h>

namespace openip
{
    class Frame3
    {
    public:
        Frame3();

        Frame3(const Frame3& f);

        ~Frame3();
        
        Frame3& operator=(const Frame3& f);

        void setFrame(Vector3<float> o, Vector3<float> z, Vector3<float> y, Vector3<float> x);
        
        void getRealCoordinates(float z, float y, float x, float& zr, float& yr, float& xr);

        void getRealCoordinates(float z, float y, float x, Vector3<float>& v);

        float getVolume();

        Vector3<float> origin;
        Vector3<float> baseX;
        Vector3<float> baseY;
        Vector3<float> baseZ;
    };
}

#endif	/* FRAME3_H */


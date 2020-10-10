#include <openipDS/Frame3.h>

namespace openip
{
    Frame3::Frame3()
    {
        origin= Vector3<float>(0,0,0);
        baseX= Vector3<float>(0,0,1);
        baseY= Vector3<float>(0,1,0);
        baseZ= Vector3<float>(1,0,0);
    }

    Frame3::Frame3(const Frame3& f)
    {
        origin= f.origin;
        baseX= f.baseX;
        baseY= f.baseY;
        baseZ= f.baseZ;
    }

    Frame3::~Frame3()
    {
    }

    Frame3& Frame3::operator=(const Frame3& f)
    {
        origin= f.origin;
        baseX= f.baseX;
        baseY= f.baseY;
        baseZ= f.baseZ;
        
        return *this;
    }

    void Frame3::setFrame(Vector3<float> o, Vector3<float> z, Vector3<float> y, Vector3<float> x)
    {
        this->origin= o;
        this->baseZ= z;
        this->baseY= y;
        this->baseX= x;
    }
    
    void Frame3::getRealCoordinates(float z, float y, float x, float& zr, float& yr, float& xr)
    {
        zr= origin.z() + z * baseZ.z() + y * baseY.z() + x * baseX.z();
        yr= origin.y() + z * baseZ.y() + y * baseY.y() + x * baseX.y();
        xr= origin.x() + z * baseZ.x() + y * baseY.x() + x * baseX.x();
    }

    void Frame3::getRealCoordinates(float z, float y, float x, Vector3<float>& v)
    {
        v= origin + baseZ * z + baseY * y + baseX * x;
    }

    float Frame3::getVolume()
    {
        return baseX.x() * baseY.y() * baseZ.z();
    }
}

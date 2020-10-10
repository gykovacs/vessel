#include <openipDS/Frame2.h>

namespace openip
{
    Frame2::Frame2()
    {
        this->origin= Vector2<float>(0,0);
        this->baseX= Vector2<float>(0,1);
        this->baseY= Vector2<float>(1,0);
    }
        
    Frame2::Frame2(const Frame2& f)
    {
        this->origin= f.origin;
        this->baseX= f.baseX;
        this->baseY= f.baseY;
    }

    Frame2::~Frame2()
    {
    }

    Frame2& Frame2::operator=(const Frame2& f)
    {
        this->origin= f.origin;
        this->baseX= f.baseX;
        this->baseY= f.baseY;
        
        return *this;
    }
    
    void Frame2::setFrame(Vector2<float> o, Vector2<float> y, Vector2<float> x)
    {
        this->origin= o;
        this->baseY= y;
        this->baseX= x;
    }
    
    void Frame2::getRealCoordinates(float y, float x, float& yr, float& xr)
    {
        xr= origin.x() + x * baseX.x() + y * baseY.x();
        yr= origin.y() + x * baseX.y() + y * baseY.y();
    }
        
    void Frame2::getRealCoordinates(float y, float x, Vector2<float>& v)
    {
        v= origin + baseX * x + baseY * y;
    }
}

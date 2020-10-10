#include <openipDS/Pixel.h>

openip::Pixel2::Pixel2()
{
}

openip::Pixel2::Pixel2(const openip::Pixel2& p_)
: openip::Vector2<int>(p_)
{
}

openip::Pixel2::Pixel2(int x, int y)
: openip::Vector2<int>(y, x)
{
}

openip::Pixel2::~Pixel2()
{
}

Pixel1 openip::Pixel2::getPixel1(int stride)
{
    return y()*stride + x();
}

int openip::Pixel2::r() const
{
    return this->x();
}

int openip::Pixel2::c() const
{
    return this->y();
}

int& openip::Pixel2::r()
{
    return this->x();
}

int& openip::Pixel2::c()
{
    return this->y();
}

std::ostream& openip::operator<<(std::ostream& o, const openip::Pixel2& p)
{
    o << "[PixelSet2 - size: ";
    o << p.x();
    o << ", ";
    o << p.y();
    o << "]";

    return o;
}

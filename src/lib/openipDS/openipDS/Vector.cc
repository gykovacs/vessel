#include <complex>

#include <openipDS/Vector.h>
#include <openipDS/Matrix.h>

namespace openip
{
    Border1::Border1(int lb, int rb, int bm)
    : leftBorder(lb), rightBorder(rb), borderMode(bm)
    {
    }

    Border1::Border1(const Border1& b)
    : leftBorder(b.leftBorder), rightBorder(b.rightBorder), borderMode(b.borderMode)
    {
    }

    Border1::Border1(Border1 a, Border1 b)
    {
        this->leftBorder= a.leftBorder > b.leftBorder ? a.leftBorder : b.leftBorder;
        this->rightBorder= a.rightBorder > b.rightBorder ? a.rightBorder : b.rightBorder;
        this->borderMode= a.borderMode;
    }

    Border1::~Border1()
    {
    }

    Border1& Border1::operator=(const Border1& b)
    {
        this->leftBorder= b.leftBorder;
        this->rightBorder= b.rightBorder;
        this->borderMode= b.borderMode;
        return *this;
    }

    Border1 maximizeBorders(Border1& a, Border1& b)
    {
        Border1 r;
        r.leftBorder= a.leftBorder > b.leftBorder ? a.leftBorder : b.leftBorder;
        r.rightBorder= a.rightBorder > b.rightBorder ? a.rightBorder : b.rightBorder;
        r.borderMode= a.borderMode;

        return r;
    }

    int Border1::getMin()
    {
        return -leftBorder;
    }

    int Border1::getMax()
    {
        return rightBorder;
    }
    
    void Border1::dump()
    {
        printf("%d %d %d\n", leftBorder, rightBorder, borderMode);
    }

}


//template class openip::Vector<bool>;
template class openip::Vector<unsigned char>;
//template class openip::Vector<short>;
//template class openip::Vector<int>;
//template class openip::Vector<float>;
template class openip::Vector<double>;
//template class openip::Vector<std::complex<float> >;
//template class openip::Vector<std::complex<double> >;

float openip::distance(float a, float b)
{
    return fabs(a - b);
}

float openip::distanceSquare(float a, float b)
{
    return openip::distance(a, b);
}

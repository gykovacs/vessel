#include <openipDS/Image.h>

namespace openip
{
    Border2::Border2(int tb, int bb, int lb, int rb, int bm)
    : topBorder(tb), bottomBorder(bb), leftBorder(lb), rightBorder(rb), borderMode(bm)
    {
    }

    Border2::Border2(const Border2& b)
    : topBorder(b.topBorder), bottomBorder(b.bottomBorder), leftBorder(b.leftBorder), rightBorder(b.rightBorder), borderMode(b.borderMode)
    {
    }

    Border2::Border2(Border2 a, Border2 b)
    {
        this->topBorder= a.topBorder > b.topBorder ? a.topBorder : b.topBorder;
        this->bottomBorder= a.bottomBorder > b.bottomBorder ? a.bottomBorder : b.bottomBorder;
        this->leftBorder= a.leftBorder > b.leftBorder ? a.leftBorder : b.leftBorder;
        this->rightBorder= a.rightBorder > b.rightBorder ? a.rightBorder : b.rightBorder;
        this->borderMode= a.borderMode;
    }

    Border2::~Border2()
    {
    }

    Border2& Border2::operator=(const Border2& b)
    {
        this->topBorder= b.topBorder;
        this->bottomBorder= b.bottomBorder;
        this->leftBorder= b.leftBorder;
        this->rightBorder= b.rightBorder;
        this->borderMode= b.borderMode;
        return *this;
    }

    Border2 maximizeBorders(Border2& a, Border2& b)
    {
        Border2 r;
        r.topBorder= a.topBorder > b.topBorder ? a.topBorder : b.topBorder;
        r.bottomBorder= a.bottomBorder > b.bottomBorder ? a.bottomBorder : b.bottomBorder;
        r.leftBorder= a.leftBorder > b.leftBorder ? a.leftBorder : b.leftBorder;
        r.rightBorder= a.rightBorder > b.rightBorder ? a.rightBorder : b.rightBorder;
        r.borderMode= a.borderMode;

        return r;
    }

    int Border2::getMin(int stride)
    {
        return -(stride + leftBorder + rightBorder)*topBorder;
    }

    int Border2::getMax(int stride)
    {
        return (stride + leftBorder + rightBorder)*bottomBorder;
    }

    /**
     * prints the description of the parameter Border2 object to the parameter output stream
     * @param o parameter output stream
     * @param b Border2 object to describe
     * @return reference to the parameter output stream
     */
    std::ostream& operator<<(std::ostream& o, const Border2& b)
    {
        o << "Border2 " << b.topBorder << " " << b.bottomBorder << " " << b.leftBorder << " " << b.rightBorder << " " << b.borderMode;
        return o;
    }

    void Border2::dump()
    {
        printf("%d %d %d %d %d\n", topBorder, bottomBorder, leftBorder, rightBorder, borderMode);
    }

}

template class openip::Image<unsigned char>;
//template class openip::Image<short>;
template class openip::Image<int>;
//template class openip::Image<float>;
template class openip::Image<double>;

#include <openipDS/Volume.h>
#include <openipDS/Stopper.h>

namespace openip
{
    Border3::Border3(int tb, int bb, int lb, int rb, int fb, int backb, int bm)
    : Border2(tb, bb, lb, rb, bm), frontBorder(fb), backBorder(backb)
    {
    }

    Border3::Border3(const Border3& b)
    : Border2(b), frontBorder(b.frontBorder), backBorder(b.backBorder)
    {
    }

    Border3::Border3(Border3 a, Border3 b)
    : Border2()
    {
        this->topBorder= a.topBorder > b.topBorder ? a.topBorder : b.topBorder;
        this->bottomBorder= a.bottomBorder > b.bottomBorder ? a.bottomBorder : b.bottomBorder;
        this->leftBorder= a.leftBorder > b.leftBorder ? a.leftBorder : b.leftBorder;
        this->rightBorder= a.rightBorder > b.rightBorder ? a.rightBorder : b.rightBorder;
        this->frontBorder= a.frontBorder > b.frontBorder ? a.frontBorder : b.frontBorder;
        this->backBorder= a.backBorder > b.backBorder ? a.backBorder : b.backBorder;
        this->borderMode= a.borderMode;
    }

    Border3::~Border3()
    {
    }

    Border3& Border3::operator=(const Border3& b)
    {
        this->topBorder= b.topBorder;
        this->bottomBorder= b.bottomBorder;
        this->leftBorder= b.leftBorder;
        this->rightBorder= b.rightBorder;
        this->frontBorder= b.frontBorder;
        this->backBorder= b.backBorder;
        this->borderMode= b.borderMode;
        
        return *this;
    }

    Border3 maximizeBorders(Border3& a, Border3& b)
    {
        Border3 r(a,b);

        return r;
    }

    void Border3::dump()
    {
        tprintf("tb: %d bb: %d lb: %d rb: %d fb: %d bb: %d bm: %d\n", topBorder, bottomBorder, leftBorder, rightBorder, frontBorder, bottomBorder, borderMode);
    }
}

#include <openipLL/ActiveContourElement.h>

namespace openip
{
    ActiveContourElement::ActiveContourElement()
    : Pixel2(), alpha(1), beta(1), gamma(1)
    {
        this->r()= 0;
        this->c()= 0;
    }

    ActiveContourElement::ActiveContourElement(const ActiveContourElement& ace)
    : Pixel2(ace), alpha(ace.alpha), beta(ace.beta), gamma(ace.gamma)
    {
        this->r()= ace.r();
        this->c()= ace.c();
    }

    ActiveContourElement::ActiveContourElement(int r, int c, float alpha, float beta, float gamma)
    : Pixel2()
    {
        this->r()= r;
        this->c()= c;
        this->alpha= alpha;
        this->beta= beta;
        this->gamma= gamma;
    }

    ActiveContourElement::~ActiveContourElement()
    {
    }
}

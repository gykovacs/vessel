#include <openipLL/ActiveContour.h>

namespace openip
{
    ActiveContour::ActiveContour()
    : std::vector<ActiveContourElement>()
    {
    }

    ActiveContour::ActiveContour(const ActiveContour& ac)
    : std::vector<ActiveContourElement>(ac)
    {
    }

    ActiveContour::~ActiveContour()
    {
    }

    ActiveContourElement& ActiveContour::operator()(int i)
    {
        while ( i < 0 )
            i+= this->size();
        while ( i >= (int)(this->size()))
            i-= this->size();
        
        return (*this)[i];
    }

    ActiveContourElement ActiveContour::operator()(int i) const
    {
        while ( i < 0 )
            i+= this->size();
        while ( i >= (int)(this->size()))
            i-= this->size();

        return (*this)[i];
    }

    void ActiveContour::add(int row, int column, float alpha, float beta, float gamma)
    {
        this->push_back(ActiveContourElement(row, column, alpha, beta, gamma));
    }

    float ActiveContour::energyInternal()
    {
        for ( unsigned int i= 0; i < this->size(); ++i )
        {
            /*e+= (alpha * sqrt(((*this)(i).r() - (*this)(i+1).r())*((*this)(i).r() - (*this)(i+1).r()) + ((*this)(i).c() - (*this)(i+1).c())*((*this)(i).c() - (*this)(i+1).c())))/2.0;
            e+= (beta * sqrt(((*this)(i-1).r() - 2*(*this)(i).r() + (*this)(i+1).r()) * ((*this)(i-1).r() - 2*(*this)(i).r() + (*this)(i+1).r()) + ((*this)(i-1).c() - 2*(*this)(i).c() + (*this)(i+1).c()) * ((*this)(i-1).c() - 2*(*this)(i).c() + (*this)(i+1).c())))/2.0;*/
        }
        return 0;
    }
}

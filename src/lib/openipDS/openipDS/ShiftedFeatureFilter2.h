#ifndef SHIFTEDFEATUREFILTER2_H
#define SHIFTEDFEATUREFILTER2_H

#include <openipDS/Filter2.h>
#include <openipDS/Feature2.h>
#include <openipDS/Filter2s.h>
#include <openipDS/MatchedFilter2s.h>
#include <openipDS/VectorTransform2s.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class ShiftedFilter2: public Filter2<INPUT, OUTPUT, WEIGHTS>
    {
    public:
        ShiftedFilter2(float angle, float dist, Filter2<INPUT, OUTPUT, WEIGHTS>* f);

        ShiftedFilter2(const ShiftedFilter2<INPUT, OUTPUT, WEIGHTS>& s);

        ~ShiftedFilter2();
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    ShiftedFilter2<INPUT, OUTPUT, WEIGHTS>::ShiftedFilter2(float angle, float dist, Filter2<INPUT, OUTPUT, WEIGHTS>* f)
        : Filter2<INPUT, OUTPUT, WEIGHTS>(*f)
    {
        float cc= floor(sin(angle) * dist + 0.5);
        float cr= floor(cos(angle) * dist + 0.5);

        float c= cr * f->stride + cc;

        for ( unsigned int i= 0; i < this->size(); ++i )
            (*this)(i).first-= c;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    ShiftedFilter2<INPUT, OUTPUT, WEIGHTS>::ShiftedFilter2(const ShiftedFilter2<INPUT, OUTPUT, WEIGHTS>& f)
        : Filter2<INPUT, OUTPUT, WEIGHTS>(f)
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    ShiftedFilter2<INPUT, OUTPUT, WEIGHTS>::~ShiftedFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class ShiftedCorrelationFilter2: public CorrelationFilter2<INPUT, OUTPUT, WEIGHTS>
    {
    public:
        ShiftedCorrelationFilter2(float angle, float dist, Filter2<INPUT, OUTPUT, WEIGHTS>* f);

        ShiftedCorrelationFilter2(const ShiftedCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>& s);

        ~ShiftedCorrelationFilter2();
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    ShiftedCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::ShiftedCorrelationFilter2(float angle, float dist, Filter2<INPUT, OUTPUT, WEIGHTS>* f)
        : CorrelationFilter2<INPUT, OUTPUT, WEIGHTS>(*f)
    {
        float cr= floor(sin(angle) * dist + 0.5);
        float cc= floor(cos(angle) * dist + 0.5);

        float c= cr * f->stride + cc;

        for ( unsigned int i= 0; i < this->size(); ++i )
            (*this)(i).first-= c;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    ShiftedCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::ShiftedCorrelationFilter2(const ShiftedCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>& f)
        : CorrelationFilter2<INPUT, OUTPUT, WEIGHTS>(f)
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    ShiftedCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::~ShiftedCorrelationFilter2()
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    class ShiftedCCorrelationFilter2: public CCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>
    {
    public:
        ShiftedCCorrelationFilter2(float angle, float dist, Filter2<INPUT, OUTPUT, WEIGHTS>* f);

        ShiftedCCorrelationFilter2(const ShiftedCCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>& s);

        ~ShiftedCCorrelationFilter2();
    };

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    ShiftedCCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::ShiftedCCorrelationFilter2(float angle, float dist, Filter2<INPUT, OUTPUT, WEIGHTS>* f)
        : CCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>(*f)
    {
        float cr= floor(sin(angle) * dist + 0.5);
        float cc= floor(cos(angle) * dist + 0.5);

        float c= cr * f->stride + cc;

        for ( unsigned int i= 0; i < this->size(); ++i )
            (*this)(i).first-= c;
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    ShiftedCCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::ShiftedCCorrelationFilter2(const ShiftedCCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>& f)
        : CCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>(f)
    {
    }

    template<typename INPUT, typename OUTPUT, typename WEIGHTS>
    ShiftedCCorrelationFilter2<INPUT, OUTPUT, WEIGHTS>::~ShiftedCCorrelationFilter2()
    {
    }
}

#endif // SHIFTEDFEATUREFILTER2_H

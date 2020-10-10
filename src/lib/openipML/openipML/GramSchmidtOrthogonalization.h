#ifndef GRAMSCHMIDTORTHOGONALIZATION_H
#define GRAMSCHMIDTORTHOGONALIZATION_H

#include <openipDS/Vector.h>

namespace openip
{
    /*template<typename INPUT>
    class GramSchmidtOrthogonalization
    {
    public:
        GramSchmidtOrthogonalization();

        GramSchmidtOrthogonalization(const GramSchmidtOrthogonalization& gso);

        ~GramSchmidtOrthogonalization();

        void apply(Vector<Vector<INPUT> >& input, Vector<Vector<INPUT> >& output);
    };

    template<typename INPUT>
    GramSchmidtOrthogonalization<INPUT>::GramSchmidtOrthogonalization()
    {
    }

    template<typename INPUT>
    GramSchmidtOrthogonalization<INPUT>::GramSchmidtOrthogonalization(const GramSchmidtOrthogonalization& gso)
    {
    }

    template<typename INPUT>
    GramSchmidtOrthogonalization<INPUT>::~GramSchmidtOrthogonalization()
    {
    }

    template<typename INPUT>
    void GramSchmidtOrthogonalization<INPUT>::apply(Vector<Vector<INPUT> >& input, Vector<Vector<INPUT> >& output)
    {
        output.resize(input.size());
        output(0)= input(0);

        for ( int i= 1; i < input.size(); ++i )
        {
            Vector<INPUT> u= input(i);
            for ( int j= 0; j < i; ++j )
            {
                u-= output(j).proj(input(i));
            }
            output(i)= u;
        }

        for ( int i= 0; i < output.size(); ++i )
        {
            if ( output(i).getNorm2() != output(i).getNorm2() )
            {
                printf("!!!!!!!!!!!\n");
                fflush(stdout);
            }
            output(i)/= output(i).getNorm2();
        }
    }*/
}

#endif // GRAMSCHMIDTORTHOGONALIZATION_H

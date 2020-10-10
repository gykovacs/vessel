/* 
 * File:   VectorTransform2.h
 * Author: gykovacs
 *
 * Created on 2011. március 18., 7:59
 */

#ifndef VECTORTRANSFORM2_H
#define	VECTORTRANSFORM2_H

#include <openipDS/Image.h>
#include <openipDS/ImageVector.h>
#include <string>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class VectorTransform2
    {
    public:
        VectorTransform2();

        VectorTransform2(const VectorTransform2& vt);

        virtual ~VectorTransform2();

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int begin= 0, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, int k, Vector<OUTPUT>& result, int n, Image<unsigned char>* support);

        virtual int dimensions();

        virtual Border2 getProposedBorder();
        
        virtual std::string getDescriptor(char space= ' ', int i= 0);

        std::string descriptor;
    };

    template<typename INPUT, typename OUTPUT>
    VectorTransform2<INPUT, OUTPUT>::VectorTransform2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    VectorTransform2<INPUT, OUTPUT>::VectorTransform2(const VectorTransform2& vt)
    {
        this->descriptor= vt.descriptor;
    }

    template<typename INPUT, typename OUTPUT>
    VectorTransform2<INPUT, OUTPUT>::~VectorTransform2()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void VectorTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& , ImageVector<OUTPUT>& , int , Image<unsigned char>* , Image<unsigned char>* )
    {
    }

    template<typename INPUT, typename OUTPUT>
    void VectorTransform2<INPUT, OUTPUT>::apply(Image<INPUT>&, Image<OUTPUT>&, Image<unsigned char>*, Image<unsigned char>*)
    {
    }

    template<typename INPUT, typename OUTPUT>
    void VectorTransform2<INPUT, OUTPUT>::apply(Image<INPUT>& /*input*/, int /*k*/, Vector<OUTPUT>& /*result*/, int /*n*/, Image<unsigned char>* /*support*/)
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 VectorTransform2<INPUT, OUTPUT>::getProposedBorder()
    {
        //printf("getproposedborder vectortransform2\n"); fflush(stdout);
        return Border2();
    }

    template<typename INPUT, typename OUTPUT>
    int VectorTransform2<INPUT, OUTPUT>::dimensions()
    {
        return 1;
    }

    template<typename INPUT, typename OUTPUT>
    std::string VectorTransform2<INPUT, OUTPUT>::getDescriptor(char space, int i)
    {
        if ( i == 0 )
        {
            std::string a= this->descriptor;
            for ( unsigned int i= 0; i < a.size(); ++i )
                if ( a[i] == ' ' )
                    a[i]= space;
            return a;
        }
        else
            return std::string("padding");
    }


}
#endif	/* VECTORTRANSFORM2_H */


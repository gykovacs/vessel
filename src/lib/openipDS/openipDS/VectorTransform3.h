/* 
 * File:   VectorTransform3.h
 * Author: gykovacs
 *
 * Created on July 20, 2011, 3:43 PM
 */

#ifndef VECTORTRANSFORM3_H
#define	VECTORTRANSFORM3_H

#include <openipDS/Volume.h>
#include <openipDS/VolumeVector.h>
#include <string>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class VectorTransform3
    {
    public:
        VectorTransform3();

        VectorTransform3(const VectorTransform3& vt);

        virtual ~VectorTransform3();

        virtual void apply(Volume<INPUT>& input, VolumeVector<OUTPUT>& output, int begin= 0, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        virtual int dimensions();

        virtual Border3 getProposedBorder();
        
        virtual std::string getDescriptor(char space= ' ', int i= 0);

        std::string descriptor;
    };

    template<typename INPUT, typename OUTPUT>
    VectorTransform3<INPUT, OUTPUT>::VectorTransform3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    VectorTransform3<INPUT, OUTPUT>::VectorTransform3(const VectorTransform3& vt)
    {
        this->descriptor= vt.descriptor;
    }

    template<typename INPUT, typename OUTPUT>
    VectorTransform3<INPUT, OUTPUT>::~VectorTransform3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void VectorTransform3<INPUT, OUTPUT>::apply(Volume<INPUT>& , VolumeVector<OUTPUT>& , int , Volume<unsigned char>* , Volume<unsigned char>* )
    {
    }

    template<typename INPUT, typename OUTPUT>
    void VectorTransform3<INPUT, OUTPUT>::apply(Volume<INPUT>&, Volume<OUTPUT>&, Volume<unsigned char>*, Volume<unsigned char>*)
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 VectorTransform3<INPUT, OUTPUT>::getProposedBorder()
    {
        //printf("getproposedborder VectorTransform3\n"); fflush(stdout);
        return Border3();
    }

    template<typename INPUT, typename OUTPUT>
    int VectorTransform3<INPUT, OUTPUT>::dimensions()
    {
        return 1;
    }

    template<typename INPUT, typename OUTPUT>
    std::string VectorTransform3<INPUT, OUTPUT>::getDescriptor(char space, int i)
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

#endif	/* VECTORTRANSFORM3_H */


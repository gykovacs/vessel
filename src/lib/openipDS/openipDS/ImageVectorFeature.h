/* 
 * File:   ImageVectorFeature.h
 * Author: gykovacs
 *
 * Created on 2011. április 11., 13:05
 */

#ifndef IMAGEVECTORFEATURE_H
#define	IMAGEVECTORFEATURE_H

#include <openipDS/Image.h>

#include <string>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class ImageVectorFeature
    {
    public:
        ImageVectorFeature();

        ImageVectorFeature(const ImageVectorFeature& vt);

        ~ImageVectorFeature();

        virtual void apply(Image<INPUT>& input, Vector<OUTPUT>& output, int begin= 0, Image<unsigned char>* roi= NULL);

        virtual void apply(Image<INPUT>& input, ImageVector<OUTPUT>& output, int begin= 0, Image<unsigned char>* roi= NULL);

        virtual int dimensions();

        virtual Border2 getProposedBorder();

        virtual std::string getDescriptor(char space= ' ', int i= 0);

        std::string descriptor;
    };

    template<typename INPUT, typename OUTPUT>
    ImageVectorFeature<INPUT, OUTPUT>::ImageVectorFeature()
    {
    }

    template<typename INPUT, typename OUTPUT>
    ImageVectorFeature<INPUT, OUTPUT>::ImageVectorFeature(const ImageVectorFeature& vt)
    {
        this->descriptor= vt.descriptor;
    }

    template<typename INPUT, typename OUTPUT>
    ImageVectorFeature<INPUT, OUTPUT>::~ImageVectorFeature()
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ImageVectorFeature<INPUT, OUTPUT>::apply(Image<INPUT>& , ImageVector<OUTPUT>& , int , Image<unsigned char>*)
    {
    }

    template<typename INPUT, typename OUTPUT>
    void ImageVectorFeature<INPUT, OUTPUT>::apply(Image<INPUT>& , Vector<OUTPUT>& , int , Image<unsigned char>*)
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 ImageVectorFeature<INPUT, OUTPUT>::getProposedBorder()
    {
        //printf("getproposedborder imagevectorfeature\n"); fflush(stdout);
        return Border2();
    }

    template<typename INPUT, typename OUTPUT>
    int ImageVectorFeature<INPUT, OUTPUT>::dimensions()
    {
        return 1;
    }

    template<typename INPUT, typename OUTPUT>
    std::string ImageVectorFeature<INPUT, OUTPUT>::getDescriptor(char space, int i)
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
#endif	/* IMAGEVECTORFEATURE_H */


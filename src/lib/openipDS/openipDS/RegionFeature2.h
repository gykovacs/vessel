/* 
 * File:   RegionFeature.h
 * Author: gykovacs
 *
 * Created on 2011. március 4., 18:32
 */

#ifndef REGIONFEATURE2_H
#define	REGIONFEATURE2_H

#include <openipDS/Region2.h>
#include <openipDS/Image.h>
#include <openipLL/thresholding.h>

#include <string>

#define DEBUG_FEATURES 0

namespace openip
{
    template<typename INPUT>
    class RegionFeature2
    {
    public:
        RegionFeature2();

        RegionFeature2(int th);

        RegionFeature2(const RegionFeature2& r);

        ~RegionFeature2();

        virtual float apply(Image<INPUT>& input, Region2& r);

        virtual void apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output);
        
        virtual std::string getDescriptor(char space= ' ', int i= 0);
        
        virtual bool requiresTransform();

        virtual int dimensions();

        template<typename T>
        int debugFeatures(Image<T>& image);

        VectorTransform2<INPUT, INPUT>* transform;
        Transform2<INPUT, unsigned char>* tm;
        
        std::string descriptor;
        bool reqTransform;
    };

    template<typename INPUT> template<typename T>
    int RegionFeature2<INPUT>::debugFeatures(Image<T>& 
#if DEBUG_FEATURES
    image
#endif
    )
    {
#if DEBUG_FEATURES
        Image<T> tmp;
        tmp= image;
        tmp.normalize(0, 255);
        char filename[1000];
        sprintf(filename, "%s.bmp", getDescriptor('_').c_str());
        writeImage(filename, tmp);
#endif
        return 0;
    }

    template<typename INPUT>
    RegionFeature2<INPUT>::RegionFeature2()
    {
        transform= NULL;
        tm= NULL;
    }

    template<typename INPUT>
    RegionFeature2<INPUT>::RegionFeature2(int th)
    {
        transform= NULL;
        tm= NULL;
        if ( th == 1 )
            tm= new OtsuThresholding2<INPUT, unsigned char>(256);
        else if ( th == 2 )
            tm= new RiddlerThresholding2<INPUT, unsigned char>(256);
        else if ( th == 3 )
            tm= new YanniThresholding2<INPUT, unsigned char>(256);
        else if ( th == 4 )
            tm= new LloydThresholding2<INPUT, unsigned char>(256);
        else if ( th == 5 )
            tm= new KittlerThresholding2<INPUT, unsigned char>(256);
        else if ( th == 6 )
            tm= new Jawahar1Thresholding2<INPUT, unsigned char>(2.0, 256);
        else if ( th == 7 )
            tm= new Jawahar2Thresholding2<INPUT, unsigned char>(2.0, 256);
        else if ( th == 8 )
            tm= new KapurThresholding2<INPUT, unsigned char>(256);
        else if ( th == 9 )
            tm= new YenThresholding2<INPUT, unsigned char>(256);
        else if ( th == 10 )
            tm= new SahooThresholding2<INPUT, unsigned char>(256);
        else if ( th == 11 )
            tm= new PunAThresholding2<INPUT, unsigned char>(256);
        else if ( th == 12 )
            tm= new PunBThresholding2<INPUT, unsigned char>(256);
        else if ( th == 13 )
            tm= new LiThresholding2<INPUT, unsigned char>(256);
        else if ( th == 14 )
            tm= new BrinkThresholding2<INPUT, unsigned char>(256);
        else if ( th == 15 )
            tm= new PalThresholding2<INPUT, unsigned char>(256);
        else if ( th == 16 )
            tm= new ShanbagThresholding2<INPUT, unsigned char>(256);
        else if ( th == 17 )
            tm= new TsaiThresholding2<INPUT, unsigned char>(256);
        else if ( th == 18 )
            tm= new HerzThresholding2<INPUT, unsigned char>(256);
    }

    template<typename INPUT>
    RegionFeature2<INPUT>::RegionFeature2(const RegionFeature2& r)
    {
        this->descriptor= r.descriptor;
        /*this->transform= r.transform;
        this->tm= r.tm;*/
    }

    template<typename INPUT>
    RegionFeature2<INPUT>::~RegionFeature2()
    {
        if ( tm )
            delete tm;
        if ( transform )
            delete transform;
    }

    template<typename INPUT>
    bool RegionFeature2<INPUT>::requiresTransform()
    {
        return reqTransform;
    }
    
    template<typename INPUT>
    float RegionFeature2<INPUT>::apply(Image<INPUT>& /*input*/, Region2& /*r*/)
    {
        return 0.0;
    }

    template<typename INPUT>
    void RegionFeature2<INPUT>::apply(Image<INPUT>& input, Region2& r, unsigned int actual, Vector<float>& output)
    {
        if ( dimensions() == 1 )
            output(actual)= apply(input, r);
    }
    
    template<typename INPUT>
    std::string RegionFeature2<INPUT>::getDescriptor(char space, int i)
    {
        if ( i == 0 )
        {
            std::string a= this->descriptor;
            if ( transform != NULL )
                a+= std::string(" transform ") + transform->getDescriptor(space);
            for ( unsigned int i= 0; i < a.size(); ++i )
                if ( a[i] == ' ' )
                    a[i]= space;
            return a;
        }
        else
            return std::string("padding");
    }

    template<typename INPUT>
    int RegionFeature2<INPUT>::dimensions()
    {
        return 1;
    }
}

#endif	/* REGIONFEATURE_H */


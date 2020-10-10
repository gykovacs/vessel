#ifndef MORPHOLOGY3_H
#define MORPHOLOGY3_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>

#include <openipDS/Image.h>
#include <openipDS/PixelSet1.h>
#include <openipDS/PixelSequence1.h>
#include <openipDS/StructuringElement2s.h>
#include <openipDS/StructuringElement3s.h>
#include <openipDS/Filter2s.h>
#include <openipDS/StatisticalFeature2s.h>
#include <openipDS/PriorityQueue.h>
#include <openipDS/Volume.h>
#include <openipDS/Transform3.h>

#include <openipLL/ComponentLabeling.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    void grayscaleErode(Volume<INPUT>* input, Volume<OUTPUT>* output, StructuringElement3& se, Volume<unsigned char>* mask= NULL)
    {
    int start= - se.min;
    int end= input->n - se.max;

    int i;

    if ( mask == NULL )
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
        {
            (*output)(i)= se.getMin(*input, i);
        }
    }
    else
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
            if ( (*mask)(i) > 0 )
            {
                INPUT min= std::numeric_limits<INPUT>::max();
                for ( StructuringElement3::iterator sit= se.begin(); sit != se.end(); ++sit )
                    if ( (*mask)(i + *sit) > 0 && (*input)(i + *sit) < min )
                        min= (*input)(i + *sit);
                (*output)(i)= min;
            }
    }
    }

    template<typename INPUT, typename OUTPUT>
    void grayscaleDilate(Volume<INPUT>* input, Volume<OUTPUT>* output, StructuringElement3& se, Volume<unsigned char>* mask= NULL)
    {
    int start= - se.min;
    int end= input->n - se.max;

    int i;

    if ( mask == NULL )
    {
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
        {
            (*output)(i)= se.getMax(*input, i);
        }
    }
    else
    {
      *output= *input;
        #pragma omp parallel for
        for ( i= start; i < end; ++i )
            if ( (*mask)(i) > 0 )
            {
                INPUT max= -std::numeric_limits<INPUT>::max();
                for ( StructuringElement3::iterator sit= se.begin(); sit != se.end(); ++sit )
                    if ( (*mask)(i + *sit) > 0 && (*input)(i + *sit) > max )
                        max= (*input)(i + *sit);
                (*output)(i)= max;
            }
    }
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleDilate3: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleDilate3(int radius);

        GrayscaleDilate3(const GrayscaleDilate3& g);

        ~GrayscaleDilate3();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleDilate3<INPUT, OUTPUT>::GrayscaleDilate3(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleDilate3 " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleDilate3<INPUT, OUTPUT>::GrayscaleDilate3(const GrayscaleDilate3& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleDilate3<INPUT, OUTPUT>::~GrayscaleDilate3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleDilate3<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleDilate3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        StructuringElementSphere sed(radius);
        sed.updateStride(input.columns, input.sliceSize);
        grayscaleDilate(&input, &output, sed, roi);
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleErode3: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleErode3(int radius);

        GrayscaleErode3(const GrayscaleErode3& g);

        ~GrayscaleErode3();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleErode3<INPUT, OUTPUT>::GrayscaleErode3(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleErode3 " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleErode3<INPUT, OUTPUT>::GrayscaleErode3(const GrayscaleErode3& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleErode3<INPUT, OUTPUT>::~GrayscaleErode3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleErode3<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleErode3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        StructuringElementSphere sed(radius);
        sed.updateStride(input.columns, input.sliceSize);
        grayscaleErode(&input, &output, sed, roi);
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleOpen3: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleOpen3(int radius);

        GrayscaleOpen3(const GrayscaleOpen3& g);

        ~GrayscaleOpen3();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleOpen3<INPUT, OUTPUT>::GrayscaleOpen3(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleOpen3 " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleOpen3<INPUT, OUTPUT>::GrayscaleOpen3(const GrayscaleOpen3& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleOpen3<INPUT, OUTPUT>::~GrayscaleOpen3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleOpen3<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleOpen3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        StructuringElementSphere sed(radius);
        sed.updateStride(input.columns, input.sliceSize);
        Volume<INPUT> tmp;
        tmp.resizeVolume(input);

        grayscaleDilate(&input, &tmp, sed, roi);
        grayscaleErode(&tmp, &output, sed, roi);
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleClose3: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleClose3(int radius);

        GrayscaleClose3(const GrayscaleClose3& g);

        ~GrayscaleClose3();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleClose3<INPUT, OUTPUT>::GrayscaleClose3(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleClose3 " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleClose3<INPUT, OUTPUT>::GrayscaleClose3(const GrayscaleClose3& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleClose3<INPUT, OUTPUT>::~GrayscaleClose3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleClose3<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleClose3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        StructuringElementSphere sed(radius);
        sed.updateStride(input.columns, input.sliceSize);
        Volume<INPUT> tmp;
        tmp.resizeVolume(input);

        grayscaleErode(&input, &tmp, sed, roi);
        grayscaleDilate(&tmp, &output, sed, roi);
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleBrightReconstruction3: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleBrightReconstruction3(int radius);

        GrayscaleBrightReconstruction3(const GrayscaleBrightReconstruction3& g);

        ~GrayscaleBrightReconstruction3();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconstruction3<INPUT, OUTPUT>::GrayscaleBrightReconstruction3(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleBrightReconstruction3 " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconstruction3<INPUT, OUTPUT>::GrayscaleBrightReconstruction3(const GrayscaleBrightReconstruction3& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconstruction3<INPUT, OUTPUT>::~GrayscaleBrightReconstruction3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleBrightReconstruction3<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleBrightReconstruction3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        GrayscaleDilate3<INPUT, OUTPUT> gd(radius);
        GrayscaleErode3<INPUT, OUTPUT> ge(1);
        Volume<OUTPUT> tmp;
        Volume<OUTPUT> tmp2;
        Volume<OUTPUT> tmp3;
        tmp.resizeVolume(input);
        tmp2.resizeVolume(input);
        tmp3.resizeVolume(input);

        gd.apply(input, tmp, roi, support);

        //writeImage("dilated.png", tmp);
        int diff;

        do
        {
            ge.apply(tmp, tmp2, roi, support);
            for ( unsigned int i= 0; i < input.n; ++i )
                tmp3(i)= input(i) > tmp2(i) ? input(i) : tmp2(i);

            diff= 0;
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( tmp3(i) != output(i) )
                    ++diff;
            output= tmp3;
            tmp= tmp3;
            //printf("%d\n", diff);
        }while ( diff > 0 );
    }

    template<typename INPUT, typename OUTPUT>
    class GrayscaleDarkReconstruction3: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleDarkReconstruction3(int radius);

        GrayscaleDarkReconstruction3(const GrayscaleDarkReconstruction3& g);

        ~GrayscaleDarkReconstruction3();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleDarkReconstruction3<INPUT, OUTPUT>::GrayscaleDarkReconstruction3(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleDarkReconstruction3 " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleDarkReconstruction3<INPUT, OUTPUT>::GrayscaleDarkReconstruction3(const GrayscaleDarkReconstruction3& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleDarkReconstruction3<INPUT, OUTPUT>::~GrayscaleDarkReconstruction3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleDarkReconstruction3<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleDarkReconstruction3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        GrayscaleDilate3<INPUT, OUTPUT> gd(1);
        GrayscaleErode3<INPUT, OUTPUT> ge(radius);
        Volume<OUTPUT> tmp;
        Volume<OUTPUT> tmp2;
        Volume<OUTPUT> tmp3;
        tmp.resizeVolume(input);
        tmp2.resizeVolume(input);
        tmp3.resizeVolume(input);

        ge.apply(input, tmp, roi, support);

        //writeImage("dilated.png", tmp);
        int diff;

        do
        {
            gd.apply(tmp, tmp2, roi, support);
            for ( unsigned int i= 0; i < input.n; ++i )
                tmp3(i)= input(i) < tmp2(i) ? input(i) : tmp2(i);

            diff= 0;
            for ( unsigned int i= 0; i < input.n; ++i )
                if ( tmp3(i) != output(i) )
                    ++diff;
            output= tmp3;
            tmp= tmp3;
            //printf("%d\n", diff);
        }while ( diff > 0 );
    }
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleAxialDilate: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleAxialDilate(int radius);

        GrayscaleAxialDilate(const GrayscaleAxialDilate& g);

        ~GrayscaleAxialDilate();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleAxialDilate<INPUT, OUTPUT>::GrayscaleAxialDilate(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleAxialDilate " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleAxialDilate<INPUT, OUTPUT>::GrayscaleAxialDilate(const GrayscaleAxialDilate& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleAxialDilate<INPUT, OUTPUT>::~GrayscaleAxialDilate()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleAxialDilate<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleAxialDilate<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        StructuringElementAxial sea(radius, input.columns, input.sliceSize);
	grayscaleDilate(&input, &output, sea, roi);
    }
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleAxialErode: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleAxialErode(int radius);

        GrayscaleAxialErode(const GrayscaleAxialErode& g);

        ~GrayscaleAxialErode();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleAxialErode<INPUT, OUTPUT>::GrayscaleAxialErode(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleAxialErode " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleAxialErode<INPUT, OUTPUT>::GrayscaleAxialErode(const GrayscaleAxialErode& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleAxialErode<INPUT, OUTPUT>::~GrayscaleAxialErode()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleAxialErode<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleAxialErode<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        StructuringElementAxial sea(radius, input.columns, input.sliceSize);
	grayscaleErode(&input, &output, sea, roi);
    }
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleCSDilate: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleCSDilate(int radius);

        GrayscaleCSDilate(const GrayscaleCSDilate& g);

        ~GrayscaleCSDilate();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSDilate<INPUT, OUTPUT>::GrayscaleCSDilate(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleCSDilate " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSDilate<INPUT, OUTPUT>::GrayscaleCSDilate(const GrayscaleCSDilate& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSDilate<INPUT, OUTPUT>::~GrayscaleCSDilate()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleCSDilate<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleCSDilate<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
      //printf("cs dilate\n");
        StructuringElementCS sea(radius, input.columns, input.sliceSize);
	grayscaleDilate(&input, &output, sea, roi);
    }
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleCSErode: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleCSErode(int radius);

        GrayscaleCSErode(const GrayscaleCSErode& g);

        ~GrayscaleCSErode();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSErode<INPUT, OUTPUT>::GrayscaleCSErode(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleCSErode " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSErode<INPUT, OUTPUT>::GrayscaleCSErode(const GrayscaleCSErode& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSErode<INPUT, OUTPUT>::~GrayscaleCSErode()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleCSErode<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleCSErode<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        StructuringElementCS sea(radius, input.columns, input.sliceSize);
	grayscaleErode(&input, &output, sea, roi);
    }
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleAxialBrightRecon: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleAxialBrightRecon(int radius);

        GrayscaleAxialBrightRecon(const GrayscaleAxialBrightRecon& g);

        ~GrayscaleAxialBrightRecon();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleAxialBrightRecon<INPUT, OUTPUT>::GrayscaleAxialBrightRecon(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleAxialBrightRecon " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleAxialBrightRecon<INPUT, OUTPUT>::GrayscaleAxialBrightRecon(const GrayscaleAxialBrightRecon& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleAxialBrightRecon<INPUT, OUTPUT>::~GrayscaleAxialBrightRecon()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleAxialBrightRecon<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleAxialBrightRecon<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        StructuringElementAxial sea(radius, input.columns, input.sliceSize);
	StructuringElementAxial sea2(radius+2, input.columns, input.sliceSize);
	Volume<INPUT> tmp;
	tmp.resizeVolume(input);
	
	grayscaleErode(&input, &tmp, sea, roi);
	grayscaleDilate(&tmp, &output, sea2, roi);
	
	for ( unsigned int i= 0; i < input.n; ++i )
	  if ( input(i) && output(i) )
	    output(i)= input(i);
	  else
	    output(i)= 0;
    }
    
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleAxialDarkRecon: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleAxialDarkRecon(int radius);

        GrayscaleAxialDarkRecon(const GrayscaleAxialDarkRecon& g);

        ~GrayscaleAxialDarkRecon();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleAxialDarkRecon<INPUT, OUTPUT>::GrayscaleAxialDarkRecon(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleAxialDarkRecon " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleAxialDarkRecon<INPUT, OUTPUT>::GrayscaleAxialDarkRecon(const GrayscaleAxialDarkRecon& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleAxialDarkRecon<INPUT, OUTPUT>::~GrayscaleAxialDarkRecon()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleAxialDarkRecon<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleAxialDarkRecon<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        StructuringElementAxial sea(radius, input.columns, input.sliceSize);
	StructuringElementAxial sea2(radius+2, input.columns, input.sliceSize);
	Volume<INPUT> tmp;
	tmp.resizeVolume(input);
	
	grayscaleDilate(&input, &tmp, sea, roi);
	grayscaleErode(&tmp, &output, sea2, roi);
	
	for ( int i= 0; i < input.n; ++i )
	  if ( input(i) || output(i) )
	    output(i)= input(i) > output(i) ? input(i) : output(i);
	  else
	    output(i)= 0;
    }
    
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleCSBrightRecon: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleCSBrightRecon(int radius);

        GrayscaleCSBrightRecon(const GrayscaleCSBrightRecon& g);

        ~GrayscaleCSBrightRecon();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSBrightRecon<INPUT, OUTPUT>::GrayscaleCSBrightRecon(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleCSBrightRecon " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSBrightRecon<INPUT, OUTPUT>::GrayscaleCSBrightRecon(const GrayscaleCSBrightRecon& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSBrightRecon<INPUT, OUTPUT>::~GrayscaleCSBrightRecon()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleCSBrightRecon<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleCSBrightRecon<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        StructuringElementCS sea(radius, input.columns, input.sliceSize);
	StructuringElementCS sea2(radius+2, input.columns, input.sliceSize);
	Volume<INPUT> tmp;
	tmp.resizeVolume(input);
	
	grayscaleErode(&input, &tmp, sea, roi);
	grayscaleDilate(&tmp, &output, sea2, roi);
	
	for ( int i= 0; i < input.n; ++i )
	  if ( !roi || (*roi)(i) )
	  {
	    if ( input(i) && output(i) )
	      output(i)= input(i);
	    else
	      output(i)= 0;
	  }
	  else
	    output(i)= 0;
    }
    
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleCSDarkRecon: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleCSDarkRecon(int radius);

        GrayscaleCSDarkRecon(const GrayscaleCSDarkRecon& g);

        ~GrayscaleCSDarkRecon();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSDarkRecon<INPUT, OUTPUT>::GrayscaleCSDarkRecon(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleCSDarkRecon " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSDarkRecon<INPUT, OUTPUT>::GrayscaleCSDarkRecon(const GrayscaleCSDarkRecon& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSDarkRecon<INPUT, OUTPUT>::~GrayscaleCSDarkRecon()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleCSDarkRecon<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleCSDarkRecon<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        StructuringElementCS sea(radius, input.columns, input.sliceSize);
	StructuringElementCS sea2(radius+2, input.columns, input.sliceSize);
	Volume<INPUT> tmp;
	tmp.resizeVolume(input);
	
	grayscaleDilate(&input, &tmp, sea, roi);
	grayscaleErode(&tmp, &output, sea2, roi);
	
	for ( unsigned int i= 0; i < input.n; ++i )
	  if ( !roi || (*roi)(i) )
	  {
	    if ( input(i) || output(i) )
	      output(i)= input(i) > output(i) ? input(i) : output(i);
	    else
	      output(i)= 0;
	  }
	  else
	    output(i)= 0;
    }
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleBrightReconLargest3: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleBrightReconLargest3(int radius);

        GrayscaleBrightReconLargest3(const GrayscaleBrightReconLargest3& g);

        ~GrayscaleBrightReconLargest3();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconLargest3<INPUT, OUTPUT>::GrayscaleBrightReconLargest3(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleBrightReconLargest3 " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconLargest3<INPUT, OUTPUT>::GrayscaleBrightReconLargest3(const GrayscaleBrightReconLargest3& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconLargest3<INPUT, OUTPUT>::~GrayscaleBrightReconLargest3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleBrightReconLargest3<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleBrightReconLargest3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        StructuringElementCS sea(radius, input.columns, input.sliceSize);
	StructuringElementCS sea2(radius+2, input.columns, input.sliceSize);
	Volume<INPUT> tmp;
	Volume<unsigned char> binary;
	tmp.resizeVolume(input);
	binary.resizeVolume(input);
	
	grayscaleErode(&input, &tmp, sea, roi);
	
	for ( int i= 0; i < tmp.n; ++i )
	  if ( tmp(i) )
	    binary(i)= 1;
	  else
	    binary(i)= 0;
	
	ExtractRegions3 er;
	Region3Set results;
	er.apply(binary, results);
	
	int maxIdx= 0;
	for ( int i= 0; i < results.size(); ++i )
	  if ( results(i).size() > results(maxIdx).size() )
	  {
	    maxIdx= i;
	  }
	  
	for ( int i= 0; i < results.size(); ++i )
	  if ( i != maxIdx )
	  {
	    for ( int j= 0; j < results(i).size(); ++j )
	      tmp(results(i)(j))= 0;
	  }
	  
	grayscaleDilate(&tmp, &output, sea2, roi);
	
	for ( int i= 0; i < input.n; ++i )
	  if ( input(i) && output(i) )
	    output(i)= input(i) > output(i) ? input(i) : output(i);
	  else
	    output(i)= 0;
    }
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleBrightReconAxialLargest3: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleBrightReconAxialLargest3(int radius1, int radius2= -1);

        GrayscaleBrightReconAxialLargest3(const GrayscaleBrightReconAxialLargest3& g);

        ~GrayscaleBrightReconAxialLargest3();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius1;
	int radius2;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconAxialLargest3<INPUT, OUTPUT>::GrayscaleBrightReconAxialLargest3(int radius1, int radius2)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius1= radius1;
	if ( radius2 == -1 )
	  radius2= radius1;
	this->radius2= radius2;
        std::stringstream ss;
        ss << "GrayscaleBrightReconAxialLargest3 " << radius1 << " " << radius2;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconAxialLargest3<INPUT, OUTPUT>::GrayscaleBrightReconAxialLargest3(const GrayscaleBrightReconAxialLargest3& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius1= g.radius1;
	this->radius2= g.radius2;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleBrightReconAxialLargest3<INPUT, OUTPUT>::~GrayscaleBrightReconAxialLargest3()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleBrightReconAxialLargest3<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius1, radius1, radius1, radius1, radius1, radius1);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleBrightReconAxialLargest3<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* support)
    {
        StructuringElementAxial sea(radius1, input.columns, input.sliceSize);
	StructuringElementCS sea2(radius2, input.columns, input.sliceSize);
	Volume<INPUT> tmp;
	Volume<unsigned char> binary;
	tmp.resizeVolume(input);
	binary.resizeVolume(input);
	
	grayscaleErode(&input, &tmp, sea, roi);
	
	for ( int i= 0; i < tmp.n; ++i )
	  if ( tmp(i) )
	    binary(i)= 1;
	  else
	    binary(i)= 0;

	ExtractRegions3 er;
	Region3Set results;
	er.apply(binary, results);
	
	int maxIdx= 0;
	int maxLength= 0;
	for ( int i= 0; i < results.size(); ++i )
	  if ( results(i).lengthSlices() > maxLength )
	  {
	    maxIdx= i;
	    maxLength= results(i).lengthSlices();
	  }
	  
	for ( int i= 0; i < results.size(); ++i )
	  if ( i != maxIdx )
	  {
	    for ( int j= 0; j < results(i).size(); ++j )
	      tmp(results(i)(j))= 0;
	  }
	  
	grayscaleDilate(&tmp, &output, sea2, roi);
	
	for ( int i= 0; i < input.n; ++i )
	  if ( input(i) && output(i) )
	    output(i)= input(i) > output(i) ? input(i) : output(i);
	  else
	    output(i)= 0;
    }
    
    
    
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleASDilate: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleASDilate(int radius);

        GrayscaleASDilate(const GrayscaleASDilate& g);

        ~GrayscaleASDilate();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleASDilate<INPUT, OUTPUT>::GrayscaleASDilate(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleASDilate " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleASDilate<INPUT, OUTPUT>::GrayscaleASDilate(const GrayscaleASDilate& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleASDilate<INPUT, OUTPUT>::~GrayscaleASDilate()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleASDilate<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleASDilate<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
      printf("as dilate\n");
        StructuringElementAS sea(radius, input.columns, input.sliceSize);
	grayscaleDilate(&input, &output, sea, roi);
    }
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleASErode: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleASErode(int radius);

        GrayscaleASErode(const GrayscaleASErode& g);

        ~GrayscaleASErode();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleASErode<INPUT, OUTPUT>::GrayscaleASErode(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleASErode " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleASErode<INPUT, OUTPUT>::GrayscaleASErode(const GrayscaleASErode& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleASErode<INPUT, OUTPUT>::~GrayscaleASErode()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleASErode<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleASErode<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        StructuringElementAS sea(radius, input.columns, input.sliceSize);
	grayscaleErode(&input, &output, sea, roi);
    }
    
    
    
    
    
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleACDilate: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleACDilate(int radius);

        GrayscaleACDilate(const GrayscaleACDilate& g);

        ~GrayscaleACDilate();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleACDilate<INPUT, OUTPUT>::GrayscaleACDilate(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleACDilate " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleACDilate<INPUT, OUTPUT>::GrayscaleACDilate(const GrayscaleACDilate& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleACDilate<INPUT, OUTPUT>::~GrayscaleACDilate()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleACDilate<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleACDilate<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        StructuringElementAC sea(radius, input.columns, input.sliceSize);
	grayscaleDilate(&input, &output, sea, roi);
    }
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleACErode: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleACErode(int radius);

        GrayscaleACErode(const GrayscaleACErode& g);

        ~GrayscaleACErode();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleACErode<INPUT, OUTPUT>::GrayscaleACErode(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleACErode " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleACErode<INPUT, OUTPUT>::GrayscaleACErode(const GrayscaleACErode& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleACErode<INPUT, OUTPUT>::~GrayscaleACErode()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleACErode<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleACErode<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        StructuringElementAC sea(radius, input.columns, input.sliceSize);
	grayscaleErode(&input, &output, sea, roi);
    }
    
    
    
    
    
    
    
    
    
    
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleCSOpen: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleCSOpen(int radius);

        GrayscaleCSOpen(const GrayscaleCSOpen& g);

        ~GrayscaleCSOpen();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSOpen<INPUT, OUTPUT>::GrayscaleCSOpen(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleCSOpen " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSOpen<INPUT, OUTPUT>::GrayscaleCSOpen(const GrayscaleCSOpen& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSOpen<INPUT, OUTPUT>::~GrayscaleCSOpen()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleCSOpen<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleCSOpen<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
      Volume<INPUT> tmp;
      tmp.resizeVolume(input);
      tmp= 0;
        StructuringElementCS sea(radius, input.columns, input.sliceSize);
	grayscaleErode(&input, &tmp, sea, roi);
	grayscaleDilate(&tmp, &output, sea, roi);
    }
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleCSClose: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleCSClose(int radius);

        GrayscaleCSClose(const GrayscaleCSClose& g);

        ~GrayscaleCSClose();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSClose<INPUT, OUTPUT>::GrayscaleCSClose(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleCSClose " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSClose<INPUT, OUTPUT>::GrayscaleCSClose(const GrayscaleCSClose& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleCSClose<INPUT, OUTPUT>::~GrayscaleCSClose()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleCSClose<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleCSClose<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        Volume<INPUT> tmp;
      tmp.resizeVolume(input);
      tmp= 0;
        StructuringElementCS sea(radius, input.columns, input.sliceSize);
	grayscaleDilate(&input, &tmp, sea, roi);
	grayscaleErode(&tmp, &output, sea, roi);
    }
    
    
    
    
    
    
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleASOpen: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleASOpen(int radius);

        GrayscaleASOpen(const GrayscaleASOpen& g);

        ~GrayscaleASOpen();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleASOpen<INPUT, OUTPUT>::GrayscaleASOpen(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleASOpen " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleASOpen<INPUT, OUTPUT>::GrayscaleASOpen(const GrayscaleASOpen& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleASOpen<INPUT, OUTPUT>::~GrayscaleASOpen()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleASOpen<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleASOpen<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        Volume<INPUT> tmp;
      tmp.resizeVolume(input);
      tmp= 0;
        StructuringElementAS sea(radius, input.columns, input.sliceSize);
	grayscaleErode(&input, &tmp, sea, roi);
	grayscaleDilate(&tmp, &output, sea, roi);
    }
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleASClose: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleASClose(int radius);

        GrayscaleASClose(const GrayscaleASClose& g);

        ~GrayscaleASClose();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleASClose<INPUT, OUTPUT>::GrayscaleASClose(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleASClose " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleASClose<INPUT, OUTPUT>::GrayscaleASClose(const GrayscaleASClose& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleASClose<INPUT, OUTPUT>::~GrayscaleASClose()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleASClose<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleASClose<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        Volume<INPUT> tmp;
      tmp.resizeVolume(input);
      tmp= 0;
        StructuringElementAS sea(radius, input.columns, input.sliceSize);
	grayscaleDilate(&input, &tmp, sea, roi);
	grayscaleErode(&tmp, &output, sea, roi);
    }
    
    
    
    
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleACOpen: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleACOpen(int radius);

        GrayscaleACOpen(const GrayscaleACOpen& g);

        ~GrayscaleACOpen();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleACOpen<INPUT, OUTPUT>::GrayscaleACOpen(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleACOpen " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleACOpen<INPUT, OUTPUT>::GrayscaleACOpen(const GrayscaleACOpen& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleACOpen<INPUT, OUTPUT>::~GrayscaleACOpen()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleACOpen<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleACOpen<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        Volume<INPUT> tmp;
      tmp.resizeVolume(input);
      tmp= 0;
        StructuringElementAC sea(radius, input.columns, input.sliceSize);
	grayscaleErode(&input, &tmp, sea, roi);
	grayscaleDilate(&tmp, &output, sea, roi);
    }
    
    template<typename INPUT, typename OUTPUT>
    class GrayscaleACClose: public Transform3<INPUT, OUTPUT>
    {
    public:
        using Transform3<INPUT, OUTPUT>::descriptor;

        GrayscaleACClose(int radius);

        GrayscaleACClose(const GrayscaleACClose& g);

        ~GrayscaleACClose();

        virtual Border3 getProposedBorder();

        virtual void apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi= NULL, Volume<unsigned char>* support= NULL);

        int radius;
    };

    template<typename INPUT, typename OUTPUT>
    GrayscaleACClose<INPUT, OUTPUT>::GrayscaleACClose(int radius)
    : Transform3<INPUT, OUTPUT>()
    {
        this->radius= radius;
        std::stringstream ss;
        ss << "GrayscaleACClose " << radius;
        this->descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleACClose<INPUT, OUTPUT>::GrayscaleACClose(const GrayscaleACClose& g)
    : Transform3<INPUT, OUTPUT>(g)
    {
        this->radius= g.radius;
    }

    template<typename INPUT, typename OUTPUT>
    GrayscaleACClose<INPUT, OUTPUT>::~GrayscaleACClose()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border3 GrayscaleACClose<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border3(radius, radius, radius, radius, radius, radius);
    }

    template<typename INPUT, typename OUTPUT>
    void GrayscaleACClose<INPUT, OUTPUT>::apply(Volume<INPUT>& input, Volume<OUTPUT>& output, Volume<unsigned char>* roi, Volume<unsigned char>* /*support*/)
    {
        Volume<INPUT> tmp;
      tmp.resizeVolume(input);
      tmp= 0;
        StructuringElementAC sea(radius, input.columns, input.sliceSize);
	grayscaleDilate(&input, &tmp, sea, roi);
	grayscaleErode(&tmp, &output, sea, roi);
    }
}

#endif // MORPHOLOGY3_H

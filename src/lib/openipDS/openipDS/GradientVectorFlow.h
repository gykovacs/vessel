/* 
 * File:   GradientVectorFlow.h
 * Author: gykovacs
 *
 * Created on 2011. január 5., 19:22
 */

#ifndef GRADIENTVECTORFLOW_H
#define	GRADIENTVECTORFLOW_H

#include <openipLL/imageIO.h>
#include <openipDS/Filter2s.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class GradientVectorFlowU: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;

        GradientVectorFlowU(float sigma= 0.2);

        GradientVectorFlowU(const GradientVectorFlowU& g);

        ~GradientVectorFlowU();

        virtual Border2 getProposedBorder();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        float sigma;
    };

    template<typename INPUT, typename OUTPUT>
    GradientVectorFlowU<INPUT, OUTPUT>::GradientVectorFlowU(float sigma)
        : Transform2<INPUT, OUTPUT>()
    {
        this->sigma= sigma;
    }

    template<typename INPUT, typename OUTPUT>
    GradientVectorFlowU<INPUT, OUTPUT>::GradientVectorFlowU(const GradientVectorFlowU& g)
        : Transform2<INPUT, OUTPUT>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    GradientVectorFlowU<INPUT, OUTPUT>::~GradientVectorFlowU()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GradientVectorFlowU<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(1, 1, 1, 1, BORDER_MODE_MIRRORED);
    }

    template<typename INPUT, typename OUTPUT>
    void GradientVectorFlowU<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& u, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        float mu= sigma;
        //float dx, dy/*, dt*/;
        //float r;
        //dx= 1;
        //dy= 1;
        //dt= dx*dy/(4*mu);
        //r= mu*dt/(dx*dy);

        Image<float> fx;
        Image<float> fy;
        Image<float> b;
        Image<float> c1;
        Image<float> c2;
        Image<float> utmp;
        Image<float> l;
        Image<float> f;
        fx.resizeImage(input);
        fy.resizeImage(input);
        b.resizeImage(input);
        c1.resizeImage(input);
        c2.resizeImage(input);
        utmp.resizeImage(input);
        l.resizeImage(input);
        f.resizeImage(input);
        fx= 0;
        fy= 0;
        f= 0;

        SobelFilterX<float, float> sfx(input.columns);
        SobelFilterY<float, float> sfy(input.columns);
        SobelFilter<INPUT, float> sf(input.columns);
        LaplacianFilter<float, float> lf(input.columns);

        sf.applyMagnitude(input, f, roi, support);
        f.normalize(0, 1);

        sfx.apply(f, fx, roi, support);
        sfy.apply(f, fy, roi, support);
        fx/= 8;
        fy/= 8;

        for ( unsigned int i= 0; i < b.n; ++i )
        {
            b(i)= fx(i)*fx(i) + fy(i)*fy(i);
            c1(i)= b(i)*fx(i);
            c2(i)= b(i)*fy(i);
        }

        fx.borderMode= BORDER_MODE_MIRRORED;
        fx.fillBorder();
        fy.borderMode= BORDER_MODE_MIRRORED;
        fy.fillBorder();
        f.borderMode= BORDER_MODE_MIRRORED;
        f.fillBorder();
        b.borderMode= BORDER_MODE_MIRRORED;
        b.fillBorder();
        c1.borderMode= BORDER_MODE_MIRRORED;
        c1.fillBorder();
        c2.borderMode= BORDER_MODE_MIRRORED;
        c2.fillBorder();
        u.borderMode= BORDER_MODE_MIRRORED;
        u.fillBorder();
        utmp.borderMode= BORDER_MODE_MIRRORED;
        l.borderMode= BORDER_MODE_MIRRORED;

        u= fx;
        float diff;
        float oldDiff= FLT_MAX;
        for ( int i= 0; i < 10000; ++i )
        {
            lf.apply(u, l, roi, support);

            for ( unsigned int i= u.columns+1; i < utmp.n - u.columns-1; ++i )
                if ( !roi || (*roi)(i) )
                    utmp(i)= (1 - b(i))*u(i) + mu*l(i) + c1(i);
            utmp.fillBorder();
            diff= 0;
            for ( unsigned int i= 0; i < utmp.n; ++i )
                diff+= fabs(utmp(i) - u(i));

            u= utmp;
            if ( fabs(diff - oldDiff) < 0.1 )
                break;
            oldDiff= diff;
        }

    }

    template<typename INPUT, typename OUTPUT>
    class GradientVectorFlowV: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;

        GradientVectorFlowV(float sigma= 0.2);

        GradientVectorFlowV(const GradientVectorFlowV& g);

        ~GradientVectorFlowV();

        virtual Border2 getProposedBorder();

        void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        float sigma;
    };

    template<typename INPUT, typename OUTPUT>
    GradientVectorFlowV<INPUT, OUTPUT>::GradientVectorFlowV(float sigma)
        : Transform2<INPUT, OUTPUT>()
    {
        this->sigma= sigma;
    }

    template<typename INPUT, typename OUTPUT>
    GradientVectorFlowV<INPUT, OUTPUT>::GradientVectorFlowV(const GradientVectorFlowV& g)
        : Transform2<INPUT, OUTPUT>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    GradientVectorFlowV<INPUT, OUTPUT>::~GradientVectorFlowV()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GradientVectorFlowV<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(1, 1, 1, 1, BORDER_MODE_MIRRORED);
    }

    template<typename INPUT, typename OUTPUT>
    void GradientVectorFlowV<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& u, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        float mu= sigma;
        //float dx, dy/*, dt*/;
        //float r;
        //dx= 1;
        //dy= 1;
        //dt= dx*dy/(4*mu);
        //r= mu*dt/(dx*dy);

        Image<float> fx;
        Image<float> fy;
        Image<float> b;
        Image<float> c1;
        Image<float> c2;
        Image<float> utmp;
        Image<float> l;
        Image<float> f;
        fx.resizeImage(input);
        fy.resizeImage(input);
        b.resizeImage(input);
        c1.resizeImage(input);
        c2.resizeImage(input);
        utmp.resizeImage(input);
        l.resizeImage(input);
        f.resizeImage(input);
        fx= 0;
        fy= 0;
        f= 0;

        SobelFilterX<float, float> sfx(input.columns);
        SobelFilterY<float, float> sfy(input.columns);
        SobelFilter<INPUT, float> sf(input.columns);
        LaplacianFilter<float, float> lf(input.columns);

        sf.applyMagnitude(input, f, roi, support);
        f.normalize(0, 1);

        sfx.apply(f, fx, roi, support);
        sfy.apply(f, fy, roi, support);
        fx/= 8;
        fy/= 8;

        for ( unsigned int i= 0; i < b.n; ++i )
        {
            b(i)= fx(i)*fx(i) + fy(i)*fy(i);
            c1(i)= b(i)*fx(i);
            c2(i)= b(i)*fy(i);
        }

        fx.borderMode= BORDER_MODE_MIRRORED;
        fx.fillBorder();
        fy.borderMode= BORDER_MODE_MIRRORED;
        fy.fillBorder();
        f.borderMode= BORDER_MODE_MIRRORED;
        f.fillBorder();
        b.borderMode= BORDER_MODE_MIRRORED;
        b.fillBorder();
        c1.borderMode= BORDER_MODE_MIRRORED;
        c1.fillBorder();
        c2.borderMode= BORDER_MODE_MIRRORED;
        c2.fillBorder();
        u.borderMode= BORDER_MODE_MIRRORED;
        u.fillBorder();
        utmp.borderMode= BORDER_MODE_MIRRORED;
        l.borderMode= BORDER_MODE_MIRRORED;

        u= fy;
        float diff;
        float oldDiff= FLT_MAX;
        for ( int i= 0; i < 10000; ++i )
        {
            lf.apply(u, l, roi, support);

            for ( unsigned int i= u.columns+1; i < utmp.n - u.columns-1; ++i )
                if ( !roi || (*roi)(i) )
                    utmp(i)= (1 - b(i))*u(i) + mu*l(i) + c2(i);
            utmp.fillBorder();
            diff= 0;
            for ( unsigned int i= 0; i < utmp.n; ++i )
                diff+= fabs(utmp(i) - u(i));

            u= utmp;
            if ( fabs(diff - oldDiff) < 0.1 )
                break;
            oldDiff= diff;
        }

    }

    template<typename INPUT, typename OUTPUT>
    class GradientVectorFlow: public Transform2<INPUT, OUTPUT>
    {
    public:
        using Transform2<INPUT, OUTPUT>::apply;

        GradientVectorFlow(float sigma= 0.2);

        GradientVectorFlow(const GradientVectorFlow& g);

        ~GradientVectorFlow();

        virtual Border2 getProposedBorder();

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& u, Image<OUTPUT>& v, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        virtual void apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi= NULL, Image<unsigned char>* support= NULL);

        float sigma;
    };

    template<typename INPUT, typename OUTPUT>
    GradientVectorFlow<INPUT, OUTPUT>::GradientVectorFlow(float sigma)
        : Transform2<INPUT, OUTPUT>()
    {
        this->sigma= sigma;
    }

    template<typename INPUT, typename OUTPUT>
    GradientVectorFlow<INPUT, OUTPUT>::GradientVectorFlow(const GradientVectorFlow& g)
        : Transform2<INPUT, OUTPUT>(g)
    {
    }

    template<typename INPUT, typename OUTPUT>
    GradientVectorFlow<INPUT, OUTPUT>::~GradientVectorFlow()
    {
    }

    template<typename INPUT, typename OUTPUT>
    Border2 GradientVectorFlow<INPUT, OUTPUT>::getProposedBorder()
    {
        return Border2(1, 1, 1, 1, BORDER_MODE_MIRRORED);
    }

    template<typename INPUT, typename OUTPUT>
    void GradientVectorFlow<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& u, Image<OUTPUT>& v, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        GradientVectorFlowU<INPUT, OUTPUT> gvfu(sigma);
        GradientVectorFlowV<INPUT, OUTPUT> gvfv(sigma);

        gvfu.apply(input, u, roi, support);
        gvfv.apply(input, v, roi, support);
    }

    template<typename INPUT, typename OUTPUT>
    void GradientVectorFlow<INPUT, OUTPUT>::apply(Image<INPUT>& input, Image<OUTPUT>& output, Image<unsigned char>* roi, Image<unsigned char>* support)
    {
        GradientVectorFlowU<INPUT, OUTPUT> gvfu(sigma);
        GradientVectorFlowV<INPUT, OUTPUT> gvfv(sigma);

        Image<float> u;
        Image<float> v;
        u.resizeImage(input);
        v.resizeImage(input);

        tprintf("GVF started\n");
        gvfu.apply(input, u, roi, support);
        gvfv.apply(input, v, roi, support);
        tprintf("GVF finished\n");

        for ( unsigned int i= 0; i < input.n; ++i )
            output(i)= atan2(u(i), v(i));
    }
}

#endif	/* GRADIENTVECTORFLOW_H */

